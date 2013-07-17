
#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define EAIL_TYPE_FOR_GENLIST "EailGenlist"
#define EAIL_TYPE_FOR_LIST_ITEM "EailItem"
#define N_ITEMS 18

/* used to determine if test code has been successfully called
 * (sometimes may happen that ATK object to test in hierarchy cannot be found)*/
static gboolean eail_test_code_called = FALSE;
static Evas_Object *glob_genlist = NULL;
static Elm_Genlist_Item_Class *_itc_group = NULL;
static int nitems = 0;

Elm_Object_Item *glob_test_item = NULL;

static void
on_done(void *data, Evas_Object *obj, void *event_info)
{
   /* quit the mainloop (elm_run) */
   elm_exit();
}

void
_test_list_item_interfaces(AtkObject *item_object)
{
   g_assert(ATK_IS_OBJECT(item_object));
   g_assert(eailu_is_object_with_type(item_object, EAIL_TYPE_FOR_LIST_ITEM));

   /**
    * for single list items focusing does NOT work
    */
   eailu_test_atk_focus(item_object, FALSE);

   g_assert(ATK_IS_ACTION(item_object));
}

void
_test_item_selection_single(AtkObject *parent, AtkObject *child, int index)
{
   g_assert(ATK_IS_SELECTION(parent));
   /* in single selection mode it should return FALSE */
   g_assert(!atk_selection_select_all_selection(ATK_SELECTION(parent)));
   g_assert
      (FALSE == atk_selection_is_child_selected(ATK_SELECTION(parent), index));

   g_assert(atk_selection_add_selection(ATK_SELECTION(parent), index));
   g_assert(atk_selection_is_child_selected(ATK_SELECTION(parent), index));
   g_assert(1 == atk_selection_get_selection_count(ATK_SELECTION(parent)));

   /* now removing selection using atk_selection interface */
   g_assert(atk_selection_remove_selection(ATK_SELECTION(parent),index));
   g_assert
     (FALSE == atk_selection_is_child_selected(ATK_SELECTION(parent), index));
   g_assert(0 == atk_selection_get_selection_count(ATK_SELECTION(parent)));
}

void
_test_multiselection_list(AtkObject *atk_list)
{
   elm_genlist_multi_select_set(glob_genlist, EINA_TRUE);
   g_assert(atk_selection_select_all_selection(ATK_SELECTION(atk_list)));

   /* all children should be now selected */
   g_assert
      (N_ITEMS == atk_selection_get_selection_count(ATK_SELECTION(atk_list)));

   /* now clearing selection */
   g_assert(atk_selection_clear_selection(ATK_SELECTION(atk_list)));
   g_assert(0 == atk_selection_get_selection_count(ATK_SELECTION(atk_list)));
}

static void
_test_scrolling_actions(AtkObject *aobj)
{
   int n_actions = 0;
   const gchar *test_desc = "test description for scroll up";
   const gchar *result_desc;

   g_assert(ATK_IS_ACTION(aobj));
   n_actions = atk_action_get_n_actions(ATK_ACTION(aobj));
   g_assert(4 == n_actions);

   /* scroll up*/
   eailu_test_action_activate(ATK_ACTION(aobj), "scroll_up");

   /* scroll_down */
   eailu_test_action_activate(ATK_ACTION(aobj), "scroll_down");

   /* scroll_left */
   eailu_test_action_activate(ATK_ACTION(aobj), "scroll_left");

   /* scroll_right*/
   eailu_test_action_activate(ATK_ACTION(aobj), "scroll_right");

   /* description test */
   g_assert(!atk_action_get_description(ATK_ACTION(aobj), 0));
   g_assert(atk_action_set_description(ATK_ACTION(aobj), 0, test_desc));

   result_desc = atk_action_get_description(ATK_ACTION(aobj), 0);
   g_assert(0 == g_strcmp0(result_desc, test_desc));
}

void
_test_list_atk_obj(AtkObject *aobj)
{
   int child_amount = 0, i = 0;

   printf("TESTING Genlist implementation....\n");

   child_amount = atk_object_get_n_accessible_children(aobj);
   g_assert(N_ITEMS == child_amount);

   _test_scrolling_actions(aobj);

   /* For list widget focusing work */
   eailu_test_atk_focus(aobj, TRUE);

   /* checking list items implementation */
   for (i = 0; i < child_amount; i++)
     {
        AtkObject *child = atk_object_ref_accessible_child(aobj, i);
        /* each list item has inside 2 text entry objects */
        //g_assert(2 == atk_object_get_n_accessible_children(child));

        _test_list_item_interfaces(child);
        _test_item_selection_single(aobj, child, i);

        g_object_unref(child);
     }

   /* now quick check if multiselection on list works properly*/
   _test_multiselection_list(aobj);
   eail_test_code_called = TRUE;
   printf("DONE. All test passed successfully \n");
}

static void on_focus_in(void *data, Evas_Object *obj, void *event_info)
{
   AtkObject *aobj = atk_get_root();
   g_assert(aobj);

   eailu_traverse_children_for_type_call_cb
                          (aobj, EAIL_TYPE_FOR_GENLIST, _test_list_atk_obj);


   g_assert(eail_test_code_called);

   /* force exit on end of test */
   on_done(NULL, NULL, NULL);
}

static char *
_item_label_get(void *data, Evas_Object *obj, const char *part)
{
   time_t t = (time_t)ecore_time_unix_get();
   char buf[256];
   int i = (int)(long)data;

   if (!strcmp(part, "elm.text"))
     snprintf(buf, sizeof(buf), "Item # %i", i);
   else
     {
        int n;
        snprintf(buf, sizeof(buf), "realized at %s", ctime(&t));
        n = strlen(buf);
        buf[n - 1] = '\0';
     }

   return strdup(buf);
}

static Evas_Object *
_item_content_get(void *data, Evas_Object *obj, const char *part)
{
   Evas_Object *ic = elm_icon_add(obj);

   if (!strcmp(part, "elm.swallow.icon"))
     elm_icon_standard_set(ic, "clock");

   evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
   return ic;
}

static char *
_group_label_get(void *data, Evas_Object *obj, const char *part)
{
   char buf[256];
   int i = (int)(long)data;

   snprintf(buf, sizeof(buf), "Group %d (item #%d)", i / 7, i);

   return strdup(buf);
}

static Evas_Object *
_group_content_get(void *data, Evas_Object *obj, const char *part)
{
   Evas_Object *ic = elm_icon_add(obj);

   if (!strcmp(part, "elm.swallow.icon"))
     elm_icon_standard_set(ic, "home");

   evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
   return ic;
}

static void
_item_sel_cb(void *data, Evas_Object *obj, void *event_info)
{
//   printf("sel item data [%p] on genlist obj [%p], item pointer [%p]\n",
//          data, obj, event_info);
}

void
initialize_genlist(Evas_Object *win)
{
   Evas_Object *box, *entry;
   static Elm_Genlist_Item_Class *_itc = NULL;
   int i = 0;

   if (!_itc)
     {
        _itc = elm_genlist_item_class_new();
        _itc->item_style = "default";
        _itc->func.text_get = _item_label_get;
        _itc->func.content_get = _item_content_get;
        _itc->func.state_get = NULL;
        _itc->func.del = NULL;
     }

   if (!_itc_group)
     {
        _itc_group = elm_genlist_item_class_new();
        _itc_group->item_style = "group_index";
        _itc_group->func.text_get = _group_label_get;
        _itc_group->func.content_get = _group_content_get;
        _itc_group->func.state_get = NULL;
        _itc_group->func.del = NULL;
     }

   box = elm_box_add(win);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, box);
   evas_object_show(box);

   glob_genlist = elm_genlist_add(win);

   evas_object_size_hint_weight_set
                           (glob_genlist, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set
                                (glob_genlist, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(box, glob_genlist);
   evas_object_show(glob_genlist);

   entry = elm_entry_add(win);
   elm_entry_single_line_set(entry, EINA_TRUE);
   evas_object_size_hint_weight_set(entry, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(entry, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(entry);


   /* filling genlist */
   for (i = 0; i < N_ITEMS; i++)
     {
        Elm_Object_Item *gli = NULL, *glg = NULL;

         if (i % 7 == 0)
           {
               glg = gli = elm_genlist_item_append(glob_genlist, _itc_group,
                                                   (void *) (long) nitems++,
                                                   NULL, ELM_GENLIST_ITEM_GROUP,
                                                   _item_sel_cb, NULL );
               elm_genlist_item_select_mode_set(
                        gli, ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY);
           }
         else
           {
               gli = elm_genlist_item_append(glob_genlist, _itc,
                                             (void *) (long) nitems++, glg,
                                             ELM_GENLIST_ITEM_NONE,
                                             _item_sel_cb, NULL );
           }

         if (i == 1)
           glob_test_item = gli;
     }

}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(on_done, on_focus_in);
   initialize_genlist(win);

   /* and show the window */
   evas_object_show(win);

   elm_run();
   elm_shutdown();

   /* exit code */
   return 0;
}
ELM_MAIN()
