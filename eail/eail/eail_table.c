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
 * @file eail_table.c
 * @brief Implementation of elm_table as AtkObject
 */
#include <Elementary.h>

#include "eail_table.h"

#define TABLE_SMART_TYPE "Evas_Object_Table"

G_DEFINE_TYPE(EailTable, eail_table, EAIL_TYPE_WIDGET)

/**
 * @brief Implementation of get_widget_childreb from EailWidget
 *
 * @param widget EailTable instance
 *
 * @returns list of widgets
 */
static Eina_List *
eail_table_children_get(EailWidget *widget)
{
   Evas_Object *table, *object;
   Eina_List *smart_members;

   g_return_val_if_fail(EAIL_IS_TABLE(widget), NULL);

   object = eail_widget_get_widget(widget);

   if (!object) return NULL;

   smart_members = evas_object_smart_members_get(object);
   /*table has one smart member it is Evas_Object_Table*/
   table = eina_list_nth(smart_members, 0);
   eina_list_free(smart_members);

   if (evas_object_smart_type_check(table, TABLE_SMART_TYPE))
     {
        return evas_object_table_children_get(table);
     }

   return NULL;
}

/**
 * @brief EailTable init
 *
 * @param table EailTable instance
 */
static void
eail_table_init(EailTable *table)
{
}

/**
 * @brief EailTable initialize
 *
 * @param object EailTable instance
 * @param data user data passed to initialize
 */
static void
eail_table_initialize(AtkObject *object, gpointer data)
{
   ATK_OBJECT_CLASS(eail_table_parent_class)->initialize(object, data);

   object->role = ATK_ROLE_FILLER;
}

/**
 * @brief EailTable class initialize
 *
 * @param klass EailTable class
 */
static void
eail_table_class_init(EailTableClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   EailWidgetClass *widget_class = EAIL_WIDGET_CLASS(klass);

   widget_class->get_widget_children = eail_table_children_get;

   class->initialize = eail_table_initialize;
}

