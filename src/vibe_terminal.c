#include <vibe_terminal.h>

#include <errno.h>
#include <string.h>

// =============================================================================
// == CONSTANTS
// =============================================================================

void
VIBE_Terminal_Acquire(VIBE_Terminal terminal[static 1],
                      VIBE_ErrorCtx errs[static 1])
{
  VIBE_Terminal_SaveOriginalMode(terminal, errs);
}

void
VIBE_Terminal_Release(VIBE_Terminal terminal[static 1],
                      VIBE_ErrorCtx errs[static 1])
{
  VIBE_Terminal_EnterOriginalMode(terminal, errs);
}

// =============================================================================
// == MODES
// =============================================================================

void
VIBE_Terminal_SaveOriginalMode(VIBE_Terminal terminal[static 1],
                               VIBE_ErrorCtx errs[static 1])
{
  if (!VIBE_GetTermios(&terminal->tty_orig, terminal->tty_input)) {
    VIBE_Error(errs, VIBE_ERRNO_TERMINAL_READ_MODE, strerror(errno));
    return;
  }

  terminal->tty_has_orig = true;
}

void
VIBE_Terminal_EnterRawMode(VIBE_Terminal terminal[static 1],
                           VIBE_ErrorCtx errs[static 1])
{
  if (!terminal->tty_has_orig)
    return;

  struct termios raw  = terminal->tty_orig;
  raw.c_iflag        &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag        &= ~(OPOST);
  raw.c_cflag        |= (CS8);
  raw.c_lflag        &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN]      = 0;
  raw.c_cc[VTIME]     = 1;

  if (!VIBE_SetTermios(&raw, terminal->tty_input)) {
    VIBE_Error(errs, VIBE_ERRNO_TERMINAL_WRITE_MODE, strerror(errno));
  }
}

void
VIBE_Terminal_EnterOriginalMode(VIBE_Terminal terminal[static 1],
                                VIBE_ErrorCtx errs[static 1])
{
  if (!terminal->tty_has_orig)
    return;

  if (!VIBE_SetTermios(&terminal->tty_orig, terminal->tty_input)) {
    VIBE_Error(errs, VIBE_ERRNO_TERMINAL_WRITE_MODE, strerror(errno));
  }
}

VIBE_Bool
VIBE_GetTermios(struct termios* mode, int fd)
{
  return tcgetattr(fd, mode) != -1;
}

VIBE_Bool
VIBE_SetTermios(struct termios* mode, int fd)
{
  return tcsetattr(fd, TCSAFLUSH, mode) != -1;
}
