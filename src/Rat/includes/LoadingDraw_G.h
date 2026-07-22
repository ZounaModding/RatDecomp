#ifndef _LOADINGDRAW_G_H_
#define _LOADINGDRAW_G_H_
#include "Manipulator_Z.h"
#include "Material_ZHdl.h"

#define LOADING_HAT_BITMAP_SIZE 32
#define LOADING_HAT_BITMAP_PIXEL_COUNT (LOADING_HAT_BITMAP_SIZE * LOADING_HAT_BITMAP_SIZE)
#define LOADING_HAT_SPIN_DURATION 0.7f
#define LOADING_DRAW_FADE_MAX_DELTA 0.016f
#define LOADING_HAT_VISIBLE_DURATION 0.2f
#define LOADING_SCREEN_FADE_IN_RATE 2.0f
#define LOADING_SCREEN_FADE_OUT_DURATION 0.25f
#define LOADING_HAT_MAX_HORIZONTAL_OFFSET 15.0f
#define LOADING_HAT_SCREEN_Y_RATIO 0.925f
#define LOADING_HAT_LEFT_X 32.0f
#define LOADING_HAT_RIGHT_X 64.0f
#define LOADING_HAT_DRAW_SIZE 32.0f
#define LOADING_HAT_SPIN_HALF 0.5f
#define LOADING_DRAW_BRIGHT_INTENSITY 0.5f
#define LOADING_DRAW_DIM_INTENSITY 0.15f
#define LOADING_DRAW_Z_OFFSET 0.05f
#define LOADING_HAT_TEXT_BUFFER_SIZE 68
#define LOADING_HAT_TEXT_ID 300
#define LOADING_HAT_TEXT_X 72.0f
#define LOADING_SCREEN_TEXT_Y_RATIO 0.06f
#define LOADING_SCREEN_TEXT_BORDER_OFFSET 2.0f

class LoadingDraw_C : public ManipulatorDraw_Z {
public:
    LoadingDraw_C()
        : m_LoadingHatSpinTimer(0.0f)
        , m_LoadingHatSpinCount(0) { }

    virtual ~LoadingDraw_C() { }

    virtual void Init();
    virtual Bool MarkHandles();
    virtual void Update(Float i_DeltaTime);
    virtual void Draw(const DrawInfo_Z& i_DrawInfo);

    static BaseObject_Z* NewObject() { return NewL_Z(40) LoadingDraw_C; }

    void StartDrawLoadingBitmap();
    void EndDrawLoadingBitmap();
    void StopAnimLoading(Bool i_Stop);
    Bool DrawLoadingBitmap(const Viewport_Z* i_Viewport, Float i_Alpha);

private:
    Float m_LoadingHatVisibleTimer;
    Float m_LoadingHatSpinTimer;
    U32 m_LoadingHatSpinCount;
    Bool m_IsStopped;
    Material_ZHdl m_LoadingHatMaterial;
    Bool m_IsStarted;
    Bool m_LevelLoadingScreenEnabled;
    Float m_LevelLoadingScreenFadeInTimer;
    Float m_LevelLoadingScreenFadeOutTimer;
};
#endif // _LOADINGDRAW_G_H_
