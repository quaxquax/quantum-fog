#include "C_ARROW.h"#include "C_MAIN_VIEW.h"#include "StCPTstate.h"#pragma mark --creation/destruction--//******************************************C_ARROW::C_ARROW()	:its_mview_p(0){}//******************************************C_ARROW::C_ARROW(LView  *  view_p,					//inconst C_PORT_NAME  &  	s_port_name,	//inconst C_PORT_NAME  &  	d_port_name,	//inconst Point  &			init_pt,		//inconst Point  &			fin_pt)			//in	:its_mview_p(view_p),	its_s_port_name(s_port_name),	its_d_port_name(d_port_name),	its_init_pt(init_pt),	its_fin_pt(fin_pt),	its_is_selected(false){}//******************************************C_ARROW::~C_ARROW(){}#pragma mark --initial and final points/port--//******************************************VOID	C_ARROW::change_source_nd(const C_PORT_NAME	 &	s_port_name,	//inconst Point	 &  init_pt,				//inconst Point	 &  fin_pt)					//in{	its_s_port_name = s_port_name;	its_init_pt = init_pt;	its_fin_pt = fin_pt;}		#pragma mark --draw arrow--//******************************************VOID	C_ARROW::draw(){	C_ARROW_DRAWER cupid(its_mview_p, arrowhead_len, arrowhead_halfwidth);	StCPTstate		saved_st(true, true, false);	saved_st.Normalize();	cupid.sketch(its_init_pt, its_fin_pt);}//******************************************VOID	C_ARROW::undraw(){//drawer	//next line calls its_mview_p->FocusDraw()	C_ARROW_DRAWER cupid(its_mview_p, arrowhead_len, arrowhead_halfwidth);	StCPTstate		saved_st(true, true, false);	saved_st.Normalize();	::RGBForeColor(&C_COLORS::its_white);	cupid.sketch(its_init_pt, its_fin_pt);}#pragma mark --draw sel handles--//******************************************VOID	C_ARROW::draw_sel_handles_wkr(){ //drawer	//This is a private function used only by	//C_ARROW::draw_sel_handles() and C_ARROW::undraw_sel_handles()	//These 2 functions call its_mview_p->FocusDraw();	//They also preserve the cpt state.	DOUBLE 	x1 = DOUBLE(its_init_pt.h),	y1 = DOUBLE(its_init_pt.v);	DOUBLE	x2 = DOUBLE(its_fin_pt.h),	y2 = DOUBLE(its_fin_pt.v);	DOUBLE   x_av = (x1 + x2)/2;	DOUBLE   y_av = (y1 + y2)/2;	DOUBLE arrow_len = sqrt( pow(x1-x2, 2)  + pow(y1-y2, 2) );	if(arrow_len <= arrowhead_len)return;	DOUBLE ss = (y2-y1)/arrow_len;	DOUBLE cc = (x2-x1)/arrow_len;	Point pix1 , pix2;	DOUBLE perp_dist=6; 	// perp_dist= the perpendicular distance from either handle center to the arrow.	::SetPt( &pix1, nearest_int16(x_av - perp_dist*ss), nearest_int16(y_av + perp_dist*cc));	::SetPt( &pix2, nearest_int16(x_av + perp_dist*ss), nearest_int16(y_av - perp_dist*cc));	Rect r1, r2;	::SetRect(&r1, pix1.h-2, pix1.v-2, pix1.h+2, pix1.v+2);	::SetRect(&r2, pix2.h-2, pix2.v-2, pix2.h+2, pix2.v+2);	::PaintRect(&r1);	::PaintRect(&r2);}//******************************************VOID	C_ARROW::draw_sel_handles(){ //drawer	if(!its_mview_p->FocusDraw())return;	StCPTstate		saved_st(true, true, false);	saved_st.Normalize();	::RGBForeColor(&C_COLORS::its_red);	draw_sel_handles_wkr();	}//******************************************VOID	C_ARROW::undraw_sel_handles(){ //drawer	if(!its_mview_p->FocusDraw())return;	StCPTstate		saved_st(true, true, false);	saved_st.Normalize();	::RGBForeColor(&C_COLORS::its_white);	draw_sel_handles_wkr();	}//******************************************Boolean		C_ARROW::select(C_MAIN_VIEW *  	mview_p)	//in{   	if(its_is_selected) return false;	its_is_selected = true;	draw_sel_handles();	mview_p->num_of_sel_arrows_pp();	return	true;	}//******************************************Boolean	C_ARROW::unselect(C_MAIN_VIEW *  	mview_p)	//in{   	if(!its_is_selected) return false;	its_is_selected = false;	undraw_sel_handles();	mview_p->num_of_sel_arrows_mm();	return	true;	}//******************************************VOID	C_ARROW::toggle_select(C_MAIN_VIEW *  	mview_p)	//in{	if(its_is_selected){		unselect(mview_p);	}else{		select(mview_p);	}}#pragma mark --draw arrow and handles--//******************************************VOID	C_ARROW::move_by(const Point  &  delta)		//in{	undraw();	if(its_is_selected) undraw_sel_handles();		its_init_pt.h += delta.h;	its_init_pt.v += delta.v;	its_fin_pt.h += delta.h;	its_fin_pt.v += delta.v;	draw();	if(its_is_selected) draw_sel_handles();}#pragma mark --no-drawing calls--//******************************************BOOLEAN		C_ARROW::pt_is_in_sel_dom(const Point  & pt)		//in //pt=point, sel_dom = selection domain{ 	if(::EqualPt(its_init_pt, its_fin_pt))return false;	DOUBLE  cyl_rad = 6;	// the radius in pixels 							// of the cylinder sheathing the arrow,							// such that we will admit that the arrow was 							// touched (selected or unselected)							// if pt fell inside this cylinder. 	// cross , len_sq are used in 	//the single cross-product condition for the outern rim of the cylinder.	DOUBLE pt_x = DOUBLE(pt.h);	DOUBLE pt_y = DOUBLE(pt.v);	DOUBLE its_init_pt_x = DOUBLE(its_init_pt.h);	DOUBLE its_init_pt_y = DOUBLE(its_init_pt.v);	DOUBLE its_fin_pt_x = DOUBLE(its_fin_pt.h);	DOUBLE its_fin_pt_y = DOUBLE(its_fin_pt.v);					DOUBLE 	cross=				-(pt_x - its_init_pt_x)*(its_fin_pt_y - its_init_pt_y)				+(its_fin_pt_x - its_init_pt_x)*(pt_y - its_init_pt_y);	DOUBLE 	len_sq= 				 (its_fin_pt_x - its_init_pt_x)*(its_fin_pt_x - its_init_pt_x)				+(its_fin_pt_y - its_init_pt_y)*(its_fin_pt_y - its_init_pt_y);	// dot1 and dot 2 are used in 	//the two dot-product conditions for the two endcaps of the cylinder.	DOUBLE	dot1=				 (pt_x - its_init_pt_x)*(its_fin_pt_x - its_init_pt_x)				+(pt_y - its_init_pt_y)*(its_fin_pt_y - its_init_pt_y);	DOUBLE	dot2=				 (pt_x - its_fin_pt_x)*(its_init_pt_x - its_fin_pt_x)				+(pt_y - its_fin_pt_y)*(its_init_pt_y - its_fin_pt_y);				if(			(			fabs( cross )   <=   cyl_rad*sqrt( len_sq )		)&&		(dot1>=0)&&		(dot2>=0)	){		return true;	}else{		return false;	}}//******************************************VOID	C_ARROW::renumber_nds(const UI_MAP  &		map)	//in{	its_s_port_name.its_nd_id = map.its_newfo[its_s_port_name.its_nd_id];	its_d_port_name.its_nd_id = map.its_newfo[its_d_port_name.its_nd_id];}//******************************************BOOLEAN 	operator==(const C_ARROW  &  ar1,	//inconst C_ARROW  &  ar2)	//in{		return(		(ar1.its_mview_p == ar2.its_mview_p)	 	&& (ar1.its_s_port_name == ar2.its_s_port_name)		&& (ar1.its_d_port_name == ar2.its_d_port_name)		&& ::EqualPt(ar1.its_init_pt, ar2.its_init_pt)		&& ::EqualPt(ar1.its_fin_pt, ar2.its_fin_pt)		&& (ar1.its_is_selected==ar2.its_is_selected)	);}