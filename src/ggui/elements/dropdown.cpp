#include <cstdint>

#include "dropdown.h"

#include "../../easydraw.h"
#include "../ggui.h"

GGuiDropDown::GGuiDropDown()
{
	SetClip(false);
	SetVCenter(true);
	SetAutoSized(true);
	options_amount = 0;
	options_selected = 0;
	dropdown_marked = 0;
	memset(options, 0, sizeof(options)/4);

}

GGuiDropDown::GGuiDropDown(void*toparent) : GGuiDropDown()
{
	SetParent(toparent);

}

GGuiDropDown::~GGuiDropDown()
{
	for (uint8_t i = 0; i < options_amount; i++)
	{
		if (options[i])
		{
			delete options[i];
			options[i] = 0;
		}

	}
}

int32_t GGuiDropDown::OnRender(void *curgui, void* currender, void *curdraw)
{
	Easydraw *draw = (Easydraw*)curdraw;

	int32_t trianglex = GetPosX(true) + (int32_t)(GetWidth() * 0.85);
	int32_t buttonsw = (int32_t)(GetWidth() * 0.15);

	SetCustomTop(GetPosY(true));
	SetCustomBottom(GetPosY(true) + GetHeight());

	SetCustomLeft(GetPosX(true) + 10);
	SetCustomRight(trianglex);

	GGuiLabel::OnRender(curgui, currender, curdraw);

	draw->SetColor(Easycolor(255, 255, 255));

	if (GetMouseOver())
	{
		if (ggui->GetButtonState(VIRTUALKEY_LBUTTON) == BUTTONSTATE_PRESSED)
		{
			draw->SetColor(Easycolor(141, 141, 141));
		}
		else {

			draw->SetColor(Easycolor(208, 208, 208));
		}
	}

	draw->OutlinedRectangle(GetPosX(true), GetPosY(true), GetWidth(), GetHeight());



	if (GetFocus() == 1)
	{
		bool wasclipped = draw->GetBeingClipped();
		int32_t oldtop, oldbottom, oldleft, oldright;
		oldtop = oldbottom = oldleft = oldright = 0;
		
		draw->GetClipped(oldtop, oldbottom, oldleft, oldright);
		draw->SetClipped(0, 0, 0, 0);
		draw->SetBeingClipped(false);

		int32_t ypos = GetPosY(true) + GetHeight();

		draw->SetColor(Easycolor(41, 41, 41, 255));
		draw->OutlinedRectangle(GetPosX(true), ypos, GetWidth()+2, dropdown_lastlisty-1);

		for (uint8_t i = 0; i < options_amount; i++)
		{
			if (!options[i])
				continue;


			
			options[i]->SetVisible(true);
			options[i]->SetCustomLeft(GetPosX(true));
			options[i]->SetCustomRight(GetPosX(true) + GetWidth());

			if (dropdown_marked == i)
			{
				options[i]->SetBGColor(49, 49, 49, 255);
			}
			else {

				options[i]->SetBGColor(26, 26, 26, 255);
			}

			options[i]->OnRender(curgui, currender, curdraw);
		}


		draw->SetColor(Easycolor(200, 200, 200));

		draw->SetBeingClipped(wasclipped);
		draw->SetClipped(oldtop, oldbottom, oldleft, oldright);

	}
	else {
		for (uint8_t i = 0; i < options_amount; i++)
		{
			if (!options[i])
				continue;

			options[i]->SetVisible(false);
			options[i]->SetBGColor(0, 0, 0, 0);
		}


		draw->SetColor(Easycolor(255, 255, 255));
	}


	int32_t trianglew = buttonsw / 2;
	int32_t triangleh = (int32_t)(GetHeight() / 2 * 0.7);

	int32_t trianglebottomx2 = trianglex + (trianglew / 2);
	int32_t triangletopx2 = trianglebottomx2 + triangleh / 2;

	int32_t trianglebottomy2 = GetPosY(true) + (triangleh);
	int32_t triangletopy2 = trianglebottomy2 + triangleh;


	draw->Triangle(trianglebottomx2, trianglebottomy2, triangletopx2, triangletopy2, trianglebottomx2 + trianglew+1, trianglebottomy2);

	return 0;
}

int32_t GGuiDropDown::AddOption(wchar_t *newoption)
{

	GGuiLabel *optlabel = new GGuiLabel(this);
	optlabel->SetSize(GetWidth(), GetFontSize()*2);
	optlabel->SetText(newoption);
	optlabel->SetVisible(false);
	optlabel->SetRenderedManually(true);

	options[options_amount] = optlabel;
	options_amount += 1;

	for (uint8_t i = 0; i < options_amount; i++)
	{
		if (!options[i])
			continue;


		dropdown_lastlisty = GetHeight() + (int32_t)(i*(GetFontSize() * 1.5));
		options[i]->SetPos(1, dropdown_lastlisty);
		dropdown_lastlisty = dropdown_lastlisty - (GetFontSize()/2);

	}

	if (options_amount == 1)
	{
		SetText(newoption);
	}

	SetAutoSized(false);
	SetAutoSized(true);

	return 0;
}

bool GGuiDropDown::GetMouseOver()
{
	bool defaultret = GGuiBaseElement::GetMouseOver();

	if (!defaultret && GetFocus() == 1)
	{
		if (ggui->GetMouseY() >= GetPosY(true) + GetHeight() && ggui->GetMouseY() <= GetPosY(true) + GetHeight() + dropdown_lastlisty)
		{
			if (ggui->GetMouseX() >= GetPosX(true) && ggui->GetMouseX() <= GetPosX(true) + GetWidth())
			{

				return true;
			}
		}
	}
	return defaultret;

}

int32_t GGuiDropDown::OnMousePressed(int32_t mousex, int32_t mousey, bool leftclick, bool initialpress)
{
	if (GetFocus() == 1 && GetVisible() && initialpress && GetMouseOver())
	{
		if (mousey > GetPosY(true) +GetHeight() && options_selected != dropdown_marked)
		{
			SelectOption(dropdown_marked);
			SetFocus(0);
		}
	}

	return GGuiLabel::OnMousePressed(mousex, mousey, leftclick, initialpress);
}

int32_t GGuiDropDown::OnMouseMove(int32_t mousex, int32_t mousey)
{
	if(!GetVisible()||GetFocus() != 1)
		return GGuiLabel::OnMouseMove(mousex, mousey);


	int32_t buttonsx = GetPosX(true) + (int32_t)(GetWidth() * 0.85);
	int32_t buttonsw = (int32_t)(GetWidth() * 0.15);


	if (!options_amount)
	{
		return 0;
	}

	for (uint8_t i = 0; i < options_amount; i++)
	{
		GGuiLabel* lab = options[i];


		int32_t x = lab->GetPosX(true);
		int32_t y = lab->GetPosY(true);
		
		int32_t w = lab->GetWidth();
		int32_t h = lab->GetHeight();

		if (mousex > x && mousey > y && x + w > mousex && y + h > mousey)
		{
			dropdown_marked = i;

		}
	}



	return GGuiLabel::OnMouseMove(mousex, mousey);
}