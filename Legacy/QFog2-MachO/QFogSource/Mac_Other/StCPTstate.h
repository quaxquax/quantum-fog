//This is just a trivial merging (a merging to achieve more symmetry) of //the classes StColorState, StColorPenState, StTextState //which are defined in UDrawingState.h#pragma once#include <UDrawingState.h>//----------------------------------------------------------------------------// C= Color, P= Pen, T=Textclass	StCPTstate {public:				StCPTstate(Boolean saveC, Boolean saveP, Boolean saveT);				~StCPTstate();					void		Save();	void		Restore();		void		Normalize();private:	Boolean		mSaveC, mSaveP, mSaveT;		RGBColor	mForegroundColor;	RGBColor	mBackgroundColor;		PenState	mPenState;	SInt16		mFontNumber;	SInt16		mSize;	SInt16		mStyle;	SInt16		mMode;};