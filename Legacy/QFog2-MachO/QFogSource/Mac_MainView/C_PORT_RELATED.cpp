#include "C_PORT_RELATED.h"#pragma mark ----C_CIRCLE----#pragma mark --creation/destruction--//******************************************C_CIRCLE::C_CIRCLE(){}//******************************************C_CIRCLE::C_CIRCLE(const Point  &		pt,		//inUSHORT				rad)	//in	:its_center(pt),	its_radius(rad){}#pragma mark --const functions--//******************************************BOOLEAN  C_CIRCLE::intersects_rect(const Rect  &  rect) //inconst{	Rect 	bigger_rect = rect;	::InsetRect(&bigger_rect, -its_radius, -its_radius);	return	::PtInRect(its_center, &bigger_rect);}#pragma mark ----C_PORT_NAME----#pragma mark --creation/destruction--//******************************************C_PORT_NAME::C_PORT_NAME(){}//******************************************C_PORT_NAME::C_PORT_NAME(USHORT	nd_id,			//inUSHORT	sub_nd_id)		//in	:its_nd_id(nd_id),	its_sub_nd_id(sub_nd_id){}#pragma mark --i/o--//******************************************LStream  & operator<<(LStream  & out_bd,			//i-oconst C_PORT_NAME  & pn)	//in{	//out_bd= out bound, headed out of cpu	out_bd << pn.its_nd_id << pn.its_sub_nd_id;	return out_bd;}//******************************************LStream  & operator>> (LStream  & in_bd,			//i-oC_PORT_NAME  & pn) 			//in{ 	in_bd >> pn.its_nd_id >> pn.its_sub_nd_id; 	return in_bd;}