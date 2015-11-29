#include "SET.h"#pragma mark --creation/destruction--//******************************************VOID	SET::clear(){	delete [] its_array_p;	its_array_p = 0;}//******************************************VOID	SET::copy(const SET  &	s)		//in{	its_num_of_elems = s.its_num_of_elems;	its_num_of_comps = s.its_num_of_comps;	//its_num_of_comps >= 1	its_array_p = new LONG[its_num_of_comps];//new[]	//delete:	in ~SET()-> SET::clear()	ThrowIfNil_(its_array_p);	for(USHORT i=0; i<its_num_of_comps; i++){		its_array_p[i]= s.its_array_p[i];	}}//******************************************VOID	SET::empty(){	its_num_of_elems = 0;	for(USHORT i=0; i<its_num_of_comps; i++){		its_array_p[i]= 0;	}}//******************************************VOID	SET::resize(USHORT 	max_num_of_elems)	//in{	ThrowIf_(max_num_of_elems==0);	USHORT i;	USHORT  new_num_of_comps = (max_num_of_elems -1)/32 + 1;	//Take out all the elements that won't be needed anymore.	for(i=max_num_of_elems; i<32*its_num_of_comps; i++){		//extract() decreases its_num_of_elems by one		//every time it is successful.		extract(i);	}	//The above loop:	//(1)won't do anything if new_num_of_comps> its_num_of_comps.	//(2)is absolutely necessary for new_num_of_comps < = its_num_of_comps,	//(3)resets its_num_of_elements to its new value.	if(its_num_of_comps !=new_num_of_comps){			LONG  *  new_array_p = new LONG[new_num_of_comps];//new[]		//delete:	substitution of delete-new pair		ThrowIfNil_(new_array_p);		if(its_num_of_comps < new_num_of_comps){			//we've grown			for(i=0; i<its_num_of_comps; i++){				new_array_p[i] = its_array_p[i];			}			for(i=its_num_of_comps ;i<new_num_of_comps; i++){				new_array_p[i] = 0;			}		}else{			//we've shrunk			for(i=0; i<new_num_of_comps; i++){				new_array_p[i] = its_array_p[i];			}		}		delete [] its_array_p;		its_array_p =new_array_p;		its_num_of_comps =new_num_of_comps;	}	}//******************************************SET::SET(){	its_num_of_elems = 0;	its_num_of_comps = 1;	its_array_p = new LONG[its_num_of_comps];//new[]	//delete:	in ~SET	ThrowIfNil_(its_array_p);	its_array_p[0] = 0;}//******************************************SET::SET(USHORT 	max_num_of_elems)	//in{	ThrowIf_(max_num_of_elems==0);	its_num_of_elems = 0;	its_num_of_comps = (max_num_of_elems -1)/32 + 1;	its_array_p = new LONG[its_num_of_comps];//new[]	//delete:	in ~SET	ThrowIfNil_(its_array_p);	for(USHORT i=0; i<its_num_of_comps; i++){		its_array_p[i]= 0;	}}//******************************************SET::SET(const  SET  &  s){	copy(s);}//******************************************SET  & 	SET::operator=(const SET  &	rhs ){	if(this != &rhs){		clear();		copy(rhs);	}	return *this;}//******************************************SET::~SET(){	clear();}#pragma mark --const functions--//******************************************BOOLEAN 	SET::contains(USHORT  elem)		//inconst{	//elements are 0 based	USHORT	index = elem/32;	if(index >=	its_num_of_comps)return false;		//pos = position	USHORT	bit_pos = elem % 32;	LONG 	mask = 1;	mask = mask<<bit_pos;		return (its_array_p[index] & mask) !=0;}#pragma mark --extract an element--//******************************************BOOLEAN 	SET::extract(USHORT 	elem)		//in{	USHORT	index = elem/32;	if(index >=	its_num_of_comps)return false;		//pos = position		USHORT	bit_pos = elem % 32;	LONG 	mask = 1;	mask = mask<<bit_pos;	LONG 	saved_comp = its_array_p[index];		its_array_p[index] &= (~mask);	if(its_array_p[index]!=saved_comp){		its_num_of_elems--;		return true;	}else{		return false;	}}#pragma mark --insert an element--//******************************************BOOLEAN 	SET::insert(USHORT	elem)		//in{	USHORT	index = elem/32;	ThrowIf_(index >= its_num_of_comps);		//pos = position		USHORT	bit_pos = elem % 32;	LONG 	mask = 1;	mask = mask<<bit_pos;	LONG 	saved_comp = its_array_p[index];		its_array_p[index] |= mask;	if(its_array_p[index]!=saved_comp){		its_num_of_elems++;		return true;	}else{		return false;	}	}#pragma mark ----used only with macgui----#ifdef   _mac_gui_app //''''''''''''''''''''''''''''''''''''''''''''\\.#pragma mark --i/o--//******************************************LStream  &		operator<<(LStream  & 		out_bd,		//i-oconst SET  & 	set)		//in{	out_bd << set.its_num_of_elems << set.its_num_of_comps;	for(USHORT i=0; i<set.its_num_of_comps; i++){		out_bd << set.its_array_p[i];	}	return out_bd;}//******************************************LStream  &		operator>>(LStream  & 	in_bd,		//i-oSET  & 		set)		//in { 	USHORT	new_num_of_comps;	in_bd >> set.its_num_of_elems >> new_num_of_comps;	if(new_num_of_comps != set.its_num_of_comps){		delete [] set.its_array_p;		set.its_array_p = new LONG[new_num_of_comps];//new[]		//delete:		ThrowIfNil_(set.its_array_p);	}	set.its_num_of_comps =new_num_of_comps;	for(USHORT i=0; i<new_num_of_comps; i++){		in_bd >> set.its_array_p[i];	}	return in_bd;}	#endif //_mac_gui_app \\............................................//		   