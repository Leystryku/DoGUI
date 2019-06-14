#pragma once


#include <cstdint>

#include "label.h"

#include "../../orender.h"
#include "../../easydraw.h"
#include "../ggui.h"

#include <stdio.h>
#include <string.h>

GGuiLabel::GGuiLabel()
{
	fontshouldclip = true;
	vcenter = hcenter = false;

	cliptop = clipbottom = clipleft = clipright = 0;
	customtop = custombottom = customleft = customright = 0;

	textoffsetx = textoffsety = 0;

	textalignbottom = 0;
	textalignright = 0;

	fontsize = 0;
	fontweight = 0;

	autosize = true;
	autosize_gotsize = false;

	fontr = fontg = fontb = fonta = 255;
	backgroundr = backgroundg = backgroundb = backgrounda = 0;

	renderw = 0;
	renderh = 0;
	textw = 0;
	texth = 0;

	text.clear();
	text = L"";

	memset(ourfontname, 0, sizeof(ourfontname) / 2);
	memset(fontname, 0, sizeof(fontname) / 2);

	//default font

	wcscpy(ourfontname, L"guifont");
	wcscpy(fontname, L"Segoe Ui Light");
	fontsize = 14;
}

GGuiLabel::GGuiLabel(void*toparent) : GGuiLabel()
{
	SetParent(toparent);

}

int32_t GGuiLabel::SetCustomLeft(int32_t tocustom)
{
	customleft = tocustom;

	return 0;
}
int32_t GGuiLabel::GetCustomLeft()
{
	return customleft;
}

int32_t GGuiLabel::SetCustomRight(int32_t tocustom)
{
	customright = tocustom;

	return 0;
}

int32_t GGuiLabel::GetCustomRight()
{
	return customright;
}

int32_t GGuiLabel::SetCustomTop(int32_t tocustom)
{
	customtop = tocustom;

	return 0;
}

int32_t GGuiLabel::GetCustomTop()
{
	return customtop;
}

int32_t GGuiLabel::SetCustomBottom(int32_t tocustom)
{
	custombottom = tocustom;

	return 0;

}

int32_t GGuiLabel::GetCustomBottom()
{
	return custombottom;
}

int32_t GGuiLabel::SetVCenter(bool tocenter)
{
	vcenter = tocenter;

	return 0;
}
bool GGuiLabel::GetVCenter()
{
	return vcenter;
}

int32_t GGuiLabel::SetHCenter(bool tocenter)
{
	hcenter = tocenter;

	return 0;
}
bool GGuiLabel::GetHCenter()
{
	return hcenter;
}


int32_t GGuiLabel::SetAlignRight(bool toalign)
{
	textalignright = toalign;

	return 0;
}

bool GGuiLabel::GetAlignRight()
{
	return textalignright;
}

int32_t GGuiLabel::SetAlignBottom(bool toalign)
{
	textalignbottom = toalign;

	return 0;
}

bool GGuiLabel::GetAlignBottom()
{
	return textalignbottom;
}

int32_t GGuiLabel::SetTextOffsetX(int32_t newoffsetx)
{
	textoffsetx = newoffsetx;
	return 0;
}

int32_t GGuiLabel::GetTextOffsetX()
{
	return textoffsetx;
}

int32_t GGuiLabel::SetTextOffsetY(int32_t newoffsety)
{
	textoffsety = newoffsety;
	return 0;
}

int32_t GGuiLabel::GetTextOffsetY()
{
	return textoffsety;
}

int32_t GGuiLabel::AddChar(int32_t addchar, int32_t pos)
{


	std::wstring bla;
	bla.push_back((wchar_t)addchar);

	text.insert(pos, bla);


	if (GetAutoSized())
	{
		SetAutoSized(false);
		SetAutoSized(true);
	}

	return 0;
}
int32_t GGuiLabel::RemoveChar(size_t pos)
{
	if (text.empty())
		return 0;


	if (pos > text.length())
		return 0;

	if (pos)
	{
		text.erase(pos - 1, 1);
	}
	else
	{
		text.pop_back();
	}

	return 0;
}


int32_t GGuiLabel::SetText(const wchar_t *totext)
{
	text.clear();
	text = text + totext;

	if (autosize)
	{
		textw = 0;
		texth = 0;
	}


	return 0;
}

int32_t GGuiLabel::GetTextString(std::wstring* copyinto)
{
	if (!copyinto)
		return 1;

	copyinto->clear();
	copyinto->append(text);


	return 0;
}

int32_t GGuiLabel::GetText(wchar_t *buf, size_t bufsize)
{

	if (text.length() > bufsize)
	{
		wcsncpy(buf, text.c_str(), bufsize);
		return 0;
	}

	//printf("OK: %d\n", text.length());

	wcsncpy(buf, text.c_str(), text.length());

	return 0;
}

size_t GGuiLabel::GetTextLength()
{
	return text.length();
}

int32_t GGuiLabel::SetClip(bool clip)
{
	fontshouldclip = clip;

	return 0;
}


bool GGuiLabel::GetClip()
{
	return fontshouldclip;
}

int32_t GGuiLabel::SetColor(int32_t tor, int32_t tog, int32_t tob, int32_t toa)
{
	fontr = tor;
	fontg = tog;
	fontb = tob;
	fonta = toa;

	return 0;
}

int32_t GGuiLabel::GetColor(int32_t &getr, int32_t &getg, int32_t &getb, int32_t &geta)
{
	getr = fontr;
	getg = fontg;
	getb = fontb;
	geta = fonta;

	return 0;
}


int32_t GGuiLabel::SetBGColor(int32_t tor, int32_t tog, int32_t tob, int32_t toa)
{
	backgroundr = tor;
	backgroundg = tog;
	backgroundb = tob;
	backgrounda = toa;

	return 0;
}

int32_t GGuiLabel::GetBGColor(int32_t &getr, int32_t &getg, int32_t &getb, int32_t &geta)
{
	getr = backgroundr;
	getg = backgroundg;
	getb = backgroundb;
	geta = backgrounda;

	return 0;
}

int32_t GGuiLabel::SetClipTop(int32_t newtop)
{
	cliptop = newtop;

	return 0;
}

int32_t GGuiLabel::SetClipBottom(int32_t newbottom)
{
	clipbottom = newbottom;

	return 0;
}

int32_t GGuiLabel::SetClipLeft(int32_t newleft)
{
	clipleft = newleft;

	return 0;
}

int32_t GGuiLabel::SetClipRight(int32_t newright)
{
	clipright = newright;

	return 0;
}

int32_t GGuiLabel::SetAutoSized(bool toautosize)
{
	autosize = toautosize;

	if (autosize)
	{
		autosize_gotsize = false;
		textw = 0;
		texth = 0;
	}

	return 0;
}

bool GGuiLabel::GetAutoSized()
{
	return autosize;
}

int32_t GGuiLabel::GetTextWidth()
{
	return textw;
}

int32_t GGuiLabel::GetTextHeight()
{
	return texth;
}



int32_t GGuiLabel::SetFont(const wchar_t* toourname, const wchar_t *tofontname, int32_t tofontsize, int32_t tofontweight)
{

	wcsncpy(ourfontname, toourname, sizeof(ourfontname) / 2);
	wcsncpy(fontname, tofontname, sizeof(fontname) / 2);

	fontsize = tofontsize;
	fontweight = tofontweight;

	return 0;
}

const wchar_t* GGuiLabel::GetOurFontName()
{
	return ourfontname;
}

const wchar_t* GGuiLabel::GetFontName()
{
	return fontname;
}


int32_t GGuiLabel::GetFontSize()
{
	return fontsize;
}

int32_t GGuiLabel::GetFontWeight()
{
	return fontweight;
}

int32_t GGuiLabel::OnRender(void *curgui, void* currender, void *curdraw)
{

	GGui *gui = (GGui*)curgui;
	Render *render = (Render*)currender;
	Easydraw *draw = (Easydraw*)curdraw;

	std::wstring drawtext = text;

	int32_t realh = 0;
	int32_t realw = 0;

	int32_t format = 0;

	if (!fontshouldclip)
	{
		format |= DRAWTEXT_WORDBREAK;

	}
	else {
		format |= DRAWTEXT_SINGLELINE;

	}

	if (vcenter)
	{

		format |= DRAWTEXT_VCENTER;
	}

	if (hcenter)
	{
		format |= DRAWTEXT_CENTER;
	}

	int32_t fix = 0;

	if (drawtext.length() && drawtext.at(drawtext.length() - 1) == ' ')
	{
		fix = 1;
		drawtext.push_back(0xA0);
	}

	if (textalignright)
	{
		format |= DRAWTEXT_RIGHT;
	}

	if (textalignbottom)
	{
		format |= DRAWTEXT_BOTTOM;
	}

	if (autosize && !autosize_gotsize)
	{
		if (!fontsize || !wcslen(ourfontname))
		{
			return 0;
		}

		autosize_gotsize = true;

		std::wstring copy = drawtext;

		draw->SetFont(draw->GetFont(ourfontname, fontname, fontsize, fontweight));
		draw->GetRawTextSize(copy.c_str(), textw, texth);

		draw->GetRawTextSize(copy.c_str(), renderw, renderh);



		//printf("W: %d [%d] H: %d [%d] == %ws\n", renderw, textw, renderh, texth, text.c_str());

		renderw = renderw + 1;
		renderh = renderh + 1;


	}

	if (!textw && !texth)
	{
		if (!autosize)
		{

			textw = GetWidth();
			texth = GetHeight();
		}
		renderw = textw;
		renderh = texth;

	}

	if (!textw || !texth)
	{
		renderw = 0;
		renderh = 0;

		return 0;
	}




	GGuiBaseElement::OnRender(curgui, currender, curdraw);

	draw->SetColor(Easycolor(fontr, fontg, fontb, fonta));
	draw->SetFont(draw->GetFont(ourfontname, fontname, fontsize, fontweight));

	easydraw_fontrect fontrect;



	fontrect.top = GetPosY(true) + textoffsety;
	fontrect.left = GetPosX(true) + textoffsetx;
	fontrect.right = fontrect.left + renderw;

	fontrect.bottom = fontrect.top + renderh;

	if (customtop || customleft || customright || custombottom)
	{
		if (customtop)
			fontrect.top = customtop;

		if (customleft)
			fontrect.left = customleft;

		if (customright)
			fontrect.right = customright;

		if (custombottom)
			fontrect.bottom = custombottom;

	}
	
	int32_t oldtop, oldbottom, oldleft, oldright;
	oldtop = oldbottom = oldleft = oldright = 0;
	bool oldclip = draw->GetBeingClipped();
	if (clipbottom || clipleft || cliptop || clipright)
	{
		draw->GetClipped(oldtop, oldbottom, oldleft, oldright);
		draw->SetClipped(cliptop, clipbottom, clipleft, clipright);
		draw->SetBeingClipped(true);
	}


	if (backgroundr || backgroundg||backgroundb || backgrounda)
	{
		int32_t w = renderw;

		if (fontrect.left)
		{

			w = 0;
			w = w +  fontrect.right - fontrect.left ;
		}
		else {
			w = w + textoffsetx;
		}

		draw->SetColor(Easycolor(backgroundr, backgroundg, backgroundb, backgrounda));
		draw->Rectangle(GetPosX(true), GetPosY(true), w, renderh+1);
	}

	draw->SetColor(Easycolor(fontr, fontg, fontb, fonta));

	draw->Text(drawtext.c_str(), GetPosX(true) + textoffsetx, GetPosY(true) + textoffsety, format, &fontrect);

	if (clipbottom || clipleft || cliptop || clipright)
	{
		draw->SetClipped(oldtop, oldbottom, oldleft, oldright);
	}

	draw->SetBeingClipped(oldclip);


	return 0;
}