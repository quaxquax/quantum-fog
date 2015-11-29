#pragma once#include "C_AMP_GEN.h"#include "SG_MAGNET.h"class QB_NET;class C_PI_WIND;	//notation: transition element < (n_neg, n_pos) | in_sum, in_row, in_spin>	//in_sum = the number of incoming particles.	//Only matrix elements with in_sum=1 are non-zero.	//in_row = when in_sum=1, the row of the in node with the particle.	//in_spin = when in_sum=1, the spin (+1 or -1) of the  particle.		//The outgoing arrow has 2 occupation numbers:  n_neg and n_pos	//All occupation numbers are either 0 or 1.	//States with n_neg + n_pos>1  are given zero amplitude.	//The outgoing states are (n-, n+) = (0, 0), (0, 1), (1, 0) //******************************************class  C_SG_MAGNET_AMP_GEN: public C_AMP_GEN{private:	DOUBLE	its_magnetic_theta;//theta in degrees for magnet	DOUBLE	its_magnetic_phi;//phi in degreees for magnet	//these vectors are initially of zero length:	VECTOR<SHORT> its_spins;// 0 for -+, 1 for +, -1 for -; in 1-1 correspodence with its_in_nds	VECTOR<DOUBLE> its_thetas;// in degrees; in 1-1 correspodence with its_in_nds	VECTOR<DOUBLE> its_phis;// in degrees; in 1-1 correspodence with its_in_ndspublic:	C_SG_MAGNET_AMP_GEN(SG_MAGNET * nd_p, QB_NET *	net_p, C_PI_WIND *  pi_wind_p);	~C_SG_MAGNET_AMP_GEN();		const DOUBLE  &  	get_magnetic_theta() const;	const DOUBLE  &  	get_magnetic_phi() const;	VOID 	set_magnetic_theta(const DOUBLE  & theta);	VOID 	set_magnetic_phi(const DOUBLE  & phi);	SHORT 				get_spin(USHORT pos) const;	const DOUBLE  &  	get_theta(USHORT pos) const;	const DOUBLE  &  	get_phi(USHORT pos) const;	VOID 	set_spin(USHORT pos, SHORT spin);	VOID 	set_theta(USHORT pos, const DOUBLE  & theta);	VOID 	set_phi(USHORT pos, const DOUBLE  & phi);	const VECTOR<SHORT>  & 		get_spins() const;	const VECTOR<DOUBLE>  & 	get_thetas() const;	const VECTOR<DOUBLE>  & 	get_phis() const;	VOID 	set_spins(const VECTOR<SHORT>  &  spins);	VOID 	set_thetas(const VECTOR<DOUBLE>  &   thetas);	VOID 	set_phis(const VECTOR<DOUBLE>  &   phis);	BOOLEAN   	param_vecs_are_empty();	VOID 		resize(USHORT len);	VOID 		resize(const VECTOR<USHORT>  & in_nd_types);	VOID		extract_at(USHORT  loc);	VOID		reorder(const UI_MAP  &	map);	COMPLEX		sg_magnet_amp(USHORT n_neg, USHORT n_pos, USHORT in_sum, USHORT in_row, SHORT in_spin);		USHORT		get_expected_degen(const USHORT  * 	in_sums, const USHORT  *  in_rows,const SHORT  * in_spins);	BOOLEAN 	parents_are_canonical(VECTOR<USHORT>  & in_nd_types);	BOOLEAN		dialog_entries_are_legal(Str255  theta, Str255  phi);	VOID		fill_trans_mat_and_st_names_of_nd();		BOOLEAN		talk();};#pragma mark -//******************************************inlineconst DOUBLE  &   C_SG_MAGNET_AMP_GEN::get_magnetic_theta() const{	return its_magnetic_theta;}//******************************************inlineconst DOUBLE  &   C_SG_MAGNET_AMP_GEN::get_magnetic_phi() const{	return its_magnetic_phi;}//******************************************inlineSHORT 	C_SG_MAGNET_AMP_GEN::get_spin(USHORT pos)		//inconst{	return its_spins[pos];}//******************************************inlineconst DOUBLE  &  	C_SG_MAGNET_AMP_GEN::get_theta(USHORT 	pos)	//inconst	{	return its_thetas[pos];}//******************************************inlineconst DOUBLE  &  	C_SG_MAGNET_AMP_GEN::get_phi(USHORT	pos)	//inconst{	return its_phis[pos];}//******************************************inlineVOID 	C_SG_MAGNET_AMP_GEN::set_spin(USHORT		pos,	//inSHORT		spin)	//in{	its_spins[pos]=spin;}//******************************************inlineVOID 	C_SG_MAGNET_AMP_GEN::set_theta(USHORT			pos,		//inconst DOUBLE  &		theta)	//in{	its_thetas[pos]=theta;}//******************************************inlineVOID 	C_SG_MAGNET_AMP_GEN::set_phi(USHORT			pos,		//inconst DOUBLE  &		phi)	//in{	its_phis[pos]=phi;}//******************************************inlineconst VECTOR<SHORT>  & 		C_SG_MAGNET_AMP_GEN::get_spins() const{ 	return its_spins;}//******************************************inlineconst VECTOR<DOUBLE>  &		C_SG_MAGNET_AMP_GEN::get_thetas() const{	return its_thetas;}//******************************************inlineconst VECTOR<DOUBLE>  & 	C_SG_MAGNET_AMP_GEN::get_phis() const{	return its_phis;}//******************************************inlineVOID 	C_SG_MAGNET_AMP_GEN::set_spins(const VECTOR<SHORT>  & 		spins)	//in{	its_spins=spins;}//******************************************inlineVOID 	C_SG_MAGNET_AMP_GEN::set_thetas(const VECTOR<DOUBLE>  &   thetas)	//in{	its_thetas=thetas;}//******************************************inlineVOID 	C_SG_MAGNET_AMP_GEN::set_phis(const VECTOR<DOUBLE>  &   phis)		//in{	its_phis=phis;}//******************************************inlineBOOLEAN   	C_SG_MAGNET_AMP_GEN::param_vecs_are_empty(){	return its_spins.get_len()==0;}