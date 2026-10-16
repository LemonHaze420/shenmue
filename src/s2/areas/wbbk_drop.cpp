#include "s2/areas/area_wbbk.hpp"
#include "s2/areas/wbbk_dialogue.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/input.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/particle_effects.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include <bit>
namespace shenmue::s2::areas {
std::uint64_t bookDropRecovery;
float bookDropTimePenalty;
namespace {
void UpdateDroppedBookDisplay() {
    UpdateBookCarryAnchor();
    EV_AT_SetAngY(BOOK_BASE, EventTransformFlags::AllAxes, {.x = 0, .y = 0, .z = 16384});
    UpdateBookStackDisplay();
}
void BeginDroppedBooks() {
    bookCarryPhase = 6;
    bookModelState.displayMode = 1;
    bookDropRecovery &= 0xffffffff00000000ULL;
    bookModelState.animationFrame = 0;
    SetMotionRegionFlag(scenFindMotionwork(BOOK_RYO), 71, 0, MotionRegion::LowerBody);
    ResetBookMotionLayers();
    mtwkRequestMotion(scenFindMotionwork(BOOK_RYO), bookModelState.mirror >= 0 ? 0x188a : 0x1889, MotionRegion::LowerBody);
    SetMotionRegionEvalTime(scenFindMotionwork(BOOK_RYO), 1, MotionRegion::LowerBody);
    EV_SetCharaControllerFlags(BOOK_DYNAMIC, CharacterControlUpdateHeight, NpcCharacterControllerState::Disabled);
    CopyBookPlayerAnchor();
    AT_SetLocalBaseChara(BOOK_DYNAMIC, BOOK_RYO);
    Vector3 position;
    EV_GetCharaPosition(BOOK_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    position.y += 1.5f;
    UpdateMotionWorkWithPosition(scenFindMotionwork(BOOK_SAI), &position, 1);
    EV_AT_SetPos(BOOK_DYNAMIC, EventTransformFlags::AllAxes, {.x = .05f, .y = 1.03f, .z = -.37f});
    EV_AT_SetAngY(BOOK_DYNAMIC, EventTransformFlags::AllAxes, {.x = 0, .y = 0, .z = 0});
    if (bookCarryVariant <= 2)
        EVT_AT_SetVoice(bookMinorDropVoices[shenGetRandomInt32(3)], 0);
    else {
        const std::int32_t choice = shenGetRandomInt32(4);
        EVT_AT_SetVoice(bookDropPlayerVoices[choice], 0);
        EVT_AT_SetVoice(bookDropCompanionVoices[choice], 45);
        if (bookDropReplyVoices[choice])
            EVT_AT_SetVoice(bookDropReplyVoices[choice], 155);
    }
    camCtrlToggleSmoothing(0);
    EVS_LoopCamera(shenGetRandomInt32(2) + 2020, bookPlayerAnchor, bookPlayerAnchor);
}
void BookDropFadeCallback(void *) {
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    HLib::DestroyCurrentTask();
}
void FinishDroppedBooks() {
    UpdateMotionWorkWithPosition(scenFindMotionwork(BOOK_RYO), nullptr, 0);
    SetRenderClockAndCompassTo(0);
    bookModelState.displayMode = 0;
    AttachBookStackToPlayer();
    bookCarryPhase = 0;
    bookCarryMotion.radialOffset = 0;
    bookCarryMotion.requested = 9;
    bookCarryMotion.pendingQte = 0;
    ChangeBookCarryMotion(9, 0);
    bookCarryIdleFrames = 0;
    EV_EnableCharacterDisplay(bookModelState.stacks[bookModelState.currentStack + 1]);
    bookDesiredCamera = 1220;
    bookCurrentCamera = 0;
    HLib::Task *task = HLib::EnqueueTaskWithoutParameter(BookDropFadeCallback, 6, 0x0d, static_cast<std::int32_t>(BOOK_TASK.name));
    HLib::SetupTimedTask(task, 1);
    DateTime time;
    GetCurrentDateTime(time);
    time.minute += static_cast<std::int32_t>(bookDropTimePenalty);
    SetCurrentDateTimeTo(time);
    Vector3 sampled;
    SampleBookCarryTrack(bookCarryTrack, sampled, bookCarryDistance * .125f, 0);
}
void AttachDroppedBook(shen_name actor, const Vector3 &position, const Vector3i &rotation) {
    EV_SetCharaControllerFlags(actor, CharacterControlUpdateHeight | CharacterControlApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(BOOK_RYO, CharacterControlUpdateHeight | CharacterControlApplyGravity, NpcCharacterControllerState::Disabled);
    SetFixoDataForMotionWork(FindCharacterByScenarioName(actor), FindCharacterByScenarioName(BOOK_RYO), 14, position, {rotation.x, rotation.y, rotation.z});
}
void QueueDroppedBookMovement(shen_name actor, const Vector3 &position, const Vector3i &rotation, std::int32_t duration) {
    EV_GetCharaPosition(actor, CHARACTER_TRANSLATION_ROOT, &bookSmoothMovement.fromPosition, EventTransformFlags::Local);
    GetScenarioCharacterEulerAngles(actor, &bookSmoothMovement.fromRotation, EventTransformFlags::Local);
    bookSmoothMovement.toPosition = position;
    bookSmoothMovement.toRotation = rotation;
    bookSmoothMovement.character = actor;
    bookSmoothMovement.frame = 0;
    bookSmoothMovement.duration = duration;
    bookSmoothMovement.flags = 0x20072;
    HLib::EnqueueTaskWithoutParameter(BookSmoothMovementCallback, 6, 0x0d, static_cast<std::int32_t>(BOOK_TASK.name));
}
void RecoverDroppedBooks() {
    std::int32_t frame = bookModelState.animationFrame;
    if (bookDropRecovery) {
        if (frame < 89) {
            UpdateDroppedBookDisplay();
            frame = bookModelState.animationFrame;
        }
        std::uint32_t recoveryFrame = static_cast<std::uint32_t>(bookDropRecovery >> 32);
        if (!recoveryFrame) {
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            EV_CleanupMapControlTask();
        }
        if (recoveryFrame == 60) {
            FinishDroppedBooks();
            EV_EnsureCharacterDraw(BOOK_BASE);
            frame = 0;
        }
        bookDropRecovery = (bookDropRecovery & 0xffffffffULL) | (std::uint64_t{recoveryFrame + 1} << 32);
        bookModelState.animationFrame = frame + 1;
        return;
    }
    if (frame == 10) {
        Vector3 position;
        Vector3i rotation;
        EV_GetCharaPosition(BOOK_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        GetScenarioCharacterEulerAngles(BOOK_RYO, &rotation, EventTransformFlags::World);
        Vector3 offset{.x = static_cast<float>(bookModelState.mirror), .y = 0, .z = -1};
        ApplyScenarioTranslationAndRotation(&position, &rotation, &offset);
    }
    if (frame == 45)
        Loop3DSoundEffect(1578);
    if (frame < 89)
        UpdateDroppedBookDisplay();
    if (frame == 90) {
        SetMotionRegionFlag(scenFindMotionwork(BOOK_RYO), 69, 2, MotionRegion::LowerBody);
        SetBookActorMotion(BOOK_RYO, 6280);
        SetMotionSpeed(scenFindMotionwork(BOOK_RYO), 1, MotionRegion::LowerBody);
        SetMotionRegionEvalTime(scenFindMotionwork(BOOK_RYO), 250, MotionRegion::LowerBody);
        CopyBookPlayerAnchor();
        EVS_LoopCamera(shenGetRandomInt32(2) + 2030, bookPlayerAnchor, bookPlayerAnchor);
        AttachBookStackToPlayer();
        bookModelState.displayMode = 2;
        UpdateDroppedBookDisplay();
        for (std::int32_t index = 0; index < 3; ++index) {
            const shen_name actor{SHEN_NAME(RZOU) + static_cast<std::uint32_t>(index)};
            AT_SetLocalBaseChara(actor, bookPlayerAnchor);
            Vector3 offset;
            offset.x = .30000001f;
            offset.y = 1;
            EV_AT_SetPos(actor, EventTransformFlags::AllAxes, offset);
        }
        AT_SetLocalBaseChara(BOOK_BASE, bookPlayerAnchor);
        EV_AT_SetPos(BOOK_BASE, EventTransformFlags::AllAxes, {.x = -.356999993f, .y = -.259999990f, .z = -.49900001f});
        EV_AT_SetAngY(BOOK_BASE, EventTransformFlags::AllAxes, {.x = 19114, .y = 0, .z = 16384});

        constexpr std::uint32_t offsetWords[3][6]{
            {0, 0, 0x5f4e4552, 0x5f415559, 0xffffffff, 0}, {0, 0xe000, 0, 0, 0x430a0000, 0x42cc4ccd}, {0x42e40000, 0, 0x53495349, 0, 0x4d495349, 0}};
        for (std::int32_t index = 0; index < 3; ++index) {
            const shen_name actor{SHEN_NAME(RZOU) + static_cast<std::uint32_t>(index)};
            const std::uint32_t *words = offsetWords[index];
            const Vector3 position{.x = std::bit_cast<float>(words[0]), .y = std::bit_cast<float>(words[1]), .z = std::bit_cast<float>(words[2])};
            const Vector3i rotation{
                .x = std::bit_cast<std::int32_t>(words[3]), .y = std::bit_cast<std::int32_t>(words[4]), .z = std::bit_cast<std::int32_t>(words[5])};
            EV_AT_SetPos(actor, EventTransformFlags::AllAxes, position);
            EV_AT_SetAngY(actor, EventTransformFlags::AllAxes, rotation);
        }
    }
    if (frame == 150)
        AttachDroppedBook(SHEN_NAME(RZOU), {.x = .108999997f, .y = .072999999f, .z = -.17f}, {.x = 3018, .y = 0, .z = 60220});
    if (frame == 170) {
        ResetCharaFixo(FindCharacterByScenarioName(SHEN_NAME(RZOU)));
        QueueDroppedBookMovement(SHEN_NAME(RZOU), {.x = -.528599977f, .y = .251399994f, .z = -.5298f}, {.x = 13848, .y = 32768, .z = 16384}, 8);
    }
    if (frame == 192)
        AttachDroppedBook(SHEN_NAME(SZOU), {.x = .128999993f, .y = .0480000004f, .z = -.109f}, {.x = 58980, .y = 29855, .z = 57526});
    if (frame == 214) {
        ResetCharaFixo(FindCharacterByScenarioName(SHEN_NAME(SZOU)));
        QueueDroppedBookMovement(SHEN_NAME(SZOU), {.x = -.50400001f, .y = .28310001f, .z = -.5284f}, {.x = 49152, .y = 0, .z = 16384}, 5);
    }
    if (frame == 240) {
        AttachDroppedBook(SHEN_NAME(TZOU), {.x = .153999999f, .y = .0160000008f, .z = -.12899999f}, {.x = 47331, .y = 30037, .z = 61531});
        UpdateMotionWorkWithPosition(scenFindMotionwork(BOOK_SAI), nullptr, 0);
    }
    if (frame == 299)
        ResetCharaFixo(FindCharacterByScenarioName(SHEN_NAME(TZOU)));
    if (frame == 260) {
        Vector3 position;
        Vector3i rotation;
        EV_GetCharaPosition(bookPlayerAnchor, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        GetScenarioCharacterEulerAngles(bookPlayerAnchor, &rotation, EventTransformFlags::World);
        Vector3 offset{.x = -.028999999f, .y = .49000001f, .z = -.37799999f};
        ApplyScenarioTranslationAndRotation(&position, &rotation, &offset);
        const Vector3 volume{.x = .1f, .y = .1f, .z = .1f};
        HLib::Task *task = HLib::EnqueueTaskWithoutParameter(BookSmokeStartCallback, 6, 0x0d, static_cast<std::int32_t>(BOOK_TASK.name));
        HLib::SetupTimedTask(task, 5);
        SetParticleEffectPosition(0, bookSmokeEffect, &offset, &volume);
        SetParticleEffectBirthRate(0, bookSmokeEffect, 0);
    }
    if (frame == 90)
        SetMotionRegionEvalTime(scenFindMotionwork(BOOK_RYO), 250, MotionRegion::LowerBody);
    if (frame == 270)
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
    if (frame >= 180) {
        Vector3 position;
        EV_GetCharaPosition(BOOK_RYO, 10, &position, EventTransformFlags::World);
        position.y -= .5f;
        UpdateMotionWorkWithPosition(scenFindMotionwork(BOOK_RYO), &position, 0);
    }
    if (frame == 330)
        FinishDroppedBooks();
    frame = bookModelState.animationFrame;
    if (frame <= 330 && frame >= 270) {
        bookModelState.animationFrame = frame + 1;
        return;
    }
    if (frame > 330) {
        EV_EnsureCharacterDraw(BOOK_BASE);
        bookModelState.animationFrame = 0;
    }
    const bool cancel = HasPressedCancel();
    ++bookModelState.animationFrame;
    if (cancel)
        bookDropRecovery = 1;
}
} // namespace
void UpdateBookCarryTask() {
    const bool dispatched = !bookCarryPhase && bookModelState.currentBook > 0;
    if (!bookCarryPhase) {
        bookFaceControl.player = 0;
        bookFaceControl.companion = 0;
        if (bookModelState.currentBook > 0)
            UpdateBookCarryLoop();
        else {
            Vector3 position;
            EV_GetCharaPosition(BOOK_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
            position.y += 1.5f;
            UpdateMotionWorkWithPosition(scenFindMotionwork(BOOK_SAI), &position, 1);
        }
    }
    if (!dispatched) {
        if (bookCarryPhase == 3) {
            bookFaceControl.companion = 1;
            bookFaceControl.player = 4;
            UpdateBookCarryLoop();
        } else if (bookCarryPhase == 8) {
            bookFaceControl.player = 14;
            bookFaceControl.companion = 3;
        } else if ((bookCarryPhase & 0xfd) == 0)
            UpdateBookCarryLoop();
    }
    if (static_cast<std::uint8_t>(bookCarryPhase) <= 8 && (317 & (1 << bookCarryPhase)))
        UpdateDroppedBookDisplay();
    if (bookCarryPhase == 8) {
        Vector3 position;
        SampleBookCarryTrack(bookCarryTrack, position, bookCarryDistance * .125f, 0);
        GetBattleDataConfig(0, SHEN_NAME(BTBA));
        if (GetBattleDataConfig(2, SHEN_NAME(BTBA))) {
            bookCarryPhase = 1;
            if (bookDesiredCamera != bookCurrentCamera) {
                bookCurrentCamera = bookDesiredCamera;
                EVS_LoopCamera(bookDesiredCamera, bookCameraAnchor, bookCameraAnchor);
            }
            return;
        }
    }
    if ((bookCarryPhase & 0xf7) == 0 && IsBookTimeExpired()) {
        EV_DisableCharacterDisplay(bookModelState.stacks[bookModelState.currentStack + 1]);
        SetMotionSpeed(scenFindMotionwork(BOOK_RYO), 0, MotionRegion::LowerBody);
        return;
    }
    if (bookCarryPhase == 5)
        BeginDroppedBooks();
    if (bookCarryPhase == 6)
        RecoverDroppedBooks();
    if (bookDesiredCamera != bookCurrentCamera) {
        bookCurrentCamera = bookDesiredCamera;
        EVS_LoopCamera(bookDesiredCamera, bookCameraAnchor, bookCameraAnchor);
    }
}
} // namespace shenmue::s2::areas
