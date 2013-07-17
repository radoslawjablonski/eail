#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

/* used to determine if test code has been successfully called
 * (sometimes may happen that ATK object to test in hierarchy cannot be found)*/
static gboolean eail_test_code_called = FALSE;

static void
_test_panel_object(AtkObject *object)
{
   AtkRole role = atk_object_get_role(object);
   g_assert(role == ATK_ROLE_PANEL);
   int child_count = atk_object_get_n_accessible_children(object);
   g_assert(child_count == 1);

   /*test AtkAction*/
   g_assert(ATK_IS_ACTION(object));
   AtkAction *action = ATK_ACTION(object);
   int actions_num = atk_action_get_n_actions(action);
   g_assert(actions_num == 1);
   const gchar *action_name = atk_action_get_name(action, 0);
   g_assert(!strcmp("toggle", action_name));
   gboolean result = atk_action_set_description(action, 0, "toggle visibility");
   g_assert(result);
   const gchar *description = atk_action_get_description(action, 0);
   g_assert(!strcmp("toggle visibility", description));
   result = atk_action_do_action(action, 0);
   g_assert(result);

   /*test AtkStateSet*/
   AtkStateSet *state_set = atk_object_ref_state_set(object);
   gboolean contains = atk_state_set_contains_state(state_set,
                                                    ATK_STATE_VISIBLE);
   g_assert(contains);
   g_object_unref(state_set);

   /* TODO: test properly focus here, probably automatically focus
    * is changed to internal object */
   /* eailu_test_atk_focus(obj, TRUE); */

   eail_test_code_called = TRUE;
}

static void
_on_done(void *data, Evas_Object *obj, void *event_info)
{
   printf("EailPanel tests PASSED\n");
   elm_exit();
}

static void
_on_focus_in(void *data, Evas_Object *obj, void *event_info)
{
   AtkObject *aobj = atk_get_root();
   g_assert(aobj != NULL);

   eailu_traverse_children_for_type_call_cb
                                       (aobj,"EailPanel", _test_panel_object);

   g_assert(eail_test_code_called);
   _on_done(NULL, obj, NULL);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win, *bg, *bx, *panel, *content;

   win = eailu_create_test_window_with_glib_init(NULL, _on_focus_in);

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   bx = elm_box_add(win);
   elm_box_horizontal_set(bx, EINA_FALSE);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(bx, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);

   panel = elm_panel_add(win);
   elm_panel_orient_set(panel, ELM_PANEL_ORIENT_TOP);
   elm_panel_toggle(panel);
   evas_object_size_hint_weight_set(panel, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panel, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(panel);
   elm_box_pack_end(bx, panel);

   content = elm_label_add(win);
   elm_object_text_set(content, "content");
   evas_object_show(content);
   elm_object_content_set(panel, content);

   panel = elm_panel_add(win);
   elm_panel_orient_set(panel, ELM_PANEL_ORIENT_RIGHT);
   elm_panel_hidden_set(panel, EINA_TRUE);
   evas_object_size_hint_weight_set(panel, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panel, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(panel);
   elm_box_pack_end(bx, panel);

   content = elm_label_add(win);
   elm_object_text_set(content, "content2");
   evas_object_show(content);
   elm_object_content_set(panel, content);

   panel = elm_panel_add(win);
   evas_object_size_hint_weight_set(panel, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panel, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(panel);
   elm_box_pack_end(bx, panel);

   content = elm_label_add(win);
   elm_object_text_set(content, "content3");
   evas_object_show(content);
   elm_object_content_set(panel, content);

   evas_object_show(win);

   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
