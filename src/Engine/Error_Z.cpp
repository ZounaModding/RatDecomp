#include "Error_Z.h"
#include "Program_Z.h"

Error_Z::~Error_Z() {
}

Error_Z::Error_Z(U64 i_Code, const Char* i_From) {
    m_Code = i_Code;
    gData.m_ErrorCodes |= m_Code;

    if (i_From && strlen(i_From) != 0) {
        m_String.Sprintf(" From %s", i_From);
    }
}
