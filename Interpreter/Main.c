#include "Zeitgeist.h"

#include <stdlib.h>
#include <stdio.h>
#include "Zeitgeist/Rendition.h"

#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>
  #include <strsafe.h>
  #include <tchar.h>
#else Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
  #error("environment not (yet) supported")
#endif

static Zeitgeist_ArrayList*
loadRenditions
  (
    Zeitgeist_State* state
  )
{
  Zeitgeist_ArrayList* renditions = Zeitgeist_createArrayList(state);
#if defined (_WIN32)
  WIN32_FIND_DATA ffd;
  TCHAR szDir[MAX_PATH];
  StringCchCopy(szDir, MAX_PATH, TEXT(".\\Renditions"));
  StringCchCat(szDir, MAX_PATH, TEXT("\\*"));
  HANDLE hFind = INVALID_HANDLE_VALUE;
  hFind = FindFirstFile(szDir, &ffd);
  if (INVALID_HANDLE_VALUE == hFind) {
    longjmp(state->jumpTarget->environment, -1);
  }
  Zeitgeist_JumpTarget jumpTarget;
  Zeitgeist_State_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    do {
      if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        if (!strcmp(ffd.cFileName, ".") || !strcmp(ffd.cFileName, "..")) {
          continue;
        }
        Zeitgeist_String* prefix = Zeitgeist_State_createString(state, ".\\Renditions\\", strlen(".\\Renditions\\"));
        Zeitgeist_String* suffix = Zeitgeist_State_createString(state, ffd.cFileName, strlen(ffd.cFileName));
        Zeitgeist_String* path = Zeitgeist_String_concatenate(state, prefix, suffix);
        Zeitgeist_Rendition* rendition = Zeitgeist_createRendition(state, path);
        Zeitgeist_ArrayList_appendObject(state, renditions, (Zeitgeist_Object*)rendition);
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
#else
  DIR* dir;
  struct dirent* ent;
  if ((dir = opendir(".\\Renditions")) != NULL) {
    Zeitgeist_JumpTarget jumpTarget;
    Zeitgeist_State_pushJumpTarget(state, &jumpTarget);
    if (!setjmp(jumpTarget.environment)) {
      while ((ent = readdir(dir)) != NULL) {
        if (dirent->d_type == DT_DIR && (!strcmp(dirent->d_name, ".") && !strcmp(dirent->d_name, ".."))) {
          Zeitgeist_String* prefix = Zeitgeist_State_createString(state, ".\\Renditions\\", strlen(".\\Renditions\\"));
          Zeitgeist_String* suffix = Zeitgeist_State_createString(state, ffd.cFileName, strlen(ffd.cFileName));
          Zeitgeist_String* path = Zeitgeist_String_concatenate(state, prefix, suffix);
          Zeitgeist_Rendition* rendition = Zeitgeist_createRendition(state, path);
          Zeitgeist_ArrayList_appendObject(state, renditions, (Zeitgeist_Object*)rendition);
        }
      }
      Zeitgeist_State_popJumpTarget(state);
      closedir(dir);
    } else {
      Zeitgeist_State_popJumpTarget(state);
      closedir(dir);
      longjmp(state->jumpTarget->environment, -1);
    }
    closedir(dir);
  } else {
    state->lastError = 1;
    longjmp(state->jumpTarget->environment, -1);
  }
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
  Zeitgeist_ArrayList* renditions = loadRenditions(state);
  Zeitgeist_Value size = Zeitgeist_ArrayList_getSize(state, renditions);
  for (size_t i = 0, n = Zeitgeist_Value_getInteger(&size); i < n; ++i) {
    Zeitgeist_Value value = Zeitgeist_ArrayList_getValue(state, renditions, i);
    if (!Zeitgeist_Value_hasObject(&value)) {
      state->lastError = 1;
      longjmp(state->jumpTarget->environment, -1);
    }
    Zeitgeist_Rendition* rendition = (Zeitgeist_Rendition*)Zeitgeist_Value_getObject(&value);
    Zeitgeist_String* name = Zeitgeist_Rendition_getName(state, rendition);
    fprintf(stdout, "%zu) `%.*s`\n", i + 1, (int)name->numberOfBytes, name->bytes);
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
  Zeitgeist_String* renditionFolder = Zeitgeist_State_createString(state,
                                                                   ".\\Renditions\\Zeitgeist-Hello-World",
                                                                   strlen(".\\Renditions\\Zeitgeist-Hello-World"));
  Zeitgeist_Rendition* foundRendition = Zeitgeist_createRendition(state, renditionFolder);
  Zeitgeist_JumpTarget jumpTarget;
  Zeitgeist_State_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Zeitgeist_String* foundRenditionName = Zeitgeist_Rendition_getName(state, foundRendition);
    if (!Zeitgeist_String_areEqual(state, foundRenditionName, renditionName)) {
      if (renditionName->numberOfBytes > INT_MAX) {
        longjmp(state->jumpTarget->environment, -1);
      }
      fprintf(stderr, "rendition `%.*s` not found\n", (int)renditionName->numberOfBytes, renditionName->bytes);
      longjmp(state->jumpTarget->environment, -1);
    }
    Zeitgeist_State_popJumpTarget(state);
  } else {
    Zeitgeist_State_popJumpTarget(state);
    longjmp(state->jumpTarget->environment, -1);
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
      onListRenditions(state);
      break;
    } else if (Zeitgeist_String_areEqual(state, arg, rendition)) {
      if (argc != 3) {
        fprintf(stderr, "error: missing argument for command `%.*s`\n", (int)arg->numberOfBytes, arg->bytes);
        longjmp(state->jumpTarget->environment, -1);
      }
      onRendition(state, Zeitgeist_State_createString(state, argv[argi+1], strlen(argv[argi+1])));
      break;
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
