

/* Generic Extra stuff (GX)
 * Ideas borrowed liberally from the linux kernel, haproxy, and code all over
 * the place.
 *
 * gx_likely(stmt)   -> Tell compiler the branch is likely
 * gx_unlikely(stmt) -> Tell compiler the branch is rare
 * gx_cold_f         -> As part of a function signature- infrequent function
 * gx_hot_f          -> Frequently called function
 * gx_pure_f         -> Pure function- side-effect free
 * gx_finline_f      -> Force the function to be inlined regardless of compiler settings
 *
 * TODO:
 * gx_prefetch_rw_keep
 * gx_prefetch_rw_toss
 * gx_prefetch_ro_keep
 * gx_prefetch_ro_toss
 *
 */
#ifndef   GX_H
  #define GX_H


  #define gx_min(m,n) ((m) < (n) ? (m) : (n))
  #define gx_max(m,n) ((m) > (n) ? (m) : (n))


  #if          __GNUC__ > 3
    #ifndef    gx_likely
      #define  gx_likely(x)   __builtin_expect(!!(x), 1)
      #define  gx_unlikely(x) __builtin_expect(!!(x), 0)
    #endif
    #ifndef    gx_cold_f
      #define  gx_pure_f      __attribute__((pure))
      #define  gx_cold_f      __attribute__((cold))
      #define  gx_hot_f       __attribute__((hot))
    #endif
    #ifndef    gx_finline_f
      #define  gx_finline_f   inline __attribute__((always_inline))
    #endif
  #else
    #ifndef    gx_likely
      #define  gx_likely(x)   (x)
      #define  gx_unlikely(x) (x)
    #endif
    #ifndef    gx_cold_f
      #define  gx_cold_f
      #define  gx_hot_f
      #define  gx_pure_f
    #endif
    #ifndef    gx_finline_f
    #define    gx_finline_f   inline
    #endif
  #endif
#endif
