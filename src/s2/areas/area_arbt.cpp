#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_arbt.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void ArbtEntry(void *argument)
{
    StartArbt(argument);
}
#else
const shen_name arbtCharacters[] = {ARBT_RYO, ARBT_PARTNER, ARBT_FOREMAN, ARBT_KNT, ARBT_AA1, ARBT_LYO, ARBT_HAK, ARBT_EIN, EVENT_CHARACTER_INVALID};
const ArbtRouteStep arbtCarryRoutes[3][3][6][16] = {
    {
        {
            {{2, 3, 45.0f, 0x8000}, {2, 2, 45.0f, 0x8000}, {2, 2, 45.0f, 0x4000}, {1, 1, 40.0f, 0x4000}, {2, 2, 45.0f, 0x8000}, {8, 3, 0.0f, 0x0}},
            {{2, 4, 45.0f, 0x8000}, {1, 2, 40.0f, 0x8000}, {1, 2, 40.0f, 0x8000}, {2, 2, 45.0f, 0x4000}, {2, 0, 35.0f, 0x4000}, {8, 3, 0.0f, 0x0}},
            {{1, 3, 40.0f, 0x4000}, {1, 1, 40.0f, 0x8000}, {2, 1, 45.0f, 0x4000}, {1, 0, 35.0f, 0x4000}, {2, 2, 45.0f, 0x4000}, {1, 1, 40.0f, 0x8000},
             {3, 1, 45.0f, 0x4000}, {8, 3, 0.0f, 0x0}},
            {{2, 3, 45.0f, 0x4000}, {1, 0, 35.0f, 0x4000}, {1, 1, 40.0f, 0x8000}, {1, 0, 30.0f, 0x4000}, {2, 1, 45.0f, 0x4000}, {1, 0, 35.0f, 0x4000},
             {3, 1, 45.0f, 0x8000}, {1, 1, 40.0f, 0x4000}, {1, 0, 35.0f, 0x8000}, {8, 3, 0.0f, 0x0}},
            {{1, 3, 40.0f, 0x4000}, {4, 1, 45.0f, 0x4000}, {1, 0, 35.0f, 0x8000}, {2, 1, 45.0f, 0x4000}, {1, 0, 35.0f, 0x4000}, {1, 0, 30.0f, 0x4000},
             {1, 1, 40.0f, 0x8000}, {3, 0, 40.0f, 0x8000}, {4, 1, 45.0f, 0x4000}, {8, 3, 0.0f, 0x0}},
            {{4, 3, 45.0f, 0x4000}, {2, 1, 45.0f, 0x8000}, {3, 0, 45.0f, 0x4000}, {1, 0, 40.0f, 0x4000}, {5, 1, 45.0f, 0x4000}, {3, 0, 40.0f, 0x8000},
             {4, 1, 45.0f, 0x4000}, {4, 0, 35.0f, 0x8000}, {5, 1, 45.0f, 0x4000}, {8, 3, 0.0f, 0x0}}
        },
        {
            {{2, 3, 45.0f, 0x8000}, {2, 2, 45.0f, 0x4000}, {2, 1, 45.0f, 0x8000}, {1, 2, 40.0f, 0x8000}, {2, 2, 45.0f, 0x4000}, {8, 3, 0.0f, 0x0}},
            {{2, 4, 45.0f, 0x4000}, {1, 2, 40.0f, 0x8000}, {1, 0, 40.0f, 0x8000}, {2, 2, 45.0f, 0x4000}, {2, 2, 35.0f, 0x8000}, {8, 3, 0.0f, 0x0}},
            {{1, 3, 40.0f, 0x4000}, {1, 1, 40.0f, 0x8000}, {2, 1, 45.0f, 0x4000}, {1, 0, 35.0f, 0x4000}, {2, 1, 45.0f, 0x8000}, {1, 2, 40.0f, 0x4000},
             {3, 1, 45.0f, 0x4000}, {8, 3, 0.0f, 0x0}},
            {{2, 3, 45.0f, 0x8000}, {1, 0, 35.0f, 0x4000}, {1, 1, 40.0f, 0x4000}, {1, 0, 30.0f, 0x8000}, {2, 1, 45.0f, 0x4000}, {1, 0, 35.0f, 0x4000},
             {3, 1, 45.0f, 0x8000}, {1, 0, 40.0f, 0x4000}, {1, 1, 35.0f, 0x4000}, {8, 3, 0.0f, 0x0}},
            {{1, 3, 40.0f, 0x4000}, {4, 1, 45.0f, 0x4000}, {1, 0, 35.0f, 0x4000}, {2, 1, 45.0f, 0x8000}, {1, 0, 35.0f, 0x4000}, {1, 0, 30.0f, 0x8000},
             {1, 1, 40.0f, 0x4000}, {3, 0, 40.0f, 0x8000}, {4, 1, 45.0f, 0x4000}, {8, 3, 0.0f, 0x0}},
            {{4, 3, 45.0f, 0x4000}, {2, 1, 45.0f, 0x4000}, {3, 1, 45.0f, 0x8000}, {1, 0, 40.0f, 0x4000}, {5, 1, 45.0f, 0x4000}, {3, 0, 40.0f, 0x8000},
             {4, 1, 45.0f, 0x8000}, {4, 0, 35.0f, 0x4000}, {5, 0, 45.0f, 0x4000}, {8, 3, 0.0f, 0x0}}
        },
        {
            {{2, 3, 45.0f, 0x4000}, {2, 2, 45.0f, 0x4000}, {2, 1, 45.0f, 0x8000}, {1, 2, 40.0f, 0x8000}, {2, 2, 45.0f, 0x8000}, {8, 3, 0.0f, 0x0}},
            {{2, 3, 45.0f, 0x4000}, {1, 2, 40.0f, 0x8000}, {1, 0, 40.0f, 0x8000}, {2, 3, 45.0f, 0x8000}, {2, 2, 35.0f, 0x4000}, {8, 3, 0.0f, 0x0}},
            {{1, 4, 40.0f, 0x8000}, {1, 1, 40.0f, 0x4000}, {2, 1, 45.0f, 0x4000}, {1, 0, 35.0f, 0x4000}, {2, 1, 45.0f, 0x8000}, {1, 1, 40.0f, 0x4000},
             {3, 1, 45.0f, 0x4000}, {8, 3, 0.0f, 0x0}},
            {{2, 3, 45.0f, 0x8000}, {1, 0, 35.0f, 0x8000}, {1, 1, 40.0f, 0x4000}, {1, 0, 30.0f, 0x4000}, {2, 1, 45.0f, 0x4000}, {1, 0, 35.0f, 0x8000},
             {3, 1, 45.0f, 0x4000}, {1, 0, 40.0f, 0x4000}, {1, 1, 35.0f, 0x4000}, {8, 3, 0.0f, 0x0}},
            {{1, 3, 40.0f, 0x4000}, {4, 1, 45.0f, 0x4000}, {1, 0, 35.0f, 0x8000}, {2, 1, 45.0f, 0x8000}, {1, 0, 35.0f, 0x4000}, {1, 0, 30.0f, 0x8000},
             {1, 1, 40.0f, 0x4000}, {3, 0, 40.0f, 0x4000}, {4, 1, 45.0f, 0x4000}, {8, 3, 0.0f, 0x0}},
            {{4, 3, 45.0f, 0x8000}, {2, 1, 45.0f, 0x4000}, {3, 1, 45.0f, 0x8000}, {1, 0, 40.0f, 0x4000}, {5, 0, 45.0f, 0x4000}, {3, 0, 40.0f, 0x8000},
             {4, 1, 45.0f, 0x4000}, {4, 0, 35.0f, 0x4000}, {5, 1, 45.0f, 0x4000}, {8, 3, 0.0f, 0x0}}
        }
    },
    {
        {
            {{2, 3, 45.0f, 0x8000}, {2, 1, 45.0f, 0x8000}, {2, 2, 45.0f, 0x4000}, {1, 1, 40.0f, 0x4000}, {2, 0, 45.0f, 0x4000}, {1, 1, 40.0f, 0x8000},
             {2, 1, 45.0f, 0x8000}, {8, 3, 0.0f, 0x0}},
            {{2, 3, 45.0f, 0x8000}, {1, 1, 40.0f, 0x8000}, {1, 2, 40.0f, 0x8000}, {2, 0, 45.0f, 0x4000}, {1, 2, 40.0f, 0x8000}, {2, 1, 45.0f, 0x4000},
             {2, 0, 35.0f, 0x4000}, {8, 3, 0.0f, 0x0}},
            {{1, 3, 40.0f, 0x4000}, {1, 1, 40.0f, 0x8000}, {2, 0, 45.0f, 0x4000}, {1, 0, 35.0f, 0x4000}, {2, 0, 45.0f, 0x4000}, {1, 1, 40.0f, 0x8000},
             {2, 1, 45.0f, 0x4000}, {1, 0, 40.0f, 0x8000}, {3, 1, 45.0f, 0x4000}, {8, 3, 0.0f, 0x0}},
            {{2, 3, 45.0f, 0x4000}, {1, 0, 35.0f, 0x4000}, {1, 1, 40.0f, 0x8000}, {1, 0, 30.0f, 0x4000}, {3, 0, 45.0f, 0x8000}, {1, 0, 40.0f, 0x4000},
             {2, 0, 45.0f, 0x4000}, {1, 0, 35.0f, 0x4000}, {3, 1, 45.0f, 0x8000}, {1, 0, 40.0f, 0x4000}, {1, 0, 35.0f, 0x8000}, {8, 3, 0.0f, 0x0}},
            {{1, 3, 40.0f, 0x4000}, {4, 0, 45.0f, 0x4000}, {1, 0, 35.0f, 0x8000}, {2, 0, 45.0f, 0x4000}, {1, 0, 40.0f, 0x4000}, {3, 0, 40.0f, 0x8000},
             {1, 0, 35.0f, 0x4000}, {1, 1, 30.0f, 0x4000}, {1, 1, 40.0f, 0x8000}, {3, 0, 40.0f, 0x8000}, {4, 0, 45.0f, 0x4000}, {8, 3, 0.0f, 0x0}},
            {{4, 3, 45.0f, 0x4000}, {2, 0, 45.0f, 0x8000}, {4, 0, 45.0f, 0x4000}, {4, 0, 35.0f, 0x8000}, {3, 1, 45.0f, 0x4000}, {1, 0, 40.0f, 0x4000},
             {5, 0, 45.0f, 0x4000}, {3, 0, 40.0f, 0x8000}, {4, 0, 45.0f, 0x4000}, {4, 0, 35.0f, 0x8000}, {5, 1, 45.0f, 0x4000}, {8, 3, 0.0f, 0x0}}
        },
        {
            {{2, 3, 45.0f, 0x4000}, {2, 1, 45.0f, 0x8000}, {2, 1, 45.0f, 0x4000}, {1, 2, 40.0f, 0x4000}, {2, 0, 45.0f, 0x8000}, {1, 1, 40.0f, 0x8000},
             {2, 1, 45.0f, 0x8000}, {8, 3, 0.0f, 0x0}},
            {{2, 3, 45.0f, 0x8000}, {1, 1, 40.0f, 0x8000}, {1, 2, 40.0f, 0x4000}, {2, 0, 45.0f, 0x8000}, {1, 1, 40.0f, 0x8000}, {2, 2, 45.0f, 0x4000},
             {2, 0, 35.0f, 0x4000}, {8, 3, 0.0f, 0x0}},
            {{1, 3, 40.0f, 0x4000}, {1, 1, 40.0f, 0x4000}, {2, 1, 45.0f, 0x8000}, {1, 0, 35.0f, 0x4000}, {2, 0, 45.0f, 0x4000}, {1, 1, 40.0f, 0x4000},
             {2, 0, 45.0f, 0x8000}, {1, 0, 40.0f, 0x8000}, {3, 1, 45.0f, 0x4000}, {8, 3, 0.0f, 0x0}},
            {{2, 3, 45.0f, 0x4000}, {1, 0, 35.0f, 0x8000}, {1, 0, 40.0f, 0x4000}, {1, 0, 30.0f, 0x4000}, {3, 0, 45.0f, 0x8000}, {1, 1, 40.0f, 0x4000},
             {2, 0, 45.0f, 0x4000}, {1, 0, 35.0f, 0x8000}, {3, 1, 45.0f, 0x4000}, {1, 0, 40.0f, 0x4000}, {1, 0, 35.0f, 0x8000}, {8, 3, 0.0f, 0x0}},
            {{1, 3, 40.0f, 0x8000}, {4, 0, 45.0f, 0x4000}, {1, 0, 35.0f, 0x4000}, {2, 1, 45.0f, 0x4000}, {1, 1, 40.0f, 0x4000}, {3, 0, 40.0f, 0x8000},
             {1, 0, 35.0f, 0x8000}, {1, 0, 30.0f, 0x4000}, {1, 0, 40.0f, 0x8000}, {3, 0, 40.0f, 0x4000}, {4, 0, 45.0f, 0x4000}, {8, 3, 0.0f, 0x0}},
            {{4, 3, 45.0f, 0x4000}, {2, 0, 45.0f, 0x4000}, {4, 0, 45.0f, 0x8000}, {4, 0, 35.0f, 0x8000}, {3, 0, 45.0f, 0x4000}, {1, 1, 40.0f, 0x4000},
             {5, 0, 45.0f, 0x8000}, {3, 0, 40.0f, 0x4000}, {4, 1, 45.0f, 0x4000}, {4, 0, 35.0f, 0x8000}, {5, 0, 45.0f, 0x4000}, {8, 3, 0.0f, 0x0}}
        },
        {
            {{2, 3, 45.0f, 0x8000}, {2, 1, 45.0f, 0x8000}, {2, 1, 45.0f, 0x4000}, {1, 1, 40.0f, 0x4000}, {2, 0, 45.0f, 0x8000}, {1, 2, 40.0f, 0x8000},
             {2, 1, 45.0f, 0x4000}, {8, 3, 0.0f, 0x0}},
            {{2, 3, 45.0f, 0x8000}, {1, 1, 40.0f, 0x8000}, {1, 2, 40.0f, 0x4000}, {2, 0, 45.0f, 0x4000}, {1, 1, 40.0f, 0x8000}, {2, 2, 45.0f, 0x4000},
             {2, 0, 35.0f, 0x8000}, {8, 3, 0.0f, 0x0}},
            {{1, 3, 40.0f, 0x4000}, {1, 1, 40.0f, 0x4000}, {2, 1, 45.0f, 0x4000}, {1, 0, 35.0f, 0x4000}, {2, 0, 45.0f, 0x8000}, {1, 1, 40.0f, 0x4000},
             {2, 0, 45.0f, 0x8000}, {1, 0, 40.0f, 0x8000}, {3, 1, 45.0f, 0x4000}, {8, 3, 0.0f, 0x0}},
            {{2, 3, 45.0f, 0x4000}, {1, 0, 35.0f, 0x8000}, {1, 1, 40.0f, 0x4000}, {1, 0, 30.0f, 0x4000}, {3, 0, 45.0f, 0x4000}, {1, 1, 40.0f, 0x4000},
             {2, 0, 45.0f, 0x8000}, {1, 0, 35.0f, 0x8000}, {3, 0, 45.0f, 0x8000}, {1, 0, 40.0f, 0x4000}, {1, 0, 35.0f, 0x4000}, {8, 3, 0.0f, 0x0}},
            {{1, 3, 40.0f, 0x8000}, {4, 0, 45.0f, 0x8000}, {1, 0, 35.0f, 0x4000}, {2, 1, 45.0f, 0x4000}, {1, 0, 40.0f, 0x4000}, {3, 0, 40.0f, 0x8000},
             {1, 0, 35.0f, 0x8000}, {1, 0, 30.0f, 0x4000}, {1, 1, 40.0f, 0x4000}, {3, 0, 40.0f, 0x4000}, {4, 0, 45.0f, 0x4000}, {8, 3, 0.0f, 0x0}},
            {{4, 3, 45.0f, 0x4000}, {2, 1, 45.0f, 0x8000}, {4, 0, 45.0f, 0x4000}, {4, 0, 35.0f, 0x8000}, {3, 1, 45.0f, 0x4000}, {1, 0, 40.0f, 0x4000},
             {5, 0, 45.0f, 0x8000}, {3, 0, 40.0f, 0x4000}, {4, 0, 45.0f, 0x4000}, {4, 0, 35.0f, 0x4000}, {5, 0, 45.0f, 0x8000}, {8, 3, 0.0f, 0x0}}
        }
    },
    {
        {
            {{2, 3, 45.0f, 0x8000}, {2, 0, 45.0f, 0x8000}, {2, 1, 45.0f, 0x4000}, {1, 0, 40.0f, 0x4000}, {3, 0, 45.0f, 0x4000}, {1, 1, 40.0f, 0x8000},
             {2, 0, 45.0f, 0x8000}, {1, 1, 40.0f, 0x8000}, {3, 1, 45.0f, 0x4000}, {8, 3, 0.0f, 0x0}},
            {{2, 3, 45.0f, 0x8000}, {1, 1, 40.0f, 0x8000}, {3, 1, 40.0f, 0x4000}, {2, 0, 45.0f, 0x4000}, {1, 1, 40.0f, 0x8000}, {3, 1, 45.0f, 0x4000},
             {1, 0, 40.0f, 0x8000}, {4, 0, 45.0f, 0x4000}, {2, 0, 35.0f, 0x8000}, {8, 3, 0.0f, 0x0}},
            {{3, 3, 40.0f, 0x4000}, {1, 1, 40.0f, 0x4000}, {4, 1, 45.0f, 0x4000}, {1, 0, 35.0f, 0x4000}, {3, 0, 45.0f, 0x8000}, {4, 1, 40.0f, 0x4000},
             {2, 0, 45.0f, 0x8000}, {1, 0, 40.0f, 0x8000}, {3, 1, 45.0f, 0x4000}, {8, 3, 0.0f, 0x0}},
            {{2, 3, 45.0f, 0x4000}, {3, 0, 35.0f, 0x8000}, {4, 0, 40.0f, 0x4000}, {1, 0, 30.0f, 0x4000}, {3, 0, 45.0f, 0x4000}, {3, 1, 40.0f, 0x4000},
             {2, 0, 45.0f, 0x8000}, {4, 0, 35.0f, 0x8000}, {3, 1, 45.0f, 0x8000}, {1, 0, 40.0f, 0x4000}, {5, 0, 40.0f, 0x4000}, {8, 3, 0.0f, 0x0}},
            {{4, 3, 40.0f, 0x8000}, {4, 0, 45.0f, 0x8000}, {1, 0, 35.0f, 0x4000}, {3, 1, 45.0f, 0x4000}, {4, 0, 40.0f, 0x4000}, {3, 0, 40.0f, 0x8000},
             {1, 0, 35.0f, 0x8000}, {5, 0, 40.0f, 0x4000}, {5, 1, 40.0f, 0x4000}, {3, 0, 40.0f, 0x4000}, {4, 0, 45.0f, 0x4000}, {8, 3, 0.0f, 0x0}},
            {{4, 3, 45.0f, 0x4000}, {5, 1, 45.0f, 0x8000}, {4, 0, 45.0f, 0x4000}, {4, 0, 35.0f, 0x8000}, {5, 0, 45.0f, 0x4000}, {1, 0, 40.0f, 0x4000},
             {5, 0, 45.0f, 0x8000}, {3, 0, 40.0f, 0x4000}, {4, 1, 45.0f, 0x4000}, {4, 0, 35.0f, 0x4000}, {5, 0, 45.0f, 0x8000}, {8, 3, 0.0f, 0x0}}
        },
        {
            {{2, 3, 45.0f, 0x4000}, {2, 0, 45.0f, 0x8000}, {2, 1, 45.0f, 0x8000}, {1, 1, 40.0f, 0x4000}, {3, 0, 45.0f, 0x4000}, {1, 1, 40.0f, 0x8000},
             {2, 0, 45.0f, 0x8000}, {1, 0, 40.0f, 0x4000}, {3, 1, 45.0f, 0x8000}, {8, 3, 0.0f, 0x0}},
            {{2, 3, 45.0f, 0x4000}, {1, 1, 40.0f, 0x8000}, {3, 1, 40.0f, 0x8000}, {2, 0, 45.0f, 0x4000}, {1, 1, 40.0f, 0x8000}, {3, 1, 45.0f, 0x4000},
             {1, 0, 40.0f, 0x4000}, {4, 0, 45.0f, 0x8000}, {2, 0, 35.0f, 0x8000}, {8, 3, 0.0f, 0x0}},
            {{3, 3, 40.0f, 0x8000}, {1, 1, 40.0f, 0x4000}, {4, 1, 45.0f, 0x4000}, {1, 0, 35.0f, 0x4000}, {3, 0, 45.0f, 0x8000}, {4, 0, 40.0f, 0x4000},
             {2, 1, 45.0f, 0x4000}, {1, 0, 40.0f, 0x8000}, {3, 1, 45.0f, 0x4000}, {8, 3, 0.0f, 0x0}},
            {{2, 3, 45.0f, 0x4000}, {3, 0, 35.0f, 0x8000}, {4, 1, 40.0f, 0x4000}, {1, 0, 30.0f, 0x4000}, {3, 0, 45.0f, 0x8000}, {3, 0, 40.0f, 0x4000},
             {2, 0, 45.0f, 0x4000}, {4, 0, 35.0f, 0x8000}, {3, 1, 45.0f, 0x4000}, {1, 0, 40.0f, 0x4000}, {5, 0, 40.0f, 0x8000}, {8, 3, 0.0f, 0x0}},
            {{4, 3, 40.0f, 0x8000}, {4, 0, 45.0f, 0x4000}, {1, 0, 35.0f, 0x8000}, {3, 1, 45.0f, 0x4000}, {4, 0, 40.0f, 0x4000}, {3, 0, 40.0f, 0x8000},
             {1, 0, 35.0f, 0x4000}, {5, 0, 40.0f, 0x4000}, {5, 1, 40.0f, 0x8000}, {3, 0, 40.0f, 0x4000}, {4, 0, 45.0f, 0x4000}, {8, 3, 0.0f, 0x0}},
            {{4, 3, 45.0f, 0x8000}, {5, 0, 45.0f, 0x4000}, {4, 0, 45.0f, 0x4000}, {4, 0, 35.0f, 0x8000}, {5, 1, 45.0f, 0x4000}, {1, 0, 40.0f, 0x8000},
             {5, 0, 45.0f, 0x4000}, {3, 0, 40.0f, 0x4000}, {4, 1, 45.0f, 0x4000}, {4, 0, 35.0f, 0x8000}, {5, 0, 45.0f, 0x4000}, {8, 3, 0.0f, 0x0}}
        },
        {
            {{2, 3, 45.0f, 0x8000}, {2, 1, 45.0f, 0x8000}, {2, 1, 45.0f, 0x4000}, {1, 0, 40.0f, 0x8000}, {3, 0, 45.0f, 0x8000}, {1, 1, 40.0f, 0x8000},
             {2, 0, 45.0f, 0x4000}, {1, 0, 40.0f, 0x4000}, {3, 1, 45.0f, 0x4000}, {8, 3, 0.0f, 0x0}},
            {{2, 3, 45.0f, 0x4000}, {1, 1, 40.0f, 0x8000}, {3, 1, 40.0f, 0x8000}, {2, 0, 45.0f, 0x4000}, {1, 1, 40.0f, 0x8000}, {3, 0, 45.0f, 0x4000},
             {1, 0, 40.0f, 0x4000}, {4, 1, 45.0f, 0x8000}, {2, 0, 35.0f, 0x8000}, {8, 3, 0.0f, 0x0}},
            {{3, 3, 40.0f, 0x4000}, {1, 1, 40.0f, 0x8000}, {4, 1, 45.0f, 0x4000}, {1, 0, 35.0f, 0x4000}, {3, 0, 45.0f, 0x8000}, {4, 1, 40.0f, 0x4000},
             {2, 1, 45.0f, 0x8000}, {1, 0, 40.0f, 0x4000}, {3, 0, 45.0f, 0x4000}, {8, 3, 0.0f, 0x0}},
            {{2, 3, 45.0f, 0x4000}, {3, 0, 35.0f, 0x8000}, {4, 0, 40.0f, 0x4000}, {1, 0, 30.0f, 0x8000}, {3, 0, 45.0f, 0x4000}, {3, 1, 40.0f, 0x4000},
             {2, 0, 45.0f, 0x8000}, {4, 0, 35.0f, 0x4000}, {3, 1, 45.0f, 0x8000}, {1, 0, 40.0f, 0x4000}, {5, 0, 40.0f, 0x4000}, {8, 3, 0.0f, 0x0}},
            {{4, 3, 40.0f, 0x4000}, {4, 0, 45.0f, 0x8000}, {1, 0, 35.0f, 0x4000}, {3, 1, 45.0f, 0x4000}, {4, 1, 40.0f, 0x4000}, {3, 0, 40.0f, 0x4000},
             {1, 0, 35.0f, 0x8000}, {5, 0, 40.0f, 0x8000}, {5, 0, 40.0f, 0x4000}, {3, 0, 40.0f, 0x4000}, {4, 0, 45.0f, 0x8000}, {8, 3, 0.0f, 0x0}},
            {{4, 3, 45.0f, 0x4000}, {5, 0, 45.0f, 0x8000}, {4, 0, 45.0f, 0x4000}, {4, 0, 35.0f, 0x8000}, {5, 1, 45.0f, 0x4000}, {1, 0, 40.0f, 0x4000},
             {5, 0, 45.0f, 0x4000}, {3, 0, 40.0f, 0x8000}, {4, 1, 45.0f, 0x8000}, {4, 0, 35.0f, 0x4000}, {5, 0, 45.0f, 0x4000}, {8, 3, 0.0f, 0x0}}
        }
    }
};
const float arbtCarrySpeeds[6] = {2.3f, 2.3f, 2.3f, 2.4f, 2.4f, 2.4f};
const ArbtDescriptor arbtDescriptors[9] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {2, 0}};
int arbtAuthorFlag = 0;
Vector3 arbtCameraOffset = {0.0f, 0.0f, 0.0f};
Vector3 arbtPayAttachPosition = {0.197f, 0.075f, -0.018f};
Vector3i arbtCameraRotation = {0, 0, 0};
Vector3i arbtPayAttachRotation = {48423, 42780, 52792};
Vector3i arbtCrateRotation = {0, 0, 0};
Vector3 arbtCrateScale = {1.0f, 1.0f, 0.6f};
Vector3 arbtLowPalletScale = {1.0f, 1.75f, 1.0f};
Vector3 arbtHighPalletScale = {1.0f, 1.75f, 1.2f};
Vector3i arbtGuideRotation = {0, 0xc000, 0};
Vector3i arbtDestinationRotation = {0, 0, 0};
Vector3 arbtGuideOffset = {-0.9f, 0.0f, -0.08f};
Vector3 arbtDestinationOffset = {0.0f, 0.0f, 0.0f};
Vector3 arbtGuideEdge0Right = {0.66f, 0.0305f, -0.272f};
Vector3 arbtGuideEdge0Left = {0.68f, 0.1f, 0.32f};
Vector3 arbtGuideEdge1Right = {-0.66f, 0.0305f, 0.272f};
Vector3 arbtGuideEdge1Left = {-0.66f, 0.0305f, -0.272f};
Vector3i arbtMoveRotation = {0, 0xc000, 0};

void StartArbt(const ArbtDescriptor *descriptor)
{
    HLibTask *task, *helper;
    ArbtWork *work;
    task = FTBL.HLib_EnqueueTaskWithParameter(ArbtReadyInit, 4, 11, sizeof(ArbtWork), TASK_EVNT);
    work = (ArbtWork *)FTBL.HLib_GetTaskParameter(task);
    helper = FTBL.HLib_EnqueueTaskWithParameter(ArbtHelperNullsub, 3, 11, 4, TASK_EVNT);
    *(ArbtWork **)FTBL.HLib_GetTaskParameter(helper) = work;
    work->helper = helper;
    work->self = task;
    if (descriptor)
        work->descriptor = descriptor;
    else {
        work->descriptor = arbtDescriptors;
        if ((unsigned int)(g_entryPoint - 90) <= 8)
            work->descriptor += g_entryPoint - 90;
    }
    ArbtMain(work);
    EventDebugRegister(ARBT_DEBUG_ID, ArbtDebugExit, work, 3000000);
}

void ArbtDebugExit(void *data)
{
    int stat = FTBL.GetPlayerStat(2);
    if (stat <= 9)
        FTBL.SetPlayerStat(2, stat + 20);
    ArbtLeave((ArbtWork *)data);
}

void ArbtLeave(ArbtWork *work)
{
    ArbtCleanup(work);
    FTBL.EV_HLKillOtherHierarchyTask(work->helper);
    FTBL.EV_HLKillOtherHierarchyTask(work->self);
    EventDebugUnregister(ARBT_DEBUG_ID);
    FTBL.EV_SetPlayerFlag(12, 966, 1);
    if (FTBL.EV_GetShenmueFlag(1077)) {
        FTBL.EV_stepSetFlags(1077, 0);
        FTBL.EV_scenQueueLoad(g_actNumber, ARBT_AK00, 0);
    } else if (work->descriptor->mode == 1) {
        FTBL.EV_SetPlayerFlag(12, 1871, 1);
        FTBL.EV_scenQueueLoad(g_actNumber, ARBT_Q100, 97);
    } else
        FTBL.EV_scenQueueLoad(g_actNumber, ARBT_AK00, 92);
}

void ArbtHelperNullsub(void *data)
{
}

void ArbtReadyInit(void *data)
{
    ArbtWork *work = (ArbtWork *)data;
    int frames;
    EventDebugCheck(ARBT_DEBUG_ID, 4);
    FTBL.CheckHelpIcon(1, 0, 0);
    FTBL.FinishLoading();
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(ARBT_RYO));
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(ARBT_RYO), 1, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(ARBT_RYO), 1, 1, 0);
    Queue_EVT_AT_CtrlHandUp_1(ARBT_RYO, 0, (void *)areaLoopHandPoses[ARBT_GRIP_HAND_POSE], 0, 0);
    Queue_EVT_AT_CtrlHandUp_1(ARBT_RYO, 1, (void *)areaLoopHandPoses[ARBT_GRIP_HAND_POSE], 0, 0);
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(ARBT_PARTNER));
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(ARBT_PARTNER), 1, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(ARBT_PARTNER), 1, 1, 0);
    Queue_EVT_AT_CtrlHandUp_1(ARBT_PARTNER, 0, (void *)areaLoopHandPoses[ARBT_GRIP_HAND_POSE], 0, 0);
    Queue_EVT_AT_CtrlHandUp_1(ARBT_PARTNER, 1, (void *)areaLoopHandPoses[ARBT_GRIP_HAND_POSE], 0, 0);
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(ARBT_RYO), 1);
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(ARBT_PARTNER), 1);
    FTBL.SetMotionBoneTargetStepLimit(FTBL.scenFindMotionwork(ARBT_RYO), 10, 2.0f);
    FTBL.SetMotionBoneTargetStepLimit(FTBL.scenFindMotionwork(ARBT_RYO), 17, 1.0f);
    FTBL.SetMotionBoneTargetStepLimit(FTBL.scenFindMotionwork(ARBT_PARTNER), 10, 2.0f);
    FTBL.SetMotionBoneTargetStepLimit(FTBL.scenFindMotionwork(ARBT_PARTNER), 17, 2.0f);
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(ARBT_RYO), 55, 0);
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(ARBT_PARTNER), 55, 0);
    FTBL.AT_SetLocalBaseChara(ARBT_RYO, ARBT_BASE);
    FTBL.AT_SetLocalBaseChara(ARBT_PARTNER, ARBT_BASE);
    FTBL.AT_SetLocalBaseChara(ARBT_FOREMAN, ARBT_BASE);
    FTBL.AT_SetLocalBaseChara(ARBT_DUMA, ARBT_RYO);
    FTBL.AT_SetLocalBaseChara(ARBT_DUMT, ARBT_PARTNER);
    FTBL.AT_SetLocalBaseChara(ARBT_DUMS, ARBT_FOREMAN);
    FTBL.EV_AT_SetPos(ARBT_DUMA, ARBT_POSITION_FLAGS, &arbtCameraOffset);
    FTBL.EV_AT_SetPos(ARBT_DUMT, ARBT_POSITION_FLAGS, &arbtCameraOffset);
    FTBL.EV_AT_SetPos(ARBT_DUMS, ARBT_POSITION_FLAGS, &arbtCameraOffset);
    FTBL.EV_AT_SetAngY_0(ARBT_DUMA, ARBT_POSITION_FLAGS, &arbtCameraRotation, 0);
    FTBL.EV_AT_SetAngY_0(ARBT_DUMT, ARBT_POSITION_FLAGS, &arbtCameraRotation, 0);
    FTBL.EV_AT_SetAngY_0(ARBT_DUMS, ARBT_POSITION_FLAGS, &arbtCameraRotation, 0);
    EVT_AT_SetPos(ARBT_PARTNER, ARBT_BASE, -1.5f, 0.0f, -1.5f, 0);
    EVT_AT_SetAng(ARBT_PARTNER, ARBT_BASE, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetPos(ARBT_FOREMAN, ARBT_BASE, -1.0f, 0.0f, -6.5f, 0);
    EVT_AT_SetAng(ARBT_FOREMAN, ARBT_BASE, 0.0f, 160.0f, 0.0f, 0);
    EVT_AT_SetPos(ARBT_RYO, ARBT_FOREMAN, 0.0f, 0.0f, -1.5f, 0);
    EVT_AT_SetAng(ARBT_RYO, ARBT_FOREMAN, 0.0f, 180.0f, 0.0f, 0);
    EVT_AT_CharaDisp(ARBT_KNT, 0, 1);
    EVT_AT_CharaDisp(ARBT_AA1, 0, 1);
    EVT_AT_CharaDisp(ARBT_LYO, 0, 1);
    EVT_AT_CharaDisp(ARBT_HAK, 0, 1);
    EVT_AT_CharaDisp(ARBT_EIN, 0, 1);
    FTBL.EV_ToggleCharacterFlag(ARBT_KNT);
    FTBL.EV_ToggleCharacterFlag(ARBT_AA1);
    FTBL.EV_ToggleCharacterFlag(ARBT_LYO);
    FTBL.EV_ToggleCharacterFlag(ARBT_HAK);
    FTBL.EV_ToggleCharacterFlag(ARBT_EIN);
    FTBL.AttachScenarioCharacter(ARBT_PAY, ARBT_FOREMAN, 14, &arbtPayAttachPosition, &arbtPayAttachRotation);
    EVT_AT_CharaDisp(ARBT_PAY, 0, 1);
    FTBL.BeginSecondarySampleExecution(0);
    FTBL.EV_LoadAudio("0018E000", 0);
    EVT_AT_SetSample(0, 0, 0);
    if (work->firstJob)
        FTBL.HLib_SetCurrentTaskTo(ArbtCarryInit);
    else {
        frames = ArbtIntroduceJob();
        EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 0);
        EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, frames);
        FTBL.HLib_SetCurrentTaskTo(ArbtCarryInit);
        FTBL.HLib_SetupTaskDelay(frames + 30);
    }
}

void SetInitialArbtCarrySettings(ArbtCarryWork *carry, int crate)
{
    int random;
    carry->phase = ArbtCarryTurnInit;
    carry->forwardSteps = 0;
    carry->turns = 2;
    carry->failures = 0;
    carry->consecutiveFailures = 0;
    carry->idleCount = 0;
    carry->speed = arbtCarrySpeeds[crate];
    random = FTBL.shenGetRandomInt32(3);
    if (g_actNumber == 1)
        carry->route = arbtCarryRoutes[0][random][crate];
    else if (g_actNumber == 2)
        carry->route = arbtCarryRoutes[1][random][crate];
    else
        carry->route = arbtCarryRoutes[2][random][crate];
    SetArbtQte(&carry->qte, 0, 0, 0);
}

const signed char arbtPressCounts[8] = {1, 1, 1, 2, 3, 4, 1, 1};

void SetArbtQte(ArbtQte *qte, int frames, unsigned int direction, int intensity)
{
    qte->success = 0;
    qte->frames = frames;
    qte->direction = direction;
    qte->intensity = intensity;
    qte->presses = arbtPressCounts[intensity];
}

void ArbtCarryInit(void *data)
{
    ArbtWork *work = (ArbtWork *)data;
    ArbtCarryWork *carry = &work->carry;
    int frames;
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.EV_DisableCharacterDisplay(ARBT_FOREMAN);
    SetInitialArbtCarrySettings(carry, work->delivered);
    if (!work->delivered) {
        if (work->firstJob)
            frames = ArbtExplainFirstCarry();
        else
            frames = ArbtExplainCarry();
    } else
        frames = ArbtIntroduceNextCrate();
    FTBL.HLib_SetCurrentTaskTo(ArbtCarryLift);
    FTBL.HLib_SetupTaskDelay(frames);
}

void ArbtCarryLift(void *data)
{
    ArbtWork *work = (ArbtWork *)data;
    ArbtCarryWork *carry = &work->carry;
    ArbtQte *qte = &carry->qte;
    ArbtCrateWork *crates = &work->crates;
    Vector3 position;
    FTBL.SetRenderClockAndCompassTo(0);
    FTBL.GetCharaPosition(crates->crates[crates->index], -1, &position, 0);
    if (position.y > 0.5f)
        qte->frames = ArbtLiftHighCrate(crates);
    else
        qte->frames = ArbtLiftLowCrate(crates);
    FTBL.HLib_SetCurrentTaskTo(carry->phase);
}

void ArbtCarryTurnInit(void *data)
{
    ArbtWork *work = (ArbtWork *)data;
    ArbtCarryWork *carry = &work->carry;
    ArbtQte *qte = &carry->qte;
    int turns, delay;
    FTBL.CheckHelpIcon(0xd020, 0, 1);
    turns = carry->turns--;
    if (turns != 0) {
        delay = qte->frames - 40;
        if (delay < 0)
            delay = 0;
        if (work->delivered > 1)
            SetArbtQte(qte, 40, 0x8000, 0);
        else
            SetArbtQte(qte, 40, 0x4000, 0);
        ArbtQueueDirection(qte->direction, qte->intensity, delay);
        FTBL.HLib_SetCurrentTaskTo(ArbtCarryQte);
        if (delay > 0)
            FTBL.HLib_SetupTaskDelay(delay);
    } else {
        carry->phase = ArbtCarryRoute;
        FTBL.HLib_SetCurrentTaskTo(carry->phase);
        if (!work->delivered) {
            delay = ArbtStartWork(qte->frames);
            qte->frames = 0;
            FTBL.HLib_SetupTaskDelay(delay);
        }
    }
}

void ArbtCarryFinishTurns(void *data)
{
    ArbtWork *work = (ArbtWork *)data;
    ArbtCarryWork *carry = &work->carry;
    ArbtQte *qte = &carry->qte;
    int turns, delay;
    turns = carry->turns--;
    if (turns > 0) {
        FTBL.CheckHelpIcon(0xd020, 0, 1);
        delay = qte->frames - 40;
        if (delay < 0)
            delay = 0;
        SetArbtQte(qte, 40, 0x8000, 0);
        ArbtQueueDirection(qte->direction, qte->intensity, delay);
        FTBL.HLib_SetCurrentTaskTo(ArbtCarryQte);
        if (delay > 0)
            FTBL.HLib_SetupTaskDelay(delay);
    } else if (carry->turns < -1) {
        FTBL.GetCurrentDateTime((DateTime *)&work->time);
        if (work->time.hour >= work->endTime.hour) {
            EVT_AT_MenCtrl(60, 0, 0, 0, 0, 255, 0, 0, 0, 0);
            FTBL.HLib_SetCurrentTaskTo(ArbtCarryAddTime);
        } else {
            FTBL.CheckHelpIcon(0xd020, 0xd020, 2);
            FTBL.CheckHelpIcon(0x2020, 0, 1);
            SetArbtQte(qte, 50, 0x2000, 7);
            ArbtQueueDirection(qte->direction, qte->intensity, 0);
            FTBL.HLib_SetCurrentTaskTo(ArbtCarryPutQte);
        }
    } else {
        delay = ArbtRequestPutDown(qte->frames);
        FTBL.HLib_SetupTaskDelay(delay);
    }
}

void ArbtCarryRoute(void *data)
{
    ArbtWork *work = (ArbtWork *)data;
    ArbtCarryWork *carry = &work->carry;
    ArbtQte *qte = &carry->qte;
    const ArbtRouteStep *step = carry->route;
    int delay;
    FTBL.CheckHelpIcon(0xd020, 0, 1);
    if (carry->forwardSteps >= step->forwardSteps) {
        carry->forwardSteps = 0;
        carry->route++;
        if (step->intensity == 8) {
            carry->turns = 2;
            carry->phase = ArbtCarryFinishTurns;
            FTBL.HLib_SetCurrentTaskTo(carry->phase);
        } else {
            delay = (float)qte->frames - step->frames;
            if (delay < 0)
                delay = 0;
            SetArbtQte(qte, step->frames, step->direction, step->intensity);
            ArbtQueueDirection(qte->direction, qte->intensity, delay);
            FTBL.HLib_SetCurrentTaskTo(ArbtCarryQte);
            if (delay > 0)
                FTBL.HLib_SetupTaskDelay(delay);
        }
    } else {
        FTBL.GetCurrentDateTime((DateTime *)&work->time);
        if (work->time.hour >= work->endTime.hour) {
            EVT_AT_MenCtrl(60, 0, 0, 0, 0, 255, 0, 0, 0, qte->frames);
            FTBL.HLib_SetCurrentTaskTo(ArbtCarryAddTime);
            if (qte->frames > 0)
                FTBL.HLib_SetupTaskDelay(qte->frames);
        } else {
            delay = qte->frames - 30;
            if (delay < 0)
                delay = 0;
            SetArbtQte(qte, 30, 0x1000, 6);
            ArbtQueueDirection(qte->direction, qte->intensity, delay);
            FTBL.HLib_SetCurrentTaskTo(ArbtCarryForwardQte);
            if (delay > 0)
                FTBL.HLib_SetupTaskDelay(delay);
        }
    }
}

void ArbtCarryQte(void *data)
{
    ArbtWork *work = (ArbtWork *)data;
    ArbtCarryWork *carry = &work->carry;
    ArbtCrateWork *crates = &work->crates;
    ArbtQte *qte = &carry->qte;
    shen_name *crate = &crates->crates[crates->index];
    unsigned int wrong = qte->direction ^ 0xd000;
    unsigned int pressed;
    int frames;
    if (--qte->frames > 0) {
        pressed = FTBL.GetCurrentControlInput(1);
        if (pressed == qte->direction || ((pressed & qte->direction) && qte->direction == 0x1000)) {
            if (--qte->presses == 0) {
                EVT_AT_SetSound(0x900ab, 0);
                qte->success = 1;
            } else if (qte->presses > 0)
                EVT_AT_SetSound(0x900ab, 0);
            else {
                EVT_AT_SetSound(0xa00ab, 0);
                FTBL.HLib_SetupTaskDelay(qte->frames);
                qte->success = 0;
                qte->frames = 0;
            }
        } else if ((pressed & wrong) && qte->frames < 20 && !qte->success) {
            EVT_AT_SetSound(0xa00ab, 0);
            FTBL.HLib_SetupTaskDelay(qte->frames);
            qte->frames = 0;
        }
    } else if (qte->success) {
        carry->consecutiveFailures = 0;
        qte->frames = ArbtWalkDirection(carry->speed, qte->intensity, qte->direction, 0);
        FTBL.HLib_SetCurrentTaskTo(carry->phase);
    } else {
        carry->failures++;
        carry->consecutiveFailures++;
        frames = ArbtFailCarry(crates, crate, qte->intensity, qte->direction);
        FTBL.HLib_SetCurrentTaskTo(ArbtCarryFalse);
        FTBL.HLib_SetupTaskDelay(frames);
    }
}

void ArbtCarryForwardQte(void *data)
{
    ArbtWork *work = (ArbtWork *)data;
    ArbtCarryWork *carry = &work->carry;
    ArbtQte *qte = &carry->qte;
    unsigned int wrong = qte->direction ^ 0xd000;
    unsigned int held, pressed;
    if (--qte->frames > 0) {
        held = FTBL.GetCurrentControlInput(0);
        pressed = FTBL.GetCurrentControlInput(1);
        if (held & qte->direction) {
            FTBL.HLib_SetCurrentTaskTo(work->carry.phase);
            if (carry->idleCount == 0 && qte->frames > 0)
                FTBL.HLib_SetupTaskDelay(qte->frames);
            else
                qte->frames = 0;
            carry->idleCount = 0;
            carry->forwardSteps++;
            qte->frames = ArbtWalkForward(carry->speed, qte->frames);
        } else if (pressed & wrong) {
            FTBL.HLib_SetCurrentTaskTo(ArbtCarryQte);
            if (carry->idleCount == 0 && qte->frames > 0)
                FTBL.HLib_SetupTaskDelay(qte->frames);
            qte->frames = 0;
            carry->idleCount = 0;
            qte->direction = 0x2000;
            ArbtWarnWrongDirection(pressed, 0);
        }
    } else {
        FTBL.HLib_SetCurrentTaskTo(carry->phase);
        if (++carry->idleCount > 3) {
            carry->idleCount = 0;
            FTBL.HLib_SetupTaskDelay(ArbtWarnIdleForward(++work->idleWarnings));
        }
    }
}

void ArbtCarryPutQte(void *data)
{
    ArbtWork *work = (ArbtWork *)data;
    ArbtCarryWork *carry = &work->carry;
    ArbtQte *qte = &carry->qte;
    if (--qte->frames > 0) {
        if (FTBL.GetCurrentControlInput(0) & qte->direction) {
            EVT_AT_SetSound(0x900ab, 0);
            FTBL.HLib_SetCurrentTaskTo(ArbtCarryPut);
            carry->idleCount = 0;
            qte->frames = 0;
        }
    } else {
        FTBL.HLib_SetCurrentTaskTo(carry->phase);
        if (++carry->idleCount > 3) {
            carry->idleCount = 0;
            FTBL.HLib_SetupTaskDelay(ArbtWarnIdlePut(++work->idleWarnings));
        }
    }
}

void ArbtCarryFalse(void *data)
{
    ArbtWork *work = (ArbtWork *)data;
    ArbtCarryWork *carry = &work->carry;
    ArbtQte *qte = &carry->qte;
    int frames;
    switch (qte->intensity) {
    case 6:
        frames = ArbtWarnWrongForward();
        break;
    case 3:
        frames = ArbtWarnWrongDouble(qte->direction);
        break;
    case 4:
    case 5:
        frames = ArbtWarnWrongMultiple(qte->direction);
        break;
    default:
        switch (carry->failures >> 1) {
        case 0:
            frames = ArbtWarnCarryOnce();
            break;
        case 1:
            frames = ArbtWarnCarryTwice();
            break;
        case 2:
            frames = ArbtWarnCarryThird();
            break;
        case 3:
            frames = ArbtWarnCarryFourth();
            break;
        default:
            frames = ArbtWarnCarryRepeatedly();
            break;
        }
        break;
    }
    FTBL.HLib_SetCurrentTaskTo(ArbtCarryLift);
    FTBL.HLib_SetupTaskDelay(frames);
}

void ArbtCarryPut(void *data)
{
    ArbtWork *work = (ArbtWork *)data;
    ArbtCarryWork *carry = &work->carry;
    ArbtCrateWork *crates = &work->crates;
    shen_name crate = crates->crates[crates->index], destination = crates->destinations[crates->index];
    int frames;
    FTBL.CheckHelpIcon(0x2020, 0x2020, 2);
    if (carry->failures)
        work->perfectStreak = 0;
    else
        work->perfectStreak++;
    if (crates->stacked[crates->index]) {
        EVT_AT_SetPosAng(crate, destination, -0.05f, 0.1f, 0.0f, 0);
        frames = ArbtPutHighCrate(crates, work->perfectStreak, destination);
    } else {
        EVT_AT_SetPosAng(crate, destination, 0.0f, 0.2f, 0.0f, 0);
        frames = ArbtPutLowCrate(crates, work->perfectStreak, destination);
    }
    FTBL.HLib_SetCurrentTaskTo(ArbtCarryEnd);
    FTBL.HLib_SetupTaskDelay(frames);
}

void ArbtCarryEnd(void *data)
{
    ArbtWork *work = (ArbtWork *)data;
    ArbtCarryWork *carry = &work->carry;
    ArbtCrateWork *crates = &work->crates;
    int limit = crates->limit - (carry->failures >> 2);
    if (limit < work->delivered)
        limit = work->delivered;
    crates->limit = limit;
    work->failures += carry->failures;
    work->delivered++;
    if (SelectNextArbtCrate(crates)) {
        EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, 0);
        FTBL.HLib_SetCurrentTaskTo(ArbtCarryInit);
        FTBL.HLib_SetupTaskDelay(30);
    } else {
        EVT_AT_MenCtrl(60, 0, 0, 0, 0, 255, 0, 0, 0, 0);
        FTBL.HLib_SetCurrentTaskTo(ArbtCarryAddTime);
    }
}

void ArbtCarryAddTime(void *data)
{
    ArbtWork *work = (ArbtWork *)data;
    FTBL.CheckHelpIcon(0, 0, 0);
    FTBL.SetRenderClockAndCompassTo(1);
    FTBL.GetCurrentDateTime((DateTime *)&work->time);
    if (work->time.hour < work->endTime.hour + 1) {
        if (++work->time.minute > 59) {
            work->time.minute = 0;
            work->time.hour++;
        }
        FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&work->time);
    } else if (!FTBL.filmCheckFadeTrackColour())
        FTBL.HLib_SetCurrentTaskTo(ArbtCarryTimeOver);
}

void ArbtCarryTimeOver(void *data)
{
    int frames;
    FTBL.EV_EnableCharacterDisplay(ARBT_FOREMAN);
    frames = ArbtAnnounceTimeOver();
    EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 0);
    EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, frames);
    FTBL.HLib_SetCurrentTaskTo(ArbtPayInit);
    FTBL.HLib_SetupTaskDelay(frames + 30);
}

void ArbtPayInit(void *data)
{
    ArbtWork *work = (ArbtWork *)data;
    if (work->descriptor->mode == 1) {
        EVT_AT_SetPos(ARBT_BASE, EVENT_CHARACTER_INVALID, 142.9f, 100.0f, 138.9f, 0);
        EVT_AT_SetAngY(ARBT_BASE, EVENT_CHARACTER_INVALID, 270.0f, 0);
    } else {
        EVT_AT_SetPos(ARBT_BASE, EVENT_CHARACTER_INVALID, 225.2f, 100.0f, 247.5f, 0);
        EVT_AT_SetAngY(ARBT_BASE, EVENT_CHARACTER_INVALID, 270.0f, 0);
        FTBL.SetMapModelDataDefaultVertexShader();
        FTBL.FreeWAIR();
        FTBL.UpdateWeatherForMinute(0, 0, 0, 0.0f);
        FTBL.UpdateWeatherForMinute(1, 0, 0, 0.0f);
        FTBL.UpdateWeather();
        FTBL.EV_SetWeatherInstruction(12, 0);
        FTBL.EV_SetWeatherInstruction(1920, 0);
        FTBL.EV_lightLoad(ARBT_LIGHT_AKB0);
        FTBL.PushGlobalLightingProgram();
        HideArbtDockProps();
    }
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(ARBT_RYO), 1);
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(ARBT_PARTNER), 1);
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(ARBT_KNT), 1);
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(ARBT_AA1), 1);
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(ARBT_LYO), 1);
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(ARBT_HAK), 1);
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(ARBT_EIN), 1);
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(ARBT_RYO), 55, 2);
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(ARBT_PARTNER), 55, 2);
    EVT_AT_CharaDisp(ARBT_KNT, 0, 0);
    EVT_AT_CharaDisp(ARBT_AA1, 0, 0);
    EVT_AT_CharaDisp(ARBT_LYO, 0, 0);
    EVT_AT_CharaDisp(ARBT_HAK, 0, 0);
    EVT_AT_CharaDisp(ARBT_EIN, 0, 0);
    FTBL.EV_EnsureCharacterDraw(ARBT_KNT);
    FTBL.EV_EnsureCharacterDraw(ARBT_AA1);
    FTBL.EV_EnsureCharacterDraw(ARBT_LYO);
    FTBL.EV_EnsureCharacterDraw(ARBT_HAK);
    FTBL.EV_EnsureCharacterDraw(ARBT_EIN);
    StopArbtMoveBox(&work->crates, 0);
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (work->delivered <= 5)
        work->delivered++;
    FTBL.HLib_SetCurrentTaskTo(ArbtPayAuto);
}

void ArbtPayAuto(void *data)
{
    ArbtWork *work = (ArbtWork *)data;
    int cash = work->delivered * 10;
    int frames;
    if (work->firstJob)
        frames = ArbtPayFirstJob(cash, work->delivered);
    else if (work->delivered > 4)
        frames = ArbtPayGoodJob(cash, work->delivered);
    else if (work->delivered > 2)
        frames = ArbtPayAverageJob(cash, work->delivered);
    else
        frames = ArbtPayPoorJob(cash, work->delivered);
    EVT_AT_SetSample(0, 1, frames - 60);
    EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, frames - 30);
    FTBL.HLib_SetCurrentTaskTo(ArbtPayEnd);
    FTBL.HLib_SetupTaskDelay(frames);
}

void ArbtPayEnd(void *data)
{
    ArbtWork *work = (ArbtWork *)data;
    FTBL.ResetScenarioCharacterAttachment(ARBT_PAY);
    if (work->descriptor->mode == 2)
        FTBL.HLib_SetCurrentTaskTo(ArbtReadyInit);
    else
        ArbtLeave(work);
}

void CreateArbtCrates(ArbtCrateWork *const crates)
{
    Vector3 position;
    int i, first, second, unit;
    shen_name name;
    crates->moveTask = 0;
    crates->moveFrames = 0;
    crates->dropTask = 0;
    crates->dropHeight = 0.0f;
    crates->index = -1;
    crates->limit = 6;
    first = 0;
    do {
        second = 0;
    } while (0);
    for (i = 0, name = ARBT_CRATE_FIRST; i < 16; i++, name += 0x1000000) {
        crates->crates[i] = name;
        FTBL.GetRandomNumberInRange(100.0f);
        if (first <= 7) {
            position.x = -6.5f;
            position.y = 0.7f - (first & 1) * 0.4f;
            position.z = (first >> 1) * 2.1f + -1.2f;
            if (first > 1) {
                if ((first & 1) == 0)
                    position.z -= 0.08f;
                else
                    position.z += 0.04f;
            }
            first++;
        } else {
            position.x = -4.5f;
            position.y = 0.7f - (second & 1) * 0.4f;
            position.z = (second >> 1) * 2.1f + -1.2f;
            if (first > 1) {
                if ((first & 1) == 0)
                    position.z -= 0.08f;
                else
                    position.z += 0.04f;
            }
            second++;
        }
        FTBL.AT_SetLocalBaseChara(name, ARBT_BASE);
        FTBL.EV_AT_SetPos(name, ARBT_POSITION_FLAGS, &position);
        FTBL.EV_AT_SetAngY_0(name, ARBT_POSITION_FLAGS, &arbtCrateRotation, 0);
        FTBL.AT_SetLocalBaseChara(name, EVENT_CHARACTER_INVALID);
        FTBL.EV_EnableCharacterDisplay(name);
        FTBL.EV_SetCharaControllerFlags(name, 1, 0);
        FTBL.mtwkSetMotionworkScale3(name, &arbtCrateScale);
        FTBL.SetScenarioCharacterDrawControl(name, 2, 1, 0.0f);
    }
    for (i = 0, name = ARBT_PALLET_FIRST; i < 16; i++, name += 0x1000000) {
        FTBL.CreateEventCharacter(3, "PLT0H01G", name);
        crates->pallets[i] = name;
        if (i < 8) {
            if (i < 4) {
                position.x = -16.8f;
                position.y = 0.05f;
                position.z = i * 2.1f + -1.2f - 0.7f;
                FTBL.EV_EnableCharacterDisplay(name);
            } else {
                position.x = -14.8f;
                position.y = 0.05f;
                position.z = (i - 4) * 2.1f + -1.2f - 0.7f;
                FTBL.EV_DisableCharacterDisplay(name);
            }
            FTBL.mtwkSetMotionworkScale3(name, &arbtLowPalletScale);
        } else {
            if (i < 12) {
                position.x = -6.5f;
                position.y = 0.05f;
                position.z = (i - 8) * 2.1f + -1.2f;
            } else {
                position.x = -4.5f;
                position.y = 0.05f;
                position.z = (i - 12) * 2.1f + -1.2f;
            }
            FTBL.EV_EnableCharacterDisplay(name);
            FTBL.mtwkSetMotionworkScale3(name, &arbtHighPalletScale);
        }
        FTBL.EV_SetCharaControllerFlags(name, 1, 0);
        FTBL.AT_SetLocalBaseChara(name, ARBT_BASE);
        FTBL.EV_AT_SetPos(name, ARBT_POSITION_FLAGS, &position);
        FTBL.EV_AT_SetAngY_0(name, ARBT_POSITION_FLAGS, &arbtCrateRotation, 0);
    }
    first = 0;
    second = 0;
    unit = 0x1000000;
    for (i = 0; i < 16; i++) {
        FTBL.GetRandomNumberInRange(100.0f);
        if (first <= 7) {
            crates->destinations[i] = ARBT_PALLET_FIRST + (first >> 1) * unit;
            crates->stacked[i] = first & 1;
            first++;
        } else {
            crates->destinations[i] = ARBT_PALLET_FIRST + unit * 4 + (second >> 1) * unit;
            crates->stacked[i] = second & 1;
            second++;
        }
    }
    SelectNextArbtCrate(crates);
}

void KillArbtPallets(ArbtCrateWork *crates)
{
    int i;
    for (i = 0; i < 16; i++)
        FTBL.SetScenarioCharacterKillMode(crates->pallets[i], 1);
}

int SelectNextArbtCrate(ArbtCrateWork *crates)
{
    StopArbtMoveBox(crates, 0);
    if (crates->index >= crates->limit)
        return crates->limit - crates->index;
    crates->index++;
    FTBL.AT_SetLocalBaseChara(crates->crates[crates->index], ARBT_BASE);
    FTBL.AT_SetLocalBaseChara(ARBT_DUMB, crates->crates[crates->index]);
    FTBL.AT_SetLocalBaseChara(ARBT_EG0R, crates->crates[crates->index]);
    FTBL.AT_SetLocalBaseChara(ARBT_EG0L, crates->crates[crates->index]);
    FTBL.AT_SetLocalBaseChara(ARBT_EG1R, crates->crates[crates->index]);
    FTBL.AT_SetLocalBaseChara(ARBT_EG1L, crates->crates[crates->index]);
    FTBL.EV_AT_SetPos(ARBT_DUMB, ARBT_POSITION_FLAGS, &arbtGuideOffset);
    FTBL.EV_AT_SetPos(ARBT_EG0R, ARBT_POSITION_FLAGS, &arbtGuideEdge0Right);
    FTBL.EV_AT_SetPos(ARBT_EG0L, ARBT_POSITION_FLAGS, &arbtGuideEdge0Left);
    FTBL.EV_AT_SetPos(ARBT_EG1R, ARBT_POSITION_FLAGS, &arbtGuideEdge1Right);
    FTBL.EV_AT_SetPos(ARBT_EG1L, ARBT_POSITION_FLAGS, &arbtGuideEdge1Left);
    FTBL.EV_AT_SetAngY_0(ARBT_DUMB, ARBT_POSITION_FLAGS, &arbtGuideRotation, 0);
    FTBL.EV_AT_SetAngY_0(ARBT_EG0R, ARBT_POSITION_FLAGS, &arbtGuideRotation, 0);
    FTBL.EV_AT_SetAngY_0(ARBT_EG0L, ARBT_POSITION_FLAGS, &arbtGuideRotation, 0);
    FTBL.EV_AT_SetAngY_0(ARBT_EG1R, ARBT_POSITION_FLAGS, &arbtGuideRotation, 0);
    FTBL.EV_AT_SetAngY_0(ARBT_EG1L, ARBT_POSITION_FLAGS, &arbtGuideRotation, 0);
    FTBL.AT_SetLocalBaseChara(ARBT_DUMN, crates->destinations[crates->index]);
    FTBL.EV_AT_SetPos(ARBT_DUMN, ARBT_POSITION_FLAGS, &arbtDestinationOffset);
    FTBL.EV_AT_SetAngY_0(ARBT_DUMN, ARBT_POSITION_FLAGS, &arbtDestinationRotation, 0);
    return crates->limit - crates->index;
}

void QueueArbtMoveBox(ArbtCrateWork *crates, int delay)
{
    if (crates->moveTask == 0) {
        crates->moveFrames = 100000000;
        crates->moveTask = FTBL.HLib_EnqueueTaskWithParameter(ArbtMoveBox, 3, 11, 4, TASK_EVNT);
        *(ArbtCrateWork **)FTBL.HLib_GetTaskParameter(crates->moveTask) = crates;
        if (delay > 0)
            FTBL.HLib_SetupTimedTask(crates->moveTask, delay);
    }
}

void StopArbtMoveBox(ArbtCrateWork *crates, int frames)
{
    if (crates->moveTask) {
        crates->moveFrames = frames;
        if (!frames) {
            FTBL.HLib_DestroyTaskInternal(crates->moveTask);
            crates->moveTask = 0;
        }
    }
}

void ArbtMoveBox(void *data)
{
    ArbtCrateWork *crates = *(ArbtCrateWork **)data;
    shen_name crate = crates->crates[crates->index];
    shen_name base;
    Vector3 hand, position;
    base = FTBL.SCEN_FindChar_Wrapper(crate);
    FTBL.AT_SetLocalBaseChara(crate, ARBT_RYO);
    FTBL.GetCharaPosition(ARBT_RYO, 17, &hand, 0x1000000);
    position.x = hand.x + 0.32f;
    position.y = hand.y - 0.1f;
    position.z = hand.z - 0.68f;
    FTBL.EV_AT_SetPos(crate, ARBT_POSITION_FLAGS, &position);
    FTBL.EV_AT_SetAngY_0(crate, ARBT_POSITION_FLAGS, &arbtMoveRotation, 0);
    FTBL.AT_SetLocalBaseChara(crate, base);
    if (--crates->moveFrames < 0) {
        FTBL.HLib_DestroyCurrentTask();
        crates->moveTask = 0;
    }
}

void QueueArbtDropBox(ArbtCrateWork *crates, int delay)
{
    shen_name crate = crates->crates[crates->index], destination = crates->destinations[crates->index];
    int stacked = crates->stacked[crates->index];
    Vector3 position, target;
    float dx, dz;
    int frames;
    if (crates->dropTask == 0) {
        FTBL.GetCharaPosition(crate, -1, &position, 0);
        FTBL.GetCharaPosition(destination, -1, &target, 0);
        dx = position.x - target.x;
        dz = position.z - (target.z + 0.7f);
        if (dz < 0.0f)
            dz = -dz;
        crates->dropHeight = 0.0f;
        frames = 10;
        if (dx > 9.0f) {
            if (dz < 0.3f) {
                crates->dropHeight = 0.3f;
                frames = 8;
            }
        } else if (dx < 1.0f) {
            if (dz < 0.3f && stacked) {
                crates->dropHeight = 0.7f;
                frames = 5;
            } else {
                crates->dropHeight = 0.3f;
                frames = 8;
            }
        }
        EVT_AT_SetSound(257, delay + frames);
        crates->dropTask = FTBL.HLib_EnqueueTaskWithParameter(ArbtDropBox, 4, 11, 4, TASK_EVNT);
        *(ArbtCrateWork **)FTBL.HLib_GetTaskParameter(crates->dropTask) = crates;
        if (delay > 0)
            FTBL.HLib_SetupTimedTask(crates->dropTask, delay);
    }
}

void ArbtDropBox(void *data)
{
    ArbtCrateWork *crates = *(ArbtCrateWork **)data;
    shen_name crate = crates->crates[crates->index];
    Vector3 position;
    FTBL.GetCharaPosition(crate, -1, &position, 0);
    if (position.y > 0.0f) {
        position.y -= 0.08f;
        if (crates->dropHeight > position.y) {
            position.y = crates->dropHeight;
            FTBL.HLib_DestroyCurrentTask();
            crates->dropTask = 0;
        }
        FTBL.EV_AT_SetPos(crate, ARBT_POSITION_FLAGS, &position);
    }
}

void ArbtMain(ArbtWork *work)
{
    int i;
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
    work->characters = Queue_EVT_AT_ReadChr(work->self, arbtCharacters);
    work->delivered = 0;
    work->failures = 0;
    work->idleWarnings = 0;
    work->perfectStreak = 0;
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"ARBT", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 4, "ARBT", 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, "ARBT", 0, 0);
    FTBL.EV_SetNewSceneIndex();
    FTBL.EV_LoadCutsceneFromCurrentScene("0018E000");
    if (work->descriptor->mode == 1)
        FTBL.EV_FreeAuthorPak(1, "0360");
    else
        FTBL.EV_FreeAuthorPak(1, "0018");
    FTBL.GetCurrentDateTime((DateTime *)&work->time);
    FTBL.GetCurrentDateTime((DateTime *)&work->endTime);
    if (FTBL.EV_GetShenmueFlag(18)) {
        FTBL.EV_stepSetFlags(18, 0);
        work->firstJob = 1;
        work->time.hour = 10;
    } else {
        work->firstJob = 0;
        if (work->descriptor->shift == 0)
            work->time.hour = 10;
        else if (work->descriptor->shift == 1)
            work->time.hour = 13;
        else if (work->descriptor->shift == 2)
            work->time.hour = 16;
        else
            work->time.hour = 20;
    }
    work->time.minute = 0;
    work->time.second = 0;
    work->endTime.minute = 0;
    work->endTime.second = 0;
    work->endTime.hour = work->time.hour + 1;
    FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&work->time);
    FTBL.shenSetTimeSimulationTo(1);
    FTBL.EV_stepSetFlags(65, 0);
    if (work->descriptor->mode == 1) {
        EVT_AT_SetPos(ARBT_BASE, EVENT_CHARACTER_INVALID, 175.7f, 100.0f, 177.0f, 0);
        EVT_AT_SetAngY(ARBT_BASE, EVENT_CHARACTER_INVALID, 270.0f, 0);
        FTBL.FreeWAIR();
        FTBL.UpdateWeatherForMinute(0, 0, 0, 0.0f);
        FTBL.UpdateWeatherForMinute(1, 0, 0, 0.0f);
        FTBL.UpdateWeather();
        FTBL.EV_SetWeatherInstruction(12, 0);
        FTBL.EV_SetWeatherInstruction(1920, 0);
        FTBL.EV_lightLoad(ARBT_LIGHT_QCAE);
        FTBL.PushGlobalLightingProgram();
        FTBL.SetMapFieldDepthSlotEnabled(0, 2);
        FTBL.SetMapFieldDepthSlotEnabled(0, 8);
        FTBL.SetMapFieldDepthSlotEnabled(0, 9);
        for (i = 0; i < 10; i++)
            FTBL.SetObjectFlagByIndex(i, 0);
    } else if (work->descriptor->shift == 3) {
        EVT_AT_SetPosAng(ARBT_BASE, EVENT_CHARACTER_INVALID, 266.8f, 260.5f, 0.0f, 0);
        FTBL.RefreshWeatherForCurrentTime(0);
        FTBL.UpdateWeather();
        FTBL.EV_SetWeatherInstruction(12, 0);
        FTBL.EV_SetWeatherInstruction(1920, 0);
        FTBL.EV_lightLoad(ARBT_AK00);
        FTBL.PushGlobalLightingProgram();
        FTBL.SetMapFieldDepthSlotEnabled(0, 8);
        FTBL.SetMapFieldDepthSlotEnabled(0, 9);
        FTBL.SetMapFieldDepthSlotEnabled(0, 10);
        FTBL.SetObjectFlagByIndex(5, 0);
        FTBL.SetObjectFlagByIndex(6, 0);
        FTBL.SetObjectFlagByIndex(8, 0);
        FTBL.SetObjectFlagByIndex(10, 0);
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(ARBT_RYO), 1);
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(ARBT_PARTNER), 1);
    } else if (work->descriptor->shift == 1) {
        EVT_AT_SetPosAng(ARBT_BASE, EVENT_CHARACTER_INVALID, 270.0f, 238.7f, 270.0f, 0);
        FTBL.FreeWAIR();
        FTBL.UpdateWeatherForMinute(0, 0, 0, 0.0f);
        FTBL.UpdateWeatherForMinute(1, 0, 0, 0.0f);
        FTBL.UpdateWeather();
        FTBL.EV_SetWeatherInstruction(12, 0);
        FTBL.EV_SetWeatherInstruction(1920, 0);
        FTBL.EV_lightLoad(ARBT_LIGHT_AKA3);
        FTBL.PushGlobalLightingProgram();
        FTBL.SetMapFieldDepthSlotEnabled(0, 2);
        FTBL.SetMapFieldDepthSlotEnabled(0, 9);
        FTBL.SetMapFieldDepthSlotEnabled(0, 10);
        FTBL.SetObjectFlagByIndex(10, 0);
        HideArbtDockProps();
        for (i = 0; i < 8; i++)
            FTBL.SetObjectFlagByIndex(i, 0);
    } else {
        EVT_AT_SetPos(ARBT_BASE, EVENT_CHARACTER_INVALID, 225.2f, 100.0f, 247.5f, 0);
        EVT_AT_SetAngY(ARBT_BASE, EVENT_CHARACTER_INVALID, 270.0f, 0);
        FTBL.RefreshWeatherForCurrentTime(0);
        FTBL.UpdateWeather();
        FTBL.ReinitializeLights(1);
        FTBL.SetLightFlagForIndex(0, 2);
        FTBL.SetLightEntryIndex(0, 1);
        FTBL.SetMapFieldDepthSlotEnabled(0, 8);
        FTBL.SetMapFieldDepthSlotEnabled(0, 9);
        FTBL.SetMapFieldDepthSlotEnabled(0, 10);
        FTBL.SetObjectFlagByIndex(5, 0);
        FTBL.SetObjectFlagByIndex(7, 0);
        FTBL.SetObjectFlagByIndex(8, 0);
        FTBL.SetObjectFlagByIndex(10, 0);
    }
    FTBL.SetRenderClockAndCompassTo(1);
    FTBL.SetLoadingBusy();
    FTBL.EV_EventSetUpHelpIcon_0(70, 0);
    SetInitialArbtCarrySettings(&work->carry, 0);
    CreateArbtCrates(&work->crates);
}

void ArbtCleanup(ArbtWork *work)
{
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(ARBT_RYO), 0);
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(ARBT_PARTNER), 0);
    FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(ARBT_RYO));
    FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(ARBT_PARTNER));
    KillArbtPallets(&work->crates);
    FTBL.EV_EventSetUpHelpIcon_0(0, 0);
    FTBL.CheckHelpIcon(0, 0, 0);
    FTBL.SetLoadingBusy();
    FTBL.SetRenderClockAndCompassTo(0);
    if (work->descriptor->mode == 1)
        FTBL.EV_FreeAuthorPak(0, "0360");
    else
        FTBL.EV_FreeAuthorPak(0, "0018");
    FTBL.LoadCutsceneAndSoundProgramForArea();
    EVT_AT_SetSample(0, 0, 0);
    FTBL.EV_ControlPackageResources(0, 6, "ARBT", 0, 0);
    ET_AT_KillWork(work->characters);
    FTBL.EV_NPC_NewLoadSw_0(1);
    FTBL.SetPlayerControlEnabled(0);
}

void HideArbtDockProps(void)
{
    FTBL.SetDisplayFlagForProp(ARBT_PROP_R024, 0);
    FTBL.SetDisplayFlagForProp(ARBT_PROP_R005, 0);
    FTBL.SetDisplayFlagForProp(ARBT_PROP_R018, 0);
    FTBL.SetDisplayFlagForProp(ARBT_PROP_R007, 0);
    FTBL.SetDisplayFlagForProp(ARBT_PROP_R010, 0);
    FTBL.SetDisplayFlagForProp(ARBT_PROP_R014, 0);
    FTBL.SetDisplayFlagForProp(ARBT_PROP_R013, 0);
    FTBL.SetDisplayFlagForProp(ARBT_PROP_R012, 0);
    FTBL.SetDisplayFlagForProp(ARBT_PROP_R030, 0);
}
#endif
#else
#include "s2/areas/area_arbt.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte_ui.hpp"
#include "s2/character.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/qte_hand_poses.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"
namespace shenmue::s2::areas {
namespace {
void ArbtReadyInit(void *);
void ArbtReadyInitAuto(void *);
void ArbtCarryInit(void *);
void ArbtCarryAutoWait(void *);
void ArbtCarryLift(void *);
void ArbtCarryRoute(void *);
void ArbtCarryFinishTurns(void *);
void ArbtCarryQte(void *);
void ArbtCarryForwardQte(void *);
void ArbtCarryPutQte(void *);
void ArbtCarryFalse(void *);
void ArbtCarryPut(void *);
void ArbtCarryEnd(void *);
void ArbtCarryAddTime(void *);
void ArbtCarryTimeOver(void *);
void ArbtCarryTimeOverAuto(void *);
void ArbtPayInit(void *);
void ArbtPayAuto(void *);
void ArbtPayAutoWait(void *);
void ArbtPayEnd(void *);
void ArbtHelpNullsub() {
}
void HideArbtDockProps() {
    for (shen_name name : ARBT_HIDDEN_PROPS)
        SetDisplayFlagForProp(name, 0);
}
void ArbtCleanup(ArbtWork &work) {
    SetPluralControlFor(ctrlGetCharaByName(ARBT_RYO), 0);
    SetPluralControlFor(ctrlGetCharaByName(ARBT_TKR), 0);
    hndlSetFlags(ctrlGetCharaByName(ARBT_RYO));
    hndlSetFlags(ctrlGetCharaByName(ARBT_TKR));
    for (shen_name name : work.crates.pallets)
        SetCharaKill(name, 1);
    EV_EventSetUpHelpIcon_0(0, 0);
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    ArbtHelpNullsub();
    shenSetClockAndCompassRenderTo(0);
    EV_FreeAuthorPak(0, work.descriptor->mode == 1 ? "0360" : "0018");
    sbtLoadProgram();
    EV_spmBeginSampleExecute();
    EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("ARBT"), 0);
    ET_AT_KillWork(work.characters);
    EV_NPC_NewLoadSw(1);
    SetPlayerControlEnabled(0);
}
void ArbtLeave(ArbtWork &work) {
    ArbtCleanup(work);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.self);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 966, 1);
    if (EV_GetShenmueFlag(0x435)) {
        EV_stepSetFlags(0, 0x435);
        EV_scenQueueLoad(g_actNumber, ARBT_AK00, 0);
    } else if (work.descriptor->mode == 1) {
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 0x74f, 1);
        EV_scenQueueLoad(g_actNumber, ARBT_Q100, 97);
    } else
        EV_scenQueueLoad(g_actNumber, ARBT_AK00, 92);
}
void ArbtReadyInit(void *data) {
    ArbtWork &work = *static_cast<ArbtWork *>(data);
    CheckHelpIcon(1, 0, CheckHelpIconOp::Set);
    FinishLoading();
    for (shen_name name : {ARBT_RYO, ARBT_TKR}) {
        SetLowQualityHands(ctrlGetCharaByName(name));
        SetHighModelXYZ(ctrlGetCharaByName(name), 1, 0, 0);
        SetHighModelXYZ(ctrlGetCharaByName(name), 1, 1, 0);
        ApplyCharacterHandPoseCommand(ctrlGetCharaByName(name), 0, reinterpret_cast<std::byte *>(crateGripHandPose), 0);
        ApplyCharacterHandPoseCommand(ctrlGetCharaByName(name), 1, reinterpret_cast<std::byte *>(crateGripHandPose), 0);
    }
    mtwkSetRequestedRootMode(scenFindMotionwork(ARBT_RYO), 1);
    mtwkSetRequestedRootMode(scenFindMotionwork(ARBT_TKR), 1);
    mtwkSetBoneTargetStepLimit(scenFindMotionwork(ARBT_RYO), 10, 2);
    mtwkSetBoneTargetStepLimit(scenFindMotionwork(ARBT_RYO), 17, 1);
    mtwkSetBoneTargetStepLimit(scenFindMotionwork(ARBT_TKR), 10, 2);
    mtwkSetBoneTargetStepLimit(scenFindMotionwork(ARBT_TKR), 17, 2);
    mtwkSetExtraFlag(scenFindMotionwork(ARBT_RYO), 55, 0);
    mtwkSetExtraFlag(scenFindMotionwork(ARBT_TKR), 55, 0);
    AT_SetLocalBaseChara(ARBT_RYO, ARBT_BASE);
    AT_SetLocalBaseChara(ARBT_TKR, ARBT_BASE);
    AT_SetLocalBaseChara(ARBT_QLN, ARBT_BASE);
    AT_SetLocalBaseChara(ARBT_DUMA, ARBT_RYO);
    AT_SetLocalBaseChara(ARBT_DUMT, ARBT_TKR);
    AT_SetLocalBaseChara(ARBT_DUMS, ARBT_QLN);
    for (shen_name name : {ARBT_DUMA, ARBT_DUMT, ARBT_DUMS})
        EV_AT_SetPos(name, EventTransformFlags::AllAxes, arbtCameraOffset);
    for (shen_name name : {ARBT_DUMA, ARBT_DUMT, ARBT_DUMS})
        EV_AT_SetAngY(name, EventTransformFlags::AllAxes, arbtCameraRotation.angles);
    EVT_AT_SetPos(ARBT_TKR, ARBT_BASE, {-1.5f, 0, -1.5f}, 0);
    EventSetRawAngle tkr{.cleanup = 0, .character = ARBT_TKR, .parent = ARBT_BASE, .rotation = {0, 0, 0}, .delay = 0};
    EVT_AT_SetAng_Callback(&tkr);
    EVT_AT_SetPos(ARBT_QLN, ARBT_BASE, {-1, 0, -6.5f}, 0);
    EventSetRawAngle qln{.cleanup = 0, .character = ARBT_QLN, .parent = ARBT_BASE, .rotation = {0, 29127, 0}, .delay = 0};
    EVT_AT_SetAng_Callback(&qln);
    EVT_AT_SetPos(ARBT_RYO, ARBT_QLN, {0, 0, -1.5f}, 0);
    EventSetRawAngle ryo{.cleanup = 0, .character = ARBT_RYO, .parent = ARBT_QLN, .rotation = {0, 32768, 0}, .delay = 0};
    EVT_AT_SetAng_Callback(&ryo);
    for (shen_name name : ARBT_PAY_CROWD)
        EV_DisableCharacterDisplay(name);
    for (shen_name name : ARBT_PAY_CROWD)
        EV_ToggleCharacterFlag(name);
    SetFixoDataForMotionWork(ctrlGetCharaByName(ARBT_PAY), ctrlGetCharaByName(ARBT_QLN), 14, {0.197f, 0.075f, -0.018f}, {48423, 42780, 52792});
    EV_DisableCharacterDisplay(ARBT_PAY);
    BeginPrimarySampleExecution(0);
    EV_LoadAudio("0018E000", 0);
    EV_spmBeginSampleExecute();
    QueueImmediateNextFadeTask(nullptr);
    if (work.firstJob)
        HLib::SetCurrentTaskTo(ArbtCarryInit);
    else {
        work.authorFrames = ArbtIntroduceJob();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        HLib::SetCurrentTaskTo(ArbtReadyInitAuto);
    }
}
void ArbtReadyInitAuto(void *data) {
    ArbtWork &work = *static_cast<ArbtWork *>(data);
    if (authGetFinished() || (!work.firstJob && HasPressedCancel())) {
        EV_CleanupMapControlTask();
        work.authorFrames = 0;
    }
    if (work.authorFrames-- == 0) {
        HLib::SetCurrentTaskTo(ArbtCarryInit);
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetupTaskDelay(30);
    }
}
void ArbtCarryInit(void *data) {
    ArbtWork &work = *static_cast<ArbtWork *>(data);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_DisableCharacterDisplay(ARBT_QLN);
    SetInitialArbtCarrySettings(work.carry, work.delivered);
    work.authorFrames = work.delivered ? ArbtIntroduceNextCrate() : work.firstJob ? ArbtExplainFirstCarry() : ArbtExplainCarry();
    HLib::SetCurrentTaskTo(ArbtCarryAutoWait);
}
void ArbtCarryAutoWait(void *data) {
    ArbtWork &work = *static_cast<ArbtWork *>(data);
    if ((work.firstJob && authGetFinished()) || (!work.firstJob && HasPressedCancel())) {
        EV_CleanupMapControlTask();
        work.authorFrames = 0;
    }
    if (work.authorFrames-- == 0)
        HLib::SetCurrentTaskTo(ArbtCarryLift);
}
void ArbtCarryLift(void *data) {
    ArbtWork &work = *static_cast<ArbtWork *>(data);
    shenSetClockAndCompassRenderTo(0);
    Vector3 position;
    EV_GetCharaPosition(work.crates.crates[work.crates.index], CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    work.carry.frames = position.y <= 0.5f ? ArbtLiftLowCrate(work.crates) : ArbtLiftHighCrate(work.crates);
    HLib::SetCurrentTaskTo(work.carry.phase);
}
void ArbtCarryFinishTurns(void *data) {
    ArbtWork &work = *static_cast<ArbtWork *>(data);
    ArbtCarryWork &carry = work.carry;
    const std::int32_t turns = carry.turns--;
    if (turns > 0) {
        CheckHelpIcon(0xd020, 0, CheckHelpIconOp::Add);
        const std::int32_t delay = carry.frames > 40 ? carry.frames - 40 : 0;
        carry.success = 0;
        carry.frames = 40;
        carry.direction = 0x8000;
        carry.presses = 1;
        carry.intensity = 0;
        ArbtQueueDirection(carry.direction, 0, delay);
        HLib::SetCurrentTaskTo(ArbtCarryQte);
        if (delay > 0)
            HLib::SetupTaskDelay(delay);
    } else if (turns - 1 >= -1) {
        EVT_AT_SetVoice("0018C090", 1, carry.frames); // @text EN: "We're putting it down." | JA: "よし、降ろすぞ。"
        HLib::SetupTaskDelay(carry.frames + 60);
    } else {
        shenGetCurrentDateTime(work.time);
        if (work.time.hour < work.endTime.hour) {
            CheckHelpIcon(0xd020, 0xd020, CheckHelpIconOp::Remove);
            CheckHelpIcon(0x2020, 0, CheckHelpIconOp::Add);
            carry.frames = 50;
            carry.success = 0;
            carry.direction = 0x2000;
            carry.intensity = 7;
            carry.presses = 1;
            ArbtQueueDirection(carry.direction, 7, 0);
            HLib::SetCurrentTaskTo(ArbtCarryPutQte);
        } else {
            SetScreenFilmFade(60, 0, 0, 0, 0, 255, 0, 0, 0);
            HLib::SetCurrentTaskTo(ArbtCarryAddTime);
        }
    }
}
void ArbtCarryRoute(void *data) {
    ArbtWork &work = *static_cast<ArbtWork *>(data);
    ArbtCarryWork &carry = work.carry;
    const ArbtRouteStep *step = carry.route;
    CheckHelpIcon(0xd020, 0, CheckHelpIconOp::Add);
    if (carry.forwardSteps >= step->forwardSteps) {
        ++carry.route;
        carry.forwardSteps = 0;
        if (step->intensity == 8) {
            carry.turns = 2;
            carry.phase = ArbtCarryFinishTurns;
            HLib::SetCurrentTaskTo(carry.phase);
        } else {
            std::int32_t delay = static_cast<std::int32_t>(static_cast<float>(carry.frames) - step->frames);
            if (delay < 0)
                delay = 0;
            carry.success = 0;
            carry.direction = step->direction;
            carry.intensity = step->intensity;
            carry.frames = static_cast<std::int32_t>(step->frames);
            carry.presses = ARBT_PRESS_COUNTS[step->intensity];
            ArbtQueueDirection(carry.direction, carry.intensity, delay);
            HLib::SetCurrentTaskTo(ArbtCarryQte);
            if (delay > 0)
                HLib::SetupTaskDelay(delay);
        }
    } else {
        shenGetCurrentDateTime(work.time);
        if (work.time.hour < work.endTime.hour) {
            const std::int32_t delay = carry.frames > 30 ? carry.frames - 30 : 0;
            carry.success = 0;
            carry.frames = 30;
            carry.direction = 0x1000;
            carry.intensity = 6;
            carry.presses = 1;
            ArbtQueueDirection(carry.direction, 6, delay);
            HLib::SetCurrentTaskTo(ArbtCarryForwardQte);
            if (delay > 0)
                HLib::SetupTaskDelay(delay);
        } else {
            EVT_AT_MenCtrl({.cleanup = 0,
                .frames = 60,
                .startAlpha = 0,
                .startRed = 0,
                .startGreen = 0,
                .startBlue = 0,
                .targetAlpha = 255,
                .targetRed = 0,
                .targetGreen = 0,
                .targetBlue = 0,
                .delay = static_cast<float>(carry.frames)});
            HLib::SetCurrentTaskTo(ArbtCarryAddTime);
            if (carry.frames > 0)
                HLib::SetupTaskDelay(carry.frames);
        }
    }
}
void ArbtCarryQte(void *data) {
    ArbtWork &work = *static_cast<ArbtWork *>(data);
    ArbtCarryWork &carry = work.carry;
    const std::uint32_t wrong = carry.direction ^ 0xd000;
    if (--carry.frames <= 0) {
        if (carry.success) {
            carry.consecutiveFailures = 0;
            carry.frames = ArbtWalkDirection(carry.speed, carry.intensity, carry.direction);
            HLib::SetCurrentTaskTo(carry.phase);
        } else {
            ++carry.failures;
            ++carry.consecutiveFailures;
            const std::int32_t frames = ArbtFailCarry(work.crates, work.crates.crates[work.crates.index], carry.intensity, carry.direction);
            HLib::SetCurrentTaskTo(ArbtCarryFalse);
            HLib::SetupTaskDelay(frames);
        }
        return;
    }
    const std::uint32_t pressed = static_cast<std::uint32_t>(GetCurrentControlInput(INPUT_PRESSED));
    if (carry.direction == 0x8000)
        SetupBattleQteIcon(4, 0);
    else if (carry.direction == 0x4000)
        SetupBattleQteIcon(64, 0);
    EnableElement(UIElement::GameQte);
    if (pressed == carry.direction || ((pressed & carry.direction) && carry.direction == 0x1000)) {
        if (carry.presses-- == 1) {
            Loop3DSoundEffect(0x900ab);
            carry.success = 1;
        } else if (carry.presses <= 0) {
            Loop3DSoundEffect(0xa00ab);
            HLib::SetupTaskDelay(carry.frames);
            carry.success = 0;
            carry.frames = 0;
        } else
            Loop3DSoundEffect(0x900ab);
    } else if ((pressed & wrong) && carry.frames < 20 && !carry.success) {
        Loop3DSoundEffect(0xa00ab);
        HLib::SetupTaskDelay(carry.frames);
        carry.frames = 0;
    }
}
void ArbtCarryForwardQte(void *data) {
    ArbtWork &work = *static_cast<ArbtWork *>(data);
    ArbtCarryWork &carry = work.carry;
    const std::uint32_t wrong = carry.direction ^ 0xd000;
    if (--carry.frames <= 0) {
        HLib::SetCurrentTaskTo(carry.phase);
        if (++carry.idleCount > 3) {
            carry.idleCount = 0;
            HLib::SetupTaskDelay(ArbtWarnIdleForward(++work.idleWarnings));
        }
        return;
    }
    const std::uint32_t held = static_cast<std::uint32_t>(GetCurrentControlInput(INPUT_HELD)),
                        pressed = static_cast<std::uint32_t>(GetCurrentControlInput(INPUT_PRESSED));
    SetupBattleQteIcon(1, 0);
    EnableElement(UIElement::GameQte);
    if (held & carry.direction) {
        HLib::SetCurrentTaskTo(carry.phase);
        if (carry.idleCount || carry.frames <= 0)
            carry.frames = 0;
        else
            HLib::SetupTaskDelay(carry.frames);
        ++carry.forwardSteps;
        carry.idleCount = 0;
        carry.frames = ArbtWalkForward(carry.speed, carry.frames);
    } else if (pressed & wrong) {
        HLib::SetCurrentTaskTo(ArbtCarryQte);
        if (!carry.idleCount && carry.frames > 0)
            HLib::SetupTaskDelay(carry.frames);
        carry.frames = 0;
        carry.idleCount = 0;
        carry.direction = 0x2000;
        EVT_AT_SetVoice((pressed & 0x8000) ? "0018A003" : (pressed & 0x4000) ? "0018A005" : "0018A006", 1, 0); // @text 0018A003 EN: "Right=@" | JA: "右…" ; 0018A005 EN: "Left=@" | JA: "左…" ; 0018A006 EN: " " | JA: "後ろ…"
    }
}
void ArbtCarryPutQte(void *data) {
    ArbtWork &work = *static_cast<ArbtWork *>(data);
    ArbtCarryWork &carry = work.carry;
    if (--carry.frames <= 0) {
        HLib::SetCurrentTaskTo(carry.phase);
        if (++carry.idleCount > 3) {
            carry.idleCount = 0;
            HLib::SetupTaskDelay(ArbtWarnIdlePut(++work.idleWarnings));
        }
    } else {
        const std::uint32_t held = static_cast<std::uint32_t>(GetCurrentControlInput(INPUT_HELD));
        SetupBattleQteIcon(16, 0);
        EnableElement(UIElement::GameQte);
        if (held & carry.direction) {
            Loop3DSoundEffect(0x900ab);
            HLib::SetCurrentTaskTo(ArbtCarryPut);
            carry.idleCount = 0;
            carry.frames = 0;
        }
    }
}
void ArbtCarryFalse(void *data) {
    ArbtWork &work = *static_cast<ArbtWork *>(data);
    const ArbtCarryWork &carry = work.carry;
    std::int32_t frames;
    if (carry.intensity == 3)
        frames = ArbtWarnWrongDouble(carry.direction);
    else if (carry.intensity > 3 && carry.intensity <= 5)
        frames = ArbtWarnWrongMultiple();
    else if (carry.intensity == 6)
        frames = ArbtWarnWrongForward();
    else
        switch (carry.failures >> 1) {
        case 0:
            frames = ArbtWarnCarryOnce();
            break;
        case 1:
            frames = ArbtWarnCarryTwice();
            break;
        case 2:
            frames = ArbtWarnCarryThird();
            break;
        case 3:
            frames = ArbtWarnCarryFourth();
            break;
        default:
            frames = ArbtWarnCarryRepeatedly();
            break;
        }
    HLib::SetCurrentTaskTo(ArbtCarryLift);
    HLib::SetupTaskDelay(frames);
}
void ArbtCarryPut(void *data) {
    ArbtWork &work = *static_cast<ArbtWork *>(data);
    const std::int32_t index = work.crates.index;
    const shen_name crate = work.crates.crates[index], destination = work.crates.destinations[index];
    CheckHelpIcon(0x2020, 0x2020, CheckHelpIconOp::Remove);
    if (work.carry.failures)
        work.perfectStreak = 0;
    else
        ++work.perfectStreak;
    EventSetPositionAngle request{.cleanup = 0,
        .character = crate,
        .parent = destination,
        .position = work.crates.stacked[index] ? Vector3{-0.05f, 0, 0.1f} : Vector3{0, 0, 0.2f},
        .degrees = 0,
        .delay = 0};
    EVT_AT_SetPosAng_Callback(&request);
    const std::int32_t frames = work.crates.stacked[index] ? ArbtPutHighCrate(work.crates, work.perfectStreak, destination) :
                                                             ArbtPutLowCrate(work.crates, work.perfectStreak, destination);
    HLib::SetCurrentTaskTo(ArbtCarryEnd);
    HLib::SetupTaskDelay(frames);
}
void ArbtCarryEnd(void *data) {
    ArbtWork &work = *static_cast<ArbtWork *>(data);
    std::int32_t frames = work.crates.limit - (work.carry.failures >> 2);
    if (frames < work.delivered)
        frames = work.delivered;
    work.crates.limit = frames;
    work.failures += work.carry.failures;
    ++work.delivered;
    if (SelectNextArbtCrate(work.crates)) {
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetCurrentTaskTo(ArbtCarryInit);
        HLib::SetupTaskDelay(30);
    } else {
        SetScreenFilmFade(60, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetCurrentTaskTo(ArbtCarryAddTime);
    }
}
void ArbtCarryAddTime(void *data) {
    ArbtWork &work = *static_cast<ArbtWork *>(data);
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    shenSetClockAndCompassRenderTo(1);
    shenGetCurrentDateTime(work.time);
    if (work.time.hour >= work.endTime.hour + 1) {
        if (!filmCheckFadeTrackColour())
            HLib::SetCurrentTaskTo(ArbtCarryTimeOver);
    } else {
        if (++work.time.minute >= 60) {
            work.time.minute = 0;
            ++work.time.hour;
        }
        SetCurrentDateTimeTo(work.time);
    }
}
void ArbtCarryTimeOver(void *data) {
    ArbtWork &work = *static_cast<ArbtWork *>(data);
    EV_EnableCharacterDisplay(ARBT_QLN);
    const std::int32_t frames = ArbtAnnounceTimeOver();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    work.authorFrames = frames;
    HLib::SetCurrentTaskTo(ArbtCarryTimeOverAuto);
}
void ArbtCarryTimeOverAuto(void *data) {
    ArbtWork &work = *static_cast<ArbtWork *>(data);
    if ((work.firstJob && authGetFinished()) || (!work.firstJob && HasPressedCancel())) {
        EV_CleanupMapControlTask();
        work.authorFrames = 0;
    }
    if (work.authorFrames-- == 0) {
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetCurrentTaskTo(ArbtPayInit);
        HLib::SetupTaskDelay(30);
    }
}
void ArbtPayInit(void *data) {
    ArbtWork &work = *static_cast<ArbtWork *>(data);
    EVT_AT_SetPos(ARBT_BASE, EVENT_CHARACTER_INVALID, work.descriptor->mode == 1 ? Vector3{142.9f, 100, 138.9f} : Vector3{225.2f, 100, 247.5f}, 0);
    EVT_AT_SetAngY(ARBT_BASE, EVENT_CHARACTER_INVALID, 270, 0);
    if (work.descriptor->mode != 1) {
        EV_SetModelDataDefaultVertexShader();
        EV_FreeWAIR();
        EV_UpdateWeatherForMinute(0, 0, 0, 0);
        EV_UpdateWeatherForMinute(1, 0, 0, 0);
        EV_UpdateWeather();
        EV_SetWeatherInstruction(12, 0);
        EV_SetWeatherInstruction(0x780, 0);
        EV_lightLoad(ARBT_LIGHT_AKB0);
        PushGlobalLightingProgram();
        HideArbtDockProps();
    }
    SetPluralControlFor(ctrlGetCharaByName(ARBT_RYO), 1);
    SetPluralControlFor(ctrlGetCharaByName(ARBT_TKR), 1);
    for (shen_name name : ARBT_PAY_CROWD)
        SetPluralControlFor(ctrlGetCharaByName(name), 1);
    mtwkSetExtraFlag(scenFindMotionwork(ARBT_RYO), 55, 2);
    mtwkSetExtraFlag(scenFindMotionwork(ARBT_TKR), 55, 2);
    for (shen_name name : ARBT_PAY_CROWD)
        EV_EnableCharacterDisplay(name);
    for (shen_name name : ARBT_PAY_CROWD)
        EV_EnsureCharacterDraw(name);
    if (work.crates.moveTask) {
        work.crates.moveFrames = 0;
        HLib::DestroyTaskInternal(work.crates.moveTask);
        work.crates.moveTask = nullptr;
    }
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (work.delivered < 6)
        ++work.delivered;
    HLib::SetCurrentTaskTo(ArbtPayAuto);
}
void ArbtPayAuto(void *data) {
    ArbtWork &work = *static_cast<ArbtWork *>(data);
    work.cash = 10 * work.delivered;
    work.payEndFrame = work.firstJob ? ArbtPayFirstJob(work.cash, work.delivered) :
        work.delivered >= 5          ? ArbtPayGoodJob(work.cash, work.delivered) :
        work.delivered >= 3          ? ArbtPayAverageJob(work.cash, work.delivered) :
                                       ArbtPayPoorJob(work.cash, work.delivered);
    work.payFrame = 0;
    HLib::SetCurrentTaskTo(ArbtPayAutoWait);
}
void ArbtPayAutoWait(void *data) {
    ArbtWork &work = *static_cast<ArbtWork *>(data);
    if (work.payFrame >= 219) {
        if (work.payFrame == work.payEndFrame - 60)
            ProcessAicaCommand(0xaa0, 0);
        if (work.payFrame == work.payEndFrame - 30)
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        if (work.payFrame == work.payEndFrame)
            HLib::SetCurrentTaskTo(ArbtPayEnd);
    } else if ((work.firstJob && authGetFinished()) || (!work.firstJob && HasPressedCancel())) {
        EV_CleanupMapControlTask();
        AddThisToCashOrSegaCoins(work.cash, static_cast<ECashOrCoins>(2));
        ProcessAicaCommand(0xaa0, 30);
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetCurrentTaskTo(ArbtPayEnd);
        HLib::SetupTaskDelay(80);
        return;
    }
    ++work.payFrame;
}
void ArbtPayEnd(void *data) {
    ArbtWork &work = *static_cast<ArbtWork *>(data);
    ResetCharaFixo(ctrlGetCharaByName(ARBT_PAY));
    if (work.descriptor->mode == 2)
        HLib::SetCurrentTaskTo(ArbtReadyInit);
    else
        ArbtLeave(work);
}
void ArbtMain(ArbtWork &work) {
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    work.characters = Queue_EVT_AT_ReadChr(work.self, ARBT_CHARACTERS);
    work.delivered = 0;
    work.failures = 0;
    work.idleWarnings = 0;
    work.perfectStreak = 0;
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("ARBT"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("ARBT"), 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("ARBT"), 0);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0018E000");
    EV_FreeAuthorPak(1, work.descriptor->mode == 1 ? "0360" : "0018");
    shenGetCurrentDateTime(work.time);
    shenGetCurrentDateTime(work.endTime);
    if (EV_GetShenmueFlag(0x12)) {
        EV_stepSetFlags(0, 0x12);
        work.firstJob = 1;
        work.time.hour = 10;
    } else {
        work.firstJob = 0;
        work.time.hour = work.descriptor->shift == 0 ? 10 : work.descriptor->shift == 1 ? 13 : work.descriptor->shift == 2 ? 16 : 20;
    }
    work.time.minute = 0;
    work.time.second = 0;
    work.endTime.hour = work.time.hour + 1;
    work.endTime.minute = 0;
    work.endTime.second = 0;
    SetCurrentDateTimeTo(work.time);
    shenSetTimeSimulationTo(1);
    EV_stepSetFlags(0, 65);
    if (work.descriptor->mode == 1) {
        EVT_AT_SetPos(ARBT_BASE, EVENT_CHARACTER_INVALID, {175.7f, 100, 177}, 0);
        EVT_AT_SetAngY(ARBT_BASE, EVENT_CHARACTER_INVALID, 270, 0);
        EV_FreeWAIR();
        EV_UpdateWeatherForMinute(0, 0, 0, 0);
        EV_UpdateWeatherForMinute(1, 0, 0, 0);
        EV_UpdateWeather();
        EV_SetWeatherInstruction(12, 0);
        EV_SetWeatherInstruction(0x780, 0);
        EV_lightLoad(ARBT_LIGHT_QCAE);
        PushGlobalLightingProgram();
        EV_AssignLightParams(0, 2);
        EV_AssignLightParams(0, 8);
        EV_AssignLightParams(0, 9);
        for (std::int32_t index = 0; index < 10; ++index)
            EV_SetObjectFlagByIndex(index, 0);
    } else if (work.descriptor->shift == 3) {
        EventSetPositionAngle position{
            .cleanup = 0, .character = ARBT_BASE, .parent = EVENT_CHARACTER_INVALID, .position = {266.8f, 0, 260.5f}, .degrees = 0, .delay = 0};
        EVT_AT_SetPosAng_Callback(&position);
        RefreshWeatherForCurrentTime(0);
        EV_UpdateWeather();
        EV_SetWeatherInstruction(12, 0);
        EV_SetWeatherInstruction(0x780, 0);
        EV_lightLoad(ARBT_AK00);
        PushGlobalLightingProgram();
        EV_AssignLightParams(0, 8);
        EV_AssignLightParams(0, 9);
        EV_AssignLightParams(0, 10);
        for (std::int32_t index : {5, 6, 8, 10})
            EV_SetObjectFlagByIndex(index, 0);
        SetPluralControlFor(ctrlGetCharaByName(ARBT_RYO), 1);
        SetPluralControlFor(ctrlGetCharaByName(ARBT_TKR), 1);
    } else if (work.descriptor->shift == 1) {
        EventSetPositionAngle position{
            .cleanup = 0, .character = ARBT_BASE, .parent = EVENT_CHARACTER_INVALID, .position = {270, 0, 238.7f}, .degrees = 270, .delay = 0};
        EVT_AT_SetPosAng_Callback(&position);
        EV_FreeWAIR();
        EV_UpdateWeatherForMinute(0, 0, 0, 0);
        EV_UpdateWeatherForMinute(1, 0, 0, 0);
        EV_UpdateWeather();
        EV_SetWeatherInstruction(12, 0);
        EV_SetWeatherInstruction(0x780, 0);
        EV_lightLoad(ARBT_LIGHT_AKA3);
        PushGlobalLightingProgram();
        EV_AssignLightParams(0, 2);
        EV_AssignLightParams(0, 9);
        EV_AssignLightParams(0, 10);
        EV_SetObjectFlagByIndex(10, 0);
        HideArbtDockProps();
        for (std::int32_t index = 0; index < 8; ++index)
            EV_SetObjectFlagByIndex(index, 0);
    } else {
        EVT_AT_SetPos(ARBT_BASE, EVENT_CHARACTER_INVALID, {225.2f, 100, 247.5f}, 0);
        EVT_AT_SetAngY(ARBT_BASE, EVENT_CHARACTER_INVALID, 270, 0);
        RefreshWeatherForCurrentTime(0);
        EV_UpdateWeather();
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        EV_AssignLightParams(0, 8);
        EV_AssignLightParams(0, 9);
        EV_AssignLightParams(0, 10);
        for (std::int32_t index : {5, 7, 8, 10})
            EV_SetObjectFlagByIndex(index, 0);
    }
    shenSetClockAndCompassRenderTo(1);
    ArbtHelpNullsub();
    EV_EventSetUpHelpIcon_0(70, 0);
    SetInitialArbtCarrySettings(work.carry, 0);
    CreateArbtCrates(work.crates);
}
} // namespace
void ArbtCarryTurnInit(void *data) {
    ArbtWork &work = *static_cast<ArbtWork *>(data);
    ArbtCarryWork &carry = work.carry;
    CheckHelpIcon(0xd020, 0, CheckHelpIconOp::Add);
    if (carry.turns-- != 0) {
        const std::int32_t delay = carry.frames > 40 ? carry.frames - 40 : 0;
        carry.success = 0;
        carry.frames = 40;
        carry.presses = 1;
        carry.intensity = 0;
        carry.direction = work.delivered < 2 ? 0x4000 : 0x8000;
        ArbtQueueDirection(carry.direction, 0, delay);
        HLib::SetCurrentTaskTo(ArbtCarryQte);
        if (delay > 0)
            HLib::SetupTaskDelay(delay);
    } else {
        carry.phase = ArbtCarryRoute;
        HLib::SetCurrentTaskTo(carry.phase);
        if (!work.delivered) {
            const std::int32_t frames = ArbtStartWork(carry.frames);
            carry.frames = 0;
            HLib::SetupTaskDelay(frames);
        }
    }
}
void InitializeArbtEvent() {
    arbtCameraOffset = {0, 0, 0};
    arbtCameraRotation.angles = {0, 0, 0};
}
void QueueArbtReadyInit(std::uintptr_t argument) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(ArbtReadyInit, 4, 0x0b, sizeof(ArbtWork), AREA_EVENT_TASK);
    ArbtWork &work = *static_cast<ArbtWork *>(HLib::GetTaskParameter(task));
    work.reserved170 = 0;
    work.self = task;
    if (argument)
        work.descriptor = reinterpret_cast<const ArbtDescriptor *>(argument);
    else {
        work.descriptor = ARBT_DESCRIPTORS;
        if (g_entryPoint - 90U <= 8U)
            work.descriptor = &ARBT_DESCRIPTORS[g_entryPoint - 90U];
    }
    ArbtMain(work);
}
void RunArbtEvent(std::uintptr_t argument) {
    QueueArbtReadyInit(argument);
}
const ScriptAreaDefinition arbtScriptDefinition{.areaId = ARBT_AREA, .field04 = 0, .init = InitializeArbtEvent, .main = RunArbtEvent};
} // namespace shenmue::s2::areas
#endif
