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
 * @file eail_slider.c
 * @brief EailSlider implementation
 */

#include <Elementary.h>

#include "eail_slider.h"

/*
 * Implementation of the *AtkObject* interface
 */

/**
 * @brief Initialization of AtkValue interface
 *
 * @param iface EailSlider object
 */
static void atk_value_interface_init(AtkValueIface *iface);

/**
 * @brief Define EailSlider GObject type
 */
G_DEFINE_TYPE_WITH_CODE(EailSlider,
                        eail_slider,
                        EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_VALUE, atk_value_interface_init))

/**
 * @brief EailSlider object initialization
 *
 * @param obj EailSlider object
 * @param data user set additional initialization data
 */
static void
eail_slider_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_slider_parent_class)->initialize(obj, data);

   obj->role = ATK_ROLE_SLIDER;
}

/**
 * @brief Gets list of child widget
 *
 * @param widget EailWidget object
 *
 * @return list of child, NULL if no children
 */
static Eina_List *
eail_slider_get_widget_children(EailWidget *widget)
{
   Eina_List *list = NULL;
   Evas_Object *child, *obj;

   obj = eail_widget_get_widget(EAIL_WIDGET(widget));
   if (obj)
     {
        child = elm_object_part_content_get(obj, "icon");
        if (child && elm_object_widget_check(child))
          list = eina_list_append(list, child);

        child = elm_object_part_content_get(obj, "end");
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
eail_slider_get_name(AtkObject *obj)
{
   const gchar *name;
   Evas_Object *widget;

   name = ATK_OBJECT_CLASS(eail_slider_parent_class)->get_name(obj);
   if (name) return name;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (widget)
     name = (const gchar*)elm_object_text_get(widget);

   return name;
}

/**
 * @brief EailSlider instance initialization
 *
 * @param slider EailSlider instance
 */
static void
eail_slider_init(EailSlider *slider)
{
}

/**
 * @brief GObject type initialization function
 *
 * @param klass EailSlider class
 */
static void
eail_slider_class_init(EailSliderClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   EailWidgetClass *widget_class = EAIL_WIDGET_CLASS(klass);

   widget_class->get_widget_children = eail_slider_get_widget_children;
   class->initialize = eail_slider_initialize;
   class->get_name = eail_slider_get_name;
}

/*
 * Implementation of the *AtkValue* interface
 */

/**
 * @brief Gets obj current value
 *
 * @param obj EailSlider instance
 * @param value EailSlider current value
 */
static void
eail_slider_get_current_value(AtkValue *obj,
                              GValue   *value)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_DOUBLE);
   g_value_set_double(value, elm_slider_value_get(widget));
}

/**
 * @brief Gets obj maxiumum value
 *
 * @param obj EailSlider instance
 * @param value EailSlider maxiumum value
 */
static void
eail_slider_get_maximum_value(AtkValue *obj,
                              GValue   *value)
{
   Evas_Object *widget;
   double max;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_DOUBLE);
   elm_slider_min_max_get(widget, NULL, &max);
   g_value_set_double(value, max);
}

/**
 * @brief Gets obj minimum value
 *
 * @param obj EailSlider instance
 * @param value EailSlider minimum value
 */
static void
eail_slider_get_minimum_value(AtkValue *obj,
                              GValue   *value)
{
   Evas_Object *widget;
   double min;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_DOUBLE);
   elm_slider_min_max_get(widget, &min, NULL);
   g_value_set_double(value, min);
}

/**
 * @brief Sets obj value
 *
 * @param obj EailSlider instance
 * @param value EailSlider new value
 * @return TRUE if new value is set successfully, FALSE otherwise
 */
static gboolean
eail_slider_set_current_value(AtkValue     *obj,
                              const GValue *value)
{
   Evas_Object *widget;
   double min, max, val;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return FALSE;

   val = g_value_get_double(value);
   elm_slider_min_max_get(widget, &min, &max);

   if (min > val || max < val)
     return FALSE;

   elm_slider_value_set(widget, val);

   return TRUE;
}

/**
 * @brief Gets obj minimum increment
 *
 * @param obj EailSlider instance
 * @param value EailSlider minimum increment
 */
static void
eail_slider_get_minimum_increment(AtkValue *obj,
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

   iface->get_current_value     = eail_slider_get_current_value;
   iface->get_maximum_value     = eail_slider_get_maximum_value;
   iface->get_minimum_value     = eail_slider_get_minimum_value;
   iface->set_current_value     = eail_slider_set_current_value;
   iface->get_minimum_increment = eail_slider_get_minimum_increment;
}
