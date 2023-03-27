
const char* win_title = "Rules and Instructions";

//useful macros
#define CLEANUP(x)   if(x!=NULL){SDL_DestroyTexture(x);}
#define GET_COLOR(c) colormap[c][0],colormap[c][1],colormap[c][2],colormap[c][3]
#define SET_COLOR(c) SDL_SetRenderDrawColor(renderer,colormap[c][0],colormap[c][1],colormap[c][2],colormap[c][3])
#define CLEAR_RENDER(target,c) SDL_SetRenderTarget(renderer,target); SET_COLOR(c); SDL_RenderClear(renderer); SDL_SetRenderTarget(renderer,NULL)

//SDL global variables
SDL_Window* window;
SDL_Surface* surface;
SDL_Renderer* renderer;


//SDL texture variables
SDL_Texture* fontTxt[FONT_COUNT];

SDL_Texture* bgTxt;
SDL_Texture* shadeTxt;
SDL_Texture* skyTxt;

SDL_Texture* bgFullTxt;
SDL_Texture* uiTxt;
SDL_Texture* txtBoxTxt;
SDL_Texture* menuTxt;
SDL_Texture* scnTxt;
SDL_Texture* arrowsTxt;

SDL_Texture* mapTxt;

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Setup and loading functions
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//Sets up SDL and associated systems
int SDLSetup(){
	//Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)<0){ERROR(1,"Failed to initialize SDL: %s",SDL_GetError());}
	//Create window/renderer
	   window    =SDL_CreateWindow(win_title,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,res.screen_width,res.screen_height,0);//SDL_WINDOW_FULLSCREEN);
	if(window   ==NULL){ERROR(2,"Failed to create window (%s)",SDL_GetError());}
	   surface   =SDL_GetWindowSurface(window);
	   renderer  =SDL_CreateRenderer(window,-1,SDL_RENDERER_PRESENTVSYNC|SDL_RENDERER_ACCELERATED);
	if(renderer ==NULL){ERROR(3,"Failed to create renderer (%s)",SDL_GetError());}
	//Create textures
	   bgFullTxt =SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,bg_raw_width,bg_raw_height);
	if(bgFullTxt==NULL){ERROR(4,"Failed to create background texture (%s)",SDL_GetError());}
	   txtBoxTxt =SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,res.txt_width,res.txt_height);
	if(txtBoxTxt==NULL){ERROR(4,"Failed to create textbox texture (%s)",SDL_GetError());}
	   menuTxt   =SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,res.menu_width,res.screen_height);
	if(menuTxt  ==NULL){ERROR(4,"Failed to create menu texture (%s)",SDL_GetError());}
	   scnTxt    =SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,bg_raw_width,bg_raw_height);
	if(scnTxt   ==NULL){ERROR(4,"Failed to create scene texture (%s)",SDL_GetError());}
	   arrowsTxt =SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,arrow_width,2*arrow_height);
	if(arrowsTxt==NULL){ERROR(4,"Failed to create arrow source texture (%s)",SDL_GetError());}
	//Audio setup
	if(audioSetup()){return 5;}
	//Set defaults, initialize window
	SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(renderer,NULL);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderPresent(renderer);
	int imgFlags = IMG_INIT_PNG;
	if(!(IMG_Init(imgFlags)&imgFlags)){ERROR(5,"Failed to initialize SDLImage: %s",IMG_GetError());}
	return 0;
}

//Changes the resolution and rebuilds the window
int setResolution(int r){
	if(r==res.resolution){return 0;}
	if(r<0||r>=res_count){ERROR(1,"Unrecognized resolution.");}
	DEBUG("Changing resolution to %s...",res_names[r]);
	res = resolutions[r];
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window   = SDL_CreateWindow(win_title,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,res.screen_width,res.screen_height,SDL_WINDOW_SHOWN);
	if(window  ==NULL){ERROR(1,"Failed to re-create window (%s)",SDL_GetError());}
	renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_PRESENTVSYNC|SDL_RENDERER_ACCELERATED);
	if(renderer==NULL){ERROR(1,"Failed to re-create renderer (%s)",SDL_GetError());}
}

//Loads a texture from an image
int loadTexture(SDL_Texture** txt, const char* path){
	SDL_Surface* lSrf = IMG_Load(path);
	if(lSrf==NULL){ERROR(1,"Failed to load texture '%s' (%s)",path,SDL_GetError());}
	*txt = SDL_CreateTextureFromSurface(renderer,lSrf);
	SDL_FreeSurface(lSrf);
	if(*txt==NULL){ERROR(2,"Failed to convert texture '%s' (%s)",path,SDL_GetError());}
	return 0;
}

//Loads the textures used for the UI
int loadUI(){
	//tbd- textures for text boxes, menus, etc.
	if(loadTexture(&mapTxt, "IMG\\zoomap.png")){return 1;}
	if(loadTexture(&arrowsTxt, "IMG\\arrows.png")){return 1;}
	return loadTexture(&uiTxt, "IMG\\UI.png");
}

//Loads a texture to be used as a font
int loadFontTexture(int f, char* path){
	return loadTexture(&fontTxt[f], path);
}

//Loads assorted textures and backgrounds
int loadMedia(){
	if(loadUI()>0){ERROR(1,"Could not load UI");}
	//if(loadBackgrounds()>0){ERROR(2,"Could not load backgrounds");}
	if(loadMusic()){return 1;}
	if(loadSE()){return 1;}
	return 0;
}



//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Generic texture drawing functions
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//Draws a texture to the window at the specified position with the specified height and width (possibly redundant with scaleTexture())
int drawTexture(SDL_Texture* txt, int x, int y, int w, int h){
	SDL_Rect dest = {x, y, w, h};
	SDL_RenderCopy(renderer, txt, NULL, &dest);
}

//Scales a drawn texture to the window at the specified position with the specified height and width.
int scaleTexture(SDL_Texture* txt,int x,int y,int w,int h){
	SDL_Rect dest = {x,y,w,h};
	SDL_RenderCopyEx(renderer,txt,NULL,&dest,0,NULL,SDL_FLIP_NONE);
}

//Draws a clipped portion of a texture to the window at the specified position with the specified height and width.
int drawClippedTexture(SDL_Texture* txt, int x, int y, int cx, int cy, int w, int h){
	SDL_Rect dest = { x, y,w,h};
	SDL_Rect mask = {cx,cy,w,h};
	SDL_RenderCopy(renderer, txt, &mask, &dest);
}
int drawRotatedTexture(SDL_Texture* txt, int x, int y, int cx, int cy, int w, int h, int r){
	SDL_Rect dest = { x, y,w,h};
	SDL_Rect mask = {cx,cy,w,h};
	SDL_RenderCopyEx(renderer,txt,&mask,&dest,(double)r,NULL,SDL_FLIP_NONE);
}

//Clears the given texture, setting it to the given color
int clearTxt(SDL_Texture* txt, COLOR c){
	SDL_SetRenderTarget(renderer,txt);
	SET_COLOR(c);
	SDL_RenderClear(renderer);
	SDL_SetRenderTarget(renderer,NULL);
}



//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Background and UI-related drawing/loading functions
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//Loads the images used for the background and constructs the texture for it.
int loadBackground(){
	int nl = 0;
	while(*(loc_names[location]+nl)){nl++;}
	char path[MAX_LOC_NAME_LEN+16] = {0};
	memcpy(path,bgStart,bgslen);
	memcpy(path+bgslen,loc_names[location],nl);
	memcpy(path+bgslen+nl,bgEnd,bgelen);
	DEBUG("Loading path for %s from %s",loc_names[location],path);
	if(loadTexture(&bgTxt,path)){ERROR(1,"Failed to load background image from path %s",path);}
	SDL_SetRenderTarget(renderer,bgFullTxt);
	const char* skypath = getSkyPath();
	if(skypath!=NULL){
	DEBUG("Getting sky from %s",skypath);
		if(loadTexture(&skyTxt,skypath)){SDL_SetRenderTarget(renderer,NULL);ERROR(2,"Failed to load sky image from path %s",path);}
		drawTexture(skyTxt,0,0,bg_raw_width,bg_raw_height);
	}else{DEBUG("No sky to draw");}
	drawTexture(bgTxt,0,0,bg_raw_width,bg_raw_height);
	if(cLocation.shade==shd_none||(hours<EVENING_START&&weather!=wth_storm)){
		DEBUG("No shade to draw");
		SDL_SetRenderTarget(renderer,NULL);
		return 0;
	}else if(cLocation.shade==shd_limited&&(weather==wth_storm||hours>=EVENING_START)){
		memcpy(path+bgslen+nl,bgShade,bgsllen+1);
	}else{
		if(weather==wth_storm||hours>=NIGHT_START){
			memcpy(path+bgslen+nl,bgShadeNgt,bgsflen+1);
		}else{
			memcpy(path+bgslen+nl,bgShadeEve,bgsflen+1);
		}
	}
	DEBUG("Getting shade from %s",path);
	if(loadTexture(&shadeTxt,path)){SDL_SetRenderTarget(renderer,NULL);ERROR(3,"Failed to load shade image from path %s",path);}
	drawTexture(shadeTxt,0,0,bg_raw_width,bg_raw_height);
	SDL_SetRenderTarget(renderer,NULL);
	return 0;
}

//Renders the UI. Can clear rendered text when called if the textbox has a background, but doesn't empty the textbox. Use with caution.
int renderUI(){
	drawTexture(uiTxt,0,0,res.screen_width,res.screen_height);
	drawClippedTexture(arrowsTxt,res.arrows_x+   up_arrow_x,res.arrows_y+   up_arrow_y,arrow_source_x,arrow_source_y[cLocation.u_arr],arrow_width,arrow_height);
	drawRotatedTexture(arrowsTxt,res.arrows_x+ down_arrow_x,res.arrows_y+ down_arrow_y,arrow_source_x,arrow_source_y[cLocation.d_arr],arrow_width,arrow_height,180);
	drawRotatedTexture(arrowsTxt,res.arrows_x+ left_arrow_x,res.arrows_y+ left_arrow_y,arrow_source_x,arrow_source_y[cLocation.l_arr],arrow_width,arrow_height,270);
	drawRotatedTexture(arrowsTxt,res.arrows_x+right_arrow_x,res.arrows_y+right_arrow_y,arrow_source_x,arrow_source_y[cLocation.r_arr],arrow_width,arrow_height,90);
}

//Renders the map.
int renderMap(){
	drawTexture(mapTxt,0,0,res.screen_width,res.screen_height);
}

int* fadeColor = 0;
int fadeCounter = 0;
int fadeTarget = 0;
int fadeMode = 0;
int fadeAlpha = 0;

//Fades out the screen
int fadeOut(int c, int time){
	fadeColor = (int*)(colormap+c);
	fadeCounter = 0;
	fadeTarget = time;
	fadeMode = 0;
	if(time<=1){
		fadeAlpha=0xFF;
		status = RUNNING;
	}
	return 0;
}
int fadeIn(int time){
	fadeCounter = time;
	fadeTarget = time;
	fadeMode = 1;
	return 0;
}

//Draws the fade-out layer, call after rendering everything else
int renderFade(){
	SDL_Rect dummy = {0,0,res.screen_width,res.screen_height};
	SDL_SetRenderDrawColor(renderer,fadeColor[0],fadeColor[1],fadeColor[2],fadeAlpha);
	SDL_RenderFillRect(renderer,&dummy);
}

int fade(){
	if(fadeMode==0&&fadeCounter++==fadeTarget){
		fadeAlpha = 0xFF;
		fadeTarget = 0;
		fadeCounter = 0;
		status = RUNNING;
	}else if(fadeMode==1&&--fadeCounter==0){
		fadeMode = 0;
		fadeTarget = 0;
		fadeAlpha = 0;
		status = RUNNING;
	}else{
		fadeAlpha = (fadeCounter*0xFF)/fadeTarget;
	}
	return 0;
}

//Renders a textbox to the screen
int renderText(){
	drawTexture(txtBoxTxt,res.txt_x,res.txt_y,res.txt_width,res.txt_height);
}

int renderMenu();

int renderScene(){
	drawTexture(scnTxt,res.bg_x,res.bg_y,res.bg_width,res.bg_height);
}

int loadScene(int s){
	if(s<0||s>=scn_count){ERROR(1,"Scene index out of bounds.");}
	if(s>0){
		DEBUG("Loading scene '%s' from '%s'...",scn_names[s],scn_paths[s]);
		if(loadTexture(&scnTxt,scn_paths[s])){ERROR(3,"Problem loading scene texture from '%s'.",scn_paths[s]);}
	}
	scene = s;
	return 0;
}

//Fully renders the screen
int renderAll(){
	//UI
	renderUI();
	//Background:
	scaleTexture(bgFullTxt,res.bg_x,res.bg_y,res.bg_width,res.bg_height);
	//Text Boxes
	renderText();
	if(status==MENU){renderMenu();}
	//Screen-covering fadeout or scene
	if(scene>0){renderScene();}
	if(fadeAlpha>0){renderFade();}
}

//Cleans up all SDL resources.
int SDL_Cleanup(){
	for(int i=0;i<FONT_COUNT;i++){
		CLEANUP(fontTxt[i]);
	}
	CLEANUP(bgTxt);
	CLEANUP(skyTxt);
	CLEANUP(shadeTxt);
	CLEANUP(bgFullTxt);
	CLEANUP(uiTxt);
	CLEANUP(scnTxt);
	CLEANUP(txtBoxTxt);
	CLEANUP(menuTxt);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}
