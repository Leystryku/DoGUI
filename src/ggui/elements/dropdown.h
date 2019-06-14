
#include <cstdint>
#include "baseelement.h"
#include "label.h"

class GGuiDropDown : public GGuiLabel
{
	GGuiLabel *options[0xFF];
	uint8_t options_amount;
	uint8_t options_selected;

	int32_t dropdown_lastlisty;
	int32_t dropdown_marked;

public:
	GGuiDropDown();
	GGuiDropDown(void* toparent);
	~GGuiDropDown();

	int32_t OnRender(void *curgui, void* currender, void *curdraw);
	int32_t AddOption(wchar_t *newoption);

	int32_t SelectOption(int32_t i)
	{
		options_selected = i;

		wchar_t textbuf[0xFF] = { 0 };
		options[options_selected]->GetText(textbuf, sizeof(textbuf) / 2);

		printf("%ws\n", textbuf);

		SetText(textbuf);
		SetAutoSized(false);
		SetAutoSized(true);

		return 0;
	}

	int32_t GetSelectedOption()
	{
		return options_selected;
	}

	GGuiLabel* GetOptionLabel(int32_t i)
	{
		return options[i];
	}


	virtual bool GetMouseOver();

	virtual int32_t OnMousePressed(int32_t mousex, int32_t mousey, bool leftclick, bool initialpress);
	virtual int32_t OnMouseMove(int32_t mousex, int32_t mousey);
};
