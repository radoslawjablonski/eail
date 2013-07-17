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
 * @file eail_widget.c
 * @brief EailWidget implementation
 */

#include <Ecore.h>
#include <Ecore_Evas.h>
#include <Elementary.h>

#include "eail_widget.h"
#include "eail_factory.h"
#include "eail_utils.h"
#include "eail_priv.h"

static void atk_component_interface_init(AtkComponentIface *iface);

/**
 * @brief EailWidget type definition
 */
G_DEFINE_TYPE_WITH_CODE(EailWidget, eail_widget, ATK_TYPE_OBJECT,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_COMPONENT,
                                              atk_component_interface_init));

/**
 * @brief EailWidget focus name
 */
#define EAIL_WIDGET_FOCUS_NAME "focused"

/*
 * Public API implementation
 */

/**
 * @brief Gets Evas_Object widget from widget
 *
 * @param widget an EailWidget
 * @return Evas_Object widget
 */
Evas_Object *
eail_widget_get_widget(EailWidget *widget)
{
    g_return_val_if_fail(EAIL_IS_WIDGET(widget), NULL);

    return widget->widget;
}

/**
 * @brief Gets widget children
 *
 * @param widget an EailWidget
 * @return children list
 */
Eina_List *
eail_widget_get_widget_children(EailWidget *widget)
{
    EailWidgetClass *klass;

    g_return_val_if_fail(EAIL_IS_WIDGET(widget), NULL);

    klass = EAIL_WIDGET_GET_CLASS(widget);
    return klass->get_widget_children(widget);
}

/*
 * Implementation of the *AtkObject* interface
 */

/**
 * @brief EailWidget initializer
 *
 * @param obj an AtkObject
 * @param data initialization data
 */
static void
eail_widget_initialize(AtkObject *obj, gpointer data)
{
    EailWidget *widget = EAIL_WIDGET(obj);

    ATK_OBJECT_CLASS(eail_widget_parent_class)->initialize(obj, data);

    widget->widget = (Evas_Object *)data;
    widget->layer = ATK_LAYER_WIDGET;

    obj->name = (gchar *)evas_object_name_get(widget->widget);
    obj->role = ATK_ROLE_UNKNOWN;

    if (!widget->widget) {
        ERR("No evas object inside EailWidget was found");
        return;
    }

    evas_object_smart_callback_add(widget->widget, EAIL_WIDGET_FOCUS_NAME,
                                   eail_widget_on_focused, widget);
}

/**
 * @brief Default get_widget_children callback
 *
 * @param widget an EailWidget
 * @return NULL
 */
static Eina_List *
eail_widget_get_real_widget_children(EailWidget *widget)
{
    return NULL;
}

/**
 * @brief Gets obj children count
 *
 * @param obj an AtkObject
 */
static gint
eail_widget_get_n_children(AtkObject *obj)
{
    gint n_children;
    Eina_List *children;

    children = eail_widget_get_widget_children(EAIL_WIDGET(obj));
    n_children = eina_list_count(children);

    eina_list_free(children);

    return n_children;
}

/**
 * @brief Gets reference to obj i-th child
 *
 * The caller must unreference it when it is no longer needed
 *
 * @param obj an AtkObject
 * @param i child index
 * @return child reference
 */
static AtkObject *
eail_widget_ref_child(AtkObject *obj, gint i)
{
    Eina_List *children;
    AtkObject *child = NULL;

    children = eail_widget_get_widget_children(EAIL_WIDGET(obj));
    if (eina_list_count(children) > i) {
        child = eail_factory_get_accessible(eina_list_nth(children, i));
        g_object_ref(child);
    }

    eina_list_free(children);

    return child;
}

/**
 * @brief Gets obj parent
 *
 * @param obj an AtkObject
 * @return obj parent
 */
static AtkObject *
eail_widget_get_parent(AtkObject *obj)
{
    Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(obj));

    if (!widget) {
        return NULL;
    }

    if (obj->accessible_parent) {
        return obj->accessible_parent;
    }

    return eail_factory_get_accessible(elm_object_parent_widget_get(widget));
}

/**
 * @brief Gets the 0-based index of this accessible in its parent
 *
 * Returns -1 if the accessible does not have an accessible parent
 *
 * @param obj an AtkObject
 * @return obj index in its parent
 */
static gint
eail_widget_get_index_in_parent(AtkObject *obj)
{
    gint index;
    Eina_List *l, *children;
    AtkObject *parent;
    Evas_Object *child;
    Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(obj));

    if (!widget) {
        return -1;
    }

    if (obj->accessible_parent)
    {
        parent = obj->accessible_parent;
    } else {
        parent = atk_object_get_parent(obj);
    }

    if (!parent) {
        return -1;
    }

    index = -1;
    children = eail_widget_get_widget_children(EAIL_WIDGET(parent));
    EINA_LIST_FOREACH(children, l, child) {
        ++index;
        if (child == widget) {
            break;
        }
    }

    eina_list_free(children);

    return index;
}

/**
 * @brief Gets obj state set
 *
 * @param obj an AtkObject
 * @return obj state set
 */
static AtkStateSet *
eail_widget_ref_state_set(AtkObject *obj)
{
    AtkStateSet *state_set;
    Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(obj));

    state_set= ATK_OBJECT_CLASS(eail_widget_parent_class)->ref_state_set(obj);

    return eail_evas_obj_ref_state_set(widget, state_set);
}

/**
 * @brief Gets obj attribute set
 *
 * @param obj an AtkObject
 * @return obj attribute set
 */
static AtkAttributeSet *
eail_widget_get_attributes(AtkObject *obj)
{
    AtkAttribute *attr;
    AtkAttributeSet *attributes;
    Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(obj));

    if (!widget) {
        return NULL;
    }

    attr = g_new(AtkAttribute, 1);
    attr->name = g_strdup("type");
    attr->value = g_strdup(elm_object_widget_type_get(widget));

    attributes = g_slist_append(NULL, attr);

    return attributes;
}

/**
 * @brief EailWidget initializer
 *
 * @param widget an EailWidget
 */
static void
eail_widget_init(EailWidget *widget)
{
}

/**
 * @brief EailWidget finalizer
 *
 * Frees memory assigned to object
 *
 * @param obj a GObject
 */
static void
eail_widget_class_finalize(GObject *obj)
{
    G_OBJECT_CLASS(eail_widget_parent_class)->finalize(obj);
}

/*
 * @brief EailWidget class initializer
 *
 * @param klass an EailWidgetClass
 */
static void
eail_widget_class_init(EailWidgetClass *klass)
{
    AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
    GObjectClass *g_object_class = G_OBJECT_CLASS(klass);

    klass->get_widget_children = eail_widget_get_real_widget_children;

    class->initialize = eail_widget_initialize;
    class->get_n_children = eail_widget_get_n_children;
    class->ref_child = eail_widget_ref_child;
    class->get_parent = eail_widget_get_parent;
    class->get_index_in_parent = eail_widget_get_index_in_parent;
    class->ref_state_set = eail_widget_ref_state_set;
    class->get_attributes = eail_widget_get_attributes;

    g_object_class->finalize = eail_widget_class_finalize;
}

/*
 * Implementation of the *AtkComponent* interface
 */

/**
 * @brief Grabs focus for component
 *
 * @param component an AtkComponent
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
eail_widget_grab_focus(AtkComponent *component)
{
    Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(component));

    return eail_evas_obj_grab_focus(widget);
}

/**
 * @brief Gets component layer
 *
 * @param component an AtkComponent
 * @return component layer
 */
static AtkLayer
eail_widget_get_layer(AtkComponent *component)
{
    return EAIL_WIDGET(component)->layer;
}

/**
 * @brief Gets the rectangle which gives the extent of the component
 *
 * @param component an AtkComponent
 * @param x rectangle upper left x coordinate
 * @param y rectangle upper left y coordinate
 * @param width component width
 * @param height component height
 * @param coord_type coordinates type
 */
static void
eail_widget_get_extents(AtkComponent  *component,
                        gint          *x,
                        gint          *y,
                        gint          *width,
                        gint          *height,
                        AtkCoordType   coord_type)
{
    Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(component));

    *x = *y = G_MININT;
    *width = *height = -1;
    if (!widget) {
        return;
    }

    evas_object_geometry_get(widget, x, y, width, height);

    if (coord_type == ATK_XY_SCREEN) {
        int ee_x, ee_y;
        Ecore_Evas *ee= ecore_evas_ecore_evas_get(evas_object_evas_get(widget));

        ecore_evas_geometry_get(ee, &ee_x, &ee_y, NULL, NULL);
        *x += ee_x;
        *y += ee_y;
    }
}

/**
 * @brief Sets component position
 *
 * @param component an AtkComponent
 * @param x upper left x coordinate
 * @param y upper left y coordinate
 * @param coord_type coordinates type
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
eail_widget_set_position(AtkComponent *component,
                         gint         x,
                         gint         y,
                         AtkCoordType coord_type)
{
    Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(component));

    if (!widget) {
        return FALSE;
    }

    if (coord_type == ATK_XY_SCREEN) {
        int ee_x, ee_y;
        Ecore_Evas *ee= ecore_evas_ecore_evas_get(evas_object_evas_get(widget));

        ecore_evas_geometry_get(ee, &ee_x, &ee_y, NULL, NULL);
        x -= ee_x;
        y -= ee_y;
    }

    if (x < 0 || y < 0) {
        return FALSE;
    }

    evas_object_move(widget, x, y);

    return TRUE;
}

/**
 * @brief Sets component size
 *
 * @param component an AtkComponent
 * @param width new width
 * @param height new height
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
eail_widget_set_size(AtkComponent *component,
                     gint          width,
                     gint          height)
{
    Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(component));

    if (!widget) {
        return FALSE;
    }

    evas_object_resize(widget, width, height);

    return TRUE;
}

/**
 * @brief Sets component extents
 *
 * @param component an AtkComponent
 * @param x upper left x coordinate
 * @param y upper left y coordinate
 * @param width new width
 * @param height new height
 * @param coord_type coordinates type
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
eail_widget_set_extents(AtkComponent  *component,
                        gint           x,
                        gint           y,
                        gint           width,
                        gint           height,
                        AtkCoordType   coord_type)
{
    Evas_Object *widget = eail_widget_get_widget(EAIL_WIDGET(component));

    if (!widget) {
        return FALSE;
    }

    if (coord_type == ATK_XY_SCREEN) {
        int ee_x, ee_y;
        Ecore_Evas *ee= ecore_evas_ecore_evas_get(evas_object_evas_get(widget));

        ecore_evas_geometry_get(ee, &ee_x, &ee_y, NULL, NULL);
        x -= ee_x;
        y -= ee_y;
    }

    if (x < 0 || y < 0) {
        return FALSE;
    }

    evas_object_move(widget, x, y);
    evas_object_resize(widget, width, height);

    return TRUE;
}

/**
 * @brief AktComponent interface initializer
 *
 * @param iface an AtkComponent
 */
static void
atk_component_interface_init(AtkComponentIface *iface)
{
    iface->grab_focus = eail_widget_grab_focus;
    iface->get_layer = eail_widget_get_layer;
    iface->get_extents = eail_widget_get_extents;
    iface->set_position = eail_widget_set_position;
    iface->set_size = eail_widget_set_size;
    iface->set_extents = eail_widget_set_extents;
}
