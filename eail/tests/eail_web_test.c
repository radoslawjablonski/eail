#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define EAIL_TYPE_FOR_WEB "EailWeb"

/* used to determine if test code has been successfully called
 * (sometimes may happen that ATK object to test in hierarchy cannot be found)*/
static gboolean eail_test_code_called = FALSE;

static void
_test_web_object(AtkObject *obj);

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
                  (aobj,EAIL_TYPE_FOR_WEB, _test_web_object);
   g_assert(eail_test_code_called);

   _on_done(NULL, NULL, NULL);
}

static void
_test_web_object(AtkObject *obj)
{
   const char *name = NULL;
   int child_count = 0;

   printf("Testing Web instance....\n");

   name = atk_object_get_name(obj);
   g_assert(name == NULL); /* name not implemented*/

   child_count =  atk_object_get_n_accessible_children(obj);
   g_assert(child_count == 0);

   eailu_test_atk_focus(obj, TRUE);

   printf("DONE. All WEB tests passed successfully \n");
   eail_test_code_called = TRUE;
}

static void
_default_web_content_set(Evas_Object *web)
{
#ifdef HAVE_ELEMENTARY_WEB
   Evas_Object *view, *frame;
   const char contents[] = ""
      "<html>\n"
      "  <head>\n"
      "    <title>Nothing to see here, move along</title>\n"
      "  </head>\n"
      "  <body>\n"
      "    <a href=\"http://www.enlightenment.org\" target=\"_blank\">E</a>\n"
      "    <br />\n"
      "    <a href=\"http://www.google.com\" target=\"_blank\">Google</a>\n"
      "    <br />\n"
      "  </body>\n"
      "</html>\n";

   view = elm_web_webkit_view_get(web);
   frame = ewk_view_frame_main_get(view);
   ewk_frame_contents_set(frame, contents, sizeof(contents) - 1, "text/html",
                          "UTF-8", NULL);
#else
  printf("Webview could not be initialized - NO WEBKIT SUPPORT ON PLATFORM\n");
  printf("However still raw web widget implementation CAN be tested \n");
  (void) web;
#endif
}

static void
_initialize_and_show_webview(Evas_Object *win)
{
   Evas_Object *web;//, *box;

//   box = elm_box_add(win);
//   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
//   elm_win_resize_object_add(win, box);
//   evas_object_show(box);

   web = elm_web_add(win);
   evas_object_size_hint_weight_set(web, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(web, EVAS_HINT_FILL, 0.5);
   _default_web_content_set(web);

   //elm_box_pack_end(box, web);

   evas_object_show(web);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, on_focus_in);
   _initialize_and_show_webview(win);

   evas_object_show(win);

   elm_run();
   elm_shutdown();

   /* exit code */
   return 0;
}
ELM_MAIN()
