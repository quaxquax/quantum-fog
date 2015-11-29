#include "C_BI_ND_PROBS_WIND.h"#include "C_MAIN_VIEW.h"#include "QB_NET.h"#include "C_BI_ND_PROBS_TABLE.h"#include "C_MARGINED_SCROLLER.h"#include "C_BI_ND_PROBS_MARGIN.h"#include "EXCEPTIONS.h"#pragma mark --creation/destruction--//******************************************C_BI_ND_PROBS_WIND::C_BI_ND_PROBS_WIND(LStream  *		inStream)	//in	:LWindow(inStream),	its_bi_nd_id(0){}//******************************************C_BI_ND_PROBS_WIND::C_BI_ND_PROBS_WIND()	:its_bi_nd_id(0){}//******************************************C_BI_ND_PROBS_WIND::~C_BI_ND_PROBS_WIND(){	its_mview_p->finish_bi_nd_probs_wind_closing();	}//******************************************VOID	C_BI_ND_PROBS_WIND::init_popup(){	MenuHandle		menu_h = its_popup_p->GetMacMenuH();	//destroy_h: by PP	ThrowIfNil_(menu_h);		// dummy item must be cleared	SInt16  num_of_items = ::CountMenuItems(menu_h);	for(SInt16 menu_item =1; menu_item<= num_of_items; menu_item++){		::DeleteMenuItem(menu_h, 1);	}		for(USHORT loc=0; loc < its_net_p->get_num_of_bi_nds(); loc++){ 	      	// Add the item      	// Insert a dummy item, then rename it so special chars are not a problem		::AppendMenu(menu_h, "\pdummy");		LStr255 descriptor_pstr;		its_net_p->get_bi_nd_name(loc).fill_LStr255(descriptor_pstr);		if(descriptor_pstr[0]!=0)::SetMenuItemText( menu_h, loc+1 , descriptor_pstr );	}	its_popup_p->SetMinValue(1);   	its_popup_p->SetMaxValue(its_net_p->get_num_of_bi_nds()); 	its_popup_p->StopBroadcasting();			its_popup_p->SetValue(1);	its_popup_p->StartBroadcasting();}//******************************************VOID	C_BI_ND_PROBS_WIND::show_owner(const LStr255  &  file_name)		//in{	LStr255  cap = "\pProject: ";	cap += (LStr255)file_name;	its_owner_cap_p->SetDescriptor(cap);}//******************************************VOID	C_BI_ND_PROBS_WIND::special_finish_create(const LStr255  & file_name,		//inUSHORT bi_nd_id)				//in{	its_owner_cap_p =(LCaption  *)FindPaneByID(bi_nd_probs_owner_cap_rid);	its_info_cap_p = (LCaption  *)FindPaneByID(bi_nd_probs_info_cap_rid);	its_popup_p = (LPopupButton  *)FindPaneByID(bi_nd_probs_popup_rid);	its_first_but_p =(LStdButton  *)FindPaneByID(bi_nd_probs_first_but_rid);	its_next_but_p =(LStdButton  *)FindPaneByID(bi_nd_probs_next_but_rid);	its_bi_nd_state_cap_p =(LCaption  *)FindPaneByID(bi_nd_probs_state_cap_rid);		C_MARGINED_SCROLLER  * scroller_p =(C_MARGINED_SCROLLER  *)FindPaneByID(bi_nd_probs_scroller_rid);		its_table_p =(C_BI_ND_PROBS_TABLE  *)FindPaneByID(bi_nd_probs_table_rid);	its_left_margin_p =(C_BI_ND_PROBS_MARGIN  *)FindPaneByID(bi_nd_probs_lmargin_rid);	its_top_margin_p =(C_BI_ND_PROBS_MARGIN  *)FindPaneByID(bi_nd_probs_tmargin_rid); 	// Activate RidL resource.   	UReanimator::LinkListenerToControls(this, this, bi_nd_probs_wind_ridl);   	   	//its SuperCommander defined in C_MAIN_VIEW::create_bi_nd_probs_wind() 	((C_MAIN_VIEW  *)GetSuperCommander())->get_top_ptrs(its_doc_p, its_mview_p, its_net_p);  	// sub-pane initializations  that do not depend on bi_nd_id	 	show_owner(file_name); 	  	init_popup();  	 	its_bi_nd_state_cap_p->SetDescriptor("\p");	USHORT 	cell_width = 80;	USHORT 	cell_height = 20;	LTableMonoGeometry	 * 	geo_p;	 	 		its_table_p->set_top_ptrs(its_doc_p, its_mview_p, its_net_p);	geo_p = new LTableMonoGeometry(its_table_p, cell_width, cell_height);	//delete:	in ~LTableView().	ThrowIfNil_(geo_p);	its_table_p->SetTableGeometry(geo_p);			its_top_margin_p->set_top_ptrs(its_doc_p, its_mview_p, its_net_p);	its_top_margin_p->set_side(top_sd);	geo_p = new LTableMonoGeometry(its_top_margin_p, cell_width, cell_height);	//delete:	in ~LTableView().		ThrowIfNil_(geo_p);	its_top_margin_p->SetTableGeometry(geo_p);	its_top_margin_p->InsertRows(1, 0, 0, 0, true);		its_left_margin_p->set_top_ptrs(its_doc_p, its_mview_p, its_net_p);	its_left_margin_p->set_side(left_sd);	geo_p = new LTableMonoGeometry(its_left_margin_p, cell_width, cell_height);	//delete:	in ~LTableView().			ThrowIfNil_(geo_p);	its_left_margin_p->SetTableGeometry(geo_p);	its_left_margin_p->InsertCols(1, 0, 0, 0, true);		scroller_p->set_top_margin_ptr((LView  *)its_top_margin_p);	scroller_p->set_left_margin_ptr((LView  *)its_left_margin_p);		//	StartIdling();	// initializations that depend on bi_nd_id	set_bi_nd_always(bi_nd_id); // function does no diff checking.	Show();//necessary! 	  	}#pragma mark --focus bi-node--//******************************************VOID 	C_BI_ND_PROBS_WIND::refresh_info_cap(){	BI_NODE  *	bi_nd_p = its_net_p->get_bi_nd_ptr(its_bi_nd_id);	USHORT	nd1, nd2;	its_net_p->get_bi_nd(its_bi_nd_id, nd1, nd2);		USHORT	num_of_rows = its_net_p->get_nd_ptr(nd1)->get_degen();	USHORT	num_of_cols = its_net_p->get_nd_ptr(nd2)->get_degen();	DOUBLE  	prob = 0;	DOUBLE		info = 0;	for(USHORT	r=0; r<num_of_rows; r++){		for(USHORT	c=0; c<num_of_cols; c++){			prob = bi_nd_p->get_cum_prob(r, c);			if(prob>probs_floor) info -= prob*log(prob)/nat_log_of_2;		}	}	LStr255	str;	str.Assign(info, FIXEDDECIMAL, 6);	its_info_cap_p->SetDescriptor(str);}//******************************************VOID	C_BI_ND_PROBS_WIND::set_bi_nd_always(USHORT  	new_id)			//in{		its_bi_nd_id =new_id;		its_popup_p->StopBroadcasting();			its_popup_p->SetValue(new_id + 1);	its_popup_p->StartBroadcasting();	refresh_info_cap();		its_table_p->set_bi_nd_always(new_id);		its_left_margin_p->set_bi_nd_always(new_id);		its_top_margin_p->set_bi_nd_always(new_id);			}#pragma mark --buttons:	message center--//******************************************VOID	C_BI_ND_PROBS_WIND::ListenToMessage(MessageT  	inMessage,		//inVOID  * 	ioParam )		//i-o{	//MONITOR_THE_CHANGE_HERE	USHORT		new_item_num;	USHORT		num_of_items = its_net_p->get_num_of_bi_nds();		switch ( inMessage ) {		case msg_bi_nd_probs_popup:			// for nd_popup: inMessage = pane id, *ioParam = current item 			new_item_num = *((SInt32  *)ioParam);						set_bi_nd_if_diff(new_item_num - 1);			break;		case msg_bi_nd_probs_first_but:			set_bi_nd_if_diff(0);					break;		case msg_bi_nd_probs_next_but:			if(its_bi_nd_id + 1< num_of_items){				set_bi_nd_if_diff(its_bi_nd_id + 1);			}else{				::SysBeep(0);			}			break;	}}#pragma mark --bi-node state cap--//******************************************VOID 	C_BI_ND_PROBS_WIND::refresh_bi_nd_state_cap(const WHICH_TABLE  &   table)		//in{	USHORT  nd1, nd2;	its_net_p->get_bi_nd(its_bi_nd_id, nd1, nd2);		STRINGY		namey1 = "";	STRINGY		namey2 = "";	STRINGY		mid_stringy = "";		switch(table){		case main_ta:			namey1 = its_net_p->get_nd_ptr(nd1)->get_st_name(its_t_cell[main_ta].row - 1);			namey2 = its_net_p->get_nd_ptr(nd2)->get_st_name(its_t_cell[main_ta].col - 1);			mid_stringy = " \\ ";			break;		case left_ta:			namey1 = its_net_p->get_nd_ptr(nd1)->get_st_name(its_t_cell[left_ta].row - 1);			break;		case top_ta:			namey2 = its_net_p->get_nd_ptr(nd2)->get_st_name(its_t_cell[top_ta].col - 1);			break;	}	LStr255		name1;	LStr255		name2;	LStr255		mid;	 	namey1.fill_LStr255(name1);	namey2.fill_LStr255(name2);	mid_stringy.fill_LStr255(mid);	LStr255		name = name1 + mid + name2;	its_bi_nd_state_cap_p->SetDescriptor(name);}//******************************************VOID 	C_BI_ND_PROBS_WIND::SpendTime(const EventRecord  &	inMacEvent )		//in{#pragma unused(inMacEvent)		STableCell 	t_cell[3];	Str255 	pstr; 		if(its_table_p->get_touched_cell(t_cell[main_ta])){		its_t_cell[left_ta].SetCell(0, 0);		its_t_cell[top_ta].SetCell(0, 0);		if(its_t_cell[main_ta] != t_cell[main_ta]){			its_t_cell[main_ta] = t_cell[main_ta];			refresh_bi_nd_state_cap(main_ta);			}	}else if(its_left_margin_p->get_touched_cell(t_cell[left_ta])){		its_t_cell[main_ta].SetCell(0, 0);		its_t_cell[top_ta].SetCell(0, 0);		if(its_t_cell[left_ta] != t_cell[left_ta]){			its_t_cell[left_ta] = t_cell[left_ta];			refresh_bi_nd_state_cap(left_ta);			}	}else if(its_top_margin_p->get_touched_cell(t_cell[top_ta])){		its_t_cell[main_ta].SetCell(0, 0);		its_t_cell[left_ta].SetCell(0, 0);		if(its_t_cell[top_ta] != t_cell[top_ta]){			its_t_cell[top_ta] = t_cell[top_ta];			refresh_bi_nd_state_cap(top_ta);			}	}else{		its_t_cell[top_ta].SetCell(0, 0);		its_t_cell[left_ta].SetCell(0, 0);		its_t_cell[main_ta].SetCell(0, 0);		its_bi_nd_state_cap_p->GetDescriptor(pstr);		if(pstr[0]!=0)its_bi_nd_state_cap_p->SetDescriptor("\p");	}}//******************************************/*VOID 	C_BI_ND_PROBS_WIND::DrawSelf(){//drawer		LWindow::DrawSelf();	Rect  frame;		its_left_margin_p->CalcPortFrameRect(frame);	::InsetRect(&frame, 1, 1);	::FrameRect(&frame);		its_top_margin_p->CalcPortFrameRect(frame);	::InsetRect(&frame, 1, 1);	::FrameRect(&frame);	}*/