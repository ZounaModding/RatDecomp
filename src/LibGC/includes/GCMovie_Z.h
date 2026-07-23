#ifndef _GCMOVIE_Z_H_
#define _GCMOVIE_Z_H_
#include "Movie_Z.h"
#include <gx.h>

class GCMovie_Z : public Movie_Z {
public:
    GCMovie_Z() {
        m_UnkBuffer_0x2e8 = NULL;
        m_UnkBuffer_0x2f8 = NULL;
        m_UnkBuffer_0x2fc = NULL;
        m_UnkBuffer_0x2f0 = NULL;
        m_UnkBuffer_0x2ec = NULL;
        m_UnkBuffer_0x2f4 = NULL;
        m_UnkS32_0x300 = 0;
    }

    virtual ~GCMovie_Z() { }

    virtual Bool Init();
    virtual void Shut();
    virtual void Play(const Char* a1, U32 a2);
    virtual void PlayMovie(const Char* a1, U32 a2);
    virtual void StopMovie();

private:
    U8 m_Unk_0x38[72];
    GXRenderModeObj* m_RenderModeObj;
    S32 m_PixelCount;
    Bool m_StopMovie;
    File_Z m_OpenMovieFile;
    U8* m_UnkBuffer_0x2e8;
    U8* m_UnkBuffer_0x2ec;
    U8* m_UnkBuffer_0x2f0;
    U8* m_UnkBuffer_0x2f4;
    U8* m_UnkBuffer_0x2f8;
    U8* m_UnkBuffer_0x2fc;
    S32 m_UnkS32_0x300;
    U8 m_Unk_0x304[60];
};

#endif // _GCMOVIE_Z_H_
