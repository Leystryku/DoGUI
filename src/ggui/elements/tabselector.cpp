#include <cstdint>

#include "tabselector.h"

#include "../../orender.h"
#include "../../easydraw.h"
#include "../ggui.h"

#include <stdio.h>
#include <string.h>

GGuiTabSelector::GGuiTabSelector()
{
	memset(tabs, 0, sizeof(tabs)/4);
	tabs_num = 0;
	ourtabsize = 0;
	selectedtab = 0;
	isdocked = false;
}

GGuiTabSelector::GGuiTabSelector(void*toparent) : GGuiTabSelector()
{
	SetParent(toparent);
}

GGuiTabSelector::~GGuiTabSelector()
{
	for (int32_t i = 0; i < tabs_num; i++)
	{
		delete tabs[i];
		tabs[i] = 0;
	}

	tabs_num = 0;

}


int32_t GGuiTabSelector::OnRender(void *curgui, void* currender, void *curdraw)
{
	Easydraw *draw = (Easydraw*)curdraw;



	if (!tabs_num)
	{
		return 0;
	}
	
	Easyfont *font = draw->GetFont(L"guifont", L"Segoe Ui Light", 14, 0);

	if (!font)
	{
		return 0;
	}

	int32_t alignto = 10;
	int32_t curx = GetPosX(true);
	int32_t curwidth = 0;

	if (!ourtabsize) //adjust depending on text size of each tab
	{

		for (int32_t i = 0; i < tabs_num; i++)
		{
			const wchar_t *tabname = tabs[i]->GetTabName();

			curwidth = 0;
			int32_t trash = 0;

			draw->SetFont(font);
			draw->GetRawTextSize(tabname, curwidth, trash);

			int32_t textmargin = 10;

			curwidth += textmargin;

			int32_t oldwidth = curwidth;
			int32_t notalign = alignto % curwidth;


			curwidth -= notalign;

			while (oldwidth > curwidth)
				curwidth += alignto;

			curwidth += alignto;


			easydraw_fontrect lolrect;
			lolrect.left = curx + textmargin;
			lolrect.right = curx + curwidth - textmargin;
			lolrect.top = GetPosY(true);
			lolrect.bottom = lolrect.top + GGuiTabSelector_YBorder;
			
			Easycolor textcolor(0, 0, 0);
			Easycolor rectcolor(137, 137, 137);

			if (selectedtab == i)
			{

				rectcolor = Easycolor(205, 205, 205);
			}

			if ( GetMouseOver() && selectedtab != i)
			{
				if (GetMouseOverX() > curx && GetMouseOverX() < curx + curwidth && GetMouseOverY() > lolrect.top && GetMouseOverY() < lolrect.bottom)
				{
					if (ggui->GetButtonState(VIRTUALKEY_LBUTTON) == BUTTONSTATE_RELEASED)
					{
						rectcolor = Easycolor(137+10, 137+10, 137+10);
					}

					if (ggui->GetButtonState(VIRTUALKEY_LBUTTON) == BUTTONSTATE_PRESSED)
					{
						rectcolor = Easycolor(137 - 10, 137 - 10, 137 - 10);
					}

					if (ggui->GetButtonState(VIRTUALKEY_LBUTTON) == BUTTONSTATE_JUSTRELEASED)
					{
						tabs[selectedtab]->SetVisible(false);
						selectedtab = i;
						tabs[selectedtab]->SetVisible(true);
					}



				}
			}


			draw->SetColor(rectcolor);

			draw->Rectangle(curx, GetPosY(true), curwidth, GGuiTabSelector_YBorder);

			draw->SetColor(textcolor);
			draw->Text(tabname, curx, GetPosY(true), DRAWTEXT_VCENTER | DRAWTEXT_CENTER, &lolrect );

			curx += curwidth + 1;


		}

	}
	else {
			int32_t largesttext = 0;

			if (ourtabsize == 1) // adjust depending on the largest text size	
			{
				for (int32_t i = 0; i < tabs_num; i++)
				{
					const wchar_t *tabname = tabs[i]->GetTabName();

					curwidth = 0;
					int32_t trash = 0;

					draw->SetFont(font);
					draw->GetRawTextSize(tabname, curwidth, trash);

					if (curwidth > largesttext)
						largesttext = curwidth;
				}


				curwidth = largesttext;

			}
			else {
				curwidth = ourtabsize;
			}



			int32_t textmargin = 10;

			curwidth += textmargin;

			int32_t oldwidth = curwidth;
			int32_t notalign = alignto % curwidth;


			curwidth -= notalign;

			while (oldwidth > curwidth)
				curwidth += alignto;

			curwidth += alignto;


			for (int32_t i = 0; i < tabs_num; i++)
			{
				const wchar_t *tabname = tabs[i]->GetTabName();

				if (selectedtab == i)
				{

					draw->SetColor(Easycolor(205, 205, 205));
				}
				else {

					draw->SetColor(Easycolor(137, 137, 137));
				}

				draw->Rectangle(curx, GetPosY(true), curwidth, GGuiTabSelector_YBorder);

				easydraw_fontrect lolrect;
				lolrect.left = curx + textmargin;
				lolrect.right = curx + curwidth - textmargin;
				lolrect.top = GetPosY(true);
				lolrect.bottom = lolrect.top + GGuiTabSelector_YBorder;

				Easycolor textcolor(0, 0, 0);
				Easycolor rectcolor(137, 137, 137);

				if (selectedtab == i)
				{

					rectcolor = Easycolor(205, 205, 205);
				}

				if (GetMouseOver() && selectedtab != i)
				{
					if (GetMouseOverX() > curx && GetMouseOverX() < curx + curwidth && GetMouseOverY() > lolrect.top && GetMouseOverY() < lolrect.bottom)
					{
						if (ggui->GetButtonState(VIRTUALKEY_LBUTTON) == BUTTONSTATE_RELEASED)
						{
							rectcolor = Easycolor(137 + 10, 137 + 10, 137 + 10);
						}

						if (ggui->GetButtonState(VIRTUALKEY_LBUTTON) == BUTTONSTATE_PRESSED)
						{
							rectcolor = Easycolor(137 - 10, 137 - 10, 137 - 10);
						}

						if (ggui->GetButtonState(VIRTUALKEY_LBUTTON) == BUTTONSTATE_JUSTRELEASED)
						{
							tabs[selectedtab]->SetVisible(false);
							selectedtab = i;
							tabs[selectedtab]->SetVisible(true);
						}



					}
				}


				draw->SetColor(rectcolor);

				draw->Rectangle(curx, GetPosY(true), curwidth, GGuiTabSelector_YBorder);

				draw->SetColor(textcolor);
				draw->Text(tabname, curx, GetPosY(true), DRAWTEXT_VCENTER | DRAWTEXT_CENTER, &lolrect);

				curx += curwidth + 1;


			}

	}


	int32_t left = GetPosX(true) + GetWidth() - curx;

	if (left > 0)
	{
		draw->SetColor(Easycolor(18, 18, 18));
		draw->Rectangle(curx, GetPosY(true), left, GGuiTabSelector_YBorder);
	}



	if (isdocked)
	{
		draw->SetColor(Easycolor(205, 205, 205));
		draw->Line(GetPosX(true), GetPosY(true) + GGuiTabSelector_YBorder, GetPosX(true) + GetWidth(), GetPosY(true) + GGuiTabSelector_YBorder);

	}
	else {
		draw->SetColor(Easycolor(205, 205, 205));
		draw->OutlinedRectangle(GetPosX(true), GetPosY(true) + GGuiTabSelector_YBorder, GetWidth(), GetHeight()- GGuiTabSelector_YBorder);

	}


	return 0;
}

int32_t GGuiTabSelector::SetDocked(bool docked)
{
	isdocked = docked;

	return 0;
}

int32_t GGuiTabSelector::Dock()
{
	if (!GetParent())
		return 1;

	SetSize(GetParent()->GetWidth() - 2, GetParent()->GetHeight() - GGuiTabSelector_YBorder - 2);
	SetPos(0, 1);
	SetDocked(true);

	return 0;
}


GGuiTab* GGuiTabSelector::AddTab(const wchar_t *tabname)
{
	GGuiTab *newtab = new GGuiTab(this);

	newtab->SetSize(GetWidth(), GetHeight() - GGuiTabSelector_YBorder - GGuiTabSelector_YPad);
	newtab->SetPos(0, GGuiTabSelector_YBorder+ GGuiTabSelector_YPad );
	newtab->SetVisible(false);
	newtab->SetTabName(tabname);


	tabs[tabs_num] = newtab;
	tabs_num += 1;

	if (tabs_num==1)
	{
		newtab->SetVisible(true);
	}

	return newtab;
}

int32_t GGuiTabSelector::SetTabSize(int32_t tosize)
{
	ourtabsize = tosize;
	return 0;
}


GGuiTab::GGuiTab()
{
	memset(ourtabname, 0, sizeof(ourtabname) / 2);

}

GGuiTab::GGuiTab(void*toparent) : GGuiTab()
{
	SetParent(toparent);
}

int32_t GGuiTab::SetTabName(const wchar_t *tabname)
{
	wcsncpy(ourtabname, tabname, sizeof(ourtabname) / 2);

	return 0;
}
const wchar_t * GGuiTab::GetTabName()
{
	return ourtabname;
}