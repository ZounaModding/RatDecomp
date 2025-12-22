#ifndef _SEQUENCEANIM_ZHDL_H_
#define _SEQUENCEANIM_ZHDL_H_

#include "ClassManager_Z.h"
#include "Program_Z.h"
class BaseObject_Z;
class SequenceAnim_Z;

HANDLE_Z(SequenceAnim_Z, BaseObject_Z);

typedef DynArray_Z<SequenceAnim_ZHdl, 32> SequenceAnim_ZHdlDA;

#endif
