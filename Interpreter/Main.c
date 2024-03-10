#include "Zeitgeist.h"

#include <stdlib.h>
#include <stdio.h>
#include "Zeitgeist/Rendition.h"

#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#include <strsafe.h>
	#include <tchar.h>
#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	#include <sys/types.h>
	#include <dirent.h>
	// strcmp
	#include <string.h>
#else
	#error("operating system not (yet) supported")
#endif

static Zeitgeist_List*
loadRenditions
	(
		Zeitgeist_State* state
	)
{
	Zeitgeist_List* renditions = Zeitgeist_createList(state);
#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
	WIN32_FIND_DATA ffd;
	TCHAR szDir[MAX_PATH];
	StringCchCopy(szDir, MAX_PATH, TEXT("." RENDITION_DIRECTORY_SEPARATOR "Renditions"));
	StringCchCat(szDir, MAX_PATH, TEXT(RENDITION_DIRECTORY_SEPARATOR "*"));
	HANDLE hFind = INVALID_HANDLE_VALUE;
	hFind = FindFirstFile(szDir, &ffd);
	if (INVALID_HANDLE_VALUE == hFind) {
		longjmp(state->jumpTarget->environment, -1);
	}
	Zeitgeist_JumpTarget jumpTarget;
	Zeitgeist_State_pushJumpTarget(state, &jumpTarget);
	if (!setjmp(jumpTarget.environment)) {
		Zeitgeist_String* prefix = Zeitgeist_State_createString(state, "." RENDITION_DIRECTORY_SEPARATOR "Renditions" RENDITION_DIRECTORY_SEPARATOR,
																														strlen("." RENDITION_DIRECTORY_SEPARATOR "Renditions" RENDITION_DIRECTORY_SEPARATOR));
		do {
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if (!strcmp(ffd.cFileName, ".") || !strcmp(ffd.cFileName, "..")) {
					continue;
				}
				Zeitgeist_String* suffix = Zeitgeist_State_createString(state, ffd.cFileName, strlen(ffd.cFileName));
				Zeitgeist_String* path = Zeitgeist_String_concatenate(state, prefix, suffix);
				Zeitgeist_Rendition* rendition = Zeitgeist_createRendition(state, path);
				Zeitgeist_List_appendObject(state, renditions, (Zeitgeist_Object*)rendition);
			}
		} while (FindNextFile(hFind, &ffd) != 0);
		Zeitgeist_State_popJumpTarget(state);
		FindClose(hFind);
		hFind = INVALID_HANDLE_VALUE;
	} else {
		Zeitgeist_State_popJumpTarget(state);
		FindClose(hFind);
		hFind = INVALID_HANDLE_VALUE;
		longjmp(state->jumpTarget->environment, -1);
	}
#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	DIR* dir;
	struct dirent* ent;
	if ((dir = opendir("." RENDITION_DIRECTORY_SEPARATOR "Renditions")) != NULL) {
		Zeitgeist_JumpTarget jumpTarget;
		Zeitgeist_State_pushJumpTarget(state, &jumpTarget);
		if (!setjmp(jumpTarget.environment)) {
			Zeitgeist_String* prefix = Zeitgeist_State_createString(state, "." RENDITION_DIRECTORY_SEPARATOR "Renditions" RENDITION_DIRECTORY_SEPARATOR,
																															strlen("." RENDITION_DIRECTORY_SEPARATOR "Renditions" RENDITION_DIRECTORY_SEPARATOR));
			while ((ent = readdir(dir)) != NULL) {
				fprintf(stdout, "%s (", ent->d_name);
				switch (ent->d_type) {
					case DT_BLK:
						fprintf(stdout, "DB_BLK");
						break;
					case DT_CHR:
						fprintf(stdout, "DT_CHR");
						break;
					case DT_DIR:
						fprintf(stdout, "DT_DIR");
						break;
					case DT_FIFO:
						fprintf(stdout, "DT_FIFO");
						break;
					case DT_LNK:
						fprintf(stdout, "DT_LNK");
						break;
					case DT_REG:
						fprintf(stdout, "DT_REG");
						break;
					case DT_SOCK:
						fprintf(stdout, "DT_SOCK");
						break;
					case DT_UNKNOWN:
						fprintf(stdout, "DT_UNKNOWN");
						break;
					default:
						fprintf(stdout, "<unknown>");
						break;
				};
				fprintf(stdout, ")\n");
				if (ent->d_type == DT_DIR && (strcmp(ent->d_name, ".") && strcmp(ent->d_name, ".."))) {
					Zeitgeist_String* suffix = Zeitgeist_State_createString(state, ent->d_name, strlen(ent->d_name));
					Zeitgeist_String* path = Zeitgeist_String_concatenate(state, prefix, suffix);
					Zeitgeist_Rendition* rendition = Zeitgeist_createRendition(state, path);
					Zeitgeist_List_appendObject(state, renditions, (Zeitgeist_Object*)rendition);
				}
			}
			Zeitgeist_State_popJumpTarget(state);
			closedir(dir);
			dir = NULL;
		} else {
			Zeitgeist_State_popJumpTarget(state);
			closedir(dir);
			dir = NULL;
			longjmp(state->jumpTarget->environment, -1);
		}
	} else {
		state->lastError = 1;
		longjmp(state->jumpTarget->environment, -1);
	}
#else
	#error("operating system not (yet) supported")
#endif
	return renditions;
}

// command implementation: "--list-renditions"
static void
onListRenditions
	(
		Zeitgeist_State* state
	)
{
	Zeitgeist_List* renditions = loadRenditions(state);
	Zeitgeist_Value size = Zeitgeist_List_getSize(state, renditions);
	for (size_t i = 0, n = Zeitgeist_Value_getInteger(&size); i < n; ++i) {
		Zeitgeist_Value value = Zeitgeist_List_getValue(state, renditions, i);
		if (!Zeitgeist_Value_hasObject(&value)) {
			state->lastError = 1;
			longjmp(state->jumpTarget->environment, -1);
		}
		Zeitgeist_Rendition* rendition = (Zeitgeist_Rendition*)Zeitgeist_Value_getObject(&value);
		Zeitgeist_String* name = Zeitgeist_Rendition_getName(state, rendition);
		fprintf(stdout, "%zu) `%.*s`\n", i + 1, (int)name->numberOfBytes, name->bytes);
	}
}

static void
onRendition1
	(
		Zeitgeist_State* state,
		Zeitgeist_Rendition* rendition
	)
{ 
	Zeitgeist_String* renditionName = Zeitgeist_Rendition_getName(state, rendition);
	Zeitgeist_ForeignProcedure* loadFunction = Zeitgeist_Rendition_getLoad(state, rendition);
	if (!loadFunction) {
		fprintf(stderr, "unable to acquire unload function of rendition `%.*s`\n", (int)renditionName->numberOfBytes, renditionName->bytes);
		longjmp(state->jumpTarget->environment, -1);
	}
	Zeitgeist_ForeignProcedure* unloadFunction = Zeitgeist_Rendition_getUnload(state, rendition);
	if (!unloadFunction) {
		fprintf(stderr, "unable to acquire unload function of rendition `%.*s`\n", (int)renditionName->numberOfBytes, renditionName->bytes);
		longjmp(state->jumpTarget->environment, -1);
	}
	Zeitgeist_ForeignProcedure* updateFunction = Zeitgeist_Rendition_getUpdate(state, rendition);
	if (!updateFunction) {
		fprintf(stderr, "unable to acquire update function of rendition `%.*s`\n", (int)renditionName->numberOfBytes, renditionName->bytes);
		longjmp(state->jumpTarget->environment, -1);
	}
	(*loadFunction)(state);
	Zeitgeist_JumpTarget jumpTarget1;
	Zeitgeist_State_pushJumpTarget(state, &jumpTarget1);
	if (!setjmp(jumpTarget1.environment)) {
		while (!Zeitgeist_State_isExitProcessRequested(state)) {
			(*updateFunction)(state);
		}
		Zeitgeist_State_popJumpTarget(state);
		(*unloadFunction)(state);
	} else {
		Zeitgeist_State_popJumpTarget(state);
		(*unloadFunction)(state);
		longjmp(state->jumpTarget->environment, -1);
	}
}

// command implementation: "--rendition <name>"
static void
onRendition
	(
		Zeitgeist_State* state,
		Zeitgeist_String* renditionName
	)
{ 
	Zeitgeist_List* loadedRenditions = loadRenditions(state);
	Zeitgeist_Value size = Zeitgeist_List_getSize(state, loadedRenditions);
	for (size_t i = 0, n = Zeitgeist_Value_getInteger(&size); i < n; ++i) {
		Zeitgeist_Value value = Zeitgeist_List_getValue(state, loadedRenditions, i);
		if (!Zeitgeist_Value_hasObject(&value)) {
			state->lastError = 1;
			longjmp(state->jumpTarget->environment, -1);
		}
		Zeitgeist_Rendition* loadedRendition = (Zeitgeist_Rendition*)Zeitgeist_Value_getObject(&value);
		Zeitgeist_String* loadedRenditionName = Zeitgeist_Rendition_getName(state, loadedRendition);
		if (Zeitgeist_String_areEqual(state, renditionName, loadedRenditionName)) {
			onRendition1(state, loadedRendition);
		}
	}
}

static void
main1
	(
		Zeitgeist_State* state,
		int argc,
		char** argv
	)
{
	Zeitgeist_String* listRenditions = Zeitgeist_State_createString(state, "--list-renditions", strlen("--list-renditions"));
	Zeitgeist_String* rendition = Zeitgeist_State_createString(state, "--rendition", strlen("--rendition"));
	Zeitgeist_String* help = Zeitgeist_State_createString(state, "--help", strlen("--help"));
	if (argc < 2) {
		fprintf(stderr, "error: no command specified\n");
		longjmp(state->jumpTarget->environment, -1);
	}
	for (int argi = 1; argi < argc; ++argi) {
		Zeitgeist_String* arg = Zeitgeist_State_createString(state, argv[argi], strlen(argv[argi]));
		if (arg->numberOfBytes > 128) {
			fprintf(stderr, "error: command `%.*s` too long\n", (int)64, arg->bytes);
			longjmp(state->jumpTarget->environment, -1);
		}
		if (Zeitgeist_String_areEqual(state, arg, listRenditions)) {
			if (argc != 2) {
				fprintf(stderr, "error: unknown arguments to command `%.*s`\n", (int)arg->numberOfBytes, arg->bytes);
				longjmp(state->jumpTarget->environment, -1);
			}
			fprintf(stdout, "listing renditions\n");
			onListRenditions(state);
			break;
		} else if (Zeitgeist_String_areEqual(state, arg, rendition)) {
			if (argc != 3) {
				fprintf(stderr, "error: missing argument for command `%.*s`\n", (int)arg->numberOfBytes, arg->bytes);
				longjmp(state->jumpTarget->environment, -1);
			}
			fprintf(stdout, "executing rendition\n");
			onRendition(state, Zeitgeist_State_createString(state, argv[argi + 1], strlen(argv[argi + 1])));
			break;
		} else if (Zeitgeist_String_areEqual(state, arg, help)) {
			state->exitProcessRequested = Zeitgeist_Boolean_True;
		} else {
			fprintf(stderr, "error: unknown command `%.*s`\n", (int)arg->numberOfBytes, arg->bytes);
			longjmp(state->jumpTarget->environment, -1);
		}
	}
}

int
main
	(
		int argc,
		char** argv
	)
{
	Zeitgeist_State* state = Zeitgeist_createState();
	if (!state) {
		return EXIT_FAILURE;
	}
	int exitCode = EXIT_SUCCESS;
	Zeitgeist_JumpTarget jumpTarget;
	Zeitgeist_State_pushJumpTarget(state, &jumpTarget);
	if (!setjmp(jumpTarget.environment)) {
		main1(state, argc, argv);
		exitCode = EXIT_SUCCESS;
		Zeitgeist_State_popJumpTarget(state);
	} else {
		exitCode = EXIT_FAILURE;
		Zeitgeist_State_popJumpTarget(state);
	}
	Zeitgeist_State_destroy(state);
	state = NULL;
	return exitCode;
}
