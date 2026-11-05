#include "act02_mksg.h"

static int32_t script_0030(int32_t arg0) {
  if (arg0 < 0) {
    return -arg0;
  } else {
    return arg0;
  }
}

static void script_004E(float arg0) {
  if (!(arg0 >= 0.0f)) {
    (void)((float)0 - arg0);
    return;
  } else {
    (void)arg0;
    return;
  }
}

static int32_t script_006D(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_FC = arg1 - arg0;
  local_F8 = arg0 - arg1;
  if ((local_F8 & 0xFFFF) < (local_FC & 0xFFFF)) {
    if (arg0 > arg1) {
      local_F4 = local_F8 * -1;
    } else {
      local_F4 = (local_F8 & 0xFFFF) * -1;
    }
  } else if (arg0 > arg1) {
    local_F4 = local_FC & 0xFFFF;
  } else {
    local_F4 = local_FC & 0xFFFF;
  }
  return local_F4;
}

static void script_00E5(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  int32_t local_FC;
  if (task_160 != arg0) {
    Script_sprintf("Door Index = %d : call_type = %d : type = %d\012", task_48[arg2], arg3, arg4);
    local_FC = arg1;
    switch (local_FC) {
      case 40731:
        Script_sprintf("Door Open Motion = DEB_AKESIME_FUSUMA_L_F\012");
        break;
      case 40732:
        Script_sprintf("Door Open Motion = DEB_AKESIME_FUSUMA_R_F\012");
        break;
      case 40705:
        Script_sprintf("Door Open Motion = DEB_HIKISIME_DOOR_L_F\012");
        break;
      case 40706:
        Script_sprintf("Door Open Motion = DEB_HIKISIME_DOOR_R_F\012");
        break;
      case 40733:
        Script_sprintf("Door Open Motion = DEB_OSISIME_DOOR_L_F\012");
        break;
      case 40707:
        Script_sprintf("Door Open Motion = DEB_OSISIME_DOOR_R_F\012");
        break;
      case 40734:
        Script_sprintf("Door Open Motion = GAK_AKESIME_FUSUMA_L_F\012");
        break;
      case 40735:
        Script_sprintf("Door Open Motion = GAK_AKESIME_FUSUMA_R_F\012");
        break;
      case 40708:
        Script_sprintf("Door Open Motion = GAK_HIKISIME_DOOR_L_F\012");
        break;
      case 40709:
        Script_sprintf("Door Open Motion = GAK_HIKISIME_DOOR_R_F\012");
        break;
      case 40736:
        Script_sprintf("Door Open Motion = GAK_OSISIME_DOOR_L_F\012");
        break;
      case 40710:
        Script_sprintf("Door Open Motion = GAK_OSISIME_DOOR_R_F\012");
        break;
      case 40737:
        Script_sprintf("Door Open Motion = JIJ_AKESIME_FUSUMA_L_F\012");
        break;
      case 40738:
        Script_sprintf("Door Open Motion = JIJ_AKESIME_FUSUMA_R_F\012");
        break;
      case 40711:
        Script_sprintf("Door Open Motion = JIJ_HIKISIME_DOOR_L_F\012");
        break;
      case 40712:
        Script_sprintf("Door Open Motion = JIJ_HIKISIME_DOOR_R_F\012");
        break;
      case 40739:
        Script_sprintf("Door Open Motion = JIJ_OSISIME_DOOR_L_F\012");
        break;
      case 40713:
        Script_sprintf("Door Open Motion = JIJ_OSISIME_DOOR_R_F\012");
        break;
      case 40740:
        Script_sprintf("Door Open Motion = KOD_AKESIME_FUSUMA_L_F\012");
        break;
      case 40741:
        Script_sprintf("Door Open Motion = KOD_AKESIME_FUSUMA_R_F\012");
        break;
      case 40714:
        Script_sprintf("Door Open Motion = KOD_HIKISIME_DOOR_L_F\012");
        break;
      case 40715:
        Script_sprintf("Door Open Motion = KOD_HIKISIME_DOOR_R_F\012");
        break;
      case 40742:
        Script_sprintf("Door Open Motion = KOD_OSISIME_DOOR_L_F\012");
        break;
      case 40716:
        Script_sprintf("Door Open Motion = KOD_OSISIME_DOOR_R_F\012");
        break;
      case 40743:
        Script_sprintf("Door Open Motion = OTH_AKESIME_FUSUMA_L_F\012");
        break;
      case 40744:
        Script_sprintf("Door Open Motion = OTH_AKESIME_FUSUMA_R_F\012");
        break;
      case 40717:
        Script_sprintf("Door Open Motion = OTH_HIKISIME_DOOR_L_F\012");
        break;
      case 40718:
        Script_sprintf("Door Open Motion = OTH_HIKISIME_DOOR_R_F\012");
        break;
      case 40745:
        Script_sprintf("Door Open Motion = OTH_OSISIME_DOOR_L_F\012");
        break;
      case 40719:
        Script_sprintf("Door Open Motion = OTH_OSISIME_DOOR_R_F\012");
        break;
      case 40746:
        Script_sprintf("Door Open Motion = PNW_AKESIME_FUSUMA_L_F\012");
        break;
      case 40747:
        Script_sprintf("Door Open Motion = PNW_AKESIME_FUSUMA_R_F\012");
        break;
      case 40720:
        Script_sprintf("Door Open Motion = PNW_HIKISIME_DOOR_L_F\012");
        break;
      case 40721:
        Script_sprintf("Door Open Motion = PNW_HIKISIME_DOOR_R_F\012");
        break;
      case 40748:
        Script_sprintf("Door Open Motion = PNW_OSISIME_DOOR_L_F\012");
        break;
      case 40722:
        Script_sprintf("Door Open Motion = PNW_OSISIME_DOOR_R_F\012");
        break;
      case 40749:
        Script_sprintf("Door Open Motion = SIN_AKESIME_FUSUMA_L_F\012");
        break;
      case 40750:
        Script_sprintf("Door Open Motion = SIN_AKESIME_FUSUMA_R_F\012");
        break;
      case 40723:
        Script_sprintf("Door Open Motion = SIN_HIKISIME_DOOR_L_F\012");
        break;
      case 40724:
        Script_sprintf("Door Open Motion = SIN_HIKISIME_DOOR_R_F\012");
        break;
      case 40751:
        Script_sprintf("Door Open Motion = SIN_OSISIME_DOOR_L_F\012");
        break;
      case 40725:
        Script_sprintf("Door Open Motion = SIN_OSISIME_DOOR_R_F\012");
        break;
      case 40757:
        Script_sprintf("Door Open Motion = SYP_AKESIME_FUSUMA_L_F\012");
        break;
      case 40752:
        Script_sprintf("Door Open Motion = SYP_AKESIME_FUSUMA_R_F\012");
        break;
      case 40726:
        Script_sprintf("Door Open Motion = SYP_HIKISIME_DOOR_L_F\012");
        break;
      case 40727:
        Script_sprintf("Door Open Motion = SYP_HIKISIME_DOOR_R_F\012");
        break;
      case 40753:
        Script_sprintf("Door Open Motion = SYP_OSISIME_DOOR_L_F\012");
        break;
      case 40728:
        Script_sprintf("Door Open Motion = SYP_OSISIME_DOOR_R_F\012");
        break;
      case 40758:
        Script_sprintf("Door Open Motion = YKI_AKESIME_FUSUMA_L_F\012");
        break;
      case 40754:
        Script_sprintf("Door Open Motion = YKI_AKESIME_FUSUMA_R_F\012");
        break;
      case 40729:
        Script_sprintf("Door Open Motion = YKI_HIKISIME_DOOR_L_F\012");
        break;
      case 40730:
        Script_sprintf("Door Open Motion = YKI_HIKISIME_DOOR_R_F\012");
        break;
      case 40755:
        Script_sprintf("Door Open Motion = YKI_OSISIME_DOOR_L_F\012");
        break;
      case 40756:
        Script_sprintf("Door Open Motion = YKI_OSISIME_DOOR_R_F\012");
        break;
      case 40759:
        Script_sprintf("Door Open Motion = JIJ_OSISIME_DOOR_AP_L_F\012");
        break;
      case 40760:
        Script_sprintf("Door Open Motion = OTH_OSISIME_DOOR_AP_L_F\012");
        break;
      case 40761:
        Script_sprintf("Door Open Motion = PNW_OSISIME_DOOR_AP_L_F\012");
        break;
      case 40762:
        Script_sprintf("Door Open Motion = SYP_OSISIME_DOOR_AP_L_F\012");
        break;
      case 40763:
        Script_sprintf("Door Open Motion = YKI_OSISIME_DOOR_AP_L_F\012");
        break;
    }
  }
  return;
}

static int32_t script_0697(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = arg0;
  if (local_F8 == 0) {
    local_FC = 594;
  } else if (local_F8 == 1) {
    local_FC = 595;
  } else if (local_F8 == 2) {
    local_FC = 596;
  } else if (local_F8 == 3) {
    local_FC = 598;
  } else if (local_F8 == 4) {
    local_FC = 599;
  } else if (local_F8 == 5) {
    local_FC = 600;
  } else if (local_F8 == 6) {
    local_FC = 1000;
  } else if (local_F8 == 7) {
    local_FC = 656;
  } else if (local_F8 == 8) {
    local_FC = 601;
  } else if (local_F8 == 9) {
    local_FC = 594;
  } else {
    local_FC = 594;
  }
  return local_FC;
}

static void script_0768(int32_t arg0, int32_t arg1, int32_t arg2) {
  script_948B(arg0, arg2, 0);
  return;
}

static void script_0778(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_8C20(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    script_3F04(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_948B(arg0, arg2, 0);
  return;
}

static void script_087C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  float local_F4;
  float local_F0;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    local_F4 = 90.0f - local_F4;
    *(int32_t *)&local_F0 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    if ((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 0x80)) {
      if (Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 0x40) {
        if ((local_FC == 0) | (local_FC == 1)) {
          local_F8 = 6;
        } else {
          local_F8 = 4;
        }
      } else if ((local_FC == 2) | (local_FC == 3)) {
        local_F8 = 6;
      } else {
        local_F8 = 4;
      }
    } else {
      local_F8 = 4;
    }
    local_FC = script_8C20(arg0, arg1, local_F8, local_F4 / (float)100, local_F0 / (float)100, arg3, task_48[arg2]);
    script_3F04(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_948B(arg0, arg2, 0);
  return;
}

static void script_0A1E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_8C20(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    if ((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 4) == 0) {
      script_2411(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_160) {
        script_57E4(arg0, arg1, arg2, arg3, local_FC, 0);
      }
    }
  }
  script_948B(arg0, arg2, 2);
  return;
}

static void script_0B7F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_8C20(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    if ((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 4) == 0) {
      script_2411(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_160) {
        script_57E4(arg0, arg1, arg2, arg3, local_FC, 0);
      }
    }
  }
  script_948B(arg0, arg2, 2);
  return;
}

static void script_0CE0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    if ((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 4) == 0) {
      float local_F4;
      float local_F0;
      local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_F4 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
      *(int32_t *)&local_F0 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
      if ((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 0x80)) {
        if (Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 0x40) {
          if ((local_FC == 0) | (local_FC == 1)) {
            local_F8 = 6;
          } else {
            local_F8 = 4;
          }
        } else if ((local_FC == 2) | (local_FC == 3)) {
          local_F8 = 6;
        } else {
          local_F8 = 4;
        }
      } else {
        local_F8 = 4;
      }
      local_FC = script_8C20(arg0, arg1, local_F8, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_48[arg2]);
      script_2411(arg0, arg1, arg2, arg3, local_FC, 1);
    } else {
      if (arg0 == task_160) {
        float local_F4;
        float local_F0;
        local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        *(int32_t *)&local_F4 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
        *(int32_t *)&local_F0 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
        local_FC = script_8C20(arg0, arg1, 6, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_48[arg2]);
        script_57E4(arg0, arg1, arg2, arg3, local_FC, 1);
      }
    }
  }
  script_948B(arg0, arg2, 2);
  return;
}

static void script_0F7E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    script_8C20(arg0, arg1, 0, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
  }
  script_948B(arg0, arg2, 0);
  return;
}

static void script_104C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_8C20(arg0, arg1, 4, 0.5f, 0.5f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    script_3390(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_948B(arg0, arg2, 1);
  return;
}

static void script_10B3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_8C20(arg0, arg1, 4, 0.5f, 0.5f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    script_3390(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_948B(arg0, arg2, 1);
  return;
}

static void script_111A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    script_8C20(arg0, arg1, 4, 0.0f, 0.5f, arg3, -1);
    script_3390(arg0, arg1, arg2, arg3, 4, 1);
  }
  script_948B(arg0, arg2, 1);
  return;
}

static void script_1161(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    if (local_FC == 0) {
      local_FC = script_8C20(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    } else {
      local_FC = script_8C20(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    }
    script_384A(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_948B(arg0, arg2, 2);
  return;
}

static void script_1297(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  Vector3i local_F0;
  Vector3i local_E4;
  Vector3i local_D8;
  Vector3 local_CC;
  Vector3 local_C0;
  Vector3 local_B4;
  Vector3 local_A8;
  Vector3 local_9C;
  float local_98;
  float local_94;
  float local_90;
  float local_8C;
  int32_t local_88;
  int32_t local_84;
  int32_t unused_80;
  int8_t local_FF7F;
  uint8_t unused_FF7C[3];
  float local_FF78;
  float local_FF74;
  if (arg0 == task_160) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      local_FF7F = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_FF78 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9 + 5);
      *(int32_t *)&local_FF74 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9 + 6);
      local_FF7F = script_8C20(arg0, arg1, 5, local_FF78 / (float)100, local_FF74 / (float)100, arg3, -1);
      script_92E9(arg0, 0);
      *(int32_t *)&local_94 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9 + 1);
      *(int32_t *)&local_90 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9 + 2);
      local_8C = 0.0f;
      local_FC = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9);
      local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_94 - 3.0f) << 16) & 0x7FFF0000);
      local_9C.x = (&task_168)->x;
      (&local_9C)->y = task_168.y;
      (&local_9C)->z = task_168.z;
      Script_charaPosRotUtils(arg1, 12, &local_CC, &local_E4, CHARA_PART_Get);
      Script_charaPosRotUtils(arg1, 7, &local_C0, &local_D8, CHARA_PART_Get);
      Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
      if (arg0 == task_160) {
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
        Script_scenCharaSubmitMoveTargetRequest(arg0, &task_168, *(&task_174 + 1), local_FC, 0);
        for (;;) {
          if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
            break;
          }
          if (Script_IsMoveTargetRequestActive(arg0) == 0) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      } else {
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
        Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), local_FC);
        for (;;) {
          if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
            break;
          }
          if (Script_GetCharaMoveStatus(arg0) == 0) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      }
      for (;;) {
        if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
          break;
        }
        if (Script_IsCharaMotionFinished(arg0)) {
          break;
        }
        local_98 = Script_GetCharaMotionRemainingFramesScaled(arg0);
        Script_ReadMOTNPositionAtFrame(arg0, local_FC, local_98, &local_A8);
        if (!(local_98 < local_94) & !(local_98 > local_90)) {
          if (local_FF7F == 1) {
            if (!(local_98 < local_94) & !(local_98 >= local_94 + 1.0f)) {
              int32_t unused_FF70;
              int32_t local_FF6C;
              local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
              local_FF6C = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_FF6C * 9 + 1) * 4 + 2);
              if (local_FF6C != -1) {
                if (arg0 == task_160) {
                  Script_executeAicaCommand(local_FF6C, 0, 0);
                } else {
                  Script_Execute3DSoundCommand_Wrapper(local_FF6C, 4, &local_9C, 5.0f, 20.0f, 0);
                }
              }
            }
            Script_EVT_GetPosition(arg0, 12, &local_B4, ENTITY_XFORM_MOTION_SPACE);
            (&local_B4)->z = (&local_B4)->z + (&local_A8)->z;
            (&local_F0)->y = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_C0)->x - (&local_B4)->x, (&local_C0)->z - (&local_B4)->z);
            (&local_F0)->y = (&local_F0)->y - (int32_t)((local_8C + 95.0f) / 360.0f * (float)0x10000);
            local_F0.x = (&local_D8)->x;
            (&local_F0)->z = (&local_D8)->z;
            Script_charaPosRotUtils(arg1, 7, 0, &local_F0, CHARA_PART_Set);
            Script_EVT_GetPosition(arg0, 18, &local_B4, ENTITY_XFORM_MOTION_SPACE);
            (&local_B4)->z = (&local_B4)->z + (&local_A8)->z;
            (&local_F0)->y = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_CC)->x - (&local_B4)->x, (&local_CC)->z - (&local_B4)->z);
            (&local_F0)->y = (&local_F0)->y + (int32_t)((local_8C + 95.0f) / 360.0f * (float)0x10000);
            local_F0.x = (&local_E4)->x;
            (&local_F0)->z = (&local_E4)->z;
            Script_charaPosRotUtils(arg1, 12, 0, &local_F0, CHARA_PART_Set);
          } else {
            if (!(local_98 < local_94) & !(local_98 >= local_94 + 1.0f)) {
              int32_t unused_FF70;
              int32_t local_FF6C;
              local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
              local_FF6C = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_FF6C * 9 + 1) * 4 + 2);
              if (local_FF6C != -1) {
                if (arg0 == task_160) {
                  Script_executeAicaCommand(local_FF6C, 0, 0);
                } else {
                  Script_Execute3DSoundCommand_Wrapper(local_FF6C, 4, &local_9C, 5.0f, 20.0f, 0);
                }
              }
            }
            Script_EVT_GetPosition(arg0, 12, &local_B4, ENTITY_XFORM_MOTION_SPACE);
            (&local_B4)->z = (&local_B4)->z + (&local_A8)->z;
            (&local_F0)->y = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_C0)->x - (&local_B4)->x, (&local_C0)->z - (&local_B4)->z);
            (&local_F0)->y = -((&local_F0)->y - (int32_t)((local_8C + 95.0f) / 360.0f * (float)0x10000));
            local_F0.x = (&local_D8)->x;
            (&local_F0)->z = (&local_D8)->z;
            Script_charaPosRotUtils(arg1, 7, 0, &local_F0, CHARA_PART_Set);
            Script_EVT_GetPosition(arg0, 18, &local_B4, ENTITY_XFORM_MOTION_SPACE);
            (&local_B4)->z = (&local_B4)->z + (&local_A8)->z;
            (&local_F0)->y = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_CC)->x - (&local_B4)->x, (&local_CC)->z - (&local_B4)->z);
            (&local_F0)->y = -((&local_F0)->y + (int32_t)((local_8C + 95.0f) / 360.0f * (float)0x10000));
            local_F0.x = (&local_E4)->x;
            (&local_F0)->z = (&local_E4)->z;
            Script_charaPosRotUtils(arg1, 12, 0, &local_F0, CHARA_PART_Set);
          }
        }
        if (!(local_98 < local_90 + 10.0f)) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      script_92E9(arg0, 1);
      local_8C = Script_MathUtils(SCRIPT_MATH_INT_TO_FLOAT, (&local_F0)->y);
      local_8C = local_8C / 182.044449f;
      if (!(local_8C <= 180.0f)) {
        local_8C = 360.0f - local_8C;
      }
      if (!(local_8C >= -180.0f)) {
        local_8C = local_8C + 360.0f;
      }
      local_88 = 0x4000;
      local_84 = 0;
      local_90 = 1.0f;
      local_FC = 0;
      {
        int32_t unused_FF70;
        int32_t local_FF6C;
        local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
        local_FF6C = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_FF6C * 9 + 1) * 4 + 3);
        if (local_FF6C != -1) {
          if (arg0 == task_160) {
            Script_executeAicaCommand(local_FF6C, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_FF6C, 4, &local_9C, 5.0f, 20.0f, 0);
          }
        }
      }
      for (;;) {
        local_94 = local_8C * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_88) * local_90;
        local_90 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_84);
        if (local_FC == 0) {
          local_88 = local_88 + 0x200;
          if (local_88 >= 0xC000) {
            local_FC = 1;
          }
        } else {
          local_88 = local_88 - 0x200;
          if (local_88 <= 0x4000) {
            local_FC = 0;
          }
        }
        local_84 = local_84 + 128;
        if (local_FF7F == 1) {
          (&local_F0)->y = -(int32_t)(((float)0 - local_94) / 360.0f * (float)0x10000);
          Script_charaPosRotUtils(arg1, 7, 0, &local_F0, CHARA_PART_Set);
          (&local_F0)->y = (int32_t)(((float)0 - local_94) / 360.0f * (float)0x10000);
          Script_charaPosRotUtils(arg1, 12, 0, &local_F0, CHARA_PART_Set);
        } else {
          (&local_F0)->y = (int32_t)(((float)0 - local_94) / 360.0f * (float)0x10000);
          Script_charaPosRotUtils(arg1, 7, 0, &local_F0, CHARA_PART_Set);
          (&local_F0)->y = -(int32_t)(((float)0 - local_94) / 360.0f * (float)0x10000);
          Script_charaPosRotUtils(arg1, 12, 0, &local_F0, CHARA_PART_Set);
        }
        SCNF_WaitFrames_0000(1);
        if (local_84 >= 0x4000) {
          break;
        }
      }
    }
    script_948B(arg0, arg2, 0);
  } else {
    script_087C(arg0, arg1, arg2, arg3);
  }
  return;
}

static void script_1B58(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  int32_t local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    local_F8 = 98.0f - local_F8;
    local_F4 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    if ((local_FC == 0) | (local_FC == 3)) {
      local_FC = script_8C20(arg0, arg1, 4, 0.34736f, 0.73184f, arg3, -1);
    } else {
      local_FC = script_8C20(arg0, arg1, 4, 0.4f, 0.712f, arg3, -1);
    }
    script_5F05(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_948B(arg0, arg2, 0);
  return;
}

static int32_t script_1C76(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  Vector3 local_E4;
  Vector3i local_D8;
  float local_D4;
  float local_D0;
  float local_CC;
  int32_t unused_C8;
  GameDateTime local_C0;
  int32_t local_BC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_F4 = Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 6);
    if ((local_F4 == 6) | (local_F4 == 7)) {
      Script_ReturnTimeDate(&local_C0);
      if (((&local_C0)->hours >= 22) | ((&local_C0)->hours <= 4)) {
        local_F0 = 1;
        local_F4 = 2;
      } else {
        local_F0 = 0;
      }
    } else {
      local_F0 = 0;
    }
    local_BC = local_F4;
    if (local_BC == 2) {
      local_BC = arg3;
      if ((local_BC == 17) | (local_BC == 11) | (local_BC == 12)) {
      } else {
        local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        local_FC = script_8C20(arg0, arg1, 7, 0.45f, 0.0f, arg3, -1);
        local_BC = arg3;
        if ((local_BC == 2) | (local_BC == 4) | (local_BC == 6) | (local_BC == 8) | (local_BC == 10) | (local_BC == 12) | (local_BC == 24) | (local_BC == 26) | (local_BC == 31) | (local_BC == 33) | (local_BC == 35)) {
          local_D4 = -0.45f;
        } else if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 17) | (local_BC == 18) | (local_BC == 19) | (local_BC == 20) | (local_BC == 21) | (local_BC == 22) | (local_BC == 23) | (local_BC == 29)) {
          local_BC = local_FC;
          if ((local_BC == 0) | (local_BC == 1)) {
            local_D4 = -0.45f;
          } else {
            if ((local_BC == 2) | (local_BC == 3)) {
              local_D4 = 0.45f;
              goto L001E32;
            }
          }
          L001E32:;
        } else {
          local_D4 = 0.45f;
        }
        Script_EVT_GetPosition(arg1, -1, &local_E4, 0);
        Script_mtwkGetSetRotation(arg1, &local_D8, 0);
        local_E4.x = (&local_E4)->x + local_D4 * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_D8)->y & 0xFFFF);
        (&local_E4)->z = (&local_E4)->z - local_D4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_D8)->y & 0xFFFF);
        *(&task_174 + 1) = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_E4)->x - (&task_168)->x, (&local_E4)->z - task_168.z);
        script_668F(arg0, arg1, arg2, arg3, local_FC, local_F0);
      }
    } else if ((local_BC == 3) | (local_BC == 4) | (local_BC == 5) | (local_BC == 6)) {
      *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_147D0, SCND_ReadFloatBits, Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 6) * 5 + 3);
      *(int32_t *)&local_CC = Script_ScenDataUtil(&data_147D0, SCND_ReadFloatBits, Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 6) * 5 + 4);
      local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      local_BC = arg3;
      if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 20) | (local_BC == 23) | (local_BC == 28) | (local_BC == 29)) {
        if ((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 0x80)) {
          if (Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 0x40) {
            if ((local_FC == 0) | (local_FC == 1)) {
              local_F8 = 6;
            } else {
              local_F8 = 4;
            }
          } else if ((local_FC == 2) | (local_FC == 3)) {
            local_F8 = 6;
          } else {
            local_F8 = 4;
          }
        } else {
          local_F8 = 4;
        }
      } else {
        local_F8 = 5;
      }
      local_FC = script_8C20(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_48[arg2]);
      script_6767(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (local_BC == 7) {
        local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        local_BC = arg3;
        if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 20) | (local_BC == 23) | (local_BC == 28) | (local_BC == 29)) {
          if ((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 0x80)) {
            if (Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 0x40) {
              if ((local_FC == 0) | (local_FC == 1)) {
                local_F8 = 6;
              } else {
                local_F8 = 4;
              }
            } else if ((local_FC == 2) | (local_FC == 3)) {
              local_F8 = 6;
            } else {
              local_F8 = 4;
            }
          } else {
            local_F8 = 4;
          }
        } else {
          local_F8 = 5;
        }
        if (Script_CycleManagerUtils(CYCLE_CMD_IS_DOOR_OR_MOBJ_TYPE2, Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 12)) == 1) {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_147D0, SCND_ReadFloatBits, Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_147D0, SCND_ReadFloatBits, Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_8C20(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_48[arg2]);
          Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
          script_6BDE(arg0, arg1, arg2, arg3, local_FC, 0);
          Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
        } else {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_147D0, SCND_ReadFloatBits, Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_147D0, SCND_ReadFloatBits, Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_8C20(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_48[arg2]);
          script_6767(arg0, arg1, arg2, arg3, local_FC, 0);
        }
        goto L00232D;
      }
    }
  }
  L00232D:;
  script_948B(arg0, arg2, 0);
  end:;
}

static void script_233B(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(arg0, &local_E8, 0x42000000);
  if ((arg1 == 5) | (arg1 == 6)) {
    local_F4.x = (&local_F4)->x - Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y) * 0.4f;
    (&local_F4)->z = (&local_F4)->z - Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y) * 0.4f;
  } else {
    local_F4.x = (&local_F4)->x - Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y) * 0.3f;
    (&local_F4)->z = (&local_F4)->z - Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y) * 0.3f;
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &local_F4, (&local_E8)->y, script_0697(arg1));
  }
  return;
}

__attribute__((scn_string_bindings(0, data_152EE)))
static void script_2411(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int8_t local_FF;
  uint8_t unused_FC[3];
  Vector3 local_F0;
  Vector3 local_E4;
  Vector3 local_D8;
  Vector3 local_CC;
  Vector3 local_C0;
  float local_BC;
  float local_B8;
  float local_B4;
  float local_B0;
  float local_AC;
  int32_t local_A8;
  float local_A4;
  int32_t local_A0;
  float local_9C;
  float local_98;
  float local_94;
  float local_90;
  float local_8C;
  float local_88;
  int16_t local_86;
  int16_t local_84;
  int32_t local_80;
  int32_t local_FF7C;
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_8C = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_88 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 7);
  if (arg5) {
    Script_charaPosRotUtils(arg1, 12, &local_D8, 0, CHARA_PART_Get);
    Script_charaPosRotUtils(arg1, 7, &local_CC, 0, CHARA_PART_Get);
  } else {
    Script_EVT_GetPosition(arg1, -1, &local_D8, 0);
    Script_EVT_GetPosition(arg1, -1, &local_CC, 0);
  }
  local_F0.x = 0.0f;
  (&local_F0)->y = 0.0f;
  (&local_F0)->z = 0.0f;
  local_E4.x = 0.0f;
  (&local_E4)->y = 0.0f;
  (&local_E4)->z = 0.0f;
  local_BC = 0.0f;
  local_86 = 0;
  {
    int32_t local_FF78;
    int32_t local_FF74;
    Vector3i local_FF68;
    local_FF74 = 0;
    local_A0 = 0x3F800000;
    local_B4 = 0.0f;
    local_B0 = 0.0f;
    local_AC = 0.0f;
    local_A8 = 0;
    Script_mtwkGetSetRotation(arg1, &local_FF68, 0);
    local_84 = 0;
    if ((arg4 == 1) | (arg4 == 3)) {
      local_FF78 = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
      local_FF7C = 18;
      local_80 = 18;
      local_A4 = 1.0f;
    } else {
      if (arg0 == task_160) {
        local_FF78 = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
        local_FF7C = 12;
        local_80 = 12;
      } else {
        local_FF78 = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
        local_FF7C = 18;
        local_80 = 18;
      }
      local_A4 = -1.0f;
    }
    script_00E5(arg0, local_FF78, arg2, arg3, arg4);
    local_FF78 = local_FF78 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_98 - 3.0f) << 16) & 0x7FFF0000);
    script_92E9(arg0, 0);
    local_C0.x = (&task_168)->x;
    (&local_C0)->y = task_168.y;
    (&local_C0)->z = task_168.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), local_FF78);
    for (;;) {
      if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
        break;
      }
      if (Script_GetCharaMoveStatus(arg0) == 0) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    local_FF78 = local_FF78 & 0xFFFF;
    if (task_160 == arg0) {
      Script_SetCharaMotionPlaybackRate(arg0, 1.3f);
      local_FF = 0;
    } else {
      local_FF = 1;
    }
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      local_B8 = Script_GetCharaMotionRemainingFramesScaled(arg0);
    }
    for (;;) {
      if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
        Script_sprintf("This Motion don't Finish!!\012");
        break;
      }
      if (local_FF) {
        if (!(local_B8 <= local_90)) {
          local_FF = 0;
          script_233B(arg0, task_98[arg2]);
        }
      }
      Script_ReadMOTNPositionAtFrame(arg0, local_FF78, local_B8, &local_E4);
      local_E4.x = (&local_E4)->x * local_A4;
      if (!(local_B8 >= local_98)) {
        local_86 = 0;
      }
      if (!(local_B8 < local_98) & !(local_B8 > local_94)) {
        if (!(local_B8 < local_98) & !(local_B8 >= local_98 + 1.5f)) {
          if (local_84 == 0) {
            int32_t local_FF64;
            local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
            local_FF64 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_FF64 * 9 + 1) * 4 + 2);
            if (local_FF64 != -1) {
              if (arg0 == task_160) {
                Script_executeAicaCommand(local_FF64, 0, 0);
              } else {
                Script_Execute3DSoundCommand_Wrapper(local_FF64, 4, &local_C0, 5.0f, 20.0f, 0);
              }
            }
            local_84 = 1;
          }
        }
        Script_EVT_GetPosition(arg0, local_80, &local_F0, ENTITY_XFORM_MOTION_SPACE);
        if (local_86 == 1) {
          local_B0 = (&local_E4)->x;
          local_BC = (&local_F0)->x;
        }
        local_B4 = (&local_E4)->x + ((&local_F0)->x - local_BC);
        local_B4 = local_B4 - local_B0;
        local_B4 = local_B4 * 1.28f;
        {
          int32_t local_FF64;
          local_FF64 = arg4;
          if ((local_FF64 == 0) | (local_FF64 == 2)) {
            if (!(local_B4 < 0.0f)) {
              local_B4 = 0.0f;
            } else {
              if (!(local_B4 > -0.75f)) {
                local_B4 = -0.75f;
                goto L002999;
              }
            }
            L002999:;
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 > 0.0f)) {
                local_B4 = 0.0f;
              } else {
                if (!(local_B4 < 0.75f)) {
                  local_B4 = 0.75f;
                  goto L0029D7;
                }
              }
              L0029D7:;
              goto L0029DA;
            }
          }
        }
      }
      L0029DA:;
      if (task_160 == arg0) {
        if (!(local_B8 <= local_94) & !(local_B8 >= local_90)) {
          local_86 = 0;
        }
      } else {
        if (!(local_B8 <= local_94) & !(local_B8 > local_88)) {
          *(int32_t *)&local_9C = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 8);
          local_B4 = local_B4 + local_9C * local_A4;
        }
      }
      if (!(local_B8 < local_90) & !(local_B8 >= local_90 + 1.5f)) {
        if (local_84 == 1) {
          int32_t local_FF64;
          local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
          local_FF64 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_FF64 * 9 + 1) * 4 + 3);
          if (local_FF64 != -1) {
            if (arg0 == task_160) {
              Script_executeAicaCommand(local_FF64, 0, 0);
            } else {
              Script_Execute3DSoundCommand_Wrapper(local_FF64, 4, &local_C0, 5.0f, 20.0f, 0);
            }
          }
          local_84 = 2;
        }
        if (task_160 != arg0) {
          if (local_FF74 == 0) {
            local_AC = local_B4 / (local_8C - local_90);
            local_FF74 = 1;
          }
        }
      }
      if (!(local_B8 < local_90) & !(local_B8 > local_8C)) {
        Script_EVT_GetPosition(arg0, local_FF7C, &local_F0, ENTITY_XFORM_MOTION_SPACE);
        if (task_160 == arg0) {
          int32_t local_FF64;
          local_B4 = (&local_E4)->x + ((&local_F0)->x - local_BC);
          local_B4 = local_B4 - local_B0;
          local_B4 = local_B4 * 1.28f;
          local_FF64 = arg4;
          if ((local_FF64 == 0) | (local_FF64 == 2)) {
            if (!(local_B4 > -0.75f)) {
              local_B4 = -0.75f;
            } else {
              if (!(local_B4 < 0.0f)) {
                local_B4 = 0.0f;
                goto L002BA6;
              }
            }
            L002BA6:;
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 < 0.75f)) {
                local_B4 = 0.75f;
              } else {
                if (!(local_B4 > 0.0f)) {
                  local_B4 = 0.0f;
                  goto L002BE4;
                }
              }
              L002BE4:;
              goto L002BE7;
            }
          }
          L002BE7:;
        } else {
          int32_t local_FF64;
          local_B4 = local_B4 - local_AC;
          local_FF64 = arg4;
          if ((local_FF64 == 0) | (local_FF64 == 2)) {
            if (!(local_B4 < 0.0f)) {
              local_B4 = 0.0f;
            }
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 > 0.0f)) {
                local_B4 = 0.0f;
              }
              goto L002C40;
            }
          }
        }
      }
      L002C40:;
      if (!(local_8C <= 0.0f) & !(local_B8 <= local_8C)) {
        local_B4 = local_B4 - 0.01f;
        if (!(local_B4 > 0.0f)) {
          local_B4 = 0.0f;
        }
      }
      {
        int32_t local_FF64;
        local_FF64 = arg4;
        switch (local_FF64) {
          case 0:
            local_F0.x = (&local_CC)->x;
            (&local_F0)->y = (&local_CC)->y;
            (&local_F0)->z = (&local_CC)->z;
            if (arg5) {
              local_F0.x = (&local_CC)->x - local_B4;
              Script_charaPosRotUtils(arg1, 7, &local_F0, 0, CHARA_PART_Set);
            } else {
              local_F0.x = (&local_CC)->x + ((float)0 - local_B4) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_FF68)->y);
              (&local_F0)->z = (&local_CC)->z + local_B4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_FF68)->y);
              Script_EVT_SetPosition(arg1, POS_XYZ, &local_F0);
            }
            break;
          case 1:
            local_F0.x = (&local_CC)->x;
            (&local_F0)->y = (&local_CC)->y;
            (&local_F0)->z = (&local_CC)->z;
            if (arg5) {
              local_F0.x = (&local_CC)->x + local_B4;
              Script_charaPosRotUtils(arg1, 7, &local_F0, 0, CHARA_PART_Set);
            } else {
              local_F0.x = (&local_CC)->x - ((float)0 - local_B4) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_FF68)->y);
              (&local_F0)->z = (&local_CC)->z - local_B4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_FF68)->y);
              Script_EVT_SetPosition(arg1, POS_XYZ, &local_F0);
            }
            break;
          case 2:
            local_F0.x = (&local_D8)->x;
            (&local_F0)->y = (&local_D8)->y;
            (&local_F0)->z = (&local_D8)->z;
            if (arg5) {
              local_F0.x = (&local_D8)->x + local_B4;
              Script_charaPosRotUtils(arg1, 12, &local_F0, 0, CHARA_PART_Set);
            } else {
              local_F0.x = (&local_D8)->x - ((float)0 - local_B4) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_FF68)->y);
              (&local_F0)->z = (&local_D8)->z - local_B4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_FF68)->y);
              Script_EVT_SetPosition(arg1, POS_XYZ, &local_F0);
            }
            break;
          case 3:
            local_F0.x = (&local_D8)->x;
            (&local_F0)->y = (&local_D8)->y;
            (&local_F0)->z = (&local_D8)->z;
            if (arg5) {
              local_F0.x = (&local_D8)->x - local_B4;
              Script_charaPosRotUtils(arg1, 12, &local_F0, 0, CHARA_PART_Set);
            } else {
              local_F0.x = (&local_D8)->x + ((float)0 - local_B4) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_FF68)->y);
              (&local_F0)->z = (&local_D8)->z + local_B4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_FF68)->y);
              Script_EVT_SetPosition(arg1, POS_XYZ, &local_F0);
            }
            break;
        }
      }
      local_86 = local_86 + 1;
      if (arg0 == task_160) {
        local_B8 = Script_GetCharaMotionRemainingFramesScaled(arg0);
        if (Script_IsCharaMotionFinished(arg0)) {
          break;
        }
      } else {
        if (!(local_B8 <= local_8C)) {
          break;
        }
        local_B8 = local_B8 + 1.0f;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  script_92E9(arg0, 1);
  {
    int32_t local_FF78;
    int32_t unused_FF68[4];
    int32_t local_FF64;
    local_FF78 = task_48[arg2];
    local_FF64 = arg3;
    if ((local_FF64 == 9) | (local_FF64 == 10) | (local_FF64 == 15)) {
      if (arg5) {
        Script_charaPosRotUtils(arg1, 12, &local_D8, 0, CHARA_PART_Set);
        Script_charaPosRotUtils(arg1, 7, &local_CC, 0, CHARA_PART_Set);
      } else {
        Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
      }
      if (task_160 == arg0) {
        local_80 = (Script_ScenDataUtil(task_0, SCND_ReadU32, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        local_80 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FF78 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
      }
    } else if ((local_FF64 == 18) | (local_FF64 == 19) | (local_FF64 == 21) | (local_FF64 == 22)) {
      if (task_160 == arg0) {
        local_80 = (Script_ScenDataUtil(task_0, SCND_ReadU32, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 8;
          Script_ScenDataUtil(task_0, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
        }
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 0x10;
          Script_ScenDataUtil(task_0, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
        }
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 2;
          Script_ScenDataUtil(task_0, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
        }
      } else {
        if (arg5) {
          Script_charaPosRotUtils(arg1, 12, &local_D8, 0, CHARA_PART_Set);
          Script_charaPosRotUtils(arg1, 7, &local_CC, 0, CHARA_PART_Set);
        } else {
          Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
        }
        local_80 = (Script_ScenDataUtil(task_0, SCND_ReadU32, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        local_80 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FF78 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
      }
    } else {
      if ((local_FF64 == 20) | (local_FF64 == 23)) {
        if (task_160 == arg0) {
          if ((arg4 == 0) | (arg4 == 1)) {
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 0x10;
              Script_ScenDataUtil(task_0, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
            }
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 4;
              Script_ScenDataUtil(task_0, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
            }
          } else {
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 8;
              Script_ScenDataUtil(task_0, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
            }
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 4;
              Script_ScenDataUtil(task_0, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
            }
          }
        } else {
          if (arg5) {
            Script_charaPosRotUtils(arg1, 12, &local_D8, 0, CHARA_PART_Set);
            Script_charaPosRotUtils(arg1, 7, &local_CC, 0, CHARA_PART_Set);
          } else {
            Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
          }
          local_80 = (Script_ScenDataUtil(task_0, SCND_ReadU32, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_80, 1);
          local_80 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FF78 * 13 + 2) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_80, 1);
        }
        goto L003323;
      }
    }
  }
  L003323:;
  return;
}

static void script_3324(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (task_160 == arg0) {
    local_FC = task_48[arg1];
    {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 13 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
    {
      int32_t local_F8;
      local_F8 = (Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 13 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
  }
  return;
}

static void script_3390(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3 local_D0;
  Vector3 local_C4;
  Vector3 local_B8;
  Vector3 local_AC;
  Vector3i local_A0;
  int32_t local_9C;
  int32_t local_98;
  float local_94;
  int8_t local_93;
  int8_t local_92;
  uint8_t unused_90[2];
  int32_t local_8C;
  int32_t local_88;
  int32_t local_84;
  if (arg4 == 4) {
    Script_charaPosRotUtils(arg1, 12, &local_C4, 0, CHARA_PART_Get);
    Script_charaPosRotUtils(arg1, 7, &local_B8, 0, CHARA_PART_Get);
  } else {
    Script_EVT_GetPosition(arg1, -1, &local_C4, 0);
    Script_EVT_GetPosition(arg1, -1, &local_B8, 0);
  }
  local_AC.x = (&task_168)->x;
  (&local_AC)->y = task_168.y;
  (&local_AC)->z = task_168.z;
  local_DC.x = (&local_C4)->x;
  (&local_DC)->y = (&local_C4)->y;
  (&local_DC)->z = (&local_C4)->z;
  local_D0.x = (&local_B8)->x;
  (&local_D0)->y = (&local_B8)->y;
  (&local_D0)->z = (&local_B8)->z;
  local_9C = 0xC000;
  local_93 = 0;
  local_98 = 0x300;
  Script_EVT_GetPosition(arg1, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(arg1, &local_A0, 0);
  {
    int32_t local_80;
    local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
    local_80 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_80 * 9 + 1) * 4 + 2);
    if (local_80 != -1) {
      if (arg0 == task_160) {
        Script_executeAicaCommand(local_80, 0, 0);
      } else {
        Script_Execute3DSoundCommand_Wrapper(local_80, 4, &local_AC, 5.0f, 20.0f, 0);
      }
    }
  }
  local_88 = 0;
  local_8C = 0;
  local_84 = 0;
  for (;;) {
    local_92 = 0;
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      Script_EVT_GetPosition(arg0, -1, &local_E8, 0);
      if (Script_CheckTargetWithinAngleRange(&local_F4, 1.5f, (&local_A0)->y, 0x10000, &local_E8)) {
        local_92 = 1;
      }
    }
    if (local_92) {
      if (local_8C) {
        task_70[arg2] = arg0;
        local_98 = -local_98;
        local_84 = 0;
      }
      local_8C = 0;
    } else {
      if ((local_93 == 1) & (local_8C == 0)) {
        int32_t local_80;
        local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
        local_80 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_80 * 9 + 1) * 4 + 3);
        if (local_80 != -1) {
          if (arg0 == task_160) {
            Script_executeAicaCommand(local_80, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_80, 4, &local_AC, 5.0f, 20.0f, 0);
          }
        }
        local_93 = 0;
        local_8C = 1;
        local_98 = -local_98;
        local_84 = 0;
        task_70[arg2] = 0;
      }
    }
    local_88 = task_48[arg2];
    local_88 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_88 * 13 + 2) & 0xFFFF;
    local_88 = local_88 + ((local_9C - 0x4000) >> 12);
    if (local_84 != local_88) {
      Script_scenToggleCharaFlag(0, local_88, local_8C);
      local_84 = local_88;
    }
    local_9C = local_9C - local_98;
    if (local_9C >= 0xC000) {
      local_9C = 0xC000;
      local_88 = task_48[arg2];
      local_88 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_88 * 13 + 2) & 0xFFFF;
      local_88 = local_88 + 8;
      Script_scenToggleCharaFlag(0, local_88, local_8C);
      break;
    }
    if (local_9C <= 0x4000) {
      local_9C = 0x4000;
      if (local_93 == 0) {
        task_70[arg2] = 0;
        local_93 = 1;
      }
    }
    local_94 = (Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_9C) + 1.0f) * 0.45f;
    {
      int32_t local_80;
      local_80 = arg4;
      if (local_80 == 4) {
        local_DC.x = (&local_C4)->x + local_94;
        local_D0.x = (&local_B8)->x - local_94;
        Script_charaPosRotUtils(arg1, 12, &local_DC, 0, CHARA_PART_Set);
        Script_charaPosRotUtils(arg1, 7, &local_D0, 0, CHARA_PART_Set);
      } else if ((local_80 == 0) | (local_80 == 1)) {
        local_DC.x = (&local_C4)->x - ((float)0 - local_94) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_A0)->y);
        (&local_DC)->z = (&local_C4)->z - local_94 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_A0)->y);
        Script_EVT_SetPosition(arg1, POS_XYZ, &local_DC);
      } else {
        if ((local_80 == 2) | (local_80 == 3)) {
          local_D0.x = (&local_B8)->x + ((float)0 - local_94) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_A0)->y);
          (&local_D0)->z = (&local_B8)->z + local_94 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_A0)->y);
          Script_EVT_SetPosition(arg1, POS_XYZ, &local_D0);
          goto L0037FA;
        }
      }
    }
    L0037FA:;
    SCNF_WaitFrames_0000(1);
  }
  if (arg4 == 4) {
    Script_charaPosRotUtils(arg1, 12, &local_C4, 0, CHARA_PART_Set);
    Script_charaPosRotUtils(arg1, 7, &local_B8, 0, CHARA_PART_Set);
  } else {
    Script_EVT_SetPosition(arg1, POS_XYZ, &local_C4);
    Script_EVT_SetPosition(arg1, POS_XYZ, &local_B8);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1530A)))
static void script_384A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  Vector3 local_E4;
  Vector3 local_D8;
  Vector3 local_CC;
  float local_C8;
  float local_C4;
  float local_C0;
  float local_BC;
  float local_B8;
  int8_t local_B7;
  uint8_t unused_B4[3];
  Vector3i local_A8;
  int32_t local_A4;
  local_A4 = arg3;
  switch (local_A4) {
    case 24:
      local_A4 = arg4;
      switch (local_A4) {
        case 1:
          local_B7 = 1;
          local_F8 = 18;
          local_F0 = 0x4000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
      }
      break;
    case 27:
      local_A4 = arg4;
      switch (local_A4) {
        case 1:
          local_B7 = 0;
          local_F8 = 12;
          local_F0 = 0xC000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
          break;
      }
      break;
    case 25:
      local_A4 = arg4;
      switch (local_A4) {
        case 1:
          local_B7 = 1;
          local_F8 = 12;
          local_F0 = 0xC000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
          break;
      }
      break;
    case 26:
      local_A4 = arg4;
      switch (local_A4) {
        case 1:
          local_B7 = 0;
          local_F8 = 18;
          local_F0 = 0x4000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
      }
      break;
  }
  *(int32_t *)&local_C4 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_C0 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 2);
  local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_C4 - 3.0f) << 16) & 0x7FFF0000);
  local_BC = 0.0f;
  script_92E9(arg0, 0);
  local_CC.x = (&task_168)->x;
  (&local_CC)->y = task_168.y;
  (&local_CC)->z = task_168.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), local_FC);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
      break;
    }
    if (Script_GetCharaMoveStatus(arg0) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  local_FC = local_FC & 0xFFFF;
  Script_EVT_GetPosition(arg1, -1, &local_E4, 0);
  Script_mtwkGetSetRotation(arg1, &local_A8, 0);
  local_F4 = (&local_A8)->y;
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
      Script_sprintf("This Motion don't Finish!!\012");
      break;
    }
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    local_C8 = Script_GetCharaMotionRemainingFramesScaled(arg0);
    if (!(local_C8 < local_C4) & !(local_C8 > local_C0)) {
      if (local_B7 == 1) {
        local_BC = local_BC - 0.3f;
      }
      if (!(local_C8 < local_C4) & !(local_C8 >= local_C4 + 1.0f)) {
        int32_t local_A0;
        local_A0 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
        local_A0 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_A0 * 9 + 1) * 4 + 2);
        if (local_A0 != -1) {
          if (arg0 == task_160) {
            Script_executeAicaCommand(local_A0, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_A0, 4, &local_CC, 5.0f, 20.0f, 0);
          }
        }
      }
      Script_EVT_GetPosition(arg0, local_F8, &local_D8, ENTITY_XFORM_WORLD_SPACE);
      local_F4 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_E4)->x - (&local_D8)->x, (&local_E4)->z - (&local_D8)->z);
      local_F4 = local_F4 + local_F0 + (int32_t)(local_BC * local_B8 / 360.0f * (float)0x10000);
    }
    {
      Vector3i local_98;
      (&local_98)->y = local_F4;
      Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_98, 0);
    }
    SCNF_WaitFrames_0000(1);
  }
  script_92E9(arg0, 1);
  local_FC = task_48[arg2];
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 13 + 1);
    local_A0 = local_A0 | 8;
    Script_ScenDataUtil(task_0, SCND_WriteU32, local_FC * 13 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 13 + 1);
    local_A0 = local_A0 | 0x10;
    Script_ScenDataUtil(task_0, SCND_WriteU32, local_FC * 13 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 13 + 1);
    local_A0 = local_A0 | 4;
    Script_ScenDataUtil(task_0, SCND_WriteU32, local_FC * 13 + 1, local_A0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_15326)))
static void script_3F04(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Vector3 local_F4;
  Vector3 local_E8;
  int32_t unused_DC[3];
  Vector3 local_D0;
  int8_t local_CF;
  int8_t local_CE;
  uint8_t unused_CC[2];
  float local_C8;
  float local_C4;
  float local_C0;
  float local_BC;
  float local_B8;
  float local_B4;
  float local_B0;
  float local_AC;
  float local_A8;
  int32_t local_A4;
  int32_t local_A0;
  int32_t local_9C;
  int32_t local_98;
  int32_t local_94;
  int32_t local_90;
  int32_t local_8C;
  int32_t local_88;
  int32_t local_84;
  int32_t local_80;
  int32_t local_FF7C;
  float local_FF78;
  float local_FF74;
  int32_t local_FF70;
  Vector3i local_FF64;
  int32_t local_FF60;
  local_CE = 0;
  local_80 = 0;
  local_FF78 = 1.0f;
  local_FF74 = 0.0f;
  {
    int32_t local_FF5C;
    local_FF5C = arg3;
    if ((local_FF5C == 4) | (local_FF5C == 6) | (local_FF5C == 33) | (local_FF5C == 35)) {
      if (arg0 == task_160) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = 1.0f;
            local_A0 = 12;
            local_9C = 18;
            local_90 = 17294;
            local_8C = 15473;
            break;
        }
      } else {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 19842;
            break;
        }
      }
    } else if ((local_FF5C == 1) | (local_FF5C == 7) | (local_FF5C == 30) | (local_FF5C == 36) | (local_FF5C == 37)) {
      if (arg0 == task_160) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = 1.0f;
            local_A0 = 12;
            local_9C = 18;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 48241;
            local_8C = 50062;
            break;
        }
      } else {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            if (task_98[arg2] == 4) {
              local_90 = 50062;
            } else {
              local_90 = 51882;
            }
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            if (task_98[arg2] == 4) {
              local_90 = 46421;
            } else {
              local_90 = 45511;
            }
            break;
        }
      }
    } else if ((local_FF5C == 3) | (local_FF5C == 5) | (local_FF5C == 32) | (local_FF5C == 34)) {
      if (arg0 == task_160) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = -1.0f;
            local_A0 = 12;
            local_9C = 12;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 12;
            local_90 = 48241;
            local_8C = 50062;
            break;
        }
      } else {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 51882;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 45511;
            break;
        }
      }
    } else if ((local_FF5C == 2) | (local_FF5C == 8) | (local_FF5C == 31)) {
      if (arg0 == task_160) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 12;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = -1.0f;
            local_A0 = 12;
            local_9C = 12;
            local_90 = 17294;
            local_8C = 15473;
            break;
        }
      } else {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 19842;
            break;
        }
      }
    } else {
      if ((local_FF5C == 13) | (local_FF5C == 14) | (local_FF5C == 28)) {
        if (arg0 == task_160) {
          local_FF5C = arg4;
          switch (local_FF5C) {
            case 0:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 12;
              local_90 = 48241;
              local_8C = 50062;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
              local_C0 = -1.0f;
              local_A0 = 12;
              local_9C = 12;
              local_90 = 50062;
              local_8C = 48241;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 15473;
              local_8C = 17294;
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
              local_C0 = 1.0f;
              local_A0 = 12;
              local_9C = 18;
              local_90 = 17294;
              local_8C = 15473;
              break;
          }
        } else {
          local_FF5C = arg4;
          switch (local_FF5C) {
            case 0:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 45511;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 51882;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              if (task_98[arg2] == 4) {
                local_90 = 14199;
              } else {
                local_90 = 13653;
              }
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              if (task_98[arg2] == 4) {
                local_90 = 18022;
              } else {
                local_90 = 19842;
              }
              break;
          }
        }
        local_CE = 1;
        goto L004AF7;
      }
    }
  }
  L004AF7:;
  (void)local_90;
  *(int32_t *)&local_B8 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_B4 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_B0 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_AC = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_A8 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 7);
  local_C8 = 0.0f;
  local_FF7C = 0;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg1)) {
    if (local_CE) {
      {
        int32_t unused_FF5C;
        Vector3i local_FF50;
        Script_charaPosRotUtils(arg1, 12, 0, &local_FF50, CHARA_PART_Get);
        local_98 = (&local_FF50)->y;
      }
      {
        int32_t unused_FF5C;
        Vector3i local_FF50;
        Script_charaPosRotUtils(arg1, 7, 0, &local_FF50, CHARA_PART_Get);
        local_94 = (&local_FF50)->y;
      }
      if ((arg4 == 2) | (arg4 == 3)) {
        local_88 = local_98;
        Script_charaPosRotUtils(arg1, 12, &local_F4, 0, CHARA_PART_Get);
      } else {
        local_88 = local_94;
        Script_charaPosRotUtils(arg1, 7, &local_F4, 0, CHARA_PART_Get);
      }
      local_FF70 = local_88;
      {
        int32_t unused_FF5C;
        Vector3i local_FF50;
        Vector3 local_FF44;
        Script_EVT_GetPosition(arg1, -1, &local_FF44, 0);
        Script_mtwkGetSetRotation(arg1, &local_FF50, 0);
        local_84 = (&local_FF50)->y;
        {
          float local_FF40;
          float local_FF3C;
          local_FF3C = (&local_F4)->z * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_84) - (&local_F4)->x * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_84);
          local_FF40 = (&local_F4)->z * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_84) + (&local_F4)->x * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_84);
          local_F4.x = local_FF40 + (&local_FF44)->x;
          (&local_F4)->y = (&local_F4)->y + (&local_FF44)->y;
          (&local_F4)->z = local_FF3C + (&local_FF44)->z;
        }
      }
    } else {
      int32_t unused_FF5C;
      Vector3i local_FF50;
      Script_mtwkGetSetRotation(arg1, &local_FF50, 0);
      local_98 = (&local_FF50)->y;
      local_94 = (&local_FF50)->y;
      local_88 = (&local_FF50)->y;
      local_FF70 = (&local_FF50)->y;
      Script_EVT_GetPosition(arg1, -1, &local_F4, 0);
      local_84 = 0;
    }
  }
  script_00E5(arg0, local_A4, arg2, arg3, arg4);
  local_A4 = local_A4 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_B8 - 3.0f) << 16) & 0x7FFF0000);
  script_92E9(arg0, 0);
  local_D0.x = (&task_168)->x;
  (&local_D0)->y = task_168.y;
  (&local_D0)->z = task_168.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), local_A4);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
      break;
    }
    if (Script_GetCharaMoveStatus(arg0) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  local_A4 = local_A4 & 0xFFFF;
  {
    int32_t local_FF5C;
    int8_t local_FF5B;
    if (arg0 == task_160) {
      (void)((local_FF5B = 0) & 0x80);
    } else {
      (void)((local_FF5B = -1) & 0x80);
    }
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      local_C4 = Script_GetCharaMotionRemainingFramesScaled(arg0);
    }
    for (;;) {
      if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
        Script_sprintf("This Motion don't Finish!!\012");
        break;
      }
      if (arg0 == task_160) {
        if (Script_IsCharaMotionFinished(arg0)) {
          break;
        }
        local_C4 = Script_GetCharaMotionRemainingFramesScaled(arg0);
      }
      if (local_FF5B & 8) {
        if (!(local_C4 <= local_B0)) {
          (void)((local_FF5B = 0) & 1);
          {
            uint8_t unused_FF54[7];
            int32_t local_FF50;
            local_FF50 = 1;
            if (local_FF50) {
              script_233B(arg0, task_98[arg2]);
            }
          }
        }
      }
      if (!(local_C4 > local_B4)) {
        if (!(local_C4 < local_B8) & !(local_C4 > local_B4)) {
          if (arg0 == task_160) {
            if (local_CF == 1) {
              local_C8 = local_C8 - 0.165526f;
            } else {
              local_C8 = local_C8 - 0.145973f;
            }
          }
        }
        if (!(local_C4 < local_B8) & !(local_C4 >= local_B8 + 1.0f)) {
          uint8_t unused_FF54[7];
          int32_t local_FF50;
          local_FF50 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
          local_FF50 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_FF50 * 9 + 1) * 4 + 2);
          if (local_FF50 != -1) {
            if (arg0 == task_160) {
              Script_executeAicaCommand(local_FF50, 0, 0);
            } else {
              Script_Execute3DSoundCommand_Wrapper(local_FF50, 4, &local_D0, 5.0f, 20.0f, 0);
            }
          }
        }
        Script_EVT_GetPosition(arg0, local_A0, &local_E8, ENTITY_XFORM_WORLD_SPACE);
        local_88 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_F4)->x - (&local_E8)->x, (&local_F4)->z - (&local_E8)->z);
        local_88 = local_88 - local_84;
        local_88 = local_88 + local_90 + (int32_t)(local_C8 * local_C0 / 360.0f * (float)0x10000);
        if (local_CE) {
          if ((arg4 == 2) | (arg4 == 3)) {
            Script_charaPosRotUtils(arg1, 12, 0, &local_FF64, CHARA_PART_Get);
          } else {
            Script_charaPosRotUtils(arg1, 7, 0, &local_FF64, CHARA_PART_Get);
          }
        } else {
          Script_mtwkGetSetRotation(arg1, &local_FF64, 0);
        }
        if ((local_CF == 0) & !(local_C4 >= local_B8)) {
          local_88 = (&local_FF64)->y;
        }
        local_FF60 = script_006D((&local_FF64)->y & 0xFFFF, local_88 & 0xFFFF);
        if (local_FF60 > 910) {
          local_88 = (&local_FF64)->y + 910;
        } else {
          if (local_FF60 < -910) {
            local_88 = (&local_FF64)->y - 910;
            goto L0050FC;
          }
        }
        L0050FC:;
        if (!(local_C4 >= local_B4)) {
          if (!(local_C0 <= 0.0f)) {
            if (script_006D(local_FF70, local_88) > 0) {
              local_88 = local_FF70;
            }
          } else {
            if (script_006D(local_FF70, local_88) < 0) {
              local_88 = local_FF70;
            }
          }
        }
      }
      if (arg0 != task_160) {
        if (!(local_B4 > local_C4) & !(local_C4 >= local_B4 + 1.0f)) {
          uint8_t unused_FF54[7];
          Vector3i local_FF48;
          if (local_CE) {
            if ((arg4 == 2) | (arg4 == 3)) {
              Script_charaPosRotUtils(arg1, 12, 0, &local_FF48, CHARA_PART_Get);
            } else {
              Script_charaPosRotUtils(arg1, 7, 0, &local_FF48, CHARA_PART_Get);
            }
          } else {
            Script_mtwkGetSetRotation(arg1, &local_FF48, 0);
          }
          local_FF74 = (float)script_0030(script_006D(local_FF70, (&local_FF48)->y) * 360 / 0x10000);
        }
        if (!(local_C4 <= local_B4) & !(local_C4 > local_A8)) {
          *(int32_t *)&local_BC = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 8);
          if (!(local_FF74 < 90.0f)) {
            local_BC = 0.0f;
          } else {
            local_FF74 = local_FF74 + local_BC;
            if (!(local_FF74 <= 90.0f)) {
              local_BC = local_FF74 - 90.0f;
            }
          }
          local_88 = local_88 - (int32_t)(local_BC * local_C0 / 360.0f * (float)0x10000);
        }
      }
      if (!(local_C4 < local_B0) & !(local_C4 >= local_B0 + 1.0f)) {
        if (local_80 == 0) {
          if (arg0 != task_160) {
            uint8_t unused_FF54[7];
            Vector3i local_FF48;
            int32_t local_FF44;
            if (local_CE) {
              if ((arg4 == 2) | (arg4 == 3)) {
                Script_charaPosRotUtils(arg1, 12, 0, &local_FF48, CHARA_PART_Get);
              } else {
                Script_charaPosRotUtils(arg1, 7, 0, &local_FF48, CHARA_PART_Get);
              }
            } else {
              Script_mtwkGetSetRotation(arg1, &local_FF48, 0);
            }
            if (local_CE) {
              if ((arg4 == 2) | (arg4 == 3)) {
                if (!((float)(((&local_FF48)->y - local_98) & 0xFFFF) * 360.0f / (float)0x10000 <= 180.0f)) {
                  local_FF44 = local_98 + 0x10000 - (&local_FF48)->y;
                } else {
                  local_FF44 = (&local_FF48)->y - local_98;
                }
              } else if (!((float)(((&local_FF48)->y - local_94) & 0xFFFF) * 360.0f / (float)0x10000 <= 180.0f)) {
                local_FF44 = local_94 + 0x10000 - (&local_FF48)->y;
              } else {
                local_FF44 = (&local_FF48)->y - local_94;
              }
            } else if (!((float)(((&local_FF48)->y - local_98) & 0xFFFF) * 360.0f / (float)0x10000 <= 180.0f)) {
              local_FF44 = local_98 + 0x10000 - (&local_FF48)->y;
            } else {
              local_FF44 = (&local_FF48)->y - local_98;
            }
            local_FF7C = (int32_t)((float)(local_FF44 & 0xFFFF) * 360.0f / (float)0x10000 / (local_AC - local_B0) * local_C0 / 360.0f * (float)0x10000);
            local_80 = 1;
          }
        }
      }
      if (!(local_C4 < local_AC) & !(local_C4 >= local_AC + 1.0f)) {
        uint8_t unused_FF54[7];
        int32_t local_FF50;
        local_FF50 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
        local_FF50 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_FF50 * 9 + 1) * 4 + 3);
        if (local_FF50 != -1) {
          if (arg0 == task_160) {
            Script_executeAicaCommand(local_FF50, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_FF50, 4, &local_D0, 5.0f, 20.0f, 0);
          }
        }
        local_A4 = task_48[arg2];
        if (arg0 == task_160) {
          if (task_160 == arg0) {
            local_A0 = (Script_ScenDataUtil(task_0, SCND_ReadU32, local_A4 * 13 + 2) >> 16) & 0xFFFF;
            Script_scenToggleCharaFlag(0, local_A0, 1);
            local_A0 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_A4 * 13 + 2) & 0xFFFF;
            Script_scenToggleCharaFlag(0, local_A0, 1);
          }
        }
      }
      if (!(local_AC <= 0.0f)) {
        if (!(local_C4 <= local_AC)) {
          if (arg0 == task_160) {
            local_FF5C = arg3;
            if ((local_FF5C == 30) | (local_FF5C == 31) | (local_FF5C == 32) | (local_FF5C == 33) | (local_FF5C == 34) | (local_FF5C == 35)) {
              local_FF78 = local_FF78 - 0.06f;
              if (!(local_FF78 >= 0.3f)) {
                local_FF78 = 0.3f;
              }
              DSWK_SetActorOpacityAndDrawCallback(arg1, local_FF78);
              goto L0055A8;
            }
          }
        }
      }
      L0055A8:;
      if (!(local_C4 < local_B0) & !(local_C4 > local_AC)) {
        if (arg0 == task_160) {
          if (local_CF == 0) {
            local_C8 = local_C8 - 0.11939f;
          } else {
            local_C8 = local_C8 + 2.381415f;
          }
          Script_EVT_GetPosition(arg0, local_9C, &local_E8, ENTITY_XFORM_WORLD_SPACE);
          local_88 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_F4)->x - (&local_E8)->x, (&local_F4)->z - (&local_E8)->z);
          local_88 = local_88 - local_84;
          local_88 = local_88 + local_8C + (int32_t)(local_C8 * local_C0 / 360.0f * (float)0x10000);
        } else {
          local_88 = local_88 + local_FF7C;
        }
      }
      if (local_CE) {
        uint8_t unused_FF54[7];
        Vector3i local_FF48;
        local_FF48.x = 0;
        (&local_FF48)->z = 0;
        if ((arg4 == 2) | (arg4 == 3)) {
          (&local_FF48)->y = local_88;
          Script_charaPosRotUtils(arg1, 12, 0, &local_FF48, CHARA_PART_Set);
        } else {
          (&local_FF48)->y = local_88;
          Script_charaPosRotUtils(arg1, 7, 0, &local_FF48, CHARA_PART_Set);
        }
      } else {
        uint8_t unused_FF54[7];
        Vector3i local_FF48;
        (&local_FF48)->y = local_88;
        Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_FF48, 0);
      }
      if (arg0 != task_160) {
        if (!(local_C4 < local_AC)) {
          break;
        }
        local_C4 = local_C4 + 1.0f;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  script_92E9(arg0, 1);
  if (local_CE) {
    int32_t unused_FF5C;
    Vector3i local_FF50;
    local_FF50.x = 0;
    (&local_FF50)->z = 0;
    (&local_FF50)->y = local_98;
    Script_charaPosRotUtils(arg1, 12, 0, &local_FF50, CHARA_PART_Set);
    (&local_FF50)->y = local_94;
    Script_charaPosRotUtils(arg1, 7, 0, &local_FF50, CHARA_PART_Set);
  } else {
    {
      int32_t unused_FF5C;
      Vector3i local_FF50;
      (&local_FF50)->y = local_98;
      Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_FF50, 0);
    }
    {
      int32_t unused_FF5C;
      Vector3i local_FF50;
      (&local_FF50)->y = local_94;
      Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_FF50, 0);
    }
  }
  if (arg0 == task_160) {
    int32_t local_FF5C;
    local_FF5C = arg3;
    if ((local_FF5C == 30) | (local_FF5C == 31) | (local_FF5C == 32) | (local_FF5C == 33) | (local_FF5C == 34) | (local_FF5C == 35)) {
      for (;;) {
        if (Script_CamCtrl(CamCtrlCmd_CheckPosRotCollision) != -1) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      DSWK_SetActorOpacityAndDrawCallback(arg1, 1.0f);
      goto L0057E3;
    }
  }
  L0057E3:;
  return;
}

__attribute__((scn_string_bindings(0, data_15342)))
static void script_57E4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3 local_D0;
  Vector3 local_C4;
  float local_C0;
  float local_BC;
  float local_B8;
  float local_B4;
  int32_t local_B0;
  float local_AC;
  float local_A8;
  float local_A4;
  int16_t local_A2;
  uint8_t unused_A0[2];
  int32_t local_9C;
  int32_t local_98;
  local_A8 = 30.0f;
  local_A4 = 60.0f;
  if (arg5) {
    Script_charaPosRotUtils(arg1, 12, &local_DC, 0, CHARA_PART_Get);
    Script_charaPosRotUtils(arg1, 7, &local_D0, 0, CHARA_PART_Get);
  } else {
    Script_EVT_GetPosition(arg1, -1, &local_DC, 0);
    Script_EVT_GetPosition(arg1, -1, &local_D0, 0);
  }
  local_F4.x = 0.0f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.0f;
  local_E8.x = 0.0f;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = 0.0f;
  local_C0 = 0.0f;
  local_A2 = 0;
  local_B8 = 0.0f;
  local_B4 = 0.0f;
  local_B0 = 0;
  {
    int32_t local_94;
    Vector3i local_88;
    Script_mtwkGetSetRotation(arg1, &local_88, 0);
    if ((arg4 == 1) | (arg4 == 3)) {
      local_94 = 1395;
      local_98 = 18;
      local_9C = 18;
      local_AC = 1.0f;
    } else {
      local_94 = -0x7FFFFA8D;
      local_98 = 12;
      local_9C = 12;
      local_AC = -1.0f;
    }
    local_94 = local_94 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_A8 - 3.0f) << 16) & 0x7FFF0000);
    script_92E9(arg0, 0);
    local_C4.x = (&task_168)->x;
    (&local_C4)->y = task_168.y;
    (&local_C4)->z = task_168.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), local_94);
    for (;;) {
      if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
        break;
      }
      if (Script_GetCharaMoveStatus(arg0) == 0) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    local_94 = local_94 & 0xFFFF;
    for (;;) {
      if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
        Script_sprintf("This Motion don't Finish!!\012");
        break;
      }
      if (Script_IsCharaMotionFinished(arg0)) {
        break;
      }
      local_BC = Script_GetCharaMotionRemainingFramesScaled(arg0);
      Script_ReadMOTNPositionAtFrame(arg0, local_94, local_BC, &local_E8);
      local_E8.x = (&local_E8)->x * local_AC;
      if (!(local_BC >= local_A8)) {
        local_A2 = 0;
      }
      if (!(local_BC < local_A8) & !(local_BC > local_A4)) {
        if (!(local_BC < local_A8) & !(local_BC >= local_A8 + 1.0f)) {
          int32_t local_84;
          local_84 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
          local_84 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_84 * 9 + 1) * 4 + 3);
          if (local_84 != -1) {
            if (arg0 == task_160) {
              Script_executeAicaCommand(local_84, 0, 0);
            } else {
              Script_Execute3DSoundCommand_Wrapper(local_84, 4, &local_C4, 5.0f, 20.0f, 0);
            }
          }
        }
        Script_EVT_GetPosition(arg0, local_9C, &local_F4, ENTITY_XFORM_MOTION_SPACE);
        if (local_A2 == 1) {
          local_B4 = (&local_E8)->x;
          local_C0 = (&local_F4)->x;
        }
        local_B8 = (&local_E8)->x + ((&local_F4)->x - local_C0);
        local_B8 = local_B8 - local_B4;
        local_B8 = local_B8 * 1.28f;
        {
          int32_t local_84;
          local_84 = arg4;
          if ((local_84 == 0) | (local_84 == 2)) {
            if (!(local_B8 < 0.75f)) {
              local_B8 = 0.75f;
            } else {
              if (!(local_B8 > 0.0f)) {
                local_B8 = 0.0f;
                goto L005B08;
              }
            }
            L005B08:;
          } else {
            if ((local_84 == 1) | (local_84 == 3)) {
              if (!(local_B8 > -0.75f)) {
                local_B8 = -0.75f;
              } else {
                if (!(local_B8 < 0.0f)) {
                  local_B8 = 0.0f;
                  goto L005B44;
                }
              }
              L005B44:;
              goto L005B47;
            }
          }
        }
      }
      L005B47:;
      if (!(local_BC <= local_A4)) {
        int32_t local_84;
        local_84 = arg4;
        if ((local_84 == 0) | (local_84 == 2)) {
          local_B8 = 0.75f;
        } else {
          if ((local_84 == 1) | (local_84 == 3)) {
            local_B8 = -0.75f;
            goto L005B8C;
          }
        }
      }
      L005B8C:;
      {
        int32_t local_84;
        local_84 = arg4;
        switch (local_84) {
          case 0:
            local_F4.x = (&local_D0)->x;
            (&local_F4)->y = (&local_D0)->y;
            (&local_F4)->z = (&local_D0)->z;
            if (arg5) {
              local_F4.x = (&local_D0)->x - local_B8;
              Script_charaPosRotUtils(arg1, 7, &local_F4, 0, CHARA_PART_Set);
            } else {
              local_F4.x = (&local_D0)->x + ((float)0 - local_B8) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_88)->y);
              (&local_F4)->z = (&local_D0)->z + local_B8 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_88)->y);
              Script_EVT_SetPosition(arg1, POS_XYZ, &local_F4);
            }
            break;
          case 1:
            local_F4.x = (&local_D0)->x;
            (&local_F4)->y = (&local_D0)->y;
            (&local_F4)->z = (&local_D0)->z;
            if (arg5) {
              local_F4.x = (&local_D0)->x + local_B8;
              Script_charaPosRotUtils(arg1, 7, &local_F4, 0, CHARA_PART_Set);
            } else {
              local_F4.x = (&local_D0)->x - ((float)0 - local_B8) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_88)->y);
              (&local_F4)->z = (&local_D0)->z - local_B8 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_88)->y);
              Script_EVT_SetPosition(arg1, POS_XYZ, &local_F4);
            }
            break;
          case 2:
            local_F4.x = (&local_DC)->x;
            (&local_F4)->y = (&local_DC)->y;
            (&local_F4)->z = (&local_DC)->z;
            if (arg5) {
              local_F4.x = (&local_DC)->x + local_B8;
              Script_charaPosRotUtils(arg1, 12, &local_F4, 0, CHARA_PART_Set);
            } else {
              local_F4.x = (&local_DC)->x - ((float)0 - local_B8) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_88)->y);
              (&local_F4)->z = (&local_DC)->z - local_B8 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_88)->y);
              Script_EVT_SetPosition(arg1, POS_XYZ, &local_F4);
            }
            break;
          case 3:
            local_F4.x = (&local_DC)->x;
            (&local_F4)->y = (&local_DC)->y;
            (&local_F4)->z = (&local_DC)->z;
            if (arg5) {
              local_F4.x = (&local_DC)->x - local_B8;
              Script_charaPosRotUtils(arg1, 12, &local_F4, 0, CHARA_PART_Set);
            } else {
              local_F4.x = (&local_DC)->x + ((float)0 - local_B8) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_88)->y);
              (&local_F4)->z = (&local_DC)->z + local_B8 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_88)->y);
              Script_EVT_SetPosition(arg1, POS_XYZ, &local_F4);
            }
            break;
        }
      }
      local_A2 = local_A2 + 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  script_92E9(arg0, 1);
  {
    int32_t local_94;
    local_94 = task_48[arg2];
    local_9C = Script_ScenDataUtil(task_0, SCND_ReadU32, local_94 * 13 + 2) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_9C, 1);
    local_9C = (Script_ScenDataUtil(task_0, SCND_ReadU32, local_94 * 13 + 2) >> 16) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_9C, 1);
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_94 * 13 + 1);
      local_84 = local_84 & -9;
      Script_ScenDataUtil(task_0, SCND_WriteU32, local_94 * 13 + 1, local_84);
    }
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_94 * 13 + 1);
      local_84 = local_84 & -17;
      Script_ScenDataUtil(task_0, SCND_WriteU32, local_94 * 13 + 1, local_84);
    }
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_94 * 13 + 1);
      local_84 = local_84 & -5;
      Script_ScenDataUtil(task_0, SCND_WriteU32, local_94 * 13 + 1, local_84);
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1535E)))
static void script_5F05(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Vector3i local_F4;
  Vector3 local_E8;
  float local_E4;
  float local_E0;
  Vector3 local_D4;
  Vector3 local_C8;
  int32_t unused_BC[3];
  Vector3 local_B0;
  int8_t local_AF;
  uint8_t unused_AC[3];
  float local_A8;
  float local_A4;
  float local_A0;
  float local_9C;
  float local_98;
  float local_94;
  float local_90;
  int32_t local_8C;
  int32_t local_88;
  int32_t local_84;
  int32_t local_80;
  int32_t local_FF7C;
  int32_t local_FF78;
  int32_t local_FF74;
  int32_t local_FF70;
  int32_t local_FF6C;
  int32_t local_FF68;
  local_FF68 = arg4;
  switch (local_FF68) {
    case 0:
      local_AF = 0;
      local_8C = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
      local_A0 = -1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 46421;
      local_FF74 = 48787;
      break;
    case 1:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = -1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 50426;
      local_FF74 = 48969;
      break;
    case 2:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
      local_A0 = 1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 15109;
      local_FF74 = 16566;
      break;
    case 3:
      local_AF = 0;
      local_8C = Script_ScenDataUtil(data_13DB0, SCND_ReadU32, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = 1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 19114;
      local_FF74 = 16748;
      break;
  }
  (void)local_FF78;
  *(int32_t *)&local_9C = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_13DB0, SCND_ReadFloatBits, Script_ScenDataUtil(data_137C0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 4);
  local_A8 = 0.0f;
  Script_charaPosRotUtils(arg1, 12, 0, &local_F4, CHARA_PART_Get);
  local_80 = (&local_F4)->y;
  Script_charaPosRotUtils(arg1, 7, 0, &local_F4, CHARA_PART_Get);
  local_FF7C = (&local_F4)->y;
  if ((arg4 == 2) | (arg4 == 3)) {
    local_FF70 = local_80;
    Script_charaPosRotUtils(arg1, 12, &local_D4, 0, CHARA_PART_Get);
  } else {
    local_FF70 = local_FF7C;
    Script_charaPosRotUtils(arg1, 7, &local_D4, 0, CHARA_PART_Get);
  }
  Script_EVT_GetPosition(arg1, -1, &local_E8, 0);
  Script_mtwkGetSetRotation(arg1, &local_F4, 0);
  local_FF6C = (&local_F4)->y;
  local_E0 = (&local_D4)->z * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FF6C) - (&local_D4)->x * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FF6C);
  local_E4 = (&local_D4)->z * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FF6C) + (&local_D4)->x * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FF6C);
  local_D4.x = local_E4 + (&local_E8)->x;
  (&local_D4)->y = (&local_D4)->y + (&local_E8)->y;
  (&local_D4)->z = local_E0 + (&local_E8)->z;
  local_8C = local_8C | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_9C - 3.0f) << 16) & 0x7FFF0000);
  script_92E9(arg0, 0);
  local_B0.x = (&task_168)->x;
  (&local_B0)->y = task_168.y;
  (&local_B0)->z = task_168.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), local_8C);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
      break;
    }
    if (Script_GetCharaMoveStatus(arg0) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  local_8C = local_8C & 0xFFFF;
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
      Script_sprintf("This Motion don't Finish!!\012");
      break;
    }
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    local_A4 = Script_GetCharaMotionRemainingFramesScaled(arg0);
    if (!(local_A4 < local_9C) & !(local_A4 > local_98)) {
      if (local_AF == 1) {
      } else {
        local_A8 = local_A8 - 0.208874f;
      }
      if (!(local_A4 < local_9C) & !(local_A4 >= local_9C + 1.0f)) {
        int32_t local_FF64;
        local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
        local_FF64 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_FF64 * 9 + 1) * 4 + 2);
        if (local_FF64 != -1) {
          if (arg0 == task_160) {
            Script_executeAicaCommand(local_FF64, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_FF64, 4, &local_B0, 5.0f, 20.0f, 0);
          }
        }
      }
      Script_EVT_GetPosition(arg0, local_88, &local_C8, ENTITY_XFORM_WORLD_SPACE);
      local_FF70 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_D4)->x - (&local_C8)->x, (&local_D4)->z - (&local_C8)->z);
      local_FF70 = local_FF70 - local_FF6C;
      local_FF70 = local_FF70 + local_FF78 + (int32_t)(local_A8 * local_A0 / 360.0f * (float)0x10000);
    }
    if (!(local_A4 < local_94) & !(local_A4 > local_90)) {
      if (local_AF == 0) {
      } else {
        local_A8 = local_A8 + 0.12844f;
      }
      if (!(local_A4 < local_90) & !(local_A4 >= local_90 + 1.0f)) {
        int32_t local_FF64;
        local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
        local_FF64 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_FF64 * 9 + 1) * 4 + 3);
        if (local_FF64 != -1) {
          if (arg0 == task_160) {
            Script_executeAicaCommand(local_FF64, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_FF64, 4, &local_B0, 5.0f, 20.0f, 0);
          }
        }
      }
      Script_EVT_GetPosition(arg0, local_84, &local_C8, ENTITY_XFORM_WORLD_SPACE);
      local_FF70 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_D4)->x - (&local_C8)->x, (&local_D4)->z - (&local_C8)->z);
      local_FF70 = local_FF70 - local_FF6C;
      local_FF70 = local_FF70 + local_FF74 + (int32_t)(local_A8 * local_A0 / 360.0f * (float)0x10000);
    }
    local_F4.x = 0;
    (&local_F4)->z = 0;
    if ((arg4 == 2) | (arg4 == 3)) {
      (&local_F4)->y = local_FF70;
      Script_charaPosRotUtils(arg1, 12, 0, &local_F4, CHARA_PART_Set);
    } else {
      (&local_F4)->y = local_FF70;
      Script_charaPosRotUtils(arg1, 7, 0, &local_F4, CHARA_PART_Set);
    }
    SCNF_WaitFrames_0000(1);
  }
  script_92E9(arg0, 1);
  local_F4.x = 0;
  (&local_F4)->z = 0;
  (&local_F4)->y = local_80;
  Script_charaPosRotUtils(arg1, 12, 0, &local_F4, CHARA_PART_Set);
  (&local_F4)->y = local_FF7C;
  Script_charaPosRotUtils(arg1, 7, 0, &local_F4, CHARA_PART_Set);
  return;
}

static void script_668F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  int32_t local_F8;
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), MN_AKI_AKI_BASE);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
      break;
    }
    if (Script_GetCharaMoveStatus(arg0) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  local_F8 = 0;
  for (;;) {
    if (local_F8 == 0) {
      local_F8 = 1;
      if (arg5 != 0) {
        local_FC = 11;
      } else {
        local_FC = Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 7);
      }
      if (local_FC == 0) {
        SCNF_WaitFrames_0000(45);
      }
      Script_EnqueueSCNT(1, 0x47524F44, local_FC);
    } else {
      if (Script_EnqueueSCNT(2) == 0) {
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1537A)))
static void script_6767(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Vector3 local_F4;
  float local_F0;
  float local_EC;
  float local_E8;
  int32_t unused_E4;
  int32_t local_E0;
  int32_t local_DC;
  int32_t local_D8;
  int32_t local_D4;
  int32_t unused_D0;
  int32_t local_CC;
  local_DC = 0;
  local_D4 = Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 6);
  local_CC = arg3;
  if ((local_CC == 1) | (local_CC == 3) | (local_CC == 5) | (local_CC == 7) | (local_CC == 9) | (local_CC == 18) | (local_CC == 21) | (local_CC == 25) | (local_CC == 27) | (local_CC == 36) | (local_CC == 37)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_147D0, SCND_ReadU32, local_D4 * 5) | -0x80000000;
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_147D0, SCND_ReadU32, local_D4 * 5);
        break;
    }
  } else if ((local_CC == 2) | (local_CC == 4) | (local_CC == 6) | (local_CC == 8) | (local_CC == 10) | (local_CC == 19) | (local_CC == 22) | (local_CC == 24) | (local_CC == 26) | (local_CC == 28)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_147D0, SCND_ReadU32, local_D4 * 5);
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_147D0, SCND_ReadU32, local_D4 * 5) | -0x80000000;
        break;
    }
  } else {
    if ((local_CC == 13) | (local_CC == 14) | (local_CC == 15) | (local_CC == 16) | (local_CC == 20) | (local_CC == 23) | (local_CC == 29)) {
      local_CC = arg4;
      switch (local_CC) {
        case 0:
          local_E0 = Script_ScenDataUtil(&data_147D0, SCND_ReadU32, local_D4 * 5);
          break;
        case 1:
          local_E0 = Script_ScenDataUtil(&data_147D0, SCND_ReadU32, local_D4 * 5) | -0x80000000;
          break;
        case 2:
          local_E0 = Script_ScenDataUtil(&data_147D0, SCND_ReadU32, local_D4 * 5);
          break;
        case 3:
          local_E0 = Script_ScenDataUtil(&data_147D0, SCND_ReadU32, local_D4 * 5) | -0x80000000;
          break;
      }
      goto L006988;
    }
  }
  L006988:;
  *(int32_t *)&local_EC = Script_ScenDataUtil(&data_147D0, SCND_ReadFloatBits, local_D4 * 5 + 1);
  *(int32_t *)&local_E8 = Script_ScenDataUtil(&data_147D0, SCND_ReadFloatBits, local_D4 * 5 + 2);
  local_E0 = local_E0 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_EC - 3.0f) << 16) & 0x7FFF0000);
  local_F4.x = (&task_168)->x;
  (&local_F4)->y = task_168.y;
  (&local_F4)->z = task_168.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), local_E0);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
      break;
    }
    if (Script_GetCharaMoveStatus(arg0) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
      Script_sprintf("This Motion don't Finish!!\012");
      break;
    }
    if (Script_IsCharaMotionFinished(arg0)) {
      if (local_DC == 1) {
        if (Script_EnqueueSCNT(2) == 0) {
          break;
        }
      }
    }
    local_F0 = Script_GetCharaMotionRemainingFramesScaled(arg0);
    if (!(local_F0 < local_EC) & !(local_F0 >= local_EC + 1.0f)) {
      int32_t local_C8;
      local_C8 = Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 9);
      if (local_C8 != -1) {
        if (arg0 == task_160) {
          Script_executeAicaCommand(local_C8, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_C8, 4, &local_F4, 5.0f, 20.0f, 0);
        }
      }
    }
    if (!(local_F0 < local_E8) & !(local_F0 >= local_E8 + 1.0f)) {
      if ((local_D4 == 6) | (local_D4 == 7)) {
        int32_t local_C8;
        local_C8 = Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 9);
        if (local_C8 != -1) {
          if (arg0 == task_160) {
            Script_executeAicaCommand(local_C8, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_C8, 4, &local_F4, 5.0f, 20.0f, 0);
          }
        }
      }
      if (local_DC == 0) {
        local_DC = 1;
        local_D8 = Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 7);
        if (local_D8 == 0) {
          SCNF_WaitFrames_0000(45);
        }
        Script_EnqueueSCNT(1, 0x47524F44, local_D8);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_15396)))
static void script_6BDE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  float local_DC[3];
  Vector3 local_D0;
  float local_C4[3];
  Vector3 local_B8;
  int32_t unused_AC[3];
  int8_t local_AB;
  int8_t local_AA;
  uint8_t unused_A8[2];
  float local_A4;
  float local_A0;
  float local_9C;
  float local_98;
  float local_94;
  int32_t local_90;
  int32_t local_8C;
  int32_t unused_88;
  int32_t local_84;
  int32_t local_80;
  int32_t local_FF7C;
  int32_t local_FF78;
  int32_t local_FF74;
  int32_t unused_FF70;
  int32_t local_FF6C;
  int32_t local_FF68;
  int32_t local_FF64;
  int32_t local_FF60;
  int32_t local_FF5C;
  int32_t local_FF58;
  int32_t unused_FF54;
  Vector3i local_FF48;
  local_AA = 0;
  local_FF5C = 0;
  local_FF58 = 0;
  task_18C = 0;
  local_90 = 1543;
  Script_EVT_GetPosition(arg1, -1, &local_D0, 0);
  Script_mtwkGetSetRotation(arg1, &local_FF48, 0);
  local_DC[0] = 0.0f;
  local_DC[1] = 0.0f;
  local_DC[2] = 0.0f;
  {
    int32_t local_FF44;
    local_FF44 = arg3;
    if ((local_FF44 == 4) | (local_FF44 == 6) | (local_FF44 == 33) | (local_FF44 == 35)) {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 1:
          local_AB = 1;
          local_9C = 1.0f;
          local_FF74 = 0;
          break;
        case 0:
          local_AB = 0;
          local_90 = local_90 | -0x80000000;
          local_9C = 1.0f;
          local_FF74 = 1;
          break;
      }
    } else if ((local_FF44 == 1) | (local_FF44 == 7) | (local_FF44 == 30) | (local_FF44 == 36) | (local_FF44 == 37)) {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 1:
          local_AB = 0;
          local_90 = local_90 | -0x80000000;
          local_9C = 1.0f;
          local_FF74 = 1;
          break;
        case 0:
          local_AB = 1;
          local_9C = 1.0f;
          local_FF74 = 0;
          break;
      }
    } else if ((local_FF44 == 3) | (local_FF44 == 5) | (local_FF44 == 32) | (local_FF44 == 34)) {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 1:
          local_AB = 1;
          local_90 = local_90 | -0x80000000;
          local_9C = -1.0f;
          local_FF74 = 1;
          break;
        case 0:
          local_AB = 0;
          local_9C = -1.0f;
          local_FF74 = 0;
          break;
      }
    } else if ((local_FF44 == 2) | (local_FF44 == 8) | (local_FF44 == 31)) {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 1:
          local_AB = 0;
          local_9C = -1.0f;
          local_FF74 = 0;
          break;
        case 0:
          local_AB = 1;
          local_90 = local_90 | -0x80000000;
          local_9C = -1.0f;
          local_FF74 = 1;
          break;
      }
    } else {
      if ((local_FF44 == 13) | (local_FF44 == 14)) {
        local_FF44 = arg4;
        switch (local_FF44) {
          case 0:
            local_AB = 0;
            local_9C = -1.0f;
            local_FF74 = 0;
            break;
          case 1:
            local_AB = 1;
            local_90 = local_90 | -0x80000000;
            local_9C = -1.0f;
            local_FF74 = 1;
            break;
          case 2:
            local_AB = 1;
            local_9C = 1.0f;
            local_FF74 = 0;
            break;
          case 3:
            local_AB = 0;
            local_90 = local_90 | -0x80000000;
            local_9C = 1.0f;
            local_FF74 = 1;
            break;
        }
        local_AA = 1;
        goto L006EEA;
      }
    }
  }
  L006EEA:;
  if (local_AA) {
    {
      int32_t unused_FF44;
      Vector3i local_FF38;
      Script_charaPosRotUtils(arg1, 12, 0, &local_FF38, CHARA_PART_Get);
      local_84 = (&local_FF38)->y;
    }
    {
      int32_t unused_FF44;
      Vector3i local_FF38;
      Script_charaPosRotUtils(arg1, 7, 0, &local_FF38, CHARA_PART_Get);
      local_80 = (&local_FF38)->y;
    }
    if ((arg4 == 2) | (arg4 == 3)) {
      local_FF7C = local_84;
      Script_charaPosRotUtils(arg1, 12, &local_F4, 0, CHARA_PART_Get);
    } else {
      local_FF7C = local_80;
      Script_charaPosRotUtils(arg1, 7, &local_F4, 0, CHARA_PART_Get);
    }
    {
      int32_t unused_FF44;
      Vector3i local_FF38;
      Vector3 local_FF2C;
      Script_EVT_GetPosition(arg1, -1, &local_FF2C, 0);
      Script_mtwkGetSetRotation(arg1, &local_FF38, 0);
      local_FF78 = (&local_FF38)->y;
      {
        float local_FF28;
        float local_FF24;
        local_FF24 = (&local_F4)->z * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FF78) - (&local_F4)->x * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FF78);
        local_FF28 = (&local_F4)->z * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FF78) + (&local_F4)->x * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FF78);
        local_F4.x = local_FF28 + (&local_FF2C)->x;
        (&local_F4)->y = (&local_F4)->y + (&local_FF2C)->y;
        (&local_F4)->z = local_FF24 + (&local_FF2C)->z;
      }
    }
  } else {
    int32_t unused_FF44;
    Vector3i local_FF38;
    Script_mtwkGetSetRotation(arg1, &local_FF38, 0);
    local_84 = (&local_FF38)->y;
    local_80 = (&local_FF38)->y;
    local_FF7C = (&local_FF38)->y;
    Script_EVT_GetPosition(arg1, -1, &local_F4, 0);
    local_FF78 = 0;
  }
  local_98 = 33.0f;
  local_94 = 49.0f;
  local_B8.x = (&task_168)->x;
  (&local_B8)->y = task_168.y;
  (&local_B8)->z = task_168.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), local_90);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
      break;
    }
    if (Script_GetCharaMoveStatus(arg0) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x524F4F44, 1);
  while ((local_8C = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x524F4F44)) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_8C) == 1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_motmGetCharaFlags(SHEN_NAME('DOOR'), MOTM_PoseNone, SCEN_Write);
  Script_PlayCharaMotion(SHEN_NAME('DOOR'), MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(SHEN_NAME('DOOR'), CDM_Show);
  {
    int32_t local_FF44;
    local_FF44 = arg3;
    if ((local_FF44 == 0) | (local_FF44 == 1) | (local_FF44 == 3) | (local_FF44 == 5) | (local_FF44 == 7) | (local_FF44 == 9) | (local_FF44 == 11) | (local_FF44 == 25) | (local_FF44 == 27) | (local_FF44 == 30) | (local_FF44 == 32) | (local_FF44 == 34) | (local_FF44 == 36) | (local_FF44 == 37)) {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 0:
          local_DC[0] = 0.45f;
          local_DC[2] = 0.5f;
          break;
        case 1:
          local_DC[0] = -0.45f;
          local_DC[2] = 0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
        case 2:
          local_DC[0] = 0.45f;
          local_DC[2] = 0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
        case 3:
          local_DC[0] = -0.45f;
          local_DC[2] = 0.5f;
          break;
      }
    } else if ((local_FF44 == 2) | (local_FF44 == 4) | (local_FF44 == 6) | (local_FF44 == 8) | (local_FF44 == 10) | (local_FF44 == 12) | (local_FF44 == 24) | (local_FF44 == 26) | (local_FF44 == 31) | (local_FF44 == 33) | (local_FF44 == 35)) {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 0:
          local_DC[0] = -0.45f;
          local_DC[2] = 0.5f;
          break;
        case 1:
          local_DC[0] = 0.45f;
          local_DC[2] = 0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
        case 2:
          local_DC[0] = -0.45f;
          local_DC[2] = 0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
        case 3:
          local_DC[0] = 0.45f;
          local_DC[2] = 0.5f;
          break;
      }
    } else if ((local_FF44 == 13) | (local_FF44 == 14) | (local_FF44 == 15) | (local_FF44 == 16) | (local_FF44 == 17) | (local_FF44 == 18) | (local_FF44 == 19) | (local_FF44 == 20) | (local_FF44 == 21) | (local_FF44 == 22) | (local_FF44 == 23) | (local_FF44 == 29)) {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 0:
          local_DC[0] = -0.45f;
          local_DC[2] = 0.5f;
          break;
        case 1:
          local_DC[0] = 0.45f;
          local_DC[2] = 0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
        case 2:
          local_DC[0] = -0.45f;
          local_DC[2] = 0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
        case 3:
          local_DC[0] = 0.45f;
          local_DC[2] = 0.5f;
          break;
      }
    } else if (local_FF44 == 28) {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 0:
          local_DC[0] = -0.45f;
          local_DC[2] = -0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
        case 1:
          local_DC[0] = -0.45f;
          local_DC[2] = 0.5f;
          break;
        case 2:
          local_DC[0] = 0.45f;
          local_DC[2] = 0.5f;
          break;
        case 3:
          local_DC[0] = 0.45f;
          local_DC[2] = -0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
      }
    } else {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 0:
          local_DC[0] = -0.45f;
          local_DC[2] = -0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
        case 1:
          local_DC[0] = -0.45f;
          local_DC[2] = 0.5f;
          break;
        case 2:
          local_DC[0] = 0.45f;
          local_DC[2] = 0.5f;
          break;
        case 3:
          local_DC[0] = 0.45f;
          local_DC[2] = -0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
      }
    }
  }
  local_FF60 = (&local_FF48)->y & 0xFFFF;
  local_C4[2] = local_DC[2] * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FF60) - *local_DC * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FF60);
  local_C4[0] = local_DC[2] * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FF60) + *local_DC * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FF60);
  local_D0.x = (&local_F4)->x + *local_C4;
  (&local_D0)->y = (&local_F4)->y;
  (&local_D0)->z = (&local_F4)->z + local_C4[2];
  Script_EVT_SetPosition(SHEN_NAME('DOOR'), POS_XYZ, &local_D0);
  Script_EVT_SetRotation(SHEN_NAME('DOOR'), 0x38000000, &local_FF48, 0);
  local_A4 = 0.0f;
  local_FF68 = 0;
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
      Script_sprintf("This Motion don't Finish!!\012");
      break;
    }
    if (Script_IsCharaMotionFinished(arg0)) {
      if (local_FF68 == 1) {
        if (Script_EnqueueSCNT(2) == 0) {
          break;
        }
      }
    }
    if (local_FF68 == 1) {
      if (local_FF5C == 0) {
        int32_t unused_FF44;
        int32_t local_FF40;
        SCNF_WaitFrames_0000(45);
        local_FF40 = Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 10);
        if (local_FF40 != -1) {
          if (arg0 == task_160) {
            Script_executeAicaCommand(local_FF40, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
          }
        }
        local_FF5C = 1;
      } else {
        if (local_FF58 <= 20) {
          local_A4 = local_A4 - 0.05f;
          local_FF7C = local_FF7C + (int32_t)(local_A4 * local_9C / 360.0f * (float)0x10000);
          if (local_AA) {
            int32_t unused_FF44;
            Vector3i local_FF38;
            local_FF38.x = 0;
            (&local_FF38)->z = 0;
            if ((arg4 == 2) | (arg4 == 3)) {
              (&local_FF38)->y = local_FF7C;
              Script_charaPosRotUtils(arg1, 12, 0, &local_FF38, CHARA_PART_Set);
            } else {
              (&local_FF38)->y = local_FF7C;
              Script_charaPosRotUtils(arg1, 7, 0, &local_FF38, CHARA_PART_Set);
            }
          } else {
            int32_t unused_FF44;
            Vector3i local_FF38;
            (&local_FF38)->y = local_FF7C;
            Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_FF38, 0);
          }
          local_FF58 = local_FF58 + 1;
        }
      }
    }
    local_A0 = Script_GetCharaMotionRemainingFramesScaled(arg0);
    if (!(local_A0 < local_98) & !(local_A0 >= local_98 + 1.0f)) {
      int32_t unused_FF44;
      int32_t local_FF40;
      local_FF40 = Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 9);
      if (local_FF40 != -1) {
        if (arg0 == task_160) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
    }
    if (!(local_A0 < local_94) & !(local_A0 >= local_94 + 1.0f)) {
      int32_t unused_FF44;
      int32_t local_FF40;
      local_FF40 = Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 9);
      if (local_FF40 != -1) {
        if (arg0 == task_160) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
      if (local_FF68 == 0) {
        local_FF68 = 1;
        local_FF64 = Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 8);
        Script_ConversationUtils(1, local_FF74);
        Script_EnqueueSCNT(1, 0x524F4F44, local_FF64);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  local_A4 = 0.0f;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) != 0) {
    local_FF6C = 0;
    L0079B5:;
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
    } else {
      local_A4 = local_A4 + 0.05f;
      local_FF7C = local_FF7C + (int32_t)(local_A4 * local_9C / 360.0f * (float)0x10000);
      if (local_AA) {
        int32_t unused_FF44;
        Vector3i local_FF38;
        local_FF38.x = 0;
        (&local_FF38)->z = 0;
        if ((arg4 == 2) | (arg4 == 3)) {
          (&local_FF38)->y = local_FF7C;
          Script_charaPosRotUtils(arg1, 12, 0, &local_FF38, CHARA_PART_Set);
        } else {
          (&local_FF38)->y = local_FF7C;
          Script_charaPosRotUtils(arg1, 7, 0, &local_FF38, CHARA_PART_Set);
        }
      } else {
        int32_t unused_FF44;
        Vector3i local_FF38;
        (&local_FF38)->y = local_FF7C;
        Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_FF38, 0);
      }
      SCNF_WaitFrames_0000(1);
      local_FF6C = local_FF6C + 1;
      if (!(local_FF6C >= 20)) goto L0079B5;
    }
    {
      int32_t unused_FF44;
      int32_t local_FF40;
      local_FF40 = Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 11);
      if (local_FF40 != -1) {
        if (arg0 == task_160) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
    }
  }
  if (local_AA) {
    int32_t unused_FF44;
    Vector3i local_FF38;
    local_FF38.x = 0;
    (&local_FF38)->z = 0;
    (&local_FF38)->y = local_84;
    Script_charaPosRotUtils(arg1, 12, 0, &local_FF38, CHARA_PART_Set);
    (&local_FF38)->y = local_80;
    Script_charaPosRotUtils(arg1, 7, 0, &local_FF38, CHARA_PART_Set);
  } else {
    {
      int32_t unused_FF44;
      Vector3i local_FF38;
      (&local_FF38)->y = local_84;
      Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_FF38, 0);
    }
    {
      int32_t unused_FF44;
      Vector3i local_FF38;
      (&local_FF38)->y = local_80;
      Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_FF38, 0);
    }
  }
  Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, local_8C);
  return;
}

static void script_7B71(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  int32_t unused_F8;
  float local_EC[3];
  Vector3 local_E0;
  float local_D4[3];
  Vector3 local_C8;
  int32_t unused_C4;
  int32_t local_BC[2];
  Vector3i local_B0;
  int32_t local_AC;
  Script_sprintf("open-move!!\012");
  local_BC[1] = arg4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) != 0) {
    Script_EVT_GetPosition(arg1, -1, &local_E0, 0);
    Script_EVT_GetPosition(arg0, -1, &local_C8, 0);
    Script_mtwkGetSetRotation(arg0, &local_B0, 0);
    local_EC[0] = 0.0f;
    local_EC[1] = 0.0f;
    local_EC[2] = 0.0f;
    if (arg0 != task_160) {
      local_AC = arg2;
      if ((local_AC == 0) | (local_AC == 1) | (local_AC == 3) | (local_AC == 5) | (local_AC == 7) | (local_AC == 9) | (local_AC == 11) | (local_AC == 25) | (local_AC == 27) | (local_AC == 30) | (local_AC == 32) | (local_AC == 34) | (local_AC == 36) | (local_AC == 37)) {
        local_AC = arg3;
        switch (local_AC) {
          case 0:
            local_EC[0] = 0.45f;
            local_EC[2] = 1.0f;
            break;
          case 1:
            local_EC[0] = 0.45f;
            local_EC[2] = -1.0f;
            break;
          case 2:
            local_EC[0] = 0.45f;
            local_EC[2] = -1.0f;
            break;
          case 3:
            local_EC[0] = 0.45f;
            local_EC[2] = 1.0f;
            break;
        }
      } else if ((local_AC == 2) | (local_AC == 4) | (local_AC == 6) | (local_AC == 8) | (local_AC == 10) | (local_AC == 12) | (local_AC == 24) | (local_AC == 26) | (local_AC == 31) | (local_AC == 33) | (local_AC == 35)) {
        local_AC = arg3;
        switch (local_AC) {
          case 0:
            local_EC[0] = -0.45f;
            local_EC[2] = 1.0f;
            break;
          case 1:
            local_EC[0] = -0.45f;
            local_EC[2] = -1.0f;
            break;
          case 2:
            local_EC[0] = 0.45f;
            local_EC[2] = -1.0f;
            break;
          case 3:
            local_EC[0] = 0.45f;
            local_EC[2] = 1.0f;
            break;
        }
      } else if ((local_AC == 13) | (local_AC == 14) | (local_AC == 15) | (local_AC == 16) | (local_AC == 17) | (local_AC == 18) | (local_AC == 19) | (local_AC == 20) | (local_AC == 21) | (local_AC == 22) | (local_AC == 23) | (local_AC == 29) | (local_AC == 28)) {
        local_AC = arg3;
        switch (local_AC) {
          case 0:
            local_EC[0] = 0.45f;
            local_EC[2] = 1.0f;
            break;
          case 1:
            local_EC[0] = 0.45f;
            local_EC[2] = -1.0f;
            break;
          case 2:
            local_EC[0] = -0.45f;
            local_EC[2] = -1.0f;
            break;
          case 3:
            local_EC[0] = -0.45f;
            local_EC[2] = 1.0f;
            break;
        }
      } else {
        local_AC = arg3;
        switch (local_AC) {
          case 0:
            local_EC[0] = 0.45f;
            local_EC[2] = 1.0f;
            break;
          case 1:
            local_EC[0] = 0.45f;
            local_EC[2] = -1.0f;
            break;
          case 2:
            local_EC[0] = -0.45f;
            local_EC[2] = -1.0f;
            break;
          case 3:
            local_EC[0] = -0.45f;
            local_EC[2] = 1.0f;
            break;
        }
      }
      local_FC = local_BC[1] & 0xFFFF;
      local_D4[2] = local_EC[2] * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FC) - *local_EC * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FC);
      local_D4[0] = local_EC[2] * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FC) + *local_EC * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FC);
      local_E0.x = (&local_E0)->x + *local_D4;
      (&local_E0)->z = (&local_E0)->z + local_D4[2];
      if ((arg3 == 0) | (arg3 == 3)) {
        local_BC[1] = local_BC[1] + 0x8000;
      }
      Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
      Script_SetCharaMoveTarget(arg0, &local_E0, local_BC[1], script_0697(arg5));
    }
  }
  return;
}

static void script_7FBB(int32_t arg0, DSWKDoorEntry *arg1, int32_t *arg2, DoorResourceDescriptor *arg3, MapObjectRecord *arg4, int32_t arg5, int32_t arg6) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t unused_F4;
  task_0 = arg1;
  task_4 = arg2;
  task_8 = arg3;
  task_C = arg4;
  task_10.mode = 0;
  task_10.id = 0;
  task_10.entry_idx = 0;
  task_160 = arg0;
  (void)((task_184 = -1) & 1);
  task_180 = 0;
  Script_motiLoad(Script_GetSceneID(), SHEN_NAME('FREE'), "M_MDOR.BIN");
  local_F8 = 0;
  L008025:;
  local_FC = Script_AddCharacterHandler(1, 0, local_F8 * 0x1000000 + 0x30726F64);
  Script_PreserveAccumulator_012C(local_FC, "This Charactor Work is NULL!!\012");
  DSWK_AllocDoorMapcRenderState(local_FC);
  Script_scenToggleVisibleFor(local_FC, 0);
  task_E8[local_F8] = local_FC;
  task_20[local_F8] = -1;
  task_48[local_F8] = -1;
  task_70[local_F8] = 0;
  task_98[local_F8] = 0;
  task_C0[local_F8] = 0;
  task_110[local_F8] = 0;
  local_F8 = local_F8 + 1;
  if (!(local_F8 >= 10)) goto L008025;
  local_F8 = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_0, SCND_ReadU32, local_F8 * 13) == -1) {
      break;
    }
    Script_ScenDataUtil(task_0, SCND_WriteU32, local_F8 * 13 + 1, 0);
    local_F8 = local_F8 + 1;
  }
  Script_EnqueueSCNT(0);
  DSWK_BindTablesAndInitState(arg1, arg2, arg3, arg4, &task_10, task_20, task_48, (shen_name *)task_70, task_98, arg0, arg5, data_137C0, data_13DB0, (shen_name *)task_E8);
  Script_scenCreateTask(script_8877, 0);
  return;
}

static void script_8147(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4) {
  task_0 = arg0;
  task_4 = arg1;
  task_8 = arg2;
  task_C = arg3;
  DSWK_RebindDoorSystemTables(arg0, arg1, arg2, arg3, arg4);
  task_180 = 1;
  return;
}

static void script_817B(void) {
  int32_t local_FC;
  local_FC = 0;
  L008183:;
  task_20[local_FC] = -1;
  task_48[local_FC] = -1;
  task_70[local_FC] = 0;
  task_98[local_FC] = 0;
  task_C0[local_FC] = 0;
  task_110[local_FC] = 0;
  local_FC = local_FC + 1;
  if (!(local_FC >= 10)) goto L008183;
  return;
}

static void script_81E4(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_F4 = 0;
  for (;;) {
    Script_sprintf("手動ドア開け番号指定\012");
    if (task_160 == arg0) {
      Script_EVT_GetPosition(arg0, -1, &task_194, 0);
    }
    local_F8 = DSWK_DoorUtils(DOOR_CMD_ALLOC_ENTRY_SLOT, arg0, arg1);
    if (local_F8 != -1) {
      for (;;) {
        local_FC = DSWK_DoorUtils(DOOR_CMD_TYPE13, local_F8);
        if (local_FC != 2) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      if (local_FC != -2) {
        break;
      }
      local_F4 = local_F4 + 1;
      if (local_F4 >= 120) {
        Script_sprintf("無限ループになりそうなので抜ける\012");
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  (void)local_FC;
  return;
}

static void script_8281(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  Vector3 local_F0;
  if (task_160 == arg0) {
    Script_EVT_GetPosition(arg0, -1, &task_194, 0);
  }
  Script_EVT_GetPosition(arg0, -1, &local_F0, 0);
  local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_CLOSEST_ELEMENT, arg0, &local_F0, arg1);
  (void)local_FC;
  return;
}

static void script_82BB(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 13) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 13 + 1);
    local_F8 = local_F8 | 2;
    Script_ScenDataUtil(task_0, SCND_WriteU32, local_FC * 13 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_831E(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 13) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 13 + 1);
    local_F8 = local_F8 & -3;
    Script_ScenDataUtil(task_0, SCND_WriteU32, local_FC * 13 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_8381(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_0, SCND_ReadU32, arg0 * 13 + 1);
  local_FC = local_FC | 2;
  Script_ScenDataUtil(task_0, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  return;
}

static void script_83B8(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_0, SCND_ReadU32, arg0 * 13 + 1);
  local_FC = local_FC & -3;
  Script_ScenDataUtil(task_0, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  return;
}

static void script_83EF(int32_t arg0) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  local_F0 = 0;
  for (;;) {
    if (task_48[local_F0] == arg0) {
      local_EC = task_E8[local_F0];
      if (Script_shenGetNode(local_EC, 7)) {
        if (Script_shenGetNode(local_EC, 5) & Script_shenGetNode(local_EC, 10)) {
          if (Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[local_F0] * 13 + 1) & 0x100) {
            local_F4.x = 0.0f;
            (&local_F4)->y = 0.0f;
            (&local_F4)->z = 0.0f;
            Script_charaPosRotUtils(local_EC, 10, &local_F4, 0, CHARA_PART_Set);
            Script_charaPosRotUtils(local_EC, 5, &local_F4, 0, CHARA_PART_Set);
          } else {
            local_F4.x = 0.0f;
            (&local_F4)->y = -1200.0f;
            (&local_F4)->z = 0.0f;
            Script_charaPosRotUtils(local_EC, 10, &local_F4, 0, CHARA_PART_Set);
            Script_charaPosRotUtils(local_EC, 5, &local_F4, 0, CHARA_PART_Set);
          }
        }
      } else {
        if (Script_shenGetNode(local_EC, 10)) {
          if (Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[local_F0] * 13 + 1) & 0x100) {
            local_F4.x = 0.0f;
            (&local_F4)->y = 0.0f;
            (&local_F4)->z = 0.0f;
            Script_charaPosRotUtils(local_EC, 10, &local_F4, 0, CHARA_PART_Set);
          } else {
            local_F4.x = 0.0f;
            (&local_F4)->y = -1200.0f;
            (&local_F4)->z = 0.0f;
            Script_charaPosRotUtils(local_EC, 10, &local_F4, 0, CHARA_PART_Set);
          }
        }
      }
      if (Script_shenGetNode(local_EC, 8) & Script_shenGetNode(local_EC, 13)) {
        if (Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[local_F0] * 13 + 1) & 0x20) {
          local_F4.x = 0.0f;
          (&local_F4)->y = 0.0f;
          (&local_F4)->z = 0.0f;
          Script_charaPosRotUtils(local_EC, 13, &local_F4, 0, CHARA_PART_Set);
          (&local_F4)->y = -1000.0f;
          Script_charaPosRotUtils(local_EC, 8, &local_F4, 0, CHARA_PART_Set);
        } else {
          local_F4.x = 0.0f;
          (&local_F4)->y = -1000.0f;
          (&local_F4)->z = 0.0f;
          Script_charaPosRotUtils(local_EC, 13, &local_F4, 0, CHARA_PART_Set);
          (&local_F4)->y = 0.0f;
          Script_charaPosRotUtils(local_EC, 8, &local_F4, 0, CHARA_PART_Set);
        }
      }
    }
    local_F0 = local_F0 + 1;
    if (local_F0 == 10) {
      break;
    }
  }
  return;
}

static void script_8631(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_0, SCND_ReadU32, arg0 * 13 + 1);
  local_FC = local_FC | 0x20;
  Script_ScenDataUtil(task_0, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  script_8739(arg0);
  script_83EF(arg0);
  return;
}

static void script_867A(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_0, SCND_ReadU32, arg0 * 13 + 1);
  local_FC = local_FC & -33;
  Script_ScenDataUtil(task_0, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  script_8747(arg0);
  script_83EF(arg0);
  return;
}

static void script_86C3(int32_t arg0, int32_t arg1) {
  if (arg1) {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_0, SCND_ReadU32, arg0 * 13 + 1);
    local_FC = local_FC & -257;
    Script_ScenDataUtil(task_0, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  } else {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_0, SCND_ReadU32, arg0 * 13 + 1);
    local_FC = local_FC | 0x100;
    Script_ScenDataUtil(task_0, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  }
  return;
}

static void script_8739(int32_t arg0) {
  script_86C3(arg0, 0);
  return;
}

static void script_8747(int32_t arg0) {
  script_86C3(arg0, 1);
  return;
}

static void script_8755(int32_t arg0) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 13) == -1) {
      break;
    }
    if (arg0) {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 13 + 1);
      local_F8 = local_F8 & -65;
      local_F8 = local_F8 & -129;
      local_F8 = local_F8 | 0x40;
      Script_ScenDataUtil(task_0, SCND_WriteU32, local_FC * 13 + 1, local_F8);
    } else {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 13 + 1);
      local_F8 = local_F8 & -65;
      local_F8 = local_F8 & -129;
      local_F8 = local_F8 | 0x80;
      Script_ScenDataUtil(task_0, SCND_WriteU32, local_FC * 13 + 1, local_F8);
    }
    local_FC = local_FC + 1;
  }
  return;
}

static void script_881C(void) {
  script_8755(1);
  return;
}

static void script_8828(void) {
  script_8755(0);
  return;
}

static void script_8834(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 13) == -1) {
      break;
    }
    DSWK_AssignDoorEntryConfig(local_FC, arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    local_FC = local_FC + 1;
  }
  return;
}

static int32_t script_8877(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  local_F4 = -1;
  local_F8 = -1;
  for (;;) {
    if (task_180 == 0) {
      local_F4 = DSWK_DoorUtils(DOOR_CMD_FIND_LOWEST_FLOAT_INDEX, 0x3FD851EA);
      if (-(local_F4 != -1) & -(task_20[local_F4] == 1) & Script_IsPlayerAllowed(0) & -((&task_10)->mode == 0)) {
        local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[local_F4], 2);
        local_F8 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_FC * 9 + 1) * 4 + 1);
        script_9647(1, local_FC, local_F4, local_F8);
        if (script_97FB(local_FC, local_F4, local_F8)) {
          local_F4 = -1;
          local_F8 = -1;
        }
      } else {
        script_9647(0, local_FC, local_F4, local_F8);
        local_F4 = -1;
        local_F8 = -1;
      }
      script_9A11(local_F4, local_F8);
    } else {
      task_180 = 0;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int8_t script_8988(int32_t arg0, int32_t arg1, int8_t arg2) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  arg2 = 0;
  Script_EVT_GetPosition(task_160, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(task_160, &local_DC, 0x42000000);
  local_E8.x = (&local_F4)->x;
  (&local_E8)->y = (&local_F4)->y;
  (&local_E8)->z = (&local_F4)->z;
  if (DSWK_DoorUtils(DOOR_CMD_CALC_VECTOR_FROM_ANGLE, arg0, &local_E8, &local_D8)) {
    local_D8 = ((&local_DC)->y - local_D8) & 0xFFFF;
    if ((local_D8 <= 21845) | (local_D8 >= 43691)) {
      if (DSWK_DoorUtils(DOOR_CMD_TEST_LINE_INTERSECTION, &local_F4, &local_E8) == 0) {
        arg2 = 1;
      }
    }
  }
  return arg2;
}

__attribute__((scn_string_bindings(0, data_15421, 1, data_1543A)))
static void script_8A20(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  Vector3i local_E0;
  local_F8 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg0], 1);
  if (local_F8 != -1) {
    local_FC = Script_AddCharacterHandler(1, 0, arg0 * 0x1000000 + 0x30786F62);
    if (local_FC == 0) {
      Script_PreserveAccumulator_012C(local_FC, "Don't make BOX Buffer!!\012");
      task_138[arg0] = -1;
    } else if (local_FC == arg0 * 0x1000000 + 0x30786F62) {
      task_138[arg0] = local_FC;
      *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_C, SCND_ReadU32, local_F8 * 9 + 5);
      *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_C, SCND_ReadU32, local_F8 * 9 + 6);
      *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_C, SCND_ReadU32, local_F8 * 9 + 7);
      Script_EVT_SetPosition(local_FC, POS_XYZ, &local_EC);
      local_E0.x = 0;
      (&local_E0)->y = Script_ScenDataUtil(task_C, SCND_ReadU32, local_F8 * 9 + 8);
      (&local_E0)->z = 0;
      Script_EVT_SetRotation(local_FC, 0x38000000, &local_E0, 0);
      *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_C, SCND_ReadU32, local_F8 * 9 + 2);
      *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_C, SCND_ReadU32, local_F8 * 9 + 3);
      *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_C, SCND_ReadU32, local_F8 * 9 + 4);
      Script_hmdlApplyScale(local_FC, &local_EC);
      Script_CreateNPCImageModule(local_FC, (const char *)Script_ScenDataUtil(task_8, SCND_ReadScriptOffset, Script_ScenDataUtil(task_C, SCND_ReadU32, local_F8 * 9 + 1) * 4), 0);
      Script_charaDisplay(local_FC, CDM_Show);
      Script_charaVisUtils(local_FC, 1);
    } else {
      Script_PreserveAccumulator_012C(local_FC, "Don't make BOX Buffer!!\012");
      task_138[arg0] = -1;
    }
  } else {
    task_138[arg0] = -1;
  }
  return;
}

static void script_8BEF(int32_t arg0) {
  if (task_138[arg0] != -1) {
    Script_SetMapControlTaskFlag(task_138[arg0], 1);
    task_138[arg0] = -1;
  }
  return;
}

static int32_t script_8C20(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5, int32_t arg6) {
  Vector3 local_F4;
  Vector3 local_E8;
  float local_DC[3];
  Vector3i local_D0;
  Vector3i local_C4;
  int32_t local_C0;
  int32_t local_BC;
  if (task_160 == arg0) {
    local_F4.x = (&task_194)->x;
    (&local_F4)->y = task_194.y;
    (&local_F4)->z = task_194.z;
  } else {
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  }
  Script_EVT_GetPosition(arg1, -1, &local_E8, 0);
  Script_mtwkGetSetRotation(arg1, &local_D0, 0);
  local_C4.x = (&local_D0)->x;
  (&local_C4)->y = (&local_D0)->y;
  (&local_C4)->z = (&local_D0)->z;
  local_C0 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_F4)->x - (&local_E8)->x, (&local_F4)->z - (&local_E8)->z);
  local_C0 = ((local_C0 - (&local_D0)->y) >> 14) & 3;
  if (arg2 != 99) {
    if (arg2 == 6) {
      if ((local_C0 == 0) | (local_C0 == 1)) {
        if ((Script_ScenDataUtil(task_0, SCND_ReadU32, arg6 * 13 + 1) & 8) | (Script_ScenDataUtil(task_0, SCND_ReadU32, arg6 * 13 + 1) & 0x40)) {
          local_C0 = local_C0 ^ 3;
        }
      } else {
        if ((Script_ScenDataUtil(task_0, SCND_ReadU32, arg6 * 13 + 1) & 0x10) | (Script_ScenDataUtil(task_0, SCND_ReadU32, arg6 * 13 + 1) & 0x80)) {
          local_C0 = local_C0 ^ 3;
        }
      }
      arg2 = 4;
    }
    {
      int32_t unused_B8;
      int32_t local_B4;
      local_B4 = arg5;
      if ((local_B4 == 0) | (local_B4 == 1) | (local_B4 == 3) | (local_B4 == 5) | (local_B4 == 7) | (local_B4 == 9) | (local_B4 == 11) | (local_B4 == 25) | (local_B4 == 27) | (local_B4 == 30) | (local_B4 == 32) | (local_B4 == 34) | (local_B4 == 36) | (local_B4 == 37)) {
        local_B4 = local_C0;
        if ((local_B4 == 0) | (local_B4 == 3)) {
          arg4 = (float)0 - arg4;
          (&local_C4)->y = (&local_C4)->y + 0x8000;
        } else {
          if ((local_B4 == 1) | (local_B4 == 2)) goto L008E0B;
        }
        L008E0B:;
      } else if ((local_B4 == 2) | (local_B4 == 4) | (local_B4 == 6) | (local_B4 == 8) | (local_B4 == 10) | (local_B4 == 12) | (local_B4 == 24) | (local_B4 == 26) | (local_B4 == 31) | (local_B4 == 33) | (local_B4 == 35)) {
        local_B4 = local_C0;
        if ((local_B4 == 2) | (local_B4 == 1)) {
          arg3 = (float)0 - arg3;
        } else {
          if ((local_B4 == 3) | (local_B4 == 0)) {
            arg3 = (float)0 - arg3;
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            goto L008EAE;
          }
        }
        L008EAE:;
      } else if ((local_B4 == 13) | (local_B4 == 14) | (local_B4 == 15) | (local_B4 == 16) | (local_B4 == 17) | (local_B4 == 18) | (local_B4 == 19) | (local_B4 == 20) | (local_B4 == 21) | (local_B4 == 22) | (local_B4 == 23) | (local_B4 == 29)) {
        local_B4 = local_C0;
        switch (local_B4) {
          case 0:
            arg3 = (float)0 - arg3;
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            break;
          case 1:
            arg3 = (float)0 - arg3;
            break;
          case 2:
            break;
          case 3:
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            break;
        }
      } else if (local_B4 == 28) {
        local_B4 = local_C0;
        switch (local_B4) {
          case 0:
            arg3 = (float)0 - arg3;
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            break;
          case 1:
            arg3 = (float)0 - arg3;
            break;
          case 2:
            break;
          case 3:
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            break;
        }
      } else {
        local_B4 = local_C0;
        if ((local_B4 == 2) | (local_B4 == 1)) {
        } else {
          if ((local_B4 == 3) | (local_B4 == 0)) {
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            goto L00905A;
          }
        }
      }
    }
    L00905A:;
    local_BC = (&local_D0)->y & 0xFFFF;
    local_DC[2] = arg4 * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_BC) - arg3 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_BC);
    local_DC[0] = arg4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_BC) + arg3 * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_BC);
    if (arg2 == -1) {
      int32_t unused_B4[2];
      Vector3 local_A8;
      int32_t local_A4;
      local_A8.x = (&local_E8)->x + *local_DC;
      (&local_A8)->y = (&local_E8)->y;
      (&local_A8)->z = (&local_E8)->z + local_DC[2];
      local_A4 = DSWK_DoorUtils(DOOR_CMD_TEST_LINE_INTERSECTION, &local_F4, &local_A8);
      if ((local_C0 == 1) | (local_C0 == 2)) {
        local_C0 = 1;
      } else {
        local_C0 = 0;
      }
      if (local_A4) {
        local_C0 = -1;
      }
    } else {
      int32_t unused_B8;
      int32_t local_B4;
      local_F4.x = (&local_E8)->x + *local_DC;
      (&local_F4)->y = (&local_E8)->y;
      (&local_F4)->z = (&local_E8)->z + local_DC[2];
      local_B4 = arg2;
      switch (local_B4) {
        case 0:
          Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
          Script_EVT_SetRotation(arg0, 0x38000000, &local_C4, 0);
          break;
        case 2:
          Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
          Script_EVT_SetRotation(arg0, 0x38000000, &local_C4, 0);
          if ((local_C0 == 1) | (local_C0 == 2)) {
            local_C0 = 1;
          } else {
            local_C0 = 0;
          }
          break;
        case 3:
          if ((local_C0 == 1) | (local_C0 == 2)) {
            local_C0 = 1;
          } else {
            local_C0 = 0;
          }
          break;
        case 4:
          task_168.x = (&local_F4)->x;
          task_168.y = (&local_F4)->y;
          task_168.z = (&local_F4)->z;
          task_174 = (&local_C4)->x;
          *(&task_174 + 1) = (&local_C4)->y;
          *(&task_174 + 2) = (&local_C4)->z;
          break;
        case 5:
          task_168.x = (&local_F4)->x;
          task_168.y = (&local_F4)->y;
          task_168.z = (&local_F4)->z;
          task_174 = (&local_C4)->x;
          *(&task_174 + 1) = (&local_C4)->y;
          *(&task_174 + 2) = (&local_C4)->z;
          if ((local_C0 == 1) | (local_C0 == 2)) {
            local_C0 = 1;
          } else {
            local_C0 = 0;
          }
          break;
        case 7:
          Script_EVT_GetPosition(arg0, -1, &task_168, 0);
          task_174 = (&local_C4)->x;
          *(&task_174 + 1) = (&local_C4)->y;
          *(&task_174 + 2) = (&local_C4)->z;
          break;
      }
    }
  }
  return local_C0;
}

static void script_92E9(int32_t arg0, int32_t arg1) {
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    if (task_160 == arg0) {
      if (arg1) {
        Script_ccowUtils(arg0, CCOW_EnableMap);
      } else {
        Script_ccowUtils(arg0, CCOW_DisableMap);
      }
    }
  }
  return;
}

static void script_931B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
  if (task_160 != arg0) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      Script_charaToggleMotEnabled(arg0, 1);
    }
  } else if ((local_F8 == 0) | (local_F8 == 1)) {
    if ((Script_ScenDataUtil(task_0, SCND_ReadU32, arg2 * 13 + 1) & 8) == 0) {
      if ((Script_ScenDataUtil(task_0, SCND_ReadU32, arg2 * 13 + 1) & 0x40) != 0) {
        local_FC = Script_ScenDataUtil(task_0, SCND_ReadU32, arg2 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      } else {
        local_FC = (Script_ScenDataUtil(task_0, SCND_ReadU32, arg2 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      }
    }
  } else {
    if ((Script_ScenDataUtil(task_0, SCND_ReadU32, arg2 * 13 + 1) & 0x10) == 0) {
      if ((Script_ScenDataUtil(task_0, SCND_ReadU32, arg2 * 13 + 1) & 0x80) != 0) {
        local_FC = (Script_ScenDataUtil(task_0, SCND_ReadU32, arg2 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      } else {
        local_FC = Script_ScenDataUtil(task_0, SCND_ReadU32, arg2 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      }
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_15453)))
static void script_948B(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (task_160 == arg0) {
    task_10.mode = 0;
    task_10.id = 0;
    if ((arg2 == 0) | (arg2 == 2)) {
      Script_PlayCharaMotion(task_160, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      if (task_184 & 0x20) {
        Script_sprintf("Door System : ");
        Script_ryoControl(RYO_RestoreControl);
      }
      Script_shenClearFreeState(1);
      if (arg2 != 2) {
        int32_t local_F8;
        local_F8 = task_48[arg1];
        local_FC = Script_ScenDataUtil(task_0, SCND_ReadU32, local_F8 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 1);
        local_FC = (Script_ScenDataUtil(task_0, SCND_ReadU32, local_F8 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 1);
      }
    }
  } else {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      Script_charaToggleMotEnabled(arg0, 0);
    }
    if ((arg2 == 0) | (arg2 == 2)) {
      int32_t local_F8;
      local_F8 = task_48[arg1];
      local_FC = Script_ScenDataUtil(task_0, SCND_ReadU32, local_F8 * 13 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
      local_FC = (Script_ScenDataUtil(task_0, SCND_ReadU32, local_F8 * 13 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
    }
  }
  task_20[arg1] = 1;
  task_110[arg1] = 0;
  task_C0[arg1] = 0;
  task_70[arg1] = 0;
  if (task_138[arg1] != -1) {
    Script_SetMapControlTaskFlag(task_138[arg1], 1);
    task_138[arg1] = -1;
  }
  return;
}

static void script_9647(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  Vector3 local_E8;
  local_FC = 0;
  if (arg0 == 1) {
    local_F4 = arg3;
    if ((local_F4 == 17) | (local_F4 == 11) | (local_F4 == 12)) {
      local_FC = 0;
    } else if ((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 2) == 0) {
      if ((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 4) == 0) {
        local_FC = script_8988(arg2, arg3, local_FC);
      } else {
        local_F4 = arg3;
        if ((local_F4 == 21) | (local_F4 == 22) | (local_F4 == 23)) {
          local_FC = script_8988(arg2, arg3, local_FC);
          goto L00970B;
        }
      }
      L00970B:;
    } else {
      if ((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 4) == 0) {
        local_FC = script_8988(arg2, arg3, local_FC);
        local_F8 = Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 6);
        if (local_F8 == 0) {
          local_FC = 0;
        } else {
          if (local_F8 >= 2) {
            Script_EVT_GetPosition(task_160, -1, &local_E8, 0);
            if (Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_E8, 1.5f)) {
              local_FC = 0;
            }
            goto L0097A6;
          }
        }
      }
    }
  }
  L0097A6:;
  if (task_110[arg2]) {
    local_FC = 0;
  }
  Script_scenUtils(SCENARIO_CMD_SET_HELP_PROMPT_OVERRIDE, local_FC);
  if ((local_FC ^ task_190) & local_FC) {
    Script_sicnUtils(0, 4, 1);
  }
  if ((local_FC ^ task_190) & task_190) {
    Script_sicnUtils(2, 4, 0);
  }
  task_190 = local_FC;
  return;
}

__attribute__((scn_string_bindings(0, data_15462)))
static int32_t script_97FB(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  local_FC = 1;
  local_F8 = arg2;
  if ((local_F8 == 17) | (local_F8 == 11) | (local_F8 == 12)) {
    if (((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg1] * 13 + 1) & 2) == 0) & ((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg1] * 13 + 1) & 4) == 0)) {
      if (task_110[arg1] == 0) {
        task_20[arg1] = 4;
        local_FC = 0;
      }
    }
  } else {
    if (Script_GetControllerInput(1) == 0x400) {
      if (-(task_70[arg1] != task_160) & task_190) {
        if ((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg1] * 13 + 1) & 2) == 0) {
          task_20[arg1] = 3;
          task_70[arg1] = task_160;
          task_98[arg1] = 0;
          task_10.mode = 1;
          task_10.entry_idx = task_48[arg1];
          local_FC = 0;
        } else {
          if (Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg1] * 13 + 6) >= 2) {
            Script_EVT_GetPosition(task_160, -1, &local_EC, 0);
            if (Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_EC, 1.5f) == 0) {
              task_20[arg1] = 6;
              task_70[arg1] = task_160;
              task_98[arg1] = 0;
              task_10.mode = 0;
              task_10.entry_idx = task_48[arg1];
              local_FC = 0;
            }
            goto L0099AF;
          }
        }
        L0099AF:;
        if (local_FC == 0) {
          Script_EVT_GetPosition(task_160, -1, &task_194, 0);
          Script_ryoControl(PCCTL_RequestNewMode, 4);
          Script_sprintf("Door System : ");
          Script_shenClearFreeState(0);
          Script_ryoControl(PCCTL_DisablePlayerControl);
          task_10.id = task_160;
          while (Script_ryoControl(PCCTL_QueryRequestIdle) == 0) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ryoControl(PCCTL_RequestNewMode, 0);
        }
      }
    }
  }
  end:;
}

static void script_9A11(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  L009A19:;
  local_F8 = task_20[local_FC];
  switch (local_F8) {
    case 6:
      if (!(arg0 != -1)) goto L009A39;
      L009A39:;
      script_9ADB(local_FC, arg1);
      break;
    case 5:
      if (!(arg0 != -1)) goto L009A59;
      L009A59:;
      script_9B3C(local_FC);
      break;
    case 4:
      if (!(arg0 != -1)) goto L009A77;
      L009A77:;
      script_9CE1(local_FC, arg0, arg1);
      break;
    case 3:
      if (!(arg0 != -1)) goto L009A99;
      L009A99:;
      script_9D79(local_FC, arg1);
      break;
    case 2:
      script_9E5B(local_FC);
      break;
    case 1:
      break;
  }
  local_FC = local_FC + 1;
  if (!(local_FC >= 10)) goto L009A19;
  return;
}

static void script_9ADB(int32_t arg0, int32_t arg1) {
  if (task_110[arg0] == 0) {
    task_110[arg0] = Script_scenCreateTask(script_1C76, 4, task_70[arg0], task_E8[arg0], arg0, arg1);
    task_C0[arg0] = task_70[arg0];
    script_8A20(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_15471, 1, data_15483)))
static void script_9B3C(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, task_70[arg0])) {
    if (task_110[arg0] == 0) {
      if (task_70[arg0] == task_160) {
        task_10.mode = 1;
        task_10.entry_idx = task_48[arg0];
        goto L009B91;
      }
      L009B91:;
      local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg0], 2);
      local_F8 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_FC * 9 + 1) * 4 + 1);
      script_931B(task_70[arg0], task_E8[arg0], task_48[arg0], local_F8);
      task_110[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_14870, SCND_ReadScriptOffset, local_F8), 4, task_70[arg0], task_E8[arg0], arg0, local_F8);
      task_C0[arg0] = task_70[arg0];
      if ((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg0] * 13 + 1) & 0x20000) == 0) {
        Script_sprintf("Door Box is ON!!\012");
        script_8A20(arg0);
      } else {
        Script_sprintf("Door Box is OFF!!\012");
        task_138[arg0] = -1;
      }
    }
  } else {
    if (task_20[arg0] != 5) {
      task_20[arg0] = 1;
      task_110[arg0] = 0;
      task_C0[arg0] = 0;
      task_70[arg0] = 0;
    }
  }
  return;
}

static void script_9CE1(int32_t arg0, int32_t arg1, int32_t arg2) {
  if ((task_110[arg0] == 0) & (arg2 >= 0)) {
    task_70[arg0] = task_160;
    task_10.mode = 2;
    task_10.entry_idx = task_48[arg0];
    task_110[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_14870, SCND_ReadScriptOffset, arg2), 4, task_70[arg0], task_E8[arg0], arg0, arg2);
    task_C0[arg0] = task_70[arg0];
    script_8A20(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_15496, 1, data_154A8)))
static void script_9D79(int32_t arg0, int32_t arg1) {
  if ((task_110[arg0] == 0) & (arg1 >= 0)) {
    script_931B(task_70[arg0], task_E8[arg0], task_48[arg0], arg1);
    task_110[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_14870, SCND_ReadScriptOffset, arg1), 4, task_70[arg0], task_E8[arg0], arg0, arg1);
    task_C0[arg0] = task_70[arg0];
    if ((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg0] * 13 + 1) & 0x20000) == 0) {
      Script_sprintf("Door Box is ON!!\012");
      script_8A20(arg0);
    } else {
      Script_sprintf("Door Box is OFF!!\012");
      task_138[arg0] = -1;
    }
  }
  return;
}

static void script_9E5B(int32_t arg0) {
  int32_t local_FC;
  task_110[arg0] = 0;
  task_C0[arg0] = 0;
  task_20[arg0] = 1;
  local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg0], 2);
  if (local_FC != -1) {
    task_70[arg0] = 0;
  }
  return;
}

static void script_9EB2(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  return;
}

static void script_9EDA(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
  return;
}

static void script_9F2E(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg1)) {
      Script_EVT_GetPosition(arg1, 4, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(arg0, 16, &local_F4, arg2);
      Script_FaceLookAtTargetUtils(arg0, FACE_LookAtPosition, &local_F4, 30);
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_9F7E(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int8_t local_E8[12];
  int32_t local_E4;
  uint8_t unused_E3;
  int8_t local_E2;
  uint8_t unused_E0[2];
  Script_ReturnTimeDate(&local_F4);
  local_E2 = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, arg0);
  local_E8[1] = (local_E2 >> 5) & 7;
  if (local_E8[1] == 0) {
    local_E8[1] = 12;
  }
  local_E8[2] = local_E2 & 0x1F;
  if ((&local_F4)->month == local_E8[1]) {
    local_E4 = (&local_F4)->day - local_E8[2];
  } else {
    if (local_E8[1] == 12) {
      local_E8[1] = 0;
    }
    local_E4 = ((&local_F4)->month - local_E8[1]) * 31 + (&local_F4)->day - local_E8[2];
  }
  return local_E4;
}

static void script_A025(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_9F7E(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static int32_t script_A048(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = (&local_F4)->hours * 60 + (&local_F4)->minutes;
  local_EC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, arg0) * 6;
  return local_F0 - local_EC;
}

static void script_A07B(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_A048(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_A09E(int32_t arg0, int32_t arg1) {
  int8_t local_F4[12];
  int32_t local_F0;
  local_F0 = script_A048(arg0);
  local_F4[4] = local_F0 / 60;
  local_F4[5] = local_F0 % 60;
  if ((local_F4[4] < *(int8_t *)(arg1 + 4)) | ((local_F4[4] == *(int8_t *)(arg1 + 4)) & (local_F4[5] < *(int8_t *)(arg1 + 5)))) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_A0FD(int32_t arg0) {
  int32_t local_F8[2];
  local_F8[0] = arg0;
  local_F8[1] = 0;
  (void)local_F8;
  return;
}

static void script_A112(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_E0 = arg3;
  switch (local_E0) {
    case 0:
      local_F4.x = 0.1f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -0.01f;
      local_E8.x = 0;
      (&local_E8)->y = 64443;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
    case 1:
      local_F4.x = 0.163f;
      (&local_F4)->y = 0.015f;
      (&local_F4)->z = 0.014f;
      local_E8.x = 0;
      (&local_E8)->y = 0x8000;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
    case 2:
      local_F4.x = 0.064f;
      (&local_F4)->y = -0.071f;
      (&local_F4)->z = -0.366f;
      local_E8.x = 22570;
      (&local_E8)->y = 47422;
      (&local_E8)->z = 50974;
      local_E4 = 18;
      break;
    case 3:
      local_F4.x = 0.1f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -0.036f;
      local_E8.x = 0;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
    case 4:
      local_F4.x = 0.563f;
      (&local_F4)->y = -0.18f;
      (&local_F4)->z = -0.033f;
      local_E8.x = 0;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0x3000;
      local_E4 = 18;
      break;
    case 5:
      local_F4.x = 0.129f;
      (&local_F4)->y = -0.037f;
      (&local_F4)->z = -0.233f;
      local_E8.x = 18313;
      (&local_E8)->y = 46148;
      (&local_E8)->z = 56615;
      local_E4 = 18;
      break;
    case 6:
      local_F4.x = 0.143f;
      (&local_F4)->y = 0.024f;
      (&local_F4)->z = 0.01f;
      local_E8.x = 10920;
      (&local_E8)->y = 11829;
      (&local_E8)->z = 4550;
      local_E4 = 18;
      break;
    case 7:
      local_F4.x = -0.066f;
      (&local_F4)->y = -1.05f;
      (&local_F4)->z = -0.413f;
      local_E8.x = 5367;
      (&local_E8)->y = 0x2000;
      (&local_E8)->z = 2274;
      local_E4 = 18;
      break;
    case 8:
      local_F4.x = 0.135f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -0.384f;
      local_E8.x = 46789;
      (&local_E8)->y = 49970;
      (&local_E8)->z = 20929;
      local_E4 = 18;
      break;
    case 9:
      local_F4.x = 0.1f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -0.02f;
      local_E8.x = 0;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
  }
  if (Script_motnUtils(MOTN_GetCharacterBudgetFlag, arg0)) {
    Script_motnUtils(MOTN_SetCharacterBudgetFlag, arg0, 0);
  }
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  Script_PlayCharaMotion(arg0, arg2, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  Script_fixoAttachEx(arg1, arg0, local_E4, &local_F4, &local_E8);
  Script_motmGetCharaFlags(arg0, MOTM_PoseHigh, SCEN_Write);
  if (Script_motnUtils(MOTN_GetCharacterBudgetFlag, arg0)) {
    Script_motnUtils(MOTN_SetCharacterBudgetFlag, arg0, 1);
  }
  return;
}

static void script_A4A8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_E0 = arg3;
  switch (local_E0) {
    case 0:
      local_F4.x = 0.066f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -0.016f;
      local_E8.x = 0xC000;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0xD000;
      local_E4 = 18;
      break;
    case 1:
      local_F4.x = 0.071f;
      (&local_F4)->y = -0.9f;
      (&local_F4)->z = -0.197f;
      local_E8.x = 1820;
      (&local_E8)->y = -64350;
      (&local_E8)->z = 364;
      local_E4 = 18;
      break;
    case 2:
      local_F4.x = 0.147f;
      (&local_F4)->y = -0.86f;
      (&local_F4)->z = -0.35f;
      local_E8.x = 3913;
      (&local_E8)->y = 0;
      (&local_E8)->z = 910;
      local_E4 = 18;
      break;
    case 3:
      local_F4.x = -0.29f;
      (&local_F4)->y = -1.095f;
      (&local_F4)->z = -0.123f;
      local_E8.x = 43146;
      (&local_E8)->y = 47880;
      (&local_E8)->z = 19113;
      local_E4 = 18;
      break;
    case 4:
      local_F4.x = 0.113f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -0.035f;
      local_E8.x = 182;
      (&local_E8)->y = 62441;
      (&local_E8)->z = 65171;
      local_E4 = 18;
      break;
    case 5:
      local_F4.x = 0.088f;
      (&local_F4)->y = 0.036f;
      (&local_F4)->z = -0.048f;
      local_E8.x = 5279;
      (&local_E8)->y = 37523;
      (&local_E8)->z = 5825;
      local_E4 = 18;
      break;
    case 6:
      local_F4.x = 0.103f;
      (&local_F4)->y = 0.012f;
      (&local_F4)->z = 0.021f;
      local_E8.x = 59892;
      (&local_E8)->y = 3845;
      (&local_E8)->z = 364;
      local_E4 = 12;
      break;
    case 7:
      local_F4.x = 0.1136f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = 0.013f;
      local_E8.x = 44540;
      (&local_E8)->y = 43679;
      (&local_E8)->z = 13666;
      local_E4 = 18;
      break;
    case 8:
      local_F4.x = 0.131f;
      (&local_F4)->y = -0.045f;
      (&local_F4)->z = -0.375f;
      local_E8.x = 18204;
      (&local_E8)->y = 47058;
      (&local_E8)->z = 58166;
      local_E4 = 18;
      break;
    case 9:
      local_F4.x = 0.07f;
      (&local_F4)->y = 0.01f;
      (&local_F4)->z = -0.02f;
      local_E8.x = 0x4000;
      (&local_E8)->y = 0xC000;
      (&local_E8)->z = 0xA000;
      local_E4 = 18;
      break;
    case 10:
      local_F4.x = 0.049f;
      (&local_F4)->y = 0.001f;
      (&local_F4)->z = -0.011f;
      local_E8.x = 64169;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
    case 11:
      local_F4.x = 0.1f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -0.02f;
      local_E8.x = 0;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
    case 12:
      local_F4.x = 0.114f;
      (&local_F4)->y = -0.018f;
      (&local_F4)->z = -0.055f;
      local_E8.x = 0x1000;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
    case 13:
      local_F4.x = 0.257f;
      (&local_F4)->y = -0.072f;
      (&local_F4)->z = 0.064f;
      local_E8.x = 0xF000;
      (&local_E8)->y = 0xA000;
      (&local_E8)->z = 0x2000;
      local_E4 = 18;
      break;
    case 14:
      local_F4.x = 0.061f;
      (&local_F4)->y = 0.015f;
      (&local_F4)->z = 0.026f;
      local_E8.x = 19842;
      (&local_E8)->y = 61986;
      (&local_E8)->z = 55977;
      local_E4 = 12;
      break;
    case 15:
      local_F4.x = 0.2f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = 0.03f;
      local_E8.x = 0xC000;
      (&local_E8)->y = 30947;
      (&local_E8)->z = 0x4000;
      local_E4 = 12;
      break;
    case 16:
      local_F4.x = 0.089f;
      (&local_F4)->y = 0.016f;
      (&local_F4)->z = -0.017f;
      local_E8.x = 0x4000;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
    case 17:
      local_F4.x = 0.11f;
      (&local_F4)->y = 0.055f;
      (&local_F4)->z = -0.005f;
      local_E8.x = 47336;
      (&local_E8)->y = 18232;
      (&local_E8)->z = 59134;
      local_E4 = 18;
      break;
    case 18:
      local_F4.x = 0.132f;
      (&local_F4)->y = 0.002f;
      (&local_F4)->z = -0.019f;
      local_E8.x = 36416;
      (&local_E8)->y = 33701;
      (&local_E8)->z = 47303;
      local_E4 = 18;
      break;
    case 19:
      local_F4.x = 0.134f;
      (&local_F4)->y = -0.033f;
      (&local_F4)->z = -0.036f;
      local_E8.x = 0;
      (&local_E8)->y = 41859;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
  }
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  Script_PlayCharaMotion(arg0, arg2, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  Script_fixoAttachEx(arg1, arg0, local_E4, &local_F4, &local_E8);
  Script_motmGetCharaFlags(arg0, MOTM_PoseHigh, SCEN_Write);
  return;
}

__attribute__((scn_string_bindings(0, data_15B08)))
static void script_AB36(void) {
  Script_sprintf("TUNED FreeTalk def\012");
  SCNT_Enqueue();
  return;
}

__attribute__((scn_string_bindings(0, data_15B1C)))
static void script_AB45(void) {
  Script_sprintf("TUNED FreeTalk def\012");
  SCNT_Enqueue();
  return;
}

static void script_AB54(void) {
  return;
}

static void script_AB57(void) {
  return;
}

static void script_AB5A(void) {
  return;
}

static void script_AB5D(void) {
  return;
}

static void script_AB60(void) {
  (void)((task_1AC = 0) & 0x20);
  return;
}

static void script_AB6D(void) {
  task_1A0 = Script_freeTalkGetCurrentChara();
  return;
}

static void script_AB79(void) {
  *(&task_1A0 + 1) = Script_GetFreeTalkStatus();
  return;
}

static int32_t script_AB88(void) {
  return *(&task_1A0 + 2) = Script_freeTalkGetAndResetState();
}

static void script_AB97(void) {
  int32_t local_FC;
  local_FC = Script_GetSceneID();
  switch (local_FC) {
    case 1:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ENTRY2_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3044484A, 2, &data_15BA0, 0xD5A6C3, TimedTransitionUpdateMemoItem150);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_ENTRY20_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3030444A, 20, &data_15B68, 0xDA3AA3, 0);
      }
      break;
    case 2:
      if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_STARTED) == 0) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_ADVANCED) == 0)) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 2, 0x54434D47, 1, &data_15B30, 0xB7420F, 0);
      }
      break;
    case 3:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x4F4D4F4A, 20, &data_15B4C, 0x2DC9FE, TimedTransitionRollbackToPreviousDay2330);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIRED_FROM_JOB) & ~Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 970)) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_ENTRY42_TIMED_EVENT_DONE) == 0) {
          Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x5953464D, 42, &data_15B84, 0xBBB817, 0);
        }
      }
      break;
  }
  return;
}

static void script_ACCF(int32_t arg0) {
  script_ACE7(arg0);
  return;
}

static void script_ACDB(int32_t arg0) {
  script_ACE7(arg0);
  return;
}

static void script_ACE7(int32_t arg0) {
  if (Script_StepUtils(GET_STEP_HIGHER, 0) == 22) {
    int32_t local_FC;
    local_FC = Script_StepUtils(GET_STEP_LOWER, 0);
    if ((local_FC == 2) | (local_FC == 3)) {
      Script_SetShouldSleepFlag(0);
      goto L00AD20;
    }
  }
  L00AD20:;
  Script_ryoSetChara(arg0);
  Script_facemotUtils(arg0, 1, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, FACE_ExpressionDisabled, 0, 2);
  Script_CharHandlers(arg0, -1, 0, 0);
  script_AB97();
  Script_SetCharaFlags(arg0, 4);
  Script_charaDisplay(arg0, CDM_Show);
  {
    int32_t local_FC;
    local_FC = Script_GetSceneID();
    if (local_FC == 2) {
      Script_LoadVoiceFile(1, "FREE02");
    } else if (local_FC == 3) {
      Script_LoadVoiceFile(1, "FREE03");
    } else if (local_FC == 4) {
      Script_LoadVoiceFile(1, "FREE04");
    } else {
      Script_LoadVoiceFile(1, "FREE01");
    }
  }
  Script_CPCTRelated(arg0, 32, 89);
  Script_clmdSetFlags(arg0, 8, 1);
  return;
}

static void script_ADDA(int32_t arg0) {
  script_ACDB(arg0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_ADF1(int32_t arg0) {
  Vector3i local_F4;
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_F4, 0);
  (&local_F4)->y = ((&local_F4)->y - arg0 + 10922) & 0xFFFF;
  if ((&local_F4)->y < 21845) {
    (void)-1;
  } else {
    (void)0;
  }
  return;
}

static void script_AE32(int32_t arg0) {
  Vector3i local_F4;
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_F4, 0x42000000);
  (&local_F4)->y = ((&local_F4)->y - arg0 + 10922) & 0xFFFF;
  if ((&local_F4)->y < 21845) {
    (void)-1;
  } else {
    (void)0;
  }
  return;
}

static void script_AE76(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  if ((local_FC == 0) | (local_FC == 5)) {
    Script_executeAicaCommand(5288, 0, 0);
  } else if (local_FC == 10) {
    Script_executeAicaCommand(70824, 0, 0);
  } else if (local_FC == 20) {
    Script_executeAicaCommand(136360, 0, 0);
  } else if (local_FC == 30) {
    Script_executeAicaCommand(201896, 0, 0);
  } else {
    Script_executeAicaCommand(267432, 0, 0);
  }
  return;
}

static void script_AEF4(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_28_REACHED)) {
    script_AE76(40);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_1E_REACHED)) {
    script_AE76(30);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_14_REACHED)) {
    script_AE76(20);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CATA1_NBO_BRANCH_CHOSEN)) {
    script_AE76(10);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP05_SA1116_DOJO_DONE)) {
    script_AE76(5);
  } else {
    script_AE76(0);
  }
  return;
}

static void script_AF73(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int8_t local_F3;
  int8_t local_F2;
  int8_t local_F1;
  uint8_t unused_EF[2];
  int8_t local_EE;
  int8_t local_ED;
  int8_t local_EC;
  int8_t local_EB;
  int8_t local_EA;
  int8_t local_E9;
  int8_t local_E8;
  int8_t local_E7;
  int8_t local_E6;
  int8_t local_DD[9];
  uint8_t unused_DC;
  int32_t local_D8;
  Script_ReturnTimeDate(&local_F4);
  local_EE = (&local_F4)->year;
  local_ED = (&local_F4)->month;
  local_EC = (&local_F4)->day;
  local_F3 = 0;
  local_D8 = local_ED;
  switch (local_D8) {
    case 12:
      while (local_F3 < 9) {
        local_DD[local_F3] = data_15F98[local_F3];
        local_F3 = local_F3 + 1;
      }
      break;
    case 1:
      while (local_F3 < 9) {
        local_DD[local_F3] = data_15FA4[local_F3];
        local_F3 = local_F3 + 1;
      }
      break;
    case 2:
      while (local_F3 < 9) {
        local_DD[local_F3] = data_15FB0[local_F3];
        local_F3 = local_F3 + 1;
      }
      break;
    case 3:
      while (local_F3 < 9) {
        local_DD[local_F3] = data_15FBC[local_F3];
        local_F3 = local_F3 + 1;
      }
      break;
  }
  local_F3 = 0;
  local_EB = 0;
  while (local_F3 < 9) {
    if (local_EC == local_DD[local_F3]) {
      local_EB = 1;
    }
    local_F3 = local_F3 + 1;
  }
  *(int8_t *)task_1D0 = Script_PlayerStatsUtils(PS_GetStepSlot, 0);
  *((int8_t *)task_1D0 + 1) = Script_PlayerStatsUtils(PS_GetStepSlot, 1);
  *((int8_t *)task_1D0 + 2) = Script_PlayerStatsUtils(PS_GetStepSlot, 2);
  *((int8_t *)task_1D0 + 3) = Script_PlayerStatsUtils(PS_GetStepSlot, 3);
  *((int8_t *)task_1D0 + 4) = Script_PlayerStatsUtils(PS_GetStepSlot, 4);
  *((int8_t *)task_1D0 + 5) = Script_PlayerStatsUtils(PS_GetStepSlot, 5);
  *((int8_t *)task_1D0 + 6) = Script_PlayerStatsUtils(PS_GetStepSlot, 6);
  *((int8_t *)task_1D0 + 7) = Script_PlayerStatsUtils(PS_GetStepSlot, 7);
  *((int8_t *)task_1D0 + 8) = Script_PlayerStatsUtils(PS_GetStepSlot, 8);
  *((int8_t *)task_1D0 + 9) = Script_PlayerStatsUtils(PS_GetStepSlot, 9);
  *((int8_t *)task_1D0 + 10) = Script_PlayerStatsUtils(PS_GetStepSlot, 10);
  *((int8_t *)task_1D0 + 11) = Script_PlayerStatsUtils(PS_GetStepSlot, 11);
  *((int8_t *)task_1D0 + 12) = Script_PlayerStatsUtils(PS_GetStepSlot, 12);
  *((int8_t *)task_1D0 + 13) = Script_PlayerStatsUtils(PS_GetStepSlot, 13);
  *((int8_t *)task_1D0 + 14) = Script_PlayerStatsUtils(PS_GetStepSlot, 14);
  *((int8_t *)task_1D0 + 15) = Script_PlayerStatsUtils(PS_GetStepSlot, 15);
  if (*(int8_t *)task_1D0 == 0) {
    *(int8_t *)task_1D0 = 8;
    Script_PlayerStatsUtils(PS_SetStepSlot, 0, *(int8_t *)task_1D0);
    *((int8_t *)task_1D0 + 1) = 4;
    Script_PlayerStatsUtils(PS_SetStepSlot, 1, *((int8_t *)task_1D0 + 1));
    *((int8_t *)task_1D0 + 2) = 7;
    Script_PlayerStatsUtils(PS_SetStepSlot, 2, *((int8_t *)task_1D0 + 2));
    *((int8_t *)task_1D0 + 3) = 9;
    Script_PlayerStatsUtils(PS_SetStepSlot, 3, *((int8_t *)task_1D0 + 3));
    *((int8_t *)task_1D0 + 4) = 6;
    Script_PlayerStatsUtils(PS_SetStepSlot, 4, *((int8_t *)task_1D0 + 4));
    *((int8_t *)task_1D0 + 5) = 5;
    Script_PlayerStatsUtils(PS_SetStepSlot, 5, *((int8_t *)task_1D0 + 5));
    *((int8_t *)task_1D0 + 6) = 2;
    Script_PlayerStatsUtils(PS_SetStepSlot, 6, *((int8_t *)task_1D0 + 6));
    *((int8_t *)task_1D0 + 7) = 5;
    Script_PlayerStatsUtils(PS_SetStepSlot, 7, *((int8_t *)task_1D0 + 7));
    *((int8_t *)task_1D0 + 8) = 7;
    Script_PlayerStatsUtils(PS_SetStepSlot, 8, *((int8_t *)task_1D0 + 8));
    *((int8_t *)task_1D0 + 9) = 3;
    Script_PlayerStatsUtils(PS_SetStepSlot, 9, *((int8_t *)task_1D0 + 9));
    *((int8_t *)task_1D0 + 10) = 2;
    Script_PlayerStatsUtils(PS_SetStepSlot, 10, *((int8_t *)task_1D0 + 10));
    *((int8_t *)task_1D0 + 11) = 5;
    Script_PlayerStatsUtils(PS_SetStepSlot, 11, *((int8_t *)task_1D0 + 11));
    *((int8_t *)task_1D0 + 12) = 6;
    Script_PlayerStatsUtils(PS_SetStepSlot, 12, *((int8_t *)task_1D0 + 12));
    *((int8_t *)task_1D0 + 13) = 3;
    Script_PlayerStatsUtils(PS_SetStepSlot, 13, *((int8_t *)task_1D0 + 13));
    *((int8_t *)task_1D0 + 14) = 2;
    Script_PlayerStatsUtils(PS_SetStepSlot, 14, *((int8_t *)task_1D0 + 14));
    *((int8_t *)task_1D0 + 15) = 4;
    Script_PlayerStatsUtils(PS_SetStepSlot, 15, *((int8_t *)task_1D0 + 15));
  }
  local_F3 = 0;
  local_F2 = 0;
  local_F1 = 0;
  while (1) {
    if ((*(int8_t *)task_1D0 == Script_PlayerStatsUtils(PS_GetStepSlot, 0)) | (*((int8_t *)task_1D0 + 1) == Script_PlayerStatsUtils(PS_GetStepSlot, 1)) | (*((int8_t *)task_1D0 + 2) == Script_PlayerStatsUtils(PS_GetStepSlot, 2)) | (*((int8_t *)task_1D0 + 3) == Script_PlayerStatsUtils(PS_GetStepSlot, 3)) | (*((int8_t *)task_1D0 + 4) == Script_PlayerStatsUtils(PS_GetStepSlot, 4)) | (*((int8_t *)task_1D0 + 5) == Script_PlayerStatsUtils(PS_GetStepSlot, 5))) {
      local_EA = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 6.0f);
      local_E9 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 6.0f);
      local_E8 = *((int8_t *)task_1D0 + local_EA);
      local_E7 = *((int8_t *)task_1D0 + local_E9);
      *((int8_t *)task_1D0 + local_EA) = local_E7;
      *((int8_t *)task_1D0 + local_E9) = local_E8;
    } else {
      local_F3 = 1;
    }
    if ((*((int8_t *)task_1D0 + 6) == Script_PlayerStatsUtils(PS_GetStepSlot, 6)) | (*((int8_t *)task_1D0 + 7) == Script_PlayerStatsUtils(PS_GetStepSlot, 7)) | (*((int8_t *)task_1D0 + 8) == Script_PlayerStatsUtils(PS_GetStepSlot, 8)) | (*((int8_t *)task_1D0 + 9) == Script_PlayerStatsUtils(PS_GetStepSlot, 9))) {
      local_EA = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f);
      local_E9 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f);
      local_E8 = *((int8_t *)task_1D0 + (local_EA + 6));
      local_E7 = *((int8_t *)task_1D0 + (local_E9 + 6));
      *((int8_t *)task_1D0 + (local_EA + 6)) = local_E7;
      *((int8_t *)task_1D0 + (local_E9 + 6)) = local_E8;
    } else {
      local_F2 = 1;
    }
    if ((*((int8_t *)task_1D0 + 10) == Script_PlayerStatsUtils(PS_GetStepSlot, 10)) | (*((int8_t *)task_1D0 + 11) == Script_PlayerStatsUtils(PS_GetStepSlot, 11)) | (*((int8_t *)task_1D0 + 12) == Script_PlayerStatsUtils(PS_GetStepSlot, 12)) | (*((int8_t *)task_1D0 + 13) == Script_PlayerStatsUtils(PS_GetStepSlot, 13))) {
      local_EA = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f);
      local_E9 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f);
      local_E8 = *((int8_t *)task_1D0 + (local_EA + 10));
      local_E7 = *((int8_t *)task_1D0 + (local_E9 + 10));
      *((int8_t *)task_1D0 + (local_EA + 10)) = local_E7;
      *((int8_t *)task_1D0 + (local_E9 + 10)) = local_E8;
    } else {
      local_F1 = 1;
    }
    local_EA = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
    local_E9 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
    local_E8 = *((int8_t *)task_1D0 + (local_EA + 14));
    local_E7 = *((int8_t *)task_1D0 + (local_E9 + 14));
    *((int8_t *)task_1D0 + (local_EA + 14)) = local_E7;
    *((int8_t *)task_1D0 + (local_E9 + 14)) = local_E8;
    if (-(local_F3 == 1) & -(local_F2 == 1) & -(local_F1 == 1)) {
      break;
    }
  }
  local_F3 = 0;
  local_E6 = 1;
  local_E8 = *(int8_t *)task_1D0;
  while (local_F3 < 16) {
    local_E7 = *((int8_t *)task_1D0 + local_F3);
    if (local_E8 < local_E7) {
      local_E8 = local_E7;
      local_E6 = local_F3 + 1;
    }
    local_F3 = local_F3 + 1;
  }
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 11, local_E6);
  Script_PlayerStatsUtils(PS_SetStepSlot, 0, *(int8_t *)task_1D0);
  Script_PlayerStatsUtils(PS_SetStepSlot, 1, *((int8_t *)task_1D0 + 1));
  Script_PlayerStatsUtils(PS_SetStepSlot, 2, *((int8_t *)task_1D0 + 2));
  Script_PlayerStatsUtils(PS_SetStepSlot, 3, *((int8_t *)task_1D0 + 3));
  Script_PlayerStatsUtils(PS_SetStepSlot, 4, *((int8_t *)task_1D0 + 4));
  Script_PlayerStatsUtils(PS_SetStepSlot, 5, *((int8_t *)task_1D0 + 5));
  Script_PlayerStatsUtils(PS_SetStepSlot, 6, *((int8_t *)task_1D0 + 6));
  Script_PlayerStatsUtils(PS_SetStepSlot, 7, *((int8_t *)task_1D0 + 7));
  Script_PlayerStatsUtils(PS_SetStepSlot, 8, *((int8_t *)task_1D0 + 8));
  Script_PlayerStatsUtils(PS_SetStepSlot, 9, *((int8_t *)task_1D0 + 9));
  Script_PlayerStatsUtils(PS_SetStepSlot, 10, *((int8_t *)task_1D0 + 10));
  Script_PlayerStatsUtils(PS_SetStepSlot, 11, *((int8_t *)task_1D0 + 11));
  Script_PlayerStatsUtils(PS_SetStepSlot, 12, *((int8_t *)task_1D0 + 12));
  Script_PlayerStatsUtils(PS_SetStepSlot, 13, *((int8_t *)task_1D0 + 13));
  Script_PlayerStatsUtils(PS_SetStepSlot, 14, *((int8_t *)task_1D0 + 14));
  Script_PlayerStatsUtils(PS_SetStepSlot, 15, *((int8_t *)task_1D0 + 15));
  SCNF_WaitFrames_0000(1);
  Script_sprintf(" : %d %d %d %d %d %d : %d %d %d %d : %d %d %d %d : %d %d \012", *(int8_t *)task_1D0, *((int8_t *)task_1D0 + 1), *((int8_t *)task_1D0 + 2), *((int8_t *)task_1D0 + 3), *((int8_t *)task_1D0 + 4), *((int8_t *)task_1D0 + 5), *((int8_t *)task_1D0 + 6), *((int8_t *)task_1D0 + 7), *((int8_t *)task_1D0 + 8), *((int8_t *)task_1D0 + 9), *((int8_t *)task_1D0 + 10), *((int8_t *)task_1D0 + 11), *((int8_t *)task_1D0 + 12), *((int8_t *)task_1D0 + 13), *((int8_t *)task_1D0 + 14), *((int8_t *)task_1D0 + 15));
  Script_sprintf("lacky : %d\012", local_E6);
  return;
}

static void script_B7EB(int32_t arg0) {
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 110);
  script_B83B();
  task_1E8 = Script_scenCreateTask(script_B8C1, 0);
  Script_shenSeqUtils(arg0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl((void *)task_1E8);
  return;
}

static void script_B83B(void) {
  Script_LightUtils(4, 4, 0.0f);
  Script_LightUtils(5, 4, 0.0f);
  Script_LightUtils(6, 4, 0.0f);
  Script_LightUtils(7, 4, 0.0f);
  Script_LightUtils(8, 4, 0.0f);
  Script_LightUtils(9, 4, 0.0f);
  Script_LightUtils(10, 4, 0.0f);
  Script_LightUtils(11, 4, 0.0f);
  Script_mapmSetRenderFlag(7, 0);
  task_1E4 = 0;
  task_1EC = 0.0f;
  task_1F0 = 0.0f;
  task_1F4 = 0.0f;
  task_1F8 = 0.0f;
  task_1FC = 0.0f;
  return;
}

static int32_t script_B8C1(void) {
  int32_t local_FC;
  for (;;) {
    local_FC = task_1E4;
    switch (local_FC) {
      case 60:
        Script_mapmSetRenderFlag(7, 1);
        break;
      case 80:
        Script_mapmSetRenderFlag(7, 0);
        break;
      case 90:
        Script_mapmSetRenderFlag(7, 1);
        break;
      case 150:
        Script_mapmSetRenderFlag(7, 0);
        break;
      case 180:
        Script_mapmSetRenderFlag(7, 1);
        break;
    }
    if ((task_1E4 > 20) & (task_1E4 < 80)) {
      task_1EC = task_1EC + 0.0166666675f;
    }
    if ((task_1E4 > 10) & (task_1E4 < 70)) {
      task_1F0 = task_1F0 + 0.0166666675f;
    }
    if (task_1E4 == 80) {
      task_1F4 = 0.1f;
    } else {
      if ((task_1E4 > 80) & (task_1E4 < 110)) {
        task_1F4 = task_1F4 + 0.043333333f;
        goto L00B9AC;
      }
    }
    L00B9AC:;
    if (task_1E4 == 80) {
      task_1F8 = 0.1f;
    } else {
      if ((task_1E4 > 80) & (task_1E4 < 120)) {
        task_1F8 = task_1F8 + 0.0375f;
        goto L00B9E5;
      }
    }
    L00B9E5:;
    if (task_1E4 == 150) {
      task_1FC = 0.1f;
    } else {
      if ((task_1E4 > 150) & (task_1E4 < 210)) {
        task_1FC = task_1FC + 0.0316666663f;
        goto L00BA21;
      }
    }
    L00BA21:;
    Script_LightUtils(10, 4, task_1EC);
    Script_LightUtils(5, 4, task_1F0);
    Script_LightUtils(6, 4, task_1F0);
    Script_LightUtils(7, 4, task_1F0);
    Script_LightUtils(9, 4, task_1F4);
    Script_LightUtils(4, 4, task_1F8);
    Script_LightUtils(11, 4, task_1FC);
    task_1E4 = task_1E4 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_BA85(void) {
  Script_ambSoundUtils(1, 1, &data_1600C, 135.0f, 280.0f, 1.0f, 0, &data_16054);
  Script_ambSoundUtils(2, 1, &data_16018, 100.0f, 200.0f, 1.0f, 0, &data_160DC);
  Script_ambSoundUtils(3, 1, data_16024, 170.0f, 100.0f, 1.0f, 2, data_16164);
  Script_ambSoundUtils(4, 1, data_16030, 170.0f, 100.0f, 1.0f, 0, &data_161EC);
  Script_ambSoundUtils(5, 0, &data_1603C, 50.0f, 170.0f, 0.8f, 0, data_16274);
  Script_ambSoundUtils(15, 1, data_16048, 300.0f, 0.0f, 1.0f, 3, &data_162FC);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static int32_t script_BB67(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  local_F8 = arg0;
  switch (local_F8) {
    case 0x4142454B:
      local_FC = 1;
      break;
    case 0x4242454B:
      local_FC = 2;
      break;
    case 0x4342454B:
      local_FC = 3;
      break;
    case 0x4442454B:
      local_FC = 4;
      break;
    case 0x4542454B:
      local_FC = 5;
      break;
  }
  return local_FC;
}

static void script_BBE2(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t unused_F8;
  if (task_1B4 == 0) {
    local_FC = (arg0 - 1) * 17;
    local_FC = local_FC + arg1 * 3 + Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, ((scn3_bits){.i = Script_ScenDataUtil(&data_16840, SCND_ReadFloatBits, arg1)}).f));
    if ((local_FC >= 0) & (local_FC < 85)) {
      task_1B4 = Script_ShowUIMessageFromID((const char *)Script_ScenDataUtil(&data_166EC, SCND_ReadScriptOffset, local_FC));
      Script_sprintf("MKSG :chara_no %d : voice_no = %d\012", arg0, local_FC);
    }
  }
  return;
}

static int32_t script_BC5A(void) {
  for (;;) {
    if (task_1B4) {
      if (Script_convGetIsActive((const void *)task_1B4) == 0) {
        task_1B4 = 0;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_16877, 1, data_1687C, 2, data_16881, 3, data_16886, 4, data_16891)))
static void script_BC86(void) {
  Script_WeatherTaskUtil(AndNotInstructionFlags, 0x784);
  script_E373();
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MKSG_HELP_ROUTE_ACTIVE, 1);
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 20, 0);
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 21, 0);
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 22, 0);
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 23, 0);
  Script_LoadSND(0, 5, "FREE", 0);
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 3, "FREE", 0);
  Script_LoadSND(0, 2, "ykm005.snd", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 127);
  Script_LoadSND(0, 4, "f1kyusok.snd", 0);
  Script_LoadSND(0, 5, "e1keibin.snd", 0);
  Script_SetMapControlTaskFlag(SHEN_NAME('MAGZ'), 1);
  Script_SetMapControlTaskFlag(SHEN_NAME('KAIS'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('ISU_'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('PEN_'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('CLIP'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('TEL_'), 0);
  return;
}

static void script_BD87(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  int16_t local_D6;
  int8_t local_D5;
  uint8_t unused_D4;
  Vector3i local_C8;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  script_119B5();
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_DC, 0x2000000);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_DC, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  if (Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 23) == 1) {
    local_D8 = 0x3250414D;
  } else {
    local_D8 = 0x3150414D;
  }
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_E8, 0);
  (&local_E8)->y = 1.675f;
  Script_EVT_SetPosition(local_D8, POS_XYZ, &local_E8);
  Script_EVT_SetRotation(local_D8, 0x38000000, &local_DC, 0);
  Script_SetGameStateTo(AUTH_EventCam);
  Script_seqEnterEvtCam(500, local_D8, local_D8);
  Script_SetCameraFlags(0);
  SCNF_WaitFrames_0000(1);
  Script_mtwkGetSetRotation(local_D8, &local_DC, 0);
  local_DC.x = 0x4000;
  Script_EVT_SetRotation(local_D8, 0x38000000, &local_DC, 0);
  SCNF_WaitFrames_0000(1);
  Script_SetCycleManagerData(31, (const uint32_t *)&local_E8, (const uint32_t *)&local_DC, 1, 0x31303030);
  Script_MoveObjectToObject(local_D8, 31);
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_C8, 0);
  (&local_C8)->y = -(&local_C8)->y;
  Script_EVT_SetRotation(local_D8, 0x38000000, &local_C8, 0);
  Script_MoveObjectToObject(local_D8, -1);
  SCNF_WaitFrames_0000(1);
  local_D6 = (Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 21) << 8) + Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 22);
  if (Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 23) == 2) {
    local_D6 = local_D6 | 0x80;
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 23) == 4) {
    local_D6 = local_D6 | 0x80;
  }
  local_D6 = local_D6 | 0x20;
  local_D5 = 0;
  while (local_D5 < 10) {
    if ((local_D6 & (1 << local_D5)) == 0) {
      Script_charaPosRotUtils(local_D8, *(int16_t *)((int8_t *)&data_168AC + (local_D5 << 1)), &data_168CC, 0, CHARA_PART_Set);
    }
    local_D5 = local_D5 + 1;
  }
  if (0) {
    Script_charaDisplay(local_D8, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(local_D8, CDM_Show);
      goto L00BF9B;
    }
  }
  L00BF9B:;
  Script_scenToggleVisibleFor(local_D8, 1);
  Script_ccowSetHeightMode(local_D8, CCOW_KeepHeight);
  Script_ccowUtils(local_D8, CCOW_DisableAll);
  while (1) {
    Script_ReturnTimeDate(&local_F4);
    if ((Script_GetControllerInput(1) == 4) | (Script_GetControllerInput(1) == 0x200) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE) == 1) | (task_1CA == -1) | (task_1CD == -1)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCameraFlags(-1);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  if (-1) {
    Script_charaDisplay(local_D8, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(local_D8, CDM_Show);
      goto L00C03A;
    }
  }
  L00C03A:;
  Script_scenToggleVisibleFor(local_D8, 0);
  local_D5 = 0;
  while (local_D5 < 10) {
    Script_charaPosRotUtils(local_D8, *(int16_t *)((int8_t *)&data_168AC + (local_D5 << 1)), &data_168C0, 0, CHARA_PART_Set);
    local_D5 = local_D5 + 1;
  }
  if (task_1CD != -1) {
    script_11A0D();
  }
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_ryoControl(RYO_RestoreControl);
  SCNF_WaitFrames_0000(1);
  return;
}

static int32_t script_C0A3(int32_t arg0) {
  int32_t local_FC;
  int8_t local_FB;
  local_FC = 0;
  local_FB = data_16BBC[arg0];
  while (1) {
    if (*(&data_16B14 + local_FB) == local_FC) {
      Script_ShowUIMessageFromID((const char *)Script_ScenDataUtil(&data_16A6C, SCND_ReadScriptOffset, local_FB));
      local_FB = local_FB + 1;
      if (*(&data_16B14 + local_FB) == -1) {
        break;
      }
    }
    if ((arg0 == 1) & (local_FC == 400)) {
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_COFFEE_SPECIAL_VARIANT)) {
        Script_ShowUIMessageFromID("E1042A002");
      } else {
        Script_ShowUIMessageFromID("E1042A001");
      }
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_C149(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_E4 = 0;
  while (1) {
    local_E0 = local_E4;
    switch (local_E0) {
      case 0:
        Script_PlayCharaMotion(CHARA_AKIR, 4420, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
        break;
      case 131:
        Script_PlayCharaMotion(CHARA_SYZU, 4425, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_motmSetBlendFrames(CHARA_SYZU, MOTM_BLEND_FRAMES_20);
        Script_EVT_GetPosition(CHARA_SYZU, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
        Script_SetCharaLookAtPosition(CHARA_AKIR, 6, &local_F4, 0);
        break;
      case 670:
        Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.3f);
        Script_SetCharaMotionPlaybackRate(CHARA_SYZU, 1.3f);
        break;
      case 770:
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        break;
      case 800:
        script_D447();
        local_F4.x = 17.5f;
        (&local_F4)->y = 0.0f;
        (&local_F4)->z = -9.6f;
        Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
        {
          Vector3i local_D4;
          (&local_D4)->y = 0;
          Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_D4, 0);
        }
        local_F4.x = 17.568001f;
        (&local_F4)->y = 0.0f;
        (&local_F4)->z = -10.87f;
        Script_EVT_SetPosition(CHARA_SYZU, POS_XYZ, &local_F4);
        {
          Vector3i local_D4;
          (&local_D4)->y = 0x8000;
          Script_EVT_SetRotation(CHARA_SYZU, MATRIX_XFORM_Y, &local_D4, 0);
        }
        Script_motmSetBlendFrames(CHARA_SYZU, MOTM_BLEND_FRAMES_5);
        Script_PlayCharaMotion(CHARA_AKIR, 4421, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_PlayCharaMotion(CHARA_SYZU, 4426, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 0.0f);
        Script_SetCharaMotionPlaybackRate(CHARA_SYZU, 0.0f);
        Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
        break;
      case 1250:
        Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
        Script_SetCharaMotionPlaybackRate(CHARA_SYZU, 1.0f);
        break;
      case 1320:
        if (0) {
          Script_charaDisplay(SHEN_NAME('MAP1'), CDM_Hide);
        } else {
          if (-1) {
            Script_charaDisplay(SHEN_NAME('MAP1'), CDM_Show);
            goto L00C3CA;
          }
        }
        L00C3CA:;
        Script_scenToggleVisibleFor(SHEN_NAME('MAP1'), 1);
        local_F4.x = 0.124f;
        (&local_F4)->y = 0.03f;
        (&local_F4)->z = -0.048f;
        local_E8.x = -16019;
        (&local_E8)->y = -36135;
        (&local_E8)->z = 29491;
        Script_fixoAttachEx(SHEN_NAME('MAP1'), CHARA_SYZU, 18, &local_F4, &local_E8);
        Script_CharHandlers(CHARA_SYZU, 0, 10, 16);
        break;
      case 1819:
        Script_fixoClearAttachment(SHEN_NAME('MAP1'));
        break;
      case 1820:
        local_F4.x = 0.18f;
        (&local_F4)->y = 0.06f;
        (&local_F4)->z = -0.02f;
        local_E8.x = -19478;
        (&local_E8)->y = -1456;
        (&local_E8)->z = 33314;
        Script_fixoAttachEx(SHEN_NAME('MAP1'), CHARA_AKIR, 18, &local_F4, &local_E8);
        Script_CharHandlers(CHARA_AKIR, 0, 10, 16);
        break;
      case 2200:
        Script_SetCharaLookAtPosition(CHARA_AKIR, -6, 0, 0);
        break;
      case 2250:
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_SetCharaLookAtPosition(CHARA_AKIR, -6, 0, 0);
        Script_fixoClearAttachment(SHEN_NAME('MAP1'));
        if (-1) {
          Script_charaDisplay(SHEN_NAME('MAP1'), CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(SHEN_NAME('MAP1'), CDM_Show);
            goto L00C553;
          }
        }
        L00C553:;
        Script_scenToggleVisibleFor(SHEN_NAME('MAP1'), 0);
        goto L00C612;
        break;
    }
    if ((local_E4 > 800) & (local_E4 <= 1780)) {
      Script_EVT_GetPosition(CHARA_SYZU, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(CHARA_AKIR, 16, &local_F4, 0);
    }
    if ((local_E4 > 1800) & (local_E4 <= 1920)) {
      Script_EVT_GetPosition(CHARA_AKIR, 18, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(CHARA_AKIR, 16, &local_F4, 0);
    }
    if ((local_E4 > 1920) & (local_E4 <= 2200)) {
      Script_EVT_GetPosition(CHARA_SYZU, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(CHARA_AKIR, 16, &local_F4, 0);
    }
    local_E4 = local_E4 + 1;
    SCNF_WaitFrames_0000(1);
  }
  L00C612:;
}

static int32_t script_C613(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_E4 = 0;
  while (1) {
    local_E0 = local_E4;
    switch (local_E0) {
      case 0:
        script_D447();
        Script_PlayCharaMotion(CHARA_AKIR, 4424, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_PlayCharaMotion(CHARA_SYZU, 4428, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 0.0f);
        Script_SetCharaMotionPlaybackRate(CHARA_SYZU, 0.0f);
        Script_EVT_GetPosition(CHARA_SYZU, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
        Script_SetCharaLookAtPosition(CHARA_AKIR, 6, &local_F4, 0);
        Script_motiSetAngFlags(CHARA_SYZU, 1);
        Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
        break;
      case 140:
        Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
        Script_SetCharaMotionPlaybackRate(CHARA_SYZU, 1.0f);
        break;
      case 440:
        if (0) {
          Script_charaDisplay(SHEN_NAME('MAP1'), CDM_Hide);
        } else {
          if (-1) {
            Script_charaDisplay(SHEN_NAME('MAP1'), CDM_Show);
            goto L00C731;
          }
        }
        L00C731:;
        Script_scenToggleVisibleFor(SHEN_NAME('MAP1'), 1);
        local_F4.x = 0.18f;
        (&local_F4)->y = 0.06f;
        (&local_F4)->z = -0.02f;
        local_E8.x = -19478;
        (&local_E8)->y = -1456;
        (&local_E8)->z = 33314;
        Script_fixoAttachEx(SHEN_NAME('MAP1'), CHARA_AKIR, 18, &local_F4, &local_E8);
        Script_CharHandlers(CHARA_AKIR, 0, 10, 16);
        break;
      case 549:
        Script_fixoClearAttachment(SHEN_NAME('MAP1'));
        break;
      case 550:
        local_F4.x = 0.13f;
        (&local_F4)->y = 0.03f;
        (&local_F4)->z = 0.063f;
        local_E8.x = 48605;
        (&local_E8)->y = 37683;
        (&local_E8)->z = 0x8000;
        Script_fixoAttachEx(SHEN_NAME('MAP1'), CHARA_SYZU, 12, &local_F4, &local_E8);
        Script_CharHandlers(CHARA_SYZU, 1, 10, 16);
        break;
      case 984:
        Script_fixoClearAttachment(SHEN_NAME('MAP1'));
        break;
      case 985:
        if (0) {
          Script_charaDisplay(SHEN_NAME('MAP1'), CDM_Hide);
        } else {
          if (-1) {
            Script_charaDisplay(SHEN_NAME('MAP1'), CDM_Show);
            goto L00C875;
          }
        }
        L00C875:;
        Script_scenToggleVisibleFor(SHEN_NAME('MAP1'), 1);
        local_F4.x = 0.18f;
        (&local_F4)->y = 0.06f;
        (&local_F4)->z = -0.02f;
        local_E8.x = -19478;
        (&local_E8)->y = -1456;
        (&local_E8)->z = 33314;
        Script_fixoAttachEx(SHEN_NAME('MAP1'), CHARA_AKIR, 18, &local_F4, &local_E8);
        Script_CharHandlers(CHARA_AKIR, 0, 10, 16);
        break;
      case 1160:
        Script_fixoClearAttachment(SHEN_NAME('MAP1'));
        if (-1) {
          Script_charaDisplay(SHEN_NAME('MAP1'), CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(SHEN_NAME('MAP1'), CDM_Show);
            goto L00C922;
          }
        }
        L00C922:;
        Script_scenToggleVisibleFor(SHEN_NAME('MAP1'), 0);
        break;
      case 1390:
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_SetCharaLookAtPosition(CHARA_AKIR, -6, 0, 0);
        goto L00CA65;
        break;
    }
    if ((local_E4 > 850) & (local_E4 <= 865)) {
      Script_EVT_GetPosition(CHARA_SYZU, 17, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      local_F4.x = (&local_F4)->x + 0.01f;
      (&local_F4)->y = (&local_F4)->y - 0.005f;
      Script_SetMotnFlags(CHARA_SYZU, 17, &local_F4, 0x78000000);
    } else if ((local_E4 > 865) & (local_E4 <= 885)) {
      Script_SetMotnFlags(CHARA_SYZU, 17, &local_F4, 0x78000000);
    } else {
      if ((local_E4 > 885) & (local_E4 <= 895)) {
        Script_EVT_GetPosition(CHARA_SYZU, 17, &local_F4, ENTITY_XFORM_WORLD_SPACE);
        local_F4.x = (&local_F4)->x - 0.01f;
        (&local_F4)->y = (&local_F4)->y + 0.005f;
        Script_SetMotnFlags(CHARA_SYZU, 17, &local_F4, 0x78000000);
        goto L00CA51;
      }
    }
    L00CA51:;
    local_E4 = local_E4 + 1;
    SCNF_WaitFrames_0000(1);
  }
  L00CA65:;
}

static int32_t script_CA66(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_E4 = 0;
  while (1) {
    local_E0 = local_E4;
    switch (local_E0) {
      case 0:
        script_D447();
        Script_PlayCharaMotion(CHARA_AKIR, 4424, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_PlayCharaMotion(CHARA_SYZU, 4428, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 0.0f);
        Script_SetCharaMotionPlaybackRate(CHARA_SYZU, 0.0f);
        Script_EVT_GetPosition(CHARA_SYZU, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
        Script_SetCharaLookAtPosition(CHARA_AKIR, 6, &local_F4, 0);
        Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
        break;
      case 90:
        Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
        Script_SetCharaMotionPlaybackRate(CHARA_SYZU, 1.0f);
        break;
      case 390:
        if (0) {
          Script_charaDisplay(SHEN_NAME('MAP1'), CDM_Hide);
        } else {
          if (-1) {
            Script_charaDisplay(SHEN_NAME('MAP1'), CDM_Show);
            goto L00CB77;
          }
        }
        L00CB77:;
        Script_scenToggleVisibleFor(SHEN_NAME('MAP1'), 1);
        local_F4.x = 0.18f;
        (&local_F4)->y = 0.06f;
        (&local_F4)->z = -0.02f;
        local_E8.x = -19478;
        (&local_E8)->y = -1456;
        (&local_E8)->z = 33314;
        Script_fixoAttachEx(SHEN_NAME('MAP1'), CHARA_AKIR, 18, &local_F4, &local_E8);
        Script_CharHandlers(CHARA_AKIR, 0, 10, 16);
        break;
      case 499:
        Script_fixoClearAttachment(SHEN_NAME('MAP1'));
        break;
      case 500:
        local_F4.x = 0.13f;
        (&local_F4)->y = 0.03f;
        (&local_F4)->z = 0.063f;
        local_E8.x = 48605;
        (&local_E8)->y = 37683;
        (&local_E8)->z = 0x8000;
        Script_fixoAttachEx(SHEN_NAME('MAP1'), CHARA_SYZU, 12, &local_F4, &local_E8);
        Script_CharHandlers(CHARA_SYZU, 1, 10, 16);
        Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
        Script_motmToggleFlags(CHARA_SYZU, SCEN_Disable, 0x40);
        Script_PlayCharaMotion(CHARA_AKIR, 4424, -1.0f, -1.0f, 690.0f, 0.7f, 0);
        Script_PlayCharaMotion(CHARA_SYZU, 4428, -1.0f, -1.0f, 690.0f, 0.7f, 0);
        Script_motiSetAngFlags(CHARA_SYZU, 1);
        break;
      case 683:
        Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
        Script_SetCharaMotionPlaybackRate(CHARA_SYZU, 1.0f);
        break;
      case 709:
        Script_fixoClearAttachment(SHEN_NAME('MAP1'));
        break;
      case 710:
        if (0) {
          Script_charaDisplay(SHEN_NAME('MAP1'), CDM_Hide);
        } else {
          if (-1) {
            Script_charaDisplay(SHEN_NAME('MAP1'), CDM_Show);
            goto L00CD4E;
          }
        }
        L00CD4E:;
        Script_scenToggleVisibleFor(SHEN_NAME('MAP1'), 1);
        local_F4.x = 0.18f;
        (&local_F4)->y = 0.06f;
        (&local_F4)->z = -0.02f;
        local_E8.x = -19478;
        (&local_E8)->y = -1456;
        (&local_E8)->z = 33314;
        Script_fixoAttachEx(SHEN_NAME('MAP1'), CHARA_AKIR, 18, &local_F4, &local_E8);
        Script_CharHandlers(CHARA_AKIR, 0, 10, 16);
        break;
      case 781:
        Script_EVT_GetPosition(CHARA_SYZU, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
        Script_SetLookPointDetails(CHARA_AKIR, 16, &local_F4, 0);
        break;
      case 885:
        Script_fixoClearAttachment(SHEN_NAME('MAP1'));
        if (-1) {
          Script_charaDisplay(SHEN_NAME('MAP1'), CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(SHEN_NAME('MAP1'), CDM_Show);
            goto L00CE2A;
          }
        }
        L00CE2A:;
        Script_scenToggleVisibleFor(SHEN_NAME('MAP1'), 0);
        break;
      case 995:
        Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
        Script_motmToggleFlags(CHARA_SYZU, SCEN_Enable, 0x40);
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_SetCharaLookAtPosition(CHARA_AKIR, -6, 0, 0);
        goto L00CFBA;
        break;
    }
    if ((local_E4 > 720) & (local_E4 <= 780)) {
      Script_EVT_GetPosition(CHARA_AKIR, 17, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(CHARA_AKIR, 16, &local_F4, 0);
    }
    if ((local_E4 > 500) & (local_E4 <= 515)) {
      Script_EVT_GetPosition(CHARA_SYZU, 17, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      local_F4.x = (&local_F4)->x + 0.01f;
      (&local_F4)->y = (&local_F4)->y - 0.005f;
      Script_SetMotnFlags(CHARA_SYZU, 17, &local_F4, 0x78000000);
    } else if ((local_E4 > 515) & (local_E4 <= 600)) {
      Script_SetMotnFlags(CHARA_SYZU, 17, &local_F4, 0x78000000);
    } else {
      if ((local_E4 > 600) & (local_E4 <= 610)) {
        Script_EVT_GetPosition(CHARA_SYZU, 17, &local_F4, ENTITY_XFORM_WORLD_SPACE);
        local_F4.x = (&local_F4)->x - 0.01f;
        (&local_F4)->y = (&local_F4)->y + 0.01f;
        Script_SetMotnFlags(CHARA_SYZU, 17, &local_F4, 0x78000000);
        goto L00CFA6;
      }
    }
    L00CFA6:;
    local_E4 = local_E4 + 1;
    SCNF_WaitFrames_0000(1);
  }
  L00CFBA:;
}

static int32_t script_CFBB(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  Vector3 local_E8;
  int32_t unused_DC[3];
  int32_t local_D8;
  int32_t local_D4;
  local_D8 = 0;
  while (1) {
    local_D4 = local_D8;
    if (local_D4 == 0) {
      script_D447();
      Script_PlayCharaMotion(CHARA_AKIR, 4424, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_PlayCharaMotion(CHARA_SYZU, 4428, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 0.0f);
      Script_SetCharaMotionPlaybackRate(CHARA_SYZU, 0.0f);
      Script_EVT_GetPosition(CHARA_SYZU, 5, &local_E8, ENTITY_XFORM_WORLD_SPACE);
      Script_SetCharaLookAtPosition(CHARA_AKIR, 6, &local_E8, 0);
      Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
    } else if (local_D4 == 1350) {
      Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
    } else if (local_D4 == 1380) {
      Script_ReturnTimeDate(&local_F4);
      (&local_F4)->hours = 23;
      (&local_F4)->minutes = 0;
      Script_SetNewDateTime(&local_F4);
      Script_cdttLoadScrUtils(CDTT_CMD_Clear, 0);
    } else if (local_D4 == 1500) {
      Script_cdttLoadScrUtils(CDTT_CMD_Init, 0);
    } else {
      if (local_D4 == 1500) {
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_SetCharaLookAtPosition(CHARA_AKIR, -6, 0, 0);
        break;
        goto L00D12A;
      }
    }
    L00D12A:;
    local_D8 = local_D8 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_D13F(void) {
  int32_t local_FC;
  int32_t local_F8;
  Script_SetGameStateTo(AUTH_EventCam);
  local_FC = 0;
  while (1) {
    local_F8 = local_FC;
    switch (local_F8) {
      case 0:
        Script_seqEnterEvtCam(4000, 0, AUTH_None);
        break;
      case 400:
        Script_seqEnterEvtCam(4001, 0, AUTH_None);
        break;
      case 500:
        Script_seqEnterEvtCam(4002, 0, AUTH_None);
        break;
      case 800:
        Script_seqEnterEvtCam(4003, 0, AUTH_None);
        break;
      case 1250:
        Script_seqEnterEvtCam(4004, 0, AUTH_None);
        break;
      case 1320:
        Script_seqEnterEvtCam(4005, 0, AUTH_None);
        break;
      case 1620:
        Script_seqEnterEvtCam(4006, 0, AUTH_None);
        break;
      case 1730:
        Script_seqEnterEvtCam(4007, 0, AUTH_None);
        break;
      case 1995:
        Script_seqEnterEvtCam(4008, 0, AUTH_None);
        break;
      case 2250:
        goto L00D254;
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  L00D254:;
}

static int32_t script_D255(void) {
  int32_t local_FC;
  int32_t local_F8;
  Script_SetGameStateTo(AUTH_EventCam);
  local_FC = 0;
  while (1) {
    local_F8 = local_FC;
    switch (local_F8) {
      case 0:
        Script_seqEnterEvtCam(4010, 0, AUTH_None);
        break;
      case 222:
        Script_seqEnterEvtCam(4011, 0, AUTH_None);
        break;
      case 550:
        Script_seqEnterEvtCam(4012, 0, AUTH_None);
        break;
      case 985:
        Script_seqEnterEvtCam(4013, 0, AUTH_None);
        break;
      case 1195:
        Script_seqEnterEvtCam(4014, 0, AUTH_None);
        break;
      case 1390:
        goto L00D30A;
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  L00D30A:;
}

static int32_t script_D30B(void) {
  int32_t local_FC;
  int32_t local_F8;
  Script_SetGameStateTo(AUTH_EventCam);
  local_FC = 0;
  while (1) {
    local_F8 = local_FC;
    switch (local_F8) {
      case 0:
        Script_seqEnterEvtCam(4020, 0, AUTH_None);
        break;
      case 221:
        Script_seqEnterEvtCam(4021, 0, AUTH_None);
        break;
      case 500:
        Script_seqEnterEvtCam(4022, 0, AUTH_None);
        break;
      case 710:
        Script_seqEnterEvtCam(4023, 0, AUTH_None);
        break;
      case 995:
        goto L00D3A8;
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  L00D3A8:;
}

static int32_t script_D3A9(void) {
  int32_t local_FC;
  int32_t local_F8;
  Script_SetGameStateTo(AUTH_EventCam);
  local_FC = 0;
  while (1) {
    local_F8 = local_FC;
    switch (local_F8) {
      case 0:
        Script_seqEnterEvtCam(4030, 0, AUTH_None);
        break;
      case 223:
        Script_seqEnterEvtCam(4031, 0, AUTH_None);
        break;
      case 684:
        Script_seqEnterEvtCam(4032, 0, AUTH_None);
        break;
      case 919:
        Script_seqEnterEvtCam(4033, 0, AUTH_None);
        break;
      case 1500:
        goto L00D446;
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  L00D446:;
}

static void script_D447(void) {
  Script_lightResetSlotToDefaults(21);
  Script_lightEnablePointLightAt(21, &data_16BD8);
  Script_LightUtils(21, 2, &data_16BE4);
  Script_LightUtils(21, 4, 0.4f);
  Script_LightUtils(21, 5, 3.0f);
  Script_LightUtils(21, 6, data_16BF4);
  Script_lightSetSlotEnabled(21, 1);
  return;
}

__attribute__((scn_string_bindings(0, data_16C24, 1, data_16C2A)))
static void script_D497(void) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int8_t local_EB;
  local_F0 = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 20) - 1;
  if (local_F0 < 12) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_COFFEE_SPECIAL_VARIANT) == 0) {
      local_F0 = local_F0 + 12;
    }
    local_EB = data_16BFC[local_F0];
  } else {
    local_EB = 0;
  }
  if (local_EB == 2) {
    if (((Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 21) << 8) + Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 22)) & 0x80) {
      Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 23, local_EB);
      local_EB = 0;
    }
  }
  if (local_EB) {
    Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
    Script_shenClearFreeState(0);
    Script_LoadVoiceFile(1, "E1042");
    if (0) {
      Script_charaDisplay(CHARA_SYZU, CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(CHARA_SYZU, CDM_Show);
        goto L00D569;
      }
    }
    L00D569:;
    Script_scenToggleVisibleFor(CHARA_SYZU, 1);
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAKO'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('HAKO'), CDM_Show);
        goto L00D59D;
      }
    }
    L00D59D:;
    Script_scenToggleVisibleFor(SHEN_NAME('HAKO'), 1);
    Script_ccowUtils(CHARA_SYZU, CCOW_DisableAll);
    Script_SetCPAMFlags(CHARA_SYZU, 0);
    Script_charaDisplay(CHARA_SYZU, CDM_Show);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_charaToggleMotEnabled(CHARA_AKIR, 1);
    Script_charaToggleMotEnabled(CHARA_SYZU, 1);
    Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
    Script_charaSetRefboardFlagTo(CHARA_SYZU, 1);
    Script_SetCharaRefBoardPos(CHARA_SYZU, &data_16C14);
    Script_SetCameraFlags(0);
    Script_PREP_Related(-1, 1);
    Script_convSetCamInactiveFlag(1);
    Script_SetGameStateTo(AUTH_None);
    if (local_EB == 1) {
      local_F4.x = 12.0f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -2.4f;
      Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
      {
        uint8_t unused_E8[3];
        Vector3i local_DC;
        (&local_DC)->y = 0x4000;
        Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_DC, 0);
      }
      local_F4.x = 12.37185f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -2.4f;
      Script_EVT_SetPosition(CHARA_SYZU, POS_XYZ, &local_F4);
      {
        uint8_t unused_E8[3];
        Vector3i local_DC;
        (&local_DC)->y = 0x4000;
        Script_EVT_SetRotation(CHARA_SYZU, MATRIX_XFORM_Y, &local_DC, 0);
      }
    } else {
      local_F4.x = 17.5f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -9.6f;
      Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
      {
        uint8_t unused_E8[3];
        Vector3i local_DC;
        (&local_DC)->y = 0;
        Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_DC, 0);
      }
      local_F4.x = 17.568001f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -10.87f;
      Script_EVT_SetPosition(CHARA_SYZU, POS_XYZ, &local_F4);
      {
        uint8_t unused_E8[3];
        Vector3i local_DC;
        (&local_DC)->y = 0x8000;
        Script_EVT_SetRotation(CHARA_SYZU, MATRIX_XFORM_Y, &local_DC, 0);
      }
    }
    Script_scenCreateTask(script_C0A3, 1, local_EB);
    local_EC = 1;
    {
      uint8_t unused_E8[3];
      int32_t local_E4;
      local_E4 = local_EB;
      switch (local_E4) {
        case 1:
          Script_scenCreateTask(script_C149, 0);
          Script_scenCreateTask(script_D13F, 0);
          local_EC = 2250;
          break;
        case 2:
          Script_scenCreateTask(script_C613, 0);
          Script_scenCreateTask(script_D255, 0);
          local_EC = 1390;
          break;
        case 3:
          Script_scenCreateTask(script_CA66, 0);
          Script_scenCreateTask(script_D30B, 0);
          local_EC = 995;
          break;
        case 4:
          Script_scenCreateTask(script_CFBB, 0);
          Script_scenCreateTask(script_D3A9, 0);
          local_EC = 1500;
          break;
      }
    }
    local_F0 = 0;
    while (local_F0 < local_EC) {
      local_F0 = local_F0 + 1;
      SCNF_WaitFrames_0000(1);
    }
    Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 23, local_EB);
    SCNF_WaitFrames_0000(60);
    local_F4.x = -16.5f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = -15.0f;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
    {
      uint8_t unused_E4[7];
      Vector3i local_D8;
      (&local_D8)->y = 0;
      Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_D8, 0);
    }
    Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
    Script_CharHandlers(CHARA_SYZU, -1, 0, 0);
    if (-1) {
      Script_charaDisplay(CHARA_SYZU, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(CHARA_SYZU, CDM_Show);
        goto L00D8FF;
      }
    }
    L00D8FF:;
    Script_scenToggleVisibleFor(CHARA_SYZU, 0);
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAKO'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('HAKO'), CDM_Show);
        goto L00D933;
      }
    }
    L00D933:;
    Script_scenToggleVisibleFor(SHEN_NAME('HAKO'), 0);
    Script_LoadVoiceFile(0, "E1042");
    Script_uiUtils(0, 44, 0);
    Script_SetCameraFlags(1);
    Script_lightSetSlotEnabled(21, 0);
    Script_PREP_Related(-1, 0);
    Script_PREP_Related(0, 1);
    Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
    Script_shenClearFreeState(1);
  }
  SCNF_WaitFrames_0000(1);
  return;
}

static void script_D996(int32_t arg0) {
  Script_ScenDataUtil(&*(int32_t *)&task_200, SCND_WriteU8, arg0 / 8, Script_ScenDataUtil(&*(int32_t *)&task_200, SCND_ReadU8, arg0 / 8) | (1 << (arg0 % 8)));
  return;
}

static void script_D9C1(int32_t arg0) {
  Script_ScenDataUtil(&*(int32_t *)&task_200, SCND_WriteU8, arg0 / 8, Script_ScenDataUtil(&*(int32_t *)&task_200, SCND_ReadU8, arg0 / 8) & ((1 << (arg0 % 8)) ^ -1));
  return;
}

static int32_t script_D9EF(int32_t arg0) {
  return Script_ScenDataUtil(&*(int32_t *)&task_200, SCND_ReadU8, arg0 / 8) & (1 << (arg0 % 8));
}

static void script_DA0C(int32_t arg0) {
  while (script_D9EF(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_DA2B(int32_t arg0, float arg1) {
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= arg1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_DA47(int32_t arg0) {
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_DA63(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3i local_F4;
  int32_t local_F0;
  Script_mtwkGetSetRotation(arg0, &local_F4, 0);
  local_F0 = arg1 - (&local_F4)->y;
  if (local_F0 < 0) {
    local_F0 = -local_F0;
  }
  if (local_F0 >= 0x8000) {
    local_F0 = local_F0 - 0x10000;
    if (local_F0 < 0) {
      local_F0 = -local_F0;
    }
  }
  if (local_F0 <= arg2) {
    (&local_F4)->y = arg1;
    Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
    (void)1;
  } else {
    if (arg3 == 0) {
      (&local_F4)->y = (&local_F4)->y - arg2;
    } else {
      (&local_F4)->y = (&local_F4)->y + arg2;
    }
    Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
    (void)0;
  }
  end:;
}

static int32_t script_DB1E(void) {
  return Script_ShowUIMessageFromID("Q1003B001");
}

static int32_t script_DB2A(void) {
  return Script_ShowUIMessageFromID("Q1003B002");
}

static int32_t script_DB36(void) {
  return Script_ShowUIMessageFromID("Q1003B004");
}

static int32_t script_DB42(void) {
  return Script_ShowUIMessageFromID("Q1003A003");
}

static int32_t script_DB4E(void) {
  return Script_ShowUIMessageFromID("Q1003A004");
}

static int32_t script_DB5A(void) {
  return Script_ShowUIMessageFromID("Q1003A005");
}

static int32_t script_DB66(void) {
  return Script_ShowUIMessageFromID("Q1003A001");
}

static int32_t script_DB72(void) {
  return Script_ShowUIMessageFromID("Q1003A002");
}

static int32_t script_DB7E(int32_t arg0) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  Script_charaDisplay(arg0, CDM_Show);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_SetCharaFlags(arg0, 4);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 7);
  Script_scenToggleActiveFor(arg0, 0);
  Script_EVT_SetPosition(arg0, POS_XYZ, &data_16C80);
  Script_EVT_SetRotation(arg0, 0x38000000, &data_16C8C, 0);
  script_DA0C(3);
  Script_executeAicaCommand(132265, 0, 0);
  local_E4 = 2;
  local_E0 = 1;
  while (1) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    Script_InterpolateAndScaleAngles(0, local_E4, &local_F4, &local_E8, "KCDT");
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    local_F4.x = (&local_F4)->x * 10.0f + (&data_16C80)->x;
    (&local_F4)->y = (&local_F4)->y * 10.0f + (&data_16C80)->y;
    (&local_F4)->z = (&local_F4)->z * 10.0f + (&data_16C80)->z;
    Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
    Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
    local_E4 = local_E4 + local_E0;
    if ((local_E4 >= 40) & (script_D9EF(7) == 0)) {
      Script_scenToggleActiveFor(arg0, 1);
    } else {
      Script_scenToggleActiveFor(arg0, 0);
    }
    if (script_D9EF(7)) {
      break;
    }
    if (local_E4 >= 85) {
      Script_EVT_SetRotation(arg0, 0x38000000, &data_16CA0, 0);
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_DD02(int32_t arg0) {
  int32_t local_F4[3];
  int32_t local_E8[3];
  Script_charaDisplay(arg0, CDM_Show);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_scenToggleActiveFor(arg0, 0);
  Script_EVT_SetPosition(arg0, POS_XYZ, &data_16CAC);
  local_F4[1] = 0xC000;
  Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, (const Vector3i *)local_F4, 0);
  Script_PlayCharaMotion(arg0, 4394, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_DA0C(1);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_12);
  Script_PlayCharaMotion(arg0, 4394, 800.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  script_DA2B(arg0, 840.0f);
  Script_executeAicaCommand(394409, 0, 0);
  script_DA2B(arg0, 850.0f);
  Script_executeAicaCommand(0x1300A0, 4, 0);
  Script_executeAicaCommand(7849, 0, 0);
  Script_EVT_GetPosition(arg0, 12, (Vector3 *)local_F4, 0);
  local_E8[0] = 0;
  local_E8[1] = 0;
  local_E8[2] = 0;
  local_F4[0] = 0x3E90624E;
  local_F4[1] = 0x3E2E147B;
  local_F4[2] = -0x429A9FBE;
  local_E8[0] = 1388;
  local_E8[1] = 44968;
  local_E8[2] = 25164;
  script_DA0C(4);
  if (script_D9EF(5)) {
    Script_scenToggleActiveFor(arg0, 1);
  } else {
    script_DA0C(6);
    Script_EVT_SetPosition(arg0, POS_XYZ, &data_16CB8);
    local_F4[1] = 0xC000;
    Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, (const Vector3i *)local_F4, 0);
    Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
    Script_PlayCharaMotion(arg0, 4396, 70.0f, -1.0f, -1.0f, 1.0f, 0);
    script_DA0C(8);
    Script_EVT_SetPosition(arg0, POS_XYZ, &data_16CC4);
    SCNF_WaitFrames_0000(20);
  }
  end:;
}

static int32_t script_DEFF(int32_t arg0) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_D8[3];
  Script_charaDisplay(arg0, CDM_Show);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 1);
  Script_charaVisUtils(arg0, 0);
  Script_FACE_LCG(arg0, FACE_BlinkAutomatic, 10, 0);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_SetCharaFlags(arg0, 4);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 7);
  Script_MotStepUtil(arg0, 1);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_EVT_SetPosition(arg0, POS_XYZ, &data_16CD0);
  local_D8[1] = 0x4000;
  Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, (const Vector3i *)local_D8, 0);
  Script_PlayCharaMotion(arg0, 4353, 133.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 0.0f);
  script_DA0C(0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  SCNF_WaitFrames_0000(1);
  script_DA2B(arg0, 235.0f);
  Script_clmdSetFlags(arg0, 4, 1);
  SCNF_WaitFrames_0000(1);
  script_DA2B(arg0, 360.0f);
  Script_executeAicaCommand(66729, 0, 0);
  script_DA0C(4);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  if (script_D9EF(5)) {
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_8);
    Script_PlayCharaMotion(arg0, 4363, 66.0f, -1.0f, -1.0f, 1.0f, 0);
    script_DA0C(7);
    Script_executeAicaCommand(263337, 0, 0);
    Script_EVT_GetPosition(arg0, 18, (Vector3 *)local_D8, 0);
    {
      Vector3i local_CC;
      local_CC.x = 0;
      (&local_CC)->y = 0;
      (&local_CC)->z = 0;
      Script_EVT_SetPosition(SHEN_NAME('KAIS'), POS_XYZ, (const Vector3 *)local_D8);
      Script_EVT_SetRotation(SHEN_NAME('KAIS'), 0x38000000, &local_CC, 0);
    }
    local_D8[0] = 0x3E2E147B;
    local_D8[1] = 0x3E0F5C29;
    local_D8[2] = 0x3CF5C28F;
    {
      Vector3i local_CC;
      local_CC.x = 12854;
      (&local_CC)->y = 45332;
      (&local_CC)->z = 13152;
      Script_fixoAttachEx(SHEN_NAME('KAIS'), arg0, 18, (const Vector3 *)local_D8, &local_CC);
    }
    SCNF_WaitFrames_0000(1);
    script_DA2B(arg0, 92.0f);
    Script_executeAicaCommand(328873, 0, 0);
    script_DA2B(arg0, 140.0f);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_30);
    Script_PlayCharaMotion(arg0, 4378, 70.0f, -1.0f, -1.0f, 1.0f, 0);
    local_E4 = 0x4000;
    script_DA2B(arg0, 150.0f);
    script_DA47(arg0);
    Script_mtwkGetSetRotation(arg0, &local_E8, 0x2000000);
    Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_clmdSetFlags(arg0, 4, 0);
    while (1) {
      script_DA63(arg0, local_E4, 4551, 1);
      SCNF_WaitFrames_0000(1);
    }
  } else {
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_8);
    Script_PlayCharaMotion(arg0, 4382, 60.0f, -1.0f, -1.0f, 1.0f, 0);
    SCNF_WaitFrames_0000(1);
    script_DA2B(arg0, 75.0f);
    Script_executeAicaCommand(197801, 0, 0);
    script_DA2B(arg0, 120.0f);
    Script_PlayCharaMotion(arg0, 4379, 40.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 0.8f);
    local_E4 = 0xC000;
    script_DA47(arg0);
    Script_mtwkGetSetRotation(arg0, &local_E8, 0x2000000);
    Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_clmdSetFlags(arg0, 4, 0);
    while (1) {
      if (script_DA63(arg0, local_E4, 4551, 1)) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    while (1) {
      Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
      if (!((&local_F4)->x < 3.64800262f)) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    while (1) {
      if (script_DA63(arg0, 0xE000, 546, 0)) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    Script_MotStepUtil(arg0, 0);
  }
  end:;
}

static int32_t script_E35E(void) {
  return SCNF_UIUtils(4, 15, 2, 13, 1, 0);
}

__attribute__((scn_string_bindings(0, data_16CDC, 1, data_16D2C)))
static void script_E373(void) {
  int32_t unused_F8[2];
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  int32_t local_E4;
  int32_t local_E0;
  float local_DC;
  float local_D8;
  int8_t local_D7;
  uint8_t unused_D4[3];
  int32_t local_D0;
  int32_t local_CC;
  int32_t local_C8;
  int32_t local_C4;
  Script_SetQteOverlayTimeoutTuning(0, QTE_TimeoutSession);
  Script_SetQteOverlayTimeoutTuning(0, QTE_DisableTimeoutBoost);
  Script_uiUtils(0, 21, 0);
  Script_SetShouldSleepFlag(0);
  Script_LoadVoiceFile(1, "Q1003");
  Script_facemotUtils(CHARA_AKIR, 0, 0);
  Script_FaceEx(CHARA_KEBA, FACE_ExpressionEnabled, 0, 2);
  (void)((local_D7 = 0) & 1);
  while (1) {
    local_E0 = 0;
    while (local_E0 < 9) {
      script_D9C1(local_E0);
      local_E0 = local_E0 + 1;
    }
    Script_shenSetLightingMode(2);
    Script_SetupLightFromTable(8);
    Script_ProcessFogFromTable(1);
    local_DC = Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 40.0f);
    local_D8 = Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 20.0f);
    local_F4 = Script_scenCreateTask(script_DEFF, 1, CHARA_AKIR);
    *(&local_F4 + 1) = Script_scenCreateTask(script_DD02, 1, CHARA_KEBA);
    *(&local_F4 + 2) = Script_scenCreateTask(script_DB7E, 1, SHEN_NAME('KAIS'));
    if (0) {
      Script_charaDisplay(SHEN_NAME('KAIS'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('KAIS'), CDM_Show);
        goto L00E494;
      }
    }
    L00E494:;
    Script_scenToggleVisibleFor(SHEN_NAME('KAIS'), 1);
    Script_charaDisplay(SHEN_NAME('TEL_'), CDM_Show);
    Script_ccowSetHeightMode(SHEN_NAME('TEL_'), CCOW_KeepHeight);
    Script_EVT_SetPosition(SHEN_NAME('TEL_'), POS_XYZ, &data_16CE4);
    Script_EVT_SetRotation(SHEN_NAME('TEL_'), 0x38000000, &data_16D14, 0);
    Script_charaDisplay(SHEN_NAME('ISU_'), CDM_Show);
    Script_ccowSetHeightMode(SHEN_NAME('ISU_'), CCOW_KeepHeight);
    Script_EVT_SetPosition(SHEN_NAME('ISU_'), POS_XYZ, &data_16CF0);
    *(&local_C8 + 1) = 0x4000;
    Script_EVT_SetRotation(SHEN_NAME('ISU_'), MATRIX_XFORM_Y, (const Vector3i *)&local_C8, 0);
    if (0) {
      Script_charaDisplay(SHEN_NAME('MAGZ'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('MAGZ'), CDM_Show);
        goto L00E54B;
      }
    }
    L00E54B:;
    Script_scenToggleVisibleFor(SHEN_NAME('MAGZ'), 1);
    Script_charaPosRotUtils(SHEN_NAME('MAGZ'), 152, 0, &data_16D20, CHARA_PART_Set);
    if (0) {
      Script_charaDisplay(SHEN_NAME('PEN_'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('PEN_'), CDM_Show);
        goto L00E595;
      }
    }
    L00E595:;
    Script_scenToggleVisibleFor(SHEN_NAME('PEN_'), 1);
    if (0) {
      Script_charaDisplay(SHEN_NAME('CLIP'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('CLIP'), CDM_Show);
        goto L00E5C9;
      }
    }
    L00E5C9:;
    Script_scenToggleVisibleFor(SHEN_NAME('CLIP'), 1);
    Script_EVT_SetPosition(SHEN_NAME('GATL'), POS_XYZ, &data_16CFC);
    Script_EVT_SetPosition(SHEN_NAME('GATR'), POS_XYZ, &data_16D08);
    Script_ccowUtils(SHEN_NAME('TEL_'), CCOW_DisableAll);
    Script_ccowUtils(SHEN_NAME('KAIS'), CCOW_DisableAll);
    Script_ccowUtils(SHEN_NAME('ISU_'), CCOW_DisableAll);
    local_F0 = 0;
    local_EC = 0;
    local_E8 = 0;
    local_E4 = 0;
    Script_PlayerStatsUtils(PS_Set70MBTimeCapSlot, 9, Script_PlayerStatsUtils(PS_Get70MBTimeCapSlot, 9) + 1);
    while (local_E8 != -1) {
      local_D0 = local_E8;
      switch (local_D0) {
        case 0:
          Script_filmCheckOrSetTransitionUtil(32, 255, 0, 0, 0, 0, 0, 0, 0);
          local_E8 = 1;
          break;
        case 1:
          if (Script_filmCheckOrSetTransitionUtil(-1) == 0) {
            local_E8 = 2;
          }
          break;
        case 2:
          if (local_EC == 5) {
            script_D996(0);
            local_E8 = 3;
          }
          break;
        case 3:
          if (local_EC == 218) {
            Script_executeAicaCommand(1193, 0, 0);
            script_D996(1);
            local_E8 = 4;
          }
          break;
        case 4:
          if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) < 360.0f)) {
            script_D996(2);
            local_E8 = 5;
          }
          break;
        case 5:
          if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) < 380.0f)) {
            script_D996(3);
            local_E8 = 6;
          }
          break;
        case 6:
          if (local_EC == 14) {
            Script_scenCreateTask(script_E35E, 0);
            local_E8 = 7;
          }
          break;
        case 7:
          local_CC = SCNF_UIUtils(0, 0, 0, 0, 0, -1);
          if (local_CC != 0xFFFF) {
            if ((local_CC & 4) == 4) {
              script_D996(5);
            }
          }
          if (local_EC >= 30) {
            if (script_D9EF(5)) {
              local_E8 = 8;
            } else {
              local_E8 = 9;
            }
            script_D996(4);
          }
          break;
        case 8:
          if (local_EC == 14) {
            script_D996(7);
          }
          if (local_F0 == 580) {
            Script_filmCheckOrSetTransitionUtil(32, 0, 0, 0, 0, 255, 0, 0, 0);
            local_E8 = 10;
            Script_executeAicaCommand(2720, 2, 30);
          }
          break;
        case 9:
          if (local_EC == 100) {
            script_D996(6);
          }
          if (local_F0 == 625) {
            Script_filmCheckOrSetTransitionUtil(32, 0, 0, 0, 0, 255, 0, 0, 0);
            local_E8 = 10;
          }
          break;
        case 10:
          if (Script_filmCheckOrSetTransitionUtil(-1) == 0) {
            local_E8 = -1;
          }
          break;
      }
      local_D0 = local_F0;
      switch (local_D0) {
        case 0:
          Script_seqEnterEvtCam(1100, 0, AUTH_None);
          break;
        case 75:
          Script_seqEnterEvtCam(1101, 0, AUTH_None);
          break;
        case 255:
          Script_seqEnterEvtCam(1102, 0, AUTH_None);
          break;
        case 280:
          Script_seqEnterEvtCam(1103, 0, AUTH_None);
          break;
        case 310:
          Script_seqEnterEvtCam(1104, 0, AUTH_None);
          break;
        case 267:
          Script_SetCamShake(0.15f, 0.02f, 0.5f);
          break;
        case 299:
          Script_scenCreateTask(script_DB1E, 0);
          break;
        case 302:
          break;
      }
      if (script_D9EF(5)) {
        local_D0 = local_F0;
        switch (local_D0) {
          case 331:
            Script_seqEnterEvtCam(1110, 0, AUTH_None);
            Script_scenCreateTask(script_DB42, 0);
            break;
          case 365:
            Script_seqEnterEvtCam(1111, 0, AUTH_None);
            break;
          case 391:
            Script_scenCreateTask(script_DB4E, 0);
            break;
          case 485:
            Script_scenCreateTask(script_DB5A, 0);
            break;
        }
      } else {
        local_D0 = local_F0;
        switch (local_D0) {
          case 341:
            Script_seqEnterEvtCam(1120, 0, AUTH_None);
            break;
          case 374:
            Script_seqEnterEvtCam(1121, 0, AUTH_None);
            break;
          case 491:
            script_D996(8);
            Script_seqEnterEvtCam(1122, 0, AUTH_None);
            break;
          case 361:
            Script_scenCreateTask(script_DB66, 0);
            break;
          case 391:
            Script_scenCreateTask(script_DB72, 0);
            break;
          case 432:
            Script_scenCreateTask(script_DB2A, 0);
            break;
          case 511:
            Script_scenCreateTask(script_DB36, 0);
            break;
        }
      }
      local_F0 = local_F0 + 1;
      if (local_E8 != local_E4) {
        local_EC = 0;
        local_E4 = local_E8;
      } else {
        local_EC = local_EC + 1;
      }
      SCNF_WaitFrames_0000(1);
    }
    local_E0 = 0;
    while (local_E0 < 3) {
      Script_srchControl((void *)*(&local_F4 + local_E0));
      local_E0 = local_E0 + 1;
    }
    Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, local_DC);
    Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, local_D8);
    Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
    if (script_D9EF(5)) {
      Script_fixoClearAttachment(SHEN_NAME('KAIS'));
      SCNF_WaitFrames_0000(1);
      break;
    } else {
      Script_IncrementDateTimeByNum(1);
      Script_ReturnTimeDate((GameDateTime *)&local_C8);
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_AFTER_8PM_GUARD_SHIFT)) {
        *((int8_t *)&local_C8 + 4) = 20;
      } else {
        *((int8_t *)&local_C8 + 4) = 19;
      }
      *((int8_t *)&local_C8 + 5) = 0;
      *((int8_t *)&local_C8 + 6) = 0;
      Script_SetNewDateTime((GameDateTime *)&local_C8);
      if ((*((int8_t *)&local_C8 + 1) == 4) & (*((int8_t *)&local_C8 + 2) == 15)) {
        *((int8_t *)&local_C8 + 4) = 7;
        *((int8_t *)&local_C8 + 5) = 59;
        *((int8_t *)&local_C8 + 6) = 0;
        Script_SetNewDateTime((GameDateTime *)&local_C8);
        Script_shenRequestGameModuleTransition(2, SHEN_NAME('JOMO'), 4);
        SCNF_WaitFrames_0000(1);
      } else {
        if (*((int8_t *)&local_C8 + 3) == 1) {
          script_AF73();
          Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 189, 1);
          goto L00EBEA;
        }
      }
      L00EBEA:;
      Script_WeatherTaskUtil(FreeTaskData);
      Script_WeatherTaskUtil(InitTaskData);
      Script_WeatherTaskUtil(AndNotInstructionFlags, 0x784);
      Script_SetupLightFromTable(8);
      Script_ProcessFogFromTable(1);
      Script_cdttLoadScrUtils(CDTT_CMD_Clear, 0);
      SCNF_WaitFrames_0000(120);
      Script_cdttLoadScrUtils(CDTT_CMD_Init, 0);
      local_C4 = Script_shenGetStats(STATS_CASH);
      Script_SetStats(STATS_CASH, local_C4 + 180);
      Script_PlayerStatsUtils(PS_SetStatU8_1, Script_PlayerStatsUtils(PS_GetStatU8_1) + 1);
      Script_PlayerStatsUtils(PS_SetStatU8_0, Script_PlayerStatsUtils(PS_GetStatU8_0) + 1);
      if (local_D7 == 0) {
        Script_PlayerStatsUtils(PS_Set70MBCounter1, Script_PlayerStatsUtils(PS_Get70MBCounter1) + 1);
        (void)((local_D7 = -1) & 4);
      }
    }
  }
  Script_FaceEx(CHARA_KEBA, FACE_ExpressionDisabled, 0, 2);
  Script_LoadVoiceFile(0, "Q1003");
  Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
  Script_EVT_SetPosition(SHEN_NAME('GATL'), POS_XYZ, &data_16D34);
  Script_EVT_SetPosition(SHEN_NAME('GATR'), POS_XYZ, &data_16D40);
  Script_SetQteOverlayTimeoutTuning(2, QTE_TimeoutSession);
  Script_MotStepUtil(CHARA_AKIR, 1);
  Script_PlayerStatsUtils(PS_Set70MBCounter0, Script_PlayerStatsUtils(PS_Get70MBCounter0) + 1);
  script_D9EF(5);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MKSG_HELP_ROUTE_ACTIVE, 1);
  return;
}

static void script_ED14(const char *arg0, const char *arg1) {
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, arg0, arg1) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_16D4C)))
static int32_t script_ED4E(const char *arg0) {
  script_ED14("scene/02/MKSG", arg0);
  return (task_202 = 0) & 0x20;
}

static void script_ED69(int32_t arg0, int32_t arg1, int32_t arg2) {
  if (arg1 == 2) {
    Script_seqEnterEvtCam(5000, 0, AUTH_None);
  }
  (void)((task_184 = 0) & 0x20);
  Script_mapmSetCamFlags(1, 6, 0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  (void)((task_202 = -1) & 0x20);
  Script_scenCreateTask(script_ED4E, 1, arg0);
  SCNF_WaitFrames_0000(90);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_F55C(arg2);
  while (task_202 & 8) {
    SCNF_WaitFrames_0000(1);
  }
  Script_mapmSetCamFlags(1, 9, 1);
  Script_mapmSetRenderFlag(9, 1);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 30.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 30.0f);
  Script_LODUtils(LOD_UTILS_SET_NPC_DISPLAY_RADIUS, 20.0f);
  Script_WeatherTaskUtil(ToggleFogTask, 0);
  Script_WeatherTaskUtil(ToggleSceneLightingFlags, 0);
  Script_SetupLightFromTable(0);
  Script_SetGlobalSceneSlotValue34(0, 1);
  Script_SetGlobalSceneSlotValue34(1, 1);
  Script_SetGlobalSceneSlotValue34(2, 1);
  Script_SetGlobalSceneSlotValue34(3, 1);
  Script_SetGlobalSceneSlotValue34(4, 1);
  Script_SetGlobalSceneSlotValue34(5, 1);
  Script_SetGlobalSceneSlotValue34(6, 1);
  Script_SetGlobalSceneSlotValue34(7, 1);
  Script_SetGlobalSceneSlotValue34(8, 1);
  Script_PREP_Related(-1, 0);
  Script_SetupLightFromTable(arg1);
  Script_PREP_Related(9, 1);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  while ((&task_10)->mode) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(30);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  if (arg1 == 2) {
    Script_SetGameStateTo(AUTH_None);
  }
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ryoControl(RYO_RestoreControl);
  (void)((task_184 = -1) & 0x20);
  return;
}

static void script_EF3F(const char *arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  (void)((task_184 = 0) & 0x20);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 70.0f);
  Script_LODUtils(LOD_UTILS_SET_NPC_DISPLAY_RADIUS, 48.0f);
  SCNF_WaitFrames_0000(120);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_F55C(0);
  Script_WeatherTaskUtil(ToggleFogTask, 1);
  Script_WeatherTaskUtil(ToggleSceneLightingFlags, 1);
  Script_SetGlobalSceneSlotValue34(0, 0);
  Script_SetGlobalSceneSlotValue34(1, 0);
  Script_SetGlobalSceneSlotValue34(2, 0);
  Script_SetGlobalSceneSlotValue34(3, 0);
  Script_SetGlobalSceneSlotValue34(4, 0);
  Script_SetGlobalSceneSlotValue34(5, 0);
  Script_SetGlobalSceneSlotValue34(6, 0);
  Script_SetGlobalSceneSlotValue34(7, 0);
  Script_SetGlobalSceneSlotValue34(8, 0);
  Script_PREP_Related(-1, 0);
  Script_ReturnTimeDate(&local_F4);
  if ((&local_F4)->hours >= 19) {
    script_82BB();
    script_83B8(8);
    Script_SetupLightFromTable(0);
    Script_ProcessFogFromTable(0);
    Script_mapmSetRenderFlag(3, 0);
    Script_mapmSetRenderFlag(7, 1);
    Script_PREP_Related(-1, 1);
    Script_SetCharaFlags(CHARA_AKIR, 0);
    Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 2);
  } else {
    Script_SetCharaFlags(CHARA_AKIR, 4);
    Script_shenSetLightingMode(1);
    Script_LightUtils(0, 0, 2);
    Script_lightSetSlotEnabled(0, 1);
  }
  Script_mapmSetCamFlags(1, 6, 1);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 70.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 60.0f);
  Script_LODUtils(LOD_UTILS_SET_NPC_DISPLAY_RADIUS, 48.0f);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, arg0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PROCESS_COLS, 0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  while ((&task_10)->mode) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(10);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ryoControl(RYO_RestoreControl);
  (void)((task_184 = -1) & 8);
  return;
}

static int32_t script_F131(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  int32_t local_D8;
  int8_t local_D7;
  int8_t local_D6;
  int8_t local_D5;
  int8_t local_D4;
  int8_t local_D3;
  uint8_t unused_D0[3];
  int32_t local_CC;
  (void)((local_D3 = -1) & 8);
  if (task_1B8) {
    local_D4 = Script_ryoSleepUtils(3, 0, 0);
  } else {
    Script_ryoSleepUtils(3, 1, 0, 1);
    Script_ryoSleepUtils(3, 1, 1, 0);
    local_D4 = 1;
  }
  Script_ReturnTimeDate(&local_F4);
  if ((&local_F4)->hours >= 19) {
    Script_SetCharaFlags(CHARA_AKIR, 0);
    Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 2);
    (void)((local_D3 = 0) & 8);
  }
  for (;;) {
    Script_ReturnTimeDate(&local_F4);
    if (((&task_10)->mode != 0) & (task_10.id == 0x52494B41)) {
      local_D7 = 0;
      local_CC = task_10.entry_idx;
      switch (local_CC) {
        case 0:
          local_D8 = (int32_t)data_16D5A;
          local_D6 = 1;
          local_D7 = 71;
          local_D5 = 1;
          break;
        case 2:
          local_D8 = (int32_t)data_16D62;
          local_D6 = 2;
          local_D7 = 72;
          local_D5 = 2;
          break;
        case 4:
          local_D8 = (int32_t)data_16D6A;
          local_D6 = 3;
          local_D7 = 73;
          local_D5 = 3;
          break;
        case 5:
          local_D8 = (int32_t)data_16D72;
          local_D6 = 4;
          local_D7 = 74;
          local_D5 = 4;
          break;
        case 6:
          local_D8 = (int32_t)data_16D7A;
          local_D6 = 5;
          local_D7 = 75;
          local_D5 = 5;
          break;
        case 9:
          local_D8 = (int32_t)data_16D82;
          local_D6 = 6;
          local_D7 = 77;
          local_D5 = 7;
          break;
        case 1:
          local_D8 = (int32_t)data_16D8A;
          local_D6 = 7;
          local_D7 = 78;
          local_D5 = 8;
          break;
        case 8:
          Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 6, 1);
          Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
          SCNF_WaitFrames_0000(90);
          Script_executeAicaCommand(2720, 1, 30);
          Script_executeAicaCommand(2720, 2, 30);
          Script_executeAicaCommand(2720, 3, 30);
          Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
          while (Script_filmCheckOrSetTransitionUtil(-1)) {
            SCNF_WaitFrames_0000(1);
          }
          if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_SECRET_BOX_OPENING_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS8A_POST_MS08_FOLLOWUP_COMPLETE) == 0)) {
            Script_shenRequestGameModuleTransition(2, SHEN_NAME('MS8A'), 90);
            SCNF_WaitFrames_0000(1);
          } else {
            Script_shenRequestGameModuleTransition(2, SHEN_NAME('MS08'), 0);
            SCNF_WaitFrames_0000(1);
          }
          break;
      }
      if (local_D4 == 1) {
        if (local_D7) {
          Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, local_D7);
        }
        Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, local_D5, 1);
        script_ED69(local_D8, local_D6, local_D7);
        Script_ryoSleepUtils(3, 1, 0, 0);
        Script_ryoSleepUtils(3, 1, 1, local_D7);
        local_D4 = 0;
      } else {
        script_EF3F((const char *)local_D8);
        Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, local_D5, 0);
        if ((&local_F4)->hours >= 19) {
          (void)((local_D3 = 0) & 8);
        }
        Script_ryoSleepUtils(3, 1, 0, 1);
        Script_ryoSleepUtils(3, 1, 1, 0);
        local_D4 = 1;
      }
    } else {
      if (local_D3 & -(Script_IsPlayerAllowed(0) != 0) & local_D4) {
        if ((&local_F4)->hours >= 19) {
          script_F9A4();
          (void)((local_D3 = 0) & 8);
        }
        goto L00F477;
      }
    }
    L00F477:;
    local_E8.x = 0.2f;
    (&local_E8)->y = 0.0f;
    (&local_E8)->z = -9.48f;
    local_DC.x = 1.71f;
    (&local_DC)->y = 0.0f;
    (&local_DC)->z = -4.11f;
    if (Script_SetTaskDataValue(CHARA_AKIR, &local_E8, &local_DC, 0)) {
      Script_ryoControl(PCCTL_DisablePlayerControl);
      Script_shenClearFreeState(0);
      Script_executeAicaCommand(2720, 1, 30);
      Script_executeAicaCommand(2720, 2, 30);
      Script_executeAicaCommand(2720, 3, 30);
      Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_shenRequestGameModuleTransition(2, SHEN_NAME('MFSY'), 1);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_F537(void) {
  while (Script_filmCheckOrSetTransitionUtil(-1) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_cdttLoadScrUtils(CDTT_CMD_Init, 0);
  end:;
}

static void script_F55C(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  if (local_FC == 71) {
    Script_cdttLoadScrUtils(CDTT_CMD_SetMode, 0x31534B4D);
  } else if (local_FC == 72) {
    Script_cdttLoadScrUtils(CDTT_CMD_SetMode, 0x32534B4D);
  } else if (local_FC == 73) {
    Script_cdttLoadScrUtils(CDTT_CMD_SetMode, 0x34534B4D);
  } else if (local_FC == 74) {
    Script_cdttLoadScrUtils(CDTT_CMD_SetMode, 0x35534B4D);
  } else if (local_FC == 75) {
    Script_cdttLoadScrUtils(CDTT_CMD_SetMode, 0x36534B4D);
  } else if (local_FC == 77) {
    Script_cdttLoadScrUtils(CDTT_CMD_SetMode, 0x39534B4D);
  } else if (local_FC == 78) {
    Script_cdttLoadScrUtils(CDTT_CMD_SetMode, 0x30534B4D);
  } else {
    Script_cdttLoadScrUtils(CDTT_CMD_SetMode, 0x47534B4D);
  }
  SCNF_WaitFrames_0000(30);
  Script_scenCreateTask(script_F537, 0);
  return;
}

static void script_F62C(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 3)) {
    Script_mapmSetRenderFlag(2, 1);
    Script_mapmSetRenderFlag(4, 0);
    Script_mapmSetRenderFlag(5, 1);
  } else if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 2)) {
    Script_mapmSetRenderFlag(2, 0);
    Script_mapmSetRenderFlag(4, 0);
    Script_mapmSetRenderFlag(5, 1);
  } else {
    Script_mapmSetRenderFlag(2, 0);
    Script_mapmSetRenderFlag(4, 1);
    Script_mapmSetRenderFlag(5, 0);
  }
  Script_ReturnTimeDate(&local_F4);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS08_SCENE_EXIT_COMPLETE)) {
    if ((&local_F4)->hours < 19) {
      Script_shenSetLightingMode(1);
      Script_LightUtils(0, 0, 2);
      Script_lightSetSlotEnabled(0, 1);
      Script_PREP_Related(-1, 0);
      Script_mapmSetRenderFlag(3, 1);
      Script_mapmSetRenderFlag(7, 0);
    } else {
      Script_SetupLightFromTable(0);
      Script_PREP_Related(-1, 1);
      Script_mapmSetRenderFlag(3, 0);
      Script_mapmSetRenderFlag(7, 1);
    }
  } else {
    Script_mapmSetRenderFlag(3, 0);
    Script_mapmSetRenderFlag(7, 1);
    Script_PREP_Related(0, 1);
  }
  return;
}

static void script_F733(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  script_7FBB(CHARA_AKIR, (DSWKDoorEntry *)data_154BC, (int32_t *)data_1572C, (DoorResourceDescriptor *)&data_157D8, (MapObjectRecord *)data_15828, 10, 0);
  SCNF_WaitFrames_0000(2);
  DSWK_DoorUtils(DOOR_CMD_SET_STATE_1);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS08_SCENE_EXIT_COMPLETE)) {
    Script_ReturnTimeDate(&local_F4);
    script_8834(3, 7, -1, 0x2302AB, -1, -1, -1);
    DSWK_AssignDoorEntryConfig(3, 0, -1, -1, -1, -1, -1, -1);
    DSWK_AssignDoorEntryConfig(7, 0, -1, -1, -1, -1, -1, -1);
    DSWK_AssignDoorEntryConfig(10, 0, -1, -1, -1, -1, -1, -1);
    if ((&local_F4)->hours < 19) {
      script_8381(3);
      script_8381(7);
      script_8381(10);
    } else {
      script_82BB();
      script_83B8(8);
    }
  }
  return;
}

static void script_F807(void) {
  Script_WeatherUtils(WeatherScript_LoadWccpClipSet0FromTaskOffset, &data_16D94, 9);
  script_F81D();
  return;
}

static void script_F81D(void) {
  int8_t local_FF;
  local_FF = 0;
  while (local_FF < 9) {
    Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, local_FF, 0);
    local_FF = local_FF + 1;
  }
  Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 0, 1);
  return;
}

static int32_t script_F851(void) {
  int32_t local_FC;
  local_FC = script_F86B();
  Script_scenCreateTask(script_F93C, 1, local_FC);
  end:;
}

__attribute__((scn_string_bindings(0, data_16F98, 2, data_16FAE, 4, data_16FC6)))
static int32_t script_F86B(void) {
  int32_t local_FC;
  local_FC = Script_MT5Loader_TRWV_TRWAVE(0, &data_16F8C, "model/object", "SEA3.PVR", "model/object", "TRSEA4.MT6", 4.0f, 0);
  Script_MT5Loader_TRWV_TRWAVE(21, local_FC, 8, 8);
  Script_MT5Loader_TRWV_TRWAVE(16, local_FC, -1, 0);
  Script_MT5Loader_TRWV_TRWAVE(13, local_FC, 0.0901960805f, 0.196078435f);
  Script_MT5Loader_TRWV_TRWAVE(15, local_FC, "model/object", "TRWTBL1.BIN");
  Script_MT5Loader_TRWV_TRWAVE(12, local_FC, 0x4000);
  Script_MT5Loader_TRWV_TRWAVE(10, local_FC, 64.0f, 64.0f, 1, 1, 3, 1);
  Script_MT5Loader_TRWV_TRWAVE(24, local_FC, 0, 203, 255, 143);
  Script_MT5Loader_TRWV_TRWAVE(11, local_FC, -0.51f, -0.68f, 0.52f);
  return local_FC;
}

static void script_F930(int32_t arg0) {
  Script_MT5Loader_TRWV_TRWAVE(3, arg0);
  return;
}

static int32_t script_F93C(int32_t arg0) {
  int8_t local_FF;
  (void)((local_FF = -1) & 1);
  for (;;) {
    if (Script_SetTaskDataValue(CHARA_AKIR, &data_16FE0, &data_16FEC, 0)) {
      if (local_FF == 0) {
        Script_MT5Loader_TRWV_TRWAVE(1, arg0);
        (void)((local_FF = -1) & 1);
      }
    } else {
      if (local_FF == -1) {
        Script_MT5Loader_TRWV_TRWAVE(2, arg0);
        (void)((local_FF = 0) & 1);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_16FF8, 1, data_17006, 2, data_1700C, 4, data_17013, 5, data_17018, 6, data_1701D, 8, data_1702D, 9, data_17033, 10, data_1703A)))
static void script_F9A4(void) {
  int32_t unused_FC;
  Vector3 local_F0;
  Vector3i local_E4;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_sicnUtils(1, 0, 0);
  SCNF_RequestIconNotification(0, 0, 0, 2);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F0, 0);
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_E4, 0);
  Script_filmCheckOrSetTransitionUtil(20, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetupLightFromTable(0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  script_ED14("scene/02/MKSG", "NIGHT");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 0, "NIGHT", "");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_LoadSND(0, 5, "FREE", 0);
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 3, "FREE", 0);
  Script_LoadSND(0, 2, "bgm109.snd", 0);
  Script_filmCheckOrSetTransitionUtil(10, 255, 0, 0, 0, 0, 0, 0, 0);
  script_B7EB(0);
  Script_AuthorUtils(AUTH_FREE, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "NIGHT");
  script_F62C();
  Script_SetCharaFlags(CHARA_AKIR, 0);
  Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 2);
  Script_LoadSND(0, 2, "SELECT", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  Script_LoadSND(0, 4, "f1kyusok.snd", 0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F0);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E4, 0);
  Script_convSetCamInactiveFlag(-1);
  Script_SetGameStateTo(AUTH_None);
  Script_WeatherTaskUtil(FreeTaskData);
  Script_WeatherTaskUtil(InitTaskData);
  script_82BB();
  script_83B8(8);
  Script_filmCheckOrSetTransitionUtil(20, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  return;
}

static int32_t script_FC24(void) {
  script_AB36();
  for (;;) {
    if (script_AB88() != -1) {
      script_AB60();
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_FC49(void) {
  Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_17048);
  for (;;) {
    Script_srchUtils(-1);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_FC6A(void) {
  while (1) {
    if (Script_filmCheckOrSetTransitionUtil(-1)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_cdttLoadScrUtils(CDTT_CMD_Init, 0);
  end:;
}

static void script_FC93(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int8_t local_F7;
  int8_t local_F6;
  int32_t local_F0;
  local_F0 = arg0;
  switch (local_F0) {
    case 71:
      local_FC = (int32_t)data_1706A;
      local_F7 = 1;
      local_F8 = 0;
      local_F6 = 1;
      break;
    case 72:
      local_FC = (int32_t)data_17072;
      local_F7 = 2;
      local_F8 = 2;
      local_F6 = 2;
      break;
    case 73:
      local_FC = (int32_t)data_1707A;
      local_F7 = 3;
      local_F8 = 4;
      local_F6 = 3;
      break;
    case 74:
      local_FC = (int32_t)data_17082;
      local_F7 = 4;
      local_F8 = 5;
      local_F6 = 4;
      break;
    case 75:
      local_FC = (int32_t)data_1708A;
      local_F7 = 5;
      local_F8 = 6;
      local_F6 = 5;
      break;
    case 77:
      local_FC = (int32_t)data_17092;
      local_F7 = 6;
      local_F8 = 9;
      local_F6 = 7;
      break;
    case 78:
      local_FC = (int32_t)data_1709A;
      local_F7 = 7;
      local_F8 = 1;
      local_F6 = 8;
      break;
  }
  Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, local_F6, 1);
  script_83B8(local_F8);
  Script_mapmSetCamFlags(1, 6, 0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  (void)((task_202 = -1) & 1);
  Script_scenCreateTask(script_ED4E, 1, local_FC);
  while (task_202 & 8) {
    SCNF_WaitFrames_0000(1);
  }
  Script_mapmSetCamFlags(1, 9, 1);
  Script_mapmSetRenderFlag(9, 1);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 30.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 30.0f);
  Script_LODUtils(LOD_UTILS_SET_NPC_DISPLAY_RADIUS, 20.0f);
  Script_WeatherTaskUtil(ToggleFogTask, 0);
  Script_WeatherTaskUtil(ToggleSceneLightingFlags, 0);
  Script_SetupLightFromTable(0);
  Script_SetGlobalSceneSlotValue34(0, 1);
  Script_SetGlobalSceneSlotValue34(1, 1);
  Script_SetGlobalSceneSlotValue34(2, 1);
  Script_SetGlobalSceneSlotValue34(3, 1);
  Script_SetGlobalSceneSlotValue34(4, 1);
  Script_SetGlobalSceneSlotValue34(5, 1);
  Script_SetGlobalSceneSlotValue34(6, 1);
  Script_SetGlobalSceneSlotValue34(7, 1);
  Script_SetGlobalSceneSlotValue34(8, 1);
  Script_PREP_Related(-1, 0);
  Script_SetupLightFromTable(local_F7);
  Script_PREP_Related(9, 1);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

__attribute__((scn_string_bindings(0, data_170A2)))
void script_FED5(void) {
  int32_t unused_FC;
  int32_t local_F8;
  int32_t local_F4;
  task_1B8 = Script_ryoSleepUtils(0);
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_scenCreateTask(script_FC6A, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, -1);
  Script_SetMapFlags(0, 1);
  Script_SetCameraSettings(5, 6, 1, 0);
  Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 70);
  script_F733();
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  script_F62C();
  script_F807();
  script_BA85();
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS08_SCENE_EXIT_COMPLETE) == 0) {
    script_ED14("scene/02/MKSG", "EVENT");
    script_BC86();
    Script_shenClearFreeState(0);
    Script_uiUtils(0, 30, 0);
    Script_ryoControl(PCCTL_SetCPCTFlag800Inverted, 0);
    Script_GetConversationDetails(CONV_DETAILS_MODE_DISABLE_PROMPT, 0);
    Script_LoadVoiceFile(1, "E1006");
    (void)((task_1C9 = 0) & 1);
    (void)((task_1CC = 0) & 1);
    task_1C5 = 0;
    (void)((task_1C6 = 0) & 1);
    Script_WeatherTaskUtil(AndNotInstructionFlags, 0x784);
    Script_SetupLightFromTable(0);
    Script_ProcessFogFromTable(0);
    task_1BC = Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, -1.0f);
    task_1C0 = Script_SetCameraSettings(CAM_UTILS_SET_HEIGHT_ABOVE_PLAYER, -1.0f);
    Script_cpctStateUtils(1, 1);
    local_F8 = 0;
    while (local_F8 < 6) {
      local_F4 = local_F8;
      if (local_F4 == 0) {
        (void)((task_1C7 = -1) & 4);
        (void)((task_1C8 = 0) & 4);
        Script_scenCreateTask(script_10F19, 2, *(&data_15F60 + local_F8), local_F8);
      } else {
        Script_scenCreateTask(script_106DB, 2, *(&data_15F60 + local_F8), local_F8);
      }
      Script_CharHandlers(*(&data_15F60 + local_F8), 2, 3, 16);
      Script_charaSetRefboardFlagTo(*(&data_15F60 + local_F8), 1);
      Script_SetCharaRefBoardPos(*(&data_15F60 + local_F8), &data_170BC);
      Script_SetShadowFlags(*(&data_15F60 + local_F8), SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 4);
      Script_SetShadowFlags(*(&data_15F60 + local_F8), SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 7);
      Script_SetShadowFlags(*(&data_15F60 + local_F8), SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 8);
      Script_SetShadowFlags(*(&data_15F60 + local_F8), SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 9);
      Script_SetShadowFlags(*(&data_15F60 + local_F8), SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 11);
      local_F8 = local_F8 + 1;
    }
    task_1B4 = 0;
    Script_scenCreateTask(script_BC5A, 0);
    (void)((task_1CC = -1) & 2);
    Script_scenCreateTask(script_12432, 0);
    Script_scenCreateTask(script_12A16, 1, CHARA_AKIR);
    Script_scenCreateTask(script_13627, 0);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MKSG_HELP_ROUTE_ACTIVE, 1);
    Script_GetCassetteFlags(51, 1, 0, 0);
    Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMFLAS, 1);
    Script_GetCurrentDrawMode(0, 1);
    Script_GetCurrentDrawMode(1, 1);
    SCNF_WaitFrames_0000(2);
    script_82BB();
    script_83B8(8);
  } else {
    script_ACDB(CHARA_AKIR);
    Script_scenCreateTask(script_F131, 0);
    Script_scenCreateTask(script_F851, 0);
    Script_scenCreateTask(script_FC24, 0);
    Script_scenCreateTask(script_FC49, 0);
    Script_motiLoad(2, SHEN_NAME('FREE'), "M_MOBJ.BIN");
    Script_CycleManagerUtils(CYCLE_CMD_SETUP, "scene/02/MFSY", (int32_t)"CYCLEMAN.BIN", 0x47534B4D);
    if (task_1B8) {
      if (Script_ryoSleepUtils(3, 0, 1)) {
        script_FC93(Script_ryoSleepUtils(3, 0, 1));
      }
      Script_ryoSleepUtils(1);
    }
    Script_ryoSleepUtils(2, 1);
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_shenClearFreeState(1);
    Script_ryoControl(RYO_RestoreControl);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 70.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 20.0f);
  return;
}

static void script_102B5(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_ccowUtils(arg1, CCOW_DisableAll);
  Script_RequestFightAnimation(arg0, 1139, 0.0f, 8, 1);
  SCNF_WaitFrames_0000(1);
  local_F4.x = 0.08f;
  (&local_F4)->y = 0.01f;
  (&local_F4)->z = 0.04f;
  local_E8.x = 8829;
  (&local_E8)->y = 22027;
  (&local_E8)->z = 61895;
  Script_fixoAttachEx(arg1, arg0, 12, &local_F4, &local_E8);
  Script_charaDisplay(arg1, CDM_Show);
  return;
}

static void script_1032D(int32_t arg0, int32_t arg1) {
  Script_lightResetSlotToDefaults(arg0);
  Script_LightUtils(arg0, 0, 4);
  Script_lightSpotLightEnable(arg0, &data_170F4, &data_17100);
  Script_LightUtils(arg0, 2, &data_1710C);
  if (arg1 == 0) {
    Script_LightUtils(arg0, 4, 2.0f);
    Script_LightUtils(arg0, 5, 11.0f);
  } else {
    Script_LightUtils(arg0, 4, 1.0f);
    Script_LightUtils(arg0, 5, 7.0f);
  }
  Script_LightUtils(arg0, 6, data_1711C);
  Script_LightUtils(arg0, 10, 2275);
  Script_lightSetSlotEnabled(arg0, 1);
  return;
}

static void script_103BD(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  Script_EVT_GetPosition(arg0, 12, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg0, &local_DC, 0x42000000);
  local_E8.x = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_DC)->y);
  (&local_E8)->y = 0.15f;
  (&local_E8)->z = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_DC)->y);
  Script_LightUtils(arg1, 7, &local_F4);
  Script_LightUtils(arg1, 8, &local_E8);
  return;
}

static void script_10421(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3 local_E8;
  int32_t unused_E4;
  int32_t local_DC[2];
  Script_EVT_GetPosition(arg0, 12, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  local_DC[1] = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_F4)->x - *(float *)arg2, (&local_F4)->z - *(float *)(arg2 + 8));
  local_DC[0] = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, (&local_F4)->x - *(float *)arg2, (&local_F4)->z - *(float *)(arg2 + 8)), (&local_F4)->y - *(float *)(arg2 + 4));
  local_E8.x = (float)0 - Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_DC[1]);
  (&local_E8)->y = (float)0 - Script_MathUtils(SCRIPT_MATH_COS_FIXED16, *(int32_t *)local_DC);
  (&local_E8)->z = (float)0 - Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_DC[1]);
  Script_LightUtils(arg1, 7, &local_F4);
  Script_LightUtils(arg1, 8, &local_E8);
  return;
}

static void script_104DE(int32_t arg0, const Vector3 *arg1, const Vector3 *arg2) {
  Vector3 local_F4;
  int32_t unused_F0;
  int32_t local_E8[2];
  local_E8[1] = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, arg1->x - arg2->x, arg1->z - arg2->z);
  local_E8[0] = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, arg1->x - arg2->x, arg1->z - arg2->z), arg1->y - arg2->y);
  local_F4.x = (float)0 - Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_E8[1]);
  (&local_F4)->y = (float)0 - Script_MathUtils(SCRIPT_MATH_COS_FIXED16, *(int32_t *)local_E8);
  (&local_F4)->z = (float)0 - Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_E8[1]);
  Script_LightUtils(arg0, 7, arg1);
  Script_LightUtils(arg0, 8, &local_F4);
  return;
}

static int32_t script_1058C(int32_t arg0, int32_t arg1) {
  int8_t local_FF;
  int8_t local_FE;
  int8_t local_FD;
  int32_t local_F8;
  local_F8 = arg0;
  if ((local_F8 == 3) | (local_F8 == 4)) {
    local_FF = 1;
    while (local_FF < 6) {
      Script_patrolUtils(*(&data_15F60 + local_FF), arg0, arg1);
      local_FF = local_FF + 1;
    }
  } else {
    switch (local_F8) {
      case 6:
        (void)((task_1CA = 0) & 2);
        local_FF = 1;
        while (local_FF < 6) {
          if ((Script_patrolUtils(*(&data_15F60 + local_FF), 2) == 1) | (Script_patrolUtils(*(&data_15F60 + local_FF), 2) == 5)) {
            (void)((task_1CA = -1) & 0x80);
          }
          local_FF = local_FF + 1;
        }
        break;
      case 5:
        local_FE = 127;
        local_FF = 1;
        while (local_FF < 6) {
          if ((Script_patrolUtils(*(&data_15F60 + local_FF), 2) == 1) | (Script_patrolUtils(*(&data_15F60 + local_FF), 2) == 5)) {
            local_FD = Script_patrolUtils(*(&data_15F60 + local_FF), arg0, arg1);
          } else {
            local_FD = 127;
          }
          if (local_FE > local_FD) {
            local_FE = local_FD;
          }
          local_FF = local_FF + 1;
        }
        (void)local_FE;
        break;
    }
  }
  end:;
}

static int32_t script_106DB(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3 local_E8;
  int32_t unused_E4;
  GameDateTime local_DC;
  int8_t local_DB;
  int8_t local_DA;
  int8_t local_D9;
  int8_t local_D8;
  int8_t local_D7;
  int8_t local_D6;
  uint8_t unused_D4[2];
  int32_t local_D0;
  float local_C4[3];
  Vector3 local_B8;
  int32_t local_B4;
  float local_B0;
  int8_t local_AF;
  int8_t local_AE;
  uint8_t unused_AC[2];
  Vector3 local_A0;
  Vector3 local_94;
  int32_t local_88[3];
  local_DA = 0;
  Script_charaDisplay(arg0, CDM_Show);
  script_1032D(data_15F90[arg1], 0);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  for (;;) {
    if (task_1CC & 1) {
      Script_EVT_GetPosition(CHARA_AKIR, -1, &local_E8, 0);
      local_D0 = local_DA;
      switch (local_D0) {
        case 0:
          local_D0 = arg1;
          if (local_D0 == 1) {
            local_F4.x = -24.3008f;
            (&local_F4)->y = 0.0f;
            (&local_F4)->z = -75.1856f;
          } else if (local_D0 == 2) {
            local_F4.x = -69.154999f;
            (&local_F4)->y = 0.0f;
            (&local_F4)->z = -12.6726f;
          } else if (local_D0 == 3) {
            local_F4.x = -26.3948f;
            (&local_F4)->y = 0.0f;
            (&local_F4)->z = -31.552999f;
          } else if (local_D0 == 4) {
            local_F4.x = -48.600399f;
            (&local_F4)->y = 0.0f;
            (&local_F4)->z = -30.4732f;
          } else {
            local_F4.x = -24.206699f;
            (&local_F4)->y = 0.0f;
            (&local_F4)->z = -53.1744f;
          }
          Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
          local_C4[1] = 2.2958874e-41f;
          Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, (const Vector3i *)local_C4, 0);
          local_DA = 1;
          script_102B5(arg0, *(&data_15F78 + arg1));
          break;
        case 1:
          local_D8 = 2;
          local_DA = 2;
          Script_ReturnTimeDate(&local_DC);
          (void)((local_D6 = -1) & 1);
          local_D0 = arg1;
          if (local_D0 == 1) {
            Script_patrolUtils(arg0, 0, &data_15C08, &data_15EFC, data_15BE8, 1, 2);
            if (((&local_DC)->hours == 20) | ((&local_DC)->hours == 23)) {
              (void)((local_D6 = 0) & 0x10);
            }
          } else if (local_D0 == 2) {
            Script_patrolUtils(arg0, 0, &data_15D24, &data_15F10, data_15BE8, 1, 1);
          } else if (local_D0 == 3) {
            Script_patrolUtils(arg0, 0, &data_15D24, &data_15F24, data_15BE8, 1, 7);
            if ((&local_DC)->hours == 23) {
              (void)((local_D6 = 0) & 0x80);
            }
          } else if (local_D0 == 4) {
            Script_patrolUtils(arg0, 0, &data_15E10, &data_15F38, data_15BE8, 1, 1);
          } else {
            Script_patrolUtils(arg0, 0, &data_15E10, &data_15F4C, data_15BE8, 1, 5);
            if ((&local_DC)->hours == 23) {
              (void)((local_D6 = 0) & 0x80);
            }
          }
          if (local_D6 == 0) {
            local_C4[0] = 1000.0f;
            local_C4[2] = 1000.0f;
            Script_EVT_SetPosition(arg0, 0x28000000, (const Vector3 *)local_C4);
            Script_charaDisplay(arg0, CDM_Hide);
            Script_charaDisplay(*(&data_15F78 + arg1), CDM_Hide);
            Script_lightSetSlotEnabled(data_15F90[arg1], 0);
            local_DA = 99;
          }
          local_DB = 0;
          break;
        case 2:
          if (Script_patrolUtils(arg0, 1) & -(task_1C4 == 1)) {
            task_1C4 = 2;
            Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_EN, -1.0f, -1.0f, -1.0f, 1.0f, 0);
            task_1B0 = arg0;
          }
          local_D9 = Script_patrolUtils(arg0, 2);
          local_D0 = local_D9;
          switch (local_D0) {
            case 0:
              if (local_D8 == 5) {
                script_BBE2(arg1, 1);
              }
              break;
            case 1:
              if (local_D8 == 5) {
              } else {
                if (local_D8 != 1) {
                  script_BBE2(arg1, 3);
                  local_D7 = 150;
                  local_DB = 15;
                  goto L010A8D;
                }
              }
              L010A8D:;
              local_D7 = local_D7 - 1;
              if (local_D7 == 0) {
                script_BBE2(arg1, 0);
                local_D7 = 150;
              }
              break;
            case 2:
              break;
            case 3:
              break;
            case 4:
              break;
            case 5:
              break;
            case 6:
              break;
          }
          local_D8 = local_D9;
          if (task_1C9 & 0x20) {
            local_DA = 0;
          }
          break;
      }
      if (local_DB) {
        script_103BD(arg0, data_15F90[arg1]);
      } else {
        script_10421(arg0, data_15F90[arg1], (int32_t)&local_E8);
        local_DB = local_DB - 1;
      }
      local_B4 = Script_scenCharaGetMotAnimId(arg0);
      local_B0 = Script_GetCharaMotionRemainingFramesScaled(arg0);
      (void)((local_AE = 0) & 1);
      local_D0 = local_B4;
      switch (local_D0) {
        case 102:
          if ((!(local_B0 >= 0.1f) & !(local_B0 <= -0.1f)) | (!(local_B0 >= 18.1f) & !(local_B0 <= 17.9f))) {
            (void)((local_AE = -1) & 1);
          }
          break;
        case 234:
          if ((!(local_B0 >= 7.1f) & !(local_B0 <= 6.9f)) | (!(local_B0 >= 16.1f) & !(local_B0 <= 15.9f))) {
            (void)((local_AE = -1) & 1);
          }
          break;
      }
      if (local_AE & 0x40) {
        Script_EVT_GetPosition(arg0, -1, (Vector3 *)local_C4, 0);
        Script_EVT_GetPosition(CHARA_AKIR, -1, &local_B8, 0);
        if (!(Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, (&local_B8)->x - *local_C4, (&local_B8)->z - local_C4[2]) <= 40.0f)) {
          (void)((local_AE = 0) & 1);
        }
      }
      if (local_AE & 0x40) {
        Script_GetAuthorState_And_SetCamPos(&local_A0, &local_94);
        local_88[1] = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_94)->x - (&local_A0)->x, (&local_94)->z - (&local_A0)->z);
        local_88[0] = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, *local_C4 - (&local_B8)->x, local_C4[2] - (&local_B8)->z);
        if (local_88[1] > *(int32_t *)local_88) {
          local_88[2] = local_88[1] - *(int32_t *)local_88;
        } else {
          local_88[2] = *(int32_t *)local_88 - local_88[1];
        }
        local_AF = (arg1 - 1) * 6 + Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
        if (local_88[2] > 0x4000) {
          local_AF = local_AF + 3;
        }
        Script_Execute3DSoundCommand_Wrapper(*(&data_17124 + local_AF), 5, (const Vector3 *)local_C4, 10.0f, 40.0f, 0);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_10D1C(int32_t arg0, int32_t arg1) {
  int32_t unused_E8[6];
  int8_t local_E7;
  uint8_t unused_E4[3];
  local_E7 = 0;
  for (;;) {
    if (Script_IsPlayerAllowed(0) & -(task_1CA == 0)) {
      if (Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 23)) {
        if (Script_GetControllerInput(1) == 4) {
          script_BD87();
        }
      }
    }
    if (task_1C7 & 8) {
      if (task_1C8 == 0) {
        Script_GetCameraInputDetails(data_15F90[arg1]);
      }
    }
    if (local_E7) {
      local_E7 = local_E7 - 1;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_10D92(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  Script_ReturnTimeDate(&local_F4);
  if ((&local_F4)->hours >= 19) {
    Script_IncrementDateTimeByNum(1);
    Script_ReturnTimeDate(&local_F4);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 23) == 4) {
    (&local_F4)->hours = 23;
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_AFTER_8PM_GUARD_SHIFT)) {
    (&local_F4)->hours = 20;
  } else {
    (&local_F4)->hours = 19;
  }
  (&local_F4)->minutes = 0;
  (&local_F4)->seconds = 0;
  Script_SetNewDateTime(&local_F4);
  local_F0 = Script_shenGetStats(STATS_CASH);
  Script_SetStats(STATS_CASH, local_F0 + 180);
  Script_PlayerStatsUtils(PS_SetStatU8_1, Script_PlayerStatsUtils(PS_GetStatU8_1) + 1);
  Script_PlayerStatsUtils(PS_SetStatU8_0, Script_PlayerStatsUtils(PS_GetStatU8_0) + 1);
  if (((&local_F4)->month == 4) & ((&local_F4)->day == 15)) {
    (&local_F4)->hours = 7;
    (&local_F4)->minutes = 59;
    (&local_F4)->seconds = 0;
    Script_SetNewDateTime(&local_F4);
    Script_shenRequestGameModuleTransition(2, SHEN_NAME('JOMO'), 4);
    SCNF_WaitFrames_0000(1);
  } else {
    if ((&local_F4)->dayOfWeek == 1) {
      script_AF73();
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 189, 1);
      goto L010EB3;
    }
  }
  L010EB3:;
  Script_WeatherTaskUtil(FreeTaskData);
  Script_WeatherTaskUtil(InitTaskData);
  Script_WeatherTaskUtil(AndNotInstructionFlags, 0x784);
  Script_lightSetGlobalSceneLighting(&data_171B4);
  Script_ProcessFogFromTable(0);
  Script_cdttLoadScrUtils(CDTT_CMD_Clear, 0);
  SCNF_WaitFrames_0000(120);
  Script_cdttLoadScrUtils(CDTT_CMD_Init, 0);
  script_D497();
  task_1C4 = 0;
  (void)((task_1C9 = -1) & 0x10);
  Script_ccowUtils(arg0, CCOW_EnableAll);
  script_11A0D();
  return;
}

__attribute__((scn_string_bindings(0, data_171EC, 1, data_171F3)))
static int32_t script_10F19(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t unused_E8[2];
  int8_t local_E7;
  int8_t local_E6;
  uint8_t unused_E4[2];
  int32_t local_E0;
  Vector3i local_D4;
  script_ACDB(arg0);
  Script_scenCreateTask(script_10D1C, 2, arg0, arg1);
  Script_lightSetGlobalSceneLighting(&data_171C4);
  SCNF_WaitFrames_0000(1);
  Script_SetShouldSleepFlag(0);
  Script_SetCharaFlags(arg0, 0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  task_1C4 = 0;
  (void)((task_1CA = 0) & 0x10);
  (void)((task_1CB = 0) & 0x10);
  (void)((local_E6 = -1) & 0x10);
  if (Script_GetCurrentEntryPoint() == 11) {
    (void)((local_E6 = 0) & 0x10);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 20) != 0) {
    (void)((local_E6 = 0) & 0x10);
  }
  Script_CharHandlers(arg0, 1, 8, 16);
  Script_SetStats(STATS_PLAYER_CONTEXT_ACTION_MODE, 3);
  Script_fixoAttachEx(SHEN_NAME('LIGH'), CHARA_AKIR, 12, &data_171D4, &data_171E0);
  Script_charaDisplay(*(&data_15F78 + arg1), CDM_Show);
  SCNF_WaitFrames_0000(2);
  script_13761(arg0);
  Script_scenCreateTask(script_11305, 1, arg0);
  for (;;) {
    local_E0 = task_1C4;
    switch (local_E0) {
      case 0:
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        local_F4.x = -16.5f;
        (&local_F4)->y = 0.0f;
        (&local_F4)->z = -17.0f;
        Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
        (&local_D4)->y = 0;
        Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_D4, 0);
        Script_InitializeOsag(arg0);
        script_1032D(data_15F90[arg1], 1);
        Script_charaDisplay(*(&data_15F78 + arg1), CDM_Show);
        script_F62C();
        SCNF_WaitFrames_0000(1);
        Script_convSetCamInactiveFlag(1);
        Script_SetGameStateTo(AUTH_None);
        script_123E9(0);
        Script_SetCameraFlags(-1);
        SCNF_WaitFrames_0000(1);
        task_1C4 = 1;
        (void)((task_1C9 = 0) & 0x10);
        (void)((task_1CD = 0) & 0x10);
        script_13761(arg0);
        Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
        (void)((task_1CA = 0) & 0x10);
        (void)((task_1CB = 0) & 0x10);
        script_123E9(0);
        if (local_E6 == -1) {
          script_114C4(arg0, arg1);
          (void)((local_E6 = 0) & 0x10);
          SCNF_WaitFrames_0000(1);
        } else {
          Script_shenClearFreeState(1);
          script_1148A(task_1CA);
          script_11872(arg0);
        }
        Script_charaSetRefboardFlagTo(arg0, 1);
        script_1371D(arg0);
        script_83B8(8);
        SCNF_WaitFrames_0000(1);
        local_F0 = 0;
        break;
      case 1:
        script_1058C(6, 0);
        if ((task_1CA == 0) & (task_1C5 == 1)) {
          Script_SetCameraFlags(0);
          script_11A65(arg0, arg1);
          Script_SetCameraFlags(-1);
        }
        break;
      case 2:
        while (task_1C6 & 4) {
          SCNF_WaitFrames_0000(1);
        }
        Script_SetCameraFlags(0);
        Script_shenClearFreeState(0);
        script_11BEB(arg0);
        local_E7 = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 20);
        local_E7 = local_E7 + 1;
        Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 20, local_E7);
        Script_executeAicaCommand(2720, 2, 20);
        SCNF_WaitFrames_0000(20);
        Script_LoadSND(0, 2, "SELECT", 0);
        Script_executeAicaCommand(16296, 0, 0);
        Script_executeAicaCommand(1184, 2, 100);
        script_12037(arg0);
        script_10D92(arg0);
        Script_ccowUtils(task_1B0, CCOW_EnableAll);
        Script_LoadSND(0, 2, "ykm005.snd", 0);
        Script_shenClearFreeState(1);
        break;
    }
    if (task_1CA != task_1CB) {
      if (task_1CA & 0x40) {
        script_123E9(1);
        script_8381(8);
      } else {
        script_123E9(0);
        script_83B8(8);
      }
      script_1148A(task_1CA);
      (void)((task_1CB = task_1CA) & 2);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_11305(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int8_t local_EF;
  uint8_t unused_EC[3];
  for (;;) {
    Script_ReturnTimeDate(&local_F4);
    if ((&local_F4)->hours == 4) {
      (void)((task_1CD = -1) & 0x10);
      if (Script_ryoControl(PCCTL_QueryShouldRun) == 0) {
        while (Script_ryoControl(PCCTL_QueryShouldRun) == 0) {
          SCNF_WaitFrames_0000(1);
        }
      }
      L01134B:;
      if (task_1CD == 0) {
      } else {
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        (void)((task_1CC = 0) & 0x10);
        Script_SetGameStateTo(AUTH_None);
        Script_srchUtils(SRCH_SCRIPT_Reset);
        Script_executeAicaCommand(2720, 2, 30);
        if (Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f)) < 1) {
          local_F0 = Script_ShowUIMessageFromID("E1006A028");
        } else {
          local_F0 = Script_ShowUIMessageFromID("E1006A029");
        }
        Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        (void)((task_1CD = 0) & 1);
        Script_shenClearFreeState(0);
        Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
        script_13761(arg0);
        Script_charaDisplay(SHEN_NAME('LIGH'), CDM_Hide);
        local_EF = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 20);
        local_EF = local_EF + 1;
        Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 20, local_EF);
        while (Script_convGetIsActive((const void *)local_F0)) {
          SCNF_WaitFrames_0000(1);
        }
        script_10D92(arg0);
        Script_shenClearFreeState(1);
        goto L01147E;
        goto L01134B;
      }
    }
    L01147E:;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_1148A(int32_t arg0) {
  if (arg0) {
    Script_executeAicaCommand(81832, 0, 0);
    Script_executeAicaCommand(1184, 2, 127);
  } else {
    Script_executeAicaCommand(16296, 0, 0);
    Script_executeAicaCommand(1184, 2, 127);
  }
  return;
}

static void script_114C4(int32_t arg0, int32_t arg1) {
  int32_t unused_E8[6];
  Vector3 local_DC;
  int32_t unused_D0[3];
  int32_t local_CC;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  script_119B5();
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, arg0, &data_17214, (int32_t)&data_15BF8, 0);
  for (;;) {
    if (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, arg0, 0.06f) == -1) {
      SCNF_WaitFrames_0000(1);
      Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_SetGameStateTo(AUTH_EventCam);
  Script_seqEnterEvtCam(553, 0, AUTH_None);
  Script_SetCharaLookAtPosition(arg0, 6, &data_17228, 0);
  Script_ShowUIMessageFromID("E1006A024");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaLookAtPosition(arg0, -6, 0, 0);
  Script_seqEnterEvtCam(550, 0, AUTH_None);
  SCNF_CHAR_SCNFApplyFacingTowardPoint(arg0, &data_17228, data_15BD8, 0.0f, 0, 0);
  SCNF_WaitFrames_0000(1);
  script_1371D(arg0);
  Script_ShowUIMessageFromID("E1006A006");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  script_11A0D();
  task_1C5 = 0;
  script_13761(arg0);
  Script_seqEnterEvtCam(551, 0, AUTH_None);
  local_DC.x = -19.49f;
  (&local_DC)->y = 0.0f;
  (&local_DC)->z = -22.93f;
  SCNF_CHAR_SCNFApplyFacingTowardPoint(arg0, &local_DC, data_15BD8, 0.0f, 0, 0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_SP2_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  local_CC = 0;
  for (;;) {
    if (local_CC == 85) {
      break;
    }
    local_CC = local_CC + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(554, 0, AUTH_None);
  Script_PlayCharaMotion(arg0, 4418, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(1);
  Script_SetGameStateTo(AUTH_EventCam);
  Script_seqEnterEvtCam(552, 0, AUTH_None);
  Script_EVT_GetPosition(arg0, -1, &local_DC, 0);
  local_DC.x = (&local_DC)->x + 1.0f;
  Script_SetCharaMotionPlaybackRate(arg0, 2.5f);
  SCNF_CHAR_SCNFApplyFacingTowardPoint(arg0, &local_DC, data_15BD8, 0.0f, 0, 0);
  Script_EVT_GetPosition(arg0, -1, &local_DC, 0);
  (&local_DC)->z = (&local_DC)->z - 1.0f;
  Script_SetCharaLookAtPosition(arg0, 6, &local_DC, 0);
  local_DC.x = (&local_DC)->x - 1.0f;
  Script_FaceLookAtTargetUtils(arg0, FACE_LookAtPosition, &local_DC, 5);
  Script_ShowUIMessageFromID("E1006A014");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_shenClearFreeState(1);
  for (;;) {
    if ((Script_GetControllerInput(1) & -2049) | (task_1CD == -1)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  local_DC.x = (&local_DC)->x - 0.2f;
  Script_SetCharaMotionPlaybackRate(arg0, 1.5f);
  SCNF_CHAR_SCNFApplyFacingTowardPoint(arg0, &local_DC, data_15BD8, 0.0f, 0, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_SetCharaLookAtPosition(arg0, -6, 0, 0);
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  SCNF_WaitFrames_0000(1);
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  return;
}

static void script_11872(int32_t arg0) {
  {
    Vector3 local_F4;
    local_F4.x = -19.34f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = -22.16f;
    Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
    SCNF_WaitFrames_0000(1);
    Script_SetGameStateTo(AUTH_EventCam);
    Script_seqSetPlayRoot(&local_F4, &data_17254);
  }
  Script_seqEnterEvtCam(600, 0, AUTH_None);
  {
    Vector3 local_F4;
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    local_F4.x = (&local_F4)->x + 1.0f;
    Script_SetCharaMotionPlaybackRate(arg0, 2.5f);
    Script_EVT_SetFacingTowardPoint(arg0, 0x38000000, &local_F4, 0);
  }
  {
    Vector3 local_F4;
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    (&local_F4)->z = (&local_F4)->z - 1.0f;
    Script_SetCharaLookAtPosition(arg0, 6, &local_F4, 0);
    local_F4.x = (&local_F4)->x - 1.0f;
    Script_FaceLookAtTargetUtils(arg0, FACE_LookAtPosition, &local_F4, 5);
    for (;;) {
      if ((Script_GetControllerInput(1) & -2049) | (task_1CD == -1)) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    local_F4.x = (&local_F4)->x - 0.2f;
    Script_SetCharaMotionPlaybackRate(arg0, 1.5f);
    SCNF_CHAR_SCNFApplyFacingTowardPoint(arg0, &local_F4, data_15BD8, 0.0f, 0, 0);
  }
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_SetCharaLookAtPosition(arg0, -20, 0, 0);
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  Script_ryoControl(RYO_RestoreControl);
  return;
}

static void script_119B5(void) {
  int8_t local_FF;
  (void)((task_1CC = 0) & 0x10);
  local_FF = 5;
  while (local_FF) {
    Script_charaDisplay(*(&data_15F60 + local_FF), CDM_Hide);
    Script_charaDisplay(*(&data_15F78 + local_FF), CDM_Hide);
    Script_lightSetSlotEnabled(data_15F90[local_FF], 0);
    local_FF = local_FF - 1;
  }
  return;
}

static void script_11A0D(void) {
  int8_t local_FF;
  (void)((task_1CC = -1) & 2);
  local_FF = 5;
  while (local_FF) {
    Script_charaDisplay(*(&data_15F60 + local_FF), CDM_Show);
    Script_charaDisplay(*(&data_15F78 + local_FF), CDM_Show);
    Script_lightSetSlotEnabled(data_15F90[local_FF], 1);
    local_FF = local_FF - 1;
  }
  return;
}

static void script_11A65(int32_t arg0, int32_t arg1) {
  Vector3i local_F4;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  script_119B5();
  Script_SetGameStateTo(AUTH_EventCam);
  Script_seqEnterEvtCam(120, 0, AUTH_None);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_EVT_SetPosition(arg0, POS_XYZ, &data_17278);
  (&local_F4)->y = 20024;
  Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_F4, 0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaLookAtPosition(arg0, 6, &data_1726C, 0);
  script_10421(arg0, data_15F90[arg1], (int32_t)&data_17260);
  (void)((task_1C7 = 0) & 1);
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(30);
  Script_ShowUIMessageFromID("E1006A010");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  (void)((task_1C7 = -1) & 1);
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  Script_SetCharaLookAtPosition(arg0, -6, 0, 0);
  task_1C5 = 2;
  script_11A0D();
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  return;
}

static void script_11BEB(int32_t arg0) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3i local_D0;
  int32_t local_CC;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_FACE(CHARA_AKIR, 4, 8, 0);
  script_13761(arg0);
  Script_SetRyoMotionDetails(task_1B0);
  (void)((task_1CC = 0) & 1);
  Script_PlayCharaMotion(CHARA_KEBA, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_KEBB, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_KEBC, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_KEBD, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_KEBE, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowUtils(task_1B0, CCOW_DisableAll);
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  Script_EVT_GetPosition(task_1B0, -1, &local_E8, 0);
  (&local_D0)->y = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_F4)->x - (&local_E8)->x, (&local_F4)->z - (&local_E8)->z);
  {
    Vector3i local_C0;
    (&local_C0)->y = (&local_D0)->y;
    Script_EVT_SetRotation(task_1B0, MATRIX_XFORM_Y, &local_C0, 0);
  }
  (&local_D0)->y = (&local_D0)->y - 0x8000;
  {
    Vector3i local_C0;
    (&local_C0)->y = (&local_D0)->y;
    Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_C0, 0);
  }
  local_E8.x = -0.3f;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = -1.1f;
  Script_MoveObjectToObject(task_1B0, arg0);
  Script_EVT_SetPosition(task_1B0, POS_XYZ, &local_E8);
  if (0) {
    Script_charaDisplay(SHEN_NAME('DUMY'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('DUMY'), CDM_Show);
      goto L011DCC;
    }
  }
  L011DCC:;
  Script_scenToggleVisibleFor(SHEN_NAME('DUMY'), 1);
  Script_EVT_GetPosition(arg0, -1, &local_DC, 0);
  Script_mtwkGetSetRotation(arg0, &local_D0, 0);
  Script_EVT_SetPosition(SHEN_NAME('DUMY'), POS_XYZ, &local_DC);
  Script_EVT_SetRotation(SHEN_NAME('DUMY'), 0x38000000, &local_D0, 0);
  Script_charaToggleMotEnabled(arg0, 0);
  Script_charaToggleMotEnabled(task_1B0, 1);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_PlayCharaMotion(arg0, 4386, -1.0f, -1.0f, 20.0f, 1.0f, 0);
  Script_PlayCharaMotion(task_1B0, 4400, -1.0f, -1.0f, 20.0f, 1.0f, 0);
  Script_SetGameStateTo(AUTH_EventCam);
  if (Script_EventCamReader(133, SHEN_NAME('DUMY'), SHEN_NAME('DUMY'), SHEN_NAME('DUMY')) == 0) {
    Script_seqEnterEvtCam(133, SHEN_NAME('DUMY'), SHEN_NAME('DUMY'));
  } else if (Script_EventCamReader(132, SHEN_NAME('DUMY'), SHEN_NAME('DUMY'), SHEN_NAME('DUMY')) == 0) {
    Script_seqEnterEvtCam(132, SHEN_NAME('DUMY'), SHEN_NAME('DUMY'));
  } else if (Script_EventCamReader(131, SHEN_NAME('DUMY'), SHEN_NAME('DUMY'), SHEN_NAME('DUMY')) == 0) {
    Script_seqEnterEvtCam(131, SHEN_NAME('DUMY'), SHEN_NAME('DUMY'));
  } else if (Script_EventCamReader(130, SHEN_NAME('DUMY'), SHEN_NAME('DUMY'), SHEN_NAME('DUMY')) == 0) {
    Script_seqEnterEvtCam(130, SHEN_NAME('DUMY'), SHEN_NAME('DUMY'));
  } else {
    Script_seqEnterEvtCam(134, SHEN_NAME('DUMY'), SHEN_NAME('DUMY'));
  }
  local_CC = 0;
  for (;;) {
    int32_t local_C8;
    local_CC = local_CC + 1;
    local_C8 = local_CC;
    switch (local_C8) {
      case 5:
        script_BBE2(script_BB67(task_1B0), 4);
        break;
      case 95:
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        break;
      case 125:
        goto L011FBF;
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  L011FBF:;
  SCNF_WaitFrames_0000(10);
  Script_MoveObjectToObject(task_1B0, -1);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  Script_charaToggleMotEnabled(task_1B0, 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('DUMY'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('DUMY'), CDM_Show);
      goto L01200B;
    }
  }
  L01200B:;
  Script_scenToggleVisibleFor(SHEN_NAME('DUMY'), 0);
  Script_FACE(CHARA_AKIR, 0, 0, 0);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  return;
}

static void script_12037(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  local_EC.x = 0.0f;
  (&local_EC)->y = 0.0f;
  local_F8 = 0;
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_FACE(CHARA_AKIR, 4, 8, 0);
  script_119B5();
  Script_charaDisplay(task_1B0, CDM_Show);
  Script_charaDisplay(SHEN_NAME('LIGH'), CDM_Hide);
  Script_ryoControl(PCCTL_UpdateRyoNavYaw, arg0);
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  (&local_EC)->z = 1.0f;
  Script_EVT_SetPosition(SHEN_NAME('GATL'), -0x48000000, &local_EC);
  (&local_EC)->z = -0.7f;
  Script_EVT_SetPosition(SHEN_NAME('GATR'), -0x48000000, &local_EC);
  Script_EVT_SetPosition(arg0, POS_XYZ, &data_17290);
  {
    Vector3i local_E0;
    (&local_E0)->y = -0x4000;
    Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_E0, 0);
  }
  Script_EVT_SetPosition(task_1B0, POS_XYZ, &data_1729C);
  {
    Vector3i local_E0;
    (&local_E0)->y = -18470;
    Script_EVT_SetRotation(task_1B0, MATRIX_XFORM_Y, &local_E0, 0);
  }
  Script_ccowUtils(task_1B0, CCOW_DisableAll);
  Script_SetRyoMotionDetails(task_1B0);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_charaToggleMotEnabled(task_1B0, 1);
  Script_PlayCharaMotion(arg0, 4372, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(task_1B0, 4399, -1.0f, 246.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  local_FC = 0;
  for (;;) {
    int32_t local_E8;
    local_E8 = local_FC;
    switch (local_E8) {
      case 0:
        Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
        Script_seqEnterEvtCam(140, 0, AUTH_None);
        break;
      case 50:
        script_BBE2(script_BB67(task_1B0), 2);
        break;
      case 109:
        Script_seqEnterEvtCam(141, 0, AUTH_None);
        Script_executeAicaCommand(0x2A04A9, 0, 0);
        Script_FACE(CHARA_AKIR, 2, 0, 100);
        break;
      case 155:
        (&local_EC)->z = -1.0f;
        Script_EVT_SetPosition(SHEN_NAME('GATL'), -0x48000000, &local_EC);
        (&local_EC)->z = 0.7f;
        Script_EVT_SetPosition(SHEN_NAME('GATR'), -0x48000000, &local_EC);
        Script_seqEnterEvtCam(142, 0, AUTH_None);
        break;
      case 270:
        Script_seqEnterEvtCam(143, 0, AUTH_None);
        break;
      case 350:
        Script_ShowUIMessageFromID("E1006A026");
        break;
      case 410:
        Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
        break;
      case 470:
        goto L012313;
        break;
    }
    if (Script_IsCharaMotionFinished(task_1B0) & -(local_F8 == 0)) {
      local_F8 = Script_scenCreateTask(script_12362, 1, task_1B0);
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  L012313:;
  Script_RequestFightAnimation(task_1B0, 1139, 0.0f, 8, 1);
  Script_ccowUtils(task_1B0, CCOW_EnableAll);
  Script_srchControl((void *)local_F8);
  script_11A0D();
  (void)((task_1CC = 0) & 2);
  Script_FACE(CHARA_AKIR, 0, 0, 0);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  return;
}

static int32_t script_12362(int32_t arg0) {
  Vector3i local_F4;
  Script_charaToggleMotEnabled(arg0, 0);
  (&local_F4)->y = 0x4000;
  Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_F4, 0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, arg0, &data_172B4, (int32_t)&data_15BF8, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, arg0, 0.06f) == -1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_123E9(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, task_1BC);
      Script_SetCameraSettings(CAM_UTILS_SET_HEIGHT_ABOVE_PLAYER, task_1C0);
      break;
    case 1:
      Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, 6.0f);
      Script_SetCameraSettings(CAM_UTILS_SET_HEIGHT_ABOVE_PLAYER, 2.7f);
      break;
  }
  return;
}

static int32_t script_12432(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3 local_D0;
  int32_t unused_CC;
  float local_C4[2];
  Vector3i local_B8;
  int32_t unused_B4;
  int32_t local_AC[2];
  int32_t local_A8;
  int32_t local_A4;
  int8_t local_A3;
  int8_t local_A2;
  int8_t local_A1;
  int8_t local_A0;
  Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_172D8);
  Script_SetMaxCharaSearchExtent(0, 3.0f);
  (void)((local_A0 = 0) & 1);
  for (;;) {
    if (task_1CA == 0) {
      local_A8 = Script_srchUtils(SRCH_SCRIPT_UpdateAndPollSelection);
      local_A4 = Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex);
      if (local_A4 >= 0) {
        int32_t local_9C;
        script_119B5();
        (void)((task_1C8 = -1) & 8);
        local_DC.x = local_D0.x = *(float *)((const char *)&data_174A4 + local_A4 * 12);
        (&local_DC)->y = (&local_D0)->y = *(float *)((const char *)&data_174A4 + (local_A4 * 12 + 4));
        (&local_DC)->z = (&local_D0)->z = *(float *)((const char *)&data_174A4 + (local_A4 * 12 + 8));
        local_9C = data_17530[local_A4];
        switch (local_9C) {
          case 0:
            (&local_DC)->z = (&local_DC)->z + 2.0f;
            break;
          case 1:
            local_DC.x = (&local_DC)->x + 2.0f;
            break;
          case 2:
            (&local_DC)->z = (&local_DC)->z - 2.0f;
            break;
          case 3:
            local_DC.x = (&local_DC)->x - 2.0f;
            break;
        }
        script_104DE(data_15F90[0], &local_DC, &local_D0);
        if ((local_A8 >= 0) & (local_A1 == 0)) {
          if (task_1C5 != 2) {
            Script_ShowUIMessageFromID((const char *)Script_ScenDataUtil(&data_1747C, SCND_ReadScriptOffset, local_A8));
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
            (void)((local_A1 = -1) & 0x10);
          }
          script_129CC(local_A8);
        }
      }
    }
    if (task_1C8 == -1) {
      if (Script_GetControllerInput(1) & -2049) {
        Script_SetGameStateTo(AUTH_None);
        Script_srchUtils(SRCH_SCRIPT_Reset);
        script_11A0D();
        (void)((local_A1 = 0) & 0x10);
        (void)((task_1C8 = 0) & 0x10);
      }
    }
    if ((task_1C5 == 0) & (local_A1 != -1)) {
      Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
      Script_mtwkGetSetRotation(CHARA_AKIR, &local_B8, 0x2000000);
      Script_GetAuthorState_And_SetCamPos(&local_DC, &local_D0);
      local_AC[1] = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_D0)->x - (&local_DC)->x, (&local_D0)->z - (&local_DC)->z);
      local_A3 = 0;
      while (local_A3 < 10) {
        int32_t local_9C;
        local_A2 = 0;
        local_E8.x = *(float *)((const char *)&data_174A4 + local_A3 * 12);
        (&local_E8)->y = 0.0f;
        (&local_E8)->z = *(float *)((const char *)&data_174A4 + (local_A3 * 12 + 8));
        if (local_A3 == 7) {
          if (!(Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, (&local_F4)->x - (&local_E8)->x, (&local_F4)->z - (&local_E8)->z) >= 5.0f)) {
            local_A2 = local_A2 + 3;
          }
        } else {
          if (!(Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, (&local_F4)->x - (&local_E8)->x, (&local_F4)->z - (&local_E8)->z) >= 6.0f)) {
            local_A2 = local_A2 + 1;
          }
        }
        local_9C = data_17530[local_A3];
        switch (local_9C) {
          case 0:
            if ((local_AC[1] < 13653) | (local_AC[1] > 53703)) {
              if (((&local_B8)->y < 21845) | ((&local_B8)->y > 43690)) {
                local_A2 = local_A2 + 1;
              }
            }
            break;
          case 1:
            if ((local_AC[1] < 30037) & (local_AC[1] > 2730)) {
              if (((&local_B8)->y < 38229) | ((&local_B8)->y > 60074)) {
                local_A2 = local_A2 + 1;
              }
            }
            break;
          case 2:
            if ((local_AC[1] < 46421) & (local_AC[1] > 19114)) {
              if (((&local_B8)->y < 54613) & ((&local_B8)->y > 10922)) {
                local_A2 = local_A2 + 1;
              }
            }
            break;
          case 3:
            if ((local_AC[1] < 62805) & (local_AC[1] > 35498)) {
              if (((&local_B8)->y < 5461) | ((&local_B8)->y > 27306)) {
                local_A2 = local_A2 + 1;
              }
            }
            break;
        }
        (&local_E8)->y = *(float *)((const char *)&data_174A4 + (local_A3 * 12 + 4));
        Script_ScreenToWorldAssign(&local_E8, (void *)local_C4);
        if (-!(*local_C4 <= (float)160) & -!(*local_C4 >= (float)480) & -!(local_C4[1] <= (float)120) & -!(local_C4[1] >= (float)320)) {
          local_A2 = local_A2 + 1;
        }
        if (-(local_A2 >= 3) & -(task_1CA == 0) & -(task_1C5 == 0)) {
          if (task_1C7 == -1) {
            if (local_A3 == 7) {
              task_1C5 = 1;
              script_129CC(local_A3);
            } else {
              if (-(Script_GetControllerInput(1) == 0x400) & Script_ryoControl(PCCTL_QueryShouldRun)) {
                Script_ryoControl(PCCTL_DisablePlayerControl);
                Script_SetCameraFlags(0);
                (void)((task_1C7 = 0) & 2);
                script_119B5();
                Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
                SCNF_WaitFrames_0000(1);
                script_10421(CHARA_AKIR, data_15F90[0], (int32_t)&local_E8);
                Script_SetGameStateTo(AUTH_EventCam);
                Script_seqEnterEvtCam(*(int16_t *)((int8_t *)&data_1751C + (local_A3 << 1)), CHARA_AKIR, CHARA_AKIR);
                Script_ShowUIMessageFromID((const char *)Script_ScenDataUtil(&data_1747C, SCND_ReadScriptOffset, local_A3));
                while (Script_convGetIsActive(0)) {
                  SCNF_WaitFrames_0000(1);
                }
                Script_SetGameStateTo(AUTH_None);
                script_11A0D();
                (void)((task_1C7 = -1) & 0x10);
                Script_srchUtils(SRCH_SCRIPT_Reset);
                Script_SetCameraFlags(-1);
                Script_ryoControl(RYO_RestoreControl);
                script_129CC(local_A3);
              }
            }
          }
        }
        local_A3 = local_A3 + 1;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_129CC(int32_t arg0) {
  int16_t local_FE;
  local_FE = (Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 21) << 8) | Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 22);
  local_FE = local_FE | (1 << arg0);
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 21, (local_FE & 0xFF00) >> 8);
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 22, local_FE & 0xFF);
  return;
}

__attribute__((scn_string_bindings(0, data_1783E, 1, data_17848)))
static int32_t script_12A16(int32_t arg0) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3i local_D0;
  Vector3i local_C4;
  int32_t unused_C0;
  int8_t local_BF;
  int8_t local_BE;
  int8_t local_BD;
  int8_t local_BC;
  int8_t local_BB;
  int8_t local_BA;
  int8_t local_B9;
  local_BE = 0;
  local_BD = 50;
  (&local_E8)->y = 0.0f;
  (&local_DC)->y = 0.0f;
  (void)((local_BB = 0) & 1);
  (void)((local_BA = 0) & 1);
  Script_camSetRotationStepSize(10);
  for (;;) {
    local_BF = 0;
    (void)((local_BB = 0) & 1);
    if (local_BE) {
      local_BE = local_BE - 1;
    }
    if (Script_ryoControl(PCCTL_QueryShouldRun)) {
      while (local_BF < 60) {
        uint8_t unused_B8;
        int32_t local_B4;
        local_E8.x = *(float *)((const char *)&data_1753C + local_BF * 8);
        (&local_E8)->z = *(float *)((const char *)&data_1753C + (local_BF * 8 + 4));
        local_B4 = data_1771C[local_BF];
        switch (local_B4) {
          case 0:
            local_DC.x = (&local_E8)->x + 1.0f;
            (&local_DC)->z = (&local_E8)->z + 2.0f;
            break;
          case 1:
            local_DC.x = (&local_E8)->x - 2.0f;
            (&local_DC)->z = (&local_E8)->z - 1.0f;
            break;
          case 2:
            local_DC.x = (&local_E8)->x + 2.0f;
            (&local_DC)->z = (&local_E8)->z - 1.0f;
            break;
          case 3:
            local_DC.x = (&local_E8)->x - 1.0f;
            (&local_DC)->z = (&local_E8)->z + 2.0f;
            break;
          case 4:
            local_DC.x = (&local_E8)->x - 1.0f;
            (&local_DC)->z = (&local_E8)->z - 2.0f;
            break;
          case 5:
            local_DC.x = (&local_E8)->x + 2.0f;
            (&local_DC)->z = (&local_E8)->z + 1.0f;
            break;
          case 6:
            local_DC.x = (&local_E8)->x - 2.0f;
            (&local_DC)->z = (&local_E8)->z + 1.0f;
            break;
          case 7:
            local_DC.x = (&local_E8)->x + 1.0f;
            (&local_DC)->z = (&local_E8)->z - 2.0f;
            break;
        }
        if (Script_SetTaskDataValue(CHARA_AKIR, &local_E8, &local_DC, 0)) {
          (void)((local_BB = -1) & 1);
          if (-(Script_GetControllerInput(1) == 0x400) & Script_ryoControl(PCCTL_QueryShouldRun) & -(task_1C4 != 2)) {
            Vector3i local_A8;
            if (task_1CA & -(script_1058C(5, 0) < 2)) {
              if (local_BE == 0) {
                Script_ShowUIMessageFromID("E1006A025");
                local_BE = 30;
              }
              break;
            }
            (void)((local_B9 = task_1CA) & 0x20);
            if ((local_BB == -1) & (local_BA == -1)) {
              SCNF_RequestIconNotification(0, 0, 0, 2);
              (void)((local_BA = 0) & 0x20);
            }
            Script_ryoControl(PCCTL_DisablePlayerControl);
            Script_mtwkGetSetRotation(arg0, &local_D0, 0x42000000);
            Script_mtwkGetSetRotation(arg0, &local_C4, 0x42000000);
            (&local_A8)->y = (&local_C4)->y;
            Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_A8, 0);
            Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
            SCNF_WaitFrames_0000(1);
            local_B4 = data_1771C[local_BF];
            if ((local_B4 == 0) | (local_B4 == 5)) {
              local_E8.x = (&local_E8)->x + 0.45f;
              (&local_E8)->z = (&local_E8)->z + 0.45f;
            } else if ((local_B4 == 1) | (local_B4 == 4)) {
              local_E8.x = (&local_E8)->x - 0.45f;
              (&local_E8)->z = (&local_E8)->z - 0.45f;
            } else if ((local_B4 == 2) | (local_B4 == 7)) {
              local_E8.x = (&local_E8)->x + 0.45f;
              (&local_E8)->z = (&local_E8)->z - 0.45f;
            } else {
              if ((local_B4 == 3) | (local_B4 == 6)) {
                local_E8.x = (&local_E8)->x - 0.45f;
                (&local_E8)->z = (&local_E8)->z + 0.45f;
                goto L012DE5;
              }
            }
            L012DE5:;
            local_B4 = data_1771C[local_BF];
            if ((local_B4 == 0) | (local_B4 == 7)) {
              (&local_C4)->y = 0xC000;
            } else if ((local_B4 == 1) | (local_B4 == 2)) {
              (&local_C4)->y = 0;
            } else if ((local_B4 == 3) | (local_B4 == 4)) {
              (&local_C4)->y = 0x4000;
            } else {
              if ((local_B4 == 5) | (local_B4 == 6)) {
                (&local_C4)->y = 0x8000;
                goto L012E65;
              }
            }
            L012E65:;
            if (data_17758[local_BF] == 0) {
              Script_SetGameStateTo(AUTH_EventCam);
              Script_seqSetPlayRoot(&local_E8, &local_C4);
              Script_seqEnterEvtCam(data_1771C[local_BF] + local_BD + 300, 0, AUTH_None);
              if (local_BD) {
                local_BD = 0;
              } else {
                local_BD = 50;
              }
            } else {
              Script_SetGameStateTo(AUTH_EventCam);
              Script_seqSetPlayRoot(&local_E8, &local_C4);
              Script_seqEnterEvtCam(data_1771C[local_BF] + data_17758[local_BF] + 300, 0, AUTH_None);
            }
            Script_SubmitCharaMoveTarget(arg0, &local_E8, (&local_D0)->y, MN_AKI_AKI_STAND_LP, 2.0f);
            while ((Script_GetCharaMoveStatus(arg0) != 0) & (task_1C4 != 2)) {
              SCNF_WaitFrames_0000(1);
            }
            if (task_1C4 == 2) {
              break;
            }
            (void)((local_BC = 0) & 0x80);
            if (task_1C7 == -1) {
              (void)((local_BC = -1) & 0x80);
              script_13761(arg0);
            }
            if (~(-(task_1CA == -1) & -(local_B9 == 0))) {
              script_1058C(4, 1);
            }
            if (task_1C4 != 2) {
              Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
              local_B4 = data_1771C[local_BF];
              if ((local_B4 == 0) | (local_B4 == 7)) {
                local_F4.x = (&local_F4)->x + 1.0f;
              } else if ((local_B4 == 1) | (local_B4 == 2)) {
                (&local_F4)->z = (&local_F4)->z - 1.0f;
              } else if ((local_B4 == 3) | (local_B4 == 4)) {
                local_F4.x = (&local_F4)->x - 1.0f;
              } else {
                if ((local_B4 == 5) | (local_B4 == 6)) {
                  (&local_F4)->z = (&local_F4)->z + 1.0f;
                  goto L013012;
                }
              }
              L013012:;
              Script_SetCharaMotionPlaybackRate(arg0, 2.0f);
              SCNF_CHAR_SCNFApplyFacingTowardPoint(arg0, &local_F4, data_15BD8, 0.0f, 0, 0);
            }
            if (task_1C4 != 2) {
              Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
              local_B4 = data_1771C[local_BF];
              if ((local_B4 == 0) | (local_B4 == 3)) {
                (&local_F4)->z = (&local_F4)->z - 1.0f;
              } else if ((local_B4 == 1) | (local_B4 == 6)) {
                local_F4.x = (&local_F4)->x + 1.0f;
              } else if ((local_B4 == 2) | (local_B4 == 5)) {
                local_F4.x = (&local_F4)->x - 1.0f;
              } else {
                if ((local_B4 == 4) | (local_B4 == 7)) {
                  (&local_F4)->z = (&local_F4)->z + 1.0f;
                  goto L0130DD;
                }
              }
              L0130DD:;
              Script_SetCharaLookAtPosition(arg0, 6, &local_F4, 0);
              local_B4 = data_1771C[local_BF];
              if ((local_B4 == 0) | (local_B4 == 7)) {
                local_F4.x = (&local_F4)->x - 1.0f;
              } else if ((local_B4 == 1) | (local_B4 == 2)) {
                (&local_F4)->z = (&local_F4)->z + 1.0f;
              } else if ((local_B4 == 3) | (local_B4 == 4)) {
                local_F4.x = (&local_F4)->x + 1.0f;
              } else {
                if ((local_B4 == 5) | (local_B4 == 6)) {
                  (&local_F4)->z = (&local_F4)->z - 1.0f;
                  goto L01317E;
                }
              }
              L01317E:;
              Script_FaceLookAtTargetUtils(arg0, FACE_LookAtPosition, &local_F4, 5);
            }
            (void)((task_1C6 = -1) & 0x10);
            while ((Script_GetControllerInput(1) & -2049) == 0) {
              if (task_1C4 == 2) {
                break;
              }
              if (task_1CD == -1) {
                break;
              }
              SCNF_WaitFrames_0000(1);
            }
            Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
            Script_SetCharaLookAtPosition(arg0, -6, 0, 0);
            if (task_1C4 != 2) {
              local_A8.x = 0;
              (&local_A8)->z = 0;
              local_B4 = data_1771C[local_BF];
              if ((local_B4 == 0) | (local_B4 == 2) | (local_B4 == 4) | (local_B4 == 6)) {
                (&local_A8)->y = 0x6000;
              } else {
                if ((local_B4 == 1) | (local_B4 == 3) | (local_B4 == 5) | (local_B4 == 7)) {
                  (&local_A8)->y = 0xA000;
                  goto L013259;
                }
              }
              L013259:;
              Script_EVT_SetRotation(arg0, -0x6C000000, &local_A8, (int32_t)data_15BD8);
              Script_SetCharaMotionPlaybackRate(arg0, 1.5f);
              for (;;) {
                SCNF_WaitFrames_0000(1);
                if (task_1C4 == 2) {
                  break;
                }
                if (Script_IsCharaMotionFinished(arg0)) {
                  Script_EVT_SetRotation(arg0, -0x70000000, &local_A8, 0);
                  Script_PlayCharaMotion(arg0, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
                  break;
                }
              }
            }
            if (task_1C4 != 2) {
              script_1058C(4, 0);
              if ((task_1C7 == 0) & (local_BC == -1)) {
                script_1371D(arg0);
              }
              Script_ryoControl(RYO_RestoreControl);
              Script_convSetCamInactiveFlag(0);
              Script_SetGameStateTo(AUTH_None);
            }
            (void)((task_1C6 = 0) & 1);
            break;
          }
        }
        local_BF = local_BF + 1;
      }
      local_BF = 0;
      while (local_BF < 10) {
        local_E8.x = *(float *)((const char *)&data_17794 + local_BF * 16);
        (&local_E8)->z = *(float *)((const char *)&data_17794 + (local_BF * 16 + 4));
        local_DC.x = (&local_E8)->x + *(float *)((const char *)&data_17794 + (local_BF * 16 + 8));
        (&local_DC)->z = (&local_E8)->z + *(float *)((const char *)&data_17794 + (local_BF * 16 + 12));
        if (Script_SetTaskDataValue(CHARA_AKIR, &local_E8, &local_DC, 0)) {
          (void)((local_BB = -1) & 1);
          if (-(Script_GetControllerInput(1) == 0x400) & Script_ryoControl(PCCTL_QueryShouldRun)) {
            uint8_t unused_B8;
            Vector3i local_AC;
            if (task_1CA & -(script_1058C(5, 0) < 2)) {
              if (local_BE == 0) {
                Script_ShowUIMessageFromID("E1006A025");
                local_BE = 30;
              }
              break;
            }
            if ((local_BB == -1) & (local_BA == -1)) {
              SCNF_RequestIconNotification(0, 0, 0, 2);
              (void)((local_BA = 0) & 0x20);
            }
            Script_ryoControl(PCCTL_DisablePlayerControl);
            SCNF_WaitFrames_0000(1);
            Script_mtwkGetSetRotation(arg0, &local_C4, 0x42000000);
            (&local_AC)->y = (&local_C4)->y;
            Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_AC, 0);
            Script_SetGameStateTo(AUTH_EventCam);
            Script_seqEnterEvtCam(data_17834[local_BF] + 300, arg0, arg0);
            (void)((local_BC = 0) & 0x80);
            if (task_1C7 == -1) {
              (void)((local_BC = -1) & 0x80);
              script_13761(arg0);
            }
            script_1058C(4, 1);
            Script_PlayCharaMotion(arg0, MN_AKI_AKI_SIT_TATEHIZA_ST, -1.0f, -1.0f, -1.0f, 1.0f, 0);
            Script_SetCharaMotionPlaybackRate(arg0, 2.5f);
            for (;;) {
              if (Script_IsCharaMotionFinished(arg0)) {
                break;
              }
              SCNF_WaitFrames_0000(1);
            }
            (void)((task_1C6 = -1) & 0x80);
            while ((Script_GetControllerInput(1) & -2049) == 0) {
              if (task_1CA & -(script_1058C(5, 0) <= 0)) {
                Script_ShowUIMessageFromID("E1006A015");
                break;
              }
              if (task_1C4 == 2) {
                break;
              }
              if (task_1CD == -1) {
                break;
              }
              SCNF_WaitFrames_0000(1);
            }
            script_1058C(4, 0);
            if (task_1C4 != 2) {
              Script_PlayCharaMotion(arg0, MN_AKI_AKI_SIT_TATEHIZA_EN, -1.0f, -1.0f, -1.0f, 1.0f, 0);
              Script_SetCharaMotionPlaybackRate(arg0, 3.0f);
              for (;;) {
                if (Script_IsCharaMotionFinished(arg0)) {
                  break;
                }
                SCNF_WaitFrames_0000(1);
              }
              if ((task_1C7 == 0) & (local_BC == -1)) {
                script_1371D(arg0);
              }
              Script_convSetCamInactiveFlag(0);
              Script_SetGameStateTo(AUTH_None);
              Script_ryoControl(RYO_RestoreControl);
            }
            (void)((task_1C6 = 0) & 0x20);
            break;
          }
        }
        local_BF = local_BF + 1;
      }
    }
    if (local_BB == -1) {
      if (local_BA == 0) {
        SCNF_RequestIconNotification(0x100, 0, 1, 1);
      }
    } else {
      if (local_BA == -1) {
        SCNF_RequestIconNotification(0, 0, 0, 2);
      }
    }
    (void)((local_BA = local_BB) & 2);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_13627(void) {
  int32_t local_FC;
  for (;;) {
    if ((&task_10)->mode) {
      local_FC = task_10.entry_idx;
      if (local_FC == 8) {
        script_119B5();
        script_13761(CHARA_AKIR);
        Script_SetCameraFlags(0);
        SCNF_WaitFrames_0000(90);
        Script_executeAicaCommand(2720, 1, 30);
        Script_executeAicaCommand(2720, 2, 30);
        Script_executeAicaCommand(2720, 3, 30);
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MKSG_MS08_TRANSITION_TRIGGERED, 1);
        Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
        if (Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 23) >= 2) {
          Script_GetCassetteFlags(50, 1, 0, 0);
        } else {
          if (Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 23)) {
            Script_GetCassetteFlags(49, 1, 0, 0);
            goto L013701;
          }
        }
        L013701:;
        Script_shenRequestGameModuleTransition(2, SHEN_NAME('MS08'), 0);
        goto L013711;
      }
    }
    L013711:;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_1371D(int32_t arg0) {
  script_1058C(3, 1);
  Script_RequestFightAnimation(arg0, 1139, 0.0f, 8, 1);
  SCNF_WaitFrames_0000(4);
  Script_SetStats(STATS_PLAYER_CONTEXT_ACTION_MODE, 3);
  (void)((task_1C7 = -1) & 4);
  Script_lightSetSlotEnabled(data_15F90[0], 1);
  return;
}

static void script_13761(int32_t arg0) {
  (void)((task_1C7 = 0) & 1);
  Script_lightSetSlotEnabled(data_15F90[0], 0);
  script_1058C(3, 0);
  Script_SetStats(STATS_PLAYER_CONTEXT_ACTION_MODE, 0);
  Script_SetRyoMotionDetails(arg0);
  return;
}
