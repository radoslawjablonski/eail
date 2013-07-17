#include <Elementary.h>
#include <atk/atk.h>

//#define DEBUG 1

#include "eail_test_utils.h"

#define ACTIONS_NUMBER 1

#define CHECK_TEXT1 "checkbox1"
#define CHECK_TEXT2 "checkbox2"

static void
_print(void *data, Evas_Object *obj, void *event_info)
{
   _printf("\tcheck %s - %smarked\n", elm_object_part_text_get(obj, "default"),
           *((Eina_Bool*)data) ? "" : "un");
}

static void
_init_check(Evas_Object *win)
{
   Evas_Object *bg, *cb1, *cb2, *icon;
   Eina_Bool value1, value2;

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   cb1 = elm_check_add(win);
   elm_object_text_set(cb1, CHECK_TEXT1);
   elm_check_state_pointer_set(cb1, &value1);
   elm_check_state_set(cb1, EINA_TRUE);
   evas_object_smart_callback_add(cb1, "changed", _print, &value1);
   evas_object_move(cb1, 10, 10);
   evas_object_resize(cb1, 200, 30);
   evas_object_show(cb1);

   icon = elm_icon_add(win);
   elm_icon_order_lookup_set(icon, ELM_ICON_LOOKUP_THEME_FDO);
   elm_icon_standard_set(icon, "home");
   evas_object_show(icon);

   cb2 = elm_check_add(win);
   elm_object_text_set(cb2, CHECK_TEXT2);
   elm_check_state_pointer_set(cb2, &value2);
   elm_check_state_set(cb2, EINA_FALSE);
   elm_object_part_content_set(cb2, "icon", icon);
   evas_object_smart_callback_add(cb2, "changed", _print, &value2);
   evas_object_move(cb2, 10, 50);
   evas_object_resize(cb2, 200, 30);
   evas_object_show(cb2);
}

static void
_do_test(AtkObject *obj)
{
   const char *type_name = g_type_name(G_TYPE_FROM_INSTANCE(obj));
   const char *name = atk_object_get_name(ATK_OBJECT(obj));
   const char *type_name_child;
   AtkStateSet *state_set;
   AtkObject *xobj;

   _printf("type_name: %s\n", type_name ? type_name : "NULL");
   g_assert(name);
   _printf("name: %s\n", name);

   g_assert_cmpstr(type_name, ==, "EailCheck");

   g_assert(ATK_IS_OBJECT(obj));
   g_assert(ATK_IS_ACTION(obj));
   g_assert(ATK_IS_TEXT(obj));

   state_set = atk_object_ref_state_set(obj);

   if (!strcmp(name, CHECK_TEXT1))
     {
        _printf("check test: %s\n", CHECK_TEXT1);

        g_assert(atk_state_set_contains_state(state_set, ATK_STATE_CHECKED));

        // test children
        g_assert(0 == atk_object_get_n_accessible_children(obj));
     }
   else if (!strcmp(name, CHECK_TEXT2))
     {
       _printf("check test: %s\n", CHECK_TEXT2);

       g_assert(!atk_state_set_contains_state(state_set, ATK_STATE_CHECKED));

       // test children
       g_assert(1 == atk_object_get_n_accessible_children(obj));

       xobj = atk_object_ref_accessible_child(obj, 0);
       g_assert(NULL != xobj);
       type_name_child = g_type_name(G_TYPE_FROM_INSTANCE(xobj));
       _printf("type_name_child: %s\n", type_name_child);
       g_assert_cmpstr(type_name_child, ==, "EailIcon");
     }
   else
     {
       _printf("not find\n");
       g_assert(FALSE);
     }

   // test AtkAction
   g_assert(ACTIONS_NUMBER == atk_action_get_n_actions(ATK_ACTION(obj)));
   eailu_test_action_activate(ATK_ACTION(obj), "check");
   g_assert((eailu_get_action_number(ATK_ACTION(obj), "typo")) == -1);
   eailu_test_action_description_all(ATK_ACTION(obj));
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

   eailu_traverse_children_for_role_call_cb(aobj, ATK_ROLE_CHECK_BOX, _do_test);

   _on_done(NULL, obj, NULL);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_check(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
