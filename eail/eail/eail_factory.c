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
 * @file eail_factory.c
 * @brief Implementation of factory of EAIL accessible widgets
 */

#include <Elementary.h>

#include "eail_factory.h"
#include "eail_widget.h"
#include "eail_window.h"
#include "eail_background.h"
#include "eail_box.h"
#include "eail_list.h"
#include "eail_genlist.h"
#include "eail_label.h"
#include "eail_button.h"
#include "eail_icon.h"
#include "eail_entry.h"
#include "eail_frame.h"
#include "eail_scroller.h"
#include "eail_slider.h"
#include "eail_action_slider.h"
#include "eail_inwin.h"
#include "eail_image.h"
#include "eail_check.h"
#include "eail_radio_button.h"
#include "eail_menu.h"
#include "eail_photo.h"
#include "eail_photocam.h"
#include "eail_separator.h"
#include "eail_spinner.h"
#include "eail_clock.h"
#include "eail_calendar.h"
#include "eail_grid.h"
#include "eail_route.h"
#include "eail_dayselector.h"
#include "eail_gengrid.h"
#include "eail_progressbar.h"
#include "eail_disk_selector.h"
#include "eail_segment_control.h"
#include "eail_conformant.h"
#include "eail_colorselector.h"
#include "eail_notify.h"
#include "eail_naviframe.h"
#include "eail_datetime.h"
#include "eail_hover.h"
#include "eail_popup.h"
#include "eail_ctxpopup.h"
#include "eail_toolbar.h"
#include "eail_multibuttonentry.h"
#include "eail_web.h"
#include "eail_index.h"
#include "eail_fileselector.h"
#include "eail_fileselector_entry.h"
#include "eail_panes.h"
#include "eail_panel.h"
#include "eail_thumb.h"
#include "eail_mapbuf.h"
#include "eail_slideshow.h"
#include "eail_hoversel.h"
#include "eail_map.h"
#include "eail_glview.h"
#include "eail_bubble.h"
#include "eail_plug.h"
#include "eail_video.h"
#include "eail_flip.h"
#include "eail_flipselector.h"
#include "eail_layout.h"
#include "eail_table.h"
#include "eail_prefs.h"
#include "eail_priv.h"

/**
 * @param widget an Evas_Object*
 *
 * @returns reference to AtkObject* which is accessible representation of given
 * Evas_Object. It should be unreferenced when no longer needed to free memory
 */
AtkObject *
eail_factory_get_accessible(Evas_Object *widget)
{
   const char *type = NULL;
   AtkObject *accessible = NULL;

   if (!elm_object_widget_check(widget))
     {
        DBG("%s is NOT elementary object widget!",
            evas_object_type_get(widget));
        return NULL;
     }

   type = elm_object_widget_type_get(widget);

   if (!strcmp(type, "elm_win"))
     {
        accessible = g_object_new(EAIL_TYPE_WINDOW, NULL);
     }
   else if (!strcmp(type, "elm_bg"))
     {
        accessible = g_object_new(EAIL_TYPE_BACKGROUND, NULL);
     }
   else if (!strcmp(type, "elm_box"))
     {
        accessible = g_object_new(EAIL_TYPE_BOX, NULL);
     }
   else if (!strcmp(type, "elm_list"))
     {
        accessible = g_object_new(EAIL_TYPE_LIST, NULL);
     }
   else if (!strcmp(type, "elm_genlist"))
     {
        accessible = g_object_new(EAIL_TYPE_GENLIST, NULL);
     }
   else if (!strcmp(type, "elm_label"))
     {
        accessible = g_object_new(EAIL_TYPE_LABEL, NULL);
     }
   else if (!strcmp(type, "elm_button"))
     {
        accessible = g_object_new(EAIL_TYPE_BUTTON, NULL);
     }
   else if (!strcmp(type, "elm_icon"))
     {
        accessible = g_object_new(EAIL_TYPE_ICON, NULL);
     }
   else if ((!strcmp(type, "entry")) || (!strcmp(type, "elm_entry")))
     {
        accessible = g_object_new(EAIL_TYPE_ENTRY, NULL);
     }
   else if (!strcmp(type, "elm_frame"))
     {
        accessible = g_object_new(EAIL_TYPE_FRAME, NULL);
     }
   else if (!strcmp(type, "elm_scroller"))
     {
        accessible = g_object_new(EAIL_TYPE_SCROLLER, NULL);
     }
   else if (!strcmp(type, "elm_inwin"))
     {
        accessible = g_object_new(EAIL_TYPE_INWIN, NULL);
     }
   else if (!strcmp(type, "elm_slider"))
     {
        accessible = g_object_new(EAIL_TYPE_SLIDER, NULL);
     }
   else if (!strcmp(type, "elm_actionslider"))
     {
        accessible = g_object_new(EAIL_TYPE_ACTION_SLIDER, NULL);
     }
   else if (!strcmp(type, "elm_image"))
     {
        accessible = g_object_new(EAIL_TYPE_IMAGE, NULL);
     }
   else if (!strcmp(type, "elm_check"))
     {
        accessible = g_object_new(EAIL_TYPE_CHECK, NULL);
     }
   else if (!strcmp(type, "elm_radio"))
     {
        accessible = g_object_new(EAIL_TYPE_RADIO_BUTTON, NULL);
     }
   else if (!strcmp(type, "elm_menu"))
     {
        accessible = g_object_new(EAIL_TYPE_MENU, NULL);
     }
   else if (!strcmp(type, "elm_photo"))
     {
        accessible = g_object_new(EAIL_TYPE_PHOTO, NULL);
     }
   else if (!strcmp(type, "elm_photocam"))
     {
        accessible = g_object_new(EAIL_TYPE_PHOTOCAM, NULL);
     }
   else if (!strcmp(type, "elm_separator"))
     {
        accessible = g_object_new(EAIL_TYPE_SEPARATOR, NULL);
     }
   else if (!strcmp(type, "elm_spinner"))
     {
        accessible = g_object_new(EAIL_TYPE_SPINNER, NULL);
     }
   else if (!strcmp(type, "elm_clock"))
     {
        accessible = g_object_new(EAIL_TYPE_CLOCK, NULL);
     }
   else if (!strcmp(type, "elm_calendar"))
     {
        accessible = g_object_new(EAIL_TYPE_CALENDAR, NULL);
     }
   else if (!strcmp(type, "elm_grid"))
     {
        accessible = g_object_new(EAIL_TYPE_GRID, NULL);
     }
   else if (!strcmp(type, "elm_route"))
     {
        accessible = g_object_new(EAIL_TYPE_ROUTE, NULL);
     }
   else if (!strcmp(type, "elm_dayselector"))
     {
        accessible = g_object_new(EAIL_TYPE_DAYSELECTOR, NULL);
     }
   else if (!strcmp(type, "elm_gengrid"))
     {
        accessible = g_object_new(EAIL_TYPE_GENGRID, NULL);
     }
   else if (!strcmp(type, "elm_progressbar"))
     {
        accessible = g_object_new(EAIL_TYPE_PROGRESSBAR, NULL);
     }
   else if (!strcmp(type, "elm_diskselector"))
     {
        accessible = g_object_new(EAIL_TYPE_DISK_SELECTOR, NULL);
     }
   else if (!strcmp(type, "elm_segment_control"))
     {
        accessible = g_object_new(EAIL_TYPE_SEGMENT_CONTROL, NULL);
     }
   else if (!strcmp(type, "elm_conformant"))
     {
        accessible = g_object_new(EAIL_TYPE_CONFORMANT, NULL);
     }
   else if (!strcmp(type, "elm_notify"))
     {
        accessible = g_object_new(EAIL_TYPE_NOTIFY, NULL);
     }
   else if ((!strcmp(type, "popup")) || (!strcmp(type, "elm_popup")))
     {
        accessible = g_object_new(EAIL_TYPE_POPUP, NULL);
     }
   else if ((!strcmp(type, "ctxpopup")) || ((!strcmp(type, "elm_ctxpopup"))))
     {
        accessible = g_object_new(EAIL_TYPE_CTXPOPUP, NULL);
     }
   else if ((!strcmp(type, "toolbar")) || (!strcmp(type, "elm_toolbar")))
     {
        accessible = g_object_new(EAIL_TYPE_TOOLBAR, NULL);
     }
   else if (!strcmp(type, "elm_multibuttonentry"))
     {
        accessible = g_object_new(EAIL_TYPE_MULTIBUTTONENTRY, NULL);
     }
   else if (!strcmp(type, "elm_web"))
     {
        accessible = g_object_new(EAIL_TYPE_WEB, NULL);
     }
   else if (!strcmp(type, "elm_index"))
     {
        accessible = g_object_new(EAIL_TYPE_INDEX, NULL);
     }
   else if (!strcmp(type, "elm_fileselector"))
     {
        accessible = g_object_new(EAIL_TYPE_FILESELECTOR, NULL);
     }
   else if ((!strcmp(type, "elm_fileselector_entry")) ||
            (!strcmp(type, "fileselector_entry")))
     {
        accessible = g_object_new(EAIL_TYPE_FILESELECTOR_ENTRY, NULL);
     }
   else if (!strcmp(type, "elm_fileselector_button"))
     {
        /* NOTE: file selector button from accesibility point of view is
         * no different in handling than regular push button */
        accessible = g_object_new(EAIL_TYPE_BUTTON, NULL);
     }
   else if (!strcmp(type, "elm_colorselector"))
     {
        accessible = g_object_new(EAIL_TYPE_COLORSELECTOR, NULL);
     }
   else if (!strcmp(type, "elm_naviframe"))
     {
        accessible = g_object_new(EAIL_TYPE_NAVIFRAME, NULL);
     }
   else if (!strcmp(type, "elm_datetime"))
     {
        accessible = g_object_new(EAIL_TYPE_DATETIME, NULL);
     }
   else if (!strcmp(type, "elm_hover"))
     {
        accessible = g_object_new(EAIL_TYPE_HOVER, NULL);
     }
   else if (!strcmp(type, "elm_panes"))
     {
        accessible = g_object_new(EAIL_TYPE_PANES, NULL);
     }
   else if ((!strcmp(type, "panel")) || (!strcmp(type, "elm_panel")))
     {
        accessible = g_object_new(EAIL_TYPE_PANEL, NULL);
     }
   else if (!strcmp(type, "elm_thumb"))
     {
        accessible = g_object_new(EAIL_TYPE_THUMB, NULL);
     }
   else if (!strcmp(type, "elm_mapbuf"))
     {
        accessible = g_object_new(EAIL_TYPE_MAPBUF, NULL);
     }
   else if (!strcmp(type, "elm_slideshow"))
     {
        accessible = g_object_new(EAIL_TYPE_SLIDESHOW, NULL);
     }
   else if (!strcmp(type, "elm_hoversel"))
     {
        accessible = g_object_new(EAIL_TYPE_HOVERSEL, NULL);
     }
   else if (!strcmp(type, "elm_map"))
     {
        accessible = g_object_new(EAIL_TYPE_MAP, NULL);
     }
   else if (!strcmp(type, "elm_glview"))
     {
        accessible = g_object_new(EAIL_TYPE_GLVIEW, NULL);
     }
   else if (!strcmp(type, "elm_bubble"))
     {
        accessible = g_object_new(EAIL_TYPE_BUBBLE, NULL);
     }
   else if (!strcmp(type, "elm_plug"))
     {
        accessible = g_object_new(EAIL_TYPE_PLUG, NULL);
     }
   else if ((!strcmp(type, "elm_video")) || (!strcmp(type, "elm_player")))
     {
        accessible = g_object_new(EAIL_TYPE_VIDEO, NULL);
     }
   else if (!strcmp(type, "elm_flip"))
     {
        accessible = g_object_new(EAIL_TYPE_FLIP, NULL);
     }
   else if (!strcmp(type, "elm_flipselector"))
     {
        accessible = g_object_new(EAIL_TYPE_FLIPSELECTOR, NULL);
     }
   else if (!strcmp(type, "elm_layout"))
     {
        accessible = g_object_new(EAIL_TYPE_LAYOUT, NULL);
     }
   else if (!strcmp(type, "elm_table"))
     {
        accessible = g_object_new(EAIL_TYPE_TABLE, NULL);
     }
   else if (!strcmp(type, "elm_prefs"))
     {
        accessible = g_object_new(EAIL_TYPE_PREFS, NULL);
     }
   else
     {
        DBG("Unrecognized specific widget type: %s", type);
        DBG("Creating basic widget with NULL type.");
        accessible = g_object_new(EAIL_TYPE_WIDGET, NULL);
     }

   if (accessible)
     {
        atk_object_initialize(accessible, widget);
     }

   return accessible;
}
