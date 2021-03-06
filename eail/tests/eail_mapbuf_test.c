#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

static void
_on_done(void *data,
         Evas_Object *obj,
         void *event_info)
{
   fprintf(stdout, "EailMapbuf TESTS DONE!\n");
   elm_exit();
}

static void
_test_mapbuf_obj(AtkObject *object)
{
   int child_count = atk_object_get_n_accessible_children(object);
   g_assert(child_count == 1);

   AtkStateSet *state_set = atk_object_ref_state_set(object);
   gboolean enabled = atk_state_set_contains_state(state_set,
                                                   ATK_STATE_ENABLED);
   g_object_unref(state_set);
   g_assert(enabled);

   AtkObject *child = atk_object_ref_accessible_child(object, 0);
   g_assert(ATK_IS_OBJECT(child));
   child_count = atk_object_get_n_accessible_children(child);
   for (int i = 0; i < child_count; i++)
     {
        AtkObject *image = atk_object_ref_accessible_child(child, i);
        gboolean is_image = ATK_IS_IMAGE(image);
        g_object_unref(image);
        g_assert(is_image);
     }
   g_object_unref(child);
   g_assert(child_count == 25);

   eailu_test_atk_focus(object, FALSE);
}

static void
_on_focus_in(void *data, Evas_Object *obj, void *event_info)
{
   AtkObject *aobj = atk_get_root();
   g_assert(aobj);

   eailu_traverse_children_for_type_call_cb
                                       (aobj, "EailMapbuf",_test_mapbuf_obj);

   _on_done(NULL, obj, NULL);
}


EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win, *bg, *bx, *mb, *grid, *ic;
   unsigned int i, j;

   win = eailu_create_test_window_with_glib_init(NULL, _on_focus_in);

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(bx, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);

   mb = elm_mapbuf_add(win);
   evas_object_size_hint_weight_set(mb, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(mb, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(bx, mb);

   grid = elm_grid_add(win);
   elm_grid_size_set(grid, 200, 200);
   evas_object_show(grid);
   for (i = 0; i < 5; i++)
     {
        for (j = 0; j < 5; j++)
          {
             int w, h;
             ic = elm_icon_add(win);
             elm_icon_standard_set(ic, "home");
             elm_image_resizable_set(ic, EINA_FALSE, EINA_FALSE);
             evas_object_show(ic);
             elm_image_object_size_get(ic, &w, &h);
             elm_grid_pack(grid, ic, 5 + (w * i),  5 + (h * j) , w, h);
          }
     }

   elm_object_content_set(mb, grid);
   elm_mapbuf_enabled_set(mb, EINA_TRUE);
   elm_mapbuf_alpha_set(mb, EINA_TRUE);
   elm_mapbuf_smooth_set(mb, EINA_TRUE);
   evas_object_show(mb);

   evas_object_resize(win, 700, 500);
   evas_object_show(win);

   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
