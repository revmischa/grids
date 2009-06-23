



////////////////////////////////////
// Utilities
///////////////////////////////////


//  expandFilename(filename)
// Expand "~/" with the $HOME env. variable in a file name.
// The caller must free the string after use.
char *expand_filename(const char *fname)
{
	char* buffer;

	if (!fname)
		return NULL;
	if (!strncmp(fname, "~/", 2)) {
		char *homedir = getenv("HOME");
		if (homedir){
			int new_size = strlen( homedir ) + strlen( fname ); // subtrat 1 for the ~, add 1 for the \0
			buffer = malloc( (new_size ) * sizeof( char ) ); 
			sprintf(buffer, "%s%s", homedir, fname + sizeof(char) ); // remove the ~		   
			return buffer;
		}
	}
	return strdup(fname);
}

