#include <Elementary.h>
#include <atk/atk.h>

//#define DEBUG 1

#include "eail_test_utils.h"

static void
_init_table(Evas_Object *win)
{
   Evas_Object *bg, *label, *table;

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   table = elm_table_add(win);
   elm_win_resize_object_add(win, table);
   evas_object_show(table);
   elm_table_padding_set(table, 5, 5);
   elm_table_homogeneous_set(table, EINA_TRUE);

   label = elm_label_add(win);
   elm_object_text_set(label, "label 0");
   evas_object_show(label);
   elm_table_pack(table, label, 0, 0, 1, 1);

   label = elm_label_add(win);
   elm_object_text_set(label, "label 1");
   evas_object_show(label);
   elm_table_pack(table, label, 1, 0, 1, 1);

   label = elm_label_add(win);
   elm_object_text_set(label, "label 2");
   evas_object_show(label);
   elm_table_pack(table, label, 0, 1, 1, 1);

   label = elm_label_add(win);
   elm_object_text_set(label, "label 3");
   evas_object_show(label);
   elm_table_pack(table, label, 1, 1, 1, 1);
}

static void
_do_test(AtkObject *obj)
{
   const char *type_name = g_type_name(G_TYPE_FROM_INSTANCE(obj));
   const char *type_name_child;
   AtkObject *xobj;
   int child;

   _printf("type_name: %s\n", type_name);

   g_assert_cmpstr(type_name, ==, "EailTable");

   // test children
   child = atk_object_get_n_accessible_children(obj);
   g_assert(4 == child);

   for (int count = 0; child > count; count++)
     {
       xobj = atk_object_ref_accessible_child(obj, count);
       g_assert(xobj);
       type_name_child = g_type_name(G_TYPE_FROM_INSTANCE(xobj));
       _printf(" child num: %d, type_name: %s\n", count, type_name_child);
       g_assert(!strcmp(type_name_child, "EailLabel"));
     }
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
   _init_table(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
