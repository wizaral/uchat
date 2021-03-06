#include "client.h"

static void set_group_names(t_contact_add *ac) {
    t_list *grp_list = ac->info->cl_data->cont_grp_names;
    char *tmp = NULL;

    for (size_t i = 0; i < grp_list->size; ++i) {
        tmp = ((t_group *)(mx_get_index(grp_list, i)->data))->name;
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(ac->combobox), tmp);
    }
}

static void set_search_completetion(t_contact_add *ac) {
    t_list *tmp = ac->info->cl_data->tmp_users;
    GtkTreeIter iter;
    t_user *name = NULL;

    for (size_t i = 0; i < tmp->size; ++i) {
        name = mx_get_index(tmp, i)->data;
        gtk_tree_store_append(ac->nameslist, &iter, NULL);
        gtk_tree_store_set(ac->nameslist, &iter, 0, name->login, -1);
    }
    gtk_entry_set_completion(GTK_ENTRY(ac->entry), ac->e_comp);
    MX_GSIG_CON(ac->dialog, "delete-event", 
                                G_CALLBACK(mx_add_contact_destroy), ac->info);
    set_group_names(ac);
}

static void build(t_info *info, t_contact_add *ac) {
    info->cl_data->tmp_users = mx_create_list();
    mx_get_list_users_json_wrapper(info);
    ac->builder = gtk_builder_new();
    gtk_builder_add_from_file(ac->builder, MX_GLADE_AC, NULL);
    ac->dialog = mx_gobject_builder(ac->builder, "dialog");
    ac->addbt = mx_gobject_builder(ac->builder, "add");
    ac->cancelbt = mx_gobject_builder(ac->builder, "cancel");
    ac->entry = mx_gobject_builder(ac->builder, "entry");
    ac->combobox = mx_gobject_builder(ac->builder, "combobox");
    ac->nameslist = GTK_TREE_STORE(gtk_builder_get_object(ac->builder,
                                                          "names"));
    ac->e_comp = GTK_ENTRY_COMPLETION(gtk_builder_get_object(ac->builder,
                                                             "e_comp"));

}

void mx_add_contact_build(t_info *info, t_contact_add *ac) {
    if (ac == NULL)
        info->windows->ac = ac = malloc(sizeof(t_contact_add));
    else
        return;

    build(info, ac);
    gtk_builder_connect_signals(ac->builder, ac);
    ac->info = info;
    set_search_completetion(ac);
    gtk_widget_show(ac->dialog);
}

void mx_add_contact_destroy(GtkWidget *widget, GdkEvent *event, 
                                                        gpointer data) {
    t_info *info = data;

    gtk_widget_destroy(info->windows->ac->dialog);
    free(info->windows->ac);
    info->windows->ac = NULL;
}
