#include "L_LIST.cp"#include "BI_NODE.h"template class L_LIST<USHORT>;template class L_LIST<BI_NODE>;#ifdef   _mac_gui_app //''''''''''''''''''''''''''''''''''''''''''''\\.	#include "C_ARROW.h" 	#include "C_ANTHOLOGY_ITEM.h" 		template class L_LIST<C_ARROW>;	template class L_LIST<C_ANTHOLOGY_ITEM>;#endif //_mac_gui_app \\............................................//