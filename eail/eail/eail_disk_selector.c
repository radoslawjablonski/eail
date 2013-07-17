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
 * @file eail_disk_selector.c
 * @brief Implementation of DiskSelector widget
 */

#include <Elementary.h>

#include "eail_disk_selector.h"
#include "eail_item.h"
#include "eail_item_parent.h"
#include "eail_priv.h"

static void atk_selection_interface_init(AtkSelectionIface *iface);
static void eail_item_parent_interface_init(EailItemParentIface *iface);

/**
 * @brief definition of EailDiskSelector as GObject
 *
 * EailDiskSelector is extended ScrollableWidget with EAIL_TYPE_ITEM_PARENT and
 * ATK_TYPE_SELECTION implemented
 */
G_DEFINE_TYPE_WITH_CODE(EailDiskSelector,
                        eail_disk_selector,
                        EAIL_TYPE_SCROLLABLE_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_SELECTION,
                                              atk_selection_interface_init)
                        G_IMPLEMENT_INTERFACE(EAIL_TYPE_ITEM_PARENT,
                                              eail_item_parent_interface_init))

/*
 * Implementation of the *AtkObject* interface
 */

/**
 * @brief Initializer for AtkObjectClass
 */
static void
eail_disk_selector_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_disk_selector_parent_class)->initialize(obj, data);
   obj->role = ATK_ROLE_LIST;
}

/**
 * @brief Initializer for GObject EailList instance
 */
static void
eail_disk_selector_init(EailDiskSelector *disk_selector)
{
}

/**
 * @brief Implementation AtkObject->get_n_children callback
 *
 * ATK doc says:\n
 * Gets the number of accessible children of the accessible.
 *
 * @param obj an AtkObject
 *
 * @returns an integer representing the number of accessible children of
 * the accessible
 */
static gint
eail_disk_selector_get_n_children(AtkObject *obj)
{
   gint n_items;

   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   n_items = eina_list_count(elm_diskselector_items_get(widget));

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
eail_disk_selector_ref_child(AtkObject *obj, gint i)
{
   const Eina_List *items;
   AtkObject *child = NULL;
   Evas_Object *widget = NULL;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return NULL;

   items = elm_diskselector_items_get(widget);

   if (eina_list_count(items) > i)
     {
        child = eail_item_new(obj, ATK_ROLE_LIST_ITEM);
        atk_object_initialize(child, eina_list_nth(items, i));

        g_object_ref(child);
     }

   return child;
}

/**
 * @brief Destructor for EailDiskSelector class
 */
static void
eail_disk_selector_finalize(GObject *object)
{
   G_OBJECT_CLASS(eail_disk_selector_parent_class)->finalize(object);
}

/**
 * @brief Initializer for GObject EailDiskSelectorClass class (defines callbacks
 * for base AtkObject)
 */
static void
eail_disk_selector_class_init(EailDiskSelectorClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   class->initialize = eail_disk_selector_initialize;
   class->get_n_children = eail_disk_selector_get_n_children;
   class->ref_child = eail_disk_selector_ref_child;
   gobject_class->finalize = eail_disk_selector_finalize;
}

/*
 * Implementation of the *AtkSelection* interface
 */

/**
 * @brief Implementation of AtkSelection->add_selection callback
 *
 * As described in ATK doc:\n
 * Adds the specified accessible child of the object to the object's selection.
 *
 * @returns TRUE if operation ended successfully, FALSE otherwise
 */
static gboolean
eail_selection_add_selection(AtkSelection *selection,
                             gint i)
{
   g_return_val_if_fail(EAIL_IS_WIDGET(selection), FALSE);
   Evas_Object *object = eail_widget_get_widget(EAIL_WIDGET(selection));
   const Eina_List *items = elm_diskselector_items_get(object);
   Elm_Object_Item *item = eina_list_nth(items, i);
   if (!item) return FALSE;
   elm_diskselector_item_selected_set(item, EINA_TRUE);
   return TRUE;
}

/**
 * @brief Implementation of AtkSelection->clear_selection callback
 *
 * As described in ATK doc:\n
 * Clears the selection in the object so that no children in the object are
 * selected.
 *
 * @returns always FALSE - this operation is not supported for DiskSelector
 */
static gboolean
eail_selection_clear_selection(AtkSelection *selection)
{
   return FALSE;
}

/**
 * @brief Implementation of AtkSelection->ref_selection callback
 *
 * As described in ATK doc:\n
 * Gets a reference to the accessible object representing the specified selected
 * child of the object. Note: callers should not rely on NULL or on a zero value
 * for indication of whether AtkSelectionIface is implemented, they should use
 * type checking/interface checking macros or the atk_get_accessible_value()
 * convenience method.
 *
 * @returns an AtkObject representing the selected accessible , or NULL if
 * selection does not implement this interface
 */
static AtkObject *
eail_selection_ref_selection(AtkSelection *selection,
                             gint i)
{
   g_return_val_if_fail(EAIL_IS_WIDGET(selection), NULL);

   Evas_Object *object = eail_widget_get_widget(EAIL_WIDGET(selection));
   const Eina_List *items = elm_diskselector_items_get(object);
   Elm_Object_Item *item = eina_list_nth(items, i);
   if (!item) return NULL;

   AtkObject *obj;
   obj = g_object_new(EAIL_TYPE_DISK_SELECTOR, NULL);
   obj->role = ATK_ROLE_LIST_ITEM;

   if (EAIL_IS_ITEM_PARENT(selection))
     atk_object_set_parent(obj, (AtkObject *)selection);

   atk_object_initialize(obj, item);
   g_object_ref(obj);
   return obj;
}

/**
 * @brief Implementation of AtkSelection->get_selection_count callback
 *
 * As described in ATK doc:\n
 * Gets the number of accessible children currently selected. Note: callers
 * should not rely on NULL or on a zero value for indication of whether
 * AtkSelectionIface is implemented, they should use type checking/interface
 * checking macros or the atk_get_accessible_value() convenience method.
 *
 * @returns always '1' - in DiskSelector widget something is always selected
 */
static gint
eail_selection_get_selection_count(AtkSelection *selection)
{
   return 1;
}

/**
 * @brief Implementation of AtkSelection->is_child_selected callback
 *
 * As described in ATK doc:\n
 * Determines if the current child of this object is selected Note: callers
 * should not rely on NULL or on a zero value for indication of whether
 * AtkSelectionIface is implemented, they should use type checking/interface
 * checking macros or the atk_get_accessible_value() convenience method.
 *
 * @returns a gboolean representing the specified child is selected, or 0 if
 * selection does not implement this interface.
 */
static gboolean
eail_selection_is_child_selected(AtkSelection *selection,
                                 gint i)
{
   g_return_val_if_fail(EAIL_IS_WIDGET(selection), FALSE);
   Evas_Object *object = eail_widget_get_widget(EAIL_WIDGET(selection));
   Elm_Object_Item *selected_item = elm_diskselector_selected_item_get(object);
   Elm_Object_Item *last_item = elm_diskselector_last_item_get(object);
   Elm_Object_Item *item = elm_diskselector_first_item_get(object);
   gint n=0;
   do
     {
        if (selected_item==item) break;
        item = elm_diskselector_item_next_get(item);
        n++;
     } while (item!=last_item);
   return i==n;
}

/**
 * @brief Implementation of AtkSelection->remove_selection callback
 *
 * As described in ATK doc:\n
 * Removes the specified child of the object from the object's selection.
 *
 * @returns TRUE if success, FALSE otherwise.
 */
static gboolean
eail_selection_remove_selection(AtkSelection *selection,
                                gint i)
{
   g_return_val_if_fail(EAIL_IS_WIDGET(selection), FALSE);
   Evas_Object *object = eail_widget_get_widget(EAIL_WIDGET(selection));
   Elm_Object_Item *selected_item = elm_diskselector_first_item_get(object);
   elm_diskselector_item_selected_set(selected_item, TRUE);
   return TRUE;
}

/**
 * @brief Implementation of AtkSelection->select_all_selection callback
 *
 * As described in ATK doc:\n
 * Causes every child of the object to be selected if the object supports
 * multiple selections.
 *
 * @returns FALSE - this operation is not supported for DiskSelector widget
 */
static gboolean
eail_selection_select_all_selection(AtkSelection *selection)
{
   return FALSE;
}

/**
 * Initialization of AtkSelectionIface interface
 */
static void atk_selection_interface_init(AtkSelectionIface *iface)
{
   iface->add_selection        = eail_selection_add_selection;
   iface->clear_selection      = eail_selection_clear_selection;
   iface->ref_selection        = eail_selection_ref_selection;
   iface->get_selection_count  = eail_selection_get_selection_count;
   iface->is_child_selected    = eail_selection_is_child_selected;
   iface->remove_selection     = eail_selection_remove_selection;
   iface->select_all_selection = eail_selection_select_all_selection;
}

/**
 * @brief Get name of disk selector child
 *
 * @param parent EailDiskSelector instance
 * @param item EailDiskSelector child instance
 *
 * @returns const gchar * representing name of the child
 */
static const gchar *
eail_disk_selector_item_name_get(EailItemParent *parent, EailItem *item)
{
   Elm_Object_Item *it = eail_item_get_item(item);
   if (!it) return NULL;

   return elm_object_item_part_text_get(it, NULL);
}

/**
 * @brief Initialization of EailItemParentIface callbacks
 */
static void
eail_item_parent_interface_init(EailItemParentIface *iface)
{
   iface->get_item_name = eail_disk_selector_item_name_get;
}
