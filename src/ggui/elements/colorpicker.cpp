#include <cstdint>

#include "colorpicker.h"

#include "../../easydraw.h"
#include "../ggui.h"

#include <math.h>



// hue = 0-360
// saturation = 0-1
// brightness = 0-1
Easycolor HSBtoRGB(const float hue, const float saturation, const float brightness) {
	float chroma = brightness * saturation;
	float huePrime = (float)fmod(hue / 60.0, 6);

	float delta = (float)(chroma * (1 - fabs(fmod(huePrime, 2) - 1)));
	unsigned char rest = (uint8_t)floor((brightness - chroma) * 255);
	Easycolor output = { 0, 0, 0 };

	if (0 <= huePrime && huePrime < 1) {
		output.r = (uint8_t)floor(chroma * 255);
		output.g = (uint8_t)floor(delta * 255);
	}
	else if (1 <= huePrime && huePrime < 2) {
		output.r = (uint8_t)floor(delta * 255);
		output.g = (uint8_t)floor(chroma * 255);
	}
	else if (2 <= huePrime && huePrime < 3) {
		output.g = (uint8_t)floor(chroma * 255);
		output.b = (uint8_t)floor(delta * 255);
	}
	else if (3 <= huePrime && huePrime < 4) {
		output.g = (uint8_t)floor(delta * 255);
		output.b = (uint8_t)floor(chroma * 255);
	}
	else if (4 <= huePrime && huePrime < 5) {
		output.r = (uint8_t)floor(delta * 255);
		output.b = (uint8_t)floor(chroma * 255);
	}
	else if (5 <= huePrime && huePrime < 6) {
		output.r = (uint8_t)floor(chroma * 255);
		output.b = (uint8_t)floor(delta * 255);
	}

	output.r += rest;
	output.g += rest;
	output.b += rest;

	return output;
}

typedef struct {
	float h;
	float s;
	float b;
} HSBValues;


HSBValues RGBtoHSB(const Easycolor color)
{

	float flr = (float)color.r / 255.f;
	float flg = (float)color.g / 255.f;
	float flb = (float)color.b / 255.f;
	float fla = (float)color.a / 255.f;

	float chroma_max = fmax(fmax(flr, flg), flb);
	float chroma_min = fmin(fmin(flr, flg), flb);
	float delta = chroma_max - chroma_min;
	HSBValues output = { 0, 0, 0 };

	if (delta > 0.f) {
		if (chroma_max == flr) {
			output.h = 60.f* (fmod(((flg - flb) / delta), 6.f));
		}
		else if (chroma_max == flg) {
			output.h = 60.f * (((flb - flr) / delta) + 2.f);
		}
		else if (chroma_max == flb) {
			output.h = 60.f * (((flr - flg) / delta) + 4.f);
		}

		if (chroma_max > 0.f) {
			output.s = delta / chroma_max;
		}
		else {
			output.s = 0.f;
		}

		output.b = chroma_max;
	}
	else {
		output.h = 0.f;
		output.s = 0.f;
		output.b = chroma_max;
	}

	if (output.h < 0) {
		output.h = 360.f + output.h;
	}

	return output;
}


void* numberinputcallback(GGuiNumberBox*box, int32_t type, void*arg)
{
	GGuiColorPicker*picker = (GGuiColorPicker*)box->GetParent();

	if (!picker)
		return 0;


	if (!wcsstr(picker->GetType(), L"Colorpicker"))
	{
		return 0;
	}


	if (type == 2)
	{

		int32_t charcode = (int32_t)arg;

		picker->RGBASelectorChange();
		return 0;
	}

	return 0;

}

void* hexinputcallback(GGuiHexTextBox*box, int32_t type, void*arg)
{
	GGuiColorPicker*picker = (GGuiColorPicker*)box->GetParent();

	if (!picker)
		return 0;


	if (!wcsstr(picker->GetType(), L"Colorpicker"))
	{
		return 0;
	}


	if (type == 2)
	{

		int32_t charcode = (int32_t)arg;

		picker->HEXSelectorChange();
		return 0;
	}

	return 0;

}

#include <Windows.h>

void GGuiColorPicker::HEXSelectorChange()
{
	int32_t packed = hexnumber;
	r = (packed >> 24) & 0xFF;
	g = (packed >> 16) & 0xFF;
	b = (packed >> 8) & 0xFF;
	a = (packed >> 0) & 0xFF;


	printf("HEXPACK: %d_%d_%d_%d\n", r, g, b, a);

	transalphapickchanged = true;
	transalphapick_slideoffset = 255 - a;

	colorselectorchanged = true;
	circlechanged = true;
	Easycolor newcol(r, g, b, a);
	HSBValues hsb = RGBtoHSB(newcol);



	float acolorselector_slideoffset = -0.708f * hsb.h + 255.f;
	float acircleoffx = hsb.s  * 255.f;
	float acircleoffy = -255 * hsb.b + 255;

	
	colorselector_slideoffset = (int32_t)acolorselector_slideoffset; circleoffx = (int32_t)acircleoffx; circleoffy = (int32_t)acircleoffy;

	if (GetFocus() == 0)
		*colorpointer = packed;

}
void GGuiColorPicker::RGBASelectorChange()
{

	transalphapickchanged = true;
	transalphapick_slideoffset = 255-a; 

	colorselectorchanged = true;
	circlechanged = true;
	Easycolor newcol(r, g, b, a);
	HSBValues hsb = RGBtoHSB(newcol);



	float acolorselector_slideoffset = -0.708f * hsb.h + 255.f;
	float acircleoffx = hsb.s  * 255.f;
	float acircleoffy = -255 * hsb.b + 255;


	printf("VALS __ OLD: %d _ %d _ %d\nNEW: %d _ %d _ %d\n", colorselector_slideoffset, circleoffx, circleoffy, (int32_t)acolorselector_slideoffset, (int32_t)acircleoffx, (int32_t)acircleoffy);

	colorselector_slideoffset = (int32_t)acolorselector_slideoffset; circleoffx = (int32_t)acircleoffx; circleoffy = (int32_t)acircleoffy;

	int32_t packed = newcol.GetUint32RGBA();
	wchar_t hextxt[0xFF] = { 0 };
	wsprintf(hextxt, L"%x", packed);

	printf("LOL: %ws\n", hextxt);

	hexnumber = packed;
}

GGuiColorPicker::GGuiColorPicker()
{
	SetSize(140, 30);
	generatedbigboxtxt = 0;
	transalphapick_slideoffset = 0;
	colorselector_slideoffset = 80;
	pastcolorboxx = pastcolorboxy = 0;
	circleoffx = 200;
	circleoffy = 0;
	circlechanged = true;
	colorselectorchanged = true;
	transalphapickchanged = true;

	memset(hextextboxtext, 0, 0xFF);


	hextextbox = new GGuiHexTextBox(this);
	hextextbox->SetNumber(&hexnumber);
	hextextbox->SetSupportExternalChange(true);
	hextextbox->SetEnterNewLine(false);

	startr = startg = startb = starta = r = g = b = a = 255;
	hexnumber = 0xFFFFFFFF;

	oldr = oldg = oldb = olda = 0;

	boxr = new GGuiNumberBox(this);
	boxg = new GGuiNumberBox(this);
	boxb = new GGuiNumberBox(this);
	boxa = new GGuiNumberBox(this);

	boxr->SetEnterNewLine(false);
	boxr->SetNumber(&r);
	boxr->SetSupportExternalChange(true);
	boxr->SetVisible(false);
	boxr->SetMax(255);
	boxr->SetMin(0);

	boxg->SetEnterNewLine(false);
	boxg->SetNumber(&g);
	boxg->SetSupportExternalChange(true);
	boxg->SetVisible(false);
	boxg->SetMax(255);
	boxg->SetMin(0);

	boxb->SetEnterNewLine(false);
	boxb->SetNumber(&b);
	boxb->SetSupportExternalChange(true);
	boxb->SetVisible(false);
	boxb->SetMax(255);
	boxb->SetMin(0);

	boxa->SetEnterNewLine(false);
	boxa->SetNumber(&a);
	boxa->SetSupportExternalChange(true);
	boxa->SetVisible(false);
	boxa->SetMax(255);
	boxa->SetMin(0);

	boxr->SetIncDecStep(10);
	boxg->SetIncDecStep(10);
	boxb->SetIncDecStep(10);
	boxa->SetIncDecStep(10);

	boxr->SetCallbackFn(numberinputcallback);
	boxg->SetCallbackFn(numberinputcallback);
	boxb->SetCallbackFn(numberinputcallback);
	boxa->SetCallbackFn(numberinputcallback);
	hextextbox->SetCallbackFn(hexinputcallback);

}

GGuiColorPicker::GGuiColorPicker(void*toparent) : GGuiColorPicker()
{
	SetParent(toparent);
}

int32_t GGuiColorPicker::SetColorP(uint32_t *tocolorpointer)
{
	colorpointer = tocolorpointer;

	return 0;
}


int32_t contentsize = 255;

int32_t GGuiColorPicker::OnRender(void *curgui, void* currender, void *curdraw)
{

	GGui *gui = (GGui*)curgui;
	Easydraw *draw = (Easydraw*)curdraw;


	int32_t curx = GetPosX();
	int32_t cury = GetPosY();
	int32_t curabsx = GetPosX(true);
	int32_t curabsy = GetPosY(true);
	int32_t curw = GetWidth();
	int32_t curh = GetHeight();

	int32_t colw = GetWidth() / 4;

	static Easymaterial*colorstripe = 0;
	static Easymaterial*checkerboard = 0;
	static Easymaterial*checkerboard_small = 0;

	if (!colorstripe)
	{
		colorstripe = new Easymaterial(draw, (uint32_t)324,
			(void*)"\x89\x50\x4E\x47\x0D\x0A\x1A\x0A\x00\x00\x00\x0D\x49\x48\x44\x52\x00\x00\x00\x01\x00\x00\x01\x00\x08\x06\x00\x00\x00\x72\x3E\x1C\x98\x00\x00\x00\x04\x67\x41\x4D\x41\x00\x00\xB1\x8F\x0B\xFC\x61\x05\x00\x00\x00\x20\x63\x48\x52\x4D\x00\x00\x7A\x26\x00\x00\x80\x84\x00\x00\xFA\x00\x00\x00\x80\xE8\x00\x00\x75\x30\x00\x00\xEA\x60\x00\x00\x3A\x98\x00\x00\x17\x70\x9C\xBA\x51\x3C\x00\x00\x00\x09\x70\x48\x59\x73\x00\x00\x0E\xC4\x00\x00\x0E\xC4\x01\x95\x2B\x0E\x1B\x00\x00\x00\x06\x62\x4B\x47\x44\x00\x00\x00\x00\x00\x00\xF9\x43\xBB\x7F\x00\x00\x00\x48\x49\x44\x41\x54\x38\x4F\xDD\x94\xB1\x09\x00\x30\x0C\xC3\x44\x86\xFE\x7F\x6F\x49\x71\xED\x37\x9C\x41\x90\x6C\xC2\x26\x08\x34\x70\x68\x81\x8D\x9E\xE6\x79\x99\x0B\x35\xF0\xC8\x41\xD9\xAF\x05\xB0\x4A\xFB\x36\xFD\xBB\xBE\x75\x20\x05\xD4\xC4\xAF\x05\xCE\x29\x4E\x9B\x47\x61\xBF\x16\xD8\x09\x7D\xCF\x86\x94\xC9\x9F\xDA\x29\xE2\x00\x00\x00\x00\x49\x45\x4E\x44\xAE\x42\x60\x82",
			L"colorpick_colorstripe");

	}

	if (!checkerboard)
	{
		checkerboard = new Easymaterial(draw, (uint32_t)0xEA,
			(void*)"\x89\x50\x4E\x47\x0D\x0A\x1A\x0A\x00\x00\x00\x0D\x49\x48\x44\x52\x00\x00\x00\x16\x00\x00\x01\x00\x08\x04\x00\x00\x00\x1D\x45\x4E\x82\x00\x00\x00\x09\x70\x48\x59\x73\x00\x00\x0B\x13\x00\x00\x0B\x13\x01\x00\x9A\x9C\x18\x00\x00\x00\x20\x63\x48\x52\x4D\x00\x00\x7A\x25\x00\x00\x80\x83\x00\x00\xF9\xFF\x00\x00\x80\xE9\x00\x00\x75\x30\x00\x00\xEA\x60\x00\x00\x3A\x98\x00\x00\x17\x6F\x92\x5F\xC5\x46\x00\x00\x00\x70\x49\x44\x41\x54\x78\xDA\xEC\xD2\xB1\x0D\x80\x30\x0C\x04\x40\x3B\xCA\x9E\xCC\xC6\xA4\xA6\x23\x48\x29\x22\xBA\x80\xEE\x2B\xEB\xE5\xEA\x75\x59\x15\x77\xCE\x71\xC6\x11\x73\xDF\xE2\x45\xBE\xF8\xDC\x57\x0B\x3C\xFB\xDF\xAF\x91\x6C\xB0\xC1\x06\x1B\x6C\xB0\xC1\x06\x1B\x6C\xB0\xC1\x06\x1B\x6C\xB0\xC1\x06\x1B\x6C\xB0\xC1\x06\x1B\x6C\xB0\xC1\x06\x1B\x6C\xB0\xC1\x06\x1B\x6C\xB0\xC1\x06\x1B\x6C\xB0\xC1\x06\x1B\x6C\xB0\xC1\x06\x1B\x6C\xB0\xB1\x99\x8D\x6B\x00\x60\x6B\x7B\xBF\xE1\x0A\x5F\xF5\x00\x00\x00\x00\x49\x45\x4E\x44\xAE\x42\x60\x82",
			L"colorpick_checkerboard");

	}

	if (!checkerboard_small)
	{
		checkerboard_small = new Easymaterial(draw, (uint32_t)0x12C,
			(void*)"\x89\x50\x4E\x47\x0D\x0A\x1A\x0A\x00\x00\x00\x0D\x49\x48\x44\x52\x00\x00\x00\x5D\x00\x00\x00\x3A\x08\x06\x00\x00\x00\xD2\x80\x55\x21\x00\x00\x00\x09\x70\x48\x59\x73\x00\x00\x0B\x13\x00\x00\x0B\x13\x01\x00\x9A\x9C\x18\x00\x00\x00\x20\x63\x48\x52\x4D\x00\x00\x7A\x25\x00\x00\x80\x83\x00\x00\xF9\xFF\x00\x00\x80\xE9\x00\x00\x75\x30\x00\x00\xEA\x60\x00\x00\x3A\x98\x00\x00\x17\x6F\x92\x5F\xC5\x46\x00\x00\x00\xB2\x49\x44\x41\x54\x78\xDA\xEC\xD8\xC1\x09\xC0\x30\x10\x03\x41\x27\xA4\x4F\xD7\xE6\x4A\x2F\x0D\xF8\x27\xE2\x83\x30\xFB\x34\x7A\x2D\x42\x98\xBB\xAA\xAA\xC6\x86\xB5\xD6\xEE\x79\xCC\x39\x87\x7C\x96\xBF\x07\x8E\x43\x3A\xE9\xA4\x83\xF4\xFF\xF0\xF8\x75\x9C\xCF\x6B\xBA\x79\x21\x1D\xA4\x93\x8E\x80\xCB\xED\xE5\x7C\x5E\xD3\xCD\x0B\xE9\x20\x9D\x74\x04\xB8\xBD\x34\xE4\x35\xDD\xBC\x90\x0E\xD2\x49\x47\x80\xDB\x4B\x43\x5E\xD3\xCD\x0B\xE9\x20\x9D\x74\x04\xB8\xBD\x34\xE4\x35\xDD\xBC\x90\x0E\xD2\x49\x47\x80\xDB\x4B\x43\x5E\xD3\xCD\x0B\xE9\x20\x9D\x74\x04\xB8\xBD\x34\xE4\x35\xDD\xBC\x90\x0E\xD2\x49\x47\x80\xDB\x4B\x43\x5E\xD3\xCD\x0B\xE9\x20\x9D\x74\x04\xB8\xBD\x34\xE4\x35\xDD\xBC\x90\x8E\x8F\x78\x01\x00\x00\xFF\xFF\x03\x00\xBC\x45\xFE\xD5\x16\x72\x25\x79\x00\x00\x00\x00\x49\x45\x4E\x44\xAE\x42\x60\x82",
			L"colorpick_checkerboardsmall");
	}

	Easymaterial *generatedtxt = (Easymaterial*)generatedbigboxtxt;

	if (!generatedtxt)
	{
		generatedtxt = draw->CreateTexture(L"colorpicker_generatedbigbox", 256, 256);
		generatedbigboxtxt = generatedtxt;
	}
	else {
		draw->SetMaterial(generatedtxt);
		draw->TexturedRectangle(100, 100, 256, 256);
	}

	if (GetFocus() == 0)
	{

		draw->SetColor(Easycolor(255, 255, 255, 255));
		draw->OutlinedRectangle(curabsx, curabsy, colw, curh, 1);

		if (colorpointer)
		{
			uint32_t color = *(uint32_t*)colorpointer;


			uint8_t a = (uint8_t)(color & 0xFF);
			uint8_t b = (uint8_t)(color >> 8);
			uint8_t g = (uint8_t)(color >> 16);
			uint8_t r = (uint8_t)(color >> 24);

			draw->SetColor(Easycolor(r, g, b, a));
		}
		else {

			draw->SetColor(Easycolor(255, 255, 255, 0));
		}

		draw->Rectangle(curabsx + 1, curabsy + 1, colw - 2, curh - 2);

		draw->SetColor(Easycolor(255, 255, 255, 255));

		int32_t rightboxx = curabsx + colw - 1;

		if (hextextbox)
		{
			hextextbox->SetSize(curw - colw, curh);
			hextextbox->SetPos(colw - 1, 0);

			hextextbox->SetVisible(true);

			if (hextextbox->GetRenderedManually())
			{
				oldr = r;
				oldg = g;
				oldb = b;
				olda = a;
				hextextbox->SetRenderedManually(false);
			}
		}

		boxr->SetVisible(false);
		boxg->SetVisible(false);
		boxb->SetVisible(false);
		boxa->SetVisible(false);

		return 0;

	}

	bool wasclipped = draw->GetBeingClipped();


	draw->SetBeingClipped(false);

	draw->SetColor(Easycolor(46, 46, 46));
	draw->OutlinedRectangle(curabsx, curabsy, colorpicker_boxw, colorpicker_boxh, 1);


	draw->SetColor(Easycolor(9, 9, 9));
	draw->Rectangle(curabsx + 1, curabsy + 1, colorpicker_boxw - 2, colorpicker_boxh - 2);

	draw->SetColor(255, 255, 255, 255);


	//content

	int32_t contentx = curabsx + 1 + colorpicker_boxpad + 1;
	int32_t contenty = curabsy + 1 + colorpicker_boxpad + 1;

	draw->SetColor(255, 255, 255, 255);


	int32_t circlex = contentx + circleoffx;
	int32_t circley = contenty + circleoffy;



	float hue = (float)(255 - colorselector_slideoffset) * (360.f/255.f);
	float saturation = (float)(circleoffx) / 255.f;
	float brightness = (float)(255 - circleoffy) / 255.f;

	
	if (colorselectorchanged)
	{

		easydraw_locked_rect rect;
		draw->GetTextureBuffer(generatedtxt->GetTexture(), true, &rect);

		int32_t*buf = (int32_t*)rect.pBits;
		memset(buf, 0, (256) * rect.Pitch / 4 + 256);

		for (int32_t height = 0; height < 255; height++)
		{
			for (int32_t width = 0; width < 255; width++)
			{
				Easycolor pixelcol = HSBtoRGB((float)hue, (float)width / 255.f, (float)height / 255.f);
				int32_t index = (255 - height) * rect.Pitch / 4 + width;
				buf[index] = pixelcol.GetUint32ARGB();

			}
		}

		draw->GetTextureBuffer(generatedtxt->GetTexture(), false, &rect);

	}

	circlechanged = false;
	colorselectorchanged = false;
	transalphapickchanged = false;
	draw->OutlinedRectangle(curabsx + 1 + colorpicker_boxpad, curabsy + 2 + colorpicker_boxpad, contentsize + 2, contentsize + 2);


	draw->SetMaterial(generatedtxt);
	draw->TexturedRectangle(contentx, contenty, 256, 256);


	//circle

	draw->SetColor(255, 255, 255, 255);
	draw->OutlinedCircle(circlex, circley, 5);





	//colorselector

	int32_t colorselectorx = contentx + contentsize + colorpicker_elementpadding;
	int32_t colorselectory = contenty;

	draw->SetColor(255, 255, 255, 255);
	draw->OutlinedRectangle(colorselectorx, colorselectory, colorpicker_selectorw, colorpicker_selectorh);


	draw->SetColor(255, 255, 255, 255);
	draw->SetMaterial(colorstripe);
	draw->TexturedRectangle(colorselectorx + 1, colorselectory + 1, 22, colorpicker_selectorh - 2);


	draw->SetColor(255, 255, 255, 255);
	draw->Line(colorselectorx, colorselectory + colorselector_slideoffset, colorselectorx + 22, colorselectory + colorselector_slideoffset);


	//transalphapick
	int32_t transalphapickx = colorselectorx + colorpicker_selectorw + colorpicker_elementpadding;
	int32_t transalphapicky = colorselectory;

	draw->SetColor(255, 255, 255, 255);
	draw->OutlinedRectangle(transalphapickx, transalphapicky, colorpicker_selectorw, colorpicker_selectorh);

	draw->SetColor(255, 255, 255, 255);
	draw->SetMaterial(checkerboard);
	draw->TexturedRectangle(transalphapickx + 1, transalphapicky + 1, 32, colorpicker_selectorh - 2);


	draw->SetColor(255, 255, 255, 255);
	draw->GradientRectangle(transalphapickx + 1, transalphapicky + 1, 22, colorpicker_selectorh - 2,
		Easycolor(r, g, b, 255),
		Easycolor(r, g, b, 255),
		Easycolor(r, g, b, 0),
		Easycolor(r, g, b, 0)

	);

	draw->SetColor(255, 255, 255, 255);
	draw->Line(transalphapickx, transalphapicky + transalphapick_slideoffset, transalphapickx + 22, transalphapicky + transalphapick_slideoffset);

	//inputs

	int32_t inputx = transalphapickx + colorpicker_selectorw + colorpicker_elementpadding;
	int32_t inputy = transalphapicky;

	int32_t inputw = 95;
	int32_t inputh = 29;

	int32_t inputpad = 10;

	//colorhistory

	draw->SetMaterial(checkerboard_small);
	draw->TexturedRectangle(inputx + 1, inputy + 1, 128, 64);

	draw->SetColor(255, 255, 255, 255);
	draw->OutlinedRectangle(inputx, inputy, inputw, inputh * 2 + 1);

	draw->SetColor(r, g, b, a);
	draw->Rectangle(inputx + 1, inputy + 1, inputw - 2, inputh - 1);

	inputy += inputh;

	if (!pastcolorboxx || !pastcolorboxy)
	{
		pastcolorboxx = inputx;
		pastcolorboxy = inputy;
	}



	draw->SetColor(oldr, oldg, oldb, olda);
	draw->Rectangle(inputx + 1, inputy, inputw - 2, inputh );

	//numinputr
	boxr->SetSize(inputw, inputh);
	boxg->SetSize(inputw, inputh);
	boxb->SetSize(inputw, inputh);
	boxa->SetSize(inputw, inputh);

	boxr->SetVisible(true);
	boxg->SetVisible(true);
	boxb->SetVisible(true);
	boxa->SetVisible(true);

	boxr->SetRenderedManually(true);
	boxg->SetRenderedManually(true);
	boxb->SetRenderedManually(true);
	boxa->SetRenderedManually(true);

	inputy += inputh + inputpad;

	boxr->SetPos(inputx - GetPosX(true), inputy - GetPosY(true));

	inputy += inputh + inputpad;

	boxg->SetPos(inputx - GetPosX(true), inputy - GetPosY(true));

	inputy += inputh + inputpad;

	boxb->SetPos(inputx - GetPosX(true), inputy - GetPosY(true));

	inputy += inputh + inputpad;

	boxa->SetPos(inputx - GetPosX(true), inputy - GetPosY(true));


	boxr->OnRender(curgui, currender, curdraw);
	boxg->OnRender(curgui, currender, curdraw);
	boxb->OnRender(curgui, currender, curdraw);
	boxa->OnRender(curgui, currender, curdraw);


	inputy += inputh + inputpad;

	if (hextextbox)
	{
		hextextbox->SetPos(inputx - GetPosX(true), inputy - GetPosY(true));
		hextextbox->SetSize(inputw, inputh);
		hextextbox->SetVisible(true);

		if (!hextextbox->GetRenderedManually())
		{
			hextextbox->SetRenderedManually(true);
		}
		hextextbox->OnRender(curgui, currender, curdraw);

	}

	draw->SetBeingClipped(wasclipped);

	return 0;
}


bool GGuiColorPicker::GetMouseOver()
{
	bool defaultret = GGuiBaseElement::GetMouseOver();


	if (!hextextbox->GetRenderedManually() && ggui->GetMouseX() >= GetPosX(true) && ggui->GetMouseX() >= GetPosX(true) + GetWidth() / 4)
	{
		return false;
	}

	if (GetFocus() != 0)
	{

		if (ggui->GetMouseY() >= GetPosY(true) && ggui->GetMouseY() <= GetPosY(true) + colorpicker_boxh)
		{
			if (ggui->GetMouseX() >= GetPosX(true) && ggui->GetMouseX() <= GetPosX(true) + colorpicker_boxw)
			{
				return true;
			}
		}
	}



	return defaultret;

}

int32_t GGuiColorPicker::OnMousePressed(int32_t mousex, int32_t mousey, bool leftclick, bool initialpress)
{

	if (!GetFocus() && colorpointer)
	{
		uint32_t newcolor = 0;
		newcolor |= (r << 24);
		newcolor |= (g << 16);
		newcolor |= (b << 8);
		newcolor |= (a << 0);
		*colorpointer = newcolor;

	}


	if (GetFocus() && GetVisible() && GetMouseOver())
	{
		int32_t curabsx = GetPosX(true);
		int32_t curabsy = GetPosY(true);

		int32_t contentx = curabsx + 1 + colorpicker_boxpad;
		int32_t contenty = curabsy + 1 + colorpicker_boxpad;

		int32_t colorselectorx = contentx + contentsize + colorpicker_elementpadding;
		int32_t colorselectory = contenty;
		int32_t transalphapickx = colorselectorx + colorpicker_selectorw + colorpicker_elementpadding;
		int32_t transalphapicky = colorselectory;

		bool changedstuff = false;

		if (ggui->GetMouseX() >= contentx && ggui->GetMouseY() >= contenty && ggui->GetMouseX() < contentx + contentsize - 1 && ggui->GetMouseY() < contenty + contentsize) // contents selector
		{
			circleoffx = ggui->GetMouseX() - contentx;
			circleoffy = ggui->GetMouseY() - contenty;
			changedstuff = true;
		}


		if (ggui->GetMouseX() >= colorselectorx && ggui->GetMouseY() >= colorselectory && ggui->GetMouseX() < colorselectorx + colorpicker_selectorw && ggui->GetMouseY() < colorselectory + colorpicker_selectorh - 1)    // colorselector pick
		{

			colorselector_slideoffset = ggui->GetMouseY() - colorselectory;
			colorselectorchanged = true;
			changedstuff = true;
		}

		if ( ggui->GetMouseX() >= transalphapickx && ggui->GetMouseY() >= transalphapicky && ggui->GetMouseX() < transalphapickx + colorpicker_selectorw && ggui->GetMouseY() < transalphapicky + colorpicker_selectorh - 1)  // transalpha pick
		{
			transalphapick_slideoffset = ggui->GetMouseY() - transalphapicky;
			transalphapickchanged = true;
			a = 255 - transalphapick_slideoffset;
			hexnumber = Easycolor(r, g, b, a).GetUint32RGBA();
			changedstuff = true;
		}


		if (changedstuff)
		{
			float hue = (float)(255 - colorselector_slideoffset) * (360.f / 255.f);
			float saturation = (float)(circleoffx) / 255.f;
			float brightness = (float)(255 - circleoffy) / 255.f;

			Easycolor pixelcol = HSBtoRGB(hue, saturation, brightness);
			r = pixelcol.r;
			g = pixelcol.g;
			b = pixelcol.b;
			hexnumber = Easycolor(r, g, b, a).GetUint32RGBA();
		}

		int32_t inputw = 95;
		int32_t inputh = 30;


		if (initialpress && ggui->GetMouseX() >= pastcolorboxx && ggui->GetMouseY() >= pastcolorboxy && ggui->GetMouseX() <= pastcolorboxx + inputw && ggui->GetMouseY() <= pastcolorboxy + inputh) // color history
		{
			r = oldr;
			g = oldg;
			b = oldb;
			a = olda;
			SetFocus(0);
			return GGuiBaseElement::OnMousePressed(mousex, mousey, leftclick, initialpress);
		}
	}


	return GGuiBaseElement::OnMousePressed(mousex, mousey, leftclick, initialpress);
}