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
 * @file eail_multibuttonentry.h
 */

#ifndef EAIL_MULTIBUTTONENTRY_H
#define EAIL_MULTIBUTTONENTRY_H

#include "eail_text.h"

#define EAIL_TYPE_MULTIBUTTONENTRY             (eail_multibuttonentry_get_type())
#define EAIL_MULTIBUTTONENTRY(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                                EAIL_TYPE_MULTIBUTTONENTRY, EailMultibuttonentry))
#define EAIL_MULTIBUTTONENTRY_CLASS(klass)     (G_TYPE_MULTIBUTTONENTRY_CLASS_CAST((klass), \
                                                EAIL_TYPE_MULTIBUTTONENTRY, EailMultibuttonentryClass))
#define EAIL_IS_MULTIBUTTONENTRY(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                                EAIL_TYPE_MULTIBUTTONENTRY))
#define EAIL_IS_MULTIBUTTONENTRY_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                                EAIL_TYPE_MULTIBUTTONENTRY))
#define EAIL_MULTIBUTTONENTRY_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                                EAIL_TYPE_MULTIBUTTONENTRY, EailMultibuttonentryClass))

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _EailMultibuttonentry      EailMultibuttonentry;
typedef struct _EailMultibuttonentryClass EailMultibuttonentryClass;

struct _EailMultibuttonentry
{
   EailText parent;

   int state;
   gchar *shrink_description;
   gchar *expand_description;
};

struct _EailMultibuttonentryClass
{
   EailTextClass parent_class;
};

enum EailMultibuttonentryState
{
    EAIL_MULTIBUTTONENTRY_EXPANDED,
    EAIL_MULTIBUTTONENTRY_SHRINKED
};

GType eail_multibuttonentry_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
