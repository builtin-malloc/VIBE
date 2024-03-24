#ifndef VIBE_ARGS_H
#define VIBE_ARGS_H

#include <vibe_types.h>

// =============================================================================
// == TYPES
// =============================================================================

typedef enum
{
  VIBE_APP_EDITOR,
  VIBE_APP_VERSION,
  VIBE_APP_HELP,
} VIBE_App;

typedef struct
{
  VIBE_App arg_app;
} VIBE_Args;

// =============================================================================
// == CONSTANTS
// =============================================================================

#define VIBE_ARGS_LONG_VERSION  "--version"
#define VIBE_ARGS_SHORT_VERSION "-v"
#define VIBE_ARGS_LONG_HELP     "--help"
#define VIBE_ARGS_SHORT_HELP    "-h"

#define VIBE_APP_DEFAULT VIBE_APP_EDITOR

#define VIBE_ARGS_DEFAULT                                                      \
  ((VIBE_Args){                                                                \
    .arg_app = VIBE_APP_DEFAULT,                                               \
  })

// =============================================================================
// == LIFE CYCLE
// =============================================================================

void
VIBE_Args_Acquire(VIBE_Args args[static 1]);
void
VIBE_Args_Release(VIBE_Args args[static 1]);

// =============================================================================
// == PARSING
// =============================================================================

void
VIBE_Args_Parse(VIBE_Args args[static 1], int argc, char* argv[]);
void
VIBE_Args_ParseLong(VIBE_Args args[static 1], const char* arg);
void
VIBE_Args_ParseShort(VIBE_Args args[static 1], const char* arg);
void
VIBE_Args_ParseFile(VIBE_Args args[static 1], const char* arg);

VIBE_Bool
VIBE_IsLongArg(const char* arg);
VIBE_Bool
VIBE_IsShortArg(const char* arg);
VIBE_Bool
VIBE_IsFileArg(const char* arg);

// =============================================================================
// == GETTERS
// =============================================================================

VIBE_App
VIBE_Args_GetApp(const VIBE_Args args[static 1]);

// =============================================================================
// == SETTERS
// =============================================================================

void
VIBE_Args_SetApp(VIBE_Args args[static 1], VIBE_App app);

#endif // VIBE_ARGS_H
