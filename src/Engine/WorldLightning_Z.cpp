#include "World_Z.h"
#include "Node_Z.h"
#include "SystemDatas_Z.h"
#include "Lod_Z.h"

void World_Z::UpdateLighting(Bool i_Force) {
    Node_Z* l_Root = m_RootNodeHdl;

    if (l_Root->GetFlag() & FL_NODE_UPDATE_LIGHTING) {
        LightData_Z* l_LightData = i_Force ? (LightData_Z*)gData.SystemDatas->GetDefaultLight() : l_Root->GetFirstDLight();

        if (!l_LightData) {
            l_LightData = gData.SystemDatas->GetDefaultLight();
        }

        HFogData_Z* l_HFogData;
        if (i_Force) {
            l_HFogData = NULL;
        }
        else {
            l_HFogData = l_Root->GetFirstHFog();
        }

        l_Root->SetLighting(l_LightData, l_HFogData, FALSE);
        l_Root->DisableFlag(FL_NODE_UPDATE_LIGHTING);
    }
}

void World_Z::UpdateLodData() {
    Node_ZHdlDA l_Nodes;
    S32 l_NbNodes = GetNodesByObjectType(l_Nodes, LOD_Z);

    if (l_NbNodes) {
        ColLineResult_Z l_Result;

        for (S32 i = 0; i < l_NbNodes; i++) {
            Node_Z* l_Node = l_Nodes[i];
            Lod_Z* l_Lod = (Lod_Z*)l_Node->GetObject();
            LodData_Z* l_LodData = (LodData_Z*)l_Node->GetObjectDatas();

            if (l_LodData->GetActorData()) {
                Vec3f l_Origin = l_Node->GetWorldTranslation() + Vec3f(0.0f, 0.5f, 0.0f);

                if (l_LodData) {
                    l_Node->EnableFlag(l_Node->IsFlagEnable(FL_NODE_COLLIDE) ? FL_NODE_UNCOLLIDED : FL_NODE_NONE);
                    l_Node->DisableFlag(FL_NODE_COLLIDE);

                    Segment_Z l_Segment;
                    l_Segment.Origin = l_Origin;
                    l_Segment.Direction = Vec3f(0.0f, -1.0f, 0.0f);
                    l_Segment.Length = 10.0f;

                    if (GetCollisionLines(l_Segment, l_Result, FL_MTL_COL_COLLISION | FL_COL_MESH_CYLINDRE_PRIM | FL_COL_MESH_BOX_PRIM | FL_COL_LOD_CYLINDRE_PRIM | FL_COL_LOD_BOX_PRIM, FL_MTL_COL_DEPTH | FL_COL_SPLINE)) {
                        if (l_LodData->SetLightingData(l_Node, l_Result.m_Node, 0.0f)) {
                            l_Node->SetHFog(l_Result.m_Node->GetHFog());
                        }
                    }

                    Sphere_Z l_Sphere;
                    if (l_Lod->GetWorldShadowSphere(l_Node, l_Sphere)) {
                        l_Sphere.Center.y += 0.1f;

                        Segment_Z l_ShadowSegment;
                        l_ShadowSegment.Origin = l_Sphere.Center;
                        l_ShadowSegment.Direction = Vec3f(0.0f, -1.0f, 0.0f);
                        l_ShadowSegment.Length = 10.0f;

                        if (GetCollisionLines(l_ShadowSegment, l_Result, FL_MTL_COL_COLLISION | FL_COL_MESH_CYLINDRE_PRIM | FL_COL_MESH_BOX_PRIM | FL_COL_LOD_CYLINDRE_PRIM | FL_COL_LOD_BOX_PRIM, FL_MTL_COL_DEPTH | FL_COL_SPLINE)) {
                            l_LodData->SetShadowData(l_Node, l_Sphere.Center, l_Result, l_Sphere.Radius);

                            if (l_Result.m_Flag & FL_MTL_COL_NOSHADOW_LOD) {
                                l_Node->DisableFlag(FL_NODE_SHADOW);
                            }

                            if (l_Result.m_Flag & FL_MTL_COL_VREFLECT) {
                                l_LodData->EnableFlag(FL_OBJECTDATAS_VREFLECT);
                            }
                        }
                    }

                    l_Node->EnableFlag(l_Node->IsFlagEnable(FL_NODE_UNCOLLIDED) ? FL_NODE_COLLIDE : FL_NODE_NONE);
                    l_Node->DisableFlag(FL_NODE_UNCOLLIDED);
                }
            }
        }
    }
}
