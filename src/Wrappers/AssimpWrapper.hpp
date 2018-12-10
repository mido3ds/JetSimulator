#pragma once
#include <assimp/scene.h>
#include <assimp/material.h>

typedef const C_STRUCT aiScene* (ASSIMP_API *aiImportFile_ptr) (
    const char* pFile,
    unsigned int pFlags);

typedef const char* (ASSIMP_API *aiGetErrorString_ptr)(void);

typedef C_ENUM aiReturn (ASSIMP_API *aiGetMaterialFloatArray_ptr)(
    const C_STRUCT aiMaterial* pMat,
    const char* pKey,
    unsigned int type,
    unsigned int index,
    ai_real* pOut,
    unsigned int* pMax);


typedef C_ENUM aiReturn (ASSIMP_API *aiGetMaterialString_ptr)(const C_STRUCT aiMaterial* pMat,
    const char* pKey,
    unsigned int type,
    unsigned int index,
    C_STRUCT aiString* pOut);

typedef void (ASSIMP_API *aiReleaseImport_ptr)(
    const C_STRUCT aiScene* pScene);

#ifndef NO_EXTERN
extern aiImportFile_ptr aiImportFile_;
extern aiGetErrorString_ptr aiGetErrorString_;
extern aiGetMaterialString_ptr aiGetMaterialString_;
extern aiGetMaterialFloatArray_ptr aiGetMaterialFloatArray_;
extern aiReleaseImport_ptr aiReleaseImport_;
#endif

int assimpInit();
void assimpFree();