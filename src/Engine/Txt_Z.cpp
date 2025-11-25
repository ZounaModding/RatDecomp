#include "Txt_Z.h"
#include "Sys_Z.h"

void Txt_Z::Load(void** i_Data) {
    S32 l_Unk;
    LOAD_Z(l_Unk);
    m_File.Init(l_Unk, NULL);
    LOADBYTES_Z(m_File.GetBuf(), m_File.GetSize());
}
