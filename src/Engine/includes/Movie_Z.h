#ifndef _MOVIE_Z_H_
#define _MOVIE_Z_H_
#include "DynArray_Z.h"
#include "Viewport_Z.h"
#include "MovieManipulator_ZHdl.h"
#include "Color_Z.h"

Bool Cmd_PlayMovie();

enum MovieError {
    MOVIE_ERROR_NONE = 0,
    MOVIE_ERROR_UNKNOWN = 1
};

class Movie_Z {
    Bool m_UnkBool_0x0;
    U32 m_UnkU32_0x4;
    U32 m_UnkU32_0x8;
    U32 m_UnkU32_0xC;
    U32 m_UnkU32_0x10;
    U32 m_UnkU32_0x14;
    MovieManipHdlDA m_MovieManipHdls;
    Float m_PlaySpeed;
    Color m_Color;

public:
    virtual ~Movie_Z() { }

    virtual Bool Init();
    virtual void Shut();
    virtual void Play(const Char* i_FilePath, U32 i_Flags);
    virtual void Pause(Bool i_Pause);
    virtual void Skip(Float i_SkipTime);

    virtual void GetExtension() { }

    virtual void PlayMovie(const Char* i_FilePath, U32 i_Flags) { }

    virtual void StopMovie() { }

    virtual void SwitchFullScreen() { }

    virtual void DisplayChanged() { }

    virtual void PaintVideo() { }

    virtual void MarkHandles();

    virtual void Update(Float i_DeltaTime) { }

    virtual void Draw(DrawInfo_Z& i_DrawInfo) { }

    virtual void RemoveManip(const MovieManipHdl& i_ManipHdl);

    Bool IsPlaying();
    U32 GetLastError();
};

void* BinkAllocate(U32 i_Bytes);
void BinkFree(void* i_Ptr);

#endif
