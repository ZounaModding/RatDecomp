#include "SubWorld_Z.h"
#include "World_Z.h"

S32 SubWorld_Z::SubWorldDisplayID;
Bool SubWorld_Z::SubWorldDisplaySelection;
Float SubWorld_Z::SubWorldDisplayScale = 1.0f;

SubWorld_Z::SubWorld_Z() { }

Bool SubWorld_Z::MarkHandles() {
    return FALSE;
}

void SubWorld_Z::Load(void** i_Data) {
}

void SubWorld_Z::EndLoad() {
}

void SubWorld_Z::AfterEndLoad() {
}

Bool SubWorldRange_Z::Inside(const Vec2f& i_Pos) const {
    return FALSE;
}
