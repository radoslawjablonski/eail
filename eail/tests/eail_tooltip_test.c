
#include <Elementary.h>
#include <atk/atk.h>

#define WINDOW_TEST_TITLE "Tolltip test"
static void _traverse_children(AtkObject *obj);


static void on_focus_in(void *data, Evas_Object *obj, void *event_info)
{
   AtkObject *aobj = atk_get_root();
   if (aobj)
     _traverse_children(aobj);
   else
     printf("error: ATK returns null\n");

   elm_exit();
}

static void
_print_object_info(AtkObject *obj)
{
   const char *name = atk_object_get_name(obj);
   const char *type_name = g_type_name(G_TYPE_FROM_INSTANCE(obj));
   AtkRole atk_role = atk_object_get_role(obj);
   const char *role_name = atk_role_get_name(atk_role);

   printf("atk_object_get_name: %s\n", name?name:"NULL");
   printf("atk_object_get_role: %s\n", role_name?role_name:"NULL");
   printf("atk_object_get_type_name: %s\n", type_name ? type_name:"NULL");
}

static void
_traverse_children(AtkObject *obj)
{
   const char *name = atk_object_get_name(obj);
   printf("Traversing on child objects of: %s\n", name?name:"NULL");

   int child_count = atk_object_get_n_accessible_children(obj);
   int i;
   for (i=0; i < child_count; i++)
    {

       AtkObject *child = atk_object_ref_accessible_child(obj, i);
       if (ATK_IS_OBJECT(child))
         {

            _print_object_info(child);
         }
       _traverse_children(child);

       g_object_unref(child);
     }
}

void
initialize_and_show_tooltip(Evas_Object *win)
{
   const char *text = "test text";
   elm_object_tooltip_text_set(win, text);
   elm_object_tooltip_show(win);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   if (!ecore_main_loop_glib_integrate())
    {
       printf("Cannot integrate with glib main loop");
       return -1;
    }

#if !GLIB_CHECK_VERSION(2,35,0)
   g_type_init();
#endif

   Evas_Object *win;

   win = elm_win_add(NULL, WINDOW_TEST_TITLE, ELM_WIN_BASIC);
   elm_win_title_set(win, WINDOW_TEST_TITLE);
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   elm_win_autodel_set(win, EINA_TRUE );

   evas_object_smart_callback_add(win, "focus,in", on_focus_in, NULL);
   initialize_and_show_tooltip(win);

   evas_object_resize(win, 320, 320);
   /* and show the window */
   evas_object_show(win);

   elm_run();
   elm_shutdown();

   /* exit code */
   return 0;
}
ELM_MAIN()
