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
 * @file eail_plug.c
 * @brief EailPlug implementation
 */

#include <Elementary.h>

#include "eail_plug.h"

/**
 * @brief Number of actions implemented
 */
#define ACTIONS_NUMBER 1

/**
 * @brief Initialization of AtkAction interface
 *
 * @param iface EailPlug object
 */
static void atk_action_interface_init(AtkActionIface *iface);

G_DEFINE_TYPE_WITH_CODE(EailPlug,
                        eail_plug,
                        EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_ACTION,
                                              atk_action_interface_init))

/*
 * Implementation of the *AtkObject* interface
 */

/**
 * @brief EailPlug object initialization
 *
 * @param obj EailPlug object
 * @param data user set additional initialization data
 */
static void
eail_plug_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_plug_parent_class)->initialize(obj, data);

   obj->role = ATK_ROLE_IMAGE;
}

/**
 * @brief Class destructor
 *
 * @param object object instance
 */
static void
eail_plug_finalize(GObject *object)
{
   EailPlug *plug = EAIL_PLUG(object);

   if (plug->click_description) free(plug->click_description);

   G_OBJECT_CLASS(eail_plug_parent_class)->finalize(object);
}

/**
 * @brief EailPlug instance initialization
 *
 * @param plug EailPlug instance
 */
static void
eail_plug_init(EailPlug *plug)
{
   plug->click_description = NULL;
}

/**
 * @brief GObject type initialization function
 *
 * @param klass EailPlug class
 */
static void
eail_plug_class_init(EailPlugClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);

   class->initialize = eail_plug_initialize;
   gobject_class->finalize = eail_plug_finalize;
}

/*
 * Implementation of the *AtkAction* interface
 */

/**
 * @brief Implementation of get_n_actions from AtkAction interface
 *
 * @param action EailPlug instance
 */
static int
eail_plug_n_actions_get(AtkAction *action)
{
   return ACTIONS_NUMBER;
}

/**
 * @brief Implementation of get_description from AtkAction interface
 *
 * @param action EailPlug instance
 * @param i action index
 *
 * @return action description
 */
static const char*
eail_plug_description_get(AtkAction *action,
                            gint i)
{
   const char *action_description;
   EailPlug *plug = EAIL_PLUG(action);

   switch (i)
     {
      case 0:
         action_description = plug->click_description;
         break;
      default:
         action_description = NULL;
         break;
     }

   return action_description;
}

/**
 * @brief Implementation of set_descritpion from AtkAction interface
 *
 * @param action EailPlug instance
 * @param i action index
 * @param description action description
 *
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
eail_plug_description_set(AtkAction *action,
                            gint i,
                            const char *description)
{
   EailPlug *plug = EAIL_PLUG(action);
   char **value;

   switch (i)
     {
      case 0:
         value = &plug->click_description;
         break;
      default:
         value = NULL;
         break;
     }

   if (value)
     {
        free(*value);
        *value = g_strdup(description);
        return TRUE;
     }

   return FALSE;
}

/**
 * @brief Implementation of get_name from AtkAction interface
 *
 * @param action EailPlug instance
 * @param i action index
 *
 * @return action name
 */
static const char*
eail_plug_action_name_get(AtkAction *action,
                            int i)
{
   const char* action_name;

   switch (i)
     {
      case 0:
         action_name = "click";
         break;
      default:
         action_name = NULL;
         break;
     }

   return action_name;
}

/**
 * @brief Implementation of do_action from AtkAction interface
 *
 * @param action EailPlug instance
 * @param i action index
 *
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
eail_plug_do_action(AtkAction *action,
                      int i)
{
   const char *action_name;
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   if (!widget) return FALSE;

   if ((elm_object_disabled_get(widget)) || (!evas_object_visible_get(widget)))
     return FALSE;

   action_name = atk_action_get_name(action, i);
   if (!action_name) return FALSE;

   if (!g_strcmp0(action_name, "click"))
     evas_object_smart_callback_call(widget, "clicked", NULL);
   else
     return FALSE;

   return TRUE;
}

static void
atk_action_interface_init(AtkActionIface *iface)
{
   g_return_if_fail(iface != NULL);

   iface->get_n_actions   = eail_plug_n_actions_get;
   iface->get_description = eail_plug_description_get;
   iface->set_description = eail_plug_description_set;
   iface->get_name        = eail_plug_action_name_get;
   iface->do_action       = eail_plug_do_action;
}
