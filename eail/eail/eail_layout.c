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
 * @file eail_layout.c
 *
 * @brief Implementation of Layout widget
*/

#include <Elementary.h>

#include "eail_layout.h"
#include "eail_priv.h"

/**
 * @def Define EailLayout GObject type
 *
 * It extends EAIL_TYPE_WIDGET
 */
G_DEFINE_TYPE(EailLayout, eail_layout, EAIL_TYPE_WIDGET)

/**
 * @brief Get elementary widgets from layout edje object
 * Iterate over edje smart objects and to list only elementary widgets
 *
 * @param edje layout edje object
 *
 * @returns list of elementary widgets
 */
static Eina_List *
_get_widgets_from_edje(const Evas_Object *edje)
{
   Evas_Object *o;
   Eina_List *widgets, *l, *list = evas_object_smart_members_get(edje);

   widgets = NULL;
   EINA_LIST_FOREACH(list, l, o)
     {
        DBG("type of layout content %s", evas_object_type_get(o));
        if (elm_object_widget_check(o))
          widgets = eina_list_append(widgets, o);
     }

   return widgets;
}

/**
 * @brief Implementations of get_widget_children from EailWidget
 *
 * @param object EailLayout instance
 *
 * @returns list of Evas_Objects
 */
static Eina_List*
eail_layout_widget_children_get(EailWidget *object)
{
   Evas_Object *edje;
   Evas_Object *widget = eail_widget_get_widget(object);

   if (!widget) return NULL;
   edje = elm_layout_edje_get(widget);

   return _get_widgets_from_edje(edje);
}

/**
 * @brief Initialize EailLayout object
 *
 * @param object EailLayout instance
 * @param data user data passed to initialization
 */
static void
eail_layout_initialize(AtkObject *object, gpointer data)
{
   ATK_OBJECT_CLASS(eail_layout_parent_class)->initialize(object, data);
   object->role = ATK_ROLE_FILLER;
}

/**
 * @brief Init EailLayout instance
 *
 * @param layout EailLayout instance
 */
static void
eail_layout_init(EailLayout *layout)
{
}

/**
 * @brief Init EailLayout class
 *
 * @param klass EailLayoutClass object
 */
static void
eail_layout_class_init(EailLayoutClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   EailWidgetClass *widget_class = EAIL_WIDGET_CLASS(klass);

   class->initialize = eail_layout_initialize;
   widget_class->get_widget_children = eail_layout_widget_children_get;
}
