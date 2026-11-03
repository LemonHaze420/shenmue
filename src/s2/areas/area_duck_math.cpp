#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_duck.hpp"

float DuckLerp(float target, float current, float rate)
{
    return (target - current) * rate + current;
}

short DuckBlendAngle(int target, int current, float rate)
{
    return (short)(int)((short)(target - current) * rate + current);
}

unsigned short DuckYawBetween(const Vector3 *from, const Vector3 *to)
{
    float x, z;
    x = from->x - to->x;
    z = from->z - to->z;
    return FTBL.InverseFixedAtan2(x, z);
}
#else
#endif
