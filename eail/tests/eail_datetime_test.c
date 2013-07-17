#include <Elementary.h>
#include <atk/atk.h>

//#define DEBUG 1

#include "eail_test_utils.h"

static time_t time_min, time_max, time_current;

static void
_changed_cb(void *data, Evas_Object *obj, void *event_info)
{
   _printf("action: changed\n");
}

static void
_printf_datetime_int(time_t atime, const char *str)
{
   struct tm tmp;

   localtime_r(&atime, &tmp);

   _printf("[%04d-%02d-%02d] [%02d:%02d:%02d] (%s)\n",
           tmp.tm_year + 1900, tmp.tm_mon + 1, tmp.tm_mday,
           tmp.tm_hour, tmp.tm_min, tmp.tm_sec, str);
}

static void
_init_datetime(Evas_Object *win)
{
   Evas_Object *bg, *bx, *dt;
   struct tm tmp;

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx);
   elm_box_horizontal_set(bx, EINA_FALSE);
   evas_object_show(bx);

   dt = elm_datetime_add(bx);
   evas_object_size_hint_weight_set(dt, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(dt, EVAS_HINT_FILL, 0.5);
   evas_object_smart_callback_add(dt, "changed", _changed_cb, NULL);

   time_min = time(NULL) - 3600;
   localtime_r(&time_min, &tmp);
   elm_datetime_value_min_set(dt, &tmp);
   _printf_datetime_int(time_min, "time_min");

   time_max = time(NULL) + 3600;
   localtime_r(&time_max, &tmp);
   elm_datetime_value_max_set(dt, &tmp);
   _printf_datetime_int(time_max, "time_max");

   time_current = time(NULL);
   localtime_r(&time_current, &tmp);
   elm_datetime_value_set(dt, &tmp);
   _printf_datetime_int(time_current, "time_current");

   elm_box_pack_end(bx, dt);
   evas_object_show(dt);
}

static void
_do_test(AtkObject *obj)
{
   const char *type_name = g_type_name(G_TYPE_FROM_INSTANCE(obj));
   time_t time_set;

   _printf("atk_object_get_type_name: %s\n", type_name ? type_name : "NULL");

   g_assert_cmpstr(type_name, ==, "EailDatetime");

   GValue value = G_VALUE_INIT;

   atk_value_get_minimum_value(ATK_VALUE(obj), &value);
   _printf_datetime_int(g_value_get_int(&value), "atk_value_get_minimum_value()");
   g_assert(g_value_get_int(&value) == time_min);

   atk_value_get_maximum_value(ATK_VALUE(obj), &value);
   _printf_datetime_int(g_value_get_int(&value), "atk_value_get_maximum_value()");
   g_assert(g_value_get_int(&value) == time_max);

   atk_value_get_current_value(ATK_VALUE(obj), &value);
   _printf_datetime_int(g_value_get_int(&value), "atk_value_get_current_value()");
   g_assert(g_value_get_int(&value) == time_current);

   atk_value_get_minimum_increment(ATK_VALUE(obj), &value);
   _printf_datetime_int(g_value_get_int(&value), "atk_value_get_minimum_increment()");
   g_assert(g_value_get_int(&value) == 1);

   time_set = time_current + 60;

   g_value_set_int(&value, time_set);
   atk_value_set_current_value(ATK_VALUE(obj), &value);
   _printf_datetime_int(g_value_get_int(&value), "atk_value_set_current_value(time_current + 60sec)");

   atk_value_get_current_value(ATK_VALUE(obj), &value);
   _printf_datetime_int(g_value_get_int(&value), "atk_value_get_current_value()");
   g_assert(g_value_get_int(&value) == time_set);
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

   eailu_traverse_children_for_role_call_cb(aobj, ATK_ROLE_DATE_EDITOR, _do_test);

   _on_done(NULL, obj, NULL);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_datetime(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
