#include "Game_Z.h"
#include "Agent_Z.h"
#include "ClassManager_Z.h"
#include "World_Z.h"

void Game_Z::Stream(const Vec3f& i_Pos, Agent_ZHdl i_NotifyAgent, abc_message i_Msg) {
    if (i_NotifyAgent.IsValid()) {
        m_StreamNotifyAgent = i_NotifyAgent;
        m_StreamNotifyMsg = i_Msg;
        m_StreamWorldPos = i_Pos;
    }

    Vec3f l_Pos;

    if (m_StreamNotifyAgent.IsValid()) {
        l_Pos = m_StreamWorldPos;
    }
    else {
        l_Pos = i_Pos;
    }

    S32DA l_SubLevelIds;
    S32DA l_UnkIds;

    if (!m_StreamUnkDA_0x164.GetSize()) {
        GetSubLevelId(l_Pos, l_SubLevelIds, l_UnkIds, TRUE);
    }

    if (Stream(m_StreamUnkDA_0x164.GetSize() ? m_StreamUnkDA_0x164 : l_SubLevelIds, l_Pos)) {
        if (m_StreamNotifyAgent.IsValid()) {
            World_Z* l_World = m_WorldHdl;
            String_Z<ARRAY_CHAR_MAX> l_Message;
            String_Z<ARRAY_CHAR_MAX> l_Tmp;
            S32* l_Ids;
            S32 l_Nb;

            if (!m_StreamUnkDA_0x164.GetSize()) {
                l_Ids = m_StreamUnkDA_0x164.GetArrayPtr();
                l_Nb = m_StreamUnkDA_0x164.GetSize();
            }
            else {
                l_Nb = l_SubLevelIds.GetSize();
                l_Ids = l_SubLevelIds.GetArrayPtr();
            }

            l_Message = "> ! EndOfStream SubLevel ";

            for (S32 i = 0; i < l_Nb; i++) {
                l_Tmp.Sprintf("%d ", l_Ids[i]);
                l_Message.StrCat(l_Tmp.Get());
            }

            Send(m_StreamNotifyMsg, m_StreamNotifyAgent);
            m_StreamNotifyAgent = HANDLE_NULL;
        }
    }
}

S32 Game_Z::GetSubLevelId(const Vec3f& i_Pos, S32DA& o_SubLevelIds, S32DA& o_SubIds, Bool i_Unk) {
    S32 i;

    o_SubLevelIds.Empty();
    o_SubIds.Empty();

    World_Z* l_World = m_WorldHdl;
    S32 l_Nb = l_World->GetNbSubWorldData();
    BitArray_Z l_LoadedBits;

    l_LoadedBits.SetSize(l_Nb);
    l_LoadedBits.ClearAllBits();

    for (i = 0; i < l_Nb; i++) {
        if (l_World->GetSubWorldData(i).m_Flag & (FL_SUBWORLD_LOADING | FL_SUBWORLD_LOADED)) {
            for (S32 j = 0; j < l_World->GetSubWorldData(i).m_UnkIds_0x148.GetSize(); j++) {
                l_LoadedBits.SetBit(l_World->GetSubWorldData(i).m_UnkIds_0x148[j]);
            }
        }
    }

    for (i = 0; i < l_Nb; i++) {
        SubWorldData_Z& l_SubWorldData = l_World->GetSubWorldData(i);

        if (l_SubWorldData.m_Type == SUBWORLD_TYPE_SUBLEVEL && !l_LoadedBits.GetBit(i) && l_SubWorldData.m_Range.Inside(Vec2f(i_Pos.x, i_Pos.z))) {
            o_SubLevelIds.Add(i);

            for (S32 j = 0; j < l_SubWorldData.m_UnkIds_0x140.GetSize(); j++) {
                o_SubLevelIds.Add(l_SubWorldData.m_UnkIds_0x140[j]);
            }

            break;
        }
    }

    BitArray_Z l_SubLevelBits;

    l_SubLevelBits.SetDABits(o_SubLevelIds, l_Nb, TRUE);
    S32 l_SubLevelNb = l_SubLevelBits.GetDABits(o_SubLevelIds, TRUE);

    o_SubIds.SetSize(l_SubLevelNb);

    for (i = 0; i < l_SubLevelNb; i++) {
        o_SubIds[i] = l_World->GetSubWorldData(o_SubLevelIds[i]).m_SubId;
    }

    return l_SubLevelBits.GetDABits(o_SubLevelIds, TRUE);
}

Bool Game_Z::Stream(const S32DA& i_SubLevelIds, const Vec3f& i_Pos) {
    return !gData.ClassMgr->IsBigFileWrite();
}
