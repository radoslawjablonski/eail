#include <Elementary.h>
#include <atk/atk.h>

//#define DEBUG 1

#include "eail_test_utils.h"

#define ACTIONS_NUMBER 1

static void
_bubble_clicked(void *data, Evas_Object *obj, void *event_info)
{
   static unsigned char counter = 0;

   if (3 < counter)
     counter = 0;

   elm_bubble_pos_set(obj, counter);

   counter++;
}

static void
_init_bubble(Evas_Object *win)
{
   Evas_Object *bg, *label, *icon, *bubble;

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   label = elm_label_add(win);
   elm_object_text_set(label, "This is the CONTENT of our bubble");
   evas_object_show(label);

   icon = elm_icon_add(win);
   elm_icon_order_lookup_set(icon, ELM_ICON_LOOKUP_THEME_FDO);
   elm_icon_standard_set(icon, "home");
   evas_object_show(icon);

   bubble = elm_bubble_add(win);
   elm_object_part_content_set(bubble, "icon", icon);
   elm_object_part_text_set(bubble, "info", "INFO");
   elm_object_text_set(bubble, "LABEL");
   elm_object_content_set(bubble, label);
   evas_object_smart_callback_add(bubble, "clicked", _bubble_clicked, NULL);
   evas_object_resize(bubble, 300, 100);
   evas_object_show(bubble);
}

static void
_do_test(AtkObject *obj)
{
   const char *type_name = g_type_name(G_TYPE_FROM_INSTANCE(obj));
   const char *type_name_child;
   AtkObject *xobj;

   _printf("type_name: %s\n", type_name);

   g_assert_cmpstr(type_name, ==, "EailBubble");

   // test children
   g_assert(2 == atk_object_get_n_accessible_children(obj));

   for (int count = 0; 2 > count; count++)
     {
       xobj = atk_object_ref_accessible_child(obj, count);
       g_assert(xobj);
       type_name_child = g_type_name(G_TYPE_FROM_INSTANCE(xobj));
       _printf(" child num: %d, type_name: %s\n", count, type_name_child);
       g_assert(!strcmp(type_name_child, "EailIcon") || !strcmp(type_name_child, "EailLabel"));
     }

   // test AtkAction
   g_assert(ACTIONS_NUMBER == atk_action_get_n_actions(ATK_ACTION(obj)));
   eailu_test_action_activate(ATK_ACTION(obj), "click");
   g_assert(-1 == (eailu_get_action_number(ATK_ACTION(obj), "typo")));
   eailu_test_action_description_all(ATK_ACTION(obj));

   /* bubble object does not support focus */
   eailu_test_atk_focus(obj, FALSE);
}

static void
_on_done(void *data, Evas_Object *obj, void *event_info)
{
   elm_exit();
}

static void
_on_focus_in(void *data, Evas_Object *obj, void *event_info)
{
   AtkObject *aobj = atk_get_root();
   g_assert(aobj);

   eailu_traverse_children_for_role_call_cb(aobj, ATK_ROLE_FILLER, _do_test);

   _on_done(NULL, obj, NULL);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_bubble(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
