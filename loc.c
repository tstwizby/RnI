//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Contains data on locations and their associated scripts
//
//Defines noteable structs:
//	Location
//
//Defines notable functions:
//	buildLocation(obj pointer, index, direction, sky type, shade type)
//	loadLocs()
//	diagnoseLoc(object pointer)
//	diagnoseLocs()
//	locCleanup()
//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//restrictions and shorthands
#define cLocation locations[gameVars[var_location]]

//Cardinal directions, used to build sky texture path names

//Relative directions, used for travel via arrow keys

//Sky types, used to determine whether or not various views of the sky are visible

//Shade types, used to determine how lighting changes in the evening/night

//Location data elements, used when modifying parts of a location on the fly

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Location struct and related global variables
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef struct Location{
	int index;
	char* name;
	DIRECTION dir;
	SKY_TYPE sky;
	SHADE_TYPE shade;
	int left;
	int right;
	int up;
	int down;
	int l_dist;
	int r_dist;
	int u_dist;
	int d_dist;
	int u_arr;
	int d_arr;
	int l_arr;
	int r_arr;
	int script;
	int action;
	int examine;
} Location;

Location locations[MAX_LOC_COUNT];

//Returning null means the sky is not visible and need not be rendered
const char* getSkyPath(){
	return wth_paths[sky_map[cLocation.sky][getTimeOfDay()][weather][cLocation.dir]];
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Location functions
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int initLocation(int i){
	if(0>i||i>=MAX_LOC_COUNT){ERROR(1,"Location index out of bounds");}
	locations[i].index = i;
	locations[i].name = loc_names[i];
	locations[i].dir = 0;
	locations[i].sky = 0;
	locations[i].shade = 0;
	locations[i].left = -1;
	locations[i].l_dist = 0;
	locations[i].l_arr = 0;
	locations[i].right = -1;
	locations[i].r_dist = 0;
	locations[i].r_arr = 0;
	locations[i].up = -1;
	locations[i].u_dist = 0;
	locations[i].u_arr = 0;
	locations[i].down = -1;
	locations[i].d_dist = 0;
	locations[i].d_arr = 0;
	locations[i].script = -1;
	locations[i].action = -1;
	locations[i].examine = -1;
	return 0;
}

int buildLocation(Location* loc, int index, DIRECTION dir, SKY_TYPE sky, SHADE_TYPE shade){
	loc->index = index;
	loc->name = loc_names[index];
	loc->dir = dir;
	loc->sky = sky;
	loc->shade = shade;
	loc->left = -1;
	loc->l_dist = 0;
	loc->l_arr = 0;
	loc->right = -1;
	loc->r_dist = 0;
	loc->r_arr = 0;
	loc->up = -1;
	loc->u_dist = 0;
	loc->u_arr = 0;
	loc->down = -1;
	loc->d_dist = 0;
	loc->d_arr = 0;
	loc->script = -1;
	loc->action = -1;
	loc->examine = -1;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Location data management functions
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int loadLocs(){
	DEBUG("Building Locations...");
	FILE* source = fopen(loc_bin,"rb");
	if(source == NULL){ERROR(1,"Could not open location source file '%s'.",loc_bin);}
	for(int lc=0;lc<loc_count;lc++){
		DEBUG("Loading location %s...",loc_names[lc]);
		uint8_t loc_buffer[18];
		if(fread(loc_buffer,18,1,source)<1){fclose(source);ERROR(2,"Failed to read data for location %s.",loc_names[lc]);}
		locations[lc].dir = loc_buffer[0];
		locations[lc].sky = loc_buffer[1];
		locations[lc].shade = loc_buffer[2];
		locations[lc].u_dist = (loc_buffer[11]&8)?loc_buffer[4]:-1;
		locations[lc].d_dist = (loc_buffer[11]&4)?loc_buffer[6]:-1;
		locations[lc].l_dist = (loc_buffer[11]&2)?loc_buffer[8]:-1;
		locations[lc].r_dist = (loc_buffer[11]&1)?loc_buffer[10]:-1;
		locations[lc].u_arr  = loc_buffer[3]?1:0;
		locations[lc].d_arr  = loc_buffer[5]?1:0;
		locations[lc].l_arr  = loc_buffer[7]?1:0;
		locations[lc].r_arr  = loc_buffer[9]?1:0;
		locations[lc].up     = loc_buffer[3];
		locations[lc].down   = loc_buffer[5];
		locations[lc].left   = loc_buffer[7];
		locations[lc].right  = loc_buffer[9];
		locations[lc].script  = loc_buffer[13]+loc_buffer[12]*256;
		locations[lc].action  = loc_buffer[15]+loc_buffer[14]*256;
		locations[lc].examine = loc_buffer[17]+loc_buffer[16]*256;
	}
	return 0;
}

int diagnoseLoc(Location l){
	DEBUG("Location %s:",l.name);
	DEBUG("\tDir: %s  Sky: %s  Shade %s",crd_names[l.dir],sky_names[l.sky],shd_names[l.shade]);
	DEBUG("\tNeighbors:");
	if(l.u_dist>=0){DEBUG("\t\tUp: %s (%d)",loc_names[l.up],l.u_dist);}
	else{DEBUG("\t\tUp: %s",scr_names[l.up]);}
	if(l.d_dist>=0){DEBUG("\t\tDown: %s (%d)",loc_names[l.down],l.d_dist);}
	else{DEBUG("\t\tDown: %s",scr_names[l.down]);}
	if(l.l_dist>=0){DEBUG("\t\tLeft: %s (%d)",loc_names[l.left],l.l_dist);}
	else{DEBUG("\t\tLeft: %s",scr_names[l.left]);}
	if(l.r_dist>=0){DEBUG("\t\tRight: %s (%d)",loc_names[l.right],l.r_dist);}
	else{DEBUG("\t\tRight: %s",scr_names[l.right]);}
	DEBUG("\tScripts:");
	DEBUG("\t\tEnter: %s",  scr_names[l.script]);
	DEBUG("\t\tAction: %s", scr_names[l.action]);
	DEBUG("\t\tExamine: %s",scr_names[l.examine]);
}
int diagnoseLocs(){
	for(int i=1;i<loc_count;i++){
		diagnoseLoc(locations[i]);
	}
}
