#pragma once
#include <cstdint>
#include "baseelement.h"

#define GGuiScrollBar_Type_ScrollSize 0
#define GGuiScrollBar_Type_CurrentScroll 1
#define GGuiScrollBar_Type_PageUp 2
#define GGuiScrollBar_Type_PageDown 3
#define GGuiScrollBar_Type_Scrolling 4

class GGuiScrollBar : public GGuiBaseElement
{
private:
	void*callbackfn;

public:
	virtual const wchar_t *GetType()
	{
		return L"ScrollBar";
	}

	~GGuiScrollBar();
	GGuiScrollBar();
	GGuiScrollBar(void* toparent);


	virtual int32_t OnMousePressed(int32_t mousex, int32_t mousey, bool leftclick, bool initialpress);
	virtual int32_t OnRender(void *curgui, void* currender, void *curdraw);

	virtual int32_t PageUp();
	virtual int32_t PageDown();

	virtual uint8_t GetScrollSize(); // 0 - 255
	virtual uint8_t GetBarScrollVal(); // 0 - 255

	virtual int32_t SetCallbackFn(void* fn);

};