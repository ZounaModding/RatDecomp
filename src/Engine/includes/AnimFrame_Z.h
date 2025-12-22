#ifndef _ANIMFRAME_Z_H_
#define _ANIMFRAME_Z_H_
#include "ResourceObject_Z.h"
#include "Node_ZHdl.h"
#include "Keyframer_Z.h"
#include "AnimFrame_ZHdl.h"
#include "AnimMessage_Z.h"

class Light_Z;
class HFog_Z;
class Omni_Z;

#define VL_STARTSTOP_STOP 0
#define VL_STARTSTOP_START 1
#define VL_STARTSTOP_PAUSE 2

#define FL_ANIM_NONE (U16)(0 << 0)
#define FL_ANIM_PLAY (U16)(1 << 0)                                   // Animation is currently playing or paused
#define FL_ANIM_STARTED (U16)(1 << 1)                                // Playback has started
#define FL_ANIM_READMESSAGE (U16)(1 << 2)                            // Messages can be interpreted
#define FL_ANIM_PLAYONCE (U16)(1 << 3)                               // Animation must only play once
#define FL_ANIM_NEVERAGAIN (U16)(1 << 4)                             // Animation will never restart, even after a reset
#define FL_ANIM_PLAYED (U16)(1 << 5)                                 // Animation has finished playing back
#define FL_ANIM_AUTOSTART (U16)(1 << 6)                              // Animation starts automatically at the beginning of the game
#define FL_ANIM_NOT_RESTART (U16)(FL_ANIM_PLAYONCE | FL_ANIM_PLAYED) // We cannot restart the animation
#define FL_ANIM_MESSAGE (U16)(1 << 7)                                // Message
#define FL_ANIM_ON_PLAY (U16)(FL_ANIM_PLAY | FL_ANIM_STARTED)        // PlayAnim currently playing
#define FL_ANIM_PAUSED (U16)(1 << 8)                                 // Animation is paused
#define FL_ANIM_UNK_0x200 (U16)(1 << 9)                              // Disables update

#define ANIMFRAME_MAX_MSG_ARRAY_SIZE 64

struct KeyFollow_Z : public Key_Z {
    friend class KeyframerFollow_Z;
    friend class AnimFrame_Z;

public:
    KeyFollow_Z() {
        m_NodeHdl = Node_ZHdl();
    }

    KeyFollow_Z(const Float i_Time) {
        SetTime(i_Time);
    }

    ~KeyFollow_Z() { }

    inline const Node_ZHdl& GetNode() const {
        return m_NodeHdl;
    }

    inline const Vec3f& GetAxis() const {
        return m_Axis;
    }

    inline U32 GetOrient() const {
        return m_Orient;
    }

    inline Float GetAvance() const {
        return m_Avance;
    }

    inline void SetNode(const Node_ZHdl& i_NodeHdl) {
        m_NodeHdl = i_NodeHdl;
    }

private:
    Node_ZHdl m_NodeHdl;
    Vec3f m_Axis;
    Float m_Avance;
    U32 m_Orient;
};

class KeyframerFollow_Z {
    friend class AnimFrame_Z;

public:
    inline S32 GetNbKeys() const {
        return m_Keys.GetSize();
    }

    inline KeyFollow_Z& GetKey(U32 i) {
        return m_Keys[i];
    }

    inline const KeyFollow_Z& GetKey(U32 i) const {
        return m_Keys[i];
    }

    inline DynArray_Z<KeyFollow_Z, 4, 1, 1>& GetKeys() {
        return m_Keys;
    }

    inline void SetNbKey(S32 NbKey) {
        m_Keys.SetSize(NbKey);
    }

    inline void SetKey(U32 i, const KeyFollow_Z& Key) {
        m_Keys[i] = Key;
    }

    inline void SetReserve(S32 i_NewReservedSize) {
        m_Keys.SetReserve(i_NewReservedSize);
    }

    inline void AddKey(const KeyFollow_Z& Key) {
        m_Keys.Add(Key);
    }

    inline void RemoveKey(U32 i) {
        m_Keys.Remove(i);
    }

    inline void Flush() {
        m_Keys.Flush();
    }

    Bool GetValue(Float i_Time, Float i_MaxTime, Vec3f& o_Value, Quat& o_Rot, Bool i_Inc);
    void MarkHandles();

private:
    DynArray_Z<KeyFollow_Z, 4, 1, 1> m_Keys;
};

struct StartStop_Z {
    AnimFrame_ZHdl m_AnimHdl; // Animation to start/stop
    U32 m_Value;              // Value (Stop = 0 / Start = 1 )

    U32 GetValue() const {
        return m_Value;
    }

    void SetValue(U32 i_Value) {
        m_Value = i_Value;
    }

    AnimFrame_ZHdl& GetAnim() {
        return m_AnimHdl;
    }

    void SetAnim(const AnimFrame_ZHdl& i_AnimHdl) {
        m_AnimHdl = i_AnimHdl;
    }
};

typedef DynArray_Z<StartStop_Z, 4> StartStop_ZDA;

struct KeyStartStop_Z : public Key_Z {
    friend class AnimFrame_Z;
    friend class KeyframerStartStop_Z;

public:
    KeyStartStop_Z() { }

    KeyStartStop_Z(const Float i_Time) {
        SetTime(i_Time);
    }

    ~KeyStartStop_Z() { }

    inline S32 GetNb() const {
        return m_StartStops.GetSize();
    }

    inline void SetNb(const S32 i_Nb) {
        m_StartStops.SetSize(i_Nb);
    }

    inline StartStop_Z& Get(const S32 i_Index) {
        return m_StartStops[i_Index];
    }

    inline const StartStop_Z& Get(const S32 i_Index) const {
        return m_StartStops[i_Index];
    }

    inline void Remove(const U32 i_Index) {
        m_StartStops.Remove(i_Index);
    }

    inline void Add(const StartStop_Z& i_StartStop) {
        m_StartStops.Add(i_StartStop);
    }

    inline void Minimize() {
        m_StartStops.Minimize();
    }

    inline const StartStop_ZDA& GetStartStops() const {
        return m_StartStops;
    }

private:
    StartStop_ZDA m_StartStops;
};

class KeyframerStartStop_Z {
    friend class AnimFrame_Z;

public:
    inline S32 GetNbKeys() const {
        return m_Keys.GetSize();
    }

    inline KeyStartStop_Z& GetKey(U32 i) {
        return m_Keys[i];
    }

    inline const KeyStartStop_Z& GetKey(U32 i) const {
        return m_Keys[i];
    }

    inline DynArray_Z<KeyStartStop_Z, 32, 1, 1>& GetKeys() {
        return m_Keys;
    }

    inline void SetNbKey(S32 NbKey) {
        m_Keys.SetSize(NbKey);
    }

    inline void SetKey(U32 i, const KeyStartStop_Z& Key) {
        m_Keys[i] = Key;
    }

    inline void SetReserve(S32 i_NewReservedSize) {
        m_Keys.SetReserve(i_NewReservedSize);
    }

    inline void AddKey(const KeyStartStop_Z& Key) {
        m_Keys.Add(Key);
    }

    inline void RemoveKey(U32 i) {
        m_Keys.Remove(i);
    }

    inline void Flush() {
        m_Keys.Flush();
    }

    Bool GetValue(Float i_PrevTime, Float& i_CurTime, Float i_MaxTime, AnimFrame_Z* i_CurAnim);
    void MarkHandles();

private:
    DynArray_Z<KeyStartStop_Z, 32, 1, 1> m_Keys;
};

class AnimFrame_Z : public ResourceObject_Z {
    friend class KeyframerStartStop_Z;

public:
    AnimFrame_Z();

    // clang-format off

    virtual ~AnimFrame_Z();                   /* 0x08 */
    virtual void Load(void** i_Data);         /* 0x0C */
    virtual void EndLoad();                   /* 0x10 */
    virtual Bool MarkHandles();               /* 0x14 */
    virtual void Update(Float i_DeltaTime) {} /* 0x18 */

    // clang-format on

    void ResetFlag();
    void ResetAnim();
    void Update();
    void UpdateMessage(Float i_DeltaTime);
    void DoStart(Float i_StartTime = 0.0f, Float i_CurTime = 0.0f, Float i_PrevTime = -1.0f);
    void Start(Float i_DeltaTime = 0.0f);
    void Stop(Float i_DeltaTime = 0.0f);
    void Pause(Float i_DeltaTime = 0.0f);
    void SetAnimOnTime(Float i_DeltaTime);

    inline Bool IsOn() const {
        return m_PlayFlag & FL_ANIM_PLAY;
    }

private:
    void InitNoteTrack(Node_Z* i_Node);
    void UpdateNoteTrack(Node_Z* i_Node);
    S32 GetAnimatedObjects(Node_Z* i_Node, Light_Z** o_Light, HFog_Z** o_Fog, Omni_Z** o_Omni);

    Node_ZHdl m_AnimatedNodeHdl;
    Float m_MaxTime;
    KeyframerVec3f_Z m_TransKfr;
    KeyframerRot_Z m_RotKfr;
    KeyframerVec3fComp_Z m_ScaleKfr;
    KeyframerFloatComp_Z m_TimeKfr;          // Stores play time for FollowSpline keyframer
    KeyframerFollow_Z m_FollowSplineKfr;     // Moves the node along a spline
    KeyframerStartStop_Z m_StartStopAnimKfr; // Starts/stops other AnimFrames
    KeyframerVec3fLinear_Z m_ColorKfr;       // Used for HFog_Z/Light_Z/Omni_Z
    KeyframerVec3fLinear_Z m_AmbientKfr;     // Used for Light_Z
    KeyframerMessage_Z m_MsgKfr;
    Float m_CurTime;
    Float m_StartTime;
    Float m_PrevTime;
    U16 m_PlayFlag;
};

#endif // _ANIMFRAME_Z_H_
