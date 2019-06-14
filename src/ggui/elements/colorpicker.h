#include <cstdint>
#include "baseelement.h"
#include "textbox.h"
#include "numberbox.h"
#include "hextextbox.h"

#define colorpicker_boxw 500
#define colorpicker_boxh 315

#define colorpicker_boxpad 30
#define colorpicker_elementpadding 12
#define colorpicker_selectorw 24
#define colorpicker_selectorh contentsize + 2

class GGuiColorPicker : public GGuiBaseElement
{
private:
	uint32_t *colorpointer;
	GGuiHexTextBox *hextextbox;
	wchar_t hextextboxtext[0xFF];

	int32_t hexnumber;
	int32_t r;
	int32_t g;
	int32_t b;
	int32_t a;

	int32_t oldr;
	int32_t oldg;
	int32_t oldb;
	int32_t olda;

	int32_t startr;
	int32_t startg;
	int32_t startb;
	int32_t starta;

	int32_t pastcolorboxx;
	int32_t pastcolorboxy;

	int32_t circleoffx;
	int32_t circleoffy;
	bool circlechanged;
	bool colorselectorchanged;
	bool transalphapickchanged;

	GGuiNumberBox *boxr;
	GGuiNumberBox *boxg;
	GGuiNumberBox *boxb;
	GGuiNumberBox *boxa;
	int32_t colorselector_slideoffset;
	int32_t transalphapick_slideoffset;

	void*generatedbigboxtxt;
public:

	virtual const wchar_t *GetType()
	{
		return L"Colorpicker";
	}

	GGuiColorPicker();
	GGuiColorPicker(void* toparent);

	virtual void RGBASelectorChange();
	virtual void HEXSelectorChange();

	virtual int32_t SetColorP(uint32_t *tocolorpointer);

	virtual int32_t OnRender(void *curgui, void* currender, void *curdraw);
	virtual bool GetMouseOver();
	virtual int32_t OnMousePressed(int32_t mousex, int32_t mousey, bool leftclick, bool initialpress);

};