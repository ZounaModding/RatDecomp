#include "ObjectGame_Z.h"
#include "Game_Z.h"
#include "World_Z.h"

void ObjectGame_Z::ActionOnActivate() {
    m_GameHdl->GetWorld()->ActionOnActivate();
}

void ObjectGame_Z::ActionOnDeactivate() {
}
