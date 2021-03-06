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
 * @file eail_web.c
 * @brief Implementation of web widget
 */

#include <atk/atk.h>
#include <Elementary.h>

#include "eail_web.h"
#include "eail_priv.h"

/**
 * @brief EailWeb GObject definition
 *
 * It extends EAIL_TYPE_WIDGET class
 */
G_DEFINE_TYPE(EailWeb, eail_web, EAIL_TYPE_WIDGET);

/*
 * Implementation of the *AtkObject* interface
 */

/**
 * @brief Initializer for AtkObject
 */
static void
eail_web_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_web_parent_class) ->initialize(obj, data);
   obj->role = ATK_ROLE_HTML_CONTAINER;
}

/**
 * @brief EailWeb GObject instance initializer
 */
static void
eail_web_init(EailWeb *web)
{

}

/**
 * @brief Implementation AtkObject->get_n_children callback
 *
 * ATK doc says:\n
 * Gets the number of accessible children of the accessible.
 * @param obj an AtkObject
 *
 * @returns an integer representing the number of accessible children of
 * the accessible
 */
static gint
eail_web_get_n_children(AtkObject *obj)
{
   /* impossible to get content so returning 0 */
   return 0;
}

/**
 * @brief Implementation AtkObject->ref_child callback
 *
 * ATK doc says:\n
 * Gets a reference to the specified accessible child of the object. The
 * accessible children are 0-based so the first accessible child is at index 0,
 * the second at index 1 and so on.
 *
 * @param obj an AtkObject
 * @param i index of a child
 *
 * @returns an AtkObject representing the specified accessible child of the
 * accessible.
 */
static AtkObject *
eail_web_ref_child(AtkObject *obj, gint i)
{
   /* impossible to get content so returning NULL */
   return NULL;
}

/**
 * @brief Destructor of EailWeb object
 */
static void
eail_web_finalize(GObject *object)
{
   G_OBJECT_CLASS(eail_web_parent_class)->finalize(object);
}

/**
 * @brief Initializer for EailWeb GObject class (defines callbacks for
 * base AtkObject)
 */
static void
eail_web_class_init(EailWebClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   class->initialize = eail_web_initialize;
   class->get_n_children = eail_web_get_n_children;
   class->ref_child = eail_web_ref_child;
   gobject_class->finalize = eail_web_finalize;
}
