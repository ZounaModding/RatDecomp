#ifndef _ERROR_Z_H_
#define _ERROR_Z_H_
#include "Types_Z.h"
#include <String_Z.h>

#define ERROR_CODE_NONE (U64)(0 << 0)
#define ERROR_CODE_INVALID_DB_PATH (U64)(1 << 27) // 0x8000000

class Error_Z {
public:
    Error_Z(U64 i_ErrorCode, const Char* i_Format = "");
    ~Error_Z();

private:
    String_Z<ARRAY_CHAR_MAX> m_String;
    U64 m_Code;
};

#endif // _ERROR_Z_H_
