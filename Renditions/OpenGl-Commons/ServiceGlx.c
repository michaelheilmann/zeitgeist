// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "ServiceGlx.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <GL/gl.h>
#include <GL/glx.h>

#include <X11/Xatom.h>

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

static int (*g_oldErrorHandler)(Display*, XErrorEvent*) = NULL;
static bool g_error = false;
static Display* g_display = NULL;
static Window g_window;
static GLXFBConfig g_glx_bestFbConfig;
static Colormap g_colorMap;
static GLXContext g_context;
static Atom WM_DELETE_WINDOW;

static bool g_quit = false;

// Helper to check for extension string presence.	Adapted from:
// http://www.opengl.org/resources/features/OGLextensions/
static bool
isExtensionSupported
	(
		const char *extensions,
		const char *extension
	)
{
	const char *start;
	const char *where, *terminator;
	
	/* Extension names should not have spaces. */
	where = strchr(extension, ' ');
	if (where || *extension == '\0')
		return false;

	/* It takes a bit of care to be fool-proof about parsing the
		 OpenGL extensions string. Don't be fooled by sub-strings,
		 etc. */
	for (start = extensions;;) {
		where = strstr(start, extension);

		if (!where)
			break;

		terminator = where + strlen(extension);

		if ( where == start || *(where - 1) == ' ' )
			if ( *terminator == ' ' || *terminator == '\0' )
				return true;

		start = terminator;
	}

	return false;
}

static int
getBestFbc
	(
		Zeitgeist_State* state,
		GLXFBConfig* configs,
		int numberOfConfigs
	)
{
	int best = -1, worst = -1, best_num_samp = -1, worst_num_samp = 999;
	int current;
	for (current = 0; current < numberOfConfigs; ++current) {
		XVisualInfo *visual = glXGetVisualFromFBConfig(g_display, configs[current]);
		if (visual) {
			int samp_buf, samples;
			glXGetFBConfigAttrib( g_display, configs[current], GLX_SAMPLE_BUFFERS, &samp_buf);
			glXGetFBConfigAttrib( g_display, configs[current], GLX_SAMPLES, &samples);
			if ( best < 0 || samp_buf && samples > best_num_samp )
				best = current, best_num_samp = samples;
			if ( worst < 0 || !samp_buf || samples < worst_num_samp )
				worst = current, worst_num_samp = samples;
		}
		XFree(visual);
	}
	return best;
}

static int
errorHandler
	(
		Display* display,
		XErrorEvent*
	)
{
	g_error = true;
}

static void
startupContext
	(
		Zeitgeist_State* state
	)
{
	g_oldErrorHandler = XSetErrorHandler(&errorHandler);

	const char *extensions = glXQueryExtensionsString( g_display,
																										 DefaultScreen( g_display ) );
	if (!extensions) {
		fprintf(stderr, "%s:%d: unable to get extension strings\n", __FILE__, __LINE__);
		XSetErrorHandler(g_oldErrorHandler);
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}

	// NOTE: It is not necessary to create or make current to a context before calling glXGetProcAddressARB.
	glXCreateContextAttribsARBProc glXCreateContextAttribsARB = NULL;
	glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB((GLubyte const *)"glXCreateContextAttribsARB");
	if (!glXCreateContextAttribsARB) {
		fprintf(stderr, "%s:%d: unable to get %s\n", __FILE__, __LINE__, "glXCreateContextAttribsARB");
		XSetErrorHandler(g_oldErrorHandler);
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	
	// Check for the GLX_ARB_create_context extension string and the function.
	// If either is not present, use GLX 1.3 context creation method.
	if (!isExtensionSupported(extensions, "GLX_ARB_create_context") || !glXCreateContextAttribsARB) {
		fprintf(stderr, "%s:%d: unable to get %s extension\n", __FILE__, __LINE__, "GLX_ARB_create_context	/ glXCreateContextAttribsARB");
		XSetErrorHandler(g_oldErrorHandler);
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	
	int contextAttribs[] = {
		GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
		GLX_CONTEXT_MINOR_VERSION_ARB, 0,
		//GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		None
	};
	g_context = glXCreateContextAttribsARB(g_display, g_glx_bestFbConfig, 0,
																				 True, contextAttribs );
	XSync(g_display, False);
	if (!g_context) {
		fprintf(stderr, "%s:%d: unable to get create context\n", __FILE__, __LINE__);
		XSetErrorHandler(g_oldErrorHandler);
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	
	if (!glXIsDirect(g_display, g_context)) {
		fprintf(stderr, "%s:%d: unable to get create context\n", __FILE__, __LINE__);
		glXDestroyContext(g_display, g_context);
		g_context = NULL;
		XSetErrorHandler(g_oldErrorHandler);
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	
	if (!glXMakeCurrent(g_display, g_window, g_context)) {
		fprintf(stderr, "%s:%d: unable to make context current\n", __FILE__, __LINE__);
		glXDestroyContext(g_display, g_context);
		g_context = NULL;
		XSetErrorHandler(g_oldErrorHandler);
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	
	XSetErrorHandler(g_oldErrorHandler);
}

static void
shutdownContext
	(
		Zeitgeist_State* state
	)
{
	glXMakeCurrent(g_display, 0, 0);
	glXDestroyContext(g_display, g_context);
	g_context = NULL;
}

static void
startupWindow
	(
		Zeitgeist_State* state
	)
{ 
	g_oldErrorHandler = XSetErrorHandler(&errorHandler);

	g_display = XOpenDisplay(NULL);
	if (!g_display) {
		fprintf(stderr, "%s:%d: unable to open display\n", __FILE__, __LINE__);
		XSetErrorHandler(g_oldErrorHandler);
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	// Get a matching FB config
	static int glx_visualAttributes[] = {
		GLX_X_RENDERABLE		, True,
		GLX_DRAWABLE_TYPE	 , GLX_WINDOW_BIT,
		GLX_RENDER_TYPE		 , GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE	 , GLX_TRUE_COLOR,
		GLX_RED_SIZE				, 8,
		GLX_GREEN_SIZE			, 8,
		GLX_BLUE_SIZE			 , 8,
		GLX_ALPHA_SIZE			, 8,
		GLX_DEPTH_SIZE			, 24,
		GLX_STENCIL_SIZE		, 8,
		GLX_DOUBLEBUFFER		, True,
		//GLX_SAMPLE_BUFFERS, 1,
		//GLX_SAMPLES			 , 4,
		None
	};
	int glx_major, glx_minor;
 
	// FBConfigs were added in GLX version 1.3.
	if (!glXQueryVersion( g_display, &glx_major, &glx_minor)) {
		XCloseDisplay(g_display);
		g_display = NULL;
		fprintf(stderr, "%s:%d: unable to get GLX version\n", __FILE__, __LINE__);
		XSetErrorHandler(g_oldErrorHandler);
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	if ((glx_major == 1 && glx_minor < 3) || (glx_major < 1)) {
		XCloseDisplay(g_display);
		g_display = NULL;
		fprintf(stderr, "%s:%d: GLX version %d.%d smaller than the minimum required version %d.%d\n", __FILE__, __LINE__, glx_major, glx_minor, 1, 3);
		XSetErrorHandler(g_oldErrorHandler);
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	
	int glx_fbConfigCount;
	GLXFBConfig* glx_fbConfigs = glXChooseFBConfig(g_display, DefaultScreen(g_display), glx_visualAttributes, &glx_fbConfigCount);
	if (!glx_fbConfigs) {
		XCloseDisplay(g_display);
		g_display = NULL;
		fprintf(stderr, "%s:%d: unable to get matching frame buffer configuration(s)\n", __FILE__, __LINE__);
		XSetErrorHandler(g_oldErrorHandler);
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	//
	int glx_bestFbConfigIndex = getBestFbc(state, glx_fbConfigs, glx_fbConfigCount);
	if (-1 == glx_bestFbConfigIndex) {
		XFree(glx_fbConfigs);
		glx_fbConfigs = NULL;
		XCloseDisplay(g_display);
		g_display = NULL;
		fprintf(stderr, "%s:%d: unable to get matching frame buffer configuration(s)\n", __FILE__, __LINE__);
		XSetErrorHandler(g_oldErrorHandler);
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	g_glx_bestFbConfig = glx_fbConfigs[ glx_bestFbConfigIndex ];
	
	//
	XFree(glx_fbConfigs);
	glx_fbConfigs = NULL;
	
	XVisualInfo *x_visualInfo = glXGetVisualFromFBConfig( g_display, g_glx_bestFbConfig );
	if (NULL == x_visualInfo) {
		XCloseDisplay(g_display);
		g_display = NULL;
		fprintf(stderr, "%s:%d: unable to get matching visual\n", __FILE__, __LINE__);
		XSetErrorHandler(g_oldErrorHandler);
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	
	XSetWindowAttributes swa;
	swa.colormap = g_colorMap = XCreateColormap( g_display,
																							 RootWindow( g_display, x_visualInfo->screen ), 
																							 x_visualInfo->visual, AllocNone );
	if (g_error) {
		XFree(x_visualInfo);
		x_visualInfo = NULL;
		XCloseDisplay(g_display);
		g_display = NULL;
		fprintf(stderr, "%s:%d: unable to create a color map\n", __FILE__, __LINE__);
		XSetErrorHandler(g_oldErrorHandler);
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	swa.background_pixmap = None ;
	swa.border_pixel			= 0;
	swa.event_mask				= StructureNotifyMask;
	
	g_window = XCreateWindow( g_display, RootWindow( g_display, x_visualInfo->screen ), 
														0, 0, 640, 480, 0, x_visualInfo->depth, InputOutput, 
														x_visualInfo->visual, 
														CWBorderPixel|CWColormap|CWEventMask, &swa );
	if (g_error) {
		XFree(x_visualInfo);
		x_visualInfo = NULL;
		XCloseDisplay(g_display);
		g_display = NULL;
		fprintf(stderr, "%s:%d: unable to create window\n", __FILE__, __LINE__);
		XSetErrorHandler(g_oldErrorHandler);
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	
	XFree(x_visualInfo);
	x_visualInfo = NULL;
	
	WM_DELETE_WINDOW = XInternAtom(g_display, "WM_DELETE_WINDOW", False); 
	XSetWMProtocols(g_display, g_window, &WM_DELETE_WINDOW, 1);	
	
	XMapWindow( g_display, g_window );
	
	XSync(g_display, False);

	XSetErrorHandler(g_oldErrorHandler);
	
	g_quitRequested = false;
}

static void
shutdownWindow
	(
		Zeitgeist_State* state
	)
{ 
	XDestroyWindow(g_display, g_window);
	XFreeColormap(g_display, g_colorMap);
	XCloseDisplay(g_display);
	g_display = NULL;
}

void
ServiceGlx_startup
	(
		Zeitgeist_State* state
	)
{ 
	startupWindow(state);
	startupContext(state);
}

void
ServiceGlx_shutdown
	(
		Zeitgeist_State* state
	)
{
	shutdownContext(state);
	shutdownWindow(state);
}

void
ServiceGlx_setTitle
	(
		Zeitgeist_State* state,
		Zeitgeist_String* title
	)
{
	Zeitgeist_String* zeroTerminator = Zeitgeist_State_createString(state, "", 1);
	title = Zeitgeist_String_concatenate(state, title, zeroTerminator);
	
	XTextProperty prop;
	prop.value = title->bytes;
	prop.encoding = XA_STRING;
	prop.format = 8;
	prop.nitems = title->numberOfBytes - 1;
	g_oldErrorHandler = XSetErrorHandler(&errorHandler);
	XSetWMName(g_display, g_window, &prop);
	XStoreName(g_display, g_window, title->bytes);
	XSync(g_display, False);
	if (g_error) {
		fprintf(stderr, "failed to set title\n");
	}
	XSetErrorHandler(g_oldErrorHandler);
}

void ServiceGlx_update(Zeitgeist_State* state) {
	XEvent e;
	while (XPending(g_display)) {
		XNextEvent(g_display, &e);
		if ((e.type == ClientMessage) && (e.xclient.data.l[0] == WM_DELETE_WINDOW)) {
			g_quitRequested = true;
			break;
		}
	}
}

Zeitgeist_Boolean
ServiceGlx_quitRequested
	(
		Zeitgeist_State* state
	)
{ return g_quitRequested; }

void
ServiceGlx_getClientSize
	(
		Zeitgeist_State* state,
		Zeitgeist_Integer *width,
		Zeitgeist_Integer *height
	)
{
	XWindowAttributes windowAttributes;
	XGetWindowAttributes(g_display, g_window, &windowAttributes);
	*width = windowAttributes.width;
	*height = windowAttributes.height;
}

void*
ServiceWgl_link
	(
		Zeitgeist_State* state,
		char const* functionName,
		char const* extensionName
	)
{
	if (extensionName) {
		char const* extensionNames = NULL;
		extensionNames = glXQueryExtensionsString(g_hDc);
		if (!isExtensionSupported(extensionNames, extensionName)) {
			extensionNames = glGetString(GL_EXTENSIONS);
			if (!isExtensionSupported(extensionNames, extensionName)) {
				Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
			}
		}
	}
	void* p = glXGetProcAddress(functionName);
	if (!p) {
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	return p;
}

void
ServiceGlx_beginFrame
	(
		Zeitgeist_State* state
	)
{/*Intentionally empty.*/}

void
ServiceGlx_endFrame
	(
		Zeitgeist_State* state
	)
{
	glxSwapBuffers(g_display, g_window);
}
