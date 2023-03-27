const char* script_raw = "Scripts\\script_raw.txt";
const char* loc_raw = "Scripts\\loc_raw.txt";
const char* dat_raw = "Scripts\\dat_raw.txt";

const char* script_bin = "Scripts\\scripts.dat";
const char* loc_bin = "Scripts\\loc.dat";
const char* dat_bin = "Scripts\\names.dat";

const char* cfg_source = "config.txt";

//writes a 2-byte integer A to file B
#define writeInt(A, B) fputc((A/256),B); fputc((A%256),B)

//reads a 2-byte integer from file B, without checking for EOF (If at EOF, should result in the integer becoming negative)
#define readInt(B) fgetc(B)*256+fgetc(B)

//builds a path for a resource
#define buildDataPath(A,B) \
	memcpy(A##_paths[B],A##_path_start,A##_path_start_len);\
	memcpy(A##_paths[B]+A##_path_start_len,A##_names[B],A##_name_lengths[B]);\
	memcpy(A##_paths[B]+A##_path_start_len+A##_name_lengths[B],A##_path_end,A##_path_end_len);\
	DEBUG("Registered %s path '%s'",script_param_types[type_##A],A##_paths[B])
