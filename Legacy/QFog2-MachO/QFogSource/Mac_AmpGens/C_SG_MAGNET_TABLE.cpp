#include "C_SG_MAGNET_TABLE.h"#include "C_SG_MAGNET_AMP_GEN.h"#include "SG_MAGNET.h"#include "EXCEPTIONS.h"#include "C_EDIT_TABLE.h"#include "QB_NET.h"#include "StCPTstate.h"#pragma mark --creation/destruction--//******************************************C_SG_MAGNET_TABLE::C_SG_MAGNET_TABLE(LStream	 * 	inStream)		//in	:C_EDIT_TABLE(inStream),	its_net_p(0),	its_amp_gen_p(0){		LTableMultiGeometry  *	geo_p		= new LTableMultiGeometry(this, 70 /* cell width */, 20/*cell height*/);	//delete:	in ~LTableView().	ThrowIfNil_(geo_p);	SetTableGeometry(geo_p);// total width = 310 = 150 + 20 + 2(70)	InsertCols(		4, //how many		0, //after which col		0, //data pointer		0, //data size 		false); //refresh	SetColWidth(		150,//width		1, //from this col		1);//to this col	SetColWidth(		20,//width		2, //from this col		2);//to this col		}//******************************************C_SG_MAGNET_TABLE::~C_SG_MAGNET_TABLE(){}//******************************************VOID	C_SG_MAGNET_TABLE::init(USHORT 					nd_id,		//inQB_NET	 *				net_p,		//inC_SG_MAGNET_AMP_GEN * 	amp_gen_p)	//in{	//its_xxxx are used to keep temporary values,	//that won't be passed into the SG_MAGNET object until 	//the user presses OK. They won't be passed at all if the user 	//presses Cancel.		its_nd = nd_id;	its_net_p =  net_p;	its_amp_gen_p = amp_gen_p;	SG_MAGNET  *  nd_p = (SG_MAGNET  *)its_net_p->get_nd_ptr(its_nd);		its_spins = its_amp_gen_p->get_spins();	its_thetas = its_amp_gen_p->get_thetas();	its_phis = its_amp_gen_p->get_phis();	InsertRows(		nd_p->get_num_of_in_nds(), //how many		0, 	//after which row		0,  //data pointer		0, 	//data size 		true); //refresh		}#pragma mark --about one cell--//******************************************VOID	C_SG_MAGNET_TABLE::get_cell_data(const STableCell  &  cell,		//inLStr255  &   str)				//out{	USHORT r = cell.row -1;	ThrowIf_(cell.col==1);	switch(cell.col){					//case 1: won't be called		case 2:			if(its_spins[r] == -1)str = "\p-";			if(its_spins[r] ==  0)str = "\p-+";			if(its_spins[r] ==  1)str = "\p+";			break;		case 3:			str.Assign(its_thetas[r], FIXEDDECIMAL, 15);			str[0] = 15;			break;		case 4:			str.Assign(its_phis[r], FIXEDDECIMAL, 15);			str[0] = 15;			break;	}}//******************************************VOID	C_SG_MAGNET_TABLE::set_cell_data(const STableCell  &   cell,		//inconst LStr255  &   str)			//in{	// This method changes cell storage and appearance.	// This method won't be called for col=1.	ThrowIf_(cell.col==1);	USHORT	r = cell.row -1;	DOUBLE	ang;	switch(cell.col){		case 2:			if(str == "\p-"){				its_spins[r] = -1;			}else if(str == "\p-+"){				its_spins[r] = 0;			}else if(str == "\p+"){				its_spins[r] = 1;			}			break;		case 3:			ang = double_t(str);			limited_degrees(ang);			its_thetas[r] = ang;			break;		case 4:			ang = double_t(str);			limited_degrees(ang);			its_phis[r] = ang;			break;	}	RefreshCell(cell);	}		//******************************************VOID	C_SG_MAGNET_TABLE::DrawCell(const STableCell  &		inCell,			//inconst Rect	 &		inLocalRect)		//in{//drawer	StCPTstate	saved_st(true, false, true);	saved_st.Normalize();	 	::TextSize(its_font_size);	::TextFont(its_font_num);	::MoveTo(inLocalRect.left + 4, inLocalRect.bottom - 4);	LStr255  		str;	USHORT	r = inCell.row -1;		switch(inCell.col){		case 1:			SG_MAGNET  *  nd_p=(SG_MAGNET  *)its_net_p->get_nd_ptr(its_nd);			USHORT in_nd = nd_p->get_in_nds()[r];			its_net_p->get_nd_ptr(in_nd)->get_name().fill_LStr255(str);			trunc_pstr_so_it_fits(str, GetColWidth(1), 8);			break;		case 2:			get_cell_data(inCell, str);						break;		case 3:		case 4:			get_cell_data(inCell, str);			str[0] = 8;			break;	}	::DrawString(str);				draw_cell_frame(inLocalRect);//see GLOBAL_FUN.h	}//******************************************BOOLEAN		C_SG_MAGNET_TABLE::str_is_valid_cell_entry(const STableCell  &  cell,		//inStr255 	pstr)					//in{	if(cell.col>=3){			if(!STRINGY(pstr).is_double()){			my_gen_alert("Illegal table-entry. The entry should be a real number but isn't. Please fix before continuing.");						return false;		}	}			return true;}//******************************************VOID	C_SG_MAGNET_TABLE::ClickCell(const STableCell  &  inCell,					//inconst SMouseDownEvent  &	inMouseDown)		//in{	//MONITOR_THE_CHANGE_HERE	//If click on a cell out of bounds for the editor	//editor's cell is set to nullcell.	//If the editor's cell is null,	//unload_editor() does nothing and returns true 		LStr255  str;	switch(inCell.col){		case 1:			if(its_editor_p->get_cell()==inCell)break;			if(!unload_editor())break;			nullify_editor();			break;		case 2:			//Can't have this:			//if(its_editor_p->get_cell()==inCell)return;			//because clicking a second time on the same cell			//should toggle the spin of the state.			if(!unload_editor())break;			nullify_editor();						get_cell_data(inCell, str);			if(str[0]==1){				if(str[1]=='-'){// spin = -					set_cell_data(inCell, "\p+");				}else{//spin = +					set_cell_data(inCell, "\p-");				}			}			//do nothing for str = - +			break;		case 3:		case 4:			C_EDIT_TABLE::ClickCell(inCell, inMouseDown);			//Note that C_EDIT_TABLE::ClickCell() 			//sets to 31 			//the max num of characters that			//the cell editor will accept.			//In the case of columns 3 and 4,			//we don't want the default value			//so we change it.			set_max_chars_of_cell_editor(15);			break;	}}#pragma mark --about many cells--//******************************************VOID	C_SG_MAGNET_TABLE::get_spin_theta_phi_vecs(VECTOR<SHORT> & spins,		//outVECTOR<DOUBLE> & thetas,	//outVECTOR<DOUBLE> & phis)		//out{	spins = its_spins;	thetas = its_thetas;	phis = its_phis;}		