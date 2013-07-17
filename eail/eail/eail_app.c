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
 * @file eail_app.c
 * @brief Implementation of elementary application
 */

#include <Elementary.h>

#include "eail_app.h"
#include "eail_factory.h"

/*This is defined and filled in elementary.*/
extern Eina_List *_elm_win_list;

/**
 * @def Definition of EailApp as GObject
 *
 * EailApp is extended ATK_TYPE_OBJECT
 */
G_DEFINE_TYPE(EailApp, eail_app, ATK_TYPE_OBJECT)


/**
 * @brief Initializer for AtkObjectClass
 */
static void
eail_app_initialize(AtkObject *obj, gpointer data)
{
   int argc;
   char **argv;

   ATK_OBJECT_CLASS(eail_app_parent_class)->initialize(obj, data);

   ecore_app_args_get(&argc, &argv);
   obj->name = argv ? (gchar *)ecore_file_file_get(argv[0]) : "";
   obj->role = ATK_ROLE_APPLICATION;
   obj->accessible_parent = NULL;
}

/**
 * @brief Destructor for EailApp object
 */
static void
eail_app_class_finalize(GObject *obj)
{
   G_OBJECT_CLASS(eail_app_parent_class)->finalize(obj);
}

/**
 * @brief Implementation AtkObject->get_n_children callback
 *
 * ATK doc says:\n
 * Gets the number of accessible children of the accessible.
 *
 * @returns an integer representing the number of accessible children of
 * the accessible
 */
static gint
eail_app_get_n_children(AtkObject *obj)
{
   return eina_list_count(_elm_win_list);
}

/**
 * @brief Implementation AtkObject->ref_child callback
 *
 * ATK doc says:\n
 * Gets a reference to the specified accessible child of the object. The
 * accessible children are 0-based so the first accessible child is at index 0,
 * the second at index 1 and so on.
 *
 * @returns an AtkObject representing the specified accessible child of the
 * accessible.
 */
static AtkObject *
eail_app_ref_child(AtkObject *obj, gint i)
{
   AtkObject *child = NULL;

   if (eina_list_count(_elm_win_list) > i)
     {
        Evas_Object *tmp = eina_list_nth(_elm_win_list, i);
        child = eail_factory_get_accessible(tmp);
        g_object_ref(child);
     }

   return child;
}

/**
 * @brief Initializer for GObject class
 */
static void
eail_app_init(EailApp *app)
{
}

/**
 * @brief Initializer for GObject EailAppClass class (defines callbacks for base
 * AtkObject)
 */
static void
eail_app_class_init(EailAppClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   GObjectClass *g_object_class = G_OBJECT_CLASS(klass);

   class->initialize = eail_app_initialize;
   class->get_n_children = eail_app_get_n_children;
   class->ref_child = eail_app_ref_child;
   class->get_parent = NULL;

   g_object_class->finalize = eail_app_class_finalize;
}
