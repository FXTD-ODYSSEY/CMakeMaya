#include "logic.h"

MVector lerp(MVector &v1, float t, MVector &v2)
{
    MVector result = ((1.0f - t) * v1) + (t * v2);
    return result;
}

Shared
{
    DLLExport MVector getValue(MVector &v, float factor)
    {
        MVector result;

        result.x = v.x * 6;
        result.y = v.y * 4;
        result.z = v.z * 15;

        result = lerp(v, factor, result);

        return result;
    }
}
