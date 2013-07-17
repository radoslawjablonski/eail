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
 * @file eail_multibuttonentry.c
 * @brief EailMultibuttonentry implementation
 */

#include <Elementary.h>

#include "eail_multibuttonentry.h"
#include "eail_item.h"
#include "eail_item_parent.h"
#include "eail_priv.h"

/**
 * @brief Number of EailMultibuttonentry implemented actions
 */
#define MULTIBUTTONENTRY_ACTIONS_NUMBER 2

static void eail_item_parent_interface_init(EailItemParentIface *iface);
static void atk_editable_text_interface_init(AtkEditableTextIface *iface);
static void atk_action_interface_init(AtkActionIface *iface);

/**
 * @brief EailMultibuttonentry type definition
 */
G_DEFINE_TYPE_WITH_CODE(EailMultibuttonentry,
                        eail_multibuttonentry,
                        EAIL_TYPE_TEXT,
                        G_IMPLEMENT_INTERFACE(EAIL_TYPE_ITEM_PARENT,
                                              eail_item_parent_interface_init)
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_EDITABLE_TEXT,
                                              atk_editable_text_interface_init)
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_ACTION,
                                              atk_action_interface_init))

/* AtkObject */
static void eail_multibuttonentry_initialize(AtkObject *obj,
                                             gpointer data);
static AtkStateSet *eail_multibuttonentry_ref_state_set(AtkObject *obj);
static gint eail_multibuttonentry_get_n_children(AtkObject *obj);
static AtkObject *eail_multibuttonentry_ref_child(AtkObject *obj,
                                                  gint i);
static void eail_multibuttonentry_finalize(GObject *object);

/* AtkAction */
static const char *eail_multibuttonentry_description_get(AtkAction *action,
                                                         gint i);
static gboolean eail_multibuttonentry_description_set(AtkAction *action,
                                                      gint i,
                                                      const char *description);
static const char *eail_multibuttonentry_action_name_get(AtkAction *action,
                                                         int i);
static int eail_multibuttonentry_n_actions_get(AtkAction *action);
static gboolean eail_multibuttonentry_do_action(AtkAction *action,
                                                int i);

/* AtkItemParent */

/* AtkEditableText */
static void eail_multibuttonentry_set_text_contents(AtkEditableText *text,
                                                    const gchar *string);

/* Callbacks */
static void _on_shrink(void *data,
                       Evas_Object *obj,
                       void *event_info);
static void _on_expand(void *data,
                       Evas_Object *obj,
                       void *event_info);

/*
 * Implementation of the *AtkObject* interface
 */

/**
 * @brief EailMultibuttonentry type initializer
 */
void
eail_multibuttonentry_initialize(AtkObject *obj, gpointer data)
{
   Evas_Object *widget = (Evas_Object *)data;
   ATK_OBJECT_CLASS(eail_multibuttonentry_parent_class)->initialize(obj, data);
   obj->role = ATK_ROLE_ENTRY;

   evas_object_smart_callback_add(widget, "contracted", _on_shrink, obj);
   evas_object_smart_callback_add(widget, "expanded", _on_expand, obj);
}

/**
 * @brief EailMultibuttonentry instance initializer
 *
 * @param multibuttonentry an EailMultibuttonentry
 */
static void
eail_multibuttonentry_init(EailMultibuttonentry *multibuttonentry)
{
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(multibuttonentry));

   multibuttonentry->shrink_description = NULL;
   multibuttonentry->expand_description = NULL;

   multibuttonentry->state = elm_multibuttonentry_expanded_get(widget) ?
       EAIL_MULTIBUTTONENTRY_EXPANDED : EAIL_MULTIBUTTONENTRY_SHRINKED;
}

/**
 * @brief Gets obj state set
 *
 * The caller must unreference it when it is no longer needed
 *
 * @param obj an AtkObject
 * @return obj state set
 */
static AtkStateSet *
eail_multibuttonentry_ref_state_set(AtkObject *obj)
{
   AtkStateSet *state_set;
   Evas_Object *widget;

   g_return_val_if_fail(EAIL_MULTIBUTTONENTRY(obj), NULL);

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   state_set = ATK_OBJECT_CLASS(eail_multibuttonentry_parent_class)->ref_state_set(obj);

   if (!widget) return state_set;

   if (elm_multibuttonentry_expanded_get(widget))
     atk_state_set_add_state(state_set, ATK_STATE_EXPANDED);
   else
     atk_state_set_add_state(state_set, ATK_STATE_DEFAULT);

   return state_set;
}

/**
 * @brief Gets multibuttonentry items list
 *
 * @param multibuttonentry an EailMultibuttonentry
 * @return multibuttonentry items list
 * */
static const Eina_List *
eail_multibuttonentry_get_items(EailMultibuttonentry *multibuttonentry)
{
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(multibuttonentry));

   if (!widget) return NULL;

   return elm_multibuttonentry_items_get(widget);
}

/**
 * @brief Gets obj child number
 *
 * @param obj an AtkObject
 * @return child number
 */
static gint
eail_multibuttonentry_get_n_children(AtkObject *obj)
{
   gint n_items;
   const Eina_List *items;

   items = eail_multibuttonentry_get_items(EAIL_MULTIBUTTONENTRY(obj));
   n_items = eina_list_count(items);

   return n_items;
}

/**
 * @brief Gets reference to i-th obj child
 *
 * The caller must unreference it when it is no longer needed
 *
 * @param obj an AtkObject
 * @param i child index
 * @return child reference
 */
static AtkObject *
eail_multibuttonentry_ref_child(AtkObject *obj, gint i)
{
   const Eina_List *items;
   AtkObject *child = NULL;

   items = eail_multibuttonentry_get_items(EAIL_MULTIBUTTONENTRY(obj));
   if (eina_list_count(items) > i)
     {
        child = eail_item_new(obj, ATK_ROLE_LABEL);
        atk_object_initialize(child, eina_list_nth(items, i));

        g_object_ref(child);
     }
   else
     ERR("Tried to ref child with index %d out of bounds!", i);

   return child;
}

/**
 * @brief EailMultibuttonentry type finalizer
 *
 * Destroys object and allocated resources
 *
 * @param object a GObject
 */
static void
eail_multibuttonentry_finalize(GObject *object)
{
   EailMultibuttonentry *multi = EAIL_MULTIBUTTONENTRY(object);

   if (multi->expand_description) free(multi->expand_description);
   if (multi->shrink_description) free(multi->shrink_description);

   G_OBJECT_CLASS(eail_multibuttonentry_parent_class)->finalize(object);
}

/**
 * @brief EailMultibuttonentry class initializer
 *
 * @param klass an EailMultibuttonentry class
 */
static void
eail_multibuttonentry_class_init(EailMultibuttonentryClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);

   class->initialize = eail_multibuttonentry_initialize;
   class->get_n_children = eail_multibuttonentry_get_n_children;
   class->ref_child = eail_multibuttonentry_ref_child;
   class->ref_state_set = eail_multibuttonentry_ref_state_set;

   gobject_class->finalize = eail_multibuttonentry_finalize;
}

/*
 * Implementation of the *EailItemParentIface* interface
 */

/**
 * @brief Implementation of EailItemParent->is_content_get_supported callback
 */
gboolean
eail_multibuttonentry_content_get_support(EailItemParent   *parent,
                                          EailItem         *item)
{
   return FALSE;
}

/**
 * @brief EailItemParent interface initializer
 *
 * @param iface an EailItemParentIface
 */
static void
eail_item_parent_interface_init(EailItemParentIface *iface)
{
   iface->is_content_get_supported = eail_multibuttonentry_content_get_support;
}

/*
 * Implementation of the *AtkEditableText* interface
 */

/**
 * @brief AtkEditableText interface initializer
 *
 * @param iface an AtkEditableTextIface
 */
static void
atk_editable_text_interface_init(AtkEditableTextIface *iface)
{
   iface->set_text_contents = eail_multibuttonentry_set_text_contents;
   /* NOTE: RJ Cut, copy & paste features are not working with
    * this widget.. */
}

/**
 * @brief
 *
 * @param text an AtkEditableText
 * @param string new text to be set
 */
static void
eail_multibuttonentry_set_text_contents(AtkEditableText *text,
                                        const gchar *string)
{
   Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(text));

   if (!widget) return;

   elm_object_text_set(widget, string);
}

/*
 * Implementation of the *AtkAction* interface
 */

/**
 * @brief AtkAction interface initializer
 *
 * @param iface an EailHoversel
 */
static void
atk_action_interface_init(AtkActionIface *iface)
{
   g_return_if_fail(iface != NULL);

   iface->do_action          = eail_multibuttonentry_do_action;
   iface->get_n_actions      = eail_multibuttonentry_n_actions_get;
   iface->get_description    = eail_multibuttonentry_description_get;
   iface->get_name           = eail_multibuttonentry_action_name_get;
   iface->set_description    = eail_multibuttonentry_description_set;
}

/**
 * @brief Gets i-th action description
 *
 * @param action an  AtkAction instance
 * @param i action index
 * @return action description
 */
static const char*
eail_multibuttonentry_description_get(AtkAction *action,
                                      gint i)
{
   EailMultibuttonentry *multi;
   const char *action_description;

   multi = EAIL_MULTIBUTTONENTRY(action);
   if (!multi) return NULL;

   switch (i)
     {
       case 0:
           action_description = multi->shrink_description;
           break;
       case 1:
           action_description = multi->expand_description;
           break;
       default:
           action_description = NULL;
           break;
     }

   return action_description;
}

/**
 * @brief Sets i-th action description
 *
 * @param action an AtkAction instance
 * @param i action index
 * @param description action desciption
 * @return TRUE if description is set successfully, FALSE otherwise
 */
static gboolean
eail_multibuttonentry_description_set(AtkAction *action,
                                      gint i,
                                      const char *description)
{
   EailMultibuttonentry *multi;
   char **value;

   multi = EAIL_MULTIBUTTONENTRY(action);
   if (!multi) return FALSE;

   switch (i)
     {
       case 0:
           value = &multi->shrink_description;
           break;
       case 1:
           value = &multi->expand_description;
           break;
       default:
           value = NULL;
           break;
     }

   if (value)
     {
        free(*value);
        *value = g_strdup(description);
        return TRUE;
     }

   return FALSE;
}

/**
 * @brief Gets i-th action name
 *
 * @param action an AtkAction instance
 * @param i action index
 * @return action description
 */
static const char*
eail_multibuttonentry_action_name_get(AtkAction *action,
                                      int i)
{
   const char* action_name;

   switch (i)
     {
       case 0:
           action_name = "shrink";
           break;
       case 1:
           action_name = "expand";
           break;
       default:
           action_name = NULL;
           break;
     }

   return action_name;
}

/**
 * @brief Gets number of implemented actions
 *
 * @param action an AtkAction instance
 * @return actions number
 */
static int
eail_multibuttonentry_n_actions_get(AtkAction *action)
{
   return MULTIBUTTONENTRY_ACTIONS_NUMBER;
}

/**
 * @brief Performs i-th action
 *
 * @param action an AtkAction instance
 * @param i action index
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
eail_multibuttonentry_do_action(AtkAction *action,
                                int i)
{
   Evas_Object *widget;
   const char *action_name;

   g_return_val_if_fail(EAIL_IS_MULTIBUTTONENTRY(action), FALSE);

   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   if (!widget) return FALSE;

   if ((elm_object_disabled_get(widget)) || (!evas_object_visible_get(widget)))
     return FALSE;

   action_name = atk_action_get_name(action, i);
   if (!action_name) return FALSE;

   if (elm_multibuttonentry_expanded_get(widget))
     elm_multibuttonentry_expanded_set(widget, EINA_FALSE);
   else
     elm_multibuttonentry_expanded_set(widget, EINA_TRUE);

   if (!g_strcmp0(action_name, "expand"))
     evas_object_smart_callback_call(widget, "expanded", NULL);
   else if(!g_strcmp0(action_name, "shrink"))
     evas_object_smart_callback_call(widget, "contracted", NULL);

   return TRUE;
}

/*
 * Calbacks
 */

/**
 * @brief Function called whenever multibutton widget is contracted
 *
 * @param data user data passed to callback
 * @param obj source object
 * @param event_info event info
 */
static void
_on_shrink(void *data,
           Evas_Object *obj,
           void *event_info)
{
    EailMultibuttonentry *multi = EAIL_MULTIBUTTONENTRY(data);
    if (multi->state == EAIL_MULTIBUTTONENTRY_EXPANDED)
     {
        atk_object_notify_state_change(ATK_OBJECT(multi), ATK_STATE_EXPANDED, FALSE);
        multi->state = EAIL_MULTIBUTTONENTRY_SHRINKED;
      }
}

/**
 * @brief Function called whenever multibutton widget is expanded
 *
 * @param data user data passed to callback
 * @param obj source object
 * @param event_info event info
 */
static void
_on_expand(void *data,
           Evas_Object *obj,
           void *event_info)
{
    EailMultibuttonentry *multi = EAIL_MULTIBUTTONENTRY(data);
    if (multi->state == EAIL_MULTIBUTTONENTRY_SHRINKED)
      {
        atk_object_notify_state_change(ATK_OBJECT(multi), ATK_STATE_EXPANDED, TRUE);
        multi->state = EAIL_MULTIBUTTONENTRY_EXPANDED;
      }
}
