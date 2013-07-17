#include <Elementary.h>
#include <atk/atk.h>

//#define DEBUG 1

#include "eail_test_utils.h"

#define ACTIONS_NUMBER 1

static void
_on_photo_clicked(void *data, Evas_Object *obj, void *ev)
{
   _printf("photo clicked called\n");
}

static void
_do_test_photo(AtkObject *obj)
{
   const char *name = atk_object_get_name(obj);
   const char *type_name = g_type_name(G_TYPE_FROM_INSTANCE(obj));
   const char * const desc_test = "top secret";
   const char *desc;
   int height = 0, width  = 0;
   int x = -1, y = -1;

   _printf("_get_name: %s\n", name ? name : "NULL");
   _printf("_get_type_name: %s\n", type_name ? type_name : "NULL");

   g_assert(ATK_IS_IMAGE(obj));

   AtkStateSet *state_set = atk_object_ref_state_set(obj);
   g_object_unref(state_set);

   // test AtkImage
   atk_image_get_image_position(ATK_IMAGE(obj), &x, &y, ATK_XY_SCREEN);
   _printf("atk_image_get_image_position on screen: x: %d y %d\n", x, y);

   g_assert(NULL == atk_image_get_image_description(ATK_IMAGE(obj)));
   g_assert(TRUE == atk_image_set_image_description(ATK_IMAGE(obj), desc_test));
   desc = atk_image_get_image_description(ATK_IMAGE(obj));
   _printf("atk_image_get_image_description: %s\n", desc ? desc : "NULL");
   g_assert(NULL != desc);
   g_assert_cmpstr(desc_test, ==, desc);

   atk_image_get_image_size(ATK_IMAGE(obj), &height, &width);
   _printf("atk_image_get_image_size: height %d width %d\n", height, width);

   // test AtkAction
   g_assert(ACTIONS_NUMBER == atk_action_get_n_actions(ATK_ACTION(obj)));
   eailu_test_action_activate(ATK_ACTION(obj), "click");
   g_assert((eailu_get_action_number(ATK_ACTION(obj), "typo")) == -1);
   eailu_test_action_description_all(ATK_ACTION(obj));
}

static void
_init_photo(Evas_Object *win)
{
   Evas_Object *bg, *bx, *photo;
   const char * const filename = "./data/th-whiterabbit01.jpg";

   bg = elm_bg_add(win);
   elm_bg_color_set(bg, 255, 255, 255);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   bx = elm_box_add(win);
   elm_box_horizontal_set(bx, EINA_TRUE);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(bx, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_homogeneous_set(bx, EINA_TRUE);
   elm_win_resize_object_add(win, bx);

   photo = elm_photo_add(win);
   g_assert(elm_photo_file_set(photo, filename));
   elm_photo_size_set(photo, 100);
   elm_photo_aspect_fixed_set(photo, EINA_FALSE);
   evas_object_smart_callback_add(photo, "clicked", _on_photo_clicked, NULL);
   evas_object_size_hint_weight_set(photo, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_photo_fill_inside_set(photo, EINA_TRUE);
   evas_object_size_hint_align_set(photo, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(bx, photo);
   evas_object_show(photo);

   evas_object_show(bx);
}

static void
_on_done(void *data, Evas_Object *obj, void *event_info)
{
   elm_exit();
}

static void
_on_focus_in(void *data, Evas_Object *obj, void *event_info)
{
   AtkObject *aobj = atk_get_root();
   g_assert(aobj);

   eailu_traverse_children_for_type_call_cb(aobj, "EailPhoto", _do_test_photo);

   _on_done(NULL, obj, NULL);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_photo(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
