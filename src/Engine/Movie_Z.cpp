#include "Movie_Z.h"
#include "Memory_Z.h"
#include "Console_Z.h"
#include "bink.h"

void* BinkAllocate(U32 i_Bytes) {
    void* l_Ptr = AllocEndAlignL_Z(i_Bytes, 13, 16);
    return l_Ptr ? l_Ptr : (void*)-1;
}

void BinkFree(void* i_Ptr) {
    if (i_Ptr && i_Ptr != (void*)-1) {
        Free_Z(i_Ptr);
    }
}

Bool Movie_Z::Init() {
    gData.Cons->AddCommand("PlayMOVie", Cmd_PlayMovie, " MovieName");
    m_UnkBool_0x0 = TRUE;
    m_UnkU32_0x4 = 2;
    m_PlaySpeed = 1.0;
    m_Color.Set(0.5f, 0.5f, 0.5f, 1.0f);
    BinkSetMemory(BinkAllocate, BinkFree);
    return TRUE;
}

void Movie_Z::Shut() {
}

void Movie_Z::Play(const Char* i_FilePath, U32 i_Flags) {
}

Bool Movie_Z::IsPlaying() {
    for (S32 i = 0; i < m_MovieManipHdls.GetSize(); i++) {
        if (m_MovieManipHdls[i].IsValid()) {
            if (m_MovieManipHdls[i]->m_PlayState == 1) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

U32 Movie_Z::GetLastError() {
    for (S32 i = 0; i < m_MovieManipHdls.GetSize(); i++) {
        if (m_MovieManipHdls[i].IsValid()) {
            if (m_MovieManipHdls[i]->m_ErrorCode != MOVIE_ERROR_NONE) {
                return m_MovieManipHdls[i]->m_ErrorCode;
            }
        }
    }
    return MOVIE_ERROR_NONE;
}

void Movie_Z::Pause(Bool i_Pause) {
}

void Movie_Z::Skip(Float i_SkipTime) {
}

void Movie_Z::MarkHandles() {
}

void Movie_Z::RemoveManip(const MovieManipHdl& i_ManipHdl) {
}

Bool Cmd_PlayMovie() {
    return FALSE;
}
