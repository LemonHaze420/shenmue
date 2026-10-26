#pragma once
#include "shenmue.h"
#if SHENMUE_ENHANCEMENT_STEAM_S1_MP && SHENMUE_IS_S1 && SHENMUE_IS_PC
#include <cstdint>
namespace shenmue::s1::multiplayer_steam {
bool Initialize();
bool BeginHost();
void Join(std::uint64_t lobby);
void Update();
bool Connected();
bool Active();
std::uint64_t Lobby();
bool Host();
bool Send(const void *bytes, std::uint32_t size);
int Receive(void *bytes, std::uint32_t size);
void Leave();
void Release();
} // namespace shenmue::s1::multiplayer_steam
#endif
