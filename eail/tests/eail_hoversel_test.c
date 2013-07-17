#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define G_NAME "EailHoverselItem"
#define ACTIONS_COUNT 2
#define ACTION_SHRINK 0
#define ACTION_EXPAND 1
#define ITEMS_COUNT 3
#define ITEM1_NAME "Print items"
#define ITEM2_NAME "Option 2"
#define ITEM3_NAME "Clear all items"

static void
_init_hoversel(Evas_Object *win)
{
   Evas_Object *bg, *rect, *hoversel;

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   rect = evas_object_rectangle_add(evas_object_evas_get(win));
   evas_object_color_set(rect, 255, 0, 0, 255);
   evas_object_show(rect);

   hoversel = elm_hoversel_add(win);
   elm_hoversel_hover_parent_set(hoversel, win);
   elm_hoversel_horizontal_set(hoversel, EINA_FALSE);
   elm_object_text_set(hoversel, "Hoversel");
   elm_object_part_content_set(hoversel, "icon", rect);

   elm_hoversel_item_add(hoversel, ITEM1_NAME, NULL, ELM_ICON_NONE,
                         NULL, NULL);
   elm_hoversel_item_add(hoversel, ITEM2_NAME, "home", ELM_ICON_STANDARD, NULL,
                         NULL);
   elm_hoversel_item_add(hoversel, ITEM3_NAME, "close",
                         ELM_ICON_STANDARD, NULL, NULL);

   evas_object_resize(hoversel, 180, 30);
   evas_object_resize(win, 200, 180);
   evas_object_move(hoversel, 10, 10);
   evas_object_show(hoversel);
   elm_hoversel_hover_begin(hoversel);
}

static void
_do_test(AtkObject *obj)
{
   const char *type_name = g_type_name(G_TYPE_FROM_INSTANCE(obj));
   AtkObject *child;
   char *item_names[] = {ITEM1_NAME, ITEM2_NAME, ITEM3_NAME};
   int n;

   g_assert(ATK_IS_OBJECT(obj));
   g_assert_cmpstr(type_name, ==, "EailHoversel");

   g_assert(atk_object_get_n_accessible_children(obj) == ITEMS_COUNT);

   n = ITEMS_COUNT;
   while (n--)
     {
        child = eailu_find_child_with_name(obj, item_names[n]);
        g_assert(child);
        g_object_unref(child);
     }

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
   atk_state_set_contains_state(state_set, ATK_STATE_DEFAULT);
   g_object_unref(state_set);

   g_assert(atk_action_do_action(ATK_ACTION(obj), ACTION_EXPAND));
   state_set = atk_object_ref_state_set(obj);
   g_assert(atk_state_set_contains_state(state_set, ATK_STATE_EXPANDED));
   g_assert(atk_state_set_contains_state(state_set, ATK_STATE_VERTICAL));
   g_object_unref(state_set);

   /* test for non-existing action */
   g_assert((eailu_get_action_number(ATK_ACTION(obj), "typo")) == -1);
}

static void
_on_done(void *data, Evas_Object *obj, void *event_info)
{
  evas_object_del((Evas_Object *)data);
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
elm_main(int argc, char **argv)
{
   Evas_Object *win = NULL;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_hoversel(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
