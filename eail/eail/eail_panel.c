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
 * @file eail_panel.c
 * @brief Implementation of panel widget
 */

#include <atk/atk.h>
#include <Elementary.h>

#include "eail_panel.h"

static void atk_action_interface_init(AtkActionIface *iface);

#define EAIL_PANEL_TOGGLE_ACTION "toggle"

/**
 * @brief Definition of EailPanel as GObject
 *
 * EailFileselectorEntry is extended EAIL_TYPE_WIDGET with ATK_TYPE_ACTION
 * interface implemented
 */
G_DEFINE_TYPE_WITH_CODE(EailPanel, eail_panel, EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_ACTION,
                                              atk_action_interface_init))


/**
 * @brief Implementation of eail_widget_get_widget_children callback from
 * EailWidget
 *
 * @returns Eina_List filled with Evas_Object* objects contained in Panel widget
 */
static Eina_List*
eail_panel_children_get(EailWidget *widget)
{
   Evas_Object *obj, *child;
   Eina_List *list;

   g_return_val_if_fail(EAIL_IS_PANEL(widget), NULL);

   obj = eail_widget_get_widget(widget);
   if (obj == NULL) return NULL;
   child = elm_object_part_content_get(obj, "default");
   if (child == NULL) return NULL;

   list = NULL;
   list = eina_list_append(list, child);

   return list;
}

/**
 * @brief Implementation of AtkObject->ref_state_set callback
 *
 * ATK doc says:
 * Gets a reference to the state set of the accessible; the caller must
 * unreference it when it is no longer needed.
 *
 * @returns a reference to an AtkStateSet which is the state set of the
 * accessible.
 */
static AtkStateSet*
eail_panel_ref_state_set(AtkObject *obj)
{
   Evas_Object *widget;
   Eina_Bool hidden;
   AtkStateSet *state_set;

   g_return_val_if_fail(EAIL_IS_PANEL(obj), NULL);

   state_set = ATK_OBJECT_CLASS(eail_panel_parent_class)->ref_state_set(obj);
   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   hidden = elm_panel_hidden_get(widget);
   if (!hidden)
     atk_state_set_add_state(state_set, ATK_STATE_VISIBLE);

   return state_set;
}

/**
 * @brief Initializer for AtkObjectClass
 */
static void
eail_panel_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_panel_parent_class)->initialize(obj, data);
   obj->role = ATK_ROLE_PANEL;
}

/**
 * @brief Initializer for GObject class
 */
static void
eail_panel_init(EailPanel *panel)
{
   panel->toggle_description = NULL;
}

/**
 * @brief Destructor for panel object
 */
static void eail_panel_finalize(GObject *g_object)
{
   EailPanel *panel = EAIL_PANEL(g_object);
   if (panel->toggle_description)
     {
        free(panel->toggle_description);
        panel->toggle_description = NULL;
     }
   G_OBJECT_CLASS(g_object)->finalize(g_object);
}

/**
 * @brief Initializer for GObject panel class (defines callbacks for base
 * AtkObject)
 */
static void
eail_panel_class_init(EailPanelClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   EailWidgetClass *widget_class = EAIL_WIDGET_CLASS(klass);
   GObjectClass *g_object_class = G_OBJECT_CLASS(klass);

   g_object_class->finalize = eail_panel_finalize;

   widget_class->get_widget_children = eail_panel_children_get;

   class->initialize = eail_panel_initialize;
   class->ref_state_set = eail_panel_ref_state_set;
}

/**
 * @brief Launches action with given index
 *
 * @param action object that implements AtkAction interface
 * @param i index (number) of action
 *
 * @returns TRUE if action was successfully launched, FALSE otherwise
 */
static gboolean
eail_panel_do_action(AtkAction *action, gint i)
{
   Evas_Object *widget;

   g_return_val_if_fail(EAIL_IS_PANEL(action), FALSE);
   g_return_val_if_fail(i == 0, FALSE);

   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   if (!widget) return FALSE;

   elm_panel_toggle(widget);

   return TRUE;
}

/**
 * @brief Gets name of action with given index
 *
 * @param action object that implements AtkAction interface
 * @param i index (number) of action
 *
 * @returns name (const gchar*) of action with given index
 */
static const char*
eail_panel_action_name_get(AtkAction *action, gint i)
{
   g_return_val_if_fail(EAIL_IS_PANEL(action), NULL);
   g_return_val_if_fail(i == 0, NULL);
   /*toggle the hidden state of the panel*/
   return EAIL_PANEL_TOGGLE_ACTION;
}

/**
 * @brief Returns number of implemented ATK Actions.
 * Implementation of AtkActionIface get_n_actions callback
 *
 * @param action object that implements AtkAction interface
 *
 * @returns gint representing amount of actions implemented by panel
 */
static gint
eail_panel_n_actions_get(AtkAction *action)
{
   g_return_val_if_fail(EAIL_IS_PANEL(action), 0);
   return 1;
}

/**
 * @brief Sets description for action with given index
 * Implementation of AtkActionIface set_description callback
 *
 * @param action object that implements AtkAction interface
 * @param i index (number) of action
 * @param description description to set
 *
 * @returns TRUE if update operation ended with success, FALSE otherwise
 */
static gboolean
eail_panel_description_set(AtkAction *action, gint i, const char *description)
{
   EailPanel *panel;

   g_return_val_if_fail(EAIL_IS_PANEL(action), FALSE);
   g_return_val_if_fail(i == 0, FALSE);

   panel = EAIL_PANEL(action);

   if (panel->toggle_description) free(panel->toggle_description);
   panel->toggle_description = g_strdup(description);

   return TRUE;
}

/**
 * @brief Gets description for action with given index
 * Implementation of AtkActionIface get_description callback
 *
 * @param action object that implements AtkAction interface
 * @param i index (number) of action
 *
 * @returns string representing description for action
 */
static const char*
eail_panel_description_get(AtkAction *action, gint i)
{
   EailPanel *panel;

   g_return_val_if_fail(EAIL_IS_PANEL(action), NULL);
   g_return_val_if_fail(i == 0, NULL);

   panel = EAIL_PANEL(action);

   return panel->toggle_description;
}

/**
 * @brief Inits Action interface for PANEL
 */
static void
atk_action_interface_init(AtkActionIface *iface)
{
   g_return_if_fail(iface != NULL);

   iface->do_action = eail_panel_do_action;
   iface->get_n_actions = eail_panel_n_actions_get;
   iface->get_name = eail_panel_action_name_get;
   iface->get_description = eail_panel_description_get;
   iface->set_description = eail_panel_description_set;
}
