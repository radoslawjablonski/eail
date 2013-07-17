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
 * @file eail_label.h
 */

#ifndef EAIL_LABEL_H
#define EAIL_LABEL_H

#include "eail_text.h"

#define EAIL_TYPE_LABEL              (eail_label_get_type())
#define EAIL_LABEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                      EAIL_TYPE_LABEL, EailLabel))
#define EAIL_LABEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), \
                                      EAIL_TYPE_LABEL, EailLabelClass))
#define EAIL_IS_LABEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                      EAIL_TYPE_LABEL))
#define EAIL_IS_LABEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                      EAIL_TYPE_LABEL))
#define EAIL_LABEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                      EAIL_TYPE_LABEL, EailLabelClass))

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _EailLabel      EailLabel;
typedef struct _EailLabelClass EailLabelClass;

struct _EailLabel
{
   EailText parent;
};

struct _EailLabelClass
{
   EailTextClass parent_class;
};

GType eail_label_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
