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
 * @file eail_scrollable_widget.c
 * @brief Implementation of widget that can scroll its content
 */

#include <atk/atk.h>
#include <Elementary.h>

#include "eail_scrollable_widget.h"
#include "eail_priv.h"

static void eail_scrollable_widget_actions_table_init(EailScrollableWidget *widget);

/**
 * @brief definition of EailScrollableWidget as GObject
 *
 * NOTE: this class extends EAIL_TYPE_ACTION_WIDGET, which implements
 * all needed functions for ATK_ACTION interface so additional implementing
 * ATK_ACTION interface here is not needed.
 * Only what is really needed, is to call:
 * eail_action_widget_action_append(...) to register all needed additional
 * actions
 */
G_DEFINE_TYPE(EailScrollableWidget, eail_scrollable_widget,
              EAIL_TYPE_ACTION_WIDGET);

#define EAIL_STR_SCROLL_UP "scroll_up"
#define EAIL_STR_SCROLL_DOWN "scroll_down"
#define EAIL_STR_SCROLL_LEFT "scroll_left"
#define EAIL_STR_SCROLL_RIGHT "scroll_right"

enum EAIL_SCROLL_TYPE
{
   EAIL_SCROLL_TYPE_UP,
   EAIL_SCROLL_TYPE_DOWN,
   EAIL_SCROLL_TYPE_LEFT,
   EAIL_SCROLL_TYPE_RIGHT
};

/**
 * @brief Initializer for AtkObjectClass
 */
static void
eail_scrollable_widget_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_scrollable_widget_parent_class)->initialize(obj, data);

   EailScrollableWidget *widget = EAIL_SCROLLABLE_WIDGET(obj);

   /* Initializing possible widget actions table*/
   eail_scrollable_widget_actions_table_init(widget);
}

/**
 * @brief Initializer for GObject class
 */
static void
eail_scrollable_widget_init(EailScrollableWidget *scrollable_widget)
{
}

/**
 * @brief Finalize function for GObject
 */
static void
eail_scrollable_widget_finalize(GObject *object)
{
   EailScrollableWidget *widget = NULL;
   G_OBJECT_CLASS(eail_scrollable_widget_parent_class)->finalize(object);

   widget = EAIL_SCROLLABLE_WIDGET(object);
   g_return_if_fail(widget);
}

/**
 * @brief Init function for GObject
 */
static void
eail_scrollable_widget_class_init(EailScrollableWidgetClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   class->initialize = eail_scrollable_widget_initialize;
   gobject_class->finalize = eail_scrollable_widget_finalize;
}

/**
 * @brief scrolls screen to given direction
 *
 * @returns TRUE if scroll action was successful, FALSE otherwise
 */
static gboolean
eail_handle_scroll(AtkAction *action, enum EAIL_SCROLL_TYPE type)
{
   Evas_Object *widget = NULL;
   int x, y, w, h;

   widget = eail_widget_get_widget(EAIL_WIDGET(action));

   elm_scroller_region_get(widget, &x, &y, &w, &h);

   if (EAIL_SCROLL_TYPE_UP == type)
      y = y - h;
   else if (EAIL_SCROLL_TYPE_DOWN == type)
      y = y + h;
   else if (EAIL_SCROLL_TYPE_RIGHT == type)
      x = x + w;
   else if (EAIL_SCROLL_TYPE_LEFT == type)
      x = x - w;

   elm_scroller_region_bring_in(widget, x, y, w, h);

   return TRUE;
}

/**
 * @brief scrolls screen up
 *
 * @returns TRUE if scroll action was successful, FALSE otherwise
 */
static gboolean
eail_action_scroll_up(AtkAction *action, void *data)
{
   return eail_handle_scroll(action, EAIL_SCROLL_TYPE_UP);
}

/**
 * @brief scrolls screen down
 *
 * @returns TRUE if scroll action was successful, FALSE otherwise
 */
static gboolean
eail_action_scroll_down(AtkAction *action, void *data)
{
   return eail_handle_scroll(action, EAIL_SCROLL_TYPE_DOWN);
}

/**
 * @brief scrolls screen left
 *
 * @returns TRUE if scroll action was successful, FALSE otherwise
 */
static gboolean
eail_action_scroll_left(AtkAction *action, void *data)
{
   return eail_handle_scroll(action, EAIL_SCROLL_TYPE_LEFT);
}

/**
 * @brief scrolls screen right
 *
 * @returns TRUE if scroll action was successful, FALSE otherwise
 */
static gboolean
eail_action_scroll_right(AtkAction *action, void *data)
{
   return eail_handle_scroll(action, EAIL_SCROLL_TYPE_RIGHT);
}

/**
 * @brief Adds actions for scrolling to actions table
 * @param widget widget that implements EailScrollable interface
 */
static void
eail_scrollable_widget_actions_table_init(EailScrollableWidget *widget)
{
   eail_action_widget_action_append(EAIL_ACTION_WIDGET(widget),
                                    EAIL_STR_SCROLL_UP, NULL,
                                    eail_action_scroll_up);
   eail_action_widget_action_append(EAIL_ACTION_WIDGET(widget),
                                    EAIL_STR_SCROLL_DOWN, NULL,
                                    eail_action_scroll_down);
   eail_action_widget_action_append(EAIL_ACTION_WIDGET(widget),
                                    EAIL_STR_SCROLL_LEFT, NULL,
                                    eail_action_scroll_left);
   eail_action_widget_action_append(EAIL_ACTION_WIDGET(widget),
                                    EAIL_STR_SCROLL_RIGHT, NULL,
                                    eail_action_scroll_right);
}
