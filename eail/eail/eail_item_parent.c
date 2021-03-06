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
 * @file eail_item_parent.c
 * @brief Implementation of interface that is used by EailItem implementation
 * to allow special behavior of wide range item-objects
*/

#include "eail_item_parent.h"

/**
 *
 * @returns GType for initialized interface
 */
GType
eail_item_parent_get_type(void)
{
   static volatile GType type_id__volatile = 0;

   if (g_once_init_enter(&type_id__volatile))
     {
        GType type_id = g_type_register_static_simple(
                                    G_TYPE_INTERFACE,
                                    "EailItemParent",
                                    sizeof(EailItemParentIface),
                                    NULL,
                                    0,
                                    NULL,
                                    0);

        g_once_init_leave(&type_id__volatile, type_id);
     }

   return type_id__volatile;
}

/**
 * @param parent object that holds EailItem in its content
 * @param item an EailItem object
 *
 * @return gchar* representing name of item or NULL if not available
 */
const gchar *
eail_item_parent_get_item_name(EailItemParent   *parent,
                               EailItem         *item)
{
   EailItemParentIface *iface;

   g_return_val_if_fail(EAIL_IS_ITEM_PARENT(parent), NULL);

   iface = EAIL_ITEM_PARENT_GET_IFACE(parent);

   if (iface->get_item_name)
     return iface->get_item_name(parent, item);

   return NULL;
}

/**
 * @param parent object that holds EailItem in its content
 * @param item an EailItem object
 *
 * @returns ATK role for given EailItem
 */
AtkRole
eail_item_parent_get_item_role(EailItemParent   *parent,
                               EailItem         *item)
{
   EailItemParentIface *iface;

   g_return_val_if_fail(EAIL_IS_ITEM_PARENT(parent), ATK_ROLE_INVALID);

   iface = EAIL_ITEM_PARENT_GET_IFACE(parent);

   if (iface->get_item_role)
     return iface->get_item_role(parent, item);

   return ATK_OBJECT(item)->role;
}

/**
 * @param parent object that holds EailItem in its content
 * @param item an EailItem object
 *
 * @returns gint representing index of item in parent object
 */
gint
eail_item_parent_get_item_index_in_parent(EailItemParent    *parent,
                                          EailItem          *item)
{
   EailItemParentIface *iface;

   g_return_val_if_fail(EAIL_IS_ITEM_PARENT(parent), -1);

   iface = EAIL_ITEM_PARENT_GET_IFACE(parent);

   if (iface->get_item_index_in_parent)
     return iface->get_item_index_in_parent(parent, item);

   return -1;
}

/**
 * @param parent object that holds EailItem in its content
 * @param item an EailItem object
 * @param state_set current state_set
 *
 * @returns AtkStateSet representing state set of given EailItem
 */
AtkStateSet *
eail_item_parent_ref_item_state_set(EailItemParent  *parent,
                                    EailItem        *item,
                                    AtkStateSet     *state_set)
{
   EailItemParentIface *iface;

   g_return_val_if_fail(EAIL_IS_ITEM_PARENT(parent), NULL);

   iface = EAIL_ITEM_PARENT_GET_IFACE(parent);

   if (iface->ref_item_state_set)
     return iface->ref_item_state_set(parent, item, state_set);

   return state_set;
}

/**
 * @param parent object that holds EailItem in its content
 * @param item an EailItem object
 *
 * @returns TRUE if operation was successfull, FALSE otherwise
 */
gboolean
eail_item_parent_grab_item_focus(EailItemParent *parent,
                                 EailItem       *item)
{
   EailItemParentIface *iface;

   g_return_val_if_fail(EAIL_IS_ITEM_PARENT(parent), FALSE);

   iface = EAIL_ITEM_PARENT_GET_IFACE(parent);

   if (iface->grab_item_focus)
     return iface->grab_item_focus(parent, item);

   return FALSE;
}

/**
 * ATK doc says:
 * Gets the rectangle which gives the extent of the component.
 *
 * @param parent object that holds EailItem in its content
 * @param item an EailItem object
 * @param x address of gint to put x coordinate
 * @param y address of gint to put y coordinate
 * @param width ddress of gint to put width
 * @param height address of gint to put height
 * @param coord_type specifies whether the coordinates are relative to the
 * screen or to the components top level window
 */
void
eail_item_parent_get_item_extents(EailItemParent    *parent,
                                  EailItem          *item,
                                  gint                *x,
                                  gint                *y,
                                  gint                *width,
                                  gint                *height,
                                  AtkCoordType         coord_type)
{
   EailItemParentIface *iface;

   g_return_if_fail(EAIL_IS_ITEM_PARENT(parent));

   iface = EAIL_ITEM_PARENT_GET_IFACE(parent);

   if (iface->get_item_extents)
     iface->get_item_extents(parent, item, x, y, width, height, coord_type);
}

/**
 * @param parent object that holds EailItem in its content
 * @param item an EailItem object
 *
 * @returns nested Evas_Object for given EailItem
 */
Evas_Object * eail_item_parent_get_evas_obj(EailItemParent   *parent,
                                            EailItem         *item)
{
   EailItemParentIface *iface;

   g_return_val_if_fail(EAIL_IS_ITEM_PARENT(parent), NULL);

   iface = EAIL_ITEM_PARENT_GET_IFACE(parent);

   if (iface->get_evas_obj)
     return iface->get_evas_obj(parent, item);

   return NULL;
}

/**
 * @param parent object that holds EailItem in its content
 * @param item an EailItem object
 *
 * @returns TRUE if 'click' is supported, FALSE otherwise
 */
gboolean
eail_item_parent_is_click_supported(EailItemParent *parent,
                                    EailItem       *item)
{
   EailItemParentIface *iface;

   g_return_val_if_fail(EAIL_IS_ITEM_PARENT(parent), FALSE);

   iface = EAIL_ITEM_PARENT_GET_IFACE(parent);

   if (iface->is_click_supported)
     return iface->is_click_supported(parent, item);

   return FALSE;
}

/**
 * @param parent object that holds EailItem in its content
 * @param item an EailItem object
 *
 * @returns TRUE if content get is supported, FALSE otherwise. Default
 * implementation returns TRUE (used if no redefinition in EailItemParent
 * interface implementation is defined)
 */
gboolean
eail_item_parent_is_is_content_get_supported(EailItemParent *parent,
                                             EailItem       *item)
{
   EailItemParentIface *iface;

   g_return_val_if_fail(EAIL_IS_ITEM_PARENT(parent), FALSE);

   iface = EAIL_ITEM_PARENT_GET_IFACE(parent);

   if (iface->is_content_get_supported)
     return iface->is_content_get_supported(parent, item);

   return TRUE;
}
