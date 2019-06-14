#pragma once
#include <cstdint>
#include "baseelement.h"
#include "label.h"

class GGuiSlider : public GGuiBaseElement
{
private:

	GGuiLabel *namelabel;
	GGuiLabel *valuelabel;

	int32_t ivalue;
	float fvalue;

	int32_t ibakvalue;
	float fbakvalue;

	float fmax;
	float fmin;
	int32_t imax;
	int32_t imin;

	bool usingint;

	bool sliderchanged;

	bool changedminmax;
	
	int32_t rightboxwidth;

public:
	virtual const wchar_t *GetType()
	{
		return L"Slider";
	}

	~GGuiSlider();
	GGuiSlider();
	GGuiSlider(void* toparent);


	virtual int32_t OnMousePressed(int32_t mousex, int32_t mousey, bool leftclick, bool initialpress);
	virtual int32_t OnRender(void *curgui, void* currender, void *curdraw);
	virtual int32_t SetName(wchar_t *newname);
	virtual int32_t GetName(std::wstring *copyinto);

	virtual int32_t SetMax(float fmax);
	virtual int32_t SetMin(float fmin);
	
	virtual int32_t SetMax(int32_t imax);
	virtual int32_t SetMin(int32_t imin);

	virtual int32_t SetValue(float tovalue);
	virtual int32_t SetValue(int32_t tovalue);


};