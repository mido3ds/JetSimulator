#include <Windows.h>

#define NO_EXTERN
#include "AssimpWrapper.hpp"

aiImportFile_ptr aiImportFile_;
aiGetErrorString_ptr aiGetErrorString_;
aiGetMaterialString_ptr aiGetMaterialString_;
aiGetMaterialFloatArray_ptr aiGetMaterialFloatArray_;
aiReleaseImport_ptr aiReleaseImport_;
HINSTANCE lib;

int assimpInit() {
    HINSTANCE lib = LoadLibrary(TEXT("assets/dlls/assimp-vc140-mt.dll"));
    if (!lib) return 0;

    aiImportFile_ = (aiImportFile_ptr) GetProcAddress(lib, "aiImportFile");
    aiGetErrorString_ = (aiGetErrorString_ptr) GetProcAddress(lib, "aiGetErrorString");
    aiGetMaterialString_ = (aiGetMaterialString_ptr) GetProcAddress(lib, "aiGetMaterialString");
    aiGetMaterialFloatArray_ = (aiGetMaterialFloatArray_ptr) GetProcAddress(lib, "aiGetMaterialFloatArray");
    aiReleaseImport_ = (aiReleaseImport_ptr) GetProcAddress(lib, "aiReleaseImport");

    if (!aiImportFile_||!aiGetErrorString_||!aiGetMaterialString_||!aiGetMaterialFloatArray_||!aiReleaseImport_)
        return 0;

    return 1;
}

void assimpFree() {
    FreeLibrary(lib);
}