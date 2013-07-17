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
 * @file eail_item_parent.h
 */

#ifndef EAIL_ITEM_PARENT_H
#define EAIL_ITEM_PARENT_H

#include "eail_item.h"

#define EAIL_TYPE_ITEM_PARENT           (eail_item_parent_get_type())
#define EAIL_ITEM_PARENT(obj)           (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                         EAIL_TYPE_ITEM_PARENT, EailItemParent))
#define EAIL_IS_ITEM_PARENT(obj)        (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                         EAIL_TYPE_ITEM_PARENT))
#define EAIL_ITEM_PARENT_GET_IFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE((obj),\
                                         EAIL_TYPE_ITEM_PARENT, EailItemParentIface))

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _EailItemParent      EailItemParent;
typedef struct _EailItemParentIface EailItemParentIface;

struct _EailItemParentIface
{
   GTypeInterface parent;

   const gchar *(*get_item_name)            (EailItemParent   *parent,
                                            EailItem         *item);
   AtkRole      (*get_item_role)            (EailItemParent   *parent,
                                            EailItem         *item);
   gint         (*get_item_index_in_parent) (EailItemParent   *parent,
                                            EailItem         *item);
   AtkStateSet *
                (*ref_item_state_set)       (EailItemParent   *parent,
                                            EailItem         *item,
                                            AtkStateSet      *state_set);

   /* AtkComponent interface */
   gboolean    (*grab_item_focus)          (EailItemParent   *parent,
                                            EailItem         *item);

   void        (*get_item_extents)         (EailItemParent   *parent,
                                            EailItem         *item,
                                            gint             *x,
                                            gint             *y,
                                            gint             *width,
                                            gint             *height,
                                            AtkCoordType      coord_type);

   Evas_Object * (*get_evas_obj)           (EailItemParent   *parent,
                                            EailItem         *item);

   gboolean    (*is_click_supported)       (EailItemParent   *parent,
                                            EailItem         *item);

   gboolean    (*is_content_get_supported) (EailItemParent   *parent,
                                            EailItem         *item);
};

/** @brief Initializer for EailParentInterface */
GType   eail_item_parent_get_type          (void);

/**
 * @brief Gets name for given EailItem
 */
const gchar * eail_item_parent_get_item_name     (EailItemParent   *parent,
                                            EailItem         *item);

/**
 * @brief Gets role for given EailItem
 */
AtkRole eail_item_parent_get_item_role     (EailItemParent   *parent,
                                            EailItem         *item);

/**
 * @brief Gets index in parent for given EailItem
 */
gint    eail_item_parent_get_item_index_in_parent(EailItemParent   *parent,
                                                  EailItem         *item);

/**
 * @brief Gets ref state for given EailItem
 */
AtkStateSet *
        eail_item_parent_ref_item_state_set (EailItemParent   *parent,
                                             EailItem         *item,
                                             AtkStateSet      *state_set);

/**
 * @brief Implementation of grab_focus func for given EailItem
 */
gboolean
        eail_item_parent_grab_item_focus    (EailItemParent   *parent,
                                             EailItem         *item);

/**
 * @brief Implementation of get_item_extents_focus for given EailItem
 */
void    eail_item_parent_get_item_extents   (EailItemParent   *parent,
                                             EailItem         *item,
                                             gint             *x,
                                             gint             *y,
                                             gint             *width,
                                             gint             *height,
                                             AtkCoordType      coord_type);

/**
 * @brief Returns state of 'click' event support for given EailItem
 */
gboolean eail_item_parent_is_click_supported (EailItemParent   *parent,
                                              EailItem         *item);

/**
 * @brief Returns state of content_get  support for given EailItem
 */
gboolean eail_item_parent_is_is_content_get_supported(EailItemParent   *parent,
                                                      EailItem         *item);

/**
 * @brief Return nested evas object for given EailItem
 */
Evas_Object * eail_item_parent_get_evas_obj (EailItemParent   *parent,
                                             EailItem         *item);

#ifdef __cplusplus
}
#endif

#endif
