#include "Assert_Z.h"
#include <kos/dbglog.h>
#include <kos/irq.h>
#include <arch/irq.h>
#include <string.h>
#include <cstdlib>

#ifdef DEBUG_Z
Char ConTemp1[255];
Char ConTemp2[255];
Char ConTemp3[255];
Char ConTemp4[255];
Char ConTemp5[255];
#endif

void ExceptionFonc_Z(const Char* a1, const Char* a2, S32 a3, const Char* a4, U32 a5, U32 a6, U32 a7, U32 a8, U32 a9, U64* a10) {
#ifdef DEBUG_Z
    Extern_Z Char WhereAmI[128];

    dbglog(DBG_ERROR, "Excep %s %s \"%s\" line%ld.\n", a1, a4, a2, a3);
    dbglog(DBG_ERROR, "  %s\n", WhereAmI);
    dbglog(DBG_ERROR, "  Console:\n");
    dbglog(DBG_ERROR, "    %s\n", ConTemp1);
    dbglog(DBG_ERROR, "    %s\n", ConTemp2);
    dbglog(DBG_ERROR, "    %s\n", ConTemp3);
    dbglog(DBG_ERROR, "    %s\n", ConTemp4);
    dbglog(DBG_ERROR, "    %s\n", ConTemp5);
    exit(0);
#endif
}

Bool ExceptionBool_Z(Bool i_Exp, const Char* i_Message, ...) {
    return FALSE;
}

void ExceptionReport(const Char* a1) {
#ifdef DEBUG_Z
    dbglog(DBG_INFO, "> Cmd: %s\n", a1);

    strncpy(ConTemp1, ConTemp2, sizeof(ConTemp1));
    strncpy(ConTemp2, ConTemp3, sizeof(ConTemp2));
    strncpy(ConTemp3, ConTemp4, sizeof(ConTemp3));
    strncpy(ConTemp4, ConTemp5, sizeof(ConTemp4));
    strncpy(ConTemp5, a1, sizeof(ConTemp5));
    ConTemp5[sizeof(ConTemp5) - 1] = 0;
#endif
}

#ifdef DEBUG_Z
static void DCErrorHandler(irq_t i_Code, irq_context_t* i_Context, void* i_Data) {
    Extern_Z Char WhereAmI[128];
    U32* l_Regs = (U32*)i_Context->r;

    dbglog(DBG_DEAD, "ErrorHandler: exception %04lx\n", (U32)i_Code);
    dbglog(DBG_DEAD, "  PC %08lx PR %08lx SR %08lx\n", i_Context->pc, i_Context->pr, i_Context->sr);
    dbglog(DBG_DEAD, "  R0-R7:  %08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx\n", l_Regs[0], l_Regs[1], l_Regs[2], l_Regs[3], l_Regs[4], l_Regs[5], l_Regs[6], l_Regs[7]);
    dbglog(DBG_DEAD, "  R8-R15: %08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx\n", l_Regs[8], l_Regs[9], l_Regs[10], l_Regs[11], l_Regs[12], l_Regs[13], l_Regs[14], l_Regs[15]);
    dbglog(DBG_DEAD, "  %s\n", WhereAmI);
    dbglog(DBG_DEAD, "  Console:\n");
    dbglog(DBG_DEAD, "    %s\n", ConTemp1);
    dbglog(DBG_DEAD, "    %s\n", ConTemp2);
    dbglog(DBG_DEAD, "    %s\n", ConTemp3);
    dbglog(DBG_DEAD, "    %s\n", ConTemp4);
    dbglog(DBG_DEAD, "    %s\n", ConTemp5);

    for (;;) {
    }
}
#endif

void ExceptionHandler() {
#ifdef DEBUG_Z
    irq_set_handler(EXC_DATA_ADDRESS_READ, DCErrorHandler, NULL);
    irq_set_handler(EXC_DATA_ADDRESS_WRITE, DCErrorHandler, NULL);
    irq_set_handler(EXC_ILLEGAL_INSTR, DCErrorHandler, NULL);
    irq_set_handler(EXC_SLOT_ILLEGAL_INSTR, DCErrorHandler, NULL);
    irq_set_handler(EXC_FPU, DCErrorHandler, NULL);
    irq_set_handler(EXC_GENERAL_FPU, DCErrorHandler, NULL);
    irq_set_handler(EXC_SLOT_FPU, DCErrorHandler, NULL);
#endif
}

void BreakPoint_Z() {
}
