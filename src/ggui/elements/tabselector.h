#pragma once
#include <cstdint>
#include "baseelement.h"

#define GGuiTabSelector_YBorder 30
#define GGuiTabSelector_YPad 5


#define GGuiTabSize_Individual 0
#define GGuiTabSize_ToBiggest 1

class GGuiTab : public GGuiBaseElement
{
private:
	wchar_t ourtabname[0xFF];

public:
	virtual const wchar_t *GetType()
	{
		return L"Tab";
	}

	GGuiTab();
	GGuiTab(void* toparent);
	virtual int32_t SetTabName(const wchar_t *tabname);
	virtual const wchar_t * GetTabName();
};

class GGuiTabSelector : public  GGuiBaseElement
{
private:
	bool isdocked;
	int32_t ourtabsize;
	int32_t tabs_num;
	int32_t selectedtab;
	GGuiTab *tabs[0xFF];
	
public:
	virtual const wchar_t *GetType()
	{
		return L"Tabselector";
	}

	~GGuiTabSelector();
	GGuiTabSelector();
	GGuiTabSelector(void* toparent);

	virtual int32_t OnRender(void *curgui, void* currender, void *curdraw);
	virtual int32_t Dock();
	virtual GGuiTab* AddTab(const wchar_t *tabname);
	virtual int32_t SetTabSize(int32_t tabsize);
	virtual int32_t SetDocked(bool docked);
};

