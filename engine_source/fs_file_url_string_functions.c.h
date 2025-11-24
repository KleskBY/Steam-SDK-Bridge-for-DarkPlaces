// fs_file_url_string_functions.c.h



// Turns c:/mypath/something into c:\mypath\something
char *File_URL_Edit_SlashesBack_Like_Windows (char *unix_path_to_file)
{
	// Translate "/" to "\"
	return String_Edit_Replace_Char (unix_path_to_file, '/' /*find*/, '\\' /*replace with*/, NULL /* don't want count */);
}

char *File_URL_Remove_Trailing_Unix_Slash (char *path_to_file)
{
	if (String_Ends_With (path_to_file, "/")) {
		size_t slen = strlen (path_to_file);
		path_to_file[slen - 1] = 0;
	}
	return path_to_file;;
}

char *File_URL_Remove_Trailing_Windows_Slash (char *path_to_file)
{
	if (String_Ends_With (path_to_file, "/")) {
		size_t slen = strlen (path_to_file);
		path_to_file[slen - 1] = 0;
	}
	return path_to_file;;
}

// May 24 2025 - Many callers
const char *File_URL_SkipPath (const char *path_to_file)
{
	return String_Skip_Char_Reverse (path_to_file, '/');
}


// Baker: What is DP equivalent.
RELATED_ (FS_StripExtension /*DP equivalent*/)
char *File_URL_Edit_Remove_Extension (char *path_to_file)
{
	char *terminate_point = strrchr (path_to_file, '.');

	if (terminate_point)
		*terminate_point = '\0';

	return path_to_file;
}

char *File_URL_Edit_Strip_Set_Extension (char *path_to_file, ccs *ext, size_t path_to_file_sizeof)
{
	File_URL_Edit_Remove_Extension (path_to_file);
	strlcat (path_to_file, ext, path_to_file_sizeof);
	return path_to_file;
}


// Returns extension with . (like .png) after last slash if exists, returns NULL if nothing found.
ccs *File_URL_GetExtension_With_Dot (ccs *path_to_file)
{
	ccs *slash_position = strrchr /*reverse*/ (path_to_file, '/');
	if (slash_position)
		path_to_file = &slash_position[1]; // After last slash
	ccs *dot_position = strrchr /*reverse*/ (path_to_file, '.');

	if (dot_position)
		return dot_position;
	return NULL;
}

void File_URL_Edit_Default_Extension (char *path_to_file, const char *dot_new_extension, size_t bufsize)
{
	//const char *extension = File_URL_GetExtension_With_Dot (path_to_file);
	ccs *extension = File_URL_GetExtension_With_Dot (path_to_file);

	if (extension[0] == 0) {
		// No extension so default it
		strlcat (path_to_file, dot_new_extension, bufsize);
	}
}


// May 18 2020 - This does NOT leave a trailing "/".  It is also UNIX only.
char *File_URL_Edit_Reduce_To_Parent_Path_No_Trailing_Slash (char *path_to_file)
{
	char *terminate_point = strrchr (path_to_file, '/');

	if (terminate_point) {
		*terminate_point = '\0';
	} else {
		path_to_file[0] = '\0';
	}

	return path_to_file;
}

char *File_URL_Edit_Reduce_To_Parent_Path_Trailing_Slash (char *path_to_file)
{
	File_URL_Edit_Reduce_To_Parent_Path_No_Trailing_Slash (path_to_file);
	if (String_Ends_With (path_to_file, "/") == false) {
		int slen = strlen(path_to_file);
		path_to_file[slen + 0] = '/';
		path_to_file[slen + 1] = NULL_CHAR_0;
	}
	return path_to_file;
}

char *File_URL_Edit_SlashesForward_Like_Unix (char *windows_path_to_file)
{
	return String_Edit_Replace_Char (windows_path_to_file, '\\' /*find*/, '/' /*replace with*/, NULL /* don't want count */);
}




