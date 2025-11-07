#ifndef _ANIMMESSAGE_Z_H_
#define _ANIMMESSAGE_Z_H_
#include "Types_Z.h"
#include "Name_Z.h"

class Skel_Z;
class BoneNode_Z;
class Node_Z;
class AnimatedAgent_Z;

class Message_Z {
    U32 m_Id;
    U32 m_U32Param;
    U32 m_FlagParam;
    Float m_FloatParam;
    Name_Z m_NameParam;
};

class RegMessage_Z {
    U32 m_Id;
    U32 m_U32Param;
    U32 m_FlagParam;
    Float m_FloatParam;
    Name_Z m_NameParam;
    Float m_Time;
    U8 m_UnkBytes_0x18[12];
    Skel_Z* m_SkelPtr;
    BoneNode_Z* m_BoneNodePtr;
    Node_Z* m_NodePtr;
};

class SkelMessage_Z {

    U32 m_NextId;
    U32 m_SphereId;
    U32 m_VsSphereId;
    Bool m_CanSphereCollide;
    Bool m_CanVsSphereCollide;
    Name_Z m_SphereName;
    Name_Z m_VsSphereName;
    Name_Z m_VsName;
    Skel_Z* m_SkelPtr;
    Skel_Z* m_VsSkelPtr;
    BoneNode_Z* m_BoneNodePtr;
    BoneNode_Z* m_VsBoneNodePtr;
    AnimatedAgent_Z* m_AnimatedAgentPtr;
};

#endif // _ANIMMESSAGE_Z_H_
