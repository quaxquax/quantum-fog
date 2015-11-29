#include "UModalDialogsPlus.h"//----------------------------------------------------------------------------//		� StDialogHandlerPlus//----------------------------------------------------------------------------//	ConstructorStDialogHandlerPlus::StDialogHandlerPlus(	ResIDT		inDialogResID,			// 'PPob' and 'RidL' Resource ID	LCommander	* inSuper)		: StDialogHandler(inDialogResID, inSuper),		mParam(0){}//----------------------------------------------------------------------------//		� DoDialogPlus//----------------------------------------------------------------------------voidStDialogHandlerPlus::DoDialogPlus(	MessageT &	ioMessage,	SInt32 &	ioParam)	{	ioParam = 0;	DoDialog();	ioMessage = mMessage;	ioParam = mParam;}//----------------------------------------------------------------------------//		� ListenToMessage//----------------------------------------------------------------------------voidStDialogHandlerPlus::ListenToMessage(	MessageT	inMessage,	void *		ioParam){	//MONITOR_THE_CHANGE_HERE		mMessage = inMessage;	mParam =  *(SInt32 *)ioParam;	// Store message and parameter. DoDialog() will return them both.}// ---------------------------------------------------------------------------//	� AskForPermission										 [static] [public]// ---------------------------------------------------------------------------boolUModalDialogsPlus::AskForPermission(	LCommander*		inSuper,	ResIDT			inDialogID,	PaneIDT			inCaptionID,	const LStr255&	inMessage){	StDialogHandler	theHandler(inDialogID, inSuper);	LWindow*		theDialog = theHandler.GetDialog();	LCaption*		theCaption = dynamic_cast<LCaption*>								(theDialog->FindPaneByID(inCaptionID));	if (theCaption == nil) {		SignalStringLiteral_("No Caption with specified ID");		return false;	}	theCaption->SetDescriptor(inMessage);	theDialog->Show();	bool		entryOK = false;	while (true) {		MessageT	hitMessage = theHandler.DoDialog();		if (hitMessage == msg_Cancel) {			break;		} else if (hitMessage == msg_OK) {			entryOK = true;			break;		}	}	return entryOK;}