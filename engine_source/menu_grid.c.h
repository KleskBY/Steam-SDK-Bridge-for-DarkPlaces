// menu_grid.c.h

//typedef struct {
//	int		rows; //
//	int		cols;
//	int		numblocktypes; //
//	frect	pct[512]; // 22 * 22
//	frect	pix[512];
//	int		val[512];
//	frect	backer;
//	frect	starter;
//} mgrid_s;

mgrid_s _mg, *mg = &_mg;

void M_Grid_ReInit (void)
{
	mg->cols = 20;
	mg->rows = 13;
	mg->numblocktypes = 2; // valid are 0 and 1

	float xx = .10; // Margin
	float yy = .05;
	float canvasw = 1.0 - xx - 0.3; // Allocate 30 %
	//float canvash = 1.0 - yy * 2.0;

	float ww = canvasw / mg->cols;//(1.0 - xx * 2.0)/mg->cols; // Width of a cell

	float canvash = canvasw;//ww;//vid_conheight.value / vid_conwidth.value * canvasw;
	float hh = canvash / mg->rows;
	int idx = 0;
	for (int r = 0; r < mg->rows; r ++) {
		for (int c = 0; c < mg->cols; c ++, idx ++) {
			//int idx = r * mg->cols + c;
			frect *pct = &mg->pct[idx];
			frect *pix = &mg->pix[idx];
			pct->left	= xx + ww * c + 1/16.0 * ww;
			pct->top	= yy + hh * r + 1/16.0 * hh;
			pct->width	= ww * (1.0-2/16.0);
			pct->height	= hh * (1.0-2/16.0);

			pix->left = pct->left * vid_conwidth.value;
			pix->top = pct->top * vid_conheight.value;
			pix->width = pct->width * vid_conwidth.value;
			pix->height = pct->height * vid_conheight.value;

			mg->val[idx] = 0;//Math_IsOdd(idx);
			//M_DrawPicFrac ("gfx/help0", fx,fy,fw,fh, /*count_0*/ 0);
		}
	}
	mg->backer.left = xx;
	mg->backer.top = yy;
	mg->backer.width = canvasw;
	mg->backer.height = canvash;

	mg->backer.width *= vid_conwidth.integer;
	mg->backer.height *= vid_conheight.integer;
	mg->backer.left *= vid_conwidth.integer;
	mg->backer.top *= vid_conheight.integer;

	mg->starter.left = xx + canvasw + ww;
	mg->starter.top = yy;
	mg->starter.width = ww;
	mg->starter.height = hh;

	mg->starter.width *= vid_conwidth.integer;
	mg->starter.height *= vid_conheight.integer;
	mg->starter.left *= vid_conwidth.integer;
	mg->starter.top *= vid_conheight.integer;



}

void M_Menu_Grid_f(cmd_state_t *cmd)
{
	KeyDest_Set (key_menu); // key_dest = key_menu;
	menu_state_set_nova (m_grid_31);
	m_entersound = true;
	M_Grid_ReInit ();
}


RELATED_ (M_ServerList_Draw FS_Dungeon_f)
// What pics?
// Change how?
// Save how?
//void Draw_Fill (const vec3_t pos, const vec3_t size, const vec3_t rgb, float alpha, float flags)
// WHITE_TEXTURE_NULL
//RELATED_ (M_DrawPic)
//	DrawQ_Pic(menu_x + cx, menu_y + cy, pico, /*w h:*/ 0, 0, q_rgba_solid_white_4_parms, DRAWFLAG_NORMAL_0);


static void M_Grid_Draw (void)
{
	vec3_t colorWhite = {1,1,1};
	vec3_t colorRed = {1,0,0};
	vec3_t colorBlack = {0,0,0};
	vec3_t colorGreen = {0,1,0};
	cachepic_t *picWhite = Draw_CachePic("white");

	M_Background(vid_conwidth.integer, vid_conheight.integer, q_darken_false);
	//M_DrawPic (0, 0, va(vabuf, sizeof(vabuf), "gfx/help%d", help_page), HOTSPOTS_NONE_0, NA0, NA0);

	M_DrawPicEx (picWhite, RECT_SEND(mg->backer), colorBlack, HOTSPOTS_NONE_0);

	M_DrawPicEx (picWhite, RECT_SEND(mg->starter), colorGreen, HOTSPOTS_1);

	int idx = 0;
	for (int r = 0; r < mg->rows; r ++) {
		for (int c = 0; c < mg->cols; c ++, idx ++) {
			//frect *pct = &mg->pct[idx];
			frect *pix = &mg->pix[idx];

			float *cc = &colorWhite[0];
			if (mg->val[idx])
				cc = &colorRed[0];

			M_DrawPicEx (picWhite, pix->left, pix->top, pix->width, pix->height, cc, HOTSPOTS_1);
		} // c
	} // r
}

RELATED_ (M_Main_Key)
// hotspot_count is reset before draw ...


static void M_Grid_Key(cmd_state_t *cmd, int key, int ascii)
{
	switch (key) {
	case K_ESCAPE: case K_MOUSE2:
		KeyDest_Set (key_game); // key_dest = key_game;
		menu_state_set_nova (m_none);
		break;

	case K_MOUSE1:
		if (hotspotx_hover == not_found_neg1)
			break;

		if (hotspotx_hover == 0) {
			S_LocalSound ("sound/misc/menu1.wav");
			RELATED_ (FS_Dungeon_f)
			Cbuf_AddTextLine (cmd, "dungeon mapcompilegrid test1");
			break;
		}

		// Hit a hotspot .. assume idx
		{
		int idx = hotspotx_hover - 1;
		mg->val[idx] = !mg->val[idx];
		}
		break;


	case K_HOME:
		//local_cursor = 0;
		break;

	case K_END:
		//local_cursor = local_count - 1;
		break;
	} // sw

}

