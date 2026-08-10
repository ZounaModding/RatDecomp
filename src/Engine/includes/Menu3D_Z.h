#ifndef _MENU3D_Z_H_
#define _MENU3D_Z_H_
#include "Menu_Z.h"

class Menu3D_Z : public Menu_Z {
public:
    virtual ~Menu3D_Z();
    virtual Bool MarkHandles();
    virtual void Update(Float i_DeltaTime);
    virtual void Draw(const Viewport_Z* a1);
    virtual void ActivateMenu(U8 a1);
    virtual void DeactivateMenu();
    virtual void UpdateItems(Bool a1, Bool a2);
    virtual void LoadMenuDesc(const Char* a1, S32 a2);
    virtual void InitMenu(S32 a1);

    static BaseObject_Z* NewObject() { return NewL_Z(83) Menu3D_Z; }
};

#endif // _MENU3D_Z_H_
