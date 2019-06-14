#pragma once


#include <cstdint>

#include "checkbox.h"

#include "../../orender.h"
#include "../../easydraw.h"
#include "../ggui.h"

#include <stdio.h>
#include <string.h>

GGuiCheckBox::GGuiCheckBox()
{
	checked = false;
	checkedp = 0;
}

GGuiCheckBox::GGuiCheckBox(void*toparent) : GGuiCheckBox()
{
	SetParent(toparent);

}

int32_t GGuiCheckBox::SetChecked(bool tochecked)
{
	if (checkedp)
	{
		*checkedp = tochecked;
	}
	checked = tochecked;

	return 0;
}

bool GGuiCheckBox::GetChecked()
{
	return checked;
}


int32_t GGuiCheckBox::SetCheckedP(bool *tocheckedp)
{
	checkedp = tocheckedp;
	checked = *checkedp;
	return 0;
}

bool* GGuiCheckBox::GetCheckedP()
{
	return checkedp;
}

int32_t GGuiCheckBox::OnRender(void *curgui, void* currender, void *curdraw)
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


	if (checkedp)
	{
		checked = *checkedp;
	}

	draw->SetColor(Easycolor(255, 255, 255, 255));
	draw->OutlinedRectangle(curabsx, curabsy, curw, curh, 1);



	if (checked)
	{
		if (!GetMouseOver())
		{
			draw->SetColor(Easycolor(255, 255, 255, 204));  //no hover and checked
		}
		else {
			draw->SetColor(Easycolor(255, 255, 255, 128)); // hover and checked
		}
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


	draw->Rectangle(curabsx + 4, curabsy + 4, curw - 8, curh - 8);

	SetPos(curx + curw + 14, cury);
	GGuiLabel::OnRender(curgui, currender, curdraw);
	SetPos(curx, cury);
	


	return 0;
}


int32_t GGuiCheckBox::OnMouseReleased(int32_t mousex, int32_t mousey, bool leftclick)
{

	if (GetMouseOver() && GetFocus() == 1)
	{
		checked = !checked;

		if (checkedp)
		{
			*checkedp = checked;
		}
	}

	return 0;

}