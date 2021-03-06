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
 * @file eail_button.c
 * @brief EailButton implementation
 */

#include <Elementary.h>

#include "eail_button.h"
#include "eail_factory.h"
#include "eail_priv.h"

/**
 * @brief Number of actions implemented
 */
#define ACTIONS_NUMBER 3

/**
 * @brief Initialization of AtkImage interface
 *
 * @param iface EailButton object
 */
static void atk_image_interface_init(AtkImageIface *iface);

/**
 * @brief Initialization of AtkAction interface
 *
 * @param iface EailButton object
 */
static void atk_action_interface_init(AtkActionIface *iface);

/**
 * @brief Define EailButton GObject type
 */
G_DEFINE_TYPE_WITH_CODE(EailButton,
                        eail_button,
                        EAIL_TYPE_TEXT,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_IMAGE,
                                              atk_image_interface_init)
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_ACTION,
                                              atk_action_interface_init))

/*
 * Implementation of the *AtkObject* interface
 */

/**
 * @brief EailButton object initialization
 *
 * @param obj EailButton object
 * @param data user set additional initialization data
 */
static void
eail_button_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_button_parent_class)->initialize(obj, data);

   obj->role = ATK_ROLE_PUSH_BUTTON;
}

/**
 * @brief Class destructor
 *
 * @param object object instance
 */
static void
eail_button_finalize(GObject *object)
{
   EailButton *button = EAIL_BUTTON(object);

   if (button->click_description) free(button->click_description);
   if (button->press_description) free(button->press_description);
   if (button->release_description) free(button->release_description);

   G_OBJECT_CLASS(eail_button_parent_class)->finalize(object);
}

/**
 * @brief EailButton instance initialization
 *
 * @param button EailButton instance
 */
static void
eail_button_init(EailButton *button)
{
   button->click_description = NULL;
   button->press_description = NULL;
   button->release_description = NULL;
}

/**
 * @brief Gets list of child widget
 *
 * @param widget EailWidget object
 *
 * @return list of child, NULL if no children
 */
static Eina_List *
eail_button_get_widget_children(EailWidget *widget)
{
   Eina_List *list = NULL;
   Evas_Object *child, *obj;

   obj = eail_widget_get_widget(EAIL_WIDGET(widget));
   if (obj)
     {
        child = elm_object_part_content_get(obj, "icon");
        if (child && elm_object_widget_check(child))
          list = eina_list_append(list, child);
     }

   return list;
}

/**
 * @brief GObject type initialization function
 *
 * @param klass EailButton class
 */
static void
eail_button_class_init(EailButtonClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   EailWidgetClass *widget_class = EAIL_WIDGET_CLASS(klass);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);

   class->initialize = eail_button_initialize;
   widget_class->get_widget_children = eail_button_get_widget_children;
   gobject_class->finalize = eail_button_finalize;
}

/*
 * Implementation of the *AtkImage* interface
 */

/**
 * @brief Implementation of get_image_position from AtkImage interface
 *
 * @param image EailButton instance
 * @param x horizontal coordinate
 * @param y vertical coordinate
 * @param coord_type coord type
 */
static void
eail_button_get_image_position(AtkImage     *image,
                               gint         *x,
                               gint         *y,
                               AtkCoordType  coord_type)
{
   Evas_Object *button_image;
   Evas_Object *widget;
   AtkObject *obj;

   widget = eail_widget_get_widget(EAIL_WIDGET(image));
   if (!widget)
     {
        *x = G_MININT;
        *y = G_MININT;
        return;
     }

   button_image = elm_object_part_content_get(widget, "icon");
   if (button_image)
     {
        obj = eail_factory_get_accessible(button_image);
        atk_image_get_image_position(ATK_IMAGE(obj), x, y, coord_type);
     }
   else
     {
        *x = G_MININT;
        *y = G_MININT;
     }
}

/**
 * @brief Implementation of get_image_size from AtkImage interface
 *
 * @param image EailButton instance
 * @param width width image
 * @param height height image
 */
static void
eail_button_get_image_size(AtkImage *image,
                           gint     *width,
                           gint     *height)
{
   Evas_Object *widget;
   Evas_Object *button_image;
   AtkObject *obj;

   widget = eail_widget_get_widget(EAIL_WIDGET(image));
   if (!widget)
     {
        *width = -1;
        *height = -1;
        return;
     }

   button_image = elm_object_part_content_get(widget, "icon");
   if (button_image)
     {
        obj = eail_factory_get_accessible(button_image);
        atk_image_get_image_size(ATK_IMAGE(obj), width, height);
     }
   else
     {
        *width = -1;
        *height = -1;
     }
}

static void
atk_image_interface_init(AtkImageIface *iface)
{
   iface->get_image_position = eail_button_get_image_position;
   iface->get_image_size     = eail_button_get_image_size;
   //iface->get_image_description = eail_button_get_image_description;
   //iface->set_image_description = eail_button_set_image_description;
}

/*
 * Implementation of the *AtkAction* interface
 */

/**
 * @brief Implementation of get_n_actions from AtkAction interface
 *
 * @param action EailButton instance
 */
static int
eail_button_n_actions_get(AtkAction *action)
{
   return ACTIONS_NUMBER;
}

/**
 * @brief Implementation of get_description from AtkAction interface
 *
 * @param action EailButton instance
 * @param i action index
 *
 * @return action description
 */
static const char*
eail_button_description_get(AtkAction *action,
                            gint i)
{
   const char *action_description;
   EailButton *button = EAIL_BUTTON(action);

   switch (i)
     {
      case 0:
         action_description = button->click_description;
         break;
      case 1:
         action_description = button->press_description;
         break;
      case 2:
         action_description = button->release_description;
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
 * @param action EailButton instance
 * @param i action index
 * @param description action description
 *
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
eail_button_description_set(AtkAction *action,
                            gint i,
                            const char *description)
{
   EailButton *button = EAIL_BUTTON(action);
   char **value;

   switch (i)
     {
      case 0:
         value = &button->click_description;
         break;
      case 1:
         value = &button->press_description;
         break;
      case 2:
         value = &button->release_description;
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
 * @param action EailButton instance
 * @param i action index
 *
 * @return action name
 */
static const char*
eail_button_action_name_get(AtkAction *action,
                            int i)
{
   const char* action_name;

   switch (i)
     {
      case 0:
         action_name = "click";
         break;
      case 1:
         action_name = "press";
         break;
      case 2:
         action_name = "release";
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
 * @param action EailButton instance
 * @param i action index
 *
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
eail_button_do_action(AtkAction *action,
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
   else if (!g_strcmp0(action_name, "press"))
     evas_object_smart_callback_call(widget, "pressed", NULL);
   else if (!g_strcmp0(action_name, "release"))
     evas_object_smart_callback_call(widget, "unpressed", NULL);
   else
     return FALSE;

   return TRUE;
}

static void
atk_action_interface_init(AtkActionIface *iface)
{
   g_return_if_fail(iface != NULL);

   iface->get_n_actions   = eail_button_n_actions_get;
   iface->get_description = eail_button_description_get;
   iface->set_description = eail_button_description_set;
   iface->get_name        = eail_button_action_name_get;
   iface->do_action       = eail_button_do_action;
}
