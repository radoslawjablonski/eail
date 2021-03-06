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
 * @file eail_progressbar.c
 * @brief EailProgressbar implementation
 */

#include <Elementary.h>

#include "eail_progressbar.h"

/**
 * @brief Initialization of AtkValue interface
 *
 * @param iface EailProgressbar object
 */
static void atk_value_interface_init(AtkValueIface *iface);

/**
 * @brief Define EailProgressbar GObject type
 */
G_DEFINE_TYPE_WITH_CODE(EailProgressbar,
                        eail_progressbar,
                        EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_VALUE, atk_value_interface_init))

/*
 * Implementation of the *AtkObject* interface
 */

/**
 * @brief EailProgressbar object initialization
 *
 * @param obj EailProgressbar object
 * @param data user set additional initialization data
 */
static void
eail_progressbar_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_progressbar_parent_class)->initialize(obj, data);

   obj->role = ATK_ROLE_PROGRESS_BAR;
}

/**
 * @brief Gets list of child widget
 *
 * @param widget EailWidget object
 *
 * @return list of child, NULL if no children
 */
static Eina_List *
eail_progressbar_get_widget_children(EailWidget *widget)
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
 * @brief Gets slider name
 *
 * @param obj EailSlider instance
 */
static const gchar*
eail_progressbar_get_name(AtkObject *obj)
{
   const gchar *name;
   Evas_Object *widget;

   name = ATK_OBJECT_CLASS(eail_progressbar_parent_class)->get_name(obj);
   if (name) return name;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (widget)
     name = (const gchar*)elm_object_text_get(widget);

   return name;
}

/**
 * @brief EailProgressbar instance initialization
 *
 * @param progressbar EailProgressbar instance
 */
static void
eail_progressbar_init(EailProgressbar *progressbar)
{
}

/**
 * @brief GObject type initialization function
 *
 * @param klass EailProgressbar class
 */
static void
eail_progressbar_class_init(EailProgressbarClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   EailWidgetClass *widget_class = EAIL_WIDGET_CLASS(klass);

   class->initialize = eail_progressbar_initialize;
   class->get_name = eail_progressbar_get_name;
   widget_class->get_widget_children = eail_progressbar_get_widget_children;
}

/*
 * Implementation of the *AtkValue* interface
 */

/**
 * @brief Gets obj current value
 *
 * @param obj EailProgressbar instance
 * @param value EailProgressbar current value
 */
static void
eail_progressbar_get_current_value(AtkValue *obj,
                                   GValue   *value)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_DOUBLE);
   g_value_set_double(value, elm_progressbar_value_get(widget));
}

/**
 * @brief Gets obj maxiumum value
 *
 * @param obj EailProgressbar instance
 * @param value EailProgressbar maxiumum value
 */
static void
eail_progressbar_get_maximum_value(AtkValue *obj,
                                   GValue   *value)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_DOUBLE);
   g_value_set_double(value, 1.0);
}

/**
 * @brief Gets obj minimum value
 *
 * @param obj EailProgressbar instance
 * @param value EailProgressbar minimum value
 */
static void
eail_progressbar_get_minimum_value(AtkValue *obj,
                                   GValue   *value)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_DOUBLE);
   g_value_set_double(value, 0.0);
}

/**
 * @brief Sets obj value
 *
 * @param obj EailProgressbar instance
 * @param value EailProgressbar new value
 * @return TRUE if new value is set successfully, FALSE otherwise
 */
static gboolean
eail_progressbar_set_current_value(AtkValue *obj,
                                   const GValue *value)
{
   Evas_Object *widget;
   double val;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return FALSE;

   val = g_value_get_double(value);

   if (0.0 > val || 1.0 < val)
     return FALSE;

   elm_progressbar_value_set(widget, val);

   return TRUE;
}

/**
 * @brief Gets obj minimum increment
 *
 * @param obj EailProgressbar instance
 * @param value EailProgressbar minimum increment
 */
static void
eail_progressbar_get_minimum_increment(AtkValue *obj,
                                       GValue *value)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_DOUBLE);
   g_value_set_double(value, G_MINDOUBLE);
}

static void
atk_value_interface_init(AtkValueIface *iface)
{
   g_return_if_fail(iface != NULL);

   iface->get_current_value     = eail_progressbar_get_current_value;
   iface->get_maximum_value     = eail_progressbar_get_maximum_value;
   iface->get_minimum_value     = eail_progressbar_get_minimum_value;
   iface->set_current_value     = eail_progressbar_set_current_value;
   iface->get_minimum_increment = eail_progressbar_get_minimum_increment;
}
