#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define EAIL_TYPE_FOR_TOOLBAR "EailToolbar"
#define EAIL_FIRST_TOOLBAR_ITEM_NAME "Print"
#define EAIL_TEST_SUBITEM_TYPE "EailItem"

/* used to determine if test code has been successfully called
 * (sometimes may happen that ATK object to test in hierarchy cannot be found)*/
static gboolean eail_test_code_called = FALSE;

static void
_test_toolbar_object(AtkObject *obj);

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
                        (aobj, EAIL_TYPE_FOR_TOOLBAR, _test_toolbar_object);

   g_assert(eail_test_code_called);
   _on_done(NULL, obj, NULL);
}

static void
_test_toolbar_object(AtkObject *obj)
{
   int child_amount = 0;
   AtkObject *nested_obj;

   printf("Testing toolbar instance....\n");

   g_assert(eailu_is_object_with_role(obj, ATK_ROLE_TOOL_BAR));
   g_assert(eailu_is_object_with_type(obj, EAIL_TYPE_FOR_TOOLBAR));
   child_amount = atk_object_get_n_accessible_children(obj);
   g_assert(3 == child_amount);

   /* test if toolbar child is EailToolbarItem type*/
   nested_obj = atk_object_ref_accessible_child(obj, 0);
   g_assert(eailu_is_object_with_type(nested_obj, EAIL_TEST_SUBITEM_TYPE));
   g_assert(
          eailu_is_object_with_name(nested_obj, EAIL_FIRST_TOOLBAR_ITEM_NAME));

   g_assert(eailu_is_object_with_role(nested_obj, ATK_ROLE_MENU_ITEM));

   eailu_test_atk_focus(obj, TRUE);
   eailu_test_action_activate(ATK_ACTION(nested_obj), "click");

   printf("DONE. All toolbar tests passed successfully \n");
   eail_test_code_called = TRUE;
}

static void
_initialize_and_show_toolbar(Evas_Object *win)
{
   Evas_Object *toolbar, *box, *menu;
   Elm_Object_Item *toolbar_item;

   box = elm_box_add(win);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, box);
   evas_object_show(box);

   toolbar = elm_toolbar_add(win);
   elm_toolbar_shrink_mode_set(toolbar, ELM_TOOLBAR_SHRINK_SCROLL);
   evas_object_size_hint_weight_set(toolbar, 0.0, 0.0);
   evas_object_size_hint_align_set(toolbar, EVAS_HINT_FILL, 0.0);
   evas_object_show(toolbar);

   elm_toolbar_item_append
         (toolbar, "document-print", EAIL_FIRST_TOOLBAR_ITEM_NAME, NULL, NULL);
   elm_toolbar_item_append(toolbar, "folder-new", "New Folder", NULL, NULL);
   toolbar_item = elm_toolbar_item_append
                  (toolbar, NULL, "Nested menu", NULL, NULL);
   elm_toolbar_item_menu_set(toolbar_item, EINA_TRUE);
   menu = elm_toolbar_item_menu_get(toolbar_item);

   elm_menu_item_add
                  (menu, NULL, "emptytrash", "first menu item", NULL, NULL);
   elm_menu_item_add
               (menu, NULL, "trashcan_full", "second menu item", NULL, NULL);

   elm_box_pack_end(box, toolbar);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, on_focus_in);
   _initialize_and_show_toolbar(win);

   evas_object_show(win);

   elm_run();
   elm_shutdown();

   /* exit code */
   return 0;
}
ELM_MAIN()
