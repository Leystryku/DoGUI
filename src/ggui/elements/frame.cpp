#pragma once

#include <cstdint>

#include "frame.h"

#include "../../orender.h"
#include "../../easydraw.h"
#include "../ggui.h"

#include <stdio.h>
#include <string.h>

GGuiFrame::GGuiFrame()
{
	memset(title, 0, sizeof(title)/2);
	SetPosPadding(0, GGuiFrame_YBorder);

}

GGuiFrame::GGuiFrame(void*toparent) : GGuiFrame()
{
	SetParent(toparent);
}

int32_t GGuiFrame::SetTitle(const wchar_t *totitle)
{
	wcsncpy(title, totitle, sizeof(title)/2);

	if (wcslen(totitle) > sizeof(title)/2)
	{
		return 1;
	}

	return 0;
}

const wchar_t *GGuiFrame::GetTitle()
{
	return title;
}

int32_t GGuiFrame::OnRender(void *curgui, void* currender, void *curdraw) 
{
	
	GGuiBaseElement::OnRender(curgui, currender, curdraw);

	GGui *gui = (GGui*)curgui;
	Render *render = (Render*)currender;
	Easydraw *draw = (Easydraw*)curdraw;

	Easyfont *font = draw->GetFont(L"guifont", L"Segoe Ui Light", 14, 0);


	//title bar
	draw->SetColor(26, 26, 26, 191);
	draw->Rectangle(GetPosX(true) + 1, GetPosY(true) + 1, GetWidth() - 2 - GGuiFrame_WCloseButton, GGuiFrame_YBorder);

	//close button

	if (closebutton_hovered)
	{
		if (GetFocus() == 1 && ggui->GetButtonState(VIRTUALKEY_LBUTTON) == BUTTONSTATE_JUSTRELEASED) //go baibai
		{
			this->Unregister(true);
			return 0;
		}

		if (GetFocus() == 1 && ggui->GetButtonState(VIRTUALKEY_LBUTTON) == BUTTONSTATE_PRESSED)
		{

			draw->SetColor(168, 48, 48, 191);
		}
		else {
			draw->SetColor(198, 78, 78, 255);
		}
	}
	else {
		draw->SetColor(198, 78, 78, 191);
	}

	draw->Rectangle(GetPosX(true) + GetWidth() - GGuiFrame_WCloseButton - 1, GetPosY(true) + 1, GGuiFrame_WCloseButton, GGuiFrame_YBorder);

	int32_t fontxw, fontxh = 0;

	draw->GetTextSize(L"X", fontxw, fontxh);

	//close button text
	draw->SetColor(255, 255, 255, 191);
	draw->SetFont(font);
	draw->Text(L"X", (GetPosX(true) + GetWidth() - GGuiFrame_WCloseButton) + GGuiFrame_WCloseButton/2 - fontxw + 1, GetPosY(true) + 1 + (GGuiFrame_YBorder/2) - fontxh + 1, DRAWTEXT_NOCLIP);

	//title bar text
	draw->SetColor(255, 255, 255, 191);
	draw->SetFont(font);
	
	draw->Text(title, GetPosX(true)+ fontxh - 1, GetPosY(true)+ fontxw, DRAWTEXT_NOCLIP);

	//inside
	draw->SetColor(26, 26, 26, 255);
	draw->Rectangle(GetPosX(true) + 1 + 0, GetPosY(true) + 1 + GGuiFrame_YBorder, GetWidth() - 2, GetHeight() - 2 - GGuiFrame_YBorder);

	//border
	draw->SetColor(255, 255, 255, 25);
	//draw->SetColor(0, 255, 0, 255);
	draw->OutlinedRectangle(GetPosX(true), GetPosY(true), GetWidth(), GetHeight());


	

	return 0;
}



int32_t GGuiFrame::OnMouseHover(int32_t mousex, int32_t mousey)
{
	int32_t closebuttonx = GetPosX(true) + GetWidth() - GGuiFrame_WCloseButton - 1;
	int32_t closebuttony = GetPosY(true) + 1;

	int32_t closebuttonxend = GetPosX(true) + GetWidth() + GGuiFrame_WCloseButton - 1;
	int32_t closebuttonyend = GetPosY(true) + GGuiFrame_YBorder - 1;

	if (closebuttonxend > mousex && closebuttonyend > mousey && closebuttonx < mousex && closebuttony < mousey)
	{
		closebutton_hovered = true;
	}
	else {
		closebutton_hovered = false;
	}

	return 0;

}
int32_t GGuiFrame::OnMouseReleased(int32_t mousex, int32_t mousey, bool leftclick)
{
	GGuiBaseElement::OnMouseReleased(mousex, mousey, leftclick);

	return 0;
}

bool GGuiFrame::GetDraggable(bool checkpos, int32_t mousex, int32_t mousey)
{


	if (checkpos )
	{
		if (mousey < GetPosY(true))
			return false;

		if (mousey > GetPosY(true) + GGuiFrame_YBorder)
			return false;




	}
	
	return GGuiBaseElement::GetDraggable(checkpos,mousex, mousey);
}