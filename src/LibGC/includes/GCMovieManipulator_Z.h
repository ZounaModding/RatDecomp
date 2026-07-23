#ifndef _GCMOVIEMANIPULATOR_Z_H_
#define _GCMOVIEMANIPULATOR_Z_H_
#include "MovieManipulator_Z.h"
#include "binktextures.h"

class GCMovieManipulator_Z : public MovieManipulator_Z {
    friend class Movie_Z;

public:
    GCMovieManipulator_Z();

    virtual ~GCMovieManipulator_Z() { }

    virtual void Draw(const DrawInfo_Z& i_DrawInfo);
    virtual void Open(const Char* a1, U32 a2);
    virtual void Stop(bool a1);
    virtual void SetError(U32 a1);
    virtual void HandleBinkCopyToBuffer();

    static BaseObject_Z* NewObject() {
        return NewL_Z(49) GCMovieManipulator_Z;
    }

private:
    BINKTEXTURESET m_TextureSet;
    Bool m_RestoreVsync;
};

#endif // _GCMOVIEMANIPULATOR_Z_H_
