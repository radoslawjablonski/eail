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
 * @file eail_video.h
 */
#ifndef EAIL_VIDEO_H
#define EAIL_VIDEO_H

#include "eail_widget.h"

#define EAIL_TYPE_VIDEO             (eail_video_get_type ())
#define EAIL_VIDEO(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                     EAIL_TYPE_VIDEO, EailVideo))
#define EAIL_VIDEO_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), \
                                     EAIL_TYPE_VIDEO, EailVideoClass))
#define EAIL_IS_VIDEO(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                     EAIL_TYPE_VIDEO))
#define EAIL_IS_VIDEO_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                                     EAIL_TYPE_VIDEO))
#define EAIL_VIDEO_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                                     EAIL_TYPE_VIDEO, EailVideoClass))
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _EailVideo       EailVideo;
typedef struct _EailVideoClass  EailVideoClass;

struct _EailVideo
{
    EailWidget parent; /*!< EailVideo parent*/
    char *forward_desc; /*!< forward action description*/
    char *next_desc; /*!< next action descritpion*/
    char *pause_desc; /*!< pause action descirption*/
    char *play_desc; /*!< play action description*/
    char *prev_desc; /*!< prev action description*/
    char *rewind_desc; /*!< rewind action description*/
    char *stop_desc; /*!< stop action description*/
    char *uri; /*!< video file URI*/
};

struct _EailVideoClass
{
    EailWidgetClass parent_class;
};

GType eail_video_get_type (void);

#ifdef __cplusplus
}
#endif

#endif
