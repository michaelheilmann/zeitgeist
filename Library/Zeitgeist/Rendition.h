// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#if !defined(ZEITGEIST_RENDITION_H_INCLUDED)
#define ZEITGEIST_RENDITION_H_INCLUDED

#include "Zeitgeist/Object.h"
#include "Zeitgeist/Value.h"

typedef void (Zeitgeist_ForeignProcedure)(Zeitgeist_State*);

#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#define RENDITION_EXTENSION ".dll"
	#define RENDITION_DIRECTORY_SEPARATOR "\\"
	typedef HMODULE Rendition_LibraryHandle;
	#define Rendition_LibraryHandle_Invalid (NULL)
#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	#include <dlfcn.h>
	#define RENDITION_EXTENSION ".so"
	#define RENDITION_DIRECTORY_SEPARATOR "/"
	typedef void* Rendition_LibraryHandle;
	#define Rendition_LibraryHandle_Invalid (NULL)
#else
	#error("operating system not (yet) supported")
#endif

typedef struct Zeitgeist_Rendition Zeitgeist_Rendition;

struct Zeitgeist_Rendition {
	Zeitgeist_Object _parent;

	/** The path to the folder of the rendition. */
	Zeitgeist_String* folderPath;

	/** The library handle if the library is loaded. Rendition_LibraryHandle_Invalid otherwise. */
	Rendition_LibraryHandle libraryHandle;

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

Zeitgeist_ForeignProcedure*
Zeitgeist_Rendition_getUpdate
	(
		Zeitgeist_State* state,
		Zeitgeist_Rendition* rendition
	);

Zeitgeist_ForeignProcedure*
Zeitgeist_Rendition_getLoad
	(
		Zeitgeist_State* state,
		Zeitgeist_Rendition* rendition
	);

Zeitgeist_ForeignProcedure*
Zeitgeist_Rendition_getUnload
	(
		Zeitgeist_State* state,
		Zeitgeist_Rendition* rendition
	);

#endif // ZEITGEIST_RENDITION_H_INCLUDED
