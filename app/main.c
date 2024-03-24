#include "./main.h"

#include <stdio.h>

// =============================================================================
// == CONSTANTS
// =============================================================================

static const char VIBE_HELP_MESSAGE[] =
  "vibe - Vi(m) Is Bloat Editor\nTODO: Help";

static const char VIBE_VERSION_MESSAGE[] = "vibe v" VIBE_VERSION;

// =============================================================================
// == GLOBAL DATA
// =============================================================================

static VIBE_Args args = VIBE_ARGS_DEFAULT;

// =============================================================================
// == MAIN
// =============================================================================

int
main(int argc, char* argv[])
{
  VIBE_Args_Acquire(&args);
  VIBE_Args_Parse(&args, argc, argv);

  VIBE_Main(&args);

  VIBE_Args_Release(&args);
}

void
VIBE_Main(const VIBE_Args args[static 1])
{
  switch (VIBE_Args_GetApp(args)) {
    case VIBE_APP_EDITOR:
      VIBE_Main_Editor(args);
      break;
    case VIBE_APP_VERSION:
      VIBE_Main_Version();
      break;
    case VIBE_APP_HELP:
      VIBE_Main_Help();
      break;
  }
}

void
VIBE_Main_Editor(const VIBE_Args args[static 1])
{
  (void)args; // TODO(daniel) Implement this
}

void
VIBE_Main_Help(void)
{
  printf("%s\n", VIBE_HELP_MESSAGE);
}

void
VIBE_Main_Version(void)
{
  printf("%s\n", VIBE_VERSION_MESSAGE);
}
