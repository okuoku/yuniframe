#include "yfrm.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "SDL.h"

#define UNSUPPORTED() do{ printf("%s: unsupported.\n", __func__); } while (0)

struct yfrm_file_s {
    SDL_RWops* rw;
};

YFRM_API void
yfrm_file_set_prefix0(const char* px){
    UNSUPPORTED();
}

YFRM_API int
yfrm_file_open_gen(const char* path, yfrm_file_t** file, const char* mode){
    SDL_RWops* rw;
    yfrm_file_t* f;

    rw = SDL_RWFromFile(path, mode);
    if(rw){
        f = malloc(sizeof(yfrm_file_t));
        f->rw = rw;
        *file = f;
        return 0;
    }else{
        return -1;
    }
}

YFRM_API int
yfrm_file_open_ro(const char* path, yfrm_file_t** file){
    return yfrm_file_open_gen(path, file, "rb");
}

YFRM_API int
yfrm_file_open_rw(const char* path, yfrm_file_t** file){
    return yfrm_file_open_gen(path, file, "ab");
}

YFRM_API int
yfrm_file_open_create(const char* path, yfrm_file_t** file){
    return yfrm_file_open_gen(path, file, "w+b");
}

YFRM_API void
yfrm_file_close(yfrm_file_t* file){
    SDL_RWclose(file->rw);
    free(file);
}

YFRM_API int
yfrm_file_pathinfo(const char* path, uint64_t* flags,
                   uint64_t* size, uint64_t* time_create,
                   uint64_t* time_mod){
    UNSUPPORTED();
    return -1;
}

YFRM_API int
yfrm_file_info(yfrm_file_t* file, uint64_t* flags,
               uint64_t* size, uint64_t* time_create,
               uint64_t* time_mod){
    // FIXME: Tentative filesize-only implementation
    int64_t siz;
    siz = SDL_RWseek(file->rw, 0, RW_SEEK_END);
    if(siz < 0){
        return -1;
    }else{
        if(size){
            *size = siz;
        }
        if(flags){
            *flags = 0;
        }
        if(time_create){
            *time_create = 0;
        }
        if(time_mod){
            *time_mod = 0;
        }
        return 0;
    }
}

YFRM_API int
yfrm_file_mkdir(const char* path){
    UNSUPPORTED();
    return -1;
}

YFRM_API int
yfrm_file_rename(const char* oldpath, const char* newpath){
    UNSUPPORTED();
    return -1;
}

YFRM_API int
yfrm_file_rmdir(const char* path){
    UNSUPPORTED();
    return -1;
}

YFRM_API int
yfrm_file_unlink(const char* path){
    UNSUPPORTED();
    return -1;
}

struct yfrm_file_readdir_s {
    int dummy;
};

YFRM_API int
yfrm_file_readdir_begin(const char* path, yfrm_file_readdir_t** rd){
    UNSUPPORTED();
    return -1;
}

YFRM_API void
yfrm_file_readdir_end(yfrm_file_readdir_t* rd){
    UNSUPPORTED();
}

YFRM_API int
yfrm_file_readdir_step(yfrm_file_readdir_t* rd, uint32_t dostep,
                       char* buf, uint32_t buflen, uint32_t* outlen){
    UNSUPPORTED();
    return -1;
}

/* Access */
YFRM_API int
yfrm_file_read(yfrm_file_t* file, uint64_t offset, char* buf, uint64_t buflen,
               uint64_t* readcnt){
    int64_t pos;
    size_t cnt;
    pos = SDL_RWseek(file->rw, offset, RW_SEEK_SET);
    if(pos != offset){
        return -1;
    }
    SDL_ClearError();
    cnt = SDL_RWread(file->rw, buf, buflen, buflen);
    if(cnt == 0){
        if(SDL_GetError()){
            return 0;
        }else{
            if(readcnt){
                *readcnt = cnt;
            }
            return 0;
        }
    }else{
        if(readcnt){
            *readcnt = cnt;
        }
        return 0;
    }
}

YFRM_API int
yfrm_file_write(yfrm_file_t* file, uint64_t offset,
                const char* buf, uint64_t buflen){
    /* FIXME: Implement it */
    UNSUPPORTED();
    return -1;
}
