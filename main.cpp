#include <mod/amlmod.h>
#include "isautils.h"
#define sizeofA(__aVar)  ((int)(sizeof(__aVar)/sizeof(__aVar[0])))

MYMOD(ThisModBy.KaaKangSatir.widerender, Wide Render, 1.0, KaaKangSatir)
BEGIN_DEPLIST()
    ADD_DEPENDENCY_VER(Mod.KaaKangSatir.aml, 1.2.1)
END_DEPLIST()

void *hGTASA;
ISAUtils* sautils;

const char* yesnoSwitch[2] = 
{
    "FEM_NO",
    "FEM_YES"
};
const char* AspectRatioNames[] =
{
    "Auto (default)",
    "3:2",
    "4:3",
    "5:4",
    "16:9",
    "16:10",
    "21:9",
    "32:9",
};
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
    if(newaspect == 0)
    {
        CameraSize(camera, rect, unk, aspect);
    }
    else
    {
        CameraSize(camera, rect, unk, bCorrectAspect!=0 ? aspect / newaspect : newaspect);
    }
}

void OnARChange(int oldVal, int newVal, void* data)
{
    clampint(0, maxEls, &newVal);
    currentAspect = newVal;
    aml->MLSSetInt("ASPCTVL", newVal);
}
void OnARCorrectionChange(int oldVal, int newVal, void* data)
{
    clampint(0, 1, &newVal);
    bCorrectAspect = newVal;
    aml->MLSSetInt("ASPCTCR", newVal);
}
extern "C" void OnModLoad()
{
    hGTASA = aml->GetLibHandle("libGTASA.so");
    sautils = (ISAUtils*)GetInterface("SAUtils");
    if(!hGTASA || !sautils) return;

    aml->MLSGetInt("ASPCTVL", &currentAspect); clampint(0, maxEls, &currentAspect);
    aml->MLSGetInt("ASPCTCR", &bCorrectAspect); clampint(0, 1, &bCorrectAspect);

    HOOKSYM(CameraSize, hGTASA, "_Z10CameraSizeP8RwCameraP6RwRectff");
}
