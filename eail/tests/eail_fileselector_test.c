#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define EAIL_TYPE_FOR_FILESELECTOR "EailFileselector"

/* used to determine if test code has been successfully called
 * (sometimes may happen that ATK object to test in hierarchy cannot be found)*/
static gboolean eail_test_code_called = FALSE;

static void
_test_fileselector_object(AtkObject *obj);

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
               (aobj, EAIL_TYPE_FOR_FILESELECTOR, _test_fileselector_object);

   g_assert(eail_test_code_called);
   _on_done(NULL, obj, NULL);
}

static void
_test_atk_focus(AtkObject *obj)
{
   AtkStateSet *state_set = NULL;
   state_set = atk_object_ref_state_set(obj);

   g_assert(atk_state_set_contains_state(state_set, ATK_STATE_FOCUSABLE));
   /* in fileselector focus always gets to one of internal children after
    * marking as focused*/
   g_assert(atk_component_grab_focus(ATK_COMPONENT(obj)));
   g_assert(!atk_state_set_contains_state(state_set, ATK_STATE_FOCUSED));
}

static void
_test_fileselector_object(AtkObject *obj)
{
   int child_amount = 0;

   printf("Testing fileselector instance....\n");

   g_assert(eailu_is_object_with_role(obj, ATK_ROLE_FILE_CHOOSER));
   g_assert(eailu_is_object_with_type(obj, EAIL_TYPE_FOR_FILESELECTOR));
   child_amount = atk_object_get_n_accessible_children(obj);

   /*
    * up button, home button, list, ok button, cancel button, entry and list -
    * overall 7 child objects
    */
   g_assert(7 == child_amount);

   _test_atk_focus(obj);

   printf("DONE. All fileselector tests passed successfully \n");
   eail_test_code_called = TRUE;
}

static void
_initialize_and_show_fileselector(Evas_Object *win)
{
   Evas_Object *fileselector, *box;

   box = elm_box_add(win);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, box);
   evas_object_show(box);

   fileselector = elm_fileselector_add(win);
   elm_fileselector_path_set(fileselector, "/tmp");
   elm_fileselector_expandable_set(fileselector, EINA_FALSE);

   evas_object_size_hint_weight_set(fileselector, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(fileselector, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(fileselector);

   elm_box_pack_end(box, fileselector);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, on_focus_in);
   _initialize_and_show_fileselector(win);

   evas_object_show(win);

   elm_run();
   elm_shutdown();

   /* exit code */
   return 0;
}
ELM_MAIN()
