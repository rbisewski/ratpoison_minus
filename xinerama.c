/* functions for grabbing information about Xinerama screens
 * Copyright (C) 2003 Cameron Patrick
 *
 * This file is part of ratpoison.
 *
 * ratpoison is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * ratpoison is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA
 */

#include "ratpoison.h"
#include <X11/extensions/Xinerama.h>

int xine_screen_count;
int rp_have_xinerama = 0;

#ifdef XINERAMA
static XineramaScreenInfo *xine_screens = NULL;
#endif

void
init_xinerama(void)
{
#ifdef XINERAMA
        int evbase, errbase, major, minor;

        rp_have_xinerama = 0;

        if (xine_screens) XFree(xine_screens);

        if (!XineramaQueryExtension(dpy, &evbase, &errbase)) {
                return;
        }

        if (XineramaQueryVersion(dpy, &major, &minor) == 0) {
                return;
        }

        if (major != 1) {
                fprintf (stderr, "Warning: Xinerama version %d.%d not supported\n", major, minor);
                return;
        }

        if (!XineramaIsActive(dpy)) {
                return;
        }

        xine_screens = XineramaQueryScreens(dpy, &xine_screen_count);
        if (xine_screens == NULL) {
                return;
        }
        if (xine_screen_count < 2) {
               XFree (xine_screens);
               xine_screens = NULL;
               return;
        }

        rp_have_xinerama = 1;
#else
        rp_have_xinerama = 0;
#endif
}

#ifdef XINERAMA
void xinerama_get_screen_info(int sc, int *x, int *y, int *w, int *h)
{
        if ((sc < xine_screen_count) && (sc >= 0)) {
                *x = xine_screens[sc].x_org;
                *y = xine_screens[sc].y_org;
                *w = xine_screens[sc].width;
                *h = xine_screens[sc].height;
        }
}
#else
void xinerama_get_screen_info(int sc UNUSED, int *x UNUSED, int *y UNUSED, int *w UNUSED, int *h UNUSED)
{
}
#endif

void
free_xinerama(void)
{
#ifdef XINERAMA
        if (xine_screens) {
                XFree(xine_screens);
        }
        rp_have_xinerama = 0;
#endif
}
