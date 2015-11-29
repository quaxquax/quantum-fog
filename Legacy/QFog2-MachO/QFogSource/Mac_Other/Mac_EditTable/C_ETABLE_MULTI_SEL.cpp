#include  "C_ETABLE_MULTI_SEL.h"//******************************************C_ETABLE_MULTI_SEL::C_ETABLE_MULTI_SEL(LTableView  * 	tv_p,		//inUSHORT			first_scol,	//inUSHORT			first_srow)	//in	:LTableMultiSelector(tv_p),	its_first_selectable_col(first_scol),	its_first_selectable_row(first_srow){}//******************************************C_ETABLE_MULTI_SEL::~C_ETABLE_MULTI_SEL(){}#pragma mark --don t draw--//******************************************USHORT	C_ETABLE_MULTI_SEL::get_sel_size()const{	Rect 	r = mSelectionRgn.Bounds();	//we only use rectangular selection regions	return (r.bottom-r.top)*(r.right-r.left);}//******************************************VOID	C_ETABLE_MULTI_SEL::set_sel_rect(const Rect  &	rect)	//in{	TableIndexT		num_of_rows, num_of_cols;	mTableView->GetTableSize(num_of_rows, num_of_cols);	STableCell	cell(1,1);	for(USHORT	r=1; r<=num_of_rows; r++){		for(USHORT	c=1; c<=num_of_cols; c++){			if( c>=rect.left && c< rect.right  && r>=rect.top && r<rect.bottom ){				SelectCell(cell);			}else{				UnselectCell(cell);			}			cell.col++;		}		cell.col = 1;		cell.row++;	}}#pragma mark --draw--//******************************************VOID	C_ETABLE_MULTI_SEL::SelectAllCells(){//NONTRIVIAL EXCERPT FROM PP	TableIndexT	rows, cols;	mTableView->GetTableSize(rows, cols);		if ((rows >0) && (cols > 0)) {			// Table has some cells											// Unhilite existing selection		mTableView->HiliteSelection(mTableView->IsActive(), false);											// Put all cells in selection		::SetRectRgn(mSelectionRgn, its_first_selectable_col, its_first_selectable_row, //diff			 SInt16(cols) + 1, SInt16(rows) + 1);//diff		mAnchorCell.SetCell(its_first_selectable_row, its_first_selectable_col); //diff		mTableView->HiliteSelection(mTableView->IsActive(), true);		mTableView->SelectionChanged();	}}//******************************************void	C_ETABLE_MULTI_SEL::ClickSelect(const STableCell		&	inCell,			//inconst SMouseDownEvent	&	inMouseDown)	//in{//NONTRIVIAL EXCERPT FROM PP//Too many diffs to mark them.//This function is called inside DragSelect()//Unlike PP, I will not allow disconnected or non-rectangular selection regions.	SignalIf_(!is_selectable_cell(inCell));	/*			if (inMouseDown.macEvent.modifiers & shiftKey) {		if(!is_selectable_cell(mAnchorCell)) {			if (!CellIsSelected(inCell)) {				UnselectAllCells();				SelectCell(inCell);			}			mAnchorCell = inCell;		}else{// anchor is selectable			STableCell	saved_anchor = mAnchorCell;			UnselectAllCells();			mAnchorCell = saved_anchor;			SelectCellBlock(mAnchorCell, inCell);					}										}else{						// No signficant modifier keys		if (!CellIsSelected(inCell)) {			UnselectAllCells();			SelectCell(inCell);		}		mAnchorCell = inCell;	}*///the following code is equivalent but simpler than the commented code	if((inMouseDown.macEvent.modifiers & shiftKey) && is_selectable_cell(mAnchorCell)) {		STableCell	saved_anchor = mAnchorCell;		UnselectAllCells();		mAnchorCell = saved_anchor;		SelectCellBlock(mAnchorCell, inCell);				}else{			if (!CellIsSelected(inCell)) {			UnselectAllCells();			SelectCell(inCell);		}		mAnchorCell = inCell;	}}//******************************************Boolean		C_ETABLE_MULTI_SEL::DragSelect(const STableCell		&	inCell,			//inconst SMouseDownEvent	&	inMouseDown)	//in{//NONTRIVIAL EXCERPT FROM PP//too many diffs to mark them	SignalIf_(!is_selectable_cell(inCell));		//Can't proceed without a selectable anchor.	//If inCell is selectable, this will set up a selectable anchor.	ClickSelect(inCell, inMouseDown);				//I will ignore command key	STableCell	currCell = inCell;		STableCell	hitCell = inCell;	SPoint32	imageLoc;	Point		mouseLoc;		while (::StillDown()) {		mTableView->FocusDraw();		::GetMouse(&mouseLoc);		if (mTableView->AutoScrollImage(mouseLoc)) {			mTableView->FocusDraw();			Rect	frame;			mTableView->CalcLocalFrameRect(frame);			SInt32 pt = ::PinRect(&frame, mouseLoc);			mouseLoc = *(Point*)&pt;		}		mTableView->LocalToImagePoint(mouseLoc, imageLoc);		mTableView->GetCellHitBy(imageLoc, hitCell);				if (currCell != hitCell) {			if (is_selectable_cell(hitCell)) {				STableCell	saved_anchor = mAnchorCell;				UnselectAllCells();				mAnchorCell = saved_anchor;				SelectCellBlock(mAnchorCell, hitCell);				currCell = hitCell;									}		}	}		//PP returns whether stayed within inCell all the time. 	//I return whether ended within anchor cell,	//which would mean the final selection is a single cell	return 	mAnchorCell==hitCell;}