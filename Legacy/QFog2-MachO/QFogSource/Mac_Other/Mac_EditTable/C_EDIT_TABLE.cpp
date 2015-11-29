#include "C_EDIT_TABLE.h"#include "C_KEYS.h"#include "EXCEPTIONS.h"#include "QFog_constants.h"#include "STRINGY.h"#include "C_ETABLE_ACTIONS.h"#include "C_ETABLE_MULTI_SEL.h"#include "StCPTstate.h"#pragma mark --creation/destruction--//******************************************VOID	C_EDIT_TABLE::set_text_traits_rid(ResIDT rid)		//in{	TextTraitsH	 text_traits_h = UTextTraits::LoadTextTraits(rid);//not destroyed by PP	//create_h:		PP creates text_traits_h with a call to ::GetResource().	//throwif_h:	PP calls SignalIfNil immediately after creation.	//destroy_h:	PP does NOT release text_traits_h resource. 	//				I delete it below, in this method.	ThrowIfNil_(text_traits_h);		its_font_size = (**text_traits_h).size;	its_font_num = (**text_traits_h).fontNumber;	ReleaseResource_((Handle)text_traits_h);	if(its_editor_p!=0)its_editor_p->SetTextTraitsID(rid);}//******************************************VOID	C_EDIT_TABLE::init(){	mUseDragSelect = true;	its_editor_p = new C_CELL_EDITOR((LView  *) this, (LCommander  *) this);//SuperCommander declared	//delete:	in ~C_EDIT_TABLE().	ThrowIfNil_(its_editor_p);		set_text_traits_rid(Verdana12_rid);	its_sel_mgr = editor_mgr;}//******************************************VOID	C_EDIT_TABLE::FinishCreateSelf(){	//IMP, about mTableSelector:	//With a non-editable LTableView, LTableSingleSelector	//may be useful. But with C_EDIT_TABLE it never will be.	//The C_CELL_EDITOR and its underlying efield will	//take care of all single cell selection duties.	//Thus, you should never use LTableSingleSelector with C_EDIT_TABLE.	//Instead, use either LTableMultiSelector or no selector.	//If you use LTableMultiSelector:	//If you subclass C_EDIT_TABLE and add a multi mTableSelector	//then you should declare the mTableSelector in the 	//subclass's constructor.	//The subclass's constructor is called before this method.	if(mTableSelector!=0){		//I allow uncut, undelete and unpaste. 		//None of these will ever be done (for more than one cell)		//if there is no mTableSelector.					LUndoer  * undoer_p = new LUndoer;		//delete:	PP deletes undoer_p via 		//			~C_EDIT_TABLE()->~LTableView()->~LView()->~LPane()		//			->~LAttachable()->LAttachable::RemoveAllAttachments().		ThrowIfNil_(undoer_p);						AddAttachment(undoer_p);	}}//******************************************C_EDIT_TABLE::C_EDIT_TABLE(LStream  *		inStream)	//in	:LTableView(inStream),	C_ACTION_FLAGS(etable_num_of_act_types){	init();}//******************************************C_EDIT_TABLE::~C_EDIT_TABLE(){	if(its_editor_p != 0){		delete its_editor_p;		its_editor_p = 0;	}}#pragma mark --about editor--//******************************************VOID	C_EDIT_TABLE::nullify_editor(){	its_editor_p->set_cell(STableCell(0,0));	its_editor_p->Hide();}#pragma mark --about one cell--//******************************************VOID	C_EDIT_TABLE::get_cell_data(const STableCell  & 	cell,		//inLStr255  & 		str)				//out{	//The following assumes that you call	//SetTableStorage(new LTableArrayStorage(this, data_size ));	//in the constructor.	//By the way,	//if you use LTableArrayStorage, all data has a COMMON datasize. 	//The datasize parameter in GetCellData() and SetCellData() is ignored!	//If you use your own storage instead of LTableArrayStorage(), then 	//override this method.	//	ioDataSize	//	input: maximum bytes of data to retrieve	//	output: actual bytes of data passed back	UInt32		io_data_size = data_size;	GetCellData(cell, str, io_data_size);}//******************************************VOID	C_EDIT_TABLE::set_cell_data(const STableCell  &		cell,		//inconst LStr255  &		str)		//in{	//This method changes cell storage and appearance.	//read comments that preface get_cell_data().	//Again, if you use your own storage, override this method.	SetCellData(cell, str, data_size);//storage	RefreshCell(cell);//appearance}//******************************************VOID	C_EDIT_TABLE::delete_cell_data(const STableCell  &   cell)			//in{	//This method changes cell storage and appearance.	//read comments that preface get_cell_data().	//Again, if you use your own storage, override this method.	SetCellData(cell, "\p", data_size);//storage	RefreshCell(cell);//appearance}//******************************************VOID	C_EDIT_TABLE::DrawCell(const STableCell  & 	inCell,			//inconst Rect  &		inLocalRect)		//in{//drawer 	StCPTstate	saved_st(true, false, true);	saved_st.Normalize();		::TextSize(its_font_size);	::TextFont(its_font_num);	::MoveTo(inLocalRect.left + 4, inLocalRect.bottom - 4);	LStr255  		str;		get_cell_data(inCell, str);		trunc_pstr_so_it_fits(str, GetColWidth(inCell.col), 8);		::DrawString(str);				draw_cell_frame(inLocalRect);//see GLOBAL_FUN.h	}#pragma mark --about one cell:	clicking it--//******************************************BOOLEAN		C_EDIT_TABLE::unload_editor(){	STableCell cell = its_editor_p->get_cell();	//If possible, you should follow a call 	//to this function (unload_editor()) by a call 	//to nullify_editor() so that redundant calls	//to this function are aborted by the following line.	if(cell.IsNullCell())return true;	Str255  pstr;	its_editor_p->GetDescriptor(pstr);	if(str_is_valid_cell_entry(cell, pstr)){		set_cell_data(cell, pstr);		return true;	}else{		LCommander::SwitchTarget(its_editor_p);		//this fixes small anomally with empty efields		if(pstr[0]==0)its_editor_p->SetDescriptor("\p?");		its_editor_p->SelectAll();		return false;	}}//******************************************VOID	C_EDIT_TABLE::install_editor_at(const STableCell  &  cell)		//in{	its_editor_p->set_cell(cell);	Rect  			cell_rect;				GetLocalCellRect( cell, cell_rect ); 	its_editor_p->move_to(cell_rect.left , cell_rect.top ); 	its_editor_p->ResizeFrameTo(cell_rect.right - cell_rect.left, 								cell_rect.bottom - cell_rect.top, false);	 					LStr255 	str;		get_cell_data( cell, str);	its_editor_p->SetDescriptor( str );	}//******************************************VOID	C_EDIT_TABLE::Click(SMouseDownEvent  & inMouseDown)		//in{	//MONITOR_THE_CHANGE_HERE/*	Variables affecting response to a click:	(1)Was shift key down when mouse went down? N, Y	(2)Number of cells selected before click? 0, 1, many	(3)Mouse went down on selected cell? N, Y	(4)Mouse went up on same cell as it went down? N, Y	If shift key up, cell editor does mouse tracking, and 	if down, LTableMultiSelector::DragClick() does it.	*/	//See LView::Click()	//start with etable as target. Then if install editor, make it the target.		LCommander::SwitchTarget(this);		BOOLEAN		shift_key_is_down = ((inMouseDown.macEvent.modifiers & shiftKey) != 0);	if(shift_key_is_down && mTableSelector!=0){		//C_EDIT_TABLE always handles the		//click first, even if the cell editor is hit.			LPane::Click(inMouseDown);//this function always calls C_EDIT_TABLE::ClickSelf()	}else{		//If a subpane (cell_editor) is hit, we let		//the cell editor  handle the click. If it can't,		//C_EDIT_TABLE handles it.	 	LTableView::Click(inMouseDown);	//this function calls either C_CELL_EDITOR::ClickSelf()		 	 								//or C_EDIT_TABLE::ClickSelf() but not both	}}//******************************************VOID	C_EDIT_TABLE::ClickSelf(const SMouseDownEvent  &  inMouseDown)		//in{	//MONITOR_THE_CHANGE_HERE		//NONTRIVIAL EXCERPT FROM PP	//See LTableView::ClickSelf()	STableCell	hitCell;	SPoint32	imagePt;		LocalToImagePoint(inMouseDown.whereLocal, imagePt);	BOOLEAN		shift_key_is_down = ((inMouseDown.macEvent.modifiers & shiftKey) != 0);	BOOLEAN		anchor_and_fin_cells_same;		if( GetCellHitBy(imagePt, hitCell)){// Click is inside of a cell		if(shift_key_is_down &&		 mTableSelector!=0 &&		 ((C_ETABLE_MULTI_SEL  *)mTableSelector)->is_selectable_cell(hitCell)){			if(unload_editor()){								UnselectAllCells();//sets anchor to (0,0)				STableCell  ed_cell = its_editor_p->get_cell();				if(!ed_cell.IsNullCell()){					((C_ETABLE_MULTI_SEL  *)mTableSelector)->set_anchor_cell(ed_cell);				}				nullify_editor();				//This ClickSelect() always calls mTableSelector->DragSelect()				anchor_and_fin_cells_same = ClickSelect(hitCell, inMouseDown);				if(anchor_and_fin_cells_same)ClickCell(hitCell, inMouseDown);			}		}else{			ClickCell(hitCell, inMouseDown);		}		}else{// Click is outside of any cell		if(unload_editor()){						UnselectAllCells();			nullify_editor();		}	}}//******************************************VOID	C_EDIT_TABLE::ClickCell(const STableCell  &		inCell,					//inconst SMouseDownEvent  &	inMouseDown)		//in{#pragma unused(inMouseDown)	//MONITOR_THE_CHANGE_HERE		//original LTableView  method is empty		if(its_editor_p->get_cell()==inCell)return;		//don't want to unload each time click cell so this	//should come after check if its_editor_p->get_cell()==inCell.	if(unload_editor()){				its_editor_p->Hide();				// All editor changes that depend on inCell are done here:		install_editor_at(inCell);				LCommander::SwitchTarget(its_editor_p);		if(mTableSelector!=0){			UnselectAllCells();			SelectCell(inCell);		}		its_editor_p->SelectAll();		its_editor_p->PostAction(0);		its_editor_p->SetMaxChars(31);//default		its_editor_p->Show();	}}#pragma mark --selection--//******************************************SEL_MANAGER		C_EDIT_TABLE::get_sel_mgr() const{	if(mTableSelector==0){		return editor_mgr;	}else{			return ((C_ETABLE_MULTI_SEL  *)mTableSelector)->get_sel_size()<=1?editor_mgr:table_mgr;	}	}#pragma mark --write/read sub matrices--//******************************************BOOLEAN		C_EDIT_TABLE::sub_mat_can_be_pasted(const C_SUB_MATRIX  &  	smat)			//in{	//Checks that smat fits in table and that	//the proposed cell entries are of the expected type.		STableCell  first_sel_cell = GetFirstSelectedCell();	if(!IsValidCell(first_sel_cell)){		my_gen_alert("Tell me where to paste by selecting at least one table cell.");		return false;	}	if(	mCols - first_sel_cell.col + 1<smat.its_num_of_cols ||		mRows - first_sel_cell.row + 1<smat.its_num_of_rows){		my_gen_alert("The contents of the Clipboard are too large to paste using the top-left corner that you've indicated.");		return false;	}	//strtok() modifies its argument string so	//must make a non-constant copy of *smat.its_content_h		LONG		contents_size = ::GetHandleSize(smat.its_content_h);	if(contents_size==0)return false;	CHAR  * 	cstr = new CHAR[contents_size];	//delete: at the end of this method	ThrowIfNil_(cstr);	StHandleState saved_h(smat.its_content_h);//preserve handle state	::HLock(smat.its_content_h);//saved handle state	::BlockMoveData(*smat.its_content_h, cstr, contents_size);	::HUnlock(smat.its_content_h);//saved handle state	//smat.its_content_h, if not empty, should contain a cstr, with null termination	SignalIf_(cstr[contents_size-1]!='\0');		STableCell	cell(first_sel_cell);	CHAR  * 	delims = "\t\r";		CHAR  *		token_cstr;	LStr255		pstr;	CHAR  * 	ch_p;	for(USHORT r=0; r<smat.its_num_of_rows; r++){		for(USHORT c=0; c<smat.its_num_of_cols; c++){			if(r==0 && c==0){				ch_p = cstr;			}else{				ch_p = 0;			}			token_cstr = strtok(ch_p, delims );			pstr = token_cstr;			if(!str_is_valid_cell_entry(cell, pstr)){				delete []	cstr;				return false;			}			cell.col++;		}		cell.col = first_sel_cell.col;		cell.row++;	}	delete []	cstr;			return true;}//******************************************VOID	C_EDIT_TABLE::resize_and_anchor_sub_mat_like_sel(C_SUB_MATRIX  &	smat)		//i-o{	SignalIf_(mTableSelector==0);	Rect 	r =((C_ETABLE_MULTI_SEL  *)mTableSelector)->get_sel_rect();	smat.its_num_of_rows = r.bottom - r.top;	smat.its_num_of_cols = r.right - r.left;	smat.its_anchor = GetFirstSelectedCell();}	//******************************************VOID	C_EDIT_TABLE::write_sub_mat(C_SUB_MATRIX  &		smat)		//i-o{	//On input, smat MUST have the desired dimensions and anchor.	//This method does not check that these are legal!	//I usually call 	//resize_and_anchor_sub_mat_like_sel(smat);	//before calling this method.		//Called by	//C_EDIT_TABLE::copy_selection()	//C_ETABLE_DEL_OR_CUT_ACTION::C_ETABLE_DEL_OR_CUT_ACTION()	//C_ETABLE_PASTE_ACTION::C_ETABLE_PASTE_ACTION()	STableCell 	cell = smat.its_anchor;	ThrowIf_(!IsValidCell(cell));	USHORT		first_col = cell.col;		LStr255 	pstr;	LStr255	 	delim = "\p";//stress the debug version#if		_do_debug		const LONG 	step_size = 32;#else		const LONG	step_size = 1024;#endif		LONG		handle_size = 0;	LONG		data_size = 0;		ThrowIf_(::GetHandleSize(smat.its_content_h)!=0);	StHandleState saved_h(smat.its_content_h);//preserve handle state	for(USHORT r=0; r<smat.its_num_of_rows;r++){		for(USHORT c=0; c<smat.its_num_of_cols; c++){			get_cell_data(cell, pstr);			//next 2 so as to allow room for adding character			//at beginning and end of pstr			if(data_size + pstr[0] + 2 >handle_size){				//always unlock handle before growing its size				::HUnlock(smat.its_content_h);//saved handle state				::SetHandleSize(smat.its_content_h, handle_size + step_size);				ThrowIfMemFail_(smat.its_content_h);				handle_size += step_size;			}			pstr = delim + pstr;			::HLock(smat.its_content_h);//saved handle state			::BlockMoveData(ConstStringPtr(pstr) + 1, *smat.its_content_h + data_size, pstr[0]);			::HUnlock(smat.its_content_h);//saved handle state			data_size += pstr[0];			delim = "\p\t";			cell.col++;		}		delim = "\p\r";		cell.col = first_col;		cell.row++;	}	//make last char a '\0'	::HLock(smat.its_content_h);//saved handle state	(*smat.its_content_h)[data_size] = '\0';	::HUnlock(smat.its_content_h);//saved handle state	//reduce handle size		//always unlock handle before growing its size		::HUnlock(smat.its_content_h);//saved handle state	::SetHandleSize(smat.its_content_h, data_size + 1);	ThrowIfMemFail_(smat.its_content_h);		}//******************************************VOID	C_EDIT_TABLE::read_sub_mat(const C_SUB_MATRIX  & 	smat)				//in{	//does no checking!	//called by 	//C_ETABLE_DEL_OR_CUT_ACTION::UndoSelf()	//C_ETABLE_PASTE_ACTION::RedoSelf()	//C_ETABLE_PASTE_ACTION::UndoSelf()	//Refs.	//(1)Think Ref. under String.h	//(2)Shammas, Mac C++ programming in 21 days	//strtok() modifies its argument string so	//must make a non-constant copy of *smat.its_content_h		LONG		contents_size = ::GetHandleSize(smat.its_content_h);	SignalIf_(contents_size==0);	CHAR  * 	cstr = new CHAR[contents_size];	//delete: at the end of this method	ThrowIfNil_(cstr);	StHandleState saved_h(smat.its_content_h); //preserve handle state	::HLock(smat.its_content_h);//saved handle state	::BlockMoveData(*smat.its_content_h, cstr, contents_size);	::HUnlock(smat.its_content_h);//saved handle state	//smat.its_content_h, if not empty, should contain a cstr, with null termination	SignalIf_(cstr[contents_size-1]!='\0');	STableCell	cell=smat.its_anchor;	USHORT		first_col= cell.col;	CHAR  * 	delims = "\t\r";		CHAR  *		token_cstr;	LStr255		pstr;	CHAR  * 	ch_p;	UnselectAllCells();//only pasted cells should be selected	for(USHORT r=0; r<smat.its_num_of_rows;r++){		for(USHORT c=0; c<smat.its_num_of_cols; c++){			if(r==0 && c==0){				ch_p = cstr;			}else{				ch_p = 0;			}			token_cstr = strtok(ch_p, delims );			pstr = token_cstr;			set_cell_data(cell, pstr);			SelectCell(cell);//all pasted cells should be selected			cell.col++;		}		cell.col = first_col;		cell.row++;	}		get_cell_data(its_editor_p->get_cell(), pstr);	its_editor_p->SetDescriptor( pstr );		delete []	cstr;}#pragma mark --copy, del, cut, paste--//The methods below do absolutely no checking.//They are used only after extensive checking has been performed.//******************************************VOID	C_EDIT_TABLE::copy_selection(C_SUB_MATRIX	 &		smat, 			//i-oBOOLEAN					fill_smat)		//in{	//This function is analogous to C_MAIN_VIEW::copy_selection().		//When this function is called,	//there is always more than one table cell selected.	//Thus, we can't use copying in table with nothing selected to clear	//scrap and table's uaction memory. However, closing the nd_pi	//window will clear the table's uaction memory.	//And copying in the main window with nothing selected	//or pressing Go_Forward will clear the scrap.				if(fill_smat){		resize_and_anchor_sub_mat_like_sel(smat);		write_sub_mat(smat);	}	UScrap::SetData('TEXT', smat.its_content_h);}//******************************************VOID	C_EDIT_TABLE::delete_selection(){	//This method is analogous to C_MAIN_VIEW::delete_selection().		//Next method returns (0,0) if no cell is selected.	STableCell 	cell = GetFirstSelectedCell();		if(!IsValidCell(cell))return;	delete_cell_data(cell);	while(GetNextSelectedCell(cell)){		delete_cell_data(cell);	}	//	its_editor_p->SetDescriptor("\p");}	#pragma mark --command center--//******************************************BOOLEAN   C_EDIT_TABLE::HandleKeyPress(const EventRecord  & in_key_event)		//in{	//MONITOR_THE_CHANGE_HERE		if( ((in_key_event.message &keyCodeMask)>>8) == 0x33){//delete key was pressed		//This is like a combination of ObeyCommand() and FindCommandStatus()		if(get_sel_mgr()==table_mgr){			return ProcessCommand(cmd_Clear);		}else{			return true;		}	}else{		return LCommander::HandleKeyPress(in_key_event);	}}//******************************************BOOLEAN		C_EDIT_TABLE::ObeyCommand(CommandT	inCommand,		//inVOID  *		ioParam)		//i-o{	//MONITOR_THE_CHANGE_HERE	if(mTableSelector==0) return LCommander::ObeyCommand(inCommand, ioParam);		BOOLEAN				obeyed = true;	BOOLEAN				needs_refresh = true;	//Imp: all checking indicated explicitly in this function C_EDIT_TABLE::ObeyCommand()	//and in C_CELL_EDITOR::Obey_Command(). 	//Also in C_EDIT_TABLE::FindCommandStatus() 	//and C_CELL_EDITOR::FindCommandStatus()	//No checking done inside C_ETABLE_DEL_OR_CUT_ACTION	//or C_ETABLE_PASTE_ACTION or copy_selection().		if(!unload_editor())return false;	//FindCommandStatus() makes sure that there is at least	//one selected cell for the commands marked //**	switch (inCommand){		case cmd_Cut:{ //**			its_editor_p->PostAction(0);			PostAction(new	C_ETABLE_DEL_OR_CUT_ACTION(this, (C_ETABLE_MULTI_SEL  *)mTableSelector, 1));//= new			//delete:			break;		}		case cmd_Copy:{ //**			C_SUB_MATRIX	smat;			copy_selection(smat, true);			break;		}		case cmd_Paste:{			C_SUB_MATRIX 		x;			C_SUB_MATRIX  * 	smat_p;			if(!its_editor_p->IsTarget()){//the editor is not target so it can't fill smat.				smat_p = &x;				if(!smat_p->absorb_clipping())break;								if((smat_p->its_num_of_cols)*(smat_p->its_num_of_rows)==1)break;//this case should				//be handled by cell editor but he can't because he is not on duty.			}else{//editor is target and filled smat_p				smat_p = (C_SUB_MATRIX  *)ioParam;			}			if(sub_mat_can_be_pasted(*smat_p)){				smat_p->its_anchor = GetFirstSelectedCell();				its_editor_p->PostAction(0);				PostAction(new	C_ETABLE_PASTE_ACTION(this, smat_p) );//= new				//delete:			}			break;		}				case cmd_Clear://**			its_editor_p->PostAction(0);			PostAction(new	C_ETABLE_DEL_OR_CUT_ACTION(this, (C_ETABLE_MULTI_SEL  *)mTableSelector, 0));//= new			//delete:			break;		case cmd_SelectAll://**			SelectAllCells();			break;		default:			obeyed = LCommander::ObeyCommand(inCommand, ioParam);			needs_refresh = false;			break;	}			if (needs_refresh) Refresh();	return obeyed;}//******************************************VOID	C_EDIT_TABLE::FindCommandStatus(CommandT		inCommand,		//inBOOLEAN		&	outEnabled,		//outBOOLEAN		&	outUsesMark,	//outUInt16		&	outMark,		//outStr255			outName_pstr)	//out{	if(mTableSelector==0){		 LCommander::FindCommandStatus(inCommand, outEnabled, outUsesMark, outMark, outName_pstr);		 return;	}	outEnabled = false;			outUsesMark = false;	outMark = noMark;	SEL_MANAGER		sel_mgr = get_sel_mgr();	 		switch (inCommand) {		case cmd_Cut:			if(sel_mgr==table_mgr)outEnabled = true;			break;		case cmd_Copy:			if(sel_mgr==table_mgr)outEnabled = true;			break;		case cmd_Paste:			if(UScrap::HasData('TEXT'))outEnabled = true;			break;				case cmd_Clear:			if(sel_mgr==table_mgr)outEnabled = true;			break;			case cmd_SelectAll:			if(sel_mgr==table_mgr)outEnabled = true;			break;		default:							LCommander::FindCommandStatus(inCommand, outEnabled, outUsesMark, outMark, outName_pstr);			break;	}	}