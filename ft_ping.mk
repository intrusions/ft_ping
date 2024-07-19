NAME         := ft_ping
CC           := gcc
SRCS_DIR     := srcs
OBJS_DIR     := .objs
INCS_DIR     := incs
MAIN         := srcs/main.c

CFLAGS :=                           \
    -Wall                           \
    -Wextra                         \
    -Werror                         \
    -pedantic                       \
    -lm

# export ASAN_OPTIONS="log_path=sanitizer.log"
# export ASAN_OPTIONS="detect_leaks=1"

CFLAGS_DBG :=                       \
    -g3                             \
    -O0                             \
    -fsanitize=address              \
    -fsanitize=undefined            \
    -fno-omit-frame-pointer         \
    -fstack-protector-strong        \
    -fno-optimize-sibling-calls 

SRCS :=                             \
    core/reverse_dns.c              \
    core/prepare_packet.c           \
    core/send_packet.c              \
    core/recv_packet.c              \
    utils/print.c                   \
    utils/manage_flags.c            \
    utils/checksum.c                \
    utils/calcul.c                  \
    utils/arr.c                     \
    utils/close.c                   \
    debug/debug_print.c