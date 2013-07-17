#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define EAIL_TYPE_FOR_SEGMENTCONTROL "EailSegmentControl"
#define EAIL_TEST_SELECTED_INDEX 3

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
_test_segment_control_object(AtkObject *obj)
{
   int child_count = 0, i = 0;

   printf("Testing SegmentControl instance....\n");
   child_count = atk_object_get_n_accessible_children(obj);
   g_assert(7 == child_count);

   /* testing child refs*/
   for (i = 0; i < 7; ++i)
     {
        AtkObject *child = atk_object_ref_accessible_child(obj, i);
        g_assert(child);

        g_object_unref(child);
     }

   g_assert(ATK_IS_SELECTION(obj));

   /* only one objec can be selected at time, if '0' is selected then
    * rest of them should be unselected*/
   for (i = 0; i < 7; ++i)
     {
         Eina_Bool state;

         state =  atk_selection_is_child_selected(ATK_SELECTION(obj), i);
         /* only item with index EAIL_TEST_SELECTED_INDEX should be selected */
         if (i == EAIL_TEST_SELECTED_INDEX)
            g_assert(TRUE == state);
         else
            g_assert(FALSE == state);

     }

   /* NOTE: segment control widget does not support focus */
   eailu_test_atk_focus(obj, FALSE);

   printf("DONE. All SegmentControl tests passed successfully \n");
   eail_test_code_called = TRUE;
}

static void
_on_focus_in(void *data, Evas_Object *obj, void *event_info)
{
   AtkObject *aobj = atk_get_root();
   g_assert(aobj);

   eailu_traverse_children_for_type_call_cb
               (aobj, EAIL_TYPE_FOR_SEGMENTCONTROL, _test_segment_control_object);

   g_assert(eail_test_code_called);
   _on_done();
}

static void
_initialize_and_show_diskselector(Evas_Object *win)
{
   Evas_Object *sgm_ctrl = NULL;
   Elm_Object_Item *obj_item = NULL;
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

   /* default */
   sgm_ctrl = elm_segment_control_add(win);
   evas_object_size_hint_weight_set
                                 (sgm_ctrl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set
                                 (sgm_ctrl, EVAS_HINT_FILL, EVAS_HINT_FILL);

   /* TODO: RJ, uncomment box addding when box-listing will be fixed */
   //elm_box_pack_end(bx, ds);

   for (i = 0; i < sizeof(lbl) / sizeof(lbl[0]); i++)
     {
         obj_item = elm_segment_control_item_add(sgm_ctrl, NULL , lbl[i]);

         /* marking element as selected */
         if (EAIL_TEST_SELECTED_INDEX == i)
            elm_segment_control_item_selected_set(obj_item, EINA_TRUE);
     }


   evas_object_show(sgm_ctrl);
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
