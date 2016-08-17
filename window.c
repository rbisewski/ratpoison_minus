/* functions for handling the window list
 * Copyright (C) 2000, 2001, 2002, 2003, 2004 Shawn Betts <sabetts@vcn.bc.ca>
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ratpoison.h"

LIST_HEAD(rp_unmapped_window);
LIST_HEAD(rp_mapped_window);

struct numset *rp_window_numset;

/* Get the mouse position relative to the the specified window */
static void
get_mouse_position (rp_window *win, int *mouse_x, int *mouse_y)
{
  Window root_win, child_win;
  int root_x, root_y;
  unsigned int mask;

  XQueryPointer (dpy, win->scr->root, &root_win, &child_win, mouse_x, mouse_y, &root_x, &root_y, &mask);
}

void
free_window (rp_window *w)
{
  if (w == NULL) return;

  free (w->user_name);
  free (w->res_name);
  free (w->res_class);
  free (w->wm_name);

  XFree (w->hints);

  free (w);
}

void
update_window_gravity (rp_window *win)
{
/*   if (win->hints->win_gravity == ForgetGravity) */
/*     { */
      if (win->transient)
        win->gravity = defaults.trans_gravity;
      else if (win->hints->flags & PMaxSize || win->hints->flags & PAspect)
        win->gravity = defaults.maxsize_gravity;
      else
        win->gravity = defaults.win_gravity;
/*     } */
/*   else */
/*     { */
/*       win->gravity = win->hints->win_gravity; */
/*     } */
}

char *
window_name (rp_window *win)
{
  if (win == NULL) return NULL;

  if (win->named)
    return win->user_name;

  switch (defaults.win_name)
    {
    case WIN_NAME_RES_NAME:
      if (win->res_name)
        return win->res_name;
      else return win->user_name;

    case WIN_NAME_RES_CLASS:
      if (win->res_class)
        return win->res_class;
      else return win->user_name;

      /* if we're not looking for the res name or res class, then
         we're looking for the window title. */
    default:
      if (win->wm_name)
        return win->wm_name;
      else return win->user_name;
    }

  return NULL;
}

/* FIXME: we need to verify that the window is running on the same
   host as something. otherwise there could be overlapping PIDs. */
struct rp_child_info *
get_child_info (Window w)
{
  rp_child_info *cur;
  int status;
  int pid;
  Atom type_ret;
  int format_ret;
  unsigned long nitems;
  unsigned long bytes_after;
  unsigned char *req;

  status = XGetWindowProperty (dpy, w, _net_wm_pid,
                               0, 0, False, XA_CARDINAL,
                               &type_ret, &format_ret, &nitems, &bytes_after, &req);

  if (status != Success || req == NULL)
    {
      PRINT_DEBUG (("Couldn't get _NET_WM_PID Property\n"));
      return NULL;
    }

  /* XGetWindowProperty always allocates one extra byte even if
     the property is zero length. */
  XFree (req);

  status = XGetWindowProperty (dpy, w, _net_wm_pid,
                               0, (bytes_after / 4) + (bytes_after % 4 ? 1 : 0),
                               False, XA_CARDINAL, &type_ret, &format_ret, &nitems,
                               &bytes_after, &req);

  if (status != Success || req == NULL)
    {
      PRINT_DEBUG (("Couldn't get _NET_WM_PID Property\n"));
      return NULL;
    }

  pid = *((int *)req);
  XFree(req);

  PRINT_DEBUG(("pid: %d\n", pid));

  list_for_each_entry (cur, &rp_children, node)
    if (pid == cur->pid)
      return cur;

  return NULL;
}

/* Allocate a new window and add it to the list of managed windows */
rp_window *
add_to_window_list (rp_screen *s, Window w)
{
  struct rp_child_info *child_info;
  rp_window *new_window;
  rp_group *group = NULL;
  int frame_num = -1;

  new_window = xmalloc (sizeof (rp_window));

  new_window->w = w;
  new_window->scr = s;
  new_window->last_access = 0;
  new_window->state = WithdrawnState;
  new_window->number = -1;
  new_window->frame_number = EMPTY;
  new_window->intended_frame_number = -1;
  new_window->named = 0;
  new_window->hints = XAllocSizeHints ();
  new_window->colormap = DefaultColormap (dpy, s->screen_num);
  new_window->transient = XGetTransientForHint (dpy, new_window->w, &new_window->transient_for);
  PRINT_DEBUG (("transient %d\n", new_window->transient));

  update_window_gravity (new_window);

  get_mouse_position (new_window, &new_window->mouse_x, &new_window->mouse_y);

  XSelectInput (dpy, new_window->w, WIN_EVENTS);

  new_window->user_name = xstrdup ("Unnamed");

  new_window->wm_name = NULL;
  new_window->res_name = NULL;
  new_window->res_class = NULL;

  /* Add the window to the end of the unmapped list. */
  list_add_tail (&new_window->node, &rp_unmapped_window);

  child_info = get_child_info (w);

  if (child_info && !child_info->window_mapped) {
    rp_frame *frame = screen_find_frame_by_frame (child_info->screen, child_info->frame);

    PRINT_DEBUG(("frame=%p\n", frame));
    group = groups_find_group_by_group (child_info->group);
    if (frame)
      frame_num = frame->number;
    /* Only map the first window in the launch frame. */
    child_info->window_mapped = 1;
  }

  /* Add the window to the group it's pid was launched in or the
     current one. */
  if (group)
    group_add_window (group, new_window);
  else
    group_add_window (rp_current_group, new_window);

  PRINT_DEBUG(("frame_num: %d\n", frame_num));
  if (frame_num >= 0)
    new_window->intended_frame_number = frame_num;

  return new_window;
}

/* Check to see if the window is in the list of windows. */
rp_window *
find_window_in_list (Window w, struct list_head *list)
{
  rp_window *cur;

  list_for_each_entry (cur, list, node)
    {
      if (cur->w == w) return cur;
    }

  return NULL;
}

/* Check to see if the window is in any of the lists of windows. */
rp_window *
find_window (Window w)
{
  rp_window *win = NULL;


  win = find_window_in_list (w, &rp_mapped_window);

  if (!win)
    {
      win = find_window_in_list (w, &rp_unmapped_window);
      if (win)
        PRINT_DEBUG (("Window found in unmapped window list\n"));
      else
        PRINT_DEBUG (("Window not found.\n"));
    }
  else
    {
      PRINT_DEBUG (("Window found in mapped window list.\n"));
    }

  return win;
}

void
set_current_window (rp_window *win)
{
  set_frames_window (current_frame(), win);
}

rp_window *
find_window_number (int n)
{
  rp_window *cur;

  list_for_each_entry (cur,&rp_mapped_window,node)
    {
/*       if (cur->state == STATE_UNMAPPED) continue; */

      if (n == cur->number) return cur;
    }

  return NULL;
}

rp_window *
find_window_name (char *name, int exact_match)
{
  rp_window_elem *cur;

  if (!exact_match)
    {
      list_for_each_entry (cur, &rp_current_group->mapped_windows, node)
        {
          if (str_comp (name, window_name (cur->win), strlen (name)))
            return cur->win;
        }
    }
  else
    {
      list_for_each_entry (cur, &rp_current_group->mapped_windows, node)
        {
          if (!strcmp (name, window_name (cur->win)))
            return cur->win;
        }
    }

  /* didn't find it */
  return NULL;
}

//! Return the previous window in the list. Assumes window is in the
//! mapped window list.
/*
 * @param   rp_window*   Current window to search in.
 *
 * @return  rp_window*   Pointer to the previous / last mapped window.
 */
rp_window* find_window_prev (rp_window *w)
{
    // Variable declaration
    rp_window *cur;

    // Input validation.
    if (!w) {
        return NULL;
    }

    // Attempt to grab the previous / last mapped window.
    //
    // i.e. cur = list_prev_entry(w, &rp_mapped_window, node);
    //
    if (w->node.prev == &rp_mapped_window) {
        cur = list_entry((&rp_mapped_window)->prev, typeof(*w), node);
    } else {
        cur = list_entry(w->node.prev, typeof(*w), node);
    }

    // Cycle thru 'till we find the one associated with this window.
    while (cur != w) {

        // If the window wasn't in the current frame and isn't null, then
        // this is the window that is wanted. So return it. 
        if (!find_windows_frame(cur)) {
            return cur;
        }

        // Otherwise just get a pointer of the back one and try again.
        //
        // i.e. cur = list_prev_entry(cur, &rp_mapped_window, node);
        //
        if (cur->node.prev == &rp_mapped_window) {
            cur = list_entry((&rp_mapped_window)->prev, typeof(*cur), node);
        } else {
            cur = list_entry(cur->node.prev, typeof(*cur), node);
        }
    }

    // Got nothing? Well return null then.
    return NULL;
}

//! Return the next window in the list. Assumes window is in the mapped
//! window list.
/*
 * @param   rp_window*   Current referenced window.
 *
 * @return  rp_window*   Pointer to the next window in the list.
 */
rp_window* find_window_next (rp_window *w)
{
    // Variable declaration
    rp_window *cur;

    // Input validation.
    if (!w) {
        return NULL;
    }

    // Attempt grab the next entry.
    //
    // i.e. cur = list_next_entry(w, &rp_mapped_window, node);
    //
    if (w->node.next == &rp_mapped_window) {
        cur = list_entry((&rp_mapped_window)->next, typeof(*w), node);
    } else {
        cur = list_entry(w->node.next, typeof(*w), node);
    }

    // Cycle thru 'till this grabs a pointer to the next window.
    while (cur != w) {

        // If the grabbed window isn't in the current frame, then return
        // that one.
        if (!find_windows_frame(cur)) {
            return cur;
        }
 
        // Otherwise grab another from the list and try again.
        //
        // i.e. cur = list_next_entry(cur, &rp_mapped_window, node);
        //
        if (cur->node.next == &rp_mapped_window) {
            cur = list_entry((&rp_mapped_window)->next, typeof(*cur), node);
        } else {
            cur = list_entry(cur->node.next, typeof(*cur), node);
        }
    }

    // Nothing found, so return null.
    return NULL;
}

rp_window *
find_window_other (rp_screen *screen)
{
  return group_last_window (rp_current_group, screen);
}



/* Assumes the list is sorted by increasing number. Inserts win into
   to Right place to keep the list sorted. */
void
insert_into_list (rp_window *win, struct list_head *list)
{
  rp_window *cur;

  list_for_each_entry (cur, list, node)
    {
      if (cur->number > win->number)
        {
          list_add_tail (&win->node, &cur->node);
          return;
        }
    }

  list_add_tail(&win->node, list);
}

static void
save_mouse_position (rp_window *win)
{
  Window root_win, child_win;
  int root_x, root_y;
  unsigned int mask;

  /* In the case the XQueryPointer raises a BadWindow error, the
     window is not mapped or has been destroyed so it doesn't matter
     what we store in mouse_x and mouse_y since they will never be
     used again. */

  ignore_badwindow++;

  XQueryPointer (dpy, win->w, &root_win, &child_win,
                 &root_x, &root_y, &win->mouse_x, &win->mouse_y, &mask);

  ignore_badwindow--;
}

/* Takes focus away from last_win and gives focus to win */
void
give_window_focus (rp_window *win, rp_window *last_win)
{
  /* counter increments every time this function is called. This way
     we can track which window was last accessed. */
  static int counter = 1;

  /* Warp the cursor to the window's saved position if last_win and
     win are different windows. */
  if (last_win != NULL && win != last_win)
    {
      save_mouse_position (last_win);
      XSetWindowBorder (dpy, last_win->w, last_win->scr->bw_color);
    }

  if (win == NULL) return;

  counter++;
  win->last_access = counter;
  unhide_window (win);

  if (defaults.warp)
    {
      PRINT_DEBUG (("Warp pointer\n"));
      XWarpPointer (dpy, None, win->w,
                    0, 0, 0, 0, win->mouse_x, win->mouse_y);
    }

  /* Swap colormaps */
  if (last_win != NULL) XUninstallColormap (dpy, last_win->colormap);
  XInstallColormap (dpy, win->colormap);

  XSetWindowBorder (dpy, win->w, win->scr->fw_color);

  /* Finally, give the window focus */
  rp_current_screen = win->scr->xine_screen_num;
  set_rp_window_focus (win);

  XSync (dpy, False);
}

/* In the current frame, set the active window to win. win will have focus. */
void set_active_window (rp_window *win)
{
  set_active_window_body(win, 0);
}

void set_active_window_force (rp_window *win)
{
  set_active_window_body(win, 1);
}

//! Find the non-dedicated frame via the current screen.
/*
 * @param   rp_screen*  Pointer to the current screen.
 * @param   rp_fram*    Pointer to the current frame.
 *
 * @return  rp_frame*   Pointer to the unused frame.
 */
static rp_frame* find_frame_non_dedicated(rp_screen *current_screen, rp_frame *current_frame)
{
    // Input validation.
    rp_frame *cur;

    // Attempt to grab the next frame from the list of frames on the
    // given screen.
    //
    // i.e. cur = list_next_entry(current_frame, &current_screen->frames, node);
    //
    if (current_frame->node.next == &current_screen->frames) {
        cur = list_entry((&current_screen->frames)->next, typeof(*current_frame), node);
    } else {
        cur = list_entry(current_frame->node.next, typeof(*current_frame), node);
    }

    // Try the only / current screen...
    while (cur != current_frame) {

        // Non-dedicated screen? Then this is the one!
        if (!cur->dedicated) {
            return cur;
        }

        // Else gotta keep lookin'
        //
        // i.e. cur = list_next_entry(cur, &current_screen->frames, node);
        //
        if (cur->node.next == &current_screen->frames) {
            cur = list_entry((&current_screen->frames)->next, typeof(*cur), node);
        } else {
            cur = list_entry(cur->node.next, typeof(*cur), node);
        }
    }

    // If we have Xinerama, we can check *all* screens...
    if (rp_have_xinerama) {

        // Cycle thru all of the current screens array.
        int i;
        for (i = 0; i < num_screens; i++) {

            // Grabbed the currently used screen? Skip it.
            if (current_screen == &screens[i]) {
                continue;
            }

            // Get the list for each entry, and search it.
            list_for_each_entry(cur, &screens[i].frames, node)
            {
                // Non-dedicated screen? Then this is the one!
                if (!cur->dedicated) {
                  return cur;
                }
            }
        }
    }

    // Otherwise got nothing, so return NULL.
    return NULL;
}

void
set_active_window_body (rp_window *win, int force)
{
  rp_window *last_win;
  rp_frame *frame = NULL, *last_frame = NULL;
  rp_screen *screen;

  if (win == NULL)
    return;

  PRINT_DEBUG (("intended_frame_number: %d\n", win->intended_frame_number));

  /* With Xinerama, we can move a window over to the current screen; otherwise
   * we have to switch to the screen that the window belongs to.
   */
  screen = rp_have_xinerama ? current_screen () : win->scr;

  /* use the intended frame if we can. */
  if (win->intended_frame_number >= 0)
    {
      frame = screen_get_frame (screen, win->intended_frame_number);
      win->intended_frame_number = -1;
      if (frame != current_frame ())
        last_frame = current_frame ();
    }

  if (frame == NULL)
    frame = screen_get_frame (screen, screen->current_frame);

  if (frame->dedicated && !force)
    {
      /* Try to find a non-dedicated frame. */
      rp_frame *non_dedicated;

      non_dedicated = find_frame_non_dedicated (screen, frame);
      if (non_dedicated != NULL)
        {
          last_frame = frame;
          frame = non_dedicated;
          set_active_frame (frame, 0);
        }
    }

  last_win = set_frames_window (frame, win);

  if (last_win != NULL)
    PRINT_DEBUG (("last window: %s\n", window_name (last_win)));
  PRINT_DEBUG (("new window: %s\n", window_name (win)));

  /* Make sure the window comes up full screen */
  maximize (win);

  /* Focus the window. */
  give_window_focus (win, last_win);

  /* The other windows in the frame will be hidden if this window
     doesn't qualify as a transient window (ie dialog box. */
  if (!window_is_transient (win))
    hide_others(win);

  /* Make sure the program bar is always on the top */
  update_window_names (win->scr, defaults.window_fmt);

  XSync (dpy, False);

  /* If we switched frame, go back to the old one. */
  if (last_frame != NULL)
    set_active_frame (last_frame, 0);

  /* Call the switch window hook */
  hook_run (&rp_switch_win_hook);
}

/* Go to the window, switching frames if the window is already in a
   frame. */
void
goto_window (rp_window *win)
{
  rp_frame *frame;

  /* There is nothing to do if it is already the current window. */
  if (current_window() == win)
    return;

  frame = find_windows_frame (win);
  if (frame)
    {
      set_active_frame (frame, 0);
    }
  else
    {
      set_active_window (win);
    }
}

/* get the window list and store it in buffer delimiting each window
   with delim. mark_start and mark_end will be filled with the text
   positions for the start and end of the current window. */
void
get_window_list (char *fmt, char *delim, struct sbuf *buffer,
                 int *mark_start, int *mark_end)
{
  rp_window_elem *we;

  if (buffer == NULL) return;

  sbuf_clear (buffer);
  find_window_other (current_screen());

  /* We only loop through the current group to look for windows. */
  list_for_each_entry (we,&rp_current_group->mapped_windows,node)
    {
      PRINT_DEBUG (("%d-%s\n", we->number, window_name (we->win)));

      if (we->win == current_window())
        *mark_start = strlen (sbuf_get (buffer));

      /* A hack, pad the window with a space at the beginning and end
         if there is no delimiter. */
      if (!delim)
        sbuf_concat (buffer, " ");

      format_string (fmt, we, buffer);

      /* A hack, pad the window with a space at the beginning and end
         if there is no delimiter. */
      if (!delim)
        sbuf_concat (buffer, " ");

      /* Only put the delimiter between the windows, and not after the the last
         window. */
      if (delim && we->node.next != &rp_current_group->mapped_windows)
        sbuf_concat (buffer, delim);

      if (we->win == current_window())
        {
          *mark_end = strlen (sbuf_get (buffer));
        }
    }

  if (!strcmp (sbuf_get (buffer), ""))
    {
      sbuf_copy (buffer, MESSAGE_NO_MANAGED_WINDOWS);
    }
}

void
init_window_stuff (void)
{
  rp_window_numset = numset_new ();
}

void
free_window_stuff (void)
{
  rp_window *cur;
  struct list_head *tmp, *iter;

  list_for_each_safe_entry (cur, iter, tmp, &rp_unmapped_window, node)
    {
      list_del (&cur->node);
      groups_del_window (cur);
      free_window (cur);
    }

  list_for_each_safe_entry (cur, iter, tmp, &rp_mapped_window, node)
    {
      list_del (&cur->node);
      groups_unmap_window (cur);
      groups_del_window (cur);
      free_window (cur);
    }

  numset_free (rp_window_numset);
}

rp_frame *
win_get_frame (rp_window *win)
{
  if (win->frame_number != EMPTY)
    return screen_get_frame (win->scr, win->frame_number);
  else
    return NULL;
}
