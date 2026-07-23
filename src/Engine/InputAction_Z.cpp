#include "InputAction_Z.h"

S32 InputActionContext_Z::AddAction(S32 i_ActionId) {
    S32 l_ActionIdx = m_Actions.Add();
    m_Actions[l_ActionIdx].m_DeviceIdx = m_DeviceIdx;
    m_Actions[l_ActionIdx].m_ActionId = i_ActionId;
    return l_ActionIdx + m_FirstActionIdx;
}

InputAction_Z& InputActionContext_Z::GetAction(S32 i_ActionIdx) {
    return m_Actions[(U16)i_ActionIdx];
}

void InputActionContext_Z::Update(Float i_DeltaTime) {
}
