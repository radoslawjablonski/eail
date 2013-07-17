#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define EAIL_TYPE_FOR_DAY_SELECTOR "EailDayselector"
#define EAIL_TYPE_FOR_CHECK "EailCheck"

/* used to determine if test code has been successfully called
 * (sometimes may happen that ATK object to test in hierarchy cannot be found)*/
static gboolean eail_test_code_called = FALSE;

static void
_test_day_selector_object(AtkObject *obj);

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
         (aobj,EAIL_TYPE_FOR_DAY_SELECTOR, _test_day_selector_object);

   g_assert(eail_test_code_called);
   _on_done(NULL, obj, NULL);
}

static void
_test_day_selector_object(AtkObject *obj)
{
   const char *name = NULL;
   int child_count = 0;
   int i;

   printf("Testing Day selector instance....\n");

   name = atk_object_get_name(obj);
   g_assert(name == NULL); /* name not implemented*/

   child_count =  atk_object_get_n_accessible_children(obj);
   g_assert(child_count == 7);

   /* all fields have to be CHECK type*/
   for (i = 0; i < child_count; i++)
    {
       AtkObject *child = atk_object_ref_accessible_child(obj, i);
       g_assert(ATK_IS_OBJECT(child));
       g_assert(eailu_is_object_with_type(child, EAIL_TYPE_FOR_CHECK));

       g_object_unref(child);
     }

   eailu_test_atk_focus(obj, TRUE);

   printf("Tests passed successfully\n");
   eail_test_code_called = TRUE;
}

static void
_initialize_and_show_dayselector(Evas_Object *win)
{
   Evas_Object *dayselector, *box;

   box = elm_box_add(win);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, box);
   evas_object_show(box);

   dayselector = elm_dayselector_add(win);
   evas_object_size_hint_weight_set(dayselector, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(dayselector, EVAS_HINT_FILL, 0.5);

   /* TODO: RJ, add TC for checking day selection change
    * evas_object_smart_callback_add(dayselector, "dayselector,changed",
    * _changed_cb, NULL);
   */
   elm_box_pack_end(box, dayselector);
   evas_object_show(dayselector);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, on_focus_in);
   _initialize_and_show_dayselector(win);

   evas_object_show(win);

   elm_run();
   elm_shutdown();

   /* exit code */
   return 0;
}
ELM_MAIN()
