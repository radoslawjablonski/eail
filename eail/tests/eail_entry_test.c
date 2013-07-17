#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

Evas_Object *en;

void
_test_state(AtkObject *obj, AtkStateType state_type)
{
    AtkStateSet *state_set;

    state_set = atk_object_ref_state_set(obj);
    g_assert(atk_state_set_contains_state(state_set, state_type));
    g_object_unref(state_set);
}

static void
_do_test(AtkObject *obj)
{
   AtkRole role;
   gchar *text;
   int start_offset, end_offset;
   const char *type_name = g_type_name(G_TYPE_FROM_INSTANCE(obj));

   g_assert_cmpstr(type_name, ==, "EailEntry");

   _test_state(obj, ATK_STATE_EDITABLE);

   role = atk_object_get_role(obj);
   if (role == ATK_ROLE_TEXT)
     {
        _test_state(obj, ATK_STATE_MULTI_LINE);

        /* AtkText test */
        text = atk_text_get_text(ATK_TEXT(obj), 6, 11);
        g_assert_cmpstr(text, ==, "ipsum");
        g_free(text);

        g_assert(atk_text_get_character_at_offset(ATK_TEXT(obj), 12) == 'd');

        g_assert(atk_text_get_character_count(ATK_TEXT(obj)) == 440);

        g_assert(atk_text_set_caret_offset(ATK_TEXT(obj), 13));
        g_assert(atk_text_get_caret_offset(ATK_TEXT(obj)) == 13);

        start_offset = 0;
        end_offset = 5;
        g_assert(atk_text_set_selection(ATK_TEXT(obj), 0, start_offset, end_offset));
        text = atk_text_get_selection(ATK_TEXT(obj), 0, &start_offset,  &end_offset);
        g_assert_cmpstr(text, ==, "LOREM");
        g_free(text);

        /* AtkEditableText */
        g_assert(ATK_IS_EDITABLE_TEXT(obj));

        text = g_strdup("Trololo lololololo olololo");
        atk_editable_text_set_text_contents(ATK_EDITABLE_TEXT(obj), text);
        g_free(text);

        end_offset = -1;
        text = atk_text_get_text(ATK_TEXT(obj), start_offset, end_offset);
        g_assert_cmpstr(text, ==, "Trololo lololololo olololo");
        g_free(text);

        //start_offset = 8;
        //end_offset = 18;
        //atk_editable_text_copy_text(ATK_EDITABLE_TEXT(obj), start_offset, end_offset);
        //atk_editable_text_set_text_contents(ATK_EDITABLE_TEXT(obj), "");
        //atk_editable_text_paste_text(ATK_EDITABLE_TEXT(obj), 0);

        //start_offset = 0;
        //end_offset = -1;
        //text = atk_text_get_text(ATK_TEXT(obj), start_offset, end_offset);
        //g_assert_cmpstr(text, ==, "lololololo");
        //g_free(text);

        //start_offset = 0;
        //end_offset = -1;
        //atk_editable_text_cut_text(ATK_EDITABLE_TEXT(obj), start_offset, end_offset);
        //text = atk_text_get_text(ATK_TEXT(obj), start_offset, end_offset);
        //g_assert_cmpstr(text, ==, NULL);
     }
   else if (role == ATK_ROLE_PASSWORD_TEXT)
     {
        _test_state(obj, ATK_STATE_SINGLE_LINE);
     }
}

static void
_init_entry(Evas_Object *win)
{
   Evas_Object *box, *tb, *bg, *label;
   const char *text1 = "Lorem ipsum dolor sit amet, consectetur adipisicing elit,"
       "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."
       "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris"
       "nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in"
       "reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla"
       "pariatur. Excepteur sint occaecat cupidatat non proident, sunt"
       "in culpa qui officia deserunt mollit anim id est laborum.";
   const char *text2 = "passwd";

   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   elm_win_autodel_set(win, EINA_TRUE);

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   box = elm_box_add(win);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, box);
   evas_object_show(box);

   tb = elm_box_add(win);
   elm_box_horizontal_set(tb, EINA_TRUE);
   evas_object_size_hint_weight_set(tb, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_weight_set(tb, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(box, tb);
   evas_object_show(tb);

   en = elm_entry_add(win);
   elm_entry_autosave_set(en, EINA_FALSE);
   elm_entry_line_wrap_set(en, ELM_WRAP_WORD);
   elm_entry_entry_set(en, text1);
   evas_object_size_hint_weight_set(en, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(box, en);
   evas_object_show(en);

   label = elm_label_add(win);
   elm_object_text_set(label, "enter password below");
   elm_object_style_set(label, "marker");
   evas_object_color_set(label, 255, 0, 0, 255);
   evas_object_size_hint_align_set(label, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(box, label);
   evas_object_show(label);

   en = elm_entry_add(win);
   elm_entry_password_set(en, EINA_TRUE);
   elm_entry_entry_set(en, text2);
   evas_object_size_hint_align_set(en, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(box, en);
   evas_object_show(en);

   evas_object_resize(win, 200, 300);
}

static void
_on_done(void *data, Evas_Object *obj, void *event_info)
{
   evas_object_del((Evas_Object*)data);
   elm_exit();
}

static void
_on_focus_in(void *data, Evas_Object *obj, void *event_info)
{
   AtkObject *aobj = atk_get_root();
   g_assert(aobj);

   eailu_traverse_children_for_type_call_cb(aobj, "EailEntry", _do_test);

   _on_done(data, obj, event_info);
}

EAPI_MAIN int
elm_main(int argc, char *argv[])
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_entry(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
