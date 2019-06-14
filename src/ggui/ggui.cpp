#pragma once

#include <string.h>
#include <stdio.h>

#include "../orender.h"
#include "../easydraw.h"


#include "ggui.h"

#include "includeelements.h"
#include <Windows.h>

std::wstring curguichars;

int32_t getcursorpos_funcaddr, getcursorpos_syscallnum = 0;
char opbak_getcursorpos[30], opbak_setcursorpos[4] = { 0 };

bool hookproc_suppressorigwindowinput = false;

int32_t MouseHookThink()
{


	static char codens[30] = { '\x8B', '\x44', '\xE4', '\x04', '\xC7', '\x40', '\x00', '\x00', '\x00', '\x00', '\x00', '\xC7', '\x40', '\x04', '\x00', '\x00', '\x00', '\x00', '\xB8', '\x01', '\x00', '\x00', '\x00', '\xC2', '\x04', '\x00' };

	size_t protsize = (int32_t)SetCursorPos - (int32_t)GetCursorPos + 0x500;


	if (!getcursorpos_funcaddr)
	{
		memcpy(opbak_setcursorpos, SetCursorPos, 4);
		memcpy(opbak_getcursorpos, GetCursorPos, 30);

		char* getcursorpos_offset = (char*)GetCursorPos;

		size_t failed = 0;

		for (failed=0;failed<200;failed++)
		{
			getcursorpos_offset = getcursorpos_offset + 1;

			if (*(char*)getcursorpos_offset == '\xC2' && *(char*)(getcursorpos_offset + 1) == '\x04' && *(char*)(getcursorpos_offset + 2) == '\x00')
				break;
		}

		if (failed >= 150)
		{
			MessageBox(0, L"Couldn't find your P", L"ERRROR! Tell this a developer!", MB_OK);
			return 0;
		}


		int32_t calloff = *(int32_t*)((char*)getcursorpos_offset - 0x6 + 1);

		getcursorpos_funcaddr = (int32_t)((char*)getcursorpos_offset  - 0x6 + 0x5 + calloff);
		getcursorpos_syscallnum = *(int32_t*)((char*)getcursorpos_offset  - 0x10 + 1);
	}


	DWORD oldf, shitf = 0;

	if (hookproc_suppressorigwindowinput)
	{
		if (*(char*)GetCursorPos == codens[0] && *(char*)((char*)GetCursorPos + 1) == codens[1] && *(char*)SetCursorPos == '\xC2')
			return 0;

		POINT opos;
		GetCursorPos(&opos);

		*(unsigned long*)((char*)codens + 7) = opos.x;
		*(unsigned long*)((char*)codens + 14) = opos.y;

		VirtualProtect(GetCursorPos, protsize, PAGE_EXECUTE_READWRITE, &oldf);


		memcpy(GetCursorPos, codens, 30);
		memcpy(SetCursorPos, "\xC2\x08\x00\x90", 4);

		VirtualProtect(GetCursorPos, protsize, oldf, &shitf);
		return 0;
	}

	if (*(char*)GetCursorPos == opbak_getcursorpos[0] && *(char*)SetCursorPos == opbak_setcursorpos[0] && *(char*)((char*)GetCursorPos + 1) == opbak_getcursorpos[1] && *(char*)((char*)SetCursorPos + 1) == opbak_setcursorpos[1])
		return 0;

	VirtualProtect(GetCursorPos, protsize, PAGE_EXECUTE_READWRITE, &oldf);

	memcpy(GetCursorPos, opbak_getcursorpos, 30);
	memcpy(SetCursorPos, opbak_setcursorpos, 4);

	VirtualProtect(GetCursorPos, protsize, oldf, &shitf);
	return 0;

}
GGui *ggui = 0;

void rendercallback(Render*currender, Easydraw*curdraw)
{


	curdraw->SetColor(Easycolor(255, 255, 255));
	curdraw->SetFont(curdraw->GetFont(L"autism", L"courier new", 12, FONTWEIGHT_EXTRABOLD));
	curdraw->Text(L"autism", 0, 0, DRAWTEXT_NOCLIP); // does this mean  I have noclip ? =D

	curdraw->SetColor(Easycolor(255, 0, 0));



	if (!ggui)
		return;

	ggui->OnRenderCallback(currender, curdraw);
}

GGui::GGui()
{
	suppressorigwindowinput = false;
	
	x = 0;
	y = 0;

	width = 0;
	height = 0;
	fwidth = 0;
	fheight = 0;

	mousex = 0;
	mousey = 0;
	
	mousexlastpress = 0;
	mouseylastpress = 0;
	mousexwhenpressed = 0;
	mouseywhenpressed = 0;

	mouseinitialxwhendragging = 0;
	mouseinitialywhendragging = 0;

	panelinitialxwhendragging = 0;
	panelinitialywhendragging = 0;

	topfocused = 0;
	topmostdad = 0;

	ourrender = 0;
	elements_count = 0;
	elements_queueddeletion = 0;

	memset(elements, 0, sizeof(elements) / 4);
	memset(elements_todelete, 0, sizeof(elements_todelete)/4);
	memset(buttonstate, 0, sizeof(buttonstate) / 4);

}


WNDPROC origWinProc;

LRESULT CALLBACK HookProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{



	if (message == WM_CHAR && wParam)
	{
		curguichars.push_back((wchar_t)wParam);
	}

	if (hookproc_suppressorigwindowinput)
	{

		if(message >= WM_NCMOUSEMOVE && message <= WM_IME_STARTCOMPOSITION)
			return 0;

		if (message >= WM_MOUSEFIRST && message <= WM_MOUSELAST)
			return 0;

	}

	if (!wParam)
		return CallWindowProc(origWinProc, hWnd, message, wParam, lParam);



	return CallWindowProc(origWinProc, hWnd, message, wParam, lParam);
}

int32_t GGui::AttachToRender(void *initrender)
{
	Render *render = (Render*)initrender;

	ourrender = initrender;
	ourdraw = render->GetDraw();

	x = render->GetX();
	y = render->GetY();
	width = render->GetWidth();
	height = render->GetHeight();
	fwidth = render->GetFullscreenWidth();
	fheight = render->GetFullscreenHeight();

	render->SetRenderCallback(rendercallback);
	origWinProc = (WNDPROC)SetWindowLongA((HWND)render->GetTargetWindow(), GWL_WNDPROC, (long)HookProc);

	return 0;
}

void RecursiveDraw(void *curgui, void* currender, void *curdraw, GGuiBaseElement*e)
{
	if (!e)
		return;

	int32_t childrennum = e->GetChildrenNum();
	
	if (!childrennum)
		return; // all good

	if (childrennum > 0xFFFF)//what
		return;

	void** buffer = new void*[childrennum+1];

	if (!buffer)
		return;

	memset(buffer, 0, childrennum + 1);

	int32_t numchildren = 0;
	e->GetChildren(childrennum, buffer, numchildren);

	for (int32_t i = 0; i < numchildren; i++)
	{
		if (!buffer[i])
			continue; // odd

		GGuiBaseElement *child = (GGuiBaseElement*)buffer[i];

		if (!child->GetVisible() || child->GetRenderedManually())
			continue;

		child->OnRender(curgui, currender, curdraw);

		if (child->GetChildrenNum())
		{
			RecursiveDraw(curgui, currender, curdraw, child);
		}
	}

	delete[] buffer;
}
void GGui::OnRenderCallback(void*currender, void*curdraw)
{
	Render *render = (Render*)currender;
	Easydraw *draw = (Easydraw*)curdraw;

	DoQueuedDeletion();

	x = render->GetX();
	y = render->GetY();

	if (buttonstate[VIRTUALKEY_F10] == BUTTONSTATE_JUSTRELEASED)
	{
		SuppressOrigWindowInput(!GetSuppressOrigWindowInput());

		printf("Window input suppressing: %d\n", (int)GetSuppressOrigWindowInput());

	}
	draw->SetColor(Easycolor(255, 255, 255));
	draw->SetFont(draw->GetFont(L"autism", L"comic sans", 12, FONTWEIGHT_LIGHT));

	Think();

	GGuiBaseElement *etopmostdad = (GGuiBaseElement*)topmostdad;


	wchar_t* copy = 0;
	size_t copy_l = curguichars.length();

	if (copy_l)
	{
		copy = new wchar_t[copy_l + 1];
		memset(copy, 0, copy_l + 1);
		wcsncpy(copy, curguichars.c_str(), copy_l);



		curguichars.erase(0, curguichars.length());
		curguichars.clear();

	}
	
	
	GGuiBaseElement **sorted = new GGuiBaseElement*[0xFFFF];
	memset(sorted, 0, sizeof(GGuiBaseElement*)/4*0xFFFF);

	size_t sorted_num = 0;

	int32_t lastfocus = 0xFFFFF;

	bool show_cursor = false;

	for (int32_t i = 0; i < elements_count; i++)
	{
		if (!elements[i])
			continue;

		
		GGuiBaseElement *baseelement = (GGuiBaseElement*)elements[i];
		
		baseelement->Think();

		for (size_t i = 0; i < copy_l; i++)
		{
			baseelement->OnChar(copy[i]);
		}

		if (!baseelement->GetVisible())
			continue;

		if (baseelement->GetRenderedManually())
			continue;

		if (baseelement->GetParent())
			continue;


		if (baseelement == etopmostdad)
			continue;

		if (!baseelement->GetFocus())
		{

			
			if (sorted_num)
			{

				memmove((char*)sorted + 4, sorted, sorted_num*4);
			}


			sorted_num += 1;
			sorted[0] = baseelement;
		}
		else {
			if (lastfocus > baseelement->GetFocus() )
			{
				sorted[sorted_num] = baseelement;
				sorted_num += 1;
			}
			else {
				if (sorted_num)
				{
					memmove((char*)sorted + 4, sorted, sorted_num * 4);
				}
				sorted_num += 1;
				sorted[0] = baseelement;
			}

		}

		
	}


	if (etopmostdad)
	{
		sorted[sorted_num] = etopmostdad;
		sorted_num += 1;
	}


	if (sorted_num)
	{
		show_cursor = true;

		for (size_t i = 0; i < sorted_num; i++)
		{
			if (!sorted[i])
				continue;

			GGuiBaseElement *baseelement = sorted[i];
			
			draw->SetBeingClipped(true);
			draw->SetClipped(baseelement->GetPosY(true), baseelement->GetPosY(true) + baseelement->GetHeight(), baseelement->GetPosX(true), baseelement->GetPosX(true) + baseelement->GetWidth());
			baseelement->OnRender(this, currender, curdraw);
			RecursiveDraw(this, currender, curdraw, baseelement);
			draw->SetClipped(0, 0, 0, 0);
			draw->SetBeingClipped(false);
		}


	}


	if (sorted)
		delete[] sorted;

	if (copy)
		delete[] copy;

	PostThinkMouse();

	for (int32_t i = 0; i < elements_count; i++)
	{
		if (!elements[i])
			continue;


		GGuiBaseElement *baseelement = (GGuiBaseElement*)elements[i];

		baseelement->PostThink();
	}


	MouseHookThink();


	if (show_cursor)
	{

		int32_t x = mousex;
		int32_t y = mousey;

		Easydraw *draw = (Easydraw*)curdraw;

		draw->SetColor(Easycolor(255, 255, 255, 255));

		draw->Rectangle(x, y, 11, 1);
		draw->Rectangle(x + 1, y + 1, 9, 1);
		draw->Rectangle(x + 2, y + 2, 7, 1);
		draw->Rectangle(x + 3, y + 3, 7, 1);
		draw->Rectangle(x + 4, y + 4, 8, 1);
		draw->Rectangle(x + 5, y + 5, 3, 1);
		draw->Rectangle(x + 6, y + 6, 2, 1);

	}

}

int32_t GGui::ElementRegister(void* velement)
{
	elements[elements_count] = (int32_t)velement;
	elements_count++;

	return 0;
}

int32_t GGui::ElementUnregisterNoChilds(void* velement, bool shoulddelete)
{
	if (!velement)
		return 0;

	GGuiBaseElement *e = (GGuiBaseElement*)velement;
	
	uint16_t deleted = 0;

	for (int32_t i=0; i < elements_count; i++)
	{
		if ((void*)elements[i] != (void*)velement)
			continue;


		for (int32_t z = i; z < elements_count; z++)
			elements[z] = elements[z + 1];

		deleted = deleted+ 1;


	}

	if (topfocused == velement)
	{
		topfocused = 0;
	}

	if (topmostdad == velement)
	{
		topmostdad = 0;
	}

	if (!deleted)
		return 1;


	elements_count -= deleted;

	if (shoulddelete)
	{
		delete e;
	}
	return 0;
}

int32_t GGui::UnregisterChildren(void *velement, bool shoulddelete)
{
	GGuiBaseElement *e = (GGuiBaseElement*)velement;

	int32_t childrennum = e->GetChildrenNum();
	if (!childrennum)
	{
		return 1;
	}

	void** buffer = new void*[childrennum + 1];
	memset(buffer, 0, childrennum + 1);

	int32_t numchildren = 0;
	e->GetChildren(childrennum, buffer, numchildren);

	for (int32_t i = 0; i < numchildren; i++)
	{


		GGuiBaseElement *child = (GGuiBaseElement*)buffer[i];

		if (!child)
			continue; // odd

		if (child->GetChildrenNum())
		{
			UnregisterChildren(child, shoulddelete);
		}

		ElementUnregisterNoChilds(child, shoulddelete);


	}
	
	delete[] buffer;

	return 0;

}

int32_t GGui::ElementUnregister(void* velement, bool deleteothers, bool deleteus)
{
	if (!velement)
		return 1;



	GGuiBaseElement *e = (GGuiBaseElement*)velement;

	if (!e->GetChildrenNum())
	{
		ElementUnregisterNoChilds(velement, deleteus);

		return 0;
	}

	UnregisterChildren(e, deleteothers);
	ElementUnregisterNoChilds(velement, deleteus);

	return 0;
}

int32_t GGui::MarkForDeletion(void *velement)
{
	elements_todelete[elements_queueddeletion] = (int)velement;
	elements_queueddeletion += 1;

	return 0;
}

int32_t GGui::DoQueuedDeletion()
{
	for (uint16_t i = 0; i < elements_queueddeletion; i++)
	{
		ElementUnregister((void*)elements_todelete[i], true, true);
		elements_todelete[i] = 0;
	}
	elements_queueddeletion = 0;

	return 0;

}
int32_t GGui::ThinkKeys()
{

	for (int32_t i = 0; i < 0xFFF; i++)
	{

		if (GetAsyncKeyState(i) & 0x8000)
		{
			if (!buttonstate[i])
			{
				buttonstate[i] = BUTTONSTATE_JUSTPRESSED;
			}
			else {
				buttonstate[i] = BUTTONSTATE_PRESSED;
			}
		}
		else {
			if (buttonstate[i] != BUTTONSTATE_RELEASED)
			{
				if (buttonstate[i] == BUTTONSTATE_JUSTRELEASED)
				{
					buttonstate[i] = BUTTONSTATE_RELEASED;
				}
				else {
					buttonstate[i] = BUTTONSTATE_JUSTRELEASED;
				}
			}

		}
	}


	return 0;
}

int32_t GGui::ThinkMouse()
{
	bool hoveringsth = false;

	MouseHookThink();


	POINT CurMousePos;
	int32_t pcurmousepos = (int32_t)&CurMousePos;

	if (!getcursorpos_funcaddr)
	{
		GetCursorPos(&CurMousePos);
	}
	else {
		__asm
		{
			push getcursorpos_syscallnum;
			push 1;
			push pcurmousepos;
			call getcursorpos_funcaddr;
		}
	}

	CurMousePos.x =  CurMousePos.x - x;
	CurMousePos.y =   CurMousePos.y - y;

	if (CurMousePos.x != mousex || CurMousePos.y != mousey)
	{

		mousex = CurMousePos.x;
		mousey = CurMousePos.y;

		for (int32_t i = 0; i < elements_count; i++)
		{
			GGuiBaseElement*element = (GGuiBaseElement*)elements[i];

			if (!element)
				continue;

			int32_t x = element->GetPosX(true);
			int32_t y = element->GetPosY(true);

			int32_t w = element->GetWidth();
			int32_t h = element->GetHeight();

		

			element->OnMouseMove(mousex, mousey);

			GGuiBaseElement *par = element->GetParent();

			if (par) // cant have a bigger input area than daddy
			{
				if (w>par->GetWidth())
				{
					w = par->GetWidth();
				}

				if (h>par->GetHeight())
				{
					h = par->GetHeight();
				}
			}



			if (CurMousePos.x >= x && x + w >= CurMousePos.x && CurMousePos.y >= y && y + h >= CurMousePos.y)
			{
				element->SetMouseOver(true, mousex, mousey);
				element->OnMouseHover(mousex, mousey);
				hoveringsth = true;
			}
			else {
				if (element->GetMouseOver())
				{
					element->SetMouseOver(false);
				}
			}

		}
	}

	//Implent automatic suppressing pl0x

	/*
	if (hoveringsth)
		SuppressOrigWindowInput(true);
	else
		SuppressOrigWindowInput(false);
	*/

	for (int32_t bid = 0; bid < 0xFFF; bid++)
	{
		if (!buttonstate[bid]) continue;

		if (buttonstate[bid] == BUTTONSTATE_JUSTPRESSED)
		{
			for (int32_t i = 0; i < elements_count; i++)
			{
				GGuiBaseElement*element = (GGuiBaseElement*)elements[i];

				if (!element)
					continue;

				if (bid == VK_LBUTTON)
				{
					if (element->GetFocus() == 1)
					{

						mousexwhenpressed = mousex;
						mouseywhenpressed = mousey;
					}


					element->OnMousePressed(mousex, mousey, true, true);
				}

				if (bid == VK_RBUTTON)
				{
					element->OnMousePressed(mousex, mousey, false, true);
				}

				element->OnKeyPressed(bid, true);



			}
			continue;

		}

		if (buttonstate[bid] == BUTTONSTATE_PRESSED)
		{
			for (int32_t i = 0; i < elements_count; i++)
			{
				GGuiBaseElement*element = (GGuiBaseElement*)elements[i];

				if (!element)
					continue;

				if (bid == VK_LBUTTON)
				{
					element->OnMousePressed(mousex, mousey, true, false);
				}

				if (bid == VK_RBUTTON)
				{
					element->OnMousePressed(mousex, mousey, false, false);
				}

				element->OnKeyPressed(bid, false);
			}

			continue;
		}

		if (buttonstate[bid] == BUTTONSTATE_JUSTRELEASED)
		{
			for (int32_t i = 0; i < elements_count; i++)
			{
				GGuiBaseElement*element = (GGuiBaseElement*)elements[i];

				if (!element)
					continue;

				if (bid == VK_LBUTTON)
				{
					element->OnMouseReleased(mousex, mousey, true);
				}

				if (bid == VK_RBUTTON)
				{
					element->OnMouseReleased(mousex, mousey, false);
				}

				element->OnKeyReleased(bid);
			}

			if (bid == VK_LBUTTON)
			{
				GGuiBaseElement *topele = (GGuiBaseElement*)topfocused;

				if (topele)
				{
					topele->SetDragged(false);
				}

				mousexwhenpressed = 0;
				mouseywhenpressed = 0;
				panelinitialxwhendragging = 0;
				panelinitialywhendragging = 0;
				mouseinitialxwhendragging = 0;
				mouseinitialywhendragging = 0;
			}

			continue;
		}

	}



	if ( buttonstate[VK_LBUTTON] == BUTTONSTATE_JUSTPRESSED)
	{
		UpdateFocuses();
	}




	return 0;
}

int32_t GGui::PostThinkMouse()
{
	GGuiBaseElement* topele = (GGuiBaseElement*)topfocused;

	if (!topele || !topele->GetDraggable() || buttonstate[VK_LBUTTON] != BUTTONSTATE_PRESSED)
	{

		mousexlastpress = mousex;
		mouseylastpress = mousey;
		//he pressed somewhere, where there are no elements
		return 0;
	}


	return 0;
}

int32_t GGui::ThinkDrag()
{
	if (!topfocused)
		return 0;

	GGuiBaseElement *topele = (GGuiBaseElement*)topfocused;

	//printf("%ws __ %x __ %x __ %x __ %x\n", topele->GetType(), topele->GetVisible(), topele->GetDraggable(true, mousex, mousey), topele->GetFocus(), GetButtonState(VK_LBUTTON));

	if (!topele->GetVisible() ||  topele->GetFocus() != 1 )
		return 0;


	if (!topele->GetDragged())
	{
		if (GetButtonState(VK_LBUTTON) != BUTTONSTATE_JUSTPRESSED || !topele->GetDraggable(true, mousex, mousey))
			return 0;

		panelinitialxwhendragging = topele->GetPosX();
		panelinitialywhendragging = topele->GetPosY();
		mouseinitialxwhendragging = mousex;
		mouseinitialywhendragging = mousey;
		topele->SetDragged(true);
		return 0;
	}
	else {
		if ( GetButtonState(VK_LBUTTON) == BUTTONSTATE_RELEASED)
		{
			panelinitialxwhendragging = panelinitialywhendragging = mouseinitialxwhendragging = mouseinitialywhendragging = 0;
			topele->SetDragged(false);
			return 0;
		}
	}


	int offsetx = mouseinitialxwhendragging - panelinitialxwhendragging;
	int offsety = mouseinitialywhendragging - panelinitialywhendragging;

	int newx = mousex - offsetx;
	int newy = mousey - offsety;

	
	if (0 > newx)
	{
		newx = 1;
	}

	if (0 > newy)
	{
		newy = 1;
	}

	if (newx + topele->GetWidth() > width)
	{
		newx = width - topele->GetWidth();
	}


	if (newy + topele->GetHeight() > height)
	{
		newy = height - topele->GetHeight();
	}

	if (topele->GetParent())
	{
		if (newx + topele->GetWidth() + topele->GetParent()->GetPosPaddingX() > topele->GetParent()->GetWidth())
		{
			newx = topele->GetParent()->GetWidth() - topele->GetPosPaddingX() -  topele->GetWidth();
		}


		if (newy + topele->GetHeight() + topele->GetParent()->GetPosPaddingY() > topele->GetParent()->GetHeight())
		{
			newy = topele->GetParent()->GetHeight() - topele->GetParent()->GetPosPaddingY() -topele->GetHeight();
		}
	}


	topele->SetPos(newx, newy);

	return 0;
}


int32_t GGui::UpdateFocuses()
{
	int32_t efocus = 0;


	GGuiBaseElement *etopmostdad = 0;

		for (int32_t i = 0; i < elements_count; i++)
		{

			GGuiBaseElement*element = (GGuiBaseElement*)elements[i];

			if (!element)
				continue;


			if (!element->GetVisible() || !element->GetMouseOver())
				continue;


			if (element->GetParent())
				continue;


			topmostdad = element;
			etopmostdad = (GGuiBaseElement*)topmostdad;

		}

	if (!topmostdad) // theres no frame on top
	{
		UpdateLostFocuses();
	}

	etopmostdad = (GGuiBaseElement*)topmostdad;

	if (etopmostdad)
	{

		printf("%x __ %ws\n", (unsigned int)etopmostdad, etopmostdad->GetType());
	}

	GGuiBaseElement *topmost = 0;

	for (int i = 0; i < elements_count; i++) // look for our the topmost selected child 
	{

		GGuiBaseElement *child = (GGuiBaseElement*)elements[i];


		if (!child || !child->GetVisible() || !child->GetMouseOver())
			continue;

		if (child == topmostdad)
			continue;

		if (etopmostdad)
		{
			GGuiBaseElement *root = child->GetRoot();

			if (!root)
				continue;

			if (root != etopmostdad)
				continue;
		}

		topmost = child;

	}


	int32_t newfocus = 0;

	if (!topmost) // we have no childs
	{
		newfocus += 1;

		if (etopmostdad)
		{
			etopmostdad->OnFocusChange(newfocus, (GGuiBaseElement*)topfocused);
			etopmostdad->SetFocus(newfocus);
		}



		topfocused = topmostdad;

		printf("nokids\n");

	}
	else {

		topfocused = topmost;
		GGuiBaseElement *cur = topmost;

		while (cur)
		{
			newfocus += 1;
			cur->OnFocusChange(newfocus, (GGuiBaseElement*)topfocused);
			cur->SetFocus(newfocus);
			cur = cur->GetParent();
		}

		newfocus += 1;

		if (etopmostdad)
		{
			etopmostdad->OnFocusChange(newfocus, (GGuiBaseElement*)topfocused);
		}
	}

	if (newfocus && topmost && topmost->GetType())
	{
		printf("%ws\n", topmost->GetType());
	}



	UpdateLostFocuses();
	return 0;
}

int32_t GGui::UpdateLostFocuses()
{
	for (int i = 0; i < elements_count; i++) // make the ones that aren't even hovered lose focus
	{
		if (!elements[i])
			continue;

		GGuiBaseElement *othere = (GGuiBaseElement*)elements[i];



		if (!othere->GetFocus()) //this didn't even have a focus
			continue;


		// mouse is over it, has focus
		if (othere->GetMouseOver())
			continue;

		othere->OnFocusChange(0, (GGuiBaseElement*)topfocused);
		othere->SetFocus(0); // we're going to relook for the focuses

	}

	return 0;
}

int32_t GGui::Think()
{
	ThinkKeys();
	ThinkMouse();
	ThinkDrag();

	return 0;
}