#pragma once
#include <cstdint>
#include <string>
#include "baseelement.h"


class GGuiLabel : public  GGuiBaseElement
{

private:
	std::wstring text;

	int32_t fontr;
	int32_t fontg;
	int32_t fontb;
	int32_t fonta;

	int32_t backgroundr;
	int32_t backgroundg;
	int32_t backgroundb;
	int32_t backgrounda;

	int32_t textw;
	int32_t texth;

	int32_t textoffsetx;
	int32_t textoffsety;

	int32_t renderw;
	int32_t renderh;

	int32_t cliptop;
	int32_t clipbottom;
	int32_t clipleft;
	int32_t clipright;

	int32_t customleft;
	int32_t customright;
	int32_t custombottom;
	int32_t customtop;

	wchar_t ourfontname[0xFF];
	wchar_t fontname[0xFF];
	int32_t fontsize;
	int32_t fontweight;
	bool autosize;
	bool autosize_gotsize;
	bool fontshouldclip;
	bool textalignright;
	bool textalignbottom;

	bool vcenter;
	bool hcenter;

public:

	virtual const wchar_t *GetType()
	{
		return L"Label";
	}

	virtual int32_t SetCustomLeft(int32_t tocustom);
	virtual int32_t GetCustomLeft();

	virtual int32_t SetCustomRight(int32_t tocustom);
	virtual int32_t GetCustomRight();

	virtual int32_t SetCustomTop(int32_t tocustom);
	virtual int32_t GetCustomTop();

	virtual int32_t SetCustomBottom(int32_t tocustom);
	virtual int32_t GetCustomBottom();

	virtual int32_t SetVCenter(bool tocenter);
	virtual bool GetVCenter();

	virtual int32_t SetHCenter(bool tocenter);
	virtual bool GetHCenter();

	virtual int32_t SetAlignRight(bool alignright);
	virtual bool GetAlignRight();

	virtual int32_t SetAlignBottom(bool alignbottom);
	virtual bool GetAlignBottom();

	virtual int32_t SetTextOffsetX(int32_t newoffsetx);
	virtual int32_t GetTextOffsetX();

	virtual int32_t SetTextOffsetY(int32_t newoffsety);
	virtual int32_t GetTextOffsetY();

	virtual int32_t AddChar(int32_t addchar, int32_t pos = 0);
	virtual int32_t RemoveChar(size_t pos = 0);

	virtual int32_t SetText(const wchar_t *newtext);

	virtual int32_t GetTextString(std::wstring* copyinto);
	virtual int32_t GetText(wchar_t*buf, size_t bufsize);
	virtual size_t GetTextLength();

	virtual int32_t SetClip(bool enabled);
	virtual bool GetClip();

	virtual int32_t SetColor(int32_t r, int32_t g, int32_t b, int32_t a);
	virtual int32_t GetColor(int32_t &r, int32_t &g, int32_t &b, int32_t &a);

	virtual int32_t SetBGColor(int32_t r, int32_t g, int32_t b, int32_t a);
	virtual int32_t GetBGColor(int32_t &r, int32_t &g, int32_t &b, int32_t &a);

	virtual int32_t SetClipTop(int32_t newcliptop);
	virtual int32_t SetClipBottom(int32_t newclipbottom);
	virtual int32_t SetClipLeft(int32_t newclipleft);
	virtual int32_t SetClipRight(int32_t newnewclipright);

	virtual int32_t SetAutoSized(bool toautosize);
	virtual bool GetAutoSized();

	virtual int32_t GetTextWidth();
	virtual int32_t GetTextHeight();

	virtual int32_t SetFont(const wchar_t* ourname, const wchar_t *fontname, int32_t fontsize, int32_t fontweight);
	virtual const wchar_t* GetOurFontName();
	virtual const wchar_t* GetFontName();
	virtual int32_t GetFontSize();
	virtual int32_t GetFontWeight();

	virtual int32_t OnRender(void *curgui, void* currender, void *curdraw);




	GGuiLabel();
	GGuiLabel(void* toparent);
};