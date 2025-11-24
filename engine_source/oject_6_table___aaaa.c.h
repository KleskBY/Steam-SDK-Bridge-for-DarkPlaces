// oject_6_table___aaaa.c.h


things_s things[] = {
	//typedef struct _things_s {
	//	char			*name;		// 1
	//	int				enum_id;	// 2
	//	union {
	//		int			vt;			// 3 Vartype
	//		int			oflags;		//   Can focus, can mouse flags.
	//	} w;
	//	size_t			moffsetof;	// 4 Field offset into the oject_s struct
	//	ccs				*sdefault;	// 5 
	//	ccs				*describe;	// 6 
	//	//parse_fn_t	parse_fn;	// 7 TODO
	//} things_st;

// On the startup of a form, these will be inherited by all controls.

#define THEME_DEFAULTS "ForeColor:#000" \
				    " BackColor:#fff"\
					" BackColorSelected:#000"\
					" ForeColorSelected:#fff" \
					" FontSize:24" \
					"" // Ender .. Jan 13 2025 - Changing Font Sizes affects devinfo a little
					//" ScrollbarColor:#fc3 ScrollbarThumbColor:#3cf"
					// February 12 2025 - BackAlpha is no longer a default.


//											VALIDATE FN?
	{ "", /*KEEP ME*/ },
	{"Form",				class_form_1,		0,							0, THEME_DEFAULTS	}, // This must be idx 1 NOT ZERO


RELATED_ (sdefault THEME_DEFAULTS Object_Default_Values)
	// THEME_DEFAULTS: ForeColor / BackColor / ForeColorDisabled / BackColorDisabled
	// We can do a string parse of the default string and if it matches, prevent writing/export
	RELATED_ (Object_Create_And_Assign)

#define TABSELECT_DEFAULTS "AutoSize:0 CellPaddingXPct:0.50 CellPaddingYPct:0.50" \
	" CellSpacingYPct:0.0625 FontDescendPct:1.0" // 0.0625 is 1/16

//#define BUTTON_DEFAULTS "ImageColor:#fff ImageAlpha:1 CellPaddingXPct:0.50 FontDescendPct:1.0"
//	"" // 0.0625 is 1/16

#define CONTEXTMENU_DEFAULTS TABSELECT_DEFAULTS

	// 1					2					3								4
	{"CheckBox",			class_checkbox,		OFCAN_FOCUS_2 | OFCAN_MOUSE_4, 0, ""	},
	{"Button",				class_button,		OFCAN_FOCUS_2 | OFCAN_MOUSE_4, 0, "ImageColor:#fff ImageAlpha:1 CellPaddingXPct:0.50 FontDescendPct:0.75" },
	{"ContextMenu",			class_contextmenu,	OFCAN_FOCUS_2 | OFCAN_MOUSE_4 | OFCAN_ONLYFOCUS_128, 0, CONTEXTMENU_DEFAULTS	},
	{"GridView",			class_gridview,		OFCAN_FOCUS_2 | OFCAN_MOUSE_4, 0, ""	},
	{"Image",				class_image,		OFCAN_MOUSE_4,				   0, "ImageColor:#fff ImageAlpha:1" },
	{"ImageGrid",			class_imagegrid,	OFCAN_FOCUS_2 | OFCAN_MOUSE_4, 0, 
		"BackAlpha:1 ImageAlpha:1 GridCellWidth:16 GridCellHeight:16 "
		"GridCellPadding:2 GridCellSpacing:2 GridMargin:2 GridRows:4 GridColumns:2", 
		"Will draw every cell gridcolumns * gridrows"
		 },
	{"Label",				class_label,		OFCAN_AUTOSIZE_32,			   0, "AutoSize:1"	},
	{"ListBox",				class_listbox,		OFCAN_FOCUS_2 | OFCAN_MOUSE_4, 0, 		},
	{"ListView",			class_listview,		OFCAN_FOCUS_2 | OFCAN_MOUSE_4, 0, 		},
	{"Polygon",				class_polygon,		0,							   0, },
	{"Rectangle",			class_rectangle,	0,							   0, "BackAlpha:1" },
	{"Scrollbar",			class_scrollbar,	OFCAN_MOUSE_4,				   0, "MaxValue:1"	},
	{"SizeDot",				class_sizedot,		OFCAN_MOUSE_4,				   0, "BackColor:#fff"},
	{"TabSelect",			class_tabselect,	OFCAN_FOCUS_2 | OFCAN_MOUSE_4 |

		OFCAN_AUTOSIZE_32 | OFCAN_NOHIGHLIGHT_64, 0, TABSELECT_DEFAULTS
			},
	{"TextBox",				class_textbox,		OFCAN_FOCUS_2 | OFCAN_MOUSE_4, 0, ""	},

	{"SlideContainer",											},
	{"PictureBox",												},	// Interactable
	{"Viewport",												},
	{"ToolTip",													},
	{"ComboBox",												},
	{"MenuBox",													},
	{"MenuPane",												},

//Polygon		vertexes:"20 20, 500 400, 550 450" forecolor:"RGB(255,255,0)"

//

	RELATED_ (oject_s)
//
	{"Name",				prop_name,				vtstring_1,		member_offsetof (form1, cm.name_a),		},
	{"Rect",				prop_rect,				mrecti_11,		member_offsetof (form1, cm.relative_rect),	},

#define IH // INHERITED MARKER for now.

// ORDER:
// Control is created.
// Form THEME defaults runs on control
// Class defaults runs on control
// Current values of form for inherited properties are COPIED to the object
//  This allows the form backcolor and such to be inherited to the control.

// Order should mirror

	{"Alignment",			prop_alignment,			vtinteger_19,	member_offsetof (form1, alignment),		},
	{"Appearance",			prop_appearance,		vtappearance_200,member_offsetof (form1, appearance),	0, "1 = Flat Form only"},
	{"AutoSize",			prop_autosize,			vtinteger_19,member_offsetof (form1, autosize),		},
	{"BackAlpha",			prop_backalpha,			vtfloat_2,		member_offsetof (form1, backalpha),		},
IH	{"BackColor",			prop_backcolor,			vtrgb_21,		member_offsetof (form1, backcolor),		},
IH	{"BackColorSelected",	prop_backcolorselected,	vtrgb_21,		member_offsetof (form1, backcolorselected),},
	{"BackColorFocusSelected",	prop_backcolorfocusselected,	vtrgb_21,		member_offsetof (form1, backcolorfocusselected),},

	{"FontDescendPct",		prop_fontdescendpct,	vtfloat_2,		member_offsetof (form1, fontdescendpct), 0, "Scale font descent (overhang) by this factor when calculating text sizing and positioning, used to avoid vertically uncentered looking text (clampf)"},
	{"ForceMinimumWidth",	prop_forceminimumwidth,	vtfloat_2,		member_offsetof (form1, forceminimumwidth),0,"If non-zero, fontsize times this number"},
	{"Caption",				prop_caption,			vtstring_1,		member_offsetof (form1, caption_a),		},
	{"CellPaddingXPct",		prop_cellpaddingxpct,	vtfloat_2,		member_offsetof (form1, cellpaddingxpct), 0, "X Button Padding as Percent of FontSize on each side" },
	{"CellPaddingYPct",		prop_cellpaddingypct,	vtfloat_2,		member_offsetof (form1, cellpaddingypct), 0, "Y Button Padding as Percent of FontSize on each side" },
	{"CellSpacingYPct",		prop_cellspacingypct,	vtfloat_2,		member_offsetof (form1, cellspacingypct), 0, "Y Cellspacing as percent of FontSize between each cell" },

	{"ColumnCount",			prop_columncount,		vtinteger_19,	member_offsetof (form1, columncount),	},
	{"ColumnHeaders",		prop_columnheaders,		vtlist_str_20,	member_offsetof (form1, columnheaders_a),},
	{"ColumnWidths",		prop_columnwidths,		mint32list_124,	member_offsetof (form1, columnwidths_a),},
IH  {"Container",			prop_container,			vtcontrolref_23,member_offsetof (form1, container),		},
    {"DidConsumeKey",		prop_didconsumekey,		vtinteger_19,	member_offsetof (form1, didconsumekey),	0, "See KeyPreview, set this in QuakeC if KeyPreview consumed the key"},
	{"FontName",			prop_fontnameindex,	vtfontnameindex_201,member_offsetof (form1, fontnameindex),	0, "0 = Roboto, 1 = loadfont user0, 2 = loadfont user1 .. 8 = user7 <-- that is wrong (Form specifies for all, can be overridden" },

	{"FlatColor",			prop_flatcolor,			vtrgb_21,		member_offsetof (form1, flatcolor), 0, "Flat Color for buttons"},	//
IH	{"FontSize",			prop_fontsize,			mfloat_102,		member_offsetof (form1, fontsize),		},
	{"ForeAlpha",			prop_forealpha,			vtfloat_2,		member_offsetof (form1, forealpha),		},
IH	{"ForeColor",			prop_forecolor,			vtrgb_21,		member_offsetof (form1, forecolor),		},
IH	{"ForeColorSelected",	prop_forecolorselected,	vtrgb_21,		member_offsetof (form1, forecolorselected),},
	{"GridCellHeight",		prop_gridcellheight,	minteger_119,	member_offsetof (form1, gridcellheight),},
	{"GridCellPadding",		prop_gridcellpadding,	minteger_119,	member_offsetof (form1, gridcellpadding),},
	{"GridCellSpacing",		prop_gridcellspacing,	minteger_119,	member_offsetof (form1, gridcellspacing),},
	{"GridCellBackColor",	prop_gridcellbackcolor,	vtrgb_21,		member_offsetof (form1, gridcellbackcolor),},
	{"GridCellWidth",		prop_gridcellwidth,		minteger_119,	member_offsetof (form1, gridcellwidth),	},
	{"GridColumns",			prop_gridcolumns,		vtinteger_19,	member_offsetof (form1, gridcolumns),	},
	{"GridMargin",			prop_gridmargin,		minteger_119,	member_offsetof (form1, gridmargin),	},
	{"GridRows",			prop_gridrows,			vtinteger_19,	member_offsetof (form1, gridrows),		},
	{"GridValues",			prop_gridvalues,		vtint32list_24,	member_offsetof (form1, grid_values_a)  },
	{"FileList",			prop_filelist,			vtlist_str_20,	member_offsetof (form1, filelist_a),	},
	{"FocusStyle",			prop_focusstyle,		vtfocusstyle_202,member_offsetof (form1, focusstyle), 0, "0: Normal 1: Hide Form only"  },
	{"ImageAlpha",			prop_imagealpha,		vtfloat_2,		member_offsetof (form1, imagealpha),},
	{"ImageColor",			prop_imagecolor,		vtrgb_21,		member_offsetof (form1, imagecolor),},
	{"ImageList",			prop_imagelist,			vtlist_str_20,	member_offsetof (form1, imagelist_a),	},
	{"ImageName",			prop_imagename,			vtstring_1,		member_offsetof (form1, image_name_a),	},
	{"IsCutout",			prop_iscutout,			vtinteger_19,	member_offsetof (form1, is_cutout),	},
	{"IsDisabled",			prop_isdisabled,		vtinteger_19,	member_offsetof (form1, is_disabled),	},
	


	{"IsHidden",			prop_ishidden,			vtinteger_19,	member_offsetof (form1, is_hidden),		},
	{"IsHorizontal",		prop_ishorizontal,		vtinteger_19,	member_offsetof (form1, is_horizontal),	},
	
	{"IsKeyPreview",		prop_iskeypreview,		vtinteger_19,	member_offsetof (form1, is_keypreview),	0, "Form will send KeyPreview Events.  See DidConsumeKey"},
	{"IsOutlined",			prop_isoutlined,		vtinteger_19,	member_offsetof (form1, is_outlined),	},
	{"IsRounded",			prop_isrounded,			vtinteger_19,	member_offsetof (form1, is_rounded),	},
	{"List",				prop_list,				vtlist_str_20,	member_offsetof (form1, list_strings_a),},
	{"MaxValue",			prop_maxvalue,			vtfloat_2,		member_offsetof (form1, maxvalue),		},	//
	//{"RowsCount",			prop_rowscount,			vtinteger_19,	member_offsetof (form1, rowscount),		},

	{"ScrollbarColor",		prop_scrollbarcolor,	vtrgb_21,		member_offsetof (form1, scrollbarcolor), 0, "Form only"},	//
	{"ScrollbarThumbColor",	prop_scrollbarthumbcolor,vtrgb_21,		member_offsetof (form1, scrollbarthumbcolor), 0, "Form only"},	//
	{"ServantOwner",		prop_servantowner,		vtcontrolref_23,member_offsetof (form1, servant_owner),	},	//
/*RT*/	{"SelectedIndex",	prop_selectedindex,		vtinteger_19,	member_offsetof (form1, selectedindex),	},	//

	{"Text",				prop_text,				vtstring_1,		member_offsetof (form1, text_a),			},	//
	{"Value",				prop_value,				vtfloat_2,		member_offsetof (form1, value),			},	//
	{"WasInitialized",		prop_wasinitialized,	vtinteger_19,	member_offsetof (form1, wasinitialized),			},	//
	{"Vertexes",			prop_vertexes,			mvertflist_22,	member_offsetof (form1, vertexlist_a),	0, "Spaced delimited pair of points"},	//
	{"UserData",			prop_userdata,			vtfloat_2,		member_offsetof (form1, userdata),	0, "Inert field for user"},	//
	{"UserData2",			prop_userdata2,			vtfloat_2,		member_offsetof (form1, userdata2),	0, "Inert field for user"},	//
	{"UserData3",			prop_userdata3,			vtfloat_2,		member_offsetof (form1, userdata3),	NULL, "Inert field for user"},	//
	{"UserString",			prop_userstring,		vtstring_1,		member_offsetof (form1, userstring),	NULL, "Inert field for user"},	//
	{"UserString2",			prop_userstring2,		vtstring_1,		member_offsetof (form1, userstring2),	0, "Inert field for user"},	//
	{"UserString3",			prop_userstring3,		vtstring_1,		member_offsetof (form1, userstring3),	0, "Inert field for user"},	//

// Demi-Properties
	//"Key" which finds an item in a list -- the form1.list -- and gets or sets the value.
	//
	{"Controls",			demiprop_controls,		0,0,0, "Access frm.controls_a like a list and get the object name"},
	{"ControlIndex",		demiprop_controlindex,	0,0,0, "Runs Object_Get_ControlIndex and finds the current index.  This actually can change (Zorder)."},
	{"ControlCount",		demiprop_controlcount,	0,0,0, "Form. Returns f->frm.controls_a->numitems"},
	{"ControlType",			demiprop_controltype,	0,0,0, "Returns k->po->name like ListView"},
	{"FileListCount",		demiprop_filelistcount,	0,0,0, "Retrieves file list num strings"},
	{"Focused",				demiprop_focused,		0,0,0, "Clear FileList"},
	{"Handle",				demiprop_handle,		0,0,0, "Returns f->ui.csqc_handle_plus1"},
	{"ImageListCount",		demiprop_imagelistcount,0,0,0, "ImageListCount -- retrieves image list num strings"},
	{"Key",					demiprop_key,			0,0,0, "Form Key/Value system treats List as 2 column array."},
	{"ListCount",			demiprop_listcount,		0,0,0, "ListCount"},
	{"SelectedRow",			demiprop_selectedrow,	0,0,0, "SelectedIndex alias for ListView since multi-column"},

	RELATED_ (prop_selectedindex)

// Methods.  meth_cmd = no args, otherwise it has 1 argument.
	{"FileListClear",		meth_cmd_filelistclear,	0,0,0, "Clear FileList"},
	{"ImageListClear",		meth_cmd_imagelistclear,0,0,0, "Clear ImageList"},
	{"ListClear",			meth_cmd_listclear,0,0,0, "Clear List"},

	{"AddItem",				meth_additem,			0,0,0, "AddItem to List"},
	{"AddFile",				meth_addfile,			0,0,0, "AddFile to FileList"},
	{"AddImage",			meth_addimage,			0,0,0, "AddImage to ImageList"},
	//{"ClipboardGetImage",	meth_listappendfilelines,0,0,0,"Image from clipboard."},
	//{"ClipboardSetImage",	meth_listappendfilelines,0,0,0,"Image to clipboard."},
	//{"ClipboardGetText",	meth_listappendfilelines,0,0,0,"Get clipboard text. Rules?  Strip whitespace?  Unicode?  No colors? What copies?"},
	{"ClipboardSetText",	meth_listappendfilelines,0,0,0,"Set clipboard text. Rules?  Strip whitespace?  Unicode?  No colors?"},	
	//{"ListAppendFileLines",	meth_listappendfilelines,0,0,0,"Open file and populate listview list with lines including a linenumber column"},
	{"Popup",				meth_popup,				0,0,0, "Set supplied userstring and show contextmenu"},
//	{"TextWrap",			meth_textwrap,			0,0,0, "Wordwrap a label with supplied text"},

	{"SendHandle",			evx_sendhandle,		0,0,0, "Occurs at handle assignment before any controls are parsed or created.  Message is handle number."},
	{"InitializedWithMessage",evx_initializedwithmessage,		0,0,0, "Occurs after form load but before draw.  Message if specified was supplied by the caller such as a piped Dialog request."},
	{"Click",evx_click,		0,0,0, "Mouseup on a pressed control fires a click.  Right click is message 1.  K_ENTER pressed for Button will fire Click.  Mouse X, Y will be in v_forward"},
	{"DoubleClick",evx_doubleclick,		0,0,0, "A second click in under 0.5 seconds will cause mouseup to fire this instead of Click. Mouse X, Y will be in v_forward"},
	//{"OnChange",evx_onchange,		0,0,0, "Every controls gets one on initialization.  No evidence that this is otherwise fired."},
	{"KeyPreview",evx_keypreview,		0,0,0, "prop_iskeypreview must be set for the form and KeyPreview will have a chance to TAKE the input before anything else gets to look at it. Set form:DidConsumeKey to true in QuakeC if input was handled"},
	{"PopUpClosed",evx_popupclosed,		0,0,0, "O_ContextMenu_Leaving fires PopUpClosed with text of name for selected item or NULL if ESC closed-- Dialog context menu"},

	// Special CSQC functions?
	// my_event_callback	Form events call this function.
	// my_close_callback	Form_Close (h, FileName); // The optional close message is sent.

//
RELATED_ (Form_Event_Onload)
	{"OnLoad",				events_OnLoad,			vtstring_1,		member_offsetof (form1, eve.onload_a), 0, "Occurs after finalize, all controls have been loaded.  No controls have had events fired."	},
	{"OnKeyPreview",		events_OnKeyPreview,	vtstring_1,		member_offsetof (form1, eve.onkeypreview_a),},
	{"OnChange",			events_OnChange,		vtstring_1,		member_offsetof (form1, eve.onchange_a), 0,	"Occurs onload after form onload and whenever a the selectedindex changes."},

// builtin functions we know

	{"zform_load",			builtin_func,					}, // ? does what?
	{"zform_keypreview",	builtin_func,					},
	{"check1_click",		builtin_func,					},
	{"tabsel_click",		builtin_func,					},	// Probably fires the idx logic

// this list requires null for first field for last entry
	{NULL,													},
};

#undef IH // INHERITED MARKER FOR NOW

#define IsFormTop(k)	(k == k->fctrl)
#define IsContained(k)	(k->container != k->fctrl)
#define IsObjectType(p) in_range(class_start, p->enum_id, class_end)
#define IsProperty(p)	in_range(prop_start, p->enum_id, prop_end)
#define IsEvent(p)		in_range(events_start, p->enum_id, events_end)
#define IsMethod(p)		in_range(meth_start, p->enum_id, meth_end)
#define IsBuiltin(p)	(p->enum_id == builtin_func)
#define ThingType(x) \
	(IsObjectType(x) ? "[Object]" :	\
	IsProperty(x) ? "[Property]" :	\
	IsEvent(x) ? "[Event]" :		\
	IsBuiltin(x) ? "[Builtin Function]" : \
	"[Unknown]") // Ender


