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
 * @file eail_fileselector.c
 * @brief Fileselector implementation
 */

#include <Elementary.h>

#include "eail_fileselector.h"
#include "eail_item.h"
#include "eail_item_parent.h"
#include "eail_priv.h"

/**
 * @brief definition of EailFileselector as GObject
 *
 * EailFileselector is extended EAIL_TYPE_LAYOUT
 */
G_DEFINE_TYPE(EailFileselector, eail_fileselector, EAIL_TYPE_LAYOUT)

/*
 * Implementation of the *AtkObject* interface
 */

/**
 * @brief Initializer for Fileselector AtkObjectClass
 */
static void
eail_fileselector_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_fileselector_parent_class)->initialize(obj, data);
   obj->role = ATK_ROLE_FILE_CHOOSER;
}

/**
 * @brief EailFileselector GObject instance initializer
 */
static void
eail_fileselector_init(EailFileselector *fileselector)
{
}

/**
 * @brief Destructor for EailFileselector
 */
static void
eail_fileselector_finalize(GObject *object)
{
   G_OBJECT_CLASS(eail_fileselector_parent_class)->finalize(object);
}

/**
 * @brief Initializer for EailFileselector GObject class (defines callbacks for
 * base AtkObject)
 */
static void
eail_fileselector_class_init(EailFileselectorClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   class->initialize = eail_fileselector_initialize;
   gobject_class->finalize = eail_fileselector_finalize;
}
