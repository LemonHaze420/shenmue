#include "act03_dslt.h"

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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
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
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    local_F4 = 90.0f - local_F4;
    *(int32_t *)&local_F0 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
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
      *(int32_t *)&local_F4 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
      *(int32_t *)&local_F0 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
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
        *(int32_t *)&local_F4 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
        *(int32_t *)&local_F0 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
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
      *(int32_t *)&local_FF78 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9 + 5);
      *(int32_t *)&local_FF74 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9 + 6);
      local_FF7F = script_8C20(arg0, arg1, 5, local_FF78 / (float)100, local_FF74 / (float)100, arg3, -1);
      script_92E9(arg0, 0);
      *(int32_t *)&local_94 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9 + 1);
      *(int32_t *)&local_90 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9 + 2);
      local_8C = 0.0f;
      local_FC = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9);
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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    local_F8 = 98.0f - local_F8;
    local_F4 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
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
      *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_17FB0, SCND_ReadFloatBits, Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 6) * 5 + 3);
      *(int32_t *)&local_CC = Script_ScenDataUtil(&data_17FB0, SCND_ReadFloatBits, Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 6) * 5 + 4);
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
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_17FB0, SCND_ReadFloatBits, Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_17FB0, SCND_ReadFloatBits, Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_8C20(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_48[arg2]);
          Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
          script_6BDE(arg0, arg1, arg2, arg3, local_FC, 0);
          Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
        } else {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_17FB0, SCND_ReadFloatBits, Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_17FB0, SCND_ReadFloatBits, Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 6) * 5 + 4);
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

__attribute__((scn_string_bindings(0, data_18ACE)))
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
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_8C = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_88 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 7);
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
      local_FF78 = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
      local_FF7C = 18;
      local_80 = 18;
      local_A4 = 1.0f;
    } else {
      if (arg0 == task_160) {
        local_FF78 = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
        local_FF7C = 12;
        local_80 = 12;
      } else {
        local_FF78 = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
          *(int32_t *)&local_9C = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 8);
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

__attribute__((scn_string_bindings(0, data_18AEA)))
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
          local_FC = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
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
          local_FC = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
      }
      break;
  }
  *(int32_t *)&local_C4 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_C0 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 2);
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

__attribute__((scn_string_bindings(0, data_18B06)))
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
            local_A4 = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = 1.0f;
            local_A0 = 12;
            local_9C = 18;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = -1.0f;
            local_A0 = 12;
            local_9C = 12;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 51882;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 12;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
              local_A4 = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 12;
              local_90 = 48241;
              local_8C = 50062;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
              local_C0 = -1.0f;
              local_A0 = 12;
              local_9C = 12;
              local_90 = 50062;
              local_8C = 48241;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 15473;
              local_8C = 17294;
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
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
              local_A4 = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 45511;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 51882;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
              local_A4 = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
  *(int32_t *)&local_B8 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_B4 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_B0 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_AC = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_A8 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 7);
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
          *(int32_t *)&local_BC = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 8);
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

__attribute__((scn_string_bindings(0, data_18B22)))
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

__attribute__((scn_string_bindings(0, data_18B3E)))
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
      local_8C = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
      local_A0 = -1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 46421;
      local_FF74 = 48787;
      break;
    case 1:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = -1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 50426;
      local_FF74 = 48969;
      break;
    case 2:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
      local_A0 = 1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 15109;
      local_FF74 = 16566;
      break;
    case 3:
      local_AF = 0;
      local_8C = Script_ScenDataUtil(data_17590, SCND_ReadU32, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = 1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 19114;
      local_FF74 = 16748;
      break;
  }
  (void)local_FF78;
  *(int32_t *)&local_9C = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_17590, SCND_ReadFloatBits, Script_ScenDataUtil(data_16FA0, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 4);
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

__attribute__((scn_string_bindings(0, data_18B5A)))
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
        local_E0 = Script_ScenDataUtil(&data_17FB0, SCND_ReadU32, local_D4 * 5) | -0x80000000;
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_17FB0, SCND_ReadU32, local_D4 * 5);
        break;
    }
  } else if ((local_CC == 2) | (local_CC == 4) | (local_CC == 6) | (local_CC == 8) | (local_CC == 10) | (local_CC == 19) | (local_CC == 22) | (local_CC == 24) | (local_CC == 26) | (local_CC == 28)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_17FB0, SCND_ReadU32, local_D4 * 5);
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_17FB0, SCND_ReadU32, local_D4 * 5) | -0x80000000;
        break;
    }
  } else {
    if ((local_CC == 13) | (local_CC == 14) | (local_CC == 15) | (local_CC == 16) | (local_CC == 20) | (local_CC == 23) | (local_CC == 29)) {
      local_CC = arg4;
      switch (local_CC) {
        case 0:
          local_E0 = Script_ScenDataUtil(&data_17FB0, SCND_ReadU32, local_D4 * 5);
          break;
        case 1:
          local_E0 = Script_ScenDataUtil(&data_17FB0, SCND_ReadU32, local_D4 * 5) | -0x80000000;
          break;
        case 2:
          local_E0 = Script_ScenDataUtil(&data_17FB0, SCND_ReadU32, local_D4 * 5);
          break;
        case 3:
          local_E0 = Script_ScenDataUtil(&data_17FB0, SCND_ReadU32, local_D4 * 5) | -0x80000000;
          break;
      }
      goto L006988;
    }
  }
  L006988:;
  *(int32_t *)&local_EC = Script_ScenDataUtil(&data_17FB0, SCND_ReadFloatBits, local_D4 * 5 + 1);
  *(int32_t *)&local_E8 = Script_ScenDataUtil(&data_17FB0, SCND_ReadFloatBits, local_D4 * 5 + 2);
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

__attribute__((scn_string_bindings(0, data_18B76)))
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
  DSWK_BindTablesAndInitState(arg1, arg2, arg3, arg4, &task_10, task_20, task_48, (shen_name *)task_70, task_98, arg0, arg5, data_16FA0, data_17590, (shen_name *)task_E8);
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

__attribute__((scn_string_bindings(0, data_18C01, 1, data_18C1A)))
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

__attribute__((scn_string_bindings(0, data_18C33)))
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

__attribute__((scn_string_bindings(0, data_18C42)))
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

__attribute__((scn_string_bindings(0, data_18C51, 1, data_18C63)))
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
      task_110[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_18050, SCND_ReadScriptOffset, local_F8), 4, task_70[arg0], task_E8[arg0], arg0, local_F8);
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
    task_110[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_18050, SCND_ReadScriptOffset, arg2), 4, task_70[arg0], task_E8[arg0], arg0, arg2);
    task_C0[arg0] = task_70[arg0];
    script_8A20(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_18C76, 1, data_18C88)))
static void script_9D79(int32_t arg0, int32_t arg1) {
  if ((task_110[arg0] == 0) & (arg1 >= 0)) {
    script_931B(task_70[arg0], task_E8[arg0], task_48[arg0], arg1);
    task_110[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_18050, SCND_ReadScriptOffset, arg1), 4, task_70[arg0], task_E8[arg0], arg0, arg1);
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

static void script_9EB2(void) {
  int32_t local_FC;
  local_FC = Script_GetSceneID();
  switch (local_FC) {
    case 1:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ENTRY2_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3044484A, 2, &data_18D0C, 0xD5A6C3, TimedTransitionUpdateMemoItem150);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_ENTRY20_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3030444A, 20, &data_18CD4, 0xDA3AA3, 0);
      }
      break;
    case 2:
      if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_STARTED) == 0) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_ADVANCED) == 0)) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 2, 0x54434D47, 1, &data_18C9C, 0xB7420F, 0);
      }
      break;
    case 3:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x4F4D4F4A, 20, &data_18CB8, 0x2DC9FE, TimedTransitionRollbackToPreviousDay2330);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIRED_FROM_JOB) & ~Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 970)) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_ENTRY42_TIMED_EVENT_DONE) == 0) {
          Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x5953464D, 42, &data_18CF0, 0xBBB817, 0);
        }
      }
      break;
  }
  return;
}

static void script_9FEA(int32_t arg0) {
  script_A002(arg0);
  return;
}

static void script_9FF6(int32_t arg0) {
  script_A002(arg0);
  return;
}

static void script_A002(int32_t arg0) {
  if (Script_StepUtils(GET_STEP_HIGHER, 0) == 22) {
    int32_t local_FC;
    local_FC = Script_StepUtils(GET_STEP_LOWER, 0);
    if ((local_FC == 2) | (local_FC == 3)) {
      Script_SetShouldSleepFlag(0);
      goto L00A03B;
    }
  }
  L00A03B:;
  Script_ryoSetChara(arg0);
  Script_facemotUtils(arg0, 1, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, FACE_ExpressionDisabled, 0, 2);
  Script_CharHandlers(arg0, -1, 0, 0);
  script_9EB2();
  Script_cpctStateUtils(1, 2);
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

static int32_t script_A0FE(int32_t arg0) {
  script_9FF6(arg0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_A115(int32_t arg0) {
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

static void script_A156(int32_t arg0) {
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

static void script_A19A(int32_t arg0) {
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

static void script_A218(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_28_REACHED)) {
    script_A19A(40);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_1E_REACHED)) {
    script_A19A(30);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_14_REACHED)) {
    script_A19A(20);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CATA1_NBO_BRANCH_CHOSEN)) {
    script_A19A(10);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP05_SA1116_DOJO_DONE)) {
    script_A19A(5);
  } else {
    script_A19A(0);
  }
  return;
}

static void script_A297(int32_t arg0) {
  int32_t local_FC;
  if (arg0 != -1) {
    task_220 = arg0;
  }
  local_FC = task_220;
  if (local_FC == 0) {
    Script_gachaponUtils(0, data_18EC0);
  } else if (local_FC == 1) {
    Script_gachaponUtils(0, data_18F69);
  } else if (local_FC == 2) {
    Script_gachaponUtils(0, data_190A4);
  } else if (local_FC == 3) {
    Script_gachaponUtils(0, data_191D9);
  } else if (local_FC == 4) {
    Script_gachaponUtils(0, data_19280);
  } else if (local_FC == 5) {
    Script_gachaponUtils(0, data_193ED);
  } else if (local_FC == 6) {
    Script_gachaponUtils(0, data_195E6);
  } else if (local_FC == 7) {
    Script_gachaponUtils(0, data_196B9);
  } else if (local_FC == 8) {
    Script_gachaponUtils(0, data_1972A);
  } else if (local_FC == 9) {
    Script_gachaponUtils(0, data_19777);
  } else if (local_FC == 10) {
    Script_gachaponUtils(0, data_197B0);
  } else if (local_FC == 11) {
    Script_gachaponUtils(0, data_1989F);
  } else if (local_FC == 12) {
    Script_gachaponUtils(0, data_19902);
  } else if (local_FC == 13) {
    Script_gachaponUtils(0, data_19957);
  } else if (local_FC == 14) {
    Script_gachaponUtils(0, data_1999E);
  } else if (local_FC == 15) {
    Script_gachaponUtils(0, data_199D7);
  } else {
    task_220 = -1;
  }
  return;
}

static void script_A438(int8_t arg0, int8_t arg1) {
  int16_t local_FE;
  int8_t local_FD;
  int32_t local_F8;
  local_FE = -1;
  if (arg0 == -1) {
    local_FD = task_220;
  } else {
    local_FD = arg0;
  }
  local_F8 = local_FD;
  switch (local_F8) {
    case 0:
      local_FE = *(int16_t *)((int8_t *)&data_18D64 + (arg1 << 1));
      break;
    case 1:
      local_FE = *(int16_t *)((int8_t *)&data_18D7C + (arg1 << 1));
      break;
    case 2:
      local_FE = *(int16_t *)((int8_t *)&data_18DA0 + (arg1 << 1));
      break;
    case 3:
      local_FE = *(int16_t *)((int8_t *)&data_18DCC + (arg1 << 1));
      break;
    case 4:
      local_FE = *(int16_t *)((int8_t *)&data_18DE0 + (arg1 << 1));
      break;
    case 5:
      local_FE = *(int16_t *)((int8_t *)&data_18E08 + (arg1 << 1));
      break;
    case 6:
      local_FE = *(int16_t *)((int8_t *)&data_18E2C + (arg1 << 1));
      break;
    case 7:
      local_FE = *(int16_t *)((int8_t *)&data_18E44 + (arg1 << 1));
      break;
    case 8:
      local_FE = *(int16_t *)((int8_t *)&data_18E54 + (arg1 << 1));
      break;
    case 9:
      local_FE = *(int16_t *)((int8_t *)&data_18E5C + (arg1 << 1));
      break;
    case 10:
      local_FE = *(int16_t *)((int8_t *)&data_18E64 + (arg1 << 1));
      break;
    case 11:
      local_FE = *(int16_t *)((int8_t *)&data_18E88 + (arg1 << 1));
      break;
    case 12:
      local_FE = *(int16_t *)((int8_t *)&data_18E98 + (arg1 << 1));
      break;
    case 13:
      local_FE = *(int16_t *)((int8_t *)&data_18EA4 + (arg1 << 1));
      break;
    case 14:
      local_FE = *(int16_t *)((int8_t *)&data_18EB0 + (arg1 << 1));
      break;
    case 15:
      local_FE = *(int16_t *)((int8_t *)&data_18EB8 + (arg1 << 1));
      break;
  }
  (void)local_FE;
  return;
}

static void script_A623(int32_t arg0, int32_t arg1) {
  for (;;) {
    *(int32_t *)arg0 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 16);
    if (data_18D44[*(int32_t *)arg0] - data_18D54[*(int32_t *)arg0]) {
      break;
    }
  }
  *(int32_t *)arg1 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_18D44[*(int32_t *)arg0] - data_18D54[*(int32_t *)arg0]);
  return;
}

static void script_A672(int32_t arg0, int32_t arg1) {
  if (arg0 == 0) {
    (void)0;
  } else if (arg0 == 14) {
    (void)0;
  } else if (arg1 < data_18D44[arg0] - data_18D54[arg0]) {
    (void)1;
  } else if (arg0 == 15) {
    (void)3;
  } else {
    (void)2;
  }
  return;
}

static void script_A6C5(int32_t arg0, int32_t arg1, int32_t arg2, float arg3) {
  Vector3 local_F4;
  int8_t local_F3;
  int8_t local_F2;
  int32_t local_EC;
  (void)((local_F3 = 0) & 1);
  if (!(arg3 >= 0.0f)) {
    (void)((local_F2 = -1) & 1);
    arg3 = arg3 * -1.0f;
  } else {
    (void)((local_F2 = 0) & 1);
  }
  local_EC = arg1;
  switch (local_EC) {
    case 1:
      local_EC = arg2;
      if ((local_EC == 0) | (local_EC == 1) | (local_EC == 12) | (local_EC == 13)) {
        (void)((local_F3 = -1) & 1);
        goto L00A736;
      }
      L00A736:;
      break;
    case 3:
      local_EC = arg2;
      if (local_EC == 6) {
        (void)((local_F3 = -1) & 1);
        goto L00A75D;
      }
      L00A75D:;
      break;
    case 4:
      local_EC = arg2;
      if ((local_EC == 7) | (local_EC == 15) | (local_EC == 16) | (local_EC == 17) | (local_EC == 18)) {
        (void)((local_F3 = -1) & 1);
        goto L00A79C;
      }
      L00A79C:;
      break;
    case 5:
      (void)((local_F3 = -1) & 1);
      break;
    case 6:
      local_EC = arg2;
      if ((local_EC == 0) | (local_EC == 1) | (local_EC == 2) | (local_EC == 3)) {
        (void)((local_F3 = -1) & 1);
        goto L00A7EA;
      }
      L00A7EA:;
      break;
    case 8:
      local_EC = arg2;
      if (local_EC == 3) {
        (void)((local_F3 = -1) & 1);
        goto L00A811;
      }
      L00A811:;
      break;
  }
  local_F4.x = (&local_F4)->y = (&local_F4)->z = 1.0f;
  if (local_F3 & 8) {
    arg2 = arg2 + data_18D44[arg1];
    local_F4.x = (&local_F4)->x * arg3;
    (&local_F4)->y = (&local_F4)->y * arg3;
    (&local_F4)->z = (&local_F4)->z * arg3;
  } else {
    local_F4.x = (&local_F4)->x * arg3 * 0.33333f;
    (&local_F4)->y = (&local_F4)->y * arg3 * 0.33333f;
    (&local_F4)->z = (&local_F4)->z * arg3 * 0.33333f;
  }
  if (local_F2 & 4) {
    script_A297(arg1);
    Script_gachaponUtils(5, arg0, arg2);
  } else {
    script_A297(arg1);
    Script_gachaponUtils(1, arg0, arg2);
  }
  if ((((scn3_bits){.f = (&local_F4)->x}).i != 0x3F800000) & (((scn3_bits){.f = (&local_F4)->x}).i != 0)) {
    Script_hmdlApplyScale(arg0, &local_F4);
  }
  return;
}

__attribute__((scn_string_bindings(1, data_19A1E)))
static void script_A8FC(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Script_sprintf(" START SLOT \012");
  Script_EVT_GetPosition(arg0, -1, &task_278, 0);
  Script_mtwkGetSetRotation(arg0, &task_284, 0);
  task_26C = arg1;
  task_26E = 0;
  task_270 = 0;
  Script_charaDisplay(arg0, CDM_Hide);
  Script_ccowUtils(arg0, CCOW_DisableAll);
  script_B8E1();
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_charaDisplay(arg0, CDM_Show);
  local_F4.x = 20.0f;
  (&local_F4)->y = 20.0f;
  (&local_F4)->z = 0.0f;
  Script_EVT_SetPosition(SHEN_NAME('SLTM'), POS_XYZ, &local_F4);
  Script_sprintf("monay : %5d \012", Script_PlayerStatsUtils(PS_GetSlotMachineCashInTotal));
  Script_sprintf("doram : %5d \012", Script_PlayerStatsUtils(PS_GetSlotMachinePlayCountSlot, task_1A2));
  Script_sprintf("bet   : %5d \012", Script_PlayerStatsUtils(PS_GetSlotMachineTotalBetSlot, task_1A2));
  Script_sprintf("win   : %5d \012", Script_PlayerStatsUtils(PS_GetSlotMachineTotalWinSlot, task_1A2));
  Script_sprintf("time  : %5d \012", Script_PlayerStatsUtils(PS_GetSlotMachinePlayFramesSlot, task_1A2));
  Script_sprintf(" END SLOT \012");
  return;
}

static int32_t script_A9F2(int32_t arg0) {
  int16_t local_FE;
  int32_t local_F8;
  int32_t local_F4;
  local_F4 = arg0;
  if (local_F4 == 0) {
    task_23C = 100;
    *(&task_23C + 1) = 0;
    *(&task_23C + 2) = 0;
    *(&task_23C + 3) = 0;
    *(&task_23C + 4) = 0;
    *(&task_23C + 5) = 0;
  } else if (local_F4 == 1) {
    task_23C = 100;
    *(&task_23C + 1) = 57;
    *(&task_23C + 2) = 14;
    *(&task_23C + 3) = 4;
    *(&task_23C + 4) = 0;
    *(&task_23C + 5) = 0;
  } else if (local_F4 == 2) {
    task_23C = 100;
    *(&task_23C + 1) = 60;
    *(&task_23C + 2) = 20;
    *(&task_23C + 3) = 10;
    *(&task_23C + 4) = 2;
    *(&task_23C + 5) = 1;
  } else if (local_F4 == 3) {
    task_23C = 100;
    *(&task_23C + 1) = 63;
    *(&task_23C + 2) = 26;
    *(&task_23C + 3) = 16;
    *(&task_23C + 4) = 4;
    *(&task_23C + 5) = 2;
  } else if (local_F4 == 4) {
    task_23C = 100;
    *(&task_23C + 1) = 67;
    *(&task_23C + 2) = 34;
    *(&task_23C + 3) = 24;
    *(&task_23C + 4) = 6;
    *(&task_23C + 5) = 3;
  } else if (local_F4 == 5) {
    task_23C = 100;
    *(&task_23C + 1) = 70;
    *(&task_23C + 2) = 40;
    *(&task_23C + 3) = 30;
    *(&task_23C + 4) = 8;
    *(&task_23C + 5) = 4;
  } else if (local_F4 == 6) {
    task_23C = 100;
    *(&task_23C + 1) = 73;
    *(&task_23C + 2) = 46;
    *(&task_23C + 3) = 36;
    *(&task_23C + 4) = 10;
    *(&task_23C + 5) = 5;
  } else if (local_F4 == 7) {
    task_23C = 100;
    *(&task_23C + 1) = 76;
    *(&task_23C + 2) = 52;
    *(&task_23C + 3) = 42;
    *(&task_23C + 4) = 12;
    *(&task_23C + 5) = 6;
  } else if (local_F4 == 8) {
    task_23C = 100;
    *(&task_23C + 1) = 76;
    *(&task_23C + 2) = 52;
    *(&task_23C + 3) = 42;
    *(&task_23C + 4) = 16;
    *(&task_23C + 5) = 8;
  } else if (local_F4 == 9) {
    task_23C = 100;
    *(&task_23C + 1) = 79;
    *(&task_23C + 2) = 58;
    *(&task_23C + 3) = 48;
    *(&task_23C + 4) = 18;
    *(&task_23C + 5) = 9;
  } else if (local_F4 == 10) {
    task_23C = 100;
    *(&task_23C + 1) = 82;
    *(&task_23C + 2) = 64;
    *(&task_23C + 3) = 54;
    *(&task_23C + 4) = 24;
    *(&task_23C + 5) = 12;
  } else if (local_F4 == 11) {
    task_23C = 100;
    *(&task_23C + 1) = 86;
    *(&task_23C + 2) = 72;
    *(&task_23C + 3) = 62;
    *(&task_23C + 4) = 30;
    *(&task_23C + 5) = 15;
  } else if (local_F4 == 12) {
    task_23C = 100;
    *(&task_23C + 1) = 89;
    *(&task_23C + 2) = 78;
    *(&task_23C + 3) = 68;
    *(&task_23C + 4) = 36;
    *(&task_23C + 5) = 18;
  } else if (local_F4 == 13) {
    task_23C = 100;
    *(&task_23C + 1) = 92;
    *(&task_23C + 2) = 84;
    *(&task_23C + 3) = 74;
    *(&task_23C + 4) = 42;
    *(&task_23C + 5) = 21;
  } else if (local_F4 == 14) {
    task_23C = 100;
    *(&task_23C + 1) = 95;
    *(&task_23C + 2) = 90;
    *(&task_23C + 3) = 80;
    *(&task_23C + 4) = 48;
    *(&task_23C + 5) = 24;
  } else if (local_F4 == 15) {
    task_23C = 100;
    *(&task_23C + 1) = 98;
    *(&task_23C + 2) = 96;
    *(&task_23C + 3) = 86;
    *(&task_23C + 4) = 54;
    *(&task_23C + 5) = 27;
  } else if (local_F4 == 16) {
    task_23C = 100;
    *(&task_23C + 1) = 99;
    *(&task_23C + 2) = 98;
    *(&task_23C + 3) = 88;
    *(&task_23C + 4) = 62;
    *(&task_23C + 5) = 31;
  } else {
    task_23C = 0;
    *(&task_23C + 1) = 0;
    *(&task_23C + 2) = 100;
    *(&task_23C + 3) = 0;
    *(&task_23C + 4) = 0;
    *(&task_23C + 5) = 0;
  }
  SCNF_WaitFrames_0000(1);
  task_224 = 1;
  Script_sprintf("level: %d \012", arg0);
  task_272 = 0;
  if (arg0 == 7) {
    local_FE = 0;
    while (local_FE < 60) {
      local_F8 = Script_GetControllerInput(1);
      if ((task_272 == 10) & (local_F8 == 0x400)) {
        task_272 = 18;
      }
      if ((task_272 == 9) & (local_F8 == 0x1000)) {
        task_272 = 10;
      }
      if ((task_272 == 8) & (local_F8 == 0x1000)) {
        task_272 = 9;
      }
      if ((task_272 == 7) & (local_F8 == 0x8000)) {
        task_272 = 8;
      }
      if ((task_272 == 6) & (local_F8 == 0x4000)) {
        task_272 = 7;
      }
      if ((task_272 == 5) & (local_F8 == 0x8000)) {
        task_272 = 6;
      }
      if ((task_272 == 4) & (local_F8 == 0x4000)) {
        task_272 = 5;
      }
      if ((task_272 == 3) & (local_F8 == 0x8000)) {
        task_272 = 4;
      }
      if ((task_272 == 2) & (local_F8 == 0x4000)) {
        task_272 = 3;
      }
      if ((task_272 == 1) & (local_F8 == 0x8000)) {
        task_272 = 2;
      }
      if ((task_272 == 0) & (local_F8 == 0x4000)) {
        task_272 = 1;
      }
      local_FE = local_FE + 1;
      SCNF_WaitFrames_0000(1);
    }
    Script_sprintf("KO: %d \012", task_272);
  }
  return Script_srchControl(0);
}

static int32_t script_B043(void) {
  {
    Vector3 local_F4;
    local_F4.x = -1.0f;
    (&local_F4)->y = 20.0f;
    (&local_F4)->z = -6.9f;
    Script_EVT_SetPosition(SHEN_NAME('SLTM'), POS_XYZ, &local_F4);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    local_E8.x = 0;
    (&local_E8)->y = 0;
    (&local_E8)->z = 0;
    Script_EVT_SetRotation(SHEN_NAME('SLTM'), 0x38000000, &local_E8, 0);
  }
  Script_MoveObjectToObject(SHEN_NAME('DRAM'), 0x4D544C53);
  {
    Vector3 local_F4;
    local_F4.x = 0.0f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 0.0f;
    Script_EVT_SetPosition(SHEN_NAME('DRAM'), POS_XYZ, &local_F4);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    local_E8.x = 0;
    (&local_E8)->y = 0;
    (&local_E8)->z = 0;
    Script_EVT_SetRotation(SHEN_NAME('DRAM'), 0x38000000, &local_E8, 0);
  }
  Script_MoveObjectToObject(SHEN_NAME('RMP1'), 0x4D544C53);
  {
    Vector3 local_F4;
    local_F4.x = 0.3f;
    (&local_F4)->y = 1.56f;
    (&local_F4)->z = 0.135f;
    Script_EVT_SetPosition(SHEN_NAME('RMP1'), POS_XYZ, &local_F4);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    local_E8.x = 0;
    (&local_E8)->y = 25486;
    (&local_E8)->z = 0;
    Script_EVT_SetRotation(SHEN_NAME('RMP1'), 0x38000000, &local_E8, 0);
  }
  Script_MoveObjectToObject(SHEN_NAME('RMP2'), 0x4D544C53);
  {
    Vector3 local_F4;
    local_F4.x = 0.3f;
    (&local_F4)->y = 1.64f;
    (&local_F4)->z = 0.135f;
    Script_EVT_SetPosition(SHEN_NAME('RMP2'), POS_XYZ, &local_F4);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    local_E8.x = 0;
    (&local_E8)->y = 0xC000;
    (&local_E8)->z = 0;
    Script_EVT_SetRotation(SHEN_NAME('RMP2'), 0x38000000, &local_E8, 0);
  }
  Script_MoveObjectToObject(SHEN_NAME('RMP3'), 0x4D544C53);
  {
    Vector3 local_F4;
    local_F4.x = 0.3f;
    (&local_F4)->y = 1.72f;
    (&local_F4)->z = 0.135f;
    Script_EVT_SetPosition(SHEN_NAME('RMP3'), POS_XYZ, &local_F4);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    local_E8.x = 0;
    (&local_E8)->y = 2730;
    (&local_E8)->z = 0;
    Script_EVT_SetRotation(SHEN_NAME('RMP3'), 0x38000000, &local_E8, 0);
  }
  Script_MoveObjectToObject(SHEN_NAME('RMPY'), 0x4D544C53);
  {
    Vector3 local_F4;
    local_F4.x = 0.3f;
    (&local_F4)->y = 1.56f;
    (&local_F4)->z = 0.135f;
    Script_EVT_SetPosition(SHEN_NAME('RMPY'), POS_XYZ, &local_F4);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    local_E8.x = 0;
    (&local_E8)->y = 25486;
    (&local_E8)->z = 0;
    Script_EVT_SetRotation(SHEN_NAME('RMPY'), 0x38000000, &local_E8, 0);
  }
  Script_MoveObjectToObject(SHEN_NAME('RMPR'), 0x4D544C53);
  {
    Vector3 local_F4;
    local_F4.x = 0.3f;
    (&local_F4)->y = 1.64f;
    (&local_F4)->z = 0.135f;
    Script_EVT_SetPosition(SHEN_NAME('RMPR'), POS_XYZ, &local_F4);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    local_E8.x = 0;
    (&local_E8)->y = 0xC000;
    (&local_E8)->z = 0;
    Script_EVT_SetRotation(SHEN_NAME('RMPR'), 0x38000000, &local_E8, 0);
  }
  Script_MoveObjectToObject(SHEN_NAME('RMPG'), 0x4D544C53);
  {
    Vector3 local_F4;
    local_F4.x = 0.3f;
    (&local_F4)->y = 1.72f;
    (&local_F4)->z = 0.135f;
    Script_EVT_SetPosition(SHEN_NAME('RMPG'), POS_XYZ, &local_F4);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    local_E8.x = 0;
    (&local_E8)->y = 2730;
    (&local_E8)->z = 0;
    Script_EVT_SetRotation(SHEN_NAME('RMPG'), 0x38000000, &local_E8, 0);
  }
  Script_MoveObjectToObject(SHEN_NAME('C50D'), 0x4D544C53);
  {
    Vector3 local_F4;
    local_F4.x = 0.0f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 0.0f;
    Script_EVT_SetPosition(SHEN_NAME('C50D'), POS_XYZ, &local_F4);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    local_E8.x = 0;
    (&local_E8)->y = 0;
    (&local_E8)->z = 0;
    Script_EVT_SetRotation(SHEN_NAME('C50D'), 0x38000000, &local_E8, 0);
  }
  Script_MoveObjectToObject(SHEN_NAME('C50P'), 0x4D544C53);
  {
    Vector3 local_F4;
    local_F4.x = 0.0f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 0.0f;
    Script_EVT_SetPosition(SHEN_NAME('C50P'), POS_XYZ, &local_F4);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    local_E8.x = 0;
    (&local_E8)->y = 0;
    (&local_E8)->z = 0;
    Script_EVT_SetRotation(SHEN_NAME('C50P'), 0x38000000, &local_E8, 0);
  }
  Script_MoveObjectToObject(SHEN_NAME('C20R'), 0x4D544C53);
  {
    Vector3 local_F4;
    local_F4.x = 0.46f;
    (&local_F4)->y = 0.6f;
    (&local_F4)->z = 0.29f;
    Script_EVT_SetPosition(SHEN_NAME('C20R'), POS_XYZ, &local_F4);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    local_E8.x = 0;
    (&local_E8)->y = 0;
    (&local_E8)->z = 0;
    Script_EVT_SetRotation(SHEN_NAME('C20R'), 0x38000000, &local_E8, 0);
  }
  Script_MoveObjectToObject(SHEN_NAME('C20L'), 0x4D544C53);
  {
    Vector3 local_F4;
    local_F4.x = 0.14f;
    (&local_F4)->y = 0.6f;
    (&local_F4)->z = 0.29f;
    Script_EVT_SetPosition(SHEN_NAME('C20L'), POS_XYZ, &local_F4);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    local_E8.x = 0;
    (&local_E8)->y = 0;
    (&local_E8)->z = 0;
    Script_EVT_SetRotation(SHEN_NAME('C20L'), 0x38000000, &local_E8, 0);
  }
  Script_MoveObjectToObject(SHEN_NAME('CA0C'), 0x4D544C53);
  {
    Vector3 local_F4;
    local_F4.x = 0.3f;
    (&local_F4)->y = 0.6f;
    (&local_F4)->z = 0.29f;
    Script_EVT_SetPosition(SHEN_NAME('CA0C'), POS_XYZ, &local_F4);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    local_E8.x = 0;
    (&local_E8)->y = 0;
    (&local_E8)->z = 0;
    Script_EVT_SetRotation(SHEN_NAME('CA0C'), 0x38000000, &local_E8, 0);
  }
  Script_MoveObjectToObject(SHEN_NAME('CA0R'), 0x4D544C53);
  {
    Vector3 local_F4;
    local_F4.x = 0.46f;
    (&local_F4)->y = 0.6f;
    (&local_F4)->z = 0.29f;
    Script_EVT_SetPosition(SHEN_NAME('CA0R'), POS_XYZ, &local_F4);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    local_E8.x = 0;
    (&local_E8)->y = 0;
    (&local_E8)->z = 0;
    Script_EVT_SetRotation(SHEN_NAME('CA0R'), 0x38000000, &local_E8, 0);
  }
  Script_MoveObjectToObject(SHEN_NAME('CA0L'), 0x4D544C53);
  {
    Vector3 local_F4;
    local_F4.x = 0.14f;
    (&local_F4)->y = 0.6f;
    (&local_F4)->z = 0.29f;
    Script_EVT_SetPosition(SHEN_NAME('CA0L'), POS_XYZ, &local_F4);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    local_E8.x = 0;
    (&local_E8)->y = 0;
    (&local_E8)->z = 0;
    Script_EVT_SetRotation(SHEN_NAME('CA0L'), 0x38000000, &local_E8, 0);
  }
  SCNF_WaitFrames_0000(30);
  SCNF_WaitFrames_0000(1);
  return Script_srchControl(0);
}

static void script_B664(int32_t arg0, int32_t arg1) {
  task_290[arg0] = arg1;
  return;
}

static int32_t script_B674(int32_t arg0) {
  int32_t local_FC;
  local_FC = task_290[arg0];
  return local_FC;
}

static int32_t script_B688(int32_t arg0) {
  task_290[1] = SCNF_RequestActionSelector_000C(0x7FFF, 0xFFFF, 3);
  return Script_srchControl(0);
}

static int32_t script_B6AA(int32_t arg0) {
  script_CCA5(arg0);
  return Script_srchControl(0);
}

static int32_t script_B6BC(int32_t arg0) {
  script_C4AF(arg0);
  return Script_srchControl(0);
}

static int32_t script_B6CE(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = SCNF_UIUtils(arg0, arg1, 1, 14, 1, 0);
  {
    int32_t local_F8;
    local_F8 = local_FC & 0xF000;
    task_290[2] = local_F8;
  }
  {
    int32_t local_F8;
    local_F8 = local_FC & 0x7FF;
    task_290[3] = local_F8;
  }
  return Script_srchControl(0);
}

static void script_B718(void) {
  int16_t local_FE;
  int16_t local_FC;
  int32_t unused_F8;
  int32_t local_F4;
  int32_t unused_F0;
  GameDateTime local_E8;
  int32_t local_E4;
  local_F4 = Script_GetControllerInput(1);
  local_E4 = local_F4;
  if (local_E4 == 0x8000) {
    script_B664(4, 2);
  } else if (local_E4 == 0x4000) {
    script_B664(4, 6);
  } else if (local_E4 == 0x400) {
    script_B664(4, 9);
  } else if (local_E4 == 0x200) {
    script_B664(4, 10);
    Script_sicnUtils(1, 0, 0);
  } else if (local_E4 == 4) {
    script_B664(4, 11);
  } else {
    script_B664(4, 13);
  }
  Script_ReturnTimeDate(&local_E8);
  local_FE = 23;
  local_FC = 30;
  if (task_274 == 1) {
    local_FE = 12;
    local_FC = 30;
  }
  if (task_274 == 2) {
    local_FE = 12;
    local_FC = 0;
  }
  if (task_274 == 3) {
    local_FE = 23;
    local_FC = 59;
  }
  if ((((&local_E8)->hours >= local_FE) & ((&local_E8)->minutes >= local_FC)) | ((&local_E8)->hours <= 4)) {
    script_B664(0, 2);
  } else {
    script_B664(0, 0);
  }
  SCNF_WaitFrames_0000(1);
  return;
}

static int32_t script_B851(int32_t arg0) {
  task_290[1] = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 36, 47, 0xFFFF, 600, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL);
  return Script_srchControl(0);
}

static int32_t script_B881(int32_t arg0) {
  task_290[1] = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 37, 34, 0xFFFF, 600, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL);
  return Script_srchControl(0);
}

static int32_t script_B8B1(int32_t arg0) {
  task_290[1] = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 40, 41, 0xFFFF, 600, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL);
  return Script_srchControl(0);
}

static void script_B8E1(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int16_t local_F2;
  int16_t local_F0;
  int16_t local_EE;
  int16_t local_EC;
  int16_t local_EA;
  int32_t local_E4;
  local_FC = 0;
  local_F2 = 2;
  while (local_F2 != 0) {
    local_F2 = *(int16_t *)((int8_t *)&data_1A124 + (local_FC << 1));
    local_F0 = *(int16_t *)((int8_t *)&data_1A124 + ((local_FC + 1) << 1));
    local_EE = *(int16_t *)((int8_t *)&data_1A124 + ((local_FC + 2) << 1));
    local_EC = *(int16_t *)((int8_t *)&data_1A124 + ((local_FC + 3) << 1));
    local_EA = *(int16_t *)((int8_t *)&data_1A124 + ((local_FC + 4) << 1));
    local_E4 = local_F2;
    switch (local_E4) {
      case 0:
        goto L00BBE9;
        break;
      case 1:
        local_FC = local_F0 * 15 - 5;
        break;
      case 2:
        if (local_F0 == 0) {
          SCNF_WaitFrames_0000(1);
        } else {
          SCNF_WaitFrames_0000(local_F0);
        }
        break;
      case 3:
        task_290[1] = -1;
        Script_scenCreateTask(script_B688, 1, local_F0);
        break;
      case 4:
        task_290[3] = -1;
        Script_scenCreateTask(script_B6CE, 2, local_EE, local_EC);
        break;
      case 16:
        script_B718();
        break;
      case 6:
        task_290[local_F0] = local_EE;
        break;
      case 7:
        task_290[local_F0] = task_290[local_F0] + local_EE;
        break;
      case 8:
        task_290[local_F0] = task_290[local_F0] - local_EE;
        break;
      case 9:
        local_F8 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, (float)local_EE);
        task_290[local_F0] = local_F8;
        break;
      case 5:
        local_F8 = task_290[local_EE];
        if (local_F8 == local_EC) {
          local_FC = local_F0 * 15 - 5;
        }
        break;
      case 15:
        local_F8 = task_290[local_EE];
        local_F4 = task_290[local_EC];
        if (local_F8 == local_F4) {
          local_FC = local_F0 * 15 - 5;
        }
        break;
      case 10:
        script_BEF5(local_EE, local_F0, local_EC);
        break;
      case 11:
        script_BBEA(local_F0);
        break;
      case 14:
        script_C21C(local_F0);
        break;
      case 13:
        Script_scenCreateTask(script_B6AA, 1, local_F0);
        break;
      case 12:
        Script_scenCreateTask(script_B6BC, 1, local_F0);
        SCNF_WaitFrames_0000(local_EE);
        break;
      case 17:
        task_290[1] = -1;
        Script_scenCreateTask(script_B851, 1, local_F0);
        break;
      case 18:
        task_290[1] = -1;
        Script_scenCreateTask(script_B881, 1, local_F0);
        break;
      case 19:
        task_290[1] = -1;
        Script_scenCreateTask(script_B8B1, 1, local_F0);
        break;
    }
    local_FC = local_FC + 5;
  }
  L00BBE9:;
  return;
}

static void script_BBEA(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      break;
    case 1:
      script_CEC5();
      break;
    case 2:
      script_D081();
      break;
    case 3:
      script_D1F3();
      break;
    case 4:
      script_D44E();
      break;
    case 5:
      script_D595();
      break;
    case 6:
      Script_scenCreateTask(script_D5EC, 0);
      break;
    case 7:
      Script_scenCreateTask(script_D63E, 0);
      break;
    case 8:
      script_E2B4();
      break;
    case 9:
      script_E333();
      break;
    case 10:
      script_E15C();
      break;
    case 11:
      script_1193B();
      break;
    case 12:
      script_10CB5();
      break;
    case 13:
      Script_scenCreateTask(script_E548, 0);
      break;
    case 14:
      script_EAD5();
      break;
    case 15:
      script_EAD5();
      break;
    case 22:
      script_FB00();
      break;
    case 23:
      script_109E8();
      break;
    case 20:
      Script_scenCreateTask(script_FE30, 0);
      break;
    case 21:
      Script_scenCreateTask(script_FB3D, 0);
      break;
    case 16:
      Script_scenCreateTask(script_10AA6, 0);
      break;
    case 17:
      Script_scenCreateTask(script_105E4, 0);
      break;
    case 18:
      Script_scenCreateTask(script_1069C, 0);
      break;
    case 19:
      Script_scenCreateTask(script_10805, 0);
      break;
    case 33:
      script_F781();
      break;
    case 34:
      script_F7DC();
      break;
    case 24:
      script_F892();
      break;
    case 25:
      script_F8ED();
      break;
    case 26:
      script_F951();
      break;
    case 27:
      script_F9E3();
      break;
    case 28:
      script_10085();
      break;
    case 29:
      script_100AA();
      break;
    case 30:
      script_100CF();
      break;
    case 31:
      script_10100();
      break;
    case 32:
      script_1039F();
      break;
    case 35:
      Script_scenCreateTask(script_FA47, 0);
      break;
    case 36:
      Script_scenCreateTask(script_FA8D, 0);
      break;
    case 37:
      Script_scenCreateTask(script_FAC7, 0);
      break;
    case 38:
      script_D2F7();
      break;
    case 39:
      Script_scenCreateTask(script_D352, 0);
      break;
    case 40:
      script_120AE();
      break;
  }
  return;
}

static void script_BEF5(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t unused_DC[9];
  int32_t local_D8;
  local_D8 = arg0;
  switch (local_D8) {
    case 0:
      local_D8 = arg1;
      switch (local_D8) {
        case 0:
          break;
        case 1:
          Script_PlayCharaMotion(CHARA_AKIR, 4107, 0.0f, 95.0f, 0.0f, 1.5f, 0);
          Script_SetNewHandMotion(CHARA_AKIR, 1, 15, 0x41C80000, 0x41F00000, 0x41C80000, 0, 2.5f, 0);
          SCNF_WaitFrames_0000(45);
          Script_seqEnterEvtCam(7122, SHEN_NAME('SLTM'), SHEN_NAME('SLTM'));
          break;
        case 2:
          Script_PlayCharaMotion(CHARA_AKIR, 4107, 95.0f, 149.0f, 0.0f, 1.5f, 0);
          break;
        case 3:
          Script_PlayCharaMotion(CHARA_AKIR, 4107, 0.0f, 50.0f, 0.0f, 1.0f, 0);
          break;
        case 4:
          Script_PlayCharaMotion(CHARA_AKIR, 4117, 0.0f, 39.0f, 0.0f, 1.0f, 0);
          break;
        case 5:
          Script_PlayCharaMotion(CHARA_AKIR, 4115, 0.0f, 26.0f, 0.0f, 2.0f, 0);
          SCNF_WaitFrames_0000(5);
          Script_PlayCharaMotion(CHARA_AKIR, 4117, 39.0f, 39.0f, 39.0f, 1.0f, 0);
          break;
        case 6:
          Script_PlayCharaMotion(CHARA_AKIR, 4116, 0.0f, 91.0f, 0.0f, 2.0f, 0);
          break;
        case 7:
          Script_PlayCharaMotion(CHARA_AKIR, 4106, 35.0f, 120.0f, 35.0f, 1.5f, 0);
          break;
        case 8:
          Script_PlayCharaMotion(CHARA_AKIR, 4106, 0.0f, 35.0f, 0.0f, 2.0f, 0);
          break;
        case 9:
          Script_PlayCharaMotion(CHARA_AKIR, 4106, 80.0f, 120.0f, 80.0f, 2.0f, 0);
          Script_SetNewHandMotion(CHARA_AKIR, 1, 76, 0, 0x41200000, 0, 0, 1.0f, 0);
          break;
        case 10:
          Script_PlayCharaMotion(CHARA_AKIR, 4106, 0.0f, 120.0f, 0.0f, 2.0f, 0);
          break;
        case 11:
          Script_PlayCharaMotion(CHARA_AKIR, 4129, 0.0f, 103.0f, 0.0f, 2.0f, 0);
          SCNF_WaitFrames_0000(15);
          Script_SetNewHandMotion(CHARA_AKIR, 1, 76, 0x41700000, 0x42C80000, 0x41200000, 0, 1.0f, 0);
          SCNF_WaitFrames_0000(5);
          Script_executeAicaCommand(0x3605A9, 0, 0);
          break;
        case 12:
          Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_STAND_LP, 0.0f, 60.0f, 0.0f, 1.0f, 30);
          break;
      }
      break;
    case 1:
      local_D8 = arg1;
      switch (local_D8) {
        case 0:
          break;
        case 14:
          Script_PlayCharaMotion(CHARA_MIKM, 4132, 0.0f, 415.0f, 0.0f, 1.0f, 30);
          break;
      }
      break;
  }
  return;
}

static void script_C21C(int32_t arg0) {
  {
    int32_t local_FC;
    local_FC = arg0;
    switch (local_FC) {
      case 0:
        break;
      case 1:
        Script_seqEnterEvtCam(7100, 0, AUTH_None);
        break;
      case 2:
        Script_seqEnterEvtCam(7101, SHEN_NAME('SLTM'), SHEN_NAME('SLTM'));
        break;
      case 3:
        Script_seqEnterEvtCam(7102, SHEN_NAME('SLTM'), SHEN_NAME('SLTM'));
        break;
      case 4:
        Script_seqEnterEvtCam(7103, SHEN_NAME('SLTM'), SHEN_NAME('SLTM'));
        break;
      case 5:
        Script_seqEnterEvtCam(7104, SHEN_NAME('SLTM'), SHEN_NAME('SLTM'));
        break;
      case 6:
        Script_seqEnterEvtCam(7105, SHEN_NAME('SLTM'), SHEN_NAME('SLTM'));
        break;
      case 7:
        Script_seqEnterEvtCam(7106, SHEN_NAME('SLTM'), SHEN_NAME('SLTM'));
        break;
      case 8:
        Script_seqEnterEvtCam(7107, SHEN_NAME('SLTM'), SHEN_NAME('SLTM'));
        break;
      case 9:
        Script_seqEnterEvtCam(7108, SHEN_NAME('SLTM'), SHEN_NAME('SLTM'));
        break;
      case 10:
        Script_seqEnterEvtCam(7109, SHEN_NAME('SLTM'), SHEN_NAME('SLTM'));
        break;
      case 11:
        Script_seqEnterEvtCam(7110, SHEN_NAME('SLTM'), SHEN_NAME('SLTM'));
        break;
      case 12:
        Script_seqEnterEvtCam(7111, SHEN_NAME('SLTM'), SHEN_NAME('SLTM'));
        break;
      case 13:
        Script_seqEnterEvtCam(7112, SHEN_NAME('SLTM'), SHEN_NAME('SLTM'));
        break;
      case 14:
        Script_seqEnterEvtCam(7113, SHEN_NAME('SLTM'), SHEN_NAME('SLTM'));
        break;
      case 15:
        Script_seqEnterEvtCam(7118, SHEN_NAME('SLTM'), SHEN_NAME('SLTM'));
        break;
      case 16:
        Script_seqEnterEvtCam(7119, SHEN_NAME('SLTM'), SHEN_NAME('SLTM'));
        break;
      case 17:
        Script_seqEnterEvtCam(7120, SHEN_NAME('SLTM'), SHEN_NAME('SLTM'));
        break;
      case 18:
        Script_seqEnterEvtCam(7121, SHEN_NAME('SLTM'), SHEN_NAME('SLTM'));
        break;
      case 19:
        Script_seqEnterEvtCam(7130, SHEN_NAME('SLTM'), SHEN_NAME('SLTM'));
        break;
    }
  }
  {
    int32_t local_FC;
    local_FC = arg0;
    if (local_FC == 0) {
    } else if (local_FC == 10) {
      Script_shenClearFreeState(1);
      Script_camSetRotationStepSize(5);
      Script_SetCameraFlags(-1);
      Script_SetCamFactor(0.15f);
      Script_sicnUtils(0, 1, 1);
    } else {
      Script_SetCameraFlags(0);
      Script_SetCamFactor(0.0f);
      Script_shenClearFreeState(0);
    }
  }
  return;
}

__attribute__((scn_string_bindings(1, data_1B664, 2, data_1B66E, 3, data_1B678, 4, data_1B682, 5, data_1B68C, 6, data_1B696, 7, data_1B6A0, 8, data_1B6AA, 9, data_1B6B4, 10, data_1B6BE, 11, data_1B6C8, 12, data_1B6D2, 49, data_1B844, 50, data_1B84E, 51, data_1B858, 52, data_1B862, 53, data_1B86C, 54, data_1B876, 55, data_1B880, 56, data_1B88A, 57, data_1B894, 58, data_1B89E, 59, data_1B8A8, 60, data_1B8B2)))
static void script_C4AF(int32_t arg0) {
  int32_t local_FC;
  int16_t local_FA;
  int32_t local_F4;
  local_F4 = arg0;
  switch (local_F4) {
    case 78:
      break;
    case 0:
      local_FC = Script_shenGetStats(STATS_SEGA_COINS);
      if (local_FC == 0) {
        Script_ShowUIMessageFromID("E1024A001");
      } else {
        local_FA = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
        local_F4 = task_26C;
        switch (local_F4) {
          case 1:
            local_F4 = local_FA;
            switch (local_F4) {
              case 0:
                Script_ShowUIMessageFromID("E1076A007");
                break;
              case 1:
                Script_ShowUIMessageFromID("E1076A008");
                break;
              case 2:
                Script_ShowUIMessageFromID("E1076A009");
                break;
            }
            break;
          case 5:
            local_F4 = local_FA;
            switch (local_F4) {
              case 0:
                Script_ShowUIMessageFromID("E1076A010");
                break;
              case 1:
                Script_ShowUIMessageFromID("E1076A011");
                break;
              case 2:
                Script_ShowUIMessageFromID("E1076A012");
                break;
            }
            break;
          case 10:
            local_F4 = local_FA;
            switch (local_F4) {
              case 0:
                Script_ShowUIMessageFromID("E1076A013");
                break;
              case 1:
                Script_ShowUIMessageFromID("E1076A014");
                break;
              case 2:
                Script_ShowUIMessageFromID("E1076A015");
                break;
            }
            break;
          case 20:
            local_F4 = local_FA;
            switch (local_F4) {
              case 0:
                Script_ShowUIMessageFromID("E1076A016");
                break;
              case 1:
                Script_ShowUIMessageFromID("E1076A017");
                break;
              case 2:
                Script_ShowUIMessageFromID("E1076A018");
                break;
            }
            break;
        }
      }
      break;
    case 1:
      Script_ShowUIMessageFromID("E1024A002");
      break;
    case 2:
      Script_ShowUIMessageFromID("E1024A003");
      break;
    case 3:
      Script_ShowUIMessageFromID("E1024A004");
      break;
    case 4:
      Script_ShowUIMessageFromID("E1024A005");
      break;
    case 5:
      Script_ShowUIMessageFromID("E1024A006");
      break;
    case 6:
      Script_ShowUIMessageFromID("E1024A007");
      break;
    case 7:
      Script_ShowUIMessageFromID("E1024A008");
      break;
    case 8:
      Script_ShowUIMessageFromID("E1024A009");
      break;
    case 9:
      Script_ShowUIMessageFromID("E1024A010");
      break;
    case 10:
      Script_ShowUIMessageFromID("E1024A011");
      break;
    case 11:
      Script_ShowUIMessageFromID("E1024A012");
      break;
    case 12:
      Script_ShowUIMessageFromID("E1024A013");
      break;
    case 13:
      Script_ShowUIMessageFromID("E1024A014");
      break;
    case 14:
      Script_ShowUIMessageFromID("E1024A015");
      break;
    case 15:
      Script_ShowUIMessageFromID("E1024A016");
      break;
    case 16:
      Script_ShowUIMessageFromID("E1024A017");
      break;
    case 17:
      Script_ShowUIMessageFromID("E1024A018");
      break;
    case 18:
      Script_ShowUIMessageFromID("E1024A019");
      break;
    case 19:
      Script_ShowUIMessageFromID("E1024A020");
      break;
    case 20:
      Script_ShowUIMessageFromID("E1024A021");
      break;
    case 21:
      Script_ShowUIMessageFromID("E1024A022");
      break;
    case 22:
      Script_ShowUIMessageFromID("E1024A023");
      break;
    case 23:
      Script_ShowUIMessageFromID("E1024A024");
      break;
    case 24:
      Script_ShowUIMessageFromID("E1024A025");
      break;
    case 25:
      Script_ShowUIMessageFromID("E1024A026");
      break;
    case 26:
      Script_ShowUIMessageFromID("E1024A027");
      break;
    case 27:
      Script_ShowUIMessageFromID("E1024A028");
      break;
    case 28:
      Script_ShowUIMessageFromID("E1024A029");
      break;
    case 29:
      Script_ShowUIMessageFromID("E1024A030");
      break;
    case 30:
      Script_ShowUIMessageFromID("E1024A031");
      break;
    case 31:
      Script_ShowUIMessageFromID("E1076A001");
      break;
    case 32:
      Script_ShowUIMessageFromID("E1076A002");
      break;
    case 33:
      Script_ShowUIMessageFromID("E1076A003");
      break;
    case 34:
      Script_ShowUIMessageFromID("E1076A004");
      break;
    case 35:
      Script_ShowUIMessageFromID("E1076A005");
      break;
    case 36:
      Script_ShowUIMessageFromID("E1076A006");
      break;
    case 37:
      Script_ShowUIMessageFromID("E1076A007");
      break;
    case 38:
      Script_ShowUIMessageFromID("E1076A008");
      break;
    case 39:
      Script_ShowUIMessageFromID("E1076A009");
      break;
    case 40:
      Script_ShowUIMessageFromID("E1076A010");
      break;
    case 41:
      Script_ShowUIMessageFromID("E1076A011");
      break;
    case 42:
      Script_ShowUIMessageFromID("E1076A012");
      break;
    case 43:
      Script_ShowUIMessageFromID("E1076A013");
      break;
    case 44:
      Script_ShowUIMessageFromID("E1076A014");
      break;
    case 45:
      Script_ShowUIMessageFromID("E1076A015");
      break;
    case 46:
      Script_ShowUIMessageFromID("E1076A016");
      break;
    case 47:
      Script_ShowUIMessageFromID("E1076A017");
      break;
    case 48:
      Script_ShowUIMessageFromID("E1076A018");
      break;
    case 49:
      Script_ShowUIMessageFromID("E1076A019");
      break;
    case 50:
      Script_ShowUIMessageFromID("E1076A020");
      break;
    case 51:
      Script_ShowUIMessageFromID("E1076A021");
      break;
    case 52:
      Script_ShowUIMessageFromID("E1076A022");
      break;
    case 53:
      Script_ShowUIMessageFromID("E1076A023");
      break;
    case 54:
      Script_ShowUIMessageFromID("E1076A024");
      break;
    case 55:
      Script_ShowUIMessageFromID("E1076A025");
      break;
    case 56:
      Script_ShowUIMessageFromID("E1076A026");
      break;
    case 57:
      Script_ShowUIMessageFromID("E1076A027");
      break;
    case 58:
      Script_ShowUIMessageFromID("E1076A028");
      break;
    case 59:
      Script_ShowUIMessageFromID("E1076A029");
      break;
    case 60:
      Script_ShowUIMessageFromID("E1076A030");
      break;
    case 61:
      Script_ShowUIMessageFromID("E1076A031");
      break;
    case 62:
      Script_ShowUIMessageFromID("E1076B001");
      break;
    case 63:
      Script_ShowUIMessageFromID("E1076B002");
      break;
    case 64:
      Script_ShowUIMessageFromID("E1076B003");
      break;
    case 65:
      Script_ShowUIMessageFromID("E1076B004");
      break;
    case 66:
      Script_ShowUIMessageFromID("E1076B005");
      break;
    case 67:
      Script_ShowUIMessageFromID("E1076B006");
      break;
    case 68:
      Script_ShowUIMessageFromID("E1076B007");
      break;
    case 69:
      Script_ShowUIMessageFromID("E1076B008");
      break;
    case 70:
      Script_ShowUIMessageFromID("E1076B009");
      break;
    case 71:
      Script_ShowUIMessageFromID("E1076B010");
      break;
    case 72:
      Script_ShowUIMessageFromID("E1076B011");
      break;
    case 73:
      Script_ShowUIMessageFromID("E1076B012");
      break;
    case 74:
      Script_ShowUIMessageFromID("E1076B013");
      break;
    case 75:
      Script_ShowUIMessageFromID("E1076B014");
      break;
    case 76:
      Script_ShowUIMessageFromID("E1076B015");
      break;
    case 77:
      Script_ShowUIMessageFromID("E1076B016");
      break;
  }
  SCNF_WaitFrames_0000(1);
  return;
}

static void script_CCA5(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      break;
    case 1:
      Script_executeAicaCommand(0x2705A9, 0, 0);
      break;
    case 2:
      Script_executeAicaCommand(0x2805A9, 0, 0);
      break;
    case 3:
      Script_executeAicaCommand(1449, 0, 0);
      break;
    case 4:
      Script_executeAicaCommand(0x2905A9, 0, 0);
      break;
    case 5:
      Script_executeAicaCommand(0x2A05A9, 0, 0);
      break;
    case 6:
      Script_executeAicaCommand(0x2B05A9, 0, 0);
      SCNF_WaitFrames_0000(70);
      Script_executeAicaCommand(0x2B05A9, 0, 0);
      SCNF_WaitFrames_0000(70);
      Script_executeAicaCommand(0x2B05A9, 0, 0);
      SCNF_WaitFrames_0000(70);
      break;
    case 7:
      Script_executeAicaCommand(0x2C05A9, 0, 0);
      break;
    case 8:
      Script_executeAicaCommand(0x2D05A9, 0, 0);
      break;
    case 9:
      Script_executeAicaCommand(0x2E05A9, 0, 0);
      break;
    case 10:
      Script_executeAicaCommand(0x2F05A9, 0, 0);
      break;
    case 11:
      Script_executeAicaCommand(0x3005A9, 0, 0);
      break;
    case 12:
      Script_executeAicaCommand(0x3105A9, 0, 0);
      break;
    case 13:
      Script_executeAicaCommand(0x3205A9, 0, 0);
      break;
    case 14:
      Script_executeAicaCommand(0x3305A9, 0, 0);
      break;
    case 15:
      Script_executeAicaCommand(0x3705A9, 0, 0);
      break;
    case 16:
      Script_executeAicaCommand(0x3805A9, 0, 0);
      break;
    case 17:
      Script_executeAicaCommand(0x3405A9, 0, 0);
      break;
    case 18:
      Script_executeAicaCommand(0x3505A9, 0, 0);
      break;
    case 19:
      Script_executeAicaCommand(0x3605A9, 0, 0);
      break;
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1B9DE)))
static void script_CEC5(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  script_145F5();
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MPK00");
  task_378 = Script_LoadHandMot(3, SHEN_NAME('DSLT'), "HMOT_YKA.BIN");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_facemotUtils(SHEN_NAME('YKUR'), 1, 0);
  Script_ResetHandMotion(SHEN_NAME('YKUR'), 1, 1, 0);
  Script_facemotUtils(CHARA_MIKM, 1, 0);
  Script_ResetHandMotion(CHARA_MIKM, 1, 1, 0);
  Script_executeAicaCommand(1184, 2, 80);
  Script_executeAicaCommand(4256, 4, 110);
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_charaSetRefboardFlagTo(SHEN_NAME('SLTM'), 1);
  Script_uiUtils(0, 28, 0);
  Script_SetNewHandMotion(CHARA_AKIR, 1, 15, 0x41C80000, 0x41F00000, 0x41C80000, 0, 2.5f, 0);
  Script_SetNewHandMotion(SHEN_NAME('YKUR'), 1, 15, 0x41C80000, 0x41F00000, 0x41C80000, 0, 2.5f, 0);
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  Script_ccowUtils(SHEN_NAME('SLTM'), CCOW_DisableAll);
  Script_MoveObjectToObject(CHARA_AKIR, 0x4D544C53);
  Script_MoveObjectToObject(SHEN_NAME('YKUR'), 0x4D544C53);
  Script_EVT_SetPosition(SHEN_NAME('SLTM'), POS_XYZ, &task_278);
  Script_EVT_SetRotation(SHEN_NAME('SLTM'), 0x38000000, &task_284, 0);
  Script_ryoControl(PCCTL_UpdateRyoNavYaw, 0x52494B41);
  return;
}

static void script_D081(void) {
  int32_t unused_E8[6];
  Script_ResetHandMotion(CHARA_MIKM, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(SHEN_NAME('YKUR'), 0, 1, 0);
  Script_faceUpdateFor(CHARA_MIKM, 1, 0);
  Script_faceUpdateFor(SHEN_NAME('YKUR'), 1, 0);
  Script_FreeTaskData(task_378);
  Script_executeAicaCommand(1184, 2, 94);
  Script_executeAicaCommand(4256, 4, 127);
  Script_SetCPAMFlags(CHARA_AKIR, 1);
  Script_SetCameraSettings(5, 6, 1, 0);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  Script_SetCameraFlags(1);
  Script_scenToggleActiveFor(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(SHEN_NAME('SLTM'), 0);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_charaDisplay(SHEN_NAME('C50D'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('C50P'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('C20R'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('C20L'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CA0C'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CA0R'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CA0L'), CDM_Hide);
  Script_uiUtils(0, 0, 0);
  Script_MoveObjectToObject(CHARA_AKIR, -1);
  Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
  script_14626();
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  return;
}

static void script_D1F3(void) {
  script_B664(1, 0);
  script_B664(2, task_272);
  script_B664(3, 0);
  script_B664(4, 0);
  script_B664(5, 0);
  script_B664(14, 0);
  script_B664(29, 0);
  script_B664(28, 0);
  script_B664(35, 0);
  script_B664(36, 0);
  script_B664(37, 0);
  script_B664(38, 0);
  script_B664(39, 0);
  script_B664(19, -255);
  script_B664(20, -255);
  script_B664(21, -255);
  script_B664(25, 0);
  script_B664(26, 0);
  script_B664(27, 0);
  script_B664(9, 0);
  script_B664(10, 0);
  script_B664(8, 2);
  script_B664(6, 2);
  return;
}

static void script_D2F7(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  while (local_FC < 78) {
    script_C4AF(local_FC);
    local_F8 = 0;
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
      local_F8 = local_F8 + 1;
    }
    Script_sprintf("VOICE_NO %03d  frame %d\012", local_FC + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static int32_t script_D352(void) {
  int32_t local_FC;
  while (1) {
    Script_PrintDebugString(5, 23, "COIN:%4d", script_B674(44));
    Script_PrintDebugString(15, 23, "BET :%4d", script_B674(14));
    Script_PrintDebugString(25, 23, "WIN :%4d", script_B674(28));
    Script_PrintDebugString(5, 24, "CLDT:%4d", script_B674(12));
    local_FC = script_B674(6);
    switch (local_FC) {
      case 1:
        Script_PrintDebugString(15, 24, "HAND: PAY OUT");
        break;
      case 2:
        Script_PrintDebugString(15, 24, "HAND: BET    ");
        break;
      case 3:
        Script_PrintDebugString(15, 24, "HAND: START_B");
        break;
      case 4:
        Script_PrintDebugString(15, 24, "HAND: START_R");
        break;
    }
    if (script_B674(5) == 1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(3, data_1BA92)))
static void script_D44E(void) {
  int16_t local_FE;
  int16_t local_FC;
  int16_t local_FA;
  uint8_t unused_F8[2];
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  int32_t unused_DC[3];
  local_F4 = Script_shenGetStats(STATS_CASH);
  script_B664(43, local_F4);
  local_FE = Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_01BNK_BANK_EVENT_ACTIVE);
  local_FC = Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_D000_VENDING_LOCK_CLEARED);
  local_FA = Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_DONE);
  if (local_FA == 1) {
    local_EC = 320;
  } else {
    local_EC = 0;
  }
  if (local_F4 >= local_EC + 100) {
    script_B664(7, 1);
  } else {
    script_B664(7, 0);
  }
  if ((local_FE == 1) & (local_FC == 0)) {
    script_B664(7, 2);
  }
  local_F0 = Script_shenGetStats(STATS_SEGA_COINS);
  if (local_F0 > 0x3B9AC9FF) {
    local_F0 = 0x3B9AC9FF;
  }
  if (local_F0 < 0) {
    local_F0 = 0;
  }
  local_E8 = local_F0 / task_26C;
  task_26E = local_F0 - local_E8 * task_26C;
  Script_sprintf("new_coin: %d \012", local_E8);
  Script_sprintf("C_AMARI : %d \012", task_26E);
  script_B664(44, local_E8);
  script_B664(12, local_E8);
  if (local_E8 >= 1) {
    script_B664(18, 1);
  } else {
    script_B664(18, 0);
  }
  Script_sprintf("coin  : %9d \012", local_F0);
  Script_sprintf("monay : %5d \012", local_F4);
  return;
}

static void script_D595(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t unused_EC[3];
  local_FC = script_B674(43);
  Script_SetStats(STATS_CASH, local_FC);
  local_F8 = script_B674(44);
  local_F8 = local_F8 * task_26C + task_26E + task_270;
  Script_sprintf("coin : %d \012", local_F8);
  Script_SetStats(STATS_SEGA_COINS, local_F8);
  script_B664(12, 0);
  return;
}

static int32_t script_D5EC(void) {
  int32_t local_FC;
  local_FC = Script_PlayerStatsUtils(PS_GetSlotMachinePlayFramesSlot, task_1A2);
  while (1) {
    local_FC = local_FC + 1;
    if (script_B674(5) == 1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayerStatsUtils(PS_SetSlotMachinePlayFramesSlot, task_1A2, local_FC);
  return Script_srchControl(0);
}

static int32_t script_D63E(void) {
  int32_t local_FC;
  int16_t local_FA;
  int16_t local_F8;
  int16_t local_F6;
  uint8_t unused_F4[2];
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  Vector3 local_DC;
  Vector3 local_D0;
  Vector3 local_C4;
  Vector3 local_B8;
  Vector3 local_AC;
  Vector3 local_A0;
  Vector3 local_94;
  Vector3 local_88;
  Vector3 local_FF7C;
  Vector3 local_FF70;
  Vector3 local_FF64;
  task_344.x = 0;
  task_344.y = 0;
  task_344.z = 0;
  task_350.x = 0;
  task_350.y = 0;
  task_350.z = 0;
  task_35C.x = 0;
  task_35C.y = 0;
  task_35C.z = 0;
  Script_charaPosRotUtils(SHEN_NAME('DRAM'), 153, 0, &task_344, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('DRAM'), 154, 0, &task_350, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('DRAM'), 155, 0, &task_35C, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 13, &local_C4, 0, CHARA_PART_Get);
  local_D0.x = (&local_C4)->x;
  (&local_D0)->y = (&local_C4)->y + 0.025f;
  (&local_D0)->z = (&local_C4)->z;
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 13, &local_D0, 0, CHARA_PART_Set);
  local_DC.x = 0.0f;
  (&local_DC)->y = 0.0f;
  (&local_DC)->z = -1.5f;
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 182, &local_B8, 0, CHARA_PART_Get);
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 183, &local_AC, 0, CHARA_PART_Get);
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 184, &local_A0, 0, CHARA_PART_Get);
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 185, &local_94, 0, CHARA_PART_Get);
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 186, &local_88, 0, CHARA_PART_Get);
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 192, &local_FF7C, 0, CHARA_PART_Get);
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 193, &local_FF70, 0, CHARA_PART_Get);
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 194, &local_FF64, 0, CHARA_PART_Get);
  Script_charaSetRefboardFlagTo(SHEN_NAME('DRAM'), 0);
  Script_SetCharaRefBoardPos(SHEN_NAME('DRAM'), &data_1BAF4);
  local_F6 = 0;
  while (1) {
    local_FC = script_B674(12);
    local_FA = script_B674(14);
    local_F8 = script_B674(16);
    local_F0 = local_FC * task_26C + task_26E;
    local_EC = local_FA * task_26C;
    local_E8 = local_F8 * task_26C;
    Script_GameUtils(SHEN_NAME('SLTM'), GC_CommitStagedEntries, 0, 0, 0, 0);
    Script_GameUtils(SHEN_NAME('SLTM'), GC_ConfigureNumericDisplay, 0x100, &data_1BAAC, 0x3B9AC9FF, 0);
    Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNumericDisplayValue, local_F0, &data_1BAB8, 1.0f, 0);
    Script_GameUtils(SHEN_NAME('SLTM'), GC_ConfigureNumericDisplay, 0x100, &data_1BAAC, 999999, 0);
    Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNumericDisplayValue, local_E8, &data_1BAE8, 1.0f, 0);
    {
      int32_t local_FF60;
      local_FF60 = local_FA;
      switch (local_FF60) {
        case 0:
          script_B664(42, 0);
          script_B664(41, 1);
          script_B664(40, 1);
          break;
        case 1:
          script_B664(40, 0);
          script_B664(42, 1);
          script_B664(35, 1);
          break;
        case 2:
          script_B664(36, 1);
          break;
        case 3:
          script_B664(37, 1);
          break;
        case 4:
          script_B664(38, 1);
          break;
        case 5:
          script_B664(39, 1);
          script_B664(41, 0);
          break;
      }
    }
    {
      int32_t local_FF60;
      local_FF60 = local_EC;
      switch (local_FF60) {
        case 0:
          Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 292, &data_1BAC4, 1.0f, 0);
          break;
        case 1:
          Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 293, &data_1BAC4, 1.0f, 0);
          break;
        case 2:
          Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 294, &data_1BAC4, 1.0f, 0);
          break;
        case 3:
          Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 295, &data_1BAC4, 1.0f, 0);
          break;
        case 4:
          Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 296, &data_1BAC4, 1.0f, 0);
          break;
        case 5:
          Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 297, &data_1BAC4, 1.0f, 0);
          break;
        case 10:
          Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 293, &data_1BAD0, 1.0f, 0);
          Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 292, &data_1BAC4, 1.0f, 0);
          break;
        case 15:
          Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 293, &data_1BAD0, 1.0f, 0);
          Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 297, &data_1BAC4, 1.0f, 0);
          break;
        case 20:
          Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 294, &data_1BAD0, 1.0f, 0);
          Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 292, &data_1BAC4, 1.0f, 0);
          break;
        case 25:
          Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 294, &data_1BAD0, 1.0f, 0);
          Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 297, &data_1BAC4, 1.0f, 0);
          break;
        case 30:
          Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 295, &data_1BAD0, 1.0f, 0);
          Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 292, &data_1BAC4, 1.0f, 0);
          break;
        case 40:
          Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 296, &data_1BAD0, 1.0f, 0);
          Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 292, &data_1BAC4, 1.0f, 0);
          break;
        case 50:
          Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 297, &data_1BAD0, 1.0f, 0);
          Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 292, &data_1BAC4, 1.0f, 0);
          break;
        case 60:
          Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 298, &data_1BAD0, 1.0f, 0);
          Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 292, &data_1BAC4, 1.0f, 0);
          break;
        case 80:
          Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 300, &data_1BAD0, 1.0f, 0);
          Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 292, &data_1BAC4, 1.0f, 0);
          break;
        case 100:
          Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 293, &data_1BADC, 1.0f, 0);
          Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 292, &data_1BAD0, 1.0f, 0);
          Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 292, &data_1BAC4, 1.0f, 0);
          break;
      }
    }
    if (local_FC == 0) {
      script_B664(41, 0);
      script_B664(40, 0);
    }
    if (local_FA > 0) {
      Script_charaSetRefboardFlagTo(SHEN_NAME('DRAM'), 1);
    } else {
      Script_charaSetRefboardFlagTo(SHEN_NAME('DRAM'), 0);
    }
    if (script_B674(8) == 0) {
      script_B664(42, 0);
      script_B664(41, 0);
      Script_charaSetRefboardFlagTo(SHEN_NAME('DRAM'), 1);
    }
    if (script_B674(8) == 1) {
      script_B664(16, script_B674(28));
      if (local_F6 < 15) {
        script_B664(35, script_B674(30));
        script_B664(36, script_B674(31));
        script_B664(37, script_B674(32));
        script_B664(38, script_B674(33));
        script_B664(39, script_B674(34));
        Script_charaSetRefboardFlagTo(SHEN_NAME('DRAM'), 1);
      } else {
        script_B664(35, 0);
        script_B664(36, 0);
        script_B664(37, 0);
        script_B664(38, 0);
        script_B664(39, 0);
        Script_charaSetRefboardFlagTo(SHEN_NAME('DRAM'), 0);
      }
    } else {
      local_F6 = 0;
    }
    if (script_B674(35) == 0) {
      Script_charaPosRotUtils(SHEN_NAME('SLTM'), 182, &local_DC, 0, CHARA_PART_Set);
    } else {
      Script_charaPosRotUtils(SHEN_NAME('SLTM'), 182, &local_B8, 0, CHARA_PART_Set);
    }
    if (script_B674(36) == 0) {
      Script_charaPosRotUtils(SHEN_NAME('SLTM'), 183, &local_DC, 0, CHARA_PART_Set);
    } else {
      Script_charaPosRotUtils(SHEN_NAME('SLTM'), 183, &local_AC, 0, CHARA_PART_Set);
    }
    if (script_B674(37) == 0) {
      Script_charaPosRotUtils(SHEN_NAME('SLTM'), 184, &local_DC, 0, CHARA_PART_Set);
    } else {
      Script_charaPosRotUtils(SHEN_NAME('SLTM'), 184, &local_A0, 0, CHARA_PART_Set);
    }
    if (script_B674(38) == 0) {
      Script_charaPosRotUtils(SHEN_NAME('SLTM'), 185, &local_DC, 0, CHARA_PART_Set);
    } else {
      Script_charaPosRotUtils(SHEN_NAME('SLTM'), 185, &local_94, 0, CHARA_PART_Set);
    }
    if (script_B674(39) == 0) {
      Script_charaPosRotUtils(SHEN_NAME('SLTM'), 186, &local_DC, 0, CHARA_PART_Set);
    } else {
      Script_charaPosRotUtils(SHEN_NAME('SLTM'), 186, &local_88, 0, CHARA_PART_Set);
    }
    if (script_B674(40) == 0) {
      Script_charaPosRotUtils(SHEN_NAME('SLTM'), 192, &local_DC, 0, CHARA_PART_Set);
    } else {
      Script_charaPosRotUtils(SHEN_NAME('SLTM'), 192, &local_FF7C, 0, CHARA_PART_Set);
    }
    if (script_B674(41) == 0) {
      Script_charaPosRotUtils(SHEN_NAME('SLTM'), 193, &local_DC, 0, CHARA_PART_Set);
    } else {
      Script_charaPosRotUtils(SHEN_NAME('SLTM'), 193, &local_FF70, 0, CHARA_PART_Set);
    }
    if (script_B674(42) == 0) {
      Script_charaPosRotUtils(SHEN_NAME('SLTM'), 194, &local_DC, 0, CHARA_PART_Set);
    } else {
      Script_charaPosRotUtils(SHEN_NAME('SLTM'), 194, &local_FF64, 0, CHARA_PART_Set);
    }
    if (script_B674(5) == 1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
    if (local_F6 > 30) {
      local_F6 = 0;
    } else {
      local_F6 = local_F6 + 1;
    }
  }
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 182, &local_B8, 0, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 183, &local_AC, 0, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 184, &local_A0, 0, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 185, &local_94, 0, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 186, &local_88, 0, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 192, &local_FF7C, 0, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 193, &local_FF70, 0, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 194, &local_FF64, 0, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 13, &local_C4, 0, CHARA_PART_Set);
  return Script_srchControl(0);
}

static void script_E15C(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  int32_t local_D8;
  local_F8 = script_B674(43);
  local_FC = script_B674(0);
  local_F4 = local_F8 + local_FC;
  local_EC = Script_shenGetStats(STATS_CASH);
  local_E4 = Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_DONE);
  if (local_E4 == 1) {
    local_D8 = 320;
  } else {
    local_D8 = 0;
  }
  if ((local_EC <= 1320) & (local_E4 == 1)) {
    local_E0 = local_EC - 320;
  } else if (local_EC >= 1000) {
    local_E0 = 1000;
  } else {
    local_E0 = local_EC;
  }
  local_DC = local_E0 / 100;
  local_DC = local_DC * 100;
  Script_scenCreateTask(script_B6BC, 1, 51);
  SCNF_WaitFrames_0000(50);
  local_F0 = SCNF_StartCashOverlaySelector(local_DC, 0, 0, 0);
  local_EC = local_EC - local_F0;
  Script_SetStats(STATS_CASH, local_EC);
  local_E8 = local_F0 / 100 * 20 / task_26C;
  script_B664(17, local_E8);
  task_380 = local_F0;
  local_F4 = local_F8 - local_F0;
  script_B664(43, local_F4);
  script_B664(0, local_F0);
  if (local_F4 >= local_D8 + 100) {
    script_B664(7, 1);
  } else {
    script_B664(7, 0);
  }
  SCNF_RequestIconNotification(0, 0, 0, 2);
  SCNF_WaitFrames_0000(1);
  return;
}

static void script_E2B4(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  Script_sprintf("OKANE : %d \012", task_380);
  local_F8 = Script_PlayerStatsUtils(PS_GetSlotMachineCashInTotal);
  Script_PlayerStatsUtils(PS_SetSlotMachineCashInTotal, local_F8 + task_380);
  while (task_380 > local_FC) {
    Script_SetNewHandMotion(CHARA_AKIR, 1, 15, 0, 0x42700000, 0, 0, 2.0f, 0);
    Script_executeAicaCommand(1449, 0, 0);
    SCNF_WaitFrames_0000(12);
    local_FC = local_FC + 100;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_E333(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t unused_EC;
  int32_t local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  int32_t local_D8;
  int32_t local_D4;
  int32_t local_D0;
  int32_t unused_C4[3];
  local_FC = script_B674(12);
  local_F8 = script_B674(13);
  local_F4 = script_B674(17);
  local_F0 = local_FC + local_F4;
  if (local_F4 >= 0) {
    local_E4 = local_F8 + local_FC + local_F4;
    if (local_E4 > 0x3B9AC9FF) {
      local_F4 = 0x3B9AC9FF - (local_F8 + local_FC);
      local_D8 = 0x3B9AC9FF - local_F8;
      local_DC = local_F8;
    } else if (local_F0 > 0x3B9AC9FF) {
      local_D8 = 0x3B9AC9FF;
      local_DC = local_F8;
    } else {
      local_D8 = local_F0;
      local_DC = local_F8;
    }
    local_E8 = local_F4;
    while (local_FC < local_D8) {
      local_FC = local_FC + 1;
      local_F4 = local_F4 - 1;
      script_B664(12, local_FC);
      if (local_E8 <= 50) {
        SCNF_WaitFrames_0000(1);
      }
      local_E8 = local_E8 - 1;
    }
  } else {
    if (local_F0 < 0) {
      local_D8 = 0;
      local_DC = local_F8 + local_FC;
    } else {
      local_D8 = local_F0;
      local_DC = local_F8 - local_F4;
    }
    local_E8 = 0;
    while (local_FC > local_D8) {
      local_FC = local_FC - 1;
      local_F4 = local_F4 + 1;
      script_B664(12, local_FC);
      if (local_FC <= 50) {
        SCNF_WaitFrames_0000(1);
      }
      local_E8 = local_E8 + 1;
    }
    if (local_E8 < 50) {
      SCNF_WaitFrames_0000(100 - local_E8);
    } else {
      SCNF_WaitFrames_0000(50);
    }
  }
  local_E0 = local_D8 + local_DC;
  script_B664(44, local_E0);
  script_B664(13, local_DC);
  script_B664(17, local_F4);
  if (local_E0 >= 1) {
    script_B664(18, 1);
  } else {
    script_B664(18, 0);
  }
  local_D4 = script_B674(43);
  local_D0 = script_B674(44);
  local_D0 = local_D0 * task_26C + task_26E + task_270;
  Script_SetStats(STATS_CASH, local_D4);
  Script_SetStats(STATS_SEGA_COINS, local_D0);
  SCNF_WaitFrames_0000(1);
  return;
}

static int32_t script_E548(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3 local_D0;
  Vector3 local_C4;
  Vector3 local_B8;
  Vector3i local_AC;
  Vector3i local_A0;
  Vector3i local_94;
  int16_t local_92;
  Script_EVT_GetPosition(SHEN_NAME('RMP1'), -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_lightResetSlotToDefaults(10);
  Script_LightUtils(10, 0, 4);
  Script_LightUtils(10, 2, &data_1BB14);
  Script_LightUtils(10, 4, 5.0f);
  Script_LightUtils(10, 5, 3.0f);
  Script_LightUtils(10, 6, data_1BB44);
  Script_LightUtils(10, 7, &local_F4);
  Script_LightUtils(10, 10, 5461);
  local_D0.x = 1.0f;
  (&local_D0)->y = 0.0f;
  (&local_D0)->z = -1.0f;
  Script_EVT_GetPosition(SHEN_NAME('RMP2'), -1, &local_E8, ENTITY_XFORM_WORLD_SPACE);
  Script_lightResetSlotToDefaults(11);
  Script_LightUtils(11, 0, 4);
  Script_LightUtils(11, 2, &data_1BB24);
  Script_LightUtils(11, 4, 5.0f);
  Script_LightUtils(11, 5, 3.0f);
  Script_LightUtils(11, 6, data_1BB44);
  Script_LightUtils(11, 7, &local_E8);
  Script_LightUtils(11, 10, 5461);
  local_C4.x = -1.0f;
  (&local_C4)->y = 0.0f;
  (&local_C4)->z = 1.0f;
  Script_EVT_GetPosition(SHEN_NAME('RMP3'), -1, &local_DC, ENTITY_XFORM_WORLD_SPACE);
  Script_lightResetSlotToDefaults(12);
  Script_LightUtils(12, 0, 4);
  Script_LightUtils(12, 2, &data_1BB34);
  Script_LightUtils(12, 4, 5.0f);
  Script_LightUtils(12, 5, 3.0f);
  Script_LightUtils(12, 6, data_1BB44);
  Script_LightUtils(12, 7, &local_DC);
  Script_LightUtils(12, 10, 5461);
  local_B8.x = 1.0f;
  (&local_B8)->y = 0.0f;
  (&local_B8)->z = 1.0f;
  Script_lightSetSlotEnabled(10, 1);
  Script_lightSetSlotEnabled(11, 1);
  Script_lightSetSlotEnabled(12, 1);
  Script_charaDisplay(SHEN_NAME('RMP1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('RMP2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('RMP3'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('RMPY'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('RMPR'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('RMPG'), CDM_Show);
  local_92 = 0;
  while (1) {
    Script_mtwkGetSetRotation(SHEN_NAME('RMPY'), &local_AC, 0);
    Script_mtwkGetSetRotation(SHEN_NAME('RMPR'), &local_A0, 0);
    Script_mtwkGetSetRotation(SHEN_NAME('RMPG'), &local_94, 0);
    (&local_AC)->y = (&local_AC)->y + 4369;
    (&local_A0)->y = (&local_A0)->y + 4369;
    (&local_94)->y = (&local_94)->y + 4369;
    Script_EVT_SetRotation(SHEN_NAME('RMPY'), 0x38000000, &local_AC, 0);
    Script_EVT_SetRotation(SHEN_NAME('RMPR'), 0x38000000, &local_A0, 0);
    Script_EVT_SetRotation(SHEN_NAME('RMPG'), 0x38000000, &local_94, 0);
    if (task_26C == 20) {
      if ((local_92 >= 0) & (local_92 <= 9)) {
        local_D0.x = (&local_D0)->x - 0.2f;
        local_C4.x = (&local_C4)->x + 0.2f;
        (&local_B8)->z = (&local_B8)->z - 0.2f;
      }
      if ((local_92 >= 10) & (local_92 <= 19)) {
        (&local_D0)->z = (&local_D0)->z + 0.2f;
        (&local_C4)->z = (&local_C4)->z - 0.2f;
        local_B8.x = (&local_B8)->x - 0.2f;
      }
      if ((local_92 >= 20) & (local_92 <= 29)) {
        local_D0.x = (&local_D0)->x + 0.2f;
        local_C4.x = (&local_C4)->x - 0.2f;
        (&local_B8)->z = (&local_B8)->z + 0.2f;
      }
      if ((local_92 >= 30) & (local_92 <= 39)) {
        (&local_D0)->z = (&local_D0)->z - 0.2f;
        (&local_C4)->z = (&local_C4)->z + 0.2f;
        local_B8.x = (&local_B8)->x + 0.2f;
      }
    } else {
      if ((local_92 >= 0) & (local_92 <= 9)) {
        local_D0.x = (&local_D0)->x + 0.2f;
        local_C4.x = (&local_C4)->x - 0.2f;
        (&local_B8)->z = (&local_B8)->z + 0.2f;
      }
      if ((local_92 >= 10) & (local_92 <= 19)) {
        (&local_D0)->z = (&local_D0)->z - 0.2f;
        (&local_C4)->z = (&local_C4)->z + 0.2f;
        local_B8.x = (&local_B8)->x + 0.2f;
      }
      if ((local_92 >= 20) & (local_92 <= 29)) {
        local_D0.x = (&local_D0)->x - 0.2f;
        local_C4.x = (&local_C4)->x + 0.2f;
        (&local_B8)->z = (&local_B8)->z - 0.2f;
      }
      if ((local_92 >= 30) & (local_92 <= 39)) {
        (&local_D0)->z = (&local_D0)->z + 0.2f;
        (&local_C4)->z = (&local_C4)->z - 0.2f;
        local_B8.x = (&local_B8)->x - 0.2f;
      }
    }
    Script_LightUtils(10, 8, &local_D0);
    Script_LightUtils(11, 8, &local_C4);
    Script_LightUtils(12, 8, &local_B8);
    local_92 = local_92 + 1;
    if (local_92 == 40) {
      local_92 = 0;
    }
    if (script_B674(8) == 2) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_EVT_SetRotation(SHEN_NAME('RMP1'), 0x38000000, &local_AC, 0);
  Script_EVT_SetRotation(SHEN_NAME('RMP2'), 0x38000000, &local_A0, 0);
  Script_EVT_SetRotation(SHEN_NAME('RMP3'), 0x38000000, &local_94, 0);
  Script_lightSetSlotEnabled(10, 0);
  Script_lightSetSlotEnabled(11, 0);
  Script_lightSetSlotEnabled(12, 0);
  Script_charaDisplay(SHEN_NAME('RMPY'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('RMPR'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('RMPG'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('RMP1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('RMP2'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('RMP3'), CDM_Show);
  Script_sprintf("消えるよ1 \012");
  return Script_srchControl(0);
}

static void script_EAD5(void) {
  int32_t local_FC;
  Script_sicnUtils(1, 0, 0);
  script_B664(8, 0);
  Script_scenCreateTask(script_EB7F, 0);
  Script_scenCreateTask(script_EC03, 0);
  Script_scenCreateTask(script_EC87, 0);
  SCNF_WaitFrames_0000(1);
  Script_executeAicaCommand(0x2C05A9, 0, 0);
  local_FC = Script_PlayerStatsUtils(PS_GetSlotMachinePlayCountSlot, task_1A2);
  Script_PlayerStatsUtils(PS_SetSlotMachinePlayCountSlot, task_1A2, local_FC + 1);
  Script_scenCreateTask(script_ED23, 0);
  SCNF_WaitFrames_0000(15);
  script_10085();
  script_C21C(13);
  while (script_B674(8) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_EB7F(void) {
  int16_t local_FE;
  while (1) {
    task_344.x = (&task_344)->x + 2978;
    Script_charaPosRotUtils(SHEN_NAME('DRAM'), 153, 0, &task_344, CHARA_PART_Set);
    if ((&task_344)->x == 65516) {
      task_344.x = 0;
    }
    local_FE = script_B674(19);
    if ((local_FE != -255) & (local_FE == (&task_344)->x / 2978)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(0x2E05A9, 0, 0);
  return Script_srchControl(0);
}

static int32_t script_EC03(void) {
  int16_t local_FE;
  while (1) {
    task_350.x = (&task_350)->x + 2978;
    Script_charaPosRotUtils(SHEN_NAME('DRAM'), 154, 0, &task_350, CHARA_PART_Set);
    if ((&task_350)->x >= 65516) {
      task_350.x = 0;
    }
    local_FE = script_B674(20);
    if ((local_FE != -255) & (local_FE == (&task_350)->x / 2978)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(0x2E05A9, 0, 0);
  return Script_srchControl(0);
}

static int32_t script_EC87(void) {
  int16_t local_FE;
  while (1) {
    task_35C.x = (&task_35C)->x + 2978;
    Script_charaPosRotUtils(SHEN_NAME('DRAM'), 155, 0, &task_35C, CHARA_PART_Set);
    if ((&task_35C)->x >= 65516) {
      task_35C.x = 0;
    }
    local_FE = script_B674(21);
    if ((local_FE != -255) & (local_FE == (&task_35C)->x / 2978)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  script_B664(8, 1);
  Script_executeAicaCommand(0x2D05A9, 0, 0);
  Script_executeAicaCommand(0x2E05A9, 0, 0);
  return Script_srchControl(0);
}

static int32_t script_ED23(void) {
  int8_t local_FF;
  int8_t local_FE;
  int8_t local_FD;
  int8_t local_FC;
  int8_t local_FB;
  int8_t local_FA;
  int16_t local_F8;
  int16_t local_F6;
  uint8_t unused_F0[6];
  local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 66.0f);
  local_FB = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 66.0f);
  local_FA = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 66.0f);
  local_F8 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 200.0f);
  if (*&task_224 == 1) {
    local_FF = *((int8_t *)data_19AE0 + local_FC);
    local_FE = *((int8_t *)data_19AE0 + (local_FB + 66));
    local_FD = *((int8_t *)data_19AE0 + (local_FA + 132));
  }
  if (*(&task_224 + 1) == 1) {
    local_FF = data_19BA8[local_FC];
    local_FE = data_19BA8[local_FB + 66];
    local_FD = data_19BA8[local_FA + 132];
  }
  if (*(&task_224 + 2) == 1) {
    local_FF = data_19C70[local_FC];
    local_FE = data_19C70[local_FB + 66];
    local_FD = data_19C70[local_FA + 132];
  }
  if (*(&task_224 + 3) == 1) {
    local_FF = data_19D38[local_FC];
    local_FE = data_19D38[local_FB + 66];
    local_FD = data_19D38[local_FA + 132];
  }
  if (*(&task_224 + 4) == 1) {
    local_FF = data_19E00[local_FC];
    local_FE = data_19E00[local_FB + 66];
    local_FD = data_19E00[local_FA + 132];
  }
  if (*(&task_224 + 5) == 1) {
    local_FF = data_19EC8[local_FC];
    local_FE = data_19EC8[local_FB + 66];
    local_FD = data_19EC8[local_FA + 132];
  }
  if (*&task_23C > local_F8) {
    task_224 = 1;
    *(&task_224 + 1) = 0;
    *(&task_224 + 2) = 0;
    *(&task_224 + 3) = 0;
    *(&task_224 + 4) = 0;
    *(&task_224 + 5) = 0;
  }
  if (*(&task_23C + 1) > local_F8) {
    task_224 = 0;
    *(&task_224 + 1) = 1;
    *(&task_224 + 2) = 0;
    *(&task_224 + 3) = 0;
    *(&task_224 + 4) = 0;
    *(&task_224 + 5) = 0;
  }
  if (*(&task_23C + 2) > local_F8) {
    task_224 = 0;
    *(&task_224 + 1) = 0;
    *(&task_224 + 2) = 1;
    *(&task_224 + 3) = 0;
    *(&task_224 + 4) = 0;
    *(&task_224 + 5) = 0;
  }
  if (*(&task_23C + 3) > local_F8) {
    task_224 = 0;
    *(&task_224 + 1) = 0;
    *(&task_224 + 2) = 0;
    *(&task_224 + 3) = 1;
    *(&task_224 + 4) = 0;
    *(&task_224 + 5) = 0;
  }
  if (*(&task_23C + 4) > local_F8) {
    task_224 = 0;
    *(&task_224 + 1) = 0;
    *(&task_224 + 2) = 0;
    *(&task_224 + 3) = 0;
    *(&task_224 + 4) = 1;
    *(&task_224 + 5) = 0;
  }
  if (*(&task_23C + 5) > local_F8) {
    task_224 = 0;
    *(&task_224 + 1) = 0;
    *(&task_224 + 2) = 0;
    *(&task_224 + 3) = 0;
    *(&task_224 + 4) = 0;
    *(&task_224 + 5) = 1;
  }
  script_F45D(local_FF, local_FE, local_FD);
  local_F6 = 0;
  while (local_F6 < 115) {
    if (local_F6 == 50) {
      script_B664(19, local_FF);
    }
    if (local_F6 == 80) {
      script_B664(20, local_FE);
    }
    if (local_F6 == 110) {
      script_B664(21, local_FD);
    }
    local_F6 = local_F6 + 1;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static int32_t script_F0E8(void) {
  int16_t local_FE;
  int8_t local_FD;
  int8_t local_FC;
  int8_t local_FB;
  local_FE = 0;
  local_FD = 0;
  local_FC = 0;
  local_FB = 0;
  while (local_FE < 200) {
    if (Script_GetControllerInput(1) == 4) {
      script_B664(25, 1);
      local_FD = 1;
    }
    if (Script_GetControllerInput(1) == 0x400) {
      script_B664(26, 1);
      local_FC = 1;
    }
    if (Script_GetControllerInput(1) == 0x200) {
      script_B664(27, 1);
      local_FB = 1;
    }
    if (-(local_FD == 1) & -(local_FC == 1) & -(local_FB == 1)) {
      break;
    }
    local_FE = local_FE + 1;
    SCNF_WaitFrames_0000(1);
  }
  script_B664(25, 1);
  script_B664(26, 1);
  script_B664(27, 1);
  script_B664(8, 1);
  Script_executeAicaCommand(0x2D05A9, 0, 0);
  return Script_srchControl(0);
}

static void script_F1D7(void) {
  int32_t unused_FC;
  script_B664(8, 0);
  Script_scenCreateTask(script_F0E8, 0);
  Script_scenCreateTask(script_F273, 0);
  Script_scenCreateTask(script_F301, 0);
  Script_scenCreateTask(script_F38F, 0);
  SCNF_WaitFrames_0000(1);
  Script_executeAicaCommand(0x2C05A9, 0, 0);
  SCNF_WaitFrames_0000(15);
  script_10085();
  script_C21C(13);
  while (script_B674(8) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  Script_scenCreateTask(script_F41D, 0);
  return;
}

static int32_t script_F273(void) {
  int16_t local_FE;
  int16_t local_FC;
  while (1) {
    task_344.x = (&task_344)->x + 2978;
    Script_charaPosRotUtils(SHEN_NAME('DRAM'), 153, 0, &task_344, CHARA_PART_Set);
    if ((&task_344)->x == 65516) {
      task_344.x = 0;
    }
    local_FE = script_B674(25);
    if (local_FE == 1) {
      local_FC = (&task_344)->x / 2978;
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  script_B664(22, local_FC);
  Script_executeAicaCommand(0x2E05A9, 0, 0);
  return Script_srchControl(0);
}

static int32_t script_F301(void) {
  int16_t local_FE;
  int16_t local_FC;
  while (1) {
    task_350.x = (&task_350)->x + 2978;
    Script_charaPosRotUtils(SHEN_NAME('DRAM'), 154, 0, &task_350, CHARA_PART_Set);
    if ((&task_350)->x >= 65516) {
      task_350.x = 0;
    }
    local_FE = script_B674(26);
    if (local_FE == 1) {
      local_FC = (&task_350)->x / 2978;
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  script_B664(23, local_FC);
  Script_executeAicaCommand(0x2E05A9, 0, 0);
  return Script_srchControl(0);
}

static int32_t script_F38F(void) {
  int16_t local_FE;
  int16_t local_FC;
  while (1) {
    task_35C.x = (&task_35C)->x + 2978;
    Script_charaPosRotUtils(SHEN_NAME('DRAM'), 155, 0, &task_35C, CHARA_PART_Set);
    if ((&task_35C)->x >= 65516) {
      task_35C.x = 0;
    }
    local_FE = script_B674(27);
    if (local_FE == 1) {
      local_FC = (&task_35C)->x / 2978;
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  script_B664(24, local_FC);
  Script_executeAicaCommand(0x2E05A9, 0, 0);
  return Script_srchControl(0);
}

static int32_t script_F41D(void) {
  int8_t local_FF;
  int8_t local_FE;
  int8_t local_FD;
  local_FF = script_B674(22);
  local_FE = script_B674(23);
  local_FD = script_B674(24);
  script_F45D(local_FF, local_FE, local_FD);
  return Script_srchControl(0);
}

static void script_F45D(int8_t arg0, int8_t arg1, int8_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int16_t local_F2;
  int8_t local_F1;
  int8_t local_F0;
  int8_t local_EF;
  int8_t local_EE;
  int8_t local_ED;
  int32_t local_E8;
  Script_sprintf(" d1:%2d  d2:%2d  d3:%2d ", arg0, arg1, arg2);
  local_FC = 0;
  local_EE = 1;
  local_F1 = 1;
  local_F0 = 1;
  local_EF = 1;
  script_B664(30, 0);
  script_B664(31, 0);
  script_B664(32, 0);
  script_B664(33, 0);
  script_B664(34, 0);
  while (local_EE <= script_B674(14)) {
    local_F8 = data_19F90[arg0 + local_F1] * 100 + data_19FAC[arg1 + local_F0] * 10 + data_19FC8[arg2 + local_EF];
    local_ED = 0;
    local_F2 = 0;
    while (local_ED < 17) {
      if (local_F8 == *(int16_t *)((int8_t *)&data_19A88 + (local_ED << 1))) {
        local_F2 = local_ED;
      }
      local_ED = local_ED + 1;
    }
    if ((local_F8 >= 100) & (local_F8 <= 199)) {
      local_F2 = 18;
    }
    if ((local_F8 >= 110) & (local_F8 <= 119)) {
      local_F2 = 19;
    }
    if (local_F8 == 111) {
      local_F2 = 1;
    }
    if ((local_F8 == 555) & (local_EE == 1)) {
      local_F2 = 20;
    }
    if ((local_F8 == 555) & (local_EE == 2)) {
      local_F2 = 21;
    }
    if ((local_F8 == 555) & (local_EE == 3)) {
      local_F2 = 22;
    }
    if ((local_F8 == 555) & (local_EE == 4)) {
      local_F2 = 23;
    }
    if ((local_F8 == 555) & (local_EE == 5)) {
      local_F2 = 24;
    }
    local_E8 = local_EE;
    switch (local_E8) {
      case 1:
        script_B664(30, *(int16_t *)((int8_t *)&data_19AAC + (local_F2 << 1)));
        local_F1 = 2;
        local_F0 = 2;
        local_EF = 2;
        break;
      case 2:
        script_B664(31, *(int16_t *)((int8_t *)&data_19AAC + (local_F2 << 1)));
        local_F1 = 0;
        local_F0 = 0;
        local_EF = 0;
        break;
      case 3:
        script_B664(32, *(int16_t *)((int8_t *)&data_19AAC + (local_F2 << 1)));
        local_F1 = 2;
        local_F0 = 1;
        local_EF = 0;
        break;
      case 4:
        script_B664(33, *(int16_t *)((int8_t *)&data_19AAC + (local_F2 << 1)));
        local_F1 = 0;
        local_F0 = 1;
        local_EF = 2;
        break;
      case 5:
        script_B664(34, *(int16_t *)((int8_t *)&data_19AAC + (local_F2 << 1)));
        break;
    }
    local_FC = local_FC + *(int16_t *)((int8_t *)&data_19AAC + (local_F2 << 1));
    local_EE = local_EE + 1;
  }
  script_B664(28, local_FC);
  script_B664(17, local_FC);
  local_F4 = Script_PlayerStatsUtils(PS_GetSlotMachineTotalWinSlot, task_1A2);
  Script_PlayerStatsUtils(PS_SetSlotMachineTotalWinSlot, task_1A2, local_F4 + local_FC * task_26C);
  if (local_FC == 0) {
    script_B664(29, 0);
  }
  if (local_FC >= 2) {
    script_B664(29, 1);
  }
  if (local_FC >= 200) {
    script_B664(29, 2);
  }
  Script_sprintf(" RETURN: %4d \012", script_B674(28));
  return;
}

static void script_F781(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = 0.33f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.83f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  local_E8.x = 0;
  (&local_E8)->y = 0;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  return;
}

static void script_F7DC(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = 0.3f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 1.0f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  local_E8.x = 0;
  (&local_E8)->y = 0;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  return;
}

static void script_F837(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = 0.34f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.84f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  local_E8.x = 0;
  (&local_E8)->y = 0;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  return;
}

static void script_F892(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = 0.26f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.84f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  local_E8.x = 0;
  (&local_E8)->y = 0;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  return;
}

static void script_F8ED(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = 0.26f;
  (&local_F4)->y = 1.0f;
  (&local_F4)->z = 0.48f;
  Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_F4);
  local_E8.x = 0xC000;
  (&local_E8)->y = 35498;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &local_E8, 0);
  return;
}

static void script_F951(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = 0.46f;
  (&local_F4)->y = 1.0f;
  (&local_F4)->z = 0.48f;
  Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_F4);
  local_E8.x = 0xC000;
  (&local_E8)->y = 0x8000;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &local_E8, 0);
  Script_SetNewHandMotion(SHEN_NAME('YKUR'), 1, 15, 0x41D00000, 0x41F00000, 0x41D00000, 0, 2.0f, 0);
  SCNF_WaitFrames_0000(1);
  return;
}

static void script_F9E3(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = 0.26f;
  (&local_F4)->y = 1.0f;
  (&local_F4)->z = 0.48f;
  Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_F4);
  local_E8.x = 0xC000;
  (&local_E8)->y = 35498;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &local_E8, 0);
  return;
}

static int32_t script_FA47(void) {
  Vector3 local_F4;
  Script_EVT_GetPosition(SHEN_NAME('CA0C'), -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  (&local_F4)->y = 0.7f;
  Script_SetCharaLookAtPosition(CHARA_AKIR, 2, &local_F4, 0);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F4, 10);
  return Script_srchControl(0);
}

static int32_t script_FA8D(void) {
  Vector3 local_F4;
  Script_EVT_GetPosition(CHARA_NITO, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetCharaLookAtPosition(CHARA_AKIR, 2, &local_F4, 2);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F4, 10);
  return Script_srchControl(0);
}

static int32_t script_FAC7(void) {
  Vector3 local_F4;
  Script_EVT_GetPosition(SHEN_NAME('SLTM'), -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -98312, 0, 0);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
  return Script_srchControl(0);
}

static void script_FB00(void) {
  int32_t local_FC;
  local_FC = script_B674(44);
  local_FC = local_FC * task_26C + task_26E + task_270;
  Script_SetStats(STATS_SEGA_COINS, local_FC);
  SCNF_SetCashValue(0, 0);
  SCNF_WaitFrames_0000(1);
  return;
}

__attribute__((scn_string_bindings(0, data_1BB80, 1, data_1BB86, 2, data_1BB8B, 3, data_1BB91, 4, data_1BB96)))
static int32_t script_FB3D(void) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  Vector3i local_DC;
  int32_t unused_D0[3];
  int32_t local_CC;
  int32_t unused_C4[2];
  int32_t local_C0;
  Script_charaDisplay(SHEN_NAME('C50D'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('C50P'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('C20R'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('C20L'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CA0C'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CA0R'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CA0L'), CDM_Hide);
  local_C0 = script_B674(13);
  local_C0 = local_C0 * task_26C;
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MPK00");
  local_CC = 0;
  while (local_CC < 50) {
    Script_InterpolateAndScaleAngles(local_CC, 2, &local_F4, &local_DC, "C50S");
    local_F4.x = (&local_F4)->x * (float)10;
    (&local_F4)->y = (&local_F4)->y * (float)10;
    (&local_F4)->z = (&local_F4)->z * (float)10;
    Script_charaPosRotUtils(SHEN_NAME('C50P'), *(&data_19FE4 + local_CC), &local_F4, &local_DC, CHARA_PART_Set);
    local_CC = local_CC + 1;
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  SCNF_WaitFrames_0000(1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MPK00");
  if ((local_C0 > 0) & (local_C0 <= 75)) {
    local_CC = 0;
    while (local_CC < local_C0) {
      Script_InterpolateAndScaleAngles(local_CC, 160, &local_F4, &local_DC, "C50S");
      local_F4.x = (&local_F4)->x * (float)10;
      (&local_F4)->y = (&local_F4)->y * (float)10;
      (&local_F4)->z = (&local_F4)->z * (float)10;
      Script_charaPosRotUtils(SHEN_NAME('C50P'), *(&data_19FE4 + local_CC), &local_F4, &local_DC, CHARA_PART_Set);
      local_CC = local_CC + 1;
      if (local_CC == 50) {
        local_CC = 75;
      }
    }
    Script_charaDisplay(SHEN_NAME('C50P'), CDM_Show);
  }
  if ((local_C0 > 75) & (local_C0 <= 150)) {
    local_CC = 0;
    while (local_CC + 50 < local_C0) {
      Script_InterpolateAndScaleAngles(local_CC, 160, &local_F4, &local_DC, "C50S");
      local_F4.x = (&local_F4)->x * (float)10;
      (&local_F4)->y = (&local_F4)->y * (float)10;
      (&local_F4)->z = (&local_F4)->z * (float)10;
      Script_charaPosRotUtils(SHEN_NAME('C50P'), *(&data_19FE4 + local_CC), &local_F4, &local_DC, CHARA_PART_Set);
      local_CC = local_CC + 1;
      if (local_CC == 50) {
        local_CC = 100;
      }
    }
    Script_charaDisplay(SHEN_NAME('C20R'), CDM_Show);
    Script_charaDisplay(SHEN_NAME('C20L'), CDM_Show);
    Script_charaDisplay(SHEN_NAME('C50P'), CDM_Show);
  }
  if ((local_C0 > 150) & (local_C0 <= 300)) {
    Script_charaDisplay(SHEN_NAME('C20R'), CDM_Show);
    Script_charaDisplay(SHEN_NAME('C20L'), CDM_Show);
    Script_charaDisplay(SHEN_NAME('CA0C'), CDM_Show);
  }
  if (local_C0 > 300) {
    Script_charaDisplay(SHEN_NAME('CA0R'), CDM_Show);
    Script_charaDisplay(SHEN_NAME('CA0L'), CDM_Show);
    Script_charaDisplay(SHEN_NAME('CA0C'), CDM_Show);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  SCNF_WaitFrames_0000(1);
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_1BB9B, 1, data_1BBA1, 2, data_1BBA6, 3, data_1BBAC)))
static int32_t script_FE30(void) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  Vector3i local_DC;
  int32_t unused_D0[3];
  int16_t local_CE;
  int16_t local_CC;
  int32_t local_C8;
  int32_t local_C4;
  int32_t local_C0;
  int32_t local_BC;
  local_C4 = script_B674(12);
  local_C4 = local_C4 * task_26C;
  local_C8 = local_C4 * -1;
  script_B664(17, local_C8);
  local_C0 = script_B674(13);
  if (local_C4 <= 50) {
    local_BC = local_C0;
  } else {
    local_BC = local_C0 + local_C4 - 50;
  }
  script_B664(13, local_BC);
  script_BBEA(21);
  SCNF_WaitFrames_0000(5);
  script_B664(13, local_C0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MPK00");
  local_CE = 0;
  while (local_CE < 50) {
    Script_InterpolateAndScaleAngles(local_CE, 2, &local_F4, &local_DC, "C50S");
    local_F4.x = (&local_F4)->x * (float)10;
    (&local_F4)->y = (&local_F4)->y * (float)10;
    (&local_F4)->z = (&local_F4)->z * (float)10;
    Script_charaPosRotUtils(SHEN_NAME('C50D'), *(&data_19FE4 + local_CE), &local_F4, &local_DC, CHARA_PART_Set);
    local_CE = local_CE + 1;
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  SCNF_WaitFrames_0000(1);
  Script_charaDisplay(SHEN_NAME('C50D'), CDM_Show);
  Script_scenCreateTask(script_B6AA, 1, 17);
  local_CC = 2;
  while (local_CC < 160) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MPK00");
    local_CE = 0;
    while (local_CE < 50) {
      Script_InterpolateAndScaleAngles(local_CE, local_CC, &local_F4, &local_DC, "C50S");
      local_F4.x = (&local_F4)->x * (float)10;
      (&local_F4)->y = (&local_F4)->y * (float)10;
      (&local_F4)->z = (&local_F4)->z * (float)10;
      Script_charaPosRotUtils(SHEN_NAME('C50D'), *(&data_19FE4 + local_CE), &local_F4, &local_DC, CHARA_PART_Set);
      if (local_C4 - 1 > local_CE) {
        local_CE = local_CE + 1;
      } else {
        local_CE = 50;
      }
    }
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    if (local_C4 * 2 + 30 > local_CC) {
      local_CC = local_CC + 2;
    } else {
      local_CC = 160;
    }
    SCNF_WaitFrames_0000(1);
  }
  if (task_26E > task_270) {
    task_270 = task_26E;
    task_26E = 0;
  }
  Script_scenCreateTask(script_B6AA, 1, 18);
  return Script_srchControl(0);
}

static void script_10085(void) {
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Hide);
  Script_scenToggleActiveFor(CHARA_AKIR, 0);
  return;
}

static void script_100AA(void) {
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Show);
  Script_scenToggleActiveFor(CHARA_AKIR, 0);
  return;
}

static void script_100CF(void) {
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Hide);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_scenToggleActiveFor(CHARA_AKIR, 1);
  return;
}

static void script_10100(void) {
  Vector3 local_F4;
  float local_E8[3];
  Vector3i local_DC;
  int8_t local_DB;
  int8_t local_DA;
  int32_t local_D4;
  Script_EVT_GetPosition(SHEN_NAME('YKUR'), -1, &local_F4, 0);
  Script_mtwkGetSetRotation(SHEN_NAME('YKUR'), &local_DC, 0);
  local_DB = script_B674(6);
  local_D4 = local_DB;
  switch (local_D4) {
    case 1:
      local_E8[0] = 0.006f;
      local_E8[1] = 0.0f;
      local_E8[2] = 0.0f;
      script_B664(6, 2);
      break;
    case 2:
      local_E8[0] = 0.008f;
      local_E8[1] = 0.0f;
      local_E8[2] = 0.0f;
      script_B664(6, 3);
      break;
    case 3:
      if (script_B674(13) > 0) {
        local_E8[0] = 0.01f;
        local_E8[1] = 0.0f;
        local_E8[2] = 0.0f;
        script_B664(6, 4);
        script_B664(15, 0);
      } else {
        local_E8[0] = 0.02f;
        local_E8[1] = 0.0f;
        local_E8[2] = 0.0f;
        script_B664(6, 6);
        Script_scenCreateTask(script_FB3D, 0);
        script_F837();
        script_100CF();
        script_C21C(11);
        script_BEF5(0, 8, 0);
        SCNF_WaitFrames_0000(5);
        Script_SetNewHandMotion(CHARA_AKIR, 1, 76, 0, 0, 0, 0, 1.0f, 0);
      }
      break;
    case 4:
      local_E8[0] = 0.01f;
      local_E8[1] = 0.0f;
      local_E8[2] = 0.0f;
      script_B664(6, 6);
      Script_scenCreateTask(script_FB3D, 0);
      script_F837();
      script_100CF();
      script_C21C(11);
      script_BEF5(0, 8, 0);
      SCNF_WaitFrames_0000(5);
      Script_SetNewHandMotion(CHARA_AKIR, 1, 76, 0, 0, 0, 0, 1.0f, 0);
      SCNF_WaitFrames_0000(12);
      break;
    case 5:
      local_E8[0] = 0.01f;
      local_E8[1] = 0.0f;
      local_E8[2] = 0.0f;
      script_B664(6, 6);
      break;
    case 6:
      local_E8[0] = 0.0f;
      local_E8[1] = 0.0f;
      local_E8[2] = 0.0f;
      break;
  }
  local_DA = 0;
  while (local_DA < 10) {
    local_F4.x = (&local_F4)->x + *local_E8;
    (&local_DC)->y = (&local_DC)->y - 182;
    Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &local_DC, 0);
    SCNF_WaitFrames_0000(1);
    local_DA = local_DA + 1;
  }
  return;
}

static void script_1039F(void) {
  Vector3 local_F4;
  float local_E8[3];
  Vector3i local_DC;
  int8_t local_DB;
  int8_t local_DA;
  int32_t local_D4;
  Script_EVT_GetPosition(SHEN_NAME('YKUR'), -1, &local_F4, 0);
  Script_mtwkGetSetRotation(SHEN_NAME('YKUR'), &local_DC, 0);
  local_DB = script_B674(6);
  local_D4 = local_DB;
  switch (local_D4) {
    case 1:
      local_E8[0] = 0.0f;
      local_E8[1] = 0.0f;
      local_E8[2] = 0.0f;
      break;
    case 2:
      local_E8[0] = 0.006f;
      local_E8[1] = 0.0f;
      local_E8[2] = 0.0f;
      script_B664(6, 1);
      break;
    case 3:
      local_E8[0] = 0.008f;
      local_E8[1] = 0.0f;
      local_E8[2] = 0.0f;
      script_B664(6, 2);
      break;
    case 4:
      local_E8[0] = 0.01f;
      local_E8[1] = 0.0f;
      local_E8[2] = 0.0f;
      script_B664(6, 3);
      break;
    case 5:
      local_E8[0] = 0.01f;
      local_E8[1] = 0.0f;
      local_E8[2] = 0.0f;
      script_B664(6, 4);
      script_B664(15, 0);
      break;
    case 6:
      if (script_B674(13) > 0) {
        local_E8[0] = 0.01f;
        local_E8[1] = 0.0f;
        local_E8[2] = 0.0f;
        script_B664(6, 4);
        script_B664(15, 0);
      } else {
        local_E8[0] = 0.02f;
        local_E8[1] = 0.0f;
        local_E8[2] = 0.0f;
        script_B664(6, 3);
      }
      script_BEF5(0, 9, 0);
      Script_SetNewHandMotion(CHARA_AKIR, 1, 76, 0, 0x41700000, 0, 0, 1.0f, 0);
      SCNF_WaitFrames_0000(20);
      script_100AA();
      script_C21C(10);
      break;
  }
  local_DA = 0;
  while (local_DA < 10) {
    local_F4.x = (&local_F4)->x - *local_E8;
    (&local_DC)->y = (&local_DC)->y + 182;
    Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &local_DC, 0);
    SCNF_WaitFrames_0000(1);
    local_DA = local_DA + 1;
  }
  return;
}

static int32_t script_105E4(void) {
  Vector3i local_F4;
  Vector3 local_E8;
  int8_t local_E7;
  Script_mtwkGetSetRotation(SHEN_NAME('YKUR'), &local_F4, 0);
  Script_EVT_GetPosition(SHEN_NAME('YKUR'), -1, &local_E8, 0);
  local_E7 = 0;
  while (local_E7 < 10) {
    if (local_E7 < 5) {
      local_F4.x = (&local_F4)->x + 182;
      (&local_E8)->y = (&local_E8)->y - 0.002f;
    }
    if (local_E7 > 4) {
      local_F4.x = (&local_F4)->x - 182;
      (&local_E8)->y = (&local_E8)->y + 0.002f;
    }
    Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &local_F4, 0);
    Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_E8);
    SCNF_WaitFrames_0000(1);
    local_E7 = local_E7 + 1;
  }
  return Script_srchControl(0);
}

static int32_t script_1069C(void) {
  int32_t unused_E8[6];
  int32_t local_E4;
  uint8_t unused_E3;
  int8_t local_E2;
  int8_t local_E1;
  uint8_t unused_E0;
  while (1) {
    local_E2 = script_B674(9);
    local_E1 = script_B674(14);
    if ((local_E2 == 0) | (local_E1 == 5)) {
      script_B664(9, 0);
    } else {
      Script_scenCreateTask(script_105E4, 0);
      SCNF_WaitFrames_0000(5);
      Script_scenCreateTask(script_B6AA, 1, 12);
      Script_scenCreateTask(script_B6AA, 1, 1);
      SCNF_WaitFrames_0000(5);
      script_B664(17, 0);
      script_B664(16, 0);
      script_B664(12, script_B674(12) - 1);
      script_B664(14, script_B674(14) + 1);
      script_B664(44, script_B674(44) - 1);
      local_E4 = Script_PlayerStatsUtils(PS_GetSlotMachineTotalBetSlot, task_1A2);
      Script_PlayerStatsUtils(PS_SetSlotMachineTotalBetSlot, task_1A2, local_E4 + task_26C);
      script_E333();
      if (script_B674(14) >= 5) {
        script_B664(9, 0);
      } else {
        script_B664(9, script_B674(9) - 1);
      }
    }
    if (script_B674(12) == 0) {
      script_B664(9, 0);
    }
    if (script_B674(5) == 1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static int32_t script_10805(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  int32_t local_E4;
  int8_t local_E3;
  local_E3 = 0;
  while (1) {
    local_FC = script_B674(10);
    local_F8 = script_B674(14);
    local_EC = script_B674(15);
    local_F4 = script_B674(12);
    local_F0 = script_B674(13);
    local_E8 = script_B674(44);
    if (local_EC == 1) {
      if (Script_GetControllerInput(1) == 0x400) {
        script_B664(10, local_FC + 1);
      }
    }
    if (local_EC == 2) {
      Script_scenCreateTask(script_B6AA, 1, 3);
      Script_SetNewHandMotion(SHEN_NAME('YKUR'), 1, 15, 0, 0x42700000, 0, 0, 2.0f, 0);
      SCNF_WaitFrames_0000(9);
      if (local_F8 == 5) {
        script_B664(13, local_F0 - 1);
        script_B664(12, local_F4 + 1);
      } else {
        script_B664(13, local_F0 - 1);
        script_B664(14, local_F8 + 1);
        script_B664(44, local_E8 - 1);
        Script_scenCreateTask(script_B6AA, 1, 1);
        local_E4 = Script_PlayerStatsUtils(PS_GetSlotMachineTotalBetSlot, task_1A2);
        Script_PlayerStatsUtils(PS_SetSlotMachineTotalBetSlot, task_1A2, local_E4 + task_26C);
      }
      script_B664(17, 0);
      script_B664(16, 0);
      script_E333();
      local_E3 = local_E3 + 1;
      if ((local_E3 == local_FC) | (local_E3 == 5) | (script_B674(13) <= 0)) {
        script_B664(10, 0);
        script_B664(15, 0);
        local_E3 = 0;
      }
    }
    if (script_B674(13) <= 0) {
      script_B664(13, 0);
    }
    if (script_B674(5) == 1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_109E8(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_FC = script_B674(14);
  local_F8 = script_B674(13);
  local_F4 = script_B674(44);
  if (local_FC == 5) {
    if (local_F4 <= 0) {
      script_B664(0, 0);
    } else if (local_F8 <= 0) {
      script_B664(0, 0);
    } else {
      script_B664(0, 2);
    }
  } else if (local_F4 <= 0) {
    script_B664(0, 1);
  } else if (local_F8 <= 0) {
    script_B664(0, 0);
  } else {
    script_B664(0, 2);
  }
  Script_sprintf("Root : %d \012", script_B674(0));
  return;
}

static int32_t script_10AA6(void) {
  Vector3i local_F4;
  Vector3 local_E8;
  int8_t local_E7;
  local_F4.x = 0;
  (&local_F4)->y = 0;
  (&local_F4)->z = 0;
  local_E8.x = 0.0f;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = 0.0f;
  local_E7 = 0;
  while (local_E7 < 30) {
    if ((local_E7 >= 0) & (local_E7 < 3)) {
      local_F4.x = (&local_F4)->x - 182;
      (&local_E8)->y = 0.0f;
    }
    if ((local_E7 >= 3) & (local_E7 < 6)) {
      local_F4.x = (&local_F4)->x + 91;
      (&local_E8)->y = 0.0f;
    }
    if ((local_E7 >= 6) & (local_E7 < 9)) {
      local_F4.x = (&local_F4)->x + 364;
      (&local_E8)->y = 0.0f;
    }
    if ((local_E7 >= 9) & (local_E7 < 12)) {
      local_F4.x = (&local_F4)->x + 1092;
      (&local_E8)->y = -0.0065f;
    }
    if ((local_E7 >= 12) & (local_E7 < 15)) {
      local_F4.x = (&local_F4)->x + 819;
      (&local_E8)->y = -0.0085f;
    }
    if ((local_E7 >= 15) & (local_E7 < 18)) {
      local_F4.x = (&local_F4)->x;
      (&local_E8)->y = -0.005f;
    }
    if ((local_E7 >= 18) & (local_E7 < 21)) {
      local_F4.x = (&local_F4)->x - 728;
      (&local_E8)->y = 0.0045f;
    }
    if ((local_E7 >= 21) & (local_E7 < 24)) {
      local_F4.x = (&local_F4)->x - 728;
      (&local_E8)->y = 0.0075f;
    }
    if ((local_E7 >= 24) & (local_E7 < 27)) {
      local_F4.x = (&local_F4)->x - 728;
      (&local_E8)->y = 0.008f;
    }
    if ((local_E7 >= 27) & (local_E7 < 30)) {
      local_F4.x = (&local_F4)->x;
      (&local_E8)->y = 0.0f;
    }
    Script_charaPosRotUtils(SHEN_NAME('SLTM'), 13, 0, &local_F4, CHARA_PART_Set);
    Script_charaPosRotUtils(SHEN_NAME('SLTM'), 13, &local_E8, 0, CHARA_PART_Add);
    local_E7 = local_E7 + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_SetNewHandMotion(CHARA_AKIR, 1, 76, 0, 0x41700000, 0, 0, 1.0f, 0);
  SCNF_WaitFrames_0000(30);
  return Script_srchControl(0);
}

static void script_10CB5(void) {
  int32_t local_FC;
  int16_t local_FA;
  int16_t local_F8;
  int16_t local_F6;
  uint8_t unused_F4[2];
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3 local_D0;
  Vector3i local_C4;
  Vector3 local_B8;
  Vector3 local_AC;
  Script_sicnUtils(1, 0, 0);
  Script_shenClearFreeState(0);
  script_100CF();
  Script_SetCPAMFlags(CHARA_MIKM, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_seqEnterEvtCam(7149, CHARA_AKIR, CHARA_AKIR);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -98312, 0, 0);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
  if (task_274 == 0) {
    Script_scenCreateTask(script_B6BC, 1, 58);
    SCNF_WaitFrames_0000(120);
  } else {
    Script_scenCreateTask(script_B6BC, 1, 22);
    SCNF_WaitFrames_0000(60);
  }
  Script_MoveObjectToObject(CHARA_AKIR, -1);
  local_FC = Script_shenGetStats(STATS_SEGA_COINS);
  if (local_FC >= 1) {
    Script_filmCheckOrSetTransitionUtil(20, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    SCNF_WaitFrames_0000(20);
    Script_SetCharaMotionPlaybackRate(CHARA_MIKM, 0.5f);
    local_D0.x = 1.2f;
    (&local_D0)->y = 0.0f;
    (&local_D0)->z = -1.5f;
    Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &local_D0);
    local_C4.x = 0;
    (&local_C4)->y = 30947;
    (&local_C4)->z = 0;
    Script_EVT_SetRotation(CHARA_AKIR, 0x78000000, &local_C4, 0);
    Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
    local_D0.x = 1.1f;
    (&local_D0)->y = 0.0f;
    (&local_D0)->z = -0.35f;
    Script_EVT_SetPosition(CHARA_MIKM, 0x78000000, &local_D0);
    local_C4.x = 0;
    (&local_C4)->y = 0;
    (&local_C4)->z = 0;
    Script_EVT_SetRotation(CHARA_MIKM, 0x78000000, &local_C4, 0);
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_E8, ENTITY_XFORM_WORLD_SPACE);
    (&local_E8)->y = 1.5f;
    Script_SetCharaLookAtPosition(CHARA_MIKM, 1, &local_E8, 2);
    Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
    Script_EVT_GetPosition(CHARA_MIKM, -1, &local_DC, ENTITY_XFORM_WORLD_SPACE);
    (&local_DC)->y = 1.5f;
    Script_SetCharaLookAtPosition(CHARA_AKIR, 1, &local_DC, 2);
    SCNF_WaitFrames_0000(10);
    Script_seqEnterEvtCam(7146, 0, AUTH_None);
    Script_filmCheckOrSetTransitionUtil(20, 255, 0, 0, 0, 0, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    SCNF_WaitFrames_0000(20);
    Script_scenCreateTask(script_B6BC, 1, 65);
    SCNF_WaitFrames_0000(90);
    Script_seqEnterEvtCam(7147, 0, AUTH_None);
    Script_scenCreateTask(script_B6BC, 1, 53);
    SCNF_WaitFrames_0000(80);
    SCNF_SetCashValue(-local_FC, 0);
    Script_SetStats(STATS_SEGA_COINS, local_FC);
    local_F8 = Script_GetOwnedItemCount(221, 0);
    local_F6 = Script_GetOwnedItemCount(222, 0);
    if ((local_FC >= 10000) & (local_F8 == 0)) {
      script_A297(12);
      Script_gachaponUtils(1, 0x4E494E4B, 4);
      Script_seqEnterEvtCam(7148, 0, AUTH_None);
      Script_scenCreateTask(script_B6BC, 1, 76);
      SCNF_WaitFrames_0000(102);
      Script_scenCreateTask(script_B6BC, 1, 60);
      SCNF_WaitFrames_0000(31);
      Script_seqEnterEvtCam(7153, 0, AUTH_None);
      Script_PlayCharaMotion(CHARA_AKIR, 4131, 110.0f, 111.0f, 0.0f, 1.0f, 30);
      Script_PlayCharaMotion(CHARA_MIKM, 4133, 0.0f, 180.0f, 0.0f, 1.0f, 30);
      SCNF_WaitFrames_0000(40);
      Script_hmdlApplyScale(SHEN_NAME('KNIN'), &data_1BBC0);
      local_B8.x = 0.13f;
      (&local_B8)->y = 0.11f;
      (&local_B8)->z = -0.077f;
      {
        Vector3i local_A0;
        local_A0.x = 27306;
        (&local_A0)->y = 22755;
        (&local_A0)->z = 50972;
        Script_EVT_GetPosition(CHARA_MIKM, 18, &local_AC, 0);
        Script_EVT_SetPosition(SHEN_NAME('KNIN'), POS_XYZ, &local_AC);
        Script_fixoAttachEx(SHEN_NAME('KNIN'), CHARA_MIKM, 18, &local_B8, &local_A0);
      }
      Script_charaDisplay(SHEN_NAME('KNIN'), CDM_Show);
      Script_scenCreateTask(script_B6BC, 1, 71);
      SCNF_WaitFrames_0000(50);
      Script_fixoClearAttachment(SHEN_NAME('KNIN'));
      SCNF_WaitFrames_0000(40);
      Script_seqEnterEvtCam(7154, 0, AUTH_None);
      Script_GetCassetteFlags(221, 1, 0, 0);
      SCNF_StartIconSystemMenu(221);
      Script_scenCreateTask(script_B6BC, 1, 57);
      Script_EVT_GetPosition(CHARA_MIKM, -1, &local_DC, ENTITY_XFORM_WORLD_SPACE);
      (&local_DC)->y = 1.0f;
      Script_SetCharaLookAtPosition(CHARA_AKIR, 1, &local_DC, 2);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_DC, 10);
      SCNF_WaitFrames_0000(40);
      local_B8.x = 0.167f;
      (&local_B8)->y = 0.035f;
      (&local_B8)->z = -0.056f;
      {
        Vector3i local_A0;
        local_A0.x = 39685;
        (&local_A0)->y = 62805;
        (&local_A0)->z = 19842;
        Script_EVT_GetPosition(CHARA_AKIR, 18, &local_AC, 0);
        Script_EVT_SetPosition(SHEN_NAME('KNIN'), POS_XYZ, &local_AC);
        Script_fixoAttachEx(SHEN_NAME('KNIN'), CHARA_AKIR, 18, &local_B8, &local_A0);
      }
      Script_seqEnterEvtCam(7155, 0, AUTH_None);
      Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_30);
      Script_PlayCharaMotion(CHARA_AKIR, 4131, 115.0f, 160.0f, 0.0f, 1.0f, 30);
      SCNF_WaitFrames_0000(60);
      Script_PlayCharaMotion(CHARA_AKIR, 4140, 10.0f, 110.0f, 0.0f, 1.0f, 30);
      SCNF_WaitFrames_0000(60);
      Script_charaDisplay(SHEN_NAME('KNIN'), CDM_Hide);
    }
    if (-(local_FC >= 50000) & -(local_F8 == 0) & -(local_F6 == 0)) {
      script_A297(12);
      Script_gachaponUtils(1, 0x4E494E4D, 5);
      Script_PlayCharaMotion(CHARA_AKIR, 4131, 119.0f, 120.0f, 0.0f, 1.0f, 30);
      Script_seqEnterEvtCam(7156, 0, AUTH_None);
      Script_scenCreateTask(script_B6BC, 1, 75);
      SCNF_WaitFrames_0000(20);
      Script_PlayCharaMotion(CHARA_MIKM, 4133, 0.0f, 180.0f, 0.0f, 1.0f, 30);
      SCNF_WaitFrames_0000(40);
      Script_hmdlApplyScale(SHEN_NAME('MNIN'), &data_1BBC0);
      local_B8.x = 0.121f;
      (&local_B8)->y = 0.11f;
      (&local_B8)->z = -0.077f;
      {
        Vector3i local_A0;
        local_A0.x = 29127;
        (&local_A0)->y = 22755;
        (&local_A0)->z = 50972;
        Script_EVT_GetPosition(CHARA_MIKM, 18, &local_AC, 0);
        Script_EVT_SetPosition(SHEN_NAME('MNIN'), POS_XYZ, &local_AC);
        Script_fixoAttachEx(SHEN_NAME('MNIN'), CHARA_MIKM, 18, &local_B8, &local_A0);
      }
      Script_charaDisplay(SHEN_NAME('MNIN'), CDM_Show);
      SCNF_WaitFrames_0000(45);
      Script_fixoClearAttachment(SHEN_NAME('MNIN'));
      SCNF_WaitFrames_0000(40);
      Script_mtwkGetSetRotation(SHEN_NAME('MNIN'), &local_C4, 0);
      local_C4.x = 0x4000;
      Script_EVT_SetRotation(SHEN_NAME('MNIN'), 0x38000000, &local_C4, 0);
      Script_seqEnterEvtCam(7157, 0, AUTH_None);
      Script_GetCassetteFlags(222, 1, 0, 0);
      SCNF_StartIconSystemMenu(222);
      Script_scenCreateTask(script_B6BC, 1, 61);
      SCNF_WaitFrames_0000(31);
      local_B8.x = 0.1f;
      (&local_B8)->y = 0.02f;
      (&local_B8)->z = -0.07f;
      {
        Vector3i local_A0;
        local_A0.x = 36408;
        (&local_A0)->y = 0;
        (&local_A0)->z = 19114;
        Script_EVT_GetPosition(CHARA_AKIR, 18, &local_AC, 0);
        Script_EVT_SetPosition(SHEN_NAME('MNIN'), POS_XYZ, &local_AC);
        Script_fixoAttachEx(SHEN_NAME('MNIN'), CHARA_AKIR, 18, &local_B8, &local_A0);
      }
      local_D0.x = 1.2f;
      (&local_D0)->y = 0.0f;
      (&local_D0)->z = -1.5f;
      Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &local_D0);
      local_C4.x = 0;
      (&local_C4)->y = 30947;
      (&local_C4)->z = 0;
      Script_EVT_SetRotation(CHARA_AKIR, 0x78000000, &local_C4, 0);
      Script_seqEnterEvtCam(7158, 0, AUTH_None);
      Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_30);
      Script_PlayCharaMotion(CHARA_AKIR, 4131, 115.0f, 160.0f, 0.0f, 1.0f, 30);
      SCNF_WaitFrames_0000(60);
      Script_PlayCharaMotion(CHARA_AKIR, 4140, 10.0f, 110.0f, 0.0f, 1.0f, 30);
      SCNF_WaitFrames_0000(40);
      Script_charaDisplay(SHEN_NAME('MNIN'), CDM_Hide);
      SCNF_WaitFrames_0000(20);
    }
    if (-(local_FC >= 50000) & -(local_F8 == 1) & -(local_F6 == 0)) {
      script_A297(12);
      Script_gachaponUtils(1, 0x4E494E4D, 5);
      Script_seqEnterEvtCam(7148, CHARA_MIKM, CHARA_MIKM);
      Script_scenCreateTask(script_B6BC, 1, 77);
      SCNF_WaitFrames_0000(102);
      Script_scenCreateTask(script_B6BC, 1, 60);
      SCNF_WaitFrames_0000(31);
      Script_seqEnterEvtCam(7156, 0, AUTH_None);
      Script_PlayCharaMotion(CHARA_AKIR, 4131, 110.0f, 111.0f, 0.0f, 1.0f, 30);
      Script_PlayCharaMotion(CHARA_MIKM, 4133, 0.0f, 180.0f, 0.0f, 1.0f, 30);
      SCNF_WaitFrames_0000(40);
      Script_hmdlApplyScale(SHEN_NAME('MNIN'), &data_1BBC0);
      local_B8.x = 0.121f;
      (&local_B8)->y = 0.11f;
      (&local_B8)->z = -0.077f;
      {
        Vector3i local_A0;
        local_A0.x = 29127;
        (&local_A0)->y = 22755;
        (&local_A0)->z = 50972;
        Script_EVT_GetPosition(CHARA_MIKM, 18, &local_AC, 0);
        Script_EVT_SetPosition(SHEN_NAME('MNIN'), POS_XYZ, &local_AC);
        Script_fixoAttachEx(SHEN_NAME('MNIN'), CHARA_MIKM, 18, &local_B8, &local_A0);
      }
      Script_charaDisplay(SHEN_NAME('MNIN'), CDM_Show);
      Script_scenCreateTask(script_B6BC, 1, 71);
      SCNF_WaitFrames_0000(50);
      Script_fixoClearAttachment(SHEN_NAME('MNIN'));
      SCNF_WaitFrames_0000(40);
      Script_mtwkGetSetRotation(SHEN_NAME('MNIN'), &local_C4, 0);
      local_C4.x = 0x4000;
      Script_EVT_SetRotation(SHEN_NAME('MNIN'), 0x38000000, &local_C4, 0);
      Script_seqEnterEvtCam(7157, 0, AUTH_None);
      Script_GetCassetteFlags(222, 1, 0, 0);
      SCNF_StartIconSystemMenu(222);
      Script_EVT_GetPosition(CHARA_MIKM, -1, &local_DC, ENTITY_XFORM_WORLD_SPACE);
      (&local_DC)->y = 1.0f;
      Script_SetCharaLookAtPosition(CHARA_AKIR, 1, &local_DC, 2);
      Script_scenCreateTask(script_B6BC, 1, 61);
      SCNF_WaitFrames_0000(31);
      local_B8.x = 0.1f;
      (&local_B8)->y = 0.02f;
      (&local_B8)->z = -0.07f;
      {
        Vector3i local_A0;
        local_A0.x = 36408;
        (&local_A0)->y = 0;
        (&local_A0)->z = 19114;
        Script_EVT_GetPosition(CHARA_AKIR, 18, &local_AC, 0);
        Script_EVT_SetPosition(SHEN_NAME('MNIN'), POS_XYZ, &local_AC);
        Script_fixoAttachEx(SHEN_NAME('MNIN'), CHARA_AKIR, 18, &local_B8, &local_A0);
      }
      Script_seqEnterEvtCam(7158, 0, AUTH_None);
      Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_30);
      Script_PlayCharaMotion(CHARA_AKIR, 4131, 115.0f, 160.0f, 0.0f, 1.0f, 30);
      SCNF_WaitFrames_0000(60);
      Script_PlayCharaMotion(CHARA_AKIR, 4140, 10.0f, 110.0f, 0.0f, 1.0f, 30);
      SCNF_WaitFrames_0000(40);
      Script_charaDisplay(SHEN_NAME('MNIN'), CDM_Hide);
      SCNF_WaitFrames_0000(20);
    }
    if (((local_FC >= 10000) & (local_F8 == 0)) | ((local_FC >= 50000) & (local_F6 == 0))) {
    } else {
      Script_seqEnterEvtCam(7144, 0, AUTH_None);
    }
    Script_scenCreateTask(script_B6BC, 1, 66);
    SCNF_WaitFrames_0000(60);
  }
  Script_executeAicaCommand(2720, 1, 30);
  Script_executeAicaCommand(2720, 2, 30);
  Script_executeAicaCommand(2720, 3, 30);
  Script_filmCheckOrSetTransitionUtil(20, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(20);
  Script_SetCharaMotionPlaybackRate(CHARA_MIKM, 1.0f);
  Script_SetCPAMFlags(CHARA_MIKM, 1);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -98312, 0, 0);
  Script_SetCharaLookAtPosition(CHARA_MIKM, -98312, 0, 0);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
  local_FA = Script_PlayerStatsUtils(PS_GetCharaItemCount, 0x4D4B494D);
  Script_PlayerStatsUtils(PS_SetCharaItemCount, 0x4D4B494D, local_FA + 1);
  Script_SetShouldSleepFlag(1);
  Script_TimedTransitionUtils(TIMED_TRANSITION_SET_ENABLED, 1);
  script_145CA();
  if (task_274 == 0) {
    script_D081();
    Script_ryoControl(PCCTL_MoveToHome);
  } else {
    Script_shenRequestGameModuleTransition(Script_GetSceneID(), SHEN_NAME('D000'), 26);
  }
  SCNF_WaitFrames_0000(1);
  return;
}

static void script_1193B(void) {
  int32_t unused_FC;
  int16_t local_FA;
  uint8_t unused_F8[2];
  Vector3 local_EC;
  Vector3 local_E0;
  Vector3 local_D4;
  Vector3 local_C8;
  Vector3 local_BC;
  Vector3 local_B0;
  Vector3 local_A4;
  Vector3 local_98;
  int32_t unused_8C[3];
  Vector3i local_80;
  Vector3i local_FF74;
  Script_SetCPAMFlags(CHARA_MIKM, 0);
  Script_EVT_GetPosition(SHEN_NAME('SLTM'), -1, &local_C8, ENTITY_XFORM_WORLD_SPACE);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_D4, ENTITY_XFORM_WORLD_SPACE);
  Script_EVT_GetPosition(CHARA_MIKM, -1, &local_B0, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(CHARA_MIKM, &local_80, 0x40000000);
  if (task_26C == 20) {
    local_BC.x = (&local_C8)->x;
    (&local_BC)->y = 0.0f;
    (&local_BC)->z = (&local_D4)->z - ((&local_C8)->z - (&local_D4)->z);
  } else {
    local_BC.x = (&local_D4)->x - ((&local_C8)->x - (&local_D4)->x);
    (&local_BC)->y = 0.0f;
    (&local_BC)->z = (&local_C8)->z;
  }
  Script_EVT_SetPosition(CHARA_MIKM, 0x78000000, &local_BC);
  Script_EVT_GetPosition(SHEN_NAME('SLTM'), -1, &local_A4, ENTITY_XFORM_WORLD_SPACE);
  SCNF_CHAR_SCNFApplyFacingTowardPoint(CHARA_MIKM, &local_A4, 0, 0.0f, 0, 0);
  Script_MoveObjectToObject(CHARA_MIKM, 0x4D544C53);
  Script_PlayCharaMotion(CHARA_MIKM, 4132, 120.0f, 119.0f, 120.0f, 1.0f, 30);
  Script_SetNewHandMotion(CHARA_MIKM, 1, 74, 0x41200000, 0x41700000, 0x41200000, 0, 1.0f, 0);
  local_D4.x = 0.6f;
  (&local_D4)->y = 0.0f;
  (&local_D4)->z = 1.1f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_D4);
  {
    int32_t local_FF70;
    local_FF70 = task_26C;
    switch (local_FF70) {
      case 1:
        script_A297(12);
        Script_gachaponUtils(1, 0x4548434D, 0);
        local_E0.x = 0.09f;
        (&local_E0)->y = 0.0f;
        (&local_E0)->z = -0.035f;
        local_FF74.x = 20935;
        (&local_FF74)->y = 0;
        (&local_FF74)->z = 11832;
        Script_EVT_GetPosition(CHARA_MIKM, 18, &local_EC, 0);
        Script_EVT_SetPosition(SHEN_NAME('MCHE'), POS_XYZ, &local_EC);
        Script_fixoAttachEx(SHEN_NAME('MCHE'), CHARA_MIKM, 18, &local_E0, &local_FF74);
        Script_hmdlApplyScale(SHEN_NAME('MCHE'), &data_1BBD8);
        Script_charaDisplay(SHEN_NAME('MCHE'), CDM_Show);
        break;
      case 5:
        script_A297(12);
        Script_gachaponUtils(1, 0x494C424D, 1);
        local_E0.x = 0.11f;
        (&local_E0)->y = -0.007f;
        (&local_E0)->z = -0.02f;
        local_FF74.x = 47331;
        (&local_FF74)->y = 0;
        (&local_FF74)->z = 14563;
        Script_EVT_GetPosition(CHARA_MIKM, 18, &local_EC, 0);
        Script_EVT_SetPosition(SHEN_NAME('MBLI'), POS_XYZ, &local_EC);
        Script_fixoAttachEx(SHEN_NAME('MBLI'), CHARA_MIKM, 18, &local_E0, &local_FF74);
        Script_hmdlApplyScale(SHEN_NAME('MBLI'), &data_1BBD8);
        Script_charaDisplay(SHEN_NAME('MBLI'), CDM_Show);
        break;
      case 10:
        script_A297(12);
        Script_gachaponUtils(1, 0x544C534D, 2);
        local_E0.x = 0.09f;
        (&local_E0)->y = 0.0f;
        (&local_E0)->z = -0.04f;
        local_FF74.x = 910;
        (&local_FF74)->y = 0x8000;
        (&local_FF74)->z = 60074;
        Script_EVT_GetPosition(CHARA_MIKM, 18, &local_EC, 0);
        Script_EVT_SetPosition(SHEN_NAME('MSLT'), POS_XYZ, &local_EC);
        Script_fixoAttachEx(SHEN_NAME('MSLT'), CHARA_MIKM, 18, &local_E0, &local_FF74);
        Script_hmdlApplyScale(SHEN_NAME('MSLT'), &data_1BBD8);
        Script_charaDisplay(SHEN_NAME('MSLT'), CDM_Show);
        break;
      case 20:
        script_A297(12);
        Script_gachaponUtils(1, 0x424B4A4D, 3);
        local_E0.x = 0.105f;
        (&local_E0)->y = -0.005f;
        (&local_E0)->z = -0.041f;
        local_FF74.x = 33314;
        (&local_FF74)->y = 0;
        (&local_FF74)->z = 30401;
        Script_EVT_GetPosition(CHARA_MIKM, 18, &local_EC, 0);
        Script_EVT_SetPosition(SHEN_NAME('MJKB'), POS_XYZ, &local_EC);
        Script_fixoAttachEx(SHEN_NAME('MJKB'), CHARA_MIKM, 18, &local_E0, &local_FF74);
        Script_hmdlApplyScale(SHEN_NAME('MJKB'), &data_1BBD8);
        Script_charaDisplay(SHEN_NAME('MJKB'), CDM_Show);
        break;
    }
  }
  script_B664(8, 2);
  script_100CF();
  Script_charaSetRefboardFlagTo(CHARA_MIKM, 1);
  Script_PlayCharaMotion(CHARA_MIKM, 4132, 120.0f, 415.0f, 120.0f, 1.0f, 30);
  SCNF_WaitFrames_0000(5);
  Script_seqEnterEvtCam(7131, SHEN_NAME('SLTM'), SHEN_NAME('SLTM'));
  Script_scenCreateTask(script_B6BC, 1, 69);
  SCNF_WaitFrames_0000(80);
  Script_EVT_GetPosition(CHARA_MIKM, -1, &local_A4, 0);
  Script_EVT_GetPosition(CHARA_MIKM, -1, &local_98, ENTITY_XFORM_WORLD_SPACE);
  (&local_98)->y = 1.5f;
  Script_SetCharaLookAtPosition(CHARA_AKIR, 1, &local_98, 0);
  SCNF_WaitFrames_0000(10);
  Script_seqEnterEvtCam(7132, SHEN_NAME('SLTM'), SHEN_NAME('SLTM'));
  Script_scenCreateTask(script_B6BC, 1, 59);
  Script_EVT_GetPosition(CHARA_MIKM, -1, &local_98, ENTITY_XFORM_WORLD_SPACE);
  (&local_98)->y = 1.5f;
  Script_SetCharaLookAtPosition(CHARA_AKIR, 1, &local_98, 2);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_98, 10);
  SCNF_CHAR_SCNFApplyFacingTowardPoint(CHARA_AKIR, &local_A4, 0, 0.0f, 0, 0);
  SCNF_WaitFrames_0000(20);
  Script_seqEnterEvtCam(7133, SHEN_NAME('SLTM'), SHEN_NAME('SLTM'));
  Script_scenCreateTask(script_B6BC, 1, 70);
  SCNF_WaitFrames_0000(90);
  {
    int32_t local_FF70;
    local_FF70 = task_26C;
    switch (local_FF70) {
      case 1:
        Script_GetCassetteFlags(211, 1, 0, 0);
        SCNF_StartIconSystemMenu(211);
        break;
      case 5:
        Script_GetCassetteFlags(220, 1, 0, 0);
        SCNF_StartIconSystemMenu(220);
        break;
      case 10:
        Script_GetCassetteFlags(213, 1, 0, 0);
        SCNF_StartIconSystemMenu(213);
        break;
      case 20:
        Script_GetCassetteFlags(212, 1, 0, 0);
        SCNF_StartIconSystemMenu(212);
        break;
    }
  }
  Script_scenCreateTask(script_B6BC, 1, 57);
  SCNF_WaitFrames_0000(30);
  SCNF_WaitFrames_0000(30);
  script_C21C(10);
  script_F781();
  Script_SetCharaLookAtPosition(CHARA_AKIR, -98312, 0, 0);
  Script_SetCharaLookAtPosition(CHARA_MIKM, -98312, 0, 0);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
  Script_FaceLookAtTargetUtils(CHARA_MIKM, FACE_LookReset);
  {
    int32_t local_FF70;
    local_FF70 = task_26C;
    switch (local_FF70) {
      case 1:
        Script_charaDisplay(SHEN_NAME('MCHE'), CDM_Hide);
        break;
      case 5:
        Script_charaDisplay(SHEN_NAME('MBLI'), CDM_Hide);
        break;
      case 10:
        Script_charaDisplay(SHEN_NAME('MSLT'), CDM_Hide);
        break;
      case 20:
        Script_charaDisplay(SHEN_NAME('MJKB'), CDM_Hide);
        break;
    }
  }
  local_FA = Script_PlayerStatsUtils(PS_GetCharaItemCount, 0x4D4B494D);
  Script_PlayerStatsUtils(PS_SetCharaItemCount, 0x4D4B494D, local_FA + 1);
  Script_charaSetRefboardFlagTo(CHARA_MIKM, 2);
  Script_SetCPAMFlags(CHARA_MIKM, 1);
  Script_MoveObjectToObject(CHARA_MIKM, -1);
  Script_EVT_SetPosition(CHARA_MIKM, 0x78000000, &local_B0);
  Script_EVT_SetRotation(CHARA_MIKM, 0x78000000, &local_80, 0);
  Script_PlayCharaMotion(CHARA_MIKM, 4155, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static void script_120AE(void) {
  int8_t local_FF;
  int8_t local_FE;
  int8_t local_FD;
  int8_t local_FC;
  int8_t local_FB;
  int8_t local_FA;
  int16_t local_F8;
  int32_t unused_F0[2];
  int32_t local_EC;
  int32_t local_E8;
  local_EC = 0;
  local_E8 = 0;
  task_384 = 0;
  *(&task_384 + 1) = 0;
  *(&task_384 + 2) = 0;
  *(&task_384 + 3) = 0;
  *(&task_384 + 4) = 0;
  *(&task_384 + 5) = 0;
  *(&task_384 + 6) = 0;
  *(&task_384 + 7) = 0;
  *(&task_384 + 8) = 0;
  *(&task_384 + 9) = 0;
  *(&task_384 + 10) = 0;
  *(&task_384 + 11) = 0;
  *(&task_384 + 12) = 0;
  task_254 = 0;
  *(&task_254 + 1) = 0;
  *(&task_254 + 2) = 0;
  *(&task_254 + 3) = 0;
  *(&task_254 + 4) = 0;
  *(&task_254 + 5) = 0;
  Script_timeSetUpdateEnabled(0);
  while (local_EC < 60000) {
    local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 66.0f);
    local_FB = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 66.0f);
    local_FA = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 66.0f);
    local_F8 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 200.0f);
    if (*&task_224 == 1) {
      local_FF = *((int8_t *)data_19AE0 + local_FC);
      local_FE = *((int8_t *)data_19AE0 + (local_FB + 66));
      local_FD = *((int8_t *)data_19AE0 + (local_FA + 132));
      task_254 = *&task_254 + 1;
    }
    if (*(&task_224 + 1) == 1) {
      local_FF = data_19BA8[local_FC];
      local_FE = data_19BA8[local_FB + 66];
      local_FD = data_19BA8[local_FA + 132];
      *(&task_254 + 1) = *(&task_254 + 1) + 1;
    }
    if (*(&task_224 + 2) == 1) {
      local_FF = data_19C70[local_FC];
      local_FE = data_19C70[local_FB + 66];
      local_FD = data_19C70[local_FA + 132];
      *(&task_254 + 2) = *(&task_254 + 2) + 1;
    }
    if (*(&task_224 + 3) == 1) {
      local_FF = data_19D38[local_FC];
      local_FE = data_19D38[local_FB + 66];
      local_FD = data_19D38[local_FA + 132];
      *(&task_254 + 3) = *(&task_254 + 3) + 1;
    }
    if (*(&task_224 + 4) == 1) {
      local_FF = data_19E00[local_FC];
      local_FE = data_19E00[local_FB + 66];
      local_FD = data_19E00[local_FA + 132];
      *(&task_254 + 4) = *(&task_254 + 4) + 1;
    }
    if (*(&task_224 + 5) == 1) {
      local_FF = data_19EC8[local_FC];
      local_FE = data_19EC8[local_FB + 66];
      local_FD = data_19EC8[local_FA + 132];
      *(&task_254 + 5) = *(&task_254 + 5) + 1;
    }
    if (*&task_23C > local_F8) {
      task_224 = 1;
      *(&task_224 + 1) = 0;
      *(&task_224 + 2) = 0;
      *(&task_224 + 3) = 0;
      *(&task_224 + 4) = 0;
      *(&task_224 + 5) = 0;
    }
    if (*(&task_23C + 1) > local_F8) {
      task_224 = 0;
      *(&task_224 + 1) = 1;
      *(&task_224 + 2) = 0;
      *(&task_224 + 3) = 0;
      *(&task_224 + 4) = 0;
      *(&task_224 + 5) = 0;
    }
    if (*(&task_23C + 2) > local_F8) {
      task_224 = 0;
      *(&task_224 + 1) = 0;
      *(&task_224 + 2) = 1;
      *(&task_224 + 3) = 0;
      *(&task_224 + 4) = 0;
      *(&task_224 + 5) = 0;
    }
    if (*(&task_23C + 3) > local_F8) {
      task_224 = 0;
      *(&task_224 + 1) = 0;
      *(&task_224 + 2) = 0;
      *(&task_224 + 3) = 1;
      *(&task_224 + 4) = 0;
      *(&task_224 + 5) = 0;
    }
    if (*(&task_23C + 4) > local_F8) {
      task_224 = 0;
      *(&task_224 + 1) = 0;
      *(&task_224 + 2) = 0;
      *(&task_224 + 3) = 0;
      *(&task_224 + 4) = 1;
      *(&task_224 + 5) = 0;
    }
    if (*(&task_23C + 5) > local_F8) {
      task_224 = 0;
      *(&task_224 + 1) = 0;
      *(&task_224 + 2) = 0;
      *(&task_224 + 3) = 0;
      *(&task_224 + 4) = 0;
      *(&task_224 + 5) = 1;
    }
    script_1287A(local_FF, local_FE, local_FD);
    Script_PrintDebugString(25, 12, "LOOP   : %6d", local_EC);
    Script_PrintDebugString(25, 13, "CHERRY1: %6d", *&task_384);
    Script_PrintDebugString(25, 14, "CHERRY2: %6d", *(&task_384 + 1));
    Script_PrintDebugString(25, 15, "CHERRY3: %6d", *(&task_384 + 2));
    Script_PrintDebugString(25, 16, "ENY_BAR: %6d", *(&task_384 + 3));
    Script_PrintDebugString(25, 17, "BAR    : %6d", *(&task_384 + 4));
    Script_PrintDebugString(25, 18, "ENY_7  : %6d", *(&task_384 + 5));
    Script_PrintDebugString(25, 19, "SUPER_B: %6d", *(&task_384 + 6));
    Script_PrintDebugString(25, 20, "GREEN_7: %6d", *(&task_384 + 7));
    Script_PrintDebugString(25, 21, "RED_7_1: %6d", *(&task_384 + 8));
    Script_PrintDebugString(25, 22, "RED_7_2: %6d", *(&task_384 + 9));
    Script_PrintDebugString(25, 23, "RED_7_3: %6d", *(&task_384 + 10));
    Script_PrintDebugString(25, 24, "RED_7_4: %6d", *(&task_384 + 11));
    Script_PrintDebugString(25, 25, "RED_7_5: %6d", *(&task_384 + 12));
    Script_PrintDebugString(0, 26, "TBL A:%5d B:%5d C:%5d", *&task_254, *(&task_254 + 1), *(&task_254 + 2));
    Script_PrintDebugString(0, 27, "    D:%5d E:%5d F:%5d", *(&task_254 + 3), *(&task_254 + 4), *(&task_254 + 5));
    local_EC = local_EC + 1;
    SCNF_WaitFrames_0000(1);
  }
  local_E8 = *&task_384 * 2 + *(&task_384 + 1) * 5 + *(&task_384 + 2) * 10 + *(&task_384 + 3) * 20 + *(&task_384 + 4) * 50 + *(&task_384 + 5) * 50 + *(&task_384 + 6) * 100 + *(&task_384 + 7) * 200 + *(&task_384 + 8) * 300 + *(&task_384 + 9) * 500 + *(&task_384 + 10) * 1000 + *(&task_384 + 11) * 1500 + *(&task_384 + 12) * 3000;
  Script_sprintf("CHERRY1: %6d \012", *&task_384);
  Script_sprintf("CHERRY2: %6d \012", *(&task_384 + 1));
  Script_sprintf("CHERRY3: %6d \012", *(&task_384 + 2));
  Script_sprintf("ENY_BAR: %6d \012", *(&task_384 + 3));
  Script_sprintf("BAR    : %6d \012", *(&task_384 + 4));
  Script_sprintf("ENY_7  : %6d \012", *(&task_384 + 5));
  Script_sprintf("SUPER_B: %6d \012", *(&task_384 + 6));
  Script_sprintf("GREEN_7: %6d \012", *(&task_384 + 7));
  Script_sprintf("RED_7_1: %6d \012", *(&task_384 + 8));
  Script_sprintf("RED_7_2: %6d \012", *(&task_384 + 9));
  Script_sprintf("RED_7_3: %6d \012", *(&task_384 + 10));
  Script_sprintf("RED_7_4: %6d \012", *(&task_384 + 11));
  Script_sprintf("RED_7_5: %6d \012", *(&task_384 + 12));
  Script_sprintf("TOTAL  : %10d \012", local_E8);
  Script_sprintf("TBL A  : %5d \012", *&task_254);
  Script_sprintf("TBL B  : %5d \012", *(&task_254 + 1));
  Script_sprintf("TBL C  : %5d \012", *(&task_254 + 2));
  Script_sprintf("TBL D  : %5d \012", *(&task_254 + 3));
  Script_sprintf("TBL E  : %5d \012", *(&task_254 + 4));
  Script_sprintf("TBL F  : %5d \012", *(&task_254 + 5));
  return;
}

static void script_1287A(int8_t arg0, int8_t arg1, int8_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  int16_t local_F6;
  int8_t local_F5;
  int8_t local_F4;
  int8_t local_F3;
  int8_t local_F2;
  int8_t local_F1;
  int32_t local_EC;
  local_FC = 0;
  local_F2 = 1;
  local_F5 = 1;
  local_F4 = 1;
  local_F3 = 1;
  while (local_F2 <= script_B674(14)) {
    local_F1 = 0;
    local_F6 = 0;
    local_F8 = data_19F90[arg0 + local_F5] * 100 + data_19FAC[arg1 + local_F4] * 10 + data_19FC8[arg2 + local_F3];
    if (local_F8 == 111) {
      *(&task_384 + 2) = *(&task_384 + 2) + 1;
      local_F6 = 1;
    } else if ((local_F8 >= 110) & (local_F8 <= 119)) {
      *(&task_384 + 1) = *(&task_384 + 1) + 1;
      local_F6 = 18;
    } else {
      if ((local_F8 >= 100) & (local_F8 <= 199)) {
        task_384 = *&task_384 + 1;
        local_F6 = 19;
      }
    }
    if (local_F8 == 222) {
      *(&task_384 + 4) = *(&task_384 + 4) + 1;
      local_F6 = 2;
    }
    if (local_F8 == 333) {
      *(&task_384 + 6) = *(&task_384 + 6) + 1;
      local_F6 = 9;
    }
    if (local_F8 == 444) {
      *(&task_384 + 7) = *(&task_384 + 7) + 1;
      local_F6 = 10;
    }
    if (local_F8 == 223) {
      *(&task_384 + 3) = *(&task_384 + 3) + 1;
      local_F6 = 3;
    }
    if (local_F8 == 232) {
      *(&task_384 + 3) = *(&task_384 + 3) + 1;
      local_F6 = 3;
    }
    if (local_F8 == 233) {
      *(&task_384 + 3) = *(&task_384 + 3) + 1;
      local_F6 = 3;
    }
    if (local_F8 == 322) {
      *(&task_384 + 3) = *(&task_384 + 3) + 1;
      local_F6 = 3;
    }
    if (local_F8 == 323) {
      *(&task_384 + 3) = *(&task_384 + 3) + 1;
      local_F6 = 3;
    }
    if (local_F8 == 332) {
      *(&task_384 + 3) = *(&task_384 + 3) + 1;
      local_F6 = 3;
    }
    if (local_F8 == 445) {
      *(&task_384 + 5) = *(&task_384 + 5) + 1;
      local_F6 = 11;
    }
    if (local_F8 == 454) {
      *(&task_384 + 5) = *(&task_384 + 5) + 1;
      local_F6 = 11;
    }
    if (local_F8 == 455) {
      *(&task_384 + 5) = *(&task_384 + 5) + 1;
      local_F6 = 11;
    }
    if (local_F8 == 544) {
      *(&task_384 + 5) = *(&task_384 + 5) + 1;
      local_F6 = 11;
    }
    if (local_F8 == 545) {
      *(&task_384 + 5) = *(&task_384 + 5) + 1;
      local_F6 = 11;
    }
    if (local_F8 == 554) {
      *(&task_384 + 5) = *(&task_384 + 5) + 1;
      local_F6 = 11;
    }
    if ((local_F8 == 555) & (local_F2 == 1)) {
      *(&task_384 + 8) = *(&task_384 + 8) + 1;
      local_F6 = 20;
    }
    if ((local_F8 == 555) & (local_F2 == 2)) {
      *(&task_384 + 9) = *(&task_384 + 9) + 1;
      local_F6 = 21;
    }
    if ((local_F8 == 555) & (local_F2 == 3)) {
      *(&task_384 + 10) = *(&task_384 + 10) + 1;
      local_F6 = 22;
    }
    if ((local_F8 == 555) & (local_F2 == 4)) {
      *(&task_384 + 11) = *(&task_384 + 11) + 1;
      local_F6 = 23;
    }
    if ((local_F8 == 555) & (local_F2 == 5)) {
      *(&task_384 + 12) = *(&task_384 + 12) + 1;
      local_F6 = 24;
    }
    local_EC = local_F2;
    switch (local_EC) {
      case 1:
        local_F5 = 2;
        local_F4 = 2;
        local_F3 = 2;
        break;
      case 2:
        local_F5 = 0;
        local_F4 = 0;
        local_F3 = 0;
        break;
      case 3:
        local_F5 = 2;
        local_F4 = 1;
        local_F3 = 0;
        break;
      case 4:
        local_F5 = 0;
        local_F4 = 1;
        local_F3 = 2;
        break;
      case 5:
        break;
    }
    local_FC = local_FC + *(int16_t *)((int8_t *)&data_19AAC + (local_F6 << 1));
    local_F2 = local_F2 + 1;
  }
  return;
}

static int32_t script_12CC7(void) {
  int8_t local_FF;
  uint8_t unused_FE;
  int16_t local_FC;
  int16_t local_FA;
  uint8_t unused_F0[10];
  int32_t local_EC;
  int32_t local_E8;
  int32_t local_E4;
  int32_t unused_E0;
  GameDateTime local_D8;
  int32_t local_D4;
  Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_1BDF4);
  local_E4 = 1;
  SCNF_WaitFrames_0000(1);
  local_FC = 23;
  local_FA = 30;
  if (task_274 == 1) {
    local_FC = 12;
    local_FA = 30;
  }
  if (task_274 == 2) {
    local_FC = 12;
    local_FA = 0;
  }
  if (task_274 == 3) {
    local_FC = 23;
    local_FA = 59;
  }
  Script_sprintf("SCN_CNG: %d\012", task_274);
  for (;;) {
    local_E8 = Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex);
    local_EC = Script_srchUtils(-1);
    if (local_E8 == -1) {
      if (local_E4 == 0) {
        Script_SetTaskExecutionSuspended((void *)task_1A4, 0);
        local_E4 = 1;
      }
    } else {
      if (local_E4 == 1) {
        Script_SetTaskExecutionSuspended((void *)task_1A4, 1);
        local_E4 = 0;
      }
    }
    local_D4 = local_EC;
    switch (local_D4) {
      case 0:
        Script_sicnUtils(1, 0, 0);
        task_1A2 = 13;
        local_FF = Script_PlayerStatsUtils(PS_GetStepSlot, 13);
        Script_scenCreateTask(script_A9F2, 1, local_FF);
        script_A8FC(SHEN_NAME('SLT1'), 10);
        Script_sicnUtils(0, 5, 1);
        break;
      case 1:
        Script_sicnUtils(1, 0, 0);
        task_1A2 = 12;
        local_FF = Script_PlayerStatsUtils(PS_GetStepSlot, 12);
        Script_scenCreateTask(script_A9F2, 1, local_FF);
        script_A8FC(SHEN_NAME('SLT2'), 10);
        Script_sicnUtils(0, 5, 1);
        break;
      case 2:
        Script_sicnUtils(1, 0, 0);
        task_1A2 = 11;
        local_FF = Script_PlayerStatsUtils(PS_GetStepSlot, 11);
        Script_scenCreateTask(script_A9F2, 1, local_FF);
        script_A8FC(SHEN_NAME('SLT3'), 10);
        Script_sicnUtils(0, 5, 1);
        break;
      case 3:
        Script_sicnUtils(1, 0, 0);
        task_1A2 = 10;
        local_FF = Script_PlayerStatsUtils(PS_GetStepSlot, 10);
        Script_scenCreateTask(script_A9F2, 1, local_FF);
        script_A8FC(SHEN_NAME('SLT4'), 10);
        Script_sicnUtils(0, 5, 1);
        break;
      case 4:
        Script_sicnUtils(1, 0, 0);
        task_1A2 = 6;
        local_FF = Script_PlayerStatsUtils(PS_GetStepSlot, 6);
        Script_scenCreateTask(script_A9F2, 1, local_FF);
        script_A8FC(SHEN_NAME('SLT5'), 5);
        Script_sicnUtils(0, 5, 1);
        break;
      case 5:
        Script_sicnUtils(1, 0, 0);
        task_1A2 = 7;
        local_FF = Script_PlayerStatsUtils(PS_GetStepSlot, 7);
        Script_scenCreateTask(script_A9F2, 1, local_FF);
        script_A8FC(SHEN_NAME('SLT6'), 5);
        Script_sicnUtils(0, 5, 1);
        break;
      case 6:
        Script_sicnUtils(1, 0, 0);
        task_1A2 = 8;
        local_FF = Script_PlayerStatsUtils(PS_GetStepSlot, 8);
        Script_scenCreateTask(script_A9F2, 1, local_FF);
        script_A8FC(SHEN_NAME('SLT7'), 5);
        Script_sicnUtils(0, 5, 1);
        break;
      case 7:
        Script_sicnUtils(1, 0, 0);
        task_1A2 = 9;
        local_FF = Script_PlayerStatsUtils(PS_GetStepSlot, 9);
        Script_scenCreateTask(script_A9F2, 1, local_FF);
        script_A8FC(SHEN_NAME('SLT8'), 5);
        Script_sicnUtils(0, 5, 1);
        break;
      case 8:
        Script_sicnUtils(1, 0, 0);
        task_1A2 = 15;
        local_FF = Script_PlayerStatsUtils(PS_GetStepSlot, 15);
        Script_scenCreateTask(script_A9F2, 1, local_FF);
        script_A8FC(SHEN_NAME('SLT9'), 20);
        Script_sicnUtils(0, 5, 1);
        break;
      case 9:
        Script_sicnUtils(1, 0, 0);
        task_1A2 = 14;
        local_FF = Script_PlayerStatsUtils(PS_GetStepSlot, 14);
        Script_scenCreateTask(script_A9F2, 1, local_FF);
        script_A8FC(SHEN_NAME('SLTA'), 20);
        Script_sicnUtils(0, 5, 1);
        break;
      case 10:
        Script_sicnUtils(1, 0, 0);
        task_1A2 = 3;
        local_FF = Script_PlayerStatsUtils(PS_GetStepSlot, 3);
        Script_scenCreateTask(script_A9F2, 1, local_FF);
        script_A8FC(SHEN_NAME('SLTB'), 1);
        Script_sicnUtils(0, 5, 1);
        break;
      case 11:
        Script_sicnUtils(1, 0, 0);
        task_1A2 = 4;
        local_FF = Script_PlayerStatsUtils(PS_GetStepSlot, 4);
        Script_scenCreateTask(script_A9F2, 1, local_FF);
        script_A8FC(SHEN_NAME('SLTC'), 1);
        Script_sicnUtils(0, 5, 1);
        break;
      case 12:
        Script_sicnUtils(1, 0, 0);
        task_1A2 = 5;
        local_FF = Script_PlayerStatsUtils(PS_GetStepSlot, 5);
        Script_scenCreateTask(script_A9F2, 1, local_FF);
        script_A8FC(SHEN_NAME('SLTD'), 1);
        Script_sicnUtils(0, 5, 1);
        break;
      case 13:
        Script_sicnUtils(1, 0, 0);
        task_1A2 = 2;
        local_FF = Script_PlayerStatsUtils(PS_GetStepSlot, 2);
        Script_scenCreateTask(script_A9F2, 1, local_FF);
        script_A8FC(SHEN_NAME('SLTE'), 1);
        Script_sicnUtils(0, 5, 1);
        break;
      case 14:
        Script_sicnUtils(1, 0, 0);
        task_1A2 = 1;
        local_FF = Script_PlayerStatsUtils(PS_GetStepSlot, 1);
        Script_scenCreateTask(script_A9F2, 1, local_FF);
        script_A8FC(SHEN_NAME('SLTF'), 1);
        Script_sicnUtils(0, 5, 1);
        break;
      case 15:
        Script_sicnUtils(1, 0, 0);
        task_1A2 = 0;
        local_FF = Script_PlayerStatsUtils(PS_GetStepSlot, 0);
        Script_scenCreateTask(script_A9F2, 1, local_FF);
        script_A8FC(SHEN_NAME('SLTG'), 1);
        Script_sicnUtils(0, 5, 1);
        break;
      case 16:
        break;
      case 17:
        break;
      case 18:
        break;
      case 19:
        break;
    }
    Script_ReturnTimeDate(&local_D8);
    if ((((&local_D8)->hours >= local_FC) & ((&local_D8)->minutes >= local_FA)) | ((&local_D8)->hours <= 4)) {
      Script_srchUtils(SRCH_SCRIPT_Reset);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_13321(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  return;
}

static void script_13349(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
  return;
}

static void script_1339D(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static int32_t script_133ED(int32_t arg0) {
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

static void script_13494(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_133ED(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static int32_t script_134B7(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = (&local_F4)->hours * 60 + (&local_F4)->minutes;
  local_EC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, arg0) * 6;
  return local_F0 - local_EC;
}

static void script_134EA(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_134B7(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_1350D(int32_t arg0, int32_t arg1) {
  int8_t local_F4[12];
  int32_t local_F0;
  local_F0 = script_134B7(arg0);
  local_F4[4] = local_F0 / 60;
  local_F4[5] = local_F0 % 60;
  if ((local_F4[4] < *(int8_t *)(arg1 + 4)) | ((local_F4[4] == *(int8_t *)(arg1 + 4)) & (local_F4[5] < *(int8_t *)(arg1 + 5)))) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_1356C(int32_t arg0) {
  int32_t local_F8[2];
  local_F8[0] = arg0;
  local_F8[1] = 0;
  (void)local_F8;
  return;
}

static void script_13581(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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

static void script_13917(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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

__attribute__((scn_string_bindings(0, data_1C108)))
static void script_13FA5(void) {
  Script_sprintf("TUNED FreeTalk def\012");
  SCNT_Enqueue();
  return;
}

__attribute__((scn_string_bindings(0, data_1C11C)))
static void script_13FB4(void) {
  Script_sprintf("TUNED FreeTalk def\012");
  SCNT_Enqueue();
  return;
}

static void script_13FC3(void) {
  return;
}

static void script_13FC6(void) {
  return;
}

static void script_13FC9(void) {
  return;
}

static void script_13FCC(void) {
  return;
}

static void script_13FCF(void) {
  (void)((task_3C4 = 0) & 0x20);
  return;
}

static void script_13FDC(void) {
  task_3B8 = Script_freeTalkGetCurrentChara();
  return;
}

static void script_13FE8(void) {
  *(&task_3B8 + 1) = Script_GetFreeTalkStatus();
  return;
}

static int32_t script_13FF7(void) {
  return *(&task_3B8 + 2) = Script_freeTalkGetAndResetState();
}

static int32_t script_14006(void) {
  int8_t local_FF;
  int32_t local_F8;
  script_13FA5();
  for (;;) {
    local_FF = script_13FF7();
    local_F8 = local_FF;
    if (local_FF != -1) {
      script_13FCF();
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

void script_14037(void) {
  int16_t local_FE;
  if (Script_ryoSleepUtils(0)) {
    task_1A0 = 0;
  } else {
    task_1A0 = 1;
  }
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  SCNF_WaitFrames_0000(5);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  Script_WeatherUtils(WeatherScript_ApplyWccpClipSet1, 5.8944f, 0.4495f, -0.1689f, 0.1263f, -0.0986f, -9.1673f, 5.8528f, -9.1696f, -0.3f, 3.0f);
  Script_WeatherTaskUtil(ToggleFogTask, 0);
  Script_WeatherTaskUtil(ToggleSceneLightingFlags, 0);
  Script_SetupLightFromTable(0);
  Script_ProcessFogFromTable(0);
  Script_PREP_Related(-1, 1);
  Script_PREP_Related(1, 0);
  Script_mapmSetCamFlags(0, 1);
  Script_SetGlobalSceneSlotValue34(1, 1);
  Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 54);
  Script_executeAicaCommand(4256, 4, 127);
  Script_Ambient1SoundUtils(1, 24232);
  script_7FBB(CHARA_AKIR, (DSWKDoorEntry *)&data_1C078, (int32_t *)data_1C0B0, (DoorResourceDescriptor *)&data_1C0C4, (MapObjectRecord *)&data_1C0D4, 1, 0);
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  DSWK_DoorUtils(DOOR_CMD_SET_STATE_1);
  SCNF_WaitFrames_0000(1);
  local_FE = Script_GetSceneID();
  Script_motiLoad(local_FE, SHEN_NAME('FREE'), "M_MOBJ.BIN");
  Script_LoadVoiceFile(1, "E1024");
  Script_scenCreateTask(script_A0FE, 1, CHARA_AKIR);
  Script_scenCreateTask(script_142F9, 0);
  Script_scenCreateTask(script_B043, 0);
  Script_scenCreateTask(script_1424D, 0);
  SCNF_WaitFrames_0000(1);
  Script_scenCreateTask(script_14006, 0);
  SCNF_WaitFrames_0000(1);
  task_1A4 = Script_scenCreateTask(script_1598B, 0);
  SCNF_WaitFrames_0000(1);
  Script_scenCreateTask(script_12CC7, 0);
  SCNF_WaitFrames_0000(1);
  Script_filmCheckOrSetTransitionUtil(32, 255, 0, 0, 0, 0, 0, 0, 0);
  script_8381(0);
  Script_sprintf("REJUM-:  %d \012", task_1A0);
  if (task_1A0 == 0) {
    Script_ryoSleepUtils(1);
  }
  Script_ryoSleepUtils(2, 1);
  return;
}

static int32_t script_1424D(void) {
  int32_t local_FC;
  for (;;) {
    if ((&task_10)->mode) {
      local_FC = task_10.entry_idx;
      if (local_FC == 0) {
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_mapmSetRenderFlag(1, 1);
        SCNF_WaitFrames_0000(120);
        Script_executeAicaCommand(2720, 1, 30);
        Script_executeAicaCommand(2720, 2, 30);
        Script_executeAicaCommand(2720, 3, 30);
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        script_145CA();
        Script_shenRequestGameModuleTransition(Script_GetSceneID(), SHEN_NAME('D000'), 26);
        SCNF_WaitFrames_0000(1);
        goto L0142ED;
      }
    }
    L0142ED:;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_142F9(void) {
  int16_t local_FE;
  int16_t local_FC;
  Vector3 local_F0;
  local_F0.x = 4.7f;
  (&local_F0)->y = 0.0f;
  (&local_F0)->z = 0.0f;
  local_FC = 0;
  Script_mapmSetRenderFlag(1, 0);
  Script_charaVisUtils(SHEN_NAME('SLT1'), 0);
  Script_charaVisUtils(SHEN_NAME('SLT2'), 0);
  Script_charaVisUtils(SHEN_NAME('SLT3'), 0);
  Script_charaVisUtils(SHEN_NAME('SLT4'), 0);
  Script_charaVisUtils(SHEN_NAME('SLT5'), 0);
  Script_charaVisUtils(SHEN_NAME('SLT6'), 0);
  Script_charaVisUtils(SHEN_NAME('SLT7'), 0);
  Script_charaVisUtils(SHEN_NAME('SLT8'), 0);
  Script_charaVisUtils(SHEN_NAME('SLT9'), 0);
  Script_charaVisUtils(SHEN_NAME('SLTA'), 0);
  Script_charaVisUtils(SHEN_NAME('SLTB'), 0);
  Script_charaVisUtils(SHEN_NAME('SLTC'), 0);
  Script_charaVisUtils(SHEN_NAME('SLTD'), 0);
  Script_charaVisUtils(SHEN_NAME('SLTE'), 0);
  Script_charaVisUtils(SHEN_NAME('SLTF'), 0);
  Script_charaVisUtils(SHEN_NAME('SLTG'), 0);
  while (1) {
    {
      Vector3i local_E4;
      Script_mtwkGetSetRotation(SHEN_NAME('FAN1'), &local_E4, 0);
      (&local_E4)->y = (&local_E4)->y - 364;
      Script_EVT_SetRotation(SHEN_NAME('FAN1'), 0x38000000, &local_E4, 0);
    }
    {
      Vector3i local_E4;
      Script_mtwkGetSetRotation(SHEN_NAME('FAN2'), &local_E4, 0);
      (&local_E4)->y = (&local_E4)->y - 364;
      Script_EVT_SetRotation(SHEN_NAME('FAN2'), 0x38000000, &local_E4, 0);
    }
    {
      Vector3i local_E4;
      Script_mtwkGetSetRotation(SHEN_NAME('FAN3'), &local_E4, 0);
      (&local_E4)->y = (&local_E4)->y - 364;
      Script_EVT_SetRotation(SHEN_NAME('FAN3'), 0x38000000, &local_E4, 0);
    }
    {
      Vector3i local_E4;
      Script_mtwkGetSetRotation(SHEN_NAME('FAN4'), &local_E4, 0);
      (&local_E4)->y = (&local_E4)->y - 364;
      Script_EVT_SetRotation(SHEN_NAME('FAN4'), 0x38000000, &local_E4, 0);
    }
    local_FE = Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_F0, 1.0f);
    if (local_FE == 0) {
      if (local_FC == 1) {
        Script_mapmSetRenderFlag(1, 0);
        local_FC = 0;
      }
    } else {
      if (local_FC == 0) {
        Script_mapmSetRenderFlag(1, 1);
        local_FC = 1;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static int32_t script_1450C(int32_t arg0, const Vector3 *arg1, float arg2, float arg3) {
  int32_t local_FC;
  Vector3 local_F0;
  int32_t unused_E4[3];
  Vector3i local_D8;
  Script_EVT_GetPosition(arg0, -1, &local_F0, 0);
  Script_mtwkGetSetRotation(arg0, &local_D8, 0x2000000);
  local_FC = Script_CheckTargetWithinAngleRange(&local_F0, arg2, (&local_D8)->y, (int32_t)(arg3 / 360.0f * (float)0x10000), arg1);
  return local_FC;
}

static void script_14550(void) {
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x4D4B494D, 1);
  task_1A8 = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x4D4B494D);
  while (task_1A8 == 0) {
    Script_sprintf(" MITU load error \012");
    SCNF_WaitFrames_0000(1);
    task_1A8 = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x4D4B494D);
  }
  L0145AB:;
  SCNF_WaitFrames_0000(1);
  if (!Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, task_1A8)) goto L0145AB;
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

static void script_145CA(void) {
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x4D4B494D, 0);
  SCNF_WaitFrames_0000(1);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

static void script_145F5(void) {
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x4E4B594D, 1);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x524D534E, 1);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

static void script_14626(void) {
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x4E4B594D, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x524D534E, 0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

static void script_14657(void) {
  int32_t local_FC;
  int16_t local_FA;
  Vector3 local_EC;
  Vector3 local_E0;
  Vector3 local_D4;
  Vector3i local_C8;
  local_FC = Script_shenGetStats(STATS_SEGA_COINS);
  if (local_FC >= 1) {
    script_145F5();
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
    Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
    Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
    Script_shenClearFreeState(0);
    local_D4.x = 4.7f;
    (&local_D4)->y = 0.0f;
    (&local_D4)->z = -1.5f;
    Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &local_D4);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_SetCharaRefBoardPos(CHARA_AKIR, &data_1C164);
    Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
    local_D4.x = 1.7f;
    (&local_D4)->y = 0.0f;
    (&local_D4)->z = -0.45f;
    Script_EVT_SetPosition(CHARA_MIKM, 0x78000000, &local_D4);
    local_C8.x = 0;
    (&local_C8)->y = 0;
    (&local_C8)->z = 0;
    Script_EVT_SetRotation(CHARA_MIKM, 0x78000000, &local_C8, 0);
    Script_PlayCharaMotion(CHARA_MIKM, 4155, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_SetCharaRefBoardPos(CHARA_MIKM, &data_1C164);
    Script_charaSetRefboardFlagTo(CHARA_MIKM, 1);
    Script_seqEnterEvtCam(7145, 0, AUTH_None);
    SCNF_WaitFrames_0000(30);
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_EC, ENTITY_XFORM_WORLD_SPACE);
    (&local_EC)->y = 1.5f;
    Script_SetCharaLookAtPosition(CHARA_MIKM, 1, &local_EC, 0);
    SCNF_WaitFrames_0000(10);
    Script_scenCreateTask(script_B6BC, 1, 68);
    SCNF_WaitFrames_0000(50);
    Script_EVT_GetPosition(CHARA_MIKM, -1, &local_E0, ENTITY_XFORM_WORLD_SPACE);
    (&local_E0)->y = 1.5f;
    Script_SetCharaLookAtPosition(CHARA_AKIR, 1, &local_E0, 2);
    SCNF_WaitFrames_0000(65);
    Script_scenCreateTask(script_B6BC, 1, 57);
    SCNF_WaitFrames_0000(30);
    Script_SetStats(STATS_SEGA_COINS, 0);
    SCNF_SetCashValue(local_FC, 0);
    Script_SetStats(STATS_SEGA_COINS, local_FC);
    Script_SetCharaLookAtPosition(CHARA_AKIR, -98312, 0, 0);
    Script_SetCharaLookAtPosition(CHARA_MIKM, -98312, 0, 0);
    local_FA = Script_PlayerStatsUtils(PS_GetCharaItemCount, 0x4D4B494D);
    Script_PlayerStatsUtils(PS_SetCharaItemCount, 0x4D4B494D, local_FA + 1);
    Script_SetCameraSettings(5, 6, 1, 0);
    Script_convSetCamInactiveFlag(1);
    Script_SetGameStateTo(AUTH_None);
    Script_ryoControl(RYO_RestoreControl);
    Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
    Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
    Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
    Script_shenClearFreeState(1);
    script_14626();
  }
  SCNF_WaitFrames_0000(1);
  return;
}

static void script_14917(void) {
  int32_t local_FC;
  int16_t local_FA;
  int16_t local_F8;
  int16_t local_F6;
  int16_t local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3 local_D0;
  Vector3i local_C4;
  Vector3 local_B8;
  Vector3 local_AC;
  Vector3i local_A0;
  int32_t local_9C;
  local_FC = Script_shenGetStats(STATS_SEGA_COINS);
  Script_SetCPAMFlags(CHARA_MIKM, 0);
  if (local_FC >= 1) {
    script_145F5();
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
    Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
    Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
    Script_shenClearFreeState(0);
    Script_ryoControl(PCCTL_UpdateRyoNavYaw, 0x52494B41);
    local_D0.x = 1.7f;
    (&local_D0)->y = 0.0f;
    (&local_D0)->z = -0.45f;
    Script_EVT_SetPosition(CHARA_MIKM, 0x78000000, &local_D0);
    local_C4.x = 0;
    (&local_C4)->y = 0;
    (&local_C4)->z = 0;
    Script_EVT_SetRotation(CHARA_MIKM, 0x78000000, &local_C4, 0);
    Script_PlayCharaMotion(CHARA_MIKM, 4155, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmToggleFlags(CHARA_MIKM, SCEN_Enable, 0x40);
    Script_motmSetBlendFrames(CHARA_MIKM, MOTM_BLEND_FRAMES_20);
    Script_SetCharaRefBoardPos(CHARA_MIKM, &data_1C174);
    Script_charaSetRefboardFlagTo(CHARA_MIKM, 1);
    Script_SetCharaMotionPlaybackRate(CHARA_MIKM, 0.5f);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
    Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_20);
    Script_SetCharaRefBoardPos(CHARA_AKIR, &data_1C174);
    Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
    Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
    Script_seqEnterEvtCam(7141, CHARA_AKIR, CHARA_AKIR);
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_E8, ENTITY_XFORM_WORLD_SPACE);
    (&local_E8)->y = 1.5f;
    Script_SetCharaLookAtPosition(CHARA_MIKM, 1, &local_E8, 0);
    SCNF_WaitFrames_0000(10);
    local_F8 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
    local_9C = local_F8;
    switch (local_9C) {
      case 0:
        Script_scenCreateTask(script_B6BC, 1, 62);
        SCNF_WaitFrames_0000(30);
        break;
      case 1:
        Script_scenCreateTask(script_B6BC, 1, 63);
        SCNF_WaitFrames_0000(30);
        break;
      case 2:
        Script_scenCreateTask(script_B6BC, 1, 64);
        SCNF_WaitFrames_0000(30);
        break;
    }
    Script_EVT_GetPosition(CHARA_MIKM, -1, &local_DC, ENTITY_XFORM_WORLD_SPACE);
    (&local_DC)->y = 1.5f;
    Script_SetCharaLookAtPosition(CHARA_AKIR, 1, &local_DC, 2);
    Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_DC, 10);
    SCNF_WaitFrames_0000(40);
    Script_scenCreateTask(script_B6BC, 1, 52);
    SCNF_WaitFrames_0000(40);
    Script_seqEnterEvtCam(7142, 0, AUTH_None);
    Script_scenCreateTask(script_B6BC, 1, 65);
    SCNF_WaitFrames_0000(10);
    local_D0.x = 4.7f;
    (&local_D0)->y = 0.0f;
    (&local_D0)->z = -1.1f;
    Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &local_D0);
    local_C4.x = 0;
    (&local_C4)->y = 0x6000;
    (&local_C4)->z = 0;
    Script_EVT_SetRotation(CHARA_AKIR, 0x78000000, &local_C4, 0);
    local_FA = Script_PlayerStatsUtils(PS_GetCharaItemCount, 0x4D4B494D);
    Script_PlayerStatsUtils(PS_SetCharaItemCount, 0x4D4B494D, local_FA + 1);
    SCNF_WaitFrames_0000(80);
    Script_seqEnterEvtCam(7143, 0, AUTH_None);
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_E8, ENTITY_XFORM_WORLD_SPACE);
    (&local_E8)->y = 1.5f;
    Script_SetCharaLookAtPosition(CHARA_MIKM, 1, &local_E8, 0);
    Script_scenCreateTask(script_B6BC, 1, 53);
    SCNF_WaitFrames_0000(10);
    SCNF_SetCashValue(-local_FC, 0);
    Script_SetStats(STATS_SEGA_COINS, local_FC);
    local_F6 = Script_GetOwnedItemCount(221, 0);
    local_F4 = Script_GetOwnedItemCount(222, 0);
    Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
    if ((local_FC >= 10000) & (local_F6 == 0)) {
      script_A297(12);
      Script_gachaponUtils(1, 0x4E494E4B, 4);
      Script_seqEnterEvtCam(7151, 0, AUTH_None);
      Script_scenCreateTask(script_B6BC, 1, 76);
      SCNF_WaitFrames_0000(102);
      Script_seqEnterEvtCam(7152, 0, AUTH_None);
      Script_scenCreateTask(script_B6BC, 1, 60);
      SCNF_WaitFrames_0000(31);
      Script_scenCreateTask(script_B6BC, 1, 74);
      SCNF_WaitFrames_0000(111);
      Script_filmCheckOrSetTransitionUtil(20, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      SCNF_WaitFrames_0000(20);
      local_D0.x = 1.2f;
      (&local_D0)->y = 0.0f;
      (&local_D0)->z = -1.5f;
      Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &local_D0);
      local_C4.x = 0;
      (&local_C4)->y = 30947;
      (&local_C4)->z = 0;
      Script_EVT_SetRotation(CHARA_AKIR, 0x78000000, &local_C4, 0);
      local_D0.x = 1.1f;
      (&local_D0)->y = 0.0f;
      (&local_D0)->z = -0.35f;
      Script_EVT_SetPosition(CHARA_MIKM, 0x78000000, &local_D0);
      local_C4.x = 0;
      (&local_C4)->y = 0;
      (&local_C4)->z = 0;
      Script_EVT_SetRotation(CHARA_MIKM, 0x78000000, &local_C4, 0);
      Script_EVT_GetPosition(CHARA_MIKM, -1, &local_DC, ENTITY_XFORM_WORLD_SPACE);
      (&local_DC)->y = 1.0f;
      Script_SetCharaLookAtPosition(CHARA_AKIR, 1, &local_DC, 2);
      Script_EVT_GetPosition(CHARA_AKIR, -1, &local_E8, ENTITY_XFORM_WORLD_SPACE);
      (&local_E8)->y = 1.0f;
      Script_SetCharaLookAtPosition(CHARA_MIKM, 1, &local_E8, 0);
      SCNF_WaitFrames_0000(10);
      Script_hmdlApplyScale(SHEN_NAME('KNIN'), &data_1C184);
      local_B8.x = 0.13f;
      (&local_B8)->y = 0.11f;
      (&local_B8)->z = -0.077f;
      local_A0.x = 27306;
      (&local_A0)->y = 22755;
      (&local_A0)->z = 50972;
      Script_EVT_GetPosition(CHARA_MIKM, 18, &local_AC, 0);
      Script_EVT_SetPosition(SHEN_NAME('KNIN'), POS_XYZ, &local_AC);
      Script_fixoAttachEx(SHEN_NAME('KNIN'), CHARA_MIKM, 18, &local_B8, &local_A0);
      Script_seqEnterEvtCam(7153, 0, AUTH_None);
      Script_PlayCharaMotion(CHARA_AKIR, 4131, 110.0f, 111.0f, 0.0f, 1.0f, 30);
      Script_PlayCharaMotion(CHARA_MIKM, 4133, 0.0f, 180.0f, 0.0f, 1.0f, 30);
      Script_filmCheckOrSetTransitionUtil(20, 255, 0, 0, 0, 0, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      SCNF_WaitFrames_0000(40);
      Script_charaDisplay(SHEN_NAME('KNIN'), CDM_Show);
      Script_scenCreateTask(script_B6BC, 1, 71);
      SCNF_WaitFrames_0000(30);
      Script_fixoClearAttachment(SHEN_NAME('KNIN'));
      SCNF_WaitFrames_0000(40);
      Script_seqEnterEvtCam(7154, 0, AUTH_None);
      Script_GetCassetteFlags(221, 1, 0, 0);
      SCNF_StartIconSystemMenu(221);
      Script_scenCreateTask(script_B6BC, 1, 57);
      Script_EVT_GetPosition(CHARA_MIKM, -1, &local_DC, ENTITY_XFORM_WORLD_SPACE);
      (&local_DC)->y = 1.0f;
      Script_SetCharaLookAtPosition(CHARA_AKIR, 1, &local_DC, 2);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_DC, 10);
      SCNF_WaitFrames_0000(40);
      local_B8.x = 0.167f;
      (&local_B8)->y = 0.035f;
      (&local_B8)->z = -0.056f;
      local_A0.x = 39685;
      (&local_A0)->y = 62805;
      (&local_A0)->z = 19842;
      Script_EVT_GetPosition(CHARA_AKIR, 18, &local_AC, 0);
      Script_EVT_SetPosition(SHEN_NAME('KNIN'), POS_XYZ, &local_AC);
      Script_fixoAttachEx(SHEN_NAME('KNIN'), CHARA_AKIR, 18, &local_B8, &local_A0);
      Script_seqEnterEvtCam(7155, 0, AUTH_None);
      Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_30);
      Script_PlayCharaMotion(CHARA_AKIR, 4131, 115.0f, 160.0f, 0.0f, 1.0f, 30);
      SCNF_WaitFrames_0000(60);
      Script_PlayCharaMotion(CHARA_AKIR, 4140, 10.0f, 110.0f, 0.0f, 1.0f, 30);
      SCNF_WaitFrames_0000(60);
      Script_charaDisplay(SHEN_NAME('KNIN'), CDM_Hide);
    }
    if (-(local_FC >= 50000) & -(local_F6 == 0) & -(local_F4 == 0)) {
      script_A297(12);
      Script_gachaponUtils(1, 0x4E494E4D, 5);
      Script_PlayCharaMotion(CHARA_AKIR, 4131, 119.0f, 120.0f, 0.0f, 1.0f, 30);
      Script_seqEnterEvtCam(7156, 0, AUTH_None);
      Script_scenCreateTask(script_B6BC, 1, 75);
      SCNF_WaitFrames_0000(20);
      Script_PlayCharaMotion(CHARA_MIKM, 4133, 0.0f, 180.0f, 0.0f, 1.0f, 30);
      SCNF_WaitFrames_0000(40);
      Script_hmdlApplyScale(SHEN_NAME('MNIN'), &data_1C184);
      local_B8.x = 0.121f;
      (&local_B8)->y = 0.11f;
      (&local_B8)->z = -0.077f;
      local_A0.x = 29127;
      (&local_A0)->y = 22755;
      (&local_A0)->z = 50972;
      Script_EVT_GetPosition(CHARA_MIKM, 18, &local_AC, 0);
      Script_EVT_SetPosition(SHEN_NAME('MNIN'), POS_XYZ, &local_AC);
      Script_fixoAttachEx(SHEN_NAME('MNIN'), CHARA_MIKM, 18, &local_B8, &local_A0);
      Script_charaDisplay(SHEN_NAME('MNIN'), CDM_Show);
      SCNF_WaitFrames_0000(46);
      Script_fixoClearAttachment(SHEN_NAME('MNIN'));
      SCNF_WaitFrames_0000(40);
      Script_mtwkGetSetRotation(SHEN_NAME('MNIN'), &local_C4, 0);
      local_C4.x = 0x4000;
      Script_EVT_SetRotation(SHEN_NAME('MNIN'), 0x38000000, &local_C4, 0);
      Script_seqEnterEvtCam(7157, 0, AUTH_None);
      Script_GetCassetteFlags(222, 1, 0, 0);
      SCNF_StartIconSystemMenu(222);
      Script_scenCreateTask(script_B6BC, 1, 61);
      SCNF_WaitFrames_0000(31);
      local_B8.x = 0.1f;
      (&local_B8)->y = 0.02f;
      (&local_B8)->z = -0.07f;
      local_A0.x = 36408;
      (&local_A0)->y = 0;
      (&local_A0)->z = 19114;
      Script_EVT_GetPosition(CHARA_AKIR, 18, &local_AC, 0);
      Script_EVT_SetPosition(SHEN_NAME('MNIN'), POS_XYZ, &local_AC);
      Script_fixoAttachEx(SHEN_NAME('MNIN'), CHARA_AKIR, 18, &local_B8, &local_A0);
      local_D0.x = 1.2f;
      (&local_D0)->y = 0.0f;
      (&local_D0)->z = -1.5f;
      Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &local_D0);
      local_C4.x = 0;
      (&local_C4)->y = 30947;
      (&local_C4)->z = 0;
      Script_EVT_SetRotation(CHARA_AKIR, 0x78000000, &local_C4, 0);
      Script_seqEnterEvtCam(7158, 0, AUTH_None);
      Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_30);
      Script_PlayCharaMotion(CHARA_AKIR, 4131, 115.0f, 160.0f, 0.0f, 1.0f, 30);
      SCNF_WaitFrames_0000(60);
      Script_PlayCharaMotion(CHARA_AKIR, 4140, 10.0f, 110.0f, 0.0f, 1.0f, 30);
      SCNF_WaitFrames_0000(40);
      Script_charaDisplay(SHEN_NAME('MNIN'), CDM_Hide);
      SCNF_WaitFrames_0000(20);
    }
    if (-(local_FC >= 50000) & -(local_F6 == 1) & -(local_F4 == 0)) {
      script_A297(12);
      Script_gachaponUtils(1, 0x4E494E4D, 5);
      Script_seqEnterEvtCam(7151, 0, AUTH_None);
      Script_scenCreateTask(script_B6BC, 1, 77);
      SCNF_WaitFrames_0000(102);
      Script_seqEnterEvtCam(7152, 0, AUTH_None);
      Script_scenCreateTask(script_B6BC, 1, 60);
      SCNF_WaitFrames_0000(31);
      Script_scenCreateTask(script_B6BC, 1, 74);
      SCNF_WaitFrames_0000(111);
      Script_filmCheckOrSetTransitionUtil(20, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      SCNF_WaitFrames_0000(20);
      local_D0.x = 1.2f;
      (&local_D0)->y = 0.0f;
      (&local_D0)->z = -1.5f;
      Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &local_D0);
      local_C4.x = 0;
      (&local_C4)->y = 30947;
      (&local_C4)->z = 0;
      Script_EVT_SetRotation(CHARA_AKIR, 0x78000000, &local_C4, 0);
      local_D0.x = 1.1f;
      (&local_D0)->y = 0.0f;
      (&local_D0)->z = -0.35f;
      Script_EVT_SetPosition(CHARA_MIKM, 0x78000000, &local_D0);
      local_C4.x = 0;
      (&local_C4)->y = 0;
      (&local_C4)->z = 0;
      Script_EVT_SetRotation(CHARA_MIKM, 0x78000000, &local_C4, 0);
      Script_EVT_GetPosition(CHARA_MIKM, -1, &local_DC, ENTITY_XFORM_WORLD_SPACE);
      (&local_DC)->y = 1.0f;
      Script_SetCharaLookAtPosition(CHARA_AKIR, 1, &local_DC, 2);
      Script_EVT_GetPosition(CHARA_AKIR, -1, &local_E8, ENTITY_XFORM_WORLD_SPACE);
      (&local_E8)->y = 1.0f;
      Script_SetCharaLookAtPosition(CHARA_MIKM, 1, &local_E8, 0);
      SCNF_WaitFrames_0000(10);
      Script_hmdlApplyScale(SHEN_NAME('MNIN'), &data_1C184);
      local_B8.x = 0.121f;
      (&local_B8)->y = 0.11f;
      (&local_B8)->z = -0.077f;
      local_A0.x = 29127;
      (&local_A0)->y = 22755;
      (&local_A0)->z = 50972;
      Script_EVT_GetPosition(CHARA_MIKM, 18, &local_AC, 0);
      Script_EVT_SetPosition(SHEN_NAME('MNIN'), POS_XYZ, &local_AC);
      Script_fixoAttachEx(SHEN_NAME('MNIN'), CHARA_MIKM, 18, &local_B8, &local_A0);
      Script_seqEnterEvtCam(7156, 0, AUTH_None);
      Script_PlayCharaMotion(CHARA_AKIR, 4131, 110.0f, 111.0f, 0.0f, 1.0f, 30);
      Script_PlayCharaMotion(CHARA_MIKM, 4133, 0.0f, 180.0f, 0.0f, 1.0f, 30);
      Script_filmCheckOrSetTransitionUtil(20, 255, 0, 0, 0, 0, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      SCNF_WaitFrames_0000(40);
      Script_charaDisplay(SHEN_NAME('MNIN'), CDM_Show);
      Script_scenCreateTask(script_B6BC, 1, 71);
      SCNF_WaitFrames_0000(30);
      Script_fixoClearAttachment(SHEN_NAME('MNIN'));
      SCNF_WaitFrames_0000(40);
      Script_mtwkGetSetRotation(SHEN_NAME('MNIN'), &local_C4, 0);
      local_C4.x = 0x4000;
      Script_EVT_SetRotation(SHEN_NAME('MNIN'), 0x38000000, &local_C4, 0);
      Script_seqEnterEvtCam(7157, 0, AUTH_None);
      Script_GetCassetteFlags(222, 1, 0, 0);
      SCNF_StartIconSystemMenu(222);
      Script_scenCreateTask(script_B6BC, 1, 61);
      SCNF_WaitFrames_0000(31);
      local_B8.x = 0.1f;
      (&local_B8)->y = 0.02f;
      (&local_B8)->z = -0.07f;
      local_A0.x = 36408;
      (&local_A0)->y = 0;
      (&local_A0)->z = 19114;
      Script_EVT_GetPosition(CHARA_AKIR, 18, &local_AC, 0);
      Script_EVT_SetPosition(SHEN_NAME('MNIN'), POS_XYZ, &local_AC);
      Script_fixoAttachEx(SHEN_NAME('MNIN'), CHARA_AKIR, 18, &local_B8, &local_A0);
      Script_seqEnterEvtCam(7158, 0, AUTH_None);
      Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_30);
      Script_PlayCharaMotion(CHARA_AKIR, 4131, 115.0f, 160.0f, 0.0f, 1.0f, 30);
      SCNF_WaitFrames_0000(60);
      Script_PlayCharaMotion(CHARA_AKIR, 4140, 10.0f, 110.0f, 0.0f, 1.0f, 30);
      SCNF_WaitFrames_0000(40);
      Script_charaDisplay(SHEN_NAME('MNIN'), CDM_Hide);
      SCNF_WaitFrames_0000(20);
    }
    if (((local_FC >= 10000) & (local_F6 == 0)) | ((local_FC >= 50000) & (local_F4 == 0))) {
    } else {
      Script_seqEnterEvtCam(7144, 0, AUTH_None);
    }
    Script_scenCreateTask(script_B6BC, 1, 66);
    SCNF_WaitFrames_0000(60);
    Script_executeAicaCommand(2720, 1, 30);
    Script_executeAicaCommand(2720, 2, 30);
    Script_executeAicaCommand(2720, 3, 30);
    Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_SetCharaLookAtPosition(CHARA_AKIR, -98312, 0, 0);
    Script_SetCharaLookAtPosition(CHARA_MIKM, -98312, 0, 0);
    Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
    Script_SetCharaMotionPlaybackRate(CHARA_MIKM, 1.0f);
    Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 8);
    Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
    Script_SetCameraSettings(5, 6, 1, 0);
    Script_convSetCamInactiveFlag(1);
    Script_SetGameStateTo(AUTH_None);
    Script_ryoControl(RYO_RestoreControl);
    Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
    Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
    Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
    Script_shenClearFreeState(1);
    script_14626();
  }
  Script_SetCPAMFlags(CHARA_MIKM, 1);
  SCNF_WaitFrames_0000(1);
  return;
}

__attribute__((scn_string_bindings(1, data_1C1AE, 3, data_1C1C9, 5, data_1C1E4)))
static int32_t script_1598B(void) {
  int16_t local_FE;
  int16_t local_FC;
  int16_t local_FA;
  int16_t local_F8;
  uint8_t unused_F6[2];
  int16_t local_F4;
  int16_t local_F2;
  int16_t local_F0;
  int16_t local_EE;
  int16_t local_EC;
  int16_t local_EA;
  int16_t local_E8;
  int16_t local_E6;
  uint8_t unused_E4[2];
  int32_t local_E0;
  Vector3 local_D4;
  Vector3 local_C8;
  Vector3 local_BC;
  Vector3 local_B0;
  Vector3i local_A4;
  local_FC = Script_GetSceneID();
  {
    int32_t unused_98[3];
    int32_t local_94;
    local_94 = local_FC;
    switch (local_94) {
      case 1:
        Script_CycleManagerUtils(CYCLE_CMD_SETUP, "scene/01/D000", (int32_t)"CYCLEMAN.BIN", 0x544C5344);
        break;
      case 2:
        Script_CycleManagerUtils(CYCLE_CMD_SETUP, "scene/02/D000", (int32_t)"CYCLEMAN.BIN", 0x544C5344);
        break;
      case 3:
        Script_CycleManagerUtils(CYCLE_CMD_SETUP, "scene/03/D000", (int32_t)"CYCLEMAN.BIN", 0x544C5344);
        break;
    }
  }
  Script_SetShouldSleepFlag(0);
  Script_TimedTransitionUtils(TIMED_TRANSITION_SET_ENABLED, 0);
  local_F0 = Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIRED_FROM_JOB);
  local_EA = Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 970);
  local_EE = Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_0172_RETURN_PROMPT_SHOWN);
  local_EC = Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_STARTED);
  local_E8 = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 31);
  {
    int32_t unused_A0;
    GameDateTime local_98;
    int32_t local_94;
    Script_ReturnTimeDate(&local_98);
    local_94 = (&local_98)->month;
    switch (local_94) {
      case 1:
        local_E6 = (&local_98)->day + 31;
        break;
      case 2:
        local_E6 = (&local_98)->day + 62;
        break;
      case 3:
        local_E6 = (&local_98)->day + 90;
        break;
      case 12:
        local_E6 = (&local_98)->day;
        break;
    }
  }
  Script_sprintf("day_check :%d,%d \012", local_E8, local_E6);
  task_274 = 0;
  if ((local_F0 == 1) & (local_EA == 0)) {
    task_274 = 1;
  }
  if ((local_EE == 1) & (local_EC == 0)) {
    task_274 = 2;
  }
  if (local_E8 == local_E6) {
    task_274 = 3;
  }
  local_FA = 23;
  local_F8 = 30;
  if (task_274 == 1) {
    local_FA = 12;
    local_F8 = 30;
  }
  if (task_274 == 2) {
    local_FA = 12;
    local_F8 = 0;
  }
  if (task_274 == 3) {
    local_FA = 23;
    local_F8 = 59;
  }
  script_14550();
  local_D4.x = 1.7f;
  (&local_D4)->y = 0.0f;
  (&local_D4)->z = -0.45f;
  Script_EVT_SetPosition(CHARA_MIKM, 0x78000000, &local_D4);
  local_A4.x = 0;
  (&local_A4)->y = 0;
  (&local_A4)->z = 0;
  Script_EVT_SetRotation(CHARA_MIKM, 0x78000000, &local_A4, 0);
  Script_PlayCharaMotion(CHARA_MIKM, 4155, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_sprintf("REJUM: %d \012", task_1A0);
  if (task_1A0 == 1) {
    script_14657();
  }
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_ryoControl(RYO_RestoreControl);
  Script_shenClearFreeState(1);
  if (Script_PlayerStatsUtils(PS_GetStepSlot, 0) <= 0) {
    Script_PlayerStatsUtils(PS_SetStepSlot, 0, 8);
    Script_PlayerStatsUtils(PS_SetStepSlot, 1, 4);
    Script_PlayerStatsUtils(PS_SetStepSlot, 2, 7);
    Script_PlayerStatsUtils(PS_SetStepSlot, 3, 9);
    Script_PlayerStatsUtils(PS_SetStepSlot, 4, 6);
    Script_PlayerStatsUtils(PS_SetStepSlot, 5, 5);
    Script_PlayerStatsUtils(PS_SetStepSlot, 6, 2);
    Script_PlayerStatsUtils(PS_SetStepSlot, 7, 5);
    Script_PlayerStatsUtils(PS_SetStepSlot, 8, 7);
    Script_PlayerStatsUtils(PS_SetStepSlot, 9, 3);
    Script_PlayerStatsUtils(PS_SetStepSlot, 10, 2);
    Script_PlayerStatsUtils(PS_SetStepSlot, 11, 5);
    Script_PlayerStatsUtils(PS_SetStepSlot, 12, 6);
    Script_PlayerStatsUtils(PS_SetStepSlot, 13, 3);
    Script_PlayerStatsUtils(PS_SetStepSlot, 14, 2);
    Script_PlayerStatsUtils(PS_SetStepSlot, 15, 4);
  }
  local_F2 = 0;
  while (1) {
    int32_t unused_A0;
    GameDateTime local_98;
    Script_ReturnTimeDate(&local_98);
    if ((((&local_98)->hours >= local_FA) & ((&local_98)->minutes >= local_F8)) | ((&local_98)->hours <= 4)) {
      while (Script_TMEXUtilities(0, 0) == 0) {
        SCNF_WaitFrames_0000(1);
      }
      while (Script_TMEXUtilities(1) != 0) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ryoControl(PCCTL_DisablePlayerControl);
      Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
      Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
      Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
      Script_shenClearFreeState(0);
      script_145F5();
      Script_ryoControl(PCCTL_UpdateRyoNavYaw, 0x52494B41);
      Script_SetCharaRefBoardPos(CHARA_AKIR, &data_1C190);
      Script_SetCharaRefBoardPos(CHARA_MIKM, &data_1C190);
      Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
      Script_charaSetRefboardFlagTo(CHARA_MIKM, 1);
      Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
      script_10CB5();
    }
    if (Script_IsPlayerAllowed(0)) {
      local_F4 = 0;
      Script_EVT_GetPosition(SHEN_NAME('SLT1'), -1, &local_D4, ENTITY_XFORM_WORLD_SPACE);
      local_C8.x = (&local_D4)->x;
      (&local_C8)->z = (&local_D4)->z;
      local_BC.x = (&local_D4)->x + 1.5f;
      (&local_BC)->z = (&local_D4)->z - 0.74f;
      local_B0.x = (&local_D4)->x + 0.2f;
      (&local_B0)->y = 0.0f;
      (&local_B0)->z = (&local_D4)->z - 0.35f;
      if (Script_SetTaskDataValue(CHARA_AKIR, &local_C8, &local_BC, 0) & script_1450C(CHARA_AKIR, &local_B0, 1.5f, 40.0f)) {
        local_F4 = 1;
        if (Script_GetControllerInput(1) == 0x400) {
          task_1A2 = 13;
          Script_sicnUtils(1, 0, 0);
          local_FE = Script_PlayerStatsUtils(PS_GetStepSlot, 13);
          Script_scenCreateTask(script_A9F2, 1, local_FE);
          script_A8FC(SHEN_NAME('SLT1'), 10);
          local_F2 = 0;
        }
      }
      Script_EVT_GetPosition(SHEN_NAME('SLT2'), -1, &local_D4, ENTITY_XFORM_WORLD_SPACE);
      local_C8.x = (&local_D4)->x;
      (&local_C8)->z = (&local_D4)->z;
      local_BC.x = (&local_D4)->x + 1.5f;
      (&local_BC)->z = (&local_D4)->z - 0.74f;
      local_B0.x = (&local_D4)->x + 0.2f;
      (&local_B0)->y = 0.0f;
      (&local_B0)->z = (&local_D4)->z - 0.35f;
      if (Script_SetTaskDataValue(CHARA_AKIR, &local_C8, &local_BC, 0) & script_1450C(CHARA_AKIR, &local_B0, 1.5f, 40.0f)) {
        local_F4 = 1;
        if (Script_GetControllerInput(1) == 0x400) {
          task_1A2 = 12;
          Script_sicnUtils(1, 0, 0);
          local_FE = Script_PlayerStatsUtils(PS_GetStepSlot, 12);
          Script_scenCreateTask(script_A9F2, 1, local_FE);
          script_A8FC(SHEN_NAME('SLT2'), 10);
          local_F2 = 0;
        }
      }
      Script_EVT_GetPosition(SHEN_NAME('SLT3'), -1, &local_D4, ENTITY_XFORM_WORLD_SPACE);
      local_C8.x = (&local_D4)->x;
      (&local_C8)->z = (&local_D4)->z;
      local_BC.x = (&local_D4)->x + 1.5f;
      (&local_BC)->z = (&local_D4)->z - 0.74f;
      local_B0.x = (&local_D4)->x + 0.2f;
      (&local_B0)->y = 0.0f;
      (&local_B0)->z = (&local_D4)->z - 0.35f;
      if (Script_SetTaskDataValue(CHARA_AKIR, &local_C8, &local_BC, 0) & script_1450C(CHARA_AKIR, &local_B0, 1.5f, 40.0f)) {
        local_F4 = 1;
        if (Script_GetControllerInput(1) == 0x400) {
          task_1A2 = 11;
          Script_sicnUtils(1, 0, 0);
          local_FE = Script_PlayerStatsUtils(PS_GetStepSlot, 11);
          Script_scenCreateTask(script_A9F2, 1, local_FE);
          script_A8FC(SHEN_NAME('SLT3'), 10);
          local_F2 = 0;
        }
      }
      Script_EVT_GetPosition(SHEN_NAME('SLT4'), -1, &local_D4, ENTITY_XFORM_WORLD_SPACE);
      local_C8.x = (&local_D4)->x;
      (&local_C8)->z = (&local_D4)->z;
      local_BC.x = (&local_D4)->x + 1.5f;
      (&local_BC)->z = (&local_D4)->z - 0.74f;
      local_B0.x = (&local_D4)->x + 0.2f;
      (&local_B0)->y = 0.0f;
      (&local_B0)->z = (&local_D4)->z - 0.35f;
      if (Script_SetTaskDataValue(CHARA_AKIR, &local_C8, &local_BC, 0) & script_1450C(CHARA_AKIR, &local_B0, 1.5f, 40.0f)) {
        local_F4 = 1;
        if (Script_GetControllerInput(1) == 0x400) {
          task_1A2 = 10;
          Script_sicnUtils(1, 0, 0);
          local_FE = Script_PlayerStatsUtils(PS_GetStepSlot, 10);
          Script_scenCreateTask(script_A9F2, 1, local_FE);
          script_A8FC(SHEN_NAME('SLT4'), 10);
          local_F2 = 0;
        }
      }
      Script_EVT_GetPosition(SHEN_NAME('SLT5'), -1, &local_D4, ENTITY_XFORM_WORLD_SPACE);
      local_C8.x = (&local_D4)->x;
      (&local_C8)->z = (&local_D4)->z;
      local_BC.x = (&local_D4)->x - 1.5f;
      (&local_BC)->z = (&local_D4)->z + 0.74f;
      local_B0.x = (&local_D4)->x - 0.2f;
      (&local_B0)->y = 0.0f;
      (&local_B0)->z = (&local_D4)->z + 0.35f;
      if (Script_SetTaskDataValue(CHARA_AKIR, &local_C8, &local_BC, 0) & script_1450C(CHARA_AKIR, &local_B0, 1.5f, 40.0f)) {
        local_F4 = 1;
        if (Script_GetControllerInput(1) == 0x400) {
          task_1A2 = 6;
          Script_sicnUtils(1, 0, 0);
          local_FE = Script_PlayerStatsUtils(PS_GetStepSlot, 6);
          Script_scenCreateTask(script_A9F2, 1, local_FE);
          script_A8FC(SHEN_NAME('SLT5'), 5);
          local_F2 = 0;
        }
      }
      Script_EVT_GetPosition(SHEN_NAME('SLT6'), -1, &local_D4, ENTITY_XFORM_WORLD_SPACE);
      local_C8.x = (&local_D4)->x;
      (&local_C8)->z = (&local_D4)->z;
      local_BC.x = (&local_D4)->x - 1.5f;
      (&local_BC)->z = (&local_D4)->z + 0.74f;
      local_B0.x = (&local_D4)->x - 0.2f;
      (&local_B0)->y = 0.0f;
      (&local_B0)->z = (&local_D4)->z + 0.35f;
      if (Script_SetTaskDataValue(CHARA_AKIR, &local_C8, &local_BC, 0) & script_1450C(CHARA_AKIR, &local_B0, 1.5f, 40.0f)) {
        local_F4 = 1;
        if (Script_GetControllerInput(1) == 0x400) {
          task_1A2 = 7;
          Script_sicnUtils(1, 0, 0);
          local_FE = Script_PlayerStatsUtils(PS_GetStepSlot, 7);
          Script_scenCreateTask(script_A9F2, 1, local_FE);
          script_A8FC(SHEN_NAME('SLT6'), 5);
          local_F2 = 0;
        }
      }
      Script_EVT_GetPosition(SHEN_NAME('SLT7'), -1, &local_D4, ENTITY_XFORM_WORLD_SPACE);
      local_C8.x = (&local_D4)->x;
      (&local_C8)->z = (&local_D4)->z;
      local_BC.x = (&local_D4)->x - 1.5f;
      (&local_BC)->z = (&local_D4)->z + 0.74f;
      local_B0.x = (&local_D4)->x - 0.2f;
      (&local_B0)->y = 0.0f;
      (&local_B0)->z = (&local_D4)->z + 0.35f;
      if (Script_SetTaskDataValue(CHARA_AKIR, &local_C8, &local_BC, 0) & script_1450C(CHARA_AKIR, &local_B0, 1.5f, 40.0f)) {
        local_F4 = 1;
        if (Script_GetControllerInput(1) == 0x400) {
          task_1A2 = 8;
          Script_sicnUtils(1, 0, 0);
          local_FE = Script_PlayerStatsUtils(PS_GetStepSlot, 8);
          Script_scenCreateTask(script_A9F2, 1, local_FE);
          script_A8FC(SHEN_NAME('SLT7'), 5);
          local_F2 = 0;
        }
      }
      Script_EVT_GetPosition(SHEN_NAME('SLT8'), -1, &local_D4, ENTITY_XFORM_WORLD_SPACE);
      local_C8.x = (&local_D4)->x;
      (&local_C8)->z = (&local_D4)->z;
      local_BC.x = (&local_D4)->x - 1.5f;
      (&local_BC)->z = (&local_D4)->z + 0.74f;
      local_B0.x = (&local_D4)->x - 0.2f;
      (&local_B0)->y = 0.0f;
      (&local_B0)->z = (&local_D4)->z + 0.35f;
      if (Script_SetTaskDataValue(CHARA_AKIR, &local_C8, &local_BC, 0) & script_1450C(CHARA_AKIR, &local_B0, 1.5f, 40.0f)) {
        local_F4 = 1;
        if (Script_GetControllerInput(1) == 0x400) {
          task_1A2 = 9;
          Script_sicnUtils(1, 0, 0);
          local_FE = Script_PlayerStatsUtils(PS_GetStepSlot, 9);
          Script_scenCreateTask(script_A9F2, 1, local_FE);
          script_A8FC(SHEN_NAME('SLT8'), 5);
          local_F2 = 0;
        }
      }
      Script_EVT_GetPosition(SHEN_NAME('SLT9'), -1, &local_D4, ENTITY_XFORM_WORLD_SPACE);
      local_C8.x = (&local_D4)->x;
      (&local_C8)->z = (&local_D4)->z;
      local_BC.x = (&local_D4)->x + 0.74f;
      (&local_BC)->z = (&local_D4)->z + 1.5f;
      local_B0.x = (&local_D4)->x + 0.35f;
      (&local_B0)->y = 0.0f;
      (&local_B0)->z = (&local_D4)->z + 0.2f;
      if (Script_SetTaskDataValue(CHARA_AKIR, &local_C8, &local_BC, 0) & script_1450C(CHARA_AKIR, &local_B0, 1.5f, 40.0f)) {
        local_F4 = 1;
        if (Script_GetControllerInput(1) == 0x400) {
          task_1A2 = 15;
          Script_sicnUtils(1, 0, 0);
          local_FE = Script_PlayerStatsUtils(PS_GetStepSlot, 15);
          Script_scenCreateTask(script_A9F2, 1, local_FE);
          script_A8FC(SHEN_NAME('SLT9'), 20);
          local_F2 = 0;
        }
      }
      Script_EVT_GetPosition(SHEN_NAME('SLTA'), -1, &local_D4, ENTITY_XFORM_WORLD_SPACE);
      local_C8.x = (&local_D4)->x;
      (&local_C8)->z = (&local_D4)->z;
      local_BC.x = (&local_D4)->x + 0.74f;
      (&local_BC)->z = (&local_D4)->z + 1.5f;
      local_B0.x = (&local_D4)->x + 0.35f;
      (&local_B0)->y = 0.0f;
      (&local_B0)->z = (&local_D4)->z + 0.2f;
      if (Script_SetTaskDataValue(CHARA_AKIR, &local_C8, &local_BC, 0) & script_1450C(CHARA_AKIR, &local_B0, 1.5f, 40.0f)) {
        local_F4 = 1;
        if (Script_GetControllerInput(1) == 0x400) {
          task_1A2 = 14;
          Script_sicnUtils(1, 0, 0);
          local_FE = Script_PlayerStatsUtils(PS_GetStepSlot, 14);
          Script_scenCreateTask(script_A9F2, 1, local_FE);
          script_A8FC(SHEN_NAME('SLTA'), 20);
          local_F2 = 0;
        }
      }
      Script_EVT_GetPosition(SHEN_NAME('SLTB'), -1, &local_D4, ENTITY_XFORM_WORLD_SPACE);
      local_C8.x = (&local_D4)->x;
      (&local_C8)->z = (&local_D4)->z;
      local_BC.x = (&local_D4)->x - 1.5f;
      (&local_BC)->z = (&local_D4)->z + 0.74f;
      local_B0.x = (&local_D4)->x - 0.2f;
      (&local_B0)->y = 0.0f;
      (&local_B0)->z = (&local_D4)->z + 0.35f;
      if (Script_SetTaskDataValue(CHARA_AKIR, &local_C8, &local_BC, 0) & script_1450C(CHARA_AKIR, &local_B0, 1.5f, 40.0f)) {
        local_F4 = 1;
        if (Script_GetControllerInput(1) == 0x400) {
          task_1A2 = 3;
          Script_sicnUtils(1, 0, 0);
          local_FE = Script_PlayerStatsUtils(PS_GetStepSlot, 3);
          Script_scenCreateTask(script_A9F2, 1, local_FE);
          script_A8FC(SHEN_NAME('SLTB'), 1);
          local_F2 = 0;
        }
      }
      Script_EVT_GetPosition(SHEN_NAME('SLTC'), -1, &local_D4, ENTITY_XFORM_WORLD_SPACE);
      local_C8.x = (&local_D4)->x;
      (&local_C8)->z = (&local_D4)->z;
      local_BC.x = (&local_D4)->x - 1.5f;
      (&local_BC)->z = (&local_D4)->z + 0.74f;
      local_B0.x = (&local_D4)->x - 0.2f;
      (&local_B0)->y = 0.0f;
      (&local_B0)->z = (&local_D4)->z + 0.35f;
      if (Script_SetTaskDataValue(CHARA_AKIR, &local_C8, &local_BC, 0) & script_1450C(CHARA_AKIR, &local_B0, 1.5f, 40.0f)) {
        local_F4 = 1;
        if (Script_GetControllerInput(1) == 0x400) {
          task_1A2 = 4;
          Script_sicnUtils(1, 0, 0);
          local_FE = Script_PlayerStatsUtils(PS_GetStepSlot, 4);
          Script_scenCreateTask(script_A9F2, 1, local_FE);
          script_A8FC(SHEN_NAME('SLTC'), 1);
          local_F2 = 0;
        }
      }
      Script_EVT_GetPosition(SHEN_NAME('SLTD'), -1, &local_D4, ENTITY_XFORM_WORLD_SPACE);
      local_C8.x = (&local_D4)->x;
      (&local_C8)->z = (&local_D4)->z;
      local_BC.x = (&local_D4)->x - 1.5f;
      (&local_BC)->z = (&local_D4)->z + 0.74f;
      local_B0.x = (&local_D4)->x - 0.2f;
      (&local_B0)->y = 0.0f;
      (&local_B0)->z = (&local_D4)->z + 0.35f;
      if (Script_SetTaskDataValue(CHARA_AKIR, &local_C8, &local_BC, 0) & script_1450C(CHARA_AKIR, &local_B0, 1.5f, 40.0f)) {
        local_F4 = 1;
        if (Script_GetControllerInput(1) == 0x400) {
          task_1A2 = 5;
          Script_sicnUtils(1, 0, 0);
          local_FE = Script_PlayerStatsUtils(PS_GetStepSlot, 5);
          Script_scenCreateTask(script_A9F2, 1, local_FE);
          script_A8FC(SHEN_NAME('SLTD'), 1);
          local_F2 = 0;
        }
      }
      Script_EVT_GetPosition(SHEN_NAME('SLTE'), -1, &local_D4, ENTITY_XFORM_WORLD_SPACE);
      local_C8.x = (&local_D4)->x;
      (&local_C8)->z = (&local_D4)->z;
      local_BC.x = (&local_D4)->x + 1.5f;
      (&local_BC)->z = (&local_D4)->z - 0.74f;
      local_B0.x = (&local_D4)->x + 0.2f;
      (&local_B0)->y = 0.0f;
      (&local_B0)->z = (&local_D4)->z - 0.35f;
      if (Script_SetTaskDataValue(CHARA_AKIR, &local_C8, &local_BC, 0) & script_1450C(CHARA_AKIR, &local_B0, 1.5f, 40.0f)) {
        local_F4 = 1;
        if (Script_GetControllerInput(1) == 0x400) {
          task_1A2 = 2;
          Script_sicnUtils(1, 0, 0);
          local_FE = Script_PlayerStatsUtils(PS_GetStepSlot, 2);
          Script_scenCreateTask(script_A9F2, 1, local_FE);
          script_A8FC(SHEN_NAME('SLTE'), 1);
          local_F2 = 0;
        }
      }
      Script_EVT_GetPosition(SHEN_NAME('SLTF'), -1, &local_D4, ENTITY_XFORM_WORLD_SPACE);
      local_C8.x = (&local_D4)->x;
      (&local_C8)->z = (&local_D4)->z;
      local_BC.x = (&local_D4)->x + 1.5f;
      (&local_BC)->z = (&local_D4)->z - 0.74f;
      local_B0.x = (&local_D4)->x + 0.2f;
      (&local_B0)->y = 0.0f;
      (&local_B0)->z = (&local_D4)->z - 0.35f;
      if (Script_SetTaskDataValue(CHARA_AKIR, &local_C8, &local_BC, 0) & script_1450C(CHARA_AKIR, &local_B0, 1.5f, 40.0f)) {
        local_F4 = 1;
        if (Script_GetControllerInput(1) == 0x400) {
          task_1A2 = 1;
          Script_sicnUtils(1, 0, 0);
          local_FE = Script_PlayerStatsUtils(PS_GetStepSlot, 1);
          Script_scenCreateTask(script_A9F2, 1, local_FE);
          script_A8FC(SHEN_NAME('SLTF'), 1);
          local_F2 = 0;
        }
      }
      Script_EVT_GetPosition(SHEN_NAME('SLTG'), -1, &local_D4, ENTITY_XFORM_WORLD_SPACE);
      local_C8.x = (&local_D4)->x;
      (&local_C8)->z = (&local_D4)->z;
      local_BC.x = (&local_D4)->x + 1.5f;
      (&local_BC)->z = (&local_D4)->z - 0.74f;
      local_B0.x = (&local_D4)->x + 0.2f;
      (&local_B0)->y = 0.0f;
      (&local_B0)->z = (&local_D4)->z - 0.35f;
      if (Script_SetTaskDataValue(CHARA_AKIR, &local_C8, &local_BC, 0) & script_1450C(CHARA_AKIR, &local_B0, 1.5f, 40.0f)) {
        local_F4 = 1;
        if (Script_GetControllerInput(1) == 0x400) {
          task_1A2 = 0;
          Script_sicnUtils(1, 0, 0);
          local_FE = Script_PlayerStatsUtils(PS_GetStepSlot, 0);
          Script_scenCreateTask(script_A9F2, 1, local_FE);
          script_A8FC(SHEN_NAME('SLTG'), 1);
          local_F2 = 0;
        }
      }
      local_D4.x = 5.7f;
      (&local_D4)->y = 0.0f;
      (&local_D4)->z = 0.0f;
      local_C8.x = (&local_D4)->x;
      (&local_C8)->z = (&local_D4)->z;
      local_BC.x = (&local_D4)->x - 2.0f;
      (&local_BC)->z = (&local_D4)->z - 1.4f;
      local_B0.x = 4.6f;
      (&local_B0)->y = 0.0f;
      (&local_B0)->z = 1.0f;
      if (Script_SetTaskDataValue(CHARA_AKIR, &local_C8, &local_BC, 0) & script_1450C(CHARA_AKIR, &local_B0, 2.5f, 90.0f)) {
        local_F4 = 1;
        local_F2 = 2;
        if (Script_GetControllerInput(1) == 0x400) {
          Script_sicnUtils(1, 0, 0);
          Script_ryoControl(PCCTL_DisablePlayerControl);
          Script_shenClearFreeState(0);
          Script_ryoControl(PCCTL_UpdateRyoNavYaw, 0x52494B41);
          Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_STAND_LP, 0.0f, 60.0f, 0.0f, 1.0f, 30);
          local_E0 = Script_shenGetStats(STATS_SEGA_COINS);
          if (local_E0 >= 1) {
            script_14917();
            Script_ryoControl(PCCTL_DisablePlayerControl);
            SCNF_WaitFrames_0000(90);
            Script_executeAicaCommand(2720, 1, 30);
            Script_executeAicaCommand(2720, 2, 30);
            Script_executeAicaCommand(2720, 3, 30);
            script_145CA();
            Script_SetShouldSleepFlag(1);
            Script_TimedTransitionUtils(TIMED_TRANSITION_SET_ENABLED, 1);
            Script_shenRequestGameModuleTransition(Script_GetSceneID(), SHEN_NAME('D000'), 26);
            while (1) {
              SCNF_WaitFrames_0000(1);
            }
          } else {
            task_10.mode = 1;
            task_10.entry_idx = 0;
            Script_SetShouldSleepFlag(1);
            Script_TimedTransitionUtils(TIMED_TRANSITION_SET_ENABLED, 1);
            script_81E4(CHARA_AKIR, 0);
            while (1) {
              SCNF_WaitFrames_0000(1);
            }
          }
        }
      }
      if (local_F4 == 1) {
        if (local_F2 == 0) {
          Script_sicnUtils(0, 5, 1);
          Script_uiUtils(0, 28, 0);
          local_F2 = 1;
        }
        if (local_F2 == 2) {
          Script_sicnUtils(0, 4, 1);
          local_F2 = 1;
        }
      } else {
        if (local_F2 == 1) {
          Script_sicnUtils(1, 0, 0);
          Script_uiUtils(0, 0, 0);
          local_F2 = 0;
        }
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}
