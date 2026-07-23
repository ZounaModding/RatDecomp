#ifndef _MENU_Z_H_
#define _MENU_Z_H_
#include "Manipulator_Z.h"

class Menu_Z : public ManipulatorDraw_Z {
public:
    virtual ~Menu_Z();
    virtual void Init();

    virtual void Update(Float a1) { }

    virtual void Draw(const Viewport_Z* a1) { }

    virtual void ActivateMenu(U8 a1) { }

    virtual void DeactivateMenu() { }

    virtual void SetExtraFontID(U8 a1) { }

    virtual void InitMenu(S32 a1);
};

#endif // _MENU_Z_H_
