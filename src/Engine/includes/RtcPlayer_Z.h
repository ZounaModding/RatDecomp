#ifndef _RTCPLAYER_Z_H_
#define _RTCPLAYER_Z_H_
#include "Manipulator_Z.h"
#include "Game_ZHdl.h"
#include "Node_ZHdl.h"
#include "Rtc_ZHdl.h"
#include "RtcAgent_ZHdl.h"
#include "World_ZHdl.h"

class RtcPlayer_Z : public ManipulatorDraw_Z {
public:
    virtual ~RtcPlayer_Z();
    virtual void Init();
    virtual Bool MarkHandles();
    virtual void Reset();
    virtual void Update(Float a1);
    virtual void Draw(const Viewport_Z* a1);
    virtual void InitRtc(const World_ZHdl& a1, const Rtc_ZHdl& a2, RtcAgent_ZHdl a3, Node_ZHdl a4, S32 a5);
    virtual void Start(const Game_ZHdl& a1);
    virtual void UpdateLod(Float a1);

private:
    U8 m_Pad_0x24[0xf0];
};

#endif // _RTCPLAYER_Z_H_
