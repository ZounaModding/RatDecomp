#include "Language_Z.h"
#include "Name_Z.h"
#include "Types_Z.h"
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

Bool TT(int i_Id) {
}
