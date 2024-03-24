#ifndef VIBE_DEFAULTS_H
#define VIBE_DEFAULTS_H

#include <vibe_custom.h>

#include <limits.h>

// =============================================================================
// == PATHS CONFIGS
// =============================================================================

#ifndef VIBE_PATH_MAX
#ifdef PATH_MAX
#define VIBE_PATH_MAX PATH_MAX
#else
#define VIBE_PATH_MAX 256
#endif
#endif

#ifndef VIBE_ERROR_LOG_PATH
#define VIBE_ERROR_LOG_PATH "./vibe-err.log"
#endif

#endif // VIBE_DEFAULTS_H
