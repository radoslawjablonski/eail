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
 * @file eail_action_widget.h
 */

#ifndef EAIL_ACTION_WIDGET_H
#define EAIL_ACTION_WIDGET_H

#include "eail_widget.h"

#define EAIL_TYPE_ACTION_WIDGET             (eail_action_widget_get_type())
#define EAIL_ACTION_WIDGET(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                             EAIL_TYPE_ACTION_WIDGET, EailActionWidget))
#define EAIL_ACTION_WIDGET_CLASS(klass)     (G_TYPE_ACTION_WIDGET_CLASS_CAST((klass), \
                                             EAIL_TYPE_ACTION_WIDGET, EailActionWidgetClass))
#define EAIL_IS_ACTION_WIDGET(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                             EAIL_TYPE_ACTION_WIDGET))
#define EAIL_IS_ACTION_WIDGET_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                             EAIL_TYPE_ACTION_WIDGET))
#define EAIL_ACTION_WIDGET_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                             EAIL_TYPE_ACTION_WIDGET, EailActionWidgetClass))

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _EailActionWidget      EailActionWidget;
typedef struct _EailActionWidgetClass EailActionWidgetClass;

/*! \brief Callback used for performing work-action by given ActionObject
 */
typedef gboolean (* Eail_Do_Action_Cb) (AtkAction *obj, void *data);

/*! \brief Appends given action to action list used by EailActionWidget
 */
void eail_action_widget_action_append(EailActionWidget *action_widget,
                                      const gchar *action_name,
                                      const gchar *keybinding,
                                      Eail_Do_Action_Cb action_cb);

struct _EailActionWidget
{
   EailWidget parent;/*!< widget that is being extended */

   Eina_List *action_objs;/*!< list of action objects (EailActionObj*) */
};

struct _EailActionWidgetClass
{
   EailWidgetClass parent_class;
};

GType eail_action_widget_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
