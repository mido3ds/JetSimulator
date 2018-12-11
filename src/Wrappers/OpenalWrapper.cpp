#include <Windows.h>

#define NO_EXTERN
#include "OpenalWrapper.hpp"

static HINSTANCE lib;

LPALENABLE alEnable;
LPALDISABLE alDisable;
LPALGETERROR alGetError;

LPALLISTENERF alListenerf;
LPALLISTENERFV alListenerfv;

LPALGENSOURCES alGenSources;
LPALDELETESOURCES alDeleteSources;

LPALSOURCEF alSourcef;
LPALSOURCEFV alSourcefv;
LPALSOURCEI alSourcei;

LPALGETSOURCEF alGetSourcef;
LPALGETSOURCEFV alGetSourcefv;
LPALGETSOURCEI alGetSourcei;

LPALSOURCEPLAY alSourcePlay;
LPALSOURCESTOP alSourceStop;
LPALSOURCEREWIND alSourceRewind;
LPALSOURCEPAUSE alSourcePause;

LPALGENBUFFERS alGenBuffers;
LPALDELETEBUFFERS alDeleteBuffers;
LPALBUFFERDATA alBufferData;
LPALBUFFERI alBufferi;

LPALCCREATECONTEXT alcCreateContext;
LPALCMAKECONTEXTCURRENT alcMakeContextCurrent;
LPALCPROCESSCONTEXT alcProcessContext;
LPALCDESTROYCONTEXT alcDestroyContext;
LPALCGETCURRENTCONTEXT alcGetCurrentContext;
LPALCGETCONTEXTSDEVICE alcGetContextsDevice;
LPALCOPENDEVICE alcOpenDevice;
LPALCCLOSEDEVICE alcCloseDevice;
LPALCGETERROR alcGetError;

inline FARPROC __GetProcAddress(LPCSTR name) {
    FARPROC f = GetProcAddress(lib, name);
    if (!f) {
        lib = 0;
        openalFree();
    }
    return f;
}

int openalInit() {
    lib = LoadLibrary(TEXT("assets/dlls/OpenAl32.dll"));
    if (!lib) return 0;

    alEnable = (LPALENABLE) __GetProcAddress("alEnable");
    alDisable = (LPALDISABLE) __GetProcAddress("alDisable");
    alGetError = (LPALGETERROR) __GetProcAddress("alGetError");
    alListenerf = (LPALLISTENERF) __GetProcAddress("alListenerf");
    alListenerfv = (LPALLISTENERFV) __GetProcAddress("alListenerfv");
    alGenSources = (LPALGENSOURCES) __GetProcAddress("alGenSources");
    alDeleteSources = (LPALDELETESOURCES) __GetProcAddress("alDeleteSources");
    alSourcef = (LPALSOURCEF) __GetProcAddress("alSourcef");
    alSourcefv = (LPALSOURCEFV) __GetProcAddress("alSourcefv");
    alSourcei = (LPALSOURCEI) __GetProcAddress("alSourcei");
    alGetSourcef = (LPALGETSOURCEF) __GetProcAddress("alGetSourcef");
    alGetSourcefv = (LPALGETSOURCEFV) __GetProcAddress("alGetSourcefv");
    alGetSourcei = (LPALGETSOURCEI) __GetProcAddress("alGetSourcei");
    alSourcePlay = (LPALSOURCEPLAY) __GetProcAddress("alSourcePlay");
    alSourceStop = (LPALSOURCESTOP) __GetProcAddress("alSourceStop");
    alSourceRewind = (LPALSOURCEREWIND) __GetProcAddress("alSourceRewind");
    alSourcePause = (LPALSOURCEPAUSE) __GetProcAddress("alSourcePause");
    alGenBuffers = (LPALGENBUFFERS) __GetProcAddress("alGenBuffers");
    alDeleteBuffers = (LPALDELETEBUFFERS) __GetProcAddress("alDeleteBuffers");
    alBufferData = (LPALBUFFERDATA) __GetProcAddress("alBufferData");
    alBufferi = (LPALBUFFERI) __GetProcAddress("alBufferi");
    alcCreateContext = (LPALCCREATECONTEXT) __GetProcAddress("alcCreateContext");
    alcMakeContextCurrent = (LPALCMAKECONTEXTCURRENT) __GetProcAddress("alcMakeContextCurrent");
    alcProcessContext = (LPALCPROCESSCONTEXT) __GetProcAddress("alcProcessContext");
    alcDestroyContext = (LPALCDESTROYCONTEXT) __GetProcAddress("alcDestroyContext");
    alcGetCurrentContext = (LPALCGETCURRENTCONTEXT) __GetProcAddress("alcGetCurrentContext");
    alcGetContextsDevice = (LPALCGETCONTEXTSDEVICE) __GetProcAddress("alcGetContextsDevice");
    alcOpenDevice = (LPALCOPENDEVICE) __GetProcAddress("alcOpenDevice");
    alcCloseDevice = (LPALCCLOSEDEVICE) __GetProcAddress("alcCloseDevice");
    alcGetError = (LPALCGETERROR) __GetProcAddress("alcGetError");

    return lib != 0;
}

void openalFree() {
    FreeLibrary(lib);
}