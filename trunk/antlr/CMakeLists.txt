INCLUDE (CheckIncludeFiles)
INCLUDE (CheckFunctionExists)

CHECK_FUNCTION_EXISTS (accept HAVE_ACCEPT)
CHECK_INCLUDE_FILES (arpa/nameser.h HAVE_ARPA_NAMESER_H)
CHECK_INCLUDE_FILES (ctype.h HAVE_CTYPE_H)
CHECK_INCLUDE_FILES (dlfcn.h HAVE_DLFCN_H)
CHECK_INCLUDE_FILES (intptr_t "inttypes.h" HAVE_INTPTR_T)
CHECK_INCLUDE_FILES (inttypes.h HAVE_INTTYPES_H)
CHECK_INCLUDE_FILES (malloc.h HAVE_MALLOC_H)
CHECK_FUNCTION_EXISTS (memmove HAVE_MEMMOVE)
CHECK_INCLUDE_FILES (memory.h HAVE_MEMORY_H)
CHECK_FUNCTION_EXISTS (memset HAVE_MEMSET)
CHECK_INCLUDE_FILES (netdb.h HAVE_NETDB_H)
CHECK_INCLUDE_FILES (netinet/in.h HAVE_NETINET_IN_H)
CHECK_INCLUDE_FILES (netinet/tcp.h HAVE_NETINET_TCP_H)
CHECK_INCLUDE_FILES (resolv.h HAVE_RESOLV_H)
CHECK_INCLUDE_FILES (socket.h HAVE_SOCKET_H)
CHECK_INCLUDE_FILES (stdarg.h HAVE_STDARG_H)
CHECK_INCLUDE_FILES (stdint.h HAVE_STDINT_H)
CHECK_INCLUDE_FILES (stdlib.h HAVE_STDLIB_H)
CHECK_FUNCTION_EXISTS (strdup HAVE_STRDUP)
CHECK_INCLUDE_FILES (strings.h HAVE_STRINGS_H)
CHECK_INCLUDE_FILES (string.h HAVE_STRING_H)
CHECK_INCLUDE_FILES (sys/malloc.h HAVE_SYS_MALLOC_H)
CHECK_INCLUDE_FILES (sys/socket.h HAVE_SYS_SOCKET_H)
CHECK_INCLUDE_FILES (sys/stat.h HAVE_SYS_STAT_H)
CHECK_INCLUDE_FILES (sys/types.h HAVE_SYS_TYPES_H)
CHECK_INCLUDE_FILES (uintptr_t "inttypes.h" HAVE_UINTPTR_T)
CHECK_INCLUDE_FILES (unistd.h HAVE_UNISTD_H)


CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/antlr3config.h.in ${CMAKE_CURRENT_BINARY_DIR}/antlr3config.h)

add_library(antlr 
 src/antlr3baserecognizer.c
 src/antlr3basetree.c
 src/antlr3basetreeadaptor.c
 src/antlr3bitset.c
 src/antlr3collections.c
 src/antlr3commontoken.c
 src/antlr3commontree.c
 src/antlr3commontreeadaptor.c
 src/antlr3commontreenodestream.c
 src/antlr3convertutf.c
 src/antlr3cyclicdfa.c
 src/antlr3debughandlers.c
 src/antlr3encodings.c
 src/antlr3exception.c
 src/antlr3filestream.c
 src/antlr3inputstream.c
 src/antlr3intstream.c
 src/antlr3lexer.c
 src/antlr3parser.c
 src/antlr3rewritestreams.c
 src/antlr3string.c
 src/antlr3stringstream.c
 src/antlr3tokenstream.c
 src/antlr3treeparser.c
 src/antlr3ucs2inputstream.c)

