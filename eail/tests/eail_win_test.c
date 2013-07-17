#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define EAIL_TYPE_FOR_WINDOW "EailWindow"
#define EAIL_ACTION_FOR_MAXIMIZE "maximize"
#define EAIL_ACTION_FOR_MINIMIZE "minimize"

/* used to determine if test code has been successfully called
 * (sometimes may happen that ATK object to test in hierarchy cannot be found)*/
static gboolean eail_test_code_called = FALSE;

static void
_on_done(void *data, Evas_Object *obj, void *event_info)
{
   elm_exit();
}

static void
_test_window(AtkObject *obj)
{
   _printf("Testing win instance....\n");

   g_assert(ATK_IS_ACTION(obj));
   g_assert(2 == atk_action_get_n_actions(ATK_ACTION(obj)));

   eailu_test_action_activate(ATK_ACTION(obj), EAIL_ACTION_FOR_MAXIMIZE);
   eailu_test_action_activate(ATK_ACTION(obj), EAIL_ACTION_FOR_MINIMIZE);

   eail_test_code_called = TRUE;
   _printf("DONE. All WIN test passed successfully \n");
}

static void
_on_focus_in(void *data, Evas_Object *obj, void *event_info)
{
   AtkObject *aobj = atk_get_root();
   g_assert(aobj);

   eailu_traverse_children_for_type_call_cb
                                    (aobj, EAIL_TYPE_FOR_WINDOW, _test_window);

   g_assert(eail_test_code_called);
   elm_exit();
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   evas_object_show(win);

   elm_run();
   elm_shutdown();

   /* exit code */
   return 0;
}
ELM_MAIN()
