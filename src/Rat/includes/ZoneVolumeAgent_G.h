#ifndef _ZONEVOLUMEAGENT_G_H_
#define _ZONEVOLUMEAGENT_G_H_
#include "VolumeAgent_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(WakeUpDynVolAgent_G, VolumeAgent_Z, 9)
public:
    WakeUpDynVolAgent_G();

    virtual ~WakeUpDynVolAgent_G() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct WakeUpDynVolAgent_G` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(CameraFxVolumeAgent_G, VolumeAgent_Z, 22)
public:
    CameraFxVolumeAgent_G();

    virtual ~CameraFxVolumeAgent_G() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct CameraFxVolumeAgent_G` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(FallVolumeAgent_G, VolumeAgent_Z, 35)
public:
    FallVolumeAgent_G();

    virtual ~FallVolumeAgent_G() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct FallVolumeAgent_G` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(SASVolumeAgent_G, VolumeAgent_Z, 48)
public:
    SASVolumeAgent_G();

    virtual ~SASVolumeAgent_G() {}
    virtual void Init();

    DECL_BHV(CheckSwitch);

private:
    // TODO: fields - see decomp_dump/types.h `struct SASVolumeAgent_G` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(RaftMoveJumpAgent_G, VolumeAgent_Z, 74)
public:
    RaftMoveJumpAgent_G();

    virtual ~RaftMoveJumpAgent_G() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct RaftMoveJumpAgent_G` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(RaftImmobileAgent_G, VolumeAgent_Z, 90)
public:
    RaftImmobileAgent_G();

    virtual ~RaftImmobileAgent_G() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct RaftImmobileAgent_G` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(RaftTourbillonAgent_G, VolumeAgent_Z, 106)
public:
    RaftTourbillonAgent_G();

    virtual ~RaftTourbillonAgent_G() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct RaftTourbillonAgent_G` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(RaftHideAgent_G, VolumeAgent_Z, 122)
public:
    RaftHideAgent_G();

    virtual ~RaftHideAgent_G() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct RaftHideAgent_G` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(PlatformCameraAgent_G, VolumeAgent_Z, 138)
public:
    PlatformCameraAgent_G();

    virtual ~PlatformCameraAgent_G() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct PlatformCameraAgent_G` for Ghidra's inferred layout
END_AGENT_CLASS

BEGIN_AGENT_CLASS(BehindCameraAgent_G, VolumeAgent_Z, 154)
public:
    BehindCameraAgent_G();

    virtual ~BehindCameraAgent_G() {}
    virtual void Init();

    // TODO: remaining public members - see decomp_dump/Rat/*.cpp.ghidra.c

private:
    // TODO: fields - see decomp_dump/types.h `struct BehindCameraAgent_G` for Ghidra's inferred layout
END_AGENT_CLASS

// clang-format on
#endif // _ZONEVOLUMEAGENT_G_H_
