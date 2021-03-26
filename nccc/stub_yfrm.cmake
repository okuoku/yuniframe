nccc_stub_begin(yfrm)

# Context
nccc_api(yfrm_init IN OUT u32)
nccc_api(yfrm_terminate IN OUT)
nccc_api(yfrm_cwgl_ctx_create
    IN u32 u32 u32 u32
    OUT ptr)
nccc_api(yfrm_cwgl_ctx_release
    IN ptr OUT)

# Events
nccc_api(yfrm_query0
    IN u32 ptr u64
    OUT u32)

# Frame
nccc_api(yfrm_frame_begin0
    IN ptr OUT)
nccc_api(yfrm_frame_end0
    IN ptr OUT)

# Audio
nccc_api(yfrm_audio_enqueue0
    IN ptr ptr u32
    OUT)
nccc_api(yfrm_audio_pause0
    IN OUT)

# Filesystem
#  Open / Close
nccc_api(yfrm_file_open_ro
    IN ptr ptr
    OUT u32) #FIXME: s32??
nccc_api(yfrm_file_open_rw
    IN ptr ptr
    OUT u32)
nccc_api(yfrm_file_open_create
    IN ptr ptr
    OUT u32)
nccc_api(yfrm_file_close
    IN ptr
    OUT)
#  Metadata
nccc_api(yfrm_file_pathinfo
    IN ptr ptr ptr ptr ptr
    OUT u32)
nccc_api(yfrm_file_info
    IN ptr ptr ptr ptr ptr
    OUT u32)
nccc_api(yfrm_file_mkdir
    IN ptr
    OUT u32)
nccc_api(yfrm_file_rmdir
    IN ptr
    OUT u32)
nccc_api(yfrm_file_rename
    IN ptr ptr
    OUT u32)
nccc_api(yfrm_file_unlink
    IN ptr
    OUT u32)
nccc_api(yfrm_file_readdir_begin
    IN ptr ptr
    OUT u32)
nccc_api(yfrm_file_readdir_step
    IN ptr u32 ptr u32 ptr
    OUT u32)
nccc_api(yfrm_file_readdir_end
    IN ptr
    OUT)
#  Access
nccc_api(yfrm_file_read
    IN ptr u64 ptr u64 ptr
    OUT u32)
nccc_api(yfrm_file_write
    IN ptr u64 ptr u64
    OUT u32)

nccc_stub_end(yfrm)
