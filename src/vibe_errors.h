#ifndef VIBE_ERRORS_H
#define VIBE_ERRORS_H

#include <vibe_config.h>
#include <vibe_types.h>

#include <limits.h>
#include <stdio.h>

// =============================================================================
// == TYPES
// =============================================================================

typedef enum
{
  VIBE_ERRNO_NONE,
  VIBE_ERRNO_ERRORCTX_FILL_PATH,
  VIBE_ERRNO_ERRORCTX_OPEN_FILE,
  VIBE_ERRNO_ARGS_UNKNOWN,
  VIBE_ERRNO_TERMINAL_READ_MODE,
  VIBE_ERRNO_TERMINAL_WRITE_MODE,
} VIBE_Errno;

typedef struct
{
  VIBE_Errno err_first;
  FILE*      err_log_file;
  char       err_log_path[PATH_MAX];
} VIBE_ErrorCtx;

// =============================================================================
// == CONSTANTS
// =============================================================================

#define VIBE_ERROR_CTX_DEFAULT                                                 \
  ((VIBE_ErrorCtx){                                                            \
    .err_first    = VIBE_ERRNO_NONE,                                           \
    .err_log_file = NULL,                                                      \
    .err_log_path = { 0 },                                                     \
  })

// =============================================================================
// == LIFE CYCLE
// =============================================================================

void
VIBE_ErrorCtx_Acquire(VIBE_ErrorCtx ctx[static 1]);
void
VIBE_ErrorCtx_Release(VIBE_ErrorCtx ctx[static 1]);

// =============================================================================
// == API
// =============================================================================

#define VIBE_Error(Ctx, ...)                                                   \
  VIBE_ErrorCtx_ErrorInFunc(Ctx, __func__, __VA_ARGS__)
#define VIBE_Warning(Ctx, ...)                                                 \
  VIBE_ErrorCtx_WarningInFunc(Ctx, __func__, __VA_ARGS__)

void
VIBE_ErrorCtx_ErrorInFunc(VIBE_ErrorCtx ctx[static 1],
                          const char*   func,
                          VIBE_Errno    err,
                          ...);
void
VIBE_ErrorCtx_WarningInFunc(VIBE_ErrorCtx ctx[static 1],
                            const char*   func,
                            VIBE_Errno    err,
                            ...);

// =============================================================================
// == CRASHES
// =============================================================================

void
VIBE_OnCrash(void (*handler)(void));

// =============================================================================
// == GETTERS
// =============================================================================

int
VIBE_ErrorCtx_GetExitCode(const VIBE_ErrorCtx ctx[static 1]);
VIBE_Bool
VIBE_ErrorCtx_HasError(const VIBE_ErrorCtx ctx[static 1]);

#endif // VIBE_ERRORS_H
