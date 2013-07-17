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
 * @file eail_widget.h
 */

#ifndef EAIL_WIDGET_H
#define EAIL_WIDGET_H

#include <atk/atk.h>

#include <Eina.h>
#include <Evas.h>

#define EAIL_TYPE_WIDGET              (eail_widget_get_type())
#define EAIL_WIDGET(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                       EAIL_TYPE_WIDGET, EailWidget))
#define EAIL_WIDGET_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), \
                                       EAIL_TYPE_WIDGET, EailWidgetClass))
#define EAIL_IS_WIDGET(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                       EAIL_TYPE_WIDGET))
#define EAIL_IS_WIDGET_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                       EAIL_TYPE_WIDGET))
#define EAIL_WIDGET_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                       EAIL_TYPE_WIDGET, EailWidgetClass))
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _EailWidget      EailWidget;
typedef struct _EailWidgetClass EailWidgetClass;

struct _EailWidget
{
   AtkObject parent;

   Evas_Object *widget;
   AtkLayer layer;
};

struct _EailWidgetClass
{
   AtkObjectClass parent_class;

   Eina_List * (*get_widget_children)  (EailWidget *widget);
};

GType           eail_widget_get_type              (void);
Evas_Object *   eail_widget_get_widget            (EailWidget *widget);
Eina_List *     eail_widget_get_widget_children   (EailWidget *widget);

#ifdef __cplusplus
}
#endif

#endif
