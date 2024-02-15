# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/18 12:00:50 by ladloff           #+#    #+#              #
#    Updated: 2024/02/15 12:13:07 by ladloff          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Shell used for command execution
SHELL		:= /bin/sh

# Name of the executable
NAME		:= minishell

# Directories
SRC_DIR		:= ./src
BUILD_DIR	:= ./build
INCLUDE_DIR	:= ./include
LIBFT_DIR	:= ./lib/libft

# Source, object, dependency files
SRC_FILES	:=	builtins/ft_cd.c \
				builtins/ft_echo.c \
				builtins/ft_env.c \
				builtins/ft_exit.c \
				builtins/ft_export.c \
				builtins/ft_pwd.c \
				builtins/ft_unset.c \
				environment/env_utils.c \
				environment/env.c \
				execution/argument_creation.c \
				execution/argument_splitting.c \
				execution/builtin_utils.c \
				execution/builtin.c \
				execution/execution_utils.c \
				execution/execution.c \
				execution/expansion_utils.c \
				execution/expansion.c \
				execution/redirection.c \
				execution/heredoc_utils.c \
				execution/heredoc.c \
				lexer/lexer_utils.c \
				lexer/lexer_utils2.c \
				lexer/lexer_utils3.c \
				lexer/lexer_utils4.c \
				lexer/lexer.c \
				lexer/quote_handling.c \
				signals/handlers.c \
				signals/signals.c \
				cleanup.c \
				exit.c \
				main.c \

OBJ_FILES	:= $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRC_FILES))
DEP_FILES	:= $(OBJ_FILES:.o=.d)

# Compiler flags
CFLAGS		:= -Wall -Wextra -Werror -MMD -MP -g3
CPPFLAGS	:= -I$(INCLUDE_DIR) -I$(LIBFT_DIR)/include

# Linker flags
LDFLAGS		:= -L$(LIBFT_DIR)
LDLIBS		:= -lft

# Check if pkg-config is available
PKG_CONFIG	:= $(shell command -v pkg-config 2> /dev/null)

ifeq ($(PKG_CONFIG),)
# pkg-config is not installed, use default paths
	ifeq ($(shell uname -s),Darwin)
		ifeq ($(shell uname -m),arm64)
			CPPFLAGS	+= -I/opt/homebrew/opt/readline/include
			LDFLAGS		+= -L/opt/homebrew/opt/readline/lib
			LDLIBS		+= -lreadline
		else ifeq ($(shell uname -m),x86_64)
			CPPFLAGS	+= -I/usr/local/opt/readline/include
			LDFLAGS		+= -L/usr/local/opt/readline/lib
			LDLIBS		+= -lreadline
		else
			$(error Unsupported macOS processor architecture)
		endif
	else ifeq ($(shell uname -s),Linux)
		CFLAGS	+= -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=600
		LDLIBS	+= -lreadline
	else
		$(error Unsupported operating system)
	endif
else
# pkg-config is installed, use its configuration
	CFLAGS		+= $(shell pkg-config --cflags-only-other readline)
	CPPFLAGS	+= $(shell pkg-config --cflags-only-I readline)
	LDFLAGS		+= $(shell pkg-config --libs-only-L readline)
	LDLIBS		+= $(shell pkg-config --libs-only-l readline)
endif

# Phony targets
.PHONY: all lib clean fclean re cleanlib fcleanlib relib fcleanall

# Default target
all: lib $(NAME)

# Target to build the lib
lib:
	$(MAKE) -C $(LIBFT_DIR)

# Target to create directories automatically, compile src files into obj files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Include generated dependency files for each object file
-include $(DEP_FILES)

# Target to link the executable
$(NAME): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(LDFLAGS) $(LDLIBS) -o $(NAME)

# Target to clean the project by removing the directory build
clean:
	rm -rf $(BUILD_DIR)

# Target to remove the executable
fclean: clean
	$(RM) $(NAME)

# Target to rebuild the project from scratch
re: fclean all

# Target to clean the lib
cleanlib:
	$(MAKE) clean -C $(LIBFT_DIR)

# Target to remove the lib and its objects
fcleanlib:
	$(MAKE) fclean -C $(LIBFT_DIR)

# Target to rebuild the lib
relib: fcleanlib lib

# Target to clean all the project and lib
fcleanall: fclean fcleanlib

# Target to rebuild all the project and lib
reall: fcleanall all
