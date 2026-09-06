#include "CameraEngineZone_Z.h"
#include "ABC_Message_Engine.h"
#include "Camera_Z.h"
#include "CameraZone_Z.h"
#include "LodMove_Z.h"
#include "Node_Z.h"
#include "World_Z.h"

#define SMOOTH_FACTOR_NORMAL 8.0f
#define SMOOTH_FACTOR_REMOVED 1.01f
#define SMOOTH_FACTOR_TIME_REACH 5.0f

void CameraEngineZone_Z::Init() {
    CameraEngine_Z::Init();
    CaptureInit();
    m_CameraWorldFocus = VEC3F_NULL;
    m_FixOrFocusRemoved = FALSE;
    m_SmoothFactor = SMOOTH_FACTOR_NORMAL;
    m_SmoothedFocus = Vec3f(0.0f, 1.0f, 1.0f);
    m_SmoothedCameraDistance = VEC3F_NULL;
    m_First = TRUE;
}

void CameraEngineZone_Z::Reset() {
    if (m_LodMoveHdl) {
        m_LodMoveHdl->EnableFixedCamera();
        m_LodMoveHdl->EnableFocusCamera();
    }

    m_CameraWorldFocus = VEC3F_NULL;
    m_FixOrFocusRemoved = FALSE;
    m_SmoothFactor = SMOOTH_FACTOR_NORMAL;
    m_SmoothedFocus = Vec3f(0.0f, 1.0f, 1.0f);
    m_SmoothedCameraDistance = VEC3F_NULL;
    m_First = TRUE;
}

void CameraEngineZone_Z::InterpMessage(const int& i_Message, Float i_Param) {
    if (i_Message == msg_camera_capture_patch_start) {
        CaptureStart(CAMERA_ENGINE_CAPTURE_TYPE_PATCH);
    }
    if (i_Message == msg_camera_capture_frame_start) {
        CaptureStart(CAMERA_ENGINE_CAPTURE_TYPE_FRAME);
    }
    if (i_Message == msg_camera_capture_texture_start) {
        CaptureStart(CAMERA_ENGINE_CAPTURE_TYPE_TEXTURE);
    }
    if (i_Message == msg_camera_capture_end) {
        CaptureEnd();
    }
    if (i_Message == msg_camera_capture_param_1) {
        CaptureParams(CAMERA_ENGINE_CAPTURE_PARAM_FRAMERATE, i_Param);
    }
    if (i_Message == msg_camera_capture_param_2) {
        CaptureParams(CAMERA_ENGINE_CAPTURE_PARAM_FRAME_NB, i_Param);
    }
    if (i_Message == msg_camera_capture_param_3) {
        CaptureParams(CAMERA_ENGINE_CAPTURE_PARAM_PATCH_NB, i_Param);
    }
    if (i_Message == msg_camera_reset_focus) {
        m_CameraWorldFocus = VEC3F_NULL;
    }
}

void CameraEngineZone_Z::Update(Float i_DeltaTime) {
    if (m_Capture) {
        CaptureDo(i_DeltaTime);
        UpdateFX(i_DeltaTime);
        return;
    }

    Node_Z* l_CameraNode = GetCameraNode();
    Camera_Z* l_Camera = (Camera_Z*)l_CameraNode->GetObject();
    World_Z* l_World = GetWorld();
    CameraZoneData_Z l_CameraZoneData;
    Vec3f l_WorldPosition;
    LodMove_Z* l_Lod = m_LodMoveHdl;

    if (!l_Lod) {
        if (l_World->GetCameraZone(VEC3F_NULL, l_CameraZoneData)) {
            CameraZoneInterpolData_Z l_DataInterpol;
            CameraZone_Z* l_CameraZone = l_CameraZoneData.m_CameraZoneHdl;
            if (l_CameraZone->GetCameraZoneInterpol(VEC3F_NULL, VEC3F_NULL, l_CameraZoneData, l_DataInterpol)) {
                l_Camera->SetTarget(l_DataInterpol.m_CamFocus);
                l_Camera->SetFov(l_DataInterpol.m_CamFov);
                l_Camera->SetFarClip(l_DataInterpol.m_CamFar);
                l_CameraNode->SetTranslation(l_DataInterpol.m_CamPos);
                l_CameraNode->Changed();
                l_CameraNode->Update();
            }
        }
        UpdateFX(i_DeltaTime);
        return;
    }

    Node_ZHdl l_NodeHdl = l_Lod->GetNode(0);

    if (!l_Lod->IsFixedCameraEnable() || !l_Lod->IsFocusCameraEnable()) {
        Node_Z* l_FromNode = l_Lod->GetCameraNodeDisable();
        l_WorldPosition = l_FromNode->GetWorldTranslation();
        if (l_World->GetCameraZone(l_WorldPosition, l_CameraZoneData, l_Lod->GetCameraId())) {
            CameraZone_Z* l_CameraZone = l_CameraZoneData.m_CameraZoneHdl;
            if (!l_Lod->IsFixedCameraEnable()) {
                for (S32 i = 0; i < l_CameraZoneData.m_NbTriggerFromZone; i++) {
                    l_CameraZone->EnableTriggerFlag(l_CameraZoneData.m_TriggerIds[i], FL_TRIGGER_NOFIX);
                }
            }
            if (!l_Lod->IsFocusCameraEnable()) {
                for (S32 i = 0; i < l_CameraZoneData.m_NbTriggerFromZone; i++) {
                    l_CameraZone->EnableTriggerFlag(l_CameraZoneData.m_TriggerIds[i], FL_TRIGGER_NOFOCUS);
                }
            }
            m_FixOrFocusRemoved = TRUE;
            m_RemovedTime = 0.0f;
        }
        l_Lod->EnableFixedCamera();
        l_Lod->EnableFocusCamera();
    }

    l_WorldPosition = l_Lod->GetPos(0) + l_Lod->GetVehicleDelta();

    Vec3f l_CameraOffset = l_Lod->GetCameraOffset();
    l_WorldPosition += l_CameraOffset;

    Smooth(l_CameraOffset, VEC3F_NULL, 1.1f, i_DeltaTime, l_CameraOffset);
    l_Lod->SetCameraOffset(l_CameraOffset);

    if (l_World->GetCameraZone(l_WorldPosition, l_CameraZoneData, l_Lod->GetCameraId())) {
        CameraZone_Z* l_CameraZone = l_CameraZoneData.m_CameraZoneHdl;
        Vec3f l_Focus = l_Lod->GetCameraFocus();

        if (m_First) {
            m_SmoothedFocus = l_Focus;
        }
        else {
            Smooth(m_SmoothedFocus, l_Focus, 1.1f, i_DeltaTime, m_SmoothedFocus);
            l_Focus = m_SmoothedFocus;
        }

        Vec3f l_Direction;
        Node_Z* l_Node = l_NodeHdl;
        l_Node->GetWorldMatrix().MulWithoutTrans(VEC3F_FRONT, l_Direction);
        Float l_Angle = O_Atan2(l_Direction.x, l_Direction.z);
        Vec3f l_Axis = Vec3f(0.0f, 1.0f, 0.0f);
        Quat l_Rotation;
        l_Rotation = Quat(l_Angle, l_Axis);

        Vec3f l_WorldFocus = l_WorldPosition + l_Rotation * l_Focus;

        CameraZoneInterpolData_Z l_DataInterpol;
        if (!l_CameraZone->GetCameraZoneInterpol(l_WorldPosition, l_WorldFocus, l_CameraZoneData, l_DataInterpol)) {
            return;
        }

        if (l_DataInterpol.m_IsInterpol && m_CameraWorldFocus != VEC3F_NULL) {
            Vec3f l_CameraDistance = l_Lod->GetCameraDistance();
            Smooth(m_SmoothedCameraDistance, l_CameraDistance, 1.5f, i_DeltaTime, m_SmoothedCameraDistance);
            l_DataInterpol.m_CamPos += m_SmoothedCameraDistance;
        }

        for (S32 i = 0; i < l_CameraZoneData.m_NbTriggerFromZone; i++) {
            if (l_CameraZone->GetTriggerFlag(l_CameraZoneData.m_TriggerIds[i]) & FL_TRIGGER_NOFOCUS) {
                break;
            }
        }

        Vec3f l_CameraDirFromZone = (l_DataInterpol.m_CamFocus - l_DataInterpol.m_CamPos).Normalize();
        l_Lod->SetCameraDirFromZone(l_CameraDirFromZone);

        if (!l_DataInterpol.m_IsInterpol || m_CameraWorldFocus == VEC3F_NULL) {
            m_CameraWorldFocus = l_DataInterpol.m_CamFocus;
        }
        else {
            if (m_FixOrFocusRemoved) {
                Vec3f l_OldCameraPosition = l_CameraNode->GetTranslation();
                Vec3f l_NewCameraPosition;

                if (m_RemovedTime >= SMOOTH_FACTOR_TIME_REACH) {
                    m_FixOrFocusRemoved = FALSE;
                    m_RemovedTime = SMOOTH_FACTOR_TIME_REACH;
                }

                Float l_Ratio = m_RemovedTime / SMOOTH_FACTOR_TIME_REACH;
                l_Ratio *= l_Ratio;
                m_SmoothFactor = SMOOTH_FACTOR_REMOVED + (SMOOTH_FACTOR_NORMAL - SMOOTH_FACTOR_REMOVED) * l_Ratio;

                Smooth(l_OldCameraPosition, l_DataInterpol.m_CamPos, m_SmoothFactor, i_DeltaTime, l_NewCameraPosition);

                l_DataInterpol.m_CamPos = l_NewCameraPosition;

                m_RemovedTime += i_DeltaTime;
            }

            Smooth(m_CameraWorldFocus, l_DataInterpol.m_CamFocus, 1.2f, i_DeltaTime, m_CameraWorldFocus);
        }

        l_DataInterpol.m_CamPos = CameraGameColl(l_World, l_DataInterpol.m_CamPos, m_CameraWorldFocus, i_DeltaTime);

        l_Camera->SetTarget(m_CameraWorldFocus);
        l_Camera->SetFov(l_DataInterpol.m_CamFov);
        l_Camera->SetFarClip(l_DataInterpol.m_CamFar);

        l_CameraNode->SetTranslation(l_DataInterpol.m_CamPos);
        l_CameraNode->Changed();
        l_Lod->SetCameraPos(l_DataInterpol.m_CamPos);
    }

    m_First = FALSE;
    l_CameraNode->Update();
    UpdateFX(i_DeltaTime);
}
