#ifndef _CONSOLE_Z_H_
#define _CONSOLE_Z_H_
#include "Types_Z.h"
#include "DynArray_Z.h"
#include <String_Z.h>
#include "Name_Z.h"
#include "PopupMenu_Z.h"
#include "HashTable_Z.h"
#define REGISTERCOMMANDC(a, b, c) gData.Cons->AddCommand(a, b, c);
#define REGISTERCOMMAND(a, b) gData.Cons->AddCommand(a, b, "No Comment");

#define CONSOLE_STACK_COMMAND_LEN_MAX 80
#define CONSOLE_MAX_STACK_COMMAND 2048
#define CONSOLE_STATIC_COMMAND_LEN_MAX 1088
#define CONSOLE_MAX_STATIC_COMMAND 16
#define CONSOLE_STATIC_COMMAND_VAR_LEN_MAX 32
#define CONSOLE_MAX_STATIC_COMMAND_VAR 32

#define FL_CONSOLE_NONE (U32)(0 << 0)
#define FL_CONSOLE_UNK_0x1 (U32)(1 << 0)
#define FL_CONSOLE_UNK_0x2 (U32)(1 << 1)
#define FL_CONSOLE_UNK_0x4 (U32)(1 << 2)
#define FL_CONSOLE_UNK_0x8 (U32)(1 << 3)
#define FL_CONSOLE_UNK_0x10 (U32)(1 << 4)
#define FL_CONSOLE_PAUSED (U32)(1 << 5)
#define FL_CONSOLE_UNK_0x40 (U32)(1 << 6)

Bool DisplayHelp();
Bool Pause();
Bool Source();
class ConsoleInterp_Z;
class DrawInfo_Z;

typedef Bool (*CommandProc)();

struct Command_Z {
    S32 m_Command;
    S32 m_Alias;
    CommandProc m_Proc;
    Command_Z* m_Prev;
};

struct ConsoleUnkBuffer_Z {
    U8 m_UnkBuf0x0[4976];

    ConsoleUnkBuffer_Z() {
        memset(&m_UnkBuf0x0, 0, (S32)((S32)m_UnkBuf0x0 + sizeof(m_UnkBuf0x0)) - (S32)&m_UnkBuf0x0);
    }
};

enum ConsoleId {
    ConsoleSystem,
    ConsoleMessage,
    ConsoleScript,
    ConsoleLevel,
    ConsoleProfiler,
    ConsoleUser,
    ConsoleNbFolder
};

class ConsoleUnkS32_Z {
public:
    S32 m_UnkU32_0x1384;
    S32 m_UnkU32_0x1388;
    S32 m_UnkU32_0x138c;
    S32 m_UnkU32_0x1390;

    ConsoleUnkS32_Z() {
        m_UnkU32_0x1390 = 0;
        m_UnkU32_0x138c = 0;
        m_UnkU32_0x1388 = 0;
        m_UnkU32_0x1384 = 0;
    }
};

class Console_Z {
protected:
    Char* m_ConsoleText;
    S32 m_UnkS32_0x4;
    DynArray_Z<Char> m_TextBolded;
    ConsoleUnkBuffer_Z m_ConsoleUnkBuffer;
    U32 m_UnkU32_0x1380;
    ConsoleUnkS32_Z m_UnkS32List_0x1384;
    U32 m_UnkU32_0x1394;
    U32 m_UnkU32_0x1398;
    U32 m_UnkU32_0x139c;
    U32 m_UnkU32_0x13a0;
    U32 m_UnkU32_0x13a4;
    U32 m_UnkU32_0x13a8;
    Bool m_UnkBool_0x13ac;
    U8 m_Pad_0x13ad[1599];
    U32 m_UnkCount_0x19F0;
    U8 m_Pad_0x19F4[2492];
    PopupMenu_Z* m_PopupMenu;

private:
    Command_Z* m_CommandList;
    U32 m_CommIndex;
    U32 m_MaxComm;
    U32 m_UnkU32_0x23bc;
    String_Z<CONSOLE_STATIC_COMMAND_LEN_MAX> m_CommandStack[CONSOLE_MAX_STATIC_COMMAND];
    S32 m_NbParam;
    Char* m_StrParam[16];
    Bool m_IsFloatParam[16];
    Float m_FloatParam[16];
    S32 m_CommandNbVar;
    String_Z<CONSOLE_STATIC_COMMAND_VAR_LEN_MAX> m_CommandVar[CONSOLE_MAX_STATIC_COMMAND_VAR];
    S32 m_StackNbVar;
    Bool m_StackVarState[8];

protected:
    U32 m_FolderFlag;

private:
    U32 m_Flag;
    U32 m_Depth;
    Bool m_UnkBool_0x6c70;
    ConsoleInterp_Z* m_Interp;

public:
    Console_Z();
    void MarkHandles();
    void InterpFileBuffer(const Char* i_FileBuffer, S32 i_Unk);
    U32 GetCurrentInterpSize();
    void* GetCurrentInterpBuffer();
    void SetVar(const Char* i_Var);
    void UnVar(const Char* i_Var);
    void PushVar(Bool i_State);
    Bool IsVar(const Char* i_Var);
    void ChangeVarState();
    void DisplayHelp();
    void AddCommand(const Char* i_Command, CommandProc i_Proc, const Char* i_Desc);
    Bool LaunchCommand(const Char* a1, const Char* i_CommandStr, U32 i_Depth, Command_Z* o_Command);

    S32 GetNbParam() const {
        return m_NbParam;
    }

    Char* GetMessageBuffer() {
        return m_ConsoleText;
    }

    Bool InterpCommandLine(const Char* i_CommandStr, U32 i_Depth);
    void InterpFile();
    Bool NewCommand(const Char* i_CommandStr, U32 i_Depth);
    void PushCommand(const Char* i_CommandLine, Bool i_TopOfStack = FALSE);
    S32 NbPushedCommand();
    Bool InterpCommand(const Char* i_CommandStr, U32 i_Depth = 0);
    Bool InterpFloat(const Char* i_CommandStr, Float& o_Value);

    ConsoleInterp_Z* GetInterp() const {
        return m_Interp;
    }

    void SetInterp(ConsoleInterp_Z* i_Interp) {
        m_Interp = i_Interp;
    }

    Command_Z* IsCommand(const Name_Z& l_CommandName) const;

    Char* GetStrParam(U32 i_Index) const {
        return m_StrParam[i_Index];
    }

    Float GetParamFloat(U32 i_Index) const {
        return m_FloatParam[i_Index];
    }

    Bool IsParamFloat(U32 i_Index) const {
        return m_IsFloatParam[i_Index];
    }

    inline void PopVar() {
        m_StackNbVar--;
    }

    U32 IsFlagEnable(U32 i_Flag) const {
        return m_Flag & i_Flag;
    }

    virtual ~Console_Z();
    virtual void Init();

    virtual Bool InitConsole() { return FALSE; }

    virtual void CloseConsole() { };

    virtual void EnableFolder(U32 a1);
    virtual void DisableFolder(U32 a1);

    virtual void EnableFlag(U32 i_Flag) { }

    virtual void DisableFlag(U32 i_Flag);
    virtual void Update(Float a1);
    virtual void Draw(DrawInfo_Z& i_DrawInfo);

    virtual PopupMenu_Z* GetPopupMenu() const {
        return NULL;
    }

    virtual void Flush() { }

    virtual void DisplayStatus(U32 a1, const Char* a2) { }

    virtual void MessageStatus(U32 a1, const Char* a2, ...);

    virtual void SaveMessage(const Char* a1) { }

    virtual void SaveWarningMessage(const Char* a1) { }

    virtual void MessageError(Bool a1, U64 a2, const Char* a3, va_list& i_Args);

    static void PrintBoxString(const Char*, const Char*, ...);
    static void PrintErrorString(U64 i_ErrorCode, const Char* i_Format, ...);
};

#endif
