#pragma once


#include <cstdint>

#include "button.h"

#include "../../orender.h"
#include "../../easydraw.h"
#include "../ggui.h"

#include <stdio.h>
#include <string.h>

GGuiButton::GGuiButton()
{
	buttonaction = 0;
	SetVCenter(true);
	SetHCenter(true);
}

GGuiButton::GGuiButton(void*toparent)  : GGuiButton()
{
	SetParent(toparent);

}

int32_t GGuiButton::SetFontColor(int32_t tor, int32_t tog, int32_t tob, int32_t toa)
{
	return GGuiLabel::SetColor(tor, tog, tob, toa);
}

int32_t GGuiButton::GetFontColor(int32_t &getr, int32_t &getg, int32_t &getb, int32_t &geta)
{	
	return GGuiLabel::GetColor(getr, getg, getb, geta);
}

int32_t GGuiButton::SetButtonAction(void*action)
{
	buttonaction = action;

	return 0;
}

void* GGuiButton::GetButtonAction()
{
	return buttonaction;
}



int32_t GGuiButton::OnRender(void *curgui, void* currender, void *curdraw)
{

	GGui *gui = (GGui*)curgui;
	Render *render = (Render*)currender;
	Easydraw *draw = (Easydraw*)curdraw;


	int curx = GetPosX();
	int cury = GetPosY();
	int curabsx = GetPosX(true);
	int curabsy = GetPosY(true);
	int curw = GetWidth();
	int curh = GetHeight();


	SetCustomTop(GetPosY(true));
	SetCustomBottom(GetPosY(true) + GetHeight());
	SetCustomLeft(GetPosX(true));
	SetCustomRight(GetPosX(true) + GetWidth());

	if (!GetMouseOver())
	{

		SetFontColor(255, 255, 255, 255);

		draw->SetColor(Easycolor(255, 255, 255, 255));
		draw->OutlinedRectangle(curabsx, curabsy, curw, curh, 1);

	}
	else {

		SetFontColor(26, 26, 26, 255);
		if (ggui->GetButtonState(VIRTUALKEY_LBUTTON) == BUTTONSTATE_PRESSED)
		{
			draw->SetColor(Easycolor(255, 255, 255, 128));

			draw->Rectangle(curabsx + 1, curabsy + 1, curw - 2, curh - 2);
		}
		else {

			draw->SetColor(Easycolor(255, 255, 255, 204));

			draw->Rectangle(curabsx, curabsy, curw, curh);
		}
	}




	


	GGuiLabel::OnRender(curgui, currender, curdraw);

	return 0;
}

int32_t GGuiButton::OnButtonPressed()
{


	return 0;
}

int32_t GGuiButton::OnButtonReleased()
{

	typedef  void(__cdecl *buttoncallbackfn)(void*);
	buttoncallbackfn buttoncallback = (buttoncallbackfn)buttonaction;

	if (buttonaction)
	{
		buttoncallback(this);
	}

	return 0;
}

int32_t GGuiButton::OnMousePressed(int32_t mousex, int32_t mousey, bool leftclick, bool initialpress)
{
	if (initialpress && GetMouseOver() && GetFocus() == 1)
	{
		OnButtonPressed();
	}
	
	return 0;
}

int32_t GGuiButton::OnMouseReleased(int32_t mousex, int32_t mousey, bool leftclick)
{

	if (GetMouseOver() && GetFocus() == 1)
	{
		OnButtonReleased();
	}

	return 0;

}