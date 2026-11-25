#pragma once

#include "common/types.hpp"
#include <cstdint>

namespace shenmue { namespace s2 {

struct Shenmue1SaveAreaMapping {
    std::uint32_t mapId;
    std::int32_t saveAreaId;
};

static const Shenmue1SaveAreaMapping shenmue1SaveAreaMappings[] = {
    {SHEN_NAME(D000), 0x63},
        {SHEN_NAME(JD00), 0x13},
            {SHEN_NAME(JU00), 0x0f},
                {SHEN_NAME(JHD0), 0x10},
                    {SHEN_NAME(MN01), 0x55},
                        {SHEN_NAME(MO01), 0x5f},
                            {SHEN_NAME(YDB1), 0x0e},
                                {SHEN_NAME(MS08), 0x4c},
                                    {SHEN_NAME(DSBA), 0x31},
                                        {SHEN_NAME(DCHA), 0x29},
                                            {SHEN_NAME(DPIA), 0x38},
                                                {SHEN_NAME(DKPA), 0x39},
                                                    {SHEN_NAME(DRSA), 0x2e},
                                                        {SHEN_NAME(DAZA), 0x28},
                                                            {SHEN_NAME(DBHB), 0x2a},
                                                                {SHEN_NAME(JABE), 0x17},
                                                                    {SHEN_NAME(DRME), 0x3a},
                                                                        {SHEN_NAME(DSKI), 0x26},
                                                                            {SHEN_NAME(DSLI), 0x37},
                                                                                {SHEN_NAME(DMAJ), 0x35},
                                                                                    {SHEN_NAME(DCBN), 0x32},
                                                                                        {SHEN_NAME(DURN), 0x34},
                                                                                            {SHEN_NAME(JOMO), 0x00},
                                                                                                {SHEN_NAME(DBYO), 0x2b},
                                                                                                    {SHEN_NAME(TATQ), 0x2d},
                                                                                                        {SHEN_NAME(MS8S), 0x53},
                                                                                                            {SHEN_NAME(DSUS), 0x33},
                                                                                                                {SHEN_NAME(DGCT), 0x25},
                                                                                                                    {SHEN_NAME(DRHT), 0x2f},
                                                                                                                        {SHEN_NAME(DSLT), 0x36},
                                                                                                                            {SHEN_NAME(MKYU), 0x61},
                                                                                                                                {SHEN_NAME(DTKY), 0x30},
                                                                                                                                    {SHEN_NAME(MKSG), 0x46},
                                                                                                                                        {SHEN_NAME(DKTY), 0x27},
                                                                                                                                            {SHEN_NAME(DJAZ),
                                                                                                                                                 0x2c},
                                                                                                                                                {SHEN_NAME(DYKZ),
                                                                                                                                                     0x3b},
                                                                                                                                                    {SHEN_NAME(MFSY),
                                                                                                                                                         0x64},
                                                                                                                                                        {0, -1},
};

typedef char SaveAreaMappingLayout[(sizeof(Shenmue1SaveAreaMapping) == 8) ? 1 : -1];

} } // namespace shenmue::s2
