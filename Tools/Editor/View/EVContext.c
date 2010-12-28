#include "EVContext.h"

int AESDLMouseButton(char);
int AESDLKey(int);

size_t EVContextUIDoOperationList(EVContext* self, ECOperationList* list, EVUIRect* bounds){
	
	//The render styles
	EVUIRenderStyle backgroundStyle;
	EVUIRenderStyleInit(& backgroundStyle);
	backgroundStyle.edgeColor=((AERGBA){0,0,0,1});
	backgroundStyle.edgeWidth=2;
	backgroundStyle.fillColor=((AERGBA){1,1,1,1});
	backgroundStyle.cornerRadius=2;
	
	EVUIRenderStyle searchboxStyle;
	EVUIRenderStyleInit(& searchboxStyle);
	searchboxStyle.edgeColor=((AERGBA){0.5,0,0,1});
	searchboxStyle.edgeWidth=2;
	searchboxStyle.fillColor=((AERGBA){1,1,1,0.5});
	searchboxStyle.cornerRadius=2;
	searchboxStyle.font=self->font;
	searchboxStyle.fontStyle.textColor=((AERGBA){0,0,0,1});
	
	EVUIRenderStyle buttonUpStyle;
	EVUIRenderStyleInit(& buttonUpStyle);
	buttonUpStyle.edgeColor=((AERGBA){0,0,0,0});
	buttonUpStyle.edgeWidth=0;
	buttonUpStyle.fillColor=((AERGBA){0,0,0,0});
	buttonUpStyle.cornerRadius=2;
	buttonUpStyle.font=self->font;
	buttonUpStyle.fontStyle.textColor=((AERGBA){0,0,0,1});
	buttonUpStyle.fontStyle.alignment=0;
	
	EVUIRenderStyle buttonDownStyle=buttonUpStyle;
	buttonDownStyle.fillColor=((AERGBA){0.5,0,0,1});
	buttonDownStyle.fontStyle.textColor=((AERGBA){1,1,1,1});
	
	const int fontHeight = AEFontLineHeightGet(self->font);
	
	//Render the background
	EVUIDrawRect(self->ui, bounds, & backgroundStyle);
	
	//Handle the searchbox
	EVUIRect searchboxBounds={bounds->x+5, bounds->y+bounds->h-5-fontHeight, bounds->w-5*2, fontHeight+0};
	if(EVUIDoTextBox(self->ui, & searchboxBounds, self->listBox.searchText, & searchboxStyle, & searchboxStyle, 1)){
		free(self->listBox.searchText);
		self->listBox.searchText=AEStringDuplicate(EVUITextboxTextGet(self->ui));
		ECOperationListSortBySearch(list, self->listBox.searchText);
		self->listBox.scroll.y=1;
	}
	
	//Figure out metrics to use for both the buttons and the scrollbar.
	const size_t scrollableHeight=searchboxBounds.y-bounds->y-5*2;
	const EVUIRect itemListBounds={searchboxBounds.x, searchboxBounds.y-5-scrollableHeight, searchboxBounds.w-6, scrollableHeight};
	const size_t itemHeight=fontHeight+2;
	const size_t visibleItemCount=(scrollableHeight/(itemHeight));
	const size_t itemCount=AEArrayLength(& list->operations);
	
	//Handle the scrollbar
	if(visibleItemCount < itemCount){
	EVUIRect scrollButtonBounds={searchboxBounds.x+searchboxBounds.w-4, itemListBounds.y+itemListBounds.h-scrollableHeight, bounds->w-searchboxBounds.w-2, scrollableHeight};
	EVUIRect scrollKnobBounds={scrollButtonBounds.x, scrollButtonBounds.y-itemHeight/2+scrollButtonBounds.h*self->listBox.scroll.y, scrollButtonBounds.w, scrollButtonBounds.h*(visibleItemCount/(long double)itemCount)};
	//abort();
	if(scrollKnobBounds.y < scrollButtonBounds.y) scrollKnobBounds.y=scrollButtonBounds.y;
	if(scrollKnobBounds.y+scrollKnobBounds.h > scrollButtonBounds.y+scrollButtonBounds.h) scrollKnobBounds.y=scrollButtonBounds.y+scrollButtonBounds.h-scrollKnobBounds.h;
	if(EVUIDoSlider(self->ui, & scrollButtonBounds, & scrollKnobBounds, & buttonUpStyle, & buttonDownStyle, & self->listBox.scroll)){
		self->listBox.scrollOffset=itemCount*(1-self->listBox.scroll.y)-1;
	}
	}
	else self->listBox.scrollOffset=0;
	if(itemCount==0) return 0;
	
	//Handle the items
	glPushAttrib(GL_SCISSOR_BIT);
	glEnable(GL_SCISSOR_TEST);
	glScissor(itemListBounds.x, itemListBounds.y, itemListBounds.w, itemListBounds.h);
	
	const bool oldMouseClippingEnabled=self->ui->mouseClipping;
	const EVUIRect oldMouseClippingBounds=self->ui->mouseClipBounds;
	
	self->ui->mouseClipping=true;
	self->ui->mouseClipBounds=itemListBounds;
	
	size_t scrollOffset=self->listBox.scrollOffset;
	
	if(scrollOffset < 0) scrollOffset=0;
	if(scrollOffset >= itemCount-1) scrollOffset=itemCount-1;
	
	if(visibleItemCount > itemCount) scrollOffset=0;
	
	size_t itemID=0;
	size_t displayedItemCount=visibleItemCount > itemCount-scrollOffset ? itemCount-scrollOffset : visibleItemCount;
	for(size_t i=0; i<displayedItemCount; i++){
		EVUIRect rect=EVUIRectNew(itemListBounds.x, (itemListBounds.y+itemListBounds.h)-itemHeight*(i+1), itemListBounds.w, itemHeight);
		const char* name=AEArrayAsCArray(& list->operations)[i+scrollOffset].name;
		if(name==NULL) abort();
		if(EVUIRectIntersects(itemListBounds, rect)) if(EVUIDoButton(self->ui, &rect, name, & buttonUpStyle, & buttonDownStyle)) itemID=i+scrollOffset+1;
	}
	
	/*size_t itemID=0;
	size_t displayedItemCount=visibleItemCount > itemCount ? itemCount : visibleItemCount;
	for(size_t i=0; i<itemCount; i++){
		EVUIRect rect=EVUIRectNew(itemListBounds.x, (itemListBounds.y+itemListBounds.h)-itemHeight*(i-scrollOffset+1), itemListBounds.w, itemHeight);
		const char* name=AEArrayAsCArray(& list->operations)[i].name;
		if(EVUIRectIntersects(itemListBounds, rect)) if(EVUIDoButton(self->ui, &rect, name, & buttonUpStyle, & buttonDownStyle)) itemID=i+1;
	}*/
	
	
	self->listBox.scrollOffset=scrollOffset;
	
	self->ui->mouseClipping=oldMouseClippingEnabled;
	self->ui->mouseClipBounds=oldMouseClippingBounds;
	glPopAttrib();
	
	return itemID;
}

void EVContextInit(EVContext* self){
	memset(self, 0, sizeof(EVContext));
	SDL_EnableUNICODE(1);
	SDL_EnableKeyRepeat(200, 50);
	self->ui=EVUINew();
	self->font=AEFontLoadFromTTF("Geo-Regular.ttf", 18);
	self->listBox.searchText=AEStringDuplicate("Search.");
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH, GL_NICEST);
}

void EVContextFrameUpdate(EVContext* self, double seconds){
	static ECOperationList* ops;
	if(not ops){
		ops=EUMemoryAllocate(sizeof(ECOperationList));
		ECOperationListInit(ops);
		ECOperationListAdd(ops, "Undo", NULL, NULL);
		ECOperationListAdd(ops, "Redo", NULL, NULL);
		ECOperationListAdd(ops, "Try To Do", NULL, NULL);
		ECOperationListAdd(ops, "Fail To Do", NULL, NULL);
		ECOperationListAdd(ops, "Do Again", NULL, NULL);
		ECOperationListAdd(ops, "Do Once More", NULL, NULL);
		ECOperationListAdd(ops, "Do Now", NULL, NULL);
	}

	EVUIBeginRender(self->ui);
	static int displayOps=0;
	static EVUIRect bounds={0,0,120,200};
	if(EVUIMouseButtonWasJustPressed(self->ui, EVUIMouseButtonRight)){
		displayOps=1;
		self->listBox.scroll.x=0;
		self->listBox.scroll.y=1;
		AEVec2 mouse;
		EVUIMouseGet(self->ui, & mouse);	
		bounds.x=mouse.x;
		bounds.y=mouse.y-bounds.h;
	}
	if(displayOps!=0 and EVContextUIDoOperationList(self, ops, &bounds)){
		puts("Selected an item!");
		displayOps=-1;
	}
	if(displayOps==-1 and not EVUIMouseButtonIsDown(self->ui, EVUIMouseButtonLeft)) displayOps=0;
	EVUIEndRender(self->ui);
}

void EVContextFixedUpdate(EVContext* self, double seconds){
	int x,y;
	SDL_GetMouseState(& x, & y);
	y=AEContextActiveGet()->h-y;
	EVUIMouseSet(self->ui, x, y);
	EVUIMouseButtonSet(self->ui, EVUIMouseButtonLeft, AESDLMouseButton(SDL_BUTTON_LEFT));
	EVUIMouseButtonSet(self->ui, EVUIMouseButtonRight, AESDLMouseButton(SDL_BUTTON_RIGHT));
	EVUIMouseButtonSet(self->ui, EVUIMouseButtonMiddle, AESDLMouseButton(SDL_BUTTON_MIDDLE));
	//EVUIMouseButtonSet(self->ui, EVUIMouseButtonUpWheel, AESDLMouseButton(SDL_BUTTON_WHEELUP));
	//EVUIMouseButtonSet(self->ui, EVUIMouseButtonDownWheel, AESDLMouseButton(SDL_BUTTON_WHEELDOWN));
	EVUIMouseButtonSet(self->ui, EVUIMouseButtonUpWheel, AESDLKey(SDLK_UP));
	EVUIMouseButtonSet(self->ui, EVUIMouseButtonDownWheel, AESDLKey(SDLK_DOWN));
	
	SDL_Event event;
	while (SDL_PollEvent(& event)) {
		switch (event.type) {
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
					case SDLK_BACKSPACE: EVUIPushBackspace(self->ui); break;
					case SDLK_LEFT: EVUIMoveCaret(self->ui, -1); break;
					case SDLK_RIGHT: EVUIMoveCaret(self->ui, 1); break;
					default: EVUIPushCharacter(self->ui, event.key.keysym.unicode); break;
				}
				break;
			default: break;
		}
	}
}

void EVContextDeinit(EVContext* self){
	memset(self, 0, sizeof(EVContext));
}