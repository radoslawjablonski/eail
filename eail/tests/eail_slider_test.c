#include <Elementary.h>
#include <atk/atk.h>

//#define DEBUG 1

#include "eail_test_utils.h"

const char * const slider_name = "slider-name";
const double slider_min = -50;
const double slider_max = 50;
const double slider_set = 20;

static void
_changed_cb(void *data, Evas_Object *obj, void *event_info)
{
   _printf("value changed\n");
}

static void
_init_slider(Evas_Object *win)
{
   Evas_Object *bg, *bx, *sl;

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);

   sl = elm_slider_add(win);
   elm_object_text_set(sl, slider_name);
   elm_slider_min_max_set(sl, slider_min, slider_max);
   elm_slider_value_set(sl, slider_set);
   elm_slider_unit_format_set(sl, "%2.1f");
   elm_slider_span_size_set(sl, 120);
   evas_object_size_hint_align_set(sl, EVAS_HINT_FILL, 0.5);
   evas_object_size_hint_weight_set(sl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_smart_callback_add(sl, "changed", _changed_cb, NULL);
   elm_box_pack_end(bx, sl);
   evas_object_show(sl);
}

static void
_do_test(AtkObject *obj)
{
   const char *name = atk_object_get_name(obj);
   const char *type_name = g_type_name(G_TYPE_FROM_INSTANCE(obj));
   const char *role_name = atk_role_get_name(atk_object_get_role(obj));
   double minimum, maximum, current, minimum_increment;
   const double val_test_set = 12.3;

   _printf("name: %s\n", name ? name : "NULL");
   _printf("role_name: %s\n", role_name ? role_name : "NULL");
   _printf("type_name: %s\n", type_name ? type_name : "NULL");

   g_assert_cmpstr(type_name, ==, "EailSlider");
   g_assert_cmpstr(name, ==, slider_name);

   GValue value = G_VALUE_INIT;
   g_value_init(&value, G_TYPE_DOUBLE);

   atk_value_get_minimum_value(ATK_VALUE(obj), &value);
   minimum = g_value_get_double(&value);
   _printf("atk_value_get_minimum_value: %0.2f\n", minimum);
   g_assert((float)slider_min == (float)minimum);

   atk_value_get_maximum_value(ATK_VALUE(obj), &value);
   maximum = g_value_get_double(&value);
   _printf("atk_value_get_maximum_value: %0.2f\n", maximum);
   g_assert((float)slider_max == (float)maximum);

   atk_value_get_current_value(ATK_VALUE(obj), &value);
   current = g_value_get_double(&value);
   _printf("atk_value_get_current_value: %0.2f\n", current);
   g_assert((float)slider_set == (float)current);

   atk_value_get_minimum_increment(ATK_VALUE(obj), &value);
   minimum_increment = g_value_get_double(&value);
   _printf("atk_value_get_minimum_increment: %0.2f\n", minimum_increment);
   g_assert((float)G_MINDOUBLE == (float)minimum_increment);

   g_value_set_double(&value, val_test_set);
   g_assert(atk_value_set_current_value(ATK_VALUE(obj), &value));
   _printf("atk_value_set_current_value: %0.2f\n", val_test_set);

   atk_value_get_current_value(ATK_VALUE(obj), &value);
   current = g_value_get_double(&value);
   _printf("atk_value_get_current_value: %0.2f\n", current);

   g_assert((float)val_test_set == (float)current);

   g_value_set_double(&value, minimum - 1);
   g_assert(!atk_value_set_current_value(ATK_VALUE(obj), &value));

   g_value_set_double(&value, maximum + 1);
   g_assert(!atk_value_set_current_value(ATK_VALUE(obj), &value));
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

   eailu_traverse_children_for_role_call_cb(aobj, ATK_ROLE_SLIDER, _do_test);

   _on_done(NULL, obj, NULL);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_slider(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
