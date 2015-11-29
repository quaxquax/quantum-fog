#pragma once//******************************************template<class TYPE>class MATRIX{private:	TYPE  *  *  its_elements_p_p;  	USHORT		its_num_of_rows;  	USHORT		its_num_of_cols;public:	VOID 	clear();	VOID 	copy(const MATRIX<TYPE>  &  s);	VOID 	set_to_default_mat(		const TYPE  & default_value,		USHORT num_of_rows,		USHORT num_of_cols);		MATRIX();	MATRIX(		const TYPE  & default_value,		USHORT num_of_rows,		USHORT num_of_cols);	MATRIX(const  MATRIX<TYPE>  &  s );	MATRIX<TYPE>  & operator=( const MATRIX<TYPE>  & rhs ) ;	virtual ~MATRIX();	USHORT  	get_num_of_rows() const;	USHORT  	get_num_of_cols() const;	TYPE  &  	entry(USHORT row , USHORT col) const;};	