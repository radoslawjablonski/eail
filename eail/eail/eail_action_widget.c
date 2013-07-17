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
 * @file eail_action_widget.c
 * @brief Implementation of widget that can hold atk actions
 */

#include <Elementary.h>

#include "eail_action_widget.h"
#include "eail_priv.h"

static void atk_action_interface_init(AtkActionIface *iface);

/**
 * @brief definition of EailActionWidget as GObject
 */
G_DEFINE_TYPE_WITH_CODE(EailActionWidget,
                        eail_action_widget,
                        EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_ACTION,
                                              atk_action_interface_init))

typedef struct _EailActionObj EailActionObj;

/** @struct ActionObject stored in actions list
 */
struct _EailActionObj
{
   const gchar * name;/*!< \brief action name */
   const gchar * keybinding;/*!< \brief action keybinding */
   gchar * description;/*!< \brief action description */
   Eail_Do_Action_Cb action;/*!< \brief action callback */
};

/**
 * @brief Initializer for AtkObjectClass
 */
static void
eail_action_widget_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_action_widget_parent_class)->initialize(obj, data);

   EailActionWidget *widget = EAIL_ACTION_WIDGET(obj);

   widget->action_objs = NULL;
}

/**
 * @brief Initializer for GObject class
 */
static void
eail_action_widget_init(EailActionWidget *action_widget)
{
}

/**
 * @brief Deallocates memory for EailActionObj
 */
static void
eail_action_obj_free(EailActionObj *action_obj)
{
   g_return_if_fail(action_obj);

   if (action_obj->description)
     g_free(action_obj->description);

   g_free(action_obj);
}

/**
 * @brief Finalize function for GObject
 */
static void
eail_action_widget_finalize(GObject *object)
{
   EailActionWidget *widget = NULL;
   const Eina_List *l = NULL;
   EailActionObj *action_obj = NULL;

   widget = EAIL_ACTION_WIDGET(object);
   g_return_if_fail(widget);

   EINA_LIST_FOREACH(widget->action_objs, l, action_obj)
     eail_action_obj_free(action_obj);

   eina_list_free(widget->action_objs);

   G_OBJECT_CLASS(eail_action_widget_parent_class)->finalize(object);
}

/**
 * @brief Init function for GObject
 */
static void
eail_action_widget_class_init(EailActionWidgetClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   class->initialize = eail_action_widget_initialize;
   gobject_class->finalize = eail_action_widget_finalize;
}

/**
 * @param action_widget ActionWidget instance that holds actions
 * @param action_name action name
 * @param keybinding keybinding
 * @param action_cb callback for action, launched when action is triggered
 */
void
eail_action_widget_action_append(EailActionWidget *action_widget,
                                 const gchar *action_name,
                                 const gchar *keybinding,
                                 Eail_Do_Action_Cb action_cb)
{
   g_return_if_fail(action_widget);

   EailActionObj *action_def_obj = g_new0(EailActionObj, 1);

   action_def_obj->name = action_name;
   action_def_obj->keybinding = keybinding;
   action_def_obj->action = action_cb;

   action_widget->action_objs = eina_list_append
                              (action_widget->action_objs, action_def_obj);

}

/**
 * @brief Returns number of implemented ATK Actions.
 * Implementation of AtkActionIface get_n_actions callback
 *
 * @param action object that implements AtkAction interface
 */
static int
eail_action_widget_n_actions_get(AtkAction *action)
{
   EailActionWidget *widget =  EAIL_ACTION_WIDGET(action);
   g_return_val_if_fail(widget, 0);

   return eina_list_count(widget->action_objs);
}

/**
 * @brief Returns EailActionObj of action with given index
 *
 * @param action object that implements AtkAction interface
 * @param i index (number) of action
 */
static EailActionObj *
eail_get_action_obj_i(AtkAction *action,
                      int i)
{
   EailActionWidget *widget =  EAIL_ACTION_WIDGET(action);
   g_return_val_if_fail(widget, NULL);
   g_return_val_if_fail(i < eina_list_count(widget->action_objs), NULL);

   EailActionObj *actionObj = eina_list_nth(widget->action_objs, i);

   return actionObj;
}

/**
 * @brief Returns EailActionObj of action with given index
 *
 * @param action object that implements AtkAction interface
 * @param i index (number) of action
 */
static const gchar*
eail_action_widget_action_name_get(AtkAction *action,
                                   int i)
{
   EailActionObj *actionObj = eail_get_action_obj_i(action, i);
   g_return_val_if_fail(actionObj, NULL);

   return actionObj->name;
}

/**
 * \brief Launches action with given index
 *
 * @param action object that implements AtkAction interface
 * @param i index (number) of action
 *
 * @returns TRUE if action was successfully launched, FALSE otherwise
 */
static gboolean
eail_action_widget_do_action(AtkAction *action,
                             int i)
{
   EailActionObj *actionObj = eail_get_action_obj_i(action, i);
   g_return_val_if_fail(actionObj, FALSE);

   return (actionObj->action)(action, NULL);
}

/**
 * \brief Gets description for action with given index
 * Implementation of AtkActionIface get_description callback
 *
 * @param action object that implements AtkAction interface
 * @param i index (number) of action
 *
 * @returns string representing description for action
 */
static const gchar*
eail_action_widget_description_get(AtkAction *action,
                                   int i)
{
   EailActionObj *actionObj = eail_get_action_obj_i(action, i);
   g_return_val_if_fail(actionObj, NULL);

   return actionObj->description;
}

/**
 * \brief Sets description for action with given index
 * Implementation of AtkActionIface set_description callback
 *
 * @param action object that implements AtkAction interface
 * @param i index (number) of action
 * @param description description to set
 *
 * @returns TRUE if update operation ended with success, FALSE otherwise
 */
static gboolean
eail_action_widget_description_set(AtkAction *action,
                                   int i,
                                   const char *description)
{
   EailActionObj *actionObj = eail_get_action_obj_i(action, i);
   g_return_val_if_fail(actionObj, FALSE);

   if (actionObj->description)
     g_free(actionObj->description);

   actionObj->description = g_strdup(description);
   return TRUE;
}

/**
 * \brief Initializes AtkAction interface
 *
 * @param iface interface to be filled
 */
static void
atk_action_interface_init(AtkActionIface *iface)
{
   g_return_if_fail(iface != NULL);

   iface->do_action       = eail_action_widget_do_action;
   iface->get_n_actions   = eail_action_widget_n_actions_get;
   iface->get_name        = eail_action_widget_action_name_get;
   iface->get_description = eail_action_widget_description_get;
   iface->set_description = eail_action_widget_description_set;
}
