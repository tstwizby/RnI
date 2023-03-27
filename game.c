//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Contains game variables and data values, such as default values and file paths
//
//Defines global variables:
//	gameVars(location, weather, hours, minutes, TEXT_SPEED, SE_VOLUME, MUS_VOLUME)
//	music
//
//Defines notable functions:
//	getTimeOfDay()
//	addTime(hours, minutes)
//	setTime(hours, minutes)
//	getSkyPath()
//	initGameValues()
//  
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef enum {type_func, type_mac, type_clr, type_wth, type_dir, type_crd, type_sky, type_shd, type_lde, type_cfg, type_vop, type_chk, type_scr, type_loc, type_mus, type_sef, type_scn, type_var, type_num, type_count} PARAM_TYPE;
const char* script_param_types[] = {	
	"function",
	"macro",
	"color",
	"weather",
	"direction",
	"cardinal direction",
	"sky type",
	"shade type",
	"location data element",
	"config variable",
	"variable modifier",
	"variable check",
	"script",
	"location",
	"music",
	"sound",
	"scene",
	"game variable",
	"number",
};


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Functions
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef enum {	f_null, f_quit, f_stop, f_goto, f_run, f_runback, f_fadeout, f_fadein, f_transition, f_playBGM, f_playSE, f_silence, f_addtime, 
				f_setcfg, f_setvar, f_checkvar, f_switchvar, f_setlocscript, f_setlocdest, f_wait, f_setscene,
				func_count} FUNCTION;
char* func_names[] = {
	"null",			//no-op
	"quit",			//self explanatory
	"stop",			//exits the current script
	"goto",			//exits the current script and passes execution to another
	"run",			//passes execution to another script, and loops back to this one when it finishes
	"runback",		//passes execution to another script, returns to the beginning of this one when it finishes
	"fadeout",		//fadeout(color, speed)
	"fadein",		//same as fadeout
	"transition",	//changes the background
	"playBGM",		//changes the background music
	"playSE",		//plays a sound effect
	"silence",		//turns off the background music, either abruptly with an argument of 0 or fading out with some delay
	"addtime",		//adds an amount of time in hours/minutes
	"setcfg",		//sets a config variable to a given value
	"setvar",		//changes a game variable in the described way
	"checkvar",		//checks to see if the game matches a given game state, runs the given script if so, and continues if not
	"switchvar",	//if the game matches a given game state, exits current script and passes control to another, and continues if not
	"setlocscript", //overrides a location script for a given location
	"setlocdest",   //overrides a movement destination for a given location
	"wait",			//does nothing for a certain amount of time
	"setscene",		//shows a scene
};
const int func_arg_count[] = {
	0,0,0,1,1,1,2,1,1,3,3,1,2,2,3,4,4,4,4,1,1
};

const int* func_arg_types[] = {
	(int[]){},
	(int[]){},
	(int[]){},
	(int[]){type_scr},
	(int[]){type_scr},
	(int[]){type_scr},
	(int[]){type_clr,	type_num},
	(int[]){type_num},
	(int[]){type_loc},
	(int[]){type_mus,	type_num,	type_num},
	(int[]){type_sef,	type_num,	type_num},
	(int[]){type_num},
	(int[]){type_num,	type_num},
	(int[]){type_cfg,	type_num},
	(int[]){type_var,	type_vop,	type_num},
	(int[]){type_var,	type_chk,	type_num,	type_scr},
	(int[]){type_var,	type_chk,	type_num,	type_scr},
	(int[]){type_loc,	type_lde,	type_scr,	type_num},
	(int[]){type_loc,	type_dir,	type_loc,	type_num},
	(int[]){type_num},
	(int[]){type_scn},
};

char** func_arg_names[] = {
	(char*[]){},
	(char*[]){},
	(char*[]){},
	(char*[]){"new"},
	(char*[]){"sub"},
	(char*[]){"sub"},
	(char*[]){"fade","time"},
	(char*[]){"time"},
	(char*[]){"destination"},
	(char*[]){"song","fadein","loops"},
	(char*[]){"effect","volume","speed"},
	(char*[]){"fadeout"},
	(char*[]){"hours","minutes"},
	(char*[]){"var","value"},
	(char*[]){"var","operation","value"},
	(char*[]){"var","check","value","sub"},
	(char*[]){"var","check","value","new"},
	(char*[]){"target","element","new","arrow"},
	(char*[]){"target","exit","destination","distance"},
	(char*[]){"time"},
	(char*[]){"target"},
};


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Macros
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef enum{mac_moveto,mac_moveandplay,mac_fadeall,mac_fadeinscene,mac_fadeoutscene,mac_count} S_MACRO;
char* mac_names[] = {
	"moveto",
	"moveandplay",
	"fadeall",
	"showscene",
	"clearscene",
};

const int* mac_arg_types[] = {
	(int[]){type_loc,type_clr,type_num,type_sef},
	(int[]){type_loc,type_clr,type_num,type_sef,type_mus},
	(int[]){type_clr,type_num},
	(int[]){type_scn,type_clr,type_num},
	(int[]){type_num,type_clr},
};

char** mac_arg_names[] = {
	(char*[]){"destination","fade","time","sound"},
	(char*[]){"destination","fade","time","sound","music"},
	(char*[]){"fade","time"},
	(char*[]){"target","fade","time"},
	(char*[]){"time","fade"},
};

const int mac_arg_count[] = {
	4,5,2,3,2
};

const int* mac_expand[] = {
	(int[]){f_fadeout,-2,-3,f_transition,-1,f_playSE,-4,100,0,f_fadein,-3},
	(int[]){f_fadeout,-2,-3,f_transition,-1,f_playSE,-4,100,0,f_playBGM,-5,-3,0,f_fadein,-3},
	(int[]){f_fadeout,-1,-2,f_silence,-2},
	(int[]){f_fadeout,-2,-3,f_setscene,-1,f_fadein,-3},
	(int[]){f_fadeout,-2,-1,f_setscene,0,f_fadein,-1},
};

const int mac_size[] = {
	11,15,5,7,7,
};


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Enumerated Types
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//Colors
typedef enum {clr_black, clr_white, clr_count} COLOR;
char* clr_names[] = {
	"black",
	"white"
};
const int colormap[][4] = {
	{0x00, 0x00, 0x00, 0xFF},
	{0xFF, 0xFF, 0xFF, 0xFF}
};


//Weather
typedef enum {wth_clear, wth_cloud, wth_storm, wth_mist, wth_count} WEATHER;
char* wth_names[] = {
	"clear",
	"cloud",
	"storm",
	"mist"
};


//Direction
typedef enum {dir_up, dir_down, dir_left, dir_right, dir_any, dir_count} DIRECTION;
char* dir_names[] = {"up","down","left","right","any"};


//Cardinal Direction
typedef enum {crd_north, crd_south, crd_east, crd_west, crd_any, crd_count} CARDINAl;
char* crd_names[] = {"N","S","E","W","A"};


//Sky Type
typedef enum {	sky_none,	//No sky visible, e.g. indoor locations without windows
				sky_full,	//Sky always visible
				sky_limited,//Sky mostly visible, but evening sky becomes night sky
				sky_count
} SKY_TYPE;
char* sky_names[] = {"none","full","limited"};


//Shade Type
typedef enum {	shd_none,		//No shade at any time
				shd_full,		//Day, evening, and night shading
				shd_limited,	//Day and night shading, the latter used both at night and in the evening.
				shd_count
} SHADE_TYPE;
char* shd_names[] = {"none","full","limited"};


//Location Data Element
typedef enum {lde_enter, lde_act, lde_ex, lde_up, lde_down, lde_left, lde_right, lde_count} LDE;
char* lde_names[] = {"enter","act","ex","up","down","left","right"};


//Config Variable
#define DEFAULT_TEXT_SPEED 2
#define DEFAULT_VOLUME 100
typedef enum{cfg_text_speed,cfg_sef_volume,cfg_mus_volume,cfg_resolution,cfg_count} CONFIG_VAR;
int cfgVars[cfg_count];
const int cfgDefault[cfg_count] = {
	DEFAULT_TEXT_SPEED,DEFAULT_VOLUME,DEFAULT_VOLUME,res_default
};
char* cfg_names[cfg_count] = {
	"text_speed","sound_volume","music_volume","resolution"
};

#define text_speed cfgVars[cfg_text_speed]
#define mus_volume cfgVars[cfg_mus_volume]
#define sef_volume cfgVars[cfg_sef_volume]

int initCfg(){
	for(int i=0;i<cfg_count;i++){
		cfgVars[i]=cfgDefault[i];
	}
}


//Variable Modifier
typedef enum{vop_set,vop_add,vop_sub,vop_count} VAR_OP;
char* vop_names[] = {"set","add","subtract"};

#define MAKE_VOP(vop,statement) int vop##_func(uint8_t var,uint8_t x){return statement;}
MAKE_VOP(vop_set,x)
MAKE_VOP(vop_add,var + x)
MAKE_VOP(vop_sub,var + x)
int (*vop_funcs[vop_count])(uint8_t,uint8_t) = {vop_set_func,vop_add_func,vop_sub_func};


//Variable Check
typedef enum{chk_eq,chk_lo,chk_hi,chk_count} VAR_CHECK;
char* chk_names[] = {"equals","lower","higher"};

#define MAKE_CHK(chk,statement) int chk##_func(uint8_t var,uint8_t x){return statement;}
MAKE_CHK(chk_eq,var==x)
MAKE_CHK(chk_lo,var<x)
MAKE_CHK(chk_hi,var>x)
int(*chk_funcs[chk_count])(uint8_t,uint8_t) = {chk_eq_func,chk_lo_func,chk_hi_func};

int all_count[type_count] = {
	func_count,
	mac_count,
	clr_count,
	wth_count,
	dir_count,
	crd_count,
	sky_count,
	shd_count,
	lde_count,
	cfg_count,
	vop_count,
	chk_count,
};


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Non-Enumerated Types
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#define registerThing(A, B)	if(A##_count==MAX_##B##_COUNT){ERROR(1,"Max number of %ss registered",script_param_types[type_##A]);}\
							for(int i=0;i<A##_count;i++){\
								if(compare(A##_names[i],name)){ERROR(1,"Duplicate %s registered (%s)",script_param_types[type_##A],name);}\
							}\
							int len = strlen(name);\
							if(len>=MAX_##B##_NAME_LEN){ERROR(1,"'%s' exceeds %s name length limits",name,script_param_types[type_##A]);}\
							memcpy(A##_names[A##_count],name,len+1);\
							DEBUG("Registered %s '%s'",script_param_types[type_##A],name); A##_count++

//Script
#define MAX_SCR_COUNT 256
#define MAX_SCR_NAME_LEN 16
#define scr_count all_count[type_scr]
char scr_names[MAX_SCR_COUNT][MAX_SCR_NAME_LEN];
char* scr_names_lnk[MAX_SCR_COUNT];

//pre-defined scripts
typedef enum{scr_set_startup,scr_set_settings,scr_set_evening,scr_set_night,scr_set_count}SCR_SET;
const char* scr_set_names[] = {
	"startup",
	"settings",
	"evening",
	"night"
};
int scr_set[scr_set_count];

#define scr_startup  scr_set[scr_set_startup]
#define scr_settings scr_set[scr_set_settings]
#define scr_evening  scr_set[scr_set_evening]
#define scr_night    scr_set[scr_set_night]


//defined in script.c
int initScript(int ind);

int register_scr(const char* name){
	registerThing(scr,SCR);
#ifndef COMPILER
	for(int i=0;i<scr_set_count;i++){
		if(compare(scr_set_names[i],name)){
			scr_set[i]=scr_count-1;
			DEBUG("Set script '%s' found.",scr_set_names[i]);
		}
	}
	return initScript(scr_count-1);
#else
	return 0;
#endif
}


//Location
#define MAX_LOC_COUNT 64
#define MAX_LOC_NAME_LEN 16
#define loc_count all_count[type_loc]
char loc_names[MAX_LOC_COUNT][MAX_LOC_NAME_LEN];
char* loc_names_lnk[MAX_LOC_COUNT];
int loc_name_lengths[MAX_LOC_COUNT];

const char* loc_path_start = "IMG\\BG\\";
const int   loc_path_start_len = 7;
const char* loc_path_end = "\\main.png";
const int   loc_path_end_len = 9;
char loc_paths[MAX_LOC_COUNT][MAX_LOC_NAME_LEN+16];

//defined in loc.c
int initLocation(int ind);

int register_loc(const char* name){
	registerThing(loc,LOC);
	loc_name_lengths[loc_count-1]=len;
#ifndef COMPILER
	buildDataPath(loc,loc_count-1);
	return initLocation(loc_count-1);
#else
	return 0;
#endif
}


//Music
#define MAX_MUS_COUNT 32
#define MAX_MUS_NAME_LEN 16
#define mus_count all_count[type_mus]
char mus_names[MAX_MUS_COUNT][MAX_MUS_NAME_LEN];
char* mus_names_lnk[MAX_MUS_COUNT];
int mus_name_lengths[MAX_MUS_COUNT];

const char* mus_path_start = "MUS\\BGM\\";
const int   mus_path_start_len = 8;
const char* mus_path_end = ".wav";
const int   mus_path_end_len = 4;
char mus_paths[MAX_MUS_COUNT][MAX_MUS_NAME_LEN+12];

int register_mus(const char* name){
	registerThing(mus,MUS);
	mus_name_lengths[mus_count-1]=len;
#ifndef COMPILER
	buildDataPath(mus,mus_count-1);
#endif
	return 0;
}


//Sound
#define MAX_SEF_COUNT 128
#define MAX_SEF_NAME_LEN 16
#define sef_count all_count[type_sef]
char sef_names[MAX_SEF_COUNT][MAX_SEF_NAME_LEN];
char* sef_names_lnk[MAX_SEF_COUNT];
int sef_name_lengths[MAX_SEF_COUNT];

const char* sef_path_start = "MUS\\SE\\";
const int   sef_path_start_len = 7;
const char* sef_path_end = ".wav";
const int   sef_path_end_len = 4;
char sef_paths[MAX_SEF_COUNT][MAX_SEF_NAME_LEN+11];

int sef_steps = 0;

int register_sef(const char* name){
	registerThing(sef,SEF);
	sef_name_lengths[sef_count-1]=len;
#ifndef COMPILER
	if(compare(name,"steps")){
		sef_steps = sef_count-1;
		DEBUG("'steps' sound effect found.");
	}
	buildDataPath(sef,sef_count-1);
#endif
	return 0;
}


//Scene
#define MAX_SCN_COUNT 32
#define MAX_SCN_NAME_LEN 16
#define scn_count all_count[type_scn]
char scn_names[MAX_SCN_COUNT][MAX_SCN_NAME_LEN];
char* scn_names_lnk[MAX_SCN_COUNT];
int scn_name_lengths[MAX_SCN_COUNT];

const char* scn_path_start = "IMG\\SCN\\";
const int   scn_path_start_len = 8;
const char* scn_path_end = ".png";
const int   scn_path_end_len = 4;
char scn_paths[MAX_SCN_COUNT][MAX_SCN_NAME_LEN+11];

int register_scn(const char* name){
	registerThing(scn,SCN);
	scn_name_lengths[scn_count-1]=len;
#ifndef COMPILER
	buildDataPath(scn,scn_count-1);
#endif
	return 0;
}

//Game Variable
#define VARCOUNT 32
#define MAX_VAR_COUNT 32
#define MAX_VAR_NAME_LEN 16

typedef enum{var_location,var_weather,var_time_hr,var_time_min,var_music,var_setCount} setVars;

#define location gameVars[var_location]
#define weather gameVars[var_weather]
#define hours gameVars[var_time_hr]
#define minutes gameVars[var_time_min]
#define bgm gameVars[var_music]

#define var_count all_count[type_var]

uint8_t gameVars[VARCOUNT];
//special values
char var_names[VARCOUNT][MAX_VAR_NAME_LEN];
char* var_names_lnk[VARCOUNT];

int register_var(const char* name){
	registerThing(var,VAR);
	return 0;
}

//Numeric Types

int num_parse(int* num, char* name){
	char* rem;
	*num = strtol(name,&rem,10);
	if((*num==0&&name[0]!='0')||rem[0]){return 0;}
	return 1;
}

//Some type bookkeeping information
char** all_names[type_count] = {
	func_names,
	mac_names,
	clr_names,
	wth_names,
	dir_names,
	crd_names,
	sky_names,
	shd_names,
	lde_names,
	cfg_names,
	vop_names,
	chk_names,
	scr_names_lnk,
	loc_names_lnk,
	mus_names_lnk,
	sef_names_lnk,
	scn_names_lnk,
	var_names_lnk,
	NULL,		//Numbers don't have names.
};

int searchNames(int type, char* name){
	if(type<0||type>=type_count){ERROR(-1,"Unrecognized type");}
	else if(type==type_num){
		int res;
		if(num_parse(&res,name)){return res;}
		return -1;
		/*
		char* rem;
		int res = strtol(name,&rem,10);
		if((res==0&&name[0]!='0')||rem[0]){
			return -1;
		}
		return res;*/
	}else{
//DEBUG("SEARCHING %d '%s' FOR '%s'",all_count[type],script_param_types[type],name);
		for(int i=0;i<all_count[type];i++){
//DEBUG("\t'%s'",all_names[type][i]);
			if(compare(all_names[type][i],name)){return i;}
		}
		return -1;
	}
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//File paths
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#define IMG_FORMAT		".png"
#define SOUND_FORMAT	".wav"
#define BG_PATH(name)	"IMG\\BG\\" #name "\\main" IMG_FORMAT
#define SKY_PATH(name)  "IMG\\BG\\sky\\" #name IMG_FORMAT
#define SHADE_PATH(name, suff) "IMG\\BG\\" #name "\\" #suff IMG_FORMAT
#define MUS_PATH(name)	"MSC\\BGM\\" #name SOUND_FORMAT
#define SE_PATH(name)	"MSC\\SE\\" #name SOUND_FORMAT

const char* bgStart = "IMG\\BG\\";
const int bgslen = 7;
const char* bgEnd = "\\main.png";
const int bgelen = 9;
const char* bgShade = "\\shade.png";
const int bgsllen = 10;
const char* bgShadeEve = "\\shade_eve.png";
const int bgsflen = 14;
const char* bgShadeNgt = "\\shade_ngt.png";


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Time-related data
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------


//Time of day
typedef enum {tod_day, tod_evening, tod_night, tod_count} TIME_OF_DAY;
const char* tod_names[] = {
	"day",
	"evening",
	"night"
};

//Special game-relevant times
#define SHIFT_START 6
#define EVENING_START 8
#define NIGHT_START 10
#define SHIFT_END 15

//Returns day/evening/night based on the current time
TIME_OF_DAY getTimeOfDay(){
	if(hours>=EVENING_START){
		if(hours>=NIGHT_START){return tod_night;}
		return tod_evening;
	}
	return tod_day;
}

int addTime(int h, int m){
	TIME_OF_DAY tod = getTimeOfDay();
	hours+=h;
	minutes+=m;
	if(minutes>=60){minutes-=60;hours++;}
	if(scr_evening&&tod==tod_day&&getTimeOfDay()==tod_evening){return scr_evening;}
	if(scr_night&&tod==tod_evening&&getTimeOfDay()==tod_night){return scr_night;}
	return 0;
}
int setTime(int h, int m){
	hours=h;
	minutes=m;
}

//Tools for finding the appropriate backgrounds for the sky/shade
//Really hacked together for now, will be a lot cleaner once I've actually drawn all the backgrounds and figured out what to cut out.

const char* wth_paths[] = {
	NULL,
	SKY_PATH(A_day_clear),	// 1
	SKY_PATH(A_day_cloud),	// 2
	SKY_PATH(A_day_storm),	// 3
	SKY_PATH(A_ngt_clear),	// 4
	SKY_PATH(A_ngt_cloud),	// 5
	SKY_PATH(A_ngt_storm),	// 6
	SKY_PATH(N_eve_clear),	// 7
	SKY_PATH(N_eve_cloud),	// 8
	SKY_PATH(W_eve_clear),	// 9
	SKY_PATH(W_eve_cloud)	//10
};

const int sky_map[sky_count][tod_count][wth_count][dir_any] = {
	//sky_type_none
	{{{0},{0},{0},{0}},
	 {{0},{0},{0},{0}},
	 {{0},{0},{0},{0}}},
	//sky_type_full
	{{{ 1, 1, 1, 1},{ 2, 2, 2, 2},{ 3, 3, 3, 3},{ 3, 3, 3, 3}},
	 {{ 7, 0, 0, 9},{ 8, 0, 0,10},{ 6, 6, 6, 6},{ 6, 6, 6, 6}},
	 {{ 4, 4, 4, 4},{ 5, 5, 5, 5},{ 6, 6, 6, 6},{ 6, 6, 6, 6}}},
	//sky_type_limited
	{{{ 1, 1, 1, 1},{ 2, 2, 2, 2},{ 3, 3, 3, 3},{ 3, 3, 3, 3}},
	 {{ 4, 4, 4, 4},{ 5, 5, 5, 5},{ 6, 6, 6, 6},{ 6, 6, 6, 6}},
	 {{ 4, 4, 4, 4},{ 5, 5, 5, 5},{ 6, 6, 6, 6},{ 6, 6, 6, 6}}}
};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Game variables and data
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#define MAX_NAME_LEN 64
#define LOAD_GAME_DAT(O,A) \
DEBUG("Loading %s names...",script_param_types[type_##A]);\
count = readInt(data);\
if(count<0){ERROR(2,"Problem reading %s count",script_param_types[type_##A]);}\
for(int i=O;i<count;i++){\
	int nlen=fgetc(data);\
	if(nlen<0){ERROR(2,"Problem reading name length");}\
	if(nlen>MAX_NAME_LEN){ERROR(1,"Generic name length restrictions exceeded (%d)",nlen);}\
	fread(name,1,nlen,data);\
	name[nlen]=0;\
	if(register_##A(name)){return 1;}\
}DEBUG("Done")


int loadGameValues(FILE* data){
	int count = 0;
	char name[MAX_NAME_LEN];
	LOAD_GAME_DAT(1,scr);
	LOAD_GAME_DAT(0,loc);
	LOAD_GAME_DAT(1,mus);
	LOAD_GAME_DAT(1,sef);
	LOAD_GAME_DAT(1,scn);
	LOAD_GAME_DAT(5,var);
	return 0;
}

int loadConfig(FILE* data){
	char buffer[2048];
	char* rem;
	for(int i=0;i<cfg_count;i++){
		if(!fgets(buffer,2048,data)){ERROR(1,"Could not read config file.");}
		if(!fgets(buffer,2048,data)){ERROR(1,"Could not read config file.");}
		int cval = strtol(buffer,&rem,10);
		if(cval==0&&buffer[0]!='0'){
			DEBUG("Could not read %s from config file, setting to default.",cfg_names[i]);
			cfgVars[i]=cfgDefault[i];
		}
		cfgVars[i]=cval;
	}
	
}

//Setting initial values for important game constants
#define linkNames(A,B)\
	for(int i=0;i<MAX_##B##_COUNT;i++){A##_names_lnk[i]=&(A##_names[i][0]);}
int initGameValues(){
	linkNames(scr,SCR);
	linkNames(loc,LOC);
	linkNames(mus,MUS);
	linkNames(sef,SEF);
	linkNames(scn,SCN);
	linkNames(var,VAR);
	DEBUG("Registering initial data");
	register_scr("none");
	register_scn("none");
	register_sef("none");
	register_mus("silence");
	register_var("location");
	register_var("weather");
	register_var("hour");
	register_var("minute");
	register_var("music");
	weather = wth_clear;
	hours = SHIFT_START;
	minutes = 0;
	bgm = 0;
	#ifndef COMPILER
	DEBUG("Loading config data from '%s'...",cfg_source);
	FILE* data = fopen(cfg_source,"r");
	if(data==NULL){ERROR(1,"Could not open file");}
	loadConfig(data);
	res = resolutions[cfgVars[cfg_resolution]];
	DEBUG("Loading data from '%s'...",dat_bin);
	data = fopen(dat_bin,"rb");
	if(data==NULL){ERROR(1,"Could not open file");}
	int err = loadGameValues(data);
	fclose(data);
	return err;
	#endif
	initCfg();
}