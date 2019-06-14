#pragma once

#include <cstdint>
#include "baseelement.h"
#include "textbox.h"

#define NUMBERBOX_MAXNUMLEN 60

class GGuiNumberBox : public GGuiTextBox
{
	float floatmax;
	float floatmin;
	int32_t floatafterdot;

	bool floatnewdot;
	int32_t dot;
	int32_t intmax;
	int32_t intmin;

	int32_t istep;
	float fstep;

	int* ourintp;
	float* ourfloatp;
	bool usingfloat;

	bool updateexternal;

	wchar_t*ourtextp;
	bool overfirst;
	bool oversecond;
public:
	virtual const wchar_t *GetType()
	{
		return L"Numberbox";
	}

	virtual bool IsAllowedChar(int32_t charcode);

	virtual int32_t SetAfterDot(int32_t amount);

	virtual int32_t OnRender(void *curgui, void* currender, void *curdraw);
	virtual int32_t SetSize(int32_t tow, int32_t toh) override;
	virtual int32_t OnMouseMove(int32_t mousex, int32_t mousey);

	virtual int32_t Increment();
	virtual int32_t Decrement();
	virtual int32_t SetIncDecStep(int32_t step);
	virtual int32_t SetIncDecStep(float step);

	virtual int32_t SetNumber(float *num);
	virtual int32_t SetNumber(int32_t* num);
	virtual int32_t OnTextChanged();
	
	virtual int32_t UpdateNumberText();

	virtual int32_t SetSupportExternalChange(bool tosupport);

	int32_t SetMax(float tomax);
	int32_t SetMax(int32_t tomax);
	int32_t SetMin(float tomin);
	int32_t SetMin(int32_t tomin);


	GGuiNumberBox();
	GGuiNumberBox(void* toparent);
	~GGuiNumberBox();

};
