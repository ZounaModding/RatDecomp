#ifndef _RTCPLAYER_Z_H_
#define _RTCPLAYER_Z_H_
#include "Manipulator_Z.h"
#include "Game_ZHdl.h"
#include "Node_ZHdl.h"
#include "Rtc_ZHdl.h"
#include "RtcAgent_ZHdl.h"
#include "World_ZHdl.h"
#include "PlayAnim_ZHdl.h"
#include "Animation_ZHdl.h"
#include "Bitmap_ZHdl.h"
#include "Math_Z.h"

struct RtcNode {
    Name_Z m_ParentName;
    Name_Z m_NodeName;
    S32 m_ParentIdx;
    Node_ZHdl m_NodeHdl; // Only set initially if the node is in rtc bf (m_IsWorldNode == FALSE), otherwise it's set after
    Node_ZHdl m_ParentNodeHdl;
    Bool m_IsWorldNode;         // Node is in the world, not in the rtc bf
    Name_Z m_WorldNodeFileName; // If m_IsWorldNode is set this is used to find the world node
};

class RtcPlayer_Z : public ManipulatorDraw_Z {
public:
    RtcPlayer_Z();

    virtual ~RtcPlayer_Z() { }

    virtual void Init();
    virtual Bool MarkHandles();
    virtual void Reset();
    virtual void Update(Float a1);
    virtual void Draw(const Viewport_Z* a1);
    virtual void InitRtc(const World_ZHdl& a1, const Rtc_ZHdl& a2, RtcAgent_ZHdl a3, Node_ZHdl a4, S32 a5);
    virtual void Start(const Game_ZHdl& a1);
    virtual void UpdateLod(Float a1);

    static BaseObject_Z* NewObject() { return NewL_Z(129) RtcPlayer_Z; }

private:
    DynArray_Z<PlayAnim_ZHdl, 32> m_PlayAnimHdls;
    DynArray_Z<Animation_ZHdl, 32> m_AnimationHdls;
    DynArray_Z<RtcNode, 16> m_RtcNodes;
    Rtc_ZHdl m_RtcHdl;
    World_ZHdl m_WorldHdl;
    Game_ZHdl m_GameHdl;
    RtcAgent_ZHdl m_RtcAgentHdl;
    Bitmap_ZHdl m_PictureBitmapHdl;
    Float m_CurTime;
    Float m_PrevTime;
    Float m_MaxTime;
    Float m_PlaybackSpeed;
    Float m_MsgTime;
    S32 m_CameraNodeIdx;
    U16 m_Flag;
    U8 m_LetterBoxPosA;
    U8 m_LetterBoxPosB;
    U8 m_LetterBoxPosC;
    U8 m_LetterBoxPosD;
    Vec3f m_LetterBoxColor;
    Float m_LetterBoxRelated_0x7c;
    U16 m_UnkU16_0x80;
    U16 m_LoopRelated_0x82;
    Float m_LoopStartTime;
};

#endif // _RTCPLAYER_Z_H_
