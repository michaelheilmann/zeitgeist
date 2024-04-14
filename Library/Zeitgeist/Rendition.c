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
Zeitgeist_Rendition_destruct
	(
		Zeitgeist_State* state,
		Zeitgeist_Rendition* rendition
	);

static void
Zeitgeist_Rendition_finalize
	(
		Zeitgeist_State* state,
		Zeitgeist_Rendition* rendition
	);


static void
Zeitgeist_Rendition_visit
	(
		Zeitgeist_State* state,
		Zeitgeist_Rendition* rendition
	);

Zeitgeist_ObjectType const g_Rendition_Type = {
	.name = "Rendition",
	.parentType = &g_Zeitgeist_Object_Type,
	.destruct = (void(*)(Zeitgeist_State*,Zeitgeist_Object*))&Zeitgeist_Rendition_destruct,
	.visit = (void(*)(Zeitgeist_State*,Zeitgeist_Object*))&Zeitgeist_Rendition_visit,
};

static void
Zeitgeist_Rendition_destruct
	(
		Zeitgeist_State* state,
		Zeitgeist_Rendition* self
	)
{
	if (self->libraryHandle) {
	#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
		FreeModule(self->libraryHandle);
		self->libraryHandle = NULL;
	#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
		dlclose(self->libraryHandle);
		self->libraryHandle = NULL;
	#else
		#error("operating system not (yet) supported")
	#endif
	}
}

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

static void
Zeitgeist_Rendition_visit
	(
		Zeitgeist_State* state,
		Zeitgeist_Rendition* rendition
	)
{
	if (rendition->folderPath) {
		Zeitgeist_String_visit(state, rendition->folderPath);
	}
}

static void
Zeitgeist_Rendition_ensureLibraryLoaded
	(
		Zeitgeist_State* state,
		Zeitgeist_Rendition* rendition
	)
{
#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
	if (!rendition->libraryHandle) {
		Zeitgeist_String* zeroTerminator = Zeitgeist_State_createString(state, "", sizeof(char));
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
		Zeitgeist_String* zeroTerminator = Zeitgeist_State_createString(state, "", sizeof(char));
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
}

Zeitgeist_Rendition*
Zeitgeist_createRendition
	(
		Zeitgeist_State* state,
		Zeitgeist_String* folderPath
	)
{
	Zeitgeist_Rendition* self = Zeitgeist_allocateObject(state, sizeof(Zeitgeist_Rendition), NULL, NULL);
	self->folderPath = folderPath;
	self->libraryHandle = NULL;

	((Zeitgeist_Object*)self)->finalize = (void (*)(Zeitgeist_State*, Zeitgeist_Object*)) & Zeitgeist_Rendition_finalize;
	((Zeitgeist_Object*)self)->visit = (void (*)(Zeitgeist_State*, Zeitgeist_Object*)) & Zeitgeist_Rendition_visit;

	return self;
}

Zeitgeist_String*
Zeitgeist_Rendition_getName
	(
		Zeitgeist_State* state,
		Zeitgeist_Rendition* rendition
	)
{
	Zeitgeist_Rendition_ensureLibraryLoaded(state, rendition);
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
	Zeitgeist_Rendition_ensureLibraryLoaded(state, rendition);
	Zeitgeist_JumpTarget jumpTarget;
	Zeitgeist_State_pushJumpTarget(state, &jumpTarget);
	if (!setjmp(jumpTarget.environment)) {
		Zeitgeist_ForeignProcedure* f = (void (*)(Zeitgeist_State*))getRenditionLibrarySymbol(rendition->libraryHandle, "Zeitgeist_Rendition_update");
		if (!f) {
			fprintf(stderr, "unable to link `%s` of `%.*s`\n", "Zeitgeist_Rendition_update", (int)rendition->folderPath->numberOfBytes, rendition->folderPath->bytes);
			longjmp(state->jumpTarget->environment, -1);
		}
		Zeitgeist_State_popJumpTarget(state);
		return f;
	} else {
		Zeitgeist_State_popJumpTarget(state);
		longjmp(state->jumpTarget->environment, -1);
	}
}

Zeitgeist_ForeignProcedure*
Zeitgeist_Rendition_getLoad
	(
		Zeitgeist_State* state,
		Zeitgeist_Rendition* rendition
	)
{
	Zeitgeist_Rendition_ensureLibraryLoaded(state, rendition);
	Zeitgeist_JumpTarget jumpTarget;
	Zeitgeist_State_pushJumpTarget(state, &jumpTarget);
	if (!setjmp(jumpTarget.environment)) {
		Zeitgeist_ForeignProcedure* f = (void (*)(Zeitgeist_State*))getRenditionLibrarySymbol(rendition->libraryHandle, "Zeitgeist_Rendition_load");
		if (!f) {
			fprintf(stderr, "unable to link `%s` of `%.*s`\n", "Zeitgeist_Rendition_load", (int)rendition->folderPath->numberOfBytes, rendition->folderPath->bytes);
			longjmp(state->jumpTarget->environment, -1);
		}
		Zeitgeist_State_popJumpTarget(state);
		return f;
	} else {
		Zeitgeist_State_popJumpTarget(state);
		longjmp(state->jumpTarget->environment, -1);
	}
}

Zeitgeist_ForeignProcedure*
Zeitgeist_Rendition_getUnload
	(
		Zeitgeist_State* state,
		Zeitgeist_Rendition* rendition
	)
{
	Zeitgeist_Rendition_ensureLibraryLoaded(state, rendition);
	Zeitgeist_JumpTarget jumpTarget;
	Zeitgeist_State_pushJumpTarget(state, &jumpTarget);
	if (!setjmp(jumpTarget.environment)) {
		Zeitgeist_ForeignProcedure* f = (void (*)(Zeitgeist_State*))getRenditionLibrarySymbol(rendition->libraryHandle, "Zeitgeist_Rendition_unload");
		if (!f) {
			fprintf(stderr, "unable to link `%s` of `%.*s`\n", "Zeitgeist_Rendition_unload", (int)rendition->folderPath->numberOfBytes, rendition->folderPath->bytes);
			longjmp(state->jumpTarget->environment, -1);
		}
		Zeitgeist_State_popJumpTarget(state);
		return f;
	} else {
		Zeitgeist_State_popJumpTarget(state);
		longjmp(state->jumpTarget->environment, -1);
	}
}
