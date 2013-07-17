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

#ifndef EAIL_UTILS_H
#define EAIL_UTILS_H

#include <glib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Helper function gets substring from string
 */
gchar *eail_get_substring(const gchar* string, gint start_offset, gint end_offset);

/**
 * @brief Generic function for generating ref_state for Evas_Objects stored in
 * ATK objects
 *
 */
AtkStateSet *eail_evas_obj_ref_state_set(Evas_Object *widget,
                                         AtkStateSet *state_set);

/**
 * @brief Generic function for grabbing focus on Evas_Object stored in ATK object
 */
gboolean eail_evas_obj_grab_focus(Evas_Object *widget);

/**
 * @brief on_focused default callback used to tracking focus changes of widgets
 */
void eail_widget_on_focused(void *data,
                            Evas_Object *obj,
                            void *event_info);

/**
 * @brief Emulates mouse 'click' event at given coordinates
 */
void eail_mouse_click_on_coords(Evas_Object *widget, int x, int y);

/**
 * @brief Gets coordinates of center of given widget
 */
void eail_get_coords_widget_center(Evas_Object *widget, int *x, int *y);

/**
 * @brief Gets edje parts list for item
 *
 */
Eina_List *eail_get_edje_parts_for_item(Elm_Object_Item *item);

/**
 * @brief Gets text content from item (each text is put into field in Eina_List
 * as const gchar*)
 */
Eina_List *eail_item_get_content_strings(Elm_Object_Item *item);
#ifdef __cplusplus
}
#endif

#endif
