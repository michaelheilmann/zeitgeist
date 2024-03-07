// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "Zeitgeist/Rendition.h"

#include "Zeitgeist.h"

// malloc, free
#include <malloc.h>

// strlen
#include <string.h>

// fprintf, stdio
#include <stdio.h>

static void
Zeitgeist_Rendition_finalize
  (
    Zeitgeist_State* state,
    Zeitgeist_Rendition* rendition
  );

static void
Zeitgeist_Rendition_finalize
  (
    Zeitgeist_State* state,
    Zeitgeist_Rendition* rendition
  )
{ 
  if (rendition->libraryHandle) {
  #if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
    FreeModule(rendition->libraryHandle);
    rendition->libraryHandle = NULL;
  #elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
    dlclose(rendition->libraryHandle);
    rendition->libraryHandle = NULL;
  #else
    #error("operating system not (yet) supported")
  #endif
  }
}

Zeitgeist_Rendition*
Zeitgeist_createRendition
  (
    Zeitgeist_State* state,
    Zeitgeist_String* folderPath
  )
{
  Zeitgeist_Rendition* rendition = malloc(sizeof(Zeitgeist_Rendition));
  if (!rendition) {
    longjmp(state->jumpTarget->environment, -1);
  }
  rendition->folderPath = folderPath;
  rendition->libraryHandle = NULL;
  ((Zeitgeist_Object*)rendition)->finalize = (void (*)(Zeitgeist_State*, Zeitgeist_Object*)) & Zeitgeist_Rendition_finalize;
  ((Zeitgeist_Object*)rendition)->next = state->objects;
  state->objects = (Zeitgeist_Object*)rendition;
  return rendition;
}

Zeitgeist_String*
Zeitgeist_Rendition_getName
  (
    Zeitgeist_State* state,
    Zeitgeist_Rendition* rendition
  )
{
  if (!rendition->libraryHandle) {
    Zeitgeist_String* libraryPath = Zeitgeist_State_createString(state, "\\library" RENDITION_EXTENSION,
                                                                 strlen("\\library" RENDITION_EXTENSION));
    libraryPath = Zeitgeist_String_concatenate(state, rendition->folderPath, libraryPath);
    char path[MAX_PATH];
    if (libraryPath->numberOfBytes >= MAX_PATH) {
      longjmp(state->jumpTarget->environment, -1);
    }
    memcpy(path, libraryPath->bytes, libraryPath->numberOfBytes);
    path[libraryPath->numberOfBytes] = '\0';

    rendition->libraryHandle = LoadLibrary(path);
    if (!rendition->libraryHandle) {
      fprintf(stderr, "unable to link `%.*s`\n", (int)libraryPath->numberOfBytes, libraryPath->bytes);
      longjmp(state->jumpTarget->environment, -1);
    }
  }
  Zeitgeist_JumpTarget jumpTarget;
  Zeitgeist_State_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Zeitgeist_String* (*getName)(Zeitgeist_State*) = (Zeitgeist_String * (*)(Zeitgeist_State*))GetProcAddress(rendition->libraryHandle, "Zeitgeist_Rendition_getName");
    if (!getName) {
      fprintf(stderr, "unable to link `%s` of `%.*s`\n", "Zeitgeist_Rendition_getName", (int)rendition->folderPath->numberOfBytes, rendition->folderPath->bytes);
      longjmp(state->jumpTarget->environment, -1);
    }
    Zeitgeist_String* name = getName(state);
    Zeitgeist_State_popJumpTarget(state);
    return name;
  } else {
    Zeitgeist_State_popJumpTarget(state);
    longjmp(state->jumpTarget->environment, -1);
  }
}
