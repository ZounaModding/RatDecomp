#ifndef _POPUPMENU_Z_H_
#define _POPUPMENU_Z_H_
#include "Types_Z.h"
#include <String_Z.h>
#include "DynArray_Z.h"

#define ID_WIREFRAME 40000
#define ID_HEADLIGHT 40001
#define ID_NO_TEXTURE 40002
#define ID_1X1_TEXTURE 40003
#define ID_1_TRIANGLE 40004
#define ID_NO_FOG 40006
#define ID_NO_OMNI 40007
#define ID_NORMALS 40008
#define ID_LOCK_VIEW 40009
#define ID_SKIN_STRIP 40010
#define ID_LIGHTINGTYPE_PIXEL_RADIOSITY 40011
#define ID_LIGHTINGTYPE_PIXEL 40012
#define ID_LIGHTINGTYPE_VERTEX_RADIOSITY 40013
#define ID_LIGHTINGTYPE_VERTEX 40014
#define ID_SCENE_NODE 40015
#define ID_BONE_NODE 40016
#define ID_OCCLUDER_ON 40017
#define ID_OCCLUDER_OUT 40018
#define ID_OCCLUDER_INV 40019
#define ID_SLIDING_SURFACE 40020
#define ID_BSPHERE 40021
#define ID_BBOX 40022
#define ID_LOD 40023
#define ID_FADE_PERCENT 40024
#define ID_CULLCONE 40025
#define ID_INV_CULLCONE 40026
#define ID_STREAMZONE 40027
#define ID_HIDEPATCH0 40028
#define ID_SHADOWVOLUME 40029
#define ID_RAY 40030
#define ID_SPHERE 40031
#define ID_MOVING_SPHERE 40032
#define ID_CAPSULE 40033
#define ID_SPH_BOX_CYL 40035
#define ID_FACE 40036
#define ID_SKELETON 40037
#define ID_SPLINE_COLLIDE 40038
#define ID_SPLINE_CLING 40039
#define ID_SPHERE_HIT 40040
#define ID_BOX_HIT 40041
#define ID_OBJECT 40043
#define ID_ANIMATION 40044
#define ID_NODE 40045
#define ID_SKEL_NODE 40046
#define ID_RESOURCE 40047
#define ID_CLIPPING 40048
#define ID_POS_PERSO 40049
#define ID_PROFILER 40050
#define ID_DISPLAY_FPS 40051
#define ID_DISABLE_MUSIC 40052
#define ID_DISABLE_MOVIE 40053
#define ID_DISABLE_ASSERT 40054
#define ID_INGAME_CONSOLE 40055
#define ID_DEBUG_SOUND 40056
#define ID_STACK_TRACE 40057
#define ID_MEMORY_INFOS 40058
#define ID_DIFFUSE 40060
#define ID_ENVMAP 40061
#define ID_NORMAL_MAP 40062
#define ID_SPECULAR_MAP 40063
#define ID_BLOOM 40105
#define ID_DEPTH_OF_FIELD 40106
#define ID_SCREEN_DISTORTION 40107
#define ID_RADIAL_MOTION_BLUR 40108
#define ID_VSYNC_ENABLE 40109
#define ID_2_FRAMES 40110
#define ID_BLACK_AND_WHITE 40111
#define ID_DEBUG_BUFFERS 40112
#define ID_SETCAMERAUSER_VIEWPORT1 40113
#define ID_SETCAMERAUSER_VIEWPORT2 40114
#define ID_SETCAMERAUSER_VIEWPORT3 40115
#define ID_SETCAMERAUSER_VIEWPORT4 40116
#define ID_RESETGAME_VIEWPORT1 40117
#define ID_RESETGAME_VIEWPORT2 40118
#define ID_RESETGAME_VIEWPORT3 40119
#define ID_RESETGAME_VIEWPORT4 40120
#define ID_NO_SOUND 40121

class PopupMenu_Z;
class PopupItem_Z;

typedef void (*ItemProc)(PopupItem_Z* i_Item);
typedef void (*ItemState)(PopupItem_Z* i_Item);

void ItemProc_Flag(PopupItem_Z* i_Item);
void ItemState_Flag(PopupItem_Z* i_Item);

void ItemProc_LightingType(PopupItem_Z* i_Item);
void ItemState_LightingType(PopupItem_Z* i_Item);

void ItemProc_RendererEffects(PopupItem_Z* i_Item);
void ItemState_RendererEffects(PopupItem_Z* i_Item);

void ItemProc_SetCameraUser(PopupItem_Z* i_Item);
void ItemState_SetCameraUser(PopupItem_Z* i_Item);

enum PopupItemId_Z {
    POPUP_ITEM_SEPARATOR = -2,
    POPUP_ITEM_PAGE = -1,
    POPUP_ITEM_STANDARD = 0,
};

class PopupItem_Z {
public:
    ItemProc m_Proc;   // Action to perform when the item is toggled
    ItemState m_State; // Function to update the toggled state of the item
    S32 m_Id;          // Identifier for the item (-2 is a separator, -1 is a page)
    String_Z<32> m_Text;
    S32 m_PageIdx; // Index of the page corresponding to the item (if it's a page item)
    S32 m_Toggled;
    U32 m_Value; // Custom value associated with the item, used for comparisons (toggled) and to set values if standard item | used for active/inactive if page
    U32* m_Flag; // Pointer to flag/value that can be modified when the item is toggled

    PopupItem_Z()
        : m_Proc(NULL)
        , m_State(NULL)
        , m_Id(0) {
        m_PageIdx = -1;
        m_Toggled = -1;
        m_Value = 0;
        m_Flag = NULL;
    }

    virtual ~PopupItem_Z() { }

    void RemoveChildPage();
};

typedef DynArray_Z<PopupItem_Z, 32> PopupItem_ZDA;

inline void ItemState_None(PopupItem_Z* i_Item) {
    i_Item->m_Toggled = -1;
}

class PopupPage_Z {
    friend class PopupMenu_Z;
    friend class PopupItem_Z;

    PopupItem_ZDA m_Items;
    S32 m_StringMaxLen; // MonopolyX360SUB.xdb
    S32 m_ParentPageIdx;
    S32 m_SelectedItemIdx;

public:
    PopupPage_Z() {
        m_StringMaxLen = 0;
        m_ParentPageIdx = -1;
        m_SelectedItemIdx = 0;
    }

    void RemoveItem(S32 i_Idx);
    void RemoveAllItems();

    inline void Minimize() {
        m_Items.Minimize();
    }
};

typedef DynArray_Z<PopupPage_Z, 32> PopupPage_ZDA;

class PopupMenu_Z {
    PopupPage_ZDA m_Pages;
    int m_CurrentPageIdx;  // Page that new items will be added to
    S32 m_SelectedPageIdx; // Page currently being displayed
    Bool m_DoDraw;         // Set to true if either left, up, right or down are being pressed (to redraw)

public:
    PopupMenu_Z();
    ~PopupMenu_Z();

    virtual void Update(Float i_DeltaTime);
    virtual void Show(S32 i_TopLeftX, S32 i_TopLeftY);
    virtual void DrawPage(S32 i_TopLeftX, S32 i_TopLeftY, PopupPage_Z& i_Page);
    virtual void AddSeparator();
    virtual void AddItem(U32 i_Id, const Char* i_Text, ItemProc i_Proc, ItemState i_State, U32 i_Value, U32* i_Flag);
    virtual void PushSubItem(const Char* i_Text, S32& o_PrevPageIdx, S32& o_NewItemIdx);
    virtual void PushSubItem(const Char* i_Text);
    virtual void PopSubItem();

    virtual void RemoveItem(S32 i_PageIdx, S32 i_ItemIdx) {
        m_Pages[i_PageIdx].RemoveItem(i_ItemIdx);
    }

    virtual PopupItem_Z* FindItem(int i_Id);

    PopupPage_Z& GetPage(S32 i_Idx) {
        return m_Pages[i_Idx];
    }

    void RemovePage(S32 i_Idx) {
        m_Pages.Remove(i_Idx);
    }

    inline void Minimize() {
        m_Pages.Minimize();
        for (U32 i = 0; (S32)i < m_Pages.GetSize(); i++) {
            m_Pages[i].Minimize();
        }
    }
};

#endif
