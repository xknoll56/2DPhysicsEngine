#include <iostream>
#include <Windows.h>
#include <d2d1.h>
#include <random>

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
float elapsedTime;

HWND hwnd;

ID2D1HwndRenderTarget* pRT = NULL;
ID2D1SolidColorBrush* pBrush = NULL;


#include "vec.h"
#include "Time.h"
#include "RigidBody.h"
#include "Collider.h"
#include "SquareSpace.h"
#include "PhysicsWorld.h"
#include "Input.h"
#include "Renderer.h"
#include "Scene.h"
#include "TestScene.h"
#include "BallRollScene.h"




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

	

		Renderer renderer;
		TestScene scene(&renderer);
		BallRollScene brs(&renderer);
		scene.setup();
		brs.setup();


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
				elapsedTime = currentTimeInSeconds;
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

			pRT->BeginDraw();
			pRT->Clear(D2D1::ColorF(0, 0, 0, 1));

			brs.update(dt);
			//renderer.DrawCapsule({ 0,0 }, 1.0f, 0.5f, currentTimeInSeconds, { 1,1,1,1 });

			HRESULT hr = pRT->EndDraw();
			if (!SUCCEEDED(hr))
				std::cout << "Failed to draw" << std::endl;

			//reset input downs
			memset(keysDown, false, INPUT_SIZE);
			memset(mouseDown, false, MOUSE_SIZE);
		}
	}
}