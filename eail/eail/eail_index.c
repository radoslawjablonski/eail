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
 * @file eail_index.c
 * @brief Implementation of Index widget
 */

#include <Elementary.h>

#include "eail_index.h"
#include "eail_item.h"
#include "eail_item_parent.h"
#include "eail_utils.h"
/* internal elm_widget api for listening internal index parts */
#define ELM_INTERNAL_API_ARGESFSDFEFC
#include <elm_widget.h>
#include "elm_widget_index.h"


#include "eail_priv.h"

static void eail_item_parent_interface_init(EailItemParentIface *iface);
static void atk_selection_interface_init(AtkSelectionIface *iface);

/**
 * @brief EailIndex GObject definition
 *
 * It extends EAIL_TYPE_WIDGET class and implements EAIL_TYPE_ITEM_PARENT and
 * ATK_TYPE_SELECTION interfaces
 */
G_DEFINE_TYPE_WITH_CODE(EailIndex,
                        eail_index,
                        EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(EAIL_TYPE_ITEM_PARENT,
                                              eail_item_parent_interface_init)
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_SELECTION,
                                              atk_selection_interface_init)
);

/*
 * Implementation of the *AtkObject* interface
 */

/**
 * @brief Initializer for AtkObject
 */
static void
eail_index_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_index_parent_class) ->initialize(obj, data);
   obj->role = ATK_ROLE_LIST;
}

/**
 * @brief EailIndex GObject instance initializer
 */
static void
eail_index_init(EailIndex *button)
{
}

/**
 * @brief gets list of Elm_Object_Item* that is contained in index widget
 *
 * @param obj and EailIndex AtkObject
 *
 * @returns const Eina_List * with Elm_Object_Item* for every index
 */
static const Eina_List *
eail_index_get_index_items(AtkObject *obj) {
   Evas_Object *index_widget = NULL;

   index_widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   g_return_val_if_fail(index_widget, NULL);

   ELM_INDEX_DATA_GET(index_widget, index_sd);
   g_return_val_if_fail(index_sd, NULL);

   return index_sd->items;
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
eail_index_get_n_children(AtkObject *obj)
{
   return eina_list_count(eail_index_get_index_items(obj));
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
eail_index_ref_child(AtkObject *obj, gint i)
{
   const Eina_List *items;
   AtkObject *child = NULL;

   items = eail_index_get_index_items(obj);
   if (eina_list_count(items) > i)
     {
        child = eail_item_new(obj, ATK_ROLE_LIST_ITEM);
        atk_object_initialize(child, eina_list_nth(items, i));

        g_object_ref(child);
     }

   return child;
}

/**
 * @brief Gets name of EailIndex
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
eail_index_get_name(AtkObject *obj)
{
   return NULL;
}

/**
 * @brief Destructor of EailIndex object
 */
static void
eail_index_finalize(GObject *object)
{
   G_OBJECT_CLASS(eail_index_parent_class)->finalize(object);
}

/**
 * @brief Initializer for EailIndex GObject class (defines callbacks for
 * base AtkObject)
 */
static void
eail_index_class_init(EailIndexClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   class->initialize = eail_index_initialize;
   class->get_n_children = eail_index_get_n_children;
   class->ref_child = eail_index_ref_child;
   class->get_name = eail_index_get_name;
   gobject_class->finalize = eail_index_finalize;
}

/**
 * @brief Implementation of EailItemParent->is_content_get_supported callback
 */
gboolean
eail_index_content_get_support(EailItemParent   *parent,
                               EailItem         *item)
{
   return FALSE;
}

const gchar *
eail_index_get_item_name(EailItemParent   *parent,
                         EailItem         *item)
{
   Elm_Object_Item *it = eail_item_get_item(item);

   return elm_index_item_letter_get(it);
}

/**
 * @brief Initialization of EailItemParentIface callbacks
 */
static void
eail_item_parent_interface_init(EailItemParentIface *iface)
{
   iface->get_item_name = eail_index_get_item_name;
   iface->is_content_get_supported = eail_index_content_get_support;
}

/**
 * @brief Implementation of AtkSelection->add_selection callback
 *
 * As described in ATK doc:\n
 * Adds the specified accessible child of the object to the object's selection.
 *
 * @param selection an AtkSelection
 * @param i index of object
 *
 * @returns TRUE if operation ended successfully, FALSE otherwise
 */
static gboolean
eail_index_add_selection(AtkSelection *selection,
                         gint i)
{
   const Eina_List *items = eail_index_get_index_items(ATK_OBJECT(selection));
   if (!items) return FALSE;
   Elm_Object_Item *item = eina_list_nth(items, i);

   if (!item) return FALSE;

   elm_index_item_selected_set(item, EINA_TRUE);
   return TRUE;
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
 * @param selection an AtkSelection
 * @param i index of object
 *
 * @returns an AtkObject representing the selected accessible , or NULL if
 * selection does not implement this interface
 */
static AtkObject *
eail_index_ref_selection(AtkSelection *selection,
                         gint i)
{
   g_return_val_if_fail(EAIL_IS_WIDGET(selection), NULL);

   return eail_index_ref_child(ATK_OBJECT(selection), i);
}

/**
 * @brief Implementation of AtkSelection->clear_selection callback
 *
 * As described in ATK doc:\n
 * Clears the selection in the object so that no children in the object are
 * selected.
 *
 * @param selection an AtkSelection
 *
 * @returns TRUE if success, FALSE otherwise.
 */
static gboolean
eail_index_clear_selection(AtkSelection *selection)
{
   g_return_val_if_fail(EAIL_IS_WIDGET(selection), FALSE);
   Evas_Object *object = eail_widget_get_widget(EAIL_WIDGET(selection));
   g_return_val_if_fail(object, FALSE);

   Elm_Object_Item *selected_item = elm_index_selected_item_get(object, 0);

   /* no selected item found so selection is already clear, doing nothing and
    * returning TRUE to mark success*/
   if (!selected_item) return TRUE;

   elm_index_item_selected_set(selected_item, EINA_FALSE);

   return TRUE;
}

/**
 * @brief Implementation of AtkSelection->get_selection_count callback
 *
 * As described in ATK doc:\n
 * Gets the number of accessible children currently selected. Note: callers
 * should not rely on NULL or on a zero value for indication of whether
 * AtkSelectionIface is implemented, they should use type checking/interface
 * checking macros or the atk_get_accessible_value() convenience method.

 * @param selection an AtkSelection
 *
 * @returns gint representing number of selected elements
 */
static gint
eail_index_get_selection_count(AtkSelection *selection)
{
   g_return_val_if_fail(EAIL_IS_WIDGET(selection), FALSE);
   Evas_Object *object = eail_widget_get_widget(EAIL_WIDGET(selection));

   if (elm_index_selected_item_get(object, 0))
     return 1;
   else
     return 0;
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
 * @param selection an AtkSelection
 * @param i index of object
 *
 * @returns a gboolean representing the specified child is selected, or 0 if
 * selection does not implement this interface.
 */
static gboolean
eail_index_is_child_selected(AtkSelection *selection,
                             gint i)
{
   g_return_val_if_fail(EAIL_IS_WIDGET(selection), FALSE);
   Evas_Object *object = eail_widget_get_widget(EAIL_WIDGET(selection));
   Elm_Object_Item *selected_item = elm_index_selected_item_get(object, 0);
   const Eina_List *items = eail_index_get_index_items(ATK_OBJECT(selection));
   Elm_Object_Item *item_witn_n_index = NULL;

   /* if no item is selected, then we are sure that none of the child is
    * selected*/
   if (!selected_item) return FALSE;

   /* no items on list so no child is selected*/
   if (!items) return FALSE;

   item_witn_n_index =  eina_list_nth(items, i);

   if (item_witn_n_index == selected_item)
     return TRUE;
   else
     return FALSE;
}

/**
 * @brief Initializer of AtkSelectionIface interface
 */
static void atk_selection_interface_init(AtkSelectionIface *iface)
{
   iface->add_selection        = eail_index_add_selection;
   iface->clear_selection      = eail_index_clear_selection;
   iface->ref_selection        = eail_index_ref_selection;
   iface->get_selection_count  = eail_index_get_selection_count;
   iface->is_child_selected    = eail_index_is_child_selected;
}
