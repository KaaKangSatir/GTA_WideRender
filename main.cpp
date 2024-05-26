#include <mod/amlmod.h>
#include "isautils.h"
#define sizeofA(__aVar)  ((int)(sizeof(__aVar)/sizeof(__aVar[0])))

MYMOD(ThisModBy.KaaKangSatir.widerender, Wide Render, 1.0, KaaKangSatir)
BEGIN_DEPLIST()
    ADD_DEPENDENCY_VER(Mod.KaaKangSatir.aml, 1.2.1)
END_DEPLIST()

void *hGTASA;
ISAUtils* sautils;

const float AspectRatios[sizeofA(AspectRatioNames)] =
{
    0,
    3.0f / 2.0f,
    4.0f / 3.0f,
    5.0f / 4.0f,
    16.0f / 9.0f,
    16.0f / 10.0f,
    21.0f / 9.0f,
    32.0f / 9.0f,
};
int currentAspect = 0;
const int maxEls = sizeofA(AspectRatioNames)-1;

int bCorrectAspect = 1;

DECL_HOOKv(CameraSize, void* camera, void* rect, float unk, float aspect)
{
    float newaspect = AspectRatios[currentAspect];
    {
        CameraSize(camera, rect, unk, aspect);
    }
    else
    {
        CameraSize(camera, rect, unk, bCorrectAspect!=1 ? aspect / newaspect : newaspect);
    }
}

extern "C" void OnModLoad()
{
    hGTASA = aml->GetLibHandle("libGTASA.so");

    HOOKSYM(CameraSize, hGTASA, "_Z10CameraSizeP8RwCameraP6RwRectff");
}
