#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>

typedef uint8_t (*conversion)(uint8_t);

//returns 1 if A = B, 0 if A != B, and a negative number if an error occurs. 
int compare(const char* A, const char* B){
	if(B==NULL||A==NULL){return -1;}
	int i=0;
	while(A[i]&&B[i]&&(A[i]==B[i])){i++;}
	return (B[i]==A[i])?1:0;
}


//Given a destination buffer and source buffer, copies the source contents except leading/trailing whitespace to destination, up to max chars.
//Returns length of trimmed string.
int trim(char* dest, char* source, int max, conversion conv){
	int start = 0;
	int i = 0;
	while((source[i])&&(isspace(source[i]))){i++;}
	start = i;
	while(source[i]&&(i-start<max)){i++;}
	i--;
	
	while(isspace(source[i])){i--;}
	for(int j=start;j<i+1;j++){
		dest[j-start]=conv(source[j]);
	}
	//memcpy(dest,source+start,i-start+1);
	dest[i-start+2]=0;
	return i-start+1;
}

//Given a source buffer, left bound, and right bound, counts the number of whitespace-separated terms between the first ocurrence of the left bound and the first subsequent ocurrence of the right bound.
//if start/end aren't found, returns 0.
int encCount(char* source, char start, char end){
	int count = 0;
	int ind = 0;
	while(source[ind]&&source[ind]!=start){ind++;}
	ind++;
	while(isspace(source[ind])){ind++;}
	if(source[ind]!=end){
		count++;
		while(source[ind]&&source[ind]!=end){
			if(isspace(source[ind])){
				while(source[ind]&&isspace(source[ind])){ind++;}
				if(source[ind]!=end){count++;}
			}
			ind++;
		}
		if(source[ind]!=end){return 0;}
	}
	return count;
}

//given a source buffer and destination buffers (assumed to be sufficient in number to hold the resultant data), copies each term as described in encCount to the destination buffers in order.
//breaks if given a source incompatible with encCount (encCount(source)=0) or too few output buffers. Returns the same thing as encCount if successful.
//it's the responsibility of the caller to free dest properly
int enclosed(char** dest, char* source, char start, char end){
	int count = 0;
	int a = 0;
	int ind = 0;
	while(source[ind]&&source[ind]!=start){ind++;}
	ind++;
	while(source[ind]!=end){
		while(isspace(source[ind])){ind++;}
		if(source[ind]!=end){
			a=ind;
			while(!isspace(source[ind])&&source[ind]!=end){ind++;}
			dest[count]=malloc((sizeof *dest)*(ind-a+1));
			dest[count][ind-a]=0;
			memcpy(dest[count],source+a,ind-a);
			count++;
		}
	}
	return count;
}

char* testA = "   hello there    ";
char* testB = "    {     a   bcde    f  ghi  j}    } ";
/*
int main(){
	char trimtest[100];
	char *enctest[5];
	printf("%s\n",testA);
	printf("%d: '%s'\n",trim(trimtest, testA),trimtest);
	printf("%s\n",testB);
	printf("%d: ",encCount(testB,'{','}'));
	enclosed(enctest,testB,'{','}');
	printf("(%s), (%s), (%s), (%s), (%s)", enctest[0],enctest[1],enctest[2],enctest[3],enctest[4]);
	for(int i=0;i<5;i++){free(enctest[i]);}
}*/