#include "server.h"

static int get_id(void *data, int argc, char **argv, char **cols) {
    int *cli = data;

    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (cli[i] == 0) {
            cli[i] = atoi(argv[0]);
            break;
        }
    }
    return 0;
}

static void get_arr_users(sqlite3 *db, cJSON *jsn, int *uid) {
    int count = 0;
    int *new_uid;
    int i;

    for (i = 0; uid[i] != 0; ++i)
        count++;
    new_uid = malloc(count * sizeof(int));
    for (i = 0; i < count; ++i)
        new_uid[i] = uid[i];

    cJSON_AddItemToObject(jsn, "clients_id", cJSON_CreateIntArray(new_uid, count));

    free(uid);
    free(new_uid);
}

void mx_get_all_users_in_chat(sqlite3 *db, cJSON *jsn, int cid) {
    char *query = NULL;
    char *err = NULL;
    int rc = 0;
    int *uid = malloc(MAX_CLIENTS * sizeof(int));

    bzero(uid, MAX_CLIENTS);
    asprintf(&query, "SELECT user_id FROM users_chats WHERE chat_id = %i "
            "AND role > 0;", cid);

    rc = sqlite3_exec(db, query, get_id, uid, &err);
    if (mx_check(rc, err, "get all users") != SQLITE_OK)
        MX_SET_TYPE(jsn, failed_send_message);
    else
        get_arr_users(db, jsn, uid);
    free(query);
}

static int get_mid(void *data, int argc, char **argv, char **cols) {
    cJSON_AddStringToObject(data, "login", argv[0]);
    cJSON_AddNumberToObject(data, "mid", atoi(argv[1]));
    cJSON_AddStringToObject(data, "time", argv[2]);
    cJSON_AddNumberToObject(data, "type", atoi(argv[3]));

    return 0;
}

void mx_insert_message_in_db(sqlite3 *db, cJSON *jsn) {
    char *query = NULL;
    char *err = NULL;
    int rc = 0;

    asprintf(&query, "INSERT INTO messages VALUES (NULL, %i, %i, %i, "
            "datetime('now', 'localtime'), '%s'); "
            "SELECT u.login, m.id, m.send_time, m.type "
            "FROM messages AS m JOIN users AS u ON u.id = m.user_id "
            "AND m.id = last_insert_rowid();",
            MX_VINT(jsn, "uid"), MX_VINT(jsn, "cid"),
            MX_VINT(jsn, "type"), MX_VSTR(jsn, "content"));
    rc = sqlite3_exec(db, query, get_mid, jsn, &err);

    if (mx_check(rc, err, "send message") != SQLITE_OK)
        MX_SET_TYPE(jsn, failed_send_message);
    else
        mx_get_all_users_in_chat(db, jsn, MX_VINT(jsn, "cid"));
    free(query);
}
