#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define EAIL_TEST_BUTTON1_NAME "Button text"

/* used to determine if test code has been successfully called
 * (sometimes may happen that ATK object to test in hierarchy cannot be found)*/
static gboolean eail_test_code_called = FALSE;

static void
_on_done(void *data, Evas_Object *obj, void *event_info)
{
   elm_exit();
}

static void
on_click(void *data, Evas_Object *obj, void *event_info)
{
   printf("\t\"%s\" button clicked with data %s\n",
          elm_object_part_text_get(obj, "default"), (const char*) data);
}

static void
_test_focus_on_button(AtkObject *obj)
{
   AtkObject *focused_obj = NULL;
   gboolean success = FALSE;

   printf("Testing focus....\n");

   focused_obj = atk_get_focus_object();
   /* at first there should be no focus object */
   g_assert(!focused_obj);

   /* changing focus */
   g_assert(ATK_IS_COMPONENT(obj));
   success = atk_component_grab_focus(ATK_COMPONENT(obj));
   g_assert(success);

   /* now focus should point to our button*/
   focused_obj = atk_get_focus_object();
   g_assert(focused_obj);

   g_assert(focused_obj == obj);

   printf("DONE. All focus test passed successfully \n");
   eail_test_code_called = TRUE;
}

static void
_on_focus_in(void *data, Evas_Object *obj, void *event_info)
{
   AtkObject *button;
   AtkObject *aobj = atk_get_root();
   g_assert(aobj);

   /* button 1 */
   button = eailu_find_child_with_name(aobj, EAIL_TEST_BUTTON1_NAME);
   _test_focus_on_button(button);

   g_assert(eail_test_code_called);
   elm_exit();
}

static void
_initialize_and_show_buttons(Evas_Object *win)
{
   Evas_Object *bg  = NULL, *button1 = NULL;

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   button1 = elm_button_add(win);
   elm_object_text_set(button1, "Button text");
   evas_object_smart_callback_add(button1, "clicked", on_click, "test data");
   evas_object_resize(button1, 100, 60);
   evas_object_move(button1, 50, 20);
   evas_object_show(button1);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _initialize_and_show_buttons(win);

   evas_object_show(win);

   elm_run();
   elm_shutdown();

   /* exit code */
   return 0;
}
ELM_MAIN()
