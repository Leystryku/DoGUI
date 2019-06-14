#include <cstdint>

#include "slider.h"

#include "../../orender.h"
#include "../../easydraw.h"
#include "../ggui.h"

#include <stdio.h>
#include <string.h>

GGuiSlider::GGuiSlider()
{
	usingint = false;
	changedminmax = true;

	rightboxwidth = 0;

	ivalue = 50;
	fvalue = 50.0f;

	fmax = 100.f;
	fmin = 0.f;

	imax = 100;
	imin = 0;


	
	ibakvalue = 0;
	fbakvalue = 0.0f;

	sliderchanged = true;

	namelabel = new GGuiLabel(this);
	valuelabel = new GGuiLabel(this);

	namelabel->SetText(L"Unnamed");
	valuelabel->SetText(L"0");

	namelabel->SetHCenter(true);

	valuelabel->SetVCenter(true);
	valuelabel->SetHCenter(true);

	namelabel->SetColor(0, 0, 0, 255);
	valuelabel->SetColor(0, 0, 0, 255);

	namelabel->SetRenderedManually(true);
	valuelabel->SetRenderedManually(true);
	
}

GGuiSlider::GGuiSlider(void*toparent) : GGuiSlider()
{
	SetParent(toparent);
}

GGuiSlider::~GGuiSlider()
{
	if (namelabel)
	{

		delete namelabel;
		namelabel = 0;
	}

	if (valuelabel)
	{
		delete valuelabel;
		valuelabel = 0;
	}
}

int32_t GGuiSlider::SetName(wchar_t* newname)
{
	namelabel->SetText(newname);

	namelabel->SetVCenter(true);
	namelabel->SetHCenter(true);

	return 0;
}

int32_t GGuiSlider::GetName(std::wstring *copyinto)
{
	return namelabel->GetTextString(copyinto);
}
 
int32_t GGuiSlider::OnMousePressed(int32_t mousex, int32_t mousey, bool leftclick, bool initialpress)
{
	if (GetFocus() != 1 || !namelabel  || !valuelabel)
		return  GGuiBaseElement::OnMousePressed(mousex, mousey, leftclick, initialpress);

	int32_t leftboxwidth = (int32_t)((float)namelabel->GetTextWidth() * 1.25f);

	int32_t leftboxx = GetPosX(true);
	int32_t rightboxx = GetPosX(true) + GetWidth() - rightboxwidth;

	int32_t leftboxend = leftboxx + leftboxwidth;

	int32_t innerpartwidth = GetWidth() - rightboxwidth - leftboxwidth;

	if (mousey > GetPosY(true)+GetHeight() || mousey < GetPosY(true) || mousex > GetPosX(true)+GetWidth()- rightboxwidth || mousex < leftboxend)
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

	if (usingint)
	{
		if (ivalue > imax)
			ivalue = imax;

		if (imin > ivalue)
			ivalue = imin;
	}
	else {
		if (fvalue > fmax)
			fvalue = fmax;

		if (fmin > fvalue)
			fvalue = fmin;
	}


	return GGuiBaseElement::OnMousePressed(mousex, mousey, leftclick, initialpress);
}

int32_t GGuiSlider::OnRender(void *curgui, void* currender, void *curdraw)
{
	Easydraw *draw = (Easydraw*)curdraw;


	Easyfont *font = draw->GetFont(L"guifont", L"Segoe Ui Light", 14, 0);

	if (!font)
	{
		return 0;
	}

	draw->SetColor(Easycolor(255, 255, 255));
	draw->OutlinedRectangle(GetPosX(true), GetPosY(true), GetWidth(), GetHeight());


	int32_t leftboxwidth = (int32_t)((float)namelabel->GetTextWidth() * 1.25f);	

	int32_t leftboxx = GetPosX(true);
	int32_t rightboxx = GetPosX(true) + GetWidth() - rightboxwidth;

	draw->Rectangle(leftboxx, GetPosY(true), leftboxwidth, GetHeight());



	draw->Rectangle(rightboxx, GetPosY(true), rightboxwidth, GetHeight());


	int32_t leftboxend = leftboxx + leftboxwidth;

	int32_t innerpartwidth = GetWidth() - rightboxwidth - leftboxwidth;

	//draw->Rectangle(leftboxend, GetPosY(true), innerpartwidth, GetHeight());

	int32_t linex = leftboxend;

	if (usingint)
	{
		linex += (innerpartwidth *(imin - ivalue)) / (imin - imax);
	}
	else {
		linex += (int32_t)(((float)innerpartwidth *(fmin - fvalue)) / (fmin - fmax));
	}

	int32_t linewidth = (GetWidth()/87) + 1;

	draw->Rectangle(linex, GetPosY(true), linewidth, GetHeight());

	namelabel->SetCustomLeft(leftboxx + GetWidth() / 42);
	namelabel->SetCustomRight(GetPosX(true) + leftboxwidth);
	namelabel->SetCustomTop(GetPosY(true));
	namelabel->SetCustomBottom(GetPosY(true) + GetHeight());

	valuelabel->SetCustomLeft(rightboxx);
	valuelabel->SetCustomRight(GetPosX(true) + GetWidth());
	valuelabel->SetCustomTop(GetPosY(true));
	valuelabel->SetCustomBottom(GetPosY(true) + GetHeight());

	wchar_t parsetxt[200] = { 0 };
	wchar_t convertedtxt[200] = { 0 };
	valuelabel->GetText(parsetxt, sizeof(parsetxt) / 2);

	if (usingint)
	{
		if (sliderchanged || _wtoi(parsetxt) != ibakvalue)
		{
			ibakvalue = ivalue;
			sliderchanged = false;
			_snwprintf(convertedtxt, sizeof(convertedtxt) / 2, L"%d", ivalue);
			valuelabel->SetText(convertedtxt);
			valuelabel->SetAutoSized(false);
			valuelabel->SetAutoSized(true);

			//printf("valuetxt: %ws __ %d __ %d\n", convertedtxt, _wtoi(parsetxt), ibakvalue);
		}
	}
	else {
		if (sliderchanged ||_wtof(parsetxt) != ibakvalue)
		{
			fbakvalue = fvalue;
			sliderchanged = false;
			
			_snwprintf(convertedtxt, sizeof(convertedtxt) / 2, L"%f", fvalue);
			valuelabel->SetText(convertedtxt);
			valuelabel->SetAutoSized(false);
			valuelabel->SetAutoSized(true);

			//printf("valuetxt: %ws\n", convertedtxt);
		}
	}


	if (changedminmax)
	{
		wchar_t test[256] = { 0 };

		int biggest = 0;

		if (usingint)
		{
			_snwprintf(test, sizeof(test), L"%d", imin);
			valuelabel->SetText(test);
			int minl = wcslen(test);
			memset(test, 0, sizeof(test));
			_snwprintf(test, sizeof(test), L"%d", imax);
			int maxl = wcslen(test);

			if (maxl > minl)
			{
				biggest = maxl;
				valuelabel->SetText(test);
			}
			else
			{
				biggest = minl;
			}

		}
		else {
			_snwprintf(test, sizeof(test), L"%f", fmin);
			valuelabel->SetText(test);
			int minl = wcslen(test);
			memset(test, 0, sizeof(test));
			_snwprintf(test, sizeof(test), L"%f", fmax);
			int maxl = wcslen(test);

			if (maxl > minl)
			{
				biggest = maxl;
				valuelabel->SetText(test);
			}
			else
			{
				biggest = minl;
			}
		}
		
		
		valuelabel->SetAutoSized(false);
		valuelabel->SetAutoSized(true);


	}


	namelabel->OnRender(curgui, currender, curdraw);
	valuelabel->OnRender(curgui, currender, curdraw);

	if (changedminmax)
	{

		rightboxwidth = (int32_t)((float)valuelabel->GetTextWidth()*1.25f);

		rightboxwidth = (rightboxwidth - (rightboxwidth % 5)) + 5;


		if (35 > rightboxwidth)
			rightboxwidth = 35;

		changedminmax = false;
	}

	return 0;
}

int32_t GGuiSlider::SetMax(float tofmax)
{
	changedminmax = true;
	fmax = tofmax;
	return 0;
}

int32_t GGuiSlider::SetMin(float tofmin)
{
	changedminmax = true;
	fmin = tofmin;
	return 0;
}

int32_t GGuiSlider::SetMax(int32_t toimax)
{
	changedminmax = true;
	imax = toimax;
	return 0;
}

int32_t GGuiSlider::SetMin(int32_t toimin)
{
	changedminmax = true;
	imin = toimin;
	return 0;
}


int32_t GGuiSlider::SetValue(float tovalue)
{

	usingint = false;
	fvalue = tovalue;
	return 0;
}

int32_t GGuiSlider::SetValue(int32_t tovalue)
{
	usingint = true;
	ivalue = tovalue;
	return 0;
}

