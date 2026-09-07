#ifndef _MENUPARSER_H_
#define _MENUPARSER_H_
#include "Types_Z.h"

#define MENU_PLATFORM_PC (S32)(1 << 0)
#define MENU_PLATFORM_PS2 (S32)(1 << 1)
#define MENU_PLATFORM_GC (S32)(1 << 2)
#define MENU_PLATFORM_UNK_0X8 (S32)(1 << 3)
#define MENU_PLATFORM_WII (S32)(1 << 4)
#define MENU_PLATFORM_MAC (S32)(1 << 5)
#define MENU_PLATFORM_XBOX (S32)(1 << 6)
#define MENU_PLATFORM_INTERFACE_DEMO_PS2 (S32)(1 << 7)
#define MENU_PLATFORM_ALL (MENU_PLATFORM_PC | MENU_PLATFORM_PS2 | MENU_PLATFORM_GC | MENU_PLATFORM_UNK_0X8 | MENU_PLATFORM_WII | MENU_PLATFORM_MAC | MENU_PLATFORM_XBOX | MENU_PLATFORM_INTERFACE_DEMO_PS2)

// Commands
Bool StartMENUDefinition();
Bool MENUSTyleTextStructDim();
Bool MENUButtonNotAvailable();
Bool MENUButtonFullScreen();
Bool MENUFrame();
Bool EndMENUFrame();
Bool MENUButtonStableY();
Bool MENUGhostButton();
Bool MENUButtonSelectableWithMouse();
Bool MENUDialog();
Bool MENUButton();
Bool EndMENUDialog();
Bool MENUButtonDesc();
Bool MENUButtonPictDesc();
Bool MENUButtonText();
Bool MENUStyleTextScroll();
Bool MENUButtonAlignVert();
Bool MENUButtonBox();
Bool MENUStyleBoxCoinScale();
Bool MENUButtonAnimateCyclicTRUE();
Bool MENUButtonBitmap();
Bool MENUButtonSelectable();
Bool MENUButtonHidden();
Bool MENUButtonBlink();
Bool MENUStyleText();
Bool MENUStyleBox();
Bool MENUStyleBitmapColor();
Bool MENUStyleBitmap();
Bool MENUStyleBitmapDim();
Bool MENUPlatform();
Bool MENUUpdate();
Bool MENUDEBug();
Bool MENUSTyleTextStruct();
Bool MENUStyleTextEqual();
Bool MENUSurroundingBitMaps();
Bool MENUButtonSurroundingBitmaps();
Bool MenuButtonMAJ();
Bool MENUStateAnimation();
Bool MENUStyleCyclicAnimation();
Bool MENUBoxAutoShrink();
Bool EndMENURessourceParsing();

#endif // _MENUPARSER_H_
