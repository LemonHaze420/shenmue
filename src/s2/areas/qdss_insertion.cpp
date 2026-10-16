#include "s2/areas/area_qdss.hpp"
#include "s2/character.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/search.hpp"
#include "s2/sound_program.hpp"
namespace shenmue::s2::areas {
std::int32_t RunQdssInsertion(QdssWork &work) {
    if (EV_GetShenmueFlag(0x3c9))
        EV_stepSetFlags(0, 0x3c9);
    srchSetLookMode();
    PrepareQdssHands(Vector3i{8736, 4004, 0}, Vector3i{0, 0, 0});
    EV_SetRefBoardParamsFor(QDSS_RYO, 1);
    EV_SetRefBoardParamsFor(QDSS_REN, 1);
    std::int32_t inserted = 0;
    for (std::uint32_t bit = 1; bit <= 8; bit <<= 1)
        if (work.inserted & bit)
            ++inserted;
    EV_EnableCharacterDisplay(QDSS_RYO);
    EV_EnableCharacterDisplay(QDSS_REN);
    EV_SetCharacterAlphaEnabled(QDSS_REN, false);
    EV_DisableCharacterDisplay(QDSS_HAND);
    const QdssKey &key = work.keys[work.selected];
    EV_EnableCharacterDisplay(key.character);
    Loop3DSoundEffect(0xa7b, -.025f);
    const bool correct = work.hole == work.selected;
    EVT_AT_SetCam(1100, QDSS_BASE, QDSS_BASE, 0, 0, 0);
    EVT_AT_SetPos(QDSS_RYO, QDSS_BASE, Vector3{.0020000001f, 0, -.50999999f}, 0);
    EVT_AT_SetAngY(QDSS_RYO, QDSS_BASE, 0, 0);
    EVT_AT_SetMot(QDSS_RYO, correct ? 10572 : 10573, 0, 1, 0x40000000U, 0);
    const Vector3 handPosition{.213f, .037999999f, -.025f};
    const Vector3i handRotation{48605, 5825, 50244};
    QueueQdssCling(key.character, QDSS_RYO, 14, &handPosition, &handRotation, 0, 0);
    EVT_AT_SetPos(QDSS_REN, QDSS_BASE, Vector3{6.8899999f, 0, -.38f}, 0);
    EVT_AT_SetAngY(QDSS_REN, QDSS_BASE, 0, 0);
    EVT_AT_SetMot(QDSS_REN, correct ? 10563 : 10564, 0, 1, 0x40000000U, 0);
    if (correct) {
        EVT_AT_SetSound(0xa7d, 70);
        EVT_AT_SetCam(1110, QDSS_BASE, QDSS_BASE, 0, 0, 130);
        QueueQdssCling(key.character, shen_name{0}, 0, nullptr, nullptr, 1, 129);
        Vector3 position = key.position;
        position.z += -.14999999f;
        EVT_AT_SetPos(key.character, QDSS_BASE, position, 130);
        EVT_AT_SetAng_Enqueue(key.character, QDSS_BASE, 0, static_cast<float>(key.rotation.y) / 182.04445f, -45, 130);
        AT_SetLocalBaseChara(key.character, QDSS_INVALID);
        EVT_AT_SetVoice(shenGetRandomInt32(2) ? "0247A149" : "0247A148", 130); // @text 0247A149 EN: "Good. It fits." | JA: "よし、入った。" ; 0247A148 EN: "Good." | JA: "よし…"
        if (!inserted) {
            QdssFade(30, true, 210);
            return 240;
        }
        EVT_AT_SetVoice(shenGetRandomInt32(2) ? "0247A151" : "0247A150", 210); // @text 0247A151 EN: "This was the right one." | JA: "この棒で合っていたみたいだ。" ; 0247A150 EN: "Heard the key click." | JA: "カギがはずれたような音がしたな。"
        EVT_AT_SetCam(1120, QDSS_BASE, QDSS_BASE, 0, 0, 300);
        EVT_AT_SetCam(1130, QDSS_BASE, QDSS_BASE, 0, 0, 370);
        if (inserted == 1)
            EVT_AT_SetVoice("0247A152", 370); // @text EN: "Three more to go." | JA: "あと３本入れるんだな。"
        else if (inserted == 2)
            EVT_AT_SetVoice("0247A153", 370); // @text EN: "Two more." | JA: "あと２本か…"
        else if (inserted == 3)
            EVT_AT_SetVoice("0247A154", 370); // @text EN: "One more." | JA: "あと１本だ。"
        return 470;
    }
    EVT_AT_SetSound(0xa7c, 218);
    if (work.selected > 3)
        EVT_AT_SetVoice(shenGetRandomInt32(2) ? "0247A157" : "0247A156", 240); // @text 0247A157 EN: "Wrong insert?" | JA: "この棒じゃないか。" ; 0247A156 EN: "Wrong hole?" | JA: "この棒は違うみたいだ。"
    else
        EVT_AT_SetVoice(shenGetRandomInt32(2) ? "0247A195" : "0247A194", 240); // @text 0247A195 EN: "Wrong insert?" | JA: "この穴じゃないのか。" ; 0247A194 EN: "Wrong hole?" | JA: "入れる穴をまちがえたか。"
    EVT_AT_SetCam(1140, QDSS_BASE, QDSS_BASE, 0, 0, 300);
    EVT_AT_SetCam(1150, QDSS_BASE, QDSS_BASE, 0, 0, 420);
    const std::int32_t choice = shenGetRandomInt32(3);
    EVT_AT_SetVoice(choice == 0 ? "0247B053" : choice == 1 ? "0247B054" : "0247B055", 300); // @text 0247B053 EN: "What are you doing, blockhead!?" | JA: "なにやってんだ。マヌケ！" ; 0247B054 EN: "Is it gonna take all day?" | JA: "…ったく。日が暮れちまうぜ。" ; 0247B055 EN: "Can't wait forever." | JA: "あーあ…つき合ってらんね。"
    EVT_AT_SetVoice(choice == 0 ? "0247A158" : choice == 1 ? "0247A159" : "0247A160", 420); // @text 0247A158 EN: "Shut up." | JA: "うるさい。" ; 0247A159 EN: "Why don't you keep quiet." | JA: "少し黙ってろ。" ; 0247A160 EN: "Don't have to." | JA: "そうかよ。"
    QueueQdssCling(key.character, shen_name{0}, 0, nullptr, nullptr, 1, 559);
    return 560;
}
void QdssFinishInsertion(QdssWork &work) {
    const QdssKey &key = work.keys[work.selected];
    QueueQdssCling(key.character, shen_name{0}, 0, nullptr, nullptr, 1, 0);
    if (work.hole == work.selected && (work.inserted & 15U) != 15U) {
        Vector3 position = key.position;
        position.z += -.14999999f;
        EVT_AT_SetPos(key.character, QDSS_BASE, position, 0);
        const std::int32_t yaw = TruncateToInt32(((static_cast<float>(key.rotation.y) / 182.04445f) * 8192.0f) / 45.0f);
        const Vector3i rotation{0, yaw, 57344};
        const shen_name previous = EV_SCEN_FindChar_Wrapper(key.character);
        AT_SetLocalBaseChara(key.character, QDSS_BASE);
        EV_AT_SetAngY(key.character, EventTransformFlags::AllAxes, rotation);
        AT_SetLocalBaseChara(key.character, previous);
        AT_SetLocalBaseChara(key.character, QDSS_INVALID);
    }
}
} // namespace shenmue::s2::areas
