/*
 * Description: configuration file to control basic options.
 */ 

/* Define this to enable debugging messages */
/* #undef DEBUG */

/* Define to 1 if you have the `getline' function. */
#define HAVE_GETLINE 1

/* Define to 1 if you have the `getopt_long' function. */
#define HAVE_GETOPT_LONG 1

/* Define to 1 if you have the `getpwuid' function. */
#define HAVE_GETPWUID 1

/* Define this to enable history */
/* #undef HAVE_HISTORY */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define if you have <langinfo.h> and nl_langinfo(CODESET). */
#define HAVE_LANGINFO_CODESET 1

/* Define to 1 if you have the `efence' library (-lefence). */
/* #undef HAVE_LIBEFENCE */

/* Xinerama */
#define HAVE_LIBXINERAMA 1

/* Xtst */
#define HAVE_LIBXTST 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <pwd.h> header file. */
#define HAVE_PWD_H 1

/* Define to 1 if you have the <readline/history.h> header file. */
/* #undef HAVE_READLINE_HISTORY_H */

/* Define to 1 if you have the `setenv' function. */
#define HAVE_SETENV 1

/* Define to 1 if you have the `setpgid' function. */
#define HAVE_SETPGID 1

/* Define to 1 if you have the `setpgrp' function. */
#define HAVE_SETPGRP 1

/* Define to 1 if you have the `setsid' function. */
#define HAVE_SETSID 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/ioctl.h> header file. */
#define HAVE_SYS_IOCTL_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if typeof works with your compiler. */
#define HAVE_TYPEOF 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the `unsetenv' function. */
#define HAVE_UNSETENV 1

/* Define to 1 if you have the `vsnprintf' function. */
#define HAVE_VSNPRINTF 1

/* Define to 1 if you have the <X11/extensions/Xinerama.h> header file. */
#define HAVE_X11_EXTENSIONS_XINERAMA_H 1

/* Define to 1 if you have the <X11/XKBlib.h> header file. */
#define HAVE_X11_XKBLIB_H 1

/* Define to 1 if you have the `XkbKeycodeToKeysym' function. */
#define HAVE_XKBKEYCODETOKEYSYM 1

/* Define to 1 if your compiler supports the `__builtin_prefetch' function. */
#define HAVE___BUILTIN_PREFETCH 1

/* Name of package */
#define PACKAGE "ratpoison"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "ratpoison-devel@nongnu.org"

/* Define to the full name of this package. */
#define PACKAGE_NAME "ratpoison"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "ratpoison 1.4.9-beta"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "ratpoison"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
//#define PACKAGE_VERSION "1.4.9-beta"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* X terminal emulator to use */
#define TERM_PROG "xterm"

/* Define this to use Xft */
#define USE_XFT_FONT 1

/* Version number of package */
#define VERSION "1.4.9-beta"

/* Define to 1 if you want to build XKB support for keyboard input. */
#define WANT_XKB 1

/* Define to 1 if the X Window System is missing or not being used. */
/* #undef X_DISPLAY_MISSING */

/* Define to __typeof__ if your compiler spells it that way. */
/* #undef typeof */

#define SYSCONFDIR "/etc"

#include "data.h"
#include "actions.h"

#define KEY_PREFIX      XK_t
#define MODIFIER_PREFIX RP_CONTROL_MASK

/* This is the abort key when typing input. */
#define INPUT_ABORT_KEY      XK_g
#define INPUT_ABORT_MODIFIER RP_CONTROL_MASK

/* This is the previous history entry key when typing input. */
#define INPUT_PREV_HISTORY_KEY      XK_p
#define INPUT_PREV_HISTORY_MODIFIER RP_CONTROL_MASK

/* This is the next history entry key when typing input. */
#define INPUT_NEXT_HISTORY_KEY      XK_n
#define INPUT_NEXT_HISTORY_MODIFIER RP_CONTROL_MASK

/* Key used to enlarge frame vertically when in resize mode.  */
#define RESIZE_VGROW_KEY      XK_n
#define RESIZE_VGROW_MODIFIER RP_CONTROL_MASK

/* Key used to shrink frame vertically when in resize mode.  */
#define RESIZE_VSHRINK_KEY      XK_p
#define RESIZE_VSHRINK_MODIFIER RP_CONTROL_MASK

/* Key used to enlarge frame horizontally when in resize mode.  */
#define RESIZE_HGROW_KEY      XK_f
#define RESIZE_HGROW_MODIFIER RP_CONTROL_MASK

/* Key used to shrink frame horizontally when in resize mode.  */
#define RESIZE_HSHRINK_KEY      XK_b
#define RESIZE_HSHRINK_MODIFIER RP_CONTROL_MASK

/* Key used to shrink frame to fit it's current window.  */
#define RESIZE_SHRINK_TO_WINDOW_KEY             XK_s
#define RESIZE_SHRINK_TO_WINDOW_MODIFIER        0

/* Key used to exit resize mode.  */
#define RESIZE_END_KEY      XK_Return
#define RESIZE_END_MODIFIER 0

/* Number of history items to store. */
#define INPUT_MAX_HISTORY 50

/* Treat windows with maxsize hints as if they were a transient window
   (don't hide the windows underneath, and center them) */
#define MAXSIZE_WINDOWS_ARE_TRANSIENTS

/* Treat windows with aspect hints as if they were a transient window
   (don't hide the windows underneath, and center them) */
#define ASPECT_WINDOWS_ARE_TRANSIENTS

/* An alias command could recursively call inself infinitely. This
   stops that behavior. */
#define MAX_ALIAS_RECURSIVE_DEPTH 16

/* Pressing a key sends the mouse to the bottom right corner. This
   doesn't work very well yet. */
/* #define HIDE_MOUSE  */

/* When the last window closes, quit ratpoison. */
/* #define AUTO_CLOSE */

/* If for some sick reason you don't want ratpoison to manage a
   window, put its name in this list. These windows get drawn but
   ratpoison won't have any knowledge of them and you won't be able to
   jump to them or give them keyboard focus. This has been added
   mostly for use with hand-helds. */
#define UNMANAGED_WINDOW_LIST "xapm","xclock","xscribble"

/* Maximum depth of a link. Used in the 'link' command. */
#define MAX_LINK_DEPTH 16

/* Bad window messages can be safely ignored now that ratpoison has
   become stable enough. Comment this line if you wish to be notified
   about bad window messages. */
#define IGNORE_BADWINDOW 1

/* This is the name of the first group that is created. */
#define DEFAULT_GROUP_NAME "default"

/* Maximum allowed history size */
#define MAX_HISTORY_SIZE 100

/* The default filename in which to store the history */
#define HISTORY_FILE ".ratpoison_history"

/* Use a visual bell in the input window */
#define VISUAL_BELL 1

/* The name of the root keymap */
#define ROOT_KEYMAP "root"

/* The name of the top level keymap */
#define TOP_KEYMAP "top"

/* The default font */
#define DEFAULT_FONT "-*-fixed-bold-r-normal-*-15-*-*-*-c-*-*-*"
#define BACKUP_FONT "*"

#define DEFAULT_XFT_FONT "Mono-11"

/* maximum xvsprintf result string length for systems with pre-C99 snprintf:
 * on errors that are either permanent or cannot be distinguished from those
 * as libc's snprintf might by returning -1 for too small buffers, at most
 * the double of this value is tried before giving up */
#define MAX_LEGACY_SNPRINTF_SIZE 102400
