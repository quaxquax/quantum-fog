#pragma once#include "QFog_constants.h"#include "C_EDIT_TABLE.h"#include "TRANS_MAT.h"#include "C_TOP_PTRS.h"//******************************************class	C_PI_AMPS_TABLE : public C_EDIT_TABLE, public LBroadcaster, public C_TOP_PTRS{protected:	USHORT 			its_nd;	TM_COL_NUM		its_in_st;	COORD_SYSTEM	its_coord_sys; //COORD_SYS defined in "QFog_constants.h"public:		C_PI_AMPS_TABLE(LStream * inStream);	virtual  ~C_PI_AMPS_TABLE();		virtual VOID		get_cell_data(const STableCell  & inCell, LStr255  & str);	virtual VOID		set_cell_data(const STableCell  & inCell, const LStr255  & str);	virtual	BOOLEAN		str_is_valid_cell_entry(const STableCell  &  cell, Str255 	pstr);	virtual VOID		delete_cell_data(const STableCell  & inCell);			VOID 	DrawCell(const STableCell  & inCell, const Rect	 & inLocalRect);	VOID  	ClickCell(const STableCell &  inCell, const SMouseDownEvent &	inMouseDown);		VOID 	set_in_st(TM_COL_NUM 	new_in_st);	VOID 	set_nd_always(USHORT new_nd);		VOID 	set_coord_sys(COORD_SYSTEM  sys);		VOID	activate_all_states();	VOID	deactivate_all_states();		VOID	read_sub_mat(const C_SUB_MATRIX  & 	smat);	VOID	delete_selection();};