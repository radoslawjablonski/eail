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
 * @file eail_fileselector_entry.c
 * @brief Implementation of fileselector entry widget
 */

#include <Elementary.h>

#include "eail_fileselector_entry.h"
#include "eail_factory.h"
#include "eail_utils.h"
#include "eail_priv.h"

static void atk_text_interface_init(AtkTextIface *iface);
static void eail_fileselector_entry_actions_init(EailActionWidget *widget);

#define ICON_CONTENT_NAME "button icon"
#define EAIL_FSE_ACTION_CLICK "click"
#define EAIL_FSE_ACTION_LONGPRESS "longpress"
#define EAIL_FSE_ACTION_PRESS "press"
#define EAIL_FSE_ACTION_UNPRESS "unpress"

/**
 * @brief Definition of EailFileselectorEntry as GObject
 *
 * EailFileselectorEntry is extended EAIL_TYPE_ACTION_WIDGET with ATK_TYPE_TEXT
 * interface implemented
 */
G_DEFINE_TYPE_WITH_CODE(EailFileselectorEntry,
                        eail_fileselector_entry,
                        EAIL_TYPE_ACTION_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_TEXT,
                                              atk_text_interface_init)
)

/*
 * Implementation of the *AtkObject* interface
 */

/**
 * @brief Initializer for AtkObject
 */
static void
eail_fileselector_entry_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_fileselector_entry_parent_class) ->initialize(obj, data);
   obj->role = ATK_ROLE_GROUPING;

   EailActionWidget *action_widget = EAIL_ACTION_WIDGET(obj);

   /* Initializing possible widget actions table*/
   eail_fileselector_entry_actions_init(action_widget);
}

/**
 * @brief Initializer for EailEntry GObject implementation
 */
static void
eail_fileselector_entry_init(EailFileselectorEntry *fileselector_entry)
{
}

/**
 * @brief Gets name of FileselectorEntry
 *
 * Implementation of AtkObject->get_name callback
 * Name is label-text on button
 *
 * ATK doc says:
 * Gets the accessible name of the accessible.
 *
 * @returns a character string representing the accessible description of
 * the accessible.
 */
static const gchar *
eail_fileselector_entry_get_name(AtkObject *obj)
{
   Evas_Object *fse_widget = eail_widget_get_widget(EAIL_WIDGET(obj));

   if (!fse_widget) return NULL;

   return elm_object_part_text_get(fse_widget, "default");
}

/**
 * @brief Helper func for returning list of nested widgets in fileselector entry
 *
 * @param obj an AtkObject
 *
 * @return list of Evas_Object* objects with nested widgets
 */
static Eina_List *
_eail_fileselector_entry_get_items(AtkObject *obj)
{
   Eina_List *items = NULL;
   Evas_Object *icon_widget = NULL, *fse_widget = NULL;

   fse_widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!fse_widget) return NULL;

   icon_widget = elm_object_part_content_get(fse_widget, ICON_CONTENT_NAME);
   if (!icon_widget) return NULL;

   items = eina_list_append(items, icon_widget);

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
eail_fileselector_entry_get_n_children(AtkObject *obj)
{
   gint n_items;
   Eina_List *items;

   items = _eail_fileselector_entry_get_items(obj);
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
eail_fileselector_entry_ref_child(AtkObject *obj, gint i)
{
   Eina_List *items = NULL;
   AtkObject *child = NULL;

   items = _eail_fileselector_entry_get_items(obj);
   if (eina_list_count(items) > i)
     {
        child = eail_factory_get_accessible(eina_list_nth(items, i));
        if (child)  g_object_ref(child);
     }

   eina_list_free(items);

   return child;
}

/**
 * @brief Destructor for FileselectorEntry class
 */
static void
eail_fileselector_entry_finalize(GObject *object)
{
   G_OBJECT_CLASS(eail_fileselector_entry_parent_class)->finalize(object);
}

/**
 * @brief Initializer for EailFileselectorEntry GObject class (defines
 * callbacks for base AtkObject)
 */
static void
eail_fileselector_entry_class_init(EailFileselectorEntryClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   class->initialize = eail_fileselector_entry_initialize;
   class->get_name = eail_fileselector_entry_get_name;
   class->get_n_children = eail_fileselector_entry_get_n_children;
   class->ref_child = eail_fileselector_entry_ref_child;
   gobject_class->finalize = eail_fileselector_entry_finalize;
}

/*
 * Implementation of the *AtkText* interface
 */

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
eail_fileselector_entry_get_text(AtkText  *text,
                                 gint      start_offset,
                                 gint      end_offset)
{
   gchar *ret_str = NULL;
   Evas_Object *fse_widget = NULL;

   fse_widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!fse_widget) return NULL;

   ret_str = eail_get_substring
                (elm_fileselector_entry_path_get(fse_widget),
                 start_offset, end_offset);

   return ret_str;
}

/**
 * @brief Gets character from popup at given offset
 *
 * Implementation of AtkTextIface->get_character_at_offset callback
 *
 * @param text an AtkText
 * @param offset an offset
 *
 * @returns the character at offset.
 */
static gunichar
eail_fileselector_entry_get_character_at_offset(AtkText *text,
                                                gint     offset)
{
   gunichar character = '\0';
   gchar *entry_str = NULL;

   entry_str = eail_fileselector_entry_get_text(text, 0, -1);
   if (entry_str)
      {
         character = g_utf8_get_char
                           (g_utf8_offset_to_pointer(entry_str, offset));
         g_free(entry_str);
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
eail_fileselector_entry_get_character_count(AtkText *text)
{
   gint count = 0;
   gchar *entry_str = NULL;
   entry_str = eail_fileselector_entry_get_text(text, 0, -1);

    if (entry_str)
      {
         count = g_utf8_strlen(entry_str, -1);
         g_free(entry_str);
      }

    return count;
}

/**
 * @brief Initializer for AtkTextIface interface
 */
static void
atk_text_interface_init(AtkTextIface *iface)
{
    iface->get_text = eail_fileselector_entry_get_text;
    iface->get_character_at_offset = eail_fileselector_entry_get_character_at_offset;
    iface->get_character_count = eail_fileselector_entry_get_character_count;
}

/*
 * Implementation of the Actions
 */

/**
 * @brief Calls callback with given name
 *
 * @returns TRUE if was successful, FALSE otherwise
 */
static gboolean
eail_fileselector_entry_do_action(AtkAction *action,
                                  const char *action_name)
{
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(action));
   if (widget == NULL) return FALSE;
   if ((elm_object_disabled_get(widget)) || (!evas_object_visible_get(widget)))
     return FALSE;

   if (action_name == NULL) return FALSE;
   /*emit signal*/
   evas_object_smart_callback_call(widget, action_name, NULL);
   return TRUE;
}

/**
 * @brief Handle for action click
 *
 * @param action an AtkAction
 * @param data additional action data (not used here)
 *
 * @return TRUE if action was triggered successfully, FALSE otherwise
 */
static gboolean
eail_action_click(AtkAction *action, void *data)
{
   return eail_fileselector_entry_do_action(action, "clicked");
}

/**
 * @brief Handle for action longpress
 *
 * @param action an AtkAction
 * @param data additional action data (not used here)
 *
 * @return TRUE if action was triggered successfully, FALSE otherwise
 */
static gboolean
eail_action_longpress(AtkAction *action, void *data)
{
   return eail_fileselector_entry_do_action(action, "longpressed");
}

/**
 * @brief Handle for action press
 *
 * @param action an AtkAction
 * @param data additional action data (not used here)
 *
 * @return TRUE if action was triggered successfully, FALSE otherwise
 */
static gboolean
eail_action_press(AtkAction *action, void *data)
{
   return eail_fileselector_entry_do_action(action, "press");
}

/**
 * @brief Handle for action unpress
 *
 * @param action an AtkAction
 * @param data additional action data (not used here)
 *
 * @return TRUE if action was triggered successfully, FALSE otherwise
 */
static gboolean
eail_action_unpress(AtkAction *action, void *data)
{
   return eail_fileselector_entry_do_action(action, "unpressed");
}

/**
 * @brief Adds fileselector actions to actions table
 *
 * @param action_widget widget that implements EailActionWidget interface
 */
static void
eail_fileselector_entry_actions_init(EailActionWidget *action_widget)
{
   eail_action_widget_action_append(action_widget,
                                    EAIL_FSE_ACTION_CLICK, NULL,
                                    eail_action_click);
   eail_action_widget_action_append(action_widget,
                                    EAIL_FSE_ACTION_LONGPRESS, NULL,
                                    eail_action_longpress);
   eail_action_widget_action_append(action_widget,
                                    EAIL_FSE_ACTION_PRESS, NULL,
                                    eail_action_press);
   eail_action_widget_action_append(action_widget,
                                    EAIL_FSE_ACTION_UNPRESS, NULL,
                                    eail_action_unpress);
}
