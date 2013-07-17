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
 * @file eail_calendar.c
 * @brief EailCalendar implementation
 */

#include <Elementary.h>

#include "eail_calendar.h"

/**
 * @brief Initialization of AtkValue interface
 *
 * @param iface EailCalendar object
 */
static void atk_value_interface_init(AtkValueIface *iface);

/**
 * @brief Define EailCalendar GObject type
 */
G_DEFINE_TYPE_WITH_CODE(EailCalendar,
                        eail_calendar,
                        EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_VALUE,
                                              atk_value_interface_init))

/*
 * Implementation of the *AtkObject* interface
 */

/**
 * @brief EailCalendar object initialization
 *
 * @param obj EailCalendar object
 * @param data user set additional initialization data
 */
static void eail_calendar_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_calendar_parent_class)->initialize(obj, data);

   obj->role = ATK_ROLE_CALENDAR;
}

/**
 * @brief EailCalendar instance initialization
 *
 * @param calendar EailCalendar instance
 */
static void
eail_calendar_init(EailCalendar *calendar)
{
}

/**
 * @brief GObject type initialization function
 *
 * @param klass EailCalendar class
 */
static void
eail_calendar_class_init(EailCalendarClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);

   class->initialize = eail_calendar_initialize;
}

/*
 * Implementation of the *AtkValue* interface
 */

/**
 * @brief Gets obj current value
 *
 * @param obj EailCalendar instance
 * @param value EailCalendar current value
 */
static void
eail_calendar_get_current_value(AtkValue *obj,
                                GValue   *value)
{
   Evas_Object *widget;
   struct tm current;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   elm_calendar_selected_time_get(widget, &current);

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_INT);
   g_value_set_int(value, mktime(&current));
}

/**
 * @brief Gets obj maxiumum value
 *
 * @param obj EailCalendar instance
 * @param value EailCalendar maxiumum value
 */
static void
eail_calendar_get_maximum_value(AtkValue *obj,
                                GValue   *value)
{
   Evas_Object *widget;
   struct tm maximum;
   int min, max;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   elm_calendar_min_max_year_get(widget, &min, &max);

   memset(&maximum, 0, sizeof(maximum));

   if (min < max)
     {
        maximum.tm_year = max - 1900;
        maximum.tm_mon = 11;
        maximum.tm_mday = 31;
        maximum.tm_hour = 23;
        maximum.tm_min = 59;
        maximum.tm_sec = 59;
     }

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_INT);
   g_value_set_int(value, mktime(&maximum));
}

/**
 * @brief Gets obj minimum value
 *
 * @param obj EailCalendar instance
 * @param value EailCalendar minimum value
 */
static void
eail_calendar_get_minimum_value(AtkValue *obj,
                                GValue   *value)
{
   Evas_Object *widget;
   struct tm minimum;
   int min, max;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   elm_calendar_min_max_year_get(widget, &min, &max);

   memset(&minimum, 0, sizeof(minimum));

   if (min < max)
     {
        minimum.tm_year = min - 1900;
        minimum.tm_mon = 0;
        minimum.tm_mday = 1;
        minimum.tm_hour = 0;
        minimum.tm_min = 0;
        minimum.tm_sec = 0;
     }

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_INT);
   g_value_set_int(value, mktime(&minimum));
}

/**
 * @brief Sets obj value
 *
 * @param obj EailCalendar instance
 * @param value EailCalendar new value
 * @return TRUE if new value is set successfully, FALSE otherwise
 */
static gboolean
eail_calendar_set_current_value(AtkValue     *obj,
                                const GValue *value)
{
   Evas_Object *widget;
   struct tm minimum, maximum, current_set;
   int min, max;
   time_t time;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return FALSE;

   elm_calendar_min_max_year_get(widget, &min, &max);

   memset(&minimum, 0, sizeof(minimum));
   minimum.tm_year = min - 1900;
   minimum.tm_mon = 0;
   minimum.tm_mday = 1;
   minimum.tm_hour = 0;
   minimum.tm_min = 0;
   minimum.tm_sec = 0;

   memset(&maximum, 0, sizeof(maximum));
   maximum.tm_year = max - 1900;
   maximum.tm_mon = 11;
   maximum.tm_mday = 31;
   maximum.tm_hour = 23;
   maximum.tm_min = 59;
   maximum.tm_sec = 59;

   time = g_value_get_int(value);

   if ((min < max) && (mktime(&minimum) > time || mktime(&maximum) < time))
     return FALSE;

   localtime_r(&time, &current_set);
   elm_calendar_selected_time_set(widget, &current_set);

   return TRUE;
}

/**
 * @brief Gets obj minimum increment
 *
 * @param obj EailCalendar instance
 * @param value EailCalendar minimum increment
 */
static void
eail_calendar_get_minimum_increment(AtkValue *obj,
                                    GValue   *value)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_INT);
   g_value_set_int(value, 1);
}

static void
atk_value_interface_init(AtkValueIface *iface)
{
   g_return_if_fail(iface != NULL);

   iface->get_current_value     = eail_calendar_get_current_value;
   iface->get_maximum_value     = eail_calendar_get_maximum_value;
   iface->get_minimum_value     = eail_calendar_get_minimum_value;
   iface->set_current_value     = eail_calendar_set_current_value;
   iface->get_minimum_increment = eail_calendar_get_minimum_increment;
}
