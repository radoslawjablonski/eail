#include <Elementary.h>
#include <atk/atk.h>

//#define DEBUG 1

#include "eail_test_utils.h"

#define ACTIONS_NUMBER 3

#define BUTTON_TEXT1   "Button1"
#define BUTTON_TEXT2   "button2"

#define ICON_SIZE 56

static void
_on_click(void *data, Evas_Object *obj, void *event_info)
{
   _printf("button click\n");
}

static void
_on_press(void *data, Evas_Object *obj, void *event_info)
{
   _printf("button press\n");
}

static void
_on_release(void *data, Evas_Object *obj, void *event_info)
{
   _printf("button release\n");
}

static void
_do_test(AtkObject *obj)
{
   const char *type_name = g_type_name(G_TYPE_FROM_INSTANCE(obj));
   const char *name = atk_object_get_name(ATK_OBJECT(obj));
   const char *type_name_child;
   int height = 0, width = 0;
   int x = -1, y = -1;
   AtkObject *xobj;

   _printf("type_name: %s\n", type_name ? type_name : "NULL");
   g_assert(name);
   _printf("name: %s\n", name);

   g_assert_cmpstr(type_name, ==, "EailButton");

   g_assert(ATK_IS_OBJECT(obj));
   g_assert(ATK_IS_ACTION(obj));
   g_assert(ATK_IS_TEXT(obj));

   // test AtkAction
   g_assert(ACTIONS_NUMBER == atk_action_get_n_actions(ATK_ACTION(obj)));
   eailu_test_action_activate(ATK_ACTION(obj), "click");
   eailu_test_action_activate(ATK_ACTION(obj), "press");
   eailu_test_action_activate(ATK_ACTION(obj), "release");
   g_assert((eailu_get_action_number(ATK_ACTION(obj), "typo")) == -1);
   eailu_test_action_description_all(ATK_ACTION(obj));

   if (!strcmp(name, BUTTON_TEXT1))
     {
        _printf("Button test: %s\n", BUTTON_TEXT1);
        // test children
        g_assert(0 == atk_object_get_n_accessible_children(obj));
     }
   else if (!strcmp(name, BUTTON_TEXT2))
     {
       _printf("Button test: %s\n", BUTTON_TEXT2);
       // test children
       g_assert(1 == atk_object_get_n_accessible_children(obj));

       xobj = atk_object_ref_accessible_child(obj, 0);
       g_assert(NULL != xobj);
       type_name_child = g_type_name(G_TYPE_FROM_INSTANCE(xobj));
       _printf("type_name_child: %s\n", type_name_child);
       g_assert_cmpstr(type_name_child, ==, "EailIcon");

       // test AtkImage
       atk_image_get_image_position(ATK_IMAGE(obj), &x, &y, ATK_XY_SCREEN);
       _printf("atk_image_get_image_position on screen: x: %d y %d\n", x, y);

       atk_image_get_image_size(ATK_IMAGE(obj), &height, &width);
       _printf("atk_image_get_image_size: height %d width %d\n", height, width);
       g_assert(ICON_SIZE == height && ICON_SIZE == width);
     }
   else
     {
       _printf("not find the button\n");
       g_assert(FALSE);
     }
}

static void
_init_button(Evas_Object *win)
{
   Evas_Object *bg, *icon, *button1, *button2;

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   icon = elm_icon_add(win);
   elm_icon_order_lookup_set(icon, ELM_ICON_LOOKUP_THEME_FDO);
   elm_icon_standard_set(icon, "home");
   evas_object_show(icon);

   button1 = elm_button_add(win);
   elm_object_text_set(button1, BUTTON_TEXT1);
   evas_object_smart_callback_add(button1, "clicked", _on_click, "button1");
   evas_object_smart_callback_add(button1, "pressed", _on_press, "button1");
   evas_object_smart_callback_add(button1, "unpressed", _on_release, "button1");

   evas_object_resize(button1, 140, 60);
   evas_object_move(button1, 50, 20);
   evas_object_show(button1);

   button2 = elm_button_add(win);
   elm_object_text_set(button2, BUTTON_TEXT2);
   evas_object_smart_callback_add(button2, "clicked", _on_click, "button2");
    elm_object_part_content_set(button2, "icon", icon);
   evas_object_resize(button2, 140, 60);
   evas_object_move(button2, 50, 120);
   evas_object_show(button2);
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

   eailu_traverse_children_for_role_call_cb(aobj, ATK_ROLE_PUSH_BUTTON, _do_test);

   _on_done(NULL, obj, NULL);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_button(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
