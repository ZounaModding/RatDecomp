#include "World_Z.h"
#include "Renderer_Z.h"
#include "Warp_Z.h"
#include "HField_Z.h"
#include "GenWorld_Z.h"
#include "SubWorld_Z.h"

// TODO: Finish matching
void World_Z::Draw(DrawInfo_Z& i_DrawInfo) {
    Viewport_Z* l_Vp = i_DrawInfo.m_Vp;
    Renderer_Z* l_Rdr = l_Vp->GetRenderer();

    if (!IsFlag(FL_WORLD_DISABLE_SCENE_DRAW)) {
        // Draw the skybox
        l_Rdr->PushSo(so_none);
        Float l_NullY = VEC3F_NULL.y;
        Float l_NullZ = VEC3F_NULL.z;
        i_DrawInfo.m_CameraTranslation.x = VEC3F_NULL.x;
        i_DrawInfo.m_CameraTranslation.y = l_NullY;
        i_DrawInfo.m_CameraTranslation.z = l_NullZ;
        i_DrawInfo.m_CameraTranslation.w = 1.0f;
        Warp_Z* l_Warp = GetWarp();
        if (l_Warp) {
            l_Warp->Draw(i_DrawInfo);
        }

        // Draw the scene
        i_DrawInfo.m_CameraTranslation = i_DrawInfo.m_CameraNode->GetTranslation();
        i_DrawInfo.m_World2Cam = i_DrawInfo.m_Camera->GetInverseWorldMatrix();
        l_Rdr->PushSo(so_scene);
        HField_Z* l_HField = GetHField();
        if (l_HField) {
            l_HField->Draw(i_DrawInfo, NULL);
        }
        GenWorld_Z* l_GenWorld = GetGenWorld();
        if (l_GenWorld) {
            l_GenWorld->Draw(i_DrawInfo, NULL);
        }
        DrawObjects(i_DrawInfo);
    }

    // Draw debug info
    if (!i_DrawInfo.m_IsOccluded && !(gData.m_GameFlag & FL_GAME_USE_CD) && gData.m_EngineFlag & FL_OCCLUDER_ON) {
        Vec2f l_Pos(i_DrawInfo.m_VpStartX + (i_DrawInfo.m_VpSizeX * 0.25f), i_DrawInfo.m_VpStartY + 10.0f);
        Color l_Col = Color(1.0f, 1.0f, 0.0f, 1.0f);
        l_Rdr->DrawString(l_Pos, "No Occlusion", l_Col, 2.0f, 1.0f);
    }
    const Mat4x4& l_InvMat = l_Vp->GetMatrixInv();
    l_Rdr->SetBlankMaterial();
    S32 i, j;
    for (i = 0; i < m_DebugFaces3D.GetSize(); i++) {
        Vertex3D l_Vtx[3];
        l_Vtx[0].m_Pos = l_InvMat * m_DebugFaces3D[i].m_P1;
        l_Vtx[1].m_Pos = l_InvMat * m_DebugFaces3D[i].m_P2;
        l_Vtx[2].m_Pos = l_InvMat * m_DebugFaces3D[i].m_P3;
        l_Vtx[0].m_Fog = l_Vtx[1].m_Fog = l_Vtx[2].m_Fog = 0.0f;
        l_Vtx[0].m_Color = l_Vtx[1].m_Color = l_Vtx[2].m_Color = m_DebugFaces3D[i].m_Col;
        l_Rdr->DrawStripAfterScene(l_Vtx, 1, FALSE);
    }
    for (i = 0; i < m_DebugLines2D.GetSize(); i++) {
        l_Rdr->DrawLine(l_InvMat * m_DebugLines2D[i].m_P1, l_InvMat * m_DebugLines2D[i].m_P2, m_DebugLines2D[i].m_Col, TRUE, FALSE);
    }

    if (gData.m_EngineFlag & FL_STREAMZONE && i_DrawInfo.m_Camera) {
        PosName2D_Z l_DebugName;
        l_DebugName.m_Pos.x = 10.0f;
        l_DebugName.m_Pos.y = 140.0f;
        Vec2f l_CamPos2D;
        Float l_CamPosY = i_DrawInfo.m_CameraTranslation.z;
        Float l_CamPosX = i_DrawInfo.m_CameraTranslation.x;
        l_CamPos2D.x = l_CamPosX;
        l_CamPos2D.y = l_CamPosY;
        Vec2f l_StreamDebugPos;
        Float l_StreamDebugY = i_DrawInfo.m_VpStartY + (i_DrawInfo.m_VpSizeY * 0.5f) + 200.0f;
        Float l_StreamDebugX = i_DrawInfo.m_VpStartX + (i_DrawInfo.m_VpSizeX * 0.5f);
        l_StreamDebugPos.x = l_StreamDebugX;
        l_StreamDebugPos.y = l_StreamDebugY;
        Float l_CameraYaw = O_Atan2(i_DrawInfo.m_Camera->GetTarget().x - l_CamPos2D.x, i_DrawInfo.m_Camera->GetTarget().z - l_CamPos2D.y);
        Float l_Angle = Pi - l_CameraYaw;
        Vec2f l_Rotation;
        Float l_Sin = O_Sin(l_Angle);
        l_Rotation.x = O_Cos(l_Angle);
        l_Rotation.y = l_Sin;

        S32 l_SubWorldDataNb = m_SubWorldDatas.GetSize();
        if (l_SubWorldDataNb) {
            SubWorld_Z::SubWorldDisplayID = Clamp(SubWorld_Z::SubWorldDisplayID, (S32)0, l_SubWorldDataNb - 1);
            SubWorldData_Z& l_DisplayedSubWorldData = m_SubWorldDatas[SubWorld_Z::SubWorldDisplayID];
            for (i = 0; i < m_SubWorldDatas.GetSize(); i++) {
                SubWorldData_Z& l_SubWorldData = m_SubWorldDatas[i];
                if (l_SubWorldData.m_Type == SUBWORLD_TYPE_SUBLEVEL) {
                    Color l_DisplayColor = Color(0.5, 0.5, 0.5, 1.0); // grey
                    l_DebugName.m_Str.Empty();
                    if (l_SubWorldData.m_Flag & FL_SUBWORLD_REMOVING) {
                        l_DebugName.m_Str.StrCpy("Removing");
                        l_DisplayColor = Color(0.0, 0.0, 1.0, 1.0); // blue
                    }
                    else if (l_SubWorldData.m_Flag & FL_SUBWORLD_LOADED) {
                        l_DebugName.m_Str.StrCpy("Loaded  ");
                        l_DisplayColor = Color(0.0, 1.0, 0.0, 1.0); // green
                    }
                    else if (l_SubWorldData.m_Flag & FL_SUBWORLD_LOADING) {
                        l_DebugName.m_Str.StrCpy("Loading ");
                        l_DisplayColor = Color(1.0, 0.0, 0.0, 1.0); // red
                    }
                    else {
                        l_DebugName.m_Str.StrCpy("        ");
                    }

                    Bool l_FoundOther = FALSE;
                    if (i != SubWorld_Z::SubWorldDisplayID) {
                        S32 l_SubIdNb = l_DisplayedSubWorldData.m_SubIds.GetSize();
                        j = 0;
                        while (j < l_SubIdNb) {
                            if (l_DisplayedSubWorldData.m_SubIds[j] == l_SubWorldData.m_SubId) {
                                break;
                            }
                            j++;
                        }
                        if (j == l_SubIdNb) {
                            l_DebugName.m_Str.StrCat("  ");
                        }
                        else {
                            l_DebugName.m_Str.StrCat(" >");
                            l_FoundOther = TRUE;
                        }
                    }
                    else {
                        l_DebugName.m_Str.StrCat("* ");
                    }

                    if (SubWorld_Z::SubWorldDisplaySelection) {
                        if (i == SubWorld_Z::SubWorldDisplayID) {
                            l_DisplayColor = Color(1.0, 1.0, 0.0, 1.0); // yellow
                            for (j = 0; j < l_SubWorldData.m_Range.m_Boxes.GetSize(); j++) {
                                Vec3f l_BoxVtx[8];
                                Vec2f l_BoxVtx2D[8];
                                l_SubWorldData.m_Range.m_Boxes[j].GetVtx(l_BoxVtx);
                                for (S32 k = 0; k < 8; k++) {
                                    Vec2f l_Point(l_BoxVtx[k].x, l_BoxVtx[k].z);
                                    Renderer_Z::GetScreen2DPt(l_BoxVtx2D[k], l_Point, l_CamPos2D, l_StreamDebugPos, l_Rotation, SubWorld_Z::SubWorldDisplayScale);
                                }
                                l_Rdr->DrawLine(l_BoxVtx2D[0], l_BoxVtx2D[1], l_DisplayColor, 0.0f);
                                l_Rdr->DrawLine(l_BoxVtx2D[1], l_BoxVtx2D[2], l_DisplayColor, 0.0f);
                                l_Rdr->DrawLine(l_BoxVtx2D[2], l_BoxVtx2D[3], l_DisplayColor, 0.0f);
                                l_Rdr->DrawLine(l_BoxVtx2D[3], l_BoxVtx2D[0], l_DisplayColor, 0.0f);
                                l_Rdr->DrawLine(l_BoxVtx2D[4], l_BoxVtx2D[5], l_DisplayColor, 0.0f);
                                l_Rdr->DrawLine(l_BoxVtx2D[5], l_BoxVtx2D[6], l_DisplayColor, 0.0f);
                                l_Rdr->DrawLine(l_BoxVtx2D[6], l_BoxVtx2D[7], l_DisplayColor, 0.0f);
                                l_Rdr->DrawLine(l_BoxVtx2D[7], l_BoxVtx2D[4], l_DisplayColor, 0.0f);
                                l_Rdr->DrawLine(l_BoxVtx2D[0], l_BoxVtx2D[4], l_DisplayColor, 0.0f);
                                l_Rdr->DrawLine(l_BoxVtx2D[1], l_BoxVtx2D[5], l_DisplayColor, 0.0f);
                                l_Rdr->DrawLine(l_BoxVtx2D[2], l_BoxVtx2D[6], l_DisplayColor, 0.0f);
                                l_Rdr->DrawLine(l_BoxVtx2D[3], l_BoxVtx2D[7], l_DisplayColor, 0.0f);
                            }
                        }
                        if (l_FoundOther) {
                            for (j = 0; j < l_SubWorldData.m_Range.m_Boxes.GetSize(); j++) {
                                Vec3f l_BoxVtx[8];
                                Vec2f l_BoxVtx2D[8];
                                l_SubWorldData.m_Range.m_Boxes[j].GetVtx(l_BoxVtx);
                                for (S32 k = 0; k < 8; k++) {
                                    Vec2f l_Point(l_BoxVtx[k].x, l_BoxVtx[k].z);
                                    Renderer_Z::GetScreen2DPt(l_BoxVtx2D[k], l_Point, l_CamPos2D, l_StreamDebugPos, l_Rotation, SubWorld_Z::SubWorldDisplayScale);
                                }
                                l_Rdr->DrawLine(l_BoxVtx2D[0], l_BoxVtx2D[1], l_DisplayColor, 0.0f);
                                l_Rdr->DrawLine(l_BoxVtx2D[1], l_BoxVtx2D[2], l_DisplayColor, 0.0f);
                                l_Rdr->DrawLine(l_BoxVtx2D[2], l_BoxVtx2D[3], l_DisplayColor, 0.0f);
                                l_Rdr->DrawLine(l_BoxVtx2D[3], l_BoxVtx2D[0], l_DisplayColor, 0.0f);
                                l_Rdr->DrawLine(l_BoxVtx2D[4], l_BoxVtx2D[5], l_DisplayColor, 0.0f);
                                l_Rdr->DrawLine(l_BoxVtx2D[5], l_BoxVtx2D[6], l_DisplayColor, 0.0f);
                                l_Rdr->DrawLine(l_BoxVtx2D[6], l_BoxVtx2D[7], l_DisplayColor, 0.0f);
                                l_Rdr->DrawLine(l_BoxVtx2D[7], l_BoxVtx2D[4], l_DisplayColor, 0.0f);
                                l_Rdr->DrawLine(l_BoxVtx2D[0], l_BoxVtx2D[4], l_DisplayColor, 0.0f);
                                l_Rdr->DrawLine(l_BoxVtx2D[1], l_BoxVtx2D[5], l_DisplayColor, 0.0f);
                                l_Rdr->DrawLine(l_BoxVtx2D[2], l_BoxVtx2D[6], l_DisplayColor, 0.0f);
                                l_Rdr->DrawLine(l_BoxVtx2D[3], l_BoxVtx2D[7], l_DisplayColor, 0.0f);
                            }
                        }
                    }

                    if (l_SubWorldData.m_Range.m_PolyVertices.GetSize()) {
                        Vec2f l_Point;
                        Vec2f l_PreviousPoint;
                        Vec2f l_FirstPoint;
                        Renderer_Z::GetScreen2DPt(l_FirstPoint, l_SubWorldData.m_Range.m_PolyVertices[0], l_CamPos2D, l_StreamDebugPos, l_Rotation, SubWorld_Z::SubWorldDisplayScale);
                        l_PreviousPoint = l_FirstPoint;
                        for (j = 1; j < l_SubWorldData.m_Range.m_PolyVertices.GetSize(); j++) {
                            Renderer_Z::GetScreen2DPt(l_Point, l_SubWorldData.m_Range.m_PolyVertices[j], l_CamPos2D, l_StreamDebugPos, l_Rotation, SubWorld_Z::SubWorldDisplayScale);
                            l_Rdr->DrawLine(l_PreviousPoint, l_Point, l_DisplayColor, 0.0f);
                            l_PreviousPoint = l_Point;
                        }
                        l_Rdr->DrawLine(l_PreviousPoint, l_FirstPoint, l_DisplayColor, 0.0f);
                    }

                    l_DebugName.m_Str.StrCat(m_SubWorldDatas[i].m_SubWorldName);
                    l_DebugName.m_Col = l_DisplayColor;
                    m_DebugNames2D.Add(l_DebugName);
                    l_DebugName.m_Pos.y += 10.0f;
                }
            }
        }

        Frustrum2D_Z l_Frustrum;
        i_DrawInfo.m_Camera->GetFrustrum2D(l_Frustrum);
        Vec2f l_Point;
        Vec2f l_PreviousPoint;
        Vec2f l_FirstPoint;
        Renderer_Z::GetScreen2DPt(l_PreviousPoint, l_Frustrum.m_Points[0], l_CamPos2D, l_StreamDebugPos, l_Rotation, SubWorld_Z::SubWorldDisplayScale);
        l_FirstPoint = l_PreviousPoint;
        for (i = 1; i < l_Frustrum.m_PointNb; i++) {
            Renderer_Z::GetScreen2DPt(l_Point, l_Frustrum.m_Points[i], l_CamPos2D, l_StreamDebugPos, l_Rotation, SubWorld_Z::SubWorldDisplayScale);
            l_Rdr->DrawLine(l_PreviousPoint, l_Point, Color(1.0f, 0.0f, 0.0f, 1.0f), 0.0f);
            l_PreviousPoint = l_Point;
        }
        l_Rdr->DrawLine(l_PreviousPoint, l_FirstPoint, Color(1.0f, 0.0f, 0.0f, 1.0f), 0.0f);
    }

    for (i = 0; i < m_DebugNames.GetSize(); i++) {
        l_Rdr->DrawString(l_InvMat * m_DebugNames[i].m_Pos, m_DebugNames[i].m_Str, m_DebugNames[i].m_Col, TRUE);
    }
    for (i = 0; i < m_DebugNames2D.GetSize(); i++) {
        l_Rdr->DrawString(m_DebugNames2D[i].m_Pos, m_DebugNames2D[i].m_Str, m_DebugNames2D[i].m_Col, 0.1f, 1.0f);
    }
}

void World_Z::EndDraw() {
    m_DebugLines2D.Empty();
    m_DebugNames.Empty();
    m_DebugNames2D.Empty();
    m_DebugFaces3D.Empty();
}

// TODO: Finish matching
void World_Z::DrawObjects(DrawInfo_Z& i_DrawInfo) {
    U32 l_NodeNoFlag = (FL_NODE_1VP_HIDE << (i_DrawInfo.m_VpCount - 1)) | FL_NODE_HIDE;
    U32 l_ObjectDatasNoFlag = FL_OBJECTDATAS_VP0_HIDE << i_DrawInfo.m_VpId;
    U32 l_ObjectDatasFlag;

    SeadZoneData_Z l_SeadZoneDatas1[SEAD_RECURSE_MAX_NODE * 3];
    SeadZoneData_Z l_SeadZoneDatas2[SEAD_RECURSE_MAX_NODE * 3];
    SeadZone_Z l_SeadZone1(l_SeadZoneDatas1, SEAD_RECURSE_MAX_NODE * 3, FL_NODE_COLLIDE, FL_NODE_HIDE);
    SeadZone_Z l_SeadZone2(l_SeadZoneDatas2, SEAD_RECURSE_MAX_NODE * 3, FL_NODE_COLLIDE, FL_NODE_HIDE);
    StaticArray_Z<OmniFrust_Z, 64, FALSE, FALSE> l_OmniFrusts;

    if ((gData.m_EngineFlag & FL_LOCK_VIEW) && m_DebugLockViewSeadZoneDatas[i_DrawInfo.m_VpId].GetSize()) {
        l_SeadZone1.m_SeadZoneDataNb = m_DebugLockViewSeadZoneDatas[i_DrawInfo.m_VpId].GetSize();
        l_SeadZone1.m_HeadSeadZoneData = m_DebugLockViewSeadZoneDatas[i_DrawInfo.m_VpId].GetArrayPtr();
        l_SeadZone1.m_TailSeadZoneData = l_SeadZone1.m_HeadSeadZoneData;
        Sys_Z::MemCpyFrom(l_SeadZoneDatas1, l_SeadZone1.m_HeadSeadZoneData, l_SeadZone1.m_SeadZoneDataNb * sizeof(SeadZoneData_Z));

        BitArray_Z* l_OccludedZones = &m_DebugLockViewOccludedZonesBAs[i_DrawInfo.m_VpId];
        if (!l_OccludedZones->GetSize()) {
            l_OccludedZones = NULL;
        }
        i_DrawInfo.m_OccludedZonesBA = l_OccludedZones;
        i_DrawInfo.m_Camera = &m_DebugLockViewCameras[i_DrawInfo.m_VpId];
        i_DrawInfo.m_Camera->GetOccluded(i_DrawInfo);
    }
    else {
        i_DrawInfo.m_Camera->GetOccluded(i_DrawInfo);
        m_SeadDisplay.SetFrustrum(l_OmniFrusts, l_SeadZone1, l_SeadZone2, i_DrawInfo, 0, l_NodeNoFlag);
        m_DebugLockViewSeadZoneDatas[i_DrawInfo.m_VpId].Empty();

        if (gData.m_EngineFlag & FL_LOCK_VIEW) {
            SeadZoneData_Z* l_SeadZoneData = l_SeadZone1.m_HeadSeadZoneData;
            S32 l_SeadZoneDataNb = l_SeadZone1.m_SeadZoneDataNb;
            while (l_SeadZoneDataNb--) {
                l_SeadZoneData->m_Node->DisableFlag(FL_NODE_WAS_VISIBLE);
                l_SeadZoneData++;
            }
        }
    }

    SeadZoneData_Z* l_SeadZoneData = l_SeadZone1.m_HeadSeadZoneData;
    S32 l_SeadZoneDataNb = l_SeadZone1.m_SeadZoneDataNb;
    Frustrum_Z& l_Frustrum = i_DrawInfo.m_Camera->GetFrustrum();
    i_DrawInfo.m_MainHFogData = m_MainHFogDataHdl;
    l_ObjectDatasNoFlag |= FL_OBJECTDATAS_HIDE;
    Object_Z* l_Object;

    for (; l_SeadZoneDataNb != 0; l_SeadZoneDataNb--, l_SeadZoneData++) {
        ObjectDatas_Z* l_ObjectDatas = l_SeadZoneData->m_ObjectDatas;
        if (l_ObjectDatas) {
            l_ObjectDatasFlag = l_ObjectDatas->GetFlag();
            if (l_ObjectDatasFlag & l_ObjectDatasNoFlag) {
                continue;
            }
            l_ObjectDatasFlag &= FL_OBJECTDATAS_DISPLAY;
        }
        else {
            l_ObjectDatasFlag = 0;
        }
        l_Object = l_SeadZoneData->m_Object;
        Node_Z* l_Node = l_SeadZoneData->m_Node;
        U32 l_DrawObject = l_Object->GetFlag() & FL_OBJECT_DISPLAY;
        if ((l_DrawObject | l_ObjectDatasFlag) && SphereVsFrustrum(l_Frustrum, i_DrawInfo, l_Node)) {
            if ((i_DrawInfo.m_ClipSph.Flag & (FL_CLIPPLANE_NEAR | FL_CLIPPLANE_SIDES)) && l_Object->IsFlag(FL_OBJECT_UNK_0x10)) {
                U32 l_ClipFlag;
                if (!BoxVsFrustrumAndFlags(l_Frustrum, i_DrawInfo.m_Local2Cam, l_Object->GetBBox(), l_ClipFlag, l_Node->GetUniformScale())) {
                    continue;
                }
                if (!(l_ClipFlag & (FL_CLIPPLANE_NEAR | FL_CLIPPLANE_SIDES | FL_CLIPPLANE_FAR))) {
                    i_DrawInfo.m_ClipSph.Flag = 0;
                }
            }

            i_DrawInfo.m_Node = l_Node;
            i_DrawInfo.m_LightData = l_Node->GetLight();
            i_DrawInfo.m_CurrentNodeHFogData = l_Node->GetHFog();
            i_DrawInfo.m_FadeValue = 1.0f;
            if (l_DrawObject) {
                if (l_OmniFrusts.GetSize()) {
                    if (l_Node->IsFlagEnable(FL_NODE_NO_OMNI)) {
                        i_DrawInfo.m_OmniFrustPtrSA.Empty();
                    }
                    else {
                        GetOmniOnClip(i_DrawInfo, l_OmniFrusts);
                    }
                }
                l_Object->Draw(i_DrawInfo, l_ObjectDatas);
            }
            if (l_ObjectDatasFlag) {
                l_ObjectDatas->Draw(i_DrawInfo);
            }
            l_Node->EnableFlag(FL_NODE_WAS_VISIBLE);
        }
    }

    if ((gData.m_EngineFlag & FL_LOCK_VIEW) && m_DebugLockViewSeadZoneDatas[i_DrawInfo.m_VpId].GetSize() == 0) {
        l_SeadZoneData = l_SeadZone1.m_HeadSeadZoneData;
        l_SeadZoneDataNb = l_SeadZone1.m_SeadZoneDataNb;
        while (l_SeadZoneDataNb--) {
            if (l_SeadZoneData->m_Node->IsFlagEnable(FL_NODE_WAS_VISIBLE)) {
                m_DebugLockViewSeadZoneDatas[i_DrawInfo.m_VpId].Add(*l_SeadZoneData);
            }
            l_SeadZoneData++;
        }

        m_DebugLockViewOccludedZonesBAs[i_DrawInfo.m_VpId].Flush();
        if (i_DrawInfo.m_OccludedZonesBA) {
            m_DebugLockViewOccludedZonesBAs[i_DrawInfo.m_VpId] = *i_DrawInfo.m_OccludedZonesBA;
        }
        m_DebugLockViewCameras[i_DrawInfo.m_VpId] = *i_DrawInfo.m_Camera;
    }

    S32 i;
    for (i = 0; i < m_NoFrustumClippingNodeHdls.GetSize(); i++) {
        Node_Z* l_Node = m_NoFrustumClippingNodeHdls[i];
        if (l_Node->GetFlag() & l_NodeNoFlag) {
            continue;
        }
        ObjectDatas_Z* l_ObjectDatas = l_Node->GetObjectDatas();
        if (l_ObjectDatas) {
            l_ObjectDatasFlag = l_ObjectDatas->GetFlag();
            if (l_ObjectDatasFlag & l_ObjectDatasNoFlag) {
                continue;
            }
        }
        l_Object = l_Node->GetObjectA();
        U32 l_DrawObject = l_Object->GetFlag() & FL_OBJECT_DISPLAY;
        Bool l_DrawObjectDatas = l_ObjectDatas && (l_ObjectDatasFlag & FL_OBJECTDATAS_DISPLAY);
        if (l_DrawObject || l_DrawObjectDatas) {
            i_DrawInfo.m_Node = l_Node;
            i_DrawInfo.m_LightData = l_Node->GetLight();
            i_DrawInfo.m_CurrentNodeHFogData = l_Node->GetHFog();
            i_DrawInfo.m_FadeValue = 1.0f;
            if (l_DrawObject) {
                if (l_OmniFrusts.GetSize()) {
                    if (l_Node->IsFlagEnable(FL_NODE_NO_OMNI)) {
                        i_DrawInfo.m_OmniFrustPtrSA.Empty();
                    }
                    else {
                        i_DrawInfo.m_ClipSph.Sph = i_DrawInfo.m_World2Cam * l_Node->GetBSphere();
                        GetOmniOnClip(i_DrawInfo, l_OmniFrusts);
                    }
                }
                l_Object->Draw(i_DrawInfo, l_ObjectDatas);
            }
            if (l_DrawObjectDatas) {
                l_ObjectDatas->Draw(i_DrawInfo);
            }
        }
    }

    Camera_Z* l_Camera = i_DrawInfo.m_Camera;
    Float l_OccludedFarClip = l_Camera->GetFrustrum().m_OccludedFarClip;
    l_Camera->SetOccludedFarClip(l_Camera->GetFrustrum().m_FarClip);
    for (i = 0; i < m_NoOccluderClippingNodeHdls.GetSize(); i++) {
        Node_Z* l_Node = m_NoOccluderClippingNodeHdls[i];
        if (l_Node->GetFlag() & l_NodeNoFlag) {
            continue;
        }
        ObjectDatas_Z* l_ObjectDatas = l_Node->GetObjectDatas();
        if (l_ObjectDatas) {
            l_ObjectDatasFlag = l_ObjectDatas->GetFlag();
            if (l_ObjectDatasFlag & l_ObjectDatasNoFlag) {
                continue;
            }
        }
        l_Object = l_Node->GetObjectA();
        U32 l_DrawObject = l_Object->GetFlag() & FL_OBJECT_DISPLAY;
        if ((l_DrawObject | l_ObjectDatasFlag) && SphereVsFrustrum(l_Frustrum, i_DrawInfo, l_Node)) {
            i_DrawInfo.m_Node = l_Node;
            i_DrawInfo.m_LightData = l_Node->GetLight();
            i_DrawInfo.m_CurrentNodeHFogData = l_Node->GetHFog();
            i_DrawInfo.m_FadeValue = 1.0f;
            if (l_DrawObject) {
                if (l_OmniFrusts.GetSize()) {
                    if (l_Node->IsFlagEnable(FL_NODE_NO_OMNI)) {
                        i_DrawInfo.m_OmniFrustPtrSA.Empty();
                    }
                    else {
                        GetOmniOnClip(i_DrawInfo, l_OmniFrusts);
                    }
                }
                l_Object->Draw(i_DrawInfo, l_ObjectDatas);
            }
            if (l_ObjectDatasFlag) {
                l_ObjectDatas->Draw(i_DrawInfo);
            }
        }
    }
    i_DrawInfo.m_Camera->SetOccludedFarClip(l_OccludedFarClip);
}

void World_Z::GetOmniOnClip(DrawInfo_Z& i_DrawInfo, StaticArray_Z<OmniFrust_Z, 64, FALSE, FALSE>& io_OmniFrusts) {
    i_DrawInfo.m_OmniFrustPtrSA.Empty();
    OmniFrust_Z* l_OmniFrust = &io_OmniFrusts[0];
    S32 l_OmniFrustNb = io_OmniFrusts.GetSize();
    while (l_OmniFrustNb--) {
        Float l_Radius = i_DrawInfo.m_ClipSph.Sph.Radius + l_OmniFrust->m_BSphereCamSpace.Radius;
        if ((i_DrawInfo.m_ClipSph.Sph.Center - l_OmniFrust->m_BSphereCamSpace.Center).GetNorm2() < l_Radius * l_Radius) {
            i_DrawInfo.m_OmniFrustPtrSA.Add(l_OmniFrust);
            if (i_DrawInfo.m_OmniFrustPtrSA.GetSize() == 64) {
                break;
            }
        }
        l_OmniFrust++;
    }
}

void World_Z::AddManipulatorSceneDraw(const ManipulatorSceneDraw_ZHdl& i_ManipulatorSceneDrawHdl) {
    S32 l_ManipulatorSceneDrawNb = m_ManipulatorSceneDrawHdls.GetSize();
    for (S32 i = 0; i < l_ManipulatorSceneDrawNb; i++) {
        if (i_ManipulatorSceneDrawHdl == m_ManipulatorSceneDrawHdls[i]) {
            return;
        }
    }
    m_ManipulatorSceneDrawHdls.Add(i_ManipulatorSceneDrawHdl);
}
