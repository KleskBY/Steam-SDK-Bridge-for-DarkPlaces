// oject_1_const_enums.c.h


// Directives ...

#define CLIPPING_ON 						(k->fctrl->frm.disable_clipping == false)
#define DO_PRINT							1		// Debug print of property parser

#define LV_TEXTMARGIN_10					10
#define LV_HEADERROW_ADD_6					6
#define LV_DEFAULT_WIDTH_100				100
#define LV_MIN_WIDTH_25						25
#define LV_TEXTMARGIN_1						1		// Subtracted from right clip area
#define LV_DRAG_COLSIZE_10					10

#define DIR_BACKWARDS_N1					-1
#define DIR_FORWARD_1						1

#define	OK_NOERROR_NULL						NULL
#define	OK_NOERROR_0						0
#define	IERR_PROP_NOT_FOUND_1				1
#define IERR_PROP_SET_ERROR_2				2

#define	TXT_CURSOR_WIDTH_4					4
#define TXT_BORDER_WIDTH_2					2
#define TXT_MARGIN_4						4
#define TXT_SCROLL_HORZ_AMOUNT_0_25			0.25	// Percent of textarea

#define	DDOT_SIZE_8							8		// 
//#define DOT_GAP_0							0

#define LENGTH_0							0

// General ...
#define CREATE_NULL							NULL
#define FORM_NULL							NULL
#define FORM_KCAN_NULL						NULL	// Form have no container.
#define SERVANT_KCAN_NULL					NULL	// Servants do not have a container, 
													// draw with owner as a zebra (Z-Ordered Top).
#define	SERVANT_TO_NULL						NULL

#define BOXWIDTH_2							2
#define RECT_CULLED_ZERO					0
#define SCROLLBAR_HOOK_REFIRE_0_1			0.1
#define	KNOB_SIZE_16						16
#define WHITE_TEXTURE_NULL					NULL

#define BUTTON_PRESS_TIME_0_3				0.3		// Amount of time for button press appearance.

RELATED_ (DOUBLE_CLICK_0_5) // Windows double-click time

typedef enum {
	DUMP_DETAIL_SAVE_FILE_0 =				0,		// Save file, no servants "ui_set no args"
													// vid_restart and form state to clipboard.
	DUMP_DETAIL_PLUS_SERVANTS_1 =			1,		
	DUMP_DETAIL_PLUS_CLIPPING_2 = 			2,
	DUMP_DETAIL_NAMES_ONLY_3 = 				3,		// Used to print a list
	DUMP_DETAIL_UNCONTAINED_4 = 			4,		// "uncontained" uses.
	DUMP_DETAIL_SINGLE_LINE_5 = 			5,		// 
	DUMP_DETAIL_NORECURSE_6 = 				6,		// USED.  To dump a single control. ui_set.
} dump_detail_e;


#define POLYGON_2D_2						2
#define POLYGON_2D_FLOAT_SEPARATOR_SPACE	" "
#define POLYGON_2D_ELEMENT_SEPARATOR_COMMA	","
#define COLUMN_SEPARATOR_COMMA				","
#define POLYGON_WHITE_TEXTURE_NULL			NULL	// Polygon draw



#define PROPERTY_COLON_DELIM				":"		// Properties colon delimiter


#define OFCAN_FOCUS_2						2		// A things flag
#define OFCAN_MOUSE_4						4		// A things flag
#define OFCAN_CONTAIN_8						8		// A things flag
#define OFCAN_RUNTIME_ONLY_NO_SAVE_16		16		// Like servant to what
#define OFCAN_AUTOSIZE_32					32		// Can specify its own size
#define OFCAN_NOHIGHLIGHT_64				64		// Does not draw highlight
#define OFCAN_ONLYFOCUS_128					128		// Context Menu - disappears on lost focus
													// replies cancel

#define	isbuttonaction_true					true
#define isbuttonaction_false				false
#define shallrecurse_true					true
#define shallrecurse_false					false

//

#define CLIPPED_2							2
#define CULLED_1							1
#define UNCULLED_0							0

#define TRANSPARENT_FALSE					false
#define OUTSET_TRUE							true
#define OUTSET_FALSE						false
#define BORDER_TRUE							true
#define THREED_TRUE							true
#define THIN_TRUE							true
#define THIN_FALSE							false

#define	q_indent_true						true
#define	q_indent_false						false
#define q_sethook_true						true
#define q_sethook_false						false

#define	isdone_true							true
#define	isdone_false						false

#define	isdown_true							true
#define	isdown_false						false

// 
#define SB_ACTION_UP_0						0
#define SB_ACTION_DOWN_1					1
#define SB_ACTION_PGUP_2					2
#define SB_ACTION_PGDOWN_3					3
#define SB_ACTION_THUMB_4					4

//
#define MOUSEUP_0							0
#define MOUSEDOWN_1							1

// 
#define CM_DOTCOUNT_COUNT_9					9
#define CM_MIDDLE_DOT_4						4
#define	CM_SERIES_COUNT_4					4
#define	CM_0_TEXT_0							0
#define	CM_1_CHECKED_1						1	// Checked .. for value use string
#define	CM_2_STRING_ID_2					2	// String id
#define	CM_3_FUNCSTRING_3					3

#define EVENT_VALSTR_NULL				 NULL

// "Things" 
typedef enum {
	thing_none_0, 

	class_start,
	class_form_1 = 1,	// Form must be 1 and the first property.  Hardcoded elsewhere.
	
	// This should be in alphabetical order.  Values mean nothing.
	class_button,		// TODO: January 12 2025
	class_contextmenu,
	class_checkbox,		// TODO: January 12 2025
	class_gridview,
	class_image,
	class_imagegrid,
	class_label, 
	class_listbox, 
	class_listview, 
	class_polygon,		// This should be in alphabetical order.  Values mean nothing.
	class_rectangle, 
	class_scrollbar,
	class_sizedot,
	class_tabselect, 
	class_textbox, 
	
	class_end,

	prop_start,
	prop_name = 100, 
	prop_container = 101,
	prop_rect = 102,

// Sort these
	
	prop_alignment, 
	prop_appearance,
	prop_autosize,
	prop_backalpha,
	prop_backcolor,
	prop_backcolorselected,
	prop_backcolorfocusselected,
	prop_caption,
	prop_cellpaddingxpct,
	prop_cellpaddingypct,
	prop_cellspacingypct,
	prop_columncount,
	prop_columnheaders,
	prop_columnwidths,
	prop_didconsumekey,
	prop_fontnameindex,
	prop_fontdescendpct,
	prop_fontsize,
	prop_forceminimumwidth,
	prop_forealpha,
	prop_forecolor,
	prop_forecolorselected,
	prop_gridcellbackcolor,
	prop_gridcellheight,
	prop_gridcellpadding,
	prop_gridcellspacing,
	prop_gridcellwidth,
	prop_gridcolumns,
	prop_gridmargin,
	//prop_gridimagelist,
	prop_gridrows,
	prop_gridvalues,
	prop_filelist,
	prop_flatcolor,
	prop_focusstyle,
	prop_imagealpha,
	prop_imagecolor,
	prop_imagelist,
	prop_imagename, 
	prop_iscutout,
	prop_isdisabled,
	prop_ishidden, 
	prop_ishorizontal, 
	prop_iskeypreview,
	prop_isoutlined, 
	prop_isrounded, 
	prop_list,
	prop_maxvalue,
	//prop_rowscount,
	prop_servantowner,
	prop_selectedindex,
	prop_scrollbarcolor,
	prop_scrollbarthumbcolor,
	prop_text,
	prop_value,
	prop_wasinitialized,
	prop_vertexes,
	prop_userdata,
	prop_userdata2,
	prop_userdata3,
	prop_userstring,
	prop_userstring2,
	prop_userstring3,

// Events are sort of like properties actually.
// January 18 2025: However, these are rather useless ... we need to manually do them
// And these don't really mean anything and are entirely unused for CSQC interface
	events_start,
	events_OnLoad, 
	events_OnKeyPreview, 
	events_OnChange,
	events_end,

	prop_end,

	RELATED_ (Object_Get_ControlIndex)
	demiprop_controls,			// Access frm.controls_a like a list and get the object name 
	demiprop_controlindex,		// Runs Object_Get_ControlIndex and finds the current index.  This actually can change (Zorder).
	demiprop_controlcount,		// Form. Returns f->frm.controls_a->numitems
	demiprop_controltype,		// Returns k->po->name like "ListView"
	demiprop_filelistcount,		// FileListCount -- retrieves file list num strings
	demiprop_focused,			// Form. Returns f->frm.kfocused->cm.name_a or f->cm.name_a
	demiprop_handle,			// Returns f->ui.csqc_handle_plus1
	demiprop_imagelistcount,	// ImageListCount -- retrieves image list num strings
	demiprop_key,				// Form "Key"/"Value" system treats List as 2 column array.
	demiprop_listcount,			// ListCount -- retrieves list num strings
	demiprop_selectedrow,		// Alias for SelectedIndex, particularly for ListView because multi-column

	meth_start,
	
	// meth_cmd = no arguments - Like properties, methods are caseless.

	meth_cmd_filelistclear,
	meth_cmd_imagelistclear,
	meth_cmd_listclear,
	
	
	
	meth_cmd_setfocus,

	meth_additem,				// item string
	meth_addimage,				// image string
	meth_addfile,				// file string
//	meth_clipboardsetimage,		// Form method?  Set
	meth_clipboardsettext,		// Form method?  Set
	meth_popup,					// arg becomes userstring like "data/myfilename.file"
	meth_listappendfilelines,	// filename to read.  Busts the 16384 limit to a listview.
	//meth_textwrap,				// Not implemented yet.

	meth_end,

	// Events QuakeC is alerted to ...
	evx_sendhandle,
	evx_initializedwithmessage,
	evx_click,
	evx_doubleclick,
	evx_onchange,
	evx_keypreview,
	evx_popupclosed,

// These things are special funtions we know
	builtin_func,
} things_e;

RELATED_ (things oject_s)

typedef enum _vartype_e {
	vtstring_1			= 1,	// ev_string_1
	vtfloat_2			= 2,	// ev_float_2
	mrectf_10			= 10,	// float rect (assumed metric) - NOT USED JAN 10 2025
	mrecti_11			= 11,	// float rect (RECT)
	vtinteger_19		= 19,	// Best used for boolean, enumerations
	vtlist_str_20		= 20,	// list of strings
	vtrgb_21			= 21,	// vec3_t rgb triplet of floats 0.0 to 1.0 (maybe higher too? up to 8?)
	mvertflist_22		= 22,	// List of polygon vertex
	vtcontrolref_23		= 23,	// List of integers
	vtint32list_24		= 24,	// NO CURRENT USERS Jan 10 2025

	mfloat_102			= 102,	// metric float .. FontSize
	minteger_119		= 119,	// Metric integer .. Width (Who uses this) NO ONE YET January 10 2025
	mint32list_124		= 124,	// Column sizes

// Enum-likes - Only fontnameindex writes "No Number Suffix Enums" the rest do ... for now
	vtappearance_200	= 200,	// normal_0 flat_1 (form only?)
	vtfontnameindex_201 = 201,	// (-1)system (0)default_0 console sbar notify chat centerprint infobar menu user0 ...
	vtfocusstyle_202	= 202,	// normal_0 nohighlight_1 (form only)

	vtevent_100			= 100,	// list of strings
} vartype_e;

RELATED_ (Label_Rect_Edit_Align XY_Set_Dot)
typedef enum {
	//ENUM_FORCE_INT_GCC_ (alignment)
	alignment_top_left_0			= 0,
	alignment_top_center_1			= 1,
	alignment_top_right_2			= 2,
	alignment_middle_left_3			= 3,
	alignment_middle_center_4		= 4,
	alignment_middle_right_5		= 5,
	alignment_bottom_left_6			= 6,
	alignment_bottom_center_7		= 7,
	alignment_bottom_right_8		= 8,
} alignment_e;

typedef enum {
	border_Wall_none_0				= 0,
	border_Wall_left_1				= 1,
	border_Wall_top_2				= 2,
	border_Wall_right_4				= 4,  
	border_Wall_topless_1_4_8_13	= 13,
	border_Wall_bottomless_1_2_4_7	= 7,
	border_Wall_bottom_8			= 8,
} border_wall_e;


typedef enum {
	drawbut_thin_inset_0			= 0,
	drawbut_thin_outset_1			= 1,
	drawbut_normal_inset_2			= 2,
	drawbut_normal_outset_3			= 3,
	drawbut_command_up_4			= 4,
	drawbut_command_pressed_5		= 5,
	drawbut_command_thick_up_6		= 6,
//	drawbut_command_thick_pressed_7 = 7,
} drawbut_e;

typedef enum {
	arrow4_left_0					= 0,
	arrow4_right_1					= 1,
	arrow4_up_2						= 2,
	arrow4_down_3					= 3,
} arrow4_e;

typedef enum {
	refresh_reason_normal_0			= 0,	// Size change or simple recalc
	refresh_reason_preautosize_1	= 1,	// Early refresh ask autosize control its size 
	refresh_reason_finalized_2		= 2,	// Single pass when a form is fully parsed
											// Chance to fill in "required" data that is missing
											// Like column widths for a listview
} refresh_reason_e;

COMPILE_TIME_ASSERT (refresh_reason_e, sizeof(refresh_reason_e) == 4); // Move along ...

typedef enum {
	roundtype_none_0				= 0,	// current behavior that does what?
	roundtype_0_50_0				= 1,	// out_x exits if width of ch / 2 passes
} roundtype_e;

typedef enum {
	appearance_default_0			= 0,
	appearance_flat_1				= 1,
} appearance_e;

typedef enum {
	fontnameindex_system_neg1		= -1,	// Force Roboto
	fontnameindex_default_0			= 0,	// You get Roboto
	fontnameindex_console_1			= 1,	// FONT_CONSOLE = 1
	fontnameindex_sbar_2			= 2,	// 'sbar', used on hud, must be fixed width
	fontnameindex_notify_3			= 3,    // 'notify', used on sprint/bprint
	fontnameindex_chat_4			= 4,	// 'chat'
	fontnameindex_centerprint_5		= 5,	// 'centerprint'
	fontnameindex_infobar_6			= 6,    // 'infobar'
	fontnameindex_menu_7			= 7,	// 'menu', should be fixed width
	fontnameindex_user0_8			= 8,
	fontnameindex_user1_9			= 9,
	fontnameindex_user2_10			= 10,
	fontnameindex_user3_11			= 11,
	fontnameindex_user4_12			= 12,
	fontnameindex_user5_13			= 13,
	fontnameindex_user6_14			= 14,
	fontnameindex_user7_15			= 15,
} fontnameindex_e;

typedef enum {
	focusstyle_default_0			= 0,
	focusstyle_nohighlight_1		= 1,
} focusstyle_e;




