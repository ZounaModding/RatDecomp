#ifndef _STREAMGAMEAGENT_G_H_
#define _STREAMGAMEAGENT_G_H_
#include "StreamAgent_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(StreamGameAgent_G, StreamAgent_Z, 7)
public:
    StreamGameAgent_G();

    virtual ~StreamGameAgent_G() {}
    virtual void Init();
private:
END_AGENT_CLASS

// clang-format on
#endif // _STREAMGAMEAGENT_G_H_
