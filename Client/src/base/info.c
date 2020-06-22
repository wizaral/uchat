#include "client.h"

static t_windows *create_windows() {
    t_windows *new = malloc(sizeof(t_windows));
    
    if (new) {
        new->log = NULL;
        new->reg = NULL;
        new->chat_switcher = NULL;
        new->main_menu = NULL;
        new->profile = NULL;
        new->contacts = NULL;
        new->exit = NULL;
        new->room_creation = NULL;
    }
    return new;
}

t_info *mx_create_info(GtkApplication *app) {
    t_info *new = malloc(sizeof(t_info));
    GdkDisplay *display;
    GdkScreen *screen;

    new->exit = 0;
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);
    new->timer = NULL;
    new->json = NULL;
    new->main_window = mx_create_main_window(app);
    new->windows = create_windows();
    new->layout = mx_layout_constructor("main_layout", WIDTH, HEIGHT);
    new->chat_list = mx_create_list();
    gtk_container_add(GTK_CONTAINER(new->main_window), new->layout);
    new->css = gtk_css_provider_new();
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(new->css), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_widget_show_all(new->main_window);
    return new;
}
