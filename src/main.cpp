#pragma once

#include <Windows.h>

#include <stdint.h>
#include <stdio.h>

#include "orender.h"
#include "ggui/ggui.h"
#include "ggui/includeelements.h"
#include "easydraw.h"

DWORD gamePID = 0;
HWND gameWindow = 0;

BOOL CALLBACK enumWindowsProc( __in  HWND hwnd, __in  LPARAM lParam )
{

	int length = GetWindowTextLength(hwnd);
	if (!length)
		return TRUE;

	wchar_t * buffer = new wchar_t[length + 1];
	memset(buffer, 0, (length + 1) * sizeof(wchar_t));

	GetWindowText(hwnd, buffer, length + 1);


	DWORD pid = 0;
	GetWindowThreadProcessId(hwnd, &pid);

	if (!gameWindow && pid == gamePID)
	{
		gameWindow = hwnd;
	}



	delete[] buffer;

	return TRUE;
}

int InitThread()
{

	AllocConsole();
	SetConsoleTitleA("DoGUI");

	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);

	CreateMutex(NULL, FALSE, L"sh_running");

	printf("Starting demo...\n");

	bool drawtest = false;

	if (GetAsyncKeyState(VK_F10))
	{
		drawtest = true;
		render = new Render(gameWindow, 144);



		MessageBox(NULL, L"Enabled Mode A0!\nMake sure to have some image at C:/testimg.png!\nPrefered IMG Res: 900x900\n", L"Notification", MB_OK);
		render->EnableTestMode();
		return 1;
	}


	ggui = new GGui;
	render = new Render(gameWindow, 255);

	ggui->AttachToRender(render);

	
	GGuiFrame *mainframe = new GGuiFrame();
	mainframe->SetSize(660, 660);
	mainframe->Center();
	mainframe->SetDraggable(true);
	mainframe->SetTitle(L"A window title worthy of Chuck Norris' approval ( supports сука ! ) ");

	
	GGuiLabel* label_buttons = new GGuiLabel(mainframe);

	label_buttons->SetSize(100, 30);
	label_buttons->SetPos(20, 20);
	label_buttons->SetFont(L"guifontbig", L"Segoe Ui Light", 20, 0);
	label_buttons->SetText(L"Buttons");

	GGuiLabel* label_buttons_desc = new GGuiLabel(mainframe);

	label_buttons_desc->SetSize(100, 30);
	label_buttons_desc->SetPos(20, 40);
	label_buttons_desc->SetText(L"The clickiest buttons you'll ever click");

	GGuiButton* butt = new GGuiButton(mainframe);

	butt->SetSize(100, 30);
	butt->SetPos(20, 70);
	butt->SetText(L"Nice Button");

	GGuiLabel* label_checkboxes = new GGuiLabel(mainframe);

	label_checkboxes->SetSize(100, 30);
	label_checkboxes->SetPos(20, 120);
	label_checkboxes->SetFont(L"guifontbig", L"Segoe Ui Light", 20, 0);
	label_checkboxes->SetText(L"Checkboxes");

	GGuiLabel* label_checkboxes_desc = new GGuiLabel(mainframe);

	label_checkboxes_desc->SetSize(100, 30);
	label_checkboxes_desc->SetPos(20, 140);
	label_checkboxes_desc->SetText(L"Once you try clicking these, you'll never want to click anything else");



	GGuiCheckBox* checkbox = new GGuiCheckBox(mainframe);

	checkbox->SetText(L"Click me baby");
	checkbox->SetPos(20, 170);
	checkbox->SetSize(20, 20);


	GGuiLabel* label_radiobuttons = new GGuiLabel(mainframe);

	label_radiobuttons->SetSize(100, 30);
	label_radiobuttons->SetPos(20, 220);
	label_radiobuttons->SetFont(L"guifontbig", L"Segoe Ui Light", 20, 0);
	label_radiobuttons->SetText(L"Radio buttons");

	GGuiLabel* label_radiobuttons_desc = new GGuiLabel(mainframe);

	label_radiobuttons_desc->SetSize(100, 30);
	label_radiobuttons_desc->SetPos(20, 240);
	label_radiobuttons_desc->SetText(L"Why the hell are these things even called \"Radio Buttons\"?!?!??");

	GGuiRadioButton* radiobutton1 = new GGuiRadioButton(mainframe);
	GGuiRadioButton* radiobutton2 = new GGuiRadioButton(mainframe);
	GGuiRadioButton* radiobutton3 = new GGuiRadioButton(mainframe);

	int32_t *selectedoption = new int32_t;
	*selectedoption = 1;

	radiobutton1->SetText(L"Option 1");
	radiobutton1->SetPos(20, 270);
	radiobutton1->SetSize(20, 20);
	radiobutton1->SetID(0);
	radiobutton1->SetSelectedP(selectedoption);

	radiobutton2->SetText(L"Option 2");
	radiobutton2->SetPos(120, 270);
	radiobutton2->SetSize(20, 20);
	radiobutton2->SetID(1);
	radiobutton2->SetSelectedP(selectedoption);

	radiobutton3->SetText(L"Option 3");
	radiobutton3->SetPos(220, 270);
	radiobutton3->SetSize(20, 20);
	radiobutton3->SetID(2);
	radiobutton3->SetSelectedP(selectedoption);

	GGuiLabel* label_textboxes = new GGuiLabel(mainframe);

	label_textboxes->SetSize(100, 30);
	label_textboxes->SetPos(20, 320);
	label_textboxes->SetFont(L"guifontbig", L"Segoe Ui Light", 20, 0);
	label_textboxes->SetText(L"Text input");

	GGuiLabel* label_textboxes_desc = new GGuiLabel(mainframe);

	label_textboxes_desc->SetSize(100, 30);
	label_textboxes_desc->SetPos(20, 340);
	label_textboxes_desc->SetText(L"I bet you can't guess what you're supposed to do with this!");

	GGuiTextBox* textbox = new GGuiTextBox(mainframe);

	wchar_t*textptr = new wchar_t[0xFF];
	memset(textptr, 0, 0xFF);

	textbox->SetTextP(textptr, 255);
	textbox->SetPos(20, 370);
	textbox->SetSize(200, 30);
	textbox->SetEnterNewLine(false);

	GGuiLabel* label_numberinput = new GGuiLabel(mainframe);

	label_numberinput->SetSize(100, 30);
	label_numberinput->SetPos(20, 420);
	label_numberinput->SetFont(L"guifontbig", L"Segoe Ui Light", 20, 0);
	label_numberinput->SetText(L"Number input");

	GGuiLabel* label_numberinput_desc = new GGuiLabel(mainframe);

	label_numberinput_desc->SetSize(100, 30);
	label_numberinput_desc->SetPos(20, 440);
	label_numberinput_desc->SetText(L"Just like a text input, but for numbers!");

	int32_t *mynum = new int32_t;
	*mynum = 1337;

	GGuiNumberBox* numberinput = new GGuiNumberBox(mainframe);

	numberinput->SetPos(20, 470);
	numberinput->SetSize(95, 30);
	numberinput->SetEnterNewLine(false);
	numberinput->SetNumber(mynum);
	numberinput->SetSupportExternalChange(true);
	*mynum = 1338;

	numberinput->SetMax(1340);
	numberinput->SetMin(1330);

	float *mynum2 = new float;
	*mynum2 = 13.37;

	GGuiNumberBox* numberinput2 = new GGuiNumberBox(mainframe);

	numberinput2->SetPos(95+10+20, 470);
	numberinput2->SetSize(95, 30);
	numberinput2->SetEnterNewLine(false);
	numberinput2->SetNumber(mynum2);

	GGuiLabel* label_dropdown = new GGuiLabel(mainframe);

	label_dropdown->SetSize(100, 30);
	label_dropdown->SetPos(20, 520);
	label_dropdown->SetFont(L"guifontbig", L"Segoe Ui Light", 20, 0);
	label_dropdown->SetText(L"Dropdown");

	GGuiLabel* label_dropdown_desc = new GGuiLabel(mainframe);

	label_dropdown_desc->SetSize(100, 30);
	label_dropdown_desc->SetPos(20, 540);
	label_dropdown_desc->SetText(L"One should be able to pick a pre-defined option");

	GGuiDropDown* dropdown = new GGuiDropDown(mainframe);

	dropdown->SetText(L"Some value");
	dropdown->SetPos(20, 570);
	dropdown->SetSize(130, 30);
	dropdown->AddOption(L"Eat an apple");
	dropdown->AddOption(L"Eat a boiled egg");
	dropdown->AddOption(L"Kill adolf hitler");
	dropdown->AddOption(L"Load the jews");

	GGuiFrame *testbigtextframe = new GGuiFrame();
	testbigtextframe->SetSize(220, 220);
	testbigtextframe->SetTitle(L"Textbox");
	testbigtextframe->SetDraggable(true);

	GGuiTextBox *textbox2 = new GGuiTextBox(testbigtextframe);

	wchar_t*textptr2 = new wchar_t[0xFF];
	memset(textptr2, 0, 0xFF);

	textbox2->SetTextP(textptr2, 0xFF);
	textbox2->SetPos(5, 10);
	textbox2->SetSize(testbigtextframe->GetWidth() - 10, testbigtextframe->GetHeight() - 10 - 40);
	textbox2->SetEnterNewLine(true);
	

	GGuiFrame *tabbedframe = new GGuiFrame();
	tabbedframe->SetPos(ggui->GetScreenWidth() - 350 - 200, ggui->GetScreenHeight() - 350 - 200);
	tabbedframe->SetSize(350, 350);
	tabbedframe->SetDraggable(true);
	tabbedframe->SetTitle(L"Tabs");

	GGuiTabSelector *tabselect = new GGuiTabSelector(tabbedframe);
	tabselect->Dock();


	GGuiTab *tab1 = tabselect->AddTab(L"Tab 1");

	GGuiScrollBar *horizontalscrollbar = new GGuiScrollBar(tab1);
	horizontalscrollbar->SetSize(260, 15);
	horizontalscrollbar->SetPos(5, 120);

	GGuiScrollBar *verticalscrollbar = new GGuiScrollBar(tab1);

	verticalscrollbar->SetSize(15, 260);
	verticalscrollbar->SetPos(tab1->GetWidth() - 15 -5, 10);

	GGuiSlider *horizontalslider = new GGuiSlider(tab1);
	horizontalslider->SetSize(260, 30);
	horizontalslider->SetPos(5, 80);
	horizontalslider->SetName(L"Slider name");


	GGuiButton *shiteeea = new GGuiButton(tab1);

	shiteeea->SetSize(140, 30);
	shiteeea->SetPos(5, 0);
	shiteeea->SetText(L"atest1");

	uint32_t *curcolorp = new uint32_t;
	*curcolorp = (uint32_t)-1;
	
	GGuiColorPicker *colorpick = new GGuiColorPicker(tab1);

	colorpick->SetPos(5, 40);
	colorpick->SetColorP(curcolorp);


	GGuiTab *tab2 = tabselect->AddTab(L"Tab 2");

	GGuiButton *shiteeeb = new GGuiButton(tab2);

	shiteeeb->SetSize(110, 30);
	shiteeeb->SetPos(5, 0);
	shiteeeb->SetText(L"btest2");

	GGuiTab *tabempty = tabselect->AddTab(L"Tab nondocked");

	GGuiTabSelector *tabselect2 = new GGuiTabSelector(tabempty);
	tabselect2->SetSize(tabselect2->GetParent()->GetWidth() - 2, tabselect2->GetParent()->GetHeight() - 2);

	tabselect2->AddTab(L"I");
	tabselect2->AddTab(L"Am");
	GGuiButton *yep = new GGuiButton(tabselect2->AddTab(L"Z"));

	yep->SetPos(5, 1);

	yep->SetSize(50, 25);
	yep->SetText(L"yep");

	printf("sad\n");


	return 0;
}

static bool didinject = false;

int OnInject()
{
	if (didinject || OpenMutex(0, false, L"sh_running"))
		return 1;

	didinject = true;

	gamePID = GetCurrentProcessId();

	printf("%x\n", gamePID);

	BOOL numeratedwindows = EnumWindows(enumWindowsProc, 0);


	if (!gameWindow||!numeratedwindows)
	{
		MessageBox(NULL, L"Error! [0]", L"Tell this a developer!", MB_OK);
		return 1;
	}

	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)InitThread, 0, 0, 0);
	Sleep(500);

	return 0;



}

void OnUnload()
{
	delete render;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{


	if (DLL_PROCESS_ATTACH)
	{

		if (OnInject())
			return FALSE;
		
		return TRUE;
	}

	if (DLL_PROCESS_DETACH)
	{
		OnUnload();
		return TRUE;
	}
	return TRUE;
}
