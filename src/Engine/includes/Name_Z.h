#ifndef _NAME_Z_H_
#define _NAME_Z_H_
#include "Types_Z.h"
#include <String_Z.h>

class Name_Z {
public:
    Name_Z(const Char* i_Str) {
        SetName(i_Str);
    }

    Name_Z(int i_ID) { m_ID = (U32)i_ID; }

    Name_Z(const String_Z<ARRAY_CHAR_MAX>& i_Str) {
        SetName(i_Str);
    }

    Name_Z() { m_ID = 0; }

    U32 GetID() const {
        return m_ID;
    }

    inline void SetName(const Char* i_Str) {
        int l_Res = 0;
        if (i_Str) {
            l_Res = GetID(i_Str, 0);
        }
        else {
            l_Res = 0;
        }
        m_ID = l_Res;
    }

    static int GetID(const Char* i_Str, U32 i_ContinueCRC = 0);
    static int GetID(U8* i_Data, U32 i_Len, U32 i_ContinueCRC = 0);

    inline Name_Z& operator=(const Name_Z& i_Name) {
        m_ID = i_Name.m_ID;
        return (*this);
    }

    Bool operator==(const Name_Z& i_Name) const { return m_ID == i_Name.m_ID; }

    Bool operator==(Name_Z& i_Name) { return m_ID == i_Name.m_ID; }

    Bool operator==(const Name_Z& i_Name) { return m_ID == i_Name.m_ID; }

    Bool operator==(Name_Z& i_Name) const { return m_ID == i_Name.m_ID; }

    Bool operator!=(const Name_Z& i_Name) const { return m_ID != i_Name.m_ID; }

    Bool operator!=(const Name_Z& i_Name) { return m_ID != i_Name.m_ID; }

    U32 m_ID;
};

#endif
