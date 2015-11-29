//This class, with some minor modifications, is called CIncludeView.h//by Mike Shields. �t comes with his Multi-Panel Dialogs library.// ===========================================================================//	File:						CIncludeView.h//  Version:					1.0 - Feb 1, 1996//	Author:					Mike Shields (mshields@inconnect.com)//							//	Copyright �1996 Mike Shields. All rights reserved.// ===========================================================================//	CIncludeView.cp		<- double-click + Command-D to see class declaration//// Class which controls the loading and disposing of a subpane.#pragma once#include <LView.h>#include <PP_Types.h>class C_HOST_VIEW : public LView{public:							C_HOST_VIEW();	C_HOST_VIEW(const C_HOST_VIEW & inOriginal);	C_HOST_VIEW(const SPaneInfo & inPaneInfo, const SViewInfo & inViewInfo, 					ResIDT inStartingPane);	C_HOST_VIEW(LStream * inStream);	virtual			~C_HOST_VIEW();//	virtual void	FinishCreateSelf(void);//	virtual void	DisposeSubPane(void);//	virtual void	LoadSubPane(void);	virtual void	ResetSubPane(ResIDT inPaneID, Boolean inRefresh);	protected:	virtual void	FinishCreateSelf(void);	virtual void	DisposeSubPane(void);	virtual void	LoadSubPane(void);		ResIDT			mPaneID;	LPane *			mCurrentIncludedPane;};