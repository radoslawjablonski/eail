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

#ifndef EAIL_TOOLBAR_H
#define EAIL_TOOLBAR_H

#include "eail_widget.h"

#define EAIL_TYPE_TOOLBAR             (eail_toolbar_get_type())
#define EAIL_TOOLBAR(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                            EAIL_TYPE_TOOLBAR, \
                                            EailToolbar))
#define EAIL_TOOLBAR_CLASS(klass)     (G_TYPE_TOOLBAR_CLASS_CAST((klass), \
                                            EAIL_TYPE_TOOLBAR, \
                                            EailToolbarClass))
#define EAIL_IS_TOOLBAR(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                            EAIL_TYPE_TOOLBAR))
#define EAIL_IS_TOOLBAR_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                            EAIL_TYPE_TOOLBAR))
#define EAIL_TOOLBAR_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                            EAIL_TYPE_TOOLBAR, \
                                            EailToolbarClass))

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _EailToolbar EailToolbar;
typedef struct _EailToolbarClass EailToolbarClass;

struct _EailToolbar
{
   EailWidget parent;
};

struct _EailToolbarClass
{
   EailWidgetClass parent_class;
};

GType eail_toolbar_get_type(void);

#ifdef __cplusplus
}
#endif

#endif

