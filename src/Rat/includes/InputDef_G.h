#ifndef _INPUTDEF_G_H_
#define _INPUTDEF_G_H_
#include "Types_Z.h"

#define ADD_INPUT_ACTION(index, actionId, baseValue)                     \
    m_Actions[index] = l_ActionContext.AddAction(actionId);              \
    l_ActionContext.GetAction(m_Actions[index]).SetBaseValue(baseValue); \
    l_ActionContext.GetAction(m_Actions[index]).SetMinMax(baseValue, 0.9f)

#endif // _INPUTDEF_G_H_
