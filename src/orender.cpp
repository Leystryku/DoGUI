#define Z_FOR2D 2.0f

#pragma comment (lib, "Dwmapi.lib")
#pragma comment (lib, "Winmm.lib")
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxerr.lib")
#pragma comment(lib, "dxguid.lib")

#include <dwmapi.h>
#include <d3dx9.h>
#include <DxErr.h>

#include <stdio.h>
#include <chrono>

#include "orender.h"
#include "easydraw.h"


int (WINAPIV * __vsnprintf)(char *, size_t, const char*, va_list) = _vsnprintf; // fix dxerr shit
int (WINAPIV * __vsnwprintf)(wchar_t *, size_t, const wchar_t*, va_list) = _vsnwprintf; // fix dxerr shit

Render *render = 0;



LRESULT CALLBACK Render__WindowProcRedirect(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	if (message == 0x0081)
	{
		SetWindowLongPtr((HWND)hwnd, GWLP_USERDATA, (LONG_PTR)((CREATESTRUCT*)lparam)->lpCreateParams);
		SetWindowPos((HWND)hwnd, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
	}


	Render* currender = (Render*)GetWindowLongPtr(hwnd, GWLP_USERDATA);


	if (currender)
	{
		return (LONG_PTR)currender->OnWndProc(hwnd, message, (uintptr_t*)wparam, (int32_t*)lparam);
	}


	return DefWindowProc(hwnd, message, wparam, lparam);
}

unsigned long WINAPI RenderThreadStub(Render *currender)
{
	if (!currender) return -1;

	currender->OnRenderInit();

	wchar_t killmutex[50] = { 0 };
	_snwprintf(killmutex, 50, L"sssskillrender_%d", currender->GetRenderID());

	while (1)
	{
		if (OpenMutex(0, 0, killmutex) || !currender->GetWidth() || !currender->GetRenderThread() || currender->OnRender())
		{
			printf("Closing render thread...\n");
			break;
		}


	}

	void *vdevice = currender->GetD3DDevice();
	void *vinstance = currender->GetD3DInstance();


	auto device = (LPDIRECT3DDEVICE9EX)vdevice;
	auto instance = (LPDIRECT3D9EX)vinstance;


	device->Release();
	instance->Release();

	return 0;
}


Render::~Render()
{
	fps_limit = 0;
	width = 0;
	height = 0;
	callback = 0;
	window = 0;
	renderthread = 0;
	targetwindow = 0;

	wchar_t killmutex[50] = { 0 };
	_snwprintf(killmutex, 50, L"sssskillrender_%d", renderid);

	CreateMutex(0, 0, killmutex);

	TerminateThread(renderthread, 0);

	if (easydraw)
	{
		delete easydraw;
		easydraw = 0;
	}

	void *vdevice = d3d_device;
	void *vinstance = d3d_instance;

	if (vdevice)
	{

		auto device = (LPDIRECT3DDEVICE9EX)vdevice;
		device->Release();
	}

	if (vinstance)
	{
		auto instance = (LPDIRECT3D9EX)vdevice;
		instance->Release();
	}



}

static uint16_t render_renderid = 0;

Render::Render(void *target, uint8_t framespersecond)
{
	printf("Render::Render\n");

	testmode = false;
	renderid = render_renderid;
	render_renderid = render_renderid + 1;

	easydraw = 0;
	d3d_device = 0;
	d3d_instance = 0;

	fps = 0;
	fps_timelast = 0;
	fps_frames = 0;
	fps_limit = framespersecond;

	callback = 0;
	targetwindow = target;

	RECT targetrect;
	if (!GetWindowRect((HWND)targetwindow, &targetrect))
	{
		MessageBox(0, L"ERROR, A-81", L"Tell this a developer!", MB_OK);
		return;
	}

	uint16_t hiswidth = (uint16_t)(targetrect.right - targetrect.left);
	uint16_t hisheight = (uint16_t)(targetrect.bottom - targetrect.top);

	if (hiswidth > 0xFFFF || hisheight > 0xFFFF || 0xFF > hiswidth || 0xFF > hisheight)
	{
		MessageBox(0, L"ERROR, A-2!\nGame appears to be in fullscreen mode!\nFix: Run it with -window -noborder!\n", L"Game is in fullscreen mode!\n", MB_OK);
		return;
	}

	fwidth = GetSystemMetrics(SM_CXSCREEN);
	fheight = GetSystemMetrics(SM_CYSCREEN);

	RECT targetrect_cl;
	GetClientRect((HWND)targetwindow, &targetrect_cl);

	int border_thickness = ((targetrect.right - targetrect.left) - (targetrect_cl.right - targetrect_cl.left));
	int titlebarheight = ((targetrect.bottom - targetrect.top) - (targetrect_cl.bottom - targetrect_cl.top));


	int curwidth = targetrect.right - targetrect.left;
	int curheight = targetrect.bottom - targetrect.top;

	curwidth -= border_thickness;
	curheight -= titlebarheight;

	width = (uint16_t)curwidth;
	height = (uint16_t)curheight;




	printf("Initializing Render Thread...\n");
	renderthread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)RenderThreadStub, this, 0, 0);

}

void *Render::OnWndProc(void *hwnd, uint32_t message, uintptr_t* wparam, int32_t *lparam)
{

	if (message == WM_ACTIVATE)
	{
		SetForegroundWindow((HWND)targetwindow);



	}


	if (message == WM_CREATE)
	{
		MARGINS margins = { -1 };

		if (!FAILED(DwmExtendFrameIntoClientArea((HWND)hwnd, &margins)))
		{
			Sleep(2);
		}

		DWM_BLURBEHIND bb = { 0 };

		// Specify blur-behind and blur region.
		bb.dwFlags = DWM_BB_ENABLE;
		bb.fEnable = true;
		bb.hRgnBlur = NULL;

		// Enable blur-behind.
		DwmEnableBlurBehindWindow((HWND)hwnd, &bb);
	};

	return (void*)DefWindowProc((HWND)hwnd, message, (WPARAM)wparam, (LPARAM)lparam);
}

void Render::OnRenderInit()
{

	printf("Initializing Render Window... [%hu:%hu]\n", width, height);

	HRESULT hr = 0;

	LPDIRECT3D9EX instance;
	hr = Direct3DCreate9Ex(D3D_SDK_VERSION, &instance);

	if (FAILED(hr) || !instance)
	{
		const wchar_t *errorString = DXGetErrorStringW(hr); // Here you get error string
		const wchar_t *errorDesc = DXGetErrorDescriptionW(hr); // Here you get error description

		printf("[ERROR!] Show this a dev!\nFailed HW Check: %ws - %ws\n", errorString, errorDesc);
		MessageBox(NULL, L"ERROR, A-3_1", L"Tell this a developer!", MB_OK);
		return;
	}

	D3DCAPS9 caps;
	ZeroMemory(&caps, sizeof(caps));

	hr = instance->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	if (FAILED(hr))
	{
		const wchar_t *errorString = DXGetErrorStringW(hr); // Here you get error string
		const wchar_t *errorDesc = DXGetErrorDescriptionW(hr); // Here you get error description

		printf("[ERROR!] Show this a dev!\nFailed HW Check: %ws - %ws\n", errorString, errorDesc);
		MessageBox(NULL, L"ERROR, A-3_2", L"Tell this a developer!", MB_OK);
		return;
	}

	d3d_instance = instance;

	SetForegroundWindow((HWND)targetwindow);

	HINSTANCE targetinstance = 0;//(HINSTANCE)GetWindowLong((HWND)targetwindow, GWL_HINSTANCE);
	targetinstance = GetModuleHandle(NULL);


	wchar_t classname[256] = { 0 };

	_snwprintf(classname, 256, L"a%d%d", rand() % 200, rand() % 200);

	WNDCLASSEX wc = { 0 };

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = Render__WindowProcRedirect;
	wc.hInstance = targetinstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszClassName = classname;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);


	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"ERROR, A-3", L"Tell this a developer!", MB_OK);
		return;
	}

	window = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW,
		classname,
		L"Sethhack Render",
		WS_POPUP | WS_VISIBLE | WS_SYSMENU,
		0,
		0,
		(int)width, (int)height,
		(HWND)targetwindow,
		NULL,
		targetinstance,
		this);

	if (!window)
	{
		MessageBox(NULL, L"ERROR, A-4", L"Tell this a developer!", MB_OK);
		return;
	}

	SetWindowLong((HWND)window, GWL_EXSTYLE, GetWindowLong((HWND)window, GWL_EXSTYLE) | 0x80000 | 0x20);
	SetLayeredWindowAttributes((HWND)window, RGB(255, 255, 255), 255, ULW_COLORKEY | LWA_COLORKEY | LWA_ALPHA);
	SetLayeredWindowAttributes((HWND)window, 1, 1, LWA_ALPHA);
	ShowWindow((HWND)window, SW_SHOW);
	SetWindowPos((HWND)window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);


	SetForegroundWindow((HWND)targetwindow);


	D3DPRESENT_PARAMETERS d3dpp;    // create a struct to hold various device information

	ZeroMemory(&d3dpp, sizeof(d3dpp));    // clear out the struct for use
	d3dpp.Windowed = TRUE;    // program windowed, not fullscreen
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = (HWND)window;    // set the window to be used by Direct3D
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;     // set the back buffer format to 32-bit

	d3dpp.BackBufferWidth = (unsigned int)width;    // set the width of the buffer
	d3dpp.BackBufferHeight = (unsigned int)height;    // set the height of the buffer

	DWORD qualitylevels = 0;
	if (instance->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_A8R8G8B8, TRUE, D3DMULTISAMPLE_NONMASKABLE, &qualitylevels) == D3D_OK)
	{
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONMASKABLE;
		d3dpp.MultiSampleQuality = qualitylevels - 1;
	}
	else
	{
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	}


	LPDIRECT3DDEVICE9EX device;

	HRESULT res = instance->CreateDeviceEx(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		(HWND)window,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		0,
		&device);

	if (FAILED(res))
	{
		DestroyWindow((HWND)window);
		MessageBox(NULL, L"ERROR, A-6", L"Tell this a developer!", MB_OK);
		return;
	}

	if (!device)
	{
		DestroyWindow((HWND)window);
		MessageBox(NULL, L"ERROR, A-7", L"Tell this a developer!", MB_OK);
	}

	device->SetRenderState(D3DRS_LIGHTING, FALSE);

	device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	d3d_device = device;

	easydraw = new Easydraw(d3d_device);

	SetLayeredWindowAttributes((HWND)window, 255, 255, LWA_ALPHA);
	//SetLayeredWindowAttributes((HWND)window, 50, 50, LWA_ALPHA);
}


void* Render::OnRender()
{
	MSG msg = { 0 };
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_QUIT)
	{
		return 0;
	}

	if (!fps_limit)
	{
		return 0;
	}

	RECT targetrect;
	if (!GetWindowRect((HWND)targetwindow, &targetrect))
	{
		return (void*)1;
	}
	else {

		RECT targetrect_cl;
		GetClientRect((HWND)targetwindow, &targetrect_cl);

		int border_thickness = ((targetrect.right - targetrect.left) - (targetrect_cl.right - targetrect_cl.left)) / 2;
		int titlebarheight = ((targetrect.bottom - targetrect.top) - (targetrect_cl.bottom - targetrect_cl.top));

		int curx = targetrect.left + border_thickness;
		int cury = targetrect.top + titlebarheight - border_thickness;

		int curwidth = targetrect.right - targetrect.left;
		int curheight = targetrect.bottom - targetrect.top;

		curwidth -= border_thickness * 2;
		curheight -= titlebarheight;


		if (curx != (uint16_t)x || cury != (uint16_t)y)
		{
			x = (uint16_t)curx;
			y = (uint16_t)cury;





			SetWindowPos((HWND)window, HWND_TOPMOST, curx, cury, curwidth, curheight, 0);

		}

		if (curwidth != (uint16_t)width || curheight != (uint16_t)height)
		{
			SetLayeredWindowAttributes((HWND)window, 0, 0, LWA_ALPHA);
			MessageBox(NULL, L"If you want to change the size:\nRestart the game and select size.\nAfter that, re-inject!\n", L"ERROR!", MB_OK);

			exit(-1);

			width = (uint16_t)curwidth;
			height = (uint16_t)curheight;

			//resizing shit
		}

	}


	int64_t curtime = std::chrono::high_resolution_clock().now().time_since_epoch() / std::chrono::milliseconds(1);



	auto framedelta = (curtime - fps_timelast);


	if (fps_limit == 255)
	{

		if (!fps_timelast || !framedelta)
		{
			fps = 1000;
		}
		else {

			fps = 1000 / framedelta;
		}
	}
	else {
		if (framedelta * fps_limit < 1000)
		{
			if (fps_limit != 255) // 255 = no limit
				return 0;
		}

		fps_frames++;

		fps = (int32_t)((int64_t)1000 / framedelta);


	}
	//printf("FPS: %lld\n", fps);


	fps_frames = 0;
	fps_timelast = curtime;





	void *vdevice = GetD3DDevice();
	LPDIRECT3DDEVICE9EX device = (LPDIRECT3DDEVICE9EX)vdevice;

	device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
	device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	device->Clear(0, 0, D3DCLEAR_TARGET , 0, Z_FOR2D, 0);
	device->BeginScene();

	device->SetFVF(CUSTOM_FVF);

	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_BLENDDIFFUSEALPHA);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	typedef  void(__cdecl *rendercallbackfn)(void*, void*);
	rendercallbackfn rendercallback = (rendercallbackfn)callback;


	if (GetForegroundWindow() == targetwindow)
	{

		Easydraw *easydraw = (Easydraw*)this->easydraw;

		if (testmode)
		{
			easydraw->Test();
		}
		else {
			if (callback)
			{
				rendercallback(this, this->easydraw);
			}
		}

	}

	device->EndScene();
	device->Present(0, 0, 0, 0);





	return 0;
}
