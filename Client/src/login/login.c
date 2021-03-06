#include "client.h"

void mx_login_screen_build(t_info *info, t_login *log) {
    if (log == NULL)
        info->windows->log = log = malloc(sizeof(t_login));

    log->builder = gtk_builder_new();
    gtk_builder_add_from_file(log->builder, MX_GLADE_LOGIN, NULL);

    log->password_entry = mx_gobject_builder(log->builder, "password_entry");
    log->username_entry = mx_gobject_builder(log->builder, "username_entry");
    log->window = mx_gobject_builder(log->builder, "login_window");
    gtk_builder_connect_signals(log->builder, info);
    MX_GSIG_CON(log->window, "delete-event", G_CALLBACK(mx_destroy), info);
    mx_css_from_file(info, "./Resources/css/login.css");
    gtk_widget_show(log->window);
}

void mx_login_screen_destroy(t_info *info) {
    info->wchange = 1;
    gtk_widget_destroy(info->windows->log->window);
    free(info->windows->log);
    info->windows->log = NULL;
    info->wchange = 0;
}
