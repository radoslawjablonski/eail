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
 * @file eail_hover.c
 * @brief EailHover implementation.
 */

#include <Elementary.h>

#include "eail_hover.h"
#include "eail_factory.h"

/**
 * @brief Number of implemented actions.
 */
#define HOVER_ACTIONS_COUNT 1

static void atk_action_interface_init(AtkActionIface *iface);

/**
 * @brief EailHover type definition
 */
G_DEFINE_TYPE_WITH_CODE(EailHover,
                        eail_hover,
                        EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_ACTION,
                                              atk_action_interface_init));

/* AtkAction */
static gboolean eail_hover_action_do_action(AtkAction *action,
                                            gint i);
static gint eail_hover_action_get_n_actions(AtkAction *action);
static const gchar *eail_hover_action_get_description(AtkAction *action,
                                                      gint i);
static const gchar *eail_hover_action_get_name(AtkAction *action,
                                               gint i);
static gboolean eail_hover_action_set_description(AtkAction *action,
                                                  gint i,
                                                  const gchar *desc);

/* AtkObject */
static AtkObject *eail_hover_get_parent(AtkObject *accessible);
static void eail_hover_initialize(AtkObject *obj, gpointer data);
static void eail_hover_finalize(GObject *obj);

/*
 * Implementation of the *AtkObject* interface
 */

/**
 * @brief Gets accessible parent of accessible.
 *
 * @param accessible an AtkObject
 * @return accessible parent
 */
static AtkObject *
eail_hover_get_parent(AtkObject *accessible)
{
   Evas_Object *hover = NULL;
   Evas_Object *hover_parent = NULL;
   AtkObject *parent = NULL;

   hover = eail_widget_get_widget(EAIL_WIDGET(accessible));
   if (!hover) return NULL;

   hover_parent = elm_hover_parent_get(hover);
   if (!hover_parent) return NULL;

   parent = eail_factory_get_accessible(hover_parent);

   return parent;
}

/**
 * @brief EailHover initializer
 */
static void
eail_hover_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_hover_parent_class)->initialize(obj, data);
   obj->role = ATK_ROLE_GLASS_PANE;
}

/**
 * @brief EailHover finalizer.
 *
 * Destroys object and allocated resources.
 *
 * @param obj a GObject
 */
static void
eail_hover_finalize(GObject *obj)
{
    EailHover *hover = EAIL_HOVER(obj);

    if (hover->clicked_description) free(hover->clicked_description);

    G_OBJECT_CLASS(eail_hover_parent_class)->finalize(obj);
}

/**
 * @brief EailHover instance initializer.
 */
static void
eail_hover_init(EailHover *hover)
{
   hover->clicked_description = NULL;
}

/**
 * @brief EailHover class initializer.
 */
static void
eail_hover_class_init(EailHoverClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);

   class->initialize = eail_hover_initialize;
   class->get_parent = eail_hover_get_parent;

   gobject_class->finalize = eail_hover_finalize;
}

/*
 * Implementation of the *AtkAction* interface
 */

/**
 * @brief AtkAction interface initializer.
 */
static void
atk_action_interface_init(AtkActionIface *iface)
{
   g_return_if_fail(iface != NULL);

   iface->do_action          = eail_hover_action_do_action;
   iface->get_n_actions      = eail_hover_action_get_n_actions;
   iface->get_description    = eail_hover_action_get_description;
   iface->get_name           = eail_hover_action_get_name;
   iface->set_description    = eail_hover_action_set_description;
}

/**
 * @brief Performs i-th action.
 *
 * @param action an AtkAction
 * @param i the action index corresponding to the action to be performed
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
eail_hover_action_do_action(AtkAction *action,
                            gint i)
{
   Evas_Object *widget = NULL;
   const char *action_name = NULL;

   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   if (!widget) return FALSE;

   if ((elm_object_disabled_get(widget)) || (!evas_object_visible_get(widget)))
     return FALSE;

   action_name = atk_action_get_name(action, i);
   if (!action_name) return FALSE;

   evas_object_smart_callback_call(widget, action_name, NULL);

   return TRUE;
}

/**
 * @brief Gets number of actions available on the object.
 *
 * @param action an AtkAction
 * @return actions number
 */
static gint
eail_hover_action_get_n_actions(AtkAction *action)
{
   return HOVER_ACTIONS_COUNT;
}

/**
 * @brief Gets i-th action description.
 *
 * @param action an AtkAction
 * @param i action number
 * @return action description string
 */
static const gchar *
eail_hover_action_get_description(AtkAction *action,
                                  gint i)
{
   const char *action_description = NULL;
   EailHover *hover;

   hover = EAIL_HOVER(action);
   if (!hover) return NULL;

   switch (i)
     {
      case 0:
         action_description = hover->clicked_description;
         break;

      default:
         action_description = NULL;
         break;
     }

   return action_description;
}

/**
 * @brief Gets i-th action name.
 *
 * @param action an AtkAction
 * @param i action number
 * * @return action description string
 */
static const gchar *
eail_hover_action_get_name(AtkAction *action,
                           gint i)
{
   const char* action_name = NULL;

   switch (i)
     {
      case 0:
         /* user clicked the empty space in the hover to dismiss */
         action_name = "clicked";
         break;

      default:
         action_name = NULL;
         break;
     }

   return action_name;
}

/**
 * @brief Sets i-th action description.
 *
 * @param action an AtkAction
 * @param i action number
 * @param desc action description
 * @return TRUE if description is set successfully, FALSE otherwise
 */
static gboolean
eail_hover_action_set_description(AtkAction *action,
                                  gint i,
                                  const gchar *desc)
{
   EailHover *hover = NULL;
   char **value = NULL;

   hover = EAIL_HOVER(action);
   if (!hover) return FALSE;

   switch (i)
     {
      case 0:
         value = &hover->clicked_description;
         break;

      default:
         value = NULL;
         break;
     }

   if (value)
     {
        free(*value);
        *value = g_strdup(desc);

        return TRUE;
     }

   return FALSE;
}

