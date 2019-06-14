#pragma once

#include <cstdint>

#include "baseelement.h"

#include "../../orender.h"
#include "../../easydraw.h"
#include "../ggui.h"

#include <stdio.h>


int32_t GGuiBaseElement::Unregister(bool deleteus)
{
	ggui->MarkForDeletion(this);
	return 0;
}

GGuiBaseElement::GGuiBaseElement()
{
	renderedmanually = false;
	draggable = false;

	parent = 0;
	beingdragged = 0;
	focus = 0;


	x = 0;
	y = 0;
	w = 1;
	h = 1;
	x_padding = 0;
	y_padding = 0;

	mouseoverx = mouseovery = 0;

	visible = 1;

	ggui->ElementRegister(this);
}

GGuiBaseElement::GGuiBaseElement(void*toparent) : GGuiBaseElement()
{
	SetParent(toparent);
}

int32_t GGuiBaseElement::SetParent(void*toparent)
{
	parent = (GGuiBaseElement*)toparent;
	return 0;
}

int32_t GGuiBaseElement::Center()
{
	SetPos(ggui->GetWidth() / 2 - w / 2, ggui->GetHeight() / 2 - h / 2);



	return 0;
}
int32_t GGuiBaseElement::SetPos(int32_t tox, int32_t toy)
{
	x = tox;
	y = toy;


	return 0;
}

int32_t GGuiBaseElement::SetSize(int32_t width, int32_t height)
{
	w = width;
	h = height;

	return 0;
}


GGuiBaseElement* GGuiBaseElement::GetParent()
{
	return parent;
}


GGuiBaseElement* GGuiBaseElement::GetRoot()
{
	GGuiBaseElement*e = this;

	while (e->GetParent())
	{
		GGuiBaseElement*par = e->GetParent();
		
		if (!par)
			break;

		e = par;
	}

	return e;
}

GGuiBaseElement* GGuiBaseElement::GetVisibleRoot()
{
	GGuiBaseElement*e = this;

	while (e->GetParent())
	{
		GGuiBaseElement*par = e->GetParent();

		if (!par)
			break;

		if (!par->GetVisible())
			return 0;

		e = par;
	}

	return e;
}

int32_t GGuiBaseElement::GetParentNum(bool ignoreinvis)
{
	int32_t numparents = 0;
	GGuiBaseElement *cur = 0;

	while (cur && cur->GetParent())
	{
		if (ignoreinvis)
		{
			if(cur->GetParent()->GetVisible())
				numparents++;
		}
		else {
			numparents++;
		}

		cur = cur->GetParent();

	}
	return numparents;
}

int32_t GGuiBaseElement::GetChildren(int32_t buffersize, void **buffer, int32_t &numchildren)
{
	numchildren = 0;

	for (uint16_t i = 0; i < ggui->GetElementsCount(); i++)
	{
		GGuiBaseElement *e = (GGuiBaseElement*)ggui->GetElements()[i];

		if (!e)
			continue;

		if (numchildren > buffersize)
			return 1;

		if (e->GetParent() != this)
			continue;

		buffer[numchildren] = e;
		numchildren += 1;

	}

	return 0;
}

int32_t GGuiBaseElement::GetChildrenNum()
{
	int32_t numchildren = 0;

	if (!ggui)
		return numchildren;

	if (!ggui->GetElements())
		return numchildren;

	for (uint16_t i = 0; i < ggui->GetElementsCount(); i++)
	{
		GGuiBaseElement *e = (GGuiBaseElement*)ggui->GetElements()[i];

		if (!e)
			continue;

		if (!e->GetParent() || e->GetParent() != this)
			continue;

		numchildren = numchildren + 1;

	}

	return numchildren;
}

int32_t GGuiBaseElement::GetPosX(bool absolute)
{
	if (absolute&&parent)
	{
		GGuiBaseElement *parentelement = (GGuiBaseElement*)parent;
		int realx = parentelement->GetPosX(true) + parentelement->GetPosPaddingX() + x;
		return realx;

	}
	return x;
}

int32_t GGuiBaseElement::GetPosY(bool absolute)
{

	if (absolute&&parent)
	{
		GGuiBaseElement *parentelement = (GGuiBaseElement*)parent;
		int realy = parentelement->GetPosY(true) + parentelement->GetPosPaddingY() + y;
		return realy;

	}

	return y;
}

int32_t GGuiBaseElement::SetPosPadding(int32_t topadx, int32_t topady)
{
	x_padding = topadx;
	y_padding = topady;
	return 0;
}

int32_t GGuiBaseElement::GetPosPaddingX()
{
	return x_padding;
}

int32_t GGuiBaseElement::GetPosPaddingY()
{
	return y_padding;
}


int32_t GGuiBaseElement::GetWidth()
{
	return w;
}

int32_t GGuiBaseElement::GetHeight()
{
	return h;
}

int32_t GGuiBaseElement::GetFocus()
{
	return focus;
}

int32_t GGuiBaseElement::SetFocus(int32_t newfocus)
{
	focus = newfocus;
	return 0;
}

int32_t GGuiBaseElement::UnFocus()
{

	SetFocus(0);

	if (!GetParent())
	{
		return 0;
	}

	GGuiBaseElement*e = this;

	while (e->GetParent())
	{
		GGuiBaseElement*par = e->GetParent();

		if (!par)
			break;
		
		par->SetFocus(par->GetFocus() - 1);

		e = par;
	}

	return 0;
}

bool GGuiBaseElement::GetVisible()
{
	if (parent && !parent->GetVisible())
		return false;

	return visible;
}

bool GGuiBaseElement::SetVisible(bool newvisible)
{
	visible = newvisible;
	

	if (focus)
	{
		void **elements = ggui->GetElements();

		GGuiBaseElement*topfocus = (GGuiBaseElement*)ggui->GetElementTopFocused();
		OnFocusChange(0, topfocus);
	}

	return 0;
}

bool GGuiBaseElement::SetDraggable(bool todraggable)
{
	draggable = todraggable;

	return 0;
}

bool GGuiBaseElement::GetDraggable(bool checkpos, int32_t mousex, int32_t mousey)
{
	return draggable;
}



bool GGuiBaseElement::SetDragged(bool todragged)
{
	beingdragged = todragged;

	return 0;
}

bool GGuiBaseElement::GetDragged()
{
	return beingdragged;
}

bool GGuiBaseElement::SetMouseOver(bool newmouseover, int32_t mousex, int32_t mousey)
{
	mouseover = newmouseover;

	if (!newmouseover)
	{
		mouseoverx = mouseovery = 0;
	}
	else {

		mouseoverx = mousex;
		mouseovery = mousey;
	}

	return 0;
}

void GGuiBaseElement::SetRenderedManually(bool torendermanually)
{
	renderedmanually = torendermanually;
}

bool GGuiBaseElement::GetRenderedManually()
{
	return renderedmanually;
}

bool GGuiBaseElement::GetMouseOver()
{
	return mouseover;
}

int32_t GGuiBaseElement::GetMouseOverX()
{
	return mouseoverx;
}

int32_t GGuiBaseElement::GetMouseOverY()
{
	return mouseovery;
}

int32_t GGuiBaseElement::OnRender(void *curgui, void* currender, void *curdraw)
{

	GGui *gui = (GGui*)curgui;
	Render *render = (Render*)currender;
	Easydraw *draw = (Easydraw*)curdraw;

	//draw->SetColor(255, 255, 255, 70);
	//draw->Rectangle(GetPosX(true), GetPosY(true), w, h);


	return 0;
}

int32_t GGuiBaseElement::Think()
{
	return 0;
}

int32_t GGuiBaseElement::PostThink()
{
	return 0;
}

int32_t GGuiBaseElement::OnFocusChange(int32_t newfocus, GGuiBaseElement* topfocus)
{

	return 0;
}

int32_t GGuiBaseElement::OnMouseMove(int32_t mousex, int32_t mousey)
{
	return 0;
}

int32_t GGuiBaseElement::OnMouseHover(int32_t mousex, int32_t mousey)
{
	return 0;
}

int32_t GGuiBaseElement::OnMousePressed(int32_t mousex, int32_t mousey, bool leftclick, bool initialpress)
{
	return 0;
}

int32_t GGuiBaseElement::OnMouseReleased(int32_t mousex, int32_t mousey, bool leftclick)
{
	return 0;
}

int32_t GGuiBaseElement::OnKeyPressed(int32_t key, bool initialpress)
{
	return 0;
}

int32_t GGuiBaseElement::OnKeyReleased(int32_t key)
{
	return 0;
}

int32_t GGuiBaseElement::OnChar(int32_t curchar)
{
	return 0;
}

int32_t GGuiBaseElement::OnDragged(int32_t mousex, int32_t mousey)
{
	return 0;
}