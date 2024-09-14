#include <yuniframe/yfrm.h>

#include <stdlib.h>

YFRM_API int
yfrm_query0(int32_t slot, int32_t* buf, size_t buflen){
    return 0;
}

YFRM_API void
yfrm_wait0(int32_t timeout){
    abort();
}
