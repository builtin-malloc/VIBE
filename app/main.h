#ifndef VIBE_MAIN_H
#define VIBE_MAIN_H

#include "./vibe_args.h"

#include <vibe_errors.h>

int
VIBE_Main(const VIBE_Args args[static 1], VIBE_ErrorCtx errs[static 1]);
void
VIBE_Main_Editor(const VIBE_Args args[static 1], VIBE_ErrorCtx errs[static 1]);
void
VIBE_Main_Help(void);
void
VIBE_Main_Version(void);

#endif // VIBE_MAIN_H
