// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "Zeitgeist/Rendition.h"

#include "Zeitgeist.h"

// malloc, free
#include <malloc.h>

// strlen
#include <string.h>

// fprintf, stdio
#include <stdio.h>

#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
	static void* getRenditionLibrarySymbol(HMODULE libraryHandle, char const *symbolName) {
		return GetProcAddress(libraryHandle, symbolName);
	} 
#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	static void* getRenditionLibrarySymbol(void* libraryHandle, char const* symbolName) {
		return dlsym(libraryHandle, symbolName);
	}
#else
	#error("operating system not (yet) supported")
#endif

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
#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
	if (!rendition->libraryHandle) {
		Zeitgeist_String* zeroTerminator = Zeitgeist_State_createString(state,	"", sizeof(char));
		Zeitgeist_String* libraryPath = Zeitgeist_State_createString(state, RENDITION_DIRECTORY_SEPARATOR "library" RENDITION_EXTENSION,
																																 strlen(RENDITION_DIRECTORY_SEPARATOR "library" RENDITION_EXTENSION));
		libraryPath = Zeitgeist_String_concatenate(state, rendition->folderPath, libraryPath);
		libraryPath = Zeitgeist_String_concatenate(state, libraryPath, zeroTerminator);
		rendition->libraryHandle = LoadLibrary(libraryPath->bytes);
		if (!rendition->libraryHandle) {
			fprintf(stderr, "unable to link `%.*s`\n", (int)libraryPath->numberOfBytes, libraryPath->bytes);
			longjmp(state->jumpTarget->environment, -1);
		}
	}
#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	if (!rendition->libraryHandle) {
		Zeitgeist_String* zeroTerminator = Zeitgeist_State_createString(state,	"", sizeof(char));
		Zeitgeist_String* libraryPath = Zeitgeist_State_createString(state, RENDITION_DIRECTORY_SEPARATOR "library" RENDITION_EXTENSION,
																																 strlen(RENDITION_DIRECTORY_SEPARATOR "library" RENDITION_EXTENSION));
		libraryPath = Zeitgeist_String_concatenate(state, rendition->folderPath, libraryPath);
		libraryPath = Zeitgeist_String_concatenate(state, libraryPath, zeroTerminator);
		rendition->libraryHandle = dlopen(libraryPath->bytes, RTLD_LAZY | RTLD_GLOBAL);
		if (!rendition->libraryHandle) {
			fprintf(stderr, "unable to link `%.*s`\n", (int)libraryPath->numberOfBytes, libraryPath->bytes);
			longjmp(state->jumpTarget->environment, -1);
		}
	}
#else
	#error("operating system not (yet) supported")
#endif
	Zeitgeist_JumpTarget jumpTarget;
	Zeitgeist_State_pushJumpTarget(state, &jumpTarget);
	if (!setjmp(jumpTarget.environment)) {
		Zeitgeist_String* (*getName)(Zeitgeist_State*) = (Zeitgeist_String * (*)(Zeitgeist_State*))getRenditionLibrarySymbol(rendition->libraryHandle, "Zeitgeist_Rendition_getName");
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

Zeitgeist_ForeignProcedure*
Zeitgeist_Rendition_getUpdate
	(
		Zeitgeist_State* state,
		Zeitgeist_Rendition* rendition
	)
{
#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
	if (!rendition->libraryHandle) {
		Zeitgeist_String* zeroTerminator = Zeitgeist_State_createString(state,	"", sizeof(char));
		Zeitgeist_String* libraryPath = Zeitgeist_State_createString(state, RENDITION_DIRECTORY_SEPARATOR "library" RENDITION_EXTENSION,
																																 strlen(RENDITION_DIRECTORY_SEPARATOR "library" RENDITION_EXTENSION));
		libraryPath = Zeitgeist_String_concatenate(state, rendition->folderPath, libraryPath);
		libraryPath = Zeitgeist_String_concatenate(state, libraryPath, zeroTerminator);
		rendition->libraryHandle = LoadLibrary(libraryPath->bytes);
		if (!rendition->libraryHandle) {
			fprintf(stderr, "unable to link `%.*s`\n", (int)libraryPath->numberOfBytes, libraryPath->bytes);
			longjmp(state->jumpTarget->environment, -1);
		}
	}
#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	if (!rendition->libraryHandle) {
		Zeitgeist_String* zeroTerminator = Zeitgeist_State_createString(state,	"", sizeof(char));
		Zeitgeist_String* libraryPath = Zeitgeist_State_createString(state, RENDITION_DIRECTORY_SEPARATOR "library" RENDITION_EXTENSION,
																																 strlen(RENDITION_DIRECTORY_SEPARATOR "library" RENDITION_EXTENSION));
		libraryPath = Zeitgeist_String_concatenate(state, rendition->folderPath, libraryPath);
		libraryPath = Zeitgeist_String_concatenate(state, libraryPath, zeroTerminator);
		rendition->libraryHandle = dlopen(libraryPath->bytes, RTLD_LAZY | RTLD_GLOBAL);
		if (!rendition->libraryHandle) {
			fprintf(stderr, "unable to link `%.*s`\n", (int)libraryPath->numberOfBytes, libraryPath->bytes);
			longjmp(state->jumpTarget->environment, -1);
		}
	}
#else
	#error("operating system not (yet) supported")
#endif
	Zeitgeist_JumpTarget jumpTarget;
	Zeitgeist_State_pushJumpTarget(state, &jumpTarget);
	if (!setjmp(jumpTarget.environment)) {
		Zeitgeist_ForeignProcedure* update = (void (*)(Zeitgeist_State*))getRenditionLibrarySymbol(rendition->libraryHandle, "Zeitgeist_Rendition_update");
		if (!update) {
			fprintf(stderr, "unable to link `%s` of `%.*s`\n", "Zeitgeist_Rendition_update", (int)rendition->folderPath->numberOfBytes, rendition->folderPath->bytes);
			longjmp(state->jumpTarget->environment, -1);
		}
		Zeitgeist_State_popJumpTarget(state);
		return update;
	} else {
		Zeitgeist_State_popJumpTarget(state);
		longjmp(state->jumpTarget->environment, -1);
	}
}

void
Zeitgeist_Rendition_unload
	(
		Zeitgeist_State* state,
		Zeitgeist_Rendition* rendition
	);
