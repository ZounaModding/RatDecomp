#ifndef _DCCONSOLE_Z_H_
#define _DCCONSOLE_Z_H_

#include "Console_Z.h"

class DCConsole_Z : public Console_Z {
public:
    virtual Bool InitConsole() {
        return TRUE;
    }
};

#endif
