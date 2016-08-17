/*
 * Description: husk of the old ratpoison.h file.
 */

#ifndef _RATPOISON_H
#define _RATPOISON_H 1

#define XINERAMA 1

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xlocale.h>
#include <fcntl.h>

/* Helper macro for error and debug reporting. */
#define PRINT_LINE(type) printf (PACKAGE ":%s:%d: %s: ",__FILE__,  __LINE__, #type)

/* Error and debug reporting macros. */
#define PRINT_ERROR(fmt)                        \
do {                                            \
  PRINT_LINE (error);                           \
  printf fmt;                                   \
  fflush (stdout);                              \
} while (0)

#ifdef DEBUG
#define PRINT_DEBUG(fmt)                        \
do {                                            \
  PRINT_LINE (debug);                           \
  printf fmt;                                   \
  fflush (stdout);                              \
} while (0)
#else
#define PRINT_DEBUG(fmt) do {} while (0)
#endif /* DEBUG */

#include "data.h"
#include "globals.h"
#include "manage.h"
#include "window.h"
#include "bar.h"
#include "events.h"
#include "number.h"
#include "input.h"
#include "messages.h"
#include "communications.h"
#include "sbuf.h"
#include "split.h"
#include "frame.h"
#include "screen.h"
#include "group.h"
#include "editor.h"
#include "history.h"
#include "completions.h"
#include "hook.h"
#include "xinerama.h"
#include "format.h"

#endif /* ! _RATPOISON_H */
