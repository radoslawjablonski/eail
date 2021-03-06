#include <Elementary.h>
#include <atk/atk.h>

//#define DEBUG 1

#include "eail_test_utils.h"

#define ACTIONS_NUMBER 1

/* used to determine if test code has been successfully called
 * (sometimes may happen that ATK object to test in hierarchy cannot be found)*/
static gboolean eail_test_code_called = FALSE;

static void
_test_ctxpopup_object(AtkObject *obj)
{
   int child_amount = 0;
   AtkObject *nested_obj;
   AtkStateSet *state_set = NULL;

   _printf("Testing ctxpopup instance....\n");

   // test AtkAction
   g_assert(ACTIONS_NUMBER == atk_action_get_n_actions(ATK_ACTION(obj)));
   eailu_test_action_activate(ATK_ACTION(obj), "dismiss");
   g_assert((eailu_get_action_number(ATK_ACTION(obj), "typo")) == -1);
   eailu_test_action_description_all(ATK_ACTION(obj));

   child_amount = atk_object_get_n_accessible_children(obj);
   /* we have to check if there is nested label in Notify */
   g_assert(1 == child_amount);

   /* test if ctxpopup child  is list-type*/
   nested_obj = atk_object_ref_accessible_child(obj, 0);
   g_assert(eailu_is_object_with_type(nested_obj, "EailList"));

   /* check if atk sees proper amount of list items in ctxpopup */
   child_amount = atk_object_get_n_accessible_children(nested_obj);
   g_assert(6 == child_amount);

   /* NOTE: in ctxpopup object focus automatically goes to nested
    * label */
   atk_component_grab_focus(ATK_COMPONENT(obj));
   state_set = atk_object_ref_state_set(nested_obj);
   /* checking if nested label is now focused... */
   g_assert(atk_state_set_contains_state(state_set, ATK_STATE_FOCUSED));

   _printf("Tests passed successfully\n");
   eail_test_code_called = TRUE;
}

static void
_ctxpopup_item_cb(void *data, Evas_Object *obj, void *event_info)
{
   _printf("ctxpopup item selected: %s\n", elm_object_item_text_get(event_info));
}

Elm_Object_Item *item_new(Evas_Object *ctxpopup, const char * label, const char *icon)
{
   Evas_Object *ic = elm_icon_add(ctxpopup);
   elm_icon_standard_set(ic, icon);
   elm_image_resizable_set(ic, EINA_FALSE, EINA_FALSE);
   return elm_ctxpopup_item_append(ctxpopup, label, ic, _ctxpopup_item_cb, NULL);
}

static void
_init_ctxpopup(Evas_Object *win)
{
   Evas_Object *ctxpopup, *box;
   Elm_Object_Item *object_it;

   box = elm_box_add(win);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, box);
   evas_object_show(box);

   ctxpopup = elm_ctxpopup_add(win);
   item_new(ctxpopup, "Go to home folder", "home");
   item_new(ctxpopup, "Save file", "file");
   item_new(ctxpopup, "Delete file", "delete");
   object_it = item_new(ctxpopup, "Navigate to folder", "folder");
   elm_object_item_disabled_set(object_it, EINA_TRUE);
   item_new(ctxpopup, "Edit entry", "edit");
   object_it = item_new(ctxpopup, "Set date and time", "clock");

   evas_object_size_hint_weight_set(ctxpopup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(ctxpopup);
   elm_box_pack_end(box, ctxpopup);
}

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
         (aobj, "EailCtxpopup", _test_ctxpopup_object);

   g_assert(eail_test_code_called);
   _on_done(NULL, obj, NULL);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, on_focus_in);
   _init_ctxpopup(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
