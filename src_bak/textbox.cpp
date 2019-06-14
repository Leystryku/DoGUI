#pragma once


#include <cstdint>

#include "textbox.h"

#include "../../orender.h"
#include "../../easydraw.h"
#include "../ggui.h"

#include <stdio.h>
#include <string.h>
#include <Windows.h>

GGuiTextBox::GGuiTextBox()
{
	desiredcaretrecx = 0;
	desiredcaretrecy = 0;

	desiredcaretx = 0;
	desiredcarety = 0;

	lastbrutecaretx = 0;
	lastbrutecarety = 0;

	callbackfn = 0;
	maxlength = 0;

	borderpadtop = 3;

	borderpadbottom = 20;

	borderpadleft = borderpadright = 9;

	caretvis = 0;
	carettick = 0;
	buttontick = 0;
	r = g = b = a = 255;

	textwidthbeforecaret = textheightbeforecaret = 0;

	textp = 0;

	caretx = carety = 0;
	caretindex = 0;


	SetFontColor(255, 255, 255, 255);
	GGuiLabel::SetText(L"");
	GGuiLabel::SetAutoSized(true);

	SetEnterNewLine(false);

}

GGuiTextBox::GGuiTextBox(void*toparent) : GGuiTextBox()
{
	SetParent(toparent);

}


int32_t GGuiTextBox::SetMaxLength(size_t tomaxl)
{
	maxlength = tomaxl;

	return 0;
}

size_t GGuiTextBox::GetMaxLength()
{
	return maxlength;
}

int32_t GGuiTextBox::SetBorderPadTop(int32_t to)
{
	borderpadtop = to;

	return 0;
}

int32_t GGuiTextBox::GetBorderPadTop()
{
	return borderpadtop;
}

int32_t GGuiTextBox::SetBorderPadBottom(int32_t to)
{
	borderpadbottom = to;

	return 0;
}

int32_t GGuiTextBox::GetBorderPadBottom()
{
	return borderpadbottom;
}

int32_t GGuiTextBox::SetBorderPadLeft(int32_t to)
{
	borderpadleft = to;

	return 0;
}

int32_t GGuiTextBox::GetBorderPadLeft()
{
	return borderpadleft;
}

int32_t GGuiTextBox::SetBorderPadRight(int32_t to)
{
	borderpadright = to;

	return 0;
}

int32_t GGuiTextBox::GetBorderPadRight()
{
	return borderpadright;
}

int32_t GGuiTextBox::SetCaretIndex(size_t caretpos)
{

	if (caretpos>GetTextLength())
	{
		return 1;
	}

	caretindex = caretpos;

	printf("SetCaretIndex: =%d =%d = %d = %d\n", textwidthbeforecaret, GetCaretX(), -GetTextOffsetX(), GetWidth());

	if (GetCaretX() > GetWidth() - GetBorderPadRight()) // caret is than w, move text
	{

		int32_t offset = (textwidthbeforecaret - GetWidth() + GetBorderPadRight());

		printf("MOVEA: %d\n", offset);
		SetTextOffsetX(-offset);

		return 0;
	}
	else
	{
		if (!caretpos || GetCaretX()> GetWidth())
		{
			SetTextOffsetX(0);
			printf("startershit\n");
		}
	}

	if (!GetEnterNewLine())
		return 0;

	printf("123 __ %d == %d", GetCaretY(), GetHeight());

	if (GetCaretY() > GetHeight() - GetBorderPadBottom()) // caret is than h, move text
	{

		int32_t offset = (textheightbeforecaret - GetHeight() + GetBorderPadBottom());

		printf("MOVEB: %d\n", offset);
		SetTextOffsetY(-offset);

		return 0;
	}
	else
	{
		if (!caretpos || GetCaretY() > GetHeight())
		{
			SetTextOffsetY(GetBorderPadTop());
			printf("2startershit\n");
		}
	}

	return 0;
}

size_t GGuiTextBox::GetCaretIndex()
{
	return caretindex;
}

int32_t GGuiTextBox::SetCaretX(int32_t tocaretx)
{

	caretx = tocaretx;

	return 0;
}

int32_t GGuiTextBox::GetCaretX()
{
	return caretx;
}

int32_t GGuiTextBox::SetCaretY(int32_t tocarety)
{
	carety = tocarety;
	return 0;
}

int32_t GGuiTextBox::GetCaretY()
{
	return carety;
}


int32_t GGuiTextBox::SetColor(int32_t tor, int32_t tog, int32_t tob, int32_t toa)
{
	r = tor;
	g = tog;
	b = tob;
	a = toa;

	return 0;
}

int32_t GGuiTextBox::GetColor(int32_t &getr, int32_t &getg, int32_t &getb, int32_t &geta)
{
	getr = r;
	getg = g;
	getb = b;
	geta = a;

	return 0;
}

int32_t GGuiTextBox::SetFontColor(int32_t tor, int32_t tog, int32_t tob, int32_t toa)
{
	return GGuiLabel::SetColor(tor, tog, tob, toa);
}

int32_t GGuiTextBox::GetFontColor(int32_t &getr, int32_t &getg, int32_t &getb, int32_t &geta)
{
	return GGuiLabel::GetColor(getr, getg, getb, geta);
}


int32_t GGuiTextBox::SetEnterNewLine(bool toenter)
{

	enternewline = toenter;

	if (enternewline)
	{
		SetClip(false);

		SetCustomTop(0);
		SetCustomBottom(0);
		SetVCenter(false);
	}
	else {
		SetClip(true);

		SetCustomTop(GetPosY(true));
		SetCustomBottom(GetPosY(true) + GetHeight());
		SetVCenter(true);

	}

	return 0;
}

bool GGuiTextBox::GetEnterNewLine()
{
	return enternewline;
}

int32_t GGuiTextBox::SetText(const wchar_t *newtext)
{


	GGuiLabel::SetText(newtext);


	if (!textp)
		return 0;

	wcsncpy(textp, newtext, curtextsize);
	desiredcaretx = 0;
	desiredcarety = 0;

	return 0;
}

int32_t GGuiTextBox::GetText(wchar_t*buf, int32_t bufsize)
{
	return	GGuiLabel::GetText(buf, bufsize);
}

int32_t GGuiTextBox::SetTextP(wchar_t*totextp, size_t tobuffersize)
{

	if (!totextp)
	{
		GGuiLabel::SetText(L"");
		curtextsize = 0;
		textp = 0;
		maxlength = 0;
		return 0;
	}

	SetCaretIndex(0);
	curtextsize = tobuffersize;
	maxlength = tobuffersize;

	GGuiLabel::SetText(totextp);
	textp = totextp;


	return 0;
}

wchar_t* GGuiTextBox::GetTextP()
{
	return textp;
}

int32_t GGuiTextBox::OnRender(void *curgui, void* currender, void *curdraw)
{

	GGui *gui = (GGui*)curgui;
	Render *render = (Render*)currender;
	Easydraw *draw = (Easydraw*)curdraw;

	draw->SetColor(Easycolor(r, g, b, a));
	draw->OutlinedRectangle(GetPosX(true) , GetPosY(true) , GetWidth() , GetHeight() );

	Easyfont *labelfont = draw->GetFont(GGuiLabel::GetOurFontName(), GGuiLabel::GetFontName(), GGuiLabel::GetFontSize(), GGuiLabel::GetFontWeight());

	if (!labelfont)
		return 0;


	if (GetCaretIndex() > GetTextLength())
	{
		SetCaretIndex(0);
	}


	if (!enternewline)
	{
		SetCustomTop(GetPosY(true));
		SetCustomBottom(GetPosY(true) + GetHeight());
	}


	int32_t caretwidth, caretheight = 0;
	draw->GetRawTextSize(L"|", caretwidth, caretheight);

	std::wstring *copy = new std::wstring;
	copy->clear();
	std::wstring *copy2 = new std::wstring;
	copy2->clear();

	int32_t ntextwidthbeforecaret, ntextheightbeforecaret = 0;
	GetTextString(copy);
	GetTextString(copy2);

	copy->erase(GetCaretIndex(), copy->length() - GetCaretIndex());



	copy->push_back('|');
	draw->GetRawTextSize(copy->c_str(), ntextwidthbeforecaret, ntextheightbeforecaret);

	if (desiredcarety&&GetEnterNewLine())
	{

		copy2->push_back('|');
		size_t start = 0; // first visible thing
		size_t end = copy2->length(); //last visible thing

		int32_t closest_delta = 2000000;
		size_t closest_i = 0;



		for (size_t i = start; end > i; i++)
		{
			ntextwidthbeforecaret = 0;
			ntextheightbeforecaret = 0;

			std::wstring substr = copy2->substr(start, i - start);

			draw->GetRawTextSize(substr.c_str(), ntextwidthbeforecaret, ntextheightbeforecaret);

			int32_t curentcaret = GetPosY(true) + ntextheightbeforecaret + GetTextOffsetY();
			printf("CUR CARET: %d __ %ws\n", curentcaret, substr.c_str());

			int32_t delta = abs(curentcaret - desiredcarety);

			if (delta < closest_delta)
			{
				printf("NUM: %lu == DELTA: %d == %ws\n", i, delta, substr.c_str());

				closest_delta = delta;
				closest_i = i;
			}
		}

		SetCaretIndex(closest_i);

		copy2->pop_back();
	}

	if (desiredcaretx)
	{

		copy2->push_back('|');
		size_t previousn = copy2->rfind('\n', GetCaretIndex());
		size_t nextn = copy2->find('\n', GetCaretIndex());
		


		if (nextn== std::string::npos)
		{
			nextn = copy2->length();
		}

		if (previousn == std::string::npos)
		{
			previousn = 0;
		}
		else {
			previousn = previousn + 1;
		}

		if (previousn == nextn)
			nextn = 0;



		int32_t closest_delta = 2000000;
		size_t closest_i = 0;

		printf("PREVIOUSN: %lu __NEXTN: %lu\n", previousn, nextn);


		for (size_t i = previousn; nextn > i; i++)
		{
			ntextwidthbeforecaret = 0;
			ntextheightbeforecaret = 0;

			std::wstring substr = copy2->substr(previousn, i- previousn);

			draw->GetRawTextSize(substr.c_str(), ntextwidthbeforecaret, ntextheightbeforecaret);
			
			int32_t curentcaret = GetPosX(true) + ntextwidthbeforecaret + GetTextOffsetX();
			printf("CUR CARET: %d __ %ws\n", curentcaret, substr.c_str());

			int32_t delta = abs(curentcaret - desiredcaretx);

			if (delta < closest_delta)
			{
				printf("NUM: %lu == DELTA: %d == %ws\n", i, delta, substr.c_str());

				closest_delta = delta;
				closest_i = i;
			}
		}

		SetCaretIndex(closest_i);

		copy2->pop_back();
	}

	copy->pop_back();

	desiredcaretx = 0;
	desiredcarety = 0;

	delete copy2;
	copy2 = 0;

	std::wstring lastline = L"";

	if (GetEnterNewLine())
	{
		int32_t lastn = 0;

		for (size_t i = 0; i < copy->length(); i++)
		{
			if (copy->at(i) == '\n')
			{
				lastn = i;
			}
		}

		if (lastn)
		{
			lastline = copy->substr(lastn, copy->length() - lastn);
		}
	}




	if (GetEnterNewLine() && lastline.length() > 0)
	{

		int32_t llw, llh = 0;

		lastline.push_back('|');
		draw->GetRawTextSize(lastline.c_str(), llw, llh);
		lastline.pop_back();

		ntextwidthbeforecaret = llw;
	}


	if (ntextwidthbeforecaret)
		ntextwidthbeforecaret -= caretwidth; // hack

	if (ntextheightbeforecaret)
		ntextheightbeforecaret -= caretheight; // hack

	if (textwidthbeforecaret != ntextwidthbeforecaret)
	{
		printf("OLD: %d NEW: %d\n", textwidthbeforecaret, ntextwidthbeforecaret);
	}


	textwidthbeforecaret = ntextwidthbeforecaret;
	textheightbeforecaret = ntextheightbeforecaret;

	delete copy;
	copy = 0;

	int32_t caretoffsetx = textwidthbeforecaret + GetTextOffsetX();
	int32_t caretoffsety = textheightbeforecaret + GetTextOffsetY();
 

	//add same for H
	SetClipLeft(GetPosX(true) + GetBorderPadLeft());
	SetClipTop(GetPosY(true));
	SetClipRight(GetPosX(true) + GetWidth() - GetBorderPadRight());
	SetClipBottom(GetPosY(true) + GetHeight());

	GGuiLabel::OnRender(curgui, currender, curdraw);

	//caret

	if (!carettick)
		carettick = GetTickCount();

	if (GetTickCount() - carettick > 500) // more than 500ms
	{
		carettick = 0;
		caretvis = !caretvis;
	}





	SetCaretX(caretoffsetx);
	//printf("CaretX: %d, OffsetX: %d, WidthBefore: %d\n", GetCaretX(), GetTextOffsetX(), textwidthbeforecaret);

	if (GetEnterNewLine())
	{
		//printf("CaretY: %d, OffsetY: %d, HeightBefore: %d\n", GetCaretY(), GetTextOffsetY(), textheightbeforecaret);
		SetCaretY(caretoffsety);

	}
	else {
		SetCaretY(5);
	}

	int32_t caret, caretg, caretb, careta = 0;
	GetFontColor(caret, caretg, caretb, careta);
	draw->SetColor(Easycolor(caret, caretg, caretb, careta));
	draw->SetFont(labelfont);


	int32_t caretstartx = GetPosX(true) + GetCaretX();
	int32_t caretstarty = GetPosY(true) + GetCaretY();

	if (GetPosX(true) + GetBorderPadLeft() > caretstartx)
	{
		caretstartx = GetPosX(true) + GetBorderPadLeft();
		SetTextOffsetX(GetTextOffsetX() + GetBorderPadLeft());
	}


	if (caretstartx > GetPosX(true) + GetWidth() - GetBorderPadRight())
	{
		caretstartx = GetPosX(true) + GetWidth() - GetBorderPadRight();
		SetTextOffsetX(GetTextOffsetX() - GetBorderPadRight());
	}

	if (GetPosY(true) + GetBorderPadTop() > caretstarty)
	{
		caretstarty = GetPosY(true) + GetBorderPadTop();
		SetTextOffsetY(GetTextOffsetY() + GetBorderPadTop());
	}


	if (caretstarty > GetPosY(true) + GetHeight() - GetBorderPadBottom())
	{
		caretstarty = GetPosY(true) + GetHeight() - GetBorderPadBottom();
		SetTextOffsetY(GetTextOffsetY() - GetBorderPadBottom());
	}

	if (caretvis && GetFocus() == 1)
		draw->Line(caretstartx, caretstarty, caretstartx, caretstarty + caretheight);


	return 0;
}

int32_t GGuiTextBox::OnTextChanged()
{

	if (curtextsize>GetTextLength())
	{
		if (textp)
		{
			memset(textp, 0, curtextsize * 2);
			GetText(textp, curtextsize);
		}
		if (callbackfn)
		{
			int32_t ret = (int32_t)callbackfn(this, 2, (void*)textp);


			if (ret)
				return 1;

		}
	}

	return 0;
}

int32_t GGuiTextBox::OnKeyPressed(int32_t key, bool initialpress)
{
	if (GetFocus() != 1)
		return 0;


	if (!buttontick)
		buttontick = GetTickCount();

	if (GetTickCount() - buttontick < 80 && !initialpress) // more than 80ms
		return 0;

	buttontick = 0;

	if (key == VIRTUALKEY_LEFT)
	{
		if (0 >= GetCaretIndex())
			return 1;

		if (GetCaretIndex() > 0)
			SetCaretIndex(GetCaretIndex() - 1);

		//printf("%d\n", caretindex);
	}

	if (key == VIRTUALKEY_RIGHT)
	{
		if (GetTextLength()>GetCaretIndex())
			SetCaretIndex(GetCaretIndex() + 1);


		//printf("%d\n", caretindex);

	}

	if (key == VIRTUALKEY_UP)
	{
		if (0 >= GetCaretIndex())
			return 1;

		std::wstring *copy = new std::wstring;
		copy->clear();

		GetTextString(copy);

		int32_t lastn = 0;
		int32_t beforelastn = 0;

		if (caretindex > 0)
		{
			for (size_t i = caretindex - 1; i > 0; i--)
			{
				if (copy->at(i) == '\n')
				{
					if (!lastn)
						lastn = i;
					else
						beforelastn = i;

					if (beforelastn)
						break;
				}
			}
		}


		delete copy;
		copy = 0;

		if (!lastn)
		{
			SetCaretIndex(0);
			return 0;
		}

		int32_t charindexthisline = caretindex - lastn;

		if (charindexthisline)
		{
			printf("\n\n\n\nCHARINDEX: %d==%d==%d\n\n\n\n", lastn, beforelastn, charindexthisline);

			if (beforelastn && lastn > beforelastn + charindexthisline)
			{
				printf("AA\n");
				SetCaretIndex(beforelastn + charindexthisline);
				return 0;
			}
			printf("BB\n");
			SetCaretIndex(lastn);
			return 0;
		}

		printf("DD\n");
		SetCaretIndex(0);

		return 0;
	}

	if (key == VIRTUALKEY_DOWN)
	{

		if (GetCaretIndex() >= GetTextLength())
			return 1;

		std::wstring *copy = new std::wstring;
		copy->clear();

		GetTextString(copy);


		size_t len = copy->length();

		size_t lastn = 0;
		size_t beforelastn = 0;

		for (size_t i = 0; i < caretindex; i++)
		{
			if (copy->at(i) == '\n')
			{
				beforelastn = lastn;
				lastn = i;
			}
		}

		size_t nextn = 0;
		size_t afternextn = 0;

		for (size_t i = caretindex; i < len; i++)
		{

			if (copy->at(i) == '\n')
			{
				if (!nextn)
					nextn = i;
				else
					afternextn = i;

				if (afternextn)
					break;
			}
		}

		delete copy;
		copy = 0;

		size_t charindexthisline = caretindex - lastn;

		if (nextn)
		{
			if (!afternextn)
			{
				afternextn = GetTextLength();
			}

			if (afternextn && afternextn > charindexthisline + nextn)
			{
				printf("\n\n\n\nCHARINDEX: %d==%d==%d\n\n\n\n", nextn, afternextn, charindexthisline);

				if (lastn)
					SetCaretIndex(nextn + charindexthisline + 0);
				else
					SetCaretIndex(nextn + charindexthisline + 1);

				return 0;
			}
			if (GetTextLength() > afternextn)
			{
				printf("B\n");
				SetCaretIndex(afternextn);
				return 0;
			}
			printf("C\n");
			SetCaretIndex(GetTextLength());
			return 0;
		}


		printf("D\n");
		SetCaretIndex(len);

	}

	return 0;
}

int32_t GGuiTextBox::OnMousePressed(int32_t mousex, int32_t mousey, bool leftclick, bool initialpress)
{
	if (!GetFocus())
		return 0;

	if (!initialpress || !leftclick)
		return 0;


	if (desiredcaretx||desiredcarety)
		return 0;

	desiredcaretx = mousex;

	if (GetEnterNewLine())
		desiredcarety = mousey;

	printf("PRESS __ %d== %d\n", mousex , GetTextOffsetX());
	printf("PRESS2 __ %d==  %d\n", mousey, GetTextOffsetY());

	return 0;
}

bool GGuiTextBox::IsAllowedChar(int32_t charcode)
{
	return true;
}

int32_t GGuiTextBox::OnChar(int32_t charcode)
{

	if (GetFocus() != 1)
		return 0;

	if (!IsAllowedChar(charcode))
		return 1;


	if (GetMaxLength() && GetTextLength() + 1 > GetMaxLength() && charcode != '\b' && !(charcode == '\r' && !enternewline) )
	{
		return 1;
	}

	if (callbackfn)
	{
		int32_t ret =  (int32_t)callbackfn(this, 1, (void*)charcode);
		

		if (ret)
			return 1;

	}

	GGuiLabel::SetAutoSized(true);


	//printf("%d\n", charcode);

	if (charcode == '\b') // backspace
	{

		if (GetCaretIndex()>0)
		{
			GGuiLabel::RemoveChar(GetCaretIndex());
			SetCaretIndex(GetCaretIndex() - 1);



			OnTextChanged();
		}

		return 0;
	}

	if (charcode == '\r')//enter
	{

		if (enternewline)
		{
			charcode = '\n';
		}
		else {
			UnFocus();
			return 0;
		}
	}

	if (charcode == 0x03 || charcode == 0x18) // copy shit
	{
		if (GetTextLength() == 0)
		{
			return 0;
		}

		printf("COPY\n");


		if (!OpenClipboard(0))
		{
			return 0;
		}

		EmptyClipboard();

		HGLOBAL hMem = GlobalAlloc(0, (GetTextLength() + 1) * sizeof(wchar_t));

		if (!hMem)
		{
			CloseClipboard();
			return 0;
		}

		wchar_t *copydata = (wchar_t*)GlobalLock(hMem);

		if (copydata)
		{
			printf("LEN: %d\n", GetTextLength());

			GetText(copydata, (GetTextLength() + 1));


			GlobalUnlock(copydata);
			SetClipboardData(CF_UNICODETEXT, copydata);


		}


		CloseClipboard();

		if (charcode == 0x18)
		{
			SetText(L"");
			OnTextChanged();
		}

		return 0;
	}



	if (charcode == 0x16) // paste shit
	{
		if (!OpenClipboard(0))
		{
			return 0;
		}

		HANDLE hClipboardData = GetClipboardData(CF_UNICODETEXT);
		if (!hClipboardData)
		{
			CloseClipboard();
			return 0;
		}

		wchar_t *pchData = (wchar_t*)GlobalLock(hClipboardData);

		if (pchData)
		{
			for (size_t i = 0; i < wcslen(pchData); i++)
			{
				OnChar(pchData[i]);
			}
			GlobalUnlock(hClipboardData);
		}

		CloseClipboard();


		OnTextChanged();

		return 0;
	}


	printf("%x __ %d\n", charcode, caretindex);

	GGuiLabel::AddChar((wchar_t)charcode, GetCaretIndex());


	SetCaretIndex(GetCaretIndex() + 1);


	OnTextChanged();

	return 0;
}

void GGuiTextBox::SetCallbackFn(void*fn)
{
	*(void**)&callbackfn = fn;
}

void* GGuiTextBox::GetCallbackFn()
{
	return callbackfn;
}