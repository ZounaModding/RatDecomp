#ifndef _MENUMANAGER_G_H_
#define _MENUMANAGER_G_H_
#include "Types_Z.h"

class Name_Z;
class styleBox;
class styleText;
class styleBitmap;
class SurroundingBitmapsStyle;

class MenuManager_G {
public:
    void ResetStyles();
    void InitVoices();
    void SetRessourcesParsed(Bool i_Parsed);
    styleBox* GetStyleBoxByName(const Name_Z& i_Name);
    S32 GetIdStyleBox(styleBox* i_Style);
    styleText* GetStyleTextByName(const Name_Z& i_Name);
    S32 GetIdStyleText(styleText* i_Style);
    styleBitmap* GetStyleBitmapByName(const Name_Z& i_Name);
    SurroundingBitmapsStyle* GetSurroundingBitmapsStyleByName(const Name_Z& i_Name);
    S32 GetIdSurroundingBitmapsStyle(SurroundingBitmapsStyle* i_Style);
    S32 GetCyclicAnimStyleIdByName(const Name_Z& i_Name);
};

#endif // _MENUMANAGER_G_H_
