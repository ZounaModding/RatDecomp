#include "GCMain_Z.h"
#include "Assert_Z.h"
#include "LowMemory_Z.h"
#include "GCConsole_Z.h"

GCGlobals gData;

Extern_Z void GetFlagsFromGame();
Extern_Z void MathInitTables();
Extern_Z Char* strcpy(Char* __dest, Char* __src);

Extern_Z "C" void OSInit();
Extern_Z "C" void DVDInit();
Extern_Z "C" void OSReport(const Char* __msg, ...);
Extern_Z "C" void __start();

Extern_Z "C" void exit(S32 __status);
Extern_Z "C" void* OSGetArenaHi();
Extern_Z "C" void* OSGetArenaLo();

Extern_Z "C" void* _stack_end;
Extern_Z "C" void* _stack_addr;
Extern_Z "C" void* _db_stack_end;
Extern_Z "C" void* _db_stack_addr;

// $SABE: Not fully matching yet, need to fix
void PrintMemoryStatus(Char* i_Comment) {
    void* l_ArenaLo = OSGetArenaLo();
    void* l_ArenaHi = OSGetArenaHi();
    void** l_StackEnd = &_stack_end;
    void** l_StackAddr = &_stack_addr;
    void** l_DbStackAddr = &_stack_addr;
    void** l_DbStackEnd = &_db_stack_end;
    U32 l_FreeMem = MemManager.GetFreeMem();
    U32 l_HeapSize = MemManager.GetHeapSize();
    U32 l_SizeInKo = ((U32)l_ArenaHi - (U32)l_ArenaLo) >> 10;
    void* l_End = (void*)0x803c317e;

    OSReport("\n\n");
    OSReport("> %s :\n", i_Comment ? i_Comment : "Memory Status");
    OSReport(">              start       end         size         usage\n");
    OSReport(">     ELF      0x%08x  0x%08x  %08d ko\n", (U32)&__start, (U32)l_End, 3836);
    OSReport(">     STACK    0x%08x  0x%08x  %08d ko\n", l_StackEnd, l_StackAddr, 256);
    OSReport(">     DBSTACK  0x%08x  0x%08x  %08d ko\n", l_DbStackEnd, &l_DbStackAddr, 64);
    OSReport(">     HEAP     0x%08x  0x%08x  %08d ko  %.2f mo\n", l_ArenaLo, l_ArenaHi, l_SizeInKo, l_HeapSize);
    OSReport("\n\n");
}

void InitProgram() {
    OSInit();
    ExceptionHandler();
    MemManager.Init();
    *(S32*)(&gData.m_Pad_0x14[0x794]) = 0;
    GetFlagsFromGame();
    gData.Cons = NewL_Z(283) GCConsole_Z;
    if (!(gData.Cons->InitConsole() & 0xFF)) {
        exit(0x14);
    }
    gData.Cons->Init();
    gData.Cons->SetVar("_GC");
    gData.Cons->SetVar("_FORCEBFREAD");
    MathInitTables();
    strcpy((Char*)(&gData.m_Pad_0x14[0x7D4]), ".\\");
    DVDInit();
}
