#include 	"C_MVIEW_ACTIONS.h"#include  	"C_MAIN_VIEW.h"#include  	"C_DOC.h"/*	LAction(			ResIDT		inStringResID = STRx_RedoEdit,			SInt16		inStringIndex = str_RedoUndo,			Boolean		inAlreadyDone = false);*///******************************************C_NET_DEL_OR_CUT_ACTION::C_NET_DEL_OR_CUT_ACTION(USHORT				copy_to_scrap,		//inC_DOC  *  			doc_p,				//inC_MAIN_VIEW  *  	view_p,				//inQB_NET  *  			net_p)				//in	:LAction(my_redo_rid, undel_ind + copy_to_scrap, false),	its_copy_to_scrap(copy_to_scrap){	//IMP: we assume that string index for redo_cut is 1 more than that for redo_del		set_top_ptrs(doc_p, view_p, net_p);	its_num_of_init_sel_arrows = view_p->get_num_of_sel_arrows();		its_init_sel_arrows_are_first = false;	its_num_of_init_sel_nds = view_p->get_num_of_sel_nds();	its_mview_p->get_map_with_sel_last(its_map_with_sel_last);	its_net_p->reorder_nds(its_map_with_sel_last);	its_init_sel_nds_are_last = true;			LHandleStream   net_stream;	its_mview_p->write_net_stream(net_stream, undel_reco);//write_net	//detach handle so ~LHandleStream() doesn't destroy it	its_undel_reco_h = net_stream.DetachDataHandle();	//create_h:		PP creates its_undel_reco_h in DetachDataHandle() with call to ::NewHandle().	//throwif_h:	PP throwifs upon creation.	//destroy_h:	I destroy its_undel_reco_h in ~C_NET_DEL_OR_CUT_ACTION().	if(its_copy_to_scrap==1)its_mview_p->copy_selection();		its_mview_p->set_is_modified(false, mview_del);}//******************************************C_NET_DEL_OR_CUT_ACTION::~C_NET_DEL_OR_CUT_ACTION(){		DisposeHandle_(its_undel_reco_h);}//******************************************BOOLEAN		C_NET_DEL_OR_CUT_ACTION::CanRedo() const{		BOOLEAN		can_redo = false;	if(		mIsDone==false &&		its_mview_p->IsOnDuty() &&		!its_mview_p->is_modified(mview_del)	){		can_redo = true;	}	return can_redo;}//******************************************BOOLEAN		C_NET_DEL_OR_CUT_ACTION::CanUndo() const{	BOOLEAN		can_undo = false;	if(		mIsDone==true &&		its_mview_p->IsOnDuty() &&		!its_mview_p->is_modified(mview_del)	){		can_undo = true;	}	return can_undo;}//******************************************VOID	C_NET_DEL_OR_CUT_ACTION::RedoSelf(){		//This function is called both for first and	//subsequent deletes.			//If this is called as a consequence of pressing Delete,	//selected nodes are last at this point	//(because the action's creator reordered net so).	//But if it is called as a consequence of pressing Redo Delete,	//selected nodes will in general not be last	//(because UndoSelf() reordered net so). 	if(!its_init_sel_nds_are_last){		its_net_p->reorder_nds(its_map_with_sel_last);		its_init_sel_nds_are_last = true;		//undo any selection done from previous undo to this redo		its_mview_p->make_only_last_nds_selected(its_num_of_init_sel_nds);	}	//If this is called as a consequence of pressing Delete,	//selected arrows are not necessarily first at this point.	//But if it is called as a consequence of pressing Redo Delete,	//selected arrows will be first 	//(because UndoSelf() restored them at the beginning of	//the linked list QB_NET::its_arrows). 		if(its_init_sel_arrows_are_first){		its_mview_p->make_only_first_arrows_selected(its_num_of_init_sel_arrows);	}		its_mview_p->delete_selection(true);//calls Refresh() for main view		its_mview_p->set_is_modified(false, mview_del);}	//******************************************VOID	C_NET_DEL_OR_CUT_ACTION::UndoSelf(){//if(its_copy_to_scrap==1)ThrowIfOSErr_(::ZeroScrap());//better to zero scrap only immediately before writing new stuff to it.	LHandleStream   net_stream(its_undel_reco_h);	its_mview_p->read_net_stream(net_stream, undel_reco);//read_net	its_init_sel_arrows_are_first = true;	UI_MAP	inv_map;	its_map_with_sel_last.get_inverse(inv_map);	its_net_p->reorder_nds(inv_map);	its_init_sel_nds_are_last = false;	//Important: this makes the main view the initial target 	//rather than some node efield	LCommander::SwitchTarget((LCommander  *)its_mview_p);//edit field is target if don't include this.	its_mview_p->Refresh();	its_mview_p->refresh_pi_wind(0);	//detach handle so ~LHandleStream() doesn't destroy it	its_undel_reco_h = net_stream.DetachDataHandle();		its_mview_p->set_is_modified(false, mview_del);}