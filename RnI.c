#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <SDL.h>
#include <SDL_image.h>



//0 - no logging
//1 - logging
//2 - log to console
#define DEBUG_VERSION 2

//defines important tools and structs
#include "error.c"
#include "buffer.c"
#include "stringparse.c"

//standalone location of various files
#include "paths.c"

#define FONT_COUNT 1

//important typedefs
typedef struct Menu{
	int size;
	int selected;
	int* actions;
	uint8_t** options;
} Menu;

//Ready -	waiting for the player to do something (e.g. open map, examine, etc)
//Running - ready to execute script/write text
//Waiting - waiting for player input to proceed
//Menu -	a menu is currently displayed
//Closed -	the game has been closed
//Paused -	input/updates are paused while something happens (e.g. fadein/out)
//Map -		a map is currently displayed
enum GameStatus {READY, RUNNING, WAITING, MENU, CLOSED, PAUSED, MAP};

enum GameStatus status = RUNNING;
int timer = 0;
int delay = 0;
int scene = 0;

#include "resolutions.c"
//defines/enumerates game data and data loading
#include "game.c"
//defines location struct, contains related functions
#include "loc.c"
//defines script struct, contains related functions
#include "script.c"
#include "Fonts.c"
#include "sound.c"
#include "SDLWrapper.c"
#include "textbox.c"
#include "text.c"

SDL_Keycode key_select = SDLK_SPACE;
SDL_Keycode key_map = SDLK_c;
SDL_Keycode key_look = SDLK_x;
SDL_Keycode key_act = SDLK_z;
SDL_Keycode key_cancel = SDLK_ESCAPE;
SDL_Keycode key_quit = SDLK_q;

SDL_Keycode key_up = SDLK_UP;
SDL_Keycode key_down = SDLK_DOWN;
SDL_Keycode key_left = SDLK_LEFT;
SDL_Keycode key_right = SDLK_RIGHT;

int SDL_Loop(){
	while(status!=CLOSED){
		//Automatic behavior, that happens independently of the player
		
		//If a script/textbox is running and nothing else is blocking it, advance the script/textbox
		if(status==RUNNING){
			if(countBuffer(txtbox.buffer)){
				tickText(&txtbox);
			}else if(countBuffer(&scrBuffer)){
				scrNext();
			}else{status = READY;}
		}else if(status==PAUSED){
			//If the screen is currently fading in/out
			if(fadeTarget){
				fade();
			}
		}else if(status==WAITING&&delay>0){
			if(--delay==0){status = RUNNING;}
		}
		//Player-driven behavior
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			int dest = 0;
			SDL_Keycode key;
			switch(e.type){
				case SDL_QUIT:
					status = CLOSED;
					break;
				case SDL_KEYDOWN:
					key = e.key.keysym.sym;
					if(key==SDLK_BACKQUOTE&&status==READY){
						loadCopyright();
					}
					switch(status){
						case RUNNING:
							if(countBuffer(txtbox.buffer)){
								writeAllText(&txtbox);
								renderText(&txtbox);
							}
							break;
						case WAITING:
							if(delay==0){status = RUNNING;}
							break;
						case MENU:
							if(key==key_down){
								if(++menuSelect==menuSize){menuSelect=0;}
							}else if(key==key_up){
								if(menuSelect--==0){menuSelect=menuSize-1;}
							}else if(key==key_select){
								selectMenu();
							}else if(key==key_cancel){
								//break out of menu if allowed
							}
							renderMenu();
							break;
						case MAP:
							if(key==key_up){
								
							}else if(key==key_down){
								
							}else if(key==key_select){
								
							}else if(key==key_cancel){
								
							}
							break;
						case READY:
							if(key==key_map){
								
							}else if(key==key_look){
								clearTextbox(&txtbox);
								loadScript(cLocation.examine);
							}else if(key==key_act){
								clearTextbox(&txtbox);
								loadScript(cLocation.action);
							}else if(key==key_left){
								if(cLocation.l_dist>=0){loadFastTravel(cLocation.left,cLocation.l_dist);}
								else{cLocation.l_arr = cLocation.left?2:0; clearTextbox(&txtbox); loadScript(cLocation.left);}
							}else if(key==key_right){
								if(cLocation.r_dist>=0){loadFastTravel(cLocation.right,cLocation.r_dist);}
								else{cLocation.r_arr = cLocation.right?2:0; clearTextbox(&txtbox); loadScript(cLocation.right);}
							}else if(key==key_up){
								if(cLocation.u_dist>=0){loadFastTravel(cLocation.up,cLocation.u_dist);}
								else{cLocation.u_arr = cLocation.up?2:0; clearTextbox(&txtbox); loadScript(cLocation.up);}
							}else if(key==key_down){
								if(cLocation.d_dist>=0){loadFastTravel(cLocation.down,cLocation.d_dist);}
								else{cLocation.d_arr = cLocation.down?2:0; clearTextbox(&txtbox); loadScript(cLocation.down);}
							}else if(key==key_cancel){
								status = MENU;
								DEBUG("Opened settings menu");
								loadScript(2);
							}else if(key==key_quit){
								status = CLOSED;
							}
					}
					break;
			}
		}
		renderAll();
		SDL_RenderPresent(renderer);
	}
	return 0;
}


int main(int argc, char* args[]){
	printf("Hi\n");
	//setup
	DEBUG("Loading data...");
	printf("Hi\n");
	int quitStatus = 0;
	if(initGameValues()){return 1;}
	if(SDLSetup()){return 1;}
	if(quitStatus=loadFont(0, 8, "testfont")){goto font_err;}
	if(quitStatus=loadMedia()){goto media_err;}
	if(quitStatus=loadScripts()){goto script_err;}
	if(quitStatus=loadLocs()){goto loc_err;}
	//diagnoseLocs();
	setupBuffers();
DEBUG("Loading startup script");
	loadScript(1);
DEBUG("Beginning main loop...");
	quitStatus = SDL_Loop();
DEBUG("quitting %d",quitStatus);
	loc_err:
	scriptCleanup();
	script_err:
	media_err:
	fontCleanup();
	font_err:
	audioCleanup();
	SDL_Cleanup();
	return quitStatus;
}