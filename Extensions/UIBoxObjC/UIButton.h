#pragma once
#include "UIBox.h"


@interface AEUIButton : AEUIBox {
@public
	void (*callback)(id);
	BOOL on;
	AERGBA borderColor,highlightBorderColor;
}
-init;
-render;
-click;
-cursorUp:(int)button;
@end
