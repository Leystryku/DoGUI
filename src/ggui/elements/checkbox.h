#pragma once
#include <cstdint>
#include "baseelement.h"
#include "label.h"


class GGuiCheckBox : public  GGuiLabel
{

protected:
	bool* checkedp;
	bool checked;

public:

	virtual const wchar_t *GetType()
	{
		return L"Checkbox";
	}

	virtual int32_t SetChecked(bool tochecked);
	virtual bool GetChecked();

	virtual int32_t SetCheckedP(bool *tobool);
	virtual bool* GetCheckedP();

	virtual int32_t OnRender(void *curgui, void* currender, void *curdraw);

	virtual int32_t OnMouseReleased(int32_t mousex, int32_t mousey, bool leftclick);


	GGuiCheckBox();
	GGuiCheckBox(void* toparent);
};