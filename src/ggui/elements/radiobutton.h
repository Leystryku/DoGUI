#pragma once
#include <cstdint>
#include "baseelement.h"
#include "label.h"


class GGuiRadioButton : public  GGuiLabel
{

protected:

	int32_t ourid;
	int32_t *selected;
	bool is_selected;

public:

	virtual const wchar_t *GetType()
	{
		return L"Radiobutton";
	}

	virtual int32_t SetID(int32_t toid);
	virtual int32_t GetID();

	virtual int32_t SetSelected();
	virtual bool GetSelected();

	virtual int32_t SetSelectedP(int32_t *toselected);
	virtual int32_t* GetSelectedP();

	virtual int32_t OnRender(void *curgui, void* currender, void *curdraw);

	virtual int32_t OnMouseReleased(int32_t mousex, int32_t mousey, bool leftclick);
	virtual	int32_t GGuiRadioButton::PostThink();

	GGuiRadioButton();
	GGuiRadioButton(void* toparent);
};