#define Z_FOR2D 0.0f

#include "easydraw.h"
#include <d3dx9.h>
#include <DxErr.h>

#include <stdio.h>


Easymaterial::Easymaterial()
{
	texture = 0;
	drawinst = 0;
	memset(name, 0, sizeof(name)/2);

}


Easymaterial::Easymaterial(Easydraw* draw, const wchar_t* filename, const wchar_t *customname)
{

	width = 0;
	height = 0;
	texture = 0;
	drawinst = 0;
	memset(name, 0, sizeof(name)/2);

	if (!draw || !filename)
		return;

	IDirect3DTexture9 *loadedtex = 0;

	HRESULT hr = 0;
	
	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)draw->GetDevice();

	D3DCOLOR colorkey = 0;

	D3DXIMAGE_INFO imginfo;

	hr = D3DXCreateTextureFromFileExW(device, filename, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, colorkey, &imginfo, 0, &loadedtex);

	if (FAILED(hr) || !loadedtex)
	{
		const wchar_t *errorString = DXGetErrorStringW(hr); // Here you get error string
		const wchar_t *errorDesc = DXGetErrorDescriptionW(hr); // Here you get error description

		printf("[ERROR!] Show this a dev!\nFailed loading texture: %ws - %ws\n", errorString, errorDesc);

		return;
	}

	D3DSURFACE_DESC desc;
	loadedtex->GetLevelDesc(0, &desc);
	memheight = desc.Height;
	memwidth = desc.Width;
	height = imginfo.Height;
	width = imginfo.Width;

	drawinst = draw;

	if (customname)
	{
		wcsncpy(name, customname, sizeof(name) / 2);
	}
	else {
		wcsncpy(name, filename, sizeof(name) / 2);
	}

	printf("NAME: %ws\n", name);
	texture = loadedtex;

}

Easymaterial::Easymaterial(Easydraw *draw, uint32_t len, const void* mem, const wchar_t *customname)
{
	width = 0;
	height = 0;
	texture = 0;
	drawinst = 0;
	memset(name, 0, sizeof(name) / 2);

	if (!draw || !mem || !len)
		return;

	IDirect3DTexture9 *loadedtex = 0;


	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)draw->GetDevice();

	D3DCOLOR colorkey = 0;

	D3DXIMAGE_INFO imginfo;


	HRESULT hr = D3DXCreateTextureFromFileInMemoryEx(device, mem, len, 0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, colorkey, &imginfo, 0, &loadedtex);

	if (FAILED(hr) || !loadedtex)
	{
		const wchar_t *errorString = DXGetErrorStringW(hr); // Here you get error string
		const wchar_t *errorDesc = DXGetErrorDescriptionW(hr); // Here you get error description

		printf("[ERROR!] Show this a dev!\nFailed loading texture: %ws - %ws\n", errorString, errorDesc);

		return;
	}

	D3DSURFACE_DESC desc;
	loadedtex->GetLevelDesc(0, &desc);
	memheight = desc.Height;
	memwidth = desc.Width;
	height = imginfo.Height;
	width = imginfo.Width;

	drawinst = draw;
	wcsncpy(name, customname, sizeof(name) / 2);
	texture = loadedtex;
}

void Easymaterial::operator = (Easymaterial* mat)
{
	texture = 0;
	memset(name, 0, sizeof(name)/2);
	drawinst = 0;

	if (!mat)
	{
		return;
	}

	if (!mat->IsValid())
	{
		return;
	}


	wcsncpy(name, mat->name, sizeof(name)/2);
	drawinst = mat->drawinst;
	texture = mat->texture;

}

Easyfont::Easyfont()
{
	drawinst = 0;
	fontsize = 0;
	weight = 0;
	memset(name, 0, sizeof(name)/2);
	memset(ourname, 0, sizeof(ourname)/2);
}

Easyfont::Easyfont(Easydraw *draw, int32_t fontnumerator, const wchar_t*newfontname, const wchar_t *fontname, int32_t tofontsize, int32_t fontweight)
{
	drawinst = 0;
	d3dfont = 0;
	weight = 0;
	fontsize = 0;
	memset(name, 0, sizeof(name)/2);
	memset(ourname, 0, sizeof(ourname)/2);

	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)draw->GetDevice();

	HRESULT hr = 0;

	ID3DXFont* loadfont;

	if (FAILED(hr = D3DXCreateFontW(device, MulDiv(tofontsize, fontnumerator, 72), 0, fontweight, 1, false, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH , fontname, &loadfont)) || !loadfont)
	{
		const wchar_t *errorString = DXGetErrorStringW(hr); // Here you get error string
		const wchar_t *errorDesc = DXGetErrorDescriptionW(hr); // Here you get error description

		printf("[ERROR!] Show this a dev!\nFailed loading font: %ws - %ws\n", errorString, errorDesc);

		return;
	}

	weight = fontweight;
	fontsize = tofontsize;
	drawinst = draw;
	wcsncpy(name, fontname, sizeof(name)/2);
	wcsncpy(ourname, newfontname, sizeof(ourname)/2);

	d3dfont = loadfont;
}

void Easyfont::operator = (Easyfont* font)
{
	drawinst = 0;
	d3dfont = 0;
	fontsize = 0;
	weight = 0;
	memset(name, 0, sizeof(name)/2);
	memset(ourname, 0, sizeof(ourname)/2);

	if (!font)
	{
		return;
	}

	if (!font->IsValid())
	{
		return;
	}

	drawinst = font->drawinst;
	fontsize = font->fontsize;
	weight = font->weight;

	wcsncpy(name, font->name, sizeof(name)/2);
	wcsncpy(ourname, font->ourname, sizeof(ourname)/2);

	d3dfont = font->d3dfont;

}

class VertexList
{
public:
	FLOAT x, y, z, rhw;
	DWORD color;

	FLOAT tu, tv;
};


Easydraw::Easydraw(void* device)
{
	matbuffer_loaded = 0;
	fontbuffer_loaded = 0;
	
	memset(&matbuffer, 0, sizeof(matbuffer));
	memset(&fontbuffer, 0, sizeof(fontbuffer));

	d3ddevice = device;
	curcol.SetFullWhite();
	logpixelsx = GetDeviceCaps(GetDC(GetDesktopWindow()), LOGPIXELSY);
	logpixelsy = GetDeviceCaps(GetDC(GetDesktopWindow()), LOGPIXELSY);
}

void Easydraw::Test()
{

	curcol.SetFullWhite();
	for (int32_t x = 0; x < 50; x++)
	{
		for (int32_t y = 0; y < 50; y++)
		{
			Pixel(x, y);
		}
	}

	curcol.SetFullRed();

	Line(150+20, 30, 250, 30);

	curcol.SetFullGreen();

	Rectangle(270, 30, 100, 100);


	curcol.SetFullBlue();

	OutlinedRectangle(400, 30, 100, 100, 10);

	Easymaterial *mat = GetMaterial(L"C:/testimg.png");
	
	if (mat)
	{

		printf("loaded mat\n");
		SetMaterial(mat);

		TexturedRectangle(550, 30, mat->GetMemWidth() / 4, mat->GetMemHeight() / 4);
	}


	Easyfont *font = GetFont(L"kkkfont", L"tahoma", 12, FONTWEIGHT_NORMAL);
	SetFont(font);

	curcol.r = 128;
	curcol.g = 200;
	curcol.b = 0;
	curcol.a = 255;

	Text(L"Monkeyaidsking", 560, 15, DT_NOCLIP);

	Circle(700, 73, 30, 16);


	Triangle(800 + 100, 100, 800 + 150, 50, 800 + 200, 100);

	static Easymaterial *generatedtxt = 0;
	if (!generatedtxt)
	{
		generatedtxt = CreateTexture(L"negtangle", 512, 512);
		easydraw_locked_rect *rect = new easydraw_locked_rect;

		GetTextureBuffer(generatedtxt->GetTexture(), true, rect);

		int32_t*buf = (int32_t*)rect->pBits;
		if (!rect->pBits)
		{
			MessageBox(NULL, L"NO PBITS WTF", L"k", MB_OK);
			return;
		}

		for (int32_t height = 0; height < 255; height++)
		{
			for (int32_t width = 0; width < 255; width++)
			{
				int32_t index = height * rect->Pitch / 4 + width;
				buf[index] = easydraw_rgba(255, 0, 0, 255); //make it red

			}
		}

		GetTextureBuffer(generatedtxt->GetTexture(), false, rect);
		delete rect;
		rect = 0;
	}
	else {
		SetMaterial(generatedtxt);
		TexturedRectangle(100, 100, 255, 255);
	}

}

Easymaterial* Easydraw::CreateTexture(wchar_t *txtname, int32_t w, int32_t h)
{
	LPDIRECT3DTEXTURE9 tex = 0;
	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)d3ddevice;
	D3DXCreateTexture
	(
		device,
		w,
		h,
		0,
		D3DUSAGE_DYNAMIC,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&tex
	);


	if (!tex)
	{
		MessageBox(NULL, txtname, L"couldnt generate texture!", MB_OK);
		return 0;
	}

	D3DSURFACE_DESC desc;
	tex->GetLevelDesc(0, &desc);

	Easymaterial *mat = new Easymaterial;
	mat->SetTexture(tex);
	mat->SetName(txtname);


	mat->SetMemHeight(desc.Height);
	mat->SetMemWidth(desc.Width);
	mat->SetHeight(h);
	mat->SetWidth(w);
	mat->SetDrawInst(this);


	return mat;

}

void Easydraw::GetTextureBuffer(void*vtex, bool locktex, easydraw_locked_rect*txtrect)
{
	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)d3ddevice;
	LPDIRECT3DTEXTURE9 tex = (LPDIRECT3DTEXTURE9)vtex;
	

	if (!tex)
		return ;

	if (!locktex)
	{
		tex->UnlockRect(0);
		return ;
	}

	D3DLOCKED_RECT *rekt = new D3DLOCKED_RECT;

	HRESULT hr = tex->LockRect(0, rekt, 0, D3DLOCK_DISCARD);


	if (FAILED(hr))
	{
		const wchar_t *errorString = DXGetErrorStringW(hr); // Here you get error string
		const wchar_t *errorDesc = DXGetErrorDescriptionW(hr); // Here you get error description

		printf("[ERROR!] Show this a dev!\nFailed locking texture: %ws - %ws\n", errorString, errorDesc);

		return;
	}

	txtrect->pBits = rekt->pBits;
	txtrect->Pitch = rekt->Pitch;

}

bool Easydraw::GetBeingClipped()
{
	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)d3ddevice;

	DWORD ret = 0;
	if (FAILED(device->GetRenderState(D3DRS_SCISSORTESTENABLE, &ret)))
		return false;

	return ret == TRUE;
}

void Easydraw::SetBeingClipped(bool toclipped)
{
	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)d3ddevice;

	if (toclipped)
	{
		device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
	}
	else {
		device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
	}
}

void Easydraw::GetClipped(int32_t& top, int32_t& bottom, int32_t& left, int32_t& right)
{
	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)d3ddevice;

	RECT clipr = { 0 };
	device->GetScissorRect(&clipr);

	top = clipr.top;
	bottom = clipr.bottom;
	left = clipr.left;
	right = clipr.right;

}

void Easydraw::SetClipped(int32_t top, int32_t bottom, int32_t left, int32_t right)
{

	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)d3ddevice;

	RECT clipr = { 0 };
	clipr.top = top;
	clipr.bottom = bottom;
	clipr.left = left;
	clipr.right = right;

	if (top || bottom || left || right)
	{

		device->SetScissorRect(&clipr);
	}
	else {

		device->SetScissorRect(&clipr);
	}
}

void Easydraw::Pixel(int32_t x, int32_t y)
{
	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)d3ddevice;

	D3DCOLOR d3dcol = (D3DCOLOR)curcol.GetUint32ARGB();


	VertexList vertexes[] =
	{
		{ (float)x, (float)y, Z_FOR2D, 1.0f, d3dcol }, //pos
	};

	device->SetTexture(0, 0);
	device->DrawPrimitiveUP(D3DPT_POINTLIST, 1, vertexes, sizeof(VertexList));

}
void Easydraw::Line(int32_t startx, int32_t starty, int32_t endx, int32_t endy)
{
	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)d3ddevice;

	D3DCOLOR d3dcol = (D3DCOLOR)curcol.GetUint32ARGB();

	VertexList vertexes[] =
	{

		{ (float)startx, (float)(starty), Z_FOR2D, 1.0f, d3dcol }, // start pos
		{ (float)endx, (float)(endy), Z_FOR2D, 1.0f, d3dcol }, // end pos
	};

	device->SetTexture(0, 0);
	device->DrawPrimitiveUP(D3DPT_LINELIST, 1, vertexes, sizeof(VertexList));

}

void Easydraw::Rectangle(int32_t x, int32_t y, int32_t width, int32_t height)
{
	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)d3ddevice;

	D3DCOLOR d3dcol = (D3DCOLOR)curcol.GetUint32ARGB();

	float startx = (float)x;
	float starty = (float)y;

	float endx = (float)(startx + width);
	float endy = (float)(starty + height);

	VertexList vertexes[] =
	{
		


		{ startx, starty, Z_FOR2D, 1.0f, d3dcol }, // top left
		{ endx, starty, Z_FOR2D, 1.0f, d3dcol }, // top right
		{ startx, endy,  Z_FOR2D, 1.0f, d3dcol }, // bottom left
		{ endx,  endy, Z_FOR2D, 1.0f, d3dcol }, // bottom right

	};

	device->SetTexture(0, 0);
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertexes, sizeof(VertexList));
}

void Easydraw::OutlinedRectangle(int32_t x, int32_t y, int32_t width, int32_t height, int32_t bordersize)
{
	
	
	Rectangle(x, y, width, bordersize);// top line
	Rectangle(x, y+height-bordersize, width, bordersize);// bottom line
	Rectangle(x, y, bordersize, height);// left line
	Rectangle(x+width-bordersize, y, bordersize, height);// right line



}

void Easydraw::TexturedRectangle(int32_t x, int32_t y, int32_t width, int32_t height)
{
	if (!curmat.IsValid())
	{
		return;
	}
	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)d3ddevice;

	D3DCOLOR d3dcol = (D3DCOLOR)curcol.GetUint32ARGB();

	float startx = (float)x;
	float starty = (float)y;

	float endx = (float)(startx + width);
	float endy = (float)(starty + height);

	VertexList vertexes[] =
	{



		{ startx, starty, Z_FOR2D, 1.0f, d3dcol, 0.0f, 0.0f}, // top left
		{ endx, starty, Z_FOR2D, 1.0f, d3dcol, 1.0f, 0.0f }, // top right
		{ startx, endy, Z_FOR2D, 1.0f, d3dcol, 0.0f, 1.0f }, // bottom left
		{ endx,  endy, Z_FOR2D, 1.0f, d3dcol ,1.0f, 1.0f }, // bottom right

	};

	device->SetTexture(0, (LPDIRECT3DTEXTURE9)curmat.GetTexture());
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertexes, sizeof(VertexList));
}

void Easydraw::GradientRectangle(int32_t x, int32_t y, int32_t width, int32_t height, Easycolor a, Easycolor b, Easycolor c, Easycolor d)
{
	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)d3ddevice;

	float startx = (float)x;
	float starty = (float)y;

	float endx = (float)(startx + width);
	float endy = (float)(starty + height);

	VertexList vertexes[] =
	{



		{ startx, starty, Z_FOR2D, 1.0f, (D3DCOLOR)a.GetUint32ARGB(), 0.0f, 0.0f }, // top left
		{ endx, starty, Z_FOR2D, 1.0f, (D3DCOLOR)b.GetUint32ARGB(), 1.0f, 0.0f }, // top right
		{ startx, endy, Z_FOR2D, 1.0f, (D3DCOLOR)c.GetUint32ARGB(), 0.0f, 1.0f }, // bottom left
		{ endx,  endy, Z_FOR2D, 1.0f, (D3DCOLOR)d.GetUint32ARGB() ,1.0f, 1.0f }, // bottom right

	};

	device->SetTexture(0, 0);
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertexes, sizeof(VertexList));
}
void Easydraw::Circle(int32_t x, int32_t y, int32_t radius, int32_t edgecount, int32_t rotation)
{

	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)d3ddevice;
	D3DCOLOR d3dcol = (D3DCOLOR)curcol.GetUint32ARGB();

	int32_t vertexnum = edgecount;


	if (!vertexnum)
	{
		vertexnum = 30;
	}


	VertexList *verts = new VertexList[vertexnum+3]; // don't ask me why +3, +3 fixes some weird bug
	memset(verts, 0, vertexnum+3);

	for (int32_t i = 0; i < vertexnum+3; i++)
	{
		//x,y, z, rhw, col, textureshita, textureshitb

		float coefficient = i / (vertexnum / 2.0f);

		coefficient *= D3DX_PI;

		float vertx = (float)((float)x + (float)radius*cos(rotation+coefficient));
		float verty = (float)((float)y + (float)radius*sin(rotation+coefficient));

		verts[i] = { vertx, verty, Z_FOR2D, 1.0f, d3dcol, 0.0f, 0.0f };
	}


	DWORD oldaaenabled = 0;

	device->GetRenderState(D3DRS_MULTISAMPLEANTIALIAS, &oldaaenabled);

	device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);

	device->SetTexture(0, 0);
	device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, vertexnum, verts, sizeof(VertexList));

	device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, oldaaenabled);

	delete[] verts;
}

void Easydraw::OutlinedCircle(int32_t x, int32_t y, int32_t radius, int32_t edgecount, int32_t rotation)
{

	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)d3ddevice;
	D3DCOLOR d3dcol = (D3DCOLOR)curcol.GetUint32ARGB();

	int32_t vertexnum = edgecount;


	if (!vertexnum)
	{
		vertexnum = 30;
	}


	VertexList *verts = new VertexList[vertexnum + 3]; // don't ask me why +3, +3 fixes some weird bug
	memset(verts, 0, vertexnum + 3);

	for (int32_t i = 0; i < vertexnum + 3; i++)
	{
		//x,y, z, rhw, col, textureshita, textureshitb

		float coefficient = i / (vertexnum / 2.0f);

		coefficient *= D3DX_PI;

		float vertx = (float)((float)x + (float)radius*cos(rotation + coefficient));
		float verty = (float)((float)y + (float)radius*sin(rotation + coefficient));
		
		verts[i] = { vertx, verty, 0.0f, 1.0f, d3dcol, 0.0f, 0.0f };
	}


	DWORD oldaaenabled = 0;

	device->GetRenderState(D3DRS_MULTISAMPLEANTIALIAS, &oldaaenabled);

	device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);

	device->SetTexture(0, 0);
	device->DrawPrimitiveUP(D3DPT_LINESTRIP, vertexnum, verts, sizeof(VertexList));

	device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, oldaaenabled);

	delete[] verts;
}

void Easydraw::Triangle(int32_t leftx, int32_t lefty, int32_t topx, int32_t topy, int32_t rightx, int32_t righty)
{
	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)d3ddevice;

	D3DCOLOR d3dcol = (D3DCOLOR)curcol.GetUint32ARGB();

	
	VertexList vertexes[] =
	{

		{ (float)topx, (float)topy, Z_FOR2D, 1.0f, d3dcol }, // top

		{ (float)rightx, (float)righty,  Z_FOR2D, 1.0f, d3dcol }, // bottom right
		{ (float)leftx, (float)lefty, Z_FOR2D, 1.0f, d3dcol }, // bottom left
	};

	DWORD oldaaenabled = 0;

	device->GetRenderState(D3DRS_MULTISAMPLEANTIALIAS, &oldaaenabled);

	device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);


	device->SetTexture(0, 0);
	device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, vertexes, sizeof(VertexList));

	device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, oldaaenabled);

}

void Easydraw::Text(const wchar_t *text, int32_t x, int32_t y, int32_t format, easydraw_fontrect* rect)
{
	if (!curfont.IsValid())
	{
		return;
	}

	ID3DXFont *font = (ID3DXFont*)curfont.GetFont();
	D3DCOLOR d3dcol = (D3DCOLOR)curcol.GetUint32ARGB();

	RECT FontRect = { x,  y,  x, y };

	if (rect)
	{
		FontRect.left = rect->left;
		FontRect.right = rect->right;
		FontRect.top = rect->top;
		FontRect.bottom = rect->bottom;
	}

	font->DrawText(0, text, -1, &FontRect, format, d3dcol);

}

void Easydraw::SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	curcol.r = r;
	curcol.g = g;
	curcol.b = b;
	curcol.a = a;
}

void Easydraw::SetColor(Easycolor color)
{
	curcol = color;
}

void Easydraw::SetMaterial(Easymaterial* mat)
{
	curmat = mat;
}

void Easydraw::SetFont(Easyfont *font)
{
	curfont = font;
}

int32_t Easydraw::GetRawTextSize(const wchar_t*txt, int32_t&w, int32_t& h, int32_t extraflags)
{
	if (!curfont.IsValid())
	{
		w = 0;
		h = 0;
		return 0;
	}



	ID3DXFont *font = (ID3DXFont*)curfont.GetFont();
	RECT rec = { 0,0,0,0 };

	font->DrawText(NULL, txt, -1, &rec, DT_CALCRECT  | extraflags, D3DCOLOR_RGBA(0, 0, 0, 0));

	w = rec.right - rec.left;
	h = rec.bottom - rec.top;

	return 1;
}

int32_t Easydraw::GetTextSize(const wchar_t*txt, int32_t&w, int32_t& h, int32_t extraflags)
{
	GetRawTextSize(txt, w, h, extraflags);

	//w * logpixelsx / 72 = ms size
	//ms size * 72 / logpixelsx = w

	w = MulDiv(w, 72, logpixelsx);

	//h * logpixelsy / 72 = ms size
	//ms size * 72 / logpixelsy = h

	h  = MulDiv(h, 72, logpixelsy);

	return 1;
}


Easymaterial* Easydraw::GetMaterial(const wchar_t *filename)
{
	for (uint16_t i = 0; i < matbuffer_loaded;i++) // caching TM
	{
		Easymaterial* curmat = matbuffer[i];

		if (!curmat->IsValid())
			continue;

		if (!wcscmp(curmat->GetName(), filename))
		{
			return curmat;
		}

	}

	Easymaterial *mat = new Easymaterial(this, filename);

	if (!mat)
	{
		return 0;
	}

	if (!mat->IsValid())
	{
		delete mat;
		return 0;
	}

	matbuffer[matbuffer_loaded] = mat;
	matbuffer_loaded++;

	return mat;
}

Easyfont* Easydraw::GetFont(const wchar_t* ourname, const wchar_t *fontname, int32_t fontsize, int32_t fontweight)
{

	if (fontbuffer_loaded)
	{
		for (uint16_t i = 0; i < fontbuffer_loaded; i++) // caching TM
		{
			Easyfont* curfont = fontbuffer[i];

			if (!curfont->IsValid())
				continue;

			if (!wcscmp(curfont->GetName(), ourname))
			{
				return curfont;
			}

		}
	}


	Easyfont *font = new Easyfont(this, logpixelsx, ourname, fontname, fontsize, fontweight);

	if (!font)
	{
		return 0;
	}

	if (!font->IsValid())
	{
		delete font;
		return 0;
	}

	fontbuffer[fontbuffer_loaded] = font;
	fontbuffer_loaded++;

	return font;
}