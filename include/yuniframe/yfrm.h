#ifndef __YUNI_YFRM_H
#define __YUNI_YFRM_H

#ifdef __cplusplus
extern "C" {
#endif
// }

#ifdef YFRM_DLL
/* Win32 DLL */
#ifdef YFRM_SHARED_BUILD
#ifdef _WIN32
#define YFRM_API __declspec(dllexport)
#else
#define YFRM_API __attribute__ ((visibility ("default")))
#endif /* _WIN32 */
#else
#ifdef _WIN32
#define YFRM_API __declspec(dllimport)
#else
#define YFRM_API
#endif
#endif
#else
/* Generic static-library */
#define YFRM_API
#endif

#include <stdint.h>
#include <stddef.h>

/* Context */
struct cwgl_ctx_s;
typedef struct cwgl_ctx_s cwgl_ctx_t;
YFRM_API int yfrm_init(void); /* Tentative */
YFRM_API void yfrm_terminate(void); /* Tentative */
YFRM_API cwgl_ctx_t* yfrm_cwgl_ctx_create(int32_t width, int32_t height, int32_t reserved, int32_t flags);
YFRM_API cwgl_ctx_t* yfrm_cwgl_ctx_reset0(cwgl_ctx_t* ctx); /* Tentative */
YFRM_API void yfrm_cwgl_ctx_release(cwgl_ctx_t* ctx);

/* Events */
YFRM_API int yfrm_query0(int32_t slot, int32_t* buf, size_t buflen);

/* Frame */
YFRM_API void yfrm_frame_begin0(void*);
YFRM_API void yfrm_frame_end0(void*);

/* Audio */
YFRM_API void yfrm_audio_enqueue0(float* ch0, float* ch1, int32_t samples);
YFRM_API void yfrm_audio_pause0(void);

/* Filesystem */
struct yfrm_file_s;
typedef struct yfrm_file_s yfrm_file_t;

YFRM_API void yfrm_file_set_prefix0(const char* prefix);
YFRM_API void yfrm_file_configure0(const char* slotname);

/* Open / Close */
YFRM_API int yfrm_file_open_ro(const char* path, yfrm_file_t** file);
YFRM_API int yfrm_file_open_rw(const char* path, yfrm_file_t** file);
YFRM_API int yfrm_file_open_create(const char* path, yfrm_file_t** file);
YFRM_API void yfrm_file_close(yfrm_file_t* file);

/* Metadata */
struct yfrm_file_readdir_s;
typedef struct yfrm_file_readdir_s yfrm_file_readdir_t;
#define YFRM_FILE_FLAG_IS_DIRECTORY 1
YFRM_API int yfrm_file_pathinfo(const char* path, uint64_t* flags, uint64_t* size, uint64_t* time_create, uint64_t* time_mod);
YFRM_API int yfrm_file_info(yfrm_file_t* file, uint64_t* flags, uint64_t* size, uint64_t* time_create, uint64_t* time_mod);
YFRM_API int yfrm_file_mkdir(const char* path);
YFRM_API int yfrm_file_rmdir(const char* path);
YFRM_API int yfrm_file_rename(const char* oldpath, const char* newpath);
YFRM_API int yfrm_file_unlink(const char* path);
YFRM_API int yfrm_file_readdir_begin(const char* path, yfrm_file_readdir_t** rd);
YFRM_API int yfrm_file_readdir_step(yfrm_file_readdir_t* rd, uint32_t dostep, char* buf, uint32_t buflen, uint32_t* outlen);
YFRM_API void yfrm_file_readdir_end(yfrm_file_readdir_t* rd);

/* Access */
YFRM_API int yfrm_file_read(yfrm_file_t* file, uint64_t offset, char* buf, uint64_t buflen, uint64_t* readcnt);
YFRM_API int yfrm_file_write(yfrm_file_t* file, uint64_t offset, const char* buf, uint64_t buflen);

// {
#ifdef __cplusplus
};
#endif

#endif
