#include "KSys_Z.h"
#include "Handle_Z.h"
#include "Program_Z.h"
#include "Types_Z.h"
#include "Error_Z.h"

ExternC_Z U32 strlen(const char* str);
ExternC_Z int strncmp(const char* str1, const char* str2, int n);
ExternC_Z Char* strpbrk(const Char* str, const Char* accept);
ExternC_Z int stricmp(const char* String1, const char* String2);
ExternC_Z Char* strstr(Char* str, const char* substr);

// $SABE TODO: FIX
void PathKToSys(const Char* i_KName, Char* o_SysName) {
    U32 l_Len = strlen(i_KName);
    U32 i;

    for (i = 0; i < l_Len; i++) {
        if (i_KName[i] == KSYS_DIRECTORY_CHAR) {
            o_SysName[i] = '\\';
        }
        else {
            o_SysName[i] = i_KName[i];
        }
    }

    o_SysName[i] = 0;
}

void PathKDBToSys(const Char* i_KName, Char* o_SysName) {
    Char l_KName[ARRAY_CHAR_MAX];
    Char l_MatchName[5];

    Char* l_KNamePtr = l_KName;
    l_KNamePtr[0] = 0;
    PathKToSys(i_KName, l_KNamePtr);

    if (strncmp(l_KName, "DB:", KSYS_DB_SIZE) == 0) {
        if (strpbrk(l_KName, "\xE9\xE8\xF9\xE7\xE0\xF9^\xF9")) {
            U64 l_OldErrorCodes = gData.m_ErrorCodes;

            gData.m_ErrorCodes = ERROR_CODE_NONE;

            Error_Z l_Error(ERROR_CODE_INVALID_DB_PATH);
            Console_Z::PrintErrorString(ERROR_CODE_INVALID_DB_PATH, "Macintosh non supported file name : %s", l_KName);

            gData.m_ErrorCodes = l_OldErrorCodes;
        }
    }

    if (gData.ClassMgr->IsBigFileRead()) {
        S32 l_BFMatchListLen = strlen(gData.m_BFMatchList);
        if (l_BFMatchListLen) {
            S32 l_BFPathListLen = strlen(gData.m_BFPathList);
            if (l_BFPathListLen) {
                strcpy(o_SysName, gData.m_AliasPath);

                if (strncmp(l_KName, "DB:", KSYS_DB_SIZE) == 0) {
                    strcat(o_SysName, l_KName + KSYS_DB_SIZE);
                }
                else {
                    strcat(o_SysName, l_KName);
                }

                if (File_Z::DoFileExists(o_SysName)) {
                    return;
                }

                Char* l_Match = strstr(gData.m_BFPathList, "*");
                const Char* l_DBName = l_KName + KSYS_DB_SIZE;

                while (l_Match) {
                    strncpy(l_MatchName, l_Match, 4);
                    l_MatchName[4] = 0;

                    if (strstr(l_KName, l_MatchName)) {
                        const Char* l_BFPath = gData.m_BFMatchList;

                        while (*l_BFPath) {
                            strcpy(o_SysName, l_BFPath);

                            if (strncmp(l_KName, "DB:", KSYS_DB_SIZE) == 0) {
                                strcat(o_SysName, l_DBName);
                            }
                            else {
                                strcat(o_SysName, l_KName);
                            }

                            if (File_Z::DoFileExists(o_SysName)) {
                                return;
                            }

                            l_BFPath += strlen(l_BFPath) + 1;
                        }
                    }

                    l_Match = strstr(l_Match + 1, "*");
                }
            }
        }
    }

    if (strncmp(l_KName, "DB:", KSYS_DB_SIZE) == 0) {
        strcpy(o_SysName, gData.m_DBPath);
        strcat(o_SysName, l_KName + KSYS_DB_SIZE);
    }
    else {
        strcpy(o_SysName, l_KName);
    }
}

void PathSysToDB(const Char* i_SysName, Char* o_KName) {
}

Bool SetDBPath() {
    return FALSE;
}

Bool SetDFPath() {
    return FALSE;
}

Bool SetBFPath() {
    return FALSE;
}

Bool EnableBF() {
    if (gData.Cons->GetNbParam() < 2) {
        return FALSE;
    }

    if (stricmp(gData.Cons->GetStrParam(1), "Read") && stricmp(gData.Cons->GetStrParam(1), "Write")) {
        return FALSE;
    }

    gData.ClassMgr->InitBigFile(!stricmp(gData.Cons->GetStrParam(1), "Read"));

    return TRUE;
}
