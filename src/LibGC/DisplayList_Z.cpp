#include "DisplayList_Z.h"
#include "Sys_Z.h"

void DisplayList_Z::Load(void** i_Data) {
    LOAD_Z(m_CurrSize);
    LOAD_Z(m_Size);
    m_Data = (U8*)AllocAlignL_Z(m_CurrSize, 11, 32);
    LOADBYTES_Z(m_Data, m_CurrSize);
    DCStoreRange(m_Data, m_CurrSize);
}
