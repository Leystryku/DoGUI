#include <cstdint>
#include <limits.h>

#include "numberbox.h"

#include "../../easydraw.h"
#include "../ggui.h"

GGuiNumberBox::GGuiNumberBox()
{
	istep = 1;
	fstep = 1.f;
	floatnewdot = false;
	floatafterdot = 3;
	dot = 0;

	floatmax = FLT_MAX;
	floatmin = FLT_MIN;

	intmax = LONG_MAX;
	intmin = LONG_MIN;

	usingfloat = false;
	ourintp = 0;
	ourfloatp = 0;
	updateexternal = false;

	ourtextp = new wchar_t[NUMBERBOX_MAXNUMLEN];
	memset(ourtextp, 0, NUMBERBOX_MAXNUMLEN*2);

	SetTextP(ourtextp, NUMBERBOX_MAXNUMLEN);
	SetEnterNewLine(false);

	overfirst = oversecond = false;

}

GGuiNumberBox::GGuiNumberBox(void*toparent) : GGuiNumberBox()
{
	SetParent(toparent);

}

GGuiNumberBox::~GGuiNumberBox()
{
	if (ourtextp)
	{
		delete ourtextp;
		ourtextp = 0;
	}
}

bool GGuiNumberBox::IsAllowedChar(int32_t charcode)
{
	if (!isdigit(charcode))
	{
		if (charcode == '\b' || charcode == '\r' || charcode == 0x16 || charcode == 0x18 || charcode == 0x03)
			return true;

		if(GetCaretIndex() == 0 && charcode == '-')
			return true;

		if (GetCaretIndex() > 0 && charcode == '.' && usingfloat && ourfloatp)
		{
			if (0 > *ourfloatp)
			{
				if (GetCaretIndex() == 1) //-.
					return false;
			}


			floatnewdot = true;
			dot = GetCaretIndex();
			if (dot > 1)
				dot = dot - 1;

			printf("NEW DOT: %d\n", dot);
			return true;
		}


		return false;
	}

	if (usingfloat)
	{
		if (GetTextLength() > 40)
			return false;

		if (floatmax)
		{
			char check[40] = { 0 };
			sprintf(check, "%f", floatmax);

			if (GetTextLength() >= strlen(check))
			{
				return false;
			}
		}
	}
	else {
		if (GetTextLength() > 10)
			return false;

		if (intmax)
		{
			size_t maxl = 0;
			int backmax = intmax;
			while (backmax != 0)
			{
				maxl = maxl + 1;
				backmax = backmax / 10;
			}
			
			if (GetTextLength() >= maxl)
			{
				return false;
			}
		}
	}

	
		

	
	return true;
}

int32_t GGuiNumberBox::SetAfterDot(int32_t amount)
{
	floatafterdot = amount;

	return 0;
}

int32_t GGuiNumberBox::SetSize(int32_t tow, int32_t toh)
{
	
	SetBorderPadRight(tow/6);

	return GGuiTextBox::SetSize(tow, toh);
}

int32_t GGuiNumberBox::OnMouseMove(int32_t mousex, int32_t mousey)
{
	int32_t buttonsx = GetPosX(true) + (int32_t)(GetWidth() * 0.85);
	int32_t buttonsw = (int32_t)(GetWidth() * 0.15);


	if (mousex >= buttonsx && buttonsx + buttonsw >= mousex)
	{
		if (mousey >= GetPosY(true) && GetPosY(true) + GetHeight() / 2 >= mousey)
		{
			overfirst = true;
		}
		else {
			overfirst = false;
		}

		if (mousey >= GetPosY(true) + GetHeight() / 2 && GetPosY(true) + GetHeight() >= mousey)
		{
		oversecond = true;
		overfirst = false;
		}
		else {
			oversecond = false;
		}

	}
	else {
		oversecond = overfirst = false;
	}

	return GGuiTextBox::OnMouseMove(mousex, mousey);
}

int32_t GGuiNumberBox::OnRender(void *curgui, void* currender, void *curdraw)
{
	int32_t buttonsx = GetPosX(true) + (int32_t)(GetWidth() * 0.85);
	int32_t buttonsw = (int32_t)(GetWidth() * 0.15);

	Easydraw *draw = (Easydraw*)curdraw;
	
	int32_t oldtop, oldbottom, oldleft, oldright;
	oldtop = oldbottom = oldleft = oldright = 0;
	
	draw->GetClipped(oldtop, oldbottom, oldleft, oldright);
	bool wasclipped = draw->GetBeingClipped();
	draw->SetClipped(GetPosY(true), GetPosY(true) + GetHeight(), GetPosX(true), buttonsx);

	GGuiTextBox::OnRender(curgui, currender, curdraw);
	draw->SetClipped(oldtop, oldbottom, oldleft, oldright);
	draw->SetBeingClipped(wasclipped);
	
	if (updateexternal)
	{
		if (usingfloat)
		{
			if (ourfloatp && *ourfloatp != (float)_wtof(ourtextp))
			{
				UpdateNumberText();

			}
		}
		else {
			if (ourintp && *ourintp != (int)_wtoi(ourtextp))
			{
				UpdateNumberText();
			}
		}
	}

	if (!overfirst)
	{
		draw->SetColor(Easycolor(255, 255, 255));
	}
	else {

		if (ggui->GetButtonState(VIRTUALKEY_LBUTTON) == BUTTONSTATE_JUSTRELEASED)
		{
			Increment();
		}

		if (ggui->GetButtonState(VIRTUALKEY_LBUTTON) == BUTTONSTATE_PRESSED)
		{
			draw->SetColor(Easycolor(141, 141, 141));
		}
		else {
			draw->SetColor(Easycolor(209, 209, 209));
		}
	}

	draw->Rectangle(buttonsx, GetPosY(true), buttonsw, GetHeight() / 2);



	if (!oversecond)
	{
		draw->SetColor(Easycolor(255, 255, 255));
	}
	else {

		if (ggui->GetButtonState(VIRTUALKEY_LBUTTON) == BUTTONSTATE_JUSTRELEASED)
		{
			Decrement();
		}

		if (ggui->GetButtonState(VIRTUALKEY_LBUTTON) == BUTTONSTATE_PRESSED)
		{
			draw->SetColor(Easycolor(141, 141, 141));
		}
		else {
			draw->SetColor(Easycolor(209, 209, 209));
		}
	}

	draw->Rectangle(buttonsx, GetPosY(true) + GetHeight() / 2, buttonsw, GetHeight() / 2);

	draw->SetColor(Easycolor(32, 32, 32));

	int32_t trianglew = buttonsw / 2;
	int32_t triangleh = GetHeight() / 2 / 2;


	int32_t trianglebottomx = buttonsx + (trianglew / 2);
	int32_t triangletopx = trianglebottomx + triangleh / 2;

	int32_t triangletopy = GetPosY(true) + (triangleh/2);
	int32_t trianglebottomy = triangletopy + triangleh;



	draw->Triangle(trianglebottomx, trianglebottomy, triangletopx, triangletopy, trianglebottomx + trianglew, trianglebottomy);

	int32_t trianglebottomx2 = buttonsx + (trianglew / 2);
	int32_t triangletopx2 = trianglebottomx2 + triangleh / 2;

	int32_t trianglebottomy2 = GetPosY(true) + GetHeight()/2 + (triangleh / 2);
	int32_t triangletopy2 = trianglebottomy2 + triangleh;



	draw->Triangle(trianglebottomx2, trianglebottomy2, triangletopx2, triangletopy2, trianglebottomx2 + trianglew, trianglebottomy2);
	return 0;
}

int32_t GGuiNumberBox::Increment()
{
	if (usingfloat)
	{
		if (ourfloatp)
		{
			float incd = *ourfloatp + fstep;
			if (*ourfloatp > incd) // overflow
			{
				return 1;
			}
			if (incd > floatmax)
			{
				*ourfloatp = floatmax;
			}
			else {

				*ourfloatp = *ourfloatp + fstep;
			}
			UpdateNumberText();
		}

	}
	else {
		if (ourintp)
		{
			int32_t incd = *ourintp + istep;
			if (*ourintp > incd) // overflow
			{
				return 1;
			}
			if ( incd> intmax)
			{
				*ourintp = intmax;
			}
			else {

				*ourintp = *ourintp + istep;
			}
			UpdateNumberText();
		}
	}

	GGuiTextBox::OnTextChanged();
	printf("INC\n");

	return 0;
}

int32_t GGuiNumberBox::Decrement()
{
	if (usingfloat)
	{
		if (ourfloatp)
		{
			float decd = *ourfloatp - fstep;
			if (*ourfloatp < decd) // underflow
			{
				return 1;
			}
			if ( decd < floatmin)
			{
				*ourfloatp = floatmin;
			}
			else {

				*ourfloatp = *ourfloatp - fstep;
			}
			UpdateNumberText();
		}

	}
	else {
		if (ourintp)
		{
			int32_t decd = *ourintp - istep;
			if (*ourintp < decd) // underflow
			{
				return 1;
			}
			if (decd < intmin)
			{
				*ourintp = intmin;
			}
			else {

				*ourintp = *ourintp - istep;
			}
			UpdateNumberText();
		}
	}
	GGuiTextBox::OnTextChanged();
	printf("DEC\n");
	return 0;
}

int32_t GGuiNumberBox::SetIncDecStep(int32_t step)
{
	istep = step;
	return 0;
}

int32_t GGuiNumberBox::SetIncDecStep(float step)
{
	fstep = step;
	return 0;
}

int32_t GGuiNumberBox::SetNumber(float *num)
{
	dot = 0;
	usingfloat = true;
	SetTextP(ourtextp, 43);
	SetText(L"");
	ourfloatp = num;

	UpdateNumberText();
	SetEnterNewLine(false);

	return 0;
}

int32_t GGuiNumberBox::SetNumber(int32_t* num)
{
	dot = 0;
	usingfloat = false;
	SetTextP(ourtextp, 12);
	SetText(L"");
	ourintp = num;
	
	UpdateNumberText();
	SetEnterNewLine(false);

	return 0;
}

int32_t GGuiNumberBox::OnTextChanged()
{

	int32_t ret = GGuiTextBox::OnTextChanged();
	if (usingfloat)
	{
		if (ourfloatp)
		{

			//printf("RN: %ws __ %d\n", ourtextp, dot);

			if (floatnewdot)
			{
				floatnewdot = false;
			
				for (size_t i = 0; i < GetTextLength(); i++)
				{
					if (ourtextp[i] != '.') continue;
				
					if (i == dot)
						continue;

					printf("oh..:  %d\n", i);

					for (size_t j = i; j < GetTextLength(); j++)
					{
						ourtextp[j] = ourtextp[j + 1];
					}
				}

				printf("N: %ws\n", ourtextp);

			}

			*ourfloatp = (float)_wtof(ourtextp);

			UpdateNumberText();

			printf("NUM: %f\n", *ourfloatp);
		}

	}
	else {
		
		if (ourintp)
		{
			*ourintp = _wtoi(ourtextp);
			UpdateNumberText();
			printf("NUM: %d\n", *ourintp);

		}
	}
	ret = GGuiTextBox::OnTextChanged();
	return ret;
}

int32_t GGuiNumberBox::UpdateNumberText()
{
	if (!usingfloat)
	{
		if (!ourintp)
			return 1;

		if (intmax && ourintp && *ourintp > intmax)
		{
			*ourintp = intmax;
		}

		if (intmin && ourintp && *ourintp < intmin)
		{
			*ourintp = intmin;
		}

		wchar_t convertnum[13] = { 0 };
		_snwprintf(convertnum, sizeof(convertnum) / 2, L"%d", *ourintp);
		SetText(convertnum);
		SetAutoSized(false);
		SetAutoSized(true);

		printf("%ws == %d\n", convertnum, *ourintp);


		return 0;
	}


	if (!ourfloatp)
	{
		return 1;
	}

	if (floatmax && ourfloatp&& *ourfloatp > floatmax)
	{
		*ourfloatp = floatmax;
	}

	if (floatmin &&ourfloatp && *ourfloatp < floatmin)
	{
		*ourfloatp = floatmin;
	}


	wchar_t convertnum[43] = { 0 };

	if (floatafterdot)
	{
		wchar_t formatting[6] = { 0 };

		_snwprintf(formatting, 6, L"%%.%df", floatafterdot);
		_snwprintf(convertnum, sizeof(convertnum) / 2, formatting, *ourfloatp);

		printf("%ws __ %ws __ %f\n", formatting, convertnum, (float)_wtof(convertnum));

		*ourfloatp = (float)_wtof(convertnum);

	}
	else {
		_snwprintf(convertnum, sizeof(convertnum) / 2, L"%f", *ourfloatp);
	}


	SetText(convertnum);
	SetAutoSized(false);
	SetAutoSized(true);

	return 0;
}

int32_t GGuiNumberBox::SetSupportExternalChange(bool tosupport)
{
	updateexternal = tosupport;
	return 0;
}

int32_t GGuiNumberBox::SetMax(float tomax)
{
	floatmax = tomax;

	return 0;
}

int32_t GGuiNumberBox::SetMax(int32_t tomax)
{
	intmax = tomax;

	return 0;
}

int32_t GGuiNumberBox::SetMin(float tomin)
{
	floatmin = tomin;

	return 0;
}

int32_t GGuiNumberBox::SetMin(int32_t tomin)
{
	intmin = tomin;

	return 0;
}