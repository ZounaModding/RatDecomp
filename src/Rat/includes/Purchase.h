#ifndef _PURCHASE_H_
#define _PURCHASE_H_
#include "Name_Z.h"
#include "Types_Z.h"

#define SHOP_GAME_MODE_COUNT 4
#define SHOP_DREAM_LEVEL_COUNT 10
#define SHOP_COOKING_MINIGAME_COUNT 8
#define SHOP_SLIDE_LEVEL_COUNT 4
#define SHOP_CHAMPIONSHIP_COUNT 4
#define SHOP_MULTI_LEVEL_COUNT 4
#define SHOP_BONUS_IMAGE_COUNT 11
#define SHOP_BONUS_SKETCH_COUNT 25
#define SHOP_CHEAT_COUNT 24
#define SHOP_BONUS_VIDEO_COUNT 5

class styleBitmap {
    U8 m_MenuStyle[0x94];
};

class Purchase {
public:
    virtual void Reset();

private:
    Bool m_Owned;
    S32 m_Id;
    S32 m_Price;
    S32 m_ImageId;
    S32 m_TextId;
    Name_Z m_Name;
    S32 m_Type;
    styleBitmap m_Style;
    U8 m_Pad_0xb0[8];
};

class MultiPlayerPurchase : public Purchase {
    Name_Z m_OtherName;
};

class SologamePurchase : public Purchase {
    S32 m_MissionId;
};

class MediaPurchase : public Purchase {
    styleBitmap m_OtherStyle;
};

class GamemodePurchase : public Purchase {
    Purchase* m_RequiredPurchase;
};

class DreamPurchase : public SologamePurchase { };

class CookingsessionPurchase : public SologamePurchase { };

class SlidePurchase : public SologamePurchase { };

class ChampionshipPurchase : public MultiPlayerPurchase { };

class MultigamePurchase : public MultiPlayerPurchase { };

class ImagePurchase : public MediaPurchase { };

class RoughPurchase : public MediaPurchase { };

class CheatPurchase : public Purchase {
    Bool m_Enabled;
};

class VideoPurchase {
    Bool m_Purchased;
    U8 m_Data[0x1bb];
};

struct Shop {
    S32 m_Money;
    Bool m_DoUpdate;
    GamemodePurchase m_GameModes[SHOP_GAME_MODE_COUNT];
    DreamPurchase m_DreamLevels[SHOP_DREAM_LEVEL_COUNT];
    CookingsessionPurchase m_CookingMinigames[SHOP_COOKING_MINIGAME_COUNT];
    SlidePurchase m_SlideLevels[SHOP_SLIDE_LEVEL_COUNT];
    ChampionshipPurchase m_Championships[SHOP_CHAMPIONSHIP_COUNT];
    MultigamePurchase m_MultiLevels[SHOP_MULTI_LEVEL_COUNT];
    ImagePurchase m_BonusImages[SHOP_BONUS_IMAGE_COUNT];
    RoughPurchase m_BonusSketches[SHOP_BONUS_SKETCH_COUNT];
    CheatPurchase m_Cheats[SHOP_CHEAT_COUNT];
    VideoPurchase m_BonusVideos[SHOP_BONUS_VIDEO_COUNT];
    Bool m_Initialized;
};

#endif // _PURCHASE_H_
