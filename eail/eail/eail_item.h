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
 * @file eail_item.h
 */

#ifndef EAIL_ITEM_H
#define EAIL_ITEM_H

#include <atk/atk.h>
#include <Elementary.h>

#define EAIL_TYPE_ITEM            (eail_item_get_type())
#define EAIL_ITEM(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                   EAIL_TYPE_ITEM, EailItem))
#define EAIL_ITEM_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), \
                                   EAIL_TYPE_ITEM, EailItemClass))
#define EAIL_IS_ITEM(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                   EAIL_TYPE_ITEM))
#define EAIL_IS_ITEM_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                   EAIL_TYPE_ITEM))
#define EAIL_ITEM_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                   EAIL_TYPE_ITEM, EailItemClass))

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _EailItem      EailItem;
typedef struct _EailItemClass EailItemClass;

struct _EailItem
{
   AtkObject parent;

   Elm_Object_Item *item;
};

struct _EailItemClass
{
   AtkObjectClass parent_class;
};

GType eail_item_get_type(void);
AtkObject *eail_item_new(AtkObject *parent, AtkRole role);
Elm_Object_Item *eail_item_get_item(EailItem *item);

#ifdef __cplusplus
}
#endif

#endif
