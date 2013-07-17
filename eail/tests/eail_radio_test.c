//Compile with:
//gcc -g radio_example_01.c -o radio_example_01 `pkg-config --cflags --libs elementary atk`

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define ACTIONS_COUNT 1
#define DEFAULT_ACTION 0
#define RADIO1_SEL 1
#define RADIO2_SEL 2
#define RADIO3_SEL 3
#define RADIO1 "Radio 1"
#define RADIO2 "Radio 2"
#define RADIO3 "Radio 3"

int val = 1;

static void _init_radio(Evas_Object *win);
static void _do_test(AtkObject *obj);
static void _on_done(void *data, Evas_Object *obj, void *event_info);
static void _on_focus_in(void *data, Evas_Object *obj, void *event_info);

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_radio(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()

static void
_init_radio(Evas_Object *win)
{
   Evas_Object *bg, *radio, *group, *bx, *ic;

   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   elm_win_autodel_set(win, EINA_TRUE);

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   bx = elm_box_add(win);
   elm_box_horizontal_set(bx, EINA_TRUE);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);

   group = radio = elm_radio_add(win);
   elm_object_text_set(radio, RADIO1);
   elm_radio_state_value_set(radio, 1);
   elm_radio_value_pointer_set(radio, &val);
   ic = elm_icon_add(win);
   elm_icon_standard_set(ic, "home");
   elm_object_part_content_set(radio, "icon", ic);
   elm_box_pack_end(bx, radio);
   evas_object_size_hint_weight_set(radio, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(radio, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(radio);

   radio = elm_radio_add(win);
   elm_object_text_set(radio, RADIO2);
   elm_radio_state_value_set(radio, 2);
   elm_radio_value_pointer_set(radio, &val);
   elm_radio_group_add(radio, group);
   ic = elm_icon_add(win);
   elm_icon_standard_set(ic, "file");
   elm_object_part_content_set(radio, "icon", ic);
   elm_box_pack_end(bx, radio);
   evas_object_size_hint_weight_set(radio, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(radio, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(radio);

   radio = elm_radio_add(win);
   elm_object_text_set(radio, RADIO3);
   elm_radio_state_value_set(radio, 3);
   elm_radio_value_pointer_set(radio, &val);
   elm_radio_group_add(radio, group);
   elm_box_pack_end(bx, radio);
   evas_object_size_hint_weight_set(radio, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(radio, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(radio);

   elm_radio_value_set(radio, RADIO2_SEL);
}

static void
_do_test(AtkObject *obj)
{
   const char *type_name = g_type_name(G_TYPE_FROM_INSTANCE(obj));
   AtkStateSet *state_set = atk_object_ref_state_set(obj);

   g_assert_cmpstr(type_name, ==, "EailRadioButton");

   /*
    * AtkAction tests
    */
   g_assert(ATK_IS_ACTION(obj));

   /* atk_action_get_n_actions test */
   g_assert(atk_action_get_n_actions(ATK_ACTION(obj)) == ACTIONS_COUNT);

   /* atk_action_set_description */
   const char *action_name = atk_action_get_name(ATK_ACTION(obj), DEFAULT_ACTION);
   char *test_string = g_strconcat("test-", action_name, NULL);
   g_assert(atk_action_set_description(ATK_ACTION(obj), DEFAULT_ACTION, test_string));

   /* atk_action_get_description */
   g_assert_cmpstr(atk_action_get_description(ATK_ACTION(obj), DEFAULT_ACTION), ==, test_string);
   g_free(test_string);

   /* atk_action_do_action */
   g_assert(atk_action_do_action(ATK_ACTION(obj), DEFAULT_ACTION));
   state_set = atk_object_ref_state_set(obj);
   g_assert(atk_state_set_contains_state(state_set, ATK_STATE_CHECKED));
   g_object_unref(state_set);

   /* test for non-existing action */
   g_assert((eailu_get_action_number(ATK_ACTION(obj), "typo")) == -1);
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

   eailu_traverse_children_for_role_call_cb(aobj, ATK_ROLE_RADIO_BUTTON, _do_test);

   _on_done(data, obj, event_info);
}

