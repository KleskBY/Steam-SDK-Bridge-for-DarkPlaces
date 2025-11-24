// common_image.h



///////////////////////////////////////////////////////////////////////////////
//  IMAGE: OPERATIONS: Baker - Image
///////////////////////////////////////////////////////////////////////////////

void Image_Flip_Buffer (void *pels, int columns, int rows, int bytes_per_pixel);
void Image_Flip_RedGreen (void *rgba, size_t numbytes);


qbool Image_Has_Alpha (const byte *bgra4_data, int w, int h);
byte *Image_Alpha_Image_Za (byte *bgra4_data, int w, int h);

void Image_Alpha_From_Luminance_BGRA (byte *bgra4_data, int w, int h);
void Image_Alpha_Black_0_Other_1_BGRA (byte *bgra4_data, int w, int h);
void Image_Alpha_Green_0_Other_Luminance_BGRA (byte *bgra4_data, int w, int h);
void Image_Alpha_Green_0_Other_1_BGRA (byte *bgra4_data, int w, int h);
void Image_Alpha_From_Luminance_LT_X_BGRA (byte *bgra4_data, int w, int h, int X);
void Image_Alpha_From_Luminance_LT_X_Green_0_BGRA (byte *bgra4_data, int w, int h, int X);
void Image_Alpha_From_Luminance_LT_X_Ignore_RGB_Provided_Tolerance_0_BGRA (byte *bgra4_data, int w, int h, int X, int r, int g, int b, int tolerance);

bgra4 *Image_Bilinear_Resize_ZAlloc (const bgra4 *rgba, int width, int height, int new_width, int new_height);
int Image_Rect_Fill3 (void *pels, unsigned rowbytes, int x, int y, int paint_width, int paint_height, int pixel_bytes, unsigned fill_color);
void *Image_Enlarge_Canvas_ZAlloc (const void *pels, int width, int height, int pixel_bytes, int new_width, int new_height, unsigned fillcolor, int is_centered);
qbool Image_Save_JPEG_Is_Ok (ccs *filename, bgra4 *pels_bgra, int width, int height);

void Image_Paste_Sub_Image (void *dst, unsigned dst_rowbytes, int dst_x, int dst_y,
			   const void *src, unsigned src_rowbytes,
			   int src_x, int src_y, int paste_width, int paste_height, int pixel_bytes);


