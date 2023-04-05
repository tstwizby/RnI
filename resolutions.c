/*
Noteable sizes:

Backgrounds- 1920x1080
  3/4 scale- 1440x810
 half scale-  960x540
  1/4 scale-  480x270

       Text- 32px per line
 half scale- 16 px per line

   Text box: four lines, with a margin and a border
     height- 2px border + 6px margin + 4x32px line + 6px margin + 2px border = 144 px
      width- bg width -2px border on each side

	 arrows- 

total height = top margin + border + bg + border + text box + bottom margin
540+144+20+20

*/

typedef enum{res_1440p,res_1080p,res_720p,res_480p,res_count} RESOLUTION;
const char* res_names[] = {
	"1440p (2560x1440)",
	"1080p (1920x1080)",
	 "720p (1280x720)",
	 "480p (640x480)",
};

const int bg_raw_width = 1920;
const int bg_raw_height = 1080;

const int no_arrow_source_x = 0;
const int no_arrow_source_y = 0;
const int arrow_source_x = 0;
const int arrow_source_y[] = {
	0,42,84
};

const int arrow_width = 82;
const int arrow_height = 42;

const int arrows_width = 130;
const int arrows_height = 130;

const int up_arrow_x    = 24;
const int up_arrow_y    = 0;
const int down_arrow_x  = 24;
const int down_arrow_y  = 88;
const int left_arrow_x  = -20;
const int left_arrow_y  = 44;
const int right_arrow_x = 68;
const int right_arrow_y = 44;

int menu_height = 0;

typedef struct _Resolution{
	RESOLUTION resolution;
	int screen_width;
	int screen_height;
	int bg_width;
	int bg_height;
	int bg_x;
	int bg_y;
	int arrows_x;
	int arrows_y;
	int txt_width;
	int txt_height;
	int txt_x;
	int txt_y;
	int menu_width;
	int menu_x;
	int menu_y;
} Resolution;

const Resolution res_1440p_s = {
	   .resolution = res_1440p,
	 .screen_width = 2560,
	.screen_height = 1440,
		 .bg_width = 1920,
		.bg_height = 1080,
			 .bg_x =  320,
			 .bg_y =   80,
		 .arrows_x =  343,
		 .arrows_y = 1183,
		.txt_width = 1744,
	   .txt_height =  176,
			.txt_x =  496,
			.txt_y = 1160,
	   .menu_width =  840,
		   .menu_x =  860,
		   .menu_y =  100};
const Resolution res_1080p_s = {
	   .resolution = res_1080p,
	 .screen_width = 1920,
	.screen_height = 1080,
		 .bg_width = 1440,
		.bg_height =  810,
			 .bg_x =  240,
			 .bg_y =   40,
		 .arrows_x =  263,
		 .arrows_y =  873,
		.txt_width = 1264,
	   .txt_height =  176,
			.txt_x =  416,
			.txt_y =  850,
	   .menu_width =  840,
		   .menu_x =  540,
		   .menu_y =   60};
const Resolution res_720p_s = {
	   .resolution = res_720p,
	 .screen_width = 1280,
	.screen_height =  720,
		 .bg_width =  960,
		.bg_height =  540,
			 .bg_x =  160,
			 .bg_y =   30,
		 .arrows_x =  170,
		 .arrows_y =  577,
		.txt_width =  810,
	   .txt_height =  144,
			.txt_x =  310,
			.txt_y =  570,
	   .menu_width =  640,
		   .menu_x =  320,
		   .menu_y =   50};
const Resolution res_480p_s = {
	   .resolution = res_480p,
	 .screen_width = 640,
	.screen_height = 480,
		 .bg_width = 480,
		.bg_height = 270,
			 .bg_x =  140,
			 .bg_y =  20,
		 .arrows_x =  5,
		 .arrows_y = 150,
		.txt_width = 600,
	   .txt_height = 164,
			.txt_x = 20,
			.txt_y = 300,
	   .menu_width = 480,
		   .menu_x =  80,
		   .menu_y =  40};

const int res_default = res_720p;
Resolution res = res_720p_s;

const Resolution resolutions[] = {
	res_1440p_s,
	res_1080p_s,
	res_720p_s,
	res_480p_s,
};

