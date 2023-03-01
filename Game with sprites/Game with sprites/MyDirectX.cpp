#include "MyDirectX.h"
#include <iostream>
using namespace std;
//Direct3D variables
LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;
LPDIRECT3DSURFACE9 backbuffer = NULL;
//DirectInput variables
LPDIRECTINPUT8 dinput = NULL;
LPDIRECTINPUTDEVICE8 dimouse = NULL;
LPDIRECTINPUTDEVICE8 dikeyboard = NULL;
DIMOUSESTATE mouse_state;
char keys[256];
XINPUT_GAMEPAD controllers[4];
//sprite object
LPD3DXSPRITE spriteobj = NULL;


// Direct3D initialization
bool Direct3D_Init(HWND window, int width, int height, bool fullscreen)
{
	//initialize Direct3D
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d) return false;
	//set Direct3D presentation parameters
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = (!fullscreen);
	d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.hDeviceWindow = window;

	//create Direct3D device
	d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);
	if (!d3ddev) return false;

	//get a pointer to the back buffer surface
	d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);

	//create sprite object
	D3DXCreateSprite(d3ddev, &spriteobj);

	return true;
};
// Direct3D shutdown
void Direct3D_Shutdown()
{
	if (spriteobj) spriteobj->Release();
	if (d3ddev) d3ddev->Release();
	if (d3d) d3d->Release();
};
// Draws a surface to the screen using StretchRect
void DrawSurface(LPDIRECT3DSURFACE9 dest,
	float x, float y,
	LPDIRECT3DSURFACE9 source)
{
	//get width/height from source surface
	D3DSURFACE_DESC desc;
	source->GetDesc(&desc);
	//create rects for drawing
	RECT source_rect = { 0, 0,
		(long)desc.Width, (long)desc.Height };
	RECT dest_rect = { (long)x, (long)y,
		(long)x + desc.Width, (long)y + desc.Height };
	//draw the source surface onto the dest
	d3ddev->StretchRect(source, &source_rect, dest,
		&dest_rect, D3DTEXF_NONE);
};
// Loads a bitmap file into a surface
LPDIRECT3DSURFACE9 LoadSurface(string filename)
{
	LPDIRECT3DSURFACE9 image = NULL;
	//get width and height from bitmap file
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(filename.c_str(), &info);
	if (result != D3D_OK) return NULL;
	//create surface
	result = d3ddev->CreateOffscreenPlainSurface(
		info.Width, //width of the surface
		info.Height, //height of the surface
		D3DFMT_X8R8G8B8, //surface format
		D3DPOOL_DEFAULT, //memory pool to use
		&image, //pointer to the surface
		NULL); //reserved (always NULL)
	if (result != D3D_OK) return NULL;
	//load surface from file into newly created surface
	result = D3DXLoadSurfaceFromFile(
		image, //destination surface
		NULL, //destination palette
		NULL, //destination rectangle
		filename.c_str(), //source filename
		NULL, //source rectangle
		D3DX_DEFAULT, //controls how image is filtered
		D3DCOLOR_XRGB(0, 0, 0), //for transparency (0 for none)
		NULL); //source image info (usually NULL)
			   //make sure file was loaded okay
	if (result != D3D_OK) return NULL;
	return image;
};
// DirectInput initialization
bool DirectInput_Init(HWND hwnd)
{
	//initialize DirectInput object
	HRESULT result = DirectInput8Create(
		GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&dinput,
		NULL);
	//initialize the keyboard
	dinput->CreateDevice(GUID_SysKeyboard, &dikeyboard, NULL);
	dikeyboard->SetDataFormat(&c_dfDIKeyboard);
	dikeyboard->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	dikeyboard->Acquire();
	//initialize the mouse
	dinput->CreateDevice(GUID_SysMouse, &dimouse, NULL);
	dimouse->SetDataFormat(&c_dfDIMouse);
	dimouse->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	dimouse->Acquire();
	d3ddev->ShowCursor(false);
	return true;
};
// DirectInput update
void DirectInput_Update()
{

	//update mouse
	dimouse->GetDeviceState(sizeof(mouse_state), (LPVOID)&mouse_state);
	//update keyboard
	dikeyboard->GetDeviceState(sizeof(keys), (LPVOID)&keys);
	
	//update controllers
	for (int i = 0; i< 4; i++)
	{
		// THE CODE BELOW IS FUCKED UP FOR SOME REASON!!!
		//ZeroMemory(&controllers[i], sizeof(XINPUT_STATE));//IDK WHAT THIS IS DOING BUT IT IS RUINING EVERYTHING.
		//get the state of the controller
		XINPUT_STATE state;
		DWORD result = XInputGetState(i, &state);
		//store state in global controllers array
		if (result == 0) controllers[i] = state.Gamepad;
	};
};
// Return mouse x movement
int Mouse_X()
{
	return mouse_state.lX;
};
// Return mouse y movement
int Mouse_Y()
{
	return mouse_state.lY;
};
// Return mouse button state
int Mouse_Button(int button)
{
	return mouse_state.rgbButtons[button] & 0x80;
};
// Return key press state
int Key_Down(int key)
{
	return (keys[key] & 0x80);
};
// DirectInput shutdown
void DirectInput_Shutdown()
{
	if (dikeyboard)
	{
		dikeyboard->Unacquire();
		dikeyboard->Release();
		dikeyboard = NULL;
	}
	if (dimouse)
	{
		dimouse->Unacquire();
		dimouse->Release();
		dimouse = NULL;
	}
};
// Returns true if controller is plugged in
bool XInput_Controller_Found()
{
	XINPUT_CAPABILITIES caps;
	ZeroMemory(&caps, sizeof(XINPUT_CAPABILITIES));
	XInputGetCapabilities(0, XINPUT_FLAG_GAMEPAD, &caps);
	if (caps.Type != 0) return false;
	return true;
};
// Vibrates the controller
void XInput_Vibrate(int contNum, int amount)
{
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = amount;
	vibration.wRightMotorSpeed = amount;
	XInputSetState(contNum, &vibration);
};

//added after sprites
LPDIRECT3DTEXTURE9 LoadTexture(std::string filename, D3DCOLOR transcolor)
{
	LPDIRECT3DTEXTURE9 texture = NULL;
	//get width and height from bitmap file
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(filename.c_str(), &info);
	if (result != D3D_OK) return NULL;
	//create the new texture by loading a bitmap image file
	D3DXCreateTextureFromFileEx(
		d3ddev, //Direct3D device object
		filename.c_str(), //bitmap filename
		info.Width, //bitmap image width
		info.Height, //bitmap image height
		1, //mip-map levels (1 for no chain)
		D3DPOOL_DEFAULT, //the type of surface (standard)
		D3DFMT_UNKNOWN, //surface format (default)
		D3DPOOL_DEFAULT, //memory class for the texture
		D3DX_DEFAULT, //image filter
		D3DX_DEFAULT, //mip filter
		transcolor, //color key for transparency
		&info, //bitmap file info (from loaded file)
		NULL, //color palette
		&texture); //destination texture
				   //make sure the bitmap texture was loaded correctly
	if (result != D3D_OK) return NULL;
	return texture;
};
//also added after sprites
D3DXVECTOR2 GetBitmapSize(string filename)
{
	D3DXIMAGE_INFO info;
	D3DXVECTOR2 size = D3DXVECTOR2(0.0f, 0.0f);
	HRESULT result = D3DXGetImageInfoFromFile(filename.c_str(), &info);
	if (result == D3D_OK)
		size = D3DXVECTOR2((float)info.Width, (float)info.Height);
	else
		size = D3DXVECTOR2((float)info.Width, (float)info.Height);
	return size;
};

void Print(
	LPD3DXFONT font,
	int x,
	int y,
	string text,
	D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255))
{
	//figure out the text boundary
	RECT rect = { x, y, 0, 0 };
	font->DrawText(NULL, text.c_str(), text.length(), &rect, DT_CALCRECT, color);
	//print the text
	font->DrawText(spriteobj, text.c_str(), text.length(), &rect, DT_LEFT, color);
};

LPD3DXFONT MakeFont(string name, int size, int weight)
{
	LPD3DXFONT font = NULL;
	D3DXFONT_DESC desc = {
		size, //height
		0, //width
		weight, //weight
		0, //miplevels
		false, //italic
		DEFAULT_CHARSET, //charset
		OUT_TT_PRECIS, //output precision
		CLIP_DEFAULT_PRECIS, //quality
		DEFAULT_PITCH, //pitch and family
		"" //font name
	};
	strcpy(desc.FaceName, name.c_str());
	D3DXCreateFontIndirect(d3ddev, &desc, &font);
	return font;
};

void FontPrint(LPD3DXFONT font, int x, int y, string text, D3DCOLOR color)
{
	//figure out the text boundary
	RECT rect = { x, y, 0, 0 };
	font->DrawText(NULL, text.c_str(), text.length(), &rect, DT_CALCRECT, color);
	//print the text
	font->DrawText(spriteobj, text.c_str(), text.length(), &rect, DT_LEFT, color);
};

//stolen from the internet: https://stackoverflow.com/questions/20177781/how-to-get-text-width-directx-c
int GetTextWidth(const char *szText, LPD3DXFONT pFont)
{
	RECT rcRect = { 0,0,0,0 };
	if (pFont)
	{
		// calculate required rect
		pFont->DrawText(NULL, szText, strlen(szText), &rcRect, DT_CALCRECT,
			D3DCOLOR_XRGB(0, 0, 0));
	}

	// return width
	return rcRect.right - rcRect.left;
};

//made this one from get text width
int GetTextHeight(const char *szText, LPD3DXFONT pFont)
{
	RECT rcRect = { 0,0,0,0 };
	if (pFont)
	{
		// calculate required rect
		pFont->DrawText(NULL, szText, strlen(szText), &rcRect, DT_CALCRECT,
			D3DCOLOR_XRGB(0, 0, 0));
	}

	// return width
	return rcRect.bottom - rcRect.top;
};

POINT GetRectCenter(RECT rect)
{
	int x, y;
	x = (rect.left + rect.right) / 2;
	y = (rect.top + rect.bottom) / 2;

	return{ x, y };
};

int GetRectWidth(RECT rect)
{
	return rect.right - rect.left;
};

int GetRectHeight(RECT rect)
{
	return rect.bottom - rect.top;
};