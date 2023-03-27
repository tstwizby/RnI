//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Contains information directly relevant to writing/reading script files.
//
//Defines and names enums:
//	CONTROL		ctrl, control characters in scripts
//	PARAM_TYPE	type, types of arguments for functions in scripts
//	FUNCTION	func, functions in scripts
//	S_MACRO		mac, macros in scripts
//	
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#define SCRIPT_MAX_LENGTH 4096 //65535
#define VAR_MAX_LENGTH 64
#define MAX_MENU_OPTIONS 8



//Control characters
typedef enum {ctrl_end, ctrl_space, ctrl_line, ctrl_func, ctrl_menu, ctrl_clear, ctrl_wait, ctrl_delim, ctrl_var, ctrl_max=32} CONTROL;

const char* ctrlNames[] = {
	"end","space","line","func","menu","clear","wait","delim","var"
};


uint8_t DEFAULT_TRANSITION_COLOR = clr_black;
uint8_t DEFAULT_TRANSITION_TIME = 10;

//Generates scripts for fast-travel
int travelScript(int loc, int dist, uint8_t* dest){
	dest[0]  = ctrl_clear;
	dest[1]  = ctrl_func;
	dest[2]  = f_playSE;
	dest[3]  = sef_steps;
	dest[4]  = 100;
	dest[5]  = 0;
	dest[6]  = ctrl_func;
	dest[7]  = f_fadeout;
	dest[8]  = DEFAULT_TRANSITION_COLOR;
	dest[9]  = DEFAULT_TRANSITION_TIME;
	dest[10] = ctrl_func;
	dest[11] = f_addtime;
	dest[12] = 0;
	dest[13] = (uint8_t)dist;
	dest[14] = ctrl_func;
	dest[15]  = f_transition;
	dest[16]  = loc;
	dest[17]  = ctrl_func;
	dest[18]  = f_fadein;
	dest[19] = DEFAULT_TRANSITION_TIME;
	dest[20] = ctrl_clear;
	dest[21] = locations[loc].script?ctrl_func:ctrl_end;
	dest[22] = f_goto;
	dest[23] = locations[loc].script;
	dest[24] = ctrl_end;
	return 24;
}



//converts character data into the appropriate format for scripts at the moment
//assumes initial character is in ascii and in the 32-127 range
//0- space
//1- newline
//2- runs a function
//3- starts a new menu item
//4- clears text box
//5- waits for player input
uint8_t convert(uint8_t a){
	if(a==32){return ctrl_space;}
	if(a==10){return ctrl_line;}
	return a+1;
	/*
	if(a==95){return 107;}  // _ (rendered as ...)
	if(64<a&&a<123){return a-32;}
	//ugh... put the 0 in the wrong place...
	if(48<a&&a<58){return a+48;}
	switch(a){
		case 44: return 59; // ,
		case 46: return 60; // .
		case 63: return 61; // ?
		case 33: return 62; // !
		case 39: return 63; // '
		case 34: return 64; // "
		case 40: return 91; // (
		case 41: return 92; // )
		case 45: return 93; // -
		case 58: return 94; // :
		case 42: return 95; // * (rendered as a dot)
		case 47: return 96; // /f
		case 48: return 106; // 0
		case 32: return ctrl_space; // space
		case 10: return ctrl_line; // newline
		default: return ctrl_space;
	}*/
}

int copyrightScript(uint8_t* dest){
	dest[0]  = ctrl_clear;
	char cpy[] = {'C','o','p','y','r','i','g','h','t',' ','P','e','t','e','r',' ','M','a','r','c','h','e','t','t','i',',',' ','2','0','2','3','.'};
	for(int i=1;i<32;i++){
		dest[i]=convert((uint8_t)(cpy[i-1]));
	}
	dest[32]=ctrl_end;
	return 32;
}

char unconvert(uint8_t a){
	if(32<a){return a-1;}
	switch(a){
		case ctrl_space: return 32;
		case ctrl_line: return 32;
		default: return '*';
	}
}
int conv_print(uint8_t* data, int max){
	int i = 0;
	printf("'");
	while((i<max||max==0)&&data[i]/*&&(data[i]>ctrl_max||data[i]==ctrl_space||data[i]==ctrl_line)*/){
		printf("%c",unconvert(data[i]));
		i++;
	}
	printf("'\n");
}

int conv_script(uint8_t* data, int max){
	int i = 0;
	printf("scr:'");
	while(i<max||(max==0&&data[i])){
		if(data[i]<ctrl_max&&data[i]!=ctrl_space){
			printf("[%s]",ctrlNames[data[i]]);
			if(data[i]==ctrl_func){
				int f = data[++i];
				printf("[%s(",func_names[f]);
				for(int j=0;j<func_arg_count[f];j++){
					i++;
					int t = func_arg_types[f][j];
					printf("%d/",data[i]);
					if(t!=type_num){printf("%s ", all_names[t][data[i]]);}
					else{printf("%d ",data[i]);}
				}
				printf(")]");
			}else if(data[i]==ctrl_menu){
				printf("(%d)",data[++i]);
			}else if(data[i]==ctrl_var){
				printf("(%s)",var_names[data[++i]]);
			}
		}else{printf("%c",unconvert(data[i]));}
		i++;
	}
	printf("'\n");
}

typedef struct Script{
	int len;
	char* name;
	uint8_t* data;
} Script;

Script scripts[MAX_SCR_COUNT];
uint8_t* scr_buffers[MAX_SCR_COUNT];

int initScript(int i){
	if(i<0||i>MAX_SCR_COUNT){ERROR(1,"Script index out of bounds");}
	scripts[i].len = 0;
	scripts[i].name = scr_names[i];
	scripts[i].data = scr_buffers[i];
	return 0;
}

int setScript(int i, int l, uint8_t* data){
	if(0>i||i>MAX_SCR_COUNT){ERROR(1,"Script index out of bounds");}
	if(0>l||l>SCRIPT_MAX_LENGTH){ERROR(1,"Script too long");}
	if(data==NULL){ERROR(1,"Missing script data");}
	scr_buffers[i] = malloc(l*sizeof(*scr_buffers));
	if(scr_buffers[i]==NULL){ERROR(1,"Could not allocate memory for script data");}
	memcpy(scr_buffers[i],data,l);
	scripts[i].len=l;
	return 0;
}

int scriptCleanup(){
	for(int i=0;i<scr_count;i++){
		free(scr_buffers[i]);
	}
}

//Loads and allocates script data.
int loadScripts(){
	DEBUG("Loading scripts...");
	scr_buffers[0] = malloc(1);
	scr_buffers[0][0] = ctrl_end;
	scripts[0].len = 1;
	FILE* source = fopen(script_bin,"rb");
	if(source == NULL){ERROR(1,"Could not open script source file '%s'.",script_bin);}
	for(int sc=1;sc<scr_count;sc++){
		DEBUG("Loading script %s...",scr_names[sc]);
		int len = readInt(source);
		if(len<0){fclose(source);scriptCleanup();ERROR(2,"Could not read script length.");}
		scr_buffers[sc] = malloc(len+1);
		if(scr_buffers[sc]==NULL){scriptCleanup();ERROR(3,"Problem allocating memory for script.");}
		if(fread(scr_buffers[sc],1,len,source)<len){fclose(source);scriptCleanup();ERROR(2,"Could not read script data.");}
		scripts[sc].len=len+1;
		scr_buffers[sc][len]=ctrl_end;
		conv_script(scr_buffers[sc],len);
	}
	DEBUG("Scripts loaded.");
	return 0;
}

