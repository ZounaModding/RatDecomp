#ifndef _DCRENDERERENUM_Z_H_
#define _DCRENDERERENUM_Z_H_

enum DrawList_Z {
    dl_opaque = 0,
    dl_punchthrough,
    dl_transparent,
    dl_count
};

// $SABE: Taken from MonopolyX360SUB.xdb and adapted, could be wrong (For more values see the file)
enum DrawingOrder_Z {
    do_null = 0,
    do_none = 1,
    do_opaque = 2,
    do_unk_3 = 3,
    do_unk_4 = 4,
    do_unk_5 = 5,
    do_unk_6 = 6,
    do_unk_7 = 7,
    do_unk_8 = 8,
    do_shadow_cast = 9,
    do_scene_draw = 10,
    do_transp_first = 11,
    do_transp = 12,
    do_unk_13 = 13,
    do_unk_14 = 14,
    do_flare = 15,
    do_postproc = 16,
    do_screen_fx = 17,
    do_global_screen_fx = 18,
    do_last = 19,
    do_count = 20
};

// $SABE: Taken from MonopolyX360SUB.xdb and adapted, could be wrong (For more values see the file)
enum DrawingState_Z {
    ds_ztest = 0x1,
    ds_zwrite = 0x2,
    ds_cwritergb = 0x4,
    ds_cwritea = 0x8,
    ds_ablend = 0x10,
    ds_noatest = 0x20,
    ds_aref128 = 0x40,
    ds_cw = 0x80,
    ds_ccw = 0x100,
    ds_unk_0x200 = 0x200,
    ds_unk_0x400 = 0x400,
    ds_disable_forced_dstalpha = 0x800,
    ds_ztestwrite = ds_ztest | ds_zwrite,                                   // 0x3
    ds_cwrite = ds_cwritergb | ds_cwritea,                                  // 0xc
    ds_alpha = ds_ablend | ds_noatest | ds_aref128,                         // 0x70
    ds_zonly = ds_ztest | ds_zwrite | ds_cw,                                // 0x83
    ds_opaque_no_alpha_write = ds_ztest | ds_zwrite | ds_cwritergb | ds_cw, // 0x87
    ds_opaque = ds_ztest | ds_zwrite | ds_cwrite | ds_cw,                   // 0x8f
    ds_cull_order = ds_cw | ds_ccw,                                         // 0x180
};

#endif
