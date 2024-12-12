
#ifndef OMG_DDS_API_DETAIL_H
#define OMG_DDS_API_DETAIL_H

#ifdef OMG_DDS_API_DETAIL_STATIC_DEFINE
#  define OMG_DDS_API_DETAIL
#  define OMG_DDS_API_DETAIL_NO_EXPORT
#else
#  ifndef OMG_DDS_API_DETAIL
#    ifdef ddscxx_EXPORTS
        /* We are building this library */
#      define OMG_DDS_API_DETAIL __declspec(dllexport)
#    else
        /* We are using this library */
#      define OMG_DDS_API_DETAIL __declspec(dllimport)
#    endif
#  endif

#  ifndef OMG_DDS_API_DETAIL_NO_EXPORT
#    define OMG_DDS_API_DETAIL_NO_EXPORT 
#  endif
#endif

#ifndef OMG_DDS_API_DETAIL_DEPRECATED
#  define OMG_DDS_API_DETAIL_DEPRECATED __declspec(deprecated)
#endif

#ifndef OMG_DDS_API_DETAIL_DEPRECATED_EXPORT
#  define OMG_DDS_API_DETAIL_DEPRECATED_EXPORT OMG_DDS_API_DETAIL OMG_DDS_API_DETAIL_DEPRECATED
#endif

#ifndef OMG_DDS_API_DETAIL_DEPRECATED_NO_EXPORT
#  define OMG_DDS_API_DETAIL_DEPRECATED_NO_EXPORT OMG_DDS_API_DETAIL_NO_EXPORT OMG_DDS_API_DETAIL_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef OMG_DDS_API_DETAIL_NO_DEPRECATED
#    define OMG_DDS_API_DETAIL_NO_DEPRECATED
#  endif
#endif

#endif /* OMG_DDS_API_DETAIL_H */
