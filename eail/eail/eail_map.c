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
 * @file eail_map.c
 * @brief EailMap implementation
 */

#include <Elementary.h>

#include "eail_map.h"

static void akt_image_interface_init(AtkImageIface *iface);

/**
 * @brief EailMap type definition
 */
G_DEFINE_TYPE_WITH_CODE(EailMap,
                        eail_map,
                        EAIL_TYPE_IMAGE,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_IMAGE,
                                              akt_image_interface_init)
)

/**
 * @brief EailMap initializer
 *
 * @param object an AtkObject
 * @param data initialization data
 */
static void
eail_map_initialize(AtkObject *object, gpointer data)
{
   ATK_OBJECT_CLASS(eail_map_parent_class)->initialize(object, data);
}

/**
 * @brief EailMap finalizer
 *
 * @param object a GObject
 */
static void
eail_map_finalize(GObject *object)
{
   EailMap *map = EAIL_MAP(object);
   eina_stringshare_del(map->name);

   G_OBJECT_CLASS(eail_map_parent_class)->finalize(object);
}

/**
 * @brief Gets object name
 *
 * EailMap name if name is assigned, map path otherwise
 *
 * @param object an AtkObject
 * @return object name
 */
static const gchar*
eail_map_name_get(AtkObject *object)
{
   Evas_Object *widget;
   EailMap *map;

   g_return_val_if_fail(EAIL_IS_MAP(object), NULL);

   map = EAIL_MAP(object);
   if (map->name) return map->name;

   widget = eail_widget_get_widget(EAIL_WIDGET(map));
   if (!widget) return NULL;

   map->name =
       eina_stringshare_printf("Map(%s)",
                               elm_map_source_get(widget,
                                                  ELM_MAP_SOURCE_TYPE_NAME));
   return map->name;
}

/**
 * @brief EailMap instance initializer
 *
 * @param map an EailMap
 */
static void
eail_map_init(EailMap *map)
{
   map->name = NULL;
}

/**
 * @brief EailMap class initializer
 *
 * @param klass an EailMapClass
 */
static void
eail_map_class_init(EailMapClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   GObjectClass *g_object_class = G_OBJECT_CLASS(klass);

   class->initialize = eail_map_initialize;
   class->get_name = eail_map_name_get;

   g_object_class->finalize = eail_map_finalize;

}

/**
 * @brief Gets image size
 *
 * Image size is the same as window size
 *
 * @param image an AtkImage
 * @param width image width
 * @param height image height
 */
static void
eail_map_image_size_get(AtkImage *image, gint *width, gint *height)
{
   atk_component_get_size(ATK_COMPONENT(image), width, height);
}

/**
 * @brief AtkImage interface initializer
 *
 * @param iface an AtkImage interface
 */
static void
akt_image_interface_init(AtkImageIface *iface)
{
   if (!iface) return;

   iface->get_image_size = eail_map_image_size_get;
}
