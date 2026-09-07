#ifndef _DIALOG_G_H_
#define _DIALOG_G_H_
#include "Types_Z.h"
#include "Name_Z.h"

class Dialog_G {
public:
    const Name_Z& GetNameDialog() {
        static Name_Z l_Name;
        return l_Name;
    }
};

#endif // _DIALOG_G_H_
