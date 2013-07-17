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
 * @file eail_check.c
 * @brief EailChceck implementation
 */

#include <Elementary.h>

#include "eail_check.h"

/**
 * @brief Initialization of AtkAction interface
 *
 * @param iface EailCheck object
 */
static void atk_action_interface_init(AtkActionIface *iface);

/**
 * @brief Define EailCheck GObject type
 */
G_DEFINE_TYPE_WITH_CODE(EailCheck,
                        eail_check,
                        EAIL_TYPE_BUTTON,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_ACTION,
                                              atk_action_interface_init))


/**
 * @brief Callback for on_changed event
 * This will be called to notify AtkObject about object state change
 *
 * @param data callback data
 * @param obj object source
 * @param event_info event info
 */
static void
_on_changed(void *data, Evas_Object *obj, void *event_info)
{
   Eina_Bool state = elm_check_state_get(obj);
   atk_object_notify_state_change(ATK_OBJECT(data), ATK_STATE_CHECKED,
                                  state == EINA_TRUE ? TRUE : FALSE);
}

/**
 * @brief Implementation of ref_state_set from AtkObject
 *
 * @param obj EailCheck instance
 *
 * @returns AtkStateSet for EailCheck instance
 */
static AtkStateSet*
eail_check_ref_state_set(AtkObject *obj)
{
    Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(obj));
    AtkStateSet *state_set =
        ATK_OBJECT_CLASS(eail_check_parent_class)->ref_state_set(obj);


    if (NULL == widget)
    {
        atk_state_set_add_state(state_set, ATK_STATE_DEFUNCT);
        return state_set;
    }
    if (elm_check_state_get(widget))
    {
        atk_state_set_add_state(state_set, ATK_STATE_CHECKED);
    }

    return state_set;
}

/**
 * @brief EailRadioButton class desctructor
 *
 * @param object EailRadioButton instance
 */
static void
eail_check_finalize(GObject *object)
{
   EailCheck *check = EAIL_CHECK(object);
   if (check->changed_description) free(check->changed_description);
   G_OBJECT_CLASS(eail_check_parent_class)->finalize(object);
}

/**
 * @brief EailCheck instance initailization
 *
 * @param button EailCheck instance
 */
static void
eail_check_init(EailCheck *button)
{
}

/**
 * @brief EailCheck object initializiation
 *
 * @param obj EailCheck object
 * @param data user set addtional initailization data
 */
static void
eail_check_initialize(AtkObject *obj, gpointer data)
{
    ATK_OBJECT_CLASS(eail_check_parent_class)->initialize(obj, data);
    obj->role = ATK_ROLE_CHECK_BOX;
    /*TODO check whether we can do this better*/
    evas_object_smart_callback_add((Evas_Object*) data, "changed", _on_changed,
                                   obj);
}

/**
 * @brief GObject type initialization function
 *
 * @param klass EailCheck class
 */
static void
eail_check_class_init(EailCheckClass *klass)
{
    AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    class->initialize = eail_check_initialize;
    class->ref_state_set = eail_check_ref_state_set;
    gobject_class->finalize = eail_check_finalize;
}

/*
 * Implementation of the *AtkAction* interface
 */

/**
 * @brief Implementation of get_n_actions from AtkAction interface
 *
 * @param action EailCheck instance
 */
static int
eail_check_n_actions_get(AtkAction *action)
{
   return 1;
}

/**
 * @brief Implementation of get_action_name from AtkAction interface
 *
 * @param action EailCheck instance
 * @param i action index
 *
 * @returns action description
 */
static const char*
eail_check_action_name_get(AtkAction *action,
                           int i)
{
   const char *action_name;

   switch (i)
     {
      case 0:
         /*"check" - This is called whenever the user changes the state of the
          * check objects*/
         action_name = "check";
         break;
      default:
         action_name = NULL;
         break;
     }

   return action_name;
}

/**
 * @brief Implementation of set_descritpion from AtkAction interface
 *
 * @param action EailCheck instance
 * @param i action index
 * @param description action description
 *
 * @returns TRUE on success FALSE otherwise
 */
static gboolean
eail_check_description_set(AtkAction *action,
                           gint i,
                           const char *description)
{
   if (i != 0) return FALSE;

   EailCheck *check = EAIL_CHECK(action);
   if (check->changed_description) free(check->changed_description);
   check->changed_description = g_strdup(description);

   return TRUE;
}

/**
 * @brief Implementation of get_description from AtkAction interface
 *
 * @param action EailCheck instance
 * @param i action index
 *
 * @returns action description
 */
static const char*
eail_check_description_get(AtkAction *action,
                           gint i)
{
   if (i != 0) return NULL;

   EailCheck *check = EAIL_CHECK(action);
   return check->changed_description;
}

/**
 * @brief Implementation of do_action from AtkAction interface
 *
 * @param action EailCheck instance
 * @param i action index
 *
 * @returns TRUE on success FALSE otherwise
 */
static gboolean
eail_check_do_action(AtkAction *action, gint i)
{
   Evas_Object *widget;
   Eina_Bool state;

   if (i != 0) return FALSE;
   g_return_val_if_fail(EAIL_IS_CHECK(action), FALSE);

   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   state = elm_check_state_get(widget);

   elm_check_state_set(widget, !state);
   evas_object_smart_callback_call(widget, "changed", NULL);

   return TRUE;
}

static void
atk_action_interface_init(AtkActionIface *iface)
{
   g_return_if_fail(iface != NULL);

   iface->get_n_actions   = eail_check_n_actions_get;
   iface->get_description = eail_check_description_get;
   iface->get_name        = eail_check_action_name_get;
   iface->set_description = eail_check_description_set;
   iface->do_action = eail_check_do_action;
}
