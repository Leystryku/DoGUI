#include <cstdint>
#include <limits.h>

#include "hextextbox.h"

#include "../../easydraw.h"
#include "../ggui.h"

GGuiHexTextBox::GGuiHexTextBox()
{
	usingfloat = false;
	ourintp = 0;
	updateexternal = false;

	ourtextp = new wchar_t[NUMBERBOX_MAXNUMLEN];
	memset(ourtextp, 0, NUMBERBOX_MAXNUMLEN*2);

	SetTextP(ourtextp, NUMBERBOX_MAXNUMLEN);
	SetEnterNewLine(false);

}

GGuiHexTextBox::GGuiHexTextBox(void*toparent) : GGuiHexTextBox()
{
	SetParent(toparent);

}

GGuiHexTextBox::~GGuiHexTextBox()
{
	if (ourtextp)
	{
		delete ourtextp;
		ourtextp = 0;
	}
}

bool GGuiHexTextBox::IsAllowedChar(int32_t charcode)
{


	if (iswalnum(charcode))
		charcode = toupper(charcode);





	if (!isdigit(charcode) && (charcode > 'F' || charcode < 'A') )
	{

		if (GetCaretIndex() == 1 && charcode == '\b')
			return false;

		if (charcode == '\b' && GetCaretIndex() == 2 && GetTextLength() == 2)
		{
			ourtextp[1] = '0';
			SetText(ourtextp);

			if (ourintp)
				*ourintp = 0;
		}
		if (charcode == '\b' || charcode == '\r' || charcode == 0x16 || charcode == 0x18 || charcode == 0x03)
			return true;


		return false;
	}

	if (GetCaretIndex() <1)
		return false;

	if (GetTextLength() >= 9)
	{
		if (GetCaretIndex() >= 9)
			return false;

		printf("okey: %d\n", GetCaretIndex());

		ourtextp[GetCaretIndex()] = charcode;
		SetText(ourtextp);

		swscanf(ourtextp, L"#%x", ourintp);
		OnTextChanged();

		SetCaretIndex(GetCaretIndex()+1 );

		return false;
	}

	return true;
}

int32_t GGuiHexTextBox::OnRender(void *curgui, void* currender, void *curdraw)
{

	Easydraw *draw = (Easydraw*)curdraw;
	
	int32_t oldtop, oldbottom, oldleft, oldright;
	oldtop = oldbottom = oldleft = oldright = 0;

	GGuiTextBox::OnRender(curgui, currender, curdraw);

	if (updateexternal)
	{
		int32_t cur = 0;
		swscanf(ourtextp, L"#%x", &cur);

		if (ourintp && *ourintp != cur)
		{
			UpdateNumberText();
		}
	}

	return 0;
}


int32_t GGuiHexTextBox::SetNumber(int32_t* num)
{
	SetTextP(ourtextp, 12);
	SetText(L"");
	ourintp = num;
	
	UpdateNumberText();
	SetEnterNewLine(false);

	return 0;
}

int32_t GGuiHexTextBox::OnTextChanged()
{

	int32_t ret = GGuiTextBox::OnTextChanged();
	
	if (ourintp)
	{
		printf("CURTXT: %ws __ %x\n", ourtextp, *ourintp);

		swscanf(ourtextp, L"#%x", ourintp);

		UpdateNumberText();
		printf("NUM: %x\n", *ourintp);
	}

	ret = GGuiTextBox::OnTextChanged();
	return ret;
}


int32_t GGuiHexTextBox::UpdateNumberText()
{
	if (!ourintp)
		return 1;

	wchar_t convertnum[15] = { 0 };
	_snwprintf(convertnum, sizeof(convertnum) / 2, L"#%x", *ourintp);
	for (size_t i = 0; i < 15; i++)
		convertnum[i] = towupper(convertnum[i]);

	SetText(convertnum);
	SetAutoSized(false);
	SetAutoSized(true);

	printf("%ws == %x\n", convertnum, *ourintp);



	return 0;
}

int32_t GGuiHexTextBox::SetSupportExternalChange(bool tosupport)
{
	updateexternal = tosupport;
	return 0;
}
