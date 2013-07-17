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
 * @file eail_item.c
 * @brief Implementation of widget that is being used to represent
 * list/toolbar/menu items
*/

#include "eail_item.h"
#include "eail_item_parent.h"
#include "eail_factory.h"
#include "eail_utils.h"
#include "eail_priv.h"

static void atk_component_interface_init(AtkComponentIface *iface);
static void atk_action_interface_init(AtkActionIface *iface);
static void atk_text_interface_init(AtkTextIface *iface);

/**
 * @brief EailItem GObject definition
 *
 * It extends ATK_TYPE_OBJECT class and implements ATK_TYPE_COMPONENT,
 * ATK_TYPE_TEXT and ATK_TYPE_ACTION interfaces
 */
G_DEFINE_TYPE_WITH_CODE(EailItem,
                        eail_item,
                        ATK_TYPE_OBJECT,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_COMPONENT,
                                              atk_component_interface_init)
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_ACTION,
                                              atk_action_interface_init)
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_TEXT,
                                              atk_text_interface_init)
                                              )

#define EAIL_ITEM_ACTIONS_NUM 1
#define EAIL_ITEM_CLICK_NAME "click"
#define EAIL_ITEM_PART_FIRST "start"
#define EAIL_ITEM_PART_SECOND "end"
#define EAIL_ITEM_PART_ICON "elm.swallow.icon"
#define EAIL_ITEM_PART_END "elm.swallow.end"
#define EAIL_TXT_SEPARATOR " "

/*
 * Public API implementation
 */

/**
 * @brief Creates new EailItem instance with given ATK role
 *
 * @param parent AtkObject that holds EailItem instance
 * @param role AtkRole that will be used by EailItem
 *
 * @returns new EailItem instance or NULL in case of error
 */
AtkObject *
eail_item_new(AtkObject *parent, AtkRole role)
{
   AtkObject *obj;

   g_return_val_if_fail(EAIL_IS_ITEM_PARENT(parent), NULL);

   obj = g_object_new(EAIL_TYPE_ITEM, NULL);
   obj->role = role;
   atk_object_set_parent(obj, parent);

   return obj;
}

/**
 * @brief Gets nested Elm_Object_Item * object
 *
 * @param item EailItem to take nested Elm_Object_Item from
 *
 * @returns nested Elm_Object_Item * object or NULL in case of error
 */
Elm_Object_Item *
eail_item_get_item(EailItem *item)
{
   g_return_val_if_fail(EAIL_IS_ITEM(item), NULL);

   return item->item;
}

/*
 * Implementation of the *AtkObject* interface
 */

/**
 * @brief Initializer for AtkObjectClass
 */
static void
eail_item_initialize(AtkObject *obj, gpointer data)
{
   EailItem *item = EAIL_ITEM(obj);

   ATK_OBJECT_CLASS(eail_item_parent_class)->initialize(obj, data);

   item->item = (Elm_Object_Item *)data;

   /* NOTE: initializing role is being done only in eail_item_new(..) */
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
eail_item_get_name(AtkObject *obj)
{
   AtkObject *parent = atk_object_get_parent(obj);

   if (!parent) return NULL;

   return eail_item_parent_get_item_name(EAIL_ITEM_PARENT(parent),
                                         EAIL_ITEM(obj));
}

/**
 * @brief Implementation of AtkObject->get_role callback
 *
 * @returns ATK role of target item in the list
 */
static AtkRole
eail_item_get_role(AtkObject *obj)
{
   AtkObject *parent = atk_object_get_parent(obj);

   if (!parent) return ATK_ROLE_INVALID;

   return eail_item_parent_get_item_role(EAIL_ITEM_PARENT(parent),
                                         EAIL_ITEM(obj));
}

/**
 * @brief Implementation of AtkObject->get_index_in_parent callback
 *
 * ATK doc says:
 * Gets the 0-based index of this accessible in its parent; returns -1 if the
 * accessible does not have an accessible parent.
 *
 * @returns an integer which is the index of the accessible in its parent
 */
static gint
eail_item_get_index_in_parent(AtkObject *obj)
{
   AtkObject *parent = atk_object_get_parent(obj);

   if (!parent) return -1;

   return eail_item_parent_get_item_index_in_parent(EAIL_ITEM_PARENT(parent),
                                                    EAIL_ITEM(obj));
}

/**
 * @brief Implementation of AtkObject->ref_state_set callback
 *
 * ATK doc says:
 * Gets a reference to the state set of the accessible; the caller must
 * unreference it when it is no longer needed.
 *
 * @returns a reference to an AtkStateSet which is the state set of the
 * accessible.
 */
static AtkStateSet *
eail_item_ref_state_set(AtkObject *obj)
{
   AtkStateSet *state_set;
   AtkObject *parent = atk_object_get_parent(obj);
   EailItem *item = EAIL_ITEM(obj);
   Elm_Object_Item *it = eail_item_get_item(item);

   state_set = ATK_OBJECT_CLASS(eail_item_parent_class)->ref_state_set(obj);

   if (!parent || !it)
     {
        atk_state_set_add_state(state_set, ATK_STATE_DEFUNCT);
        return state_set;
     }

   if (!elm_object_item_disabled_get(it))
     {
        atk_state_set_add_state(state_set, ATK_STATE_SENSITIVE);
        atk_state_set_add_state(state_set, ATK_STATE_ENABLED);
     }

   return eail_item_parent_ref_item_state_set(EAIL_ITEM_PARENT(parent),
                                              item, state_set);
}


/**
 * @brief Initializer for GObject class
 */
static void
eail_item_init(EailItem *item)
{
}

/**
 * @brief Finalize implementation for GObject class
 */
static void
eail_item_class_finalize(GObject *obj)
{
   G_OBJECT_CLASS(eail_item_parent_class)->finalize(obj);
}

/**
 * @brief Helper function used for adding content Evas_Object* part if part
 * with given name is available
 *
 * @param eail_obj_item item used for lookup for given content part
 * @param items list of items that will be extended by found part
 * @param part_str name of part to be found
 *
 * @returns filled list with Evas_Object* objects
 */
static Eina_List *
_eail_item_append_part_if_exist(Elm_Object_Item *eail_obj_item,
                                Eina_List *items,
                                gchar *part_str)
{
   Evas_Object *content = NULL;

   content = elm_object_item_part_content_get
                                         (eail_obj_item, part_str);
   if (content)
     items = eina_list_append(items, content);

   return items;
}

/**
 * @brief Gets list of item content parts with well documented content strings
 *
 * @param obj_item item to get content from
 *
 * @returns list filled with Evas_Object* that has been found
 */
static Eina_List *
_eail_item_get_basic_parts(Elm_Object_Item *obj_item)
{
   Eina_List *items = NULL;

   items = _eail_item_append_part_if_exist
                                       (obj_item, items, EAIL_ITEM_PART_FIRST);
   items = _eail_item_append_part_if_exist
                                       (obj_item, items, EAIL_ITEM_PART_SECOND);
   items = _eail_item_append_part_if_exist
                                       (obj_item, items, EAIL_ITEM_PART_ICON);
   items = _eail_item_append_part_if_exist
                                       (obj_item, items, EAIL_ITEM_PART_END);

   return items;
}

/**
 * @brief Helper function that adds unique entries from one list to another
 *
 * @param item_list source list, unique items will be appended here
 * @param additional_items list with potential items to add
 *
 * @returns result list filled with unique entries from both lists
 */
static Eina_List *
_eail_add_unique_listsparts(Eina_List *item_list,
                            Eina_List *additional_items)
{
   Eina_List *l = NULL;
   Elm_Object_Item *item_edj;

   EINA_LIST_FOREACH(additional_items, l, item_edj)
    {
      if (!eina_list_data_find(item_list, item_edj))
        item_list = eina_list_append(item_list, item_edj);
    }

   return item_list;
}

/**
 * @brief Gets content part items from given EailItem object
 *
 * @param eail_item EailItem object used to get content from
 *
 * @returns content parts (Evas_Object *) for given item. List should be freed
 * when results will be processed.
 */
static Eina_List *
_eail_item_get_part_items(EailItem *eail_item)
{
   Eina_List *items = NULL, *edje_items = NULL;
   Elm_Object_Item *obj_item = NULL;

   obj_item = eail_item_get_item(eail_item);
   g_return_val_if_fail(obj_item, NULL);

   /* parts from well documented default content parts - there are being used
    * mostly by desktop applications. Some of these parts are not listed in
    * edje objects (used below), so results from both list need to be merged
    * to list full item content */
   items = _eail_item_get_basic_parts(obj_item);

   /* content parts from taken from edje object*/
   edje_items = eail_get_edje_parts_for_item(obj_item);

   /* adding unique edje items to content list*/
   items = _eail_add_unique_listsparts(items, edje_items);

   eina_list_free(edje_items);

   return items;
}

/**
 * @brief Checks if content_get is supported by given EailItem object
 *
 * @param atk_object an AtkObject
 *
 * @returns TRUE if content_get is supported, FALSE otherwise
 */
static gboolean
_eail_item_is_content_get_supported(AtkObject *atk_object)
{
   AtkObject *parent = NULL;


   parent = atk_object_get_parent(atk_object);
   g_return_val_if_fail(parent, FALSE);

   return eail_item_parent_is_is_content_get_supported
                              (EAIL_ITEM_PARENT(parent),EAIL_ITEM(atk_object));
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
eail_item_get_n_children(AtkObject *obj)
{
   gint n_items;
   Eina_List *items;

   /* if content_get is not supported then child count will be always '0'*/
   if (!_eail_item_is_content_get_supported(obj))
     return 0;

   items = _eail_item_get_part_items(EAIL_ITEM(obj));
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
eail_item_ref_child(AtkObject *obj, gint i)
{
   Eina_List *items;
   AtkObject *child = NULL;

   items = _eail_item_get_part_items(EAIL_ITEM(obj));
   if (eina_list_count(items) > i)
     child = eail_factory_get_accessible(eina_list_nth(items, i));
   else
      ERR("Tried to ref child with index %d out of bounds!", i);

   eina_list_free(items);

   return child;
}

/**
 * @brief Initializer for GObject class (defines callbacks for base AtkObject)
 */
static void
eail_item_class_init(EailItemClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   GObjectClass *g_object_class = G_OBJECT_CLASS(klass);

   class->initialize = eail_item_initialize;
   class->get_name = eail_item_get_name;
   class->get_role = eail_item_get_role;
   class->get_index_in_parent = eail_item_get_index_in_parent;
   class->ref_state_set = eail_item_ref_state_set;
   class->get_n_children = eail_item_get_n_children;
   class->ref_child = eail_item_ref_child;

   g_object_class->finalize = eail_item_class_finalize;
}

/*
 * Implementation of the *AtkComponent* interface
 */

/**
 * @brief Implementation of AtkComponent->grab_focus callback
 *
 * ATK doc says:
 * Grabs focus for this component.
 *
 * @returns TRUE if successful, FALSE otherwise.
 */
static gboolean
eail_item_grab_focus(AtkComponent *component)
{
   AtkObject *obj = ATK_OBJECT(component);
   AtkObject *parent = atk_object_get_parent(obj);

   if (!parent) return FALSE;

   return eail_item_parent_grab_item_focus(EAIL_ITEM_PARENT(parent),
                                           EAIL_ITEM(obj));
}

/**
 * @brief Implementation of AtkComponent->get_extents callback
 *
 * ATK doc says:
 * Gets the rectangle which gives the extent of the component.
 *
 * @param component an AtkComponent
 * @param x address of gint to put x coordinate
 * @param y address of gint to put y coordinate
 * @param width ddress of gint to put width
 * @param height address of gint to put height
 * @param coord_type specifies whether the coordinates are relative to the
 * screen or to the components top level window
 */
static void
eail_item_get_extents(AtkComponent    *component,
                      gint            *x,
                      gint            *y,
                      gint            *width,
                      gint            *height,
                      AtkCoordType     coord_type)
{
   AtkObject *obj = ATK_OBJECT(component);
   AtkObject *parent = atk_object_get_parent(obj);

   *x = *y = G_MININT;
   *width = *height = -1;

   if (!parent) return;

   eail_item_parent_get_item_extents(EAIL_ITEM_PARENT(parent),
                                     EAIL_ITEM(obj),
                                     x, y, width, height, coord_type);
}

/**
 * @brief Initializer for AtkComponentIface
 */
static void
atk_component_interface_init(AtkComponentIface *iface)
{
   iface->grab_focus  = eail_item_grab_focus;
   iface->get_extents = eail_item_get_extents;
}

/*
 * AtkAction interface init
 */

/**
 * @brief Checks if click action is supported by given EailItem object
 *
 * @param action an AtkAction
 *
 * @returns TRUE if 'click' is supported, FALSE otherwise
 */
static gboolean
_eail_item_is_click_supported(AtkAction *action)
{
   AtkObject *parent = NULL;


   parent = atk_object_get_parent(ATK_OBJECT(action));
   g_return_val_if_fail(parent, FALSE);

   return eail_item_parent_is_click_supported
                                 (EAIL_ITEM_PARENT(parent),EAIL_ITEM(action));

}

/**
 * @brief Returns number of implemented ATK Actions.
 * Implementation of AtkActionIface get_n_actions callback
 *
 * @param action object that implements AtkAction interface
 */
static int
eail_item_n_actions_get(AtkAction *action)
{
   /* if parent item does not support click action, then return no action */
   if (!_eail_item_is_click_supported(action))
     return 0;

   return EAIL_ITEM_ACTIONS_NUM;
}

/**
 * @brief Returns EailActionObj of action with given index
 *
 * @param action object that implements AtkAction interface
 * @param i index (number) of action
 */
static const char*
eail_item_action_name_get(AtkAction *action, int i)
{
   const char* action_name;

   /* if parent item does not support click action, then return no action */
   if (!_eail_item_is_click_supported(action))
     return NULL;

   switch (i)
     {
      case 0:
         /*"clicked": the user clicked the menu item*/
         action_name = EAIL_ITEM_CLICK_NAME;
         break;
      default:
         action_name = NULL;
         break;
     }

   return action_name;
}

/**
 * @brief Performs 'click' event on given item
 *
 * @param atk_item item object to click on
 *
 * @returns TRUE if operation was successful, FALSE otherwise
 */
static gboolean
_eail_item_perform_click_action(AtkObject *atk_item)
{
   AtkObject *parent = NULL;
   Evas_Object *widget = NULL;
   int x, y;

   parent = atk_object_get_parent(atk_item);
   g_return_val_if_fail(parent, FALSE);

   widget = eail_item_parent_get_evas_obj
                        (EAIL_ITEM_PARENT(parent), EAIL_ITEM(atk_item));
   if (!widget)
     {
        ERR("No widget for click found");
        return FALSE;
     }

   /* getting coordinates of center of the widget to make sure, that
    * click will be performed on active widget area */
   eail_get_coords_widget_center(widget, &x, &y);
   eail_mouse_click_on_coords(widget, x, y);

   return TRUE;
}

/**
 * \brief Launches action with given index
 *
 * @param action object that implements AtkAction interface
 * @param i index (number) of action
 *
 * @returns TRUE if action was successfully launched, FALSE otherwise
 */
static gboolean
eail_item_do_action(AtkAction *action, int i)
{
   const char *action_name = atk_action_get_name(action, i);

   /* if parent item does not support click action, then return immediately */
   if (!_eail_item_is_click_supported(action))
     return FALSE;

   if (action_name == NULL) return FALSE;

   if (g_strcmp0(EAIL_ITEM_CLICK_NAME, action_name) == 0)
     {
        DBG("Calling click action");

        return _eail_item_perform_click_action(ATK_OBJECT(action));
     }

   return FALSE;
}

/**
 * @brief Initializer for AtkActionIface
 */
static void
atk_action_interface_init(AtkActionIface *iface)
{
   g_return_if_fail(iface != NULL);

   iface->do_action     = eail_item_do_action;
   iface->get_n_actions = eail_item_n_actions_get;
   iface->get_name      = eail_item_action_name_get;
}

/**
 * @brief Gets text content from item
 *
 * Implementation of AtkTextIface->get_text callback\n
 *
 * @param text an AtkText
 * @param start_offset start position
 * @param end_offset end position, or -1 for the end of the string.
 *
 * @returns a newly allocated string containing the text from start_offset
 * up to, but not including end_offset. Use g_free() to free the returned
 * string.
 */
static gchar*
eail_item_get_text(AtkText   *text,
                   gint       start_offset,
                   gint       end_offset)
{
   Eina_Strbuf *buf = NULL;
   Elm_Object_Item *obj_item = NULL;
   Eina_List *string_parts = NULL, *l = NULL;
   gchar *string_part = NULL;
   gchar *ret_str = NULL;

   obj_item = eail_item_get_item(EAIL_ITEM(text));
   g_return_val_if_fail(obj_item, NULL);

   string_parts = eail_item_get_content_strings(obj_item);
   if (!string_parts) return NULL;

   buf = eina_strbuf_new();
   EINA_LIST_FOREACH(string_parts, l, string_part)
    {
       eina_strbuf_append(buf, string_part);
       eina_strbuf_append(buf, EAIL_TXT_SEPARATOR);
    }

   /* ret_str is newly allocated */
   ret_str = eail_get_substring
                     (eina_strbuf_string_get(buf), start_offset, end_offset);

   eina_list_free(string_parts);
   eina_strbuf_free(buf);

   return ret_str;
}

/**
 * @brief Gets character from item at given offset
 *
 * Implementation of AtkTextIface->get_character_at_offset callback
 *
 * @param text an AtkText
 * @param offset an offset
 *
 * @returns the character at offset.
 */
static gunichar
eail_item_get_character_at_offset(AtkText *text, gint offset)
{
   gunichar character = '\0';
   gchar* time_str = NULL;

   time_str = eail_item_get_text(text, 0, -1);

   if (time_str)
     {
         character = g_utf8_get_char
                              (g_utf8_offset_to_pointer(time_str, offset));
         g_free(time_str);
     }

   return character;
}

/**
 * @brief Gets character count from text content in item
 * Implementation of AtkTextIface->get_character_count callback
 *
 * @param text an AtkText
 *
 * @returns the character count
 */
static gint
eail_item_get_character_count(AtkText *text)
{
   gint count = 0;
   gchar* str = NULL;

   str = eail_item_get_text(text, 0, -1);

   if (str)
      {
         count = g_utf8_strlen(str, -1);
         g_free(str);
      }

   return count;
}

static void
atk_text_interface_init(AtkTextIface *iface)
{
   iface->get_text = eail_item_get_text;
   iface->get_character_at_offset = eail_item_get_character_at_offset;
   iface->get_character_count = eail_item_get_character_count;
}
