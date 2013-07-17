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
 * @file eail_flipselector.h
 */

#ifndef EAIL_FLIPSELECTOR_H
#define EAIL_FLIPSELECTOR_H

#include "eail_widget.h"

#define EAIL_TYPE_FLIPSELECTOR              (eail_flipselector_get_type())
#define EAIL_FLIPSELECTOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                             EAIL_TYPE_FLIPSELECTOR, EailFlipselector))
#define EAIL_FLIPSELECTOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), \
                                             EAIL_TYPE_FLIPSELECTOR, EailFlipselectorClass));
#define EAIL_IS_FLIPSELECTOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                             EAIL_TYPE_FLIPSELECTOR))
#define EAIL_IS_FLIPSELECTOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                             EAIL_TYPE_FLIPSELECTOR))
#define EAIL_FLIPSELECTOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                             EAIL_TYPE_FLIPSELECTOR, EailFlipselectorClass))

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _EailFlipselector      EailFlipselector;
typedef struct _EailFlipselectorClass EailFlipselectorClass;

struct _EailFlipselector
{
   EailWidget parent;

   char *next_description;
   char *prev_description;
};

struct _EailFlipselectorClass
{
   EailWidgetClass parent_class;
};

GType eail_flipselector_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
