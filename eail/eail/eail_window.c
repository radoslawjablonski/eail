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
 * @file eail_window.c
 * @brief EailWindow Implementation
 */

#include <Elementary.h>
#include "eail_window.h"
#include "eail_utils.h"
#include "eail_priv.h"

static void eail_window_actions_init(EailActionWidget *widget);

/**
 * @brief Focus signal name
 */
#define EAIL_WINDOW_FOCUS_NAME "focus,in"

#define EAIL_WIN_ACTION_MAXIMIZE "maximize"
#define EAIL_WIN_ACTION_MINIMIZE "minimize"

/**
 * @brief EailWindow type definition
 */
G_DEFINE_TYPE(EailWindow, eail_window, EAIL_TYPE_ACTION_WIDGET)

/**
 * @brief Initialize window focus handler
 *
 * @param obj an AtkObject
 */
static void
eail_window_init_focus_handler(AtkObject *obj)
{
   Evas_Object *nested_widget = NULL;
   g_return_if_fail(EAIL_IS_WIDGET(obj));

   nested_widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!nested_widget) {
       ERR("No evas object inside EailWidget was found");
       return;
   }

   evas_object_smart_callback_add(nested_widget, EAIL_WINDOW_FOCUS_NAME,
                                  eail_widget_on_focused, obj);
}

/**
 * @brief EailWindow initializer
 *
 * @param obj an AtkObject
 * @param data initialization data
 */
static void
eail_window_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_window_parent_class)->initialize(obj, data);
   EAIL_WIDGET(obj)->layer = ATK_LAYER_WINDOW;

   if (!elm_object_widget_check((Evas_Object *) data)) return;

   obj->name = (gchar *)elm_win_title_get((Evas_Object *)data);
   obj->role = ATK_ROLE_WINDOW;

   eail_window_init_focus_handler(obj);
   eail_window_actions_init(EAIL_ACTION_WIDGET(obj));
}

/**
 * @brief Gets children list in given edje
 *
 * @param edje lowest (stacked) Evas object
 * @return children list
 */
static Eina_List *
_parse_edje(const Evas_Object *edje)
{
   const Evas_Object *content_part = NULL;
   const Evas_Object *menu_part = NULL;
   Eina_List *list = NULL;

   if (edje_object_part_exists(edje, "elm.swallow.menu"))
     menu_part = edje_object_part_swallow_get(edje, "elm.swallow.menu");
   if (edje_object_part_exists(edje, "elm.swallow.contents"))
     content_part = edje_object_part_swallow_get(edje, "elm.swallow.contents");
   if ((menu_part) &&
       (!strcmp(evas_object_type_get(menu_part), "Evas_Object_Box")))
        list = evas_object_box_children_get(menu_part);
   if ((content_part) &&
       (!strcmp(evas_object_type_get(content_part), "Evas_Object_Box")))
     {
        if (list)
          list = eina_list_merge(list,
                                 evas_object_box_children_get(content_part));
        else
          list = evas_object_box_children_get(content_part);
     }

   return list;
}

/**
 * @brief Gets widget children
 *
 * @param widget an EailWidget
 * @return widget list
 */
static Eina_List *
eail_window_get_widget_children(EailWidget *widget)
{
   Evas_Object *o, *obj = eail_widget_get_widget(widget);
   Eina_List *win_childs = NULL;
   Eina_List *widgets = NULL;
   Evas *e;

   /*in elementary >= 1.7.99 we get edje object if object are stacked
     in containers like box, grid etc we need to get children from this
     edje*/

   e = evas_object_evas_get(obj);
   o = evas_object_bottom_get(e);
   if (!strcmp(evas_object_type_get(o), "edje"))
     {
        widgets = _parse_edje(o);
     }
   /* Sometimes we have a mix of widgets grouped in containters with
    * those directly on elm_win objct. So list evas objects laying on
    * window to be sure we get everything */
   o = obj;
   while ((o = evas_object_below_get(o)))
     {
        if (elm_object_widget_check(o))
          {
             /*be sure that object belongs to window and not to other
              * container*/
             if (obj == elm_object_parent_widget_get(o))
               win_childs = eina_list_append(win_childs, o);
          }
     }

   if (win_childs)
     {
        /*reverse list to get correct order of widgets tree*/
        win_childs = eina_list_reverse(win_childs);
        /*merge window childs together with containers*/
        if (widgets)
          widgets = eina_list_merge(widgets, win_childs);
        else
          widgets = win_childs;
     }

   return widgets;
}

/**
 * @brief Gets obj state set
 *
 * @param obj an AtkObject
 * @return obj state set
 */
static AtkStateSet *
eail_window_ref_state_set(AtkObject *obj)
{
   double x, y;
   AtkStateSet *state_set;
   Eina_List *l, *children;
   Eina_Bool resizable = EINA_TRUE;
   Evas_Object *child, *widget = eail_widget_get_widget(EAIL_WIDGET(obj));

   if (!widget) return NULL;

   state_set= ATK_OBJECT_CLASS(eail_window_parent_class)->ref_state_set(obj);

   if (elm_win_focus_get(widget))
     {
        atk_state_set_add_state(state_set, ATK_STATE_ACTIVE);
     }

   if (elm_win_iconified_get(widget))
     {
        atk_state_set_add_state(state_set, ATK_STATE_ICONIFIED);
     }

   if (elm_win_modal_get(widget))
     {
        atk_state_set_add_state(state_set, ATK_STATE_MODAL);
     }

   children = eail_widget_get_widget_children(EAIL_WIDGET(obj));
   EINA_LIST_FOREACH(children, l, child)
     {
        evas_object_size_hint_weight_get(child, &x, &y);
        if (x != EVAS_HINT_EXPAND || y != EVAS_HINT_EXPAND)
          {
             resizable = EINA_FALSE;
             break;
          }
     }
   eina_list_free(children);

   if (resizable)
     {
        atk_state_set_add_state(state_set, ATK_STATE_RESIZABLE);
     }

   return state_set;
}

/**
 * @brief EailWindow instance initializer
 *
 * @param window an EailWindow
 */
static void
eail_window_init(EailWindow *window)
{
}

/**
 * @brief EailWindow class initializer
 *
 * @param klass an EailWindowClass
 */
static void
eail_window_class_init(EailWindowClass *klass)
{
   DBG(">");
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   EailWidgetClass *widget_class = EAIL_WIDGET_CLASS(klass);

   widget_class->get_widget_children = eail_window_get_widget_children;

   class->initialize = eail_window_initialize;
   class->ref_state_set = eail_window_ref_state_set;
}

/**
 * @brief Handle for action minimize
 *
 * @param action an AtkAction
 * @param data additional action data (not used here)
 *
 * @return TRUE if action was triggered successfully, FALSE otherwise
 */
static gboolean
eail_action_minimize(AtkAction *action, void *data)
{
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(action));
   g_return_val_if_fail(widget, FALSE);

   elm_win_iconified_set(widget, EINA_TRUE);

   return TRUE;
}

/**
 * @brief Handle for action maximize
 *
 * @param action an AtkAction
 * @param data additional action data (not used here)
 *
 * @return TRUE if action was triggered successfully, FALSE otherwise
 */
static gboolean
eail_action_maximize(AtkAction *action, void *data)
{
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(action));
   g_return_val_if_fail(widget, FALSE);

   elm_win_maximized_set(widget, EINA_TRUE);

   return TRUE;
}

/**
 * @brief Adds window actions to actions table
 *
 * @param action_widget widget that implements EailActionWidget interface
 */
static void
eail_window_actions_init(EailActionWidget *action_widget)
{
   eail_action_widget_action_append(action_widget,
                                    EAIL_WIN_ACTION_MAXIMIZE, NULL,
                                    eail_action_maximize);
   eail_action_widget_action_append(action_widget,
                                    EAIL_WIN_ACTION_MINIMIZE, NULL,
                                    eail_action_minimize);
}
