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
 * @file eail.c
 * @brief Eail initialization part
 */

#include <Elementary.h>

#include <gmodule.h>
#include <atk-bridge.h>
#include "eail.h"
#include "eail_app.h"
#include "eail_priv.h"

/**
 * @brief Domain index that will be used as the DOMAIN parameter on EINA log macros.
 *
 * A negative return value means an log occurred.
 */
int _eail_log_dom = -1;

/**
 * @brief This is used to track last focused atk object
 */
static AtkObject *eail_atk_last_focused_obj = NULL;

/**
 * @brief Gets toolkint name
 * @return toolkint name
 */
static const gchar * eail_get_toolkit_name(void)
{
   return "elementary";
}

/**
 * @brief Gets toolkit version
 *
 * @return toolkit version
 */
static const gchar * eail_get_toolkit_version(void)
{
   return g_strdup_printf("%i.%i.%i", elm_version->major,
                          elm_version->minor,
                          elm_version->micro);
}

/**
 * @brief Gets the root accessible container for the current application
 *
 * @return root Accessible container
 */
static AtkObject * eail_get_root(void)
{
   static AtkObject *root = NULL;

   if (!root)
     {
        root = g_object_new(EAIL_TYPE_APP, NULL);
        atk_object_initialize(root, NULL);
   }

   return root;
}

/**
 * @brief Callback to be called when an object receives focus
 *
 * @param current_focused_obj an AtkObject
 */
static void
eail_focus_listener_cb(AtkObject *current_focused_obj)
{
   if (current_focused_obj == eail_atk_last_focused_obj)
     return;

   if (eail_atk_last_focused_obj)
     g_object_unref(eail_atk_last_focused_obj);

   g_object_ref(current_focused_obj);

   eail_atk_last_focused_obj = current_focused_obj;
}

/**
 * @brief Initializes object focus tracking
 */
static void
eail_app_focus_listener_init()
{
   atk_add_focus_tracker(eail_focus_listener_cb);
}

/**
 * @brief AtkUtil class initialization
 */
static void atk_util_install(void)
{
   AtkUtilClass *uclass;

   uclass = ATK_UTIL_CLASS(g_type_class_ref(ATK_TYPE_UTIL));
   uclass->get_toolkit_name = eail_get_toolkit_name;
   uclass->get_toolkit_version = eail_get_toolkit_version;
   uclass->get_root = eail_get_root;

   eail_app_focus_listener_init();

   uclass->add_global_event_listener = NULL;
   uclass->remove_global_event_listener = NULL;
   uclass->add_key_event_listener = NULL;
   uclass->remove_key_event_listener = NULL;
}

/**
 * @brief Function to be executed by Elementary when EAIL module is loaded
 *
 * @param m Elm_Module
 * @return 1 on success, otherwise 0
 */
int
elm_modapi_init(void *m)
{
   static gboolean initialized = FALSE;
   if (initialized) return 1;

   _eail_log_dom = eina_log_domain_register("eail", EAIL_LOG_COLOR);
   if (!_eail_log_dom)
     {
        EINA_LOG_ERR("could not register eail log domain.");
        _eail_log_dom = EINA_LOG_DOMAIN_GLOBAL;
     }

   if (!ecore_main_loop_glib_integrate())
     {
        ERR("Cannot integrate with glib main loop");
        return 0;
     }

   initialized = TRUE;

#if !GLIB_CHECK_VERSION(2,35,0)
   g_type_init();
#endif

   atk_util_install();

   atk_misc_instance = g_object_new(ATK_TYPE_MISC, NULL);

   atk_bridge_adaptor_init(NULL, NULL);

   return 1;

}

/**
 * @brief Function to be executed by Elementary when EAIL module is unloaded
 *
 * @param m Elm_Module
 * @return always one
 */
int
elm_modapi_shutdown(void *m)
{
   if ((_eail_log_dom > -1) && (_eail_log_dom != EINA_LOG_DOMAIN_GLOBAL))
     {
        eina_log_domain_unregister(_eail_log_dom);
        _eail_log_dom = -1;
     }
   /*always succeed*/
   return 1;
}

