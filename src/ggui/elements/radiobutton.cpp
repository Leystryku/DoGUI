#pragma once


#include <cstdint>

#include "radiobutton.h"

#include "../../orender.h"
#include "../../easydraw.h"
#include "../ggui.h"

#include <stdio.h>
#include <string.h>

GGuiRadioButton::GGuiRadioButton()
{
	selected = 0;
	ourid = 0;
}

GGuiRadioButton::GGuiRadioButton(void*toparent) : GGuiRadioButton()
{
	SetParent(toparent);

}

int32_t GGuiRadioButton::SetID(int32_t toid)
{
	ourid = toid;
	return 0;
}

int32_t GGuiRadioButton::GetID()
{
	return ourid;
}

int32_t GGuiRadioButton::SetSelected()
{
	if (!GetSelectedP())
		return 1;

	*GetSelectedP() = ourid;
	is_selected = true;

	return 0;
}

bool GGuiRadioButton::GetSelected()
{
	return is_selected;
}


int32_t GGuiRadioButton::SetSelectedP(int32_t *toselected)
{
	selected = toselected;

	if (*selected == ourid)
	{
		is_selected = true;
	}
	else {
		is_selected = false;
	}

	return 0;
}

int32_t* GGuiRadioButton::GetSelectedP()
{
	return selected;
}

int32_t GGuiRadioButton::OnRender(void *curgui, void* currender, void *curdraw)
{

	GGui *gui = (GGui*)curgui;
	Render *render = (Render*)currender;
	Easydraw *draw = (Easydraw*)curdraw;


	int32_t curx = GetPosX();
	int32_t cury = GetPosY();
	int32_t curabsx = GetPosX(true);
	int32_t curabsy = GetPosY(true);
	int32_t curw = GetWidth();
	int32_t curh = GetHeight();

	if (!curw)
		return 1;

	int32_t radius = curw / 2;

	draw->SetColor(Easycolor(255, 255, 255, 255));
	draw->OutlinedCircle(curabsx + radius, curabsy + radius, radius);



	if (GetSelected())
	{
		draw->SetColor(Easycolor(255, 255, 255, 204));  //no hover and checked
	}
	else {
		if (!GetMouseOver())
		{
			draw->SetColor(Easycolor(255, 255, 255, 0)); // no hover and unchecked
		}
		else {

			if (gui->GetButtonState(VIRTUALKEY_LBUTTON) != BUTTONSTATE_PRESSED)
			{
				draw->SetColor(Easycolor(255, 255, 255, 25)); // hover and unchecked
			}
			else {

				draw->SetColor(Easycolor(255, 255, 255, 0)); // hover and unchecked
			}
		}
	}


	draw->Circle(curabsx + radius, curabsy + radius, radius - 4);

	SetPos(curx + curw + 14, cury);
	GGuiLabel::OnRender(curgui, currender, curdraw);
	SetPos(curx, cury);



	return 0;
}


int32_t GGuiRadioButton::OnMouseReleased(int32_t mousex, int32_t mousey, bool leftclick)
{

	if (!GetSelectedP())
		return 0;

	if (GetMouseOver() && GetFocus() == 1)
	{
		*GetSelectedP() = ourid;
	}


	return 0;

}

int32_t GGuiRadioButton::PostThink()
{
	if (ggui->GetButtonState(VIRTUALKEY_LBUTTON) == BUTTONSTATE_JUSTRELEASED)
	{
		if (*GetSelectedP() == ourid)
		{
			is_selected = true;
		}
		else {
			is_selected = false;
		}
	}

	return 0;
}