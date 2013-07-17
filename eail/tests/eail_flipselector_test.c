
#include <Elementary.h>

#include "atk/atk.h"
#include "eail_test_utils.h"

#define ITEM_SEL_EINA 2
#define ITEM_SEL_EDJE 3
#define ITEM_SEL_ELM 0
#define ACTIONS_COUNT 2
#define MAX_SELECTIONS 1
#define ITEMS_COUNT 8

static void _init_flipselector(Evas_Object *win);
static void _do_test(AtkObject *obj);
static void _on_focus_in(void *data,
                         Evas_Object *obj,
                         void *event_info);
static void _on_done(void *data,
                     Evas_Object *obj,
                     void *event_info);

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win = NULL;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_flipselector(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()

void
_init_flipselector(Evas_Object *win)
{
   unsigned int i;
   Evas_Object *bg, *bx, *fp;
   static const char *lbl[] =
     {
        "Elementary",
        "Evas",
        "Eina",
        "Edje",
        "Eet",
        "Ecore",
        "Efreet",
        "Edbus"
     };

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);

   fp = elm_flipselector_add(win);
   evas_object_size_hint_weight_set(fp, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   for (i = 0; i < sizeof(lbl) / sizeof(lbl[0]); i++)
     elm_flipselector_item_append(fp, lbl[i], NULL, NULL);

   elm_box_pack_end(bx, fp);
   evas_object_show(fp);
}

void
_do_test(AtkObject *obj)
{
   const char *type_name = g_type_name(G_TYPE_FROM_INSTANCE(obj));

   g_assert(ATK_IS_OBJECT(obj));
   g_assert_cmpstr(type_name, ==, "EailFlipselector");

   g_assert(atk_object_get_n_accessible_children(obj) == ITEMS_COUNT);

   /*
    * AtkSelection tests
    */
   g_assert(ATK_IS_SELECTION(obj));

   /* atk_selection_add_selection test */
   g_assert(atk_selection_add_selection(ATK_SELECTION(obj), ITEM_SEL_EINA));
   g_assert(atk_selection_is_child_selected(ATK_SELECTION(obj), ITEM_SEL_EINA));

   /* atk_selection_clear_selection test */
   g_assert(atk_selection_clear_selection(ATK_SELECTION(obj)) == FALSE);

   /* eail_flipselector_ref_selection test */
   g_assert(atk_selection_is_child_selected(ATK_SELECTION(obj), ITEM_SEL_EINA));

   /* atk_selection_get_selection_count test */
   g_assert(atk_selection_get_selection_count(ATK_SELECTION(obj)) == MAX_SELECTIONS);

   /* atk_selection_is_child_selected test */
   g_assert(atk_selection_is_child_selected(ATK_SELECTION(obj), ITEM_SEL_EINA));

   /* atk_selection_remove_selection test */
   g_assert(atk_selection_remove_selection(ATK_SELECTION(obj), ITEM_SEL_EINA));
   g_assert(atk_selection_is_child_selected(ATK_SELECTION(obj), ITEM_SEL_ELM));

   /* atk_selection_select_all_selection test */
   g_assert(atk_selection_select_all_selection(ATK_SELECTION(obj)) == FALSE);

   /*
    * AtkAction tests
    */
   g_assert(ATK_IS_ACTION(obj));

   /* atk_action_get_n_actions test */
   g_assert(atk_action_get_n_actions(ATK_ACTION(obj)) == ACTIONS_COUNT);

   /* test existing actions */
   g_assert(atk_selection_add_selection(ATK_SELECTION(obj), ITEM_SEL_EINA));
   eailu_test_action_activate(ATK_ACTION(obj), "next");
   g_assert(atk_selection_is_child_selected(ATK_SELECTION(obj), ITEM_SEL_EDJE));

   eailu_test_action_activate(ATK_ACTION(obj), "prev");
   g_assert(atk_selection_is_child_selected(ATK_SELECTION(obj), ITEM_SEL_EINA));

   /* test for non-existing action */
   g_assert((eailu_get_action_number(ATK_ACTION(obj), "typo")) == -1);

   /* test set/get action description */
   eailu_test_action_description_all(ATK_ACTION(obj));
}

void
_on_focus_in(void *data, Evas_Object *obj, void *event_info)
{
   AtkObject *aobj = atk_get_root();

   g_assert(aobj);
   eailu_traverse_children_for_role_call_cb(aobj, ATK_ROLE_LIST, _do_test);

   _on_done(data, obj, event_info);
}

static void
_on_done(void *data,
         Evas_Object *obj,
         void *event_info)
{
   evas_object_del((Evas_Object*)data);
   elm_exit();
}
