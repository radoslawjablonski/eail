#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define EAIL_TYPE_FOR_DISKSELECTOR "EailDiskSelector"

static void _on_focus_in(void *data, Evas_Object *obj, void *event_info);
static void _on_done();

/* used to determine if test code has been successfully called
 * (sometimes may happen that ATK object to test in hierarchy cannot be found)*/
static gboolean eail_test_code_called = FALSE;

static void
_on_done()
{
   elm_exit();
}

static void
_test_disk_selector_actions(AtkObject *obj)
{
   g_assert(ATK_IS_ACTION(obj));
   eailu_test_action_activate(ATK_ACTION(obj), "scroll_left");
   eailu_test_action_activate(ATK_ACTION(obj), "scroll_right");
   eailu_test_action_activate(ATK_ACTION(obj), "scroll_up");
   eailu_test_action_activate(ATK_ACTION(obj), "scroll_down");
}

static void
_test_disk_selector_object(AtkObject *obj)
{
   int child_count = 0, i = 0;

   printf("Testing Diskselector instance....\n");
   child_count = atk_object_get_n_accessible_children(obj);
   g_assert(7 == child_count);

   g_assert(ATK_IS_SELECTION(obj));

   /* first element should be selected as default */
   g_assert(atk_selection_is_child_selected(ATK_SELECTION(obj), 0));

   /* only one objec can be selected at time, if '0' is selected then
    * rest of them should be unselected*/
   for (i = 1; i < 7; ++i)
      g_assert(FALSE == atk_selection_is_child_selected(ATK_SELECTION(obj), i));

   eailu_test_atk_focus(obj, TRUE);

   _test_disk_selector_actions(obj);
   printf("DONE. All Diskselector tests passed successfully \n");
   eail_test_code_called = TRUE;
}

static void
_on_focus_in(void *data, Evas_Object *obj, void *event_info)
{
   AtkObject *aobj = atk_get_root();
   g_assert(aobj);

   eailu_traverse_children_for_type_call_cb
               (aobj, EAIL_TYPE_FOR_DISKSELECTOR, _test_disk_selector_object);

   g_assert(eail_test_code_called);
   _on_done();
}

static void
_initialize_and_show_diskselector(Evas_Object *win)
{
   Evas_Object  *bx, *ds;
   unsigned int i;
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

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(bx, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);

   /* default */
   ds = elm_diskselector_add(win);
   evas_object_size_hint_weight_set(ds, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(ds, EVAS_HINT_FILL, EVAS_HINT_FILL);

   /* TODO: RJ, uncomment box addding when box-listing will be fixed */
   elm_box_pack_end(bx, ds);

   for (i = 0; i < sizeof(lbl) / sizeof(lbl[0]); i++)
     elm_diskselector_item_append(ds, lbl[i], NULL, NULL, NULL);
   evas_object_show(ds);
   evas_object_resize(win, 800, 200);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _initialize_and_show_diskselector(win);

   evas_object_show(win);

   elm_run();
   elm_shutdown();

   /* exit code */
   return 0;
}
ELM_MAIN()
