#include <vibe_config.h>
#include <vibe_errors.h>

#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

// =============================================================================
// == CONSTANTS
// =============================================================================

#define VIBE_ERROR_CTX_BEGIN_FMT "[%s] "
#define VIBE_ERROR_CTX_END_FMT   " (in %s)\r\n"

// TODO(daniel): Use default OS log dir
static const char VIBE_ERROR_CTX_DEFAULT_LOG_FILE_PATH[] = VIBE_ERROR_LOG_PATH;

static const char VIBE_ERROR_CTX_ERROR_TAG[]   = "ERROR";
static const char VIBE_ERROR_CTX_WARNING_TAG[] = " WARN";

static const char* const VIBE_ERRORNO_FMTS[] = {
  [VIBE_ERRNO_NONE] = "<NO ERROR>: This is a bug and should never be printed",
  [VIBE_ERRNO_ERRORCTX_FILL_PATH] =
    "Failed to save error log file path \"%s\": %s",
  [VIBE_ERRNO_ERRORCTX_OPEN_FILE] = "Failed to open error log file \"%s\": %s",
  [VIBE_ERRNO_ARGS_UNKNOWN]       = "Unknown argument: %s",
  [VIBE_ERRNO_TERMINAL_READ_MODE] =
    "Failed to read %zu bytes from the terminal: %s",
  [VIBE_ERRNO_TERMINAL_WRITE_MODE] =
    "Failed to write %zu bytes from the terminal: %s",
};

// =============================================================================
// == IO HELPERS
// =============================================================================

static void
VIBE_ErrorCtx_FillDefaultLogFilePath(VIBE_ErrorCtx ctx[static 1])
{
  const int bytes_written = snprintf(ctx->err_log_path,
                                     sizeof(ctx->err_log_path),
                                     "%s",
                                     VIBE_ERROR_CTX_DEFAULT_LOG_FILE_PATH);

  if (bytes_written < 0) {
    VIBE_Error(ctx,
               VIBE_ERRNO_ERRORCTX_FILL_PATH,
               VIBE_ERROR_CTX_DEFAULT_LOG_FILE_PATH,
               strerror(errno));
  }
}

static void
VIBE_ErrorCtx_CloseLogFile(VIBE_ErrorCtx ctx[static 1])
{
  if (ctx->err_log_file) {
    fclose(ctx->err_log_file);
  }
}

static void
VIBE_ErrorCtx_OpenLogFile(VIBE_ErrorCtx ctx[static 1])
{

  FILE* const file = fopen(ctx->err_log_path, "w");
  if (!file) {
    VIBE_Error(
      ctx, VIBE_ERRNO_ERRORCTX_OPEN_FILE, ctx->err_log_path, strerror(errno));
    return;
  }

  VIBE_ErrorCtx_CloseLogFile(ctx);
  ctx->err_log_file = file;
}

// =============================================================================
// == LIFE CYCLE
// =============================================================================

void
VIBE_ErrorCtx_Acquire(VIBE_ErrorCtx ctx[static 1])
{
  VIBE_ErrorCtx_FillDefaultLogFilePath(ctx);
  if (!VIBE_ErrorCtx_HasError(ctx))
    VIBE_ErrorCtx_OpenLogFile(ctx);
}

void
VIBE_ErrorCtx_Release(VIBE_ErrorCtx ctx[static 1])
{
  VIBE_ErrorCtx_CloseLogFile(ctx);
}

// =============================================================================
// == API
// =============================================================================

static void
VIBE_ErrorCtx_WriteErrorToFile(FILE*       file,
                               const char* func,
                               const char* tag,
                               const char* fmt,
                               va_list     args)
{
  if (!file)
    file = stderr;

  fprintf(file, VIBE_ERROR_CTX_BEGIN_FMT, tag);
  vfprintf(file, fmt, args);
  fprintf(file, VIBE_ERROR_CTX_END_FMT, func);
}

void
VIBE_ErrorCtx_ErrorInFunc(VIBE_ErrorCtx ctx[static 1],
                          const char*   func,
                          VIBE_Errno    err,
                          ...)
{
  assert(func);
  assert(err != VIBE_ERRNO_NONE);

  if (ctx->err_first == VIBE_ERRNO_NONE)
    ctx->err_first = err;

  const char* tag = VIBE_ERROR_CTX_ERROR_TAG;
  const char* fmt = VIBE_ERRORNO_FMTS[err];

  va_list args;
  va_start(args, err);
  VIBE_ErrorCtx_WriteErrorToFile(ctx->err_log_file, func, tag, fmt, args);
  va_end(args);
}

void
VIBE_ErrorCtx_WarningInFunc(VIBE_ErrorCtx ctx[static 1],
                            const char*   func,
                            VIBE_Errno    err,
                            ...)
{
  assert(func);
  assert(err != VIBE_ERRNO_NONE);

  const char* tag = VIBE_ERROR_CTX_WARNING_TAG;
  const char* fmt = VIBE_ERRORNO_FMTS[err];

  va_list args;
  va_start(args, err);
  VIBE_ErrorCtx_WriteErrorToFile(ctx->err_log_file, func, tag, fmt, args);
  va_end(args);
}

// =============================================================================
// == GETTERS
// =============================================================================

int
VIBE_ErrorCtx_GetExitCode(const VIBE_ErrorCtx ctx[static 1])
{
  return ctx->err_first;
}

VIBE_Bool
VIBE_ErrorCtx_HasError(const VIBE_ErrorCtx ctx[static 1])
{
  return ctx->err_first != VIBE_ERRNO_NONE;
}
