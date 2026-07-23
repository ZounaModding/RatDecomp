#include "GCMovie_Z.h"
#include "ARAM_Z.h"
#include "Console_Z.h"
#include "bink.h"
#include "binkngc.h"
#include <vi.h>

Bool PlayCautionMovie();

Bool GCMovie_Z::Init() {
    if (!Movie_Z::Init()) {
        return FALSE;
    }

    gData.Cons->AddCommand("PlayCautionMovie", PlayCautionMovie, "No Comment");

    switch (VIGetTvFormat()) {
        case VI_NTSC:
            m_RenderModeObj = &GXNtsc480Int;
            break;
        case VI_PAL:
            m_RenderModeObj = &GXPal528Int;
            break;
        case VI_MPAL:
            m_RenderModeObj = &GXMpal480Int;
            break;
    }

    m_PixelCount = ((m_RenderModeObj->fbWidth + 15) & 0xFFF0) * m_RenderModeObj->xfbHeight * 2;
    m_StopMovie = FALSE;

    RADARAMCALLBACKS l_AramCallbacks = { (RADARAMALLOC)AramManager_Alloc, (RADARAMFREE)AramManager_Free };
    BinkSetSoundSystem(BinkOpenAX, &l_AramCallbacks);
    return TRUE;
}

void GCMovie_Z::Shut() {
}

void GCMovie_Z::PlayMovie(const Char* a1, U32 a2) {
}

void GCMovie_Z::Play(const Char* a1, U32 a2) {
}

void GCMovie_Z::StopMovie() {
}

Bool PlayCautionMovie() {
    return TRUE;
}
