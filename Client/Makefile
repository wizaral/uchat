CC = clang
LIB = libmx.a
NAME = uchat

base = main socket info close \
	client_receive_message_handler client_send_message_handler \
	receive_file send_file

handlers = read_non_requested_jsons handle_del_msg handle_edit_msg            \
	handle_send_msg handle_invite_user handle_being_invited handle_leave_room \
	handle_ban handle_unban handle_ulogin handle_ulogout handle_ucreate       \
	handle_send_msg_img

login = login validate callbacks json

register = register validate json

admin = admin json callbacks

chat = chat interface callbacks get_json_chat_list_login callbacks_2          \
	json_send_msg_1 get_json_history json_left_chat json_ban                  \
	get_users_from_chat_to_list preferences json_unban callbacks_3

ban = callbacks ban

unban = callbacks unban

invite = callbacks invite json

message = message callbacks interface edit_json delete_json

message_img = message_img callbacks interface json

room_creation = room_creation validate callbacks json

change_pass = change_pass callbacks validate

main_chat_screen = main_screen

contacts = contacts callbacks json create_table get_json_list_contacts \
	save_json_groups del_cnt_json start_chat_json create_menus callbacks_2

cont_move = cont_move move_cnt callbacks

profile = profile json callbacks

other_profile = other_profile

delete_profile = delete_profile callbacks json

preferences = preferences callbacks chg_pass_json

add_contact = add_contact callbacks add_json_user_to_contacts add_group

create_group = create_group callbacks delete_group_json

utils = css wait_for_json create_hash json_get_type dialog entry toggle       \
	notify find_something player validate_chars gobject_builder get_image     \

dirs = base login register admin add_contact chat create_group main_chat_screen \
	profile preferences contacts message message_img room_creation utils      \
	change_pass handlers delete_profile other_profile ban invite unban        \
	cont_move

FILES = $(foreach dir, $(dirs), $($(dir):%=$(dir)/%))

SRC_DIR = src/
OBJ_DIR = obj/
LIB_DIR = ../libmx/

SRC = $(FILES:%=$(SRC_DIR)%.c)
OBJ = $(FILES:%=$(OBJ_DIR)%.o)
LIB_PATH = $(LIB_DIR)$(LIB)

WFLAGS = -Wall -Wextra -Werror -Wpedantic -Wno-unused-command-line-argument \
	-Wno-unused-variable -Wno-unused-function -Wno-unused-parameter

LFLAGS = -Iinc -I$(LIB_DIR)inc -I/usr/local/opt/openssl/include \
	-L/usr/local/opt/openssl/lib -lssl -lcrypto -L. -lfmodex

CFLAGS = -Ofast -march=native -flto -rdynamic

PKGCONFIG = `pkg-config --libs --cflags gtk+-3.0`
PGKCONFIGFLAGS = `pkg-config --cflags gtk+-3.0`

COMPILE = $(CC) -std=c11 -pipe $(WFLAGS) $(LFLAGS)
EXEC_IT = make -sf Makefile
EXEC_LD = $(EXEC_IT) -C $(LIB_DIR)
PRINT = printf
MKDIR = mkdir -p
RM = /bin/rm -rf
TARGET = release

R = \033[1;91m
G = \033[1;92m
Y = \033[1;93m
B = \033[1;94m
M = \033[1;95m
C = \033[1;96m
S = \033[38;5;45;1m
D = \033[0m
F = \033[5m
A = \033[A
K = \033[K

all: $(LIB) $(NAME)

$(OBJ_DIR):
	@$(MKDIR) $@ $(foreach dir, $(dirs), $@/$(dir))

$(LIB):
	@$(EXEC_LD)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@$(PRINT) "$K$G COMPILING $Y[$M$(TARGET)$Y] $B$(<:$(SRC_DIR)%=%)$D\r"
	@$(COMPILE) $(CFLAGS) $(PGKCONFIGFLAGS) -o $@ -c $<

$(NAME): $(OBJ_DIR) $(OBJ)
	@$(PRINT) "$K$G COMPILING $Y[$M$(TARGET)$Y] $R$(NAME)$D\r"
	@$(COMPILE) $(CFLAGS) $(PKGCONFIG) $(LIB_PATH) $(OBJ) -o $(NAME)
	@$(PRINT) "$K"

clean:
	@$(EXEC_LD) $@
	@$(RM) $(OBJ_DIR)

uninstall:
	@$(EXEC_LD) $@
	@$(RM) $(OBJ_DIR) $(NAME)

reinstall: uninstall all

.PHONY: all clean uninstall reinstall
