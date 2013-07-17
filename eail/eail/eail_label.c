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
 * @file eail_label.c
 * @brief EailLabel implementation.
 */

#include <Elementary.h>

#include "eail_label.h"

G_DEFINE_TYPE(EailLabel, eail_label, EAIL_TYPE_TEXT);

/* AtkObject */
static AtkStateSet *eail_label_ref_state_set(AtkObject *accessible);

/*
 * AtkObject implementation
 */

/**
 * @brief EailLabel initializer.
 *
 * @param obj EailLabel instance
 * @param data initialization data
 */
static void
eail_label_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_label_parent_class)->initialize(obj, data);
   obj->role = ATK_ROLE_LABEL;
}

/**
 * @brief EailLabel interface intializer.
 *
 * @param label EailLabel instance
 */
static void
eail_label_init(EailLabel *label)
{
}

/**
 * @brief EailLabel class initializer.
 *
 * @param klass EailLabel class
 */
static void
eail_label_class_init(EailLabelClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);

   class->initialize = eail_label_initialize;
   class->ref_state_set = eail_label_ref_state_set;
}

/**
 * @brief Gets state set of the accessible.
 *
 * The caller must unreference it when it is no longer needed.
 *
 * @param accessible AtkObject instance
 * @return accessible state set
 */
static AtkStateSet*
eail_label_ref_state_set(AtkObject *accessible)
{
   AtkStateSet *state_set;
   Evas_Object *widget;

   state_set =
       ATK_OBJECT_CLASS(eail_label_parent_class)->ref_state_set(accessible);

   widget = eail_widget_get_widget(EAIL_WIDGET(accessible));
   if (!widget) return state_set;

   atk_state_set_add_state(state_set, ATK_STATE_MULTI_LINE);

   return state_set;
}
