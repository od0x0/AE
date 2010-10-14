#pragma once
#include "UIView.h"
#include "UITextView.h"

@class AEUIButtonView;

typedef int (*AEUIButtonViewClickCallback)(AEUIButtonView* button, void* context);

@interface AEUIButtonView : AEUITextView {
	int buttonID;
	void* clickCallbackContext;
	AEUIButtonViewClickCallback clickCallback;
	unsigned char state;
}
-(int)buttonID;
-(void)setButtonID:(int)newButtonID;
-(AEUIButtonViewClickCallback)clickCallback;
-(void)setClickCallback:(AEUIButtonViewClickCallback)newClickCallback withContext:(void*)context;
@end
