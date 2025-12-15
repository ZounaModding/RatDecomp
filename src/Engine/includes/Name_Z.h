#ifndef _NAME_Z_H_
#define _NAME_Z_H_
#include "Types_Z.h"

class Name_Z {
public:
    Name_Z(const Name_Z& a1) {
        m_ID = a1.m_ID;
    }

    Name_Z(const Char* a1) {
        S32 result = 0;
        if (a1) {
            result = GetID(a1, 0);
        }
        else {
            result = 0;
        }
        m_ID = result;
    }

    Name_Z(const S32 a1) { m_ID = (U32)a1; }

    Name_Z() { m_ID = 0; }

    void SetID(S32 a1) { m_ID = a1; }

    U32 GetID() const {
        return m_ID;
    }

    static U32 GetID(const Char* a1, U32 a2 = 0);
    static U32 GetID(U8* data, U32 size, U32 result);

    inline Name_Z& operator=(const Name_Z& i_Name) {
        m_ID = i_Name.m_ID;
        return (*this);
    }

    Bool operator==(const Name_Z& a1) const { return m_ID == a1.m_ID; }

    Bool operator==(Name_Z a1) { return m_ID == a1.m_ID; }

    Bool operator!=(const Name_Z& a1) const { return m_ID != a1.m_ID; }

    Bool operator!=(Name_Z a1) { return m_ID != a1.m_ID; }

    U32 m_ID;
};

#endif
