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
 * @file eail_naviframe.c
 * @brief Implementation of Naviframe widget
 */

#include "eail_naviframe.h"
#include "eail_naviframe_page.h"
#include "eail_priv.h"

static void atk_selection_interface_init(AtkSelectionIface *iface);
static void atk_action_interface_init(AtkActionIface *action);

/**
 * @brief Define EailNaviframe GObject type
 *
 * It extends EAIL_TYPE_WIDGET and implements ATK_TYPE_SELECTION and
 * ATK_TYPE_ACTION interfaces
 */
G_DEFINE_TYPE_WITH_CODE(EailNaviframe,
                        eail_naviframe,
                        EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_SELECTION,
                                              atk_selection_interface_init)
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_ACTION,
                                              atk_action_interface_init))

/**
 * @brief Helper func used by eail_navifremeref_selection for obtaining index
 * number
 *
 * @param list list of items
 * @param data data used for lookup
 *
 * @returns index of item in list or -1 if not found
 */
static int
_find_index_in_items(const Eina_List *list, const void *data)
{
   int index = 0;
   Eina_Bool found = EINA_FALSE;
   const Eina_List *l;
   EailNaviframePage *it;

   EINA_LIST_FOREACH(list, l, it)
     {
        if (it->page == data)
          {
             found = EINA_TRUE;
             break;
          }

        index++;
     }

   return (found ? index : -1);
}

/**
 * @brief Refreshes history of realized pages
 *
 * @param self an EailNaviframe object
 * @param list list of realized pages
 */
static void
_refresh_cached_pages(EailNaviframe *self, const Eina_List *list)
{
   int list_count = eina_list_count(list);

   eina_list_free(self->cached_pages);

   for (int i = 0; i < list_count; i++)
     {
        AtkObject *child = eail_naviframe_page_new(ATK_OBJECT(self), i);
        atk_object_initialize(child, eina_list_nth(list, i));
        g_object_ref(child);
        self->cached_pages = eina_list_append(self->cached_pages, child);
     }
}

/**
 * @brief Initializer for AtkObjectClass
 */
static void
eail_naviframe_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_naviframe_parent_class)->initialize(obj, data);

   obj->role = ATK_ROLE_PAGE_TAB_LIST;
}

/**
 * @brief Initializer for GObject EailNaviframe instance
 */
static void
eail_naviframe_init(EailNaviframe *naviframe)
{
   naviframe->cached_pages = NULL;
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
eail_naviframe_ref_child(AtkObject *obj, gint i)
{
   Eina_List *list;
   EailNaviframe *naviframe;
   EailWidget *widget;
   Evas_Object *e_object;

   g_return_val_if_fail(EAIL_IS_NAVIFRAME(obj), NULL);

   naviframe = EAIL_NAVIFRAME(obj);
   widget = EAIL_WIDGET(naviframe);

   e_object = eail_widget_get_widget(widget);

   list = elm_naviframe_items_get(e_object);
   int list_count = eina_list_count(list);
   if (i >= list_count)
     {
        eina_list_free(list);
        return NULL;
     }

   AtkObject *child = eail_naviframe_page_new(obj, i);
   atk_object_initialize(child, eina_list_nth(list, i));
   g_object_ref(child);
   naviframe->cached_pages = eina_list_append(naviframe->cached_pages, child);

   eina_list_free(list);

   return child;
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
eail_naviframe_n_children_get(AtkObject *obj)
{
   EailNaviframe *naviframe;
   Evas_Object *widget;
   Eina_List *list;
   int list_count;

   g_return_val_if_fail(EAIL_IS_NAVIFRAME(obj), -1);

   naviframe = EAIL_NAVIFRAME(obj);
   widget = eail_widget_get_widget(EAIL_WIDGET(naviframe));
   list = elm_naviframe_items_get(widget);
   list_count = eina_list_count(list);
   eina_list_free(list);

   return list_count;
}

/**
 * @brief Destructor for naviframe object
 */
static void
eail_naviframe_finalize(GObject *obj)
{
   EailNaviframe *naviframe = EAIL_NAVIFRAME(obj);

   eina_list_free(naviframe->cached_pages);
   if (naviframe->click_title_description)
     free(naviframe->click_title_description);

   G_OBJECT_CLASS(eail_naviframe_parent_class)->finalize(obj);
}

/**
 * @brief Initializer for GObject naviframe class (defines callbacks for base
 * AtkObject)
 */
static void
eail_naviframe_class_init(EailNaviframeClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);

   class->initialize = eail_naviframe_initialize;
   class->get_n_children = eail_naviframe_n_children_get;
   class->ref_child = eail_naviframe_ref_child;

   gobject_class->finalize = eail_naviframe_finalize;
}

/**
 * @brief Implementation of AtkSelection->add_selection callback
 *
 * As described in ATK doc:\n
 * Adds the specified accessible child of the object to the object's selection.
 *
 * @param obj an AtkSelection
 * @param i index of object
 *
 * @returns TRUE if operation ended successfully, FALSE otherwise
 */
static gboolean
eail_naviframe_add_selection(AtkSelection *obj, int i)
{
   EailNaviframePage *page;
   EailNaviframe *naviframe;
   g_return_val_if_fail(EAIL_IS_NAVIFRAME(obj), FALSE);

   naviframe = EAIL_NAVIFRAME(obj);
   page = eina_list_nth(naviframe->cached_pages, i);
   if (!page) return FALSE;

   elm_naviframe_item_promote(page->page);

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
eail_naviframe_ref_selection(AtkSelection *selection, gint i)
{
   Evas_Object *widget;
   Elm_Object_Item *item;
   Eina_List *list;
   int index;
   EailNaviframe *naviframe;

   g_return_val_if_fail(EAIL_IS_NAVIFRAME(selection), NULL);
   g_return_val_if_fail(i == 0, NULL);

   widget = eail_widget_get_widget(EAIL_WIDGET(selection));
   item = elm_naviframe_top_item_get(widget);
   list = elm_naviframe_items_get(widget);
   naviframe = EAIL_NAVIFRAME(selection);
   index = _find_index_in_items(naviframe->cached_pages, item);
   /*item not found in cache, refresh it*/
   if (index < 0)
     {
        DBG("Refreshing cached pages.");
        _refresh_cached_pages(naviframe, list);
     }
   eina_list_free(list);
   return eail_naviframe_ref_child(ATK_OBJECT(selection), index);
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
 * @param selection an AtkSelection
 *
 * @returns gint representing number of selected elements
 */
static gint
eail_naviframe_selection_count(AtkSelection *selection)
{
   EailNaviframe *naviframe;
   g_return_val_if_fail(EAIL_IS_NAVIFRAME(selection), 0);

   naviframe = EAIL_NAVIFRAME(selection);
   if (!eail_widget_get_widget(EAIL_WIDGET(naviframe))) return 0;

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
 * @param selection an AtkSelection
 * @param i index of object
 *
 * @returns a gboolean representing the specified child is selected, or 0 if
 * selection does not implement this interface.
 */
static gboolean
eail_naviframe_is_child_selected(AtkSelection *selection, gint i)
{
   EailNaviframe *naviframe;
   Evas_Object *widget;
   Eina_List *list;
   Elm_Object_Item *it, *item;
   gboolean is_selected = FALSE;

   g_return_val_if_fail(EAIL_IS_NAVIFRAME(selection), is_selected);
   naviframe = EAIL_NAVIFRAME(selection);
   widget = eail_widget_get_widget(EAIL_WIDGET(naviframe));
   item = elm_naviframe_top_item_get(widget);
   list = elm_naviframe_items_get(widget);
   it = eina_list_nth(list, i);

   if (it == item) is_selected = TRUE;
   eina_list_free(list);

   return is_selected;
}

/**
 * @brief Initialization of AtkSelectionIface interface */
static void
atk_selection_interface_init(AtkSelectionIface *iface)
{
   g_return_if_fail(iface != NULL);

   iface->add_selection       = eail_naviframe_add_selection;
   iface->ref_selection       = eail_naviframe_ref_selection;
   iface->get_selection_count = eail_naviframe_selection_count;
   iface->is_child_selected   = eail_naviframe_is_child_selected;
}

/**
 * @brief Returns number of implemented ATK Actions.
 * Implementation of AtkActionIface get_n_actions callback
 *
 * @param action object that implements AtkAction interface
 *
 * @returns gint representing amount of actions implemented by naviframe
 */
static int
eail_naviframe_n_actions_get(AtkAction *action)
{
   return 1;
}

/**
 * @brief Gets name of action with given index
 *
 * @param action object that implements AtkAction interface
 * @param i index (number) of action
 *
 * @returns name (const gchar*) of action with given index
 */
static const char *
eail_naviframe_action_name_get(AtkAction *action, gint i)
{
   const char *action_name;

   if (i == 0)
     action_name = "click_title";
   else
     action_name = NULL;

   return action_name;
}

/**
 * @brief Sets description for action with given index
 * Implementation of AtkActionIface set_description callback
 *
 * @param action object that implements AtkAction interface
 * @param i index (number) of action
 * @param description description to set
 *
 * @returns TRUE if update operation ended with success, FALSE otherwise
 */
static gboolean
eail_naviframe_action_description_set(AtkAction *action,
                                           gint i,
                                           const char *description)
{
   EailNaviframe *naviframe;

   if (i != 0) return FALSE;

   naviframe = EAIL_NAVIFRAME(action);
   if (naviframe->click_title_description)
     free(naviframe->click_title_description);
   naviframe->click_title_description = g_strdup(description);

   return TRUE;
}

/**
 * @brief Gets description for action with given index
 * Implementation of AtkActionIface get_description callback
 *
 * @param action object that implements AtkAction interface
 * @param i index (number) of action
 *
 * @returns string representing description for action
 */
static const char *
eail_naviframe_action_description_get(AtkAction *action, gint i)
{
   EailNaviframe *naviframe;

   g_return_val_if_fail(EAIL_IS_NAVIFRAME(action), NULL);
   naviframe = EAIL_NAVIFRAME(action);

   if (i == 0) return naviframe->click_title_description;

   return NULL;
}

/**
 * @brief Launches action with given index
 *
 * @param action object that implements AtkAction interface
 * @param i index (number) of action
 *
 * @returns TRUE if action was successfully launched, FALSE otherwise
 */
static gboolean
eail_naviframe_do_action(AtkAction *action, gint i)
{
   EailNaviframe *naviframe;
   Evas_Object *widget, *target;
   Elm_Object_Item *it;

   g_return_val_if_fail(EAIL_IS_NAVIFRAME(action), FALSE);

   naviframe = EAIL_NAVIFRAME(action);

   widget = eail_widget_get_widget(EAIL_WIDGET(naviframe));
   it = elm_naviframe_top_item_get(widget);
   if (!elm_naviframe_item_title_visible_get(it)) return FALSE;

   target = elm_object_item_widget_get(it);
   if (!target) return FALSE;

   /*SIG_TITLE_CLICKED "title,clicked"*/

   evas_object_smart_callback_call(target, "title,clicked", it);

   return TRUE;
}

/**
 * @brief Inits Action interface for Naviframe
 */
static void
atk_action_interface_init(AtkActionIface *iface)
{
   g_return_if_fail(iface != NULL);

   iface->do_action       = eail_naviframe_do_action;
   iface->get_n_actions   = eail_naviframe_n_actions_get;
   iface->get_name        = eail_naviframe_action_name_get;
   iface->set_description = eail_naviframe_action_description_set;
   iface->get_description = eail_naviframe_action_description_get;
}
