#pragma once


#include <cstdint>


#define CUSTOM_FVF D3DFVF_XYZRHW | D3DFVF_DIFFUSE  | D3DFVF_TEX1

class Render
{
private:

	bool testmode;

	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
	uint16_t fwidth;
	uint16_t fheight;
	int64_t fps;
	uint8_t fps_limit;
	uint32_t fps_frames;

	int64_t fps_timelast;
	
	void*callback;
	void*window;
	void*targetwindow;

	void* renderthread;

	void* d3d_instance;
	void* d3d_device;

	void* easydraw;

	uint16_t renderid;


public:
	Render(void *targetwindow, uint8_t framespersecond = 60);

	~Render();

	void SetRenderCallback(void* rendercallback)
	{
		callback = rendercallback;
	};

	void *GetRenderCallback()
	{
		return callback;
	}

	void* GetRenderThread()
	{
		return renderthread;
	}

	void* GetWindow()
	{
		return window;
	}

	void* GetTargetWindow()
	{
		return targetwindow;
	}

	uint16_t GetWidth()
	{
		return width;
	}

	uint16_t GetHeight()
	{
		return height;
	}

	uint16_t GetFullscreenWidth()
	{
		return fwidth;
	}

	uint16_t GetFullscreenHeight()
	{
		return fheight;
	}

	uint32_t GetFPSLimit()
	{
		return fps_limit;
	}

	uint16_t GetX()
	{
		return x;
	}

	uint16_t GetY()
	{
		return y;
	}

	void SetFPSLimit(uint8_t limit)
	{
		fps_limit = limit;
	}

	int64_t GetFPS()
	{
		return fps;
	}

	void OnRenderInit();
	void* OnRender();
	void* OnWndProc(void *hwnd, uint32_t message, uintptr_t* wparam, int32_t *lparam);

	void *GetD3DDevice()
	{
		return d3d_device;
	}


	void *GetD3DInstance()
	{
		return d3d_instance;
	}


	void *GetDraw()
	{
		return easydraw;
	}

	uint16_t GetRenderID()
	{
		return renderid;
	}

	void EnableTestMode()
	{
		testmode = true;
	}

	void Show();
	void Hide();

};

extern Render *render;
