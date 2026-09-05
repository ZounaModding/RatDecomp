#include "Game_Z.h"
#include "ClassManager_Z.h"
#include "ObjectGame_Z.h"
#include "ObjectsGame_Z.h"

void Game_Z::DeclareParticlesManipulator(const PlayParticles_ZHdl& i_PlayParticlesHdl) {
    m_ObjectsGameMgrHdl->AddGamePlayParticles(i_PlayParticlesHdl);
}

void Game_Z::DeclareObjectGame(const ObjectGame_ZHdl& i_ObjectGameHdl) {
    for (S32 i = 0; i < m_ObjectGameHdls.GetSize(); i++) {
        if (m_ObjectGameHdls[i] == i_ObjectGameHdl) {
            return;
        }
    }

    m_ObjectGameHdls.Add(i_ObjectGameHdl);
    i_ObjectGameHdl->SetGame(GetHandle());
}
