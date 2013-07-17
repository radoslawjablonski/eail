#include <Elementary.h>
#include <atk/atk.h>
#include "eail_test_utils.h"

#define CHILDREN_COUNT 1

static void
_do_test(AtkObject *obj)
{
   AtkObject *child;
   const char *child_name;
   const char *type_name = g_type_name(G_TYPE_FROM_INSTANCE(obj));

   g_assert_cmpstr(type_name, ==, "EailInwin");

   g_assert(atk_object_get_n_accessible_children(obj) == CHILDREN_COUNT);

   child = atk_object_ref_accessible_child(obj, 0);
   g_assert(child);
   child_name = g_type_name(G_TYPE_FROM_INSTANCE(child));
   g_assert_cmpstr(child_name, ==, "EailButton");
   g_object_unref(child);

}

static void
_init_inwin(Evas_Object *win)
{
   Evas_Object *o, *bg, *parent, *inwin;

   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   elm_win_autodel_set(win, EINA_TRUE);

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   parent = win;
   inwin = elm_win_inwin_add(parent);
   elm_object_style_set(inwin, "default");
   evas_object_show(inwin);
   elm_win_inwin_activate(inwin);

   o = elm_button_add(parent);
   elm_object_text_set(o, "Test");
   evas_object_show(o);

   elm_win_inwin_content_set(inwin, o);
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

   eailu_traverse_children_for_role_call_cb(aobj, ATK_ROLE_GLASS_PANE, _do_test);

   _on_done(data, obj, event_info);
}

EAPI_MAIN int
elm_main(int argc, char *argv[])
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_inwin(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
