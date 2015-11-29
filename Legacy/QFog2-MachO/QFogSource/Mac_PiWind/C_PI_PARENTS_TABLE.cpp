#include "C_PI_PARENTS_TABLE.h"#include "STRETCH_OR_FOLD.h"#include "QB_NET.h"#include "GLOBAL_FUN.h"#include  "C_PI_PA_ST_POPUP.h"#include  "C_PI_WIND.h"#include "StCPTstate.h"#pragma mark --creation/destruction--//******************************************C_PI_PARENTS_TABLE::C_PI_PARENTS_TABLE(LStream   *  inStream)		//in	:LTableView(inStream){	LTableMonoGeometry  *	geo_p		= new LTableMonoGeometry(this, 132 /* cell width */, 20/*cell height*/);	//delete:	in ~LTableView().	ThrowIfNil_(geo_p);	SetTableGeometry(geo_p);		// total width = 264 = 2(132)	InsertCols(		2, //how many		0, //after which col		0, //data pointer		0, //data size 		false); //refresh	its_popups_p_p = 0;	mRows=0;}//******************************************C_PI_PARENTS_TABLE::C_PI_PARENTS_TABLE(){}//******************************************C_PI_PARENTS_TABLE::~C_PI_PARENTS_TABLE(){	for(USHORT i=0; i<mRows;i++){		delete its_popups_p_p[i];		its_popups_p_p[i] = 0;	}	delete [] its_popups_p_p;	its_popups_p_p = 0;}#pragma mark --popup stuff--//******************************************VOID	C_PI_PARENTS_TABLE::reset_num_of_popups(USHORT	new_num_of_rows,	//inUSHORT	old_num_of_rows)	//in{	USHORT i;	if(old_num_of_rows !=new_num_of_rows){		C_PI_PA_ST_POPUP  *  * 		x_p_p = 0;		if(new_num_of_rows!=0){			x_p_p = new C_PI_PA_ST_POPUP  * [new_num_of_rows];//new[]			//delete:	substitution of delete-new pair			ThrowIfNil_(x_p_p);		}				if(old_num_of_rows<new_num_of_rows){			for(i=0; i<old_num_of_rows; i++){				x_p_p[i] = its_popups_p_p[i];			}			for(i=old_num_of_rows; i<new_num_of_rows ; i++){				x_p_p[i] = create_popup_in_this_row(i+1);			}		}else{ //new_num_of_rows<old_num_of_rows			for(i=0; i<new_num_of_rows; i++){				x_p_p[i] = its_popups_p_p[i];			}			for(USHORT i=new_num_of_rows; i<old_num_of_rows; i++){				delete its_popups_p_p[i];				its_popups_p_p[i] = 0;			}		}		delete [] its_popups_p_p;		its_popups_p_p = x_p_p;	}}	//**********************************************C_PI_PA_ST_POPUP  *  C_PI_PARENTS_TABLE::create_popup_in_this_row(USHORT	table_row)		//in{	STableCell	cell;	cell.row = table_row;	cell.col = 2;			//GetLocalCellRect( cell, cell_rect ) doesn't work	//because it returns cell_rect =(0,0,0,0) if cell not visible through scroller				SInt32	left, top, right, bottom;	GetImageCellBounds(cell, left, top, right, bottom);	/*	struct SPaneInfo {	 PaneIDT  paneID;	 SInt16  width;	 SInt16  height;	 Boolean  visible;	 Boolean  enabled;	 SBooleanRect bindings;	 SInt32  left;	 SInt32  top;	 SInt32  userCon;	 LView*  superView;	*/		SPaneInfo	pane_info;		pane_info.paneID = first_pi_pa_st_popup_PPob_rid + table_row -1;	pane_info.width = 16;	pane_info.height = 16;	pane_info.visible = true;	pane_info.enabled = true;	//		SBooleanRect  bindings;		bindings.left = bindings.top = bindings.right = bindings.bottom = false;	pane_info.bindings = bindings;	//	pane_info.left = right -17;	pane_info.top = top + 1;	pane_info.userCon = 0;	pane_info.superView = this;	C_PI_PA_ST_POPUP  *  popup_p = new C_PI_PA_ST_POPUP(		pane_info,	//const SPaneInfo	 &	inPaneInfo		first_pi_pa_st_popup_PPob_rid + table_row -1, //MessageT	inValueMessage		1, //SInt16	inTitleOptions		12345, //ResIDT	inMENUid		0, //SInt16		inTitleWidth		kControlPopupButtonProc,	//SInt16	inKind		Verdana9_rid, //ResIDT	inTextTraitsID		"\p", //Str255	inTitle		0, //OSType	inResTypeMENU, inMacRefCon = '\?\?\?\?'		1, //SInt16	inInitialMenuItem		LPopupButton::imp_class_ID, //ClassIDT	inImpID		table_row,	//USHORT	table_row		its_doc_p,		//C_DOC  *  		doc_p		its_mview_p,	//C_MAIN_VIEW  * 	mview_p		its_net_p); 	//QB_NET  *  		net_p		//delete: 	in ~C_PI_PARENTS_TABLE()				ThrowIfNil_(popup_p);		popup_p->FinishCreate();		popup_p->Show();				LView	 *   view_p = this;		do{			view_p = view_p->GetSuperView();		}while(view_p->GetSuperView()!=0);		popup_p->AddListener( (LListener  *)(C_PI_WIND  *)view_p );			return popup_p;}#pragma mark --const fun--//******************************************USHORT		C_PI_PARENTS_TABLE::get_in_nd_st(USHORT 	row) 		//inconst{	NODE  *  	nd_p = its_net_p->get_nd_ptr(its_nd);	STRETCH_OR_FOLD sof_in(nd_p->get_in_nd_degens());	return  sof_in.vec_label(its_in_st)[row - 1];}#pragma mark --draw one cell--//******************************************VOID	C_PI_PARENTS_TABLE::DrawCell(const STableCell &  inCell,		//inconst Rect &  inLocalRect)		//in{//drawer	StCPTstate	saved_st(true, false, true);	saved_st.Normalize();		::TextFont(systemFont);		::MoveTo(inLocalRect.left + 4, inLocalRect.bottom - 4);	LStr255  		str;	NODE  *  	nd_p = its_net_p->get_nd_ptr(its_nd);	USHORT 		in_nd = nd_p->get_in_nds()[inCell.row-1];	if(inCell.col == 1){		its_net_p->get_nd_ptr(in_nd)->get_name().fill_LStr255(str);		//pixs_for_margins = 8		trunc_pstr_so_it_fits(str, GetColWidth(1), 8); //see GLOBAL_FUN.h	}else if(inCell.col == 2){		STRETCH_OR_FOLD sof_in(nd_p->get_in_nd_degens());		USHORT 	in_nd_st = sof_in.vec_label(its_in_st)[inCell.row-1];		its_net_p->get_nd_ptr(in_nd)->get_st_name(in_nd_st).fill_LStr255(str);		//add extra 18 pixs to pixs_for_margins to create room for popup		trunc_pstr_so_it_fits(str, GetColWidth(2), 8 + 18);	}			::DrawString(str);			draw_cell_frame(inLocalRect);//see GLOBAL_FUN.h	}#pragma mark --functions that call RefreshCell()--//******************************************VOID	C_PI_PARENTS_TABLE::set_in_st(TM_COL_NUM 	new_in_st) 		//in{	its_in_st =new_in_st;			STableCell  cell;	cell.col = 2;	USHORT	num_of_rows = its_net_p->get_nd_ptr(its_nd)->get_num_of_in_nds(); 	for(USHORT r=0; r<num_of_rows; r++){		cell.row = r + 1;		RefreshCell(cell);	}}//******************************************VOID	C_PI_PARENTS_TABLE::set_nd_always(USHORT	new_nd) 		//in{	its_nd =new_nd;	USHORT  old_num_of_rows = mRows;	if(old_num_of_rows!=0){ //this if prevents a warning from being called		RemoveRows(		old_num_of_rows, //how many		1, //starting with row		true);//refresh	}	NODE  *  	nd_p = its_net_p->get_nd_ptr(its_nd);	UInt32		new_num_of_rows = nd_p->get_num_of_in_nds();	InsertRows(		new_num_of_rows,//how many		0, //after which row		0, //data pointer		0, //data size 		true); //refresh			//col 1			STableCell  cell;	cell.col = 1;	for(USHORT r=0; r<new_num_of_rows; r++){		cell.row = r + 1;		RefreshCell(cell);	}		//col 2 -- strings	its_in_st = 0;	if(new_num_of_rows!=0)set_in_st(0);		//col 2 -- popups	reset_num_of_popups(new_num_of_rows, old_num_of_rows);	for(USHORT	i = 0; i< new_num_of_rows; i++){		its_popups_p_p[i]->set_nd_always(nd_p->get_in_nds()[i]);	}}//******************************************VOID	C_PI_PARENTS_TABLE::rename_a_nd(USHORT nd)		//in {	USHORT	in_nd;	STableCell  cell;	cell.col = 1;	USHORT	num_of_rows = its_net_p->get_nd_ptr(its_nd)->get_num_of_in_nds();	for(USHORT r=0; r<num_of_rows; r++){		cell.row = r + 1;		in_nd = its_net_p->get_nd_ptr(its_nd)->get_in_nds()[r];		if(in_nd==nd){			RefreshCell(cell);			return;		}	}}	