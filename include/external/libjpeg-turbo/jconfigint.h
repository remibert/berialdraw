/* jconfigint.h - generated for berialdraw (libjpeg-turbo 2.1.5.1 + Windows MSVC) */
#define BUILD  "20250524"

#ifdef _MSC_VER
/* MSVC configuration */
#define INLINE  __forceinline
#define THREAD_LOCAL  __declspec(thread)
#define FALLTHROUGH
#define HAVE_INTRIN_H
#undef HAVE_BUILTIN_CTZL
#else
/* GCC/Clang configuration */
#undef inline
#define INLINE  __attribute__((always_inline)) inline
#define THREAD_LOCAL  __thread
#define FALLTHROUGH  __attribute__((fallthrough));
#define HAVE_BUILTIN_CTZL
#endif

#define PACKAGE_NAME  "libjpeg-turbo"
#define VERSION  "2.1.5.1"
#ifdef __LP64__
#define SIZEOF_SIZE_T  8
#else
#define SIZEOF_SIZE_T  4
#endif

#if defined(__has_attribute) && !defined(_MSC_VER)
#if __has_attribute(fallthrough)
#undef FALLTHROUGH
#define FALLTHROUGH  __attribute__((fallthrough));
#endif
#endif
