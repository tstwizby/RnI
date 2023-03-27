//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Handles errors and logging
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#ifndef DEBUG_VERSION
#define DEBUG_VERSION 2
#endif

#if DEBUG_VERSION == 0
#define RAW_NAMES 0
#else
#define RAW_NAMES 1
#endif

#if DEBUG_VERSION > 1
#define ERROR(code, message,...) printf(message "\n", ##__VA_ARGS__); return code
#define DEBUG(message,...) printf(message "\n", ##__VA_ARGS__)
#elif DEBUG_VERSION == 1
#define ERROR(code, message,...) logMessage(message "\n",...); return code
#define DEBUG(message,...) logMessage(message "\n",...)
const char* log_path = "log.txt";

int clearLog(){
	FILE* log = fopen(log_path,"w");
	if(!log){printf("Could not write to log file '%s'!\n",log_path);return 1;}
	fprintf(log,"Starting log...\n");
	fclose(log);
}

int log(const char* message,...){
	FILE* log = fopen(log_path,"a");
	if(!log){printf("Could not write to log file '%s'\n",log_path);return 1;}
	va_list argptr;
	va_start(argptr,message);
	vfprintf(log,message,argptr);
	fclose(log);
	va_end(argptr);
	return 0;
}
#else
#define ERROR(code, message,...) return code
#define DEBUG(message,...)
#endif

