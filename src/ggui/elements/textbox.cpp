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
	alltextmarked = false;
	addoldcaretcords = false;
	oldcaret = 0;
	oldcaretx = 0;
	oldcarety = 0;

	desiredcaretmoved = false;
	desiredcaretchanged = false;
	desiredcaretset = 0;
	desiredcaretx = 0;
	desiredcarety = 0;

	callbackfn = 0;
	maxlength = 0;

	borderpadbottom = borderpadtop = borderpadleft = borderpadright = 6;

	caretvis = 0;
	carettick = 0;
	buttontick = 0;
	r = g = b = a = 255;

	textwidthbeforecaret = textheightbeforecaret = 0;

	textp = 0;

	caretx = 0;
	carety = 0;
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

bool GGuiTextBox::GetTextBeingMarked()
{
	return desiredcaretmoved || alltextmarked || oldcaretx|| oldcarety;
}

bool GGuiTextBox::GetAllTextBeingMarked()
{
	return alltextmarked;
}

int32_t GGuiTextBox::SetTextBeingMarked(bool marked)
{
	if (marked)
	{
		alltextmarked = true;
	}
	else {
		oldcaret = oldcaretx = oldcarety = desiredcarety = desiredcaretx = desiredcaretset = 0;
		desiredcaretchanged = desiredcaretmoved = false;
		addoldcaretcords = false;
		alltextmarked = false;
	}
	return 0;
}

size_t GGuiTextBox::GetCaretOldIndex()
{
	return oldcaret;
}

int32_t GGuiTextBox::SetCaretOldIndex(size_t toindex)
{
	oldcaret = toindex;
	return 0;
}

int32_t GGuiTextBox::SetCaretIndex(size_t caretpos)
{

	if (caretpos>GetTextLength())
	{
		return 1;
	}

	caretindex = caretpos;

	printf("SetCaretIndexX: =%d =%d = %d = %d\n", textwidthbeforecaret, GetCaretX(), -GetTextOffsetX(), GetWidth());

	if (GetCaretX() > GetWidth() - GetBorderPadRight()) // caret is than w, move text
	{

		int32_t offset = (textwidthbeforecaret - GetWidth() + GetBorderPadRight());

		printf("MOVE: %d\n", offset);
		SetTextOffsetX(-offset);

		return 0;
	}
	else
	{
		if (!caretpos || GetCaretX()> GetWidth())
		{
			SetTextOffsetX(GetBorderPadLeft());
			printf("startershit\n");
		}
	}

	if (!GetEnterNewLine())
		return 0;


	printf("SetCaretIndexY: =%d =%d = %d = %d\n", textheightbeforecaret, GetCaretY(), -GetTextOffsetY(), GetHeight());

	if (GetCaretY() > GetHeight() - GetBorderPadBottom()) // caret is than h, move text
	{

		int32_t offset = (textheightbeforecaret - GetHeight() + GetBorderPadBottom());

		printf("MOVE: %d\n", offset);
		SetTextOffsetY(-offset);

		return 0;
	}
	else
	{
		if (!caretpos || GetCaretY()> GetHeight())
		{
			SetTextOffsetY(GetBorderPadTop());
			printf("startershit\n");
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

	if (GetCaretIndex()>wcslen(textp))
		SetCaretIndex(wcslen(textp));

	desiredcaretchanged = false;
	desiredcaretset = 0;
	desiredcaretx = 0;
	desiredcarety = 0;
	oldcaretx = 0;
	oldcarety = 0;
	oldcaret = 0;

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


	if (!enternewline)
	{
		SetCustomTop(GetPosY(true));
		SetCustomBottom(GetPosY(true) + GetHeight());
	}


	int32_t caretwidth, caretheight = 0;
	draw->GetRawTextSize(L"|", caretwidth, caretheight);

	std::wstring *copy = new std::wstring;
	copy->clear();


	int32_t ntextwidthbeforecaret, ntextheightbeforecaret = 0;
	GetTextString(copy);

	size_t previousn = 0;
	size_t nextn = 0;
	previousn = copy->rfind('\n', GetCaretIndex()-1);
	nextn = copy->find('\n', GetCaretIndex()-1);




	if (nextn == std::wstring::npos)
		nextn = 0;

	if (previousn == std::wstring::npos)
		previousn = 0;

	if (previousn == nextn)
		nextn = 0;





	wchar_t nextchar = 0;

	if (copy->length()>GetCaretIndex() )
		nextchar = copy->at(GetCaretIndex());

	copy->erase(GetCaretIndex(), copy->length() - GetCaretIndex()); //erase after the caret

	

	copy->push_back('|');
	draw->GetRawTextSize(copy->c_str(), ntextwidthbeforecaret, ntextheightbeforecaret);
	if (GetEnterNewLine())
	{
		copy->pop_back();
	}
	

	if (GetEnterNewLine())
	{
		
		int32_t trashint = 0;
		

		copy->erase(0, previousn);//erase everything before this line


		copy->push_back('|');
		draw->GetRawTextSize(copy->c_str(), ntextwidthbeforecaret, trashint);//height can stay the way it is

	}

	delete copy;
	copy = 0;


	if (desiredcaretchanged && (desiredcarety ||desiredcaretx))
	{
		size_t oldindex = GetCaretIndex();

		desiredcaretchanged = false;
		std::wstring *copy2 = new std::wstring;
		GetTextString(copy2);

		
		printf("PRE: %ws\n", copy2->c_str());
		bool found = false;
		size_t bestcaret = 0;

		int32_t numn = 0;

		size_t lastn = 0;

		for (size_t i = 0; i < copy2->length(); i++)
		{

			int32_t curcharw = 0;
			int32_t curcharh = 0;

			wchar_t curchar[2] = { 0 };
			curchar[0] = copy2->at(i);


			if (copy2->at(i) == ' ')
				curchar[0] = 'A';

			draw->GetRawTextSize(curchar, curcharw, curcharh);
			
			
			std::wstring text = copy2->substr(lastn, i- lastn).c_str();
			
			if (copy2->at(i) == ' ')
			{
				curcharw = 5;
				text.append(L".");
			}

			draw->GetRawTextSize(text.c_str(), ntextwidthbeforecaret, ntextheightbeforecaret);
			
			if (copy2->at(i) == '\n')
			{
				lastn = i;
				numn = numn + 1;
				curchar[0] = 'A';

				draw->GetRawTextSize(curchar, curcharw, curcharh);
				continue;

			}

			size_t startx = GetPosX(true) + ntextwidthbeforecaret + GetTextOffsetX();
			size_t starty = GetPosY(true)  + GetTextOffsetY() + curcharh*numn;
			
			size_t endx = startx + curcharw;
			size_t endy = starty + curcharh;

			
			if (desiredcaretx >= startx && desiredcaretx <= endx && (!GetEnterNewLine() || desiredcarety >= starty && desiredcarety <=  endy) )
			{
				//yep that's the char
				printf("yeah: %d:%d | %d:%d | %d:%d\n", desiredcaretx, desiredcarety, startx, starty, endx, endy);
				bestcaret = i;
				found = true;

			}

		}

		if (found)
		{
			SetCaretIndex(bestcaret);
		}
		else {
			
			bestcaret = 0;
			numn = 0;
			lastn = 0;

			for (size_t i = 0; i < copy2->length(); i++)
			{

				int32_t curcharw = 0;
				int32_t curcharh = 0;

				wchar_t curchar[2] = { 0 };
				curchar[0] = copy2->at(i);


				draw->GetRawTextSize(curchar, curcharw, curcharh);

				if (copy2->at(i) == '\n')
				{

					lastn = i;
					numn = numn + 1;
					curchar[0] = 'A';

					draw->GetRawTextSize(curchar, curcharw, curcharh);
					continue;
				}

				std::wstring text = copy2->substr(lastn, i - lastn).c_str();

				draw->GetRawTextSize(text.c_str(), ntextwidthbeforecaret, ntextheightbeforecaret);


				size_t startx = GetPosX(true) + ntextwidthbeforecaret + GetTextOffsetX();
				size_t starty = GetPosY(true) + GetTextOffsetY() + curcharh*numn;

				size_t endx = startx + curcharw;
				size_t endy = starty + curcharh;


				if ( desiredcarety >= starty && desiredcarety <= endy)
				{
					//yep that's the char
					printf("yeah2: %d:%d | %d:%d | %d:%d\n", desiredcaretx, desiredcarety, startx, starty, endx, endy);
					bestcaret = i;
					found = true;
				}

			}

			if (found)
				SetCaretIndex(bestcaret+1);
		}




		if (found && desiredcaretset == 0)
		{
			if (oldindex == GetCaretIndex())
			{
				SetTextBeingMarked(false);
			}
			else {
				desiredcaretset = 1;
				oldcaret = bestcaret;
				addoldcaretcords = true;

			}
		}

		delete copy2;
		copy2 = 0;
		return OnRender(curgui, currender, curdraw);
	}





	std::wstring lastline = L"";


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


	int32_t caretoffsetx = textwidthbeforecaret + GetTextOffsetX();
	int32_t caretoffsety = textheightbeforecaret + GetTextOffsetY();
 
	if (addoldcaretcords)
	{
		addoldcaretcords = false;
		oldcaretx = GetPosX(true) + caretoffsetx;
		oldcarety = GetPosY(true) +  caretoffsety;
	}

	int32_t oldtop, oldbottom, oldleft, oldright = 0;

	draw->GetClipped(oldtop, oldbottom, oldleft, oldright);
	if ((oldcaretx && oldcaretx - GetPosX(true) != caretoffsetx) || (oldcarety  && oldcarety - GetPosY(true) != caretoffsety ) || GetAllTextBeingMarked() )
	{

		draw->SetClipped(GetPosY(true) + GetBorderPadTop(), GetPosY(true) + GetHeight() - GetBorderPadBottom(), GetPosX(true) + GetBorderPadLeft(), GetPosX(true) + GetWidth() - GetBorderPadRight());
		std::wstring *copy2 = new std::wstring;
		GetTextString(copy2);

		size_t lastn = 0;
		size_t numn = 0;




		int32_t dotwidth = 0;
		int32_t dotheight = 0;
		int32_t awidth = 0;
		int32_t aheight = 0;

		wchar_t curchar[2] = { 0 };
		curchar[0] = 'A';

		draw->GetRawTextSize(curchar, awidth, aheight);
		draw->GetRawTextSize(L".", dotwidth, dotheight);

		copy2->append(L".");
		if (copy2->back() != '\n')
			copy2->append(L"\n");


		bool didcut = false;

		for (size_t i = 0; i < copy2->length(); i++)
		{

			if (copy2->at(i) != '\n')
				continue;



			size_t cutstart = 0;
			size_t cutend = 0;

			if (GetAllTextBeingMarked())
			{
				cutend = GetTextLength();
			}
			else {
				if (GetCaretIndex() > GetCaretOldIndex())
				{
					cutstart = GetCaretOldIndex();
					cutend = GetCaretIndex();
				}
				else {
					cutstart = GetCaretIndex();
					cutend = GetCaretOldIndex();
				}
			}


			if (!GetAllTextBeingMarked())
			{
				if (cutstart > i) // this line starts before the cut start
				{

					lastn = i;
					numn = numn + 1;
					continue; // dont draw this line
				}


				if (cutend < lastn) // this line starts after the cut end
				{
					break;
				}
			}


			//printf("LASTN: %d _ i - lastn: %d\n", lastn, i - lastn);

			std::wstring text = copy2->substr(lastn, i - lastn);
			std::wstring cutoffleft = L"";
			std::wstring cutoffright = L"";

			if (!GetAllTextBeingMarked())
			{

				if (cutstart > lastn)
				{
					cutoffleft = text.substr(0, cutstart - lastn);
				}

				size_t lineend = lastn + text.length();

				if (lineend > cutend) // is this whole line bigger than what we want to cut?
				{
					cutoffright = text.substr(cutend - lastn, lineend - cutend);
					printf("str: %ws\n", cutoffright.c_str());
				}

			}

			text.append(L".");


			int32_t trash = 0;

			int32_t textw = 0;
			int32_t cutoffleftw = 0;
			int32_t cutoffrightw = 0;

			draw->GetRawTextSize(text.c_str(), textw, trash);

			if (cutoffleft.length())
				draw->GetRawTextSize(cutoffleft.c_str(), cutoffleftw, trash);

			if (cutoffright.length())
				draw->GetRawTextSize(cutoffright.c_str(), cutoffrightw, trash);

			textw -= dotwidth;

			int32_t startx = GetPosX(true) + cutoffleftw + GetTextOffsetX();
			int32_t starty = GetPosY(true) + GetTextOffsetY() + (aheight*numn);

			/*
			if (GetPosY(true) > starty ||  GetPosX(true) > startx || starty + aheight > GetPosY(true) + GetHeight() || startx + textw - cutoffleftw - cutoffrightw  > GetPosX(true) + GetWidth() )
			{
				lastn = i;
				numn = numn + 1;
				continue;
			}*/

			draw->SetColor(Easycolor(255, 0, 0));

			printf("%d __ %d stock options", cutoffleftw, cutoffrightw);

			draw->Rectangle(startx, starty, textw - cutoffleftw - cutoffrightw, aheight);

			lastn = i;
			numn = numn + 1;


		}



		delete copy2;
		copy2 = 0;

	}
	draw->SetClipped(oldtop, oldbottom, oldleft, oldright);

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
	SetCaretY(caretoffsety);
	//printf("CaretX: %d, OffsetX: %d, WidthBefore: %d\n", GetCaretX(), GetTextOffsetX(), textwidthbeforecaret);


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


	if (caretstartx+caretwidth > GetPosX(true) + GetWidth() - GetBorderPadRight())
	{
		caretstartx = GetPosX(true) + GetWidth() - GetBorderPadRight();
		SetTextOffsetX(GetTextOffsetX() - GetBorderPadRight());
	}

	if (GetPosY(true) + GetBorderPadTop() > caretstarty)
	{
		caretstarty = GetPosY(true) + GetBorderPadTop();
		SetTextOffsetY(GetTextOffsetY() + GetBorderPadTop());
	}

	if (GetEnterNewLine())
	{
		if (caretstarty +caretheight > GetPosY(true) + GetHeight() - GetBorderPadBottom())
		{
			caretstarty = GetPosY(true) + GetHeight() - GetBorderPadBottom();
			SetTextOffsetY(GetTextOffsetY() - GetBorderPadBottom());
		}
	}
	else {
		if (caretstarty > GetPosY(true) + GetHeight() - GetBorderPadBottom())
		{
			caretstarty = GetPosY(true) + GetHeight() - GetBorderPadBottom();
			SetTextOffsetY(GetTextOffsetY() - GetBorderPadBottom());
		}
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

		SetTextBeingMarked(false);

		if (0 >= GetCaretIndex())
			return 1;

		if (GetCaretIndex() > 0)
			SetCaretIndex(GetCaretIndex() - 1);


		//printf("%d\n", caretindex);
	}

	if (key == VIRTUALKEY_RIGHT)
	{

		SetTextBeingMarked(false);

		if (GetTextLength()>GetCaretIndex())
			SetCaretIndex(GetCaretIndex() + 1);

		//printf("%d\n", caretindex);

	}

	if (key == VIRTUALKEY_UP)
	{

		SetTextBeingMarked(false);

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
		SetTextBeingMarked(false);

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

	if (!leftclick)
		return 0;


	alltextmarked = false;

	if (!initialpress)
	{

			if (desiredcaretx != mousex || desiredcarety != mousey)
			{
				desiredcaretmoved = true;
				desiredcaretchanged = true;
				desiredcaretx = mousex;
				desiredcarety = mousey;
			}

			return 0;

	}


	if (!initialpress)
		return 0;

	SetTextBeingMarked(false);

	desiredcaretchanged = true;
	desiredcaretx = mousex;
	desiredcarety = mousey;


	printf("PRESS __ %d== %d\n", mousex , GetTextOffsetX());
	printf("PRESS2 __ %d==  %d\n", mousey, GetTextOffsetY());

	return 0;
}

int32_t GGuiTextBox::OnMouseReleased(int32_t mousex, int32_t mousey, bool leftclick)
{
	if (!GetFocus())
		return 0;

	if (!leftclick)
		return 0;


	printf("RELEASE  __ %d== %d\n", mousex, GetTextOffsetX());
	printf("RELEASE2 __ %d==  %d\n", mousey, GetTextOffsetY());

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


	if (GetMaxLength() && GetTextLength() + 1 > GetMaxLength() && charcode != 0x03 && charcode != 0x18 && charcode != '\b' && !(charcode == '\r' && !enternewline) )
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

	if (charcode == '\n' && !GetEnterNewLine())
		return 1;

	if (charcode == '\b') // backspace
	{
		if (GetTextBeingMarked())
		{
			if (GetAllTextBeingMarked())
			{
				SetText(L"");
				OnTextChanged();
				SetTextBeingMarked(false);
				return 0;
			}

			size_t caretindex = GetCaretIndex();
			size_t oldindex = GetCaretOldIndex();

			SetTextBeingMarked(false);

			if (caretindex > oldindex)
			{
				for (size_t i = oldindex; i < caretindex - 1; i++)
				{
					OnChar('\b');
				}
			}

			if (oldindex>caretindex)
			{
				SetCaretIndex(oldindex + 1);
				for (size_t i = caretindex; i < oldindex; i++)
				{
					OnChar('\b');
				}
			}
		}

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
			SetTextBeingMarked(false);
			UnFocus();
			return 0;
		}
	}

	if (charcode == 0x01) // ctrl+a | mark everything
	{
		if (GetTextLength() == 0)
		{
			return 0;
		}
		SetTextBeingMarked(true);
		return 0;
	}

	if (charcode == 0x03 || charcode == 0x18) // ctrl+c || ctrl+x | copy stuff
	{
		if (GetTextLength() == 0)
		{
			return 0;
		}

		size_t caretindex = GetCaretIndex();
		size_t oldindex = GetCaretOldIndex();



		printf("COPY\n");


		if (!OpenClipboard(0))
		{
			return 0;
		}

		EmptyClipboard();

		HGLOBAL hMem = 0;
		
		size_t copyfrom = 0;
		size_t copylen = 0;

		if (GetTextBeingMarked() && !GetAllTextBeingMarked() && GetTextLength() > 1)
		{
			if (caretindex > oldindex)
			{
				copyfrom = oldindex;
				copylen = caretindex - oldindex + 1;
			}

			if (oldindex>caretindex)
			{
				copyfrom = caretindex;
				copylen = oldindex - caretindex + 2;
			}
		}
		else {
			copyfrom = 0;
			copylen = GetTextLength() + 1;
		}
		
			
		hMem = GlobalAlloc(0, copylen * sizeof(wchar_t));

		if (!hMem)
		{
			CloseClipboard();
			return 0;
		}

		wchar_t *tmptxtbuf = new wchar_t[GetTextLength() + 1];
		GetText(tmptxtbuf, (GetTextLength() + 1));

		wchar_t *copydata = (wchar_t*)GlobalLock(hMem);

		if (copydata)
		{
			printf("LEN: %d\n", GetTextLength());

			wcsncpy(copydata, tmptxtbuf + copyfrom, copylen);


			GlobalUnlock(copydata);
			SetClipboardData(CF_UNICODETEXT, copydata);


		}

		delete tmptxtbuf;
		tmptxtbuf = 0;


		CloseClipboard();

		if (charcode == 0x18)
		{
			if (GetTextBeingMarked() && !GetAllTextBeingMarked())
			{
				OnChar('\b');
				SetTextBeingMarked(false);
			}
			else {
				SetText(L"");
				OnTextChanged();
			}
		
		}

		return 0;
	}



	if (charcode == 0x16) // ctrl+v | paste stuff
	{
		if (!OpenClipboard(0))
		{
			return 0;
		}

		if (GetTextBeingMarked())
		{
			OnChar('\b');
		}
		SetTextBeingMarked(false);

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
				if (pchData[i] == '\r')
					continue; //fuck the dumb \r\n shit

				OnChar(pchData[i]);
			}
			GlobalUnlock(hClipboardData);
		}

		CloseClipboard();


		OnTextChanged();

		return 0;
	}


	printf("%x __ %d\n", charcode, caretindex);

	if (GetTextBeingMarked())
	{
		OnChar('\b');
	}

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