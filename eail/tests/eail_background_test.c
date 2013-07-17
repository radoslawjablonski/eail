#include <Elementary.h>
#include <Ecore_X.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define TEST_STR "bg-test"
#define IMG_PATH "./data/08.jpg"
#define X 50
#define Y 60
#define WIDTH 320
#define HEIGHT 280

static void _do_test(AtkObject *obj);
static void _init_background(Evas_Object *win);
static void _on_done(void *data, Evas_Object *obj, void *event_info);
static void _on_focus_in(void *data, Evas_Object *obj, void *event_info);

static void
_do_test(AtkObject *obj)
{
   const char *type_name = g_type_name(G_TYPE_FROM_INSTANCE(obj));
   const char *desc;
   int w, h;
   AtkAttributeSet *attr_set, *node;
   AtkAttribute *p;

   g_assert_cmpstr(type_name, ==, "EailBackground");

   /* AtkImage tests */
   g_assert(ATK_IS_IMAGE(obj));

   g_assert(atk_image_set_image_description(ATK_IMAGE(obj), TEST_STR));
   desc = atk_image_get_image_description(ATK_IMAGE(obj));
   g_assert(!g_strcmp0(desc, TEST_STR));

   atk_image_get_image_size(ATK_IMAGE(obj), &w, &h);
   g_assert(w == WIDTH);
   g_assert(h == HEIGHT);

   atk_image_get_image_position(ATK_IMAGE(obj), &w, &h, ATK_XY_WINDOW);
   g_assert(w == 0);
   g_assert(h == 0);

   /* Others */
   attr_set = atk_object_get_attributes(obj);
   g_assert(g_slist_length(attr_set) == 3);

   node = g_slist_nth(attr_set, 0);
   p = (AtkAttribute *)node->data;
   g_assert_cmpstr(p->name, ==, "type");
   g_assert_cmpstr(p->value, ==, "elm_bg");

   node = g_slist_next(node);
   p = (AtkAttribute *)node->data;
   g_assert_cmpstr(p->name, ==, "file");
   g_assert_cmpstr(p->value, ==, IMG_PATH);

   node = g_slist_next(node);
   p = (AtkAttribute *)node->data;
   g_assert_cmpstr(p->name, ==, "display mode");
   g_assert_cmpstr(p->value, ==, "stretch");

   g_slist_free(attr_set);
}

static void
_init_background(Evas_Object *win)
{
   Evas_Object *bg;
   char buf[PATH_MAX];

   bg = elm_bg_add(win);
   elm_bg_load_size_set(bg, 20, 20);
   elm_bg_option_set(bg, ELM_BG_OPTION_STRETCH);
   snprintf(buf, sizeof(buf), IMG_PATH);
   elm_bg_file_set(bg, buf, NULL);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);
   evas_object_resize(win, WIDTH, HEIGHT);
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

   eailu_traverse_children_for_role_call_cb(aobj, ATK_ROLE_IMAGE, _do_test);

   _on_done(NULL, obj, NULL);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_background(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
