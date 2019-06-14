#pragma once
#include <cstdint>
#include "baseelement.h"
#include "label.h"


class GGuiTextBox : public  GGuiLabel
{

private:

	int32_t caretvis;
	int32_t carettick;
	int32_t buttontick;

	int32_t borderpadtop;
	int32_t borderpadbottom;
	int32_t borderpadleft;
	int32_t borderpadright;


	wchar_t* textp;
	size_t curtextsize;

	size_t  caretindex;
	int32_t r;
	int32_t g;
	int32_t b;
	int32_t a;


	int32_t caretx;
	int32_t carety;

	int32_t textwidthbeforecaret;
	int32_t textheightbeforecaret;

	int32_t desiredcaretx;
	int32_t desiredcarety;
	int32_t desiredcaretrecx;
	int32_t desiredcaretrecy;
	int32_t lastbrutecaretx;
	int32_t lastbrutecarety;

	size_t maxlength;

	bool enternewline;

	void *(*callbackfn)(void*element, int32_t type, void*arg);
public:

	virtual const wchar_t *GetType()
	{
		return L"Textbox";
	}

	virtual int32_t SetMaxLength(size_t tomaxl);
	virtual size_t GetMaxLength();

	virtual int32_t SetBorderPadTop(int32_t to);
	virtual int32_t GetBorderPadTop();

	virtual int32_t SetBorderPadBottom(int32_t to);
	virtual int32_t GetBorderPadBottom();

	virtual int32_t SetBorderPadLeft(int32_t to);
	virtual int32_t GetBorderPadLeft();

	virtual int32_t SetBorderPadRight(int32_t to);
	virtual int32_t GetBorderPadRight();

	virtual int32_t SetCaretIndex(size_t caretpos);
	virtual size_t GetCaretIndex();

	virtual int32_t SetCaretX(int32_t tocaretx);
	virtual int32_t GetCaretX();

	virtual int32_t SetCaretY(int32_t tocarety);
	virtual int32_t GetCaretY();

	virtual int32_t SetColor(int32_t r, int32_t g, int32_t b, int32_t a);
	virtual int32_t GetColor(int32_t &r, int32_t &g, int32_t &b, int32_t &a);

	virtual int32_t SetFontColor(int32_t r, int32_t g, int32_t b, int32_t a);
	virtual int32_t GetFontColor(int32_t &r, int32_t &g, int32_t &b, int32_t &a);

	virtual int32_t SetEnterNewLine(bool toenter);
	virtual bool GetEnterNewLine();

	virtual int32_t SetTextP(wchar_t*totextp, size_t tobuffersize);
	virtual wchar_t* GetTextP();



	virtual int32_t SetText(const wchar_t *newtext);
	virtual int32_t GetText(wchar_t*buf, int32_t bufsize);

	virtual int32_t OnRender(void *curgui, void* currender, void *curdraw);

	virtual int32_t OnTextChanged();

	virtual int32_t OnKeyPressed(int32_t thekey, bool initialpress);

	virtual int32_t OnMousePressed(int32_t mousex, int32_t mousey, bool leftclick, bool initialpress);


	virtual bool IsAllowedChar(int32_t curchar);
	virtual int32_t OnChar(int32_t curchar);

	virtual void SetCallbackFn(void*fn);
	virtual void* GetCallbackFn();

	GGuiTextBox();
	GGuiTextBox(void* toparent);
};