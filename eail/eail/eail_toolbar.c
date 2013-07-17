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
 * @file eail_toolbar.c
 * @brief Implementation of toolbar widget
 */

#include <atk/atk.h>
#include <Elementary.h>

#include "eail_toolbar.h"
#include "eail_item.h"
#include "eail_item_parent.h"
#include "eail_priv.h"

static void eail_item_parent_interface_init(EailItemParentIface *iface);

/**
 * @brief Definition of EailToolbar as GObject
 *
 * EailToolbar is extended EAIL_TYPE_WIDGET with EAIL_TYPE_ITEM_PARENT interface
 * implemented
 */
G_DEFINE_TYPE_WITH_CODE(EailToolbar, eail_toolbar, EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(EAIL_TYPE_ITEM_PARENT,
                                              eail_item_parent_interface_init)
);

/*
 * Implementation of the *AtkObject* interface
 */
static void
eail_toolbar_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_toolbar_parent_class) ->initialize(obj, data);
   obj->role = ATK_ROLE_TOOL_BAR;
}

/**
 * @brief EailPopup GObject instance initializer
 */
static void
eail_toolbar_init(EailToolbar *toolbar)
{

}

/*
 * @return list of Elm_Object_Item* objects
 * */
static Eina_List *
eail_toolbar_get_items(EailToolbar *toolbar)
{
   Eina_List *items = NULL;
   Elm_Object_Item *item;
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(toolbar));

   item = elm_toolbar_first_item_get(widget);
   while (item)
     {
        items = eina_list_append(items, item);
        item = elm_toolbar_item_next_get(item);
     }

   return items;
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
eail_toolbar_get_n_children(AtkObject *obj)
{
   gint n_items;
   Eina_List *items;

   items = eail_toolbar_get_items(EAIL_TOOLBAR(obj));
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
 * @param obj an AtkObject
 * @param i index of a child
 *
 * @returns an AtkObject representing the specified accessible child of the
 * accessible.
 */
static AtkObject *
eail_toolbar_ref_child(AtkObject *obj, gint i)
{
   Eina_List *items;
   AtkObject *child = NULL;

   items = eail_toolbar_get_items(EAIL_TOOLBAR(obj) );
   if (eina_list_count(items) > i)
     {
        child = eail_item_new(obj, ATK_ROLE_MENU_ITEM);
        atk_object_initialize(child, eina_list_nth(items, i));

        g_object_ref(child);
     }
   else
     ERR("Tried to ref child with index %d out of bounds!", i);

   eina_list_free(items);

   return child;
}

/**
 * @brief Destructor for EailToolbar object
 */
static void
eail_toolbar_finalize(GObject *object)
{
   G_OBJECT_CLASS(eail_toolbar_parent_class)->finalize(object);
}

/**
 * @brief Initializer for EailToolbar GObject class (defines callbacks for
 * base AtkObject)
 */
static void
eail_toolbar_class_init(EailToolbarClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   class->initialize = eail_toolbar_initialize;
   class->get_n_children = eail_toolbar_get_n_children;
   class->ref_child = eail_toolbar_ref_child;
   gobject_class->finalize = eail_toolbar_finalize;
}

static Evas_Object *
eail_toolbar_get_evas_obj(EailItemParent   *parent,
                       EailItem         *item)
{
   Elm_Object_Item *it = eail_item_get_item(item);

   if (!it)
     {
        ERR("Error. Could not get Elm_Object_item from EailItem");
        return NULL;
     }

   return elm_toolbar_item_object_get(it);
}

/**
 * @brief Returns internal widget from menu item
 *
 * Implementation of EailItemParent->get_evas_obj callback
 */
gboolean
eail_toolbar_is_item_click_supported(EailItemParent   *parent,
                                     EailItem         *item)
{
   return TRUE;
}

/**
 * @brief Get name of list child
 *
 * @param parent EailList instance
 * @param item EailList child instance
 *
 * @returns const gchar * representing name of the child
 */
static const gchar *
eail_toolbar_item_name_get(EailItemParent *parent, EailItem *item)
{
   Elm_Object_Item *it = eail_item_get_item(item);
   if (!it) return NULL;

   return elm_object_item_part_text_get(it, NULL);
}

/**
 * Initialization of EailItemParentIface interface callbacks
 */
static void
eail_item_parent_interface_init(EailItemParentIface *iface)
{
   iface->get_item_name            = eail_toolbar_item_name_get;
   iface->get_evas_obj             = eail_toolbar_get_evas_obj;
   iface->is_click_supported       = eail_toolbar_is_item_click_supported;
}
