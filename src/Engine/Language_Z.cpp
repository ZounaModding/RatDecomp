#include "Language_Z.h"
#include "Name_Z.h"
#include "Types_Z.h"
#include "Assert_Z.h"
#include "Main_Z.h"
#include "Program_Z.h"
#include "SoundManager_Z.h"
#include "Console_Z.h"

static const Char s_Unused_1422[] = "Empty Text";

#define LANG_TO_STRING(Lang) "PATCH_STR_" #Lang

Name_Z CharNames[20] = {
    Name_Z::GetID("EMPTY", 0),
    Name_Z::GetID("CROSS", 0),
    Name_Z::GetID("CIRCLE", 0),
    Name_Z::GetID("SQUARE", 0),
    Name_Z::GetID("TRIANGLE", 0),
    Name_Z::GetID("UPDOWN", 0),
    Name_Z::GetID("LEFTRIGHT", 0),
    Name_Z::GetID("DIRPAD", 0),
    Name_Z::GetID("L1", 0),
    Name_Z::GetID("R1", 0),
    Name_Z::GetID("L2", 0),
    Name_Z::GetID("R2", 0),
    Name_Z::GetID("L3", 0),
    Name_Z::GetID("R3", 0),
    Name_Z::GetID("UP", 0),
    Name_Z::GetID("DOWN", 0),
    Name_Z::GetID("LEFT", 0),
    Name_Z::GetID("RIGHT", 0),
    Name_Z::GetID("SELECT", 0),
    Name_Z::GetID("START", 0),
};

Language_Z LangValid[LANG_COUNT + 1] = {
    { LANG_ENGLISH_Z, LANG_TYPE_NONE, LANG_TO_STRING(LANG_ENGLISH_Z) },
    { LANG_JAPANESE_Z, LANG_TYPE_NONE, LANG_TO_STRING(LANG_JAPANESE_Z) },
    { LANG_FRENCH_Z, LANG_TYPE_NONE, LANG_TO_STRING(LANG_FRENCH_Z) },
    { LANG_SPANISH_Z, LANG_TYPE_NONE, LANG_TO_STRING(LANG_SPANISH_Z) },
    { LANG_GERMAN_Z, LANG_TYPE_NONE, LANG_TO_STRING(LANG_GERMAN_Z) },
    { LANG_ITALIAN_Z, LANG_TYPE_NONE, LANG_TO_STRING(LANG_ITALIAN_Z) },
    { LANG_DUTCH_Z, LANG_TYPE_NONE, LANG_TO_STRING(LANG_DUTCH_Z) },
    { LANG_PORTUGUESE_Z, LANG_TYPE_NONE, LANG_TO_STRING(LANG_PORTUGUESE_Z) },
    { LANG_COUNT, LANG_TYPE_NONE, "" }
};

LangTextDA Language_Z::LangTextPtrs; // Stores the translation text id and the index of the first char of the string in LangTextTxt
CharDA Language_Z::LangTextTxt;      // Array of chars for all the strings, indexed by LangTextPtrs
LanguageEnum_Z Language_Z::LangLoaded = LANG_NONE;
LanguageEnum_Z Language_Z::LangForMpeg;
LanguageEnum_Z Language_Z::LangForWav = LANG_NONE;
S32 Language_Z::CurChar; // Index of last char in LangTextTxt (where to add the next string)

U8 GetCharIdFromText(Char* i_Text) {
    Name_Z i_Name;
    for (S32 i = 0; i < 20; i++) {
        if (i_Text != NULL) {
            i_Name = Name_Z::GetID(i_Text, 0);
        }
        else {
            i_Name.m_ID = 0;
        }
        if (CharNames[i] == i_Name) {
            return i;
        }
    }
    return 0xFF;
}

// $SABE: It's mispelled on Wii even though it's completely inlined on GC, so let's use the mispelled name
Bool FintLangTextEntry(U32 i_Id, U32& o_TxtIdx) {
    U32 l_Last;
    U32 l_First;
    if (Language_Z::LangTextPtrs.GetSize() == 0) {
        o_TxtIdx = 0;
        return FALSE;
    }
    else {
        l_First = 0;
        l_Last = Language_Z::LangTextPtrs.GetSize() - 1;

        while (TRUE) {
            o_TxtIdx = (l_Last + l_First) / 2;

            if (i_Id == Language_Z::LangTextPtrs[o_TxtIdx].m_TrTextId) {
                return TRUE;
            }

            if (Language_Z::LangTextPtrs[o_TxtIdx].m_TrTextId > (U32)i_Id) {
                l_Last = o_TxtIdx;
                if (o_TxtIdx == l_First) {
                }
                else {
                    continue;
                }
            }
            else {
                if (l_First == l_Last - 1) {
                    l_First = l_Last;
                    continue;
                }

                l_First = o_TxtIdx;
                if (l_Last != o_TxtIdx) {
                    continue;
                }
                o_TxtIdx = o_TxtIdx + 1;
            }

            return FALSE;
        }
    }
}

Char* TT(int i_Id) {
    if (Language_Z::LangLoaded == LANG_NONE) {
        return "";
    }
    if (i_Id < 0) {
        return "";
    }
    if (i_Id > LANGTEXT_MAX_TRTEXT_ID) {
        return "";
    }

    U32 l_Idx;
    Bool l_Found = FintLangTextEntry(i_Id, l_Idx);

    if (!l_Found) {
        return "";
    }

    return &Language_Z::LangTextTxt[Language_Z::LangTextPtrs[l_Idx].m_TxtIdx];
}

void AddString(int i_TrTextId, Char* i_Text) {
    if (Language_Z::LangLoaded == LANG_NONE) {
        return;
    }
    if (i_Text == NULL) {
        return;
    }
    if (i_TrTextId >= LANGTEXT_MIN_TRTEXT_ID && i_TrTextId <= LANGTEXT_MAX_TRTEXT_ID) {
        Char* l_StrFormat = strstr(i_Text, "STR_");
        if (l_StrFormat) {
            LanguageHandleSTR(i_Text);
        }
        S32 l_TextLen = strlen(i_Text);
        if (l_TextLen < 1) {
            return;
        }

        if (Language_Z::CurChar + l_TextLen + 1 > DYA_SIZEMAX - 1) {
            return;
        }

        U32 l_Idx;
        Bool l_Found = FintLangTextEntry(i_TrTextId, l_Idx);
        if (!l_Found) {
            if (l_Idx == Language_Z::LangTextPtrs.GetSize()) {
                Language_Z::LangTextPtrs.Add();
            }
            else {
                Language_Z::LangTextPtrs.Insert(l_Idx);
            }
        }
        Language_Z::LangTextTxt.SetSize(Language_Z::CurChar + l_TextLen + 1);
        strcpy(&Language_Z::LangTextTxt[Language_Z::CurChar], i_Text);
        Language_Z::LangTextPtrs.GetArrayPtr()[l_Idx].m_TrTextId = i_TrTextId;
        Language_Z::LangTextPtrs.GetArrayPtr()[l_Idx].m_TxtIdx = Language_Z::CurChar;
        Language_Z::CurChar += l_TextLen + 1;
    }
}

int SetLanguage(int i_TextLang, int i_SoundLang, int i_MovieLang) {
    if (i_TextLang < 0) {
        i_TextLang = GetLanguage();
    }
    LanguageEnum_Z l_LangForWav = (LanguageEnum_Z)i_TextLang;
    if (i_SoundLang > 0) {
        l_LangForWav = (LanguageEnum_Z)i_SoundLang;
    }
    Language_Z::LangForWav = l_LangForWav;
    Language_Z::LangForMpeg = (LanguageEnum_Z)i_MovieLang;
    if (Language_Z::LangLoaded == i_TextLang) {
        return i_TextLang;
    }
    String_Z<ARRAY_CHAR_MAX> l_CommandBSource;
    Language_Z::LangLoaded = (LanguageEnum_Z)i_TextLang;
    l_CommandBSource.Sprintf("BSource TRTEXT\\TT%02d.%s", i_TextLang, "GC");
    gData.Cons->PushCommand("DisableConsole ON");
    gData.Cons->PushCommand(l_CommandBSource);
    gData.Cons->PushCommand("DisableConsole OFF");
    String_Z<ARRAY_CHAR_MAX> l_SoundBFName;
    l_SoundBFName.Sprintf("\\MUSIC\\DIALOG\\%02d", Language_Z::LangForWav);
    l_SoundBFName.Upr();
    gData.SoundMgr->CloseStreamBF();
    gData.SoundMgr->OpenStreamBF(l_SoundBFName);
    return i_TextLang;
}

Bool FreeLanguage() {
    Language_Z::CurChar = 0;
    Language_Z::LangTextPtrs.SetSize(0);
    Language_Z::LangTextTxt.SetSize(0);
    return TRUE;
}

LanguageEnum_Z GetLanguage() {
    LanguageEnum_Z l_Lang = Language_Z::LangLoaded;
    if (Language_Z::LangLoaded != LANG_NONE) {
        return l_Lang;
    }
    l_Lang = ConsoleConfiguration::GetLanguage();
    S32 i = 0;
    while (TRUE) {
        ASSERTLE_Z(LangValid[i].m_Lang != LANG_COUNT, "BAD Language in LanguageZ.cpp", 294, "LangValid[i].Langue != LANG_COUNT");
        if (LangValid[i].m_Lang == LANG_COUNT) {
            break;
        }
        if (LangValid[i].m_Lang == l_Lang) {
            if (LangValid[i].m_LangType != LANG_TYPE_NONE) {
                return l_Lang;
            }
            break;
        }
        i++;
    }
    do {
        if (LangValid[i].m_LangType == LANG_TYPE_MAIN) {
            return LangValid[i].m_Lang;
        }
        if (LangValid[i].m_Lang == LANG_COUNT) {
            break;
        }
        i++;
    } while (TRUE);
    return LANG_ENGLISH_Z;
}

void ValidLanguage(LanguageEnum_Z i_Lang, Bool i_IsMain) {
    S32 i = 0;
    while (TRUE) {
        if (LangValid[i].m_Lang != LANG_COUNT) {
            if (LangValid[i].m_Lang == i_Lang) {
                if (i_IsMain) {
                    LangValid[i].m_LangType = LANG_TYPE_MAIN;
                }
                else {
                    LangValid[i].m_LangType = LANG_TYPE_ALTERNATIVE;
                }
                return;
            }
            i++;
        }
        else {
            ASSERTLE_Z(LangValid[i].m_Lang != LANG_COUNT, "BAD Valid Language in LanguageZ.cpp", 332, "LangValid[i].Langue != LANG_COUNT");
            return;
        }
    }
}

LanguageEnum_Z GetLanguageForMPEG() {
    return Language_Z::LangForMpeg;
}

LanguageEnum_Z GetLanguageForWAVE() {
    return Language_Z::LangForWav;
}

Char EmptyWithNum[512];
