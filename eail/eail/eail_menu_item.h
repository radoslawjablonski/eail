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

#ifndef EAIL_MENU_ITEM_H
#define EAIL_MENU_ITEM_H

#include "eail_item.h"

#define EAIL_TYPE_MENU_ITEM            (eail_menu_item_get_type())
#define EAIL_MENU_ITEM(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                        EAIL_TYPE_MENU_ITEM, EailMenuItem))
#define EAIL_MENU_ITEM_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), \
                                        EAIL_TYPE_MENU_ITEM, EailMenuItemClass))
#define EAIL_IS_MENU_ITEM(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                        EAIL_TYPE_MENU_ITEM))
#define EAIL_IS_MENU_ITEM_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                        EAIL_TYPE_MENU_ITEM))
#define EAIL_MENU_ITEM_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                        EAIL_TYPE_MENU_ITEM, EailMenuItemClass))

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _EailMenuItem      EailMenuItem;
typedef struct _EailMenuItemClass EailMenuItemClass;

struct _EailMenuItem
{
   EailItem parent;
};

struct _EailMenuItemClass
{
   EailItemClass parent_class;
};

GType               eail_menu_item_get_type    (void);
AtkObject *         eail_menu_item_new         (AtkObject *parent);

#ifdef __cplusplus
}
#endif

#endif
