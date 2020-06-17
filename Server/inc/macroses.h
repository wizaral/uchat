#pragma once
#define MX_INE " IF NOT EXISTS "
#define MX_CREATE(NAME) "CREATE TABLE" MX_INE #NAME " ("
#define MX_INT_PK(NAME) #NAME " INTEGER PRIMARY KEY AUTOINCREMENT,"
#define MX_TRIGGER(NAME, ACT) "CREATE TRIGGER" MX_INE #NAME" AFTER " #ACT" ON "
#define MX_INSERT(TABLE, ...) "INSERT INTO " #TABLE" (" #__VA_ARGS__") VALUES "
#define MX_DEL_WH(TABLE, COND) "DELETE FROM " #TABLE " WHERE " #COND ";"
