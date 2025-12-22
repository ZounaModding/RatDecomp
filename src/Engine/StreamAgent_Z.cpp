#include "StreamAgent_Z.h"
#include "ABC_Message_Engine.h"
#include "StreamFile_Z.h"

// clang-format off

BEGIN_INIT_AGENT_CLASS(StreamAgent_Z, ABC_Agent)
    ADD_BEHAVIOR(StreamEnd)
    ADD_BEHAVIOR(StreamAborted)
    ADD_BEHAVIOR(StreamError)
    ADD_BEHAVIOR(StreamResumed)
END_INIT_AGENT_CLASS

BEHAVIOR(StreamAgent_Z,StreamEnd)
    CATEGORY(cat_running_stream)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_stream_end)
    ACTION
        End();
END_BEHAVIOR

BEHAVIOR(StreamAgent_Z,StreamError)
    CATEGORY(cat_running_stream)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_stream_error)
    ACTION
        Resume(m_StreamHdl->GetStream().GetErrorCode());
END_BEHAVIOR

BEHAVIOR(StreamAgent_Z,StreamResumed)
    CATEGORY(cat_running_stream)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_stream_resumed)
    ACTION
        Resumed();
END_BEHAVIOR

void StreamAgent_Z::Resumed() { }

BEHAVIOR(StreamAgent_Z,StreamAborted)
    CATEGORY(cat_running_stream)
BEGIN_BEHAVIOR
    CONDITION
        Message(msg_stream_aborted)
    ACTION
        Aborted();
END_BEHAVIOR

// clang-format on
