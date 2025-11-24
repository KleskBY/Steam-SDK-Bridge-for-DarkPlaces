// common_image.c.h

#define B	bgra4_data[y + 0]
#define G	bgra4_data[y + 1]
#define R	bgra4_data[y + 2]
//int lum = data_bgra[y + 0] + data_bgra[y + 1] + data_bgra[y + 2];
//int luma = ((r * 2) + (g * 5) + (b * 1)) / 8;


void Image_Alpha_From_Luminance_LT_X_BGRA (byte *bgra4_data, int w, int h, int X)
{
	int numpels = w * h * BGRA_4;
	for (int y = 0; y < numpels; y += BGRA_4) {
		byte luma = (( R << 1) + R + (G << 2) + B) >> 3;  // unsigned char >> is div 8 fast.
		if (luma < X)
			bgra4_data[y + 3] = luma;
		else bgra4_data[y + 3] = 255;
	} // for
}

void Image_Alpha_From_Luminance_LT_X_Green_0_BGRA (byte *bgra4_data, int w, int h, int X)
{
	int numpels = w * h * BGRA_4;
	for (int y = 0; y < numpels; y += BGRA_4) {
		if (G > 240 && G > R && G > B) {
			B = G = R = bgra4_data[y + 3] = 0; // GREEN - Ignore .. alpha = 0%
			continue;
		}
		byte luma = (( R << 1) + R + (G << 2) + B) >> 3;  // unsigned char >> is div 8 fast.
		if (luma < X)
			bgra4_data[y + 3] = luma;
		else bgra4_data[y + 3] = 255;
	} // for
}

void Image_Alpha_From_Luminance_LT_X_Ignore_RGB_Provided_Tolerance_0_BGRA (byte *bgra4_data, int w, int h, int X, int r, int g, int b, int tolerance)
{
	int numpels = w * h * BGRA_4;
	for (int y = 0; y < numpels; y += BGRA_4) {
		if (in_range(g - tolerance, G, g + tolerance) && in_range(r - tolerance, R, r + tolerance) && in_range(b - tolerance, B, b + tolerance)) {//  == g && R == r && B == b) {
			B = G = R = bgra4_data[y + 3] = 0; // GREEN - Ignore .. alpha = 0%
			continue;
		}
		int did_fix = false;
		if (G > R && G > B) {
			// Spread the difference
			int overage = G - R;
			int new_red = (int)R + overage;
			int over255 = new_red - 255;
			G = R;
			//if (overage > 2)
				did_fix = true; //255 - G;
			if (over255 > 0) {
				int new_blue = (int)B + over255;
				if (new_blue >255)new_blue = 255;
				R = 255;
				B = new_blue;
			} else {
				R = new_red;
			}
			
			// B = G = R = bgra4_data[y + 3] = 0; // GREEN - Ignore .. alpha = 0%
			//continue;
		}
		byte luma = (( R << 1) + R + (G << 2) + B) >> 3;  // unsigned char >> is div 8 fast.
		if (luma < X)
			bgra4_data[y + 3] = luma;
		else bgra4_data[y + 3] = 255;

		//if (did_fix)
		//	bgra4_data[y + 3] = bgra4_data[y + 3] / 2;
	} // for
}

void Image_Alpha_From_Luminance_BGRA (byte *bgra4_data, int w, int h)
{
	int numpels = w * h * BGRA_4;
	for (int y = 0; y < numpels; y += BGRA_4) {
		byte luma = (( R << 1) + R + (G << 2) + B) >> 3;  // unsigned char >> is div 8 fast.
		bgra4_data[y + 3] = luma;
	} // for
}

void Image_Alpha_Black_0_Other_1_BGRA (byte *bgra4_data, int w, int h)
{
	int numpels = w * h * BGRA_4;
	for (int y = 0; y < numpels; y += BGRA_4) {
		if (R == 0 && G == 0 && B == 0)
			bgra4_data[y + 3] = 0;
		else
			bgra4_data[y + 3] = 255;
	} // for

}

// Green = 0, non-green = luminance.  Green of 255 + B and R are both <= 64 is green.
void Image_Alpha_Green_0_Other_Luminance_BGRA (byte *bgra4_data, int w, int h)
{
	int numpels = w * h * BGRA_4;
	for (int y = 0; y < numpels; y += BGRA_4) {
		if (G == 255) {
			if (B <= 64 || R <= 64) {
				B = 0;
				G = 0;
				R = 0;
				bgra4_data[y + 3] = 0; // GREEN - Ignore .. alpha = 0%
				continue;
			}
		}

		byte luma = (( R << 1) + R + (G << 2) + B) >> 3;  // unsigned char >> is div 8 fast.

		bgra4_data[y + 3] = luma;
	} // for

}

// Green = 0, non-green = 1
void Image_Alpha_Green_0_Other_1_BGRA (byte *bgra4_data, int w, int h)
{
	int numpels = w * h * BGRA_4;
	for (int y = 0; y < numpels; y += BGRA_4) {
		if (G == 255) {
			if (B <= 64 || R <= 64) {
				B = 0;
				G = 0;
				R = 0;
				bgra4_data[y + 3] = 0; // GREEN - Ignore .. alpha = 0%
				continue;
			}
		}

		bgra4_data[y + 3] = 255; // Not green.  Alpha = 1
	} // for
}


qbool Image_Has_Alpha (const byte *bgra4_data, int w, int h)
{
	int has_zero = 0;
	int has_full = 0;
	int numpels = w * h * BGRA_4;
	for (int y = 3; y < numpels; y += BGRA_4) {
		if (!has_full && bgra4_data[y] == 255) {
			has_full = true;
			if (has_zero)
				return true;
		}

		if (!has_zero && bgra4_data[y] == 0) {
			has_zero = true;
			if (has_full)
				return true;
		}

		if (in_range (1, bgra4_data[y], 254))
			return true; // HAS ALPHA CHANNEL


		// Found one!

	} // for
	return false;
}

byte *Image_Alpha_Image_Za (byte *bgra4_data, int w, int h)
{
	fs_offset_t sizealloc = (w * h * BGRA_4);
	byte *temp_za = (byte *)Mem_Alloc (zonemempool, sizealloc);

	for (int y = 0; y < sizealloc; y += BGRA_4) {
		byte alpha = bgra4_data[y + 3];
		temp_za[y + 0] = temp_za[y + 1] = temp_za[y + 2] = alpha;
		temp_za[y + 3] = 255;
	}

	return temp_za;
}

bgra4 *Image_Bilinear_Resize_ZAlloc (const bgra4 *rgba, int width, int height, int new_width, int new_height)
{
	size_t sizealloc = sizeof(unsigned int) * (new_width * new_height);
    /*RETURNING_ALLOC___*/ bgra4 *temp_o = (bgra4 *)Mem_Alloc (zonemempool, sizealloc);
    int a, b, col, d, x, y, index, i, j ;
    float x_ratio = ((float)(width-1))/new_width ;
    float y_ratio = ((float)(height-1))/new_height ;
    float x_diff, y_diff, blue, red, green,alpha;
    int offset = 0 ;
    for (i=0;i<new_height;i++) {
        for (j=0;j<new_width;j++) {
            x = (int)(x_ratio * j) ;
            y = (int)(y_ratio * i) ;
            x_diff = (x_ratio * j) - x ;
            y_diff = (y_ratio * i) - y ;
            index = (y*width+x) ;
            a = rgba[index] ;
            b = rgba[index+1] ;
            col = rgba[index+width] ;
            d = rgba[index+width+1] ;

            // blue element
            // Yb = Ab(1-width)(1-height) + Bb(width)(1-height) + Cb(height)(1-width) + Db(wh)
            blue = (a&0xff)*(1-x_diff)*(1-y_diff) + (b&0xff)*(x_diff)*(1-y_diff) +
                   (col&0xff)*(y_diff)*(1-x_diff)   + (d&0xff)*(x_diff*y_diff);

            // green element
            // Yg = Ag(1-width)(1-height) + Bg(width)(1-height) + Cg(height)(1-width) + Dg(wh)
            green = ((a>>8)&0xff)*(1-x_diff)*(1-y_diff) + ((b>>8)&0xff)*(x_diff)*(1-y_diff) +
                    ((col>>8)&0xff)*(y_diff)*(1-x_diff)   + ((d>>8)&0xff)*(x_diff*y_diff);

            // red element
            // Yr = Ar(1-width)(1-height) + Br(width)(1-height) + Cr(height)(1-width) + Dr(wh)
            red = ((a>>16)&0xff)*(1-x_diff)*(1-y_diff) + ((b>>16)&0xff)*(x_diff)*(1-y_diff) +
                  ((col>>16)&0xff)*(y_diff)*(1-x_diff)   + ((d>>16)&0xff)*(x_diff*y_diff);

            alpha = ((a>>24)&0xff)*(1-x_diff)*(1-y_diff) + ((b>>24)&0xff)*(x_diff)*(1-y_diff) +
                  ((col>>24)&0xff)*(y_diff)*(1-x_diff)   + ((d>>24)&0xff)*(x_diff*y_diff);

            temp_o[offset++] =
                    //0xff000000 | // hardcode alpha
					((((int)alpha)<<24)&0xff000000) |
                    ((((int)red)<<16)&0xff0000) |
                    ((((int)green)<<8)&0xff00) |
                    ((int)blue) ;
        }
    }
    RETURNING___ return temp_o ;
}

// Draw Quad = true if ok, false if not
int Image_Rect_Fill3 (void *pels, unsigned rowbytes, int x, int y, int paint_width, int paint_height, int pixel_bytes, unsigned fill_color)
{
	byte *buf = (byte *)pels;
//	int rowbytes = pix_width * pixel_bytes;
	int startoffset = y * rowbytes + x * pixel_bytes;
	int i, j, bufoffset;
	int pix_width = rowbytes / pixel_bytes; // Because we reinterpret the buffer as short or unsigned for first row paint
	// Non-byte pixels must fill the first row immediately

#ifdef _DEBUG
	if (paint_width < 0 || paint_height < 0) {
		Con_PrintLinef ("Negative size");
		return false;
	}
#endif

	switch (pixel_bytes)
	{
	case 2: // Short
		for (i = 0; i < paint_width; i ++)
			((short *)buf)[y * pix_width + x + i] = (short)fill_color; // Fill first row
		break;

	case RGBA_4: // 4, unsigned
		for (i = 0; i < paint_width; i ++)
			((unsigned *)buf)[y * pix_width + x + i] = fill_color; // Fill first row
		break;

	default: // Hopefully default is 1
		break;
	}

	for (j = 0, bufoffset = startoffset; j < paint_height; y ++, j++, bufoffset += rowbytes)
	{
		// Single byte pixels: memset.  multi-byte pixels: skip first row we already filled, then memcpy it
		if (pixel_bytes == 1) memset (&buf[bufoffset], fill_color, paint_width); // byte pixels just memset
		else if (j > 0) memcpy (&buf[bufoffset], &buf[startoffset], paint_width * pixel_bytes);
	}

	return true;
}



void sImage_Format_Buffer (byte *buf, int w, int h, int pixel_bytes, unsigned fillcolor)
{
//	if (pixel_bytes == RGBA_4) Image_Rect_Fill (fillcolor,0,0,w,h,buf,w,h,pixel_bytes);  // unsigned
	if (pixel_bytes == RGBA_4) {
		size_t rowbytes = w * RGBA_4;
		Image_Rect_Fill3 (buf, rowbytes, 0, 0, w, h, pixel_bytes, fillcolor);  // unsigned
			//if (pixel_bytes == RGBA_4) Image_Rect_Fill3 (buf,w*h*pixel_bytes,0,0,w,h,pixel_bytes,fillcolor);  // unsigned
	}
	else if (pixel_bytes == 1) memset (buf, fillcolor, w * h);
	else {
		// log_fatal ("Invalid pixel_bytes"); // We can actually handle 2 above using same text as if (pixel_bytes == RGBA_4)
	}
}

// Literal, if you want to clamp src/dst coords and width, do it in another function
void Image_Paste_Sub_Image (void *dst, unsigned dst_rowbytes, int dst_x, int dst_y,
   const void *src, unsigned src_rowbytes, int src_x, int src_y, 
   int paste_width, int paste_height, int pixel_bytes)
{
	int r;
	byte *bdst = (byte *)dst;
	byte *bsrc = (byte *)src;

	size_t paste_rowbytes = paste_width * pixel_bytes; // Amount to copy per row

	int dst_offset = dst_y * dst_rowbytes + dst_x * pixel_bytes;
	int src_offset = src_y * src_rowbytes + src_x * pixel_bytes;

	for (r = 0; r < paste_height; r++, dst_offset += dst_rowbytes, src_offset += src_rowbytes)
		memcpy (&bdst[dst_offset], &bsrc[src_offset], paste_rowbytes);
}

RELATED_ (Image_Flip_Buffer Image_Flip_RedGreen)

/*ALLOC___ */void *Image_Enlarge_Canvas_ZAlloc (const void *pels, int width, int height, int pixel_bytes, int new_width, int new_height, unsigned fillcolor, int is_centered)
{
	/*RETURNING_ALLOC___ */byte *temp_o = (byte *)Mem_Alloc (zonemempool, pixel_bytes * new_width * new_height);

	if (fillcolor != 0) sImage_Format_Buffer (temp_o, new_width, new_height, pixel_bytes, fillcolor);

	if (is_centered)
	{
		#define c_rint(x)	((x) > 0 ? (int)((x) + 0.5) : (int)((x) - 0.5))
		int center_x = c_rint ((new_width - width) / 2.0);
		int center_y = c_rint ((new_height - height) / 2.0);
		Image_Paste_Sub_Image (temp_o, new_width * pixel_bytes, center_x, center_y, pels, width * pixel_bytes, 0, 0, width, height, pixel_bytes);
		#undef c_rint
	}
	else Image_Paste_Sub_Image (temp_o, new_width * pixel_bytes, 0, 0, pels, width * pixel_bytes, 0, 0, width, height, pixel_bytes);

	RETURNING___ return (void *)temp_o;
}


qbool JPEG_SaveImage_preflipped (const char *filename, int width, int height, unsigned char *data);
void Image_CopyMux(unsigned char *outpixels, const unsigned char *inpixels, int inputwidth, int inputheight, qbool inputflipx, qbool inputflipy, qbool inputflipdiagonal, int numoutputcomponents, int numinputcomponents, int *outputinputcomponentindices);

qbool Image_Save_JPEG_Is_Ok (ccs *filename, bgra4 *pels_bgra, int width, int height)
{
	rgb3 *noalphabuffer_3 = (rgb3 *)Mem_Alloc (tempmempool, width * height * RGB_3);
	int	indices[4] = {2,1,0,3}; // BGRA
	Image_CopyMux (noalphabuffer_3, (byte *)pels_bgra, width, height,
		/*flipx*/ false, /*flipy*/ true, /*flipdiagonal*/ false, 3, 4, indices);

	int is_ok = JPEG_SaveImage_preflipped (filename, width, height, noalphabuffer_3);

	Mem_FreeNull_ (noalphabuffer_3);
	return is_ok;
}

