
#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define EAIL_TYPE_FOR_MENU "EailMenu"
#define EAIL_TYPE_FOR_MENU_ITEM "EailMenuItem"
#define T_MENU_ITEM_WITH_NO_CHILD_NAME "first item"
#define T_SUBM_WITH_ICON_NAME "RJ Menu 1 with icon"
#define T_NESTED_BUTTON_NAME "button - delete items (2nd level)"
#define T_MENU_WITH_SUB_AND_ICON_NAME "second item"

/* used to determine if test code has been successfully called
 * (sometimes may happen that ATK object to test in hierarchy cannot be found)*/
static gboolean eail_test_code_called = FALSE;

static void
on_done(void *data, Evas_Object *obj, void *event_info)
{
   /* quit the mainloop (elm_run) */
   elm_exit();
}

static void
_show(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
   Evas_Event_Mouse_Down *ev = event_info;
   elm_menu_move(data, ev->canvas.x, ev->canvas.y);
   evas_object_show(data);
}

void test_menu_atk_obj(AtkObject *aobj)
{
   AtkObject *found_obj = NULL;
   int child_amount = 0;

   printf("TESTING EAIL_MENU implementation....\n");
   found_obj = eailu_find_child_with_name(aobj, T_SUBM_WITH_ICON_NAME);

   g_assert(found_obj);

   /* checking if nested button can be found */
   found_obj = eailu_find_child_with_name(aobj, T_NESTED_BUTTON_NAME);
   g_assert(found_obj);

   /* checking if menu with submenus AND nested icon can be found */
   found_obj = eailu_find_child_with_name(aobj, T_MENU_WITH_SUB_AND_ICON_NAME);
   g_assert(found_obj);
   eailu_test_atk_focus(found_obj, FALSE);

   /* submenu with icons should have 7 children - one for each of 5 submenus,
    * plus 1 for separator, which is shown as MENU ITEM with empty name
    * plus 1 for nested ICON */
   child_amount = atk_object_get_n_accessible_children(found_obj);
   g_assert(7 == child_amount);

   /* For menu widget focusing doesn't work */
   eailu_test_atk_focus(aobj, FALSE);

   /* Testing click action.
    * TODO: RJ, here w have to delay checking - it has to be done
    * when menu object will be fully visible*/
   g_assert(ATK_IS_ACTION(found_obj));
   eailu_test_action_activate(ATK_ACTION(found_obj), "click");
   /* NOTE: actually only firing action
    * TODO: verify if target item has been REALLY clicked, eg.
    * by checking if object has been selected */

   eail_test_code_called = TRUE;
   printf("DONE. All test passed successfully \n");
}

static void on_focus_in(void *data, Evas_Object *obj, void *event_info)
{
   AtkObject *aobj = atk_get_root();
   g_assert(aobj);

   test_menu_atk_obj(aobj);

   g_assert(eail_test_code_called);
   /* force exit on end of test */
   on_done(NULL, NULL, NULL);
}

void
initialize_menu(Evas_Object *win)
{
   Evas_Object *menu, *button;
   Elm_Object_Item *menu_it, *menu_it1;

   menu = elm_menu_add(win);
   elm_object_tree_focus_allow_set(win, EINA_TRUE);

   elm_menu_item_add
            (menu, NULL, NULL, T_MENU_ITEM_WITH_NO_CHILD_NAME, NULL, NULL);
   menu_it = elm_menu_item_add
               (menu, NULL, "mail-reply-all", T_MENU_WITH_SUB_AND_ICON_NAME,
                NULL, NULL );
   elm_menu_item_add
      (menu, menu_it, "object-rotate-left", T_SUBM_WITH_ICON_NAME, NULL, NULL);

   button = elm_button_add(win);
   elm_object_text_set(button, T_NESTED_BUTTON_NAME);
   menu_it1 = elm_menu_item_add(menu, menu_it, NULL, NULL, NULL, NULL );
   elm_object_item_content_set(menu_it1, button);

   /* separator by atk is treated as empty MENU ITEM */
   elm_menu_item_separator_add(menu, menu_it);
   elm_menu_item_add(menu, menu_it, NULL, "third item (2nd lev)", NULL, NULL);
   elm_menu_item_add(menu, menu_it, NULL, "fourth item (2nd lev)", NULL, NULL);
   elm_menu_item_add
         (menu, menu_it, "window-new", "sub menu (2nd lev)", NULL, NULL );

   menu_it = elm_menu_item_add(menu, NULL, NULL, "third item", NULL, NULL );
   elm_object_item_disabled_set(menu_it, EINA_TRUE );

   evas_object_event_callback_add(win, EVAS_CALLBACK_MOUSE_DOWN, _show, menu);
   evas_object_show(menu);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(on_done, on_focus_in);
   initialize_menu(win);

   /* and show the window */
   evas_object_show(win);

   elm_run();
   elm_shutdown();

   /* exit code */
   return 0;
}
ELM_MAIN()
