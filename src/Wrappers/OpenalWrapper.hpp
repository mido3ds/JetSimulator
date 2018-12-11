#pragma once

#define AL_NO_PROTOTYPES
#include <openal/al.h>
#include <openal/alc.h>

#ifndef NO_EXTERN
extern LPALENABLE alEnable;
extern LPALDISABLE alDisable;
extern LPALGETERROR alGetError;
extern LPALLISTENERF alListenerf;
extern LPALLISTENERFV alListenerfv;
extern LPALGENSOURCES alGenSources;
extern LPALDELETESOURCES alDeleteSources;
extern LPALSOURCEF alSourcef;
extern LPALSOURCEFV alSourcefv;
extern LPALSOURCEI alSourcei;
extern LPALGETSOURCEF alGetSourcef;
extern LPALGETSOURCEFV alGetSourcefv;
extern LPALGETSOURCEI alGetSourcei;
extern LPALSOURCEPLAY alSourcePlay;
extern LPALSOURCESTOP alSourceStop;
extern LPALSOURCEREWIND alSourceRewind;
extern LPALSOURCEPAUSE alSourcePause;
extern LPALGENBUFFERS alGenBuffers;
extern LPALDELETEBUFFERS alDeleteBuffers;
extern LPALBUFFERDATA alBufferData;
extern LPALBUFFERI alBufferi;
extern LPALCCREATECONTEXT alcCreateContext;
extern LPALCMAKECONTEXTCURRENT alcMakeContextCurrent;
extern LPALCPROCESSCONTEXT alcProcessContext;
extern LPALCDESTROYCONTEXT alcDestroyContext;
extern LPALCGETCURRENTCONTEXT alcGetCurrentContext;
extern LPALCGETCONTEXTSDEVICE alcGetContextsDevice;
extern LPALCOPENDEVICE alcOpenDevice;
extern LPALCCLOSEDEVICE alcCloseDevice;
extern LPALCGETERROR alcGetError;
#endif //NO_EXTERN

int openalInit();
void openalFree();