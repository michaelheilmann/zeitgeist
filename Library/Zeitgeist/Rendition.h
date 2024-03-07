// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#if !defined(ZEITGEIST_RENDITION_H_INCLUDED)
#define ZEITGEIST_RENDITION_H_INCLUDED

#include "Zeitgeist/Object.h"
#include "Zeitgeist/Value.h"

#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>
  #define RENDITION_EXTENSION ".dll"
#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
  #include <dlfcn.h>
  #define RENDITION_EXTENSION ".so"
#else
  #error("operating system not (yet) supported")
#endif

typedef struct Zeitgeist_Rendition Zeitgeist_Rendition;

struct Zeitgeist_Rendition {
  Zeitgeist_Object _parent;
  // The path to the folder of the rendition.
  Zeitgeist_String* folderPath;
#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
  HMODULE libraryHandle;
#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
  void* libraryHandle;
#else
  #error("operating system not (yet) supported")
#endif
  Zeitgeist_String*(*getName)(Zeitgeist_State* state, Zeitgeist_Rendition* rendition);
};

Zeitgeist_Rendition*
Zeitgeist_createRendition
  (
    Zeitgeist_State* state,
    Zeitgeist_String* folderPath
  );

Zeitgeist_String*
Zeitgeist_Rendition_getName
  (
    Zeitgeist_State* state,
    Zeitgeist_Rendition* rendition
  );

#endif // ZEITGEIST_RENDITION_H_INCLUDED
