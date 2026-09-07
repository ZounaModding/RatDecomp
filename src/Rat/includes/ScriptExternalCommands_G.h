#ifndef _SCRIPTEXTERNALCOMMANDS_G_H_
#define _SCRIPTEXTERNALCOMMANDS_G_H_
#include "Types_Z.h"

class Node_Z;

void RegisterExternalScriptCommands();
void EXT_InterpLine(Char* i_Line, Char* o_Command, Char* o_Param1, Char* o_Param2, Char* o_Param3, Char* o_Param4);
Bool EXT_Getparameter(Node_Z* i_Node, Char* i_Command, Char* o_Param1, int i_Index, Char* o_Param2, Char* o_Param3);

#endif // _SCRIPTEXTERNALCOMMANDS_G_H_
