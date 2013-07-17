#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define EAIL_TYPE_FOR_MBE "EailMultibuttonentry"
#define EAIL_TEST_INPUT_TEXT "Input text"
#define EAIL_TEST_CHANGED_TEXT "custom text"
#define EAIL_TEST_FIRST_ITEM_NAME "first"
#define ACTIONS_COUNT 2
#define ACTION_SHRINK 0
#define ACTION_EXPAND 1
Evas_Object *mb_entry;
/* used to determine if test code has been successfully called
 * (sometimes may happen that ATK object to test in hierarchy cannot be found)*/
static gboolean eail_test_code_called = FALSE;

static void
_test_multibutton_entry_object(AtkObject *obj);

static void
_on_done(void *data, Evas_Object *obj, void *event_info)
{
   evas_object_del((Evas_Object *)data);
   elm_exit();
}

static void
on_focus_in(void *data, Evas_Object *obj, void *event_info)
{
   AtkObject *aobj = atk_get_root();
   g_assert(aobj);

   eailu_traverse_children_for_type_call_cb
       (aobj, EAIL_TYPE_FOR_MBE, _test_multibutton_entry_object);

   g_assert(eail_test_code_called);
   _on_done(NULL, obj, NULL);
}

static void
_test_multibutton_entry_object(AtkObject *obj)
{
   int child_amount = 0, i =0;
   AtkObject *nested_obj;
   gchar *text = NULL;

   printf("Testing multibutton_entry instance....\n");
   /* standard tests, types, child num etc.*/
   child_amount = atk_object_get_n_accessible_children(obj);
   g_assert(4 == child_amount);
   for (i = 0; i < child_amount; ++i)
     {
        nested_obj = atk_object_ref_accessible_child(obj, i);
        g_assert(nested_obj);
        eailu_is_object_with_role(nested_obj, ATK_ROLE_LABEL);
        eailu_is_object_with_type(nested_obj, "EailItem");

        g_object_unref(nested_obj);
     }
   /* checking if there is child with given name */
   nested_obj = eailu_find_child_with_name(obj, EAIL_TEST_FIRST_ITEM_NAME);
   g_assert(nested_obj);
   g_object_unref(nested_obj);
   nested_obj = NULL;

   /* checking text before...*/
   g_assert(ATK_IS_TEXT(obj));
   text = atk_text_get_text(ATK_TEXT(obj), 0, -1);
   g_assert(!g_strcmp0(text, EAIL_TEST_INPUT_TEXT));

   /* now checking if edit text works properly */
   g_assert(ATK_IS_EDITABLE_TEXT(obj));
   atk_editable_text_set_text_contents
       (ATK_EDITABLE_TEXT(obj), EAIL_TEST_CHANGED_TEXT);
   text = atk_text_get_text(ATK_TEXT(obj), 0, -1);
   g_assert(!g_strcmp0(text, EAIL_TEST_CHANGED_TEXT));

   /* AtkAction tests */

   /*
    * AtkAction tests
    */
   g_assert(ATK_IS_ACTION(obj));

   /* atk_action_get_n_actions test */
   g_assert(atk_action_get_n_actions(ATK_ACTION(obj)) == ACTIONS_COUNT);

   /* atk_action_set_description */
   const char *action_name;
   char *test_string;
   AtkStateSet *state_set;

   action_name = atk_action_get_name(ATK_ACTION(obj), ACTION_SHRINK);
   test_string = g_strconcat("test-", action_name, NULL);
   g_assert(atk_action_set_description(ATK_ACTION(obj), ACTION_SHRINK, test_string));
   g_assert_cmpstr(atk_action_get_description(ATK_ACTION(obj), ACTION_SHRINK), ==, test_string);
   g_free(test_string);

   action_name = atk_action_get_name(ATK_ACTION(obj), ACTION_EXPAND);
   test_string = g_strconcat("test-", action_name, NULL);
   g_assert(atk_action_set_description(ATK_ACTION(obj), ACTION_EXPAND, test_string));
   g_assert_cmpstr(atk_action_get_description(ATK_ACTION(obj), ACTION_EXPAND), ==, test_string);
   g_free(test_string);

   /* atk_action_do_action */
   g_assert(atk_action_do_action(ATK_ACTION(obj), ACTION_SHRINK));
   state_set = atk_object_ref_state_set(obj);
   g_assert(atk_state_set_contains_state(state_set, ATK_STATE_DEFAULT));
   g_object_unref(state_set);

   g_assert(atk_action_do_action(ATK_ACTION(obj), ACTION_EXPAND));
   state_set = atk_object_ref_state_set(obj);
   g_assert(atk_state_set_contains_state(state_set, ATK_STATE_EXPANDED));
   g_object_unref(state_set);

   /* test for non-existing action */
   g_assert((eailu_get_action_number(ATK_ACTION(obj), "typo")) == -1);
   printf("DONE. All multibutton_entry tests passed successfully \n");

   eail_test_code_called = TRUE;
}

static void
_initialize_and_show_multibutton_entry(Evas_Object *win)
{
   Evas_Object *scroller, *bg, *box;

   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   elm_win_autodel_set(win, EINA_TRUE);

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   box = elm_box_add(win);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, box);
   evas_object_show(box);

   scroller = elm_scroller_add(win);
   elm_scroller_bounce_set(scroller, EINA_FALSE, EINA_TRUE);
   elm_scroller_policy_set
       (scroller, ELM_SCROLLER_POLICY_OFF,ELM_SCROLLER_POLICY_AUTO);
   evas_object_show(scroller);

   mb_entry = elm_multibuttonentry_add(win);
   elm_object_part_text_set(mb_entry, "guide", "Tap to add recipient");
   elm_object_text_set(mb_entry, EAIL_TEST_INPUT_TEXT);
   evas_object_size_hint_weight_set(mb_entry, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(mb_entry, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_content_set(scroller, mb_entry);
   evas_object_show(mb_entry);

   elm_multibuttonentry_item_append
       (mb_entry, EAIL_TEST_FIRST_ITEM_NAME, NULL, NULL);
   elm_multibuttonentry_item_append(mb_entry, "second", NULL, NULL);
   elm_multibuttonentry_item_append(mb_entry, "third", NULL, NULL);
   elm_multibuttonentry_item_append(mb_entry, "fourth", NULL, NULL);

   elm_object_focus_set(mb_entry, EINA_TRUE);
   elm_box_pack_end(box, mb_entry);

   evas_object_resize(win, 130, 180);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, on_focus_in);
   _initialize_and_show_multibutton_entry(win);

   evas_object_show(win);

   elm_run();
   elm_shutdown();

   /* exit code */
   return 0;
}
ELM_MAIN()
