// sys_sdl_clipboard.c.h

CALLERS_ (Sys_ConsoleInput_WinQuake) // May 24 2025

char *Sys_Clipboard_Get_Text_Line_Static (void)
{
	static char out[SYSTEM_STRING_SIZE_1024];
	char *cliptext_a = Sys_Clipboard_Get_Text_ZAlloc();

	out[0] = 0; // In case cliptext_a is NULL
	if (cliptext_a) {
		c_strlcpy (out, cliptext_a);
		Z_Free(cliptext_a); cliptext_a = NULL;
		String_Edit_To_Single_Line (out); // spaces < 32 except for newline, cr, backspace which it kills.
	}

	return out;
}


char *Sys_Clipboard_Get_Text_ZAlloc (void)
{
	char *data = NULL;
	char *cliptext;

	cliptext = SDL_GetClipboardText();
	if (cliptext != NULL) {
		data = Z_StrDup (cliptext);
		SDL_free(cliptext);
	}

	return data;
}


// Returns 1 on success, 0 on failure
qbool Sys_Clipboard_Set_Text_Is_Ok(const char *text_to_clipboard)
{
	return !SDL_SetClipboardText(text_to_clipboard);
}


// Baker: Will have NO alpha channel ... all zeroes!
#ifdef _WIN32
unsigned *Sys_Clipboard_Get_Image_BGRA_ZAlloc (int *outwidth, int *outheight)
{
// Was extremely helpful info ... https://sites.google.com/site/michaelleesimons/clipboard
	byte *ptr = NULL;
	(*outwidth) = 0;
	(*outheight) = 0;
	if (OpenClipboard(NULL)) {
		HBITMAP hBitmap = (HBITMAP)GetClipboardData (CF_BITMAP);
		BITMAP csBitmap;

		if (hBitmap && GetObject(hBitmap, sizeof(csBitmap), &csBitmap) && csBitmap.bmBitsPixel == BGRA_BPP_32) {
			// allocate buffer
			int bufsize = csBitmap.bmWidth * csBitmap.bmHeight * (csBitmap.bmBitsPixel / 8);

			//csBitmap.bmBits = ptr = (byte *)malloc (bufsize); // "bmbits buffer"
			csBitmap.bmBits = ptr = (byte *)Mem_Alloc (zonemempool, bufsize); // "bmbits buffer"
			GetBitmapBits((HBITMAP)hBitmap, bufsize, csBitmap.bmBits);

#if 0 // We want BGRA
			// Convert BGRA --> RGBA, set alpha full since clipboard loses it somehow
			for (int i = 0; i < bufsize; i += RGBA_4)
			{
				byte temp = ptr[i + 0];
				ptr[i + 0] = ptr[i + 2];
				ptr[i + 2] = temp;
				ptr[i + 3] = 255; // Full alpha
			}
#endif
			*outwidth = csBitmap.bmWidth;
			*outheight = csBitmap.bmHeight;
		}
		CloseClipboard ();
	}
	return (unsigned *)ptr;
}
#else
unsigned *Sys_Clipboard_Get_Image_BGRA_ZAlloc (int *outwidth, int *outheight)
{
	return NULL;
}
#endif // _WIN32

#ifdef _WIN32
qbool Sys_Clipboard_Set_Image_BGRA_Is_Ok (const unsigned *bgra, int width, int height)
{
	HBITMAP hBitmap = CreateBitmap (width, height, 1, BGRA_BPP_32, bgra);

	OpenClipboard (NULL);

	if (EmptyClipboard()) {
		if ((SetClipboardData (CF_BITMAP, hBitmap)) == NULL) {
			//logd ("SetClipboardData failed"); // Was fatal.  But for clipboard?  Seriously?
			return false;
		}
	}

	CloseClipboard ();
	return true;
}

qbool Sys_Clipboard_Set_Image_Alpha_BGRA_Is_Ok (const unsigned *bgra, int width, int height)
{
	byte *alphaimage_za = Image_Alpha_Image_Za ((byte *)bgra, width, height);
	int is_ok_clip = Sys_Clipboard_Set_Image_BGRA_Is_Ok ((bgra4 *)alphaimage_za, width, height);
	Mem_FreeNull_ (alphaimage_za);
	return is_ok_clip != 0;
}

#else // LINUX ETC ... 
qbool Sys_Clipboard_Set_Image_BGRA_Is_Ok (const unsigned *bgra, int width, int height)
{
	return false;
}
#endif //  _WIN32

CALLERS_ (Sys_ConsoleInput_WinQuake)
char *Clipboard_Get_Text_Line_Static (void) // For WinQuake -dedicated only
{
	static char out[SYSTEM_STRING_SIZE_1024] = {0};
	char *cliptext_a = Sys_Clipboard_Get_Text_ZAlloc();
	if (cliptext_a) {
		c_strlcpy (out, cliptext_a);
		Mem_FreeNull_ (cliptext_a);
		String_Edit_To_Single_Line (out); // spaces < 32 except for newline, cr, backspace which it kills.
	}

	return out;
}
