#include "DynamicAnim2_Z.h"
#include "AnimationManager_Z.h"
#include "BoneNode_Z.h"
#include "Program_Z.h"
#include "Skel_Z.h"
#include "UserDefine_Z.h"
#include <string.h>

void DynamicAnim2_Z::Reset() {
}

Bool DynamicAnim2_Z::IsNodeDynamic(BoneNode_Z* i_Node, int i_Id) {
    if (!i_Node) {
        return FALSE;
    }

    UserDefine_Z* l_UserDefine = i_Node->GetUserDefine();
    if (l_UserDefine) {
        U32 l_Length;
        Char* l_Command = l_UserDefine->GetCmd().GetFirstCommand(l_Length);
        while (l_Command) {
            if (!strncmp(l_Command, "DYNAMICON", strlen("DYNAMICON"))) {
                return TRUE;
            }
            l_Command = l_UserDefine->GetCmd().GetNextCommand(l_Length);
        }
    }

    if (i_Id >= BONE_DYN_START && i_Id < BONE_DYN_END) {
        return TRUE;
    }
    return FALSE;
}

Bool DynamicAnim2_Z::IsSkelDynamic(Skel_Z* i_Skel) {
    if (i_Skel) {
        for (S32 i = 0; i < gData.AnimMgr->GetNbNodes(); i++) {
            if (i < i_Skel->GetNbBone()) {
                if (IsNodeDynamic(i_Skel->GetBone(i), i)) {
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

void DynamicAnim2_Z::SetSkel(Skel_Z* i_Skel) {
}

void DynamicAnim2_Z::Update(Float i_DeltaTime) {
}

void DynamicAnim2_Z::AddConicForce(const Vec3f& a1, const Vec3f& a2, Float a3, Float a4, Float a5, Float a6, Float a7) {
}

void DynamicAnim2_Z::AddSphericForce(const Sphere_Z& a1, Float a2, Float a3) {
}
