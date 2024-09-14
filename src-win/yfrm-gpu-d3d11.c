#ifdef YFRM_CWGL_USE_ANGLE
#include <windows.h>
#include <d3d11.h>

void* /* pfdev */
yfrm_gpu_initpfdev_d3d11(void){
    ID3D11Device* pfdev;
    HRESULT hr;
    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    const D3D_FEATURE_LEVEL featureLevelArray[] = { 
        D3D_FEATURE_LEVEL_11_0, 
        D3D_FEATURE_LEVEL_10_0, 
    };

    hr = D3D11CreateDevice(NULL /* Default device */,
                           D3D_DRIVER_TYPE_HARDWARE,
                           NULL,
                           createDeviceFlags,
                           featureLevelArray,
                           2,
                           D3D11_SDK_VERSION,
                           &pfdev,
                           NULL,
                           NULL);
    if(hr){
        return NULL;
    }else{
        return pfdev;
    }
}
#endif

