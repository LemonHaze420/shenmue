#pragma once
#include "s2/areas/area_ksht.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name kshtCompanionList[]{KSHT_SIN, KSHT_INVALID};
inline constexpr shen_name kshtGuestList[]{SHEN_NAME(IWA_), SHEN_NAME(LRY_), KSHT_INVALID};
inline constexpr shen_name kshtParentList[]{SHEN_NAME(SNP_), SHEN_NAME(SNM_), KSHT_INVALID};
inline constexpr shen_name kshtCompanionAndChildList[]{KSHT_SIN, SHEN_NAME(SN5_), KSHT_INVALID};
inline constexpr shen_name kshtAuthorIds[]{SHEN_NAME(A100), SHEN_NAME(A101), SHEN_NAME(A102), SHEN_NAME(A103), SHEN_NAME(A104), SHEN_NAME(A105),
    SHEN_NAME(A106), SHEN_NAME(A107), SHEN_NAME(A120), SHEN_NAME(A121), SHEN_NAME(A210), SHEN_NAME(A211), SHEN_NAME(A212), SHEN_NAME(A213), SHEN_NAME(A214),
    SHEN_NAME(A215), SHEN_NAME(A280), SHEN_NAME(A281), SHEN_NAME(A282), SHEN_NAME(A370), SHEN_NAME(A371), SHEN_NAME(A430), SHEN_NAME(A431), SHEN_NAME(A450),
    SHEN_NAME(A451)};
struct KshtAuthorLoop {
    KshtAuthorController *parent;
    std::int16_t frame;
    std::byte reserved0a[6];
};
static_assert(sizeof(KshtAuthorLoop) == 0x10);
void PrepareKshtActor(shen_name actor);
void RestoreKshtActor(shen_name actor);
void PrepareKshtProp(shen_name actor);
void RestoreKshtProp(shen_name actor);
void FreeKshtCharacters(const shen_name *characters);
void LoadKshtCharacters(const shen_name *characters, HLib::Task *parent);
void ResetKshtMotion(shen_name actor);
void ResetKshtCamera();
void QueueKshtCloudAuthor(KshtAuthorController *);
void SetupKshtCloudAuthor();
void ExitKshtCloudAuthor(KshtAuthorController *);
void QueueKshtDrawingAuthor(KshtAuthorController *);
void SetupKshtDrawingAuthor();
void ExitKshtDrawingAuthor(KshtAuthorController *);
void QueueKshtConversationAuthor(KshtAuthorController *);
void SetupKshtConversationAuthor();
void ExitKshtConversationAuthor(KshtAuthorController *);
void QueueKshtRabbitAuthor(KshtAuthorController *);
void SetupKshtRabbitAuthor();
void ExitKshtRabbitAuthor(KshtAuthorController *);
void QueueKshtDepartureAuthor(KshtAuthorController *);
void SetupKshtDepartureAuthor();
void ExitKshtDepartureAuthor(KshtAuthorController *);
void QueueKshtMorningAuthor(KshtAuthorController *);
void SetupKshtMorningAuthor();
void ExitKshtMorningAuthor(KshtAuthorController *);
void QueueKshtRabbitDropAuthor(KshtAuthorController *);
void SetupKshtRabbitDropAuthor();
void ExitKshtRabbitDropAuthor(KshtAuthorController *);
} // namespace shenmue::s2::areas
