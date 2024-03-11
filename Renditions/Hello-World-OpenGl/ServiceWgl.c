// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "ServiceWgl.h"

// fprintf, stdout
#include <stdio.h>

// malloc, free
#include <malloc.h>

#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem

	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#pragma comment (lib, "gdi32.lib")
	#pragma comment (lib, "user32.lib")
	#pragma comment (lib, "opengl32.lib")

	// Executables (but not DLLs) exporting this symbol with this value will be automatically directed to the high-performance GPU
	// on Nvidia Optimus systems with up-to-date drivers.
	__declspec(dllexport) DWORD NvOptimusEnablement = 1;

	// Executables (but not DLLs) exporting this symbol with this value will be automatically directed to the high-performance GPU
	// on AMD PowerXpress systems with up-to-date drivers
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	#error("operating system not (yet) supported")
#else
	#error("operating system not (yet) supported")
#endif

#include <GL/gl.h>
#include <GL/glext.h>

#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
	#include <GL/wglext.h>
#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	#error("operating system not (yet) supported")
#else
	#error("operating system not (yet) supported")
#endif


static PFNWGLGETPIXELFORMATATTRIBFVARBPROC wglGetPixelFormatAttribfvARB = NULL;

static PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB = NULL;

static PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB = NULL;

static PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = NULL;

static PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;


static HINSTANCE g_hInstance = NULL;

static char* g_className = NULL;

static ATOM g_classHandle = 0;

static HWND g_hWnd = NULL;

static HDC g_hDc = NULL;

static HGLRC g_hGlrc = NULL;



static LRESULT CALLBACK
windowCallbackLegacy
	(
		HWND wnd,
		UINT msg,
		WPARAM wparam,
		LPARAM lparam
	);

static LRESULT CALLBACK
windowCallback
	(
		HWND wnd,
		UINT msg,
		WPARAM wparam,
		LPARAM lparam
	);

static void
choosePixelFormatLegacy
	(
		Zeitgeist_State* state
	);

void
choosePixelFormat
	(
		Zeitgeist_State* state
	);

static void
shutdownLegacy
	(
		Zeitgeist_State* state
	);

static void
startupLegacy
	(
		Zeitgeist_State* state
	);

static void
shutdown
	(
		Zeitgeist_State* state
	);

static void
startup
	(
		Zeitgeist_State* state
	);



static LRESULT CALLBACK
windowCallbackLegacy
	(
		HWND wnd,
		UINT msg,
		WPARAM wparam,
		LPARAM lparam
	)
{
	return DefWindowProc(wnd, msg, wparam, lparam);
}

static LRESULT CALLBACK
windowCallback
	(
		HWND wnd,
		UINT msg,
		WPARAM wparam,
		LPARAM lparam
	)
{
	return DefWindowProc(wnd, msg, wparam, lparam);
}

static void
choosePixelFormatLegacy
	(
		Zeitgeist_State* state
	)
{
	PIXELFORMATDESCRIPTOR descriptor;
	memset(&descriptor, 0, sizeof(descriptor));
	descriptor.nSize = sizeof(descriptor);
	descriptor.nVersion = 1;
	descriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	descriptor.iPixelType = PFD_TYPE_RGBA;
	descriptor.cColorBits = 24;
	int pixelFormat = ChoosePixelFormat(g_hDc, &descriptor);
	if (!pixelFormat) {
		//
		state->lastError = 1;
		longjmp(state->jumpTarget->environment, -1);
	}
	if (!DescribePixelFormat(g_hDc, pixelFormat, sizeof(descriptor), &descriptor)) {
		//
		state->lastError = 1;
		longjmp(state->jumpTarget->environment, -1);
	}
	if (!SetPixelFormat(g_hDc, pixelFormat, &descriptor)) {
		//
		state->lastError = 1;
		longjmp(state->jumpTarget->environment, -1);
	}
}

void
choosePixelFormat
	(
		Zeitgeist_State* state
	)
{ 
	int samples = 0;
	int const pixelFormatAttribs[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_COLOR_BITS_ARB, 24,
		WGL_ALPHA_BITS_ARB, 8,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_SAMPLE_BUFFERS_ARB, samples > 0 ? 1 : 0,
		WGL_SAMPLES_ARB, samples,
		WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE,
		0, 0
	};
	int i, n;
	if (!wglChoosePixelFormatARB(g_hDc, pixelFormatAttribs, 0, 1, &i, &n)) {
		state->lastError = 1;
		longjmp(state->jumpTarget->environment, -1);
	}
	PIXELFORMATDESCRIPTOR desc;
	if (!DescribePixelFormat(g_hDc, i, sizeof(desc), &desc)) {
		state->lastError = 1;
		longjmp(state->jumpTarget->environment, -1);
	}
	if (!SetPixelFormat(g_hDc, i, &desc)) {
		state->lastError = 1;
		longjmp(state->jumpTarget->environment, -1);
	}
}

static void
shutdownLegacy
	(
		Zeitgeist_State* state
	)
{
	wglCreateContextAttribsARB = NULL;
	wglGetPixelFormatAttribfvARB = NULL;
	wglChoosePixelFormatARB = NULL;
	wglGetPixelFormatAttribfvARB = NULL;
	wglGetPixelFormatAttribivARB = NULL;
}

static void
startupLegacy
	(
		Zeitgeist_State* state
	)
{
	g_hInstance = GetModuleHandle(NULL);
	if (!g_hInstance) {
		state->lastError = 1;
		longjmp(state->jumpTarget->environment, -1);
	}
	g_className = _strdup("windowClassLegacy");
	if (!g_className) {
		g_hInstance = NULL;
		//
		state->lastError = 1;
		longjmp(state->jumpTarget->environment, -1);
	}
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = &windowCallbackLegacy;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = g_hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = g_className;
	g_classHandle = RegisterClassEx(&wcex);
	if (!g_classHandle) {
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		state->lastError = 1;
		longjmp(state->jumpTarget->environment, -1);
	}
	g_hWnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
		 										  g_className,
													"Zeitgeist",
													WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
													CW_USEDEFAULT,
													CW_USEDEFAULT,
													CW_USEDEFAULT,
													CW_USEDEFAULT,
													NULL,
													NULL,
													g_hInstance,
													NULL);
	if (!g_hWnd) {
		//
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		state->lastError = 1;
		longjmp(state->jumpTarget->environment, -1);
	}
	g_hDc = GetDC(g_hWnd);
	if (!g_hDc) {
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
		//
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		state->lastError = 1;
		longjmp(state->jumpTarget->environment, -1);
	}
	Zeitgeist_JumpTarget jumpTarget;
	Zeitgeist_State_pushJumpTarget(state, &jumpTarget);
	if (!setjmp(jumpTarget.environment)) {
		choosePixelFormatLegacy(state);
		Zeitgeist_State_popJumpTarget(state);
	} else {
		Zeitgeist_State_popJumpTarget(state);
		//
		ReleaseDC(g_hWnd, g_hDc);
		g_hDc = NULL;
		//
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
		//
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		state->lastError = 1;
		longjmp(state->jumpTarget->environment, -1);
	}
	g_hGlrc = wglCreateContext(g_hDc);
	if (!g_hGlrc) {
		//
		ReleaseDC(g_hWnd, g_hDc);
		g_hDc = NULL;
		//
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
		//
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		state->lastError = 1;
		longjmp(state->jumpTarget->environment, -1);
	}
	if (!wglMakeCurrent(g_hDc, g_hGlrc)) {
		//
		wglDeleteContext(g_hGlrc);
		g_hGlrc = NULL;
		//
		ReleaseDC(g_hWnd, g_hDc);
		g_hDc = NULL;
		//
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
		//
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		state->lastError = 1;
		longjmp(state->jumpTarget->environment, -1);
	}
	//
	wglGetPixelFormatAttribivARB = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribivARB");
	if (!wglGetPixelFormatAttribivARB) {
		//
		wglDeleteContext(g_hGlrc);
		g_hGlrc = NULL;
		//
		ReleaseDC(g_hWnd, g_hDc);
		g_hDc = NULL;
		//
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
		//
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		state->lastError = 1;
		longjmp(state->jumpTarget->environment, -1);
	}
	wglGetPixelFormatAttribfvARB = (PFNWGLGETPIXELFORMATATTRIBFVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribfvARB");
	if (!wglGetPixelFormatAttribfvARB) {
		wglGetPixelFormatAttribivARB = NULL;
		//
		wglDeleteContext(g_hGlrc);
		g_hGlrc = NULL;
		//
		ReleaseDC(g_hWnd, g_hDc);
		g_hDc = NULL;
		//
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
		//
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		state->lastError = 1;
		longjmp(state->jumpTarget->environment, -1);
	}
	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	if (!wglGetPixelFormatAttribfvARB) {
		wglGetPixelFormatAttribfvARB = NULL;
		wglGetPixelFormatAttribivARB = NULL;
		//
		wglDeleteContext(g_hGlrc);
		g_hGlrc = NULL;
		//
		ReleaseDC(g_hWnd, g_hDc);
		g_hDc = NULL;
		//
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
		//
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		state->lastError = 1;
		longjmp(state->jumpTarget->environment, -1);
	}
	wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
	if (!wglGetPixelFormatAttribfvARB) {
		wglChoosePixelFormatARB = NULL;
		wglGetPixelFormatAttribfvARB = NULL;
		wglGetPixelFormatAttribivARB = NULL;
		//
		wglDeleteContext(g_hGlrc);
		g_hGlrc = NULL;
		//
		ReleaseDC(g_hWnd, g_hDc);
		g_hDc = NULL;
		//
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
		//
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		state->lastError = 1;
		longjmp(state->jumpTarget->environment, -1);
	}
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	if (!wglCreateContextAttribsARB) {
		wglChoosePixelFormatARB = NULL;
		wglGetPixelFormatAttribfvARB = NULL;
		wglGetPixelFormatAttribivARB = NULL;
		//
		wglDeleteContext(g_hGlrc);
		g_hGlrc = NULL;
		//
		ReleaseDC(g_hWnd, g_hDc);
		g_hDc = NULL;
		//
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
		//
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		state->lastError = 1;
		longjmp(state->jumpTarget->environment, -1);
	}
	//
	wglDeleteContext(g_hGlrc);
	g_hGlrc = NULL;
	//
	ReleaseDC(g_hWnd, g_hDc);
	g_hDc = NULL;
	//
	DestroyWindow(g_hWnd);
	g_hWnd = NULL;
	//
	UnregisterClass(g_className, g_hInstance);
	g_classHandle = 0;
	//
	free(g_className);
	g_className = NULL;
	//
	g_hInstance = NULL;
}

static void
shutdown
	(
		Zeitgeist_State* state
	)
{ 
	if (g_hGlrc) {
		wglDeleteContext(g_hGlrc);
		g_hGlrc = NULL;
	}
	//
	if (g_hDc) {
		ReleaseDC(g_hWnd, g_hDc);
		g_hDc = NULL;
	}
	//
	if (g_hWnd) {
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
	}
	//
	if (g_classHandle) {
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
	}
	//
	if (g_className) {
		free(g_className);
		g_className = NULL;
	}
	//
	if (g_hInstance) {
		g_hInstance = NULL;
	}
}

static void
startup
	(
		Zeitgeist_State* state
	)
{
	g_hInstance = GetModuleHandle(NULL);
	if (!g_hInstance) {
		shutdownLegacy(state);
		//
		state->lastError = 1;
		longjmp(state->jumpTarget->environment, -1);
	}
	g_className = _strdup("windowClass");
	if (!g_className) {
		g_hInstance = NULL;
		//
		shutdownLegacy(state);
		//
		state->lastError = 1;
		longjmp(state->jumpTarget->environment, -1);
	}
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = &windowCallback;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = g_hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = g_className;
	g_classHandle = RegisterClassEx(&wcex);
	if (!g_classHandle) {
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		shutdownLegacy(state);
		//
		state->lastError = 1;
		longjmp(state->jumpTarget->environment, -1);
	}
	g_hWnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
		g_className,
		"Zeitgeist",
		WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		g_hInstance,
		NULL);
	if (!g_hWnd) {
		//
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		shutdownLegacy(state);
		//
		state->lastError = 1;
		longjmp(state->jumpTarget->environment, -1);
	}
	g_hDc = GetDC(g_hWnd);
	if (!g_hDc) {
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
		//
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		shutdownLegacy(state);
		//
		state->lastError = 1;
		longjmp(state->jumpTarget->environment, -1);
	}
	Zeitgeist_JumpTarget jumpTarget;
	Zeitgeist_State_pushJumpTarget(state, &jumpTarget);
	if (!setjmp(jumpTarget.environment)) {
		choosePixelFormat(state);
		Zeitgeist_State_popJumpTarget(state);
	} else {
		Zeitgeist_State_popJumpTarget(state);
		//
		ReleaseDC(g_hWnd, g_hDc);
		g_hDc = NULL;
		//
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
		//
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		shutdownLegacy(state);
		//
		state->lastError = 1;
		longjmp(state->jumpTarget->environment, -1);
	}
	//
	const int contextAttribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 1,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};
	g_hGlrc = wglCreateContextAttribsARB(g_hDc, NULL, contextAttribs);
	if (!g_hGlrc) {
		//
		ReleaseDC(g_hWnd, g_hDc);
		g_hDc = NULL;
		//
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
		//
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		shutdownLegacy(state);
		//
		state->lastError = 1;
		longjmp(state->jumpTarget->environment, -1);
	}
	if (!wglMakeCurrent(g_hDc, g_hGlrc)) {
		//
		wglDeleteContext(g_hGlrc);
		g_hGlrc = NULL;
		//
		ReleaseDC(g_hWnd, g_hDc);
		g_hDc = NULL;
		//
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
		//
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		shutdownLegacy(state);
		//
		state->lastError = 1;
		longjmp(state->jumpTarget->environment, -1);
	}
}

void
ServiceWgl_startup
	(
		Zeitgeist_State* state
	)
{
	fprintf(stdout, "[Hello World (OpenGL)] starting up OpenGL service\n");
	startupLegacy(state);
	Zeitgeist_JumpTarget jumpTarget;
	Zeitgeist_State_pushJumpTarget(state, &jumpTarget);
	if (!setjmp(jumpTarget.environment)) {
		startup(state);
		Zeitgeist_State_popJumpTarget(state);
	} else {
		Zeitgeist_State_popJumpTarget(state);
		shutdownLegacy(state);
		longjmp(state->jumpTarget->environment, -1);
	}
}

void
ServiceWgl_shutdown
	(
		Zeitgeist_State* state
 	)
{
	fprintf(stdout, "[Hello World (OpenGL)] shutting down OpenGL service\n");
	shutdown(state);
	shutdownLegacy(state);
}
