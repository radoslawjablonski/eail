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
 * @file eail_naviframe_page.h
 */

#ifndef EAIL_NAVIFRAME_PAGE_H
#define EAIL_NAVIFRAME_PAGE_H

#include <Elementary.h>
#include <atk/atk.h>

#define EAIL_TYPE_NAVIFRAME_PAGE            (eail_naviframe_page_get_type())
#define EAIL_NAVIFRAME_PAGE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj),\
                                             EAIL_TYPE_NAVIFRAME_PAGE, EailNaviframePage))
#define EAIL_NAVIFRAME_PAGE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), \
                                             EAIL_TYPE_NAVIFRAME_PAGE, EailNaviframePageClass))
#define EAIL_IS_NAVIFRAME_PAGE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj),\
                                             EAIL_TYPE_NAVIFRAME_PAGE))
#define EAIL_IS_NAVIFRAME_PAGE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                                             EAIL_TYPE_NAVIFRAME_PAGE))
#define EAIL_NAVIFRAME_PAGE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                                             EAIL_TYPE_NAVIFRAME_PAGE, EailNaviframePageClass))

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _EailNaviframePage      EailNaviframePage;
typedef struct _EailNaviframePageClass EailNaviframePageClass;

struct _EailNaviframePage
{
   AtkObject parent; /*!< EailNaviframePage parent*/

   AtkObject *parent_naviframe; /*!< EailNaviframe to which page belongs to*/
   Evas_Object *naviframe; /*!< elm_naviframe instance*/
   Elm_Object_Item *page; /*!< elm_naviframe item instance*/
   //prev button, icon, next button, content
   Evas_Object *content[4]; /*!< holds content of page*/

   gint index; /*!< index in parent*/
   gint child_count; /*!< number of childs*/
   const char *name; /*!< title of page*/
};

struct _EailNaviframePageClass
{
   AtkObjectClass parent_class; /*!< EailNaviframPage parent class*/
};

GType eail_naviframe_page_get_type(void);

/**
 * @brief Creates new EailNaviframPage object
 *
 * @param naviframe EailNaviframe parent
 * @param pagenum index of item in parent object
 *
 * @returns new EailNaviframePage object
 */
AtkObject *eail_naviframe_page_new(AtkObject *naviframe, gint pagenum);

#ifdef __cplusplus
}
#endif

#endif
