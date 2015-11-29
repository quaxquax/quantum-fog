#pragma once#include "QFog_constants.h"#include "C_COLORS.h"#include "QB_NET.h"#include "GLOBAL_FUN.h"#include "C_PORT_RELATED.h"#include "C_PI_WIND.h"#include "C_BI_ND_WIND.h"#include "C_ND_PROBS_WIND.h"#include "C_BI_ND_PROBS_WIND.h"#include "C_STORIES_WIND.h"#include "C_ND_EFIELD.h"#include "UTextMenus.h"#include "C_WIND_BDS_RECO.h"#include "C_ACTION_FLAGS.h"class C_DOC;class C_PALETTE;class SET;//******************************************class	C_MAIN_VIEW : public LView, public LCommander, public LListener, public C_ACTION_FLAGS{protected:	C_DOC  * 	its_doc_p;	QB_NET  *	its_net_p;	USHORT		its_num_of_sel_nds;	USHORT		its_num_of_sel_arrows;		SInt16 		its_image_horiz_inches;	SInt16		its_image_vert_inches;	BOOLEAN		its_has_grid_lines;	SInt16  	its_fnum; //font number	SInt16		its_fname_item_num; //font name's item number	SInt16 		its_fsize; //font size		SInt32		its_pane_counter;		Point		its_pasting_center;	LWindow  * 				its_main_wind_p; 	C_PI_WIND  *			its_pi_wind_p; 	C_BI_ND_WIND  * 		its_bi_nd_wind_p; 	C_ND_PROBS_WIND  * 		its_nd_probs_wind_p;  	C_BI_ND_PROBS_WIND  * 	its_bi_nd_probs_wind_p;  	C_STORIES_WIND  * 		its_stories_wind_p;  	//the document does not possess its own palette but it  	//does possess its preferred palette bounds 	C_WIND_BDS_RECO 		its_pal_bds_reco; 	C_WIND_BDS_RECO 		its_pi_wind_bds_reco; 	C_WIND_BDS_RECO		 	its_bi_nd_wind_bds_reco; 	C_WIND_BDS_RECO		 	its_nd_probs_wind_bds_reco;  	C_WIND_BDS_RECO			its_bi_nd_probs_wind_bds_reco;  	C_WIND_BDS_RECO			its_stories_wind_bds_reco;		public:		C_MAIN_VIEW(LStream *	inStream);	virtual 		~C_MAIN_VIEW();	VOID  			FinishCreateSelf();	VOID	 	set_net_ptr(QB_NET  *  net_p);	VOID		get_top_ptrs(C_DOC  *  & doc_p, C_MAIN_VIEW  *  & mview_p, QB_NET  *  & net_p);	const C_WIND_BDS_RECO  &		get_pal_bds_reco() const;	VOID	set_pal_bds_reco();	VOID	read_bounds_res_of_satellite_winds();	VOID	write_bounds_res_of_satellite_winds();	BOOLEAN		save_pi();		VOID		refresh_pi_wind();	VOID		refresh_pi_wind(USHORT	nd_id);	VOID 		create_pi_wind(USHORT  sel_nd);	VOID 		finish_pi_wind_closing();	VOID 		create_bi_nd_wind();	VOID 		finish_bi_nd_wind_closing();	VOID 		create_nd_probs_wind(USHORT  sel_nd);	VOID 		finish_nd_probs_wind_closing();		VOID 		create_bi_nd_probs_wind(USHORT  sel_loc);	VOID 		finish_bi_nd_probs_wind_closing();		VOID 		create_stories_wind();	VOID 		finish_stories_wind_closing();	SInt16  	get_fnum() const;	SInt16 		get_fsize() const;	VOID		check_fsize_and_fnum();			SInt16  	get_image_horiz_inches();			VOID  		set_image_horiz_inches(SInt16  num);	SInt16  	get_image_vert_inches();	VOID  		set_image_vert_inches(SInt16  num);	VOID		shift_wind_to_top_left_corner_of_desktop();		VOID		set_min_max_and_std_of_wind(BOOLEAN	 set_bds_to_std);	BOOLEAN		handle_drawing_size_dialog();		virtual VOID    DrawSelf();	VOID	disable_nd_efields();	VOID	enable_nd_efields();			VOID  	create_nd_efield(const Point  &  m_down_pt, USHORT nd_id);			VOID  	destroy_nd_efield( USHORT nd_id);			VOID 	ListenToMessage(MessageT  inMessage, VOID  * ioParam );		VOID   	show_story_caps(const VECTOR<USHORT>   *  net_story_p);		C_PORT_NAME 	get_touched_avail_s_port(const Point  &  pt);	C_PORT_NAME 	get_touched_avail_d_port(const Point  &  pt);	USHORT  		get_touched_nd(const Point  &  pt);	C_ARROW  *  	get_touched_ar(const Point  &  pt);	USHORT		get_num_of_sel_nds();	BOOLEAN		at_least_one_nd_is_sel();	VOID		num_of_sel_nds_pp();	VOID		num_of_sel_nds_mm();	USHORT		get_num_of_sel_arrows();	BOOLEAN		at_least_one_arrow_is_sel();	VOID		num_of_sel_arrows_pp();	VOID		num_of_sel_arrows_mm();	BOOLEAN		s_and_d_and_ar_selected(const C_ARROW  *  ar_p);	BOOLEAN		s_or_d_or_ar_selected(const C_ARROW	 *	ar_p);			VOID    	select_all();	Boolean    	unselect_all();	VOID    	select_nd_and_its_entourage(USHORT nd_id);	VOID    	select_rect_contents(const Rect  & rect);	VOID   		move_selection_by(const Point  & delta);	VOID    	respond_to_selection_tool(const Point  &  m_down_pt, Boolean pal_open);	Rect		get_sel_rect();	VOID		get_map_with_sel_last(UI_MAP  &	 map_with_sel_last);	VOID		make_only_last_nds_selected(USHORT	num_of_sel_nds);//	VOID		get_selected_arrows(SET & 	sel_ars);//	VOID		set_selected_arrows(const SET &	 sel_ars);	VOID		make_only_first_arrows_selected(USHORT	num_of_sel_ars);		VOID   	respond_to_bi_nd_selector(const Point  &  m_down_pt);	VOID    respond_to_drag_tool(const Point  &  m_down_pt);	VOID   	respond_to_open_tool(const Point  &  m_down_pt);	VOID   	respond_to_no_tool(const Point  &  m_down_pt);	VOID   	rotate_nd(const Point  &  m_down_pt);	VOID 	paint_nd(const Point  &  pt);	VOID 	copy_nd_color(const Point  &  pt);	VOID  	maybe_add_arrow(const Point  &  m_down_pt);	VOID	maybe_change_source_nd_of_ar(const Point  &  m_down_pt);	VOID			Click(SMouseDownEvent	& inMouseDown);	virtual VOID	ClickSelf(const SMouseDownEvent &  in_mouse_down);	virtual VOID  	AdjustMouseSelf(Point in_port_pt, const EventRecord  & in_mac_event, RgnHandle outMouseRgn);	VOID		translate_pt_from_sel_to_pasting_rect(Point	 &	pt, const Rect  &  sel_rect);	BOOLEAN		pasting_should_restore_amps_of(USHORT nd_id);	BOOLEAN		undeleting_should_restore_amps_of(USHORT nd_id);	VOID		write_net_stream(LStream  &  net_stream, NET_STREAM_TYPE stream_type);		VOID		read_net_stream(LStream  &  net_stream, NET_STREAM_TYPE  stream_type); 	VOID	copy_selection();	VOID	paste_clipping();	VOID    delete_selection(BOOLEAN  sel_is_last);		virtual BOOLEAN  	HandleKeyPress(const EventRecord  & in_key_event);	virtual BOOLEAN 	ObeyCommand(CommandT inCommand, VOID * ioParam);	virtual VOID		FindCommandStatus(CommandT inCommand,							BOOLEAN & outEnabled, BOOLEAN & outUsesMark,							UInt16 & outMark, Str255 outName_pstr);};#pragma mark -	//******************************************inlineVOID 	C_MAIN_VIEW::set_net_ptr(QB_NET	 * 	net_p)	//in{	//used only once, in C_DOC constructor	its_net_p = net_p;}//******************************************inlineconst C_WIND_BDS_RECO  &   C_MAIN_VIEW::get_pal_bds_reco() const{	return	its_pal_bds_reco;	}//******************************************inlineVOID	C_MAIN_VIEW::refresh_pi_wind(){	if(its_pi_wind_p!=0)its_pi_wind_p->set_nd_always();}//******************************************inlineVOID	C_MAIN_VIEW::refresh_pi_wind(USHORT	nd_id){	if(its_pi_wind_p!=0)its_pi_wind_p->set_nd_always(nd_id);}//******************************************inlineSInt16  	C_MAIN_VIEW::get_fnum() const{ 	return its_fnum;}//******************************************inlineSInt16  	C_MAIN_VIEW::get_fsize() const{ 	return its_fsize;}//******************************************inlineSInt16  	C_MAIN_VIEW::get_image_horiz_inches(){	return its_image_horiz_inches;}//******************************************inlineVOID  	C_MAIN_VIEW::set_image_horiz_inches(SInt16  num)		//in{ 	its_image_horiz_inches = num;}//******************************************inlineSInt16  	C_MAIN_VIEW::get_image_vert_inches(){	return its_image_vert_inches;}//******************************************inlineVOID  	C_MAIN_VIEW::set_image_vert_inches(SInt16  num)		//in{	its_image_vert_inches = num;}//******************************************inlineUSHORT	C_MAIN_VIEW::get_num_of_sel_nds(){   	return	its_num_of_sel_nds;	}//******************************************inlineBOOLEAN   C_MAIN_VIEW::at_least_one_nd_is_sel(){	return (its_num_of_sel_nds > 0);}//******************************************inlineVOID	C_MAIN_VIEW::num_of_sel_nds_pp(){   	its_num_of_sel_nds++;	}//******************************************inlineVOID	C_MAIN_VIEW::num_of_sel_nds_mm(){	SignalIf_(its_num_of_sel_nds==0);  	its_num_of_sel_nds--;	}//******************************************inlineUSHORT	C_MAIN_VIEW::get_num_of_sel_arrows(){   	return	its_num_of_sel_arrows;	}//******************************************inlineBOOLEAN   C_MAIN_VIEW::at_least_one_arrow_is_sel(){	return (its_num_of_sel_arrows > 0);}//******************************************inlineVOID	C_MAIN_VIEW::num_of_sel_arrows_pp(){   	its_num_of_sel_arrows++;	}//******************************************inlineVOID	C_MAIN_VIEW::num_of_sel_arrows_mm(){	SignalIf_(its_num_of_sel_arrows==0);  	its_num_of_sel_arrows--;	}