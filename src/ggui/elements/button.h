#pragma once
#include <cstdint>
#include "baseelement.h"
#include "label.h"


class GGuiButton : public  GGuiLabel
{

private:
	void*buttonaction;

public:

	virtual const wchar_t *GetType()
	{
		return L"Button";
	}

	virtual int32_t SetFontColor(int32_t r, int32_t g, int32_t b, int32_t a);
	virtual int32_t GetFontColor(int32_t &r, int32_t &g, int32_t &b, int32_t &a);

	virtual int32_t SetButtonAction(void*action);
	virtual void* GetButtonAction();


	virtual int32_t OnRender(void *curgui, void* currender, void *curdraw);

	virtual int32_t OnButtonPressed();
	virtual int32_t OnButtonReleased();

	virtual int32_t OnMousePressed(int32_t mousex, int32_t mousey, bool leftclick, bool initialpress);
	virtual int32_t OnMouseReleased(int32_t mousex, int32_t mousey, bool leftclick);


	GGuiButton();
	GGuiButton(void* toparent);
};