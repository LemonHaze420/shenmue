#include "s2/colour_bar_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/file_loader_dc.hpp"
#include "s2/hlib.hpp"
#include "s2/input_dc.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/texture_dc.hpp"

namespace shenmue {
namespace s2 {

namespace {

extern "C" void njSetBackColor(std::uint32_t first, std::uint32_t second, std::uint32_t third);

// @verified - d
void ColourBarCallback(void *taskData) {
    ColourBarWork &work = *static_cast<ColourBarWork *>(taskData);
    if (g_inputBuffer[0].pressedPriority & 0x40)
        ++work.selection;
    else if (g_inputBuffer[0].pressedPriority & 0x80)
        --work.selection;
    work.selection &= 1;
    DrawLegacySpritePacket(work.packet, work.textures[work.selection], -5.0f);
}

// @verified - d
void ColourBarBackground(void *) {
    njSetBackColor(0xff00af00, 0xffaf0000, 0xff0000af);
}

// @verified - d
void InitializeColourBar(ColourBarWork &work) {
    INIT_LEGACY_SPRITE_PACKET(work.packet);
    AdjustScreenAspectRatio(work.packet.quad, HorizontalStretch, 320, 240, 1024, 512);
}

} // namespace

// @verified - d
void COLB_Module(void *) {
    HLib::SetCurrentTaskTo(ColourBarBackground);
    InitSceneEffects();
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(ColourBarCallback, 15, 0, sizeof(ColourBarWork), SHEN_NAME4('C', 'O', 'L', 'B'));
    ColourBarWork &work = *static_cast<ColourBarWork *>(HLib::GetTaskParameter(task));
    InitializeColourBar(work);
    void *const data = shenLoadFile("sprite", "COLBAR.SPR", SHEN_NAME4('C', 'O', 'L', 'B'));
    LoadTextureChunks(reinterpret_cast<const TextureChunk *>(data), work.textures);
    work.selection = 0;
    HLib::FreeTaskData(data);
}

} // namespace s2
} // namespace shenmue

#endif
