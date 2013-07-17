#include <Elementary.h>

#include "eail_test_utils.h"

#define CHILDREN_COUNT 9

Elm_Prefs_Data *prefs_data;

static void
_do_test(AtkObject *obj)
{
   AtkObject *child;
   const char *type_name = g_type_name(G_TYPE_FROM_INSTANCE(obj));
   int i;

   g_assert_cmpstr(type_name, ==, "EailPrefs");

   g_assert(atk_object_get_n_accessible_children(obj) == CHILDREN_COUNT);

   for (i = 0; i < atk_object_get_n_accessible_children(obj); ++i)
     {
        child = atk_object_ref_accessible_child(obj, i);
        g_assert(child);
        g_object_unref(child);
     }
}

static void
_init_prefs(Evas_Object *win)
{
   Evas_Object *bg, *prefs;

   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   elm_win_autodel_set(win, EINA_TRUE);

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   prefs = elm_prefs_add(win);
   evas_object_size_hint_weight_set(prefs, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, prefs);
   evas_object_show(prefs);
   elm_prefs_autosave_set(prefs, EINA_TRUE);
   prefs_data = elm_prefs_data_new("./data/prefs_example_02.cfg", NULL,
                                   EET_FILE_MODE_READ_WRITE);
   elm_prefs_file_set(prefs, "./data/prefs_example_02.epb", NULL);
   elm_prefs_data_set(prefs, prefs_data);

   evas_object_resize(win, 320, 320);
}

static void
_on_done(void *data, Evas_Object *obj, void *event_info)
{
   evas_object_del((Evas_Object*)data);
   elm_exit();
}

static void
_on_focus_in(void *data, Evas_Object *obj, void *event_info)
{
   AtkObject *aobj = atk_get_root();
   g_assert(aobj);

   eailu_traverse_children_for_role_call_cb(aobj, ATK_ROLE_DIALOG, _do_test);

   _on_done(NULL, obj, NULL);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_prefs(win);
   evas_object_show(win);
   elm_run();
   elm_prefs_data_unref(prefs_data);
   elm_shutdown();

   return 0;
}

ELM_MAIN()
