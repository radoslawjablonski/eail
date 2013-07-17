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
 * @file eail_entry.c
 * @brief Implementation of entry widget
 */

#include <Elementary.h>

#include "eail_entry.h"

static void atk_text_interface_init(AtkTextIface *iface);
static void atk_editable_text_interface_init(AtkEditableTextIface *iface);

/**
 * @brief Definition of EailEntry as GObject
 *
 * EailEntry is extended EAIL_TYPE_TEXT with ATK_TYPE_TEXT and
 * ATK_TYPE_EDITABLE_TEXT interfaces implemented
 */
G_DEFINE_TYPE_WITH_CODE(EailEntry,
                        eail_entry,
                        EAIL_TYPE_TEXT,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_TEXT,
                                              atk_text_interface_init)
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_EDITABLE_TEXT,
                                              atk_editable_text_interface_init))

/*
 * Implementation of the *AtkObject* interface
 */

/**
 * @brief Initializer for AtkObject
 */
static void
eail_entry_initialize(AtkObject *obj, gpointer data)
{
   Evas_Object *widget = (Evas_Object*)data;
   ATK_OBJECT_CLASS(eail_entry_parent_class)->initialize(obj, data);

   if (elm_entry_password_get(widget))
     obj->role = ATK_ROLE_PASSWORD_TEXT;
   else
     obj->role = ATK_ROLE_TEXT;
}

/**
 * @brief Implementation of AtkObject->ref_state_set callback
 *
 * ATK doc says:\n
 * Gets a reference to the state set of the accessible; the caller must
 * unreference it when it is no longer needed.
 *
 * @param obj an AtkObject
 *
 * @returns a reference to an AtkStateSet which is the state set of the
 * accessible.
 */
static AtkStateSet *
eail_entry_ref_state_set(AtkObject *obj)
{
   AtkStateSet *state_set;
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(obj));

   if (!widget)
     return NULL;

   state_set = ATK_OBJECT_CLASS(eail_entry_parent_class)->ref_state_set(obj);

   if (elm_entry_editable_get(widget))
     atk_state_set_add_state(state_set, ATK_STATE_EDITABLE);

   if (elm_entry_single_line_get(widget))
     atk_state_set_add_state(state_set, ATK_STATE_SINGLE_LINE);
   else
     atk_state_set_add_state(state_set, ATK_STATE_MULTI_LINE);

   return state_set;
}

/**
 * @brief Initializer for EailEntry GObject implementation
 */
static void
eail_entry_init(EailEntry *entry)
{
}

/**
 * @brief Initializer for EailPopup GObject class (defines callbacks for
 * base AtkObject)
 */
static void
eail_entry_class_init(EailEntryClass *klass)
{
    AtkObjectClass *class = ATK_OBJECT_CLASS(klass);

    class->initialize = eail_entry_initialize;
    class->ref_state_set = eail_entry_ref_state_set;
}

/*
 * Implementation of the *AtkEditableText* interface
 */

static gint
eail_entry_get_caret_offset(AtkText *text)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget)
     return 0;

   return elm_entry_cursor_pos_get(widget);
}

/**
 * @brief Sets offset for caret
 *
 * Implementation of AtkTextIface->set_caret_offset callback
 * ATK doc says:\n
 * Sets the caret (cursor) position to the specified offset.
 *
 * @param text an AtkText
 * @param offset starting position
 *
 * @returns TRUE if success, FALSE otherwise.
 */
static gboolean
eail_entry_set_caret_offset (AtkText *text,
                             gint     offset)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget)
     return FALSE;

   elm_entry_cursor_pos_set(widget, offset);

   return TRUE;
}

/**
 * @brief Gets text selection from entry
 *
 * Implementation of AtkTextIface->get_selection callback
 *
 * @param text an AtkText
 * @param selection_num The selection number. The selected regions are assigned
 * numbers that correspond to how far the region is from the start of the text.
 * The selected region closest to the beginning of the text region is assigned
 * the number 0, etc. Note that adding, moving or deleting a selected region can
 *  change the numbering.
 * @param start_offset passes back the start position of the selected region
 * @param end_offset passes back the end position of the selected region
 *
 * @returns a newly allocated string containing the selected text. Use g_free()
 * to free the returned string.
 */
static gchar *
eail_entry_get_selection(AtkText *text,
                         gint     selection_num,
                         gint    *start_offset,
                         gint    *end_offset)
{
   const char* selection;
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget)
     return NULL;

   if (0 != selection_num)
     return NULL;

   selection = elm_entry_selection_get(widget);
   if (NULL != selection)
     return g_utf8_strup(selection, -1);

   return NULL;
}

/**
 * @brief Sets text selection for entry
 *
 * Implementation of AtkTextIface->set_selection callback
 *
 * @param text an AtkText
 * @param selection_num The selection number. The selected regions are assigned
 * numbers that correspond to how far the region is from the start of the text.
 * The selected region closest to the beginning of the text region is assigned
 * the number 0, etc. Note that adding, moving or deleting a selected region can
 *  change the numbering.
 * @param start_pos start position of the selected region
 * @param end_pos end position of the selected region
 *
 * @returns TRUE if success, FALSE otherwise
 */
static gboolean
eail_entry_set_selection(AtkText *text,
                         gint     selection_num,
                         gint     start_pos,
                         gint     end_pos)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget)
     return FALSE;

   if (0 != selection_num)
     return FALSE;

   elm_entry_cursor_pos_set(widget, start_pos);
   elm_entry_cursor_selection_begin(widget);
   elm_entry_cursor_pos_set(widget, end_pos);
   elm_entry_cursor_selection_end(widget);

   return TRUE;
}

/**
 * @brief Initializes AtkTextIface interface
 */
static void
atk_text_interface_init(AtkTextIface *iface)
{
   iface->get_caret_offset = eail_entry_get_caret_offset;
   iface->set_caret_offset = eail_entry_set_caret_offset;
   iface->get_selection    = eail_entry_get_selection;
   iface->set_selection    = eail_entry_set_selection;
}

/*
 * Implementation of the *AtkEditableText* interface
 */

/**
 * @brief Set text content for entry
 * @param text an AtkEditableText
 * @param string string to be set
 */
static void
eail_entry_set_text_contents(AtkEditableText *text,
                             const gchar     *string)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (widget && elm_entry_editable_get(widget))
     elm_object_text_set(widget, string);
}

/**
 * @brief Copies text content from entry to clipboard
 *
 * @param text an AtkEditableText
 * @param start_pos start position of cursor
 * @param end_pos end position of cursor
 */
static void
eail_entry_copy_text(AtkEditableText *text,
                     gint             start_pos,
                     gint             end_pos)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget)
     return;

   elm_entry_cursor_pos_set(widget, start_pos);
   elm_entry_cursor_selection_begin(widget);
   elm_entry_cursor_pos_set(widget, end_pos);
   elm_entry_cursor_selection_end(widget);
   elm_entry_selection_copy(widget);
   elm_entry_select_none(widget);
}

/**
 * @brief Cuts text content from entry to clipboard
 *
 * @param text an AtkEditableText
 * @param start_pos start position of cursor
 * @param end_pos end position of cursor
 */
static void
eail_entry_cut_text(AtkEditableText *text,
                    gint             start_pos,
                    gint             end_pos)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget || !elm_entry_editable_get(widget))
     return;

   elm_entry_cursor_pos_set(widget, start_pos);
   elm_entry_cursor_selection_begin(widget);
   elm_entry_cursor_pos_set(widget, end_pos);
   elm_entry_cursor_selection_end(widget);
   elm_entry_selection_cut(widget);
}

/**
 * @brief Pastes text content from clipboard into entry
 *
 * @param text an AtkEditableText
 * @param position start position of cursor
 */
static void
eail_entry_paste_text(AtkEditableText *text,
                      gint             position)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(text));
   if (!widget || !elm_entry_editable_get(widget))
     return;

   elm_entry_cursor_pos_set(widget, position);
   elm_entry_selection_paste(widget);
}

/**
 * @brief Initialization for AtkEditableTextIface interface
 */
static void
atk_editable_text_interface_init(AtkEditableTextIface *iface)
{
   iface->set_text_contents = eail_entry_set_text_contents;
   iface->copy_text = eail_entry_copy_text;
   iface->cut_text = eail_entry_cut_text;
   iface->paste_text = eail_entry_paste_text;
}
