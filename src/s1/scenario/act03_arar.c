#include "act03_arar.h"

static void script_0030(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  return;
}

static void script_0039(void) {
  Script_ryoControl(RYO_RestoreControl);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_0048(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L000074;
    }
  }
  L000074:;
  Script_scenToggleVisibleFor(arg0, 1);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_charaToggleMotEnabled(arg0, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 2);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_CharHandlers(arg0, 2, 8, 16);
  Script_SetCPAMFlags(arg0, 0);
  Script_charaDisplay(arg0, CDM_Show);
  return;
}

static void script_00D5(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_ccowSetHeightMode(arg0, CCOW_FollowGround);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_FaceEx(arg0, FACE_ExpressionDisabled, 0, 2);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_CharHandlers(arg0, -1, 0, 0);
  Script_SetCPAMFlags(arg0, 1);
  return;
}

static void script_0125(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0153(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0181(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_01B5(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_01E9(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 150, 68, 50, 0);
  return;
}

static void script_0207(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  Script_GetDataFromFightProgram(arg0, arg1);
  local_FC = 0;
  while (local_FC < arg2) {
    local_FC = local_FC + 1;
    Script_SetFightProgramStateBytes(arg0, local_FC, 1);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0241(int32_t arg0, int32_t arg1) {
  while (arg1 > 0) {
    Script_SetFightProgramStateBytes(arg0, arg1, 1);
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  return;
}

static void script_0275(void) {
  int8_t local_DD[35];
  int8_t local_DC;
  int8_t local_DB;
  int8_t local_DA;
  int8_t local_D9;
  int32_t local_D4;
  local_DC = 0;
  while (local_DC < 35) {
    local_DD[local_DC] = local_DC % 4;
    local_DC = local_DC + 1;
  }
  local_D9 = 0;
  for (;;) {
    local_D4 = local_D9;
    switch (local_D4) {
      case 0:
        local_DB = 0;
        local_DA = 17;
        local_D9 = 1;
        break;
      case 1:
        local_DB = 17;
        local_DA = 35;
        local_D9 = 0;
        break;
    }
    local_DC = local_DB;
    while (local_DC < local_DA) {
      Script_SetImageModuleTaskDataFlag(SHEN_NAME('FIR1'), local_DD[local_DC]);
      Script_SetImageModuleTaskDataFlag(SHEN_NAME('FIR2'), local_DD[local_DC]);
      Script_SetImageModuleTaskDataFlag(SHEN_NAME('FIR3'), local_DD[local_DC]);
      Script_SetImageModuleTaskDataFlag(SHEN_NAME('FIR4'), local_DD[local_DC]);
      local_DD[local_DC] = local_DD[local_DC] + 1;
      local_DD[local_DC] = local_DD[local_DC] % 6;
      local_DC = local_DC + 1;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_036D(int32_t arg0, int32_t arg1) {
  Script_FACE_LCG(arg0, FACE_BlinkClose, arg1, 0);
  SCNF_WaitFrames_0000(arg1 + 1);
  Script_FACE_LCG(arg0, FACE_BlinkOpen, arg1, 0);
  SCNF_WaitFrames_0000(arg1);
  return;
}

static void script_039D(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP05_SA1116_DOJO_DONE) == 0) {
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_SA1116_DOJO_TALK_DONE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_01LET_EVENT_DONE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_NINJIN_EVENT_DONE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_HIHY_EVENT_DONE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_YD01_LOOKOUT_TRANSITION_TRIGGERED, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_SCENE1_DOOR_0B_PROMPT_DONE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_SLEEP_RESUME_INITIALIZED, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP05_SA1116_DOJO_DONE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_RAKK_FOLLOWUP_GATE_CLEARED, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CATA1_INTRO_COMPLETE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 686, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE2_COMPLETE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 795, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CATA1_NBO_BRANCH_CHOSEN, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_D000_HATO_STOPPER_CLEARED, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_KAME_YAMAMOTO_SEARCH_STARTED, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_KAME_YAMAMOTO_CLUE_HEARD, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_KAME_YAMAMOTO_SEARCH_CLOSED, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_KAME_YAMAMOTO_HOUSE_FOUND, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_14_REACHED, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_JHW0_EVENT_SEEN, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_JHW0_EVENT_COMPLETE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_1E_REACHED, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_28_REACHED, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_SEEK_THE_THREE_BLADES, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_VISIT_LIU_BARBER, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_DRHT_LIU_BARBER_VISITED, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_RYU_EVENT_ACTIVE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_D000_DEQB_EVENT_DONE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_D000_YAMAGISHI_EVENT_COMPLETE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_DBYO_DJAZ_DHQB_CHAIN_ACTIVE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_DJAZ_BALL_EVENT_SUCCESS, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_DHQB_POST_CHALLENGE_RETURN_HOME, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_DBYO_SA1010_CUTSCENE_COMPLETE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_DHQB_CHALLENGE_COMPLETE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_VISIT_NAGAI_INDUSTRIES, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CHARLIE_AT_MILITARY_SURPLUS, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_D000_VENDING_STEP4_BRANCH_SEEN, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_JUTADN_EVENT_ACTIVE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_TATQ_FOLLOWUP_QUEUED, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_TATQ_HORS_EVENT_PENDING, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_TATQ_INITIAL_VISIT_COMPLETE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_LEAD_ON_CHARLIE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE3_COMPLETE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_TATQ_FOLLOWUP_COMPLETE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_A01124_INE_EVENT_DONE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_SA1077_INE_HANDOFF_DONE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_D000_YBHN_EVENT_DONE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 900, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 881, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAKG_ROUTE_ACTIVE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_D000_VENDING_STEP6_BRANCH_SEEN, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_D000_DEFB_EVENT_DONE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_E1049_ROUTE_UNLOCKED, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_E1049_ROUTE_ACTIVE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_D000_YKHI_EVENT_PENDING, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_ASK_WHO_READS_CHINESE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_D000_VENDING_MACHINE_PURCHASED_ONCE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE7_COMPLETE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_D000_VENDING_EVENT_SEEN, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_TOKI_TEGS_CUTSCENE_COMPLETE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 185, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_TOKI_PHONE_HOME_PROMPT_SHOWN, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_UNLOCKED, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_DONE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_SPEND_BUDGET_BONUS_ACTIVE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_COFFEE_ROUTE_ENABLED, 1);
    Script_GetCassetteFlags(2, 1, 0, 0);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_GORO_QTE_COMPLETE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_BOX_RESCUE_ROUTE_UNLOCKED, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_BOX_RESCUE_ROUTE_COMPLETE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_COFFEE_EVENT_STARTED, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_COFFEE_SPECIAL_VARIANT, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_COFFEE_EVENT_COMPLETE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE4_COMPLETE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_DBYO_SA1018_CUTSCENE_COMPLETE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_DNOZ_HOME_PROMPT_SHOWN, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 928, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 917, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_POST_BOX_ROUTE_SLEEP_BRANCH_MARKED, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MS8S_MAIN_EVENT_COMPLETE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_HISA_REQUEST_UNLOCKED, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_HISA_REQUEST_ACCEPTED, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_HISA_QTE_RESOLVED, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MKSG_MS08_TRANSITION_TRIGGERED, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MKSG_HELP_ROUTE_ACTIVE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 838, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_HISA_REQUEST_DECLINED, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MS08_SCENE_EXIT_COMPLETE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_SORA_SCENE_PENDING, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_SORA_SCENE_STARTED, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_TAIKE_EVENT_DONE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_AKIR_INE_DUO_SCENE_COMPLETE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_DNOZ_EVENT_DONE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_DNOZ_ROUTE_ACTIVE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE5_COMPLETE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_POST_BOX_ROUTE_SLEEP_BRANCH_MARKED, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_DKTY_TUBA_CUTSCENE_COMPLETE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_SA1037_SEQUENCE_COMPLETE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_WAKE_FROM_SLEEP_PENDING, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_WAKE_FROM_SLEEP_CUTSCENE_SEEN, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KHKS_HINT_INTRO_SEEN, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_FIRST_SEARCH_REACTION_SEEN, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK2_FIRST_SEARCH_REACTION_SEEN, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_FIRST_SEARCH_HINT_SEEN, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ROTATING_DOOR_UNLOCKED, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_TSUBA_PLACED, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KATANA_INSERTED, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_HANGING_SCROLL_TAKEN, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_DOJO_SWORD_DISPLAYED, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK2_HANGING_SCROLL_TAKEN, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_BASEMENT_SWORD_FOUND, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_SECRET_BOX_OPENING_COMPLETE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MS8A_POST_MS08_FOLLOWUP_COMPLETE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 305, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_A0160_FUKU_EVENT_DONE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_01BNK_BANK_EVENT_ACTIVE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_PRE_0172_FUKU_INE_STAGE_DONE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_ARAR_ETUKO_REWARD_STAGE_ACTIVE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_PIGGY_BANK_TAKEN, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_ARAR_ETUKO_REWARD_ROUTE_COMPLETE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_D000_VENDING_LOCK_CLEARED, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_0140_BANK_STAGE_ACTIVE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_0140_BANK_STAGE_PROMPT_SHOWN, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_COMPLETE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_RESOLVED, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_PENDING, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_DSKI_01MIT_CUTSCENE_PHASE_A_DONE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_ARAR_ETUKO_CASSETTE_REWARD_COMPLETE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_0154_PIGGY_BANK_PROMPT_SHOWN, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_VENDING_HOME_PROMPT_SHOWN, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_HANA_STAGE_ACTIVE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_0172_SCENE2_PROMPT_SHOWN, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_0172_RETURN_DONE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_STARTED, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_ADVANCED, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_0172_RETURN_PROMPT_SHOWN, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_ARAR_POST_GMCT_STAGE_ACTIVE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_D000_JIMSE_EVENT_DONE, 1);
  }
  return;
}

static void script_0998(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_SetMotnFlag(arg0, -1);
  Script_SetCPAMFlags(arg0, 0);
  Script_SetCharaFlags(arg0, 4);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 3);
  Script_SetCharaRefBoardPos(arg0, &data_4FB0);
  Script_charaSetRefboardFlagTo(arg0, 1);
  return;
}

static void script_09FB(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_SetMotnFlag(arg0, -1);
  Script_SetCPAMFlags(arg0, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_facemotUtils(arg0, 1, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 1);
  Script_FACE_LCG(arg0, FACE_BlinkAutomatic, 6, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 1, data_408C, 1);
  Script_handSetFloatsFor(arg0, 0, data_408C, 1);
  Script_SetCharaFlags(arg0, 4);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 3);
  Script_SetCharaRefBoardPos(arg0, &data_4FB0);
  Script_charaSetRefboardFlagTo(arg0, 1);
  return;
}

static void script_0ACC(void) {
  int32_t unused_F4[3];
  script_09FB(CHARA_AKIR);
  script_09FB(CHARA_JIMY);
  Script_charaDisplay(SHEN_NAME('DOOR'), CDM_Show);
  Script_SetupLightFromTable(0);
  Script_PREP_Related(-1, 1);
  return;
}

static int32_t script_0B02(void) {
  for (;;) {
    task_0 = task_0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_0B1B(void) {
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, 2, "bgm013.snd", 0x8000);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(9640, 0, 0);
  Script_executeAicaCommand(1184, 2, 107);
  Script_LoadSND(4, 0, 0, 0);
  return;
}

static void script_0B7C(void) {
  Vector3 local_F4;
  if (-1) {
    Script_charaDisplay(CHARA_ETKO, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_ETKO, CDM_Show);
      goto L000BA6;
    }
  }
  L000BA6:;
  Script_scenToggleVisibleFor(CHARA_ETKO, 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('INF1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('INF1'), CDM_Show);
      goto L000BDA;
    }
  }
  L000BDA:;
  Script_scenToggleVisibleFor(SHEN_NAME('INF1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('INF2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('INF2'), CDM_Show);
      goto L000C0E;
    }
  }
  L000C0E:;
  Script_scenToggleVisibleFor(SHEN_NAME('INF2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('INF3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('INF3'), CDM_Show);
      goto L000C42;
    }
  }
  L000C42:;
  Script_scenToggleVisibleFor(SHEN_NAME('INF3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('MAGA'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('MAGA'), CDM_Show);
      goto L000C76;
    }
  }
  L000C76:;
  Script_scenToggleVisibleFor(SHEN_NAME('MAGA'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('TRAY'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('TRAY'), CDM_Show);
      goto L000CAA;
    }
  }
  L000CAA:;
  Script_scenToggleVisibleFor(SHEN_NAME('TRAY'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('PURS'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('PURS'), CDM_Show);
      goto L000CDE;
    }
  }
  L000CDE:;
  Script_scenToggleVisibleFor(SHEN_NAME('PURS'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('COMP'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('COMP'), CDM_Show);
      goto L000D12;
    }
  }
  L000D12:;
  Script_scenToggleVisibleFor(SHEN_NAME('COMP'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('MONY'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('MONY'), CDM_Show);
      goto L000D46;
    }
  }
  L000D46:;
  Script_scenToggleVisibleFor(SHEN_NAME('MONY'), 0);
  for (;;) {
    {
      int32_t local_F0;
      local_F0 = task_0;
      switch (local_F0) {
        case 1:
          Script_CharHandlers(CHARA_AKIR, 2, 8, 16);
          Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
          Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
          break;
        case 691:
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_3FA8, 0);
          break;
        case 831:
          Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
          break;
        case 1379:
          Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_4170, 1);
          break;
        case 1524:
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_408C, 30);
          break;
        case 1560:
          Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
          break;
        case 1388:
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_4170, 1);
          break;
        case 1535:
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_408C, 30);
          break;
        case 1570:
          Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
          break;
        case 1709:
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_408C, 1);
          break;
        case 1710:
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_50A4, 6);
          break;
        case 1716:
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_4170, 4);
          break;
        case 1818:
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_50A4, 3);
          break;
        case 1824:
          Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
          break;
      }
    }
    {
      int32_t local_F0;
      local_F0 = task_0;
      switch (local_F0) {
        case 1:
          Script_FACE(CHARA_AKIR, 0, 0, 1);
          break;
        case 305:
          Script_FACE(CHARA_AKIR, 7, 0, 10);
          break;
        case 431:
          Script_FACE(CHARA_AKIR, 2, 0, 1);
          break;
        case 633:
          Script_FACE(CHARA_AKIR, 10, 0, 10);
          break;
        case 976:
          Script_FACE(CHARA_AKIR, 0, 0, 1);
          break;
        case 1169:
          Script_FACE(CHARA_AKIR, 2, 0, 1);
          break;
        case 1212:
          Script_FACE(CHARA_AKIR, 4, 0, 6);
          break;
        case 1251:
          Script_FACE(CHARA_AKIR, 5, 0, 1);
          break;
        case 1824:
          Script_FACE(CHARA_AKIR, 0, 0, 1);
          break;
      }
    }
    {
      int32_t local_F0;
      local_F0 = task_0;
      switch (local_F0) {
        case 691:
          Script_clmdSetFlags(CHARA_AKIR, 2, 1);
          break;
        case 830:
          Script_clmdSetFlags(CHARA_AKIR, 2, 0);
          break;
      }
    }
    if (task_0 == 1) {
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
    } else if ((task_0 >= 1203) & (task_0 <= 1211)) {
      Script_EVT_GetPosition(CHARA_KJHS, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F4, 8);
    } else {
      if (task_0 >= 1624) {
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
        goto L001111;
      }
    }
    L001111:;
    {
      int32_t local_F0;
      local_F0 = task_0;
      switch (local_F0) {
        case 1:
          Script_FACE(CHARA_JIMY, 0, 0, 1);
          break;
        case 321:
          Script_FACE(CHARA_JIMY, 10, 0, 1);
          break;
        case 431:
          Script_FACE(CHARA_JIMY, 0, 0, 1);
          break;
        case 624:
          Script_FACE(CHARA_JIMY, 2, 0, 8);
          break;
        case 831:
          Script_FACE(CHARA_JIMY, 0, 0, 1);
          break;
        case 976:
          Script_FACE(CHARA_JIMY, 5, 0, 1);
          break;
        case 1169:
          Script_FACE(CHARA_JIMY, 0, 0, 1);
          break;
        case 1251:
          Script_FACE(CHARA_JIMY, 1, 2, 1);
          break;
        case 1369:
          Script_FACE(CHARA_JIMY, 7, 0, 1);
          break;
        case 1709:
          Script_FACE(CHARA_JIMY, 0, 0, 1);
          break;
        case 1725:
          Script_FACE(CHARA_JIMY, 4, 1, 1);
          break;
        case 1817:
          Script_FACE(CHARA_JIMY, 0, 0, 1);
          break;
        case 1824:
          Script_FACE(CHARA_JIMY, 4, 0, 1);
          break;
        case 2083:
          Script_FACE(CHARA_JIMY, 0, 0, 1);
          break;
      }
    }
    {
      int32_t local_F0;
      local_F0 = task_0;
      switch (local_F0) {
        case 1:
          Script_ResetHandMotion(CHARA_JIMY, 0, 1, 0);
          Script_ResetHandMotion(CHARA_JIMY, 0, 0, 0);
          Script_CharHandlers(CHARA_JIMY, 2, 8, 16);
          break;
        case 1725:
          Script_ResetHandMotion(CHARA_JIMY, 1, 1, 0);
          Script_ResetHandMotion(CHARA_JIMY, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_JIMY, 1, data_408C, 1);
          Script_handSetFloatsFor(CHARA_JIMY, 0, data_408C, 1);
          break;
        case 1816:
          Script_ResetHandMotion(CHARA_JIMY, 0, 1, 0);
          Script_ResetHandMotion(CHARA_JIMY, 0, 0, 0);
          break;
        case 1824:
          Script_ResetHandMotion(CHARA_JIMY, 1, 1, 0);
          Script_ResetHandMotion(CHARA_JIMY, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_JIMY, 1, data_408C, 1);
          Script_handSetFloatsFor(CHARA_JIMY, 0, data_408C, 1);
          break;
      }
    }
    if (task_0 == 1) {
      Script_CharHandlers(CHARA_KJHS, 2, 8, 16);
      Script_CharHandlers(CHARA_MNWA, 2, 8, 16);
    }
    {
      int32_t local_F0;
      local_F0 = task_0;
      switch (local_F0) {
        case 1824:
          Script_SetAnimatedCameraFov(StartTransition, 0x43340000, 800);
          break;
        case 2082:
          Script_SetAnimatedCameraFov(0, 0, 0);
          break;
      }
    }
    if (task_0 == 1381) {
      Script_executeAicaCommand(81832, 0, 0);
      Script_executeAicaCommand(1184, 2, 127);
    }
    if (task_0 == 2022) {
      Script_executeAicaCommand(2720, 1, 30);
      Script_executeAicaCommand(2720, 2, 30);
      Script_executeAicaCommand(2720, 3, 30);
      Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_14AA(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_charaDisplay(SHEN_NAME('TKET'), CDM_Show);
  Script_scenToggleActiveFor(SHEN_NAME('TKET'), 0);
  Script_ccowSetHeightMode(SHEN_NAME('TKET'), CCOW_KeepHeight);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_408C, 1);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_408C, 1);
  Script_ResetHandMotion(CHARA_JIMY, 1, 1, 0);
  Script_ResetHandMotion(CHARA_JIMY, 1, 0, 0);
  Script_handSetFloatsFor(CHARA_JIMY, 1, data_408C, 1);
  Script_handSetFloatsFor(CHARA_JIMY, 0, data_408C, 1);
  for (;;) {
    if (Script_shenSeqUtils(-1)) {
      {
        int32_t local_E4;
        local_E4 = task_0;
        switch (local_E4) {
          case 1:
            Script_FACE(CHARA_AKIR, 0, 0, 1);
            break;
          case 299:
            Script_FACE(CHARA_AKIR, 2, 0, 8);
            break;
          case 348:
            Script_FACE(CHARA_AKIR, 5, 0, 8);
            break;
          case 404:
            Script_FACE(CHARA_AKIR, 2, 0, 8);
            break;
          case 799:
            Script_FACE(CHARA_AKIR, 5, 0, 8);
            break;
          case 829:
            Script_FACE(CHARA_AKIR, 2, 0, 8);
            break;
          case 869:
            Script_FACE(CHARA_AKIR, 5, 0, 8);
            break;
          case 929:
            Script_FACE(CHARA_AKIR, 2, 0, 8);
            break;
          case 1094:
            Script_FACE(CHARA_AKIR, 5, 0, 8);
            break;
          case 1326:
            Script_FACE(CHARA_AKIR, 5, 2, 8);
            break;
          case 1349:
            Script_FACE(CHARA_AKIR, 5, 0, 8);
            break;
          case 1404:
            Script_FACE(CHARA_AKIR, 5, 5, 8);
            break;
          case 1449:
            Script_FACE(CHARA_AKIR, 2, 0, 8);
            break;
          case 1659:
            Script_FACE(CHARA_AKIR, 5, 0, 8);
            break;
          case 1699:
            Script_FACE(CHARA_AKIR, 2, 0, 8);
            break;
          case 1938:
            Script_FACE(CHARA_AKIR, 5, 0, 8);
            break;
        }
      }
      {
        int32_t local_E4;
        local_E4 = task_0;
        switch (local_E4) {
          case 799:
            Script_handSetFloatsFor(CHARA_AKIR, 1, data_45E4, 8);
            break;
          case 1289:
            Script_handSetFloatsFor(CHARA_AKIR, 1, data_408C, 8);
            Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &data_58B4);
            Script_handSetFloatsFor(CHARA_AKIR, 0, data_526C, 8);
            break;
          case 1809:
            Script_handSetFloatsFor(CHARA_AKIR, 0, data_408C, 8);
            break;
        }
      }
      {
        int32_t local_E4;
        local_E4 = task_0;
        switch (local_E4) {
          case 1:
            Script_FACE(CHARA_JIMY, 4, 0, 1);
            break;
          case 5:
            Script_FACE(CHARA_JIMY, 7, 0, 8);
            break;
          case 59:
            Script_FACE(CHARA_JIMY, 6, 0, 8);
            break;
          case 829:
            Script_FACE(CHARA_JIMY, 4, 2, 8);
            break;
          case 1209:
            Script_FACE(CHARA_JIMY, 4, 1, 8);
            break;
          case 1449:
            Script_FACE(CHARA_JIMY, 4, 0, 8);
            break;
          case 1528:
            Script_FACE(CHARA_JIMY, 4, 1, 8);
            break;
          case 1608:
            Script_FACE(CHARA_JIMY, 4, 0, 8);
            break;
          case 1809:
            Script_FACE(CHARA_JIMY, 4, 1, 8);
            break;
        }
      }
      {
        int32_t local_E4;
        local_E4 = task_0;
        switch (local_E4) {
          case 404:
            local_F4.x = 0.121f;
            (&local_F4)->y = 0.0f;
            (&local_F4)->z = 0.076f;
            local_E8.x = 0;
            (&local_E8)->y = 0;
            (&local_E8)->z = 0x4000;
            Script_fixoAttachEx(SHEN_NAME('TKET'), CHARA_JIMY, 12, &local_F4, &local_E8);
            Script_handSetFloatsFor(CHARA_JIMY, 0, data_4FC0, 1);
            break;
          case 473:
            local_F4.x = 0.121f;
            (&local_F4)->y = 0.0f;
            (&local_F4)->z = -0.076f;
            local_E8.x = 0;
            (&local_E8)->y = 0;
            (&local_E8)->z = 0x4000;
            Script_handSetFloatsFor(CHARA_JIMY, 0, data_408C, 8);
            Script_fixoAttachEx(SHEN_NAME('TKET'), CHARA_AKIR, 18, &local_F4, &local_E8);
            Script_handSetFloatsFor(CHARA_AKIR, 1, data_4FC0, 1);
            break;
          case 568:
            Script_charaDisplay(SHEN_NAME('TKET'), CDM_Hide);
            Script_handSetFloatsFor(CHARA_AKIR, 1, data_408C, 8);
            break;
        }
      }
      if (task_0 == 1960) {
        Script_executeAicaCommand(2720, 1, 30);
        Script_executeAicaCommand(2720, 2, 30);
        Script_executeAicaCommand(2720, 3, 30);
        Script_filmCheckOrSetTransitionUtil(45, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_1A4F(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3i local_D0;
  int16_t local_CE;
  local_CE = 1;
  Script_SetMotnFlag(CHARA_AKIR, -1);
  if (-1) {
    Script_charaDisplay(CHARA_ETKO, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_ETKO, CDM_Show);
      goto L001A8B;
    }
  }
  L001A8B:;
  Script_scenToggleVisibleFor(CHARA_ETKO, 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('INF1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('INF1'), CDM_Show);
      goto L001ABF;
    }
  }
  L001ABF:;
  Script_scenToggleVisibleFor(SHEN_NAME('INF1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('INF2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('INF2'), CDM_Show);
      goto L001AF3;
    }
  }
  L001AF3:;
  Script_scenToggleVisibleFor(SHEN_NAME('INF2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('INF3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('INF3'), CDM_Show);
      goto L001B27;
    }
  }
  L001B27:;
  Script_scenToggleVisibleFor(SHEN_NAME('INF3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('MAGA'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('MAGA'), CDM_Show);
      goto L001B5B;
    }
  }
  L001B5B:;
  Script_scenToggleVisibleFor(SHEN_NAME('MAGA'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('TRAY'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('TRAY'), CDM_Show);
      goto L001B8F;
    }
  }
  L001B8F:;
  Script_scenToggleVisibleFor(SHEN_NAME('TRAY'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('PURS'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('PURS'), CDM_Show);
      goto L001BC3;
    }
  }
  L001BC3:;
  Script_scenToggleVisibleFor(SHEN_NAME('PURS'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('COMP'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('COMP'), CDM_Show);
      goto L001BF7;
    }
  }
  L001BF7:;
  Script_scenToggleVisibleFor(SHEN_NAME('COMP'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('MONY'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('MONY'), CDM_Show);
      goto L001C2B;
    }
  }
  L001C2B:;
  Script_scenToggleVisibleFor(SHEN_NAME('MONY'), 0);
  if (-1) {
    Script_charaDisplay(CHARA_KJHS, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_KJHS, CDM_Show);
      goto L001C5F;
    }
  }
  L001C5F:;
  Script_scenToggleVisibleFor(CHARA_KJHS, 0);
  if (-1) {
    Script_charaDisplay(CHARA_MNWA, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_MNWA, CDM_Show);
      goto L001C93;
    }
  }
  L001C93:;
  Script_scenToggleVisibleFor(CHARA_MNWA, 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('TKET'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('TKET'), CDM_Show);
      goto L001CC7;
    }
  }
  L001CC7:;
  Script_scenToggleVisibleFor(SHEN_NAME('TKET'), 0);
  Script_charaDisplay(SHEN_NAME('TKE2'), CDM_Show);
  Script_scenToggleActiveFor(SHEN_NAME('TKET'), 0);
  Script_scenToggleActiveFor(SHEN_NAME('TKE2'), 0);
  Script_ccowSetHeightMode(SHEN_NAME('TKET'), CCOW_KeepHeight);
  Script_ccowSetHeightMode(SHEN_NAME('TKE2'), CCOW_KeepHeight);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_ResetHandMotion(CHARA_JIMY, 1, 1, 0);
  Script_ResetHandMotion(CHARA_JIMY, 1, 0, 0);
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      switch (local_CE) {
        case 1:
          Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_10);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_4170, 10);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_4170, 10);
          Script_handSetFloatsFor(CHARA_JIMY, 0, data_408C, 10);
          Script_handSetFloatsFor(CHARA_JIMY, 1, data_408C, 10);
          break;
        case 541:
          Script_SetNewHandMotion(CHARA_AKIR, 0, 621, 0, 0, 0, 0, 1.0f, 0);
          break;
      }
      switch (local_CE) {
        case 1:
          local_F4.x = 1.89f;
          (&local_F4)->y = 0.75f;
          (&local_F4)->z = -4.7f;
          Script_EVT_SetPosition(SHEN_NAME('TKE2'), POS_XYZ, &local_F4);
          local_D0.x = 0;
          (&local_D0)->y = 0;
          (&local_D0)->z = 0;
          Script_EVT_SetRotation(SHEN_NAME('TKE2'), 0x38000000, &local_D0, 0);
          Script_charaDisplay(SHEN_NAME('TKE2'), CDM_Show);
          break;
        case 473:
          local_D0.x = -7462;
          (&local_D0)->y = -8372;
          (&local_D0)->z = 5095;
          Script_SetHandRootRotation(CHARA_JIMY, 1, 0x15, &local_D0);
          break;
        case 504:
          local_F4.x = 0.102f;
          (&local_F4)->y = 0.013f;
          (&local_F4)->z = 0.093f;
          local_D0.x = 4182;
          (&local_D0)->y = 58618;
          (&local_D0)->z = 10919;
          Script_fixoAttachEx(SHEN_NAME('TKE2'), CHARA_JIMY, 18, &local_F4, &local_D0);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_408C, 0);
          break;
        case 506:
          local_D0.x = -6005;
          (&local_D0)->y = 5641;
          (&local_D0)->z = 2184;
          Script_SetHandRootRotation(CHARA_JIMY, 0, 0x15, &local_D0);
          local_D0.x = 2910;
          (&local_D0)->y = -7278;
          (&local_D0)->z = 0;
          Script_SetHandRootRotation(CHARA_JIMY, 1, 0x15, &local_D0);
          local_F4.x = 0.129f;
          (&local_F4)->y = 0.013f;
          (&local_F4)->z = 0.093f;
          local_D0.x = 52590;
          (&local_D0)->y = 60980;
          (&local_D0)->z = 1996;
          Script_fixoAttachEx(SHEN_NAME('TKE2'), CHARA_JIMY, 18, &local_F4, &local_D0);
          break;
        case 540:
          Script_fixoClearAttachment(SHEN_NAME('TKE2'));
          Script_charaDisplay(SHEN_NAME('TKE2'), CDM_Hide);
          Script_EVT_GetPosition(CHARA_AKIR, 12, &local_F4, 0);
          local_F4.x = (&local_F4)->x + 0.07f;
          Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F4, 1);
          Script_charaDisplay(SHEN_NAME('TKET'), CDM_Show);
          break;
        case 615:
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_4170, 10);
          break;
      }
      switch (local_CE) {
        case 1:
          Script_FACE(CHARA_AKIR, 0, 0, 5);
          break;
        case 140:
          Script_FACE(CHARA_AKIR, 2, 0, 5);
          break;
        case 241:
          Script_FACE(CHARA_AKIR, 0, 0, 5);
          break;
        case 300:
          Script_FACE(CHARA_AKIR, 2, 5, 5);
          break;
        case 340:
          Script_FACE(CHARA_AKIR, 2, 4, 5);
          break;
        case 431:
          Script_FACE_LCG(CHARA_AKIR, FACE_BlinkAutomatic, 0, 0);
          Script_FACE(CHARA_AKIR, 0, 0, 5);
          break;
        case 506:
          Script_FACE(CHARA_AKIR, 2, 0, 5);
          break;
        case 615:
          Script_FACE(CHARA_AKIR, 0, 0, 5);
          break;
        case 891:
          Script_FACE(CHARA_AKIR, 2, 0, 5);
          break;
        case 966:
          Script_FACE_LCG(CHARA_AKIR, FACE_BlinkAutomatic, 0, 0);
          break;
      }
      if ((local_CE >= 140) & (local_CE <= 240)) {
        Script_EVT_GetPosition(CHARA_JIMY, 5, &local_E8, 0);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_E8, 8);
      } else if (local_CE == 241) {
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
      } else if ((local_CE >= 506) & (local_CE <= 600)) {
        Script_EVT_GetPosition(CHARA_AKIR, 12, &local_E8, 0);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_E8, 8);
      } else {
        switch (local_CE) {
          case 601:
            Script_EVT_GetPosition(CHARA_JIMY, 5, &local_E8, 0);
            (&local_E8)->y = (&local_E8)->y + 1.0f;
            Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_E8, 8);
            break;
          case 941:
            Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
            break;
        }
      }
      if ((local_CE >= 431) & (local_CE <= 470)) {
        Script_EVT_GetPosition(CHARA_AKIR, 5, &local_DC, 0);
        Script_FaceLookAtTargetUtils(CHARA_JIMY, FACE_LookAtPosition, &local_DC, 8);
      } else if (local_CE == 471) {
        Script_FaceLookAtTargetUtils(CHARA_JIMY, FACE_LookReset);
      } else if ((local_CE >= 615) & (local_CE <= 695)) {
        Script_EVT_GetPosition(CHARA_AKIR, 5, &local_DC, 0);
        Script_FaceLookAtTargetUtils(CHARA_JIMY, FACE_LookAtPosition, &local_DC, 8);
      } else if (local_CE == 696) {
        Script_FaceLookAtTargetUtils(CHARA_JIMY, FACE_LookReset);
      } else if ((local_CE >= 796) & (local_CE <= 890)) {
        Script_EVT_GetPosition(CHARA_AKIR, 5, &local_DC, 0);
        Script_FaceLookAtTargetUtils(CHARA_JIMY, FACE_LookAtPosition, &local_DC, 8);
      } else {
        if (local_CE == 891) {
          Script_FaceLookAtTargetUtils(CHARA_JIMY, FACE_LookReset);
          goto L00231C;
        }
      }
      L00231C:;
      switch (local_CE) {
        case 1:
          Script_FACE_LCG(CHARA_JIMY, FACE_BlinkClose, 0, 0);
          Script_FACE(CHARA_JIMY, 6, 0, 5);
          break;
        case 230:
          Script_FACE_LCG(CHARA_JIMY, FACE_BlinkAutomatic, 0, 0);
          break;
        case 321:
          Script_FACE(CHARA_JIMY, 0, 0, 5);
          break;
        case 431:
          Script_FACE(CHARA_JIMY, 3, 0, 5);
          break;
        case 506:
          Script_FACE(CHARA_JIMY, 0, 0, 5);
          break;
        case 615:
          Script_FACE(CHARA_JIMY, 3, 0, 5);
          break;
        case 696:
          Script_FACE(CHARA_JIMY, 6, 0, 5);
          break;
        case 891:
          Script_FACE(CHARA_JIMY, 0, 0, 5);
          break;
        case 966:
          Script_FACE(CHARA_JIMY, 6, 0, 5);
          break;
      }
      if (Script_GetControllerInput(1) == 0x200) {
        Script_shenSeqUtils(-6);
      }
      local_CE = local_CE + 1;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_245A(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3i local_D0;
  int16_t local_CE;
  local_CE = 1;
  Script_SetMotnFlag(CHARA_AKIR, -1);
  if (-1) {
    Script_charaDisplay(CHARA_ETKO, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_ETKO, CDM_Show);
      goto L002496;
    }
  }
  L002496:;
  Script_scenToggleVisibleFor(CHARA_ETKO, 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('INF1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('INF1'), CDM_Show);
      goto L0024CA;
    }
  }
  L0024CA:;
  Script_scenToggleVisibleFor(SHEN_NAME('INF1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('INF2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('INF2'), CDM_Show);
      goto L0024FE;
    }
  }
  L0024FE:;
  Script_scenToggleVisibleFor(SHEN_NAME('INF2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('INF3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('INF3'), CDM_Show);
      goto L002532;
    }
  }
  L002532:;
  Script_scenToggleVisibleFor(SHEN_NAME('INF3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('MAGA'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('MAGA'), CDM_Show);
      goto L002566;
    }
  }
  L002566:;
  Script_scenToggleVisibleFor(SHEN_NAME('MAGA'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('TRAY'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('TRAY'), CDM_Show);
      goto L00259A;
    }
  }
  L00259A:;
  Script_scenToggleVisibleFor(SHEN_NAME('TRAY'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('PURS'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('PURS'), CDM_Show);
      goto L0025CE;
    }
  }
  L0025CE:;
  Script_scenToggleVisibleFor(SHEN_NAME('PURS'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('COMP'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('COMP'), CDM_Show);
      goto L002602;
    }
  }
  L002602:;
  Script_scenToggleVisibleFor(SHEN_NAME('COMP'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('MONY'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('MONY'), CDM_Show);
      goto L002636;
    }
  }
  L002636:;
  Script_scenToggleVisibleFor(SHEN_NAME('MONY'), 0);
  if (-1) {
    Script_charaDisplay(CHARA_KJHS, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_KJHS, CDM_Show);
      goto L00266A;
    }
  }
  L00266A:;
  Script_scenToggleVisibleFor(CHARA_KJHS, 0);
  if (-1) {
    Script_charaDisplay(CHARA_MNWA, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_MNWA, CDM_Show);
      goto L00269E;
    }
  }
  L00269E:;
  Script_scenToggleVisibleFor(CHARA_MNWA, 0);
  Script_charaDisplay(SHEN_NAME('TKET'), CDM_Show);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('TKE2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('TKE2'), CDM_Show);
      goto L0026DD;
    }
  }
  L0026DD:;
  Script_scenToggleVisibleFor(SHEN_NAME('TKE2'), 0);
  Script_scenToggleActiveFor(SHEN_NAME('TKET'), 0);
  Script_scenToggleActiveFor(SHEN_NAME('TKE2'), 0);
  Script_ccowSetHeightMode(SHEN_NAME('TKET'), CCOW_KeepHeight);
  Script_ccowSetHeightMode(SHEN_NAME('TKE2'), CCOW_KeepHeight);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_ResetHandMotion(CHARA_JIMY, 1, 1, 0);
  Script_ResetHandMotion(CHARA_JIMY, 1, 0, 0);
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      switch (local_CE) {
        case 1:
          Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_10);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_4170, 10);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_4170, 10);
          Script_handSetFloatsFor(CHARA_JIMY, 0, data_408C, 10);
          Script_handSetFloatsFor(CHARA_JIMY, 1, data_408C, 10);
          break;
        case 541:
          Script_SetNewHandMotion(CHARA_AKIR, 0, 621, 0, 0, 0, 0, 1.0f, 0);
          break;
      }
      switch (local_CE) {
        case 1:
          local_F4.x = 1.89f;
          (&local_F4)->y = 0.75f;
          (&local_F4)->z = -4.7f;
          Script_EVT_SetPosition(SHEN_NAME('TKET'), POS_XYZ, &local_F4);
          local_D0.x = 0;
          (&local_D0)->y = 0;
          (&local_D0)->z = 0;
          Script_EVT_SetRotation(SHEN_NAME('TKET'), 0x38000000, &local_D0, 0);
          Script_charaDisplay(SHEN_NAME('TKET'), CDM_Show);
          break;
        case 473:
          local_D0.x = -7462;
          (&local_D0)->y = -8372;
          (&local_D0)->z = 5095;
          Script_SetHandRootRotation(CHARA_JIMY, 1, 0x15, &local_D0);
          break;
        case 504:
          local_F4.x = 0.102f;
          (&local_F4)->y = 0.013f;
          (&local_F4)->z = 0.093f;
          local_D0.x = 4182;
          (&local_D0)->y = 58618;
          (&local_D0)->z = 10919;
          Script_fixoAttachEx(SHEN_NAME('TKET'), CHARA_JIMY, 18, &local_F4, &local_D0);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_408C, 0);
          break;
        case 506:
          local_D0.x = -6005;
          (&local_D0)->y = 5641;
          (&local_D0)->z = 2184;
          Script_SetHandRootRotation(CHARA_JIMY, 0, 0x15, &local_D0);
          local_D0.x = 2910;
          (&local_D0)->y = -7278;
          (&local_D0)->z = 0;
          Script_SetHandRootRotation(CHARA_JIMY, 1, 0x15, &local_D0);
          local_D0.x = -4731;
          (&local_D0)->y = 5095;
          (&local_D0)->z = -545;
          Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &local_D0);
          local_F4.x = 0.129f;
          (&local_F4)->y = -0.013f;
          (&local_F4)->z = 0.052f;
          local_D0.x = 52590;
          (&local_D0)->y = 60980;
          (&local_D0)->z = 1996;
          Script_fixoAttachEx(SHEN_NAME('TKET'), CHARA_JIMY, 18, &local_F4, &local_D0);
          break;
        case 540:
          Script_fixoClearAttachment(SHEN_NAME('TKET'));
          Script_EVT_GetPosition(CHARA_AKIR, 12, &local_F4, 0);
          local_F4.x = (&local_F4)->x + 0.07f;
          Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F4, 1);
          local_F4.x = 0.135f;
          (&local_F4)->y = -0.022f;
          (&local_F4)->z = 0.119f;
          local_D0.x = 57525;
          (&local_D0)->y = 41313;
          (&local_D0)->z = 18196;
          Script_fixoAttachEx(SHEN_NAME('TKE2'), CHARA_AKIR, 12, &local_F4, &local_D0);
          Script_charaDisplay(SHEN_NAME('TKE2'), CDM_Show);
          break;
        case 541:
          local_D0.x = 0;
          (&local_D0)->y = 0;
          (&local_D0)->z = 0;
          Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &local_D0);
          local_F4.x = 0.135f;
          (&local_F4)->y = -0.022f;
          (&local_F4)->z = 0.119f;
          local_D0.x = 57525;
          (&local_D0)->y = 41313;
          (&local_D0)->z = 18196;
          Script_charaDisplay(SHEN_NAME('TKET'), CDM_Hide);
          break;
        case 615:
          local_F4.x = 0.14f;
          (&local_F4)->y = 0.018f;
          (&local_F4)->z = 0.111f;
          local_D0.x = 46420;
          (&local_D0)->y = 43499;
          (&local_D0)->z = 30941;
          Script_fixoAttachEx(SHEN_NAME('TKE2'), CHARA_AKIR, 12, &local_F4, &local_D0);
          Script_charaDisplay(SHEN_NAME('TKE2'), CDM_Show);
          break;
        case 696:
          local_F4.x = 0.149f;
          (&local_F4)->y = 0.033f;
          (&local_F4)->z = 0.091f;
          local_D0.x = 29225;
          (&local_D0)->y = 45423;
          (&local_D0)->z = 43527;
          Script_fixoAttachEx(SHEN_NAME('TKE2'), CHARA_AKIR, 12, &local_F4, &local_D0);
          Script_charaDisplay(SHEN_NAME('TKE2'), CDM_Show);
          local_D0.x = -8918;
          (&local_D0)->y = -909;
          (&local_D0)->z = -3821;
          Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &local_D0);
          break;
      }
      switch (local_CE) {
        case 1:
          Script_FACE(CHARA_AKIR, 0, 0, 5);
          break;
        case 140:
          Script_FACE(CHARA_AKIR, 2, 0, 5);
          break;
        case 241:
          Script_FACE(CHARA_AKIR, 0, 0, 5);
          break;
        case 300:
          Script_FACE(CHARA_AKIR, 2, 5, 5);
          break;
        case 321:
          Script_FACE_LCG(CHARA_AKIR, FACE_BlinkClose, 16, 8);
          break;
        case 340:
          Script_FACE(CHARA_AKIR, 2, 4, 5);
          break;
        case 431:
          Script_FACE_LCG(CHARA_AKIR, FACE_BlinkAutomatic, 0, 0);
          Script_FACE(CHARA_AKIR, 0, 0, 5);
          break;
        case 506:
          Script_FACE(CHARA_AKIR, 2, 0, 5);
          break;
        case 615:
          Script_FACE(CHARA_AKIR, 0, 0, 5);
          break;
        case 891:
          Script_FACE_LCG(CHARA_AKIR, FACE_BlinkClose, 16, 4);
          Script_FACE(CHARA_AKIR, 2, 0, 5);
          break;
        case 966:
          Script_FACE_LCG(CHARA_AKIR, FACE_BlinkAutomatic, 0, 0);
          Script_FACE(CHARA_AKIR, 10, 0, 5);
          break;
      }
      if ((local_CE >= 140) & (local_CE <= 240)) {
        Script_EVT_GetPosition(CHARA_JIMY, 5, &local_E8, 0);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_E8, 8);
      } else if (local_CE == 241) {
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
      } else if (local_CE == 321) {
        local_E8.x = 2.39f;
        (&local_E8)->y = -0.98f;
        (&local_E8)->z = -3.4f;
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_E8, 8);
      } else if ((local_CE >= 506) & (local_CE <= 695)) {
        Script_EVT_GetPosition(CHARA_JIMY, 5, &local_E8, 0);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_E8, 8);
      } else if (local_CE == 696) {
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
      } else if ((local_CE >= 796) & (local_CE <= 890)) {
        Script_EVT_GetPosition(CHARA_JIMY, 5, &local_E8, 0);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_E8, 8);
      } else {
        switch (local_CE) {
          case 891:
            Script_EVT_GetPosition(CHARA_AKIR, 12, &local_E8, 0);
            Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_E8, 8);
            break;
          case 1061:
            Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
            break;
        }
      }
      if ((local_CE >= 431) & (local_CE <= 470)) {
        Script_EVT_GetPosition(CHARA_AKIR, 5, &local_DC, 0);
        Script_FaceLookAtTargetUtils(CHARA_JIMY, FACE_LookAtPosition, &local_DC, 8);
      } else if (local_CE == 471) {
        Script_FaceLookAtTargetUtils(CHARA_JIMY, FACE_LookReset);
      } else if ((local_CE >= 615) & (local_CE <= 695)) {
        Script_EVT_GetPosition(CHARA_AKIR, 5, &local_DC, 0);
        Script_FaceLookAtTargetUtils(CHARA_JIMY, FACE_LookAtPosition, &local_DC, 8);
      } else if (local_CE == 696) {
        Script_FaceLookAtTargetUtils(CHARA_JIMY, FACE_LookReset);
      } else if ((local_CE >= 796) & (local_CE <= 890)) {
        Script_EVT_GetPosition(CHARA_AKIR, 5, &local_DC, 0);
        Script_FaceLookAtTargetUtils(CHARA_JIMY, FACE_LookAtPosition, &local_DC, 8);
      } else {
        if (local_CE == 891) {
          Script_FaceLookAtTargetUtils(CHARA_JIMY, FACE_LookReset);
          goto L002FB6;
        }
      }
      L002FB6:;
      switch (local_CE) {
        case 1:
          Script_FACE_LCG(CHARA_JIMY, FACE_BlinkClose, 0, 0);
          Script_FACE(CHARA_JIMY, 6, 0, 5);
          break;
        case 230:
          Script_FACE_LCG(CHARA_JIMY, FACE_BlinkAutomatic, 0, 0);
          break;
        case 321:
          Script_FACE(CHARA_JIMY, 0, 0, 5);
          break;
        case 431:
          Script_FACE(CHARA_JIMY, 3, 0, 5);
          break;
        case 506:
          Script_FACE(CHARA_JIMY, 0, 0, 5);
          break;
        case 615:
          Script_FACE(CHARA_JIMY, 3, 0, 5);
          break;
        case 696:
          Script_FACE(CHARA_JIMY, 6, 0, 5);
          break;
        case 891:
          Script_FACE(CHARA_JIMY, 0, 0, 5);
          break;
        case 966:
          Script_FACE(CHARA_JIMY, 6, 0, 5);
          break;
      }
      if (Script_GetControllerInput(1) == 0x200) {
        Script_shenSeqUtils(-6);
      }
      local_CE = local_CE + 1;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_30F4(void) {
  Vector3i local_F4;
  if (-1) {
    Script_charaDisplay(CHARA_JIMY, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_JIMY, CDM_Show);
      goto L00311E;
    }
  }
  L00311E:;
  Script_scenToggleVisibleFor(CHARA_JIMY, 0);
  if (-1) {
    Script_charaDisplay(CHARA_KJHS, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_KJHS, CDM_Show);
      goto L003152;
    }
  }
  L003152:;
  Script_scenToggleVisibleFor(CHARA_KJHS, 0);
  if (-1) {
    Script_charaDisplay(CHARA_MNWA, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_MNWA, CDM_Show);
      goto L003186;
    }
  }
  L003186:;
  Script_scenToggleVisibleFor(CHARA_MNWA, 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('INF3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('INF3'), CDM_Show);
      goto L0031BA;
    }
  }
  L0031BA:;
  Script_scenToggleVisibleFor(SHEN_NAME('INF3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('TRAY'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('TRAY'), CDM_Show);
      goto L0031EE;
    }
  }
  L0031EE:;
  Script_scenToggleVisibleFor(SHEN_NAME('TRAY'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('MONY'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('MONY'), CDM_Show);
      goto L003222;
    }
  }
  L003222:;
  Script_scenToggleVisibleFor(SHEN_NAME('MONY'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('PURS'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('PURS'), CDM_Show);
      goto L003256;
    }
  }
  L003256:;
  Script_scenToggleVisibleFor(SHEN_NAME('PURS'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('COMP'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('COMP'), CDM_Show);
      goto L00328A;
    }
  }
  L00328A:;
  Script_scenToggleVisibleFor(SHEN_NAME('COMP'), 0);
  Script_ccowSetHeightMode(SHEN_NAME('INF1'), CCOW_KeepHeight);
  Script_ccowSetHeightMode(SHEN_NAME('INF2'), CCOW_KeepHeight);
  Script_ccowSetHeightMode(SHEN_NAME('MAGA'), CCOW_KeepHeight);
  for (;;) {
    {
      int32_t local_F0;
      local_F0 = task_0;
      switch (local_F0) {
        case 1:
          Script_FACE(CHARA_AKIR, 0, 0, 0);
          break;
        case 189:
          Script_FACE(CHARA_AKIR, 3, 0, 27);
          break;
        case 308:
          Script_FACE(CHARA_AKIR, 0, 0, 15);
          break;
        case 452:
          Script_FACE(CHARA_AKIR, 2, 0, 20);
          break;
        case 739:
          Script_FACE(CHARA_AKIR, 0, 0, 20);
          break;
        case 1047:
          Script_FACE(CHARA_AKIR, 1, 0, 10);
          break;
        case 1421:
          Script_FACE(CHARA_AKIR, 0, 0, 0);
          break;
        case 1430:
          Script_FACE(CHARA_AKIR, 3, 0, 10);
          break;
        case 1550:
          Script_FACE(CHARA_AKIR, 0, 0, 20);
          break;
        case 1631:
          Script_FACE(CHARA_AKIR, 3, 0, 20);
          break;
        case 1740:
          Script_FACE(CHARA_AKIR, 7, 0, 10);
          break;
        case 1848:
          Script_FACE(CHARA_AKIR, 2, 0, 20);
          break;
      }
    }
    {
      int32_t local_F0;
      local_F0 = task_0;
      switch (local_F0) {
        case 1:
          Script_CharHandlers(CHARA_AKIR, 2, 8, 16);
          Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
          Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
          break;
        case 1978:
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_408C, 0);
          break;
        case 1995:
          Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
          break;
      }
    }
    {
      int32_t local_F0;
      local_F0 = task_0;
      switch (local_F0) {
        case 1:
          (&local_F4)->z = 0;
          Script_charaPosRotUtils(SHEN_NAME('MAGA'), 152, 0, &local_F4, CHARA_PART_Set);
          Script_charaPosRotUtils(SHEN_NAME('MAGA'), 162, 0, &local_F4, CHARA_PART_Set);
          (&local_F4)->z = -8865;
          Script_charaPosRotUtils(SHEN_NAME('MAGA'), 153, 0, &local_F4, CHARA_PART_Set);
          (&local_F4)->z = 8865;
          Script_charaPosRotUtils(SHEN_NAME('MAGA'), 163, 0, &local_F4, CHARA_PART_Set);
          (&local_F4)->z = -0x1600;
          Script_charaPosRotUtils(SHEN_NAME('MAGA'), 154, 0, &local_F4, CHARA_PART_Set);
          (&local_F4)->z = 0x1600;
          Script_charaPosRotUtils(SHEN_NAME('MAGA'), 164, 0, &local_F4, CHARA_PART_Set);
          (&local_F4)->z = -4109;
          Script_charaPosRotUtils(SHEN_NAME('MAGA'), 155, 0, &local_F4, CHARA_PART_Set);
          (&local_F4)->z = 4109;
          Script_charaPosRotUtils(SHEN_NAME('MAGA'), 165, 0, &local_F4, CHARA_PART_Set);
          Script_ReadModelFormat7(CHARA_ETKO, 0, data_58CC, 0);
          Script_ReadModelFormat7(CHARA_ETKO, 1, data_58F4, 0);
          Script_handSetFloatsFor(CHARA_ETKO, 1, data_5350, 0);
          Script_handSetFloatsFor(CHARA_ETKO, 0, data_5350, 0);
          Script_SetHandRootRotation(CHARA_ETKO, 1, 0x15, &data_58C0);
          break;
        case 133:
          Script_ResetHandMotion(CHARA_ETKO, 0, 0, 0);
          break;
        case 216:
          Script_ResetHandMotion(CHARA_ETKO, 0, 1, 0);
          break;
        case 1093:
          Script_ResetHandMotion(CHARA_ETKO, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_ETKO, 0, data_5350, 0);
          break;
        case 1195:
          Script_ResetHandMotion(CHARA_ETKO, 0, 0, 0);
          Script_ResetHandMotion(CHARA_ETKO, 1, 1, 0);
          Script_handSetFloatsFor(CHARA_ETKO, 1, data_5350, 0);
          Script_SetHandRootRotation(CHARA_ETKO, 1, 0x15, &data_58C0);
          break;
        case 1200:
          Script_CharHandlers(CHARA_ETKO, 1, 1, 16);
          break;
        case 1341:
          Script_ResetHandMotion(CHARA_ETKO, 0, 1, 0);
          break;
      }
    }
    if (task_0 > 2099) {
      Script_executeAicaCommand(2720, 1, 30);
      Script_executeAicaCommand(2720, 2, 30);
      Script_executeAicaCommand(2720, 3, 30);
      Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_374D(void) {
  if (-1) {
    Script_charaDisplay(CHARA_JIMY, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_JIMY, CDM_Show);
      goto L003777;
    }
  }
  L003777:;
  Script_scenToggleVisibleFor(CHARA_JIMY, 0);
  if (-1) {
    Script_charaDisplay(CHARA_KJHS, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_KJHS, CDM_Show);
      goto L0037AB;
    }
  }
  L0037AB:;
  Script_scenToggleVisibleFor(CHARA_KJHS, 0);
  if (-1) {
    Script_charaDisplay(CHARA_MNWA, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_MNWA, CDM_Show);
      goto L0037DF;
    }
  }
  L0037DF:;
  Script_scenToggleVisibleFor(CHARA_MNWA, 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('INF1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('INF1'), CDM_Show);
      goto L003813;
    }
  }
  L003813:;
  Script_scenToggleVisibleFor(SHEN_NAME('INF1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('INF2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('INF2'), CDM_Show);
      goto L003847;
    }
  }
  L003847:;
  Script_scenToggleVisibleFor(SHEN_NAME('INF2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('MAGA'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('MAGA'), CDM_Show);
      goto L00387B;
    }
  }
  L00387B:;
  Script_scenToggleVisibleFor(SHEN_NAME('MAGA'), 0);
  Script_ccowSetHeightMode(SHEN_NAME('INF3'), CCOW_KeepHeight);
  Script_ccowSetHeightMode(SHEN_NAME('TRAY'), CCOW_KeepHeight);
  Script_ccowSetHeightMode(SHEN_NAME('MONY'), CCOW_KeepHeight);
  Script_ccowSetHeightMode(SHEN_NAME('PURS'), CCOW_KeepHeight);
  for (;;) {
    {
      int32_t local_FC;
      local_FC = task_0;
      switch (local_FC) {
        case 1:
          Script_FACE(CHARA_AKIR, 0, 0, 0);
          break;
        case 805:
          Script_FACE(CHARA_AKIR, 7, 0, 6);
          break;
        case 847:
          Script_FACE(CHARA_AKIR, 2, 0, 15);
          break;
        case 983:
          Script_FACE(CHARA_AKIR, 7, 0, 17);
          break;
        case 1424:
          Script_FACE(CHARA_AKIR, 0, 0, 0);
          break;
        case 1486:
          Script_FACE(CHARA_AKIR, 7, 0, 20);
          break;
        case 1701:
          Script_FACE(CHARA_AKIR, 3, 0, 20);
          break;
      }
    }
    {
      int32_t local_FC;
      local_FC = task_0;
      switch (local_FC) {
        case 750:
          Script_clmdSetFlags(CHARA_AKIR, 2, 1);
          break;
        case 1100:
          Script_clmdSetFlags(CHARA_AKIR, 2, 0);
          break;
      }
    }
    if ((task_0 >= 1041) & (task_0 <= 1298)) {
      Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_5984);
      Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
      Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_5188, 0);
      Script_handSetFloatsFor(CHARA_AKIR, 0, data_3FA8, 0);
    } else {
      Script_CharHandlers(CHARA_AKIR, 2, 8, 16);
      Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
      Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
    }
    {
      int32_t local_FC;
      local_FC = task_0;
      switch (local_FC) {
        case 1086:
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          break;
        case 1094:
          Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
          break;
        case 1125:
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          break;
        case 1235:
          Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
          break;
        case 1252:
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          break;
        case 1299:
          Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
          break;
      }
    }
    {
      int32_t local_FC;
      local_FC = task_0;
      switch (local_FC) {
        case 1091:
          Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
          break;
        case 1165:
          Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
          break;
        case 1175:
          Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
          break;
        case 1223:
          Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
          break;
        case 1256:
          Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
          break;
        case 1282:
          Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
          break;
      }
    }
    {
      int32_t local_FC;
      local_FC = task_0;
      switch (local_FC) {
        case 2:
          Script_ReadModelFormat7(CHARA_ETKO, 0, data_5934, 0);
          Script_ReadModelFormat7(CHARA_ETKO, 1, data_595C, 0);
          Script_charaDisplay(SHEN_NAME('COMP'), CDM_Show);
          Script_fixoAttachEx(SHEN_NAME('COMP'), CHARA_ETKO, 12, &data_591C, &data_5928);
          Script_handSetFloatsFor(CHARA_ETKO, 1, data_57C4, 0);
          Script_handSetFloatsFor(CHARA_ETKO, 0, data_5518, 0);
          break;
        case 647:
          Script_handSetFloatsFor(CHARA_ETKO, 1, data_5434, 8);
          break;
        case 727:
          Script_SetHandRootRotation(CHARA_ETKO, 1, 0x15, &data_5990);
          Script_handSetFloatsFor(CHARA_ETKO, 1, data_56E0, 8);
          break;
        case 1040:
          Script_ResetHandMotion(CHARA_ETKO, 0, 0, 0);
          break;
        case 1070:
          Script_handSetFloatsFor(CHARA_ETKO, 1, data_55FC, 16);
          break;
        case 1089:
          Script_ResetHandMotion(CHARA_ETKO, 0, 1, 0);
          break;
        case 1298:
          Script_handSetFloatsFor(CHARA_ETKO, 0, data_5518, 0);
          break;
        case 1445:
          Script_handSetFloatsFor(CHARA_ETKO, 1, data_57C4, 8);
          break;
        case 1590:
          Script_ResetHandMotion(CHARA_ETKO, 0, 1, 0);
          Script_ResetHandMotion(CHARA_ETKO, 0, 0, 0);
          break;
      }
    }
    if ((task_0 > 1040) & (task_0 < 1220)) {
      int32_t unused_FC;
      Vector3 local_F0;
      Script_EVT_GetPosition(CHARA_AKIR, 5, &local_F0, 0);
      Script_FaceLookAtTargetUtils(CHARA_ETKO, FACE_LookAtPosition, &local_F0, 4);
    } else {
      if (task_0 == 1220) {
        Script_FaceLookAtTargetUtils(CHARA_ETKO, FACE_LookReset);
        goto L003DAD;
      }
    }
    L003DAD:;
    if (task_0 > 1968) {
      Script_executeAicaCommand(2720, 1, 30);
      Script_executeAicaCommand(2720, 2, 30);
      Script_executeAicaCommand(2720, 3, 30);
      Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

void script_3E10(void) {
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_LoadSND(0, 4, "a1_hijym.snd", 0);
  Script_LoadSND(0, 2, "bgm063.snd", 0);
  Script_executeAicaCommand(18088, 0, 0);
  Script_executeAicaCommand(1184, 2, 107);
  Script_LoadVoiceFile(1, "A0174");
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "scene/03/ARAR", "ARAR") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_scenCreateTask(script_3EB9, 1, CHARA_AKIR);
  return;
}

static int32_t script_3EB9(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Script_SetFogColour(&data_59D0);
  Script_ryoSetChara(arg0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  script_0ACC();
  local_FC = Script_scenCreateTask(script_14AA, 0);
  task_0 = 0;
  local_F8 = Script_scenCreateTask(script_0B02, 0);
  Script_set_val_to_1(3, 0);
  Script_shenSeqUtils(0);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_SA1116_DOJO_TALK_DONE)) {
    script_039D();
  }
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_ARAR_POST_GMCT_STAGE_ACTIVE, 1);
  Script_shenRequestGameModuleTransition(3, SHEN_NAME('D000'), 2);
  SCNF_WaitFrames_0000(1);
  end:;
}
