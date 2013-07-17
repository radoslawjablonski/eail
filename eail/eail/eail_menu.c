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
 * @file eail_menu.c
 * @brief Implementation of menu widget
 */

#include <Elementary.h>

#include "eail_menu.h"
#include "eail_item_parent.h"
#include "eail_item.h"
#include "eail_menu_item.h"
#include "eail_priv.h"

static void
eail_item_parent_interface_init(EailItemParentIface *iface);

/**
 * @brief definition of EailMenu as GObject
 *
 * EailList is extended Widget with EAIL_TYPE_ITEM_PARENT implemented
 */
G_DEFINE_TYPE_WITH_CODE(EailMenu,
                        eail_menu,
                        EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(EAIL_TYPE_ITEM_PARENT,
                                              eail_item_parent_interface_init))

/*
 * Implementation of the *AtkObject* interface
 */

/**
 * @brief Initializer for AtkObjectClass
 */
static void
eail_menu_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_menu_parent_class)->initialize(obj, data);

   obj->role = ATK_ROLE_MENU;
}

/**
 * @brief Prepares Eina_List filled with Elm_Object_Item* objects
 * representing items in menu
 *
 * @return filled list with list items. Call eina_list_free on that list when
 * results processing has been finished
 */
static Eina_List *
eail_menu_get_items(EailMenu *list)
{
   Eina_List *items = NULL;
   Elm_Object_Item *item;
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(list));

   item = elm_menu_first_item_get(widget);
   while (item)
     {
        items = eina_list_append(items, item);
        item = elm_menu_item_next_get(item);
     }

   return items;
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
eail_menu_get_n_children(AtkObject *obj)
{
   gint n_items;
   Eina_List *items;

   items = eail_menu_get_items(EAIL_MENU(obj));
   n_items = eina_list_count(items);

   eina_list_free(items);

   return n_items;
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
eail_menu_ref_child(AtkObject *obj, gint i)
{
   Eina_List *items;
   AtkObject *child = NULL;

   items = eail_menu_get_items(EAIL_MENU(obj));
   if (eina_list_count(items) > i)
     {
        child = eail_menu_item_new(obj);
        atk_object_initialize(child, eina_list_nth(items, i));

        g_object_ref(child);
     }

   eina_list_free(items);

   return child;
}


/**
 * @brief Gets name of EailItem
 *
 * Implementation of AtkObject->get_name callback
 *
 * ATK doc says:
 * Gets the accessible name of the accessible.
 *
 * @returns a character string representing the accessible description of
 * the accessible.
 */
static const gchar *
eail_menu_get_name(AtkObject *obj)
{
   AtkObject *parent = atk_object_get_parent(obj);

   if (!parent) return NULL;

   return parent->name;
}

/**
 * @brief Initializer for GObject EailMenu instance
 */
static void
eail_menu_init(EailMenu *menu)
{
}

/**
 * @brief Initializer for GObject class (defines callbacks for base AtkObject)
 */
static void
eail_menu_class_init(EailMenuClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);

   class->initialize = eail_menu_initialize;
   class->get_n_children = eail_menu_get_n_children;
   class->ref_child = eail_menu_ref_child;
   class->get_name = eail_menu_get_name;
}

/*
 * EailParent interface implementation
 */

/**
 * @brief Implementation of EailItemParent->get_evas_obj callback
 */
Evas_Object *
eail_menu_get_evas_obj(EailItemParent   *parent,
                       EailItem         *item)
{
   Elm_Object_Item *it = eail_item_get_item(item);

   if (!it)
     {
        ERR("Error. Could not get Elm_Object_item from EailItem");
        return NULL;
     }

   return elm_menu_item_object_get(it);
}

/**
 * @brief Implementation of EailItemParent->is_click_supported callback
 */
gboolean
eail_menu_is_item_click_supported(EailItemParent   *parent,
                                  EailItem         *item)
{
   return TRUE;
}

/**
 * @brief Initializes of EailItemParentIface callbacks
 */
static void
eail_item_parent_interface_init(EailItemParentIface *iface)
{
   iface->get_evas_obj = eail_menu_get_evas_obj;
   iface->is_click_supported = eail_menu_is_item_click_supported;
}
