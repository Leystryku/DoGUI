#include <cstdint>

#include "scrollbar.h"

#include "../../orender.h"
#include "../../easydraw.h"
#include "../ggui.h"

#include <stdio.h>
#include <string.h>

typedef  int32_t (__cdecl *scrollbarcallbackfn)(void* element, int32_t type, int32_t data);

int32_t defaultvalue = 0;

int32_t defaultfunc(void*element, int32_t type, int32_t data)
{
	if (type == GGuiScrollBar_Type_ScrollSize)
	{
		//scrollbar size
		return 255/4; // quarter size
	}

	if (type == GGuiScrollBar_Type_CurrentScroll)
	{
		//how much we scrolled
		return defaultvalue;
	}

	if (type == GGuiScrollBar_Type_PageUp)
	{
		

		defaultvalue = defaultvalue - 10;
		if (0 > defaultvalue)
			defaultvalue = 0;

		printf("pgup: %d\n", defaultvalue);
		return 0; // do nothing
	}

	if (type == GGuiScrollBar_Type_PageDown)
	{
		defaultvalue = defaultvalue + 10;
		
		if (defaultvalue > 255)
			defaultvalue = 255;

		printf("pgdown: %d\n", defaultvalue);

		return 0; // do nothing
	}

	if (type == GGuiScrollBar_Type_Scrolling)
	{
		printf("scroul: %d\n", data);
		if (data > 255)
			data = 255;

		defaultvalue = data;
		return 0;
	}

	return 0;
}

GGuiScrollBar::GGuiScrollBar()
{
	callbackfn = defaultfunc;

}

GGuiScrollBar::GGuiScrollBar(void*toparent) : GGuiScrollBar()
{
	SetParent(toparent);
}

GGuiScrollBar::~GGuiScrollBar()
{

}

int32_t GGuiScrollBar::PageUp()
{
	scrollbarcallbackfn scrollcallback = (scrollbarcallbackfn)callbackfn;
	scrollcallback(this, GGuiScrollBar_Type_PageUp, 0);

	//printf("pgup\n");

	return 0;
}

int32_t GGuiScrollBar::PageDown()
{
	scrollbarcallbackfn scrollcallback = (scrollbarcallbackfn)callbackfn;
	scrollcallback(this, GGuiScrollBar_Type_PageDown, 0);
	//printf("pgdown\n");

	return 0;
}

uint8_t GGuiScrollBar::GetScrollSize()
{
	scrollbarcallbackfn scrollcallback = (scrollbarcallbackfn)callbackfn;
	return (uint8_t)scrollcallback(this, GGuiScrollBar_Type_ScrollSize, 0);
}

uint8_t GGuiScrollBar::GetBarScrollVal()
{
	scrollbarcallbackfn scrollcallback = (scrollbarcallbackfn)callbackfn;
	return (uint8_t)scrollcallback(this, GGuiScrollBar_Type_CurrentScroll, 0);
}

int32_t GGuiScrollBar::SetCallbackFn(void* fn)
{
	callbackfn = fn;

	return 0;
}

int32_t GGuiScrollBar::OnMousePressed(int32_t mousex, int32_t mousey, bool leftclick, bool initialpress)
{
	if (GetFocus() != 1|| mousex < GetPosX(true) || mousey<GetPosY(true) || mousex>GetPosX(true)+GetWidth() || mousey > GetPosY(true)+GetHeight() )
		return  GGuiBaseElement::OnMousePressed(mousex, mousey, leftclick, initialpress);

	int32_t buttonsx = GetPosX(true);
	int32_t trianglew = GetWidth() / 2 + 1;

	bool is_vertical = true;

	if (GetWidth() > GetHeight())
	{
		is_vertical = false;
	}

	if (is_vertical)
	{

	
		if (mousey <= GetPosY(true) + GetWidth())
		{
			if(initialpress)
				PageUp();

			return GGuiBaseElement::OnMousePressed(mousex, mousey, leftclick, initialpress);
		}

		if (mousey >= GetPosY(true) + GetHeight() - GetWidth())
		{
			if (initialpress)
				PageDown();

			return GGuiBaseElement::OnMousePressed(mousex, mousey, leftclick, initialpress);
		}

	}
	else {
		if (mousex <= GetPosX(true) + GetHeight())
		{
			if (initialpress)
				PageUp();

			return GGuiBaseElement::OnMousePressed(mousex, mousey, leftclick, initialpress);
		}

		if (mousex >= GetPosX(true) + GetWidth() - GetHeight())
		{
			if (initialpress)
				PageDown();

			return GGuiBaseElement::OnMousePressed(mousex, mousey, leftclick, initialpress);
		}
	}
	scrollbarcallbackfn scrollcallback = (scrollbarcallbackfn)callbackfn;



	int32_t fullbarheight = GetHeight() - (GetWidth() * 2);
	int32_t barheight = fullbarheight * GetScrollSize() / 255;

	int32_t relativescroll = mousey - GetPosY(true) - GetWidth();

	if(!is_vertical)
		relativescroll = mousex - GetPosX(true) - GetHeight();


	//printf("rscroll: %d\n", relativescroll);

	if (scrollcallback)
	{
		int32_t convertscroll = (relativescroll ) / barheight;
		
		scrollcallback(this, GGuiScrollBar_Type_Scrolling, convertscroll);
	}

	/*
	int32_t leftboxwidth = (int32_t)((float)namelabel->GetTextWidth() * 1.25f);

	int32_t leftboxx = GetPosX(true);
	int32_t rightboxx = GetPosX(true) + GetWidth() - rightboxwidth;

	int32_t leftboxend = leftboxx + leftboxwidth;

	int32_t innerpartwidth = GetWidth() - rightboxwidth - leftboxwidth;

	if (mousey > GetPosY(true) + GetHeight() || mousey < GetPosY(true) || mousex > GetPosX(true) + GetWidth() - rightboxwidth || mousex < leftboxend)
		return  GGuiBaseElement::OnMousePressed(mousex, mousey, leftclick, initialpress);

	sliderchanged = true;

	int32_t relativex = mousex - leftboxend;

	if (relativex)
	{
		if (usingint)
		{
			ivalue = (int32_t)imin + relativex * (imax - imin) / innerpartwidth;

		}
		else {
			fvalue = (float)((float)relativex * fmax / (float)innerpartwidth);
		}

	}
	else {
		ivalue = imin;
		fvalue = fmin;
	}

	*/

	return GGuiBaseElement::OnMousePressed(mousex, mousey, leftclick, initialpress);
}

int32_t GGuiScrollBar::OnRender(void *curgui, void* currender, void *curdraw)
{
	Easydraw *draw = (Easydraw*)curdraw;


	Easyfont *font = draw->GetFont(L"guifont", L"Segoe Ui Light", 14, 0);

	if (!font)
	{
		return 0;
	}

	bool is_vertical = true;

	if (GetWidth() > GetHeight())
	{
		is_vertical = false;
	}


	draw->SetColor(Easycolor(10, 10, 10, 255));
	draw->Rectangle(GetPosX(true), GetPosY(true), GetWidth(), GetHeight());

	//45, 45, 45, 255


	if (is_vertical)
	{
		int32_t buttonsx = GetPosX(true);
		int32_t buttonsy = GetPosY(true);

		int32_t trianglew = GetWidth() / 2 + 1;
		int32_t triangleh = trianglew;

		draw->SetColor(30, 30, 30, 255);

		draw->SetColor(Easycolor(10, 10, 10, 255));

		if (ggui->InArea(ggui->GetMouseX(), ggui->GetMouseY(), buttonsx, buttonsy, GetWidth(), GetWidth()))
		{

			if (ggui->GetButtonState(VIRTUALKEY_LBUTTON) == BUTTONSTATE_PRESSED)
			{
				draw->SetColor(30, 30, 30, 255);
			}
			else {
				draw->SetColor(22, 22, 22, 255);
			}
		}

		draw->Rectangle(buttonsx, GetPosY(true), GetWidth(), GetWidth());

		draw->SetColor(Easycolor(10, 10, 10, 255));

		if ( ggui->InArea(ggui->GetMouseX(), ggui->GetMouseY(), buttonsx, buttonsy + GetHeight() - GetWidth(), GetWidth(), GetWidth()))
		{

			if (ggui->GetButtonState(VIRTUALKEY_LBUTTON) == BUTTONSTATE_PRESSED)
			{
				draw->SetColor(30, 30, 30, 255);
			}
			else {
				draw->SetColor(22, 22, 22, 255);
			}
		}

		draw->Rectangle(buttonsx, GetPosY(true) + GetHeight() - GetWidth(), GetWidth(), GetWidth());

		draw->SetColor(Easycolor(45, 45, 45, 255));



		int32_t triangle1bottomx = buttonsx + (trianglew/2) - 1;
		int32_t triangle1topx = triangle1bottomx + triangleh / 2;

		int32_t triangle1topy = GetPosY(true) + (triangleh/2);
		int32_t triangle1bottomy = triangle1topy + triangleh;



		draw->Triangle(triangle1bottomx, triangle1bottomy,
			triangle1topx, triangle1topy,
			triangle1bottomx + trianglew, triangle1bottomy);


		int32_t triangle2bottomx = buttonsx + (trianglew / 2) - 1;
		int32_t triangle2topx = triangle2bottomx + triangleh / 2;

		int32_t triangle2topy = GetPosY(true) + GetHeight() - (int32_t)(triangleh*1.5);
		int32_t triangle2bottomy = triangle2topy + triangleh;

		draw->Triangle(triangle2bottomx, triangle2topy,
			triangle2topx, triangle2bottomy,
			triangle2bottomx + trianglew, triangle2topy);
		
		int32_t barwidth = GetWidth() / 3;
		int32_t fullbarheight = GetHeight() - (GetWidth() * 2);

		int32_t barheight = fullbarheight * GetScrollSize() / 255;

		uint8_t pscroll = GetBarScrollVal();
		int32_t offset = (barheight*pscroll / 255);
		offset = (int32_t)(offset* (float)((float)(fullbarheight- barheight )/ (float)barheight));

		draw->Rectangle(buttonsx + GetWidth() / 2 - (barwidth/2), GetPosY(true) + GetWidth() + offset, barwidth, barheight);


	}
	else {
		int32_t buttonsx = GetPosX(true);
		int32_t buttonsy = GetPosY(true);
		int32_t triangleh = GetHeight() - 2;

		
		draw->SetColor(Easycolor(10, 10, 10, 255));

		if (ggui->InArea(ggui->GetMouseX(), ggui->GetMouseY(), buttonsx, buttonsy, triangleh, triangleh))
		{

			if (ggui->GetButtonState(VIRTUALKEY_LBUTTON) == BUTTONSTATE_PRESSED)
			{
				draw->SetColor(30, 30, 30, 255);
			}
			else {
				draw->SetColor(22, 22, 22, 255);
			}
		}

		draw->Rectangle(buttonsx, buttonsy, GetHeight(), GetHeight());

		draw->SetColor(Easycolor(10, 10, 10, 255));

		if (ggui->InArea(ggui->GetMouseX(), ggui->GetMouseY(), buttonsx+ GetWidth() - triangleh , buttonsy, triangleh, triangleh))
		{

			if (ggui->GetButtonState(VIRTUALKEY_LBUTTON) == BUTTONSTATE_PRESSED)
			{
				draw->SetColor(30, 30, 30, 255);
			}
			else {
				draw->SetColor(22, 22, 22, 255);
			}
		}

		draw->Rectangle(GetPosX(true) + GetWidth() - GetHeight(), GetPosY(true), GetHeight(), GetHeight());

		draw->SetColor(Easycolor(45, 45, 45, 255));


		int32_t trianglew = triangleh;


		draw->Triangle(buttonsx + triangleh / 2 - 4, buttonsy + (triangleh/2),
			buttonsx + triangleh - 2, buttonsy + triangleh,
			buttonsx + triangleh - 2, buttonsy);



		draw->Triangle(buttonsx - 2 + GetWidth(), buttonsy + (triangleh / 2),
			buttonsx - triangleh / 2 - triangleh/2 + GetWidth(), buttonsy + triangleh,
			buttonsx - triangleh / 2 - triangleh/2 + GetWidth(), buttonsy);


	}


	return 0;
}
