#include "s2/debug.hpp"
#include "s2/area_event.hpp"
#if SHENMUE_IS_DC
#include "s2/archive_data.hpp"
#endif
#include "s2/scene_data.hpp"
#include "s2/area_jump.hpp"
#include "s2/date_time.hpp"
#include "s2/door.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/field_collision.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"

#include "s2/areas/area_0001.hpp"
#include "s2/areas/area_game.hpp"
#include "s2/areas/area_titl.hpp"
#include "s2/areas/area_omak.hpp"
#include "s2/areas/area_pawn.hpp"
#include "s2/areas/area_qdss.hpp"
#include "s2/areas/area_ro10.hpp"
#include "s2/areas/area_uede.hpp"
#include "s2/areas/area_gabt.hpp"
#include "s2/areas/area_knmn.hpp"
#include "s2/areas/area_knw0.hpp"
#include "s2/areas/area_ycpa.hpp"
#include "s2/areas/area_ksht.hpp"
#include "s2/areas/area_maki.hpp"
#include "s2/areas/area_wbbk.hpp"
#include "s2/areas/area_ymqb.hpp"
#include "s2/areas/area_qpam.hpp"
#include "s2/areas/area_qrdk.hpp"
#include "s2/areas/area_sysp.hpp"
#include "s2/areas/area_dart.hpp"
#include "s2/areas/area_sfkm.hpp"
#include "s2/areas/area_sflu.hpp"
#include "s2/areas/area_sfm2.hpp"
#include "s2/areas/area_sfs2.hpp"
#include "s2/areas/area_sfsr.hpp"
#include "s2/areas/area_sftc.hpp"
#include "s2/areas/area_sfts.hpp"
#include "s2/areas/area_sfwf.hpp"
#include "s2/areas/area_sfiz.hpp"
#include "s2/areas/area_sfbr.hpp"
#include "s2/areas/area_hasi.hpp"
#include "s2/areas/area_qdh.hpp"
#include "s2/areas/area_qdtb.hpp"
#include "s2/areas/area_qdtg.hpp"
#include "s2/areas/area_kew0.hpp"
#include "s2/areas/area_qfeb.hpp"
#include "s2/areas/area_qes1.hpp"
#include "s2/areas/area_qes2.hpp"
#include "s2/areas/area_qes3.hpp"
#include "s2/areas/area_qdmr.hpp"
#include "s2/areas/area_urni.hpp"
#include "s2/areas/area_0004.hpp"
#include "s2/areas/area_0005.hpp"
#include "s2/areas/area_0209.hpp"
#include "s2/areas/area_ar01.hpp"
#include "s2/areas/area_arsf.hpp"
#include "s2/areas/area_kww1.hpp"
#include "s2/areas/area_kwm1.hpp"
#include "s2/areas/area_aka3.hpp"
#include "s2/areas/area_aky0.hpp"
#include "s2/areas/area_kwwq.hpp"
#include "s2/areas/area_wsg1.hpp"
#include "s2/areas/area_arm0.hpp"
#include "s2/areas/area_arz0.hpp"
#include "s2/areas/area_wet0.hpp"
#include "s2/areas/area_wka0.hpp"
#include "s2/areas/area_qaw6.hpp"
#include "s2/areas/area_qc06.hpp"
#include "s2/areas/area_krk4.hpp"
#include "s2/areas/area_qae1.hpp"
#include "s2/areas/area_qae6.hpp"
#include "s2/areas/area_qaw1.hpp"
#include "s2/areas/area_qdkj.hpp"
#include "s2/areas/area_wecf.hpp"
#include "s2/areas/area_qe01.hpp"
#include "s2/areas/area_qe06.hpp"
#include "s2/areas/area_qedj.hpp"
#include "s2/areas/area_qfrr.hpp"
#include "s2/areas/area_qaxx.hpp"
#include "s2/areas/area_we00.hpp"
#include "s2/areas/area_qb00.hpp"
#include "s2/areas/area_arc0.hpp"
#include "s2/areas/area_qf06.hpp"
#include "s2/areas/area_qtb4.hpp"
#include "s2/areas/area_qf39.hpp"
#include "s2/areas/area_qug0.hpp"
#include "s2/areas/area_chime.hpp"
#include "s2/event_runtime.hpp"
#include "s2/qkqb_event.hpp"
#include "s2/tglb_event.hpp"
#include "s2/step_event.hpp"
#include "s2/dai2_event.hpp"
#include "s2/geto_event.hpp"
#include "s2/anbd_event.hpp"
#include "s2/wnpc_event.hpp"
#include "s2/krht_event.hpp"
#include "s2/hide_event.hpp"
#include "s2/nigt_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/author_viewer_pair_event.hpp"
#include "s2/snapshot_viewer.hpp"
#include "s2/areas/area_vend.hpp"
#include "s2/areas/area_q7qb.hpp"
#include "s2/areas/area_frbt.hpp"
#include "s2/areas/area_brib.hpp"
#include "s2/areas/area_arkq.hpp"
#include "s2/areas/area_9kan.hpp"
#include "s2/areas/area_arbt.hpp"
#include "s2/areas/area_mqte.hpp"
#include "s2/areas/area_mdtp.hpp"
#include "s2/areas/area_bgsm.hpp"
#include "s2/areas/area_armw.hpp"
#include "s2/areas/area_qabt.hpp"
#include "s2/areas/area_bkw0.hpp"
#include "s2/areas/area_exqt.hpp"
#include "s2/areas/area_dgct.hpp"
#include "s2/areas/area_khin.hpp"
#include "s2/areas/area_qlgt.hpp"
#include "s2/areas/area_pati.hpp"
#include "s2/areas/area_anan.hpp"
#include "s2/areas/area_cb00.hpp"
#include "s2/areas/area_etrk.hpp"
#include "s2/areas/area_gach.hpp"
#include "s2/areas/area_kach.hpp"
#include "s2/areas/area_slot.hpp"
#include "s2/areas/area_slt1.hpp"
#include "s2/areas/area_kesa.hpp"
#include "s2/areas/area_krct.hpp"
#include "s2/areas/area_chaw.hpp"
#include "s2/scripts/cwon/area_cwon.hpp"
#include "s2/areas/area_pvsj.hpp"
#include "s2/areas/area_tejo.hpp"
#include "s2/areas/area_toma.hpp"
#include "s2/areas/area_syw0.hpp"
#include "s2/areas/area_ytqb.hpp"
#include "s2/areas/area_mfw0.hpp"
#include "s2/areas/area_0266.hpp"
#include "s2/areas/area_0271.hpp"
#include "s2/areas/area_0273.hpp"
#include "s2/areas/area_0274.hpp"
#include "s2/areas/area_0275.hpp"
#include "s2/areas/area_0276.hpp"
#include "s2/areas/area_0277.hpp"
#include "s2/areas/area_0278.hpp"
#include "s2/areas/area_0282.hpp"
#include "s2/areas/area_0289.hpp"
#include "s2/areas/area_028e.hpp"
#include "s2/areas/area_0290.hpp"
#include "s2/areas/area_0291.hpp"
#include "s2/areas/area_0297.hpp"
#include "s2/areas/area_0307.hpp"
#include "s2/areas/area_0308.hpp"
#include "s2/areas/area_0309.hpp"
#include "s2/areas/area_031e.hpp"
#include "s2/areas/area_0324.hpp"
#include "s2/areas/area_0337.hpp"
#include "s2/areas/area_0339.hpp"
#include "s2/areas/area_0340.hpp"
#include "s2/areas/area_0361.hpp"
#include "s2/areas/area_0362.hpp"
#include "s2/areas/area_0364.hpp"
#include "s2/areas/area_039e.hpp"
#include "s2/areas/area_0404.hpp"
#include "s2/areas/area_048e.hpp"
#include "s2/areas/area_0500.hpp"
#include "s2/areas/area_0502.hpp"
#include "s2/areas/area_0504.hpp"
#include "s2/areas/area_0505.hpp"
#include "s2/areas/area_0507.hpp"
#include "s2/areas/area_0509.hpp"
#include "s2/areas/area_0510.hpp"
#include "s2/areas/area_0512.hpp"
#include "s2/areas/area_0514.hpp"
#include "s2/areas/area_0516_event.hpp"
#include "s2/areas/area_0517.hpp"
#include "s2/areas/area_0518.hpp"
#include "s2/areas/area_0519.hpp"
#include "s2/areas/area_0521.hpp"
#include "s2/areas/area_0523.hpp"
#include "s2/areas/area_0524.hpp"
#include "s2/areas/area_0525.hpp"
#include "s2/areas/area_0527.hpp"
#include "s2/areas/area_0528.hpp"
#include "s2/areas/area_0529.hpp"
#include "s2/areas/area_0530.hpp"
#include "s2/areas/area_0531.hpp"
#include "s2/areas/area_0537.hpp"
#include "s2/areas/area_0538.hpp"
#include "s2/areas/area_053e.hpp"
#include "s2/areas/area_0540.hpp"
#include "s2/areas/area_0541.hpp"
#include "s2/areas/area_0543.hpp"
#include "s2/areas/area_0545.hpp"
#include "s2/areas/area_0547.hpp"
#include "s2/areas/area_0549.hpp"
#include "s2/areas/area_054e.hpp"
#include "s2/areas/area_063e.hpp"
#include "s2/areas/area_122e.hpp"
#include "s2/areas/area_188e.hpp"
#include "s2/areas/area_192e.hpp"
#include "s2/areas/area_194e.hpp"
#include "s2/areas/area_202e.hpp"
#include "s2/areas/area_234e.hpp"
#include "s2/areas/area_235e.hpp"
#include "s2/areas/area_246e.hpp"
#include "s2/areas/area_253e.hpp"
#include "s2/areas/area_275e.hpp"
#include "s2/areas/area_280e.hpp"
#include "s2/areas/area_282e.hpp"
#include "s2/areas/area_284e.hpp"
#include "s2/areas/area_290e.hpp"
#include "s2/areas/area_301e.hpp"
#include "s2/areas/area_302e.hpp"
#include "s2/areas/area_303e.hpp"
#include "s2/areas/area_329e.hpp"
#include "s2/areas/area_347e.hpp"
#include "s2/areas/area_527e.hpp"
#include "s2/areas/area_528e.hpp"
#include "s2/areas/area_529e.hpp"
#include "s2/areas/area_545e.hpp"
#include "s2/areas/area_552e.hpp"
#include "s2/areas/area_wbsr.hpp"
#include "s2/areas/area_wkta.hpp"
#include "s2/areas/area_wrsq.hpp"
#include "s2/areas/area_wssf.hpp"
#include "s2/areas/area_wtkq.hpp"
#include "s2/areas/area_sfjf.hpp"
#include "s2/areas/area_aktm.hpp"
#include "s2/areas/area_aozr.hpp"
#include "s2/areas/area_apa0.hpp"
#include "s2/areas/area_bjmp.hpp"
#include "s2/areas/area_bok2.hpp"
#include "s2/areas/area_bosy.hpp"
#include "s2/areas/area_btel.hpp"
#include "s2/areas/area_istb.hpp"
#include "s2/areas/area_kick.hpp"
#include "s2/areas/area_koro.hpp"
#include "s2/areas/area_duck.hpp"
#include "s2/areas/area_ablb.hpp"
#include "s2/areas/area_b102.hpp"
#include "s2/areas/area_bell.hpp"
#include "s2/areas/area_bt40.hpp"
#include "s2/areas/area_dabo.hpp"
#include "s2/areas/area_elevator_event.hpp"
#include "s2/areas/area_gate.hpp"
#include "s2/areas/area_hpam.hpp"
#include "s2/areas/area_ahqb.hpp"
#include "s2/areas/area_cret.hpp"
#include "s2/areas/area_crti.hpp"
#include "s2/areas/area_find.hpp"
#include "s2/areas/area_gais.hpp"
#include "s2/areas/area_jiag.hpp"
#include "s2/areas/area_jukb.hpp"
#include "s2/areas/area_kami.hpp"
#include "s2/areas/area_kbrg.hpp"
#include "s2/areas/area_kebi.hpp"
#include "s2/areas/area_kest.hpp"
#include "s2/areas/area_kwwt.hpp"
#include "s2/areas/area_lock.hpp"
#include "s2/areas/area_okjo.hpp"
#include "s2/areas/area_op02.hpp"
#include "s2/areas/area_pach.hpp"
#include "s2/areas/area_qdfo.hpp"
#include "s2/areas/area_qehi.hpp"
#include "s2/areas/area_qesc.hpp"
#include "s2/areas/area_qyss.hpp"
#include "s2/areas/area_qyua.hpp"
#include "s2/areas/area_rati.hpp"
#include "s2/areas/area_scmn.hpp"
#include "s2/areas/area_tes2.hpp"
#include "s2/areas/area_tgbt.hpp"
#include "s2/areas/area_tofb.hpp"
#include "s2/areas/area_topa.hpp"
#include "s2/areas/area_ttrp.hpp"
#include "s2/areas/area_vsyb.hpp"
#include "s2/areas/area_wbbr.hpp"
#include "s2/areas/area_wonc.hpp"
#include "s2/areas/area_wsye.hpp"
#include "s2/areas/area_yoru.hpp"
#include "s2/areas/area_yuku.hpp"
#include "s2/areas/area_4bil.hpp"
#include "s2/areas/area_4btl.hpp"
#include "s2/areas/area_0007.hpp"
#include "s2/areas/area_0008.hpp"
#include "s2/areas/area_0012.hpp"
#include "s2/areas/area_0013.hpp"
#include "s2/areas/area_0016.hpp"
#include "s2/areas/area_0028.hpp"
#include "s2/areas/area_0030.hpp"
#include "s2/areas/area_0031.hpp"
#include "s2/areas/area_0032.hpp"
#include "s2/areas/area_0033.hpp"
#include "s2/areas/area_0034.hpp"
#include "s2/areas/area_0035.hpp"
#include "s2/areas/area_0036.hpp"
#include "s2/areas/area_0037.hpp"
#include "s2/areas/area_0038.hpp"
#include "s2/areas/area_0039.hpp"
#include "s2/areas/area_0045.hpp"
#include "s2/areas/area_0047.hpp"
#include "s2/areas/area_0048.hpp"
#include "s2/areas/area_0049.hpp"
#include "s2/areas/area_0050.hpp"
#include "s2/areas/area_0055.hpp"
#include "s2/areas/area_0056.hpp"
#include "s2/areas/area_0057.hpp"
#include "s2/areas/area_0058.hpp"
#include "s2/areas/area_0059.hpp"
#include "s2/areas/area_0062.hpp"
#include "s2/areas/area_0063.hpp"
#include "s2/areas/area_0064.hpp"
#include "s2/areas/area_0066.hpp"
#include "s2/areas/area_0067.hpp"
#include "s2/areas/area_0068.hpp"
#include "s2/areas/area_0069.hpp"
#include "s2/areas/area_0076.hpp"
#include "s2/areas/area_0077.hpp"
#include "s2/areas/area_0080.hpp"
#include "s2/areas/area_011e.hpp"
#include "s2/areas/area_0138.hpp"
#include "s2/areas/area_0139.hpp"
#include "s2/areas/area_0140.hpp"
#include "s2/areas/area_0141.hpp"
#include "s2/areas/area_0142.hpp"
#include "s2/areas/area_0143.hpp"
#include "s2/areas/area_0146.hpp"
#include "s2/areas/area_0151.hpp"
#include "s2/areas/area_0173.hpp"
#include "s2/areas/area_0175.hpp"
#include "s2/areas/area_0184.hpp"
#include "s2/areas/area_0186.hpp"
#include "s2/areas/area_0200.hpp"
#include "s2/areas/area_0201.hpp"
#include "s2/areas/area_0202.hpp"
#include "s2/areas/area_0204.hpp"
#include "s2/areas/area_0207.hpp"
#include "s2/areas/area_020e.hpp"
#include "s2/areas/area_0210.hpp"
#include "s2/areas/area_0213.hpp"
#include "s2/areas/area_0214.hpp"
#include "s2/areas/area_0215.hpp"
#include "s2/areas/area_0216.hpp"
#include "s2/areas/area_0217.hpp"
#include "s2/areas/area_0218.hpp"
#include "s2/areas/area_0221.hpp"
#include "s2/areas/area_0222.hpp"
#include "s2/areas/area_0225.hpp"
#include "s2/areas/area_0228.hpp"
#include "s2/areas/area_0232.hpp"
#include "s2/areas/area_0235.hpp"
#include "s2/areas/area_0236.hpp"
#include "s2/areas/area_0237.hpp"
#include "s2/areas/area_0240.hpp"
#include "s2/areas/area_0247.hpp"
#include "s2/areas/area_0248.hpp"
#include "s2/areas/area_0249.hpp"
#include "s2/areas/area_0250.hpp"
#include "s2/areas/area_0252.hpp"
#include "s2/areas/area_0256.hpp"
#include "s2/areas/area_0257.hpp"
#include "s2/areas/area_0258.hpp"
#include "s2/areas/area_0259.hpp"
#include "s2/areas/area_0260.hpp"
#include "s2/areas/area_0262.hpp"
#include "s2/areas/area_0264.hpp"
#include "s2/areas/area_0265.hpp"
#include "s2/areas/area_ab00.hpp"
#include "s2/areas/area_ak00.hpp"
#include "s2/areas/area_aks0.hpp"
#include "s2/areas/area_aks1.hpp"
#include "s2/areas/area_akt0.hpp"
#include "s2/areas/area_akt2.hpp"
#include "s2/areas/area_akt3.hpp"
#include "s2/areas/area_ar02.hpp"
#include "s2/areas/area_ar03.hpp"
#include "s2/areas/area_ara0.hpp"
#include "s2/areas/area_kes1.hpp"
#include "s2/areas/area_kmz1.hpp"
#include "s2/areas/area_kmz2.hpp"
#include "s2/areas/area_kmz3.hpp"
#include "s2/areas/area_kmz4.hpp"
#include "s2/areas/area_krc1.hpp"
#include "s2/areas/area_krh1.hpp"
#include "s2/areas/area_krk2.hpp"
#include "s2/areas/area_krm3.hpp"
#include "s2/areas/area_ksh1.hpp"
#include "s2/areas/area_kww4.hpp"
#include "s2/areas/area_q100.hpp"
#include "s2/areas/area_q200.hpp"
#include "s2/areas/area_q300.hpp"
#include "s2/areas/area_qa00.hpp"
#include "s2/areas/area_qa11.hpp"
#include "s2/areas/area_qa22.hpp"
#include "s2/areas/area_qc00.hpp"
#include "s2/areas/area_qc01.hpp"
#include "s2/areas/area_qcae.hpp"
#include "s2/areas/area_qd01.hpp"
#include "s2/areas/area_qe00.hpp"
#include "s2/areas/area_qf00.hpp"
#include "s2/areas/area_qf01.hpp"
#include "s2/areas/area_qf02.hpp"
#include "s2/areas/area_qf40.hpp"
#include "s2/areas/area_qgbt.hpp"
#include "s2/areas/area_qjbt.hpp"
#include "s2/areas/area_qkbt.hpp"
#include "s2/areas/area_qlbt.hpp"
#include "s2/areas/area_qr00.hpp"
#include "s2/areas/area_qrr0.hpp"
#include "s2/areas/area_qsfa.hpp"
#include "s2/areas/area_qsfb.hpp"
#include "s2/areas/area_qsfc.hpp"
#include "s2/areas/area_qsfd.hpp"
#include "s2/areas/area_qtb1.hpp"
#include "s2/areas/area_qtb2.hpp"
#include "s2/areas/area_qtb3.hpp"
#include "s2/areas/area_tbak.hpp"
#include "s2/areas/area_wb00.hpp"
#include "s2/areas/area_wb01.hpp"
#include "s2/areas/area_weg0.hpp"
#include "s2/areas/area_wem1.hpp"
#include "s2/areas/area_wes1.hpp"
#include "s2/areas/area_wesm.hpp"
#include "s2/areas/area_wk00.hpp"
#include "s2/areas/area_wn00.hpp"
#include "s2/areas/area_wr00.hpp"
#include "s2/areas/area_wrs2.hpp"
#include "s2/areas/area_ws00.hpp"
#include "s2/areas/area_wsy0.hpp"
#include "s2/areas/area_wt00.hpp"
#include "s2/areas/area_wta0.hpp"
#include "s2/areas/area_0009.hpp"
#include "s2/areas/area_0010.hpp"
#include "s2/areas/area_0014.hpp"
#include "s2/areas/area_0015.hpp"
#include "s2/areas/area_0017.hpp"
#include "s2/areas/area_0023.hpp"
#include "s2/areas/area_0025.hpp"
#include "s2/areas/area_0026.hpp"
#include "s2/areas/area_0027.hpp"
#include "s2/areas/area_0029.hpp"
#include "s2/areas/area_WBSS.hpp"
#include "s2/areas/area_WKKT.hpp"
#include "s2/areas/area_0061.hpp"
#include "s2/areas/area_krk6.hpp"
#include "s2/areas/area_qd00.hpp"
#include "s2/areas/area_4nin.hpp"
#include "s2/qt_nj_event.hpp"
#include "s2/areas/area_sfkk.hpp"
#include "s2/areas/area_qtcv.hpp"
#include "s2/areas/area_WB88.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace shenmue::s2 {

// @verified - p
void AreaBaseDummyCallback(std::uint32_t *parameters) {
    EV_debug_log("common\\et_areabase.c", 132, "EVENT areabase : call %d is dummy function\n", *parameters);
}

std::array<AreaBaseCallback, 10> areaBaseCallbacks{AreaBaseDummyCallback, AreaBaseDummyCallback, AreaBaseDummyCallback, AreaBaseDummyCallback,
    AreaBaseDummyCallback, AreaBaseDummyCallback, AreaBaseDummyCallback, AreaBaseDummyCallback, AreaBaseDummyCallback, AreaBaseDummyCallback};
std::uint32_t areaFlags{};
std::int32_t g_areaHeldItemRestorePending = 0;
namespace {

struct AreaSlot {
    std::array<char, 12> areaId;
    std::uint32_t reserved0c;
};
static_assert(sizeof(AreaSlot) == 0x10);
static_assert(offsetof(AreaSlot, reserved0c) == 0x0c);

std::array<AreaSlot, 4> areaSlots{};
#if !SHENMUE_IS_DC
std::array<ScriptAreaMainFunction, 4> currentAreaMainFunctions{};
#endif
HLib::Task *preAreaMainLoopTask = nullptr;
HLib::Task *areaEventEntryTask = nullptr;
std::uint32_t savedStartParameters = 0;
float areaInitFlag = 0.0f;
std::int32_t nightAuthorState = 0;
std::int32_t areaCleanupState = 0;
AreaTerminalMediaState terminalMediaState{};
HLib::Task *startupFadeTask = nullptr;
HLib::Task *nextStartupFadeTask = nullptr;

void EVT_PreAreaInit();

void ApplyPlayerEntryLocation(Character *character, const Vector3 &position, std::int32_t yaw) {
    using enum CameraState;
    shenApplyTranslationToAll(&character->entity.node, position);
    character->entity.node.rotation[0] = 0;
    character->entity.node.rotation[2] = 0;
    shenDecrementRotYAllNodes(&character->entity.node, static_cast<std::int16_t>(yaw));
    if (MotionWork *const motion = FindMotionWork(character))
        SetMotionEntityRotation(motion, Vector3i{0, yaw, 0});
    camCtrlSetState(Init);
    camCtrlSetFovTransitionTo(true);
}

void InitMapAndCharaFlags() {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = control ? static_cast<Character *>(control->ryo) : nullptr;
    if (!character)
        return;

    Vector3 position{};
    std::int32_t yaw{};

    const MapEventAreaDefinition &area = GetCurrentMapEventAreaDefinition();
    for (const MapEventTransition *entry = area.transitions; entry && entry->eventId != -1; ++entry) {
        if (entry->eventId != g_entryPoint)
            continue;
        position = entry->position;
        yaw = entry->yaw;
        MoveObjectToPosFromThisScene(SHEN_NAME(0000), g_entryPoint, position, yaw);
        ApplyPlayerEntryLocation(character, position, yaw);
        return;
    }
}

#if !SHENMUE_IS_DC
void scenInitEvent1() {

    areas::areaChimeLastSound = 0;
    EVT_PreAreaInit();
}

void scenInitEvent2() {

    EV_ResetFreeTalkFlags();
}

void scenInitEvent3() {
    ResetCompiledEventState();
}

constexpr std::array<AreaInitFunction, 3> areaPreInitCallbacks{scenInitEvent1, scenInitEvent2, scenInitEvent3};
#endif

void BeginNightTransition(HLib::TaskCallback callback);
void EVT_AreaMainLoop_Callback(void *) {
    shenUpdateFlagEvaluation(0);
    if (ryoNotBusy() && EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1006) == 0) {
        const bool night = EV_GetShenmueFlag(0xff) != 0;
        if (night && areaCleanupState == 1) {
            constexpr shen_name yoru{SHEN_NAME(YORU)};
            auto *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
            if (control && Author_HasMapInfoAuthor(control->mapInfo, yoru)) {
                BeginNightTransition(EVT_NightAuthor_Callback);
            } else if (g_currentMap.name == SHEN_NAME(WSG1)) {
                BeginNightTransition(EVT_NightChange_Callback);
            } else {
                ExecuteGameplayAreaCleanup(true);
                areaCleanupState = 0;
            }
        } else if (!night && areaCleanupState == 0 && !authGetLoaded()) {
            ExecuteGameplayAreaCleanup(false);
            areaCleanupState = 1;
        }
    }
    if (g_areaHeldItemRestorePending != 0 && !IsAreaItemSelectionActive() && !EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 966)) {
        EV_setHoldStatesFlags(1);
        g_areaHeldItemRestorePending = 0;
    }
}
void BeginNightTransition(HLib::TaskCallback callback) {
    SetPlayerControlEnabled(0);
    SetCurrentAreaControlLock(true);
    WaitForNextTask_0(GetCurrentAreaScreenFilmTask(), 1, 30);
    nightAuthorState = 0;
    areaCleanupState = 0;
    HLib::SetCurrentTaskTo(callback);
}

void EVT_PreAreaMainLoop_Callback(void *) {
    if ((areaFlags & AreaFlag_KeepPlayerControlLocked) == 0) {
        SetPlayerControlEnabled(1);
    }
    EV_SetCanDoAreaJumpTo(true);
    constexpr std::uint32_t kSkipFinishLoading = 0x8U;
    if ((GetLoadingPhaseFlags() & kSkipFinishLoading) == 0) {
        FinishLoading();
    }
    if (g_areaHeldItemRestorePending != 0) {
        EV_setHoldStatesFlags(0);
        g_areaHeldItemRestorePending = 1;
    }
    HLib::SetCurrentTaskTo(EVT_AreaMainLoop_Callback);
}

void EVT_PreAreaInit() {
    savedStartParameters = 0;
    areaFlags = 0;
    g_areaHeldItemRestorePending = 0;
    preAreaMainLoopTask = nullptr;
    areaInitFlag = 0.0f;
    nightAuthorState = 0;
    areaCleanupState = 0;
    terminalMediaState = {};
}

void EVT_PreAreaMainLoop(std::uintptr_t rawParameters) {
    if (rawParameters != 0) {
        std::int32_t *const parameters = reinterpret_cast<std::int32_t *>(rawParameters);
        if (*parameters > 10)
            EV_debug_log("common\\et_areabase.c", 180, "EVENT areabase : Illigal function number\n");
        else
            areaBaseCallbacks[*parameters - 1](reinterpret_cast<std::uint32_t *>(parameters + 1));
        return;
    }

    savedStartParameters = static_cast<std::uint32_t>(HasSavedAreaJumpLocation());
    areaFlags = 0;
    g_areaHeldItemRestorePending = static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 966));
    DateTime dateTime{};
    GetCurrentDateTime(dateTime);
    SetWeatherUpdateEnabledTo(1);
    RefreshWeatherForCurrentTime(0);
    EV_UpdateWeather();
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1006, 0);
    preAreaMainLoopTask = HLib::EnqueueTaskWithoutParameter(EVT_PreAreaMainLoop_Callback, 3, 0x0b, static_cast<std::int32_t>(SHEN_NAME(base)));
    areaInitFlag = 1.0f;
    areaCleanupState = static_cast<std::uint8_t>(dateTime.hour - 5) <= 13U ? 1 : 0;
    SelectPostFxBank(1, 1);
    UploadGlowParams(areaCleanupState == 0 ? nightGlowParameters : defaultGlowParameters);
    SFIL_UploadBloomParams(areaCleanupState == 0);
    ExecuteGameplayArea(savedStartParameters);
    if (savedStartParameters) {
        EV_ApplyTranslationClockVisibility(savedStartParameters);
    }
    if ((shenUpdateFlagEvaluation(1) & 1) != 0 && (areaFlags & AreaFlag_SuppressAutoControlLock) == 0) {
        areaFlags |= AreaFlag_KeepPlayerControlLocked;
    }
    if ((areaFlags & AreaFlag_DeferPreAreaMainLoop) == 0) {
        QueueImmediateNextFadeTask(preAreaMainLoopTask);
    }
}

#if !SHENMUE_IS_DC


constexpr char nativeScriptAreaIds[] = "0001000400050006000700080009001000120013001400150016001700230025"
                                       "0026002700280029003000310032003300340035003600370038003900450047"
                                       "0048004900500055005600570058005900610062006300640066006700680069"
                                       "007600770080011E01380139014001410142014301460151015E017301750184"
                                       "0186020002010202020402070209020E02100213021402150216021702180221"
                                       "0222022502280232023502360237024002470248024902500252025602570258"
                                       "025902600262026402650266027102730274027502760277027802820289028E"
                                       "029002910297030703080309031E0324033703390340035E036103620364039E"
                                       "0404048E05000502050405050507050905100512051405160517051805190521"
                                       "0523052405250527052805290530053105370538053E05400541054305450547"
                                       "0549054E061E063E081E122E182E188E192E194E202E234E235E246E253E275E"
                                       "280E282E284E290E295E301E302E303E329E332E347E4BIL4BTL4NIN503E517E"
                                       "519E527E528E529E545E552E9KANABLBAHQBAKTMANANANBDAOZRAPA0ARBTARKQ"
                                       "ARMWB102BELLBGSMBJMPBKW0BOK2BOSYBRIBBT40BTELCB00CHAWCRETCRTICWON"
                                       "DABODAI2DARTDGCTDUCKELV2ELVTETRKEXQTFINDFRBTGABTGACHGAISGAMEGATE"
                                       "GETOHASIHIDEHPAMISTBJIAGJUKBKACHKAMIKBRGKEBIKESAKESTKEW0KHINKICK"
                                       "KNMNKNW0KOROKRCTKRHTKSHTKWWTLOCKMAKIMDTPMFW0MQTENIGTOKJOOMAKOP02"
                                       "PACHPATIPAWNPVSJQ7QBQABTQDFOQDH1QDH2QDMRQDSSQDTBQDTGQEHIQES1QES2"
                                       "QES3QESCQFEBQKQBQLGTQPAMQRDKQTCVQTNJQYSSQYUARATIRO10SCMNSFBRSFIZ"
                                       "SFJFSFKKSFKMSFLUSFM2SFS2SFSRSFTCSFTSSFWFSLOTSLT1STEPSYSPSYW0TEJO"
                                       "TES2TGBTTGLBTITLTOFBTOMATOPATTRPUEDEURNIVENDVSYBWB88WBBKWBBRWBSR"
                                       "WBSSWKKTWKTAWNPCWONCWRSQWSSFWSYEWTKQYCPAYMQBYORUYTQBYUKUSTVW";
static_assert(sizeof(nativeScriptAreaIds) == 351 * 4 + 1);

bool IsNativeSpecializedArea(const char *areaId) {
    for (std::size_t offset = 0; offset < sizeof(nativeScriptAreaIds) - 1; offset += 4) {
        if (std::strncmp(areaId, nativeScriptAreaIds + offset, 4) == 0) {
            return true;
        }
    }
    return false;
}

constexpr std::array<const ScriptAreaDefinition *, 351> recoveredScriptAreas{{
    &areas::area0001ScriptDefinition,
    &areas::area0004ScriptDefinition,
    &areas::area0005ScriptDefinition,
    &areas::area0209ScriptDefinition,
    &qkqbScriptDefinition,
    &tglbScriptDefinition,
    &stepScriptDefinition,
    &dai2ScriptDefinition,
    &getoScriptDefinition,
    &anbdScriptDefinition,
    &wnpcScriptDefinition,
    &krhtScriptDefinition,
    &hideScriptDefinition,
    &nigtScriptDefinition,
    &authorViewer503EScriptDefinition,
    &authorViewer332EScriptDefinition,
    &authorViewer517EScriptDefinition,
    &authorViewer519EScriptDefinition,
    &authorViewer081EScriptDefinition,
    &authorViewer182EScriptDefinition,
    &authorViewer015EScriptDefinition,
    &authorViewer295EScriptDefinition,
    &authorViewer035EScriptDefinition,
    &authorViewer061EScriptDefinition,
    &snapshotViewerScriptDefinition,
    &vendingMachineScriptDefinition,
    &areas::q7qbScriptDefinition,
    &areas::frbtScriptDefinition,
    &areas::bribScriptDefinition,
    &areas::arkqScriptDefinition,
    &areas::area9KANScriptDefinition,
    &areas::arbtScriptDefinition,
    &areas::mqteScriptDefinition,
    &mdtpScriptDefinition,
    &areas::bgsmScriptDefinition,
    &areas::armwScriptDefinition,
    &areas::qabtScriptDefinition,
    &areas::bkw0ScriptDefinition,
    &areas::exqtScriptDefinition,
    &areas::dgctScriptDefinition,
    &khinScriptDefinition,
    &areas::areaQLGTScriptDefinition,
    &patiScriptDefinition,
    &areas::ananScriptDefinition,
    &cb00Definition,
    &areas::etrkScriptDefinition,
    &gachScriptDefinition,
    &kachScriptDefinition,
    &gameScriptDefinition,
    &areas::titlScriptDefinition,
    &areas::omakScriptDefinition,
    &areas::pawnScriptDefinition,
    &areas::qdssScriptDefinition,
    &areas::ro10ScriptDefinition,
    &areas::uedeScriptDefinition,
    &areas::gabtScriptDefinition,
    &areas::knmnScriptDefinition,
    &areas::knw0ScriptDefinition,
    &areas::ycpaScriptDefinition,
    &areas::kshtScriptDefinition,
    &areas::makiScriptDefinition,
    &areas::wbbkScriptDefinition,
    &slotScriptDefinition,
    &slt1ScriptDefinition,
    &areas::areaKESAScriptDefinition,
    &areas::areaKRCTScriptDefinition,
    &areaCHAWScriptDefinition,
    &areaCWONScriptDefinition,
    &areaPVSJScriptDefinition,
    &tejoScriptDefinition,
    &areas::tomaScriptDefinition,
    &areas::syw0ScriptDefinition,
    &areas::ytqbScriptDefinition,
    &areas::mfw0ScriptDefinition,
    &authorViewer0006ScriptDefinition,
    &areas::area0266ScriptDefinition,
    &areas::area0271ScriptDefinition,
    &areas::area0273ScriptDefinition,
    &areas::area0274ScriptDefinition,
    &areas::area0275ScriptDefinition,
    &areas::area0276ScriptDefinition,
    &areas::area0277ScriptDefinition,
    &areas::area0278ScriptDefinition,
    &areas::area0282ScriptDefinition,
    &areas::area0289ScriptDefinition,
    &areas::area028EScriptDefinition,
    &areas::area0290ScriptDefinition,
    &areas::area0291ScriptDefinition,
    &areas::area0297ScriptDefinition,
    &areas::area0307ScriptDefinition,
    &areas::area0308ScriptDefinition,
    &areas::area0309ScriptDefinition,
    &areas::area031EScriptDefinition,
    &areas::area0324ScriptDefinition,
    &areas::area0337ScriptDefinition,
    &areas::area0339ScriptDefinition,
    &areas::area0340ScriptDefinition,
    &areas::area0361ScriptDefinition,
    &areas::area0362ScriptDefinition,
    &areas::area0364ScriptDefinition,
    &areas::area039EScriptDefinition,
    &areas::area0404ScriptDefinition,
    &areas::area048EScriptDefinition,
    &areas::area0500ScriptDefinition,
    &areas::area0502ScriptDefinition,
    &areas::area0504ScriptDefinition,
    &areas::area0505ScriptDefinition,
    &areas::area0507ScriptDefinition,
    &areas::area0509ScriptDefinition,
    &areas::area0510ScriptDefinition,
    &areas::area0512ScriptDefinition,
    &areas::area0514ScriptDefinition,
    &areas::area0516ScriptDefinition,
    &areas::area0517ScriptDefinition,
    &areas::area0518ScriptDefinition,
    &areas::area0519ScriptDefinition,
    &areas::area0521ScriptDefinition,
    &areas::area0523ScriptDefinition,
    &areas::area0524ScriptDefinition,
    &areas::area0525ScriptDefinition,
    &areas::area0527ScriptDefinition,
    &areas::area0528ScriptDefinition,
    &areas::area0529ScriptDefinition,
    &areas::area0530ScriptDefinition,
    &areas::area0531ScriptDefinition,
    &areas::area0537ScriptDefinition,
    &areas::area0538ScriptDefinition,
    &areas::area053EScriptDefinition,
    &areas::area0540ScriptDefinition,
    &areas::area0541ScriptDefinition,
    &areas::area0543ScriptDefinition,
    &areas::area0545ScriptDefinition,
    &areas::area0547ScriptDefinition,
    &areas::area0549ScriptDefinition,
    &areas::area054EScriptDefinition,
    &areas::area063EScriptDefinition,
    &areas::area122EScriptDefinition,
    &areas::area188EScriptDefinition,
    &areas::area192EScriptDefinition,
    &areas::area194EScriptDefinition,
    &areas::area202EScriptDefinition,
    &areas::area234EScriptDefinition,
    &areas::area235EScriptDefinition,
    &areas::area246EScriptDefinition,
    &areas::area253EScriptDefinition,
    &areas::area275EScriptDefinition,
    &areas::area280EScriptDefinition,
    &areas::area282EScriptDefinition,
    &areas::area284EScriptDefinition,
    &areas::area290EScriptDefinition,
    &areas::area301EScriptDefinition,
    &areas::area302EScriptDefinition,
    &areas::area303EScriptDefinition,
    &areas::area329EScriptDefinition,
    &areas::area347EScriptDefinition,
    &areas::area527EScriptDefinition,
    &areas::area528EScriptDefinition,
    &areas::area529EScriptDefinition,
    &areas::area545EScriptDefinition,
    &areas::area552EScriptDefinition,
    &wbsrScriptDefinition,
    &wktaScriptDefinition,
    &wrsqScriptDefinition,
    &wssfScriptDefinition,
    &wtkqScriptDefinition,
    &areaSfjfScriptDefinition,
    &areas::areaAKTMScriptDefinition,
    &areas::areaAOZRScriptDefinition,
    &areas::areaAPA0ScriptDefinition,
    &areas::areaBJMPScriptDefinition,
    &areas::areaBOK2ScriptDefinition,
    &areas::areaBOSYScriptDefinition,
    &areas::areaBTELScriptDefinition,
    &areas::areaISTBScriptDefinition,
    &areas::areaKICKScriptDefinition,
    &areas::areaKOROScriptDefinition,
    &areas::areaDUCKScriptDefinition,
    &areaABLBScriptDefinition,
    &areaB102ScriptDefinition,
    &areaBELLScriptDefinition,
    &areaBT40ScriptDefinition,
    &areaDABOScriptDefinition,
    &areaELV2ScriptDefinition,
    &areaELVTScriptDefinition,
    &areaGATEScriptDefinition,
    &areaHPAMScriptDefinition,
    &areas::areaAHQBScriptDefinition,
    &areas::areaCRETScriptDefinition,
    &areas::areaCRTIScriptDefinition,
    &areas::areaFINDScriptDefinition,
    &areas::areaGAISScriptDefinition,
    &areas::areaJIAGScriptDefinition,
    &areas::areaJUKBScriptDefinition,
    &areas::areaKAMIScriptDefinition,
    &areas::areaKBRGScriptDefinition,
    &areas::areaKEBIScriptDefinition,
    &areas::areaKESTScriptDefinition,
    &areas::areaKWWTScriptDefinition,
    &areas::areaLOCKScriptDefinition,
    &areas::areaOKJOScriptDefinition,
    &areas::areaOP02ScriptDefinition,
    &areas::areaPACHScriptDefinition,
    &qdfoScriptDefinition,
    &qehiScriptDefinition,
    &qescScriptDefinition,
    &qyssScriptDefinition,
    &qyuaScriptDefinition,
    &ratiScriptDefinition,
    &scmnScriptDefinition,
    &tes2ScriptDefinition,
    &tgbtScriptDefinition,
    &tofbScriptDefinition,
    &topaScriptDefinition,
    &ttrpScriptDefinition,
    &vsybScriptDefinition,
    &wbbrScriptDefinition,
    &woncScriptDefinition,
    &wsyeScriptDefinition,
    &yoruScriptDefinition,
    &yukuScriptDefinition,
    &areas::area4BilScriptDefinition,
    &areas::area4BtlScriptDefinition,
    &areas::area0007ScriptDefinition,
    &areas::area0008ScriptDefinition,
    &areas::area0012ScriptDefinition,
    &areas::area0013ScriptDefinition,
    &areas::area0016ScriptDefinition,
    &areas::area0028ScriptDefinition,
    &areas::area0030ScriptDefinition,
    &areas::area0031ScriptDefinition,
    &areas::area0032ScriptDefinition,
    &areas::area0033ScriptDefinition,
    &areas::area0034ScriptDefinition,
    &areas::area0035ScriptDefinition,
    &areas::area0036ScriptDefinition,
    &areas::area0037ScriptDefinition,
    &areas::area0038ScriptDefinition,
    &areas::area0039ScriptDefinition,
    &areas::area0045ScriptDefinition,
    &areas::area0047ScriptDefinition,
    &areas::area0048ScriptDefinition,
    &areas::area0049ScriptDefinition,
    &areas::area0050ScriptDefinition,
    &areas::area0055ScriptDefinition,
    &areas::area0056ScriptDefinition,
    &areas::area0057ScriptDefinition,
    &areas::area0058ScriptDefinition,
    &areas::area0059ScriptDefinition,
    &areas::area0062ScriptDefinition,
    &areas::area0063ScriptDefinition,
    &areas::area0064ScriptDefinition,
    &areas::area0066ScriptDefinition,
    &areas::area0067ScriptDefinition,
    &areas::area0068ScriptDefinition,
    &areas::area0069ScriptDefinition,
    &areas::area0076ScriptDefinition,
    &areas::area0077ScriptDefinition,
    &areas::area0080ScriptDefinition,
    &areas::area011EScriptDefinition,
    &areas::area0138ScriptDefinition,
    &areas::area0139ScriptDefinition,
    &areas::area0140ScriptDefinition,
    &areas::area0141ScriptDefinition,
    &areas::area0142ScriptDefinition,
    &areas::area0143ScriptDefinition,
    &areas::area0146ScriptDefinition,
    &areas::area0151ScriptDefinition,
    &areas::area0173ScriptDefinition,
    &areas::area0175ScriptDefinition,
    &areas::area0184ScriptDefinition,
    &areas::area0186ScriptDefinition,
    &areas::area0200ScriptDefinition,
    &areas::area0201ScriptDefinition,
    &areas::area0202ScriptDefinition,
    &areas::area0204ScriptDefinition,
    &areas::area0207ScriptDefinition,
    &areas::area020EScriptDefinition,
    &areas::area0210ScriptDefinition,
    &areas::area0213ScriptDefinition,
    &areas::area0214ScriptDefinition,
    &areas::area0215ScriptDefinition,
    &areas::area0216ScriptDefinition,
    &areas::area0217ScriptDefinition,
    &areas::area0218ScriptDefinition,
    &areas::area0221ScriptDefinition,
    &areas::area0222ScriptDefinition,
    &areas::area0225ScriptDefinition,
    &areas::area0228ScriptDefinition,
    &areas::area0232ScriptDefinition,
    &areas::area0235ScriptDefinition,
    &areas::area0236ScriptDefinition,
    &areas::area0237ScriptDefinition,
    &areas::area0240ScriptDefinition,
    &areas::area0247ScriptDefinition,
    &areas::area0248ScriptDefinition,
    &areas::area0249ScriptDefinition,
    &areas::area0250ScriptDefinition,
    &areas::area0252ScriptDefinition,
    &areas::area0256ScriptDefinition,
    &areas::area0257ScriptDefinition,
    &areas::area0258ScriptDefinition,
    &areas::area0259ScriptDefinition,
    &areas::area0260ScriptDefinition,
    &areas::area0262ScriptDefinition,
    &areas::area0264ScriptDefinition,
    &areas::area0265ScriptDefinition,
    &areas::area0009ScriptDefinition,
    &areas::area0010ScriptDefinition,
    &areas::area0014ScriptDefinition,
    &areas::area0015ScriptDefinition,
    &areas::area0017ScriptDefinition,
    &areas::area0023ScriptDefinition,
    &areas::area0025ScriptDefinition,
    &areas::area0026ScriptDefinition,
    &areas::area0027ScriptDefinition,
    &areas::area0029ScriptDefinition,
    &areas::areaWBSSScriptDefinition,
    &areas::areaWKKTScriptDefinition,
    &areas::area0061ScriptDefinition,
    &areas::area4NinScriptDefinition,
    &qtnjScriptDefinition,
    &areaSfkkScriptDefinition,
    &qtcvScriptDefinition,
    &areas::areaWB88ScriptDefinition,
    &areas::ymqbScriptDefinition,
    &qpamScriptDefinition,
    &areas::qrdkScriptDefinition,
    &areas::syspScriptDefinition,
    &areas::dartScriptDefinition,
    &areaSfkmScriptDefinition,
    &areaSfluScriptDefinition,
    &areaSfm2ScriptDefinition,
    &areaSfs2ScriptDefinition,
    &areaSfsrScriptDefinition,
    &areaSftcScriptDefinition,
    &areaSftsScriptDefinition,
    &areaSfwfScriptDefinition,
    &areaSfizScriptDefinition,
    &areaSfbrScriptDefinition,
    &hasiScriptDefinition,
    &qdh1ScriptDefinition,
    &qdh2ScriptDefinition,
    &qdtbScriptDefinition,
    &qdtgScriptDefinition,
    &kew0ScriptDefinition,
    &qfebScriptDefinition,
    &qes1ScriptDefinition,
    &qes2Definition,
    &qes3Definition,
    &qdmrDefinition,
    &urniDefinition,
}};
#endif

constexpr std::array<const GameplayAreaDefinition *, 98> recoveredGameplayAreas{{
    &areas::areaAR01GameplayDefinition,
    &areas::areaARSFGameplayDefinition,
    &areas::areaKWW1GameplayDefinition,
    &areas::areaKWM1GameplayDefinition,
    &areas::areaAKA3GameplayDefinition,
    &areas::areaAKY0GameplayDefinition,
    &areas::areaKWWQGameplayDefinition,
    &areas::areaWSG1GameplayDefinition,
    &areas::areaARM0GameplayDefinition,
    &areas::areaARZ0GameplayDefinition,
    &areas::areaWET0GameplayDefinition,
    &areas::areaWKA0GameplayDefinition,
    &areas::areaQAW6GameplayDefinition,
    &areas::areaQC06GameplayDefinition,
    &areas::areaKRK4GameplayDefinition,
    &areas::areaQAE1GameplayDefinition,
    &areas::areaQAE6GameplayDefinition,
    &areas::areaQAW1GameplayDefinition,
    &areas::areaQDKJGameplayDefinition,
    &areas::areaWECFGameplayDefinition,
    &areas::areaQE01GameplayDefinition,
    &areas::areaQE06GameplayDefinition,
    &areas::areaQEDJGameplayDefinition,
    &areas::areaQFRRGameplayDefinition,
    &areas::areaQAXXGameplayDefinition,
    &areas::areaWE00GameplayDefinition,
    &areas::areaQB00GameplayDefinition,
    &areas::areaARC0GameplayDefinition,
    &areas::areaQF06GameplayDefinition,
    &areas::areaQTB4GameplayDefinition,
    &areas::areaQF39GameplayDefinition,
    &areas::areaQUG0GameplayDefinition,
    &areas::areaAB00GameplayDefinition,
    &areas::areaAK00GameplayDefinition,
    &areas::areaAKS0GameplayDefinition,
    &areas::areaAKS1GameplayDefinition,
    &areas::areaAKT0GameplayDefinition,
    &areas::areaAKT2GameplayDefinition,
    &areas::areaAKT3GameplayDefinition,
    &areas::areaAR02GameplayDefinition,
    &areas::areaAR03GameplayDefinition,
    &areas::areaARA0GameplayDefinition,
    &areas::areaKES1GameplayDefinition,
    &areas::areaKMZ1GameplayDefinition,
    &areas::areaKMZ2GameplayDefinition,
    &areas::areaKMZ3GameplayDefinition,
    &areas::areaKMZ4GameplayDefinition,
    &areas::areaKRC1GameplayDefinition,
    &areas::areaKRH1GameplayDefinition,
    &areas::areaKRK2GameplayDefinition,
    &areas::areaKRM3GameplayDefinition,
    &areas::areaKSH1GameplayDefinition,
    &areas::areaKWW4GameplayDefinition,
    &areas::areaQ100GameplayDefinition,
    &areas::areaQ200GameplayDefinition,
    &areas::areaQ300GameplayDefinition,
    &areas::areaQA00GameplayDefinition,
    &areas::areaQA11GameplayDefinition,
    &areas::areaQA22GameplayDefinition,
    &areas::areaQC00GameplayDefinition,
    &areas::areaQC01GameplayDefinition,
    &areas::areaQCAEGameplayDefinition,
    &areas::areaQD01GameplayDefinition,
    &areas::areaQE00GameplayDefinition,
    &areas::areaQF00GameplayDefinition,
    &areas::areaQF01GameplayDefinition,
    &areas::areaQF02GameplayDefinition,
    &areas::areaQF40GameplayDefinition,
    &areas::areaQGBTGameplayDefinition,
    &areas::areaQJBTGameplayDefinition,
    &areas::areaQKBTGameplayDefinition,
    &areas::areaQLBTGameplayDefinition,
    &areas::areaQR00GameplayDefinition,
    &areas::areaQRR0GameplayDefinition,
    &areas::areaQSFAGameplayDefinition,
    &areas::areaQSFBGameplayDefinition,
    &areas::areaQSFCGameplayDefinition,
    &areas::areaQSFDGameplayDefinition,
    &areas::areaQTB1GameplayDefinition,
    &areas::areaQTB2GameplayDefinition,
    &areas::areaQTB3GameplayDefinition,
    &areas::areaTBAKGameplayDefinition,
    &areas::areaWB00GameplayDefinition,
    &areas::areaWB01GameplayDefinition,
    &areas::areaWEG0GameplayDefinition,
    &areas::areaWEM1GameplayDefinition,
    &areas::areaWES1GameplayDefinition,
    &areas::areaWESMGameplayDefinition,
    &areas::areaWK00GameplayDefinition,
    &areas::areaWN00GameplayDefinition,
    &areas::areaWR00GameplayDefinition,
    &areas::areaWRS2GameplayDefinition,
    &areas::areaWS00GameplayDefinition,
    &areas::areaWSY0GameplayDefinition,
    &areas::areaWT00GameplayDefinition,
    &areas::areaWTA0GameplayDefinition,
    &areas::areaKRK6GameplayDefinition,
    &areas::areaQD00GameplayDefinition,
}};
constexpr std::array<std::uint32_t, recoveredGameplayAreas.size()> recoveredGameplayAreaNativeIndices{{
        9, 15, 30, 29, 2, 8, 32, 94, 14, 16, 87, 89, 42, 47, 25, 39, 40, 41, 51, 82, 53, 54, 55, 62, 43, 81, 44, 13, 59, 76, 60, 77,
        0, 1, 3, 4, 5, 6, 7, 10, 11, 12, 17, 18, 19, 20, 21, 22,
        23, 24, 27, 28, 31, 33, 34, 35, 36, 37, 38, 45, 46, 48, 50, 52,
        56, 57, 58, 61, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74,
        75, 78, 79, 80, 83, 84, 85, 86, 88, 90, 91, 92, 93, 95, 96, 97, 26, 49}};

std::size_t FunctionIndex(EventFunctionType type) {
    return static_cast<std::size_t>(type);
}

void CopyAreaSlotId(AreaSlot &slot, const char *areaId) {
    std::strncpy(slot.areaId.data(), areaId, slot.areaId.size());
}

void DoorGameplayAreaCleanup(std::int32_t sceneId) {
    (void)ExecuteGameplayAreaCleanup(sceneId != 0);
}

} // namespace

// The trailing digit selects the slot; an unchanged slot is not reloaded unless forced.
bool LoadAreaFunc_Wrapper_0(const char *areaId, std::int32_t force) {
    const std::size_t length = std::strlen(areaId);
    const std::int32_t type = length ? std::atoi(areaId + length - 1) : 0;
    if (static_cast<std::uint32_t>(type - 1) > 2U)
        return false;
    AreaSlot &slot = areaSlots[static_cast<std::size_t>(type)];
#if SHENMUE_IS_DC
    if (force == 1 || std::strcmp(slot.areaId.data(), areaId) != 0) {
        const char *const directory = std::strncmp(areaId + length - 3, "MD", 2) == 0 ? "module" : Filepath_Gen(FilepathGenMode::SceneDirectory);
        char path[256];
        const int pathLength = std::snprintf(path, sizeof(path), "/%s/%s", directory, areaId);
        if (pathLength < 0 || static_cast<std::size_t>(pathLength) >= sizeof(path))
            return false;
        const std::size_t size = static_cast<std::size_t>(ArchiveData_GetFileSize(path));
        if (!size || !ArchiveData_Read(path, 0, GetEventExecutionRegion(static_cast<EventFunctionType>(type)), size))
            return false;
        CopyAreaSlotId(slot, areaId);
    }
#else
    if (force == 1 || std::strncmp(slot.areaId.data(), areaId, slot.areaId.size()) != 0) {
        LoadAreaFuncByIDAndFuncType(static_cast<EventFunctionType>(type), areaId);
        CopyAreaSlotId(slot, areaId);
    }
#endif
    return true;
}

const char *GetAreaSlotName(std::uint32_t slot) {
    return slot > 3 ? nullptr : areaSlots[slot].areaId.data();
}

// @verified - p
void scenFreeEvent(const char *requestedScenario) {
    for (std::size_t index = 1; index < areaSlots.size(); ++index) {
        AreaSlot &slot = areaSlots[index];
        if (std::strcmp(requestedScenario, slot.areaId.data()) != 0)
            continue;
        slot.areaId.fill(0);
        return;
    }
}

void scenInitCallbacks() {
    areaSlots = {};
#if SHENMUE_IS_DC
    InitializeEventExecutionRegions();
#else
    currentAreaMainFunctions = {};
#endif
}

void Startup_EventBase_Callback(void *) {
    stepUpdateFlagUtil(0, 500);
    const MapEventAreaDefinition &record = GetCurrentMapEventAreaDefinition();
    switch (record.startupMode) {
    case 1:
        break;
    case 2:
    case 3:
    case 4:
#if SHENMUE_IS_DC
        ExecuteCurrentAreaMainFunction(static_cast<EventFunctionType>(record.startupMode - 1U));
#else
        ExecuteCurrAreaMainFunc_Wrapper(record.startupMode - 1U, 0);
#endif
        break;
    case 5:
    case 6:
    case 7:
        LoadMapPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), record.startupPayload);
        LoadPak();
#if SHENMUE_IS_DC
        ExecuteCurrentAreaMainFunction(static_cast<EventFunctionType>(record.startupMode - 4U));
#else
        ExecuteCurrAreaMainFunc_Wrapper(record.startupMode - 4U, 0);
#endif
        break;
    default:
        LoadAreaFunc_Wrapper_0(record.startupPayload, 0);
        ExecuteAreaMainFunction(record.startupPayload);
        break;
    }
    HLib::DestroyCurrentTask();
}

bool shenLoadEventFrom(const char *areaId, EventFunctionType functionType) {
    const std::size_t index = FunctionIndex(functionType);
    if (index < 1 || index > 3)
        return false;
    CopyAreaSlotId(areaSlots[index], areaId);
#if !SHENMUE_IS_DC
    LoadAreaFuncByIDAndFuncType(functionType, areaId);
#endif
    return true;
}



void EI_Startup_BeginEventFade() {
    if ((GetLoadingPhaseFlags() & kLoadingPhaseSkipStartupFadeWait) == 0)
        WaitForNextTask_0(GetCurrentAreaScreenFilmTask(), 0, 31);
    (void)EV_spmBeginSampleExecute();
    terminalMediaState.sampleExecutionActive = true;
    if ((GetLoadingPhaseFlags() & kLoadingPhaseSkipTitleFade) == 0)
        ST_LoadTitleFade();
    if ((GetLoadingPhaseFlags() & kLoadingPhaseWaitNpcBeforeFade) == 0)
        LoadPak();
}

void EI_Startup_EventFade1_Callback(void *) {
    if ((GetLoadingPhaseFlags() & kLoadingPhaseWaitNpcBeforeFade) != 0 && NpcWait())
        return;
    EI_Startup_BeginEventFade();
    HLib::SetCurrentTaskTo(EI_Startup_EventFade2_Callback);
}

void EI_Startup_EventFade2_Callback(void *) {
    if (DoesScreenFilmColourExist(GetCurrentAreaScreenFilmTask()))
        return;
    SetLoadingPhaseFlags(GetLoadingPhaseFlags() & ~kLoadingPhaseWaitNpcBeforeFade);
    shenSetTimeSimulationTo(true);
    EV_setHoldStatesFlags(1);
    if (nextStartupFadeTask) {
        HLib::ResolveDeferredCallback(nextStartupFadeTask);
    }
    startupFadeTask = nullptr;
    HLib::DestroyCurrentTask();
}

bool Queue_EI_Startup_EventFade1() {
    startupFadeTask = HLib::EnqueueTaskWithoutParameter(EI_Startup_EventFade1_Callback, 3, 0x0b, static_cast<std::int32_t>(SHEN_NAME(EVBS)));
    nextStartupFadeTask = nullptr;
    shenSetTimeSimulationTo(false);
    EV_setHoldStatesFlags(0);
    InitMapAndCharaFlags();
    HLib::SetupTimedTask(startupFadeTask, 0);
    return startupFadeTask != nullptr;
}

void QueueImmediateNextFadeTask(HLib::Task *task) {
    HLib::ResolveDeferredCallback(startupFadeTask);
    nextStartupFadeTask = task;
    if (task)
        HLib::SetupTimedTask(task, 0);
}

void EVT_NightAuthor_Callback(void *) {
    using enum CameraState;
    constexpr shen_name ryo{SHEN_NAME(RYO_)};
    if (nightAuthorState == 0) {
        if (DoesScreenFilmColourExist(GetCurrentAreaScreenFilmTask()))
            return;
        EV_DisableCharacterDisplay(ryo);
        EV_ToggleCharacterFlag(ryo);
        ExecuteGameplayAreaCleanup(true);
        auto *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        constexpr shen_name yoru{SHEN_NAME(YORU)};
        if (control) {
            if (HasDoorAuthorEntries())
                SetTemporaryDoorAuthor(yoru);
            Author_InitializeFromMapInfoAuthor(control->mapInfo, yoru);
        }
        if (HasDoorAuthorEntries()) {
            (void)SetupDoorAreaCallbacks();
            ExecuteDoorAreaCleanup(0);
        }
        if (g_cameraControl)
            g_cameraControl->showFilmBars = 0;
        WaitForNextTask_0(GetCurrentAreaScreenFilmTask(), 0, 30);
        (void)EV_spmBeginSampleExecute();
        (void)ExecuteAicaCommand(0x5a0U, 3, -64);
        (void)ExecuteAicaCommand(0x11a0U, 3, -64);
        terminalMediaState.sampleExecutionActive = true;
        nightAuthorState = 1;
        return;
    }
    if (nightAuthorState == 1) {
        if (authGetLoaded())
            return;
        if (HasDoorAuthorEntries())
            (void)authLoadCurrentQueued();
        (void)ExecuteAicaCommand(0x5a0U, 3, -15);
        (void)ExecuteAicaCommand(0x11a0U, 3, -15);
        Author_Free();
        if (HasDoorAuthorEntries())
            SetTemporaryDoorAuthor({});
        EV_EnableCharacterDisplay(ryo);
        EV_EnsureCharacterDraw(ryo);
        camCtrlSetState(Init);
        camCtrlSetFovTransitionTo(true);
        WaitForNextTask_0(GetCurrentAreaScreenFilmTask(), 0, 30);
        nightAuthorState = 2;
        return;
    }
    if (DoesScreenFilmColourExist(GetCurrentAreaScreenFilmTask()))
        return;
    SetCurrentAreaControlLock(false);
    SetPlayerControlEnabled(1);
    HLib::SetCurrentTaskTo(EVT_AreaMainLoop_Callback);
}

void EVT_NightChange_Callback(void *) {
    using enum CameraState;
    if (nightAuthorState == 0) {
        if (DoesScreenFilmColourExist(GetCurrentAreaScreenFilmTask()))
            return;
        ExecuteGameplayAreaCleanup(true);
        (void)EV_spmBeginSampleExecute();
        camCtrlSetState(Init);
        camCtrlSetFovTransitionTo(true);
        WaitForNextTask_0(GetCurrentAreaScreenFilmTask(), 0, 30);
        terminalMediaState.sampleExecutionActive = true;
        nightAuthorState = 1;
        return;
    }
    if (DoesScreenFilmColourExist(GetCurrentAreaScreenFilmTask()))
        return;
    SetCurrentAreaControlLock(false);
    SetPlayerControlEnabled(1);
    HLib::SetCurrentTaskTo(EVT_AreaMainLoop_Callback);
}

#if !SHENMUE_IS_DC
void LoadAreaFuncByIDAndFuncType(EventFunctionType functionType, const char *areaId) {
    const std::size_t index = FunctionIndex(functionType);
    if (index < 1 || index > 3)
        return;

    AreaInitFunction init = nullptr;
    ScriptAreaMainFunction main = nullptr;
    bool found = false;
    for (const ScriptAreaDefinition *definition : recoveredScriptAreas) {
        if (std::strncmp(areaId, reinterpret_cast<const char *>(&definition->areaId), 4) == 0) {
            init = definition->init;
            main = definition->main;
            found = true;
            break;
        }
    }


    if (!found && IsNativeSpecializedArea(areaId)) {
        currentAreaMainFunctions[index] = nullptr;
        return;
    }
    if (!found && functionType == EventFunctionType::Event1) {
        init = EVT_PreAreaInit;
        main = EVT_PreAreaMainLoop;
    }
    currentAreaMainFunctions[index] = main;
    areaPreInitCallbacks[index - 1]();
    if (init)
        init();
}
std::int64_t ExecuteCurrAreaMainFunc(const char *slotName, std::uintptr_t argument) {
    for (std::size_t index = 0; index < areaSlots.size(); ++index) {
        if (std::strcmp(slotName, areaSlots[index].areaId.data()) != 0)
            continue;
        currentAreaMainFunctions[index](argument);
        return 1;
    }
    return 0;
}

std::int64_t ExecuteCurrAreaMainFunc_Wrapper(std::uint32_t slot, std::uintptr_t argument) {
    if (slot > 3U)
        return 0;
    return ExecuteCurrAreaMainFunc(areaSlots[slot].areaId.data(), argument);
}

#endif

bool ExecuteAreaMainFunction(const char *areaId, std::uintptr_t argument) {
    if (!areaId)
        return false;
#if SHENMUE_IS_DC
    for (std::size_t index = 0; index < areaSlots.size(); ++index) {
        if (std::strcmp(areaId, areaSlots[index].areaId.data()) != 0)
            continue;
        ExecuteEventOverlay(static_cast<std::uint32_t>(index), argument);
        return true;
    }
    return false;
#else
    return ExecuteCurrAreaMainFunc(areaId, argument) != 0;
#endif
}

bool ExecuteCurrentAreaMainFunction(EventFunctionType functionType, std::uintptr_t argument) {
    const std::size_t index = FunctionIndex(functionType);
    if (index == 0 || index >= areaSlots.size() || areaSlots[index].areaId[0] == '\0') {
        return false;
    }
    return ExecuteAreaMainFunction(areaSlots[index].areaId.data(), argument);
}
bool QueueAreaEventEntry() {
    SetDoorAreaCleanupCallback(DoorGameplayAreaCleanup);
    InitEventTaskQueue();
    areaEventEntryTask = HLib::EnqueueTaskWithoutParameter(Startup_EventBase_Callback, 3, 0x0b, static_cast<std::int32_t>(SHEN_NAME(EVBS)));
    return areaEventEntryTask != nullptr;
}

HLib::Task *GetCurrentAreaScreenFilmTask() {
    const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return control ? control->screenFilmTask : nullptr;
}

void ReleaseAreaEventTaskOwners() {
    ReleaseEventGameplayModeOwners();
    if (startupFadeTask)
        HLib::DestroyTask(startupFadeTask);
    areaEventEntryTask = nullptr;
    preAreaMainLoopTask = nullptr;
    startupFadeTask = nullptr;
    nextStartupFadeTask = nullptr;
    areaSlots = {};
#if !SHENMUE_IS_DC
    currentAreaMainFunctions = {};
#endif
}

bool ExecuteGameplayArea(std::uint32_t startParameters) {
    const shen_name areaId = GetAreaDefinitionMapNameForCurrentMap();
    for (std::size_t index = 0; index < recoveredGameplayAreas.size(); ++index) {
        const GameplayAreaDefinition &definition = *recoveredGameplayAreas[index];
        if (definition.areaId.name != areaId.name || !definition.main) {
            continue;
        }
        if (definition.init) {
            definition.init();
        }
        definition.main(startParameters);
        return true;
    }
    return false;
}

bool ExecuteGameplayAreaCleanup(bool afternoon) {
    const shen_name areaId = GetAreaDefinitionMapNameForCurrentMap();
    SelectPostFxBank(1, 1);
    UploadGlowParams(afternoon ? nightGlowParameters : defaultGlowParameters);
    SFIL_UploadBloomParams(afternoon);
    for (std::size_t index = 0; index < recoveredGameplayAreas.size(); ++index) {
        const GameplayAreaDefinition *const definition = recoveredGameplayAreas[index];
        if (definition->areaId.name != areaId.name || !definition->main) {
            continue;
        }
        if (definition->cleanup) {
            definition->cleanup(afternoon ? 1 : 0, recoveredGameplayAreaNativeIndices[index]);
        }
        return true;
    }
    return false;
}

void SetCharaKill(shen_name characterId, std::uint32_t mode) {
    auto *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (!control)
        return;
    Character *const character = scenFindChara(control->scenarioTask, characterId);
    ctrlSetKillAndIncrementCounter(character, mode);
}

void SetCurrentAreaControlLock(bool locked) {
    if (locked) {
        areaFlags |= AreaFlag_KeepPlayerControlLocked;
    } else {
        areaFlags &= ~AreaFlag_KeepPlayerControlLocked;
    }
}

bool CurrentAreaOwnsControlLock() {
    return (areaFlags & AreaFlag_KeepPlayerControlLocked) != 0;
}

bool IsAreaItemSelectionActive() {
    constexpr std::array selectionElements{
        UIElement::MainMenu,
        UIElement::MainMenuWheel,
        UIElement::ActionSelector,
    };
    for (const UIElement element : selectionElements) {
        const UIElementState &state = g_uiElements[static_cast<std::size_t>(element)];
        if (state.isVisible != 0 && state.enabled != 0)
            return true;
    }
    return false;
}

void PublishAreaTerminalMediaState(shen_name cutsceneArea, const char *audioBank, std::int32_t audioVariant, bool sampleExecutionActive) {
    terminalMediaState = {};
    terminalMediaState.cutsceneArea = cutsceneArea;
    if (audioBank) {
        std::snprintf(terminalMediaState.audioBank.data(), terminalMediaState.audioBank.size(), "%s", audioBank);
    }
    terminalMediaState.audioVariant = audioVariant;
    terminalMediaState.sampleExecutionActive = sampleExecutionActive;
}

const AreaTerminalMediaState &GetAreaTerminalMediaState() {
    return terminalMediaState;
}

} // namespace shenmue::s2
