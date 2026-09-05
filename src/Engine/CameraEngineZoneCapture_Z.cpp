#include "CameraEngineZone_Z.h"

void CameraEngineZone_Z::CaptureDo(Float a1) {
}

void CameraEngineZone_Z::CaptureEnd() {
}

void CameraEngineZone_Z::CaptureStart(S32 a1) {
}

void CameraEngineZone_Z::CaptureInit() {
    m_Capture = FALSE;
    m_CaptureBitmap = NULL;
    m_CaptureNb = 0;
    m_UnkS32_0xac = 0;
    m_CaptureType = CAMERA_ENGINE_CAPTURE_TYPE_NONE;
    CaptureParams(CAMERA_ENGINE_CAPTURE_PARAM_FRAMERATE, 1.0f);
    CaptureParams(CAMERA_ENGINE_CAPTURE_PARAM_FRAME_NB, 250.0f);
    CaptureParams(CAMERA_ENGINE_CAPTURE_PARAM_PATCH_NB, 60.0f);
}

void CameraEngineZone_Z::CaptureParams(S32 i_Index, Float i_Param) {
    if (!m_Capture) {
        if (i_Index == CAMERA_ENGINE_CAPTURE_PARAM_FRAMERATE && i_Param > 0.000001f) {
            m_CaptureFramerate = i_Param;
        }
        if (i_Index == CAMERA_ENGINE_CAPTURE_PARAM_FRAME_NB && i_Param) {
            m_CaptureFrameNb = i_Param;
        }
        if (i_Index == CAMERA_ENGINE_CAPTURE_PARAM_PATCH_NB && i_Param) {
            m_CapturePatchMax = i_Param;
        }
    }
}
