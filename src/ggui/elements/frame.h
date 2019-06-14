#pragma once
#include <cstdint>
#include "baseelement.h"

#define GGuiFrame_YBorder 30
#define GGuiFrame_WCloseButton 30

class GGuiFrame : public  GGuiBaseElement
{

protected:
	wchar_t title[0xFF];
	bool closebutton_hovered;
	bool closebutton_pressed;
public:

	virtual const wchar_t *GetType()
	{
		return L"Frame";
	}

	virtual int32_t SetTitle(const wchar_t *newtitle);
	virtual const wchar_t* GetTitle();


	virtual int32_t OnRender(void *curgui, void* currender, void *curdraw);
	virtual int32_t OnMouseHover(int32_t mousex, int32_t mouse);
	virtual int32_t OnMouseReleased(int32_t mousex, int32_t mousey, bool leftclick);
	
	virtual bool GetDraggable(bool checkpos = false, int32_t mousex=0, int32_t mousey=0);


	GGuiFrame();
	GGuiFrame(void* toparent);
};