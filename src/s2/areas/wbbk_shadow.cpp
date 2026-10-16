#include "s2/areas/wbbk_shadow.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/mt7.hpp"
#include "s2/pak.hpp"
#include <new>

namespace shenmue::s2::areas {
namespace {
struct BookShadowReader {
    MT7Model model;
    void *source;
};
BookShadowReader *bookShadowReader;
} // namespace
void LoadBookShadowReader() {
    std::uint32_t size;
    void *const source = LoadLegacySceneFile("misc", "SYD_BOOK.MT7", SHEN_NAME(SVBK), &size);
    BookShadowReader *const reader = new (std::nothrow) BookShadowReader{};
    bookShadowReader = reader;
    if (!reader)
        return;
    reader->source = source;
    if (!ReadModelFormat7({static_cast<const std::byte *>(source), size}, "misc/SYD_BOOK.MT7", reader->model, 0)) {
        delete reader;
        bookShadowReader = nullptr;
    }
}
void ReleaseBookShadowReader() {
    if (!bookShadowReader)
        return;
    ReleaseModelFormat7(bookShadowReader->model);
    HLib::FreeTaskData(bookShadowReader->source);
    delete bookShadowReader;
    bookShadowReader = nullptr;
}
void BindBookStackShadow(shen_name actor) {
    SetCharacterAuxiliaryData(FindCharacterByScenarioName(actor), &bookShadowReader->model);
    SetCharacterUnknownFlag(FindCharacterByScenarioName(actor), 1);
    SetScenarioCharacterType(actor, CharacterTypeStencilShadow);
    SetScenarioCharacterDrawControl(actor, 1, 0);
}
} // namespace shenmue::s2::areas
