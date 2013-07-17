/*
 * Copyright (c) 2013 Samsung Electronics Co., Ltd.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

/**
 * @file eail_utils.c
 * @brief Implementation of utility functions used in various places in EAIL
 * code
 */

#include <atk/atk.h>
#include <Elementary.h>
#include <Eina.h>
#include <Evas.h>

/* internal elm_widget api for listening EDJE parts */
#define ELM_INTERNAL_API_ARGESFSDFEFC
#include <elm_widget.h>

#include "eail_utils.h"
#include "eail_priv.h"

/**
 * @param string base string to get substring from
 * @param start_offset beginning offset
 * @param end_offset end offset
 *
 * @returns newly allocated substring
 */
gchar *
eail_get_substring(const gchar *string,
                   gint         start_offset,
                   gint         end_offset)
{
   gchar *substring = NULL;
   gint len = g_utf8_strlen(string, -1);
   gint sub_len = 0;

   if ((start_offset <  0) ||
      (start_offset > len - 1) ||
      (end_offset   <  -1))
    return NULL;
   else if (end_offset == -1 ||
           end_offset >= len - 1)
     sub_len = len - start_offset + 1;
   else
     sub_len = end_offset - start_offset;

   substring = g_malloc0(sub_len + 1);

   return g_utf8_strncpy(substring, &string[start_offset], sub_len);
}

/**
 * @param widget Evas_Object for getting state_set
 * @param state_set current state_set taken from objects parent
 *
 * @returns filled AtkStateSet for given EvasObject
 */
AtkStateSet *
eail_evas_obj_ref_state_set(Evas_Object *widget, AtkStateSet *state_set)
{
   if (!widget) {
       atk_state_set_add_state(state_set, ATK_STATE_DEFUNCT);
       return state_set;
   }

   if (!elm_object_disabled_get(widget)) {
       atk_state_set_add_state(state_set, ATK_STATE_SENSITIVE);
       atk_state_set_add_state(state_set, ATK_STATE_ENABLED);
   }

   if (evas_object_visible_get(widget)) {
       int x, y, width, height;
       int vp_x, vp_y, vp_width, vp_height;

       atk_state_set_add_state(state_set, ATK_STATE_VISIBLE);

       evas_object_geometry_get(widget, &x, &y, &width, &height);
       evas_output_viewport_get(evas_object_evas_get(widget),
                                &vp_x, &vp_y, &vp_width, &vp_height);

       if ((x + width) >= vp_x && (y + height) >= vp_y &&
           (vp_x + vp_width) >= x && (vp_y + vp_height) >= y) {
           atk_state_set_add_state(state_set, ATK_STATE_SHOWING);
       }
   }

   if (elm_object_focus_allow_get(widget)) {
       atk_state_set_add_state(state_set, ATK_STATE_FOCUSABLE);

       if (elm_object_focus_get(widget)) {
           atk_state_set_add_state(state_set, ATK_STATE_FOCUSED);
       }
   }

   return state_set;
}

/**
 * @param widget an Evas_Object
 * @return TRUE if grabbing focus was successfull, FALSE otherwise
 */
gboolean
eail_evas_obj_grab_focus(Evas_Object *widget)
{
   Ecore_Evas *ee = ecore_evas_ecore_evas_get(evas_object_evas_get(widget));

   if (!widget || !elm_object_focus_allow_get(widget)) {
         return FALSE;
   }

   ecore_evas_activate(ee);
   elm_object_focus_set(widget, EINA_TRUE);

   return TRUE;
}

/**
 * @param data passed to callback
 * @param obj object that has been focused
 * @param event_info additional event info
 */
void
eail_widget_on_focused(void *data, Evas_Object *obj, void *event_info)
{
   g_return_if_fail(ATK_IS_OBJECT(data));

   atk_focus_tracker_notify(ATK_OBJECT(data));
}

/**
 * @param widget an Evas_Object to click on
 * @param x X coord
 * @param y y coord
 */
void
eail_mouse_click_on_coords(Evas_Object *widget, int x, int y)
{
   Evas* evas = NULL;
   evas =  evas_object_evas_get(widget);

   evas_event_feed_mouse_move(evas, x, y, 0, NULL);
   evas_event_feed_mouse_down(evas, 1, EVAS_BUTTON_NONE, 0, NULL);
   evas_event_feed_mouse_up(evas, 1, EVAS_BUTTON_NONE, 0, NULL);
}

/**
 * @param widget an Evas_Object to get values for
 * @param x address of int to put X coord in
 * @param y address of int to put Y coord in
 */
void
eail_get_coords_widget_center(Evas_Object *widget, int *x, int *y)
{
   int w, h;
   evas_object_geometry_get(widget, x, y, &w, &h);

   *x = *x + (w / 2);
   *y = *y + (h / 2);
}

static Eina_List *
_eail_get_evas_obj_list_from_item(Elm_Object_Item *item)
{
   Evas_Object *edje = NULL;

   edje = VIEW(item);
   if (!edje)
     {
        DBG("Edje object for item not found. Returning empty list");\
        return NULL;
     }

    return evas_object_smart_members_get(edje);
}

/**
 * @param item Elm_Object_Item * to get strings from
 * @returns EinaList filled with const gchar* that are representing text
 * content of item. List have to be freed when no longer needed but DO NOT
 * FREE CONTENT STRINGS.
 */
Eina_List *
eail_item_get_content_strings(Elm_Object_Item *item)
{
   Eina_List *strings_list = NULL;
   Eina_List *edje_parts = NULL;
   Evas_Object *part = NULL;
   Eina_List *l;


   edje_parts = _eail_get_evas_obj_list_from_item(item);
   EINA_LIST_FOREACH(edje_parts, l, part)
     {
      const gchar *type_name = evas_object_type_get(part);

      if (0 == strcmp(type_name, "text"))
        {
           const gchar *text = evas_object_text_text_get(part);

           if (text)
             strings_list = eina_list_append(strings_list, text);
        }
     }

   return strings_list;
}

/**
 * It does filtering inside and returs only parts that can be used later in
 * eail factory (only widgets will be returned)
 *
 * @param item Elm_Object_Item * to get objects from
 *
 * @returns Eina_List* filled with Evas_Object* objects representing content
 * found inside item
 */
Eina_List *
eail_get_edje_parts_for_item(Elm_Object_Item *item)
{
   Eina_List *edje_parts = NULL;
   Eina_List *usable_parts = NULL;
   int i = 0;

   edje_parts = _eail_get_evas_obj_list_from_item(item);
   for (i = 0; i < eina_list_count(edje_parts); ++i)
     {
        Evas_Object *obj = eina_list_nth(edje_parts, i);

        /* adding only parts that can be used by eail_factory later */
        if (elm_object_widget_check(obj))
          usable_parts = eina_list_append(usable_parts, obj);
     }

   eina_list_free(edje_parts);

   return usable_parts;
}
