#pragma once
#include <cstdint>

class GGuiBaseElement
{
private:

	GGuiBaseElement* parent;

	int32_t x;
	int32_t y;
	int32_t w;
	int32_t h;

	int32_t x_padding;
	int32_t y_padding;

	int32_t focus;

	int32_t mouseoverx;
	int32_t mouseovery;

	bool beingdragged;
	bool draggable;
	bool mouseover;
	bool visible;
	bool renderedmanually;
public:

	GGuiBaseElement();
	GGuiBaseElement(void*toparent);

	virtual int32_t Unregister(bool deletetus);

	virtual const wchar_t *GetType()
	{
		return 0;
	}

	virtual int32_t SetParent(void* toparent);
	virtual GGuiBaseElement* GetParent();
	virtual GGuiBaseElement* GetRoot();
	virtual GGuiBaseElement* GetVisibleRoot();
	virtual int GetParentNum(bool ignoreinvis = true);
	virtual int32_t GetChildren(int32_t buffersize, void **buffer, int32_t &numchildren);
	virtual int32_t GetChildrenNum();

	virtual int32_t SetPos(int32_t x, int32_t y);
	virtual int32_t GetPosX(bool absolute = false);
	virtual int32_t GetPosY(bool absolute = false);

	virtual int32_t SetPosPadding(int32_t topadx, int32_t topady);
	virtual int32_t GetPosPaddingX();
	virtual int32_t GetPosPaddingY();


	virtual int32_t Center();
	virtual int32_t SetSize(int32_t width, int32_t height);
	virtual int32_t GetWidth();
	virtual int32_t GetHeight();

	virtual int32_t GetFocus();
	virtual int32_t SetFocus(int32_t newfocus);
	virtual int32_t UnFocus();

	virtual bool GetVisible();
	virtual bool SetVisible(bool visible);

	virtual bool GetDragged();
	virtual bool SetDragged(bool todragged);

	virtual bool GetDraggable(bool checkpos=false, int32_t mousex=0, int32_t mousey=0);
	virtual bool SetDraggable(bool todraggable);


	virtual int32_t GetMouseOverX();
	virtual int32_t GetMouseOverY();
	virtual bool GetMouseOver();
	virtual bool SetMouseOver(bool newmouseover, int32_t mousex=0, int32_t mousey=0);

	virtual void SetRenderedManually(bool torendermanually);
	virtual bool GetRenderedManually();

	virtual int32_t OnRender(void *curgui, void* currender, void *curdraw);
	virtual int32_t Think();
	virtual int32_t PostThink();
	virtual int32_t OnFocusChange(int32_t newfocus, GGuiBaseElement* topfocus);
	virtual int32_t OnDragged(int32_t mousex, int32_t mousey);

	virtual int32_t OnMouseMove(int32_t mousex, int32_t mousey);
	virtual int32_t OnMouseHover( int32_t mousex, int32_t mousey);
	virtual int32_t OnMousePressed(int32_t mousex, int32_t mousey, bool leftclick, bool initialpress);
	virtual int32_t OnMouseReleased(int32_t mousex, int32_t mousey, bool leftclick);

	virtual int32_t OnKeyPressed(int32_t key, bool initialpress);
	virtual int32_t OnKeyReleased(int32_t key);

	virtual int32_t OnChar(int32_t curchar);

};