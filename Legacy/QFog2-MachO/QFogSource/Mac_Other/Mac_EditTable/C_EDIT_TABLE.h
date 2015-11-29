#pragma once#include "C_CELL_EDITOR.h"#include "C_SUB_MATRIX.h"#include "C_ACTION_FLAGS.h"enum SEL_MANAGER {editor_mgr, table_mgr};//selection manager is either the cell editor or the etable//******************************************class	C_EDIT_TABLE : public LTableView, public LCommander, public C_ACTION_FLAGS{protected:	C_CELL_EDITOR  *   	its_editor_p;	SInt16				its_font_size;	SInt16				its_font_num;	SEL_MANAGER			its_sel_mgr;//sel_mgr = selection manager	enum {data_size=32};public:	VOID 	set_text_traits_rid(ResIDT rid);	VOID	init();	VOID	FinishCreateSelf();	C_EDIT_TABLE(LStream * inStream);	virtual  ~C_EDIT_TABLE();	VOID			set_max_chars_of_cell_editor(USHORT	num);		BOOLEAN 		cell_editor_is_target() const;	VOID			nullify_editor();	virtual VOID 		get_cell_data(const STableCell  & inCell, LStr255  & str);	virtual VOID 		set_cell_data(const STableCell  & inCell, const LStr255  & str);	virtual BOOLEAN 	str_is_valid_cell_entry(const STableCell  &  cell, Str255 	pstr);	virtual VOID		delete_cell_data(const STableCell  & inCell);	virtual VOID  		DrawCell(const STableCell & inCell, const Rect & inLocalRect);			virtual BOOLEAN 	unload_editor();	virtual VOID 		install_editor_at( const STableCell  &  cell);	virtual VOID  		Click(SMouseDownEvent &	inMouseDown);	virtual VOID  		ClickSelf(const SMouseDownEvent &	inMouseDown);	virtual VOID  		ClickCell(const STableCell &  inCell, const SMouseDownEvent &	inMouseDown);	SEL_MANAGER		get_sel_mgr() const;	BOOLEAN				sub_mat_can_be_pasted(const C_SUB_MATRIX  &  smat);	VOID				resize_and_anchor_sub_mat_like_sel(C_SUB_MATRIX  &	smat);	VOID				write_sub_mat(C_SUB_MATRIX  &	smat);	virtual VOID		read_sub_mat(const C_SUB_MATRIX  & 	smat);	VOID				copy_selection(C_SUB_MATRIX  &  smat, BOOLEAN  fill_smat);	virtual VOID		delete_selection();	virtual BOOLEAN  	HandleKeyPress(const EventRecord  & in_key_event);	virtual BOOLEAN 	ObeyCommand(CommandT inCommand, VOID * ioParam);	virtual VOID		FindCommandStatus(CommandT inCommand,							BOOLEAN & outEnabled, BOOLEAN & outUsesMark,							UInt16 & outMark, Str255 outName_pstr);				};#pragma mark -//******************************************inlineVOID	C_EDIT_TABLE::set_max_chars_of_cell_editor(USHORT	num)	//in{	its_editor_p->SetMaxChars(num);}//******************************************inlineBOOLEAN		C_EDIT_TABLE::cell_editor_is_target() const{	return its_editor_p->IsTarget();}//******************************************inlineBOOLEAN		C_EDIT_TABLE::str_is_valid_cell_entry(const STableCell  &  cell,		//inStr255		pstr)				//in{	// should override this	return true;}