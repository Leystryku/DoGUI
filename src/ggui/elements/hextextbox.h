#pragma once

#include <cstdint>
#include "baseelement.h"
#include "textbox.h"

#define NUMBERBOX_MAXNUMLEN 60

class GGuiHexTextBox : public GGuiTextBox
{

	int* ourintp;
	bool usingfloat;

	bool updateexternal;

	wchar_t*ourtextp;
public:
	virtual const wchar_t *GetType()
	{
		return L"HexTextBox";
	}

	virtual bool IsAllowedChar(int32_t charcode);


	virtual int32_t OnRender(void *curgui, void* currender, void *curdraw);


	virtual int32_t SetNumber(int32_t* num);
	virtual int32_t OnTextChanged();
	
	virtual int32_t UpdateNumberText();

	virtual int32_t SetSupportExternalChange(bool tosupport);


	GGuiHexTextBox();
	GGuiHexTextBox(void* toparent);
	~GGuiHexTextBox();

};
