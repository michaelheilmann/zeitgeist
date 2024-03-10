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

static Display* g_display = NULL;

void
ServiceGlx_startup
	(
		Zeitgeist_State* state
	)
{ 
	g_display = XOpenDisplay(NULL);
  if (!g_display) {
    fprintf(stderr, "%s:%d: unable to open display\n", __FILE__, __LINE__);
    state->lastError = 1;
		longjmp(state->jumpTarget->environment, -1);
  }
}

void
ServiceGlx_shutdown
	(
		Zeitgeist_State* state
	)
{ 
	XCloseDisplay(g_display);
	g_display = NULL;
}
