#ifndef _PARTICLESAGENT_ZHDL_H_
#define _PARTICLESAGENT_ZHDL_H_

#include "Agent_ZHdl.h"
class ParticlesAgent_Z;
class Agent_Z;

HANDLE_Z(ParticlesAgent_Z, Agent_Z);

typedef DynArray_Z<ParticlesAgent_ZHdl, 8> ParticlesAgent_ZHdlDA;

#endif
