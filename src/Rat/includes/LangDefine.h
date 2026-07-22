#ifndef _LANGDEFINE_H_
#define _LANGDEFINE_H_
#include "DynArray_Z.h"
#include "Language_Z.h"

struct langDefine {
    S32 m_LangNameTrTextId;
    S32 m_TrTextId;
    S32 m_DialogId;
    S32 m_MpegId;
    LanguageEnum_Z m_Lang;
};

typedef DynArray_Z<langDefine, 8> langDefineDA;

struct ArrayLang {
    ArrayLang();

    langDefineDA m_LangDefines;
};
#endif // _LANGDEFINE_H_
