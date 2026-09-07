#include "Names.h"
#include "ItemMgr_G.h"

U32 ItemMgr_FrameCounter = 1;

void ItemMgr_G::Update(Float i_DeltaTime) {
    ItemMgr_FrameCounter++;
    FlushUnusedItems(2, 240);
}

void ItemMgr_G::FlushUnusedItems(U32 i_Count, U32 i_MaxAge) {
}
