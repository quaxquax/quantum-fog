#include 	"EXCEPTIONS.h"//******************************************VOID 	my_gen_alert(const STRINGY  & 	stringy)	//in	{#ifdef   _ansi_app //''''''''''''''''''''''''''''''''''''''''''''\\.	cout<<stringy<<endl;#endif //_ansi_app \\............................................//#ifdef   _mac_gui_app //''''''''''''''''''''''''''''''''''''''''''''\\.	LStr255	 str = stringy.get_string();		UCursor::InitTheCursor();	::ParamText(str, "\p", "\p", "\p");	UDesktop::Deactivate();	::CautionAlert(gen_alert_rid, 0);	UDesktop::Activate();	#endif //_mac_gui_app \\............................................//}#ifdef   _mac_gui_app //''''''''''''''''''''''''''''''''''''''''''''\\.//******************************************VOID 	amp_generator_found_no_states_alert()	{	my_gen_alert("Operation cancelled. For the given parameters, there is NO possible focus-node state with a non-zero amplitude.");}//******************************************VOID 	only_allowed_in_initial_run_state_alert()	{	my_gen_alert("This operation is allowed only if the program is in the initial run-state. The program is not currently in that state. To place it there, press \"Go Back To Initial State\" in the \"Go\" menu.");}#endif //_mac_gui_app \\............................................//#pragma mark --IO_EXCEPTION--//******************************************IO_EXCEPTION::IO_EXCEPTION(){}//******************************************IO_EXCEPTION::~IO_EXCEPTION(){}//******************************************VOID  IO_EXCEPTION::print_error(const STRINGY  &	file_name,	//inUSHORT 				line_num)	//inconst{#pragma unused(file_name, line_num)}#pragma mark --UNLISTED_NODE--//******************************************UNLISTED_NODE::UNLISTED_NODE(const STRINGY  &	stringy)	//in	:its_nd_name(stringy){}//******************************************UNLISTED_NODE::~UNLISTED_NODE(){}//******************************************VOID	UNLISTED_NODE::print_error(const STRINGY  & file_name,		//inUSHORT line_num)				//inconst{	my_gen_alert(		STRINGY("Bad Input: Check file ")		&& file_name		&& ", line "		&& line_num		&& ". "		&& "'" 		&& its_nd_name 		&& "' is not a node."	);}#pragma mark --UNLISTED_STATE--//******************************************UNLISTED_STATE::UNLISTED_STATE(const STRINGY  &	nd_name,	//inconst STRINGY  &	st_name)	//in	:its_nd_name(nd_name),	its_st_name(st_name){}//******************************************UNLISTED_STATE::~UNLISTED_STATE(){}//******************************************VOID	UNLISTED_STATE::print_error(const STRINGY  & file_name,				//inUSHORT line_num)						//inconst{	my_gen_alert(		STRINGY("Bad Input: Check file ")		&& file_name		&& ", line "		&& line_num		&& ". "		&& "'" 		&& its_st_name 		&& "' is not a state of node '"		&& its_nd_name		&& "'."				);}#pragma mark --UNLISTED_IN_NODE--//******************************************UNLISTED_IN_NODE::UNLISTED_IN_NODE(const STRINGY  &	nd_name,	//inconst STRINGY  &	in_nd_name)	//in	:its_nd_name(nd_name),	its_in_nd_name(in_nd_name){}//******************************************UNLISTED_IN_NODE::~UNLISTED_IN_NODE(){}//******************************************VOID	UNLISTED_IN_NODE::print_error(const STRINGY  & file_name,				//inUSHORT line_num)						//inconst{	my_gen_alert(		STRINGY("Bad Input: Check file ")		&& file_name		&& ", line "		&& line_num		&& ". "		&& "'" 		&& its_in_nd_name 		&& "' is not a parent of node '"		&& its_nd_name		&& "'."				);}#pragma mark --ILLEGAL_NAME--//******************************************ILLEGAL_NAME::ILLEGAL_NAME(const STRINGY  &	stringy)	//in	:its_name(stringy){}//******************************************ILLEGAL_NAME::~ILLEGAL_NAME(){}//******************************************VOID	ILLEGAL_NAME::print_error(const STRINGY  & 	file_name,			//inUSHORT 	line_num)						//inconst{	my_gen_alert(		STRINGY("Bad Input: Check file ")		&& file_name		&& ", line "		&& line_num		&& ". "		&& "'"		&& its_name 		&& "' is not a legal name. Legal names cannot contain white-space, or the character '#', and they must have 31 or less characters."	);}#pragma mark --ZERO_DEGEN--//******************************************ZERO_DEGEN::ZERO_DEGEN(const STRINGY  &	stringy)	//in	:its_nd_name(stringy){}//******************************************ZERO_DEGEN::~ZERO_DEGEN(){}//******************************************VOID	ZERO_DEGEN::print_error(const STRINGY  & file_name,			//inUSHORT line_num)					//inconst{	my_gen_alert(		STRINGY("Bad Input: Check file ")		&& file_name		&& ", line "		&& line_num		&& ". "		&& "You've assigned zero states to node '" 		&& its_nd_name 		&& "'. Nodes with zero states are forbidden."	);}#pragma mark --WRONG_PATTERN--//******************************************WRONG_PATTERN::WRONG_PATTERN(){}//******************************************WRONG_PATTERN::~WRONG_PATTERN(){}//******************************************VOID	WRONG_PATTERN::print_error(const STRINGY  & file_name,				//inUSHORT line_num)						//inconst{	my_gen_alert(		STRINGY("Bad Input: Check file ")		&& file_name		&& ", line "		&& line_num		&& ". "		&& "This line does not conform with the expected pattern." 	);}#pragma mark --ILLEGAL_AMP--//******************************************ILLEGAL_AMP::ILLEGAL_AMP(const DOUBLE  &		x,	//inconst DOUBLE  &		y)	//in	:its_x(x),	its_y(y){}//******************************************ILLEGAL_AMP::~ILLEGAL_AMP(){}//******************************************VOID	ILLEGAL_AMP::print_error(const STRINGY  & file_name,				//inUSHORT line_num)						//inconst{	my_gen_alert(		STRINGY("Bad Input: Check file ")		&& file_name		&& ", line "		&& line_num		&& ". "		&& "The amplitude (" 		&& its_x		&& ", "		&& its_y		&& " ) is inadmissible because its magnitude is greater than 1."	);}