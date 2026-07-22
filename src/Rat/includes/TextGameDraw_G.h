#ifndef _TEXTGAMEDRAW_G_H_
#define _TEXTGAMEDRAW_G_H_
#include "Color_Z.h"
#include "Game_ZHdl.h"
#include "Manipulator_Z.h"
#include "Math_Z.h"
#include "Name_Z.h"
#include "Node_ZHdl.h"

struct ObjTextDraw_G {
    Bool m_HasColorCode;
    Node_ZHdl m_NodeHdl;
    Name_Z m_NodeName;
    Vec3f m_Origin;
    Vec3f m_WidthVector;
    Vec3f m_HeightVector;
    S32 m_HasTrText;
    S32 m_TrTextId;
    Bool m_HasScroll;
    Float m_ScrollRelated0;
    Float m_ScrollRelated1;
    Float m_ScrollRelated2;
    Float m_ScrollRelated3;
    Float m_ScrollRelated4;
    Vec2f m_PosMaybe;
    Color m_Color;
};

typedef DynArray_Z<ObjTextDraw_G, 4> ObjTextDraw_GDA;

class TextGameDraw_G : public ManipulatorSceneDraw_Z {
public:
    virtual ~TextGameDraw_G();
    virtual void Init();
    virtual void Reset();
    virtual void Update(Float i_DeltaTime);
    virtual void Draw(const DrawInfo_Z& i_DrawInfo);

    static BaseObject_Z* NewObject() { return NewL_Z(73) TextGameDraw_G; }

    void Minimize();
    void StreamDone(const Game_ZHdl& i_GameHdl, const Node_ZHdl& i_NodeHdl);
    void StreamRemoving(const Game_ZHdl& i_GameHdl, const Node_ZHdl& i_NodeHdl);

private:
    ObjTextDraw_GDA m_ObjTextDrawDA;
    Game_ZHdl m_GameHdl;
    S32 m_Unk_0x2c;
    S32 m_Unk_0x30;
    S32 m_Unk_0x34;
    S32 m_Unk_0x38;
    S32 m_Unk_0x3c;
    S32 m_Unk_0x40;
    S32 m_Unk_0x44;
    S32 m_Unk_0x48;
};
#endif // _TEXTGAMEDRAW_G_H_
