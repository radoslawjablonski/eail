#include <Elementary.h>
#include <atk/atk.h>

//#define DEBUG 1

#include "eail_test_utils.h"

static void
_init_route(Evas_Object *win)
{
   Evas_Object *bg;

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   elm_route_add(win);
}

static void
_do_test(AtkObject *obj)
{
   const char *type_name = g_type_name(G_TYPE_FROM_INSTANCE(obj));
   const char *role_name = atk_role_get_name(atk_object_get_role(obj));

   _printf("atk_object_get_role: %s\n", role_name ? role_name : "NULL");
   _printf("atk_object_get_type_name: %s\n", type_name ? type_name : "NULL");

   g_assert_cmpstr(type_name, ==, "EailRoute");

   GValue value = G_VALUE_INIT;
   atk_value_get_current_value(ATK_VALUE(obj), &value);
   _printf("atk_value_get_current_value:\n%s\n", g_value_get_string(&value));

   atk_value_get_maximum_value(ATK_VALUE(obj), &value);
   _printf("atk_value_get_maximum_value:\n%s\n", g_value_get_string(&value));

   atk_value_get_minimum_value(ATK_VALUE(obj), &value);
   _printf("atk_value_get_minimum_value:\n%s\n", g_value_get_string(&value));

   atk_value_get_minimum_increment(ATK_VALUE(obj), &value);
   _printf("atk_value_get_minimum_increment:\n%s\n", g_value_get_string(&value));

   /* NOTE: route widget does not support focus */
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

   eailu_traverse_children_for_role_call_cb(aobj, ATK_ROLE_IMAGE_MAP, _do_test);

   _on_done(NULL, obj, NULL);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_route(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
