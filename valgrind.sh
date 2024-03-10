#!/usr/bin/env bash
valgrind --suppressions=leaks_ignore.supp --leak-check=full --show-leak-kinds=all --track-origins=yes ./minishell
