


#define HAVE_ENDIAN_H 1
#define HAVE_FCNTL_H 1
#define HAVE_SCHED_H 1
#define HAVE_UNISTD_H 1
#define HAVE_SYS_PARAM_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_TIME_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_STDINT_H 1

#define HAVE_CLOSE 1
#define HAVE_GETPID 1
#define HAVE_GETTIMEOFDAY 1
#define HAVE_OPEN 1
#define HAVE_READ 1
#define HAVE_SCHED_YIELD 1

#define HAVE_SYNC_BUILTINS 1
#define HAVE_ATOMIC_BUILTINS 1

#define HAVE_LOCALE_H 1
#define HAVE_SETLOCALE 1

#define HAVE_INT32_T 1
#ifndef HAVE_INT32_T
#  define int32_t JSON_INT32
#endif

#define HAVE_UINT32_T 1
#ifndef HAVE_UINT32_T
#  define uint32_t JSON_UINT32
#endif

#define HAVE_UINT16_T 1
#ifndef HAVE_UINT16_T
#  define uint16_t JSON_UINT16
#endif

#define HAVE_UINT8_T 1
#ifndef HAVE_UINT8_T
#  define uint8_t JSON_UINT8
#endif

#define HAVE_SSIZE_T 1

#ifndef HAVE_SSIZE_T
#  define ssize_t JSON_SSIZE
#endif

#define USE_URANDOM 1
#define USE_WINDOWS_CRYPTOAPI 1

#define INITIAL_HASHTABLE_ORDER JANSSON_INITIAL_HASHTABLE_ORDER
