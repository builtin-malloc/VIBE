#include "./vibe_args.h"

#include <assert.h>
#include <string.h>

// =============================================================================
// == LIFE CYCLE
// =============================================================================

void
VIBE_Args_Acquire(VIBE_Args args[static 1])
{
  (void)args; // Nothing to do for now...
}

void
VIBE_Args_Release(VIBE_Args args[static 1])
{
  (void)args; // Nothing to do for now...
}

// =============================================================================
// == PARSING
// =============================================================================

void
VIBE_Args_Parse(VIBE_Args args[static 1], int argc, char* argv[])
{
  assert(argc > 0);
  assert(argv);

  for (int i = 0; i < argc; ++i) {
    const char* const arg = argv[i];

    if (VIBE_IsLongArg(arg))
      VIBE_Args_ParseLong(args, arg);
    if (VIBE_IsShortArg(arg))
      VIBE_Args_ParseShort(args, arg);
    if (VIBE_IsFileArg(arg))
      VIBE_Args_ParseFile(args, arg);
  }
}

void
VIBE_Args_ParseLong(VIBE_Args args[static 1], const char* arg)
{
  if (strcmp(arg, VIBE_ARGS_LONG_HELP) == 0) {
    VIBE_Args_SetApp(args, VIBE_APP_HELP);
  } else if (strcmp(arg, VIBE_ARGS_LONG_VERSION) == 0) {
    VIBE_Args_SetApp(args, VIBE_APP_VERSION);
  } else {
    // TODO(daniel): Print warning
    VIBE_Args_SetApp(args, VIBE_APP_HELP);
  }
}

void
VIBE_Args_ParseShort(VIBE_Args args[static 1], const char* arg)
{
  if (strcmp(arg, VIBE_ARGS_SHORT_HELP) == 0) {
    VIBE_Args_SetApp(args, VIBE_APP_HELP);
  } else if (strcmp(arg, VIBE_ARGS_SHORT_VERSION) == 0) {
    VIBE_Args_SetApp(args, VIBE_APP_VERSION);
  } else {
    // TODO(daniel): Print warning
    VIBE_Args_SetApp(args, VIBE_APP_HELP);
  }
}

void
VIBE_Args_ParseFile(VIBE_Args args[static 1], const char* arg)
{
  // TODO(daniel): Implement this
  (void)args;
  (void)arg;
}

VIBE_Bool
VIBE_IsLongArg(const char* arg)
{
  assert(arg);
  return arg[0] == '-' && arg[1] == '-' && arg[2] != '-' && arg[2] != '\0';
}

VIBE_Bool
VIBE_IsShortArg(const char* arg)
{
  assert(arg);
  return arg[0] == '-' && arg[1] != '-' && arg[1] != '\0';
}

VIBE_Bool
VIBE_IsFileArg(const char* arg)
{
  assert(arg);
  return !VIBE_IsLongArg(arg) && !VIBE_IsShortArg(arg);
}

// =============================================================================
// == GETTERS
// =============================================================================

VIBE_App
VIBE_Args_GetApp(const VIBE_Args args[static 1])
{
  return args->arg_app;
}

// =============================================================================
// == SETTERS
// =============================================================================

void
VIBE_Args_SetApp(VIBE_Args args[static 1], VIBE_App app)
{
  if (args->arg_app < app) {
    args->arg_app = app;
  }
}
