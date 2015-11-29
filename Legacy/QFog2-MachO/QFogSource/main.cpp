#include "QFog_constants.h"#if  _do_debug	LONG 	req_stack_space;#endif//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%#ifdef   _ansi_app //''''''''''''''''''''''''''''''''''''''''''''\\.#include "QB_NET.h"#include "STRINGY.h"#include <ctype.h>int main(){	SetDebugThrow_(debugAction_Alert);	SetDebugSignal_(debugAction_Alert);	 	QB_NET net; 	 	MY_IFSTREAM	 topo_strm; 	topo_strm.open("topology");	if( !topo_strm.is_open() ) { cerr << "'topology' file is missing."; exit(1);}	net.read_topology_file(topo_strm);	topo_strm.close();			//Must create nd_name_dict: 	//after loading node names, but	//before reading nd_sts_strm	//Dictionaries destroyed at the end of main(). 	net.create_nd_name_dictionary(); 	MY_IFSTREAM	 nd_sts_strm; 	nd_sts_strm.open("node_states"); 	if( !nd_sts_strm.is_open() ) { cerr << "'node_states' file is missing."; exit(1);}	 	net.read_nd_sts_file(nd_sts_strm);	nd_sts_strm.close();		//Must create nd_sts_dictionaries: 	//after loading names of node states, but	//before reading amps_strm	//Dictionaries destroyed at the end of main(). 	net.create_nd_st_dictionaries();	  	MY_IFSTREAM	 amps_strm; 	amps_strm.open("amplitudes"); 	if( !amps_strm.is_open() ) { cerr << "'amplitudes' file is missing."; exit(1); }	net.read_amps_file(amps_strm);	amps_strm.close();		MY_IFSTREAM  rt_strm;	rt_strm.open("run_type");//rt = run type 	if( !rt_strm.is_open() ) { cerr << "'run_type' file is missing."; exit(1); }	rt_strm.ignore(500, my_endline);	USHORT	 has_stories_file;	rt_strm>> has_stories_file;	rt_strm.close();	  	MY_IFSTREAM 	evid_stream;	evid_stream.open("evid_cases");	 if( !evid_stream.is_open() ) { cerr << "'evid_cases' file is missing."; exit(1); }	net.good_ecases_file(evid_stream); //will quit if file not good.	//No need to check is_open() in the case of out streams.	//For out streams, if you try to open to a file "X" and "X" is not found,	//a new file named "X" is created.		 	MY_OFSTREAM  *  nd_probs_stream_p = new MY_OFSTREAM();	//delete: in this method	ThrowIfNil_(nd_probs_stream_p);	nd_probs_stream_p->open("posterior_nd_probs");	net.write_header_of_nd_probs_file(nd_probs_stream_p);		MY_OFSTREAM  *  bi_nd_probs_stream_p = new MY_OFSTREAM();	//delete: in this method	ThrowIfNil_(bi_nd_probs_stream_p);	bi_nd_probs_stream_p->open("posterior_bi_nd_probs");	MY_OFSTREAM  *  stories_stream_p = 0;	if(has_stories_file){	 	stories_stream_p = new MY_OFSTREAM();		//delete: in this method	 	ThrowIfNil_(stories_stream_p);		stories_stream_p->open("net_stories");	}		CHAR   		ecase_name_cstr[64];	STRINGY   	nd_name;	STRINGY   	nd_st_name;	USHORT  	nd_id;	USHORT		nd_st_id; 	CHAR  		ch;	BOOLEAN 	more_nodes;		//Below, we use a  /**/  to indicate a line	//which does not appear in good_ecases_file().	try{		ch=my_endline;		while( ch!='#' ){ evid_stream.my_get(ch); }		while(ch=='#'){//loop over ecases/**/		//default active-state list	/**/		net.fill_act_st_sets_of_all_nds();			evid_stream >> ecase_name_cstr;			evid_stream.my_get(ch);			while(ch!=my_endline){				evid_stream.my_get(ch);			}			while(isspace(ch)){evid_stream.my_get(ch);}			if(ch=='#'){				more_nodes = false;			}else{				more_nodes = true;				evid_stream.putback(ch);			}								while(more_nodes){ //loop over nodes with non default active-state set				evid_stream >> nd_name;				nd_id = net.get_dict().get_id_of_this_stringy(nd_name);/**/			//default active-state list does not apply to nd_id/**/			net.get_nd_ptr(nd_id)->empty_act_st_set();				evid_stream.my_get(ch);				while(ch!=my_endline){ //loop over the active state names of a single node					if(isspace(ch)){						evid_stream.my_get(ch);					}else{						evid_stream.putback(ch);						evid_stream >> nd_st_name;						nd_st_id = net.get_nd_ptr(nd_id)->get_dict().get_id_of_this_stringy(nd_st_name);/**/					net.activate_a_nd_state(nd_id, nd_st_id);						evid_stream.my_get(ch);					}				}//end of loop over state names				while(isspace(ch)){evid_stream.my_get(ch);}				if(ch=='#'){					more_nodes = false;				}else{					more_nodes = true;					evid_stream.putback(ch);				}								}//end of loop over nodes with non default active-state set/**/		net.go_forward(ecase_name_cstr, /**/			nd_probs_stream_p, bi_nd_probs_stream_p, stories_stream_p, 0 );						}//end of loop over ecases	}// end of try{}	catch(END_OF_FILE){/**/	//do the last ecase/**/	net.go_forward(ecase_name_cstr, /**/		nd_probs_stream_p, bi_nd_probs_stream_p, stories_stream_p, 0 );					}		evid_stream.close();	net.kill_dictionaries();	nd_probs_stream_p->close();	delete  nd_probs_stream_p;	nd_probs_stream_p = 0;		bi_nd_probs_stream_p->close();	delete  bi_nd_probs_stream_p;	bi_nd_probs_stream_p = 0;		if(has_stories_file){		stories_stream_p->close();		delete  stories_stream_p;		stories_stream_p = 0;	}}#endif //_ansi_app \\............................................//#pragma mark ----used only with macgui---- //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//NONTRIVIAL EXCERPT FROM PP //see  CDebugApp.cp#ifdef   _mac_gui_app //''''''''''''''''''''''''''''''''''''''''''''\\.	#include "C_APP.h"#include <UMemoryEater.h>#include <MetroNubUtils.h>VOID	mac_main();VOID 	main(VOID){	//don't want exceptions to propagate outside of here	try {		mac_main();	} catch (...) {		SignalPStr_("\pException caught in main!!!");	}}//******************************************VOID	mac_main(){#if _do_debug	// if we're under the MW debugger, we'll use the debugger for everything	// (throw, signal, DebugNew).  Else we'll use alerts for it all		if ( AmIBeingMWDebugged() )	{		// I prefer to use the low level debugger for everything, justr		// because SysBreak can cause some Mixed Mode ugliness when		// trying to view your stack crawl. ::Debugger/::DebugStr is a 		// better thing to use.				UDebugging::gDebugSignal = debugAction_LowLevelDebugger;		UDebugging::gDebugThrow = debugAction_LowLevelDebugger;	}	else	{		// not under the MW Debugger, so we'll use alerts. If you use		// another debugger, you could set this up the same way. Point		// is to use the debugger's facilities when under the debugger,		// else alerts.				UDebugging::gDebugSignal = debugAction_Alert;		UDebugging::gDebugThrow = debugAction_Alert;		// we'll use our own error handler for DebugNew.. uses alerts		// instead of DebugStr calls.			#if PP_DEBUGNEW_SUPPORT			UDebugNew::InstallDefaultErrorHandler();		#endif	}#else	// we're in final build mode, so nothing should be seen!		UDebugging::gDebugSignal = debugAction_Nothing;	UDebugging::gDebugThrow = debugAction_Nothing;	#endif		// There are some objects that the MSL creates at static	// initialization time. These are totally kosher and of course	// must stick around for the duration, but they do clutter up	// the leaks log that Spotlight produces. Let's just do a few	// things to clean up the log for those 3 leaks:#if  _do_debug		{		SLResetLeaks_();		#if PP_DEBUGNEW_SUPPORT			UDebugNew::Forget();		#endif	}#endif	//initializations	#if  _do_debug		req_stack_space= max_long;	//note this is absent from CDebugSample	#endif	//SetDebugThrow_(debugAction_Alert);	//SetDebugSignal_(debugAction_Alert);	InitializeHeap(32);			//note this is different from CDebugSample	UQDGlobals::InitializeToolbox();	::InitCursor();	::FlushEvents(everyEvent, nil);		// we will remember our grow zone object	//so we can delete it later.		LGrowZone * zone_p = new LGrowZone(20000);	//delete: no need to delete while application running	{				C_APP 	my_app;		my_app.Run();	}#if  _do_debug	// this cleanup isn't necessary in the shipping application.	// we just clean up to keep things like DebugNew and	// Spotlight quiet.		LMenuBar * theBar = LMenuBar::GetCurrentMenuBar();	delete theBar;	URegistrar::DisposeClassTable();	LPeriodical::DeleteIdlerAndRepeaterQueues();	UMemoryEater::DeleteMemoryLists();	LModelDirector * theDirector = LModelDirector::GetModelDirector();	delete theDirector;	LModelObject::DestroyLazyList();	UScreenPort::Dispose();		LComparator * theCompare = LComparator::GetComparator();	delete theCompare;		LLongComparator * theLongCompare = LLongComparator::GetComparator();	delete theLongCompare;	DisposeOf_(zone_p);	/*#if PP_Target_Classic	THPrint		printRecordH = UPrintingMgr::GetDefaultPrintRecord();//diff	DisposeHandle_((Handle)printRecordH);//diff#endif*/	#if PP_DEBUGNEW_SUPPORT		UDebugNew::Report();		#endif#endif	//_do_debug}#endif //_mac_gui_app \\............................................//