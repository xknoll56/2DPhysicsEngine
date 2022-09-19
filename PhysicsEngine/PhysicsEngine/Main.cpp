#include <iostream>
#include <Windows.h>
#include <d2d1.h>
#include "vec.h"
#include "Input.h"

typedef int int32;
typedef unsigned int uint32;
typedef float f32;
typedef long int64;
typedef unsigned long uint64;
typedef short int16;
typedef unsigned short uint16;

float SCREEN_WIDTH = 1024;
float SCREEN_HEIGHT = 576;
#define DEBUG_BUILD
float aspect;

HWND hwnd;


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	LRESULT result = 0;
	switch (msg)
	{
	case WM_KEYDOWN:
	{
		if (!keys[wparam])
			keysDown[wparam] = true;
		keys[wparam] = true;
		if (wparam == VK_ESCAPE)
			DestroyWindow(hwnd);
		break;
}
	case WM_KEYUP:
	{
		keys[wparam] = false;
		break;
	}
	case WM_LBUTTONDOWN:
	{
		if (!mouse[MOUSE_LEFT])
			mouseDown[MOUSE_LEFT] = true;
		mouse[MOUSE_LEFT] = true;
		break;
	}
	case WM_LBUTTONUP:
	{
		mouse[MOUSE_LEFT] = false;
		break;
	}
	case WM_RBUTTONDOWN:
	{
		if (!mouse[MOUSE_RIGHT])
			mouseDown[MOUSE_RIGHT] = true;
		mouse[MOUSE_RIGHT] = true;
		break;
	}
	case WM_RBUTTONUP:
	{
		mouse[MOUSE_RIGHT] = false;
		break;
	}
	case WM_MBUTTONDOWN:
	{
		if (!mouse[MOUSE_MIDDLE])
			mouseDown[MOUSE_MIDDLE] = true;
		mouse[MOUSE_MIDDLE] = true;
		break;
	}
	case WM_MBUTTONUP:
	{
		mouse[MOUSE_MIDDLE] = false;
		break;
	}
	case WM_MOUSELEAVE:
	{
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	case WM_SIZE:
	{
		//global_windowDidResize = true;
		break;
	}
	default:
		result = DefWindowProcW(hwnd, msg, wparam, lparam);
	}
	return result;
}


float camWidth = 5;
vec2 camPos;

void DrawBox(ID2D1HwndRenderTarget* pRT, ID2D1SolidColorBrush* pBrush, vec2 position, vec2 scale, float angle, RGBA color)
{
	pBrush->SetColor(D2D1::ColorF(color.r, color.g, color.b, color.a));
	float modifiedScale = 0.5f * (float)SCREEN_WIDTH / camWidth;

	pRT->SetTransform(D2D1::Matrix3x2F::Scale(scale.x, scale.y) *
		D2D1::Matrix3x2F::Rotation(angle) *
		D2D1::Matrix3x2F::Translation(0.5f * (FLOAT)SCREEN_WIDTH + (position.x-camPos.x)*modifiedScale, 0.5f * (FLOAT)SCREEN_HEIGHT - (position.y-camPos.y)*modifiedScale));

	pRT->FillRectangle(D2D1::RectF(-modifiedScale, -modifiedScale, modifiedScale, modifiedScale), pBrush);
}

void DrawBox(ID2D1HwndRenderTarget* pRT, ID2D1SolidColorBrush* pBrush, vec2 position, vec2 scale, float angle, bool fill, RGBA color)
{
	pBrush->SetColor(D2D1::ColorF(color.r, color.g, color.b, color.a));
	float modifiedScale = 0.5f * (float)SCREEN_WIDTH / camWidth;

	pRT->SetTransform(D2D1::Matrix3x2F::Scale(scale.x, scale.y) *
		D2D1::Matrix3x2F::Rotation(angle) *
		D2D1::Matrix3x2F::Translation(0.5f * (FLOAT)SCREEN_WIDTH + (position.x - camPos.x) * modifiedScale, 0.5f * (FLOAT)SCREEN_HEIGHT - (position.y - camPos.y) * modifiedScale));

	if(fill)
		pRT->FillRectangle(D2D1::RectF(-modifiedScale, -modifiedScale, modifiedScale, modifiedScale), pBrush);
	else
		pRT->DrawRectangle(D2D1::RectF(-modifiedScale, -modifiedScale, modifiedScale, modifiedScale), pBrush);
}

void DrawLine(ID2D1HwndRenderTarget* pRT, ID2D1SolidColorBrush* pBrush, vec2 position1, vec2 position2, float yScale, bool fill, RGBA color)
{
	vec2 dp = position2 - position1;
	float len = dp.mag() *0.5f;
	dp.normalize();

	vec2 mid = (position2 + position1) * 0.5f;
	float theta = atanf(-dp.y / dp.x)*57.29577f;
	DrawBox(pRT, pBrush, mid, {len, yScale}, theta, color);

}

void DrawCircle(ID2D1HwndRenderTarget* pRT, ID2D1SolidColorBrush* pBrush, vec2 pos, float radius, float angle, RGBA color)
{
	pBrush->SetColor(D2D1::ColorF(color.r, color.g, color.b, color.a));
	float modifiedScale = radius / camWidth;
	pRT->SetTransform(D2D1::Matrix3x2F::Scale(modifiedScale, modifiedScale) *
		D2D1::Matrix3x2F::Rotation(angle) *
		D2D1::Matrix3x2F::Translation(0.5f * (FLOAT)SCREEN_WIDTH + (pos.x-camPos.x) * (0.5f / camWidth) * SCREEN_WIDTH, 0.5f * (FLOAT)SCREEN_HEIGHT - (pos.y-camPos.y) * (0.5f/camWidth)*SCREEN_WIDTH));

	pRT->DrawEllipse(D2D1::Ellipse({ 0.0F, 0.0F }, SCREEN_WIDTH * 0.5F, SCREEN_WIDTH * 0.5F), pBrush, 5.0f);

}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nShowCmd*/)
{

#ifdef DEBUG_BUILD
	AllocConsole();
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
	std::cout << "Console Setup Complete.\n";
#endif

	// Open a window

	{
		WNDCLASSEXW winClass = {};
		winClass.cbSize = sizeof(WNDCLASSEXW);
		winClass.style = CS_HREDRAW | CS_VREDRAW;
		winClass.lpfnWndProc = &WndProc;
		winClass.hInstance = hInstance;
		winClass.hIcon = LoadIconW(0, IDI_APPLICATION);
		winClass.hCursor = LoadCursorW(0, IDC_ARROW);
		winClass.lpszClassName = L"MyWindowClass";
		winClass.hIconSm = LoadIconW(0, IDI_APPLICATION);

		if (!RegisterClassExW(&winClass)) {
			MessageBoxA(0, "RegisterClassEx failed", "Fatal Error", MB_OK);
			return GetLastError();
		}

		RECT initialRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
		AdjustWindowRectEx(&initialRect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_OVERLAPPEDWINDOW);
		LONG initialWidth = initialRect.right - initialRect.left;
		LONG initialHeight = initialRect.bottom - initialRect.top;

		hwnd = CreateWindowExW(WS_EX_OVERLAPPEDWINDOW,
			winClass.lpszClassName,
			L"Engine",
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			CW_USEDEFAULT, CW_USEDEFAULT,
			initialWidth,
			initialHeight,
			0, 0, hInstance, 0);

		if (!hwnd) {
			MessageBoxA(0, "CreateWindowEx failed", "Fatal Error", MB_OK);
			return GetLastError();
		}

		ID2D1Factory* pD2DFactory = NULL;
		HRESULT hr = D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			&pD2DFactory
		);

		// Obtain the size of the drawing area.
		RECT rc;
		GetClientRect(hwnd, &rc);

		// Create a Direct2D render target          
		ID2D1HwndRenderTarget* pRT = NULL;
		hr = pD2DFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(
				hwnd,
				D2D1::SizeU(
					rc.right - rc.left,
					rc.bottom - rc.top)
			),
			&pRT
		);

		ID2D1SolidColorBrush* pBrush = NULL;
		if (SUCCEEDED(hr))
		{

			pRT->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Green),
				&pBrush
			);
		}

		//clear the inputs
		memset(keys, false, INPUT_SIZE);
		memset(keysDown, false, INPUT_SIZE);
		memset(mouse, false, MOUSE_SIZE);
		memset(mouseDown, false, MOUSE_SIZE);

		// Timing
		LONGLONG startPerfCount = 0;
		LONGLONG perfCounterFrequency = 0;
		{
			LARGE_INTEGER perfCount;
			QueryPerformanceCounter(&perfCount);
			startPerfCount = perfCount.QuadPart;
			LARGE_INTEGER perfFreq;
			QueryPerformanceFrequency(&perfFreq);
			perfCounterFrequency = perfFreq.QuadPart;
		}
		double currentTimeInSeconds = 0.0;

		vec2 pos{ 1.0f, 1.0 };
		camPos = { 1,1 };

		bool running = true;
		while (running)
		{
			//calculate delta time
			float dt;
			{
				double previousTimeInSeconds = currentTimeInSeconds;
				LARGE_INTEGER perfCount;
				QueryPerformanceCounter(&perfCount);

				currentTimeInSeconds = (double)(perfCount.QuadPart - startPerfCount) / (double)perfCounterFrequency;
				dt = (float)(currentTimeInSeconds - previousTimeInSeconds);
				//if (dt > (1.f / 60.f))
				//	dt = (1.f / 60.f);
			}

			MSG msg = {};
			while (PeekMessageW(&msg, 0, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
					running = false;
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}

			//reset input downs
			memset(keysDown, false, INPUT_SIZE);
			memset(mouseDown, false, MOUSE_SIZE);

			pRT->BeginDraw();
			pRT->Clear(D2D1::ColorF(0, 0, 0, 1));

			float angle = 0.0f;
			float radius = 0.5f;

			if (keys[KEY_W])
				camPos += vec2(0, dt);
			if (keys[KEY_A])
				camPos += vec2(-dt, 0);
			if (keys[KEY_S])
				camPos += vec2(0, -dt);
			if (keys[KEY_D])
				camPos += vec2(dt, 0);

			DrawBox(pRT, pBrush, pos, { 0.5f,0.5f }, angle, RGBA{ 0.0f, 1.0f, 0.0f, 1.0f });

			pBrush->SetColor(D2D1::ColorF(0.0f, 0.0f, 1.0f, 1.0f));
			DrawBox(pRT, pBrush, { 0,0 }, { 0.5f,0.5f }, angle, RGBA{ 0.0f, 0.0f, 1.0f, 1.0f });

			pBrush->SetColor(D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f));
			DrawCircle(pRT, pBrush, { 0,0 }, radius, angle, RGBA(1.0f, 0.0f, 0.0f, 1.0f));

			DrawLine(pRT, pBrush, { 0,0 }, { cosf(currentTimeInSeconds), sinf(currentTimeInSeconds)}, 0.005f, true, {1,0,0,1});

			HRESULT hr = pRT->EndDraw();
			if (!SUCCEEDED(hr))
				std::cout << "Failed to draw" << std::endl;
		}
	}
}