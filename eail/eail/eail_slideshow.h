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
 * @file eail_slideshow.h
 */

#ifndef EAIL_SLIDESHOW_H
#define EAIL_SLIDESHOW_H

#include "eail_widget.h"

#define EAIL_TYPE_SLIDESHOW             (eail_slideshow_get_type())
#define EAIL_SLIDESHOW(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                         EAIL_TYPE_SLIDESHOW, EailSlideshow))
#define EAIL_SLIDESHOW_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), \
                                         EAIL_TYPE_SLIDESHOW, \
                                         EailSlideshowClass))
#define EAIL_IS_SLIDESHOW(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                         EAIL_TYPE_SLIDESHOW))
#define EAIL_IS_SLIDESHOW_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                         EAIL_TYPE_SLIDESHOW))
#define EAIL_SLIDESHOW_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                         EAIL_TYPE_SLIDESHOW, \
                                         EailSlideshowClass))

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _EailSlideshow EailSlideshow;
typedef struct _EailSlideshowClass EailSlideshowClass;

struct _EailSlideshow
{
   EailWidget parent;/*!< parent class*/
   gchar *next_description;/*!< next action description*/
   gchar *previous_description;/*!< previous action description*/
   gchar *stop_description;/*!< stop action description*/
   gchar *start_description;/*!< start action description*/
};

struct _EailSlideshowClass
{
   EailWidgetClass parent_class;
};

GType eail_slideshow_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
