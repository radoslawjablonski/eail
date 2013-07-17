#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define BUTTON_TEXT_SET(BT, TEXT) \
   elm_object_text_set((BT), (TEXT)); \
   elm_object_tooltip_text_set((BT), (TEXT)); \
   elm_object_tooltip_window_mode_set((BT), EINA_TRUE)

#define PACKAGE_DATA_DIR "./data"

static const char *img1 = PACKAGE_DATA_DIR "/whiterabbit01.jpg";
static const char *img2 = PACKAGE_DATA_DIR "/01.jpg";
static const char *img3 = PACKAGE_DATA_DIR "/02.jpg";
static const char *img4 = PACKAGE_DATA_DIR "/03.jpg";
static const char *img5 = PACKAGE_DATA_DIR "/04.jpg";
static const char *img8 = PACKAGE_DATA_DIR "/small-01.jpg";

static Evas_Object *nf;
static Eina_Bool eail_title_clicked_cb_called = EINA_FALSE;
Evas_Object *
_content_new(Evas_Object *parent, const char *img)
{
   Evas_Object *photo = elm_photo_add(parent);
   elm_photo_file_set(photo, img);
   elm_photo_fill_inside_set(photo, EINA_TRUE);
   elm_object_style_set(photo, "shadow");
   return photo;
}

static void
_title_clicked(void *data, Evas_Object *obj, void *event_info)
{
   printf("Title Clicked!\n");
   eail_title_clicked_cb_called = EINA_TRUE;
}

static void
_title_visible(void *data, Evas_Object *obj , void *event_info )
{
   elm_naviframe_item_title_visible_set(data,
                                        !elm_naviframe_item_title_visible_get(data));
}

static void
_promote(void *data, Evas_Object *obj , void *event_info )
{
   Elm_Object_Item *it = evas_object_data_get(nf, data);
   elm_naviframe_item_promote(it);
}

static void
_page5(Evas_Object *nf)
{
   Evas_Object *bt, *bt2, *content;
   Elm_Object_Item *it;

   bt = elm_button_add(nf);
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   BUTTON_TEXT_SET(bt, "Page 4");
   evas_object_smart_callback_add(bt, "clicked", _promote, "page4");

   bt2 = elm_button_add(nf);
   evas_object_size_hint_align_set(bt2, EVAS_HINT_FILL, EVAS_HINT_FILL);
   BUTTON_TEXT_SET(bt2, "Page 1");
   evas_object_smart_callback_add(bt2, "clicked", _promote, "page1");

   content = _content_new(nf, img5);
   it = elm_naviframe_item_insert_after(nf,
                                        elm_naviframe_top_item_get(nf),
                                        "Page 5",
                                        bt,
                                        bt2,
                                        content,
                                        NULL);
   elm_object_item_part_text_set(it, "subtitle",
                                 "This page is inserted without transition");

   evas_object_data_set(nf, "page5", it);
   elm_naviframe_item_promote(it);
}

static void
_page4(Evas_Object *nf)
{
   Evas_Object *bt, *ic, *content;
   char buf[PATH_MAX];
   Elm_Object_Item *it;

   ic = elm_icon_add(nf);
   elm_icon_standard_set(ic, "arrow_right");

   bt = elm_button_add(nf);
   evas_object_smart_callback_add(bt, "clicked", _promote, "page5");
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_layout_content_set(bt, "icon", ic);

   content = _content_new(nf, img4);

   it = elm_naviframe_item_push(nf,
                                "Page 4",
                                NULL,
                                bt,
                                content,
                                NULL);
   elm_object_item_part_text_set(it, "subtitle", "Title area visibility test");

   ic = elm_icon_add(nf);
   snprintf(buf, sizeof(buf), "%s",
            img8);
   elm_image_file_set(ic, buf, NULL);
   evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
   elm_object_item_part_content_set(it, "icon", ic);
   elm_naviframe_item_title_visible_set(it, EINA_FALSE);
   evas_object_smart_callback_add(content, "clicked", _title_visible, it);

   evas_object_data_set(nf, "page4", it);
}

static void
_page3(Evas_Object *nf)
{
   Evas_Object *bt2, *ic, *content;
   char buf[PATH_MAX];
   Elm_Object_Item *it;

   bt2 = elm_button_add(nf);
   evas_object_size_hint_align_set(bt2, EVAS_HINT_FILL, EVAS_HINT_FILL);
   BUTTON_TEXT_SET(bt2, "Next");
   evas_object_smart_callback_add(bt2, "clicked", _promote, "page4");

   content = _content_new(nf, img3);

   it = elm_naviframe_item_push(nf,
                                "Page 3",
                                NULL,
                                bt2,
                                content,
                                NULL);
   ic = elm_icon_add(nf);
   snprintf(buf, sizeof(buf), "%s", img8);
   elm_image_file_set(ic, buf, NULL);
   evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);

   elm_object_item_part_content_set(it, "icon", ic);

   evas_object_data_set(nf, "page3", it);
}

static void
_page2(Evas_Object *nf)
{
   Evas_Object *bt, *content, *ic;
   Elm_Object_Item *it;

   bt = elm_button_add(nf);
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_smart_callback_add(bt, "clicked", _promote, "page3");

   ic = elm_icon_add(nf);
   elm_icon_standard_set(ic, "arrow_right");
   evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
   elm_layout_content_set(bt, "icon", ic);

   content = _content_new(nf, img2);

   it = elm_naviframe_item_push(nf, "Page 2 - Long Title Here",
                                NULL, bt, content,  NULL);
   elm_object_item_part_text_set(it, "subtitle", "Here is sub-title part!");
   evas_object_data_set(nf, "page2", it);

}

static void
_page1(Evas_Object *nf)
{
   Evas_Object *btn, *content;
   Elm_Object_Item *it;

   btn = elm_button_add(nf);
   evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_smart_callback_add(btn, "clicked", _promote, "page2");
   BUTTON_TEXT_SET(btn, "Next");
   evas_object_show(btn);

   content = _content_new(nf, img1);
   it = elm_naviframe_item_push(nf, "Page 1", NULL, btn, content, NULL);
   evas_object_data_set(nf, "page1", it);
}


static void
_test_naviframe_page(AtkObject *obj, int index)
{
   int child_count = atk_object_get_n_accessible_children(obj);
   printf("n_accessible_children done\n");
   g_assert(child_count > 0);
   AtkStateSet *state_set = atk_object_ref_state_set(obj);
   /*g_assert(atk_state_set_is_empty == TRUE);*/
   if (index == 4)
     {
        AtkStateType states[3] = {ATK_STATE_SHOWING, ATK_STATE_VISIBLE,
             ATK_STATE_ENABLED};
        /*we are showing last page*/
        gboolean contains = atk_state_set_contains_states(state_set, states, 3);
        g_assert(contains);
     }
   g_object_unref(state_set);
   printf("ref_state_set done\n");
   int index_in_parent = atk_object_get_index_in_parent(obj);
   g_assert(index_in_parent == index);

   /*check if last child is naviframe page content*/
   AtkObject *child = atk_object_ref_accessible_child(obj, child_count - 1);
   g_assert(eailu_is_object_with_type(child, "EailPhoto"));
   g_object_unref(child);
   printf("ref_accessible_child done\n");
   AtkObject *parent = atk_object_get_parent(obj);
   g_object_ref(parent);
   g_assert(ATK_IS_OBJECT(parent));
   g_assert(eailu_is_object_with_type(parent, "EailNaviframe"));
   g_object_unref(parent);
   printf("get_parent done\n");

   int x = -1;
   int y = -1;
   int width = -1;
   int height = -1;

   atk_component_get_extents(ATK_COMPONENT(obj),
                             &x, &y,
                             &width, &height,
                             ATK_XY_SCREEN);
   g_assert(x > 0);
   g_assert(y > 0);
   g_assert(height > 0);
   g_assert(width > 0);

}

static void
_test_naviframe_object(AtkObject *obj)
{
   AtkAction *action;
   const char *action_name, *desc;
   gboolean result;
   int actions_n, child_count = 0;

   eail_title_clicked_cb_called = EINA_FALSE;

   g_object_ref(obj);
   child_count = atk_object_get_n_accessible_children(obj);
   g_assert(child_count == 5);

   for (int i = 0; i < child_count; i++)
     {
        const char *name = NULL;
        AtkObject *child = atk_object_ref_accessible_child(obj, i);
        g_assert(ATK_IS_OBJECT(child));
        g_assert(eailu_is_object_with_type(child, "EailNaviframePage"));
        name = atk_object_get_name(child);
        g_assert(name != NULL);
        printf("child name %s  - done\n", name);
        g_assert(ATK_ROLE_PAGE_TAB == atk_object_get_role(child));
        _test_naviframe_page(child, i);
        g_object_unref(child);
     }

   /*test AtkSelection*/
   g_assert(ATK_IS_SELECTION(obj));
   int selection_count = 0;
   selection_count = atk_selection_get_selection_count(ATK_SELECTION(obj));
   printf("atk_selection_count: %d done\n", selection_count);
   g_assert(selection_count == 1);
   gboolean selected = atk_selection_is_child_selected(ATK_SELECTION(obj), 4);
   g_assert(selected == TRUE);
   selected = atk_selection_is_child_selected(ATK_SELECTION(obj), 1);
   g_assert(selected != TRUE);
   g_assert(atk_selection_add_selection(ATK_SELECTION(obj), 0) == TRUE);
   AtkObject *selection = atk_selection_ref_selection(ATK_SELECTION(obj), 0);
   int index_in_parent = atk_object_get_index_in_parent(selection);
   printf("Index in parent: %d done\n", index_in_parent);
   g_assert(index_in_parent == 0);
   g_assert(eailu_is_object_with_type(selection, "EailNaviframePage"));

   /* TODO: test properly focus here, probably automatically focus
    * is changed to internal object */
   /* eailu_test_atk_focus(obj, TRUE); */

   /*test AtkActionIface*/

   g_assert(ATK_IS_ACTION(obj));
   action = ATK_ACTION(obj);
   actions_n = atk_action_get_n_actions(action);
   printf("action count %d\n", atk_action_get_n_actions(action));
   g_assert(actions_n == 1);
   action_name = atk_action_get_name(action, 0);
   g_assert(!strcmp("click_title", action_name));
   result = atk_action_set_description(action, 0, "click_title description");
   g_assert(result);
   desc = atk_action_get_description(action, 0);
   g_assert(!strcmp(desc, "click_title description"));
   result = atk_action_do_action(action, 0);
   g_assert(result);
   g_assert(eail_title_clicked_cb_called);

   g_object_unref(selection);
   g_object_unref(obj);
}

static void
_on_done(void *data, Evas_Object *obj, void *event_info)
{
   printf("EailNaviframe Test PASSED\n");
   elm_exit();
}

static void
_on_focus_in(void *data, Evas_Object *obj, void *event_info)
{
   AtkObject *aobj = atk_get_root();
   g_assert(aobj);

   eailu_traverse_children_for_type_call_cb
                              (aobj, "EailNaviframe", _test_naviframe_object);

   _on_done(NULL, obj, NULL);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;
   win = eailu_create_test_window_with_glib_init(NULL, _on_focus_in);
   nf = elm_naviframe_add(win);
   evas_object_size_hint_weight_set(nf, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, nf);
   evas_object_show(nf);
   evas_object_smart_callback_add(nf, "title,clicked", _title_clicked, 0);

   _page1(nf);
   _page2(nf);
   _page3(nf);
   _page4(nf);
   _page5(nf);

   evas_object_resize(win, 800, 600);
   evas_object_show(win);

   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
