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
 * @file eail_notify.c
 * @brief Implementation of notify widget
 */

#include <atk/atk.h>
#include <Elementary.h>

#include "eail_notify.h"
#include "eail_factory.h"
#include "eail_priv.h"

/**
 * @brief Definition of EailPopup as GObject
 *
 * EailNotify is extended EAIL_TYPE_WIDGET
 */
G_DEFINE_TYPE(EailNotify, eail_notify, EAIL_TYPE_WIDGET);


/**
 * @brief Initializer for AtkObject
 */
static void
eail_notify_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_notify_parent_class) ->initialize(obj, data);
   obj->role = ATK_ROLE_NOTIFICATION;
}

/**
 * @brief Initializer for GObject EailNotify instance
 */
static void
eail_notify_init(EailNotify *button)
{

}

/**
 * @brief Helper function to getting nested content in notify widget
 *
 * @param obj an AtkObject
 *
 * @returns nested widget content from notify widget
 */
static Evas_Object *
_eail_get_nested_widget(AtkObject *obj)
{
   Evas_Object *notify_widget = NULL, *nested_widget = NULL;
   /* getting widget of notify class */
   notify_widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!notify_widget)
     {
        ERR("No widget found for notification object!");
     }

   nested_widget = elm_object_part_content_get(notify_widget, "default");

   return nested_widget;
}

/**
 * @brief Implementation AtkObject->get_n_children callback
 *
 * ATK doc says:\n
 * Gets the number of accessible children of the accessible.
 * @param obj an AtkObject
 *
 * @returns an integer representing the number of accessible children of
 * the accessible
 */
static gint
eail_notify_get_n_children(AtkObject *obj)
{
   Evas_Object *nested_widget = NULL;

   nested_widget = _eail_get_nested_widget(obj);
   if (nested_widget)
     return 1;

   return 0;
}

/**
 * @brief Implementation AtkObject->ref_child callback
 *
 * ATK doc says:\n
 * Gets a reference to the specified accessible child of the object. The
 * accessible children are 0-based so the first accessible child is at index 0,
 * the second at index 1 and so on.
 *
 * @param obj an AtkObject
 * @param i index of child to ref
 *
 * @returns an AtkObject representing the specified accessible child of the
 * accessible.
 */
static AtkObject *
eail_notify_ref_child(AtkObject *obj, gint i)
{
   Evas_Object *nested_widget = NULL;
   AtkObject *atk_obj;

   nested_widget = _eail_get_nested_widget(obj);
   atk_obj = eail_factory_get_accessible(nested_widget);

   if (atk_obj)
     g_object_ref(atk_obj);

   return atk_obj;
}

/**
 * @brief Destructor for EailNotify object
 */
static void
eail_notify_finalize(GObject *object)
{
   G_OBJECT_CLASS(eail_notify_parent_class)->finalize(object);
}

/**
 * @brief Initializer for EailNotify GObject class (defines callbacks for
 * base AtkObject)
 */
static void
eail_notify_class_init(EailNotifyClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   class->initialize = eail_notify_initialize;
   class->get_n_children = eail_notify_get_n_children;
   class->ref_child = eail_notify_ref_child;
   gobject_class->finalize = eail_notify_finalize;


}
