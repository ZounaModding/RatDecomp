#include "Keyframer_Z.h"
#include "Program_Z.h"

void KeyframerFloat_Z::Load(void** i_Data) {
    LOAD_Z(m_Flag);
    S32 l_Nb;
    LOADDYNARRAY_Z(m_Keys);
}

void KeyframerFloatComp_Z::Load(void** i_Data) {
    LOAD_Z(m_Flag);
    S32 l_Nb;
    LOADDYNARRAY_Z(m_Keys);
}

void KeyframerVec3fComp_Z::Load(void** i_Data) {
    LOAD_Z(m_Flag);
    S32 l_Nb;
    LOADDYNARRAY_Z(m_Keys);
}

void KeyframerVec3f_Z::Load(void** i_Data) {
    LOAD_Z(m_Flag);
    S32 l_Nb;
    LOADDYNARRAY_Z(m_Keys);
}

void KeyframerExtVec3f_Z::Load(void** i_Data) {
    LOAD_Z(m_Flag);
    S32 l_Nb;
    LOADDYNARRAY_Z(m_Keys);
}

void KeyframerRot_Z::Load(void** i_Data) {
    S32 l_Nb;
    LOADDYNARRAY_Z(m_Keys);
}

void KeyframerBezierRot_Z::Load(void** i_Data) {
    S32 l_Nb;
    LOADDYNARRAY_Z(m_Keys);
}

void KeyframerFloatLinearComp_Z::Load(void** i_Data) {
    LOAD_Z(m_Flag);
    S32 l_Nb;
    LOADDYNARRAY_Z(m_Keys);
}

void KeyframerVec3fLinear_Z::Load(void** i_Data) {
    LOAD_Z(m_Flag);
    S32 l_Nb;
    LOADDYNARRAY_Z(m_Keys);
}

void KeyframerHdl_Z::Load(void** i_Data) {
    S32 l_Nb;
    LOADDYNARRAYSIZE_Z(m_Keys);
    for (S32 i = 0; i < l_Nb; i++) {
        gData.ClassMgr->LoadLink(m_Keys[i].m_Value, i_Data);
        LOAD_Z(m_Keys[i].m_Time);
    }
}

void KeyframerFlag_Z::Load(void** i_Data) {
    S32 l_Nb;
    LOADDYNARRAY_Z(m_Keys);
}

void KeyframerVec2fLinear_Z::Load(void** i_Data) {
    LOAD_Z(m_Flag);
    S32 l_Nb;
    LOADDYNARRAY_Z(m_Keys);
}

void KeyframerVec2fLinearComp_Z::Load(void** i_Data) {
    LOAD_Z(m_Flag);
    S32 l_Nb;
    LOADDYNARRAY_Z(m_Keys);
}

void KeyframerVec4fLinearComp_Z::Load(void** i_Data) {
    LOAD_Z(m_Flag);
    S32 l_Nb;
    LOADDYNARRAY_Z(m_Keys);
}

void KeyframerVec4fLinear_Z::Load(void** i_Data) {
    LOAD_Z(m_Flag);
    S32 l_Nb;
    LOADDYNARRAY_Z(m_Keys);
}

void KeyframerHdl_Z::UpdateLinks() {
    for (S32 i = 0; i < GetNbKeys(); i++) {
        gData.ClassMgr->UpdateLink(m_Keys[i].m_Value);
    }
}
