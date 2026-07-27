#include "PopupMenu_Z.h"
#include "Program_Z.h"
#include "Renderer_Z.h"
#include "GameManager_Z.h"
#include "DebugTools_Z.h"
#include "InputEngine_Z.h"
#include "Console_Z.h"

static U32 TrashbinFlag;

void PopupItem_Z::RemoveChildPage() {
    if (m_Id == POPUP_ITEM_PAGE) {
        PopupMenu_Z* l_PopupMenu = gData.Cons->GetPopupMenu();
        l_PopupMenu->GetPage(m_PageIdx).RemoveAllItems();
        l_PopupMenu->RemovePage(m_PageIdx);
    }
}

void PopupPage_Z::RemoveAllItems() {
    S32 i = m_Items.GetSize() - 1;
    while (i >= 0) {
        RemoveItem(i);
        i--;
    }
}

void PopupPage_Z::RemoveItem(S32 i_Idx) {
    m_Items[i_Idx].RemoveChildPage();
    m_Items.Remove(i_Idx);
}

void ItemProc_Flag(PopupItem_Z* i_Item) {
    *i_Item->m_Flag = *i_Item->m_Flag ^ i_Item->m_Value;
}

void ItemState_Flag(PopupItem_Z* i_Item) {
    i_Item->m_Toggled = (*i_Item->m_Flag & i_Item->m_Value) != 0;
}

void ItemProc_LightingType(PopupItem_Z* i_Item) {
    gData.MainRdr->SetLightingType(i_Item->m_Value);
}

void ItemState_LightingType(PopupItem_Z* i_Item) {
    i_Item->m_Toggled = i_Item->m_Value == gData.MainRdr->GetLightingType() ? TRUE : FALSE;
}

void ItemProc_RendererEffects(PopupItem_Z* i_Item) {
    if (gData.MainRdr->IsEffectFlag(i_Item->m_Value)) {
        gData.MainRdr->DisableEffectFlag(i_Item->m_Value);
    }
    else {
        gData.MainRdr->EnableEffectFlag(i_Item->m_Value);
    }
}

void ItemState_RendererEffects(PopupItem_Z* i_Item) {
    i_Item->m_Toggled = gData.MainRdr->IsEffectFlag(i_Item->m_Value);
}

void ItemProc_SetCameraUser(PopupItem_Z* i_Item) {
    S32 l_VpId = -1;
    switch (i_Item->m_Id) {
        case ID_SETCAMERAUSER_VIEWPORT1:
            l_VpId = 0;
            break;
        case ID_SETCAMERAUSER_VIEWPORT2:
            l_VpId = 1;
            break;
        case ID_SETCAMERAUSER_VIEWPORT3:
            l_VpId = 2;
            break;
        case ID_SETCAMERAUSER_VIEWPORT4:
            l_VpId = 3;
            break;
    }
    String_Z<264> l_CmdStr;
    sprintf(l_CmdStr, "SwitchCameraUser %d", l_VpId);
    gData.Cons->InterpCommand(l_CmdStr, 0);
}

void ItemState_SetCameraUser(PopupItem_Z* i_Item) {
    S32 l_VpId;
    switch (i_Item->m_Id) {
        case ID_SETCAMERAUSER_VIEWPORT1:
            l_VpId = 0;
            break;
        case ID_SETCAMERAUSER_VIEWPORT2:
            l_VpId = 1;
            break;
        case ID_SETCAMERAUSER_VIEWPORT3:
            l_VpId = 2;
            break;
        case ID_SETCAMERAUSER_VIEWPORT4:
            l_VpId = 3;
            break;
    }
    i_Item->m_Toggled = gData.GameMgr->IsCameraUser(l_VpId) ? TRUE : FALSE;
}

PopupMenu_Z::~PopupMenu_Z() { }

PopupMenu_Z::PopupMenu_Z() {
    m_DoDraw = FALSE;
    m_CurrentPageIdx = 0;
    m_SelectedPageIdx = 0;

    U32* l_EngineFlagPtr = &gData.m_EngineFlag;
    U32* l_DebugFlagPtr = &TrashbinFlag;
    U32* l_CollisionFlagPtr = &TrashbinFlag;

    m_Pages.Add();

    PushSubItem("Debug");
    PushSubItem("Scene");
    AddItem(ID_WIREFRAME, "Wireframe", ItemProc_Flag, ItemState_Flag, FL_WIREFRAME, l_EngineFlagPtr);
    AddItem(ID_HEADLIGHT, "HeadLight", ItemProc_Flag, ItemState_Flag, FL_DEBUG_HEADLIGHT, l_DebugFlagPtr);
    AddItem(ID_NO_TEXTURE, "No Texture", ItemProc_Flag, ItemState_Flag, FL_NO_TEXTURE, l_EngineFlagPtr);
    AddItem(ID_1X1_TEXTURE, "1x1 Texture", ItemProc_Flag, ItemState_Flag, FL_1X1_TEXTURE, l_EngineFlagPtr);
    AddItem(ID_1_TRIANGLE, "1 Triangle", ItemProc_Flag, ItemState_Flag, FL_1_TRIANGLE, l_EngineFlagPtr);
    AddItem(ID_NO_FOG, "No Fog", ItemProc_Flag, ItemState_Flag, FL_DEBUG_NO_FOG, l_DebugFlagPtr);
    AddItem(ID_NO_OMNI, "No Omni", ItemProc_Flag, ItemState_Flag, FL_NO_OMNI, l_EngineFlagPtr);
    AddItem(ID_LOCK_VIEW, "Lock View", ItemProc_Flag, ItemState_Flag, FL_LOCK_VIEW, l_EngineFlagPtr);
    AddItem(ID_NORMALS, "Normals", ItemProc_Flag, ItemState_Flag, FL_DEBUG_NORMALS, l_DebugFlagPtr);
    AddItem(ID_SKIN_STRIP, "Skin Strip", ItemProc_Flag, ItemState_Flag, FL_DEBUG_SKIN_STRIP, l_DebugFlagPtr);

    PushSubItem("Hierarchie");
    AddItem(ID_SCENE_NODE, "Scene Node", ItemProc_Flag, ItemState_Flag, FL_DEBUG_SCENE_NODE, l_DebugFlagPtr);
    AddItem(ID_BONE_NODE, "Bone Node", ItemProc_Flag, ItemState_Flag, FL_DEBUG_BONE_NODE, l_DebugFlagPtr);
    PopSubItem();

    AddItem(ID_OCCLUDER_ON, "Occluder On", ItemProc_Flag, ItemState_Flag, FL_OCCLUDER_ON, l_EngineFlagPtr);
    AddItem(ID_OCCLUDER_OUT, "Occluder Out", ItemProc_Flag, ItemState_Flag, FL_DEBUG_OCCLUDER_OUT, l_DebugFlagPtr);
    AddItem(ID_OCCLUDER_INV, "Occluder Inv", ItemProc_Flag, ItemState_Flag, FL_DEBUG_OCCLUDER_INV, l_DebugFlagPtr);
    AddItem(ID_SLIDING_SURFACE, "Sliding Surface", ItemProc_Flag, ItemState_Flag, FL_DEBUG_SLIDING_SURFACE, l_DebugFlagPtr);
    AddItem(ID_BSPHERE, "BSphere", ItemProc_Flag, ItemState_Flag, FL_DEBUG_BSPHERE, l_DebugFlagPtr);
    AddItem(ID_BBOX, "BBox", ItemProc_Flag, ItemState_Flag, FL_DEBUG_BBOX, l_DebugFlagPtr);
    AddItem(ID_LOD, "LOD", ItemProc_Flag, ItemState_Flag, FL_DEBUG_LOD, l_DebugFlagPtr);
    AddItem(ID_FADE_PERCENT, "Fade %%", ItemProc_Flag, ItemState_Flag, FL_DEBUG_FADE_PERCENT, l_DebugFlagPtr);
    AddItem(ID_CULLCONE, "CullCone", ItemProc_Flag, ItemState_Flag, FL_DEBUG_CULLCONE, l_DebugFlagPtr);
    AddItem(ID_INV_CULLCONE, "Inv CullCone", ItemProc_Flag, ItemState_Flag, FL_DEBUG_INV_CULLCONE, l_DebugFlagPtr);
    AddItem(ID_STREAMZONE, "StreamZone", ItemProc_Flag, ItemState_Flag, FL_STREAMZONE, l_EngineFlagPtr);
    AddItem(ID_HIDEPATCH0, "HidePatch0", ItemProc_Flag, ItemState_Flag, FL_DEBUG_HIDEPATCH0, l_DebugFlagPtr);
    AddItem(ID_SHADOWVOLUME, "ShadowVolume", ItemProc_Flag, ItemState_Flag, FL_DEBUG_SHADOWVOLUME, l_DebugFlagPtr);
    PopSubItem();

    PushSubItem("Collision");
    PushSubItem("Scene");
    AddItem(ID_RAY, "Ray", ItemProc_Flag, ItemState_Flag, FL_COLLISION_RAY, l_CollisionFlagPtr);
    AddItem(ID_SPHERE, "Sphere", ItemProc_Flag, ItemState_Flag, FL_COLLISION_SPHERE, l_CollisionFlagPtr);
    AddItem(ID_MOVING_SPHERE, "Moving Sphere", ItemProc_Flag, ItemState_Flag, FL_COLLISION_MOVING_SPHERE, l_CollisionFlagPtr);
    AddItem(ID_CAPSULE, "Capsule", ItemProc_Flag, ItemState_Flag, FL_COLLISION_CAPSULE, l_CollisionFlagPtr);
    PopSubItem();

    AddItem(ID_SPH_BOX_CYL, "Sph&Box&Cyl", ItemProc_Flag, ItemState_Flag, FL_COLLISION_SPH_BOX_CYL, l_CollisionFlagPtr);
    AddItem(ID_FACE, "Face", ItemProc_Flag, ItemState_Flag, FL_COLLISION_FACE, l_CollisionFlagPtr);
    AddItem(ID_SKELETON, "Skeleton", ItemProc_Flag, ItemState_Flag, FL_COLLISION_SKELETON, l_CollisionFlagPtr);
    AddItem(ID_SPLINE_COLLIDE, "Spline Collide", ItemProc_Flag, ItemState_Flag, FL_DEBUG_SPLINE_COLLIDE, l_DebugFlagPtr);
    AddItem(ID_SPLINE_CLING, "Spline Cling", ItemProc_Flag, ItemState_Flag, FL_DEBUG_SPLINE_CLING, l_DebugFlagPtr);
    AddItem(ID_SPHERE_HIT, "Sphere Hit", ItemProc_Flag, ItemState_Flag, FL_COLLISION_SPHERE_HIT, l_CollisionFlagPtr);
    AddItem(ID_BOX_HIT, "Box Hit", ItemProc_Flag, ItemState_Flag, FL_COLLISION_BOX_HIT, l_CollisionFlagPtr);
    PopSubItem();

    PushSubItem("Name");
    AddItem(ID_OBJECT, "Object", ItemProc_Flag, ItemState_Flag, FL_DEBUG_OBJECT, l_DebugFlagPtr);
    AddItem(ID_ANIMATION, "Animation", ItemProc_Flag, ItemState_Flag, FL_DEBUG_ANIMATION, l_DebugFlagPtr);
    AddItem(ID_NODE, "Node", ItemProc_Flag, ItemState_Flag, FL_DEBUG_NODE, l_DebugFlagPtr);
    AddItem(ID_SKEL_NODE, "Skel Node", ItemProc_Flag, ItemState_Flag, FL_DEBUG_SKEL_NODE, l_DebugFlagPtr);
    AddItem(ID_RESOURCE, "Resource", ItemProc_Flag, ItemState_Flag, FL_DEBUG_RESOURCE, l_DebugFlagPtr);
    PopSubItem();

    PushSubItem("Manipulator");
    AddItem(ID_CLIPPING, "Clipping", ItemProc_Flag, ItemState_Flag, FL_DEBUG_CLIPPING, l_DebugFlagPtr);
    PopSubItem();

    AddItem(ID_POS_PERSO, "Pos Perso", ItemProc_Flag, ItemState_Flag, FL_POS_PERSO, l_EngineFlagPtr);
    AddItem(ID_PROFILER, "Profiler", ItemProc_Flag, ItemState_Flag, FL_PROFILER, l_EngineFlagPtr);
    AddItem(ID_DISPLAY_FPS, "Display Fps", ItemProc_Flag, ItemState_Flag, FL_DISPLAY_FPS, l_EngineFlagPtr);
    AddItem(ID_DISABLE_MUSIC, "Disable Music", ItemProc_Flag, ItemState_Flag, FL_DISABLE_MUSIC, l_EngineFlagPtr);
    AddItem(ID_DISABLE_MOVIE, "Disable Movie", ItemProc_Flag, ItemState_Flag, FL_DISABLE_MOVIE, l_EngineFlagPtr);
    AddItem(ID_DISABLE_ASSERT, "Disable Assert", ItemProc_Flag, ItemState_Flag, FL_DISABLE_ASSERT, l_EngineFlagPtr);
    AddItem(ID_INGAME_CONSOLE, "Ingame Console", ItemProc_Flag, ItemState_Flag, FL_INGAME_CONSOLE, l_EngineFlagPtr);
    AddItem(ID_DEBUG_SOUND, "Debug Sound", ItemProc_Flag, ItemState_Flag, FL_DEBUG_SOUND, l_EngineFlagPtr);
    AddItem(ID_STACK_TRACE, "Stack Trace", ItemProc_Flag, ItemState_Flag, FL_STACK_TRACE, l_EngineFlagPtr);
    AddItem(ID_MEMORY_INFOS, "Memory Infos", ItemProc_Flag, ItemState_Flag, FL_MEMORY_INFOS, l_EngineFlagPtr);
    PopSubItem();

    AddSeparator();

    PushSubItem("Material");

    PushSubItem("Lighting type");
    AddItem(ID_LIGHTINGTYPE_PIXEL_RADIOSITY, "(slit 0) Pixel + Radiosity", ItemProc_LightingType, ItemState_LightingType, 0, NULL);
    AddItem(ID_LIGHTINGTYPE_PIXEL, "(slit 1) Pixel", ItemProc_LightingType, ItemState_LightingType, 1, NULL);
    AddItem(ID_LIGHTINGTYPE_VERTEX_RADIOSITY, "(slit 3) Vertex + Radiosity", ItemProc_LightingType, ItemState_LightingType, 3, NULL);
    AddItem(ID_LIGHTINGTYPE_VERTEX, "(slit 2) Vertex", ItemProc_LightingType, ItemState_LightingType, 2, NULL);
    PopSubItem();

    PushSubItem("Stages");
    AddItem(ID_DIFFUSE, "Diffuse", ItemProc_Flag, ItemState_Flag, FL_MATERIAL_DIFFUSE, &gData.m_MaterialFlag);
    AddItem(ID_ENVMAP, "Envmap", ItemProc_Flag, ItemState_Flag, FL_MATERIAL_ENVMAP, &gData.m_MaterialFlag);
    AddItem(ID_NORMAL_MAP, "Normal Map", ItemProc_Flag, ItemState_Flag, FL_MATERIAL_NORMAL_MAP, &gData.m_MaterialFlag);
    AddItem(ID_SPECULAR_MAP, "Specular Map", ItemProc_Flag, ItemState_Flag, FL_MATERIAL_SPECULAR_MAP, &gData.m_MaterialFlag);
    PopSubItem();

    PopSubItem();

    PushSubItem("Renderer Effects");
    AddItem(ID_BLOOM, "Bloom", ItemProc_RendererEffects, ItemState_RendererEffects, FL_EFFECT_BLOOM, NULL);
    AddItem(ID_DEPTH_OF_FIELD, "Depth of Field", ItemProc_RendererEffects, ItemState_RendererEffects, FL_EFFECT_DEPTH_OF_FIELD, NULL);
    AddItem(ID_SCREEN_DISTORTION, "Screen Distortion", ItemProc_RendererEffects, ItemState_RendererEffects, FL_EFFECT_SCREEN_DISTORTION, NULL);
    AddItem(ID_RADIAL_MOTION_BLUR, "Radial Motion Blur", ItemProc_RendererEffects, ItemState_RendererEffects, FL_EFFECT_RADIAL_MOTION_BLUR, NULL);
    AddItem(ID_VSYNC_ENABLE, "VSync Enable", ItemProc_RendererEffects, ItemState_RendererEffects, FL_EFFECT_VSYNC, NULL);
    AddItem(ID_2_FRAMES, "2 Frames", ItemProc_Flag, ItemState_Flag, FL_GAME_2_FRAMES, &gData.m_GameFlag);
    AddItem(ID_BLACK_AND_WHITE, "Black and White", ItemProc_RendererEffects, ItemState_RendererEffects, FL_EFFECT_BLACK_AND_WHITE, NULL);
    AddItem(ID_DEBUG_BUFFERS, "Debug Buffers", ItemProc_RendererEffects, ItemState_RendererEffects, FL_EFFECT_DEBUG_BUFFERS, NULL);
    PopSubItem();

    AddSeparator();

    PushSubItem("Camera User");
    AddItem(ID_SETCAMERAUSER_VIEWPORT1, "Viewport 1", ItemProc_SetCameraUser, ItemState_SetCameraUser, 0, NULL);
    AddItem(ID_SETCAMERAUSER_VIEWPORT2, "Viewport 2", ItemProc_SetCameraUser, ItemState_SetCameraUser, 0, NULL);
    AddItem(ID_SETCAMERAUSER_VIEWPORT3, "Viewport 3", ItemProc_SetCameraUser, ItemState_SetCameraUser, 0, NULL);
    AddItem(ID_SETCAMERAUSER_VIEWPORT4, "Viewport 4", ItemProc_SetCameraUser, ItemState_SetCameraUser, 0, NULL);
    PopSubItem();

    PushSubItem("Reset Game");
    AddItem(ID_RESETGAME_VIEWPORT1, "Viewport 1", ItemProc_SetCameraUser, ItemState_None, 0, NULL);
    AddItem(ID_RESETGAME_VIEWPORT2, "Viewport 2", ItemProc_SetCameraUser, ItemState_None, 0, NULL);
    AddItem(ID_RESETGAME_VIEWPORT3, "Viewport 3", ItemProc_SetCameraUser, ItemState_None, 0, NULL);
    AddItem(ID_RESETGAME_VIEWPORT4, "Viewport 4", ItemProc_SetCameraUser, ItemState_None, 0, NULL);
    PopSubItem();

    AddItem(ID_NO_SOUND, "No Sound", ItemProc_Flag, ItemState_Flag, FL_DEBUG_NO_SOUND, l_DebugFlagPtr);

    Minimize();
}

PopupItem_Z* PopupMenu_Z::FindItem(int i_Id) {
    for (S32 i = 0; i < m_Pages.GetSize(); i++) {
        for (S32 j = 0; j < GetPage(i).m_Items.GetSize(); j++) {
            if (GetPage(i).m_Items[j].m_Id == i_Id) {
                return &GetPage(i).m_Items[j];
            }
        }
    }
    return NULL;
}

void PopupMenu_Z::AddItem(U32 i_Id, const Char* i_Text, ItemProc i_Proc, ItemState i_State, U32 i_Value, U32* i_Flag) {
    if (i_Flag != &TrashbinFlag) {
        PopupPage_Z& l_CurrentPage = m_Pages[m_CurrentPageIdx];
        l_CurrentPage.m_Items.Add();
        S32 l_LastItemIdx = l_CurrentPage.m_Items.GetSize() - 1;
        m_Pages[m_CurrentPageIdx].m_Items[l_LastItemIdx].m_Text = i_Text;
        m_Pages[m_CurrentPageIdx].m_Items[l_LastItemIdx].m_Id = i_Id;
        m_Pages[m_CurrentPageIdx].m_Items[l_LastItemIdx].m_Value = i_Value;
        m_Pages[m_CurrentPageIdx].m_Items[l_LastItemIdx].m_Flag = i_Flag;
        m_Pages[m_CurrentPageIdx].m_Items[l_LastItemIdx].m_Proc = i_Proc;
        m_Pages[m_CurrentPageIdx].m_Items[l_LastItemIdx].m_State = i_State;
    }
}

void PopupMenu_Z::AddSeparator() {
    PopupPage_Z& l_CurrentPage = m_Pages[m_CurrentPageIdx];
    l_CurrentPage.m_Items.Add();
    S32 l_LastItemIdx = l_CurrentPage.m_Items.GetSize() - 1;
    m_Pages[m_CurrentPageIdx].m_Items[l_LastItemIdx].m_Id = POPUP_ITEM_SEPARATOR;
    m_Pages[m_CurrentPageIdx].m_Items[l_LastItemIdx].m_State = ItemState_None;
}

void PopupMenu_Z::PushSubItem(const Char* i_Text) {
    S32 l_Temp;
    PushSubItem(i_Text, l_Temp, l_Temp);
}

void PopupMenu_Z::PushSubItem(const Char* i_Text, S32& o_PrevPageIdx, S32& o_NewItemIdx) {
    PopupPage_Z& l_CurrentPage = m_Pages[m_CurrentPageIdx];
    l_CurrentPage.m_Items.Add();
    S32 l_LastItemIdx = l_CurrentPage.m_Items.GetSize() - 1;
    m_Pages[m_CurrentPageIdx].m_Items[l_LastItemIdx].m_Text = i_Text;
    m_Pages[m_CurrentPageIdx].m_Items[l_LastItemIdx].m_Id = POPUP_ITEM_PAGE;
    m_Pages[m_CurrentPageIdx].m_Items[l_LastItemIdx].m_Value = 0;
    m_Pages[m_CurrentPageIdx].m_Items[l_LastItemIdx].m_State = ItemState_None;
    o_PrevPageIdx = m_CurrentPageIdx;
    o_NewItemIdx = l_LastItemIdx;

    m_Pages.Add();
    S32 l_LastPageIdx = m_Pages.GetSize() - 1;
    m_Pages[l_LastPageIdx].m_ParentPageIdx = m_CurrentPageIdx;
    m_Pages[l_LastPageIdx].m_StringMaxLen = 0;
    m_Pages[m_CurrentPageIdx].m_Items[l_LastItemIdx].m_PageIdx = l_LastPageIdx;
    m_CurrentPageIdx = l_LastPageIdx;
}

void PopupMenu_Z::PopSubItem() {
    S32 l_CurrentPageIdx = m_CurrentPageIdx;
    m_CurrentPageIdx = m_Pages[l_CurrentPageIdx].m_ParentPageIdx;
    if (m_Pages[l_CurrentPageIdx].m_Items.GetSize() == 0) {
        m_Pages.Remove(l_CurrentPageIdx);
        m_Pages[m_CurrentPageIdx].m_Items.Remove(m_Pages[m_CurrentPageIdx].m_Items.GetSize() - 1);
    }
}

void PopupMenu_Z::Update(Float i_DeltaTime) {
    if ((gData.m_EngineFlag & FL_POPUP_MENU) == FL_ENGINE_NONE) {
        return;
    }

    m_DoDraw = FALSE;

    Bool l_WasUpPressed = FALSE;
    Bool l_WasDownPressed = FALSE;
    Bool l_WasRightPressed = FALSE;
    Bool l_WasLeftPressed = FALSE;

    static Bool bOldUp[8] = { FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE };
    static Bool bOldDown[8] = { FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE };
    static Bool bOldRight[8] = { FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE };
    static Bool bOldLeft[8] = { FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE };

    for (S32 i = 0; i < gData.InputMgr->GetDeviceCount(); i++) {
        Bool l_IsUpPressed = FALSE;
        Bool l_IsDownPressed = FALSE;
        Bool l_IsRightPressed = FALSE;
        Bool l_IsLeftPressed = FALSE;

        if (gData.InputMgr->GetDevice(i).m_YPressed) {
            m_DoDraw = TRUE;
        }
        if (gData.InputMgr->GetDevice(i).m_DPadUpPressed) {
            l_IsUpPressed = TRUE;
        }
        if (gData.InputMgr->GetDevice(i).m_DPadDownPressed) {
            l_IsDownPressed = TRUE;
        }
        if (gData.InputMgr->GetDevice(i).m_DPadRightPressed) {
            l_IsRightPressed = TRUE;
        }
        if (gData.InputMgr->GetDevice(i).m_DPadLeftPressed) {
            l_IsLeftPressed = TRUE;
        }

        if (bOldUp[i] != l_IsUpPressed) {
            if (bOldUp[i] == 0) {
                l_WasUpPressed = TRUE;
            }
            bOldUp[i] = l_IsUpPressed;
        }
        if (bOldDown[i] != l_IsDownPressed) {
            if (bOldDown[i] == 0) {
                l_WasDownPressed = TRUE;
            }
            bOldDown[i] = l_IsDownPressed;
        }
        if (bOldRight[i] != l_IsRightPressed) {
            if (bOldRight[i] == 0) {
                l_WasRightPressed = TRUE;
            }
            bOldRight[i] = l_IsRightPressed;
        }
        if (bOldLeft[i] != l_IsLeftPressed) {
            if (bOldLeft[i] == 0) {
                l_WasLeftPressed = TRUE;
            }
            bOldLeft[i] = l_IsLeftPressed;
        }
    }

    if (m_DoDraw) {
        if (l_WasDownPressed) {
            m_Pages[m_SelectedPageIdx].m_SelectedItemIdx++;
            if (m_Pages[m_SelectedPageIdx].m_SelectedItemIdx >= m_Pages[m_SelectedPageIdx].m_Items.GetSize()) {
                m_Pages[m_SelectedPageIdx].m_SelectedItemIdx = 0;
            }

            while (m_Pages[m_SelectedPageIdx].m_Items[m_Pages[m_SelectedPageIdx].m_SelectedItemIdx].m_Id == POPUP_ITEM_SEPARATOR) {
                m_Pages[m_SelectedPageIdx].m_SelectedItemIdx++;
                if (m_Pages[m_SelectedPageIdx].m_SelectedItemIdx >= m_Pages[m_SelectedPageIdx].m_Items.GetSize()) {
                    m_Pages[m_SelectedPageIdx].m_SelectedItemIdx = 0;
                }
            }
        }

        if (l_WasUpPressed) {
            m_Pages[m_SelectedPageIdx].m_SelectedItemIdx--;
            if (m_Pages[m_SelectedPageIdx].m_SelectedItemIdx < 0) {
                m_Pages[m_SelectedPageIdx].m_SelectedItemIdx = m_Pages[m_SelectedPageIdx].m_Items.GetSize() - 1;
            }

            while (m_Pages[m_SelectedPageIdx].m_Items[m_Pages[m_SelectedPageIdx].m_SelectedItemIdx].m_Id == POPUP_ITEM_SEPARATOR) {
                m_Pages[m_SelectedPageIdx].m_SelectedItemIdx--;
                if (m_Pages[m_SelectedPageIdx].m_SelectedItemIdx < 0) {
                    m_Pages[m_SelectedPageIdx].m_SelectedItemIdx = m_Pages[m_SelectedPageIdx].m_Items.GetSize() - 1;
                }
            }
        }

        if (l_WasRightPressed) {
            PopupItem_Z& l_SelectedItem = m_Pages[m_SelectedPageIdx].m_Items[m_Pages[m_SelectedPageIdx].m_SelectedItemIdx];

            if (l_SelectedItem.m_Id == POPUP_ITEM_PAGE) {
                if (m_Pages[l_SelectedItem.m_PageIdx].m_Items.GetSize() != 0) {
                    l_SelectedItem.m_Value = 1;
                    m_SelectedPageIdx = l_SelectedItem.m_PageIdx;
                }
            }
            else {
                l_SelectedItem.m_Proc(&l_SelectedItem);
            }
        }

        if (l_WasLeftPressed) {
            if (m_SelectedPageIdx > 0) {
                m_SelectedPageIdx = m_Pages[m_SelectedPageIdx].m_ParentPageIdx;
                m_Pages[m_SelectedPageIdx].m_Items[m_Pages[m_SelectedPageIdx].m_SelectedItemIdx].m_Value = 0;
            }
        }
    }
}

void PopupMenu_Z::Show(S32 i_TopLeftX, S32 i_TopLeftY) {
    if ((gData.m_EngineFlag & FL_POPUP_MENU) != FL_ENGINE_NONE && m_DoDraw) {
        DrawPage(i_TopLeftX, i_TopLeftY, m_Pages[0]);
    }
}

void PopupMenu_Z::DrawPage(S32 i_TopLeftX, S32 i_TopLeftY, PopupPage_Z& i_Page) {
    int l_MaxTextLength = 0;
    if (0 >= i_Page.m_Items.GetSize()) {
        return;
    }

    Renderer_Z* l_Renderer = gData.MainRdr;

    Color l_Color;
    l_Color.Set(0.2, 0.2, 0.2, 1.0f);
    Color l_RedColor;
    l_RedColor.Set(1.0f, 0.0f, 0.0f, 1.0f);
    Color l_GreenColor;
    l_GreenColor.Set(0.0f, 1.0f, 0.0f, 1.0f);
    S32 i;
    for (i = 0; i < i_Page.m_Items.GetSize(); i++) {
        if (i_Page.m_Items[i].m_Id < POPUP_ITEM_STANDARD) {
            l_MaxTextLength = Max(l_MaxTextLength, (int)i_Page.m_Items[i].m_Text.StrLen() + 2);
        }
        else {
            l_MaxTextLength = Max(l_MaxTextLength, (int)i_Page.m_Items[i].m_Text.StrLen());
        }
    }

    Vec2f l_TopLeft;
    l_TopLeft.x = (Float)i_TopLeftX;
    l_TopLeft.y = (Float)i_TopLeftY;
    Vec2f l_BottomRight;
    l_BottomRight.x = 8.0f + (10.0f + (Float)(l_TopLeft.x + l_MaxTextLength * 8));
    l_BottomRight.y = (Float)(l_TopLeft.y + i_Page.m_Items.GetSize() * 10) + 4.0f;

    l_Renderer->SetBlankMaterial();
    l_Renderer->DrawQuad(l_TopLeft, l_BottomRight, l_Color, 1.001f);

    Float l_YIncrement = 10.0f;
    l_Color.Set(1.0f, 1.0f, 1.0f, 1.0f);
    l_TopLeft = Vec2f((Float)(i_TopLeftX + 4), (Float)(i_TopLeftY + 2));
    for (i = 0; i < i_Page.m_Items.GetSize(); i++) {
        PopupItem_Z& l_Item = i_Page.m_Items[i];

        if (l_Item.m_State) {
            l_Item.m_State(&l_Item);
        }

        Color l_ItemColor;
        if (i == i_Page.m_SelectedItemIdx) {
            l_ItemColor.Set(0.0f, 1.0f, 1.0f, 1.0f);
        }
        else {
            l_ItemColor = l_Color;
        }

        if (l_Item.m_Id == POPUP_ITEM_PAGE) {
            l_Renderer->DrawString(l_TopLeft, l_Item.m_Text, l_ItemColor, 0.1, 1.0f);

            l_TopLeft.x += l_Item.m_Text.StrLen() * 8 + 10;
            l_Renderer->DrawString(l_TopLeft, "->", l_Color, 0.1, 1.0f);
            l_TopLeft.x -= l_Item.m_Text.StrLen() * 8 + 10;

            if (l_Item.m_Value == 1) {
                DrawPage((S32)l_BottomRight.x + 4, (S32)l_TopLeft.y - 2, m_Pages[l_Item.m_PageIdx]);
            }
        }
        else if (l_Item.m_Id == POPUP_ITEM_SEPARATOR) {
            l_Renderer->DrawString(l_TopLeft, "-----------", l_ItemColor, 0.1, 1.0f);
        }
        else {
            if (l_Item.m_Toggled == 1) {
                l_Renderer->DrawString(l_TopLeft, "X", l_GreenColor, 0.1, 1.0f);
            }
            else if (l_Item.m_Toggled == 0) {
                l_Renderer->DrawString(l_TopLeft, "0", l_RedColor, 0.1, 1.0f);
            }

            l_TopLeft.x += 10.0f;
            l_Renderer->DrawString(l_TopLeft, l_Item.m_Text, l_ItemColor, 0.1, 1.0f);
            l_TopLeft.x -= 10.0f;
        }
        l_TopLeft.y += l_YIncrement;
    }
}
