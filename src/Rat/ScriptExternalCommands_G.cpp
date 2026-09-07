#include "ScriptExternalCommands_G.h"
#include "Lod_Z.h"
#include "Node_Z.h"
#include "Object_Z.h"
#include "UserDefine_Z.h"
#include <string.h>

void RegisterExternalScriptCommands() {
}

void EXT_InterpLine(Char* i_Line, Char* o_Command, Char* o_Param1, Char* o_Param2, Char* o_Param3, Char* o_Param4) {
    Char* l_Params[5];
    S32 l_ParamLength = 0;

    *o_Command = '\0';
    *o_Param1 = '\0';
    *o_Param2 = '\0';
    *o_Param3 = '\0';
    l_Params[0] = o_Command;
    *o_Param4 = '\0';
    l_Params[1] = o_Param1;
    l_Params[2] = o_Param2;
    l_Params[3] = o_Param3;
    l_Params[4] = o_Param4;

    Char** l_CurrentParam = l_Params;
    while (*i_Line != '\n' && *i_Line != '\0') {
        if (*i_Line == ' ' || *i_Line == '\t') {
            if (l_ParamLength != 0) {
                **l_CurrentParam = '\0';
                l_ParamLength = 0;
                ++l_CurrentParam;
            }
        }
        else {
            **l_CurrentParam = *i_Line;
            ++*l_CurrentParam;
            ++l_ParamLength;
        }
        ++i_Line;
    }
    **l_CurrentParam = '\0';
}

Bool EXT_Getparameter(Node_Z* i_Node, Char* i_Command, Char* o_Param1, int i_Index, Char* o_Param2, Char* o_Param3) {
    U32 l_CommandLength;
    Char l_CommandName[64];
    Char l_Param1[64];
    Char l_Param2[64];
    Char l_Param3[64];
    Char l_Param4[64];
    Char l_ObjectCommandName[64];
    Char l_ObjectParam1[64];
    Char l_ObjectParam2[64];
    Char l_ObjectParam3[64];
    Char l_ObjectParam4[64];

    if (!i_Node) {
        return FALSE;
    }

    UserDefine_Z* l_UserDefine = i_Node->GetUserDefine();
    if (l_UserDefine) {
        Char* l_Line = l_UserDefine->GetFirstCommand(l_CommandLength);
        S32 l_CommandIndex = 0;
        while (l_Line) {
            EXT_InterpLine(l_Line, l_CommandName, l_Param1, l_Param2, l_Param3, l_Param4);
            if (stricmp(l_CommandName, i_Command) == 0) {
                if (i_Index < 0 || i_Index == l_CommandIndex) {
                    if (o_Param1) {
                        strcpy(o_Param1, l_Param1);
                    }
                    if (o_Param2) {
                        strcpy(o_Param2, l_Param2);
                    }
                    if (o_Param3) {
                        strcpy(o_Param3, l_Param3);
                    }
                    return TRUE;
                }
                ++l_CommandIndex;
            }
            l_Line = l_UserDefine->GetNextCommand(l_CommandLength);
        }
    }

    Object_Z* l_Object = i_Node->GetObjectA();
    if (l_Object && l_Object->GetGeometryType() == LOD_Z) {
        Lod_Z* l_Lod = (Lod_Z*)l_Object;
        if (l_Lod) {
            UserDefine_Z* l_UserDefine = l_Lod->GetUserDefine();
            if (l_UserDefine) {
                S32 l_CommandIndex = 0;
                Char* l_Line = l_UserDefine->GetFirstCommand(l_CommandLength);
                while (l_Line) {
                    EXT_InterpLine(l_Line, l_ObjectCommandName, l_ObjectParam1, l_ObjectParam2, l_ObjectParam3, l_ObjectParam4);
                    if (stricmp(l_ObjectCommandName, i_Command) == 0) {
                        if (i_Index < 0 || i_Index == l_CommandIndex) {
                            if (o_Param1) {
                                strcpy(o_Param1, l_ObjectParam1);
                            }
                            return TRUE;
                        }
                        ++l_CommandIndex;
                    }
                    l_Line = l_UserDefine->GetNextCommand(l_CommandLength);
                }
            }
        }
    }
    return FALSE;
}
