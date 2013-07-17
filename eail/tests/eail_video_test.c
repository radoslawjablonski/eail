#include <Elementary.h>
#include <atk/atk.h>
#include "eail_test_utils.h"

#define VIDEO_FILE "./data/softboy.avi"

static int eail_test_code_called = 0;
static void
_on_done(void *data, Evas_Object *obj, void *even_info)
{
   elm_exit();
}

static void
_test_video_object(AtkObject *object)
{
   AtkStateSet *state_set;
   AtkAction *action;
   AtkStreamableContent *streamable;
   gboolean contains, success;
   const gchar *name, *action_name, *mime, *uri;
   int action_num, n_mimes;
   GIOChannel *gio;

   eail_test_code_called++;

   state_set = atk_object_ref_state_set(object);
   contains = atk_state_set_contains_state(state_set, ATK_STATE_ANIMATED);
   g_object_unref(state_set);
   g_assert(contains);
   name = atk_object_get_name(object);
   fprintf(stdout, "name (it shoud not be null)%s\n", name);

   /*TODO got problems with avi playback and cannot test this, write real test*/
   g_assert(ATK_IS_ACTION(object));
   action = ATK_ACTION(object);
   action_num =  atk_action_get_n_actions(action);
   g_assert(action_num == 7);
   action_name = atk_action_get_name(action, 2);
   g_assert(!strcmp(action_name, "pause"));
   success =  atk_action_set_description(action, 2, "pause action desc");
   g_assert(success);
   name = atk_action_get_description(action, 2);
   g_assert(!strcmp(name, "pause action desc"));
   success = atk_action_do_action(action, 2);
   fprintf(stdout, "cannot test do action\n");

   /* test set/get action description */
   eailu_test_action_description_all(ATK_ACTION(object));

   /*AtkStreamableContent test*/
   g_assert(ATK_IS_STREAMABLE_CONTENT(object));
   streamable = ATK_STREAMABLE_CONTENT(object);
   n_mimes = atk_streamable_content_get_n_mime_types(streamable);
   g_assert(n_mimes == 1);
   mime = atk_streamable_content_get_mime_type(streamable, 0);
   g_assert(!strcmp(mime, "video/x-msvideo"));
   uri = atk_streamable_content_get_uri(streamable, "video/x-msvideo");
   fprintf(stdout, "file uri %s\n", uri);
   char *res = g_strrstr(uri, "softboy.avi");
   g_assert(res);
   gio = NULL;
   gio = atk_streamable_content_get_stream(streamable, "video/x-msvideo");
   g_assert(gio);
   if (gio)
     {
        g_io_channel_shutdown(gio, TRUE, NULL);
        g_io_channel_unref(gio);
     }
}

static void
_on_focus_in(void *data, Evas_Object *obj, void *event_info)
{
   AtkObject *aobj = atk_get_root();
   fprintf(stdout, "root is %snull\n", aobj == NULL ? "" : "not");
   g_assert(ATK_IS_OBJECT(aobj));
   eailu_traverse_children_for_type_call_cb(aobj,
                                            "EailVideo",
                                            _test_video_object);
   g_assert(0 < eail_test_code_called);
   g_object_unref(aobj);
   _on_done(NULL, NULL, NULL);
}

static void
_playback_started(void *data, Evas_Object *obj, void *ev)
{
   _on_focus_in(NULL, data, NULL);
}

static void
_test_video()
{
   Evas_Object *win, *bg, *video, *emotion;

   win = eailu_create_test_window_with_glib_init(_on_done, NULL);

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   video = elm_video_add(win);
   evas_object_size_hint_weight_set(video, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, video);
   evas_object_show(video);


   elm_video_file_set(video, VIDEO_FILE);
   elm_video_play(video);
   emotion = elm_video_emotion_get(video);
   evas_object_smart_callback_add(emotion,
                                  "playback_started",
                                  _playback_started,
                                  win);

   evas_object_resize(win, 800, 800);
   evas_object_show(win);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   _test_video();


   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
