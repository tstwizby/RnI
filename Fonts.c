//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Font class and associated stuff (temporary, due for a rework post-demo)
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//Some notes on font format:
//  fonts are stored at the moment in .dat files in /fonts, along with png textures
//  the font data format consists of an eight byte header followed by character width data
//  the header contains the following data:
//    1- the height of the font in pixels
//    2- the maximum character width in pixels
//    3- the number of characters in a row in the image
//    4- the number of rows containing characters in the image
//    5- the vertical offset of each row (to account for letters like 'g' or 'q' that descend below the bottom of a line)
//    6- the width of a space
//    7- dummy
//    8- dummy


//Defined in SDLWrapper.c
int loadFontTexture(int f, char* path);

typedef struct Font{
	int height;
	int width;
	int columns;
	int rows;
	int voffset;
	int space;
	uint8_t* charwidth;
} Font;

Font fonts[FONT_COUNT];

//Loads data for font f from the file name given by path.
int loadFont(int f, int l, char* path){
	char datapath[l+11];
	datapath[0]='F';
	datapath[1]='o';
	datapath[2]='n';
	datapath[3]='t';
	datapath[4]='s';
	datapath[5]='\\';
	for(int i=0;path[i];i++){datapath[i+6]=path[i];}
	datapath[l+6]='.';
	datapath[l+7]='p';
	datapath[l+8]='n';
	datapath[l+9]='g';
	datapath[l+10]=0;
	if(loadFontTexture(f,datapath)>0){ERROR(1,"Could not load font texture");}
	datapath[l+7]='d';
	datapath[l+8]='a';
	datapath[l+9]='t';
	datapath[l+10]=0;
	FILE *fontfile;
	uint8_t header[8];
	fontfile = fopen(datapath, "rb");
	if(fread(header,1,8,fontfile)<8){fclose(fontfile);ERROR(2,"Error reading header of font data file %s",datapath);}
	fonts[f].height=header[0];
	fonts[f].width=header[1];
	fonts[f].columns=header[2];
	fonts[f].rows=header[3];
	fonts[f].voffset=header[4];
	fonts[f].space=header[5];
	printf("%d %d %d %d %d %d\n",header[0],header[1],header[2],header[3],header[4],header[5]);
	int ccount = header[2]*header[3];
	fonts[f].charwidth=malloc(ccount);
	if(fread(fonts[f].charwidth, 1, ccount, fontfile)<ccount){fclose(fontfile);ERROR(2,"Error reading font data file %s",datapath);}
	fclose(fontfile);
	return 0;
}

//Calculates the length of a word in the given font in pixels. Fails for words longer than 100 characters
int wordLength(int f, uint8_t* data){
	int len = 0;
	for(int i=0;data[i]>ctrl_max&&i<100;i++){
		len += fonts[f].charwidth[data[i]-ctrl_max-1];
	}
	return len;
}

int fontCleanup(){
	for(int i=0;i<FONT_COUNT;i++){free(fonts[i].charwidth);}
	return 0;
}