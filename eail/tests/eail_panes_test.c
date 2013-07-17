#include <Elementary.h>
#include <atk/atk.h>

//#define DEBUG 1

#include "eail_test_utils.h"

static void
_do_test(AtkObject *obj)
{
   int child_count = 0;
   child_count = atk_object_get_n_accessible_children(obj);
   g_assert(child_count == 2);
   _printf("n_accessible_children - done\n");

   AtkStateSet *state_set = atk_object_ref_state_set(obj);
   gboolean v_contains = atk_state_set_contains_state(state_set,
                                                      ATK_STATE_VERTICAL);
   gboolean h_contains = atk_state_set_contains_state(state_set,
                                                      ATK_STATE_HORIZONTAL);
   g_object_unref(state_set);
   g_assert(v_contains || h_contains);
   _printf("ref_state_set - done\n");

   /*AtkValue iface tests*/
   g_assert(ATK_IS_VALUE(obj));
   GValue value = G_VALUE_INIT;
   double d_value;
   atk_value_get_maximum_value(ATK_VALUE(obj), &value);
   d_value = g_value_get_double(&value);
   g_assert(d_value == 1.0);
   atk_value_get_minimum_value(ATK_VALUE(obj), &value);
   d_value = g_value_get_double(&value);
   g_assert(d_value == 0.0);
   g_value_set_double(&value, 0.3);
   gboolean success = atk_value_set_current_value(ATK_VALUE(obj), &value);
   g_assert(success);
   atk_value_get_current_value(ATK_VALUE(obj), &value);
   g_assert(G_VALUE_HOLDS_DOUBLE(&value));
   d_value = g_value_get_double(&value);
   g_assert(d_value == 0.3);
   _printf("atk_value - done\n");

   eailu_test_atk_focus(obj, TRUE);
}

static void
_init_panes(Evas_Object *win)
{
   Evas_Object *bg, *panes, *panes_h, *bt;

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_focus_highlight_enabled_set(win, EINA_TRUE);
   evas_object_show(bg);

   panes = elm_panes_add(win);
   elm_win_resize_object_add(win, panes);
   evas_object_size_hint_weight_set(panes, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(panes);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "Left");
   evas_object_size_hint_weight_set(bt, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(bt);
   elm_object_part_content_set(panes, "left", bt);

   panes_h = elm_panes_add(win);
   elm_panes_horizontal_set(panes_h, EINA_TRUE);
   evas_object_size_hint_weight_set(panes_h, EVAS_HINT_EXPAND,
                                    EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes_h, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(panes_h);
   elm_object_part_content_set(panes, "right", panes_h);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "Up");
   evas_object_size_hint_weight_set(bt, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(bt);
   elm_object_part_content_set(panes_h, "left", bt);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "Down");
   evas_object_size_hint_weight_set(bt, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(bt);
   elm_object_part_content_set(panes_h, "right", bt);
}

static void
_on_done(void *data, Evas_Object *obj, void *event_info)
{
   _printf("EailPanes Test PASSED\n");
   elm_exit();
}

static void
_on_focus_in(void *data, Evas_Object *obj, void *event_info)
{
   AtkObject *aobj = atk_get_root();
   g_assert(aobj);

   eailu_traverse_children_for_type_call_cb(aobj, "EailPanes", _do_test);

   _on_done(NULL, obj, NULL);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_panes(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
