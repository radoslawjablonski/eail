#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define ACTIONS_COUNT 1

static void _init_hover(Evas_Object *win);
static void _do_test(AtkObject *obj);
static void _on_focus_in(void *data,
                         Evas_Object *obj,
                         void *event_info);
static void _show_hover(void *data, Evas_Object *obj, void *event_info);
static void _hide_hover(void *data, Evas_Object *obj, void *event_info);

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win = NULL;

   win = eailu_create_test_window_with_glib_init(NULL, _on_focus_in);
   if (!win) return 1;

   _init_hover(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()

static void
_hide_hover(void *data, Evas_Object *obj, void *event_info)
{
   evas_object_hide(data);
}

static void
_show_hover(void *data, Evas_Object *obj, void *event_info)
{
   evas_object_show(data);
}

void
_init_hover(Evas_Object *win)
{
   Evas_Object *bt, *bt2, *rect, *rect2, *rect3, *hover;

   rect = evas_object_rectangle_add(evas_object_evas_get(win));
   evas_object_size_hint_min_set(rect, 25, 25);
   evas_object_color_set(rect, 255, 0, 0, 255);
   evas_object_show(rect);

   rect2 = evas_object_rectangle_add(evas_object_evas_get(win));
   evas_object_size_hint_min_set(rect2, 25, 25);
   evas_object_color_set(rect2, 0, 255, 0, 255);
   evas_object_show(rect2);

   rect3 = evas_object_rectangle_add(evas_object_evas_get(win));
   evas_object_size_hint_min_set(rect3, 25, 25);
   evas_object_color_set(rect3, 0, 0, 255, 255);
   evas_object_show(rect3);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "Show hover");
   evas_object_move(bt, 60, 90);
   evas_object_resize(bt, 80, 20);
   evas_object_show(bt);

   bt2 = elm_button_add(win);
   elm_object_text_set(bt2, "Hide hover");
   evas_object_show(bt2);

   hover = elm_hover_add(win);
   elm_hover_parent_set(hover, win);
   elm_hover_target_set(hover, bt);
   elm_object_style_set(hover, "popout");
   elm_object_part_content_set(hover, "left", rect);
   elm_object_part_content_set(hover, "top", rect2);
   elm_object_part_content_set(hover, "right", rect3);
   elm_object_part_content_set(hover, "middle", bt2);
   evas_object_smart_callback_add(bt, "clicked", _show_hover, hover);
   evas_object_smart_callback_add(bt2, "clicked", _hide_hover, hover);
   evas_object_show(hover);

   evas_object_resize(win, 200, 200);
   evas_object_show(win);
}

void
_do_test(AtkObject *obj)
{
   const char *type_name = g_type_name(G_TYPE_FROM_INSTANCE(obj));
   AtkObject *parent = atk_object_get_parent(obj);
   const char *parent_name = g_type_name(G_TYPE_FROM_INSTANCE(parent));

   g_assert(ATK_IS_OBJECT(obj));
   g_assert_cmpstr(type_name, ==, "EailHover");
   g_assert_cmpstr(parent_name, ==, "EailWindow");

   /*
    * AtkAction tests
    */
   g_assert(ATK_IS_ACTION(obj));

   /* atk_action_get_n_actions test */
   g_assert(atk_action_get_n_actions(ATK_ACTION(obj)) == ACTIONS_COUNT);

   /* test existing actions */
   eailu_test_action_activate(ATK_ACTION(obj), "clicked");

   /* test for non-existing action */
   g_assert((eailu_get_action_number(ATK_ACTION(obj), "typo")) == -1);

   /* test set/get action description */
   eailu_test_action_description_all(ATK_ACTION(obj));
}

void
_on_focus_in(void *data,
             Evas_Object *obj,
             void *event_info)
{
   AtkObject *aobj = atk_get_root();

   g_assert(aobj);
   eailu_traverse_children_for_role_call_cb(aobj, ATK_ROLE_GLASS_PANE, _do_test);

   elm_exit();
}
