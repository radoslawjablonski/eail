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
 * @file eail_menu_item.c
 * @brief Implementation of widget that is being used to represent menu items
 */

#include "eail_menu_item.h"
#include "eail_menu.h"
#include "eail_item.h"
#include "eail_item_parent.h"
#include "eail_factory.h"
#include "eail_priv.h"

/**
 * @brief EailMenuItem GObject definition
 *
 * It extends EAIL_TYPE_ITEM class
 */
G_DEFINE_TYPE(EailMenuItem, eail_menu_item, EAIL_TYPE_ITEM)

/*
 * Public API implementation
 */

/**
 * @brief Creates new MenuItem instance
 *
 * @param parent AtkObject that holds EailItem instance
 *
 * @returns new EailMenuItem instance or NULL in case of error
 */
AtkObject *
eail_menu_item_new(AtkObject *parent)
{
   AtkObject *obj;

   obj = g_object_new(EAIL_TYPE_MENU_ITEM, NULL);
   obj->role = ATK_ROLE_MENU_ITEM;

   if (EAIL_IS_ITEM_PARENT(parent))
     atk_object_set_parent(obj, parent);

   return obj;
}

/**
 * @brief Initializer for AtkObjectClass
 */
static void
eail_menu_item_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_menu_item_parent_class)->initialize(obj, data);
}


/**
 * @brief Initialization of EailMenuItem GObject
 */
static void
eail_menu_item_init(EailMenuItem *item)
{
}

/**
 * @brief Prepares Eina_List filled with Elm_Object_Item* objects
 * representing items in the list
 *
 * @return filled list with menu items. Call eina_list_free on that list when
 * results processing has been finished
 */
static const Eina_List *
eail_menu_item_get_items(EailMenuItem *menu_item_obj)
{
   const Eina_List *items = NULL;
   EailItem *eail_item = EAIL_ITEM(menu_item_obj);
   Elm_Object_Item *it = eail_item_get_item(eail_item);

   items = elm_menu_item_subitems_get(it);

   return items;
}

/**
 * @brief Gets name of EailMenuItem
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
eail_menu_item_get_name(AtkObject *obj)
{
   EailItem *eail_item = EAIL_ITEM(obj);
   Elm_Object_Item *it = eail_item_get_item(eail_item);

   if (!it) return NULL;

   return elm_object_item_part_text_get(it, NULL);
}

/**
 * @brief Implementation of AtkObject->get_role callback
 *
 * @returns ATK role of target item in the list
 */
static AtkRole
eail_menu_item_get_role(AtkObject *obj)
{
   return ATK_OBJECT(obj)->role;
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
eail_menu_item_get_n_children(AtkObject *obj)
{
   gint n_items;
   const Eina_List *items;
   EailItem *eail_item = EAIL_ITEM(obj);
   Evas_Object *inside_widget = NULL;

   items = eail_menu_item_get_items(EAIL_MENU_ITEM(obj));
   n_items = eina_list_count(items);

   /* increasing child items count for widgets that are stored inside
    * menu item (eg. icon or button). They will be appended at the end
    * of the children list in eail_menu_item_ref_child func*/
   inside_widget = elm_object_item_content_get(eail_item_get_item(eail_item));
   if (inside_widget)
     n_items++;

   return n_items;
}

/**
 * @brief Creates content widgets from given menu_item AtkObject
 *
 * @param menu_item_obj an AtkObject menu item
 * @returns AtkObject representing nested content in menu item
 */
static AtkObject *
_eail_menu_item_create_content_atk_obj(AtkObject *menu_item_obj)
{
   EailItem *eail_item = EAIL_ITEM(menu_item_obj);
   Evas_Object *content_widget = NULL;
   AtkObject *content_atk_obj = NULL;

   content_widget = elm_object_item_content_get(eail_item_get_item(eail_item));

   if (!content_widget)
     {
        ERR("Content widget could not be found in menu_item.");
        return NULL;
     }

   content_atk_obj = eail_factory_get_accessible(content_widget);

   return content_atk_obj;
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
eail_menu_item_ref_child(AtkObject *obj, gint i)
{
   const Eina_List *items;
   AtkObject *child = NULL;
   Elm_Object_Item *elm_obj_item = NULL;

   items = eail_menu_item_get_items(EAIL_MENU_ITEM(obj));
   if (eina_list_count(items) > i)
     {
        elm_obj_item = eina_list_nth(items, i);

        /* creating new menu item from Elm_Object_Item */
        child = eail_menu_item_new(obj);
        atk_object_initialize(child, elm_obj_item);
     }
   else if (eina_list_count(items) == i)
     {
        /* ref to widget inside menu item has to be referenced */
        child = _eail_menu_item_create_content_atk_obj(obj);
     }

   if (child)
     g_object_ref(child);
   else
     ERR("Could not ref menu item child for index %d", i);

   return child;
}

/**
 * @brief Destructor for GObject MenuItem implementation
 */
static void
eail_menu_item_class_finalize(GObject *obj)
{
   G_OBJECT_CLASS(eail_menu_item_parent_class)->finalize(obj);
}

/**
 * @brief Initializer for GObject class (defines callbacks for base AtkObject)
 */
static void
eail_menu_item_class_init(EailMenuItemClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   GObjectClass *g_object_class = G_OBJECT_CLASS(klass);

   class->initialize = eail_menu_item_initialize;
   class->get_n_children = eail_menu_item_get_n_children;
   class->ref_child = eail_menu_item_ref_child;
   class->get_name = eail_menu_item_get_name;
   class->get_role = eail_menu_item_get_role;

   g_object_class->finalize = eail_menu_item_class_finalize;
}
