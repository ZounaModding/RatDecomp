#include "CameraEngineZone_Z.h"

void CameraEngineZone_Z::CaptureDo(Float a1) {
}

void CameraEngineZone_Z::CaptureEnd() {
}

void CameraEngineZone_Z::CaptureStart(S32 i_Type) {
    if (m_Capture) {
        return;
    }
    m_CapturedTotalTime = 0.0f;
    m_Capture = TRUE;
    m_CaptureType = i_Type;
    if (m_CaptureType == CAMERA_ENGINE_CAPTURE_TYPE_FRAME) {
        return;
    }
    m_CaptureFrameNb = 0;
}

void CameraEngineZone_Z::CaptureInit() {
    m_Capture = FALSE;
    m_CaptureBitmap = NULL;
    m_CameraZoneHdl = HANDLE_NULL;
    m_CaptureType = CAMERA_ENGINE_CAPTURE_TYPE_PATCH;
    m_CaptureFrameNb = 3;
    CaptureParams(CAMERA_ENGINE_CAPTURE_PARAM_PATCH_DELTA, 1.0f);
    CaptureParams(CAMERA_ENGINE_CAPTURE_PARAM_PATCH_NB, 250.0f);
    CaptureParams(CAMERA_ENGINE_CAPTURE_PARAM_FRAMERATE, 60.0f);
}

void CameraEngineZone_Z::CaptureParams(S32 i_Index, Float i_Param) {
    if (!m_Capture) {
        if (i_Index == CAMERA_ENGINE_CAPTURE_PARAM_PATCH_DELTA && i_Param > 0.000001f) {
            m_CaptureDelta = i_Param;
        }
        if (i_Index == CAMERA_ENGINE_CAPTURE_PARAM_PATCH_NB && i_Param) {
            m_CapturePatchMax = i_Param;
        }
        if (i_Index == CAMERA_ENGINE_CAPTURE_PARAM_FRAMERATE && i_Param) {
            m_CaptureFramerate = i_Param;
        }
    }
}
