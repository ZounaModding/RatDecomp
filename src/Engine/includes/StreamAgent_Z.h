#ifndef _STREAMAGENT_Z_H_
#define _STREAMAGENT_Z_H_
#include "ABC_Agent.h"
#include "StreamFile_ZHdl.h"
// clang-format off

BEGIN_AGENT_CLASS(StreamAgent_Z, ABC_Agent, 8)
public:
    StreamAgent_Z() {
        EnableFlag(FL_AGENT_UNK_0x100000);
    }

    virtual ~StreamAgent_Z() { }                                          /* 0x08 */
    virtual Bool MarkHandles();                                           /* 0x0C */

    DECL_BHV(StreamEnd)
    DECL_BHV(StreamAborted)
    DECL_BHV(StreamError)
    DECL_BHV(StreamResumed)

    virtual void Start();                                                 /* 0x30 */
    virtual void End();                                                   /* 0x34 */
    virtual void Resume(S32 i_Cause);                                     /* 0x38 */
    virtual void Resumed();                                               /* 0x3C */
    virtual void Abort();                                                 /* 0x40 */
    virtual void Aborted();                                               /* 0x44 */

private:
    StreamFile_ZHdl m_StreamHdl;
END_AGENT_CLASS

// clang-format on
#endif // _STREAMAGENT_Z_H_
