#include <iostream>
#include <fstream>
#include <filesystem>

#include <string.h>

#include "yfrm.h"

namespace fs = std::filesystem;

struct yfrm_file_s {
    fs::path* pth; // may be prefixed
    std::fstream* strm;
};

static fs::path* prefix = nullptr;

YFRM_API void
yfrm_file_set_prefix0(const char* px){
    if(prefix){
        delete prefix;
    }
    prefix = new fs::path(fs::u8path(px));
}

/* Open / Close */
static int
file_open_gen(const char* path, yfrm_file_t** file, 
              std::ios_base::openmode mode){
    try {
        // FIXME: should we try-catch this? maybe.
        auto pth = fs::u8path(path);
        fs::path p;
        if(prefix){
            p = *prefix / pth;
        }else{
            p = pth;
        }
        auto strm = new std::fstream(p, mode | std::ios_base::binary);
        auto out = new yfrm_file_t();
        out->pth = new fs::path(p);
        out->strm = strm;

        *file = out;
        return 0;
    } catch(...) {
        return -1;
    }

}
YFRM_API int
yfrm_file_open_ro(const char* path, yfrm_file_t** file){
    return file_open_gen(path, file, std::ios_base::in);
}

YFRM_API int
yfrm_file_open_rw(const char* path, yfrm_file_t** file){
    return file_open_gen(path, file, std::ios_base::in | std::ios_base::out);
}

YFRM_API int
yfrm_file_open_create(const char* path, yfrm_file_t** file){
    return file_open_gen(path, file, 
                         std::ios_base::trunc | std::ios_base::in | 
                         std::ios_base::out);
}

YFRM_API void
yfrm_file_close(yfrm_file_t* file){
    if(file->pth){
        delete file->pth;
    }
    if(file->strm){
        delete file->strm;
    }
    delete file;
}

/* Metadata */
static int
file_info_gen(const std::filesystem::path& path, 
              uint64_t* out_flags, uint64_t* out_size,
              uint64_t* out_time_create, uint64_t* out_time_mod){
    /* FIXME: We need real fstat() for some safety */
    try {
        uint64_t flags = 0;
        uint64_t size;
        uint64_t time_create = 0;
        uint64_t time_mod = 0;
        // FIXME: Check cache behaviour
        fs::file_status st = fs::status(path);
        fs::file_type typ = st.type();
        if(typ == fs::file_type::directory){
            flags |= YFRM_FILE_FLAG_IS_DIRECTORY;
            size = 0;
        }else{
            size = fs::file_size(path);
        }
        if(out_flags){
            *out_flags = flags;
        }
        if(out_size){
            *out_size = size;
        }
        if(out_time_create){
            *out_time_create = time_create;
        }
        if(out_time_mod){
            *out_time_mod = time_mod;
        }
        return 0;
    } catch (...) {
        return -1;
    }
}

YFRM_API int
yfrm_file_pathinfo(const char* path, uint64_t* flags, uint64_t* size,
                   uint64_t* time_create, uint64_t* time_mod){
    auto pth = fs::u8path(path);
    fs::path p;
    if(prefix){
        p = *prefix / pth;
    }else{
        p = pth;
    }
    return file_info_gen(p, flags, size, time_create, time_mod);
}

YFRM_API int
yfrm_file_info(yfrm_file_t* file, uint64_t* flags, uint64_t* size,
               uint64_t* time_create, uint64_t* time_mod){
    return file_info_gen(*file->pth, flags, size, time_create, time_mod);
}

YFRM_API int
yfrm_file_mkdir(const char* path){
    bool done;
    try {
        done = fs::create_directory(fs::u8path(path));
        if(done){
            return 0;
        }else{
            return -1;
        }
    } catch(...){
        return -1;
    }
}

YFRM_API int
yfrm_file_rmdir(const char* path){
    // FIXME: Also works on file
    try {
        (void)fs::remove(fs::u8path(path));
        return 0;
    } catch(...) {
        return -1;
    }
}

YFRM_API int
yfrm_file_rename(const char* oldpath, const char* newpath){
    try {
        fs::rename(fs::u8path(oldpath), fs::u8path(newpath));
        return 0;
    } catch(...) {
        return -1;
    }
}

YFRM_API int
yfrm_file_unlink(const char* path){
    // FIXME: Also works on directory
    try {
        (void)fs::remove(fs::u8path(path));
        return 0;
    } catch(...) {
        return -1;
    }
}

struct yfrm_file_readdir_s {
    fs::directory_iterator* it;
};

YFRM_API int
yfrm_file_readdir_begin(const char* path, yfrm_file_readdir_t** rd){
    auto ret = new yfrm_file_readdir_t();
    auto it = new fs::directory_iterator(fs::u8path(path));
    *rd = ret;
    ret->it = it;
    return 0;
}

YFRM_API void
yfrm_file_readdir_end(yfrm_file_readdir_t* rd){
    delete rd->it;
    delete rd;
}

YFRM_API int
yfrm_file_readdir_step(yfrm_file_readdir_t* rd, uint32_t dostep,
                       char* buf, uint32_t buflen, uint32_t* outlen){
    fs::directory_iterator end;

    if(dostep){
        std::error_code ec;
        rd->it->increment(ec);
    }
    if(*rd->it == end){
        *outlen = 0;
    }else{
        auto str = (*rd->it)->path().filename().generic_u8string();
        uint64_t pthlen = str.length();
        size_t cpylen = buflen > pthlen ? pthlen : buflen-1;
        *outlen = pthlen;
        memcpy(buf, str.data(), cpylen);
        buf[cpylen] = 0;
    }
    return 0;
}

/* Access */
YFRM_API int
yfrm_file_read(yfrm_file_t* file, uint64_t offset, char* buf, uint64_t buflen,
               uint64_t* readcnt){
    uint64_t cnt;
    file->strm->clear();
    cnt = file->strm->seekg(offset).read(buf, buflen).gcount();
    if(readcnt){
        *readcnt = cnt;
    }
    return 0;
}

YFRM_API int
yfrm_file_write(yfrm_file_t* file, uint64_t offset, 
                const char* buf, uint64_t buflen){
    file->strm->seekp(offset).write(buf, buflen);
    return 0;
}
