#ifndef VIBE_TERMINAL_H
#define VIBE_TERMINAL_H

#include <vibe_config.h>
#include <vibe_errors.h>
#include <vibe_types.h>

#include <termios.h>
#include <unistd.h>

// =============================================================================
// == TYPES
// =============================================================================

typedef struct
{
  struct termios tty_orig;
  VIBE_Bool      tty_has_orig;
  int            tty_input;
  int            tty_output;
} VIBE_Terminal;

// =============================================================================
// == LIFE CYCLE
// =============================================================================

#define VIBE_TERMINAL_DEFAULT                                                  \
  ((VIBE_Terminal){                                                            \
    .tty_has_orig = false,                                                     \
    .tty_input    = STDIN_FILENO,                                              \
    .tty_output   = STDOUT_FILENO,                                             \
  })

// =============================================================================
// == CONSTANTS
// =============================================================================

void
VIBE_Terminal_Acquire(VIBE_Terminal terminal[static 1],
                      VIBE_ErrorCtx errs[static 1]);
void
VIBE_Terminal_Release(VIBE_Terminal terminal[static 1],
                      VIBE_ErrorCtx errs[static 1]);

// =============================================================================
// == MODES
// =============================================================================

void
VIBE_Terminal_SaveOriginalMode(VIBE_Terminal terminal[static 1],
                               VIBE_ErrorCtx errs[static 1]);
void
VIBE_Terminal_EnterRawMode(VIBE_Terminal terminal[static 1],
                           VIBE_ErrorCtx errs[static 1]);
void
VIBE_Terminal_EnterOriginalMode(VIBE_Terminal terminal[static 1],
                                VIBE_ErrorCtx errs[static 1]);

VIBE_Bool
VIBE_GetTermios(struct termios* mode, int fd);
VIBE_Bool
VIBE_SetTermios(struct termios* mode, int fd);

#endif // VIBE_TERMINAL_H
