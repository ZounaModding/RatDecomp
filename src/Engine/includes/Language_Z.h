#ifndef _LANGUAGE_Z_H_
#define _LANGUAGE_Z_H_
#include "Types_Z.h"
#include "Name_Z.h"
#include "DynArray_Z.h"

#define LANGTEXT_TXT_INDEX_BITS 18
#define LANGTEXT_TT_ID_BITS (32 - LANGTEXT_TXT_INDEX_BITS)
#define LANGTEXT_MIN_TRTEXT_ID 0
#define LANGTEXT_MAX_TRTEXT_ID ((1 << LANGTEXT_TT_ID_BITS) - 1)

enum LanguageEnum_Z {
    LANG_NONE = -1,
    LANG_JAPANESE_Z = 0,
    LANG_ENGLISH_Z = 1,
    LANG_FRENCH_Z = 2,
    LANG_SPANISH_Z = 3,
    LANG_GERMAN_Z = 4,
    LANG_ITALIAN_Z = 5,
    LANG_DUTCH_Z = 6,
    LANG_PORTUGUESE_Z = 7,
    LANG_COUNT = 8
};

enum LanguageType_Z {
    LANG_TYPE_NONE = 0,
    LANG_TYPE_ALTERNATIVE = 1,
    LANG_TYPE_MAIN = 2
};

// Global functions

U8 GetCharIdFromText(Char* i_Text);
// $SABE: It's mispelled on Wii even though it's completely inlined on GC, so let's use the mispelled name
Bool FintLangTextEntry(U32 i_Id, U32& o_TxtIdx);
Char* TT(int i_TrTextId);
void AddString(int i_TrTextId, Char* i_Text);
int SetLanguage(int i_TextLang, int i_SoundLang, int i_MovieLang);
Bool FreeLanguage();
LanguageEnum_Z GetLanguage();
void ValidLanguage(LanguageEnum_Z i_Lang, Bool i_IsMain);
LanguageEnum_Z GetLanguageForMPEG();
LanguageEnum_Z GetLanguageForWAVE();

// Game defined functions

Extern_Z Bool LanguageHandleSTR(Char* i_Text);

typedef DynArray_Z<Char, 256> CharDA;

struct Language_Z {
    // clang-format off

    struct LangText {
        U32 m_TrTextId : LANGTEXT_TT_ID_BITS,   // Id of the translation text (specified in ttXX.xx)
            m_TxtIdx : LANGTEXT_TXT_INDEX_BITS; // Index of the first char of the string in LangTextTxt
    };

    // clang-format on

    typedef DynArray_Z<LangText, 64> LangTextDA;

    // Static members

    static LangTextDA LangTextPtrs; // Stores the translation text id and the index of the first char of the string in LangTextTxt
    static CharDA LangTextTxt;      // Array of chars for all the strings, indexed by LangTextPtrs
    static LanguageEnum_Z LangLoaded;
    static LanguageEnum_Z LangForMpeg;
    static LanguageEnum_Z LangForWav;
    static S32 CurChar; // Index of last char in LangTextTxt (where to add the next string)

    // Instance members

    LanguageEnum_Z m_Lang;
    LanguageType_Z m_LangType;
    Char m_LangName[32];

    void SetLangType(LanguageType_Z i_LangType) {
        m_LangType = i_LangType;
    }
};

typedef DynArray_Z<Language_Z::LangText, 64> LangTextDA;

#endif
