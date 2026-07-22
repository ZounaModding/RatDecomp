#ifndef _IT_SOUND_G_H_
#define _IT_SOUND_G_H_
#include "Agent_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(IT_SOUND, Agent_Z, 25)
public:
    IT_SOUND();

    virtual ~IT_SOUND() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct IT_SOUND` for Ghidra's inferred layout
END_AGENT_CLASS

// clang-format on
#endif // _IT_SOUND_G_H_
