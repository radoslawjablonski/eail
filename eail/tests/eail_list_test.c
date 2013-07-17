
#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define EAIL_TYPE_FOR_LIST "EailList"
#define EAIL_TYPE_FOR_LIST_ITEM "EailItem"

/* used to determine if test code has been successfully called
 * (sometimes may happen that ATK object to test in hierarchy cannot be found)*/
static gboolean eail_test_code_called = FALSE;
static Evas_Object *glob_elm_list = NULL;

static void
on_done(void *data, Evas_Object *obj, void *event_info)
{
   /* quit the mainloop (elm_run) */
   elm_exit();
}

void
_test_list_item_interfaces(AtkObject *item_object)
{
   g_assert(ATK_IS_OBJECT(item_object));
   g_assert(eailu_is_object_with_type(item_object, EAIL_TYPE_FOR_LIST_ITEM));

   /**
    * for single list items focusing does NOT work
    */
   eailu_test_atk_focus(item_object, FALSE);

   g_assert(ATK_IS_ACTION(item_object));
}

void
_test_item_selection_single(AtkObject *parent, AtkObject *child, int index)
{
   g_assert(ATK_IS_SELECTION(parent));
   /* in single selection mode it should return FALSE */
   g_assert(!atk_selection_select_all_selection(ATK_SELECTION(parent)));
   g_assert
      (FALSE == atk_selection_is_child_selected(ATK_SELECTION(parent), index));

   /* NOTE: calling click action to select child*/
  eailu_test_action_activate(ATK_ACTION(child), "click");
  g_assert(atk_selection_is_child_selected(ATK_SELECTION(parent), index));
  g_assert(1 == atk_selection_get_selection_count(ATK_SELECTION(parent)));

  /* now removing selection using atk_selection interface */
  g_assert(atk_selection_remove_selection(ATK_SELECTION(parent),index));
  g_assert
     (FALSE == atk_selection_is_child_selected(ATK_SELECTION(parent), index));
  g_assert(0 == atk_selection_get_selection_count(ATK_SELECTION(parent)));
}

void
_test_multiselection_list(AtkObject *atk_list)
{
   elm_list_multi_select_set(glob_elm_list, EINA_TRUE);
   g_assert(atk_selection_select_all_selection(ATK_SELECTION(atk_list)));

   /* all children should be now selected */
   g_assert(7 == atk_selection_get_selection_count(ATK_SELECTION(atk_list)));

   /* now clearing selection */
   g_assert(atk_selection_clear_selection(ATK_SELECTION(atk_list)));
   g_assert(0 == atk_selection_get_selection_count(ATK_SELECTION(atk_list)));
}

static void
_test_scrolling_actions(AtkObject *aobj)
{
   int n_actions = 0;
   const gchar *test_desc = "test description for scroll up";
   const gchar *result_desc;

   g_assert(ATK_IS_ACTION(aobj));
   n_actions = atk_action_get_n_actions(ATK_ACTION(aobj));
   g_assert(4 == n_actions);

   /* scroll up*/
   eailu_test_action_activate(ATK_ACTION(aobj), "scroll_up");

   /* scroll_down */
   eailu_test_action_activate(ATK_ACTION(aobj), "scroll_down");

   /* scroll_left */
   eailu_test_action_activate(ATK_ACTION(aobj), "scroll_left");

   /* scroll_right*/
   eailu_test_action_activate(ATK_ACTION(aobj), "scroll_right");

   /* description test */
   g_assert(!atk_action_get_description(ATK_ACTION(aobj), 0));
   g_assert(atk_action_set_description(ATK_ACTION(aobj), 0, test_desc));

   result_desc = atk_action_get_description(ATK_ACTION(aobj), 0);
   g_assert(0 == g_strcmp0(result_desc, test_desc));
}

void
_test_list_atk_obj(AtkObject *aobj)
{
   int child_amount = 0, i = 0;

   printf("TESTING EAIL_List implementation....\n");

   child_amount = atk_object_get_n_accessible_children(aobj);
   g_assert(7 == child_amount);

   _test_scrolling_actions(aobj);

   /* For list widget focusing work */
   eailu_test_atk_focus(aobj, TRUE);

   /* checking list items implementation */
     for (i = 0; i < child_amount; i++)
      {
         AtkObject *child = atk_object_ref_accessible_child(aobj, i);
         _test_list_item_interfaces(child);
         _test_item_selection_single(aobj, child, i);
         /* each list item has inside 2 text entry objects */
         printf("Child amount %d\n", atk_object_get_n_accessible_children(child));
         g_assert(2 == atk_object_get_n_accessible_children(child));



         g_object_unref(child);
       }

   /* now quick check if multiselection on list works properly*/
   _test_multiselection_list(aobj);
   eail_test_code_called = TRUE;
   printf("DONE. All test passed successfully \n");
}

static void on_focus_in(void *data, Evas_Object *obj, void *event_info)
{
   AtkObject *aobj = atk_get_root();
   g_assert(aobj);

   eailu_traverse_children_for_type_call_cb
                                 (aobj, EAIL_TYPE_FOR_LIST, _test_list_atk_obj);

   g_assert(eail_test_code_called);
   /* force exit on end of test */
   on_done(NULL, NULL, NULL);
}

void
initialize_list(Evas_Object *win)
{
   Evas_Object *box, *entry;
   int i = 0;

   static const char *lbl[] =
      {
         "Sunday",
         "Monday",
         "Tuesday",
         "Wednesday",
         "Thursday",
         "Friday",
         "Saturday"
      };

   box = elm_box_add(win);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, box);
   evas_object_show(box);

   glob_elm_list = elm_list_add(win);
   evas_object_size_hint_weight_set(glob_elm_list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(glob_elm_list, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_tree_focus_allow_set(win, EINA_TRUE);

   entry = elm_entry_add(win);
   elm_entry_single_line_set(entry, EINA_TRUE);
   evas_object_size_hint_weight_set(entry, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(entry, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(entry);

   for (i = 0; i < sizeof(lbl) / sizeof(lbl[0]); i++)
     elm_list_item_append(glob_elm_list, lbl[i], entry, entry, NULL, NULL);

   elm_box_pack_end(box, glob_elm_list);

   elm_list_go(glob_elm_list);
   evas_object_show(glob_elm_list);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(on_done, on_focus_in);
   initialize_list(win);

   /* and show the window */
   evas_object_show(win);

   elm_run();
   elm_shutdown();

   /* exit code */
   return 0;
}
ELM_MAIN()
