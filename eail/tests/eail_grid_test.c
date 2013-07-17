#include <Elementary.h>
#include <atk/atk.h>

//#define DEBUG 1

#include "eail_test_utils.h"

enum _slide_style
{
   SLIDE_SHORT,
   SLIDE_LONG,
   SLIDE_BOUNCE,
   SLIDE_STYLE_LAST
};

static void
_change_radio_cb(void *data, Evas_Object *obj, void *event)
{
   Evas_Object *lb = (Evas_Object *)data;
   int style = elm_radio_value_get((Evas_Object *)obj);

   switch (style)
     {
      case SLIDE_SHORT:
         _printf("change_radio: slide_short\n");
         elm_object_style_set(lb, "slide_short");
         break;

      case SLIDE_LONG:
         _printf("change_radio: slide_long\n");
         elm_object_style_set(lb, "slide_long");
         break;

      case SLIDE_BOUNCE:
         _printf("change_radio: slide_bounce\n");
         elm_object_style_set(lb, "slide_bounce");
         break;

      default:
         return;
     }
}

static void
_change_slider_cb(void *data, Evas_Object *obj, void *event_info)
{
   Evas_Object *lb = (Evas_Object *)data;
   double val = elm_slider_value_get(obj);
   elm_label_slide_duration_set(lb, val);
}

static void
_do_test(AtkObject *obj)
{
   const char *type_name = g_type_name(G_TYPE_FROM_INSTANCE(obj));
   const char *role_name = atk_role_get_name(atk_object_get_role(obj));

   _printf("role: %s\n", role_name ? role_name : "NULL");
   _printf("type_name: %s\n", type_name ? type_name : "NULL");

   g_assert_cmpstr(type_name, ==, "EailGrid");

   // test children
   g_assert(8 == atk_object_get_n_accessible_children(obj));
}

static void
_init_grid(Evas_Object *win)
{
   Evas_Object *bg, *gd, *lb, *rd, *rdg, *sl;

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   gd = elm_grid_add(win);
   elm_grid_size_set(gd, 100, 100);
   evas_object_size_hint_weight_set(gd, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, gd);
   evas_object_show(gd);

   lb = elm_label_add(win);
   elm_object_text_set(lb, "Test Label Ellipsis:");
   elm_grid_pack(gd, lb, 5, 5, 90, 10);
   evas_object_show(lb);

   lb = elm_label_add(win);
   elm_object_text_set(lb,
                       "This is a label set to ellipsis. "
                       "If set ellipsis to true and the text doesn't fit "
                       "in the label an ellipsis(\"...\") will be shown "
                       "at the end of the widget."
                      );

   elm_label_ellipsis_set(lb, EINA_TRUE);
   elm_grid_pack(gd, lb, 5, 15, 90, 10);
   evas_object_show(lb);

   lb = elm_label_add(win);
   elm_object_text_set(lb, "Test Label Slide:");
   elm_grid_pack(gd, lb, 5, 30, 90, 10);
   evas_object_show(lb);

   lb = elm_label_add(win);
   elm_object_style_set(lb, "slide_short");
   elm_object_text_set(lb,
                       "This is a label set to slide. "
                       "If set slide to true the text of the label "
                       "will slide/scroll through the length of label."
                       "This only works with the themes \"slide_short\", "
                       "\"slide_long\" and \"slide_bounce\"."
                      );
   elm_grid_pack(gd, lb, 5, 40, 90, 10);
   evas_object_show(lb);

   rd = elm_radio_add(win);
   elm_radio_state_value_set(rd, SLIDE_SHORT);
   elm_object_text_set(rd, "slide_short");
   evas_object_size_hint_weight_set(rd, EVAS_HINT_EXPAND, EVAS_HINT_FILL);
   evas_object_smart_callback_add(rd, "changed", _change_radio_cb, lb);
   elm_grid_pack(gd, rd, 5, 50, 30, 10);
   evas_object_show(rd);
   rdg = rd;

   rd = elm_radio_add(win);
   elm_radio_state_value_set(rd, SLIDE_LONG);
   elm_radio_group_add(rd, rdg);
   elm_object_text_set(rd, "slide_long");
   evas_object_size_hint_weight_set(rd, EVAS_HINT_EXPAND, EVAS_HINT_FILL);
   evas_object_smart_callback_add(rd, "changed", _change_radio_cb, lb);
   elm_grid_pack(gd, rd, 35, 50, 30, 10);
   evas_object_show(rd);

   rd = elm_radio_add(win);
   elm_radio_state_value_set(rd, SLIDE_BOUNCE);
   elm_radio_group_add(rd, rdg);
   elm_object_text_set(rd, "slide_bounce");
   evas_object_size_hint_weight_set(rd, EVAS_HINT_EXPAND, EVAS_HINT_FILL);
   evas_object_smart_callback_add(rd, "changed", _change_radio_cb, lb);
   elm_grid_pack(gd, rd, 65, 50, 30, 10);
   evas_object_show(rd);

   sl = elm_slider_add(win);
   elm_object_text_set(sl, "Slide Duration");
   elm_slider_unit_format_set(sl, "%1.1f units");
   elm_slider_min_max_set(sl, 1, 20);
   elm_slider_value_set(sl, 10);
   evas_object_size_hint_align_set(sl, EVAS_HINT_FILL, 0.5);
   evas_object_size_hint_weight_set(sl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_smart_callback_add(sl, "changed", _change_slider_cb, lb);
   elm_grid_pack(gd, sl, 5, 60, 90, 10);
   evas_object_show(sl);
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

   eailu_traverse_children_for_role_call_cb(aobj, ATK_ROLE_FILLER, _do_test);

   _on_done(NULL, obj, NULL);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_grid(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
