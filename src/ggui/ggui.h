#include <cstdint>
#include "../vk.h"


#define BUTTONSTATE_INVALID -1
#define BUTTONSTATE_RELEASED 0
#define BUTTONSTATE_JUSTPRESSED 1
#define BUTTONSTATE_PRESSED 2
#define BUTTONSTATE_JUSTRELEASED 3

extern bool hookproc_suppressorigwindowinput;

class GGui
{
private:
	void*ourrender;
	void*ourdraw;
	
	int32_t buttonstate[0xFFF];

	int32_t elements[0xFFFF];
	int32_t elements_todelete[0xFFFF];

	uint16_t elements_count;
	uint16_t elements_queueddeletion;

	void* topfocused;
	void* topmostdad;

	int32_t x;
	int32_t y;

	int32_t width;
	int32_t height;

	int32_t fwidth;
	int32_t fheight;

	int32_t mousex;
	int32_t mousey;

	int32_t mousexwhenpressed;
	int32_t mouseywhenpressed;

	int32_t mouseinitialxwhendragging;
	int32_t mouseinitialywhendragging;

	int32_t panelinitialxwhendragging;
	int32_t panelinitialywhendragging;

	int32_t mousexlastpress;
	int32_t mouseylastpress;

	bool suppressorigwindowinput;

public:

	GGui();

	int32_t AttachToRender(void*render);
	void OnRenderCallback(void* render, void* draw);

	int32_t UpdateFocuses();
	int32_t UpdateLostFocuses();
	int32_t Think();
	int32_t ThinkKeys();
	int32_t ThinkMouse();
	int32_t PostThinkMouse();
	int32_t ThinkDrag();

	int32_t ElementRegister(void*element);
	int32_t ElementUnregister(void *element, bool deleteothers=true, bool deleteus=false);
	int32_t ElementUnregisterNoChilds(void *element, bool shoulddelete=true);
	int32_t UnregisterChildren(void *element, bool shoulddelete=true);
	int32_t MarkForDeletion(void *velement);
	int32_t DoQueuedDeletion();

	int32_t GetButtonState(int32_t button)
	{
		if (button > sizeof(buttonstate))
		{
			return BUTTONSTATE_INVALID;
		}

		return buttonstate[button];
	}

	void** GetElements()
	{
		return (void**)elements;
	}

	void* GetElementTopFocused()
	{
		return topfocused;
	}

	uint16_t GetElementsCount()
	{
		return elements_count;
	}

	int32_t GetMouseX()
	{
		return mousex;
	}

	int32_t GetMouseY()
	{
		return mousey;
	}

	int32_t GetX()
	{
		return x;
	}

	int32_t GetY()
	{
		return y;
	}

	int32_t GetWidth()
	{
		return width;
	}

	int32_t GetHeight()
	{
		return height;
	}

	int32_t GetScreenWidth()
	{
		return fwidth;
	}

	int32_t GetScreenHeight()
	{
		return fheight;
	}

	bool InArea(int32_t checkx, int32_t checky, int32_t x, int32_t y, int32_t w, int32_t h)
	{
		if (checkx< x || checky < y || checkx > x + w || checky > y + h)
			return false;

		return true;
	}

	bool GetSuppressOrigWindowInput()
	{
		return suppressorigwindowinput;
	}

	int32_t SuppressOrigWindowInput(bool tosuppress)
	{
		suppressorigwindowinput = tosuppress;
		hookproc_suppressorigwindowinput = tosuppress;

		return	0;
	}
};

extern GGui *ggui;

