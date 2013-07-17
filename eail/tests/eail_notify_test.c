#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define EAIL_TYPE_FOR_CHECK "EailNotify"
#define EAIL_TEST_LABEL_TITTLE "test label"

/* used to determine if test code has been successfully called
 * (sometimes may happen that ATK object to test in hierarchy cannot be found)*/
static gboolean eail_test_code_called = FALSE;

static void
_test_notify_object(AtkObject *obj);

static void
_on_done(void *data, Evas_Object *obj, void *event_info)
{
   elm_exit();
}

static void
on_focus_in(void *data, Evas_Object *obj, void *event_info)
{
   AtkObject *aobj = atk_get_root();
   g_assert(aobj);

   eailu_traverse_children_for_type_call_cb
                              (aobj, EAIL_TYPE_FOR_CHECK, _test_notify_object);

   g_assert(eail_test_code_called);
   _on_done(NULL, obj, NULL);
}

static void
_test_notify_object(AtkObject *obj)
{
   int child_amount = 0;
   AtkObject *nested_obj;

   printf("Testing notify instance....\n");

   child_amount = atk_object_get_n_accessible_children(obj);
   /* we have to check if there is nested label in Notify */
   g_assert(1 == child_amount);

   /* test if notify child  is label-type*/
   nested_obj = atk_object_ref_accessible_child(obj, 0);
   g_assert(eailu_is_object_with_type(nested_obj, "EailLabel"));
   g_assert(eailu_is_object_with_name(nested_obj, EAIL_TEST_LABEL_TITTLE));

   /* For notify widget focusing doesn't work */
   eailu_test_atk_focus(obj, FALSE);

   printf("Tests passed successfully\n");
   eail_test_code_called = TRUE;
}

static void
_initialize_and_show_notify(Evas_Object *win)
{
   Evas_Object *notify, *box, *label;

   box = elm_box_add(win);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, box);
   evas_object_show(box);

   label = elm_label_add(win);
   elm_object_text_set(label, EAIL_TEST_LABEL_TITTLE);
   evas_object_show(label);

   notify = elm_notify_add(win);
   elm_notify_allow_events_set(notify, EINA_FALSE);
   //elm_notify_timeout_set(notify, 5);
   elm_object_content_set(notify, label);
   evas_object_size_hint_weight_set(notify, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(notify);
   elm_box_pack_end(box, notify);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, on_focus_in);
   _initialize_and_show_notify(win);

   evas_object_show(win);

   elm_run();
   elm_shutdown();

   /* exit code */
   return 0;
}
ELM_MAIN()
