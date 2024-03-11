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

static int (*g_oldErrorHandler)(Display*, XErrorEvent*) = NULL;
static bool g_error = false;
static Display* g_display = NULL;
static Window g_window;
static Colormap g_colorMap;

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

static int ServiceGlx_errorHandler(Display* display, XErrorEvent*) {
	g_error = true;
}

void
ServiceGlx_startup
	(
		Zeitgeist_State* state
	)
{ 
	g_oldErrorHandler = XSetErrorHandler(&ServiceGlx_errorHandler);

	g_display = XOpenDisplay(NULL);
  if (!g_display) {
    fprintf(stderr, "%s:%d: unable to open display\n", __FILE__, __LINE__);
		XSetErrorHandler(g_oldErrorHandler);
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
  }
  // Get a matching FB config
  static int glx_visualAttributes[] =
    {
      GLX_X_RENDERABLE    , True,
      GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
      GLX_RENDER_TYPE     , GLX_RGBA_BIT,
      GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
      GLX_RED_SIZE        , 8,
      GLX_GREEN_SIZE      , 8,
      GLX_BLUE_SIZE       , 8,
      GLX_ALPHA_SIZE      , 8,
      GLX_DEPTH_SIZE      , 24,
      GLX_STENCIL_SIZE    , 8,
      GLX_DOUBLEBUFFER    , True,
      //GLX_SAMPLE_BUFFERS  , 1,
      //GLX_SAMPLES         , 4,
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
	GLXFBConfig glx_bestFbConfig = glx_fbConfigs[ glx_bestFbConfigIndex ];
	
	//
	XFree(glx_fbConfigs);
	glx_fbConfigs = NULL;
	
	XVisualInfo *x_visualInfo = glXGetVisualFromFBConfig( g_display, glx_bestFbConfig );
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
  swa.border_pixel      = 0;
  swa.event_mask        = StructureNotifyMask;
	
  g_window = XCreateWindow( g_display, RootWindow( g_display, x_visualInfo->screen ), 
                            0, 0, 100, 100, 0, x_visualInfo->depth, InputOutput, 
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
	
	XMapWindow( g_display, g_window );
	
	XSync(g_display, False);
	
  sleep(1);
	
	XSetErrorHandler(g_oldErrorHandler);
}

void
ServiceGlx_shutdown
	(
		Zeitgeist_State* state
	)
{ 
	XDestroyWindow(g_display, g_window);
	XFreeColormap(g_display, g_colorMap);
	XCloseDisplay(g_display);
	g_display = NULL;
}
