#include "act02_mfsy.h"

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
  if (task_1CC != arg0) {
    Script_sprintf("Door Index = %d : call_type = %d : type = %d\012", task_B4[arg2], arg3, arg4);
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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
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
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
    local_F4 = 90.0f - local_F4;
    *(int32_t *)&local_F0 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
    if ((Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 1) & 0x80)) {
      if (Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 1) & 0x40) {
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
    local_FC = script_8C20(arg0, arg1, local_F8, local_F4 / (float)100, local_F0 / (float)100, arg3, task_B4[arg2]);
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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_8C20(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    if ((Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 1) & 4) == 0) {
      script_2411(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_1CC) {
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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_8C20(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    if ((Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 1) & 4) == 0) {
      script_2411(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_1CC) {
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
    if ((Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 1) & 4) == 0) {
      float local_F4;
      float local_F0;
      local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_F4 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
      *(int32_t *)&local_F0 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
      if ((Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 1) & 0x80)) {
        if (Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 1) & 0x40) {
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
      local_FC = script_8C20(arg0, arg1, local_F8, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_B4[arg2]);
      script_2411(arg0, arg1, arg2, arg3, local_FC, 1);
    } else {
      if (arg0 == task_1CC) {
        float local_F4;
        float local_F0;
        local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        *(int32_t *)&local_F4 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
        *(int32_t *)&local_F0 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
        local_FC = script_8C20(arg0, arg1, 6, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_B4[arg2]);
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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
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
  if (arg0 == task_1CC) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      local_FF7F = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_FF78 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + local_FF7F) * 9 + 5);
      *(int32_t *)&local_FF74 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + local_FF7F) * 9 + 6);
      local_FF7F = script_8C20(arg0, arg1, 5, local_FF78 / (float)100, local_FF74 / (float)100, arg3, -1);
      script_92E9(arg0, 0);
      *(int32_t *)&local_94 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + local_FF7F) * 9 + 1);
      *(int32_t *)&local_90 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + local_FF7F) * 9 + 2);
      local_8C = 0.0f;
      local_FC = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + local_FF7F) * 9);
      local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_94 - 3.0f) << 16) & 0x7FFF0000);
      local_9C.x = (&task_1D4)->x;
      (&local_9C)->y = task_1D4.y;
      (&local_9C)->z = task_1D4.z;
      Script_charaPosRotUtils(arg1, 12, &local_CC, &local_E4, CHARA_PART_Get);
      Script_charaPosRotUtils(arg1, 7, &local_C0, &local_D8, CHARA_PART_Get);
      Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
      if (arg0 == task_1CC) {
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
        Script_scenCharaSubmitMoveTargetRequest(arg0, &task_1D4, *(&task_1E0 + 1), local_FC, 0);
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
        Script_SetCharaMoveTarget(arg0, &task_1D4, *(&task_1E0 + 1), local_FC);
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
              local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
              local_FF6C = Script_ScenDataUtil(task_74, SCND_ReadU32, Script_ScenDataUtil(task_78, SCND_ReadU32, local_FF6C * 9 + 1) * 4 + 2);
              if (local_FF6C != -1) {
                if (arg0 == task_1CC) {
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
              local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
              local_FF6C = Script_ScenDataUtil(task_74, SCND_ReadU32, Script_ScenDataUtil(task_78, SCND_ReadU32, local_FF6C * 9 + 1) * 4 + 2);
              if (local_FF6C != -1) {
                if (arg0 == task_1CC) {
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
        local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
        local_FF6C = Script_ScenDataUtil(task_74, SCND_ReadU32, Script_ScenDataUtil(task_78, SCND_ReadU32, local_FF6C * 9 + 1) * 4 + 3);
        if (local_FF6C != -1) {
          if (arg0 == task_1CC) {
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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
    local_F8 = 98.0f - local_F8;
    local_F4 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
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
    local_F4 = Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 6);
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
        *(&task_1E0 + 1) = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_E4)->x - (&task_1D4)->x, (&local_E4)->z - task_1D4.z);
        script_668F(arg0, arg1, arg2, arg3, local_FC, local_F0);
      }
    } else if ((local_BC == 3) | (local_BC == 4) | (local_BC == 5) | (local_BC == 6)) {
      *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_3B270, SCND_ReadFloatBits, Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 6) * 5 + 3);
      *(int32_t *)&local_CC = Script_ScenDataUtil(&data_3B270, SCND_ReadFloatBits, Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 6) * 5 + 4);
      local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      local_BC = arg3;
      if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 20) | (local_BC == 23) | (local_BC == 28) | (local_BC == 29)) {
        if ((Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 1) & 0x80)) {
          if (Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 1) & 0x40) {
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
      local_FC = script_8C20(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_B4[arg2]);
      script_6767(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (local_BC == 7) {
        local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        local_BC = arg3;
        if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 20) | (local_BC == 23) | (local_BC == 28) | (local_BC == 29)) {
          if ((Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 1) & 0x80)) {
            if (Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 1) & 0x40) {
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
        if (Script_CycleManagerUtils(CYCLE_CMD_IS_DOOR_OR_MOBJ_TYPE2, Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 12)) == 1) {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_3B270, SCND_ReadFloatBits, Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_3B270, SCND_ReadFloatBits, Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_8C20(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_B4[arg2]);
          Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
          script_6BDE(arg0, arg1, arg2, arg3, local_FC, 0);
          Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
        } else {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_3B270, SCND_ReadFloatBits, Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_3B270, SCND_ReadFloatBits, Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_8C20(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_B4[arg2]);
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

__attribute__((scn_string_bindings(0, data_3BD8E)))
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
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_8C = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_88 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 7);
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
      local_FF78 = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
      local_FF7C = 18;
      local_80 = 18;
      local_A4 = 1.0f;
    } else {
      if (arg0 == task_1CC) {
        local_FF78 = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
        local_FF7C = 12;
        local_80 = 12;
      } else {
        local_FF78 = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
        local_FF7C = 18;
        local_80 = 18;
      }
      local_A4 = -1.0f;
    }
    script_00E5(arg0, local_FF78, arg2, arg3, arg4);
    local_FF78 = local_FF78 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_98 - 3.0f) << 16) & 0x7FFF0000);
    script_92E9(arg0, 0);
    local_C0.x = (&task_1D4)->x;
    (&local_C0)->y = task_1D4.y;
    (&local_C0)->z = task_1D4.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &task_1D4, *(&task_1E0 + 1), local_FF78);
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
    if (task_1CC == arg0) {
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
          script_233B(arg0, task_104[arg2]);
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
            local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
            local_FF64 = Script_ScenDataUtil(task_74, SCND_ReadU32, Script_ScenDataUtil(task_78, SCND_ReadU32, local_FF64 * 9 + 1) * 4 + 2);
            if (local_FF64 != -1) {
              if (arg0 == task_1CC) {
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
      if (task_1CC == arg0) {
        if (!(local_B8 <= local_94) & !(local_B8 >= local_90)) {
          local_86 = 0;
        }
      } else {
        if (!(local_B8 <= local_94) & !(local_B8 > local_88)) {
          *(int32_t *)&local_9C = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 8);
          local_B4 = local_B4 + local_9C * local_A4;
        }
      }
      if (!(local_B8 < local_90) & !(local_B8 >= local_90 + 1.5f)) {
        if (local_84 == 1) {
          int32_t local_FF64;
          local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
          local_FF64 = Script_ScenDataUtil(task_74, SCND_ReadU32, Script_ScenDataUtil(task_78, SCND_ReadU32, local_FF64 * 9 + 1) * 4 + 3);
          if (local_FF64 != -1) {
            if (arg0 == task_1CC) {
              Script_executeAicaCommand(local_FF64, 0, 0);
            } else {
              Script_Execute3DSoundCommand_Wrapper(local_FF64, 4, &local_C0, 5.0f, 20.0f, 0);
            }
          }
          local_84 = 2;
        }
        if (task_1CC != arg0) {
          if (local_FF74 == 0) {
            local_AC = local_B4 / (local_8C - local_90);
            local_FF74 = 1;
          }
        }
      }
      if (!(local_B8 < local_90) & !(local_B8 > local_8C)) {
        Script_EVT_GetPosition(arg0, local_FF7C, &local_F0, ENTITY_XFORM_MOTION_SPACE);
        if (task_1CC == arg0) {
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
      if (arg0 == task_1CC) {
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
    local_FF78 = task_B4[arg2];
    local_FF64 = arg3;
    if ((local_FF64 == 9) | (local_FF64 == 10) | (local_FF64 == 15)) {
      if (arg5) {
        Script_charaPosRotUtils(arg1, 12, &local_D8, 0, CHARA_PART_Set);
        Script_charaPosRotUtils(arg1, 7, &local_CC, 0, CHARA_PART_Set);
      } else {
        Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
      }
      if (task_1CC == arg0) {
        local_80 = (Script_ScenDataUtil(task_6C, SCND_ReadU32, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        local_80 = Script_ScenDataUtil(task_6C, SCND_ReadU32, local_FF78 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
      }
    } else if ((local_FF64 == 18) | (local_FF64 == 19) | (local_FF64 == 21) | (local_FF64 == 22)) {
      if (task_1CC == arg0) {
        local_80 = (Script_ScenDataUtil(task_6C, SCND_ReadU32, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_6C, SCND_ReadU32, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 8;
          Script_ScenDataUtil(task_6C, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
        }
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_6C, SCND_ReadU32, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 0x10;
          Script_ScenDataUtil(task_6C, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
        }
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_6C, SCND_ReadU32, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 2;
          Script_ScenDataUtil(task_6C, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
        }
      } else {
        if (arg5) {
          Script_charaPosRotUtils(arg1, 12, &local_D8, 0, CHARA_PART_Set);
          Script_charaPosRotUtils(arg1, 7, &local_CC, 0, CHARA_PART_Set);
        } else {
          Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
        }
        local_80 = (Script_ScenDataUtil(task_6C, SCND_ReadU32, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        local_80 = Script_ScenDataUtil(task_6C, SCND_ReadU32, local_FF78 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
      }
    } else {
      if ((local_FF64 == 20) | (local_FF64 == 23)) {
        if (task_1CC == arg0) {
          if ((arg4 == 0) | (arg4 == 1)) {
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_6C, SCND_ReadU32, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 0x10;
              Script_ScenDataUtil(task_6C, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
            }
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_6C, SCND_ReadU32, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 4;
              Script_ScenDataUtil(task_6C, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
            }
          } else {
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_6C, SCND_ReadU32, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 8;
              Script_ScenDataUtil(task_6C, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
            }
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_6C, SCND_ReadU32, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 4;
              Script_ScenDataUtil(task_6C, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
            }
          }
        } else {
          if (arg5) {
            Script_charaPosRotUtils(arg1, 12, &local_D8, 0, CHARA_PART_Set);
            Script_charaPosRotUtils(arg1, 7, &local_CC, 0, CHARA_PART_Set);
          } else {
            Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
          }
          local_80 = (Script_ScenDataUtil(task_6C, SCND_ReadU32, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_80, 1);
          local_80 = Script_ScenDataUtil(task_6C, SCND_ReadU32, local_FF78 * 13 + 2) & 0xFFFF;
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
  if (task_1CC == arg0) {
    local_FC = task_B4[arg1];
    {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_6C, SCND_ReadU32, local_FC * 13 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
    {
      int32_t local_F8;
      local_F8 = (Script_ScenDataUtil(task_6C, SCND_ReadU32, local_FC * 13 + 2) >> 16) & 0xFFFF;
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
  local_AC.x = (&task_1D4)->x;
  (&local_AC)->y = task_1D4.y;
  (&local_AC)->z = task_1D4.z;
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
    local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
    local_80 = Script_ScenDataUtil(task_74, SCND_ReadU32, Script_ScenDataUtil(task_78, SCND_ReadU32, local_80 * 9 + 1) * 4 + 2);
    if (local_80 != -1) {
      if (arg0 == task_1CC) {
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
        task_DC[arg2] = arg0;
        local_98 = -local_98;
        local_84 = 0;
      }
      local_8C = 0;
    } else {
      if ((local_93 == 1) & (local_8C == 0)) {
        int32_t local_80;
        local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
        local_80 = Script_ScenDataUtil(task_74, SCND_ReadU32, Script_ScenDataUtil(task_78, SCND_ReadU32, local_80 * 9 + 1) * 4 + 3);
        if (local_80 != -1) {
          if (arg0 == task_1CC) {
            Script_executeAicaCommand(local_80, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_80, 4, &local_AC, 5.0f, 20.0f, 0);
          }
        }
        local_93 = 0;
        local_8C = 1;
        local_98 = -local_98;
        local_84 = 0;
        task_DC[arg2] = 0;
      }
    }
    local_88 = task_B4[arg2];
    local_88 = Script_ScenDataUtil(task_6C, SCND_ReadU32, local_88 * 13 + 2) & 0xFFFF;
    local_88 = local_88 + ((local_9C - 0x4000) >> 12);
    if (local_84 != local_88) {
      Script_scenToggleCharaFlag(0, local_88, local_8C);
      local_84 = local_88;
    }
    local_9C = local_9C - local_98;
    if (local_9C >= 0xC000) {
      local_9C = 0xC000;
      local_88 = task_B4[arg2];
      local_88 = Script_ScenDataUtil(task_6C, SCND_ReadU32, local_88 * 13 + 2) & 0xFFFF;
      local_88 = local_88 + 8;
      Script_scenToggleCharaFlag(0, local_88, local_8C);
      break;
    }
    if (local_9C <= 0x4000) {
      local_9C = 0x4000;
      if (local_93 == 0) {
        task_DC[arg2] = 0;
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

__attribute__((scn_string_bindings(0, data_3BDAA)))
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
          local_FC = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
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
          local_FC = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
      }
      break;
  }
  *(int32_t *)&local_C4 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_C0 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 2);
  local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_C4 - 3.0f) << 16) & 0x7FFF0000);
  local_BC = 0.0f;
  script_92E9(arg0, 0);
  local_CC.x = (&task_1D4)->x;
  (&local_CC)->y = task_1D4.y;
  (&local_CC)->z = task_1D4.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1D4, *(&task_1E0 + 1), local_FC);
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
        local_A0 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
        local_A0 = Script_ScenDataUtil(task_74, SCND_ReadU32, Script_ScenDataUtil(task_78, SCND_ReadU32, local_A0 * 9 + 1) * 4 + 2);
        if (local_A0 != -1) {
          if (arg0 == task_1CC) {
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
  local_FC = task_B4[arg2];
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_6C, SCND_ReadU32, local_FC * 13 + 1);
    local_A0 = local_A0 | 8;
    Script_ScenDataUtil(task_6C, SCND_WriteU32, local_FC * 13 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_6C, SCND_ReadU32, local_FC * 13 + 1);
    local_A0 = local_A0 | 0x10;
    Script_ScenDataUtil(task_6C, SCND_WriteU32, local_FC * 13 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_6C, SCND_ReadU32, local_FC * 13 + 1);
    local_A0 = local_A0 | 4;
    Script_ScenDataUtil(task_6C, SCND_WriteU32, local_FC * 13 + 1, local_A0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_3BDC6)))
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
      if (arg0 == task_1CC) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 19842;
            break;
        }
      }
    } else if ((local_FF5C == 1) | (local_FF5C == 7) | (local_FF5C == 30) | (local_FF5C == 36) | (local_FF5C == 37)) {
      if (arg0 == task_1CC) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = 1.0f;
            local_A0 = 12;
            local_9C = 18;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            if (task_104[arg2] == 4) {
              local_90 = 50062;
            } else {
              local_90 = 51882;
            }
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            if (task_104[arg2] == 4) {
              local_90 = 46421;
            } else {
              local_90 = 45511;
            }
            break;
        }
      }
    } else if ((local_FF5C == 3) | (local_FF5C == 5) | (local_FF5C == 32) | (local_FF5C == 34)) {
      if (arg0 == task_1CC) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = -1.0f;
            local_A0 = 12;
            local_9C = 12;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 51882;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 45511;
            break;
        }
      }
    } else if ((local_FF5C == 2) | (local_FF5C == 8) | (local_FF5C == 31)) {
      if (arg0 == task_1CC) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 12;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 19842;
            break;
        }
      }
    } else {
      if ((local_FF5C == 13) | (local_FF5C == 14) | (local_FF5C == 28)) {
        if (arg0 == task_1CC) {
          local_FF5C = arg4;
          switch (local_FF5C) {
            case 0:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 12;
              local_90 = 48241;
              local_8C = 50062;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
              local_C0 = -1.0f;
              local_A0 = 12;
              local_9C = 12;
              local_90 = 50062;
              local_8C = 48241;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 15473;
              local_8C = 17294;
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
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
              local_A4 = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 45511;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 51882;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              if (task_104[arg2] == 4) {
                local_90 = 14199;
              } else {
                local_90 = 13653;
              }
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              if (task_104[arg2] == 4) {
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
  *(int32_t *)&local_B8 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_B4 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_B0 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_AC = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_A8 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 7);
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
  local_D0.x = (&task_1D4)->x;
  (&local_D0)->y = task_1D4.y;
  (&local_D0)->z = task_1D4.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1D4, *(&task_1E0 + 1), local_A4);
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
    if (arg0 == task_1CC) {
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
      if (arg0 == task_1CC) {
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
              script_233B(arg0, task_104[arg2]);
            }
          }
        }
      }
      if (!(local_C4 > local_B4)) {
        if (!(local_C4 < local_B8) & !(local_C4 > local_B4)) {
          if (arg0 == task_1CC) {
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
          local_FF50 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
          local_FF50 = Script_ScenDataUtil(task_74, SCND_ReadU32, Script_ScenDataUtil(task_78, SCND_ReadU32, local_FF50 * 9 + 1) * 4 + 2);
          if (local_FF50 != -1) {
            if (arg0 == task_1CC) {
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
      if (arg0 != task_1CC) {
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
          *(int32_t *)&local_BC = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 8);
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
          if (arg0 != task_1CC) {
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
        local_FF50 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
        local_FF50 = Script_ScenDataUtil(task_74, SCND_ReadU32, Script_ScenDataUtil(task_78, SCND_ReadU32, local_FF50 * 9 + 1) * 4 + 3);
        if (local_FF50 != -1) {
          if (arg0 == task_1CC) {
            Script_executeAicaCommand(local_FF50, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_FF50, 4, &local_D0, 5.0f, 20.0f, 0);
          }
        }
        local_A4 = task_B4[arg2];
        if (arg0 == task_1CC) {
          if (task_1CC == arg0) {
            local_A0 = (Script_ScenDataUtil(task_6C, SCND_ReadU32, local_A4 * 13 + 2) >> 16) & 0xFFFF;
            Script_scenToggleCharaFlag(0, local_A0, 1);
            local_A0 = Script_ScenDataUtil(task_6C, SCND_ReadU32, local_A4 * 13 + 2) & 0xFFFF;
            Script_scenToggleCharaFlag(0, local_A0, 1);
          }
        }
      }
      if (!(local_AC <= 0.0f)) {
        if (!(local_C4 <= local_AC)) {
          if (arg0 == task_1CC) {
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
        if (arg0 == task_1CC) {
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
      if (arg0 != task_1CC) {
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
  if (arg0 == task_1CC) {
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

__attribute__((scn_string_bindings(0, data_3BDE2)))
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
    local_C4.x = (&task_1D4)->x;
    (&local_C4)->y = task_1D4.y;
    (&local_C4)->z = task_1D4.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &task_1D4, *(&task_1E0 + 1), local_94);
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
          local_84 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
          local_84 = Script_ScenDataUtil(task_74, SCND_ReadU32, Script_ScenDataUtil(task_78, SCND_ReadU32, local_84 * 9 + 1) * 4 + 3);
          if (local_84 != -1) {
            if (arg0 == task_1CC) {
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
    local_94 = task_B4[arg2];
    local_9C = Script_ScenDataUtil(task_6C, SCND_ReadU32, local_94 * 13 + 2) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_9C, 1);
    local_9C = (Script_ScenDataUtil(task_6C, SCND_ReadU32, local_94 * 13 + 2) >> 16) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_9C, 1);
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_6C, SCND_ReadU32, local_94 * 13 + 1);
      local_84 = local_84 & -9;
      Script_ScenDataUtil(task_6C, SCND_WriteU32, local_94 * 13 + 1, local_84);
    }
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_6C, SCND_ReadU32, local_94 * 13 + 1);
      local_84 = local_84 & -17;
      Script_ScenDataUtil(task_6C, SCND_WriteU32, local_94 * 13 + 1, local_84);
    }
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_6C, SCND_ReadU32, local_94 * 13 + 1);
      local_84 = local_84 & -5;
      Script_ScenDataUtil(task_6C, SCND_WriteU32, local_94 * 13 + 1, local_84);
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_3BDFE)))
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
      local_8C = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
      local_A0 = -1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 46421;
      local_FF74 = 48787;
      break;
    case 1:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = -1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 50426;
      local_FF74 = 48969;
      break;
    case 2:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
      local_A0 = 1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 15109;
      local_FF74 = 16566;
      break;
    case 3:
      local_AF = 0;
      local_8C = Script_ScenDataUtil(data_3A850, SCND_ReadU32, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = 1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 19114;
      local_FF74 = 16748;
      break;
  }
  (void)local_FF78;
  *(int32_t *)&local_9C = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_3A850, SCND_ReadFloatBits, Script_ScenDataUtil(data_3A260, SCND_ReadU8, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 4);
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
  local_B0.x = (&task_1D4)->x;
  (&local_B0)->y = task_1D4.y;
  (&local_B0)->z = task_1D4.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1D4, *(&task_1E0 + 1), local_8C);
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
        local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
        local_FF64 = Script_ScenDataUtil(task_74, SCND_ReadU32, Script_ScenDataUtil(task_78, SCND_ReadU32, local_FF64 * 9 + 1) * 4 + 2);
        if (local_FF64 != -1) {
          if (arg0 == task_1CC) {
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
        local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
        local_FF64 = Script_ScenDataUtil(task_74, SCND_ReadU32, Script_ScenDataUtil(task_78, SCND_ReadU32, local_FF64 * 9 + 1) * 4 + 3);
        if (local_FF64 != -1) {
          if (arg0 == task_1CC) {
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
  Script_SetCharaMoveTarget(arg0, &task_1D4, *(&task_1E0 + 1), MN_AKI_AKI_BASE);
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
        local_FC = Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 7);
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

__attribute__((scn_string_bindings(0, data_3BE1A)))
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
  local_D4 = Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 6);
  local_CC = arg3;
  if ((local_CC == 1) | (local_CC == 3) | (local_CC == 5) | (local_CC == 7) | (local_CC == 9) | (local_CC == 18) | (local_CC == 21) | (local_CC == 25) | (local_CC == 27) | (local_CC == 36) | (local_CC == 37)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_3B270, SCND_ReadU32, local_D4 * 5) | -0x80000000;
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_3B270, SCND_ReadU32, local_D4 * 5);
        break;
    }
  } else if ((local_CC == 2) | (local_CC == 4) | (local_CC == 6) | (local_CC == 8) | (local_CC == 10) | (local_CC == 19) | (local_CC == 22) | (local_CC == 24) | (local_CC == 26) | (local_CC == 28)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_3B270, SCND_ReadU32, local_D4 * 5);
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_3B270, SCND_ReadU32, local_D4 * 5) | -0x80000000;
        break;
    }
  } else {
    if ((local_CC == 13) | (local_CC == 14) | (local_CC == 15) | (local_CC == 16) | (local_CC == 20) | (local_CC == 23) | (local_CC == 29)) {
      local_CC = arg4;
      switch (local_CC) {
        case 0:
          local_E0 = Script_ScenDataUtil(&data_3B270, SCND_ReadU32, local_D4 * 5);
          break;
        case 1:
          local_E0 = Script_ScenDataUtil(&data_3B270, SCND_ReadU32, local_D4 * 5) | -0x80000000;
          break;
        case 2:
          local_E0 = Script_ScenDataUtil(&data_3B270, SCND_ReadU32, local_D4 * 5);
          break;
        case 3:
          local_E0 = Script_ScenDataUtil(&data_3B270, SCND_ReadU32, local_D4 * 5) | -0x80000000;
          break;
      }
      goto L006988;
    }
  }
  L006988:;
  *(int32_t *)&local_EC = Script_ScenDataUtil(&data_3B270, SCND_ReadFloatBits, local_D4 * 5 + 1);
  *(int32_t *)&local_E8 = Script_ScenDataUtil(&data_3B270, SCND_ReadFloatBits, local_D4 * 5 + 2);
  local_E0 = local_E0 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_EC - 3.0f) << 16) & 0x7FFF0000);
  local_F4.x = (&task_1D4)->x;
  (&local_F4)->y = task_1D4.y;
  (&local_F4)->z = task_1D4.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1D4, *(&task_1E0 + 1), local_E0);
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
      local_C8 = Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 9);
      if (local_C8 != -1) {
        if (arg0 == task_1CC) {
          Script_executeAicaCommand(local_C8, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_C8, 4, &local_F4, 5.0f, 20.0f, 0);
        }
      }
    }
    if (!(local_F0 < local_E8) & !(local_F0 >= local_E8 + 1.0f)) {
      if ((local_D4 == 6) | (local_D4 == 7)) {
        int32_t local_C8;
        local_C8 = Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 9);
        if (local_C8 != -1) {
          if (arg0 == task_1CC) {
            Script_executeAicaCommand(local_C8, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_C8, 4, &local_F4, 5.0f, 20.0f, 0);
          }
        }
      }
      if (local_DC == 0) {
        local_DC = 1;
        local_D8 = Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 7);
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

__attribute__((scn_string_bindings(0, data_3BE36)))
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
  task_1F8 = 0;
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
  local_B8.x = (&task_1D4)->x;
  (&local_B8)->y = task_1D4.y;
  (&local_B8)->z = task_1D4.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1D4, *(&task_1E0 + 1), local_90);
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
        local_FF40 = Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 10);
        if (local_FF40 != -1) {
          if (arg0 == task_1CC) {
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
      local_FF40 = Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 9);
      if (local_FF40 != -1) {
        if (arg0 == task_1CC) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
    }
    if (!(local_A0 < local_94) & !(local_A0 >= local_94 + 1.0f)) {
      int32_t unused_FF44;
      int32_t local_FF40;
      local_FF40 = Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 9);
      if (local_FF40 != -1) {
        if (arg0 == task_1CC) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
      if (local_FF68 == 0) {
        local_FF68 = 1;
        local_FF64 = Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 8);
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
      local_FF40 = Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 11);
      if (local_FF40 != -1) {
        if (arg0 == task_1CC) {
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
    if (arg0 != task_1CC) {
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
  task_6C = arg1;
  task_70 = arg2;
  task_74 = arg3;
  task_78 = arg4;
  task_7C.mode = 0;
  task_7C.id = 0;
  task_7C.entry_idx = 0;
  task_1CC = arg0;
  (void)((task_1F0 = -1) & 1);
  task_1EC = 0;
  Script_motiLoad(Script_GetSceneID(), SHEN_NAME('FREE'), "M_MDOR.BIN");
  local_F8 = 0;
  L008025:;
  local_FC = Script_AddCharacterHandler(1, 0, local_F8 * 0x1000000 + 0x30726F64);
  Script_PreserveAccumulator_012C(local_FC, "This Charactor Work is NULL!!\012");
  DSWK_AllocDoorMapcRenderState(local_FC);
  Script_scenToggleVisibleFor(local_FC, 0);
  task_154[local_F8] = local_FC;
  task_8C[local_F8] = -1;
  task_B4[local_F8] = -1;
  task_DC[local_F8] = 0;
  task_104[local_F8] = 0;
  task_12C[local_F8] = 0;
  task_17C[local_F8] = 0;
  local_F8 = local_F8 + 1;
  if (!(local_F8 >= 10)) goto L008025;
  local_F8 = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_6C, SCND_ReadU32, local_F8 * 13) == -1) {
      break;
    }
    Script_ScenDataUtil(task_6C, SCND_WriteU32, local_F8 * 13 + 1, 0);
    local_F8 = local_F8 + 1;
  }
  Script_EnqueueSCNT(0);
  DSWK_BindTablesAndInitState(arg1, arg2, arg3, arg4, &task_7C, task_8C, task_B4, (shen_name *)task_DC, task_104, arg0, arg5, data_3A260, data_3A850, (shen_name *)task_154);
  Script_scenCreateTask(script_8877, 0);
  return;
}

static void script_8147(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4) {
  task_6C = arg0;
  task_70 = arg1;
  task_74 = arg2;
  task_78 = arg3;
  DSWK_RebindDoorSystemTables(arg0, arg1, arg2, arg3, arg4);
  task_1EC = 1;
  return;
}

static void script_817B(void) {
  int32_t local_FC;
  local_FC = 0;
  L008183:;
  task_8C[local_FC] = -1;
  task_B4[local_FC] = -1;
  task_DC[local_FC] = 0;
  task_104[local_FC] = 0;
  task_12C[local_FC] = 0;
  task_17C[local_FC] = 0;
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
    if (task_1CC == arg0) {
      Script_EVT_GetPosition(arg0, -1, &task_200, 0);
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
  if (task_1CC == arg0) {
    Script_EVT_GetPosition(arg0, -1, &task_200, 0);
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
    if (Script_ScenDataUtil(task_6C, SCND_ReadU32, local_FC * 13) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_6C, SCND_ReadU32, local_FC * 13 + 1);
    local_F8 = local_F8 | 2;
    Script_ScenDataUtil(task_6C, SCND_WriteU32, local_FC * 13 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_831E(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_6C, SCND_ReadU32, local_FC * 13) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_6C, SCND_ReadU32, local_FC * 13 + 1);
    local_F8 = local_F8 & -3;
    Script_ScenDataUtil(task_6C, SCND_WriteU32, local_FC * 13 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_8381(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_6C, SCND_ReadU32, arg0 * 13 + 1);
  local_FC = local_FC | 2;
  Script_ScenDataUtil(task_6C, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  return;
}

static void script_83B8(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_6C, SCND_ReadU32, arg0 * 13 + 1);
  local_FC = local_FC & -3;
  Script_ScenDataUtil(task_6C, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  return;
}

static void script_83EF(int32_t arg0) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  local_F0 = 0;
  for (;;) {
    if (task_B4[local_F0] == arg0) {
      local_EC = task_154[local_F0];
      if (Script_shenGetNode(local_EC, 7)) {
        if (Script_shenGetNode(local_EC, 5) & Script_shenGetNode(local_EC, 10)) {
          if (Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[local_F0] * 13 + 1) & 0x100) {
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
          if (Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[local_F0] * 13 + 1) & 0x100) {
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
        if (Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[local_F0] * 13 + 1) & 0x20) {
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
  local_FC = Script_ScenDataUtil(task_6C, SCND_ReadU32, arg0 * 13 + 1);
  local_FC = local_FC | 0x20;
  Script_ScenDataUtil(task_6C, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  script_8739(arg0);
  script_83EF(arg0);
  return;
}

static void script_867A(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_6C, SCND_ReadU32, arg0 * 13 + 1);
  local_FC = local_FC & -33;
  Script_ScenDataUtil(task_6C, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  script_8747(arg0);
  script_83EF(arg0);
  return;
}

static void script_86C3(int32_t arg0, int32_t arg1) {
  if (arg1) {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_6C, SCND_ReadU32, arg0 * 13 + 1);
    local_FC = local_FC & -257;
    Script_ScenDataUtil(task_6C, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  } else {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_6C, SCND_ReadU32, arg0 * 13 + 1);
    local_FC = local_FC | 0x100;
    Script_ScenDataUtil(task_6C, SCND_WriteU32, arg0 * 13 + 1, local_FC);
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
    if (Script_ScenDataUtil(task_6C, SCND_ReadU32, local_FC * 13) == -1) {
      break;
    }
    if (arg0) {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_6C, SCND_ReadU32, local_FC * 13 + 1);
      local_F8 = local_F8 & -65;
      local_F8 = local_F8 & -129;
      local_F8 = local_F8 | 0x40;
      Script_ScenDataUtil(task_6C, SCND_WriteU32, local_FC * 13 + 1, local_F8);
    } else {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_6C, SCND_ReadU32, local_FC * 13 + 1);
      local_F8 = local_F8 & -65;
      local_F8 = local_F8 & -129;
      local_F8 = local_F8 | 0x80;
      Script_ScenDataUtil(task_6C, SCND_WriteU32, local_FC * 13 + 1, local_F8);
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
    if (Script_ScenDataUtil(task_6C, SCND_ReadU32, local_FC * 13) == -1) {
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
    if (task_1EC == 0) {
      local_F4 = DSWK_DoorUtils(DOOR_CMD_FIND_LOWEST_FLOAT_INDEX, 0x3FD851EA);
      if (-(local_F4 != -1) & -(task_8C[local_F4] == 1) & Script_IsPlayerAllowed(0) & -((&task_7C)->mode == 0)) {
        local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[local_F4], 2);
        local_F8 = Script_ScenDataUtil(task_74, SCND_ReadU32, Script_ScenDataUtil(task_78, SCND_ReadU32, local_FC * 9 + 1) * 4 + 1);
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
      task_1EC = 0;
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
  Script_EVT_GetPosition(task_1CC, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(task_1CC, &local_DC, 0x42000000);
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

__attribute__((scn_string_bindings(0, data_3BEC1, 1, data_3BEDA)))
static void script_8A20(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  Vector3i local_E0;
  local_F8 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg0], 1);
  if (local_F8 != -1) {
    local_FC = Script_AddCharacterHandler(1, 0, arg0 * 0x1000000 + 0x30786F62);
    if (local_FC == 0) {
      Script_PreserveAccumulator_012C(local_FC, "Don't make BOX Buffer!!\012");
      task_1A4[arg0] = -1;
    } else if (local_FC == arg0 * 0x1000000 + 0x30786F62) {
      task_1A4[arg0] = local_FC;
      *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_78, SCND_ReadU32, local_F8 * 9 + 5);
      *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_78, SCND_ReadU32, local_F8 * 9 + 6);
      *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_78, SCND_ReadU32, local_F8 * 9 + 7);
      Script_EVT_SetPosition(local_FC, POS_XYZ, &local_EC);
      local_E0.x = 0;
      (&local_E0)->y = Script_ScenDataUtil(task_78, SCND_ReadU32, local_F8 * 9 + 8);
      (&local_E0)->z = 0;
      Script_EVT_SetRotation(local_FC, 0x38000000, &local_E0, 0);
      *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_78, SCND_ReadU32, local_F8 * 9 + 2);
      *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_78, SCND_ReadU32, local_F8 * 9 + 3);
      *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_78, SCND_ReadU32, local_F8 * 9 + 4);
      Script_hmdlApplyScale(local_FC, &local_EC);
      Script_CreateNPCImageModule(local_FC, (const char *)Script_ScenDataUtil(task_74, SCND_ReadScriptOffset, Script_ScenDataUtil(task_78, SCND_ReadU32, local_F8 * 9 + 1) * 4), 0);
      Script_charaDisplay(local_FC, CDM_Show);
      Script_charaVisUtils(local_FC, 1);
    } else {
      Script_PreserveAccumulator_012C(local_FC, "Don't make BOX Buffer!!\012");
      task_1A4[arg0] = -1;
    }
  } else {
    task_1A4[arg0] = -1;
  }
  return;
}

static void script_8BEF(int32_t arg0) {
  if (task_1A4[arg0] != -1) {
    Script_SetMapControlTaskFlag(task_1A4[arg0], 1);
    task_1A4[arg0] = -1;
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
  if (task_1CC == arg0) {
    local_F4.x = (&task_200)->x;
    (&local_F4)->y = task_200.y;
    (&local_F4)->z = task_200.z;
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
        if ((Script_ScenDataUtil(task_6C, SCND_ReadU32, arg6 * 13 + 1) & 8) | (Script_ScenDataUtil(task_6C, SCND_ReadU32, arg6 * 13 + 1) & 0x40)) {
          local_C0 = local_C0 ^ 3;
        }
      } else {
        if ((Script_ScenDataUtil(task_6C, SCND_ReadU32, arg6 * 13 + 1) & 0x10) | (Script_ScenDataUtil(task_6C, SCND_ReadU32, arg6 * 13 + 1) & 0x80)) {
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
          task_1D4.x = (&local_F4)->x;
          task_1D4.y = (&local_F4)->y;
          task_1D4.z = (&local_F4)->z;
          task_1E0 = (&local_C4)->x;
          *(&task_1E0 + 1) = (&local_C4)->y;
          *(&task_1E0 + 2) = (&local_C4)->z;
          break;
        case 5:
          task_1D4.x = (&local_F4)->x;
          task_1D4.y = (&local_F4)->y;
          task_1D4.z = (&local_F4)->z;
          task_1E0 = (&local_C4)->x;
          *(&task_1E0 + 1) = (&local_C4)->y;
          *(&task_1E0 + 2) = (&local_C4)->z;
          if ((local_C0 == 1) | (local_C0 == 2)) {
            local_C0 = 1;
          } else {
            local_C0 = 0;
          }
          break;
        case 7:
          Script_EVT_GetPosition(arg0, -1, &task_1D4, 0);
          task_1E0 = (&local_C4)->x;
          *(&task_1E0 + 1) = (&local_C4)->y;
          *(&task_1E0 + 2) = (&local_C4)->z;
          break;
      }
    }
  }
  return local_C0;
}

static void script_92E9(int32_t arg0, int32_t arg1) {
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    if (task_1CC == arg0) {
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
  if (task_1CC != arg0) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      Script_charaToggleMotEnabled(arg0, 1);
    }
  } else if ((local_F8 == 0) | (local_F8 == 1)) {
    if ((Script_ScenDataUtil(task_6C, SCND_ReadU32, arg2 * 13 + 1) & 8) == 0) {
      if ((Script_ScenDataUtil(task_6C, SCND_ReadU32, arg2 * 13 + 1) & 0x40) != 0) {
        local_FC = Script_ScenDataUtil(task_6C, SCND_ReadU32, arg2 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      } else {
        local_FC = (Script_ScenDataUtil(task_6C, SCND_ReadU32, arg2 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      }
    }
  } else {
    if ((Script_ScenDataUtil(task_6C, SCND_ReadU32, arg2 * 13 + 1) & 0x10) == 0) {
      if ((Script_ScenDataUtil(task_6C, SCND_ReadU32, arg2 * 13 + 1) & 0x80) != 0) {
        local_FC = (Script_ScenDataUtil(task_6C, SCND_ReadU32, arg2 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      } else {
        local_FC = Script_ScenDataUtil(task_6C, SCND_ReadU32, arg2 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      }
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_3BEF3)))
static void script_948B(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (task_1CC == arg0) {
    task_7C.mode = 0;
    task_7C.id = 0;
    if ((arg2 == 0) | (arg2 == 2)) {
      Script_PlayCharaMotion(task_1CC, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      if (task_1F0 & 0x10) {
        Script_sprintf("Door System : ");
        Script_ryoControl(RYO_RestoreControl);
      }
      Script_shenClearFreeState(1);
      if (arg2 != 2) {
        int32_t local_F8;
        local_F8 = task_B4[arg1];
        local_FC = Script_ScenDataUtil(task_6C, SCND_ReadU32, local_F8 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 1);
        local_FC = (Script_ScenDataUtil(task_6C, SCND_ReadU32, local_F8 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 1);
      }
    }
  } else {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      Script_charaToggleMotEnabled(arg0, 0);
    }
    if ((arg2 == 0) | (arg2 == 2)) {
      int32_t local_F8;
      local_F8 = task_B4[arg1];
      local_FC = Script_ScenDataUtil(task_6C, SCND_ReadU32, local_F8 * 13 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
      local_FC = (Script_ScenDataUtil(task_6C, SCND_ReadU32, local_F8 * 13 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
    }
  }
  task_8C[arg1] = 1;
  task_17C[arg1] = 0;
  task_12C[arg1] = 0;
  task_DC[arg1] = 0;
  if (task_1A4[arg1] != -1) {
    Script_SetMapControlTaskFlag(task_1A4[arg1], 1);
    task_1A4[arg1] = -1;
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
    } else if ((Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 1) & 2) == 0) {
      if ((Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 1) & 4) == 0) {
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
      if ((Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 1) & 4) == 0) {
        local_FC = script_8988(arg2, arg3, local_FC);
        local_F8 = Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg2] * 13 + 6);
        if (local_F8 == 0) {
          local_FC = 0;
        } else {
          if (local_F8 >= 2) {
            Script_EVT_GetPosition(task_1CC, -1, &local_E8, 0);
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
  if (task_17C[arg2]) {
    local_FC = 0;
  }
  Script_scenUtils(SCENARIO_CMD_SET_HELP_PROMPT_OVERRIDE, local_FC);
  if ((local_FC ^ task_1FC) & local_FC) {
    Script_sicnUtils(0, 4, 1);
  }
  if ((local_FC ^ task_1FC) & task_1FC) {
    Script_sicnUtils(2, 4, 0);
  }
  task_1FC = local_FC;
  return;
}

__attribute__((scn_string_bindings(0, data_3BF02)))
static int32_t script_97FB(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  local_FC = 1;
  local_F8 = arg2;
  if ((local_F8 == 17) | (local_F8 == 11) | (local_F8 == 12)) {
    if (((Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg1] * 13 + 1) & 2) == 0) & ((Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg1] * 13 + 1) & 4) == 0)) {
      if (task_17C[arg1] == 0) {
        task_8C[arg1] = 4;
        local_FC = 0;
      }
    }
  } else {
    if (Script_GetControllerInput(1) == 0x400) {
      if (-(task_DC[arg1] != task_1CC) & task_1FC) {
        if ((Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg1] * 13 + 1) & 2) == 0) {
          task_8C[arg1] = 3;
          task_DC[arg1] = task_1CC;
          task_104[arg1] = 0;
          task_7C.mode = 1;
          task_7C.entry_idx = task_B4[arg1];
          local_FC = 0;
        } else {
          if (Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg1] * 13 + 6) >= 2) {
            Script_EVT_GetPosition(task_1CC, -1, &local_EC, 0);
            if (Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_EC, 1.5f) == 0) {
              task_8C[arg1] = 6;
              task_DC[arg1] = task_1CC;
              task_104[arg1] = 0;
              task_7C.mode = 0;
              task_7C.entry_idx = task_B4[arg1];
              local_FC = 0;
            }
            goto L0099AF;
          }
        }
        L0099AF:;
        if (local_FC == 0) {
          Script_EVT_GetPosition(task_1CC, -1, &task_200, 0);
          Script_ryoControl(PCCTL_RequestNewMode, 4);
          Script_sprintf("Door System : ");
          Script_shenClearFreeState(0);
          Script_ryoControl(PCCTL_DisablePlayerControl);
          task_7C.id = task_1CC;
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
  local_F8 = task_8C[local_FC];
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
  if (task_17C[arg0] == 0) {
    task_17C[arg0] = Script_scenCreateTask(script_1C76, 4, task_DC[arg0], task_154[arg0], arg0, arg1);
    task_12C[arg0] = task_DC[arg0];
    script_8A20(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_3BF11, 1, data_3BF23)))
static void script_9B3C(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, task_DC[arg0])) {
    if (task_17C[arg0] == 0) {
      if (task_DC[arg0] == task_1CC) {
        task_7C.mode = 1;
        task_7C.entry_idx = task_B4[arg0];
        goto L009B91;
      }
      L009B91:;
      local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg0], 2);
      local_F8 = Script_ScenDataUtil(task_74, SCND_ReadU32, Script_ScenDataUtil(task_78, SCND_ReadU32, local_FC * 9 + 1) * 4 + 1);
      script_931B(task_DC[arg0], task_154[arg0], task_B4[arg0], local_F8);
      task_17C[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_3B310, SCND_ReadScriptOffset, local_F8), 4, task_DC[arg0], task_154[arg0], arg0, local_F8);
      task_12C[arg0] = task_DC[arg0];
      if ((Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg0] * 13 + 1) & 0x20000) == 0) {
        Script_sprintf("Door Box is ON!!\012");
        script_8A20(arg0);
      } else {
        Script_sprintf("Door Box is OFF!!\012");
        task_1A4[arg0] = -1;
      }
    }
  } else {
    if (task_8C[arg0] != 5) {
      task_8C[arg0] = 1;
      task_17C[arg0] = 0;
      task_12C[arg0] = 0;
      task_DC[arg0] = 0;
    }
  }
  return;
}

static void script_9CE1(int32_t arg0, int32_t arg1, int32_t arg2) {
  if ((task_17C[arg0] == 0) & (arg2 >= 0)) {
    task_DC[arg0] = task_1CC;
    task_7C.mode = 2;
    task_7C.entry_idx = task_B4[arg0];
    task_17C[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_3B310, SCND_ReadScriptOffset, arg2), 4, task_DC[arg0], task_154[arg0], arg0, arg2);
    task_12C[arg0] = task_DC[arg0];
    script_8A20(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_3BF36, 1, data_3BF48)))
static void script_9D79(int32_t arg0, int32_t arg1) {
  if ((task_17C[arg0] == 0) & (arg1 >= 0)) {
    script_931B(task_DC[arg0], task_154[arg0], task_B4[arg0], arg1);
    task_17C[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_3B310, SCND_ReadScriptOffset, arg1), 4, task_DC[arg0], task_154[arg0], arg0, arg1);
    task_12C[arg0] = task_DC[arg0];
    if ((Script_ScenDataUtil(task_6C, SCND_ReadU32, task_B4[arg0] * 13 + 1) & 0x20000) == 0) {
      Script_sprintf("Door Box is ON!!\012");
      script_8A20(arg0);
    } else {
      Script_sprintf("Door Box is OFF!!\012");
      task_1A4[arg0] = -1;
    }
  }
  return;
}

static void script_9E5B(int32_t arg0) {
  int32_t local_FC;
  task_17C[arg0] = 0;
  task_12C[arg0] = 0;
  task_8C[arg0] = 1;
  local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg0], 2);
  if (local_FC != -1) {
    task_DC[arg0] = 0;
  }
  return;
}

static void script_9EB2(void) {
  int32_t local_FC;
  local_FC = Script_GetSceneID();
  switch (local_FC) {
    case 1:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ENTRY2_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3044484A, 2, &data_3C7E0, 0xD5A6C3, TimedTransitionUpdateMemoItem150);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_ENTRY20_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3030444A, 20, &data_3C7A8, 0xDA3AA3, 0);
      }
      break;
    case 2:
      if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_STARTED) == 0) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_ADVANCED) == 0)) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 2, 0x54434D47, 1, &data_3C770, 0xB7420F, 0);
      }
      break;
    case 3:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x4F4D4F4A, 20, &data_3C78C, 0x2DC9FE, TimedTransitionRollbackToPreviousDay2330);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIRED_FROM_JOB) & ~Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 970)) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_ENTRY42_TIMED_EVENT_DONE) == 0) {
          Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x5953464D, 42, &data_3C7C4, 0xBBB817, 0);
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
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  Script_facemotUtils(arg0, 1, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, FACE_ExpressionDisabled, 0, 2);
  Script_CharHandlers(arg0, -1, 0, 0);
  script_9EB2();
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

static void script_A10E(int32_t arg0) {
  script_9FF6(arg0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_A125(int32_t arg0) {
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

static void script_A166(int32_t arg0) {
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

static void script_A1AA(int32_t arg0) {
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

static void script_A228(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_28_REACHED)) {
    script_A1AA(40);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_1E_REACHED)) {
    script_A1AA(30);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_14_REACHED)) {
    script_A1AA(20);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CATA1_NBO_BRANCH_CHOSEN)) {
    script_A1AA(10);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP05_SA1116_DOJO_DONE)) {
    script_A1AA(5);
  } else {
    script_A1AA(0);
  }
  return;
}

static void script_A2A7(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  return;
}

static void script_A2B0(void) {
  Script_ryoControl(RYO_RestoreControl);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_A2BF(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L00A2EB;
    }
  }
  L00A2EB:;
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

static void script_A34C(int32_t arg0) {
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

static int32_t script_A39C(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_A3CA(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_A3F8(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_A42C(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_A460(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 150, 68, 50, 0);
  return;
}

static void script_A47E(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static void script_A4B8(int32_t arg0, int32_t arg1) {
  while (arg1 > 0) {
    Script_SetFightProgramStateBytes(arg0, arg1, 1);
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  return;
}

static void script_A4EC(void) {
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

static void script_A5E4(int32_t arg0, int32_t arg1) {
  Script_FACE_LCG(arg0, FACE_BlinkClose, arg1, 0);
  SCNF_WaitFrames_0000(arg1 + 1);
  Script_FACE_LCG(arg0, FACE_BlinkOpen, arg1, 0);
  SCNF_WaitFrames_0000(arg1);
  return;
}

static void script_A614(int32_t arg0) {
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_SetCharaRefBoardPos(arg0, &data_3D3AC);
  return;
}

static void script_A62C(int32_t arg0) {
  Script_facemotUtils(arg0, 1, 0);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 1, data_3CE54, 1);
  Script_handSetFloatsFor(arg0, 0, data_3CE54, 1);
  return;
}

static void script_A671(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  return;
}

static void script_A699(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
  return;
}

static void script_A6ED(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static int32_t script_A73D(int32_t arg0) {
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

static int32_t script_A7E4(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_A73D(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  end:;
}

static int32_t script_A807(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = (&local_F4)->hours * 60 + (&local_F4)->minutes;
  local_EC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, arg0) * 6;
  return local_F0 - local_EC;
}

static void script_A83A(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_A807(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_A85D(int32_t arg0, int32_t arg1) {
  int8_t local_F4[12];
  int32_t local_F0;
  local_F0 = script_A807(arg0);
  local_F4[4] = local_F0 / 60;
  local_F4[5] = local_F0 % 60;
  if ((local_F4[4] < *(int8_t *)(arg1 + 4)) | ((local_F4[4] == *(int8_t *)(arg1 + 4)) & (local_F4[5] < *(int8_t *)(arg1 + 5)))) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static int32_t script_A8BC(int32_t arg0) {
  int32_t local_F8[2];
  local_F8[0] = arg0;
  local_F8[1] = 0;
  return (int32_t)local_F8;
}

static void script_A8D1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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

static void script_AC67(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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

__attribute__((scn_string_bindings(0, data_3D5AC)))
static void script_B2F5(void) {
  Script_sprintf("TUNED FreeTalk def\012");
  SCNT_Enqueue();
  return;
}

__attribute__((scn_string_bindings(0, data_3D5C0)))
static void script_B304(void) {
  Script_sprintf("TUNED FreeTalk def\012");
  SCNT_Enqueue();
  return;
}

static void script_B313(void) {
  return;
}

static void script_B316(void) {
  return;
}

static void script_B319(void) {
  return;
}

static void script_B31C(void) {
  return;
}

static void script_B31F(void) {
  (void)((task_218 = 0) & 0x20);
  return;
}

static int32_t script_B32C(void) {
  return task_20C = Script_freeTalkGetCurrentChara();
}

static void script_B338(void) {
  *(&task_20C + 1) = Script_GetFreeTalkStatus();
  return;
}

static int32_t script_B347(void) {
  return *(&task_20C + 2) = Script_freeTalkGetAndResetState();
}

static void script_B356(int32_t arg0) {
  int8_t local_FF;
  int32_t local_F8;
  local_FF = 0;
  local_F8 = Script_shenGetStats(STATS_CASH);
  if (local_F8 - arg0 < 69000) {
    local_FF = 1;
  }
  (void)local_FF;
  return;
}

static int32_t script_B382(int32_t arg0, int32_t arg1) {
  int8_t local_FF;
  int32_t local_F8;
  (void)((local_FF = 0) & 4);
  local_F8 = Script_shenGetStats(STATS_CASH);
  if (local_F8 - arg1 >= arg0) {
    (void)((local_FF = -1) & 4);
  }
  return local_FF & 0x80;
}

static int8_t script_B3B5(int32_t arg0, int32_t arg1, int32_t arg2) {
  int8_t local_FF;
  int32_t local_F8;
  int32_t local_F4;
  int8_t local_F3;
  int32_t local_EC;
  int32_t local_E8;
  local_F8 = 0;
  local_F4 = 0;
  local_FF = 0;
  local_EC = Script_StepUtils(GET_STEP_HIGHER, 0) * 100 + Script_StepUtils(GET_STEP_LOWER, 0);
  local_E8 = arg0;
  if ((local_E8 == 0) | (local_E8 == 1)) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_SPEND_BUDGET_BONUS_ACTIVE)) {
      local_F4 = 320;
    }
  } else {
    if (local_E8 == 2) {
      local_F4 = 160;
      goto L00B423;
    }
  }
  L00B423:;
  local_F8 = local_F8 + local_F4;
  if ((Script_StepUtils(GET_STEP_HIGHER, 0) == 17) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_D000_VENDING_LOCK_CLEARED) == 0)) {
    local_F8 = local_F8 + 69000;
    (void)((local_F3 = 0) & 1);
  } else if ((local_EC >= 2202) & (local_EC <= 2204)) {
    (void)((local_F3 = 0) & 1);
    local_F8 = local_F4 = 0;
    local_FF = 4;
  } else {
    (void)((local_F3 = -(script_B382(local_F8, arg1) != 0)) & 0x10);
  }
  if (~local_F3) {
    if (local_F4 & -(local_F8 == local_F4)) {
      local_FF = 2;
    } else if (local_F8 > local_F4) {
      local_FF = 3;
    } else {
      if (local_FF == 0) {
        local_FF = 1;
      }
    }
    if (arg2) {
      script_B4F7(local_FF, arg0);
    }
  }
  return local_FF;
}

__attribute__((scn_string_bindings(0, data_3D5F4, 8, data_3D638, 9, data_3D641, 10, data_3D64A, 11, data_3D653, 12, data_3D65C, 13, data_3D665)))
static void script_B4F7(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  if (arg0 == 1) {
  } else {
    Script_LoadVoiceFile(1, "SY11");
    local_F4 = arg0;
    switch (local_F4) {
      case 2:
        if (arg1 == 2) {
          local_F8 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 3);
          local_F4 = local_F8;
          switch (local_F4) {
            case 0:
              local_FC = Script_ShowUIMessageFromID("SY11A006");
              break;
            case 1:
              local_FC = Script_ShowUIMessageFromID("SY11A007");
              break;
            case 2:
              local_FC = Script_ShowUIMessageFromID("SY11A008");
              break;
          }
        } else {
          local_F8 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 4);
          local_F4 = local_F8;
          switch (local_F4) {
            case 0:
              local_FC = Script_ShowUIMessageFromID("SY11A002");
              break;
            case 1:
              local_FC = Script_ShowUIMessageFromID("SY11A003");
              break;
            case 2:
              local_FC = Script_ShowUIMessageFromID("SY11A004");
              break;
            case 3:
              local_FC = Script_ShowUIMessageFromID("SY11A005");
              break;
          }
        }
        break;
      case 3:
        local_FC = Script_ShowUIMessageFromID("SY11A001");
        break;
      case 4:
        local_F4 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
        if (local_F4 == 1) {
          local_FC = Script_ShowUIMessageFromID("SY11A009");
        } else if (local_F4 == 2) {
          local_FC = Script_ShowUIMessageFromID("SY11A010");
        } else {
          local_FC = Script_ShowUIMessageFromID("SY11A011");
        }
        break;
      case 5:
        local_FC = Script_ShowUIMessageFromID("SY11A009");
        break;
    }
    while (Script_convGetIsActive((const void *)local_FC)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_LoadVoiceFile(0, "SY11");
  }
  return;
}

__attribute__((scn_string_bindings(0, data_3D66A, 1, data_3D66F, 2, data_3D678, 3, data_3D67D, 4, data_3D682, 5, data_3D68B, 6, data_3D694, 7, data_3D69D)))
static void script_B6C0(int32_t arg0, int32_t arg1) {
  int8_t local_FF;
  int8_t local_FE;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  local_FE = 1;
  local_FF = 0;
  local_F4 = Script_StepUtils(GET_STEP_HIGHER, 0) * 100 + Script_StepUtils(GET_STEP_LOWER, 0);
  if ((Script_StepUtils(GET_STEP_HIGHER, 0) == 17) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_D000_VENDING_LOCK_CLEARED) == 0)) {
    local_FE = 0;
    if (arg1) {
      if (Script_ryoControl(PCCTL_QueryShouldRun)) {
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        local_FF = 1;
      }
      Script_LoadVoiceFile(1, "SY11");
      local_F8 = Script_ShowUIMessageFromID("SY11A001");
      while (Script_convGetIsActive((const void *)local_F8)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_LoadVoiceFile(0, "SY11");
    }
  }
  if ((local_F4 >= 2202) & (local_F4 <= 2204)) {
    local_FE = 0;
    if (arg1) {
      if (Script_ryoControl(PCCTL_QueryShouldRun)) {
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        local_FF = 1;
      }
      Script_LoadVoiceFile(1, "SY11");
      local_F0 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
      if (local_F0 == 1) {
        local_F8 = Script_ShowUIMessageFromID("SY11A009");
      } else if (local_F0 == 2) {
        local_F8 = Script_ShowUIMessageFromID("SY11A010");
      } else {
        local_F8 = Script_ShowUIMessageFromID("SY11A011");
      }
      while (Script_convGetIsActive((const void *)local_F8)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_LoadVoiceFile(0, "SY11");
    }
  }
  if (arg1) {
    if (local_FF == 1) {
      Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_ryoControl(RYO_RestoreControl);
    }
  }
  (void)local_FE;
  return;
}

static int32_t script_B896(int32_t arg0, const char *arg1) {
  task_232 = 0;
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) != 0) {
      break;
    }
    if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg1) == 0) {
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/stream/", arg1) == 0) {
        SCNF_WaitFrames_0000(1);
      }
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
        SCNF_WaitFrames_0000(1);
      }
    }
  }
  task_232 = 1;
}

static void script_B90B(const char *arg0) {
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg0) != 0) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, arg0);
  }
  return;
}

static int32_t script_B92C(int32_t arg0, const char *arg1, int32_t arg2) {
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, arg0, arg1, arg2);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_LoadSND(4, 0, 0, 0);
}

static int32_t script_B971(int32_t arg0, int32_t arg1) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  if (task_230 == 0) {
    Script_executeAicaCommand(arg0, 0, 0);
  }
  return Script_srchControl(0);
}

static void script_B99F(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_B92C, 3, arg0, arg1, arg2);
  return;
}

static void script_B9B3(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_B971, 2, arg0, arg1);
  return;
}

static int32_t script_B9C5(const char *arg0, int32_t arg1) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  if (task_230 == 0) {
    Script_StartOrFreeSEQ(0);
    Script_ShowUIMessageFromID(arg0);
  }
  return Script_srchControl(0);
}

static void script_B9F6(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_B9C5, 2, arg0, arg1);
  return;
}

static int32_t script_BA08(int32_t arg0, int32_t arg1, int32_t arg2, const Vector3 *arg3, const Vector3i *arg4, int32_t arg5) {
  if (arg5 >= 0) {
    SCNF_WaitFrames_0000(arg5);
  }
  if (task_230 == 0) {
    Script_fixoAttachEx(arg0, arg1, arg2, arg3, arg4);
  }
  return Script_srchControl(0);
}

static void script_BA3B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_BA08, 6, arg0, arg1, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_BA55(int32_t arg0, int32_t arg1) {
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_fixoClearAttachment(arg0);
  Script_charaDisplay(arg0, CDM_Hide);
  return Script_srchControl(0);
}

static void script_BA7D(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_BA55, 2, arg0, arg1);
  return;
}

static void script_BA8F(int32_t arg0, int32_t arg1) {
  Vector3i local_F4;
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  if (arg1) {
    Script_mtwkGetSetRotation(arg0, &local_F4, 0x2000000);
    Script_EVT_SetRotation(arg0, 0x78000000, &local_F4, 0);
  }
  return;
}

static int32_t script_BACD(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5) {
  if (arg5 >= 0) {
    SCNF_WaitFrames_0000(arg5);
  }
  if (task_230 == 0) {
    if (arg4 & -0x80000000) {
      Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
    } else {
      Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
    }
    if (!(arg3 >= 0.1f)) {
      arg3 = -1.0f;
    }
    Script_PlayCharaMotion(arg0, arg1, arg2, ((scn3_bits){.f = arg3}).i, arg2, 1.0f, 0);
  }
  return Script_srchControl(0);
}

static int32_t script_BB3E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  if (task_230 == 0) {
    if (arg3 & -0x80000000) {
      Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
    } else {
      Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
    }
    Script_PlayCharaMotion(arg0, arg1, arg2, -1.0f, -1.0f, 1.0f, 0);
  }
  return Script_srchControl(0);
}

static void script_BBA0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_BB3E, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static void script_BBB8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_BACD, 6, arg0, arg1, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_BBD2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (task_230 == 0) {
    Script_SetNewHandMotion(arg0, arg1, arg2, 0, 0, 0, 0, 1.0f, 0);
  }
  return Script_srchControl(0);
}

static void script_BC10(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_BBD2, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_BC26(int32_t arg0, int32_t arg1, const Vector3i *arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (task_230 == 0) {
    Script_SetHandRootRotation(arg0, arg1, 0x15, arg2);
  }
  return Script_srchControl(0);
}

static void script_BC57(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_BC26, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_BC6D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  if (task_230 == 0) {
    Script_FACE(arg0, arg1, arg2, arg3);
  }
  return Script_srchControl(0);
}

static void script_BC9E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_BC6D, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_BCB6(int32_t arg0, int32_t arg1, const Vector3 *arg2, int32_t arg3) {
  int32_t local_FC;
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (task_230 == 0) {
    local_FC = Script_CEWPRelated(arg0);
    Script_MoveObjectToObject(arg0, arg1);
    Script_EVT_SetPosition(arg0, POS_XYZ, arg2);
    Script_MoveObjectToObject(arg0, local_FC);
  }
  return Script_srchControl(0);
}

static void script_BD03(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_BCB6, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_BD19(int32_t arg0, int32_t arg1, const Vector3i *arg2, int32_t arg3) {
  int32_t local_FC;
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (task_230 == 0) {
    local_FC = Script_CEWPRelated(arg0);
    Script_MoveObjectToObject(arg0, arg1);
    Script_EVT_SetRotation(arg0, 0x38000000, arg2, 0);
    Script_MoveObjectToObject(arg0, local_FC);
  }
  return Script_srchControl(0);
}

static void script_BD68(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_BD19, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_BD7E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (task_230 == 0) {
    Script_seqEnterEvtCam(arg0, arg1, arg2);
  }
  return Script_srchControl(0);
}

static void script_BDAC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_BD7E, 4, arg0, arg1, arg2, arg3);
  return;
}

static void script_BDC2(int32_t arg0) {
  Vector4 local_F0;
  local_F0.x = 0.0f;
  (&local_F0)->y = 0.2f;
  (&local_F0)->z = 0.2f;
  (&local_F0)->w = 0.2f;
  Script_SetCharaRefBoardPos(arg0, &local_F0);
  Script_charaSetRefboardFlagTo(arg0, 1);
  return;
}

static void script_BE01(void) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    Script_PrintDebugString(2, 4, "%03d", local_FC);
    SCNF_WaitFrames_0000(1);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_BE2D(int32_t arg0, int32_t arg1, int32_t arg2) {
  int8_t local_FF;
  int32_t local_F8;
  local_FF = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 100.0f);
  if (~(arg2 & 2)) {
    if (local_FF > 74) {
      task_224 = 5;
      task_220 = (char *)data_3D6B9;
      arg1 = (int32_t)task_220;
    }
  }
  local_F8 = task_224;
  switch (local_F8) {
    case 0:
      Script_PlayerStatsUtils(PS_SetIndexedU8Table0Slot, 0, Script_PlayerStatsUtils(PS_GetIndexedU8Table0Slot, 0) + 1);
      break;
    case 1:
      Script_PlayerStatsUtils(PS_SetIndexedU8Table0Slot, 1, Script_PlayerStatsUtils(PS_GetIndexedU8Table0Slot, 1) + 1);
      break;
    case 2:
      Script_PlayerStatsUtils(PS_SetIndexedU8Table0Slot, 2, Script_PlayerStatsUtils(PS_GetIndexedU8Table0Slot, 2) + 1);
      break;
    case 3:
      Script_PlayerStatsUtils(PS_SetIndexedU8Table0Slot, 3, Script_PlayerStatsUtils(PS_GetIndexedU8Table0Slot, 3) + 1);
      break;
    case 4:
      Script_PlayerStatsUtils(PS_SetIndexedU8Table0Slot, 4, Script_PlayerStatsUtils(PS_GetIndexedU8Table0Slot, 4) + 1);
      break;
    case 5:
      Script_PlayerStatsUtils(PS_SetIndexedU8Table0Slot, 5, Script_PlayerStatsUtils(PS_GetIndexedU8Table0Slot, 5) + 1);
      break;
  }
  script_B896(arg0, (const char *)arg1);
  script_BDC2(arg0);
  return;
}

static int32_t script_BF53(int32_t arg0, int32_t arg1, int32_t arg2) {
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  if (arg1 == 0) {
    Script_charaPosRotUtils(arg0, 152, &data_3D6C0, 0, CHARA_PART_Set);
    Script_charaPosRotUtils(arg0, 153, &data_3D6CC, 0, CHARA_PART_Set);
  } else {
    Script_charaPosRotUtils(arg0, 152, &data_3D6CC, 0, CHARA_PART_Set);
    Script_charaPosRotUtils(arg0, 153, &data_3D6C0, 0, CHARA_PART_Set);
  }
  return Script_srchControl(0);
}

static void script_BFC5(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_BF53, 3, arg0, arg1, arg2);
  return;
}

static int32_t script_BFD9(int32_t arg0) {
  SCNF_WaitFrames_0000(73);
  SCNF_SetCashValue(-100, 1);
  arg0 = arg0 - 100;
  Script_SetStats(STATS_CASH, arg0);
  end:;
}

static int32_t script_BFFF(int32_t arg0) {
  SCNF_SetCashValue(100, 1);
  arg0 = arg0 + 100;
  Script_SetStats(STATS_CASH, arg0);
  end:;
}

static int32_t script_C01D(int32_t arg0, int32_t arg1) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  SCNF_StartIconSystemMenu(arg0);
  Script_GetCassetteFlags(arg0, 1, 0, 0);
  return Script_srchControl(0);
}

static void script_C04B(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_C01D, 2, arg0, arg1);
  return;
}

__attribute__((scn_string_bindings(0, data_3D6D8)))
static void script_C05D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int8_t local_FF;
  uint8_t unused_FC[3];
  int32_t local_F8;
  int32_t local_F4;
  int32_t unused_F0;
  GameDateTime local_E8;
  int32_t local_E4;
  local_FF = 0;
  local_F8 = Script_shenGetStats(STATS_CASH);
  Script_ReturnTimeDate(&local_E8);
  if (Script_scenUtils(SCENARIO_CMD_QUERY_FOCUS_OR_TIME_LIMIT) == 0) {
    if (Script_IsPlayerAllowed(0)) {
      if (local_F8 > 99) {
        if ((&local_E8)->hours < 19) {
          script_10529();
        }
        script_C474(arg0);
        Script_scenCreateTask(script_BFD9, 1, local_F8);
        Script_scenCreateTask(script_B896, 2, arg2, "VEND");
        script_CA3D(arg0, arg1);
        script_C54F(arg0, arg1, arg2, arg3);
        local_F4 = script_C6F9(arg0, arg1, arg2, arg3, 1);
        local_E4 = local_F4;
        switch (local_E4) {
          case 13:
            if (task_224 == 5) {
              script_CB0B(arg0, arg1, 1);
            } else {
              script_CDFC(arg0, arg1, 1);
            }
            break;
          case 16:
            local_F8 = Script_shenGetStats(STATS_CASH);
            Script_scenCreateTask(script_BFFF, 1, local_F8);
            break;
        }
        script_C5F0(arg0, arg1, arg2, arg3);
        if ((&local_E8)->hours < 19) {
          script_104AA();
        }
      } else {
        if (Script_ryoControl(PCCTL_QueryShouldRun)) {
          local_FF = 1;
          Script_ryoControl(PCCTL_DisablePlayerControl);
        }
        script_B4F7(2, 0);
        if (local_FF == 1) {
          Script_ryoControl(RYO_RestoreControl);
        }
      }
    }
  }
  return;
}

static void script_C1B2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t unused_FC;
  GameDateTime local_F4;
  Script_ReturnTimeDate(&local_F4);
  if ((&local_F4)->hours < 19) {
    script_10529();
  }
  script_C317(arg0, arg1, arg2, arg3);
  script_C6F9(arg0, arg1, arg2, arg3, 0);
  script_C5F0(arg0, arg1, arg2, arg3);
  if ((&local_F4)->hours < 19) {
    script_104AA();
  }
  return;
}

__attribute__((scn_string_bindings(0, data_3D6DD)))
static void script_C20E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_FC = Script_shenGetStats(STATS_CASH);
  if (Script_scenUtils(SCENARIO_CMD_GET_SCEN_FLAG100) == 0) {
    if (local_FC > 99) {
      script_C474(arg0);
      Script_scenCreateTask(script_BFD9, 1, local_FC);
      Script_scenCreateTask(script_B896, 2, arg2, "VEND");
      script_CA3D(arg0, arg1);
      script_C54F(arg0, arg1, arg2, arg3);
      local_F8 = script_C6F9(arg0, arg1, arg2, arg3, 1);
      local_F4 = local_F8;
      switch (local_F4) {
        case 13:
          script_CDFC(arg0, arg1, 1);
          break;
        case 16:
          local_FC = Script_shenGetStats(STATS_CASH);
          Script_scenCreateTask(script_BFFF, 1, local_FC);
          break;
      }
      script_C5F0(arg0, arg1, arg2, arg3);
    }
  }
  return;
}

static void script_C2DD(void) {
  task_21C = 0x314E4143;
  task_224 = 3;
  task_220 = (char *)data_3D6E2;
  task_230 = 0;
  task_231 = 0;
  task_232 = 0;
  task_238 = 0;
  return;
}

__attribute__((scn_string_bindings(0, data_3D6E7, 1, data_3D6EE, 2, data_3D6FB)))
static void script_C317(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3i local_F4;
  Vector3 local_E8;
  script_C2DD();
  Script_ryoControl(PCCTL_UpdateRyoNavYaw, arg0);
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_uiUtils(0, 25, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 2);
  task_228 = Script_ryoControl(PCCTL_QueryShouldRun);
  if (task_228) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
    Script_ccowUtils(arg0, CCOW_DisableAll);
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
    Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND);
  }
  Script_LoadVoiceFile(1, "01JUCE");
  task_238 = Script_LoadHandMot(2, SHEN_NAME('VEND'), "HMOT_CAN.BIN");
  Script_scenCreateTask(script_B896, 2, arg2, "VEND");
  while (task_232 != 1) {
    SCNF_WaitFrames_0000(1);
  }
  Script_facemotUtils(arg3, 1, 0);
  Script_ResetHandMotion(arg3, 1, 1, 0);
  Script_charaDisplay(arg1, CDM_Hide);
  Script_ccowUtils(arg1, CCOW_DisableAll);
  Script_EVT_GetPosition(arg1, -1, &local_E8, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg1, &local_F4, 0x40000000);
  Script_EVT_SetPosition(arg2, 0x78000000, &local_E8);
  Script_EVT_SetRotation(arg2, 0x78000000, &local_F4, 0);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  script_BDC2(arg3);
  Script_InitMOTMGlobalState(arg0, arg2, arg3);
  return;
}

__attribute__((scn_string_bindings(0, data_3D700, 2, data_3D71D)))
static void script_C474(int32_t arg0) {
  script_C2DD();
  Script_SetCPAMFlags(arg0, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 2);
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_uiUtils(0, 25, 0);
  task_228 = Script_ryoControl(PCCTL_QueryShouldRun);
  if (task_228) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SCEN_FLAG100) == 0) {
    Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  }
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND);
  Script_LoadVoiceFile(1, "01JUCE");
  Script_sprintf("HandMot read by VEND\012");
  task_238 = Script_LoadHandMot(2, SHEN_NAME('VEND'), "HMOT_CAN.BIN");
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  return;
}

static void script_C54F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  Vector3i local_E8;
  while (task_232 != 1) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_facemotUtils(arg3, 1, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_ResetHandMotion(arg3, 1, 1, 0);
  script_BDC2(arg3);
  Script_charaDisplay(arg1, CDM_Hide);
  Script_ccowUtils(arg1, CCOW_DisableAll);
  Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg1, &local_E8, 0x40000000);
  Script_EVT_SetPosition(arg2, 0x78000000, &local_F4);
  Script_EVT_SetRotation(arg2, 0x78000000, &local_E8, 0);
  Script_InitMOTMGlobalState(arg0, arg2, arg3);
  return;
}

__attribute__((scn_string_bindings(0, data_3D72A, 1, data_3D731)))
static void script_C5F0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3i local_F4;
  Script_charaSetRefboardFlagTo(arg3, 0);
  Script_ResetHandMotion(arg3, 0, 1, 0);
  Script_faceUpdateFor(arg3, 1, 0);
  SCNF_WaitFrames_0000(1);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  if (task_238 != 0) {
    Script_FreeTaskData(task_238);
  }
  Script_LoadVoiceFile(0, "01JUCE");
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_SetGameStateTo(AUTH_None);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_mtwkGetSetRotation(arg0, &local_F4, 0x2000000);
  Script_EVT_SetRotation(arg0, 0x78000000, &local_F4, 0);
  if (task_228) {
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_ryoControl(RYO_RestoreControl);
  }
  SCNF_WaitFrames_0000(1);
  script_B90B("VEND");
  Script_FaceEx(arg0, FACE_ExpressionDisabled, 0, 2);
  Script_charaSetRefboardFlagTo(arg0, 2);
  Script_SetCPAMFlags(arg0, 1);
  task_231 = 14;
  Script_shenClearFreeState(1);
  return;
}

static int32_t script_C6F9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  int32_t unused_E8[6];
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  int32_t local_D8;
  int8_t local_D7;
  uint8_t unused_D4[3];
  int32_t local_D0;
  local_DC = 0;
  local_D8 = 0;
  local_E4 = 1;
  local_E0 = -1;
  local_D7 = 0;
  Script_sicnUtils(0, 37, 1);
  local_D8 = Script_shenGetStats(STATS_CASH);
  local_D8 = local_D8 + 100;
  Script_SetStats(STATS_CASH, local_D8);
  local_D7 = script_B3B5(2, 100, 0);
  local_D8 = Script_shenGetStats(STATS_CASH);
  local_D8 = local_D8 - 100;
  Script_SetStats(STATS_CASH, local_D8);
  if (local_D7 > 0) {
    Script_shenClearFreeState(0);
  }
  for (;;) {
    if (local_DC > 1) {
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE) == 1) {
        if (local_E4 < 3) {
          Script_sprintf("vendmachine exit is EV_STOP\012");
          Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE, 0);
          local_E4 = 16;
        }
      } else {
        if (local_E0 >= 1) {
          if (local_D7 > 0) {
            script_B3B5(2, 100, 1);
            Script_sprintf("vendmachine exit is MONEY_LOCK -> %d,%d\012", local_D7, local_DC);
            local_E4 = 16;
          }
        }
      }
    }
    if (local_E0 != local_E4) {
      local_E0 = local_E4;
      task_231 = local_E4;
      local_D0 = local_E4;
      if (local_D0 == 1) {
        script_BC57(arg3, 1, (int32_t)data_3D738, -1);
        script_BC10(arg3, 1, 542, -1);
      } else if (local_D0 == 3) {
        task_231 = 1;
        Script_shenClearFreeState(0);
        Script_sicnUtils(1, 0, 0);
      } else if (local_D0 == 16) {
        Script_sicnUtils(1, 0, 0);
      } else if ((local_D0 == 2) | (local_D0 == 4)) {
        task_231 = 1;
      } else {
        switch (local_D0) {
          case 5:
            script_B9B3(0x5D05A9, -1);
            break;
          case 6:
            script_B9B3(0x5F05A9, 5);
            break;
          case 8:
            task_220 = (char *)data_3D78A;
            task_224 = 0;
            if (arg4 & 1) {
              script_BE2D(task_21C, (int32_t)task_220, arg4);
            }
            break;
          case 9:
            task_220 = (char *)data_3D78F;
            task_224 = 1;
            if (arg4 & 1) {
              script_BE2D(task_21C, (int32_t)task_220, arg4);
            }
            break;
          case 10:
            task_220 = (char *)data_3D794;
            task_224 = 2;
            if (arg4 & 1) {
              script_BE2D(task_21C, (int32_t)task_220, arg4);
            }
            break;
          case 11:
            task_220 = (char *)data_3D799;
            task_224 = 3;
            if (arg4 & 1) {
              script_BE2D(task_21C, (int32_t)task_220, arg4);
            }
            break;
          case 12:
            task_220 = (char *)data_3D79E;
            task_224 = 4;
            if (arg4 & 1) {
              script_BE2D(task_21C, (int32_t)task_220, arg4);
            }
            break;
        }
      }
    }
    local_E4 = Script_UpdateCharaSelectionScene(arg0, arg2, arg3, CHARA_SELECT_IDLE);
    if ((local_E0 == 13) | (local_E0 == 14) | (local_E0 == 15) | (local_E0 == 16)) {
      break;
    }
    local_DC = local_DC + 1;
    if (local_D7 < 1) {
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_charaDisplay(arg1, CDM_Show);
  Script_charaDisplay(arg2, CDM_Hide);
  Script_ccowUtils(arg1, CCOW_EnableAll);
  Script_charaDisplay(arg3, CDM_Hide);
  return local_E0;
}

static void script_CA3D(int32_t arg0, int32_t arg1) {
  Script_shenClearFreeState(0);
  script_BD03(arg0, arg1, (int32_t)&data_3D7A4, -1);
  script_BD68(arg0, arg1, (int32_t)data_3D7B0, -1);
  script_BC57(arg0, 1, (int32_t)data_3D7BC, -1);
  script_BC57(arg0, 0, (int32_t)data_3D7BC, -1);
  script_BC10(arg0, 1, 541, -1);
  script_BBA0(arg0, 712, 0, -0x80000000, -1);
  script_B9B3(0x5E05A9, 125);
  script_BDAC(4013, arg1, arg1, -1);
  SCNF_WaitFrames_0000(10);
  script_BA8F(arg0, 0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_shenClearFreeState(1);
  return;
}

__attribute__((scn_string_bindings(3, data_3D825, 4, data_3D830)))
static void script_CB0B(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (arg2 == 0) {
    script_B896(task_21C, task_220);
  }
  script_BA3B(task_21C, arg0, 18, (int32_t)&data_3D7C8, (int32_t)&data_3D7D4, 82);
  script_BFC5(task_21C, 1, 0);
  script_BD03(arg0, arg1, (int32_t)&data_3D7E0, -1);
  script_BD68(arg0, arg1, (int32_t)data_3D7EC, -1);
  script_BC57(arg0, 1, (int32_t)&data_3D7F8, -1);
  script_BC10(arg0, 1, 545, -1);
  script_BC10(arg0, 0, 546, -1);
  script_BBA0(arg0, 45059, 0, -0x80000000, -1);
  script_B9B3(0x6405A9, 80);
  script_BDAC(4019, arg1, arg1, -1);
  SCNF_WaitFrames_0000(10);
  script_BA8F(arg0, 0);
  local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
  if (local_FC == 0) {
    script_B9F6((int32_t)"01JUCEA011", -1);
  } else if (local_FC == 1) {
    script_B9F6((int32_t)"01JUCEA012", -1);
  } else {
    script_B9F6((int32_t)"01JUCEA013", -1);
  }
  SCNF_WaitFrames_0000(90);
  script_BC9E(arg0, 10, 5, 16, 180);
  script_BC9E(arg0, 9, 5, 16, 251);
  script_BC9E(arg0, 4, 5, 16, 351);
  script_BC9E(arg0, 1, 4, 16, 391);
  script_BC9E(arg0, 9, 5, 16, 426);
  script_BC9E(arg0, 10, 1, 16, 521);
  script_BC9E(arg0, 1, 2, 16, 545);
  script_BC9E(arg0, 0, 0, 16, 596);
  script_B9B3(0x6005A9, 108);
  script_BFC5(task_21C, 0, 108);
  script_BBB8(arg0, 45058, 0, 0x441C0000, 0, -1);
  script_B9B3(0x3E05A9, 210);
  script_B9B3(0x3F05A9, 330);
  script_B9B3(0x3E05A9, 470);
  script_B9B3(0x3F05A9, 500);
  script_B9F6((int32_t)"01JUCEA002", 550);
  script_B9F6((int32_t)"01JUCEA003", 400);
  script_BDAC(4015, arg1, arg1, -1);
  script_BDAC(4016, arg1, arg1, 575);
  SCNF_WaitFrames_0000(10);
  script_BA8F(arg0, 1);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_16);
  script_BBA0(arg0, 45061, 0x42100000, 0, -1);
  script_BA7D(task_21C, 30);
  script_C04B(66, 32);
  SCNF_WaitFrames_0000(10);
  script_BA8F(arg0, 0);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
  script_B90B(task_220);
  Script_shenClearFreeState(1);
  return;
}

__attribute__((scn_string_bindings(0, data_3D89C, 1, data_3D8A7)))
static void script_CDFC(int32_t arg0, int32_t arg1, int32_t arg2) {
  if (arg2 == 0) {
    script_B896(task_21C, task_220);
  }
  if (task_224 == 4) {
    script_BA3B(task_21C, arg0, 18, (int32_t)&data_3D854, (int32_t)&data_3D860, 82);
    script_BD03(arg0, arg1, (int32_t)&data_3D86C, -1);
    script_BD68(arg0, arg1, (int32_t)data_3D878, -1);
    script_BC57(arg0, 1, (int32_t)data_3D890, -1);
    script_BC10(arg0, 1, 544, -1);
  } else {
    script_BA3B(task_21C, arg0, 18, (int32_t)&data_3D83C, (int32_t)&data_3D848, 82);
    script_BD03(arg0, arg1, (int32_t)&data_3D86C, -1);
    script_BD68(arg0, arg1, (int32_t)data_3D878, -1);
    script_BC57(arg0, 1, (int32_t)&data_3D884, -1);
    script_BC10(arg0, 1, 545, -1);
  }
  script_BFC5(task_21C, 1, 0);
  script_BC10(arg0, 0, 546, -1);
  script_BBA0(arg0, 45059, 0, -0x80000000, -1);
  script_B9B3(0x6405A9, 80);
  script_B9B3(0x6005A9, 290);
  script_BDAC(4014, arg1, arg1, -1);
  script_BFC5(task_21C, 0, 290);
  SCNF_WaitFrames_0000(10);
  script_BA8F(arg0, 0);
  script_BC9E(arg0, 10, 5, 16, 180);
  script_BC9E(arg0, 9, 5, 16, 251);
  script_BC9E(arg0, 4, 5, 16, 351);
  script_BC9E(arg0, 1, 4, 16, 391);
  script_BC9E(arg0, 9, 5, 16, 426);
  script_BC9E(arg0, 10, 1, 16, 521);
  script_BC9E(arg0, 1, 2, 16, 545);
  script_BC9E(arg0, 0, 0, 16, 596);
  if (task_224 == 4) {
    script_BBA0(arg0, 45060, 0, 0, -1);
  } else {
    script_BBA0(arg0, 45058, 0, 0, -1);
  }
  script_B9B3(0x3E05A9, 210);
  script_B9B3(0x3F05A9, 330);
  script_B9B3(0x3E05A9, 470);
  script_B9B3(0x3F05A9, 500);
  script_B9B3(0x6305A9, 690);
  script_B9F6((int32_t)"01JUCEA002", 550);
  script_B9F6((int32_t)"01JUCEA003", 400);
  script_BDAC(4015, arg1, arg1, -1);
  script_BDAC(4016, arg1, arg1, 575);
  script_BA7D(task_21C, 690);
  SCNF_WaitFrames_0000(10);
  script_BA8F(arg0, 0);
  script_B90B(task_220);
  Script_shenClearFreeState(1);
  return;
}

static int32_t script_D0E6(int32_t arg0) {
  int8_t local_FF;
  int8_t local_FE;
  int8_t local_FD;
  int8_t local_FC;
  int8_t local_FB;
  int8_t local_FA;
  int8_t local_F9;
  int8_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  float local_90[8][3];
  Vector3 local_84;
  float local_FF24[8][3];
  int32_t local_FEC4[8][3];
  Vector3i local_FEB8;
  (void)((local_FF = 0) & 1);
  (void)((local_FE = 0) & 1);
  (void)((local_FD = 0) & 1);
  (void)((local_FC = 0) & 1);
  (void)((local_FB = 0) & 1);
  (void)((local_FA = 0) & 1);
  local_F9 = 0;
  local_F8 = 0;
  local_F4 = 0;
  while (local_F4 < 8) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, *(&data_3D5D4 + local_F4))) {
      Script_EVT_GetPosition(*(&data_3D5D4 + local_F4), -1, (Vector3 *)local_90[local_F4], ENTITY_XFORM_WORLD_SPACE);
      Script_mtwkGetSetRotation(*(&data_3D5D4 + local_F4), (Vector3i *)local_FEC4[local_F4], 0x40000000);
      local_FF24[local_F4][0] = local_90[local_F4][0] + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FEC4[local_F4][1]) * 0.3f;
      local_FF24[local_F4][1] = local_90[local_F4][1];
      local_FF24[local_F4][2] = local_90[local_F4][2] + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FEC4[local_F4][1]) * 0.3f;
    }
    local_F4 = local_F4 + 1;
  }
  for (;;) {
    local_F9 = -1;
    (void)((local_FB = 0) & 2);
    Script_EVT_GetPosition(arg0, -1, &local_84, ENTITY_XFORM_WORLD_SPACE);
    Script_mtwkGetSetRotation(arg0, &local_FEB8, 0x42000000);
    local_84.x = (&local_84)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_FEB8)->y) * 0.3f;
    (&local_84)->z = (&local_84)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_FEB8)->y) * 0.3f;
    local_F0 = 0;
    while (local_F0 < 8) {
      if (Script_CheckTargetWithinAngleRange((const Vector3 *)local_FF24[local_F0], 1.4f, local_FEC4[local_F0][1] + 0x8000, 12743, &local_84) != 0) {
        if (Script_CheckTargetWithinAngleRange(&local_84, 1.4f, (&local_FEB8)->y, 5461, (const Vector3 *)local_FF24[local_F0]) != 0) {
          local_F9 = local_F0;
          break;
        }
      }
      local_F0 = local_F0 + 1;
    }
    if (Script_shenGetStats(STATS_CASH) < 100) {
      local_F8 = 1;
    } else {
      local_F8 = Script_ryoControl(PCCTL_GetTatehiza);
    }
    if (local_FA == -1) {
      if (Script_scenUtils(SCENARIO_CMD_GET_SCEN_FLAG100) == 0) {
        (void)((local_FA = 0) & 0x80);
      }
    }
    if ((local_F9 != -1) & (local_FA == 0)) {
      if (Script_GetControllerInput(1) & 0x400) {
        if (local_FB == 0) {
          (void)((local_FB = -1) & 0x80);
        }
        if (Script_scenUtils(SCENARIO_CMD_GET_SCEN_FLAG100)) {
          (void)((local_FA = -1) & 0x80);
        }
      } else {
        (void)((local_FB = 0) & 0x80);
      }
      if (local_FA != -1) {
        if (local_FD == 0) {
          Script_uiUtils(0, 25, 0);
          (void)((local_FD = -1) & 0x80);
        }
      } else {
        if (local_FD == -1) {
          Script_uiUtils(0, 0, 0);
        }
        (void)((local_FD = 0) & 0x80);
      }
      if (local_FE == 0) {
        Script_sicnUtils(0, 1, 1);
        (void)((local_FE = -1) & 0x80);
      }
      if (local_F8 == 0) {
        if (-(local_FB == -1) & -(local_FC == 0) & -(local_FA != -1)) {
          (void)((local_FC = -1) & 0x80);
        }
        if (local_FC == 0) {
          if (local_FF == 0) {
            SCNF_RequestIconNotification(0x100, 0, 0, 1);
            (void)((local_FF = -1) & 0x80);
          }
        } else {
          if (local_FF == -1) {
            SCNF_RequestIconNotification(0, 0, 0, 2);
          }
          (void)((local_FF = 0) & 0x80);
        }
      } else {
        if (local_FF == -1) {
          SCNF_RequestIconNotification(0, 0, 0, 2);
        }
        (void)((local_FF = 0) & 0x80);
        (void)((local_FC = 0) & 0x80);
      }
    } else {
      if (local_FE == -1) {
        Script_sicnUtils(1, 0, 0);
        (void)((local_FE = 0) & 0x80);
      }
      if (local_FD == -1) {
        Script_uiUtils(0, 0, 0);
        (void)((local_FD = 0) & 0x80);
      }
      if (local_FF == -1) {
        SCNF_RequestIconNotification(0, 0, 0, 2);
        (void)((local_FF = 0) & 0x80);
      }
    }
    if ((local_FB == -1) & (local_FC == -1)) {
      script_C20E(arg0, *(&data_3D5D4 + local_F9), SHEN_NAME('JIHN'), SHEN_NAME('HAND'));
      (void)((local_FB = 0) & 0x80);
      (void)((local_FC = 0) & 0x80);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_D54A(int32_t arg0) {
  script_C2DD();
  task_22C = Script_scenCreateTask(script_D0E6, 1, arg0);
  return;
}

static void script_D565(void) {
  Script_srchControl((void *)task_22C);
  task_22C = 0;
  return;
}

static void script_D576(void) {
  (void)task_231;
  return;
}

static void script_D57D(void) {
  Script_ExitCharaSelector();
  task_230 = 1;
  return;
}

static void script_D58A(void) {
  Script_ambSoundUtils(1, 1, &data_3D8C0, 135.0f, 280.0f, 1.0f, 0, &data_3D950);
  Script_ambSoundUtils(2, 1, &data_3D8CC, 135.0f, 60.0f, 1.0f, 0, &data_3D9D8);
  Script_ambSoundUtils(3, 0, &data_3D8D8, 50.0f, 170.0f, 0.8f, 0, data_3DA60);
  Script_ambSoundUtils(4, 0, &data_3D8E4, 10.0f, 5.0f, 1.0f, 0, &data_3DAE8);
  Script_ambSoundUtils(5, 0, data_3D8F0, 8.0f, 5.0f, 1.0f, 0, &data_3DB70);
  Script_ambSoundUtils(6, 1, &data_3D8FC, 170.0f, 100.0f, 1.0f, 2, data_3DBF8);
  Script_ambSoundUtils(7, 1, &data_3D908, 170.0f, 100.0f, 1.0f, 0, &data_3DC80);
  Script_ambSoundUtils(8, 1, &data_3D914, 40.0f, 70.0f, 0.8f, 0, &data_3DD08);
  Script_ambSoundUtils(9, 0, &data_3D920, 3.0f, 60.0f, 0.8f, 0, data_3DD90);
  Script_ambSoundUtils(10, 0, &data_3D92C, 60.0f, 35.0f, 0.7f, 0, &data_3DE18);
  Script_ambSoundUtils(13, 0, &data_3D938, 8.0f, 5.0f, 1.0f, 0, &data_3DEA0);
  Script_ambSoundUtils(15, 1, data_3D944, 300.0f, 0.0f, 1.0f, 3, &data_3DF28);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_D744(void) {
  Script_ambSoundUtils(1, 1, &data_3DFB0, 135.0f, 280.0f, 1.0f, 0, &data_3E010);
  Script_ambSoundUtils(2, 1, &data_3DFBC, 135.0f, 60.0f, 1.0f, 0, &data_3E098);
  Script_ambSoundUtils(3, 0, &data_3DFC8, 50.0f, 170.0f, 0.8f, 0, data_3E120);
  Script_ambSoundUtils(6, 1, &data_3DFD4, 170.0f, 100.0f, 1.0f, 2, data_3E1A8);
  Script_ambSoundUtils(7, 1, &data_3DFE0, 170.0f, 100.0f, 1.0f, 0, &data_3E230);
  Script_ambSoundUtils(8, 1, &data_3DFEC, 40.0f, 70.0f, 0.8f, 0, &data_3E2B8);
  Script_ambSoundUtils(10, 0, &data_3DFF8, 60.0f, 35.0f, 0.7f, 0, &data_3E340);
  Script_ambSoundUtils(15, 1, data_3E004, 300.0f, 0.0f, 1.0f, 3, &data_3E3C8);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_D86E(void) {
  Script_ambSoundUtils(1, 0, &data_3E450, 1.0f, 10.0f, 1.0f, 0, &data_3E45C);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_D89F(void) {
  Script_ambSoundUtils(1, 0, &data_3E4E4, 1.0f, 10.0f, 1.0f, 0, &data_3E4F0);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_D8D0(void) {
  Script_ambSoundUtils(1, 1, &data_3E578, 135.0f, 280.0f, 1.0f, 0, &data_3E5D8);
  Script_ambSoundUtils(2, 1, &data_3E584, 135.0f, 60.0f, 1.0f, 0, &data_3E660);
  Script_ambSoundUtils(3, 0, &data_3E590, 50.0f, 170.0f, 0.8f, 0, data_3E6E8);
  Script_ambSoundUtils(6, 1, &data_3E59C, 170.0f, 100.0f, 1.0f, 2, data_3E770);
  Script_ambSoundUtils(7, 1, &data_3E5A8, 170.0f, 100.0f, 1.0f, 0, &data_3E7F8);
  Script_ambSoundUtils(8, 1, &data_3E5B4, 40.0f, 70.0f, 0.8f, 0, &data_3E880);
  Script_ambSoundUtils(15, 1, data_3E5CC, 300.0f, 0.0f, 1.0f, 3, &data_3E908);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_D9D6(void) {
  script_D58A();
  return;
}

static void script_D9DE(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3 local_D0;
  int32_t local_CC;
  int32_t local_C8;
  int32_t local_C4;
  int32_t local_C0;
  int32_t unused_BC;
  GameDateTime local_B4;
  int32_t local_B0;
  Script_ReturnTimeDate(&local_B4);
  if ((&local_B4)->hours >= 17) {
  } else {
    Script_GULLProgram_Utils(0, 36, 3);
    local_F4.x = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f) + -83.8f;
    (&local_F4)->y = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 8.0f) + 2.6f;
    (&local_F4)->z = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f) + -19.0f;
    local_CC = Script_GULLProgram_Utils(1, &local_F4, 13, 0, data_3E990, data_3E9C4, data_3E9F8);
    local_E8.x = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f) + -67.8f;
    (&local_E8)->y = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 8.0f) + 2.2f;
    (&local_E8)->z = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f) + 26.0f;
    local_C8 = Script_GULLProgram_Utils(1, &local_E8, 13, 0, data_3EA2C, data_3EA60, data_3EA94);
    local_DC.x = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f) + 61.0f;
    (&local_DC)->y = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f) + 4.5f;
    (&local_DC)->z = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f) + 9.0f;
    local_C4 = Script_GULLProgram_Utils(1, &local_DC, 19, 0, data_3EAC8, data_3EB14, data_3EB60);
    if ((local_CC == 0) | (local_C8 == 0) | (local_C4 == 0)) {
    } else {
      local_C0 = 1;
      while (local_C0 < 9) {
        local_F4.x = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f) + -83.8f;
        (&local_F4)->y = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 8.0f) + 2.6f;
        (&local_F4)->z = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f) + -19.0f;
        local_E8.x = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f) + -67.8f;
        (&local_E8)->y = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 8.0f) + 2.2f;
        (&local_E8)->z = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f) + 26.0f;
        local_DC.x = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f) + 61.0f;
        (&local_DC)->y = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f) + 4.5f;
        (&local_DC)->z = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f) + 9.0f;
        local_D0.x = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f) + -27.9f;
        (&local_D0)->y = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 8.0f) + 4.8f;
        (&local_D0)->z = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f) + 25.5f;
        local_B0 = local_C0 % 3;
        switch (local_B0) {
          case 0:
            Script_GULLProgram_Utils(1, &local_F4, 13, (void *)local_CC, 0, 0, 0);
            Script_GULLProgram_Utils(1, &local_E8, 13, (void *)local_CC, 0, 0, 0);
            Script_GULLProgram_Utils(1, &local_DC, 13, (void *)local_CC, 0, 0, 0);
            Script_GULLProgram_Utils(1, &local_D0, 13, (void *)local_CC, 0, 0, 0);
            break;
          case 1:
            Script_GULLProgram_Utils(1, &local_F4, 13, (void *)local_C8, 0, 0, 0);
            Script_GULLProgram_Utils(1, &local_E8, 13, (void *)local_C8, 0, 0, 0);
            Script_GULLProgram_Utils(1, &local_DC, 13, (void *)local_C8, 0, 0, 0);
            Script_GULLProgram_Utils(1, &local_D0, 13, (void *)local_C8, 0, 0, 0);
            break;
          case 2:
            Script_GULLProgram_Utils(1, &local_F4, 19, (void *)local_C4, 0, 0, 0);
            Script_GULLProgram_Utils(1, &local_E8, 19, (void *)local_C4, 0, 0, 0);
            Script_GULLProgram_Utils(1, &local_DC, 19, (void *)local_C4, 0, 0, 0);
            Script_GULLProgram_Utils(1, &local_D0, 19, (void *)local_C4, 0, 0, 0);
            break;
        }
        local_C0 = local_C0 + 1;
      }
      local_F4.x = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f) + -83.8f;
      (&local_F4)->y = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 8.0f) + 2.6f;
      (&local_F4)->z = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f) + -19.0f;
      Script_GULLProgram_Utils(1, &local_F4, 19, (void *)local_C4, 0, 0, 0);
    }
  }
  return;
}

static void script_DDFC(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  (void)((task_2A1 = 0) & 4);
  (void)((task_2A2 = 0) & 4);
  task_2A0 = 3;
  Script_ReturnTimeDate(&local_F4);
  if ((&local_F4)->hours >= 17) {
  } else {
    Script_HATO_Funcs(Alloc);
    Script_scenCreateTask(script_DE42, 0);
  }
  return;
}

static int32_t script_DE42(void) {
  Vector3 local_F4;
  int32_t unused_EC[2];
  GameDateTime local_E4;
  uint8_t unused_E3;
  int8_t local_E2;
  int8_t local_E1;
  uint8_t unused_E0;
  (void)((local_E1 = 0) & 1);
  for (;;) {
    if (task_2A1 & 0x10) {
      SCNF_WaitFrames_0000(1);
      continue;
    }
    Script_ReturnTimeDate(&local_E4);
    if (~local_E1 & -((&local_E4)->hours >= 17)) {
      Script_GULLProgram_Utils(2);
      (void)((local_E1 = -1) & 0x10);
    }
    (void)((local_E2 = 0) & 0x10);
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    if (Script_CheckTargetWithinAngleRange(&data_3EBAC, 40.0f, 0, 0x8000, &local_F4)) {
      if (task_2A0 != 0) {
        task_2A0 = 0;
        (void)((local_E2 = -1) & 0x10);
      }
    } else if (Script_CheckTargetWithinAngleRange(&data_3EBB8, 40.0f, 0, 0x8000, &local_F4)) {
      if (task_2A0 != 1) {
        task_2A0 = 1;
        (void)((local_E2 = -1) & 0x10);
      }
    } else if (Script_CheckTargetWithinAngleRange(&data_3EBC4, 40.0f, 0, 0x8000, &local_F4)) {
      if (task_2A0 != 2) {
        task_2A0 = 2;
        (void)((local_E2 = -1) & 0x10);
      }
    } else {
      if (task_2A0 != 3) {
        task_2A0 = 3;
        (void)((local_E2 = -1) & 0x10);
      }
    }
    if (local_E2 | task_2A2) {
      Script_HATO_Funcs(StopTask);
      if (local_E1 & 2) {
        break;
      }
      script_DF96(task_2A0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_DF96(int32_t arg0) {
  int32_t unused_F8[2];
  int32_t local_F4;
  local_F4 = task_2A0;
  if (local_F4 == 0) {
    task_28C.x = -106.0f;
    task_28C.z = 79.0f;
    task_298 = 7.0f;
    task_29C = 20;
  } else if (local_F4 == 1) {
    task_28C.x = 0.0f;
    task_28C.z = 90.0f;
    task_298 = 5.5f;
    task_29C = 12;
  } else if (local_F4 == 2) {
    task_28C.x = 130.0f;
    task_28C.z = 24.0f;
    task_298 = 5.5f;
    task_29C = 14;
  } else {
    goto L00E05E;
  }
  task_28C.y = 0.0f;
  Script_HATO_Funcs(InitTask, &task_28C, task_298);
  Script_HATO_Funcs(StartTask, 4, task_29C);
  L00E05E:;
  return;
}

static void script_E05F(void) {
  (void)((task_2A1 = -1) & 1);
  task_2A0 = 3;
  Script_HATO_Funcs(StopTask);
  return;
}

static void script_E07A(void) {
  (void)((task_2A1 = 0) & 1);
  return;
}

static void script_E087(void) {
  int32_t unused_FC;
  Script_InvestigationUtils(ShenmueUtils_SetupOrSyncAnchors, SHEN_NAME('YKUL'), 0x52554B59);
  Script_InvestigationUtils(ShenmueUtils_SetSpecialAssetList, data_3ED90);
  Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('YKUL'), 0);
  script_E0C4();
  return;
}

static void script_E0C4(void) {
  task_2A3 = 0;
  Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_3EE04);
  Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, 0);
  return;
}

static void script_E0E3(int32_t arg0) {
  int32_t local_FC;
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  if (task_2A3 == 0) {
    task_2A3 = 1;
    Script_ryoControl(PCCTL_RequestNewMode, 1);
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &task_248, 0);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    if (task_287 & 8) {
      Script_sicnUtils(1, 0, 0);
    }
    local_FC = arg0;
    switch (local_FC) {
      case 0:
        Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, &data_3EC08);
        Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, &data_3F2C8, 60);
        break;
      case 1:
        Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, &data_3EC40);
        Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, &data_3F2D4, 60);
        break;
      case 2:
        Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, &data_3EC78);
        Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, &data_3F2E0, 60);
        break;
      case 3:
        Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, &data_3ECB0);
        Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, &data_3F2EC, 60);
        break;
      case 4:
        Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, &data_3ECE8);
        Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, &data_3F2F8, 60);
        break;
      case 5:
        Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, &data_3ED20);
        Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, &data_3F304, 60);
        break;
      case 6:
        Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, &data_3ED58);
        Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, &data_3F310, 60);
        break;
      case 7:
        Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, data_3EBD0);
        Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, &data_3F31C, 60);
        break;
      case 8:
        Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, data_3EBD0);
        Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, &data_3F31C, 60);
        break;
    }
    for (;;) {
      if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    for (;;) {
      Script_InvestigationUtils(ShenmueUtils_UpdateSpecialAssetInput);
      SCNF_WaitFrames_0000(1);
      if (Script_GetControllerInput(0) == 0x400) {
        break;
      }
      if (Script_GetControllerInput(0) == 0x200) {
        break;
      }
    }
    Script_InvestigationUtils(ShenmueUtils_RequestInteractionModeExit);
    for (;;) {
      if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    Script_srchUtils(SRCH_SCRIPT_Reset);
    Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
    task_2A3 = 0;
    Script_ryoControl(RYO_RestoreControl);
    if (task_287 & 8) {
      Script_sicnUtils(0, 4, 1);
    }
  }
  return;
}

static int32_t script_E357(int32_t arg0, int32_t arg1) {
  int32_t unused_F8[2];
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Vector3 local_E0;
  local_F4 = -1;
  local_F0 = -1;
  local_EC = 0;
  if (Script_CheckTargetWithinAngleRange(&data_3F328, 1.5f, 40049, 12743, &task_23C)) {
    local_F4 = 0;
  } else if (Script_CheckTargetWithinAngleRange(&data_3F334, 1.5f, 23665, 12743, &task_23C)) {
    local_F4 = 1;
  } else if (Script_CheckTargetWithinAngleRange(&data_3F340, 1.5f, 40777, 12743, &task_23C)) {
    local_F4 = 2;
  } else if (Script_CheckTargetWithinAngleRange(&data_3F34C, 1.5f, 0x8000, 12743, &task_23C)) {
    local_F4 = 3;
  } else if (Script_CheckTargetWithinAngleRange(&data_3F358, 1.5f, 23847, 12743, &task_23C)) {
    local_F4 = 4;
  } else if (Script_CheckTargetWithinAngleRange(&data_3F364, 1.5f, 0x4000, 12743, &task_23C)) {
    local_F4 = 5;
  } else if (Script_CheckTargetWithinAngleRange(&data_3F370, 1.5f, 6553, 12743, &task_23C)) {
    local_F4 = 6;
  } else if (Script_CheckTargetWithinAngleRange(&data_3F37C, 1.5f, 0, 12743, &task_23C)) {
    local_F4 = 7;
  } else if (task_26C.hours >= 19) {
    local_F0 = -1;
    script_E6B1();
  } else {
    if (Script_CheckTargetWithinAngleRange(&data_3F388, 1.5f, 0xC000, 12743, &task_23C)) {
      local_F0 = 1;
      script_E6A5();
    }
  }
  local_E0.x = (&task_23C)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, task_248.y) * 0.3f;
  (&local_E0)->y = 0.0f;
  (&local_E0)->z = task_23C.z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, task_248.y) * 0.3f;
  if (Script_CheckTargetWithinAngleRange(&local_E0, 1.4f, task_248.y, 7281, &data_3F328)) {
  } else if (Script_CheckTargetWithinAngleRange(&local_E0, 1.4f, task_248.y, 7281, &data_3F334)) {
  } else if (Script_CheckTargetWithinAngleRange(&local_E0, 1.4f, task_248.y, 7281, &data_3F340)) {
  } else if (Script_CheckTargetWithinAngleRange(&local_E0, 1.4f, task_248.y, 7281, &data_3F34C)) {
  } else if (Script_CheckTargetWithinAngleRange(&local_E0, 1.4f, task_248.y, 7281, &data_3F358)) {
  } else if (Script_CheckTargetWithinAngleRange(&local_E0, 1.4f, task_248.y, 7281, &data_3F364)) {
  } else if (Script_CheckTargetWithinAngleRange(&local_E0, 1.4f, task_248.y, 7281, &data_3F370)) {
  } else if (Script_CheckTargetWithinAngleRange(&local_E0, 1.4f, task_248.y, 7281, &data_3F37C)) {
  } else if (Script_CheckTargetWithinAngleRange(&local_E0, 1.4f, task_248.y, 7281, &data_3F388)) {
  } else {
    local_F4 = -1;
  }
  if ((local_F4 >= 0) | (local_F0 >= 0)) {
    if (Script_GetControllerInput(1) == 0x400) {
      Script_sicnUtils(1, 0, 0);
      if (local_F4 >= 0) {
        script_E0E3(local_F4);
      } else {
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_ryoControl(PCCTL_RequestNewMode, 1);
        SCNF_PrepareCassetteUI();
        Script_ryoControl(RYO_RestoreControl);
      }
      Script_sicnUtils(0, 4, 1);
    }
    local_EC = 4;
  }
  return local_EC;
}

static void script_E6A5(void) {
  Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, -1);
  return;
}

static void script_E6B1(void) {
  Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, 0);
  return;
}

static void script_E6BD(int32_t arg0) {
  int8_t local_FF;
  local_FF = 16;
  if (arg0 == 1) {
    while (local_FF) {
      Script_Ambient1SoundUtils(2, local_FF);
      local_FF = local_FF - 1;
    }
  } else {
    while (local_FF) {
      Script_Ambient1SoundUtils(3, local_FF);
      local_FF = local_FF - 1;
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_3F3A0, 2, data_3F3B9)))
static int32_t script_E708(void) {
  int32_t local_FC;
  local_FC = Script_MT5Loader_TRWV_TRWAVE(0, &data_3F394, "scene/02/MFSY", "Z1SEA3.PVR", "scene/02/MFSY", "Z2TRSEA4.MT6", 4.0f, 0);
  script_E743(local_FC);
  return local_FC;
}

__attribute__((scn_string_bindings(0, data_3F3D4)))
static void script_E743(int32_t arg0) {
  Script_MT5Loader_TRWV_TRWAVE(21, arg0, 8, 8);
  Script_MT5Loader_TRWV_TRWAVE(16, arg0, -1, 0);
  Script_MT5Loader_TRWV_TRWAVE(13, arg0, 0.0901960805f, 0.196078435f);
  Script_MT5Loader_TRWV_TRWAVE(15, arg0, "scene/02/MFSY", "Z3TRWTB1.BIN");
  Script_MT5Loader_TRWV_TRWAVE(12, arg0, 0x4000);
  Script_MT5Loader_TRWV_TRWAVE(10, arg0, 64.0f, 64.0f, 1, 1, 3, 1);
  Script_MT5Loader_TRWV_TRWAVE(24, arg0, 0, 203, 255, 143);
  Script_MT5Loader_TRWV_TRWAVE(11, arg0, -0.51f, -0.68f, 0.52f);
  return;
}

static int32_t script_E7D9(int32_t arg0) {
  (void)((task_364 = -1) & 8);
  (void)((task_365 = -1) & 8);
  for (;;) {
    if (~task_365) {
      SCNF_WaitFrames_0000(1);
      continue;
    }
    if (Script_SetTaskDataValue(CHARA_AKIR, &data_3F3F0, &data_3F3FC, 0) == 0) {
      if (~task_364) {
        Script_MT5Loader_TRWV_TRWAVE(1, arg0);
        (void)((task_364 = -1) & 8);
      }
    } else {
      if (task_364 & 4) {
        Script_MT5Loader_TRWV_TRWAVE(2, arg0);
        (void)((task_364 = 0) & 8);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_E866(void) {
  (void)((task_365 = -1) & 8);
  (void)((task_364 = -1) & 8);
  Script_MT5Loader_TRWV_TRWAVE(1, task_254);
  return;
}

static void script_E887(void) {
  (void)((task_365 = 0) & 8);
  (void)((task_364 = 0) & 8);
  Script_MT5Loader_TRWV_TRWAVE(2, task_254);
  return;
}

static void script_E8A8(void) {
  (void)((task_365 = 0) & 1);
  (void)((task_364 = -1) & 1);
  SCNF_WaitFrames_0000(1);
  Script_MT5Loader_TRWV_TRWAVE(5, task_254, &data_3F408);
  Script_MT5Loader_TRWV_TRWAVE(10, task_254, 64.0f, 64.0f, 1, 0, 0, 1);
  Script_MT5Loader_TRWV_TRWAVE(22, task_254, 3, 1);
  Script_MT5Loader_TRWV_TRWAVE(1, task_254);
  return;
}

static void script_E90A(void) {
  Script_MT5Loader_TRWV_TRWAVE(5, task_254, &data_3F414);
  script_E743(task_254);
  Script_MT5Loader_TRWV_TRWAVE(22, task_254, 3, 2);
  Script_MT5Loader_TRWV_TRWAVE(2, task_254);
  (void)((task_365 = -1) & 1);
  return;
}

static int32_t script_E948(int32_t arg0) {
  int32_t local_FC;
  local_FC = 0;
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  local_FC = Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT_INTERNAL_HUMAN, arg0);
  if (local_FC == 0) {
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 1);
    local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
    while (local_FC == 0) {
      Script_sprintf("MFSY: %s Read Failed\012", script_A8BC(arg0));
      SCNF_WaitFrames_0000(1);
      local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
    }
    L00E9BE:;
    SCNF_WaitFrames_0000(1);
    if (!Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_FC)) goto L00E9BE;
  }
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  if (Script_charaHasMotionwork(arg0)) {
    Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  }
  Script_SetCPAMFlags(arg0, 0);
  Script_charaSetRenderOpacity(arg0, 1.0f);
  Script_charaDisplay(arg0, CDM_Show);
  return local_FC;
}

static void script_EA10(int32_t arg0, int32_t arg1) {
  Script_FaceEx(arg0, FACE_ExpressionDisabled, 0, 2);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  SCNF_WaitFrames_0000(1);
  Script_faceUpdateFor(arg0, 0, 0);
  Script_faceUpdateFor(arg0, 1, 0);
  SCNF_WaitFrames_0000(1);
  if (arg1 != 0) {
    Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, arg1);
  }
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 0);
  arg1 = 0;
  return;
}

static int32_t script_EA82(void) {
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(1);
  Script_shenClearFreeState(1);
  end:;
}

static void script_EAA8(const char *arg0, const char *arg1) {
  Script_sprintf("MFSY: Load PackFile Start %s:%s\012", arg0, arg1);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, arg0, arg1) == 0) {
    Script_sprintf("MFSY: Load PackFile Failed %s:%s\012", arg0, arg1);
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_EAFC(const char *arg0, const char *arg1) {
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, -1);
  script_EAA8(arg0, arg1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_3F479, 1, data_3F47E, 2, data_3F483, 3, data_3F488, 4, data_3F48D)))
static void script_EB20(void) {
  Script_LoadSND(0, 6, "FREE", 0);
  Script_LoadSND(0, 5, "FREE", 0);
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 3, "FREE", 0);
  Script_LoadSND(0, 2, "FREE", 0);
  return;
}

__attribute__((scn_string_bindings(0, data_3F492, 1, data_3F499)))
static void script_EB73(void) {
  script_EB20();
  Script_LoadSND(0, 2, "SELECT", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  Script_Ambient1SoundUtils(2, 17);
  script_D58A();
  Script_LoadSND(0, 4, "f1yokosu.snd", 0);
  return;
}

static void script_EBBF(const char *arg0, const char *arg1) {
  script_EB20();
  if (arg0 != 0) {
    Script_LoadSND(0, 2, arg0, 0);
  }
  if (arg1 != 0) {
    Script_LoadSND(0, 4, arg1, 0);
  }
  return;
}

static void script_EBF3(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_shenRequestGameModuleTransition(arg0, arg1, arg2);
  SCNF_WaitFrames_0000(1);
  return;
}

static float script_EC08(int32_t arg0, float arg1, float arg2, float arg3, float arg4) {
  float local_FC;
  float local_F8;
  float local_F4;
  float local_F0;
  Vector3 local_E4;
  Script_EVT_GetPosition(arg0, -1, &local_E4, ENTITY_XFORM_WORLD_SPACE);
  local_F4 = arg3 - arg1;
  local_FC = (float)0 - (arg4 - arg2);
  local_F8 = (&local_E4)->x - arg1;
  local_F0 = (&local_E4)->z - arg2;
  return local_FC * local_F8 + local_F4 * local_F0;
}

static int32_t script_EC53(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  int32_t local_FC;
  local_FC = 0;
  if (!((float)((scn3_bits){.f = script_EC08(arg0, *(float *)arg1, *(float *)(arg1 + 8), *(float *)arg2, *(float *)(arg2 + 8))}).i < 0.0f)) {
    if (!((float)((scn3_bits){.f = script_EC08(arg0, *(float *)arg2, *(float *)(arg2 + 8), *(float *)arg3, *(float *)(arg3 + 8))}).i < 0.0f)) {
      if (!((float)((scn3_bits){.f = script_EC08(arg0, *(float *)arg3, *(float *)(arg3 + 8), *(float *)arg4, *(float *)(arg4 + 8))}).i < 0.0f)) {
        if (!((float)((scn3_bits){.f = script_EC08(arg0, *(float *)arg4, *(float *)(arg4 + 8), *(float *)arg1, *(float *)(arg1 + 8))}).i < 0.0f)) {
          local_FC = 1;
        }
      }
    }
  }
  return local_FC;
}

static void script_ECF5(void) {
  (void)((task_366 = -1) & 1);
  (void)((task_367 = -1) & 1);
  (void)((task_368 = -1) & 1);
  (void)((task_369 = -1) & 1);
  (void)((task_36A = -1) & 1);
  (void)((task_36B = -1) & 1);
  (void)((task_36C = -1) & 1);
  (void)((task_36D = -1) & 1);
  (void)((task_36E = -1) & 1);
  (void)((task_36F = -1) & 1);
  return;
}

__attribute__((scn_string_bindings(0, data_3F4A6, 2, data_3F4C5, 4, data_3F4E4, 6, data_3F503, 8, data_3F521, 10, data_3F541, 12, data_3F560, 14, data_3F587, 16, data_3F5AC, 18, data_3F5CC)))
static void script_ED5C(void) {
  if (task_366 & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 887) == 1) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_GORO_QTE_COMPLETE) == 0)) {
    script_3958D();
    Script_scenCreateTask(script_F709, 0);
    Script_sprintf("%s\012", "MFSY: GORO_QTE Check Start");
    (void)((task_366 = 0) & 8);
  }
  if (task_36C & -((-(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_COFFEE_EVENT_STARTED) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_COFFEE_ROUTE_ENABLED) == 1) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS08_SCENE_EXIT_COMPLETE) == 0)) != 0)) {
    Script_scenCreateTask(script_F742, 0);
    Script_sprintf("%s\012", "MFSY: COFFE_EV Check Start");
    (void)((task_36C = 0) & 8);
  }
  if (task_36D & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS08_SCENE_EXIT_COMPLETE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_COFFEE_EVENT_STARTED) == 1) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_COFFEE_EVENT_COMPLETE) == 0)) {
    Script_scenCreateTask(script_F7B1, 0);
    Script_sprintf("%s\012", "MFSY: COFFE_SA Check Start");
    (void)((task_36D = 0) & 8);
  }
  if (task_367 & -((-(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_BOX_RESCUE_ROUTE_UNLOCKED) == 1) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_BOX_RESCUE_ROUTE_COMPLETE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_POST_BOX_ROUTE_SLEEP_BRANCH_MARKED) == 0)) != 0)) {
    Script_scenCreateTask(script_F8BD, 0);
    Script_sprintf("%s\012", "MFSY: BOX_USE Check Start");
    (void)((task_367 = 0) & 8);
  }
  if (task_368 & -((-(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_HISA_REQUEST_UNLOCKED) == 1) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_HISA_REQUEST_ACCEPTED) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_HISA_REQUEST_DECLINED) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS08_SCENE_EXIT_COMPLETE) == 0)) != 0)) {
    Script_scenCreateTask(script_F8CC, 0);
    Script_sprintf("%s\012", "MFSY: HISA_IRAI Check Start");
    (void)((task_368 = 0) & 8);
  }
  if (task_369 & -((-(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_HISA_REQUEST_ACCEPTED) == 1) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_HISA_QTE_RESOLVED) == 0)) != 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_HISA_REQUEST_DECLINED) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS08_SCENE_EXIT_COMPLETE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_WHAT_HAPPENED_TO_MAI) == 0)) {
    Script_scenCreateTask(script_FC4D, 0);
    Script_sprintf("%s\012", "MFSY: HISA_QTE Check Start");
    (void)((task_369 = 0) & 8);
  }
  if (task_36A & -((-(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_HISA_QTE_RESOLVED) == 1) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_WHAT_HAPPENED_TO_MAI) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MKSG_HELP_ROUTE_ACTIVE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 798) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS08_SCENE_EXIT_COMPLETE) == 0)) != 0)) {
    Script_scenCreateTask(script_FD03, 0);
    Script_sprintf("%s\012", "MFSY: HISA_QTE_HOUKOKU Check Start");
    (void)((task_36A = 0) & 8);
  }
  if (task_36B & -((-(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS8S_MAIN_EVENT_COMPLETE) == 1) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS08_SCENE_EXIT_COMPLETE) == 0)) != 0)) {
    Script_scenCreateTask(script_FF39, 0);
    Script_sprintf("%s\012", "MFSY: FLASHLIGHT_QTE Check Start");
    (void)((task_36B = 0) & 8);
  }
  if (task_36E & -((-(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MKSG_HELP_ROUTE_ACTIVE) == 1) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS08_SCENE_EXIT_COMPLETE) == 0)) != 0)) {
    Script_scenCreateTask(script_FFEF, 0);
    Script_sprintf("%s\012", "MFSY: MKSG HELP Check Start");
    (void)((task_36E = 0) & 8);
  }
  if (task_36F & -((-(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_BOX_RESCUE_ROUTE_UNLOCKED) == 1) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_BOX_RESCUE_ROUTE_COMPLETE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_POST_BOX_ROUTE_SLEEP_BRANCH_MARKED) == 1) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_BOX_RESCUE_ROUTE_COMPLETE) == 0)) != 0)) {
    Script_scenCreateTask(script_1004A, 0);
    Script_sprintf("%s\012", "MFSY: GORO_KYUUSAI Check Start");
    (void)((task_36F = 0) & 8);
  }
  return;
}

static void script_F0EE(const Vector3 *arg0, const Vector3 *arg1, int32_t arg2) {
  for (;;) {
    if (Script_IsPlayerAllowed(0)) {
      if (Script_SetTaskDataValue(CHARA_AKIR, arg0, arg1, 0)) {
        if (script_F170(arg2) == 1) {
          break;
        }
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_F12C(const Vector3 *arg0, float arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  int32_t unused_F8[2];
  for (;;) {
    if (Script_IsPlayerAllowed(0) & task_284) {
      if (Script_CheckTargetWithinAngleRange(arg0, arg1, arg2, arg3, &task_23C)) {
        if (script_F170(arg4) == 1) {
          break;
        }
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int8_t script_F170(int32_t arg0) {
  int16_t local_FE;
  int16_t local_FC;
  int16_t local_FA;
  int8_t local_F9;
  local_F9 = 0;
  local_FE = *(int8_t *)arg0 * 60 + *(int8_t *)(arg0 + 1);
  local_FC = task_26C.hours * 60 + task_26C.minutes;
  local_FA = *(int8_t *)(arg0 + 2) * 60 + *(int8_t *)(arg0 + 3);
  if ((local_FE <= local_FC) & (local_FC <= local_FA)) {
    if (*(int8_t *)(arg0 + 4) & 0x10) {
      local_F9 = 1;
    } else {
      if (*(int8_t *)(arg0 + 4) & 1 & -(task_275 == 0)) {
        local_F9 = 1;
      }
      if (*(int8_t *)(arg0 + 4) & 2 & -(task_275 == 1)) {
        local_F9 = 1;
      }
      if (*(int8_t *)(arg0 + 4) & 4 & -(task_275 == 2)) {
        local_F9 = 1;
      }
      if (*(int8_t *)(arg0 + 4) & 8 & -(task_275 == 3)) {
        local_F9 = 1;
      }
    }
  }
  return local_F9;
}

static int32_t script_F244(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  int8_t local_DB;
  int8_t local_DA;
  int32_t local_D4;
  (void)((local_DB = 0) & 4);
  (void)((local_DA = 0) & 4);
  (void)((task_287 = 0) & 4);
  (&local_E8)->y = 0.0f;
  for (;;) {
    local_E0 = Script_srchUtils(-1);
    if (local_E0 == 37) {
      Script_ryoControl(PCCTL_DisablePlayerControl);
      Script_ryoControl(PCCTL_RequestNewMode, 1);
      SCNF_PrepareCassetteUI();
      Script_ryoControl(RYO_RestoreControl);
    }
    local_E0 = Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex);
    if ((local_E0 >= 0) & (local_E0 < 9)) {
      script_E0E3(local_E0);
    }
    if ((local_E0 == 37) & (task_26C.hours >= 19)) {
      Script_srchUtils(SRCH_SCRIPT_Reset);
    }
    if (Script_IsPlayerAllowed(0)) {
      if (Script_SetTaskDataValue(CHARA_AKIR, &data_3F5F0, &data_3F5FC, 0)) {
        if (task_27F & 8) {
          script_11661();
        }
        Script_uiUtils(0, 63, 0);
        (void)((local_DA = 0) & 8);
      } else {
        if (local_DA == 0) {
          Script_uiUtils(0, 0, 0);
          (void)((local_DA = -1) & 8);
        }
      }
      if (Script_SetTaskDataValue(CHARA_AKIR, &data_3F608, &data_3F614, 0)) {
        script_2044F(CHARA_KEBL);
      }
      if (Script_SetTaskDataValue(CHARA_AKIR, &data_3F620, &data_3F62C, 0)) {
        script_2044F(CHARA_SNJY);
      }
      if (Script_SetTaskDataValue(CHARA_AKIR, &data_3F638, &data_3F644, 0)) {
        script_2044F(CHARA_KEBJ);
      }
      if (Script_SetTaskDataValue(CHARA_AKIR, &data_3F6B0, &data_3F6BC, 0)) {
        script_20D64(CHARA_KEBB);
        script_ED5C();
      }
      if (script_EC53(CHARA_AKIR, (int32_t)&data_3F650, (int32_t)&data_3F65C, (int32_t)&data_3F668, (int32_t)&data_3F674)) {
        script_2044F(CHARA_KEBI);
      }
      if (script_EC53(CHARA_AKIR, (int32_t)&data_3F680, (int32_t)"ff6A", (int32_t)&data_3F698, (int32_t)&data_3F6A4)) {
        script_2044F(CHARA_KEBG);
        script_ED5C();
      }
      local_DC = 0;
      local_E4 = 0;
      local_D4 = task_278;
      switch (local_D4) {
        case 1:
          local_E8.x = -119.4f;
          (&local_E8)->z = 108.5f;
          if (Script_CheckTargetWithinAngleRange(&local_E8, 1.5f, 0xC000, 12743, &task_23C)) {
            local_E4 = 0x3048494A;
          } else {
            local_E8.x = 0.8f;
            (&local_E8)->z = 104.55f;
            if (Script_CheckTargetWithinAngleRange(&local_E8, 1.5f, 0x2000, 12743, &task_23C)) {
              local_E4 = 0x3148494A;
            }
          }
          break;
        case 0:
          local_E8.x = -119.4f;
          (&local_E8)->z = 108.5f;
          if (Script_CheckTargetWithinAngleRange(&local_E8, 1.5f, 0xC000, 12743, &task_23C)) {
            local_E4 = 0x3048494A;
          } else {
            local_E8.x = -97.8f;
            (&local_E8)->z = 11.9f;
            if (Script_CheckTargetWithinAngleRange(&local_E8, 1.5f, 0x6000, 12743, &task_23C)) {
              local_E4 = 0x3148494A;
            }
          }
          break;
        case 2:
          local_E8.x = 0.8f;
          (&local_E8)->z = 104.55f;
          if (Script_CheckTargetWithinAngleRange(&local_E8, 1.5f, 0x2000, 12743, &task_23C)) {
            local_E4 = 0x3148494A;
          } else {
            local_E8.x = 51.7f;
            (&local_E8)->z = 52.4f;
            if (Script_CheckTargetWithinAngleRange(&local_E8, 1.5f, 0x2000, 12743, &task_23C)) {
              local_E4 = 0x3048494A;
            }
          }
          break;
      }
      if (local_E4) {
        local_F4.x = (&task_23C)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, task_248.y) * 0.3f;
        (&local_F4)->y = 0.0f;
        (&local_F4)->z = task_23C.z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, task_248.y) * 0.3f;
        if (Script_CheckTargetWithinAngleRange(&local_F4, 1.4f, task_248.y, 7281, &local_E8) == 0) {
          local_E4 = 0;
        }
        if (local_E4) {
          local_DC = 5;
          if (~local_DB) {
            Script_uiUtils(0, 25, 0);
          }
          if (script_1123D(local_E4)) {
            local_DC = 0;
            (void)((local_DB = 0) & 1);
          }
        }
      }
      if (local_DC == 0) {
        local_DC = script_E357(local_DB, local_E4);
      }
      if (local_DC) {
        (void)((task_287 = -1) & 8);
        if (~local_DB) {
          Script_sicnUtils(0, local_DC, 1);
          (void)((local_DB = -1) & 8);
        }
      } else {
        if (local_DB | task_287) {
          Script_uiUtils(0, 0, 0);
          Script_sicnUtils(1, 0, 0);
          (void)((local_DB = 0) & 8);
          (void)((task_287 = 0) & 8);
        }
      }
    } else {
      if (local_DB & 8) {
        Script_sicnUtils(1, 0, 0);
        (void)((local_DB = 0) & 8);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_F709(void) {
  script_F12C((const Vector3 *)&data_3F6D4, 8.0f, 0, 0x8000, (int32_t)&data_3F6E0);
  script_110D8();
  script_3955A();
  script_EB73();
  script_11125();
  script_ED5C();
  end:;
}

static int32_t script_F742(void) {
  script_F12C((const Vector3 *)&data_3F6E8, 5.0f, 0xE000, 15473, (int32_t)&data_3F6F4);
  script_110D8();
  script_2716B();
  Script_scenUtils(SCENARIO_CMD_SET_CHARA_RADIUS_SQUARED, 0x555A5953, 0.0f);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_COFFEE_EVENT_STARTED, 1);
  script_11125();
  (void)((task_289 = -1) & 0x20);
  Script_ryoSleepUtils(3, 1, 9, task_289);
  (void)((task_288 = -1) & 0x20);
  script_ED5C();
  end:;
}

__attribute__((scn_string_bindings(0, data_3F708)))
static int32_t script_F7B1(void) {
  int8_t local_FF;
  (void)((local_FF = 0) & 0x40);
  if (~task_288) {
    task_268 = script_E948(CHARA_SYZU);
    Script_scenUtils(SCENARIO_CMD_SET_CHARA_RADIUS_SQUARED, 0x555A5953, 0.0f);
    script_27DEC("HAKO");
    script_27E10();
    (void)((task_288 = -1) & 8);
  }
  for (;;) {
    if (Script_IsPlayerAllowed(0) & Script_CheckTargetWithinAngleRange(&data_3F6FC, 1.6f, 0xA000, 12743, &task_23C) & Script_CheckTargetWithinAngleRange(&task_23C, 1.5f, task_248.y, 7281, &data_3F6FC)) {
      if (~local_FF) {
        SCNF_RequestIconNotification(0x100, 0, 0, 1);
        (void)((local_FF = -1) & 8);
      }
      if (Script_GetControllerInput(1) == 0x400) {
        script_27FEA();
        (void)((task_289 = -1) & 8);
        Script_ryoSleepUtils(3, 1, 9, task_289);
      }
    } else {
      if (local_FF & 0x80) {
        SCNF_RequestIconNotification(0, 0, 0, 2);
        (void)((local_FF = 0) & 8);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_F8BD(void) {
  script_2B1E0(CHARA_AKIR);
  end:;
}

__attribute__((scn_string_bindings(0, data_3F721, 1, data_3F72C, 2, data_3F739, 3, data_3F73F, 4, data_3F74D, 6, data_3F75F, 8, data_3F771, 10, data_3F783, 12, data_3F795, 13, data_3F79A, 14, data_3F7A0)))
static int32_t script_F8CC(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  int32_t local_E4;
  script_F12C((const Vector3 *)&data_3F710, 8.0f, 0x2000, 0x4000, (int32_t)&data_3F71C);
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 0, 400, 0);
  Script_ryoControl(PCCTL_RequestNewMode, 1);
  script_110D8();
  Script_executeAicaCommand(2720, 2, 30);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LODSnapshotUtils(DISP_CMD_SaveDistancesAndCaptureLodState);
  script_E887();
  Script_GULLProgram_Utils(2);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  local_FC = script_E948(CHARA_HISA);
  local_F8 = script_E948(CHARA_HINA);
  local_F4 = script_E948(CHARA_ISYM);
  local_F0 = script_E948(CHARA_NMNO);
  local_EC = script_E948(CHARA_MIHO);
  local_E8 = script_E948(CHARA_JOE_);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  script_EBBF("bgm117.snd", "a1_hisak.snd");
  Script_LoadVoiceFile(1, "01HIS");
  script_EAFC("scene/02/MFSY", "HISA");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 0, "seqdata0.bin", "HISA");
  Script_AuthorUtils(0, 1, "seqdata1.bin", "HISA");
  Script_AuthorUtils(0, 2, "seqdata2.bin", "HISA");
  Script_AuthorUtils(0, 4, "seqdata4.bin", "HISA");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  script_25F18(1);
  Script_charaSetRefboardFlagTo(CHARA_HISA, 1);
  local_E4 = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 2, 3, 0xFFFF, 300, 3, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT);
  if (local_E4 == 2) {
    script_2665E(1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_HISA_REQUEST_ACCEPTED, 1);
  } else {
    script_26788(1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_HISA_REQUEST_DECLINED, 1);
  }
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(1);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  Script_LoadVoiceFile(0, "01HIS");
  Script_AuthorUtils(AUTH_FREE, 0);
  Script_AuthorUtils(AUTH_FREE, 1);
  Script_AuthorUtils(AUTH_FREE, 2);
  Script_AuthorUtils(AUTH_FREE, 4);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "HISA");
  script_EA10(CHARA_HISA, local_FC);
  script_EA10(CHARA_HINA, local_F8);
  script_EA10(CHARA_ISYM, local_F4);
  script_EA10(CHARA_NMNO, local_F0);
  script_EA10(CHARA_MIHO, local_EC);
  script_EA10(CHARA_JOE_, local_E8);
  script_EB73();
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_HISA_REQUEST_ACCEPTED) == 1) {
    script_A671(CHARA_AKIR, 0.1096f, -0.0221f, 100.688698f);
    script_A699(CHARA_AKIR, 0.0f, 330.0f, 0.0f);
  } else {
    script_A671(CHARA_AKIR, 0.0035f, -0.0221f, 101.907303f);
  }
  Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  script_E866();
  script_D9DE();
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  Script_SetGameStateTo(AUTH_None);
  script_11125();
  script_ED5C();
  end:;
}

static int32_t script_FC4D(void) {
  int32_t unused_FC;
  script_F0EE((const Vector3 *)&data_3F7A8, (const Vector3 *)&data_3F7B4, (int32_t)&data_3F7C0);
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 0, 400, 0);
  script_110D8();
  script_3435C();
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_HISA_QTE_RESOLVED, 1);
  script_EB73();
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x41534948) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
  Script_SetGameStateTo(AUTH_None);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  Script_filmCheckOrSetTransitionUtil(10, 255, 0, 0, 0, 0, 0, 0, 0);
  script_11125();
  script_ED5C();
  return Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 1, 400, 0);
}

__attribute__((scn_string_bindings(0, data_3F7EC, 1, data_3F7FA, 3, data_3F80C, 4, data_3F811, 5, data_3F81C, 6, data_3F829, 7, data_3F82F, 8, data_3F83C, 9, data_3F842)))
static int32_t script_FD03(void) {
  int32_t local_FC;
  for (;;) {
    if ((Script_CheckTargetWithinAngleRange(&data_3F7C8, 5.0f, 0x2000, 0x4000, &task_23C) == 0) | (Script_IsPlayerAllowed(0) == 0) | ~task_284) {
      SCNF_WaitFrames_0000(1);
      continue;
    }
    if (task_26C.hours * 100 + task_26C.minutes > 1900) goto L00FF38;
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x41534948)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  script_110D8();
  Script_executeAicaCommand(2720, 2, 30);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  script_EAFC("scene/02/MFSY", "HISA");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 3, "seqdata3.bin", "HISA");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  local_FC = script_E948(CHARA_HISA);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  script_EBBF("bgm117.snd", "a1_hisak.snd");
  Script_LoadVoiceFile(1, "01HIS");
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(CHARA_HISA, 1);
  Script_GULLProgram_Utils(2);
  Script_filmCheckOrSetTransitionUtil(10, 255, 0, 0, 0, 0, 0, 0, 0);
  script_26830(0);
  Script_PlayCharaMotion(CHARA_HISA, 28685, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_LoadSND(0, 4, "f1yokosu.snd", 0);
  Script_LoadVoiceFile(0, "01HIS");
  Script_AuthorUtils(AUTH_FREE, 3);
  script_28641();
  Script_Ambient1SoundUtils(2, 17);
  script_D744();
  script_28D10(CHARA_AKIR);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "HISA");
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_AFTER_8PM_GUARD_SHIFT, 1);
  script_EB73();
  script_EA10(CHARA_HISA, local_FC);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  script_D9DE();
  script_11125();
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_convSetCamInactiveFlag(-1);
  Script_SetGameStateTo(AUTH_None);
  script_ED5C();
  L00FF38:;
}

static int32_t script_FF39(void) {
  while (task_26C.hours < 19) {
    SCNF_WaitFrames_0000(1);
  }
  while ((Script_SetTaskDataValue(CHARA_AKIR, &data_3F848, &data_3F854, 0) == 0) | (Script_IsPlayerAllowed(0) == 0) | ~task_284) {
    SCNF_WaitFrames_0000(1);
  }
  script_110D8();
  Script_executeAicaCommand(2720, 1, 30);
  Script_executeAicaCommand(2720, 2, 30);
  Script_executeAicaCommand(2720, 3, 30);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_EBF3(2, SHEN_NAME('MKSG'), 0);
  end:;
}

static int32_t script_FFEF(void) {
  while ((Script_SetTaskDataValue(CHARA_AKIR, &data_3F860, &data_3F86C, 0) == 0) | (Script_IsPlayerAllowed(0) == 0) | ~task_284) {
    SCNF_WaitFrames_0000(1);
  }
  script_110D8();
  script_29CB3();
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 838, 1);
  script_11125();
  script_ED5C();
  end:;
}

static int32_t script_1004A(void) {
  script_F12C((const Vector3 *)&data_3F878, 8.0f, 0, 0x8000, (int32_t)&data_3F884);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  script_2A9C2();
  script_ED5C();
  end:;
}

static void script_100B0(void) {
  int32_t local_FC;
  local_FC = task_276;
  switch (local_FC) {
    case 0:
      Script_scenToggleVisibleFor(SHEN_NAME('GATL'), 0);
      Script_scenToggleVisibleFor(SHEN_NAME('GATR'), 0);
      script_A671(SHEN_NAME('GATL'), -125.97f, 0.0f, -16.075001f);
      script_A699(SHEN_NAME('GATL'), 0.0f, 90.0f, 0.0f);
      script_A671(SHEN_NAME('GATR'), -125.97f, 0.0f, -7.485f);
      script_A699(SHEN_NAME('GATR'), 0.0f, 90.0f, 0.0f);
      break;
    case 1:
      Script_scenToggleVisibleFor(SHEN_NAME('GATL'), 1);
      Script_scenToggleVisibleFor(SHEN_NAME('GATR'), 1);
      script_A671(SHEN_NAME('GATL'), -125.97f, 0.0f, -13.05f);
      script_A699(SHEN_NAME('GATL'), 0.0f, 90.0f, 0.0f);
      script_A671(SHEN_NAME('GATR'), -125.97f, 0.0f, -10.165f);
      script_A699(SHEN_NAME('GATR'), 0.0f, 90.0f, 0.0f);
      break;
    case 2:
      Script_scenToggleVisibleFor(SHEN_NAME('GATL'), 1);
      Script_scenToggleVisibleFor(SHEN_NAME('GATR'), 1);
      script_A671(SHEN_NAME('GATL'), -125.97f, 0.0f, -14.45f);
      script_A699(SHEN_NAME('GATL'), 0.0f, 90.0f, 0.0f);
      script_A671(SHEN_NAME('GATR'), -125.97f, 0.0f, -11.565f);
      script_A699(SHEN_NAME('GATR'), 0.0f, 90.0f, 0.0f);
      break;
  }
  return;
}

static void script_10234(void) {
  Script_scenToggleVisibleFor(SHEN_NAME('GATL'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GATR'), 0);
  script_A671(SHEN_NAME('GATL'), -120.82f, 0.0f, 13.77f);
  script_A699(SHEN_NAME('GATL'), 0.0f, 90.0f, 0.0f);
  script_A671(SHEN_NAME('GATR'), -120.82f, 0.0f, 22.062f);
  script_A699(SHEN_NAME('GATR'), 0.0f, 90.0f, 0.0f);
  return;
}

static void script_102A9(void) {
  Script_scenToggleVisibleFor(SHEN_NAME('GATL'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GATR'), 0);
  script_A671(SHEN_NAME('GATL'), -120.82f, 0.0f, 64.171997f);
  script_A699(SHEN_NAME('GATL'), 0.0f, 90.0f, 0.0f);
  script_A671(SHEN_NAME('GATR'), -120.82f, 0.0f, 72.45f);
  script_A699(SHEN_NAME('GATR'), 0.0f, 90.0f, 0.0f);
  return;
}

static void script_1031E(void) {
  Script_scenToggleVisibleFor(SHEN_NAME('GATL'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GATR'), 0);
  script_A671(SHEN_NAME('GATL'), 139.404999f, 0.0f, 19.351f);
  script_A699(SHEN_NAME('GATL'), 0.0f, -90.0f, 0.0f);
  script_A671(SHEN_NAME('GATR'), 139.404999f, 0.0f, 27.628f);
  script_A699(SHEN_NAME('GATR'), 0.0f, -90.0f, 0.0f);
  return;
}

static void script_10393(void) {
  script_A671(SHEN_NAME('JIH0'), -119.7f, 0.0f, 108.5f);
  script_A699(SHEN_NAME('JIH0'), 0.0f, 90.0f, 0.0f);
  script_A671(SHEN_NAME('JIH1'), 1.008f, 0.0f, 104.762901f);
  script_A699(SHEN_NAME('JIH1'), 0.0f, 225.0f, 0.0f);
  return;
}

static void script_103F0(void) {
  script_A671(SHEN_NAME('JIH0'), -119.7f, 0.0f, 108.5f);
  script_A699(SHEN_NAME('JIH0'), 0.0f, 90.0f, 0.0f);
  script_A671(SHEN_NAME('JIH1'), -97.6f, 0.0f, 11.7f);
  script_A699(SHEN_NAME('JIH1'), 0.0f, 315.0f, 0.0f);
  return;
}

static void script_1044D(void) {
  script_A671(SHEN_NAME('JIH1'), 1.008f, 0.0f, 104.762901f);
  script_A699(SHEN_NAME('JIH1'), 0.0f, 225.0f, 0.0f);
  script_A671(SHEN_NAME('JIH0'), 51.9f, 0.0f, 52.6f);
  script_A699(SHEN_NAME('JIH0'), 0.0f, 225.0f, 0.0f);
  return;
}

static void script_104AA(void) {
  if (0) {
    Script_charaDisplay(SHEN_NAME('FOOD'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('FOOD'), CDM_Show);
      goto L0104D4;
    }
  }
  L0104D4:;
  Script_scenToggleVisibleFor(SHEN_NAME('FOOD'), 1);
  Script_charaDisplay(SHEN_NAME('FOD_'), CDM_Show);
  Script_charaDisplay(0x31786264, CDM_Show);
  Script_charaDisplay(0x32786264, CDM_Show);
  Script_tmnmUtils(SHEN_NAME('FOOD'), 1535, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
  (void)((task_286 = -1) & 4);
  return;
}

static void script_10529(void) {
  if (-1) {
    Script_charaDisplay(SHEN_NAME('FOOD'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('FOOD'), CDM_Show);
      goto L010553;
    }
  }
  L010553:;
  Script_scenToggleVisibleFor(SHEN_NAME('FOOD'), 0);
  Script_charaDisplay(SHEN_NAME('FOD_'), CDM_Hide);
  Script_charaDisplay(0x31786264, CDM_Hide);
  Script_charaDisplay(0x32786264, CDM_Hide);
  (void)((task_286 = 0) & 4);
  return;
}

static void script_1058B(void) {
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x31584F42) == 0) {
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('BOX1'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('BOX1'), CDM_Show);
        goto L0105CC;
      }
    }
    L0105CC:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOX1'), 1);
    if (0) {
      Script_charaDisplay(SHEN_NAME('BOX2'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('BOX2'), CDM_Show);
        goto L010600;
      }
    }
    L010600:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOX2'), 1);
    if (0) {
      Script_charaDisplay(SHEN_NAME('BOX3'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('BOX3'), CDM_Show);
        goto L010634;
      }
    }
    L010634:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOX3'), 1);
    if (0) {
      Script_charaDisplay(SHEN_NAME('BOX4'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('BOX4'), CDM_Show);
        goto L010668;
      }
    }
    L010668:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOX4'), 1);
    if (0) {
      Script_charaDisplay(SHEN_NAME('BOX5'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('BOX5'), CDM_Show);
        goto L01069C;
      }
    }
    L01069C:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOX5'), 1);
    if (0) {
      Script_charaDisplay(SHEN_NAME('BOX6'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('BOX6'), CDM_Show);
        goto L0106D0;
      }
    }
    L0106D0:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOX6'), 1);
    if (0) {
      Script_charaDisplay(SHEN_NAME('BOX7'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('BOX7'), CDM_Show);
        goto L010704;
      }
    }
    L010704:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOX7'), 1);
    if (0) {
      Script_charaDisplay(SHEN_NAME('BOX8'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('BOX8'), CDM_Show);
        goto L010738;
      }
    }
    L010738:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOX8'), 1);
    if (0) {
      Script_charaDisplay(SHEN_NAME('BOX9'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('BOX9'), CDM_Show);
        goto L01076C;
      }
    }
    L01076C:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOX9'), 1);
    if (0) {
      Script_charaDisplay(SHEN_NAME('BOXA'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('BOXA'), CDM_Show);
        goto L0107A0;
      }
    }
    L0107A0:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOXA'), 1);
    if (0) {
      Script_charaDisplay(SHEN_NAME('BOXB'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('BOXB'), CDM_Show);
        goto L0107D4;
      }
    }
    L0107D4:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOXB'), 1);
    if (0) {
      Script_charaDisplay(SHEN_NAME('BOXC'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('BOXC'), CDM_Show);
        goto L010808;
      }
    }
    L010808:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOXC'), 1);
    if (0) {
      Script_charaDisplay(SHEN_NAME('BOXD'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('BOXD'), CDM_Show);
        goto L01083C;
      }
    }
    L01083C:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOXD'), 1);
    if (0) {
      Script_charaDisplay(SHEN_NAME('BOXE'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('BOXE'), CDM_Show);
        goto L010870;
      }
    }
    L010870:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOXE'), 1);
    if (0) {
      Script_charaDisplay(SHEN_NAME('BOXF'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('BOXF'), CDM_Show);
        goto L0108A4;
      }
    }
    L0108A4:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOXF'), 1);
    if (0) {
      Script_charaDisplay(SHEN_NAME('BOXG'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('BOXG'), CDM_Show);
        goto L0108D8;
      }
    }
    L0108D8:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOXG'), 1);
    if (0) {
      Script_charaDisplay(SHEN_NAME('BOXH'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('BOXH'), CDM_Show);
        goto L01090C;
      }
    }
    L01090C:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOXH'), 1);
    if (0) {
      Script_charaDisplay(SHEN_NAME('BOXI'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('BOXI'), CDM_Show);
        goto L010940;
      }
    }
    L010940:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOXI'), 1);
    if (0) {
      Script_charaDisplay(SHEN_NAME('BOXJ'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('BOXJ'), CDM_Show);
        goto L010974;
      }
    }
    L010974:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOXJ'), 1);
    if (0) {
      Script_charaDisplay(SHEN_NAME('BOXK'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('BOXK'), CDM_Show);
        goto L0109A8;
      }
    }
    L0109A8:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOXK'), 1);
  }
  return;
}

static void script_109B5(void) {
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x31584F42) == 0) {
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BOX1'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('BOX1'), CDM_Show);
        goto L0109F6;
      }
    }
    L0109F6:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOX1'), 0);
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BOX2'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('BOX2'), CDM_Show);
        goto L010A2A;
      }
    }
    L010A2A:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOX2'), 0);
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BOX3'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('BOX3'), CDM_Show);
        goto L010A5E;
      }
    }
    L010A5E:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOX3'), 0);
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BOX4'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('BOX4'), CDM_Show);
        goto L010A92;
      }
    }
    L010A92:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOX4'), 0);
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BOX5'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('BOX5'), CDM_Show);
        goto L010AC6;
      }
    }
    L010AC6:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOX5'), 0);
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BOX6'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('BOX6'), CDM_Show);
        goto L010AFA;
      }
    }
    L010AFA:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOX6'), 0);
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BOX7'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('BOX7'), CDM_Show);
        goto L010B2E;
      }
    }
    L010B2E:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOX7'), 0);
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BOX8'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('BOX8'), CDM_Show);
        goto L010B62;
      }
    }
    L010B62:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOX8'), 0);
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BOX9'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('BOX9'), CDM_Show);
        goto L010B96;
      }
    }
    L010B96:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOX9'), 0);
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BOXA'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('BOXA'), CDM_Show);
        goto L010BCA;
      }
    }
    L010BCA:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOXA'), 0);
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BOXB'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('BOXB'), CDM_Show);
        goto L010BFE;
      }
    }
    L010BFE:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOXB'), 0);
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BOXC'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('BOXC'), CDM_Show);
        goto L010C32;
      }
    }
    L010C32:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOXC'), 0);
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BOXD'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('BOXD'), CDM_Show);
        goto L010C66;
      }
    }
    L010C66:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOXD'), 0);
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BOXE'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('BOXE'), CDM_Show);
        goto L010C9A;
      }
    }
    L010C9A:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOXE'), 0);
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BOXF'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('BOXF'), CDM_Show);
        goto L010CCE;
      }
    }
    L010CCE:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOXF'), 0);
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BOXG'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('BOXG'), CDM_Show);
        goto L010D02;
      }
    }
    L010D02:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOXG'), 0);
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BOXH'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('BOXH'), CDM_Show);
        goto L010D36;
      }
    }
    L010D36:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOXH'), 0);
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BOXI'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('BOXI'), CDM_Show);
        goto L010D6A;
      }
    }
    L010D6A:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOXI'), 0);
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BOXJ'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('BOXJ'), CDM_Show);
        goto L010D9E;
      }
    }
    L010D9E:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOXJ'), 0);
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BOXK'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('BOXK'), CDM_Show);
        goto L010DD2;
      }
    }
    L010DD2:;
    Script_scenToggleVisibleFor(SHEN_NAME('BOXK'), 0);
  }
  return;
}

static void script_10DDF(void) {
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x31584F42) == 0) {
  } else {
    Script_scenToggleVisibleFor(SHEN_NAME('BOX1'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('BOX2'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('BOX3'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('BOX4'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('BOX5'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('BOX6'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('BOX7'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('BOX8'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('BOX9'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('BOXA'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('BOXB'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('BOXC'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('BOXD'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('BOXE'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('BOXF'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('BOXG'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('BOXH'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('BOXI'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('BOXJ'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('BOXK'), 0);
  }
  return;
}

static void script_10EE9(void) {
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 180.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 100.0f);
  Script_LODUtils(LOD_UTILS_SET_NPC_DISPLAY_RADIUS, 48.0f);
  Script_mapmSetRenderFlag(0, 1);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 1);
  Script_mapmSetRenderFlag(3, 1);
  Script_mapmSetRenderFlag(4, 0);
  Script_mapmSetRenderFlag(5, 1);
  Script_mapmSetRenderFlag(6, 1);
  Script_mapmSetRenderFlag(7, 1);
  Script_mapmSetRenderFlag(8, 0);
  Script_mapmSetRenderFlag(9, 1);
  Script_mapmSetRenderFlag(10, 1);
  Script_mapmSetRenderFlag(11, 1);
  Script_mapmSetRenderFlag(12, 0);
  Script_mapmSetRenderFlag(13, 0);
  Script_mapmSetRenderFlag(14, 0);
  Script_mapmSetRenderFlag(15, 0);
  Script_mapmSetRenderFlag(16, 0);
  Script_mapmSetRenderFlag(17, 1);
  Script_mapmSetRenderFlag(18, 1);
  if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 2)) {
    Script_mapmSetRenderFlag(2, 0);
    Script_mapmSetRenderFlag(6, 0);
    Script_mapmSetRenderFlag(4, 1);
    Script_mapmSetRenderFlag(8, 1);
  } else {
    if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 3)) {
      Script_mapmSetRenderFlag(2, 0);
      Script_mapmSetRenderFlag(6, 0);
      Script_mapmSetRenderFlag(4, 1);
      Script_mapmSetRenderFlag(8, 1);
      Script_mapmSetRenderFlag(15, 1);
      goto L011026;
    }
  }
  L011026:;
  Script_scenToggleCharaFlag(0, 7, 1);
  Script_scenToggleCharaFlag(0, 200, 1);
  Script_scenToggleCharaFlag(0, 201, 0);
  if (task_26C.hours >= 19) {
    Script_mapmSetRenderFlag(13, 1);
    Script_mapmSetRenderFlag(17, 0);
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS08_SCENE_EXIT_COMPLETE) == 1) {
      Script_mapmSetRenderFlag(18, 0);
    }
    if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 2) | Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 3)) {
      Script_mapmSetRenderFlag(14, 1);
    } else {
      Script_mapmSetRenderFlag(12, 1);
    }
    Script_scenToggleCharaFlag(0, 7, 0);
    Script_scenToggleCharaFlag(0, 200, 0);
    Script_scenToggleCharaFlag(0, 201, 1);
  }
  Script_mapmSetCamFlags(0, -1);
  return;
}

static void script_110D8(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_mapmSetRenderFlag(16, 0);
  script_10DDF();
  return;
}

static void script_11125(void) {
  script_1058B();
  script_10EE9();
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_SetMotnFlag(CHARA_AKIR, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_3F88C);
  Script_SetMaxInterpSpeed(0.5f);
  if (task_26C.hours < 19) {
    Script_SetCharaFlags(CHARA_AKIR, 4);
    Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 2);
  } else {
    Script_SetCharaFlags(CHARA_AKIR, 0);
    Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  }
  Script_SetShadowRange(60.0f);
  Script_InitializeOsag(CHARA_AKIR);
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  return;
}

static int32_t script_1123D(int32_t arg0) {
  if (Script_GetControllerInput(1) == 0x400) {
    Script_sicnUtils(1, 0, 0);
    if (-(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_COFFEE_ROUTE_ENABLED) == 1) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS08_SCENE_EXIT_COMPLETE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_COFFEE_EVENT_STARTED) == 1) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_COFFEE_EVENT_COMPLETE) == 0) & task_289) {
      script_27A17(CHARA_AKIR, arg0, SHEN_NAME('JIHN'), SHEN_NAME('HAND'));
    } else {
      script_C05D(CHARA_AKIR, arg0, SHEN_NAME('JIHN'), SHEN_NAME('HAND'));
      Script_sicnUtils(0, 5, 1);
      (void)0;
    }
  }
  end:;
}

static void script_112DA(void) {
  return;
}

static void script_112DD(void) {
  if (-(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_HISA_REQUEST_UNLOCKED) == 1) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_HISA_QTE_RESOLVED) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MKSG_HELP_ROUTE_ACTIVE) == 0)) {
    Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 0, 1, 0);
  }
  return;
}

__attribute__((scn_string_bindings(1, data_3F8A7, 2, data_3F8B4, 3, data_3F8C2, 5, data_3F8D4, 7, data_3F8E6, 8, data_3F8EB, 9, data_3F8F9, 11, data_3F90E, 13, data_3F923, 14, data_3F92B, 15, data_3F930, 16, data_3F938, 17, data_3F93F)))
static void script_1131C(void) {
  int32_t local_FC;
  int8_t local_FB;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND);
  local_FC = script_E948(SHEN_NAME('BUS_'));
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 880, 1);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 887) == 0) {
    script_EB20();
    Script_LoadSND(0, 2, "rim099.snd", 0);
  }
  Script_LoadSND(0, 4, "a1_busno.snd", 0);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 887) == 1) {
    script_EAA8("scene/02/MFSY", "YBUS");
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.5f)) {
      local_FB = 2;
      Script_AuthorUtils(0, local_FB, "seqdata9.bin", "YBUS");
    } else {
      local_FB = 3;
      Script_AuthorUtils(0, local_FB, "seqdataB.bin", "YBUS");
    }
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  } else {
    script_EAA8("scene/02/MFSY", "YBUSFLY");
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    if (task_26C.hours < 19) {
      local_FB = 0;
      Script_AuthorUtils(0, local_FB, "seqdata6.bin", "YBUSFLY");
    } else {
      local_FB = 1;
      Script_AuthorUtils(0, local_FB, "seqdataD.bin", "YBUSFLY");
    }
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    script_E8A8();
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  }
  Script_LODSnapshotUtils(DISP_CMD_SaveDistancesAndCaptureLodState);
  script_11E0B(local_FB);
  script_11DB1(local_FB);
  script_11E59(local_FB);
  Script_LODSnapshotUtils(DISP_CMD_RestoreDistancesAndApplyLodState);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetGameStateTo(AUTH_None);
  Script_AuthorUtils(AUTH_FREE, local_FB);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 887) == 1) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "YBUS");
  } else {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "YBUSFLY");
  }
  script_EA10(SHEN_NAME('BUS_'), local_FC);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 887) == 0) {
    script_EB20();
    Script_LoadSND(0, 2, "SELECT", 0);
    Script_executeAicaCommand(16296, 0, 0);
    Script_executeAicaCommand(1184, 2, 100);
    script_E90A();
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 887, 1);
  }
  Script_LoadSND(0, 4, "f1yokosu.snd", 0);
  if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 935) == 1) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_LUNCH_WITH_TOM) == 0)) {
  } else {
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  }
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  return;
}

static void script_11661(void) {
  (void)((task_374 = 0) & 8);
  if (Script_CycleManagerUtils(CYCLE_CMD_GET_MOBJ_SPECIAL_DWORD120, 0x5F535542) != 0) {
    Script_srchUtils(SRCH_SCRIPT_Reset);
    Script_SetGameStateTo(AUTH_None);
    Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
    SCNF_RequestIconNotification(0x100, 0, 8, 1);
    (void)((task_374 = -1) & 8);
    for (;;) {
      if (Script_IsPlayerAllowed(0) & -(Script_GetControllerInput(1) == 0x400)) {
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
        Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
        Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
        Script_shenClearFreeState(0);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        for (;;) {
          if (Script_CycleManagerUtils(CYCLE_CMD_GET_MOBJ_SPECIAL_DWORD120, 0x5F535542) != 0) {
            SCNF_RequestIconNotification(0, 0, 0, 2);
            script_11840();
          } else {
            Script_sprintf("MFSY: sinesinesinesinesinesinesinesine\012");
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        Script_ryoControl(RYO_RestoreControl);
        Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
        Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
        Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
        Script_shenClearFreeState(1);
      }
      if (Script_CycleManagerUtils(CYCLE_CMD_GET_MOBJ_SPECIAL_DWORD120, 0x5F535542) == 0) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    SCNF_RequestIconNotification(0, 0, 0, 2);
    Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_3F974, 1, data_3F982, 2, data_3F987)))
static int32_t script_117DA(void) {
  script_EAA8("scene/02/MFSY", "YBUS");
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, 4, "a1_busno.snd", 0x10000);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadSND(4, 0, 0, 0);
  return (task_370 = 0) & 8;
}

__attribute__((scn_string_bindings(0, data_3F9A0, 2, data_3F9B2, 4, data_3F9C4, 5, data_3F9C9)))
static void script_11840(void) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int16_t local_EA;
  int8_t local_E9;
  int8_t local_E8;
  local_EA = 0;
  (void)((local_E8 = 0) & 4);
  if (Script_CycleManagerUtils(CYCLE_CMD_GET_MOBJ_SPECIAL_DWORD120, 0x5F535542) < 0) {
    local_F4.x = -127.823997f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 141.0f;
    local_EC = 0x8000;
    (void)((local_E8 = -1) & 1);
  }
  if (!((&task_23C)->x <= -127.87f)) {
    local_F4.x = -128.823196f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 141.382401f;
    local_EC = 23665;
    (void)((local_E8 = -1) & 0x20);
  }
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_SET_MOBJ_FIELD_108_BOOL, 0x5F535542, 1);
  (void)((task_370 = -1) & 0x20);
  Script_scenCreateTask(script_117DA, 0);
  if (local_E8 & 1) {
    Script_charaSubmitMoveRequestEx(CHARA_AKIR, &local_F4, local_EC, MN_AKI_AKI_TATI_IWA_L, 0, 1.0f);
  } else {
    Script_charaSubmitMoveRequestEx(CHARA_AKIR, &data_3F994, 0x8000, MN_AKI_AKI_TATI_IWA_L, 0, 0.8f);
  }
  for (;;) {
    if ((float)Script_CycleManagerUtils(CYCLE_CMD_GET_MOBJ_PRIMARY_DOOR_OPEN_PERCENT, 0x5F535542) >= 90.0f) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  if (local_E8 & 1) {
    while (Script_CycleManagerUtils(CYCLE_CMD_GET_MOBJ_SPECIAL_DWORD120, 0x5F535542) < 0) {
      if (local_EA == 900) {
        break;
      }
      local_EA = local_EA + 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  while (task_370 & 0x40) {
    SCNF_WaitFrames_0000(1);
  }
  local_F0 = script_E948(SHEN_NAME('BUS_'));
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  if (local_E8 & 1) {
    Script_charaSubmitMoveRequestEx(CHARA_AKIR, &data_3F994, 0x8000, MN_AKI_AKI_TATI_IWA_L, 0, 0.8f);
  }
  while (Script_IsMoveTargetRequestActive(CHARA_AKIR) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "YBUS");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.5f)) {
    local_E9 = 4;
    Script_AuthorUtils(0, local_E9, "seqdata7.bin", "YBUS");
  } else {
    local_E9 = 5;
    Script_AuthorUtils(0, local_E9, "seqdataA.bin", "YBUS");
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  script_11E0B(local_E9);
  script_11DB1(local_E9);
  Script_executeAicaCommand(2720, 1, 30);
  Script_executeAicaCommand(2720, 2, 30);
  Script_executeAicaCommand(2720, 3, 30);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_11E59(local_E9);
  Script_AuthorUtils(AUTH_FREE, local_E9);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "YBUS");
  script_EA10(SHEN_NAME('BUS_'), local_F0);
  Script_PlayerStatsUtils(PS_SetStatU8_1, Script_PlayerStatsUtils(PS_GetStatU8_1) + 1);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 880, 0);
  script_11B73();
  SCNF_WaitFrames_0000(1);
  script_EBF3(2, SHEN_NAME('D000'), 0);
  return;
}

static void script_11B73(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  Script_ReturnTimeDate(&local_F4);
  (&local_F4)->minutes = (&local_F4)->minutes + 20;
  if ((&local_F4)->minutes >= 60) {
    (&local_F4)->hours = (&local_F4)->hours + 1;
    (&local_F4)->minutes = (&local_F4)->minutes % 60;
  }
  Script_SetNewDateTime(&local_F4);
  return;
}

__attribute__((scn_string_bindings(0, data_3F9CE, 1, data_3F9D7, 2, data_3F9E0, 3, data_3F9E9)))
static void script_11BBF(void) {
  Script_AddCharacterHandler(0, "DR02_021", 0x31647375);
  Script_AddCharacterHandler(0, "DR02_021", 0x32647375);
  Script_AddCharacterHandler(0, "DR02_021", 0x33647375);
  Script_AddCharacterHandler(0, "DR02_021", 0x34647375);
  Script_charaDisplay(0x31647375, CDM_Show);
  Script_charaDisplay(0x32647375, CDM_Show);
  Script_charaDisplay(0x33647375, CDM_Show);
  Script_charaDisplay(0x34647375, CDM_Show);
  script_A671(0x31647375, 26.107008f, 0.0f, 101.283951f);
  script_A671(0x32647375, 34.207653f, 0.0f, 93.183311f);
  script_A671(0x33647375, 18.006388f, 0.0f, 109.384659f);
  script_A671(0x34647375, 69.641426f, 0.0f, 37.471775f);
  script_A699(0x31647375, 0.0f, 45.0f, 0.0f);
  script_A699(0x32647375, 0.0f, 45.0f, 0.0f);
  script_A699(0x33647375, 0.0f, 45.0f, 0.0f);
  script_A699(0x34647375, 0.0f, -135.0f, 0.0f);
  return;
}

static void script_11CE2(void) {
  Script_SetMapControlTaskFlag(0x31647375, 1);
  Script_SetMapControlTaskFlag(0x32647375, 1);
  Script_SetMapControlTaskFlag(0x33647375, 1);
  Script_SetMapControlTaskFlag(0x34647375, 1);
  return;
}

static void script_11D11(void) {
  if (-1) {
    Script_charaDisplay(SHEN_NAME('OTO8'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('OTO8'), CDM_Show);
      goto L011D3B;
    }
  }
  L011D3B:;
  Script_scenToggleVisibleFor(SHEN_NAME('OTO8'), 0);
  if (0) {
    Script_charaDisplay(SHEN_NAME('OTC8'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('OTC8'), CDM_Show);
      goto L011D6F;
    }
  }
  L011D6F:;
  Script_scenToggleVisibleFor(SHEN_NAME('OTC8'), 1);
  Script_SetupLightFromTable(0);
  if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 2) | Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 3)) {
    Script_mapmSetRenderFlag(14, 1);
  } else {
    Script_mapmSetRenderFlag(12, 1);
  }
  return;
}

static void script_11DB1(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  if ((local_FC == 0) | (local_FC == 1)) {
    script_128CE(arg0);
  } else if ((local_FC == 2) | (local_FC == 3)) {
    script_11FE1(arg0);
  } else {
    if ((local_FC == 4) | (local_FC == 5)) {
      script_11E97(arg0);
      goto L011E0A;
    }
  }
  L011E0A:;
  return;
}

static void script_11E0B(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  if (local_FC == 0) {
    script_11BBF();
  } else if (local_FC == 1) {
    script_11BBF();
    script_11D11();
  } else {
    if ((local_FC == 2) | (local_FC == 3) | (local_FC == 4) | (local_FC == 5)) goto L011E58;
  }
  L011E58:;
  return;
}

static void script_11E59(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  if ((local_FC == 0) | (local_FC == 1)) {
    script_11CE2();
  } else {
    if ((local_FC == 2) | (local_FC == 3) | (local_FC == 4) | (local_FC == 5)) goto L011E96;
  }
  L011E96:;
  return;
}

static void script_11E97(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  script_A2A7();
  script_12113();
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 153, 0, &data_3F9F4, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 154, 0, &data_3FA00, CHARA_PART_Set);
  local_FC = 0;
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 60.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 60.0f);
  Script_shenSeqUtils(arg0);
  while (Script_shenSeqUtils(-1)) {
    local_F8 = local_FC;
    switch (local_F8) {
      case 0:
        Script_executeAicaCommand(0x1B04A9, 0, 0);
        break;
      case 58:
        Script_scenCreateTask(script_1281C, 0);
        break;
      case 146:
        Script_charaPosRotUtils(SHEN_NAME('BUS_'), 153, 0, &data_3FA0C, CHARA_PART_Set);
        Script_charaPosRotUtils(SHEN_NAME('BUS_'), 154, 0, &data_3FA0C, CHARA_PART_Set);
        Script_scenToggleActiveFor(CHARA_AKIR, 0);
        break;
      case 147:
        Script_executeAicaCommand(0x1C04A9, 0, 0);
        Script_scenCreateTask(script_126F5, 2, SHEN_NAME('BUS_'), 0);
        break;
      case 272:
        goto L011FCA;
        break;
    }
    local_FC = local_FC + 1;
    script_1282A();
    SCNF_WaitFrames_0000(1);
  }
  L011FCA:;
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 120.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 70.0f);
  return;
}

static void script_11FE1(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  script_A2A7();
  script_12113();
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 157, 0, &data_3FA18, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 158, 0, &data_3FA24, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 155, 0, &data_3FA30, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 156, 0, &data_3FA3C, CHARA_PART_Set);
  local_FC = 0;
  Script_executeAicaCommand(0x1B04A9, 0, 0);
  Script_shenSeqUtils(arg0);
  while (Script_shenSeqUtils(-1)) {
    local_F8 = local_FC;
    switch (local_F8) {
      case 145:
        Script_scenCreateTask(script_12343, 1, 2);
        break;
      case 200:
        Script_executeAicaCommand(0x1C04A9, 0, 0);
        Script_scenCreateTask(script_126F5, 2, SHEN_NAME('BUS_'), 0);
        break;
      case 250:
        goto L0120DC;
        break;
    }
    script_1282A();
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  L0120DC:;
  Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_shenSeqUtils(-6);
  script_12256();
  return;
}

static void script_12113(void) {
  Vector3 local_F4;
  int32_t unused_F0;
  GameDateTime local_E8;
  Script_WeatherUtils(WeatherScript_SetWccpClipSet0Enabled, 0);
  Script_scenCreateTask(script_1282A, 0);
  script_A614(CHARA_AKIR);
  script_A2BF(CHARA_AKIR);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  script_121CF();
  Script_SetMaxNPCsNear(32);
  Script_mapmSetCamFlags(0, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 100.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 100.0f);
  Script_ReturnTimeDate(&local_E8);
  local_F4.x = 0.0f;
  (&local_F4)->y = -100.0f;
  (&local_F4)->z = 0.0f;
  if ((&local_E8)->hours * 100 + (&local_E8)->minutes >= 1730) {
    (&local_F4)->y = 0.0f;
  }
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 196, &local_F4, 0, CHARA_PART_Set);
  return;
}

static void script_121CF(void) {
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 153, 0, &data_3FA48, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 154, 0, &data_3FA48, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 155, 0, &data_3FA48, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 156, 0, &data_3FA48, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 157, 0, &data_3FA48, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 158, 0, &data_3FA48, CHARA_PART_Set);
  return;
}

static void script_12256(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  script_A34C(CHARA_AKIR);
  Script_ReturnTimeDate(&local_F4);
  if ((&local_F4)->hours < 19) {
    Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 4);
    Script_SetCharaFlags(CHARA_AKIR, 4);
  } else {
    Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 2);
    Script_SetCharaFlags(CHARA_AKIR, 0);
  }
  Script_mapmSetCamFlags(0, 1);
  Script_SetMaxNPCsNear(5);
  if (0) {
    Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_AKIR, CDM_Show);
      goto L0122EC;
    }
  }
  L0122EC:;
  Script_scenToggleVisibleFor(CHARA_AKIR, 1);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_3FA54);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_3FA60, 0);
  return;
}

static int32_t script_12343(int32_t arg0) {
  int8_t local_FF;
  Vector3i local_F0;
  Vector3i local_E4;
  Vector3i local_D8;
  Vector3i local_CC;
  Vector3i local_C0;
  Vector3i local_B4;
  int32_t local_B0;
  local_F0.x = 0;
  (&local_F0)->y = 0;
  (&local_F0)->z = 0;
  local_E4.x = 0;
  (&local_E4)->y = 0;
  (&local_E4)->z = 0;
  local_D8.x = 0;
  (&local_D8)->y = 0;
  (&local_D8)->z = 0;
  local_CC.x = 0;
  (&local_CC)->y = 0;
  (&local_CC)->z = 0;
  local_C0.x = 0;
  (&local_C0)->y = 0;
  (&local_C0)->z = 0;
  local_B4.x = 0;
  (&local_B4)->y = 0;
  (&local_B4)->z = 0;
  switch (arg0) {
    case 0:
      (&local_F0)->y = 14654;
      (&local_E4)->y = -30474;
      break;
    case 2:
      (&local_D8)->y = 14654;
      (&local_CC)->y = -30474;
      (&local_C0)->y = -14654;
      (&local_B4)->y = 30474;
      break;
  }
  local_FF = 0;
  while (local_FF <= 30) {
    if (Script_shenSeqUtils(-1) == 0) {
      break;
    }
    local_B0 = arg0;
    switch (local_B0) {
      case 0:
        (&local_F0)->y = (&local_F0)->y - 455;
        (&local_E4)->y = (&local_E4)->y + 983;
        break;
      case 1:
        (&local_F0)->y = (&local_F0)->y + 455;
        (&local_E4)->y = (&local_E4)->y - 983;
        break;
      case 2:
        (&local_D8)->y = (&local_D8)->y - 455;
        (&local_CC)->y = (&local_CC)->y + 983;
        (&local_C0)->y = (&local_C0)->y + 455;
        (&local_B4)->y = (&local_B4)->y - 983;
        break;
      case 3:
        (&local_D8)->y = (&local_D8)->y + 455;
        (&local_CC)->y = (&local_CC)->y - 983;
        (&local_C0)->y = (&local_C0)->y - 455;
        (&local_B4)->y = (&local_B4)->y + 983;
        break;
    }
    Script_charaPosRotUtils(SHEN_NAME('BUS_'), 153, 0, &local_F0, CHARA_PART_Set);
    Script_charaPosRotUtils(SHEN_NAME('BUS_'), 154, 0, &local_E4, CHARA_PART_Set);
    Script_charaPosRotUtils(SHEN_NAME('BUS_'), 157, 0, &local_D8, CHARA_PART_Set);
    Script_charaPosRotUtils(SHEN_NAME('BUS_'), 158, 0, &local_CC, CHARA_PART_Set);
    Script_charaPosRotUtils(SHEN_NAME('BUS_'), 155, 0, &local_C0, CHARA_PART_Set);
    Script_charaPosRotUtils(SHEN_NAME('BUS_'), 156, 0, &local_B4, CHARA_PART_Set);
    local_FF = local_FF + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_125D2(int32_t arg0, int32_t arg1) {
  Vector3i local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  int32_t local_E4;
  local_E4 = arg1;
  if (local_E4 == 3) {
    local_F0 = 23;
    local_EC = 18;
  } else {
    if ((local_E4 == 1) | (local_E4 == 2)) {
      local_F0 = 163;
      local_EC = 164;
      goto L012612;
    }
  }
  L012612:;
  local_E8 = 0;
  while (Script_shenSeqUtils(-1)) {
    if (local_E8 <= 24) {
      local_F4.x = 0;
      (&local_F4)->y = 182;
      (&local_F4)->z = 0;
      Script_charaPosRotUtils(arg0, local_EC, 0, &local_F4, CHARA_PART_Add);
      Script_charaPosRotUtils(arg0, local_F0, 0, &local_F4, CHARA_PART_Add);
    } else if (local_E8 <= 48) {
      local_F4.x = 0;
      (&local_F4)->y = -182;
      (&local_F4)->z = 0;
      Script_charaPosRotUtils(arg0, local_EC, 0, &local_F4, CHARA_PART_Add);
      Script_charaPosRotUtils(arg0, local_F0, 0, &local_F4, CHARA_PART_Add);
    } else {
      local_F4.x = 0;
      (&local_F4)->y = 0;
      (&local_F4)->z = 0;
      Script_charaPosRotUtils(arg0, local_EC, 0, &local_F4, CHARA_PART_Set);
      Script_charaPosRotUtils(arg0, local_F0, 0, &local_F4, CHARA_PART_Set);
      break;
    }
    local_E8 = local_E8 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_126F5(int32_t arg0, int32_t arg1) {
  Vector3i local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  int32_t local_D8;
  (&local_F4)->y = 0;
  (&local_F4)->z = 0;
  (&local_E8)->y = 0;
  (&local_E8)->z = 0;
  local_D8 = arg1;
  switch (local_D8) {
    case 0:
      local_E4 = 163;
      local_E0 = 173;
      local_DC = 182;
      local_F4.x = 2730;
      local_E8.x = -2730;
      break;
    case 1:
      local_E4 = 163;
      local_E0 = 164;
      local_DC = 165;
      local_F4.x = 2730;
      local_E8.x = 2730;
      break;
    case 2:
      local_E4 = 163;
      local_E0 = 164;
      local_DC = 165;
      local_F4.x = -2730;
      local_E8.x = -2730;
      break;
    case 3:
      local_E4 = 8;
      local_E0 = 18;
      local_DC = 23;
      local_F4.x = -910;
      local_E8.x = -910;
      break;
  }
  while (Script_shenSeqUtils(-1)) {
    Script_charaPosRotUtils(arg0, local_E4, 0, &local_F4, CHARA_PART_Add);
    Script_charaPosRotUtils(arg0, local_E0, 0, &local_F4, CHARA_PART_Add);
    Script_charaPosRotUtils(arg0, local_DC, 0, &local_E8, CHARA_PART_Add);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_1281C(void) {
  SCNF_SetCashValue(-160, 1);
  end:;
}

static int32_t script_1282A(void) {
  Vector3 local_F4;
  float local_F0;
  float local_EC;
  float local_E8;
  float local_E4;
  float local_E0;
  float local_DC;
  float local_D8;
  float local_D4;
  Script_EVT_GetPosition(SHEN_NAME('BUS_'), -1, &local_F4, 0);
  local_F0 = (&local_F4)->x - 4.0f;
  local_EC = (&local_F4)->z - 4.5f;
  local_E8 = (&local_F4)->x + 4.0f;
  local_E4 = (&local_F4)->z - 4.5f;
  local_E0 = (&local_F4)->x + 4.0f;
  local_DC = (&local_F4)->z + 4.5f;
  local_D8 = (&local_F4)->x - 4.0f;
  local_D4 = (&local_F4)->z + 4.5f;
  Script_WeatherUtils(WeatherScript_ApplyWccpClipSet1, local_F0, local_EC, local_E8, local_E4, local_E0, local_DC, local_D8, local_D4, 0.0f, 3.5f);
  end:;
}

static void script_128CE(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  Vector3 local_E0;
  Vector3i local_D4;
  int32_t local_D0;
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 115);
  Script_SetShadowRange(120.0f);
  script_A2A7();
  script_13428(arg0);
  local_E0.x = 0.0f;
  (&local_E0)->y = 0.0f;
  (&local_E0)->z = 0.0f;
  local_D4.x = 0;
  (&local_D4)->y = 0;
  (&local_D4)->z = 0;
  Script_fixoAttachEx(SHEN_NAME('FON3'), SHEN_NAME('FOK1'), 0, &local_E0, &local_D4);
  Script_fixoAttachEx(SHEN_NAME('FON7'), SHEN_NAME('FOK2'), 0, &local_E0, &local_D4);
  Script_fixoAttachEx(SHEN_NAME('FON8'), SHEN_NAME('FOK3'), 0, &local_E0, &local_D4);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 1);
  Script_scenToggleActiveFor(SHEN_NAME('BIR1'), 0);
  Script_scenToggleActiveFor(SHEN_NAME('BIR2'), 0);
  Script_scenToggleActiveFor(SHEN_NAME('BIR3'), 0);
  Script_scenToggleActiveFor(SHEN_NAME('BIR4'), 0);
  Script_scenToggleActiveFor(SHEN_NAME('BIR5'), 0);
  Script_scenToggleActiveFor(SHEN_NAME('BIR6'), 0);
  Script_scenToggleActiveFor(SHEN_NAME('BIR7'), 0);
  Script_scenToggleActiveFor(CHARA_AKIR, 0);
  Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(SHEN_NAME('EXT1'), SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(SHEN_NAME('EXT2'), SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(SHEN_NAME('EXT3'), SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(SHEN_NAME('EXT4'), SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetCharaFlags(SHEN_NAME('FOK1'), 2);
  Script_SetCharaFlags(SHEN_NAME('FOK2'), 2);
  Script_SetCharaFlags(SHEN_NAME('FOK3'), 2);
  Script_SetCharaFlags(SHEN_NAME('YEL1'), 2);
  Script_SetCharaFlags(SHEN_NAME('YEL2'), 2);
  Script_SetCharaFlags(SHEN_NAME('YEL3'), 2);
  Script_SetCharaFlags(SHEN_NAME('BLU1'), 2);
  Script_SetCharaFlags(SHEN_NAME('BLU2'), 2);
  Script_SetCharaFlags(SHEN_NAME('WAG1'), 2);
  Script_SetCharaFlags(SHEN_NAME('WAG2'), 2);
  local_EC.x = 1.0f;
  (&local_EC)->y = 1.0f;
  (&local_EC)->z = 1.41f;
  Script_SetCharaFlags(SHEN_NAME('FOK1'), 100, 1, &local_EC);
  Script_SetCharaFlags(SHEN_NAME('FOK2'), 100, 1, &local_EC);
  Script_SetCharaFlags(SHEN_NAME('FOK3'), 100, 1, &local_EC);
  local_EC.x = 1.0f;
  (&local_EC)->y = 1.0f;
  (&local_EC)->z = 3.28f;
  Script_SetCharaFlags(SHEN_NAME('WAG1'), 100, 1, &local_EC);
  Script_SetCharaFlags(SHEN_NAME('WAG2'), 100, 1, &local_EC);
  local_EC.x = 1.82f;
  (&local_EC)->y = 1.0f;
  (&local_EC)->z = 5.0f;
  Script_SetCharaFlags(SHEN_NAME('YEL1'), 100, 1, &local_EC);
  Script_SetCharaFlags(SHEN_NAME('YEL2'), 100, 1, &local_EC);
  Script_SetCharaFlags(SHEN_NAME('YEL3'), 100, 1, &local_EC);
  Script_SetCharaFlags(SHEN_NAME('BLU1'), 100, 1, &local_EC);
  Script_SetCharaFlags(SHEN_NAME('BLU2'), 100, 1, &local_EC);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 120.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 60.0f);
  Script_scenCreateTask(script_126F5, 2, SHEN_NAME('FOK2'), 3);
  Script_scenCreateTask(script_126F5, 2, SHEN_NAME('YEL1'), 2);
  Script_scenCreateTask(script_126F5, 2, SHEN_NAME('BLU1'), 2);
  local_F8 = Script_scenCreateTask(script_126F5, 2, SHEN_NAME('BUS_'), 0);
  if (arg0 == 0) {
    Script_scenCreateTask(script_126F5, 2, SHEN_NAME('FOK1'), 3);
    Script_scenCreateTask(script_126F5, 2, SHEN_NAME('FOK3'), 3);
    Script_scenCreateTask(script_126F5, 2, SHEN_NAME('YEL2'), 2);
    Script_scenCreateTask(script_126F5, 2, SHEN_NAME('YEL3'), 2);
    Script_scenCreateTask(script_126F5, 2, SHEN_NAME('WAG2'), 1);
    Script_scenCreateTask(script_13560, 0);
    Script_charaDisplay(SHEN_NAME('BIR1'), CDM_Show);
    Script_charaDisplay(SHEN_NAME('BIR2'), CDM_Hide);
    Script_charaDisplay(SHEN_NAME('BIR3'), CDM_Show);
    Script_charaDisplay(SHEN_NAME('BIR4'), CDM_Show);
    Script_charaDisplay(SHEN_NAME('BIR5'), CDM_Hide);
    Script_charaDisplay(SHEN_NAME('BIR6'), CDM_Hide);
    Script_charaDisplay(SHEN_NAME('BIR7'), CDM_Hide);
    Script_charaDisplay(SHEN_NAME('EXT3'), CDM_Show);
    Script_charaDisplay(SHEN_NAME('EXT4'), CDM_Show);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BIR1'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('BIR1'), CDM_Show);
        goto L012D07;
      }
    }
    L012D07:;
    Script_scenToggleVisibleFor(SHEN_NAME('BIR1'), 0);
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BIR2'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('BIR2'), CDM_Show);
        goto L012D3B;
      }
    }
    L012D3B:;
    Script_scenToggleVisibleFor(SHEN_NAME('BIR2'), 0);
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BIR3'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('BIR3'), CDM_Show);
        goto L012D6F;
      }
    }
    L012D6F:;
    Script_scenToggleVisibleFor(SHEN_NAME('BIR3'), 0);
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BIR4'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('BIR4'), CDM_Show);
        goto L012DA3;
      }
    }
    L012DA3:;
    Script_scenToggleVisibleFor(SHEN_NAME('BIR4'), 0);
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BIR5'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('BIR5'), CDM_Show);
        goto L012DD7;
      }
    }
    L012DD7:;
    Script_scenToggleVisibleFor(SHEN_NAME('BIR5'), 0);
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BIR6'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('BIR6'), CDM_Show);
        goto L012E0B;
      }
    }
    L012E0B:;
    Script_scenToggleVisibleFor(SHEN_NAME('BIR6'), 0);
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BIR7'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('BIR7'), CDM_Show);
        goto L012E3F;
      }
    }
    L012E3F:;
    Script_scenToggleVisibleFor(SHEN_NAME('BIR7'), 0);
    if (-1) {
      Script_charaDisplay(SHEN_NAME('EXT3'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('EXT3'), CDM_Show);
        goto L012E73;
      }
    }
    L012E73:;
    Script_scenToggleVisibleFor(SHEN_NAME('EXT3'), 0);
    if (-1) {
      Script_charaDisplay(SHEN_NAME('EXT4'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('EXT4'), CDM_Show);
        goto L012EA7;
      }
    }
    L012EA7:;
    Script_scenToggleVisibleFor(SHEN_NAME('EXT4'), 0);
  }
  Script_charaDisplay(SHEN_NAME('EXT1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('EXT2'), CDM_Show);
  if (-1) {
    Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_AKIR, CDM_Show);
      goto L012EF1;
    }
  }
  L012EF1:;
  Script_scenToggleVisibleFor(CHARA_AKIR, 0);
  local_FC = 0;
  Script_mapmSetCamFlags(2, 5, 0);
  Script_shenSeqUtils(arg0);
  while (Script_shenSeqUtils(-1)) {
    local_D0 = local_FC;
    switch (local_D0) {
      case 142:
        if (arg0 == 0) {
          Script_charaDisplay(SHEN_NAME('BIR2'), CDM_Show);
          Script_charaDisplay(SHEN_NAME('BIR5'), CDM_Show);
          Script_charaDisplay(SHEN_NAME('BIR6'), CDM_Show);
          Script_charaDisplay(SHEN_NAME('BIR7'), CDM_Show);
        }
        break;
      case 330:
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 106.0f);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 100.0f);
        Script_mapmSetCamFlags(2, 5, 1);
        Script_mapmSetCamFlags(0, 1);
        break;
      case 380:
        if (arg0 == 0) {
          Script_scenCreateTask(script_125D2, 2, SHEN_NAME('FOK2'), 3);
        }
        break;
      case 850:
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 225.0f);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 120.0f);
        break;
      case 900:
        if (arg0 == 0) {
          Script_scenCreateTask(script_125D2, 2, SHEN_NAME('BLU1'), 1);
        }
        break;
      case 1000:
        if (arg0 == 0) {
          Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 140.0f);
        }
        if (-1) {
          Script_charaDisplay(SHEN_NAME('EXT1'), CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(SHEN_NAME('EXT1'), CDM_Show);
            goto L013055;
          }
        }
        L013055:;
        Script_scenToggleVisibleFor(SHEN_NAME('EXT1'), 0);
        if (-1) {
          Script_charaDisplay(SHEN_NAME('EXT2'), CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(SHEN_NAME('EXT2'), CDM_Show);
            goto L013089;
          }
        }
        L013089:;
        Script_scenToggleVisibleFor(SHEN_NAME('EXT2'), 0);
        if (-1) {
          Script_charaDisplay(SHEN_NAME('EXT3'), CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(SHEN_NAME('EXT3'), CDM_Show);
            goto L0130BD;
          }
        }
        L0130BD:;
        Script_scenToggleVisibleFor(SHEN_NAME('EXT3'), 0);
        if (-1) {
          Script_charaDisplay(SHEN_NAME('EXT4'), CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(SHEN_NAME('EXT4'), CDM_Show);
            goto L0130F1;
          }
        }
        L0130F1:;
        Script_scenToggleVisibleFor(SHEN_NAME('EXT4'), 0);
        break;
      case 1090:
        if (arg0 == 0) {
          Script_scenCreateTask(script_125D2, 2, SHEN_NAME('BLU1'), 1);
        }
        break;
      case 1100:
        if (arg0 == 0) {
          Script_scenCreateTask(script_125D2, 2, SHEN_NAME('YEL2'), 1);
        }
        break;
      case 1200:
        if (0) {
          Script_charaDisplay(CHARA_AKIR, CDM_Hide);
        } else {
          if (-1) {
            Script_charaDisplay(CHARA_AKIR, CDM_Show);
            goto L013182;
          }
        }
        L013182:;
        Script_scenToggleVisibleFor(CHARA_AKIR, 1);
        Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
        Script_SetCharaRefBoardPos(CHARA_AKIR, &data_3FA6C);
        break;
      case 1323:
        Script_executeAicaCommand(0x1B04A9, 0, 0);
        break;
      case 1420:
        Script_srchControl((void *)local_F8);
        break;
      case 1435:
        Script_scenCreateTask(script_12343, 1, 3);
        break;
      case 1528:
        Script_scenToggleActiveFor(CHARA_AKIR, 1);
        break;
      case 1550:
        Script_scenCreateTask(script_12343, 1, 2);
        break;
      case 1599:
        Script_executeAicaCommand(0x1C04A9, 0, 0);
        break;
      case 1650:
        Script_scenCreateTask(script_126F5, 2, SHEN_NAME('BUS_'), 0);
        break;
      case 1670:
        goto L013394;
        break;
    }
    if (local_FC <= 647) {
      Script_WeatherUtils(WeatherScript_ApplyWccpClipSet1, 39.74f, 96.5f, 52.4f, 100.2f, 23.8f, 125.17f, 18.25f, 119.7f, 0.0f, 6.9f);
    } else if (local_FC <= 791) {
      Script_WeatherUtils(WeatherScript_ApplyWccpClipSet1, 9.35f, 119.52f, -15.7f, 145.4f, -22.9f, 138.86f, 3.3f, 114.3f, 0.0f, 6.9f);
    } else if (local_FC <= 1000) {
      Script_WeatherUtils(WeatherScript_ApplyWccpClipSet1, 14.08f, 124.3f, 29.9f, 140.1f, 14.1f, 159.0f, -2.29f, 143.68f, 0.0f, 9.4f);
    } else if (local_FC <= 1387) {
      Script_WeatherUtils(WeatherScript_ApplyWccpClipSet1, -134.5f, 141.288193f, -127.6f, 140.6f, -126.89f, 142.8f, -133.7f, 142.0f, 0.0f, 3.5f);
    } else {
      script_1282A();
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  L013394:;
  Script_executeAicaCommand(2720, 2, 60);
  if (Script_shenSeqUtils(-7)) {
    Script_filmCheckOrSetTransitionUtil(1, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  } else {
    Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_shenSeqUtils(-6);
  Script_fixoClearAttachment(SHEN_NAME('FON3'));
  Script_fixoClearAttachment(SHEN_NAME('FON7'));
  Script_fixoClearAttachment(SHEN_NAME('FON8'));
  script_12256();
  return;
}

static void script_13428(int32_t arg0) {
  Vector3 local_F4;
  int32_t unused_F0;
  GameDateTime local_E8;
  script_12113();
  Script_mapmSetCamFlags(0, 1);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 200.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 150.0f);
  Script_ReturnTimeDate(&local_E8);
  local_F4.x = 0.0f;
  (&local_F4)->y = -100.0f;
  (&local_F4)->z = 0.0f;
  Script_charaPosRotUtils(SHEN_NAME('BLU2'), 196, &local_F4, 0, CHARA_PART_Set);
  if (arg0 == 0) {
    if ((&local_E8)->hours * 100 + (&local_E8)->minutes >= 1730) {
      (&local_F4)->y = 0.0f;
    }
    Script_charaPosRotUtils(SHEN_NAME('BUS_'), 196, &local_F4, 0, CHARA_PART_Set);
    Script_charaPosRotUtils(SHEN_NAME('YEL1'), 196, &local_F4, 0, CHARA_PART_Set);
    Script_charaPosRotUtils(SHEN_NAME('YEL2'), 196, &local_F4, 0, CHARA_PART_Set);
    Script_charaPosRotUtils(SHEN_NAME('YEL3'), 196, &local_F4, 0, CHARA_PART_Set);
    Script_charaPosRotUtils(SHEN_NAME('BLU1'), 196, &local_F4, 0, CHARA_PART_Set);
    Script_charaPosRotUtils(SHEN_NAME('BUS_'), 196, &local_F4, 0, CHARA_PART_Set);
    Script_charaPosRotUtils(SHEN_NAME('WAG1'), 196, &local_F4, 0, CHARA_PART_Set);
    Script_charaPosRotUtils(SHEN_NAME('WAG2'), 196, &local_F4, 0, CHARA_PART_Set);
  } else {
    script_A2BF(SHEN_NAME('EXT1'));
    script_A2BF(SHEN_NAME('EXT2'));
  }
  return;
}

static int32_t script_13560(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_FC = 0;
  local_F8 = 0;
  local_F4 = 12;
  Script_charaDisplay(SHEN_NAME('BIR1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('BIR2'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('BIR3'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('BIR4'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('BIR5'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('BIR6'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('BIR7'), CDM_Show);
  while (Script_shenSeqUtils(-1)) {
    Script_SetImageModuleTaskDataFlag(SHEN_NAME('BIR1'), local_F8);
    Script_SetImageModuleTaskDataFlag(SHEN_NAME('BIR3'), local_F8);
    Script_SetImageModuleTaskDataFlag(SHEN_NAME('BIR4'), local_F8);
    if (local_FC < 185) {
      Script_SetImageModuleTaskDataFlag(SHEN_NAME('BIR2'), local_F4);
      Script_SetImageModuleTaskDataFlag(SHEN_NAME('BIR5'), local_F4);
      Script_SetImageModuleTaskDataFlag(SHEN_NAME('BIR6'), local_F4);
      Script_SetImageModuleTaskDataFlag(SHEN_NAME('BIR7'), local_F4);
    } else {
      Script_SetImageModuleTaskDataFlag(SHEN_NAME('BIR2'), local_F8);
      Script_SetImageModuleTaskDataFlag(SHEN_NAME('BIR5'), local_F8);
      Script_SetImageModuleTaskDataFlag(SHEN_NAME('BIR6'), local_F8);
      Script_SetImageModuleTaskDataFlag(SHEN_NAME('BIR7'), local_F8);
    }
    local_F8 = local_FC % 12;
    local_F4 = local_FC % 8 + 12;
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_13685(int32_t arg0, int32_t arg1) {
  task_378 = arg0;
  script_13CAA(arg1);
  task_38C = 9999;
  task_390 = 9999;
  task_394 = 0;
  task_398 = 0;
  task_3A1 = 0;
  task_3A4 = 0;
  script_139BD();
  task_3A8 = 0;
  task_37C = 0x52554B59;
  task_380 = 0x4C554B59;
  return;
}

static void script_136E4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  script_13685(arg0, arg1);
  task_39C = Script_scenCreateTask(script_1370B, 4, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_1370B(float arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  Vector3i local_E8;
  Vector3 local_DC;
  float local_D8;
  int8_t local_D7;
  local_F4.x = arg0;
  (&local_F4)->y = arg1;
  (&local_F4)->z = arg2;
  local_D8 = arg3 + 180.0f;
  local_D7 = 0;
  task_3A2 = 0;
  task_3A3 = 0;
  for (;;) {
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_DC, ENTITY_XFORM_WORLD_SPACE);
    Script_mtwkGetSetRotation(CHARA_AKIR, &local_E8, 0x42000000);
    if (Script_CheckTargetWithinAngleRange(&local_F4, 2.0f, (int32_t)(local_D8 / 360.0f * (float)0x10000), 12743, &local_DC) & Script_CheckTargetWithinAngleRange(&local_DC, 2.0f, (&local_E8)->y, 12743, &local_F4)) {
      if (Script_IsPlayerAllowed(0) & -(task_3A1 == 0)) {
        if (local_D7 == 0) {
          Script_uiUtils(0, 9, 0);
          Script_sicnUtils(0, 5, 1);
          local_D7 = 1;
        }
        if (Script_GetControllerInput(1) == 0x400) {
          if ((local_D7 == 1) & (MMTK_ScriptMemoToggle(2, 0) == 0)) {
            Script_uiUtils(0, 0, 0);
            Script_sicnUtils(1, 0, 0);
            local_D7 = 0;
          }
          task_3A2 = 1;
          script_13E16();
          task_3A2 = 0;
        }
      } else {
        if ((local_D7 == 1) & (MMTK_ScriptMemoToggle(2, 0) == 0)) {
          Script_uiUtils(0, 0, 0);
          Script_sicnUtils(1, 0, 0);
          local_D7 = 0;
        }
      }
    } else {
      if ((local_D7 == 1) & (MMTK_ScriptMemoToggle(2, 0) == 0)) {
        Script_uiUtils(0, 0, 0);
        Script_sicnUtils(1, 0, 0);
        local_D7 = 0;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_138A9(void) {
  {
    int32_t local_FC;
    local_FC = 0;
    task_3A0 = 0;
    for (;;) {
      if ((Script_GetControllerInput(1) == 0x400) | (Script_GetControllerInput(1) == 0x200)) {
        task_3A0 = 1;
        Script_TELMUtils(TELM_CMD_RequestHangUp, task_378);
        break;
      }
      local_FC = local_FC + 1;
      if (local_FC == 300) {
        task_3A0 = 1;
        Script_TELMUtils(TELM_CMD_RequestHangUp, task_378);
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  if (task_398) {
    int32_t local_FC;
    local_FC = 0;
    for (;;) {
      if ((task_398 == 0) | (local_FC < -200)) {
        break;
      }
      local_FC = local_FC - 5;
      Script_SetSequenceAudioVolume(0, (void *)task_398, local_FC);
      SCNF_WaitFrames_0000(1);
    }
    if (task_398) {
      Script_StartOrFreeSEQ((void *)task_398);
    }
  }
  return Script_srchControl(0);
}

static void script_13970(const char *arg0, int32_t arg1) {
  task_3AC[arg1] = Script_TELMUtils(TELM_CMD_RegisterPhoneNumber, task_378, arg0);
  if (task_384 == arg1) {
    task_38C = task_3AC[arg1];
  }
  if (task_388 == arg1) {
    task_390 = task_3AC[arg1];
  }
  return;
}

static void script_139BD(void) {
  script_13970((const char *)&data_3FC44, 0);
  script_13970((const char *)&data_3FC48, 1);
  script_13970((const char *)&data_3FC4C, 2);
  script_13970((const char *)&data_3FC50, 7);
  script_13970("61564", 3);
  script_13970("37742", 4);
  script_13970("37749", 5);
  script_13970("22550", 6);
  script_13970("046861564", 8);
  script_13970("046837742", 9);
  script_13970("046837749", 10);
  script_13970("046822550", 11);
  script_13970("2?????", 12);
  script_13970("5?????", 13);
  script_13970("7?????", 14);
  script_13970("8?????", 15);
  script_13970("3?????", 16);
  script_13970("4?????", 17);
  script_13970("6?????", 18);
  script_13970("9?????", 19);
  script_13970("0?????????", 20);
  script_13970("1?????", 21);
  return;
}

static int32_t script_13AF4(int32_t arg0) {
  if (arg0 > 0) {
    if ((arg0 == task_38C) | (arg0 == task_390)) {
      Script_scenCreateTask(script_138A9, 0);
      for (;;) {
        Script_executeAicaCommand(394667, 0, 0);
        SCNF_WaitFrames_0000(15);
        Script_executeAicaCommand(460203, 0, 0);
        SCNF_WaitFrames_0000(15);
        if (task_3A0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      arg0 = Script_TELMUtils(TELM_CMD_GetActiveResult, task_378);
    }
  }
  return arg0;
}

static void script_13B6D(void) {
  if (task_3A1 == 0) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_TOKI_TEGS_CUTSCENE_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_UNLOCKED) == 0)) {
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 185, 1);
    }
    task_3A1 = script_13BCF();
    if (task_3A1 == 0) {
      task_3A1 = task_3A1 + 1;
      if (task_3A2) {
        task_3A3 = 1;
      }
    }
  }
  return;
}

static int8_t script_13BCF(void) {
  int32_t local_FC;
  int8_t local_FB;
  local_FC = Script_shenGetStats(STATS_CASH);
  local_FB = 0;
  if ((Script_StepUtils(GET_STEP_HIGHER, 0) == 17) & (Script_StepUtils(GET_STEP_LOWER, 0) <= 5)) {
    local_FB = -1;
  } else {
    if (Script_StepUtils(GET_STEP_HIGHER, 0) >= 11) {
      if (local_FC <= 169) {
        local_FB = -10;
      }
      goto L013C2E;
    }
  }
  L013C2E:;
  if (local_FC < 10) {
    local_FB = -15;
  }
  return local_FB;
}

__attribute__((scn_string_bindings(0, data_3FCDE, 4, data_3FD02)))
static void script_13C41(int32_t arg0) {
  int32_t local_FC;
  Script_LoadVoiceFile(1, "E1066");
  local_FC = arg0;
  if (local_FC == -1) {
    Script_ShowUIMessageFromID("E1066A012");
  } else if (local_FC == -10) {
    Script_ShowUIMessageFromID("E1066A013");
  } else {
    Script_ShowUIMessageFromID("E1066A006");
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadVoiceFile(0, "E1066");
  return;
}

static void script_13CAA(int32_t arg0) {
  int32_t local_FC;
  task_384 = arg0;
  local_FC = arg0;
  if (local_FC == 4) {
    task_388 = 9;
  } else {
    task_388 = 9999;
  }
  return;
}

static void script_13CD5(void) {
  Script_convSetCamInactiveFlag(0);
  if ((task_384 == 9998) | (task_384 == 9997)) {
    SCNF_SetCashValue(10, 1);
  }
  return;
}

static int32_t script_13CFC(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  Script_ReturnTimeDate(&local_F4);
  if ((&local_F4)->hours <= 4) {
    arg0 = -1;
  } else {
    if (((&local_F4)->hours >= 23) & ((&local_F4)->minutes >= 30)) {
      arg0 = -1;
      goto L013D3A;
    }
  }
  L013D3A:;
  return arg0;
}

static int8_t script_13D3E(int32_t arg0) {
  int8_t local_FF;
  int8_t local_FE;
  local_FF = 0;
  local_FE = 1;
  for (;;) {
    Script_executeAicaCommand(263595, 0, 0);
    {
      int8_t local_FD;
      local_FD = 0;
      for (;;) {
        if (Script_GetControllerInput(1) == 0x200) {
          local_FE = 0;
          break;
        }
        local_FD = local_FD + 1;
        if (local_FD > 45) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    }
    Script_executeAicaCommand(329131, 0, 0);
    if (local_FE == 0) {
      break;
    }
    {
      int8_t local_FD;
      local_FD = 0;
      for (;;) {
        if (Script_GetControllerInput(1) == 0x200) {
          local_FE = 0;
          break;
        }
        local_FD = local_FD + 1;
        if (local_FD > 45) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    }
    if (local_FE == 0) {
      break;
    }
    local_FF = local_FF + 1;
    if (local_FF == arg0) {
      break;
    }
  }
  return local_FE;
}

static void script_13E16(void) {
  script_13B6D();
  script_13E23();
  return;
}

static void script_13E23(void) {
  int32_t unused_F4[3];
  Vector4 local_E4;
  int32_t local_E0;
  int32_t local_DC;
  float local_D8;
  int32_t unused_D4;
  int8_t local_D3;
  uint8_t unused_D0[3];
  if (task_3A1 == 1) {
    Script_scenToggleActiveFor(CHARA_AKIR, 0);
    Script_ryoControl(PCCTL_UpdateRyoNavYaw, 0x52494B41);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
    Script_scenToggleVisibleFor(task_378, 1);
    if (0) {
      Script_charaDisplay(task_380, CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(task_380, CDM_Show);
        goto L013EA7;
      }
    }
    L013EA7:;
    Script_scenToggleVisibleFor(task_380, 1);
    if (0) {
      Script_charaDisplay(task_37C, CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(task_37C, CDM_Show);
        goto L013ED5;
      }
    }
    L013ED5:;
    Script_scenToggleVisibleFor(task_37C, 1);
    Script_InvestigationUtils(ShenmueUtils_SetupOrSyncAnchors, task_380, task_37C);
    local_D8 = Script_charaSetClipAngle(task_378, -1.0f);
    Script_charaSetClipAngle(task_378, 1.0f);
    SCNF_WaitFrames_0000(1);
    script_1FE50();
    script_14DD4();
    script_1428D();
    local_E0 = 0;
    local_DC = 0;
    task_40A = 0;
    for (;;) {
      if (MMTK_ScriptMemoToggle(2, 0) == 0) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE)) {
          task_40A = 1;
          Script_sicnUtils(1, 0, 0);
          Script_uiUtils(0, 0, 0);
          break;
        } else if (Script_GetControllerInput(1) == 0x400) {
          local_D3 = script_13BCF();
          if (local_D3) {
            script_13C41(local_D3);
            Script_sicnUtils(1, 0, 0);
            Script_uiUtils(0, 0, 0);
            break;
          } else {
            Script_sicnUtils(1, 0, 0);
            script_1428D();
            local_E0 = 0;
            local_DC = 0;
          }
        } else if ((Script_GetControllerInput(1) == 0x200) | (Script_GetControllerInput(1) == 0x2000) | (local_E0 > 1800)) {
          Script_sicnUtils(1, 0, 0);
          Script_uiUtils(0, 0, 0);
          break;
        } else {
          if (Script_GetControllerInput(1) == 4) {
            MMTK_ScriptMemoToggle(0, 0);
            goto L01402E;
          }
        }
        L01402E:;
        local_E0 = local_E0 + 1;
      } else {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE)) {
          MMTK_ScriptMemoToggle(1, 0);
          goto L014052;
        }
      }
      L014052:;
      SCNF_WaitFrames_0000(1);
    }
    if ((task_384 == 9998) | (task_384 == 4)) {
      script_201BF();
      SCNF_WaitFrames_0000(1);
      SCNF_WaitFrames_0000(1);
      SCNF_WaitFrames_0000(1);
      SCNF_WaitFrames_0000(1);
      Script_convSetCamInactiveFlag(0);
      Script_SetCPAMFlags(CHARA_AKIR, 1);
      Script_charaSetRefboardFlagTo(task_37C, 0);
      Script_charaSetRefboardFlagTo(task_380, 0);
      Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
      Script_SetGameStateTo(AUTH_None);
      Script_SetCameraFieldOfView(10922);
      Script_scenToggleActiveFor(CHARA_AKIR, 1);
    }
    script_1FF4F();
    Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
    Script_ryoControl(RYO_RestoreControl);
    Script_charaSetRefboardFlagTo(task_37C, 0);
    Script_charaSetRefboardFlagTo(task_380, 0);
    Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
    Script_charaSetClipAngle(task_378, local_D8);
    Script_scenToggleVisibleFor(task_378, 0);
    if (-1) {
      Script_charaDisplay(task_37C, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(task_37C, CDM_Show);
        goto L01414D;
      }
    }
    L01414D:;
    Script_scenToggleVisibleFor(task_37C, 0);
    if (-1) {
      Script_charaDisplay(task_380, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(task_380, CDM_Show);
        goto L01417B;
      }
    }
    L01417B:;
    Script_scenToggleVisibleFor(task_380, 0);
    task_3A1 = 0;
    task_3A3 = 0;
  } else {
    if ((task_3A1 == -1) | (task_3A1 == -10) | (task_3A1 == -15)) {
      Script_ryoControl(PCCTL_DisablePlayerControl);
      Script_ryoControl(PCCTL_UpdateRyoNavYaw, 0x52494B41);
      Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
      Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
      local_E4.x = 0.8f;
      (&local_E4)->y = 0.8f;
      (&local_E4)->z = 0.8f;
      (&local_E4)->w = 0.8f;
      Script_SetCharaRefBoardPos(CHARA_AKIR, &local_E4);
      script_1FE50();
      script_13C41(task_3A1);
      script_1FF4F();
      Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
      Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
      Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
      Script_ryoControl(RYO_RestoreControl);
      Script_convSetCamInactiveFlag(1);
      Script_SetGameStateTo(AUTH_None);
      task_3A1 = 0;
      task_3A3 = 0;
      goto L01428C;
    }
  }
  L01428C:;
  return;
}

__attribute__((scn_string_bindings(1, data_3FD14, 7, data_3FD4C, 8, data_3FD52, 13, data_3FD80, 14, data_3FD86, 15, data_3FD8C, 16, data_3FD92, 18, data_3FDA2, 21, data_3FDC0, 23, data_3FDD0, 25, data_3FDE4, 28, data_3FDFE)))
static void script_1428D(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int8_t local_EB;
  int8_t local_EA;
  int8_t local_E9;
  int8_t local_E8;
  int8_t local_E7;
  int8_t local_E6;
  int8_t local_E5;
  int8_t local_E4;
  Vector4 local_D4;
  local_D4.x = 0.8f;
  (&local_D4)->y = 0.8f;
  (&local_D4)->z = 0.8f;
  (&local_D4)->w = 0.8f;
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &local_D4);
  local_EC = Script_StepUtils(GET_STEP_HIGHER, 0) * 10 + Script_StepUtils(GET_STEP_LOWER, 0);
  Script_TELMUtils(TELM_CMD_LoadHandMotionAndSetup, task_378);
  local_E4 = 0;
  task_40B = 0;
  for (;;) {
    task_408 = Script_TELMUtils(TELM_CMD_GetActiveResult, task_378);
    task_408 = script_13AF4(task_408);
    Script_ReturnTimeDate(&local_F4);
    if (MMTK_ScriptMemoToggle(2, 0) & Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE)) {
      Script_sprintf("メモ帳消去\012");
      MMTK_ScriptMemoToggle(1, 0);
    }
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE) & -(task_408 != -4) & -(task_408 != 0) & -(local_E4 == 0)) {
      local_E4 = 1;
      Script_TELMUtils(TELM_CMD_RequestHangUp, task_378);
      task_408 = Script_TELMUtils(TELM_CMD_GetActiveResult, task_378);
    }
    if ((task_408 == *task_3AC) | (task_408 == task_3AC[1])) {
      Script_TELMUtils(TELM_CMD_RequestHangUp, task_378);
      SCNF_WaitFrames_0000(1);
      Script_LoadVoiceFile(1, "E1032");
      script_14E92();
      if (task_408 == *task_3AC) {
        int32_t local_D0;
        local_D0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
        if (local_D0 == 0) {
          task_398 = Script_ShowUIMessageFromID("E1032A001");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
        } else if (local_D0 == 1) {
          task_398 = Script_ShowUIMessageFromID("E1032A002");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
        } else {
          task_398 = Script_ShowUIMessageFromID("E1032A003");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
        }
      } else {
        int32_t local_D0;
        local_D0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
        if (local_D0 == 0) {
          task_398 = Script_ShowUIMessageFromID("E1032A004");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
        } else {
          task_398 = Script_ShowUIMessageFromID("E1032A005");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
        }
      }
      Script_LoadVoiceFile(0, "E1032");
      script_13CD5();
    }
    if ((task_408 == task_3AC[7]) | (task_408 == task_3AC[2]) | (task_408 == task_3AC[3]) | (task_408 == task_3AC[8]) | (task_408 == task_3AC[4]) | (task_408 == task_3AC[9]) | (task_408 == task_3AC[5]) | (task_408 == task_3AC[10]) | (task_408 == task_3AC[6]) | (task_408 == task_3AC[11])) {
      local_EB = 2;
      local_EA = 1;
      if (task_408 == task_3AC[7]) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_UNLOCKED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_DONE) == 0) & Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_OPERATOR_ADDRESS_LOOKUP_REFUSED)) {
          int32_t local_D0;
          Script_TELMUtils(TELM_CMD_RequestHangUp, task_378);
          SCNF_WaitFrames_0000(1);
          Script_LoadVoiceFile(1, "E1032");
          local_D0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f));
          if (local_D0 == 0) {
            task_398 = Script_ShowUIMessageFromID("E1032A061");
            while (Script_convGetIsActive((const void *)task_398)) {
              SCNF_WaitFrames_0000(1);
            }
            task_398 = 0;
          } else if (local_D0 == 1) {
            task_398 = Script_ShowUIMessageFromID("E1032A062");
            while (Script_convGetIsActive((const void *)task_398)) {
              SCNF_WaitFrames_0000(1);
            }
            task_398 = 0;
          } else if (local_D0 == 2) {
            task_398 = Script_ShowUIMessageFromID("E1032A063");
            while (Script_convGetIsActive((const void *)task_398)) {
              SCNF_WaitFrames_0000(1);
            }
            task_398 = 0;
          } else {
            task_398 = Script_ShowUIMessageFromID("E1032A064");
            while (Script_convGetIsActive((const void *)task_398)) {
              SCNF_WaitFrames_0000(1);
            }
            task_398 = 0;
          }
          Script_LoadVoiceFile(0, "E1032");
          script_13CD5();
          local_EA = 0;
        }
      }
      if (task_408 == task_3AC[2]) {
        local_EB = 1;
      }
      if ((task_408 == task_3AC[4]) | (task_408 == task_3AC[9])) {
        local_E9 = script_13CFC(local_E9);
        if (local_E9 == -1) {
          task_408 = -10;
          local_EA = 0;
        }
      }
      if ((task_408 == task_3AC[3]) | (task_408 == task_3AC[8])) {
        local_F0 = Script_StepUtils(GET_STEP_HIGHER, 0);
        if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_TOKI_TEGS_CUTSCENE_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_UNLOCKED) == 0)) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS08_SCENE_EXIT_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS8A_POST_MS08_FOLLOWUP_COMPLETE) == 0)) | (local_F0 >= 16)) {
          local_E6 = script_13CFC(local_E6);
          if (local_E6 == -1) {
            task_408 = -10;
            local_EA = 0;
          }
        } else {
          task_408 = -10;
          local_EA = 0;
        }
      }
      if ((task_408 == task_3AC[5]) | (task_408 == task_3AC[10])) {
        local_E8 = script_1773E();
        switch (local_E8) {
          case -1:
            task_408 = -10;
            local_EA = 0;
            break;
          case -2:
            task_408 = -11;
            local_EA = 0;
            break;
        }
      }
      if ((task_408 == task_3AC[6]) | (task_408 == task_3AC[11])) {
        local_E7 = script_18A65();
        local_E7 = script_13CFC(local_E7);
        if (local_E7 == -1) {
          task_408 = -10;
          local_EA = 0;
        }
      }
      if (local_EA) {
        if (script_13D3E(local_EB)) {
          Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
          Script_shenClearFreeState(0);
          Script_LoadVoiceFile(1, "E1032");
          script_14E92();
          script_1FFC2();
          local_E5 = 0;
          if (task_408 == task_3AC[7]) {
            script_14EB7();
          }
          if (task_408 == task_3AC[2]) {
            script_155FE();
          }
          if ((task_408 == task_3AC[5]) | (task_408 == task_3AC[10])) {
            script_178F4(local_E8);
          }
          if ((task_408 == task_3AC[6]) | (task_408 == task_3AC[11])) {
            script_1901D(local_E7);
          }
          if ((task_408 == task_3AC[3]) | (task_408 == task_3AC[8])) {
            local_E5 = script_1D9A3();
          }
          if ((task_408 == task_3AC[4]) | (task_408 == task_3AC[9])) {
            script_1CEDB();
          }
          script_20287();
          if (local_E5) {
            script_1DF2A();
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_UNLOCKED, 1);
          }
          Script_LoadVoiceFile(0, "E1032");
          script_201AB();
          Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
          Script_shenClearFreeState(1);
          task_408 = 0;
        } else {
          Script_TELMUtils(TELM_CMD_RequestHangUp, task_378);
          script_13CD5();
        }
      }
    }
    if (task_408 == -10) {
      SCNF_WaitFrames_0000(1);
      Script_LoadVoiceFile(1, "E1032");
      script_14E92();
      if (script_13D3E(6)) {
        int32_t local_D0;
        local_D0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f));
        if (local_D0 == 0) {
        } else if (local_D0 == 1) {
          task_398 = Script_ShowUIMessageFromID("E1032A031");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
        } else if (local_D0 == 2) {
          task_398 = Script_ShowUIMessageFromID("E1032A029");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
        } else if (local_D0 == 3) {
          task_398 = Script_ShowUIMessageFromID("E1032A032");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
        } else {
          task_398 = Script_ShowUIMessageFromID("E1032A033");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
        }
      }
      Script_LoadVoiceFile(0, "E1032");
      Script_TELMUtils(TELM_CMD_RequestHangUp, task_378);
      script_13CD5();
    }
    if (task_408 == -11) {
      int32_t local_D0;
      Script_scenCreateTask(script_138A9, 0);
      script_14E92();
      for (;;) {
        Script_executeAicaCommand(394667, 0, 0);
        SCNF_WaitFrames_0000(15);
        Script_executeAicaCommand(460203, 0, 0);
        SCNF_WaitFrames_0000(15);
        if (task_3A0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      local_D0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f));
      if (local_D0 == 0) {
      } else if (local_D0 == 1) {
        task_398 = Script_ShowUIMessageFromID("E1032A028");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      } else if (local_D0 == 2) {
        task_398 = Script_ShowUIMessageFromID("E1032A029");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      } else {
        task_398 = Script_ShowUIMessageFromID("E1032A030");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      }
      task_408 = Script_TELMUtils(TELM_CMD_GetActiveResult, task_378);
      script_13CD5();
    }
    if ((task_408 == task_3AC[12]) | (task_408 == task_3AC[13]) | (task_408 == task_3AC[14]) | (task_408 == task_3AC[15]) | (task_408 == task_3AC[16]) | (task_408 == task_3AC[17]) | (task_408 == task_3AC[18]) | (task_408 == task_3AC[19]) | (task_408 == task_3AC[20]) | (task_408 == task_3AC[21])) {
      SCNF_WaitFrames_0000(1);
      Script_LoadVoiceFile(1, "E1032");
      script_14E92();
      Script_scenCreateTask(script_138A9, 0);
      for (;;) {
        task_398 = Script_ShowUIMessageFromID("E1032G001");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
        if (task_3A0) {
          break;
        }
        task_398 = Script_ShowUIMessageFromID("E1032G002");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
        if (task_3A0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      Script_LoadVoiceFile(0, "E1032");
      task_408 = Script_TELMUtils(TELM_CMD_GetActiveResult, task_378);
      script_13CD5();
    }
    if (task_408 == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(6219, task_378, task_378);
  script_14DD4();
  Script_sicnUtils(0, 5, 1);
  Script_uiUtils(0, 9, 0);
  SCNF_WaitFrames_0000(1);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  return;
}

static void script_14DD4(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  Vector3i local_DC;
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_EVT_GetPosition(task_378, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(task_378, &local_E8, 0);
  local_F4.x = (&local_F4)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y) * 0.75f;
  (&local_F4)->y = -0.3f;
  (&local_F4)->z = (&local_F4)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y) * 0.75f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  (&local_DC)->y = (&local_E8)->y;
  Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_DC, 0);
  Script_InitializeOsag(CHARA_AKIR);
  return;
}

static void script_14E92(void) {
  int32_t local_FC;
  local_FC = Script_PlayerStatsUtils(PS_GetStrayCatSceneVisitCountSlot, 3);
  local_FC = local_FC + 1;
  Script_PlayerStatsUtils(PS_SetStrayCatSceneVisitCountSlot, 3, local_FC);
  return;
}

__attribute__((scn_string_bindings(0, data_3FE04, 5, data_3FE36, 6, data_3FE40, 7, data_3FE4A, 8, data_3FE54, 14, data_3FE90, 16, data_3FEA4, 17, data_3FEAE, 18, data_3FEB8, 19, data_3FEC2, 21, data_3FED6, 22, data_3FEE0, 23, data_3FEEA, 24, data_3FEF4, 25, data_3FEFE)))
static void script_14EB7(void) {
  int32_t local_FC;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_UNLOCKED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_DONE) == 0)) {
    int32_t local_F8;
    task_394 = 81;
    Script_scenCreateTask(script_1F4CE, 0);
    task_398 = Script_ShowUIMessageFromID("E1032I001");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032A052");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032I007");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032I008");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    local_FC = SCNF_RequestActionSelectorFromParams_0008(27, 28, 29, 30, 600, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD);
    task_394 = 82;
    local_F8 = local_FC;
    switch (local_F8) {
      case 27:
        task_398 = Script_ShowUIMessageFromID("E1032A097");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
        task_398 = Script_ShowUIMessageFromID("E1032I009");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
        task_398 = Script_ShowUIMessageFromID("E1032I010");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
        task_398 = Script_ShowUIMessageFromID("E1032A054");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
        task_398 = Script_ShowUIMessageFromID("E1032I011");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
        break;
      case 28:
        task_398 = Script_ShowUIMessageFromID("E1032A055");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
        task_398 = Script_ShowUIMessageFromID("E1032I012");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
        task_398 = Script_ShowUIMessageFromID("E1032A056");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
        task_398 = Script_ShowUIMessageFromID("E1032I013");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
        task_398 = Script_ShowUIMessageFromID("E1032A057");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
        task_398 = Script_ShowUIMessageFromID("E1032I011");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_DONE, 1);
        break;
      case 29:
        task_398 = Script_ShowUIMessageFromID("E1032A059");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
        task_398 = Script_ShowUIMessageFromID("E1032I009");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
        task_398 = Script_ShowUIMessageFromID("E1032I010");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
        task_398 = Script_ShowUIMessageFromID("E1032A054");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
        task_398 = Script_ShowUIMessageFromID("E1032I011");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
        break;
      case 30:
        task_398 = Script_ShowUIMessageFromID("E1032A060");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
        task_398 = Script_ShowUIMessageFromID("E1032I009");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
        task_398 = Script_ShowUIMessageFromID("E1032I010");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
        task_398 = Script_ShowUIMessageFromID("E1032A054");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
        task_398 = Script_ShowUIMessageFromID("E1032I011");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
        break;
    }
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_OPERATOR_ADDRESS_LOOKUP_REFUSED, 1);
  } else {
    task_394 = 80;
    Script_scenCreateTask(script_1F4CE, 0);
    task_398 = Script_ShowUIMessageFromID("E1032I001");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    {
      int32_t local_F8;
      local_F8 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
      if (local_F8 == 0) {
        task_398 = Script_ShowUIMessageFromID("E1032A007");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      } else {
        task_398 = Script_ShowUIMessageFromID("E1032A006");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      }
    }
    {
      int32_t local_F8;
      local_F8 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f));
      if (local_F8 == 0) {
        task_398 = Script_ShowUIMessageFromID("E1032I002");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      } else if (local_F8 == 1) {
        task_398 = Script_ShowUIMessageFromID("E1032I003");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      } else if (local_F8 == 2) {
        task_398 = Script_ShowUIMessageFromID("E1032I004");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      } else if (local_F8 == 3) {
        task_398 = Script_ShowUIMessageFromID("E1032I005");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      } else {
        task_398 = Script_ShowUIMessageFromID("E1032I006");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      }
    }
    {
      int32_t local_F8;
      local_F8 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
      if (local_F8 == 0) {
        task_398 = Script_ShowUIMessageFromID("E1032A008");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      } else {
        task_398 = Script_ShowUIMessageFromID("E1032A009");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      }
    }
  }
  return;
}

__attribute__((scn_string_bindings(5, data_3FF94, 6, data_3FF9E, 7, data_3FFA8, 8, data_3FFB2, 9, data_3FFBC, 10, data_3FFC6, 11, data_3FFD0, 12, data_3FFDA, 13, data_3FFE4, 14, data_3FFEE, 15, data_3FFF8, 16, data_40002, 17, data_4000C, 18, data_40016, 19, data_40020, 20, data_4002A, 21, data_40034, 22, data_4003E, 23, data_40048, 24, data_40052, 25, data_4005C, 26, data_40066, 27, data_40070, 28, data_4007A, 29, data_40084, 30, data_4008E, 31, data_40098, 32, data_400A2, 33, data_400AC, 34, data_400B6, 35, data_400C0, 36, data_400CA, 37, data_400D4, 38, data_400DE, 39, data_400E8, 40, data_400F2, 41, data_400FC, 42, data_40106, 43, data_40110, 44, data_4011A, 45, data_40124, 46, data_4012E, 47, data_40138, 48, data_40142, 49, data_4014C, 50, data_40156, 51, data_40160, 52, data_4016A, 53, data_40174, 54, data_4017E, 55, data_40188, 56, data_40192, 57, data_4019C, 58, data_401A6, 59, data_401B0, 60, data_401BA, 61, data_401C4, 62, data_401CE, 63, data_401D8, 64, data_401E2, 65, data_401EC, 66, data_401F6, 67, data_40200, 68, data_4020A, 69, data_40214, 70, data_4021E, 71, data_40228, 72, data_40232, 73, data_4023C, 74, data_40246, 75, data_40250, 76, data_4025A, 77, data_40264, 78, data_4026E, 79, data_40278, 80, data_40282, 81, data_4028C, 82, data_40296, 83, data_402A0, 84, data_402AA, 85, data_402B4, 86, data_402BE, 87, data_402C8, 88, data_402D2, 89, data_402DC, 90, data_402E6, 91, data_402F0, 92, data_402FA, 93, data_40304, 94, data_4030E, 95, data_40318, 96, data_40322, 97, data_4032C, 98, data_40336, 99, data_40340, 100, data_4034A, 101, data_40354, 102, data_4035E, 103, data_40368, 104, data_40372, 105, data_4037C, 106, data_40386, 107, data_40390, 108, data_4039A, 109, data_403A4, 110, data_403AE, 111, data_403B8, 112, data_403C2, 113, data_403CC, 114, data_403D6, 115, data_403E0, 116, data_403EA, 117, data_403F4, 118, data_403FE, 119, data_40408, 120, data_40412, 121, data_4041C, 122, data_40426, 123, data_40430, 124, data_4043A, 125, data_40444, 126, data_4044E, 127, data_40458, 128, data_40462, 129, data_4046C, 130, data_40476, 131, data_40480, 132, data_4048A, 133, data_40494, 134, data_4049E, 135, data_404A8, 136, data_404B2, 137, data_404BC, 138, data_404C6, 139, data_404D0, 140, data_404DA, 141, data_404E4, 142, data_404EE, 143, data_404F8, 144, data_40502, 145, data_4050C, 146, data_40516, 147, data_40520, 148, data_4052A, 149, data_40534, 150, data_4053E, 151, data_40548, 152, data_40552, 153, data_4055C, 154, data_40566, 155, data_40570, 156, data_4057A, 157, data_40584, 158, data_4058E, 159, data_40598, 160, data_405A2, 161, data_405AC, 162, data_405B6)))
static void script_155FE(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  task_394 = 70;
  Script_scenCreateTask(script_1F4CE, 0);
  task_398 = Script_ShowUIMessageFromID("E1032K001");
  while (Script_convGetIsActive((const void *)task_398)) {
    SCNF_WaitFrames_0000(1);
  }
  task_398 = 0;
  task_398 = Script_ShowUIMessageFromID("E1032K002");
  while (Script_convGetIsActive((const void *)task_398)) {
    SCNF_WaitFrames_0000(1);
  }
  task_398 = 0;
  task_398 = Script_ShowUIMessageFromID("E1032K019");
  while (Script_convGetIsActive((const void *)task_398)) {
    SCNF_WaitFrames_0000(1);
  }
  task_398 = 0;
  task_398 = Script_ShowUIMessageFromID("E1032K020");
  while (Script_convGetIsActive((const void *)task_398)) {
    SCNF_WaitFrames_0000(1);
  }
  task_398 = 0;
  task_398 = Script_ShowUIMessageFromID("E1032K022");
  while (Script_convGetIsActive((const void *)task_398)) {
    SCNF_WaitFrames_0000(1);
  }
  task_398 = 0;
  Script_ReturnTimeDate(&local_F4);
  if (((&local_F4)->month == 1) | ((&local_F4)->month == 3) | ((&local_F4)->month == 5) | ((&local_F4)->month == 7) | ((&local_F4)->month == 8) | ((&local_F4)->month == 10) | ((&local_F4)->month == 12)) {
    if ((&local_F4)->day >= 31) {
      if ((&local_F4)->month == 12) {
        local_F4.year = (&local_F4)->year + 1;
        (&local_F4)->month = 1;
      } else {
        (&local_F4)->month = (&local_F4)->month + 1;
      }
      (&local_F4)->day = 1;
    } else {
      (&local_F4)->day = (&local_F4)->day + 1;
    }
  } else if ((&local_F4)->month == 2) {
    if ((&local_F4)->day >= 28) {
      (&local_F4)->month = 3;
      (&local_F4)->day = 1;
    } else {
      (&local_F4)->day = (&local_F4)->day + 1;
    }
  } else if ((&local_F4)->day >= 30) {
    (&local_F4)->month = (&local_F4)->month + 1;
    (&local_F4)->day = 1;
  } else {
    (&local_F4)->day = (&local_F4)->day + 1;
  }
  local_F0 = Script_WeatherUtils_0(WeatherDataUtil_LookupFCASDateCode, (&local_F4)->year, (&local_F4)->month, (&local_F4)->day);
  local_EC = local_F0;
  if (local_EC == 1) {
    task_398 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 2) {
    task_398 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 3) {
    task_398 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 4) {
    task_398 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 5) {
    task_398 = Script_ShowUIMessageFromID("E1032K003");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 6) {
    task_398 = Script_ShowUIMessageFromID("E1032K003");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 7) {
    task_398 = Script_ShowUIMessageFromID("E1032K003");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 8) {
    task_398 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 9) {
    task_398 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 10) {
    task_398 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 11) {
    task_398 = Script_ShowUIMessageFromID("E1032K005");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 12) {
    task_398 = Script_ShowUIMessageFromID("E1032K005");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 13) {
    task_398 = Script_ShowUIMessageFromID("E1032K005");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 14) {
    task_398 = Script_ShowUIMessageFromID("E1032K006");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 15) {
    task_398 = Script_ShowUIMessageFromID("E1032K006");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 16) {
    task_398 = Script_ShowUIMessageFromID("E1032K006");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 17) {
    task_398 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 18) {
    task_398 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 19) {
    task_398 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 20) {
    task_398 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 21) {
    task_398 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 22) {
    task_398 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 23) {
    task_398 = Script_ShowUIMessageFromID("E1032K009");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 24) {
    task_398 = Script_ShowUIMessageFromID("E1032K009");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 25) {
    task_398 = Script_ShowUIMessageFromID("E1032K009");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 26) {
    task_398 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 27) {
    task_398 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 28) {
    task_398 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 29) {
    task_398 = Script_ShowUIMessageFromID("E1032K011");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 30) {
    task_398 = Script_ShowUIMessageFromID("E1032K011");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 31) {
    task_398 = Script_ShowUIMessageFromID("E1032K011");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 32) {
    task_398 = Script_ShowUIMessageFromID("E1032K012");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 33) {
    task_398 = Script_ShowUIMessageFromID("E1032K012");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 34) {
    task_398 = Script_ShowUIMessageFromID("E1032K012");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 35) {
    task_398 = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 36) {
    task_398 = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 37) {
    task_398 = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 38) {
    task_398 = Script_ShowUIMessageFromID("E1032K014");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 39) {
    task_398 = Script_ShowUIMessageFromID("E1032K014");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 40) {
    task_398 = Script_ShowUIMessageFromID("E1032K014");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 41) {
    task_398 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 42) {
    task_398 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 43) {
    task_398 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K006");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 44) {
    task_398 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 45) {
    task_398 = Script_ShowUIMessageFromID("E1032K005");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 46) {
    task_398 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 47) {
    task_398 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 48) {
    task_398 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 54) {
    task_398 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K005");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 50) {
    task_398 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 51) {
    task_398 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 52) {
    task_398 = Script_ShowUIMessageFromID("E1032K011");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 53) {
    task_398 = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 54) {
    task_398 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K005");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 55) {
    task_398 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 56) {
    task_398 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 57) {
    task_398 = Script_ShowUIMessageFromID("E1032K003");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 58) {
    task_398 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 59) {
    task_398 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 60) {
    task_398 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 61) {
    task_398 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 62) {
    task_398 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 63) {
    task_398 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 64) {
    task_398 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 65) {
    task_398 = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 66) {
    task_398 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else if (local_EC == 67) {
    task_398 = Script_ShowUIMessageFromID("E1032K014");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else {
    task_398 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  }
  return;
}

static int8_t script_17685(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  int8_t local_E7;
  uint8_t unused_E4[3];
  Script_ReturnTimeDate(&local_F4);
  if (arg0 == arg2) {
    if (arg1 < arg3) {
      local_E7 = 1;
    } else {
      local_E7 = 0;
    }
  } else if (arg0 < arg2) {
    local_E7 = 1;
  } else {
    local_E7 = 0;
  }
  local_F0 = arg0 * 100 + arg1;
  local_EC = arg2 * 100 + arg3;
  local_E8 = (&local_F4)->hours * 100 + (&local_F4)->minutes;
  if (local_E7) {
    if ((local_E8 >= local_F0) & (local_E8 < local_EC)) {
      local_E7 = 1;
    } else {
      local_E7 = 0;
    }
  } else if ((local_E8 >= local_F0) | (local_E8 < local_EC)) {
    local_E7 = 1;
  } else {
    local_E7 = 0;
  }
  return local_E7;
}

static int8_t script_1773E(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int8_t local_EF;
  int8_t local_EE;
  int8_t local_ED;
  int8_t local_EC;
  int32_t local_E8;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = Script_StepUtils(GET_STEP_HIGHER, 0);
  if (script_17685(8, 30, 15, 10)) {
    local_EE = 0;
  } else if (script_17685(15, 10, 20, 30)) {
    local_EE = 1;
  } else {
    local_EE = 2;
  }
  if ((&local_F4)->day & 1) {
    local_ED = 0;
  } else {
    local_ED = 1;
  }
  local_EF = 0;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_KIDNAPPED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0)) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 950) == 0) {
      local_EF = 7;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 950, 1);
    } else {
      local_EF = -2;
    }
  } else {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG)) {
      if ((local_EE == 2) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NAOYUKI_MOTORCYCLE_THANKS_CALL) == 0)) {
        local_EF = 8;
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NAOYUKI_MOTORCYCLE_THANKS_CALL, 1);
      }
      goto L017833;
    }
  }
  L017833:;
  if (local_EF == 0) {
    local_EF = script_13CFC(local_EF);
  }
  if (local_EF == 0) {
    local_E8 = local_EE;
    if (local_E8 == 0) {
      if (local_ED == 0) {
        local_EF = 5;
      } else {
        local_EF = 6;
      }
    } else if (local_E8 == 1) {
      if (local_ED == 0) {
        local_EF = -1;
      } else {
        local_EF = -2;
      }
    } else {
      local_EC = (&local_F4)->day & 3;
      local_E8 = local_EC;
      if (local_E8 == 1) {
        local_EF = 1;
      } else if (local_E8 == 2) {
        if (local_F0 >= 11) {
          local_EF = 2;
        } else {
          local_EF = 3;
        }
      } else {
        local_EF = 4;
      }
    }
  }
  return local_EF;
}

__attribute__((scn_string_bindings(0, data_405C0, 8, data_40610, 9, data_4061A, 14, data_4064C, 17, data_4066A, 18, data_40674, 19, data_4067E, 21, data_40692, 25, data_406BA, 27, data_406CE, 33, data_4070A, 40, data_40750, 41, data_4075A, 42, data_40764, 43, data_4076E, 44, data_40778, 48, data_407A0, 49, data_407AA, 52, data_407C8, 53, data_407D2, 57, data_407FA, 59, data_4080E, 61, data_40822, 62, data_4082C, 70, data_4087C, 75, data_408AE, 76, data_408B8, 77, data_408C2, 82, data_408F4, 83, data_408FE, 84, data_40908, 85, data_40912, 86, data_4091C, 87, data_40926, 88, data_40930, 92, data_40958, 93, data_40962)))
static void script_178F4(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 1:
      task_394 = 170;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032L001");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A098");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032L002");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A099");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032L003");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032L004");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032L005");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A100");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      break;
    case 2:
      task_394 = 180;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032L001");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A101");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032L006");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032L007");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032L008");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032L009");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A102");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032L010");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      break;
    case 3:
      task_394 = 190;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032L011");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A101");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032L012");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A103");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032L013");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032L014");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032L015");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_394 = 191;
      task_398 = Script_ShowUIMessageFromID("E1032A103");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032L016");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A105");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032L017");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A106");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      break;
    case 4:
      task_394 = 200;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032L018");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A107");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032L019");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A108");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032L020");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032L021");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A109");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032L022");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A110");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032L023");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      break;
    case 5:
      task_394 = 210;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032M001");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A111");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032M002");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A112");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032M003");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A113");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032M004");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A114");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032M005");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      break;
    case 6:
      task_394 = 220;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032M006");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A111");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032M002");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A115");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032M018");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A122");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032M019");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A123");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032M019");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032M020");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A114");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032M010");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      break;
    case 7:
      task_394 = 230;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032N001");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A124");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A125");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032N002");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A126");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032N003");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_394 = 231;
      task_398 = Script_ShowUIMessageFromID("E1032A127");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A128");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032N004");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A129");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032N005");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      break;
    case 8:
      task_394 = 240;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032L011");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A101");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032L019");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A117");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032L024");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032L025");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032L026");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A118");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032L023");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      break;
    case 9:
      task_398 = Script_ShowUIMessageFromID("E1032M001");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A111");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032M002");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A112");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032M015");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A119");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032M016");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A118");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032M017");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      break;
  }
  return;
}

static int8_t script_18A65(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int8_t local_EF;
  int8_t local_EE;
  uint8_t unused_EC[2];
  int32_t local_E8;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = Script_StepUtils(GET_STEP_HIGHER, 0) * 10 + Script_StepUtils(GET_STEP_LOWER, 0);
  local_EF = 0;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIND_TERRY) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG) == 0)) {
    local_EF = -1;
  } else if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_DNOZ_EVENT_DONE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_DKTY_TUBA_CUTSCENE_COMPLETE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_DNOZ_ROUTE_ACTIVE) == 0)) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_01BNK_BANK_EVENT_ACTIVE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_CRIED) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIND_TERRY) == 0))) {
    if (script_17685(8, 30, 21, 0)) {
      local_EF = 1;
    } else {
      local_EF = 2;
    }
  } else if ((local_F0 == 13) | (local_F0 == 174)) {
    if (script_17685(8, 30, 22, 0)) {
      local_EF = 11;
    } else {
      local_EF = 10;
    }
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_CRIED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_SORRY_ABOUT_OTHER_DAY_CALL) == 0)) {
    local_EF = 3;
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_JUTADN_EVENT_ACTIVE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_D000_YBHN_EVENT_DONE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 946) == 0)) {
    local_EF = 6;
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_D000_YBHN_EVENT_DONE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE5_COMPLETE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_COMPLETE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_MEGUMI_KEEPS_CAT) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_DNOZ_ROUTE_ACTIVE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_0172_SCENE2_PROMPT_SHOWN) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 955) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 537) == 0)) {
    local_EF = 12;
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_DNOZ_ROUTE_ACTIVE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_0172_SCENE2_PROMPT_SHOWN) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 955) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE5_COMPLETE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_COMPLETE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_MEGUMI_KEEPS_CAT) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_SPRUNG_ON_YOU_CALL) == 0)) {
    local_EF = 7;
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE5_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_COMPLETE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_MEGUMI_KEEPS_CAT) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_0172_SCENE2_PROMPT_SHOWN) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 955) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_SHRINE_KITTEN_CALL) == 0) & -(local_F0 >= 162)) {
    local_EF = 13;
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_MEGUMI_KEEPS_CAT) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_0172_SCENE2_PROMPT_SHOWN) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 955) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_KITTEN_RECOVERED_CALL) == 0) & -(local_F0 >= 201)) {
    local_EF = 14;
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_0172_SCENE2_PROMPT_SHOWN) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 955) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_FLOWERS_THANKS_CALL) == 0)) {
    local_EF = 4;
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 955) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIND_TERRY) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_STAY_AWAY_FROM_HARBOR_CALL) == 0)) {
    local_EF = 16;
  } else {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_AFTER_RESCUE_CALL) == 0)) {
      local_EF = 5;
      goto L018DDE;
    }
  }
  L018DDE:;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIND_TERRY) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG) == 0)) {
    local_EF = -1;
  } else {
    if (Script_StepUtils(GET_STEP_HIGHER, 0) * 10 + Script_StepUtils(GET_STEP_LOWER, 0) == 215) {
      if (script_17685(8, 30, 21, 0)) {
        local_EF = 1;
      } else {
        local_EF = 2;
      }
      goto L018E47;
    }
  }
  L018E47:;
  if (local_EF == 0) {
    if ((local_F0 <= 13) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_TALK_TO_INE_CALL) == 0)) {
      local_EF = 17;
    } else if (-(local_F0 >= 21) & -(local_F0 <= 102) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_PARENTS_IN_VANCOUVER_CALL) == 0)) {
      local_EF = 18;
    } else if (-(local_F0 >= 61) & -(local_F0 <= 102) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_CALL_NAOYUKI_AND_ICHIRO_CALL) == 0)) {
      local_EF = 19;
    } else if (-(local_F0 >= 111) & -(local_F0 <= 193) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_LAPIS_FORTUNETELLER_CALL) == 0)) {
      local_EF = 20;
    } else if (-(local_F0 >= 141) & -(local_F0 <= 193) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_FIRST_MEETING_CALL) == 0)) {
      local_EF = 22;
    } else if (-(local_F0 >= 171) & -(local_F0 <= 193) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_NURSERY_TEACHER_CALL) == 0)) {
      local_EF = 23;
    } else if (-(local_F0 >= 211) & -(local_F0 <= 218) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_FEELS_WATCHED_CALL) == 0)) {
      local_EF = 24;
    } else if (-(local_F0 >= 201) & -(local_F0 <= 241) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_TRIP_INVITATION_CALL) == 0)) {
      local_EF = 25;
    } else {
      if (-(local_F0 >= 215) & -(local_F0 <= 241) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_RELAY_RACE_PHOTO_CALL) == 0)) {
        local_EF = 21;
        goto L018FAB;
      }
    }
  }
  L018FAB:;
  if (local_EF == 0) {
    local_EE = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f));
    local_E8 = local_EE;
    switch (local_E8) {
      case 0:
        local_EF = 8;
        break;
      case 1:
        local_EF = 9;
        break;
      case 2:
        local_EF = 10;
        break;
      case 3:
        local_EF = 26;
        break;
    }
  }
  return local_EF;
}

__attribute__((scn_string_bindings(0, data_40976, 1, data_40980, 2, data_4098A, 4, data_4099E, 5, data_409A8, 6, data_409B2, 7, data_409BC, 8, data_409C6, 9, data_409D0, 10, data_409DA, 11, data_409E4, 12, data_409EE, 13, data_409F8, 15, data_40A0C, 16, data_40A16, 17, data_40A20, 18, data_40A2A, 19, data_40A34, 20, data_40A3E, 21, data_40A48, 22, data_40A52, 23, data_40A5C, 25, data_40A70, 27, data_40A84, 28, data_40A8E, 31, data_40AAC, 32, data_40AB6, 33, data_40AC0, 38, data_40AF2, 39, data_40AFC, 40, data_40B06, 41, data_40B10, 43, data_40B24, 44, data_40B2E, 45, data_40B38, 56, data_40BA6, 57, data_40BB0, 58, data_40BBA, 59, data_40BC4, 60, data_40BCE, 61, data_40BD8, 64, data_40BF6, 65, data_40C00, 66, data_40C0A, 67, data_40C14, 68, data_40C1E, 69, data_40C28, 70, data_40C32, 71, data_40C3C, 72, data_40C46, 74, data_40C5A, 76, data_40C6E, 78, data_40C82, 79, data_40C8C, 80, data_40C96, 81, data_40CA0, 82, data_40CAA, 84, data_40CBE, 86, data_40CD2, 87, data_40CDC, 88, data_40CE6, 89, data_40CF0, 90, data_40CFA, 91, data_40D04, 92, data_40D0E, 93, data_40D18, 94, data_40D22, 96, data_40D36, 99, data_40D54, 100, data_40D5E, 101, data_40D68, 102, data_40D72, 103, data_40D7C, 104, data_40D86, 106, data_40D9A, 107, data_40DA4, 111, data_40DCC, 113, data_40DE0, 115, data_40DF4, 116, data_40DFE, 117, data_40E08, 118, data_40E12, 119, data_40E1C, 120, data_40E26, 125, data_40E58, 127, data_40E6C, 128, data_40E76, 129, data_40E80, 132, data_40E9E, 133, data_40EA8, 134, data_40EB2, 135, data_40EBC, 136, data_40EC6, 137, data_40ED0, 138, data_40EDA, 139, data_40EE4, 140, data_40EEE, 141, data_40EF8, 142, data_40F02, 143, data_40F0C, 144, data_40F16, 150, data_40F52, 158, data_40FA2, 159, data_40FAC, 160, data_40FB6, 161, data_40FC0, 163, data_40FD4, 172, data_4102E, 173, data_41038, 174, data_41042, 175, data_4104C, 182, data_41092, 185, data_410B0, 186, data_410BA, 187, data_410C4, 198, data_41132, 200, data_41146, 201, data_41150, 208, data_41196, 209, data_411A0, 216, data_411E6, 224, data_41236, 225, data_41240, 226, data_4124A, 232, data_41286, 234, data_4129A, 235, data_412A4, 237, data_412B8, 239, data_412CC, 244, data_412FE, 245, data_41308, 256, data_41376, 258, data_4138A, 260, data_4139E, 261, data_413A8, 265, data_413D0, 269, data_413F8, 271, data_4140C, 273, data_41420, 274, data_4142A, 275, data_41434, 276, data_4143E, 277, data_41448, 278, data_41452, 280, data_41466, 282, data_4147A, 285, data_41498, 288, data_414B6, 289, data_414C0, 290, data_414CA, 295, data_414FC, 297, data_41510, 299, data_41524, 300, data_4152E, 301, data_41538, 302, data_41542, 303, data_4154C, 304, data_41556, 308, data_4157E, 311, data_4159C, 313, data_415B0, 315, data_415C4, 316, data_415CE, 317, data_415D8, 318, data_415E2, 331, data_41664)))
static void script_1901D(int32_t arg0) {
  int8_t local_FF;
  int32_t local_F8;
  int32_t local_F4;
  local_F8 = Script_StepUtils(GET_STEP_HIGHER, 0) * 10 + Script_StepUtils(GET_STEP_LOWER, 0);
  local_F4 = arg0;
  switch (local_F4) {
    case 1:
      task_394 = 90;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032O001");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A136");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032O002");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A137");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032O007");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A127");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032O004");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A138");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032O005");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A139");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032O006");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      break;
    case 2:
      task_394 = 90;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032O001");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A136");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032O002");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A140");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032O007");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A127");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032O004");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A138");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032O005");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A139");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032O006");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      break;
    case 3:
      task_394 = 90;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P002");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P003");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A141");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P004");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A142");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P005");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_SORRY_ABOUT_OTHER_DAY_CALL, 1);
      break;
    case 4:
      task_394 = 100;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P006");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A143");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P007");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A144");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P004");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032O008");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_394 = 101;
      task_398 = Script_ShowUIMessageFromID("E1032P008");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P009");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_FLOWERS_THANKS_CALL, 1);
      break;
    case 5:
      task_394 = 110;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P010");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A145");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P011");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A146");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P012");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A147");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P013");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A148");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P014");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      SCNF_WaitFrames_0000(60);
      task_394 = 111;
      task_398 = Script_ShowUIMessageFromID("E1032A149");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P015");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P016");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P017");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_AFTER_RESCUE_CALL, 1);
      break;
    case 6:
      task_394 = 120;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P018");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A150");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P019");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A151");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P004");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P016");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P017");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 946, 1);
      break;
    case 7:
      task_394 = 130;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032P021");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P010");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      SCNF_WaitFrames_0000(60);
      task_394 = 131;
      task_398 = Script_ShowUIMessageFromID("E1032A153");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P022");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A154");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P023");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P024");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P017");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_SPRUNG_ON_YOU_CALL, 1);
      break;
    case 8:
      task_394 = 140;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P025");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A155");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P004");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A156");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032O008");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P008");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_394 = 141;
      task_398 = Script_ShowUIMessageFromID("E1032A154");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P027");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P017");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      break;
    case 9:
      task_394 = 150;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032P021");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P028");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A158");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P030");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_394 = 151;
      task_398 = Script_ShowUIMessageFromID("E1032A159");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P066");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P016");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P017");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      break;
    case 10:
      task_394 = 160;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032P021");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P032");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A160");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P033");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      SCNF_WaitFrames_0000(60);
      task_394 = 161;
      if ((local_F8 == 13) | (local_F8 == 174)) {
        task_398 = Script_ShowUIMessageFromID("E1032P034");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      } else {
        local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
        local_F4 = local_FF;
        switch (local_F4) {
          case 0:
            task_398 = Script_ShowUIMessageFromID("E1032P080");
            while (Script_convGetIsActive((const void *)task_398)) {
              SCNF_WaitFrames_0000(1);
            }
            task_398 = 0;
            break;
          case 1:
            task_398 = Script_ShowUIMessageFromID("E1032P081");
            while (Script_convGetIsActive((const void *)task_398)) {
              SCNF_WaitFrames_0000(1);
            }
            task_398 = 0;
            break;
        }
      }
      task_398 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P035");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A141");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P036");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      break;
    case 11:
      task_394 = 162;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032P021");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P025");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A160");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P033");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      SCNF_WaitFrames_0000(60);
      task_394 = 163;
      local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      local_F4 = local_FF;
      switch (local_F4) {
        case 0:
          task_398 = Script_ShowUIMessageFromID("E1032P082");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
          break;
        case 1:
          task_398 = Script_ShowUIMessageFromID("E1032P083");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
          break;
        case 2:
          task_398 = Script_ShowUIMessageFromID("E1032P084");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
          task_398 = Script_ShowUIMessageFromID("E1032P085");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
          break;
      }
      task_398 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P086");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      break;
    case 12:
      task_394 = 370;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A164");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P018");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A165");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P037");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P120");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A173");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032O008");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P008");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_394 = 371;
      task_398 = Script_ShowUIMessageFromID("E1032A154");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P027");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P017");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 537, 1);
      break;
    case 13:
      task_394 = 380;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A164");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P018");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A170");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P044");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A200");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A172");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P045");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A173");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P046");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P047");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_394 = 381;
      task_398 = Script_ShowUIMessageFromID("E1032A201");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P049");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A176");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P050");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A177");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P051");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_SHRINE_KITTEN_CALL, 1);
      break;
    case 14:
      task_394 = 390;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A164");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P018");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A178");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P004");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P052");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A179");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A180");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A181");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_394 = 391;
      task_398 = Script_ShowUIMessageFromID("E1032P053");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P087");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P055");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A182");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P051");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_KITTEN_RECOVERED_CALL, 1);
      break;
    case 15:
      task_394 = 400;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A164");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P018");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A183");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P038");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A184");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P056");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P088");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P058");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A220");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A185");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P059");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_KEEP_KITTEN_CALL, 1);
      break;
    case 16:
      task_394 = 410;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A164");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P018");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P060");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A186");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P061");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A187");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P062");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_394 = 411;
      task_398 = Script_ShowUIMessageFromID("E1032A188");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P063");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      SCNF_WaitFrames_0000(40);
      task_398 = Script_ShowUIMessageFromID("E1032P064");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P065");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A189");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P066");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A190");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_STAY_AWAY_FROM_HARBOR_CALL, 1);
      break;
    case 17:
      task_394 = 270;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P067");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A191");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A192");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P068");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P069");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A193");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_TALK_TO_INE_CALL, 1);
      break;
    case 24:
      task_394 = 280;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P070");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A194");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P071");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A195");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P072");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P073");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A151");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P074");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P075");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P089");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P090");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_394 = 281;
      task_398 = Script_ShowUIMessageFromID("E1032A196");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P079");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A197");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_FEELS_WATCHED_CALL, 1);
      break;
    case 21:
      task_394 = 290;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P010");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P091");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P092");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A203");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P093");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      SCNF_WaitFrames_0000(60);
      task_398 = Script_ShowUIMessageFromID("E1032P094");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A204");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P095");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_RELAY_RACE_PHOTO_CALL, 1);
      break;
    case 26:
      task_394 = 300;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032P021");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P096");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P097");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A103");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_394 = 301;
      local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      local_F4 = local_FF;
      switch (local_F4) {
        case 0:
          task_398 = Script_ShowUIMessageFromID("E1032P098");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
          break;
        case 1:
          task_398 = Script_ShowUIMessageFromID("E1032P099");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
          break;
        case 2:
          task_398 = Script_ShowUIMessageFromID("E1032P100");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
          break;
      }
      task_398 = Script_ShowUIMessageFromID("E1032A206");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      break;
    case 18:
      task_394 = 310;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032P021");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P101");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A207");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P102");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A208");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P103");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_394 = 311;
      task_398 = Script_ShowUIMessageFromID("E1032A209");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P104");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P105");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A210");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P106");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A154");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P107");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A204");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P108");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_PARENTS_IN_VANCOUVER_CALL, 1);
      break;
    case 19:
      task_394 = 320;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032P109");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P110");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A211");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P111");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P112");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_394 = 321;
      task_398 = Script_ShowUIMessageFromID("E1032A212");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P113");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A158");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P114");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P115");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A213");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P116");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_CALL_NAOYUKI_AND_ICHIRO_CALL, 1);
      break;
    case 25:
      task_394 = 330;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P117");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A214");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P118");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A154");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P119");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P120");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_394 = 331;
      task_398 = Script_ShowUIMessageFromID("E1032A215");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P121");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A163");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P123");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A217");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_TRIP_INVITATION_CALL, 1);
      break;
    case 20:
      task_394 = 340;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P010");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P124");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P125");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A218");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P126");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A154");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P127");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A213");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P128");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A204");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P116");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_LAPIS_FORTUNETELLER_CALL, 1);
      break;
    case 23:
      task_394 = 350;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032P109");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P117");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A214");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P129");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P130");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P131");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A213");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A219");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P132");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      SCNF_WaitFrames_0000(60);
      task_394 = 351;
      task_398 = Script_ShowUIMessageFromID("E1032A153");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P133");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A220");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P134");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A204");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P116");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_NURSERY_TEACHER_CALL, 1);
      break;
    case 22:
      task_394 = 360;
      Script_scenCreateTask(script_1F4CE, 0);
      task_398 = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P010");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A221");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P135");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P136");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A222");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P137");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P138");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A223");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P139");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_394 = 361;
      task_398 = Script_ShowUIMessageFromID("E1032A224");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      SCNF_WaitFrames_0000(60);
      task_398 = Script_ShowUIMessageFromID("E1032P140");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A225");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A226");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032P004");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_FIRST_MEETING_CALL, 1);
      break;
  }
  return;
}

__attribute__((scn_string_bindings(5, data_416A0, 7, data_416B4, 8, data_416BE, 21, data_41740, 22, data_4174A, 30, data_4179A, 32, data_417AE, 33, data_417B8, 48, data_4184E, 49, data_41858)))
static void script_1CEDB(void) {
  if (Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f))) {
    task_394 = 250;
    Script_scenCreateTask(script_1F4CE, 0);
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_FC == 0) {
        task_398 = Script_ShowUIMessageFromID("E1032E001");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      } else if (local_FC == 1) {
        task_398 = Script_ShowUIMessageFromID("E1032E002");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      } else {
        task_398 = Script_ShowUIMessageFromID("E1032E003");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      }
    }
    task_398 = Script_ShowUIMessageFromID("E1032A010");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032E004");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      switch (local_FC) {
        case 0:
          task_398 = Script_ShowUIMessageFromID("E1032A011");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
          task_398 = Script_ShowUIMessageFromID("E1032E005");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
          task_398 = Script_ShowUIMessageFromID("E1032A012");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
          break;
        case 1:
          task_398 = Script_ShowUIMessageFromID("E1032A013");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
          task_398 = Script_ShowUIMessageFromID("E1032E006");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
          task_398 = Script_ShowUIMessageFromID("E1032A014");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
          break;
        case 2:
          task_398 = Script_ShowUIMessageFromID("E1032A015");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
          task_398 = Script_ShowUIMessageFromID("E1032E007");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
          task_398 = Script_ShowUIMessageFromID("E1032A016");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
          break;
      }
    }
    task_398 = Script_ShowUIMessageFromID("E1032E008");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_394 = 251;
    task_398 = Script_ShowUIMessageFromID("E1032E009");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032A017");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032E010");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_FC == 0) {
        task_398 = Script_ShowUIMessageFromID("E1032E011");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      } else if (local_FC == 1) {
        task_398 = Script_ShowUIMessageFromID("E1032E012");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      } else {
        task_398 = Script_ShowUIMessageFromID("E1032E013");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      }
    }
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_FC == 0) {
        task_398 = Script_ShowUIMessageFromID("E1032A018");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      } else if (local_FC == 1) {
        task_398 = Script_ShowUIMessageFromID("E1032A019");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      } else {
        task_398 = Script_ShowUIMessageFromID("E1032A020");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      }
    }
    task_398 = Script_ShowUIMessageFromID("E1032A021");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  } else {
    task_394 = 260;
    Script_scenCreateTask(script_1F4CE, 0);
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_FC == 0) {
        task_398 = Script_ShowUIMessageFromID("E1032F001");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      } else if (local_FC == 1) {
        task_398 = Script_ShowUIMessageFromID("E1032F002");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      } else {
        task_398 = Script_ShowUIMessageFromID("E1032F003");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      }
    }
    task_398 = Script_ShowUIMessageFromID("E1032A022");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032F004");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      switch (local_FC) {
        case 0:
          task_398 = Script_ShowUIMessageFromID("E1032A011");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
          task_398 = Script_ShowUIMessageFromID("E1032F005");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
          task_398 = Script_ShowUIMessageFromID("E1032A012");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
          break;
        case 1:
          task_398 = Script_ShowUIMessageFromID("E1032A013");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
          task_398 = Script_ShowUIMessageFromID("E1032F006");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
          task_398 = Script_ShowUIMessageFromID("E1032A023");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
          break;
        case 2:
          task_398 = Script_ShowUIMessageFromID("E1032A024");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
          task_398 = Script_ShowUIMessageFromID("E1032F007");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
          task_398 = Script_ShowUIMessageFromID("E1032A025");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
          task_398 = Script_ShowUIMessageFromID("E1032A026");
          while (Script_convGetIsActive((const void *)task_398)) {
            SCNF_WaitFrames_0000(1);
          }
          task_398 = 0;
          break;
      }
    }
    task_394 = 261;
    task_398 = Script_ShowUIMessageFromID("E1032F008");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032F009");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_FC == 0) {
        task_398 = Script_ShowUIMessageFromID("E1032F010");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      } else if (local_FC == 1) {
        task_398 = Script_ShowUIMessageFromID("E1032F011");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      } else {
        task_398 = Script_ShowUIMessageFromID("E1032F012");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      }
    }
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_FC == 0) {
        task_398 = Script_ShowUIMessageFromID("E1032F013");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      } else if (local_FC == 1) {
        task_398 = Script_ShowUIMessageFromID("E1032F014");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      } else {
        task_398 = Script_ShowUIMessageFromID("E1032F015");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      }
    }
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
      if (local_FC == 0) {
        task_398 = Script_ShowUIMessageFromID("E1032A019");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      } else {
        task_398 = Script_ShowUIMessageFromID("E1032A018");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      }
    }
    task_398 = Script_ShowUIMessageFromID("E1032A027");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  }
  return;
}

static int8_t script_1D9A3(void) {
  int8_t local_FF;
  local_FF = 0;
  task_394 = 55;
  Script_scenCreateTask(script_1F4CE, 0);
  local_FF = script_1D9CB();
  return local_FF;
}

static int8_t script_1D9CB(void) {
  int32_t local_FC;
  int32_t local_F8;
  int8_t local_F7;
  local_F7 = 0;
  task_398 = Script_ShowUIMessageFromID("E1032A034");
  while (Script_convGetIsActive((const void *)task_398)) {
    SCNF_WaitFrames_0000(1);
  }
  task_398 = 0;
  if (Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f))) {
    local_F8 = 0;
  } else {
    local_F8 = 2;
  }
  script_1DAE6(local_F8);
  local_FC = script_1DC38();
  script_1DC53(local_FC);
  if (script_1DBCF(local_F8, local_FC)) {
    if (local_F8 == 0) {
      local_F8 = 2;
    } else {
      local_F8 = 0;
    }
    task_394 = 56;
    Script_mapmSetCamFlags(0, 0);
    Script_charaVisUtils(CHARA_AKIR, 1);
    script_1DAE6(local_F8);
    local_FC = script_1DC38();
    script_1DC53(local_FC);
    Script_mapmSetCamFlags(0, 1);
    Script_charaVisUtils(CHARA_AKIR, 0);
    if (script_1DBCF(local_F8, local_FC)) {
      local_F7 = script_1E029();
    } else {
      script_1DD3C(local_FC);
    }
  } else {
    script_1DD3C(local_FC);
  }
  return local_F7;
}

static void script_1DAE6(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      task_398 = Script_ShowUIMessageFromID("E1032H005");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      break;
    case 1:
      task_398 = Script_ShowUIMessageFromID("E1032H004");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      break;
    case 2:
      task_398 = Script_ShowUIMessageFromID("E1032H009");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      break;
    case 3:
      task_398 = Script_ShowUIMessageFromID("E1032H006");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      break;
  }
  return;
}

static int32_t script_1DBCF(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = -1;
  local_F8 = arg1;
  switch (local_F8) {
    case 5:
      local_FC = 1;
      break;
    case 6:
      local_FC = 2;
      break;
    case 7:
      local_FC = 3;
      break;
    case 8:
      local_FC = 0;
      break;
  }
  if (arg0 == local_FC) {
    (void)1;
  } else {
    (void)0;
  }
  end:;
}

static int32_t script_1DC38(void) {
  return SCNF_RequestActionSelectorFromParams_0008(7, 8, 6, 5, 300, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD);
}

static void script_1DC53(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 5:
      task_398 = Script_ShowUIMessageFromID("E1032A042");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      break;
    case 6:
      task_398 = Script_ShowUIMessageFromID("E1032A040");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      break;
    case 7:
      task_398 = Script_ShowUIMessageFromID("E1032A043");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      break;
    case 8:
      task_398 = Script_ShowUIMessageFromID("E1032A041");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      break;
  }
  return;
}

__attribute__((scn_string_bindings(1, data_418D0, 2, data_418DA, 3, data_418E4, 4, data_418EE)))
static void script_1DD3C(int32_t arg0) {
  int32_t local_FC;
  task_394 = 58;
  script_1DF61(1);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 714)) {
    if ((arg0 == 5) | (arg0 == 6) | (arg0 == 7) | (arg0 == 8)) {
      int32_t local_F8;
      local_F8 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f));
      if (local_F8 == 0) {
      } else if (local_F8 == 1) {
        task_398 = Script_ShowUIMessageFromID("E1032A045");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      } else if (local_F8 == 2) {
        task_398 = Script_ShowUIMessageFromID("E1032A046");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      } else {
        task_398 = Script_ShowUIMessageFromID("E1032A047");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      }
    } else {
      int32_t local_F8;
      local_F8 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_F8 == 0) {
      } else if (local_F8 == 1) {
        task_398 = Script_ShowUIMessageFromID("E1032A046");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      } else {
        task_398 = Script_ShowUIMessageFromID("E1032A047");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      }
    }
  } else {
    task_398 = Script_ShowUIMessageFromID("E1032A044");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  }
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 714, 1);
  if (Script_StepUtils(GET_STEP_HIGHER, 0) == 9) {
    local_FC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 9);
    local_FC = local_FC + 1;
    Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 9, local_FC);
  }
  return;
}

static void script_1DF2A(void) {
  task_398 = Script_ShowUIMessageFromID("E1032A051");
  while (Script_convGetIsActive((const void *)task_398)) {
    SCNF_WaitFrames_0000(1);
  }
  task_398 = 0;
  SCNF_WaitFrames_0000(10);
  return;
}

static void script_1DF61(int32_t arg0) {
  Script_executeAicaCommand(525739, 0, 0);
  SCNF_WaitFrames_0000(10);
  if (arg0) {
    task_398 = Script_ShowUIMessageFromID("E1032A039");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
  }
  Script_executeAicaCommand(394667, 0, 0);
  SCNF_WaitFrames_0000(15);
  Script_executeAicaCommand(460203, 0, 0);
  SCNF_WaitFrames_0000(15);
  Script_executeAicaCommand(394667, 0, 0);
  SCNF_WaitFrames_0000(15);
  Script_executeAicaCommand(460203, 0, 0);
  SCNF_WaitFrames_0000(15);
  Script_executeAicaCommand(394667, 0, 0);
  SCNF_WaitFrames_0000(15);
  Script_executeAicaCommand(460203, 0, 0);
  SCNF_WaitFrames_0000(15);
  return;
}

__attribute__((scn_string_bindings(0, data_41916, 1, data_41920, 2, data_4192A, 3, data_41934, 4, data_4193E, 5, data_41948, 6, data_41952, 7, data_4195C, 8, data_41966, 10, data_4197A, 11, data_41984, 15, data_419AC, 16, data_419B6, 17, data_419C0, 18, data_419CA, 19, data_419D4, 20, data_419DE, 21, data_419E8, 22, data_419F2, 23, data_419FC, 24, data_41A06, 25, data_41A10, 26, data_41A1A, 27, data_41A24, 28, data_41A2E, 29, data_41A38, 33, data_41A60, 34, data_41A6A, 35, data_41A74, 36, data_41A7E, 37, data_41A88, 38, data_41A92, 39, data_41A9C, 40, data_41AA6, 41, data_41AB0, 42, data_41ABA, 43, data_41AC4, 44, data_41ACE, 45, data_41AD8, 46, data_41AE2, 47, data_41AEC, 48, data_41AF6, 49, data_41B00, 52, data_41B1E, 53, data_41B28, 54, data_41B32, 59, data_41B64)))
static int8_t script_1E029(void) {
  int32_t local_FC;
  int32_t local_F8;
  int8_t local_F7;
  int32_t local_F0;
  local_F8 = Script_StepUtils(GET_STEP_HIGHER, 0) * 10 + Script_StepUtils(GET_STEP_LOWER, 0);
  local_F7 = 0;
  task_394 = 57;
  local_FC = Script_StepUtils(GET_STEP_HIGHER, 0);
  if (local_FC >= 17) {
    task_398 = Script_ShowUIMessageFromID("E1032H007");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032A120");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032H010");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032A066");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032H011");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    SCNF_WaitFrames_0000(60);
    if (((local_F8 >= 170) & (local_F8 <= 193)) | ((local_F8 >= 220) & (local_F8 <= 241))) {
      local_F0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
      if (local_F0 == 0) {
        task_398 = Script_ShowUIMessageFromID("E1032J001");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
        task_398 = Script_ShowUIMessageFromID("E1032A019");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
        task_398 = Script_ShowUIMessageFromID("E1032A131");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
        task_398 = Script_ShowUIMessageFromID("E1032J010");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
        task_398 = Script_ShowUIMessageFromID("E1032J013");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      } else {
        task_398 = Script_ShowUIMessageFromID("E1032J001");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
        task_398 = Script_ShowUIMessageFromID("E1032A019");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
        task_398 = Script_ShowUIMessageFromID("E1032A134");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
        task_398 = Script_ShowUIMessageFromID("E1032J015");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
        task_398 = Script_ShowUIMessageFromID("E1032A135");
        while (Script_convGetIsActive((const void *)task_398)) {
          SCNF_WaitFrames_0000(1);
        }
        task_398 = 0;
      }
      script_1DF61(0);
    } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 546)) {
      task_398 = Script_ShowUIMessageFromID("E1032J001");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032J018");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A163");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032J019");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032J020");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      Script_executeAicaCommand(525739, 0, 0);
      SCNF_WaitFrames_0000(10);
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED, 1);
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 546, 0);
    } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED)) {
      task_398 = Script_ShowUIMessageFromID("E1032J016");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A161");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032J017");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A162");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      Script_executeAicaCommand(525739, 0, 0);
      SCNF_WaitFrames_0000(10);
    } else {
      task_398 = Script_ShowUIMessageFromID("E1032J001");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A131");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032J010");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A132");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032J011");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032J012");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED, 1);
    }
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS08_SCENE_EXIT_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS8A_POST_MS08_FOLLOWUP_COMPLETE) == 0)) {
    task_394 = 64;
    task_398 = Script_ShowUIMessageFromID("E1032H007");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032A120");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032H010");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032A066");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032H011");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    SCNF_WaitFrames_0000(60);
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 546)) {
      task_394 = 65;
      task_398 = Script_ShowUIMessageFromID("E1032J001");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032J018");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A163");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032J019");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032J020");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      Script_executeAicaCommand(525739, 0, 0);
      SCNF_WaitFrames_0000(10);
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED, 1);
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 546, 0);
    } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED)) {
      task_394 = 66;
      task_398 = Script_ShowUIMessageFromID("E1032J016");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A161");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032J017");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A162");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      Script_executeAicaCommand(525739, 0, 0);
      SCNF_WaitFrames_0000(10);
    } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_SECRET_BOX_OPENING_COMPLETE)) {
      task_398 = Script_ShowUIMessageFromID("E1032J001");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A067");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032J002");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED, 1);
    } else {
      task_398 = Script_ShowUIMessageFromID("E1032J001");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032J003");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A069");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032J004");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
      task_398 = Script_ShowUIMessageFromID("E1032A070");
      while (Script_convGetIsActive((const void *)task_398)) {
        SCNF_WaitFrames_0000(1);
      }
      task_398 = 0;
    }
  } else {
    local_F7 = 1;
    task_398 = Script_ShowUIMessageFromID("E1032H007");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032A048");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032H008");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_394 = 63;
    task_398 = Script_ShowUIMessageFromID("E1032A049");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    task_398 = Script_ShowUIMessageFromID("E1032A050");
    while (Script_convGetIsActive((const void *)task_398)) {
      SCNF_WaitFrames_0000(1);
    }
    task_398 = 0;
    script_1DF61(1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 714, 1);
  }
  return local_F7;
}

static void script_1ED06(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_EVT_GetPosition(SHEN_NAME('TBOX'), -1, &local_F4, 0);
  Script_mtwkGetSetRotation(SHEN_NAME('TBOX'), &local_E8, 0);
  (&local_E8)->y = (&local_E8)->y + -1820;
  local_F4.x = (&local_F4)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y) * 1.0f;
  (&local_F4)->z = (&local_F4)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y) * 1.0f;
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_30);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_scenCharaSubmitMoveTargetRequest(CHARA_AKIR, &local_F4, (&local_E8)->y, MN_AKI_AKI_STAND_LP, 0);
  while (Script_IsMoveTargetRequestActive(CHARA_AKIR) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_1EDC9(void) {
  Vector3i local_F4;
  Vector3i local_E8;
  Vector3 local_DC;
  Vector3i local_D0;
  Vector3i local_C4;
  script_201BF();
  Script_InitializeOsag(CHARA_AKIR);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
  Script_PlayCharaMotion(CHARA_AKIR, 32001, 23.0f, 53.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  task_394 = 62;
  Script_scenCreateTask(script_1F4CE, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(263851, 0, 0);
  local_F4.x = 0;
  (&local_F4)->y = 1307;
  (&local_F4)->z = 0;
  local_E8.x = 0;
  (&local_E8)->y = -601;
  (&local_E8)->z = 0;
  Script_PlayCharaMotion(CHARA_AKIR, 32001, 53.0f, 56.0f, -1.0f, 0.2f, 0);
  for (;;) {
    Script_charaPosRotUtils(SHEN_NAME('TBOX'), 16, 0, &local_E8, CHARA_PART_Add);
    Script_charaPosRotUtils(SHEN_NAME('TBOX'), 21, 0, &local_F4, CHARA_PART_Add);
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  local_F4.x = 0;
  (&local_F4)->y = 490;
  (&local_F4)->z = 0;
  local_E8.x = 0;
  (&local_E8)->y = -225;
  (&local_E8)->z = 0;
  Script_PlayCharaMotion(CHARA_AKIR, 32001, 56.0f, 63.0f, -1.0f, 0.5f, 0);
  for (;;) {
    Script_charaPosRotUtils(SHEN_NAME('TBOX'), 16, 0, &local_E8, CHARA_PART_Add);
    Script_charaPosRotUtils(SHEN_NAME('TBOX'), 21, 0, &local_F4, CHARA_PART_Add);
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, 32001, 63.0f, 90.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_EVT_GetPosition(SHEN_NAME('TBOX'), -1, &local_DC, 0);
  Script_mtwkGetSetRotation(SHEN_NAME('TBOX'), &local_D0, 0);
  local_DC.x = (&local_DC)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_D0)->y) * 1.5f;
  (&local_DC)->z = (&local_DC)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_D0)->y) * 1.5f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_DC);
  (&local_D0)->y = (&local_D0)->y + 0x8000;
  (&local_C4)->y = (&local_D0)->y;
  Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_C4, 0);
  Script_charaSetRefboardFlagTo(task_37C, 0);
  Script_charaSetRefboardFlagTo(task_380, 0);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_scenToggleActiveFor(CHARA_AKIR, 1);
  Script_charaPosRotUtils(SHEN_NAME('TBOX'), 21, 0, &task_40C, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('TBOX'), 16, 0, &task_418, CHARA_PART_Set);
  task_394 = -1;
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_SetCameraFieldOfView(10922);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_1F0F1(void) {
  Script_seqEnterEvtCam(6220, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
  script_1ED06();
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_30);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, 32002, 10.0f, 50.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_1F176(void) {
  Script_PlayCharaMotion(CHARA_AKIR, 32002, 20.0f, 50.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x20);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x20);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_10);
  SCNF_WaitFrames_0000(10);
  return;
}

static void script_1F205(void) {
  Vector3i local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  Script_seqEnterEvtCam(6220, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
  Script_ryoControl(PCCTL_DisablePlayerControl);
  script_1ED06();
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 4);
  Script_PlayCharaMotion(CHARA_AKIR, 32002, 25.0f, 51.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, 32002, 51.0f, 85.0f, -1.0f, 1.0f, 0);
  Script_charaPosRotUtils(SHEN_NAME('TBOX'), 21, 0, &task_40C, CHARA_PART_Get);
  Script_charaPosRotUtils(SHEN_NAME('TBOX'), 16, 0, &task_418, CHARA_PART_Get);
  local_F4.x = 0;
  (&local_F4)->y = 713;
  (&local_F4)->z = 0;
  local_E8.x = 0;
  (&local_E8)->y = -328;
  (&local_E8)->z = 0;
  Script_executeAicaCommand(263851, 0, 0);
  local_E4 = Script_scenCreateTask(script_1F41B, 0);
  for (;;) {
    Script_charaPosRotUtils(SHEN_NAME('TBOX'), 16, 0, &local_E8, CHARA_PART_Add);
    Script_charaPosRotUtils(SHEN_NAME('TBOX'), 21, 0, &local_F4, CHARA_PART_Add);
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, 32002, 85.0f, 95.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_charaPosRotUtils(SHEN_NAME('TBOX'), 21, 0, &task_40C, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('TBOX'), 16, 0, &task_418, CHARA_PART_Set);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_srchControl((void *)local_E4);
  Script_motiSetAngFlags(CHARA_AKIR, 0);
  Script_seqEnterEvtCam(6219, task_378, task_378);
  SCNF_WaitFrames_0000(1);
  return;
}

static int32_t script_1F41B(void) {
  Vector3i local_F4;
  Vector3i local_E8;
  Vector3 local_DC;
  Vector3 local_D0;
  int32_t local_CC;
  Script_motiSetAngFlags(CHARA_AKIR, 1);
  for (;;) {
    Script_charaPosRotUtils(SHEN_NAME('TBOX'), 21, &local_DC, &local_F4, CHARA_PART_Get);
    Script_charaPosRotUtils(SHEN_NAME('TBOX'), 16, &local_D0, &local_E8, CHARA_PART_Get);
    Script_charaSetupXForm(SHEN_NAME('TBOX'), 21, &local_DC);
    local_CC = (&local_F4)->y + (&local_E8)->y + 2730;
    local_DC.x = (&local_DC)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_CC) * 0.6f;
    (&local_DC)->y = 1.2f;
    (&local_DC)->z = (&local_DC)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_CC) * 0.6f;
    Script_SetMotnFlags(CHARA_AKIR, 17, &local_DC, 0x38000000);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_1F4CE(void) {
  int32_t local_FC;
  for (;;) {
    local_FC = task_394;
    switch (local_FC) {
      case -1:
        goto L01FE42;
        break;
      case 0:
        break;
      case 51:
        Script_seqEnterEvtCam(6222, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 53:
        Script_seqEnterEvtCam(6222, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 54:
        Script_seqEnterEvtCam(6222, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 55:
        Script_seqEnterEvtCam(6223, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 56:
        Script_seqEnterEvtCam(6224, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 57:
        Script_seqEnterEvtCam(6225, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 58:
        Script_seqEnterEvtCam(6226, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 62:
        Script_seqEnterEvtCam(6221, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 63:
        Script_seqEnterEvtCam(6227, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 64:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 65:
        Script_seqEnterEvtCam(6233, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 66:
        Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 70:
        local_FC = task_3A4;
        switch (local_FC) {
          case 0:
            Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
            break;
          case 1:
            Script_seqEnterEvtCam(6229, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
            break;
          case 2:
            Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
            break;
          case 3:
            Script_seqEnterEvtCam(6231, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
            break;
          case 4:
            Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
            break;
          case 5:
            Script_seqEnterEvtCam(6233, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
            break;
          case 6:
            Script_seqEnterEvtCam(6234, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
            break;
        }
        task_3A4 = task_3A4 + 1;
        if (task_3A4 > 6) {
          task_3A4 = 0;
        }
        break;
      case 80:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 81:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 82:
        Script_seqEnterEvtCam(6234, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 90:
        Script_seqEnterEvtCam(6233, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 100:
        Script_seqEnterEvtCam(6234, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 101:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 110:
        Script_seqEnterEvtCam(6231, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 111:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 120:
        Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 130:
        Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 131:
        Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 140:
        Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 141:
        Script_seqEnterEvtCam(6229, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 150:
        Script_seqEnterEvtCam(6234, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 151:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 160:
        Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 161:
        Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 162:
        Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 163:
        Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 170:
        Script_seqEnterEvtCam(6234, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 180:
        Script_seqEnterEvtCam(6233, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 190:
        Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 191:
        Script_seqEnterEvtCam(6229, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 200:
        Script_seqEnterEvtCam(6229, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 210:
        Script_seqEnterEvtCam(6231, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 220:
        Script_seqEnterEvtCam(6233, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 230:
        Script_seqEnterEvtCam(6234, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 231:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 240:
        Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 250:
        Script_seqEnterEvtCam(6231, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 251:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 260:
        Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 261:
        Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 270:
        Script_seqEnterEvtCam(6234, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 280:
        Script_seqEnterEvtCam(6231, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 281:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 290:
        Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 300:
        Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 301:
        Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 310:
        Script_seqEnterEvtCam(6234, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 311:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 320:
        Script_seqEnterEvtCam(6234, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 321:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 330:
        Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 331:
        Script_seqEnterEvtCam(6229, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 340:
        Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 350:
        Script_seqEnterEvtCam(6231, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 351:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 360:
        Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 361:
        Script_seqEnterEvtCam(6229, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 370:
        Script_seqEnterEvtCam(6231, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 371:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 380:
        Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 381:
        Script_seqEnterEvtCam(6229, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 390:
        Script_seqEnterEvtCam(6234, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 391:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 400:
        Script_seqEnterEvtCam(6233, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 410:
        Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 411:
        Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
    }
    if (task_394) {
      task_394 = 0;
    }
    SCNF_WaitFrames_0000(1);
  }
  L01FE42:;
  task_394 = 0;
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(1, data_41BA6)))
static void script_1FE50(void) {
  Vector4 local_F0;
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  SCNF_WaitFrames_0000(1);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JD00/", "TBOX") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(1);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_shenClearFreeState(0);
  local_F0.x = 0.8f;
  (&local_F0)->y = 0.8f;
  (&local_F0)->z = 0.8f;
  (&local_F0)->w = 0.8f;
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(task_37C, 1);
  Script_charaSetRefboardFlagTo(task_380, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &local_F0);
  Script_SetCharaRefBoardPos(task_37C, &local_F0);
  Script_SetCharaRefBoardPos(task_380, &local_F0);
  if (task_3A1 < 0) {
    script_1F0F1();
  } else {
    script_1F205();
  }
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_shenClearFreeState(1);
  return;
}

__attribute__((scn_string_bindings(0, data_41BAB)))
static void script_1FF4F(void) {
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_shenClearFreeState(0);
  if (task_3A1 < 0) {
    script_1F176();
  } else {
    script_1EDC9();
  }
  SCNF_WaitFrames_0000(1);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_shenClearFreeState(1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "TBOX");
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  SCNF_WaitFrames_0000(10);
  return;
}

static void script_1FFC2(void) {
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  {
    Vector3 local_F4;
    Vector3i local_E8;
    Script_EVT_GetPosition(task_378, -1, &local_F4, 0);
    Script_mtwkGetSetRotation(task_378, &local_E8, 0);
    local_F4.x = (&local_F4)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y) * 0.52f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = (&local_F4)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y) * 0.52f;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
    {
      Vector3i local_DC;
      (&local_DC)->y = (&local_E8)->y - 1820;
      Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_DC, 0);
    }
  }
  Script_InitializeOsag(CHARA_AKIR);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, 32003, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_3FA7C, 1);
  Script_ConfigHandMotionIK(CHARA_AKIR, 0, 0, 0, 1);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_3FA7C, 1);
  Script_ConfigHandMotionIK(CHARA_AKIR, 1, 0, 0, 1);
  Script_scenCreateTask(script_203CC, 0);
  SCNF_WaitFrames_0000(1);
  Script_TELMUtils(TELM_CMD_Cleanup, task_378);
  Script_TELMUtils(TELM_CMD_SetTargetTaskAndMode, task_378, 0x52494B41, 3);
  {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = -0.03f;
    (&local_F4)->y = -0.01f;
    (&local_F4)->z = -0.02f;
    local_E8.x = 59162;
    (&local_E8)->y = 7824;
    (&local_E8)->z = 62441;
    Script_TELMUtils(TELM_CMD_SetCustomReceiverOffset, task_378, &local_F4);
    Script_TELMUtils(TELM_CMD_SetCustomReceiverEuler, task_378, &local_E8);
  }
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_executeAicaCommand(591275, 0, 0);
  return;
}

static void script_201AB(void) {
  task_394 = -1;
  Script_TELMUtils(TELM_CMD_Cleanup, task_378);
  return;
}

static void script_201BF(void) {
  Vector3 local_F4;
  Script_EVT_GetPosition(task_378, -1, &local_F4, 0);
  {
    Vector3i local_E8;
    Script_mtwkGetSetRotation(task_378, &local_E8, 0);
    local_F4.x = (&local_F4)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y) * 0.5f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = (&local_F4)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y) * 0.5f;
  }
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  {
    Vector3i local_E8;
    Vector3i local_DC;
    Script_mtwkGetSetRotation(SHEN_NAME('TBOX'), &local_E8, 0);
    (&local_DC)->y = (&local_E8)->y;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_DC, 0);
  }
  Script_InitializeOsag(CHARA_AKIR);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static void script_20287(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  task_409 = 1;
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 4);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_20);
  Script_PlayCharaMotion(CHARA_AKIR, 32004, -1.0f, 55.0f, -1.0f, 1.5f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_3FB60, 5);
  Script_TELMUtils(TELM_CMD_ResetPeerState, task_378);
  Script_executeAicaCommand(67243, 0, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 32004, 55.0f, -1.0f, -1.0f, 1.5f, 0);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  local_F4.x = 0.0f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.0f;
  local_E8.x = 0;
  (&local_E8)->y = 0;
  (&local_E8)->z = 0;
  Script_TELMUtils(TELM_CMD_SetCustomReceiverOffset, task_378, &local_F4);
  Script_TELMUtils(TELM_CMD_SetCustomReceiverEuler, task_378, &local_E8);
  return;
}

static int32_t script_203CC(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = -0.164926f;
  (&local_F4)->y = 1.545938f;
  (&local_F4)->z = -0.138711f;
  local_E8.x = -8009;
  (&local_E8)->y = 4187;
  (&local_E8)->z = -2184;
  task_409 = 0;
  for (;;) {
    if (task_409) {
      break;
    }
    Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &local_E8);
    Script_SetMotnFlags(CHARA_AKIR, 11, &local_F4, 0x39000000);
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_2044F(int32_t arg0) {
  float local_FC;
  float local_F8;
  float local_F4;
  int32_t unused_E8[3];
  Vector3i local_DC;
  int32_t local_D8;
  int32_t local_D4;
  int32_t local_D0;
  uint8_t unused_CF;
  int8_t local_CE;
  uint8_t unused_CC[2];
  int32_t local_C8;
  if ((arg0 != 0x4742454B) & (arg0 != 0x4242454B)) {
    Script_PlayerStatsUtils(PS_SetCharaItemCount, arg0, Script_PlayerStatsUtils(PS_GetCharaItemCount, arg0) + 1);
  }
  local_C8 = arg0;
  switch (local_C8) {
    case 0x4C42454B:
      local_FC = -119.2f;
      local_F8 = 21.4f;
      local_F4 = 350.0f;
      local_D4 = (int32_t)data_41BB0;
      local_D0 = 4000;
      break;
    case 0x594A4E53:
      local_FC = -119.2f;
      local_F8 = 71.9f;
      local_F4 = 350.0f;
      local_D4 = (int32_t)data_41BB7;
      local_D0 = 5000;
      break;
    case 0x4A42454B:
      local_FC = 137.8f;
      local_F8 = 20.0f;
      local_F4 = 160.0f;
      local_D4 = (int32_t)data_41BBE;
      local_D0 = 6000;
      break;
    case 0x4942454B:
      local_FC = 32.7f;
      local_F8 = 85.3f;
      local_F4 = 215.0f;
      local_D4 = (int32_t)data_41BC5;
      local_D0 = 3000;
      break;
    case 0x4742454B:
      local_FC = 8.79f;
      local_F8 = 132.36f;
      local_F4 = 125.0f;
      local_D4 = (int32_t)data_41BCC;
      local_D0 = 1000;
      break;
    case 0x4242454B:
      local_FC = -124.5f;
      local_F8 = -8.4f;
      local_F4 = 350.0f;
      local_D4 = (int32_t)data_41BD3;
      local_D0 = 2000;
      break;
  }
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  script_E05F();
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmGetCharaFlags(CHARA_AKIR, MOTM_PoseNone, SCEN_Write);
  Script_AddCharacterHandler(3, 0, 0x796D7564);
  Script_LoadVoiceFile(1, (const char *)local_D4);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  local_D8 = script_E948(arg0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  script_A671(0x796D7564, local_FC, 0.0f, local_F8);
  script_A699(0x796D7564, 0.0f, local_F4, 0.0f);
  Script_MoveObjectToObject(CHARA_AKIR, 0x796D7564);
  script_A671(CHARA_AKIR, -1.04f, 0.0f, -3.29f);
  script_A699(CHARA_AKIR, 0.0f, 135.0f, 0.0f);
  Script_MoveObjectToObject(arg0, 0x796D7564);
  script_A671(arg0, 0.0f, 0.0f, 0.0f);
  script_A699(arg0, 0.0f, 0.0f, 0.0f);
  Script_scenCreateTask(script_20A11, 3, local_D0, CHARA_AKIR, arg0);
  if (((arg0 == 0x4742454B) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_BOX_RESCUE_ROUTE_UNLOCKED) == 0)) | ((arg0 == 0x4242454B) & (Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 14) == 1))) {
    script_20889();
    (void)((local_CE = 0) & 1);
  } else {
    script_20942();
    (void)((local_CE = -1) & 1);
  }
  task_4A4 = 0;
  Script_MoveObjectToObject(arg0, -1);
  script_EA10(arg0, local_D8);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  Script_MoveObjectToObject(CHARA_AKIR, -1);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_DC, 0x42000000);
  Script_EVT_SetRotation(CHARA_AKIR, 0x78000000, &local_DC, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_InitializeOsag(CHARA_AKIR);
  script_E07A();
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  Script_SetGameStateTo(AUTH_None);
  Script_convSetCamInactiveFlag(-1);
  Script_SetMapControlTaskFlag(0x796D7564, 1);
  Script_LoadVoiceFile(0, (const char *)local_D4);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  return;
}

static void script_20889(void) {
  int32_t local_FC;
  task_4A4 = 0;
  for (;;) {
    local_FC = task_4A4;
    switch (local_FC) {
      case 0:
        Script_seqEnterEvtCam(210, 0x796D7564, 0x796D7564);
        break;
      case 149:
        Script_seqEnterEvtCam(211, 0x796D7564, 0x796D7564);
        break;
      case 235:
        Script_seqEnterEvtCam(212, 0x796D7564, 0x796D7564);
        break;
      case 236:
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_BOX_RESCUE_ROUTE_UNLOCKED) == 0) {
          Script_scenCreateTask(script_20D31, 0);
        }
        break;
      case 445:
        goto L020941;
        break;
    }
    task_4A4 = task_4A4 + 1;
    SCNF_WaitFrames_0000(1);
  }
  L020941:;
  return;
}

static void script_20942(void) {
  int32_t local_FC;
  {
    int32_t local_F8;
    local_F8 = task_4AC;
    switch (local_F8) {
      case 0:
        local_FC = 210;
        break;
      case 1:
        local_FC = 213;
        break;
      case 2:
        local_FC = 215;
        break;
    }
  }
  task_4AC = task_4AC + 1;
  if (task_4AC > 2) {
    task_4AC = 0;
  }
  task_4A4 = 0;
  for (;;) {
    int32_t local_F8;
    local_F8 = task_4A4;
    switch (local_F8) {
      case 0:
        Script_seqEnterEvtCam(local_FC, 0x796D7564, 0x796D7564);
        break;
      case 149:
        Script_seqEnterEvtCam(local_FC + 1, 0x796D7564, 0x796D7564);
        break;
      case 235:
        goto L020A10;
        break;
    }
    task_4A4 = task_4A4 + 1;
    SCNF_WaitFrames_0000(1);
  }
  L020A10:;
  return;
}

static int32_t script_20A11(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  Vector3 local_F0;
  for (;;) {
    local_FC = task_4A4;
    switch (local_FC) {
      case 1:
        Script_charaSetRefboardFlagTo(arg1, 1);
        Script_SetCharaRefBoardPos(arg1, &data_41BDC);
        Script_charaSetRefboardFlagTo(arg2, 1);
        Script_SetCharaRefBoardPos(arg2, &data_41BDC);
        Script_PlayCharaMotion(arg1, 28680, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_PlayCharaMotion(arg2, 28675, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        break;
      case 2:
        Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
        break;
      case 15:
        Script_PlayCharaMotion(arg2, 28676, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetCharaMotionPlaybackRate(arg2, 1.5f);
        if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg2) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
          Script_EVT_GetPosition(CHARA_AKIR, 4, &local_F0, ENTITY_XFORM_WORLD_SPACE);
          Script_SetLookPointDetails(arg2, 16, &local_F0, 2);
          Script_FaceLookAtTargetUtils(arg2, FACE_LookAtPosition, &local_F0, 30);
        }
        if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg1) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg2)) {
          Script_EVT_GetPosition(arg2, 4, &local_F0, ENTITY_XFORM_WORLD_SPACE);
          Script_SetLookPointDetails(arg1, 16, &local_F0, 2);
          Script_FaceLookAtTargetUtils(arg1, FACE_LookAtPosition, &local_F0, 30);
        }
        script_21A8B(arg0, 200, 10);
        break;
      case 70:
        script_21A8B(arg0, 200, 20);
        break;
      case 88:
        Script_PlayCharaMotion(arg2, 28673, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetCharaMotionPlaybackRate(arg2, 1.5f);
        break;
      case 140:
        if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg1)) {
          Script_SetCharaLookAtPosition(arg1, -10, 0, 0);
          Script_FaceLookAtTargetUtils(arg1, FACE_LookReset);
        }
        break;
      case 148:
        Script_PlayCharaMotion(arg1, 28681, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        script_21A8B(arg0, 200, 30);
        goto L020C3B;
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  L020C3B:;
  if (((arg2 == 0x4742454B) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_BOX_RESCUE_ROUTE_UNLOCKED) == 0)) | ((arg2 == 0x4242454B) & (Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 14) == 1))) {
    Script_charaToggleMotEnabled(arg1, 0);
    Script_PlayCharaMotion(arg1, 28681, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    for (;;) {
      if (Script_IsCharaMotionFinished(arg1)) {
        break;
      }
      Script_RotateNodeUtils(arg1, &data_39DC0, 0);
      SCNF_WaitFrames_0000(1);
    }
    script_21A8B(arg0, 100, 10);
    Script_PlayCharaMotion(arg1, 28684, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    for (;;) {
      if (Script_IsCharaMotionFinished(arg1)) {
        break;
      }
      Script_RotateNodeUtils(arg1, &data_39DC0, 0);
      SCNF_WaitFrames_0000(1);
    }
    Script_charaToggleMotEnabled(arg1, 0);
    if (arg2 == 0x4742454B) {
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_BOX_RESCUE_ROUTE_UNLOCKED, 1);
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_GORO_QTE_COMPLETE, 1);
    }
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_41BEC, 1, data_41BFA)))
static int32_t script_20D31(void) {
  (void)((task_27C = 0) & 4);
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  script_EAA8("scene/02/MFSY", "MBUI");
  end:;
}

__attribute__((scn_string_bindings(0, data_41BFF, 1, data_41C06)))
static void script_20D64(int32_t arg0) {
  Vector3i local_F4;
  int32_t local_F0;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS08_SCENE_EXIT_COMPLETE) == 0) {
    Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 14, Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 14) + 1);
    script_2044F(CHARA_KEBB);
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_HISA_REQUEST_UNLOCKED) == 0) {
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_HISA_REQUEST_UNLOCKED, 1);
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_OLD_WAREHOUSE_DISTRICT_IS_NW, 1);
    }
    if (Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 14) == 1) {
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_SNEAK_IN_AT_NIGHT, 1);
    }
  } else {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
    Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
    Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
    Script_shenClearFreeState(0);
    Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_AddCharacterHandler(3, 0, 0x796D7564);
    Script_LoadVoiceFile(1, "SA1039");
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
    local_F0 = script_E948(arg0);
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
    script_A671(0x796D7564, -124.5f, 0.0f, -8.4f);
    script_A699(0x796D7564, 0.0f, 350.0f, 0.0f);
    Script_MoveObjectToObject(CHARA_AKIR, 0x796D7564);
    script_A671(CHARA_AKIR, -1.04f, 0.0f, -3.29f);
    script_A699(CHARA_AKIR, 0.0f, 135.0f, 0.0f);
    Script_MoveObjectToObject(arg0, 0x796D7564);
    script_A671(arg0, 0.0f, 0.0f, 0.0f);
    script_A699(arg0, 0.0f, 0.0f, 0.0f);
    Script_scenCreateTask(script_21165, 3, 2000, CHARA_AKIR, CHARA_KEBB);
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED) == 1) {
      Script_scenCreateTask(script_213BA, 3, 2000, CHARA_AKIR, CHARA_KEBB);
    } else {
      Script_scenCreateTask(script_21479, 3, 2000, CHARA_AKIR, 0x4242454B);
    }
    script_210D5();
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED) == 1) {
      Script_shenClearFreeState(0);
      Script_executeAicaCommand(2720, 1, 30);
      Script_executeAicaCommand(2720, 2, 30);
      Script_executeAicaCommand(2720, 3, 30);
      Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      script_EBF3(2, SHEN_NAME('MKSG'), 0);
    }
    task_4A4 = 0;
    Script_MoveObjectToObject(CHARA_AKIR, -1);
    Script_MoveObjectToObject(arg0, -1);
    script_EA10(arg0, local_F0);
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
    Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
    Script_mtwkGetSetRotation(CHARA_AKIR, &local_F4, 0x42000000);
    Script_EVT_SetRotation(CHARA_AKIR, 0x78000000, &local_F4, 0);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_InitializeOsag(CHARA_AKIR);
    Script_ryoControl(RYO_RestoreControl);
    Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
    Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
    Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
    Script_shenClearFreeState(1);
    Script_SetGameStateTo(AUTH_None);
    Script_convSetCamInactiveFlag(-1);
    Script_SetMapControlTaskFlag(0x796D7564, 1);
    Script_LoadVoiceFile(0, "SA1039");
    Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  }
  return;
}

static void script_210D5(void) {
  int32_t local_FC;
  task_4A4 = 0;
  for (;;) {
    local_FC = task_4A4;
    switch (local_FC) {
      case 0:
        Script_seqEnterEvtCam(220, 0x796D7564, 0x796D7564);
        break;
      case 160:
        Script_seqEnterEvtCam(221, 0x796D7564, 0x796D7564);
        break;
      case 240:
        Script_seqEnterEvtCam(222, 0x796D7564, 0x796D7564);
        break;
      case 441:
        goto L021164;
        break;
    }
    task_4A4 = task_4A4 + 1;
    SCNF_WaitFrames_0000(1);
  }
  L021164:;
  return;
}

static int32_t script_21165(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  Vector3 local_F0;
  for (;;) {
    local_FC = task_4A4;
    switch (local_FC) {
      case 1:
        Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
        Script_charaSetRefboardFlagTo(arg1, 1);
        Script_charaSetRefboardFlagTo(arg2, 1);
        Script_SetCharaRefBoardPos(arg1, &data_41C10);
        Script_SetCharaRefBoardPos(arg2, &data_41C10);
        Script_PlayCharaMotion(arg1, 28680, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_PlayCharaMotion(arg2, 28675, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        break;
      case 2:
        Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
        break;
      case 15:
        Script_motmSetBlendFrames(arg2, MOTM_BLEND_FRAMES_10);
        Script_PlayCharaMotion(arg2, 28676, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetCharaMotionPlaybackRate(arg2, 0.75f);
        if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg1) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg2)) {
          Script_EVT_GetPosition(arg2, 4, &local_F0, ENTITY_XFORM_WORLD_SPACE);
          Script_SetLookPointDetails(arg1, 16, &local_F0, 2);
          Script_FaceLookAtTargetUtils(arg1, FACE_LookAtPosition, &local_F0, 30);
        }
        if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg2) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
          Script_EVT_GetPosition(CHARA_AKIR, 4, &local_F0, ENTITY_XFORM_WORLD_SPACE);
          Script_SetLookPointDetails(arg2, 16, &local_F0, 2);
          Script_FaceLookAtTargetUtils(arg2, FACE_LookAtPosition, &local_F0, 30);
        }
        script_21A8B(arg0, 200, 10);
        break;
      case 68:
        script_21A8B(arg0, 100, 20);
        break;
      case 88:
        Script_motmSetBlendFrames(arg2, MOTM_BLEND_FRAMES_10);
        Script_PlayCharaMotion(arg2, 28673, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetCharaMotionPlaybackRate(arg2, 1.5f);
        break;
      case 90:
        script_21A8B(arg0, 100, 30);
        break;
      case 148:
        Script_PlayCharaMotion(arg2, 28675, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        break;
      case 170:
        script_21A8B(arg0, 200, 40);
        break;
      case 200:
        script_21A8B(arg0, 100, 40);
        goto L0213B9;
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  L0213B9:;
}

static int32_t script_213BA(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  for (;;) {
    local_FC = task_4A4;
    switch (local_FC) {
      case 240:
        script_21A8B(arg0, 200, 60);
        break;
      case 280:
        Script_PlayCharaMotion(arg2, 28683, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_motmToggleFlags(arg2, SCEN_Enable, 0x80);
        break;
      case 365:
        if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg1)) {
          Script_SetCharaLookAtPosition(arg1, -10, 0, 0);
          Script_FaceLookAtTargetUtils(arg1, FACE_LookReset);
        }
        Script_PlayCharaMotion(arg2, 28682, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_motmToggleFlags(arg2, SCEN_Enable, 0x80);
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_21479(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  for (;;) {
    local_FC = task_4A4;
    switch (local_FC) {
      case 240:
        script_21A8B(arg0, 200, 50);
        break;
      case 330:
        if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg1)) {
          Script_SetCharaLookAtPosition(arg1, -10, 0, 0);
          Script_FaceLookAtTargetUtils(arg1, FACE_LookReset);
        }
        Script_PlayCharaMotion(arg1, 28681, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        goto L0214F8;
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  L0214F8:;
}

static int32_t script_214F9(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = arg0 + arg1 + arg2;
  local_F8 = local_FC;
  if (local_F8 == 1110) {
    Script_ShowUIMessageFromID("SA1035A001");
  } else if (local_F8 == 2110) {
    Script_ShowUIMessageFromID("SA1039A004");
  } else if (local_F8 == 2120) {
    Script_ShowUIMessageFromID("SA1039A001");
  } else if (local_F8 == 2130) {
    Script_ShowUIMessageFromID("SA1039A002");
  } else if (local_F8 == 2140) {
    Script_ShowUIMessageFromID("SA1039A003");
  } else if (local_F8 == 1210) {
    if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.5f)) {
      Script_ShowUIMessageFromID("SA1035B001");
    } else {
      Script_ShowUIMessageFromID("SA1035B002");
    }
  } else if (local_F8 == 1220) {
    if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.5f)) {
      Script_ShowUIMessageFromID("SA1035B003");
    } else {
      Script_ShowUIMessageFromID("SA1035B004");
    }
  } else if (local_F8 == 1230) {
    if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.5f)) {
      Script_ShowUIMessageFromID("SA1035B005");
    } else {
      Script_ShowUIMessageFromID("SA1035B006");
    }
  } else if (local_F8 == 2210) {
    if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.5f)) {
      Script_ShowUIMessageFromID("SA1039B001");
    } else {
      Script_ShowUIMessageFromID("SA1039B002");
    }
  } else if (local_F8 == 2220) {
    float local_F4;
    local_F4 = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f);
    if (!(local_F4 >= 0.33f)) {
      Script_ShowUIMessageFromID("SA1039B003");
    } else if (!(local_F4 >= 0.67f)) {
      Script_ShowUIMessageFromID("SA1039B004");
    } else {
      Script_ShowUIMessageFromID("SA1039B005");
    }
  } else if (local_F8 == 2230) {
    if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.5f)) {
      Script_ShowUIMessageFromID("SA1039B006");
    } else {
      Script_ShowUIMessageFromID("SA1039B007");
    }
  } else if (local_F8 == 2240) {
    Script_ShowUIMessageFromID("SA1039B008");
  } else if (local_F8 == 2250) {
    Script_ShowUIMessageFromID("SA1039B009");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("SA1039B010");
  } else if (local_F8 == 2260) {
    Script_ShowUIMessageFromID("SA1039B011");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("SA1039B012");
  } else if (local_F8 == 4210) {
    if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.5f)) {
      Script_ShowUIMessageFromID("SA1098B001");
    } else {
      Script_ShowUIMessageFromID("SA1098B002");
    }
  } else if (local_F8 == 4220) {
    float local_F4;
    local_F4 = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f);
    if (!(local_F4 >= 0.33f)) {
      Script_ShowUIMessageFromID("SA1098B003");
    } else if (!(local_F4 >= 0.67f)) {
      Script_ShowUIMessageFromID("SA1098B004");
    } else {
      Script_ShowUIMessageFromID("SA1098B005");
    }
  } else if (local_F8 == 4230) {
    if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.5f)) {
      Script_ShowUIMessageFromID("SA1098B006");
    } else {
      Script_ShowUIMessageFromID("SA1098B007");
    }
  } else if (local_F8 == 5210) {
    if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.5f)) {
      Script_ShowUIMessageFromID("SA1097B001");
    } else {
      Script_ShowUIMessageFromID("SA1097B002");
    }
  } else if (local_F8 == 5220) {
    float local_F4;
    local_F4 = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f);
    if (!(local_F4 >= 0.33f)) {
      Script_ShowUIMessageFromID("SA1097B003");
    } else if (!(local_F4 >= 0.67f)) {
      Script_ShowUIMessageFromID("SA1097B004");
    } else {
      Script_ShowUIMessageFromID("SA1097B005");
    }
  } else if (local_F8 == 5230) {
    if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.5f)) {
      Script_ShowUIMessageFromID("SA1097B006");
    } else {
      Script_ShowUIMessageFromID("SA1097B007");
    }
  } else if (local_F8 == 6210) {
    if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.5f)) {
      Script_ShowUIMessageFromID("SA1099B001");
    } else {
      Script_ShowUIMessageFromID("SA1099B002");
    }
  } else if (local_F8 == 6220) {
    float local_F4;
    local_F4 = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f);
    if (!(local_F4 >= 0.33f)) {
      Script_ShowUIMessageFromID("SA1099B003");
    } else if (!(local_F4 >= 0.67f)) {
      Script_ShowUIMessageFromID("SA1099B004");
    } else {
      Script_ShowUIMessageFromID("SA1099B005");
    }
  } else if (local_F8 == 6230) {
    if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.5f)) {
      Script_ShowUIMessageFromID("SA1099B006");
    } else {
      Script_ShowUIMessageFromID("SA1099B007");
    }
  } else if (local_F8 == 3210) {
    if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.5f)) {
      Script_ShowUIMessageFromID("SA1096B001");
    } else {
      Script_ShowUIMessageFromID("SA1096B002");
    }
  } else if (local_F8 == 3220) {
    float local_F4;
    local_F4 = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f);
    if (!(local_F4 >= 0.33f)) {
      Script_ShowUIMessageFromID("SA1096B003");
    } else if (!(local_F4 >= 0.67f)) {
      Script_ShowUIMessageFromID("SA1096B004");
    } else {
      Script_ShowUIMessageFromID("SA1096B005");
    }
  } else if (local_F8 == 3230) {
    if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.5f)) {
      Script_ShowUIMessageFromID("SA1096B006");
    } else {
      Script_ShowUIMessageFromID("SA1096B007");
    }
  } else {
    Script_sprintf("MFSY: Illegal Condition(%d)\012", local_FC);
  }
  end:;
}

static void script_21A8B(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_214F9, 3, arg0, arg1, arg2);
  return;
}

static void script_21A9F(void) {
  int32_t local_FC;
  float local_F4[2];
  float local_EC[2];
  float local_E4[2];
  float local_DC[2];
  float local_D4[2];
  float local_CC[2];
  int32_t local_C8;
  float local_C4;
  float local_C0;
  int32_t local_BC;
  float local_B8;
  float local_B4;
  int32_t local_B0;
  float local_AC;
  float local_A8;
  int32_t local_A4;
  float local_A0;
  float local_9C;
  int32_t local_98;
  float local_94;
  int32_t local_90;
  int32_t local_8C;
  float local_88;
  int32_t local_84;
  int32_t local_80;
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 93);
  script_A2A7();
  script_21FBE();
  local_E4[0] = 0.0f;
  local_E4[1] = 0.0f;
  Script_LightUtils(9, 6, (const float *)local_E4);
  local_DC[0] = 0.0f;
  local_DC[1] = 0.0f;
  Script_LightUtils(12, 6, (const float *)local_DC);
  local_D4[0] = 0.0f;
  local_D4[1] = 0.0f;
  Script_LightUtils(14, 6, (const float *)local_D4);
  local_EC[0] = 0.0f;
  local_EC[1] = 0.0f;
  Script_LightUtils(3, 6, (const float *)local_EC);
  local_CC[0] = 0.0f;
  local_CC[1] = 0.0f;
  Script_LightUtils(17, 6, (const float *)local_CC);
  local_F4[0] = 0.7f;
  local_F4[1] = 0.7f;
  Script_LightUtils(2, 6, (const float *)local_F4);
  Script_shenSeqUtils(-9, 0, 0);
  Script_shenSeqUtils(0);
  local_C0 = 1.0f;
  local_B4 = 1.0f;
  local_A8 = 1.0f;
  local_90 = 0x3F800000;
  local_9C = 1.0f;
  local_84 = 0x3F800000;
  local_C8 = 0;
  local_BC = 0;
  local_B0 = 0;
  local_A4 = 0;
  local_8C = 0;
  local_98 = 0;
  local_C4 = 0.307619035f;
  local_B8 = 0.307083338f;
  local_AC = 0.0f;
  local_A0 = 0.0f;
  local_88 = 0.0f;
  local_94 = 0.0f;
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_80 = local_FC;
    switch (local_80) {
      case 0:
        local_BC = 1;
        break;
      case 1:
        local_C8 = 1;
        break;
      case 36:
        local_A4 = 1;
        break;
      case 100:
        local_8C = 1;
        break;
      case 115:
        local_B0 = 1;
        break;
      case 230:
        Script_executeAicaCommand(2720, 2, 30);
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        break;
    }
    SCNF_WaitFrames_0000(1);
    local_FC = local_FC + 1;
    if (local_BC == 1) {
      if (!(local_B8 >= 0.67f)) {
        local_B8 = local_B8 + 0.00558333332f;
        local_DC[0] = local_B8;
        local_DC[1] = local_B8;
        Script_LightUtils(12, 6, (const float *)local_DC);
        if (!(local_B4 <= 0.0f)) {
          Script_charaSetRenderOpacity(SHEN_NAME('KAB2'), local_B4);
          local_B4 = local_B4 - 0.00833333377f;
        }
      } else {
        if (-1) {
          Script_charaDisplay(SHEN_NAME('KAB2'), CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(SHEN_NAME('KAB2'), CDM_Show);
            goto L021D26;
          }
        }
        L021D26:;
        Script_scenToggleVisibleFor(SHEN_NAME('KAB2'), 0);
        local_BC = 0;
      }
    }
    if (local_C8 == 1) {
      if (!(local_C4 >= 0.68f)) {
        local_C4 = local_C4 + 0.0032380952f;
        local_E4[0] = local_C4;
        local_E4[1] = local_C4;
        Script_LightUtils(9, 6, (const float *)local_E4);
        if (!(local_C0 <= 0.0f)) {
          Script_charaSetRenderOpacity(SHEN_NAME('KAB1'), local_C0);
          local_C0 = local_C0 - 0.00588235306f;
        }
      } else {
        if (-1) {
          Script_charaDisplay(SHEN_NAME('KAB1'), CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(SHEN_NAME('KAB1'), CDM_Show);
            goto L021DBE;
          }
        }
        L021DBE:;
        Script_scenToggleVisibleFor(SHEN_NAME('KAB1'), 0);
        local_C8 = 0;
      }
    }
    if (local_B0 == 1) {
      if (!(local_AC >= 1.48f)) {
        local_AC = local_AC + 0.0148f;
        local_D4[0] = local_AC;
        local_D4[1] = local_AC;
        Script_LightUtils(14, 6, (const float *)local_D4);
        if (!(local_A8 <= 0.0f)) {
          Script_charaSetRenderOpacity(SHEN_NAME('KAB3'), local_A8);
          local_A8 = local_A8 - 0.01f;
        }
      } else {
        if (-1) {
          Script_charaDisplay(SHEN_NAME('KAB3'), CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(SHEN_NAME('KAB3'), CDM_Show);
            goto L021E56;
          }
        }
        L021E56:;
        Script_scenToggleVisibleFor(SHEN_NAME('KAB3'), 0);
        local_B0 = 0;
      }
    }
    if (local_A4 == 1) {
      if (!(local_A0 >= 0.63f)) {
        local_A0 = local_A0 + 0.00370588223f;
        local_EC[0] = local_A0;
        local_EC[1] = local_A0;
        Script_LightUtils(3, 6, (const float *)local_EC);
        if (!(local_9C <= 0.0f)) {
          Script_charaSetRenderOpacity(SHEN_NAME('KAB4'), local_9C);
          local_9C = local_9C - 0.00588235306f;
        }
      } else {
        if (-1) {
          Script_charaDisplay(SHEN_NAME('KAB4'), CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(SHEN_NAME('KAB4'), CDM_Show);
            goto L021EEE;
          }
        }
        L021EEE:;
        Script_scenToggleVisibleFor(SHEN_NAME('KAB4'), 0);
        local_A4 = 0;
      }
    }
    if (local_8C == 1) {
      if (!(local_88 >= 0.58f)) {
        local_88 = local_88 + 0.00682352902f;
        local_F4[0] = local_88;
        local_F4[1] = local_88;
        Script_LightUtils(2, 6, (const float *)local_F4);
      } else {
        local_A4 = 0;
      }
    }
    if (local_98 == 1) {
      if (!(local_94 >= 0.69f)) {
        local_94 = local_94 + 0.008625f;
        local_CC[0] = local_94;
        local_CC[1] = local_94;
        Script_LightUtils(17, 6, (const float *)local_CC);
      } else {
        local_98 = 0;
        if (-1) {
          Script_charaDisplay(SHEN_NAME('KAB5'), CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(SHEN_NAME('KAB5'), CDM_Show);
            goto L021FAE;
          }
        }
        L021FAE:;
        Script_scenToggleVisibleFor(SHEN_NAME('KAB5'), 0);
      }
    }
  }
  return;
}

static void script_21FBE(void) {
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 120.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 120.0f);
  Script_SetupLightFromTable(1);
  Script_PREP_Related(-1, 1);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KAB1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KAB1'), CDM_Show);
      goto L02200E;
    }
  }
  L02200E:;
  Script_scenToggleVisibleFor(SHEN_NAME('KAB1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KAB2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KAB2'), CDM_Show);
      goto L022042;
    }
  }
  L022042:;
  Script_scenToggleVisibleFor(SHEN_NAME('KAB2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KAB3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KAB3'), CDM_Show);
      goto L022076;
    }
  }
  L022076:;
  Script_scenToggleVisibleFor(SHEN_NAME('KAB3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KAB4'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KAB4'), CDM_Show);
      goto L0220AA;
    }
  }
  L0220AA:;
  Script_scenToggleVisibleFor(SHEN_NAME('KAB4'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KAB5'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KAB5'), CDM_Show);
      goto L0220DE;
    }
  }
  L0220DE:;
  Script_scenToggleVisibleFor(SHEN_NAME('KAB5'), 0);
  if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 2) | Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 3)) {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('YUKT'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('YUKT'), CDM_Show);
        goto L02212B;
      }
    }
    L02212B:;
    Script_scenToggleVisibleFor(SHEN_NAME('YUKT'), 0);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('YUKI'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('YUKI'), CDM_Show);
        goto L022162;
      }
    }
    L022162:;
    Script_scenToggleVisibleFor(SHEN_NAME('YUKI'), 0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_41E88, 1, data_41E96, 3, data_41EAC)))
static void script_2216F(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  task_500 = arg1;
  task_504 = arg0;
  local_E4 = task_500;
  switch (local_E4) {
    case 0:
      task_50C = 1;
      break;
    case 1:
      task_50C = 1;
      break;
    case 2:
      task_50C = 0;
      break;
    case 3:
      task_50C = 0;
      break;
    case 4:
      task_50C = 0;
      break;
    case 5:
      task_50C = 0;
      break;
  }
  Script_ccowUtils(SHEN_NAME('JBOX'), CCOW_DisableAll);
  script_EAA8("scene/02/MFSY", "ALP_JUKE");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  task_4C0 = Script_LoadHandMot(2, SHEN_NAME('MO01'), "HMOT_JBX.BIN");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  script_A671(SHEN_NAME('YKUR'), 6.513f, 1.061f, -7.255f);
  script_A699(SHEN_NAME('YKUR'), 290.01709f, 184.954803f, 345.003693f);
  Script_MoveObjectToObject(SHEN_NAME('JCNT'), 0x584F424A);
  Script_MoveObjectToObject(SHEN_NAME('ASTD'), 0x584F424A);
  SCNF_WaitFrames_0000(1);
  switch (task_504) {
    case 0:
      {
        {
          Vector3 local_D8;
          local_D8.x = -0.461f;
          (&local_D8)->z = 0.566f;
          Script_EVT_SetPosition(SHEN_NAME('JCNT'), 0x28000000, &local_D8);
        }
        {
          Vector3 local_D8;
          local_D8.x = -0.225f;
          (&local_D8)->z = 1.138f;
          Script_EVT_SetPosition(SHEN_NAME('ASTD'), 0x28000000, &local_D8);
        }
        {
          Vector3i local_D8;
          (&local_D8)->y = 0;
          Script_EVT_SetRotation(SHEN_NAME('JCNT'), MATRIX_XFORM_Y, &local_D8, 0);
        }
        {
          Vector3i local_D8;
          (&local_D8)->y = 0;
          Script_EVT_SetRotation(SHEN_NAME('ASTD'), MATRIX_XFORM_Y, &local_D8, 0);
        }
        break;
      }
    case 1:
      {
        {
          Vector3 local_D8;
          local_D8.x = -0.461f;
          (&local_D8)->z = 0.566f;
          Script_EVT_SetPosition(SHEN_NAME('JCNT'), 0x28000000, &local_D8);
        }
        {
          Vector3 local_D8;
          local_D8.x = -0.225f;
          (&local_D8)->z = 1.138f;
          Script_EVT_SetPosition(SHEN_NAME('ASTD'), 0x28000000, &local_D8);
        }
        {
          Vector3i local_D8;
          (&local_D8)->y = 0;
          Script_EVT_SetRotation(SHEN_NAME('JCNT'), MATRIX_XFORM_Y, &local_D8, 0);
        }
        {
          Vector3i local_D8;
          (&local_D8)->y = 0;
          Script_EVT_SetRotation(SHEN_NAME('ASTD'), MATRIX_XFORM_Y, &local_D8, 0);
        }
        break;
      }
    case 2:
      {
        {
          Vector3 local_D8;
          local_D8.x = -0.461f;
          (&local_D8)->z = 0.566f;
          Script_EVT_SetPosition(SHEN_NAME('JCNT'), 0x28000000, &local_D8);
        }
        {
          Vector3 local_D8;
          local_D8.x = -0.225f;
          (&local_D8)->z = 1.138f;
          Script_EVT_SetPosition(SHEN_NAME('ASTD'), 0x28000000, &local_D8);
        }
        {
          Vector3i local_D8;
          (&local_D8)->y = 0;
          Script_EVT_SetRotation(SHEN_NAME('JCNT'), MATRIX_XFORM_Y, &local_D8, 0);
        }
        {
          Vector3i local_D8;
          (&local_D8)->y = 0;
          Script_EVT_SetRotation(SHEN_NAME('ASTD'), MATRIX_XFORM_Y, &local_D8, 0);
        }
        break;
      }
  }
  Script_LoadVoiceFile(1, "E1067");
  Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('D_TE'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('ASTD'), 0);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_facemotUtils(SHEN_NAME('YKUR'), 1, 0);
  SCNF_WaitFrames_0000(1);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('D_TE'), CDM_Hide);
  Script_charaPosRotUtils(SHEN_NAME('JBOX'), 161, &data_41E70, 0, CHARA_PART_SetScale);
  if (task_50C == 1) {
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, CHARA_PART_Get);
    (&local_E8)->y = (&local_E8)->y - 3094;
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, CHARA_PART_Set);
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, CHARA_PART_Get);
    local_E8.x = (&local_E8)->x + 236;
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, CHARA_PART_Set);
  }
  Script_scenCreateTask(script_22649, 0);
  task_4E4 = Script_scenCreateTask(script_23F2F, 0);
  task_4EC = Script_scenCreateTask(script_23FA1, 0);
  if (task_50C == 1) {
    task_4D3 = 1;
    task_4D5 = 1;
  }
  (void)((task_50E = 0) & 0x10);
  return;
}

static void script_22582(void) {
  int8_t local_FF;
  local_FF = 100;
  for (;;) {
    if (Script_GetInputData(1, 0) == 0x1000) {
      local_FF = local_FF + 1;
      if (local_FF >= 126) {
        local_FF = 126;
      }
      Script_executeAicaCommand(1184, 2, local_FF);
    }
    if (Script_GetInputData(1, 0) == 0x2000) {
      local_FF = local_FF - 1;
      if (local_FF <= 0) {
        local_FF = 0;
      }
      Script_executeAicaCommand(1184, 2, local_FF);
    }
    if (Script_GetInputData(1, 0) == 0x8000) {
      Script_executeAicaCommand(171, 0, 0);
    }
    if (Script_GetInputData(1, 0) == 0x4000) {
      Script_executeAicaCommand(AICA_CMD_PLAY, 0, 0);
    }
    Script_PrintDebugString(2, 15, "Volume = %3d", local_FF);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_22649(void) {
  int8_t local_FF;
  (void)((local_FF = 0) & 2);
  for (;;) {
    if (Script_scenCheckCharaExist(SHEN_NAME('JBOX')) == 0) {
      break;
    }
    if (script_245CB()) {
      if (Script_GetControllerInput(1) == 0x400) {
        (void)((local_FF = 0) & 2);
        script_2279D();
      }
      if (local_FF == 0) {
        (void)((local_FF = -1) & 2);
        Script_sicnUtils(0, 5, 1);
        Script_uiUtils(0, 7, 0);
      }
    } else {
      if (local_FF == -1) {
        (void)((local_FF = 0) & 2);
        Script_sicnUtils(1, 0, 0);
        Script_uiUtils(0, 0, 0);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_226F2(float arg0, float arg1, float arg2, int32_t arg3) {
  Vector3 local_F4;
  local_F4.x = arg0;
  (&local_F4)->y = arg1;
  (&local_F4)->z = arg2;
  Script_scenCharaSubmitMoveTargetRequest(CHARA_AKIR, &local_F4, arg3, MN_AKI_AKI_STAND_LP, 0);
  while (Script_IsMoveTargetRequestActive(CHARA_AKIR) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2273C(int32_t arg0) {
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int8_t script_22756(void) {
  int8_t local_FF;
  local_FF = script_B3B5(2, 100, 1);
  return local_FF;
}

static int8_t script_2276E(void) {
  int8_t local_FF;
  uint8_t unused_F8[7];
  GameDateTime local_F0;
  local_FF = 0;
  Script_ReturnTimeDate(&local_F0);
  if (((&local_F0)->hours >= 18) & ((&local_F0)->minutes >= 45)) {
    local_FF = 1;
  }
  return local_FF;
}

__attribute__((scn_string_bindings(0, data_41ED0, 9, data_41F2A, 10, data_41F34, 11, data_41F3E, 13, data_41F52)))
static void script_2279D(void) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  if (script_246BC(0) == 0) {
    if (~task_50E) {
      (void)((task_50E = -1) & 8);
      Script_ShowUIMessageFromID("E1067A011");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    }
  } else {
    Script_uiUtils(0, 7, 0);
    script_22D3E();
    if (task_500 == 2) {
      Script_SetCharaRefBoardPos(CHARA_AKIR, &data_41EC0);
    }
    Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
    Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
    Script_EVT_GetPosition(SHEN_NAME('JBOX'), -1, &local_EC, ENTITY_XFORM_WORLD_SPACE);
    Script_SetCharaLookAtPosition(CHARA_AKIR, 1, &local_EC, 1);
    L02283F:;
    Script_seqEnterEvtCam(160, SHEN_NAME('JBOX'), SHEN_NAME('JBOX'));
    {
      int32_t unused_E0[3];
      int32_t local_DC;
      int32_t local_D8;
      local_DC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
      local_D8 = local_DC;
      if (local_D8 == 0) {
        Script_ShowUIMessageFromID("E1067A001");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
      } else {
        Script_ShowUIMessageFromID("E1067A002");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
      }
    }
    {
      int32_t unused_E0[3];
      int32_t local_DC;
      int32_t local_D8;
      local_DC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      local_D8 = local_DC;
      if (local_D8 == 0) {
        Script_ShowUIMessageFromID("E1067A003");
      } else if (local_D8 == 1) {
        Script_ShowUIMessageFromID("E1067A004");
      } else {
        Script_ShowUIMessageFromID("E1067A005");
      }
    }
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    local_FC = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 46, 47, 0xFFFF, 450, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL);
    if (local_FC == 46) {
      if (script_22756() < 2) {
        if (Script_shenGetStats(STATS_CASH) < 100) {
          int32_t unused_DC[4];
          int32_t local_D8;
          local_D8 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
          if (local_D8 == 0) {
            Script_ShowUIMessageFromID("E1067A006");
          } else if (local_D8 == 1) {
            Script_ShowUIMessageFromID("E1067A007");
          } else {
            Script_ShowUIMessageFromID("E1067A008");
          }
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          goto L022BF9;
        }
        if (script_2276E()) {
          int32_t unused_E0[3];
          int32_t local_DC;
          int32_t local_D8;
          local_DC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
          local_D8 = local_DC;
          if (local_D8 == 0) {
            Script_ShowUIMessageFromID("E1067A009");
          } else {
            Script_ShowUIMessageFromID("E1067A011");
          }
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          goto L022BF9;
        }
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_ResetHandMotion(SHEN_NAME('YKUR'), 1, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 851);
        Script_shenClearFreeState(0);
        SCNF_WaitFrames_0000(1);
        Script_seqEnterEvtCam(170, SHEN_NAME('JBOX'), SHEN_NAME('JBOX'));
        Script_SetCharaLookAtPosition(CHARA_AKIR, -98312, 0, 0);
        Script_PlayCharaMotion(CHARA_AKIR, 24577, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        SCNF_WaitFrames_0000(60);
        Script_EVT_GetPosition(SHEN_NAME('JBOX'), -1, &local_EC, ENTITY_XFORM_WORLD_SPACE);
        Script_SetCharaLookAtPosition(CHARA_AKIR, 1, &local_EC, 1);
        SCNF_WaitFrames_0000(40);
        Script_executeAicaCommand(263337, 0, 0);
        script_2273C(CHARA_AKIR);
        Script_EVT_GetPosition(SHEN_NAME('D_TE'), -1, &local_EC, ENTITY_XFORM_WORLD_SPACE);
        Script_SetLookPointDetails(CHARA_AKIR, 11, &local_EC, 1);
        local_F8 = Script_shenGetStats(STATS_CASH);
        SCNF_SetCashValue(-100, 1);
        local_F8 = local_F8 - 100;
        Script_SetStats(STATS_CASH, local_F8);
        Script_shenClearFreeState(1);
        Script_uiUtils(0, 7, 0);
        script_22EBE();
        task_50D = 1;
        goto L022BF9;
      }
      goto L022BF9;
    } else {
      int32_t unused_E0[3];
      int32_t local_DC;
      int32_t local_D8;
      local_DC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      local_D8 = local_DC;
      if (local_D8 == 0) {
        Script_ShowUIMessageFromID("E1067A009");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
      } else if (local_D8 == 1) {
        Script_ShowUIMessageFromID("E1067A010");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
      } else {
        Script_ShowUIMessageFromID("E1067A011");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
      }
      goto L022BF9;
    }
    SCNF_WaitFrames_0000(1);
    goto L02283F;
    L022BF9:;
    Script_SetCharaLookAtPosition(CHARA_AKIR, -98312, 0, 0);
    Script_MoveObjectToObject(SHEN_NAME('YKUR'), -1);
    Script_MoveObjectToObject(SHEN_NAME('D_TE'), -1);
    Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
    Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('D_TE'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('ASTD'), 0);
    Script_charaSetRefboardFlagTo(CHARA_AKIR, 0);
    Script_SetGameStateTo(AUTH_None);
    Script_shenClearFreeState(1);
    Script_uiUtils(0, 0, 0);
    Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE, 0);
    Script_ryoControl(RYO_RestoreControl);
  }
  return;
}

static void script_22C91(const char *arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, arg2, arg0, 0x8000);
  task_4D3 = 1;
  task_4D5 = 1;
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadSND(4, 0, 0, 0);
  task_4D4 = 4;
  SCNF_WaitFrames_0000(20);
  Script_executeAicaCommand(132265, 0, 0);
  SCNF_WaitFrames_0000(40);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, arg2, arg3);
  Script_PlayerStatsUtils(PS_SetIndexedU8Table1Slot, arg1, Script_PlayerStatsUtils(PS_GetIndexedU8Table1Slot, arg1) + 1);
  task_50C = 1;
  return;
}

static void script_22D3E(void) {
  Vector3 local_F4;
  int32_t unused_D0[9];
  Vector3i local_C4;
  int32_t unused_BC[2];
  SCNF_WaitFrames_0000(1);
  Script_uiUtils(0, 7, 0);
  Script_sicnUtils(1, 0, 0);
  Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('D_TE'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('ASTD'), 1);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 1);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_MoveObjectToObject(SHEN_NAME('YKUR'), 0x584F424A);
  Script_MoveObjectToObject(SHEN_NAME('D_TE'), 0x52554B59);
  Script_EVT_GetPosition(SHEN_NAME('ASTD'), -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(SHEN_NAME('ASTD'), &local_C4, 0x40000000);
  script_226F2((&local_F4)->x, (&local_F4)->y, (&local_F4)->z, (&local_C4)->y);
  local_F4.x = -0.419f;
  (&local_F4)->y = 1.058f;
  (&local_F4)->z = 0.803f;
  local_C4.x = 20028;
  (&local_C4)->y = 64635;
  (&local_C4)->z = 30038;
  Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_F4);
  Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &local_C4, 0);
  local_F4.x = 0.0f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.0f;
  Script_EVT_SetPosition(SHEN_NAME('D_TE'), POS_XYZ, &local_F4);
  Script_mtwkGetSetRotation(SHEN_NAME('JBOX'), &local_C4, 0x40000000);
  Script_EVT_SetRotation(SHEN_NAME('D_TE'), 0x78000000, &local_C4, 0);
  return;
}

static void script_22EBE(void) {
  L022EC0:;
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Show);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_sicnUtils(0, 37, 1);
  Script_seqEnterEvtCam(152, SHEN_NAME('D_TE'), SHEN_NAME('D_TE'));
  task_510 = 1;
  Script_SetNewHandMotion(SHEN_NAME('YKUR'), 1, 852, 0, 0, 0, 0, 1.0f, 0);
  Script_SetCameraFlags(-1);
  script_22F5A();
  Script_sicnUtils(1, 0, 0);
  Script_shenClearFreeState(0);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Hide);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  goto L022F59;
  SCNF_WaitFrames_0000(1);
  goto L022EC0;
  L022F59:;
  return;
}

__attribute__((scn_string_bindings(0, data_41F5C, 1, data_41F67, 2, data_41F72, 3, data_41F7D, 4, data_41F88, 5, data_41F93, 6, data_41F9E, 7, data_41FA9, 8, data_41FB4, 9, data_41FBF, 10, data_41FCA, 11, data_41FD5, 12, data_41FE0, 13, data_41FEB, 14, data_41FF6, 15, data_42001, 16, data_4200C, 17, data_42017, 18, data_42022, 19, data_4202D, 24, data_42064, 25, data_4206F, 26, data_4207A, 27, data_42085, 28, data_42090, 29, data_4209B, 34, data_420D2, 35, data_420DD, 36, data_420E8, 37, data_420F3, 38, data_420FE, 39, data_42109, 44, data_42140, 45, data_4214B, 46, data_42156, 47, data_42161, 48, data_4216C, 49, data_42177, 52, data_42198, 54, data_421AE, 55, data_421B9, 56, data_421C4, 57, data_421CF, 58, data_421DA, 59, data_421E5)))
static void script_22F5A(void) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  int8_t local_E7;
  uint8_t unused_E4[3];
  int32_t local_E0;
  switch (task_504) {
    case 0:
      task_50F = 10;
      break;
    case 1:
      task_50F = 4;
      break;
    case 2:
      task_50F = 2;
      break;
  }
  for (;;) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE)) {
      break;
    }
    if ((Script_GetControllerInput(0) == 0x8000) | (Script_GetControllerInput(0) == 0x2000) | (Script_GetControllerInput(0) == 0xA000)) {
      task_4C4 = 0;
      task_4C5 = 0;
      task_4C6 = 0;
      local_E7 = 0;
      if (task_504 == 0) {
        if (task_50F <= 19) {
          while (local_E7 < 16) {
            Script_EVT_GetPosition(SHEN_NAME('YKUR'), -1, &local_F4, 0);
            local_F4.x = (&local_F4)->x + 0.002f;
            Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_F4);
            local_E7 = local_E7 + 1;
            SCNF_WaitFrames_0000(1);
          }
          task_50F = task_50F + 1;
          if (task_50F >= 20) {
            task_50F = 20;
          }
        }
      } else {
        if (task_50F <= 9) {
          while (local_E7 < 16) {
            Script_EVT_GetPosition(SHEN_NAME('YKUR'), -1, &local_F4, 0);
            local_F4.x = (&local_F4)->x + 0.002f;
            Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_F4);
            local_E7 = local_E7 + 1;
            SCNF_WaitFrames_0000(1);
          }
          task_50F = task_50F + 1;
          if (task_50F >= 10) {
            task_50F = 10;
          }
        }
      }
    }
    if ((Script_GetControllerInput(0) == 0x4000) | (Script_GetControllerInput(0) == 0x1000) | (Script_GetControllerInput(0) == 0x5000)) {
      task_4C4 = 0;
      task_4C5 = 0;
      task_4C6 = 0;
      local_E7 = 0;
      if (task_50F >= 2) {
        while (local_E7 < 16) {
          Script_EVT_GetPosition(SHEN_NAME('YKUR'), -1, &local_F4, 0);
          local_F4.x = (&local_F4)->x - 0.002f;
          Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_F4);
          local_E7 = local_E7 + 1;
          SCNF_WaitFrames_0000(1);
        }
        task_50F = task_50F - 1;
        if (task_50F <= 1) {
          task_50F = 1;
        }
      }
    }
    if (Script_GetControllerInput(1) == 0x400) {
      task_4C4 = 0;
      task_4C5 = 0;
      task_4C6 = 0;
      if (task_50F >= 11) {
        script_23A8F();
      } else {
        Script_shenClearFreeState(0);
        Script_sicnUtils(1, 0, 0);
        Script_SetCameraFlags(0);
        Script_seqEnterEvtCam(180, SHEN_NAME('JBOX'), SHEN_NAME('JBOX'));
        local_E7 = 0;
        while (local_E7 < 10) {
          Script_EVT_GetPosition(SHEN_NAME('YKUR'), -1, &local_F4, 0);
          (&local_F4)->y = (&local_F4)->y - 0.0015f;
          Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_F4);
          local_E7 = local_E7 + 1;
          script_23A92(1);
          SCNF_WaitFrames_0000(1);
        }
        Script_executeAicaCommand(1193, 0, 0);
        local_E7 = 0;
        while (local_E7 < 10) {
          Script_EVT_GetPosition(SHEN_NAME('YKUR'), -1, &local_F4, 0);
          (&local_F4)->y = (&local_F4)->y + 0.0015f;
          Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_F4);
          local_E7 = local_E7 + 1;
          script_23A92(0);
          SCNF_WaitFrames_0000(1);
        }
        if (task_508 != 0) {
          Script_srchControl((void *)task_508);
        }
        Script_executeAicaCommand(197801, 0, 0);
        task_4FC = 0;
        script_23B44();
        for (;;) {
          if (task_4FC == 1) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        local_E0 = task_500;
        switch (local_E0) {
          case 0:
            local_E0 = task_50F;
            switch (local_E0) {
              case 1:
                script_22C91("ykm003.snd", 0, 2, 100);
                break;
              case 2:
                script_22C91("ykm002.snd", 1, 2, 100);
                break;
              case 3:
                script_22C91("bgm045.snd", 2, 2, 100);
                break;
              case 4:
                script_22C91("tsm002.snd", 3, 2, 100);
                break;
              case 5:
                script_22C91("bgm015.snd", 20, 2, 100);
                break;
              case 6:
                script_22C91("bgm016.snd", 21, 2, 100);
                break;
              case 7:
                script_22C91("bgm014.snd", 22, 2, 100);
                break;
              case 8:
                script_22C91("bgm047.snd", 23, 2, 100);
                break;
              case 9:
                script_22C91("bgm075.snd", 24, 2, 100);
                break;
              case 10:
                script_22C91("bgm010.snd", 25, 2, 100);
                break;
            }
            break;
          case 1:
            local_E0 = task_50F;
            switch (local_E0) {
              case 1:
                script_22C91("ykm003.snd", 0, 2, 100);
                break;
              case 2:
                script_22C91("ykm002.snd", 1, 2, 100);
                break;
              case 3:
                script_22C91("bgm045.snd", 2, 2, 100);
                break;
              case 4:
                script_22C91("tsm002.snd", 3, 2, 100);
                break;
              case 5:
                script_22C91("bgm015.snd", 20, 2, 100);
                break;
              case 6:
                script_22C91("bgm016.snd", 21, 2, 100);
                break;
              case 7:
                script_22C91("bgm014.snd", 22, 2, 100);
                break;
              case 8:
                script_22C91("bgm047.snd", 23, 2, 100);
                break;
              case 9:
                script_22C91("bgm075.snd", 24, 2, 100);
                break;
              case 10:
                script_22C91("bgm010.snd", 25, 2, 100);
                break;
            }
            break;
          case 2:
            local_E0 = task_50F;
            switch (local_E0) {
              case 1:
                script_22C91("bgm061.snd", 4, 2, 100);
                break;
              case 2:
                script_22C91("bgm058.snd", 5, 2, 100);
                break;
              case 3:
                script_22C91("bgm057.snd", 6, 2, 100);
                break;
              case 4:
                script_22C91("bgm056.snd", 7, 2, 100);
                break;
              case 5:
                script_22C91("bgm015.snd", 20, 2, 100);
                break;
              case 6:
                script_22C91("bgm016.snd", 21, 2, 100);
                break;
              case 7:
                script_22C91("bgm014.snd", 22, 2, 100);
                break;
              case 8:
                script_22C91("bgm047.snd", 23, 2, 100);
                break;
              case 9:
                script_22C91("bgm075.snd", 24, 2, 100);
                break;
              case 10:
                script_22C91("bgm010.snd", 25, 2, 100);
                break;
            }
            break;
          case 3:
            local_E0 = task_50F;
            switch (local_E0) {
              case 1:
                script_22C91("bgm125.snd", 8, 2, 100);
                break;
              case 2:
                script_22C91("bgm074.snd", 9, 2, 100);
                break;
              case 3:
                script_22C91("bgm053.snd", 10, 2, 100);
                break;
              case 4:
                script_22C91("bgm124.snd", 11, 2, 100);
                break;
              case 5:
                script_22C91("bgm015.snd", 20, 2, 100);
                break;
              case 6:
                script_22C91("bgm016.snd", 21, 2, 100);
                break;
              case 7:
                script_22C91("bgm014.snd", 22, 2, 100);
                break;
              case 8:
                script_22C91("bgm047.snd", 23, 2, 100);
                break;
              case 9:
                script_22C91("bgm075.snd", 24, 2, 100);
                break;
              case 10:
                script_22C91("bgm010.snd", 25, 2, 100);
                break;
            }
            break;
          case 4:
            local_E0 = task_50F;
            switch (local_E0) {
              case 1:
                script_22C91("bgm054.snd", 12, 2, 100);
                break;
              case 2:
                script_22C91("bgm092.snd", 13, 2, 100);
                break;
              case 3:
                script_22C91("bgm086.snd", 14, 2, 100);
                break;
              case 4:
                script_22C91("bgm048.snd", 15, 2, 100);
                break;
              case 5:
                script_22C91("bgm015.snd", 20, 2, 100);
                break;
              case 6:
                script_22C91("bgm016.snd", 21, 2, 100);
                break;
              case 7:
                script_22C91("bgm014.snd", 22, 2, 100);
                break;
              case 8:
                script_22C91("bgm047.snd", 23, 2, 100);
                break;
              case 9:
                script_22C91("bgm075.snd", 24, 2, 100);
                break;
              case 10:
                script_22C91("bgm010.snd", 25, 2, 100);
                break;
            }
            break;
          case 5:
            local_E0 = task_50F;
            switch (local_E0) {
              case 1:
                script_22C91("bgm021.snd", 16, 2, 100);
                break;
              case 2:
                script_22C91("bgm060.snd", 17, 2, 100);
                break;
              case 3:
                script_22C91("bgm090.snd", 18, 2, 100);
                break;
              case 4:
                script_22C91("bgm072.snd", 19, 2, 100);
                break;
              case 5:
                script_22C91("bgm015.snd", 20, 2, 100);
                break;
              case 6:
                script_22C91("bgm016.snd", 21, 2, 100);
                break;
              case 7:
                script_22C91("bgm014.snd", 22, 2, 100);
                break;
              case 8:
                script_22C91("bgm047.snd", 23, 2, 100);
                break;
              case 9:
                script_22C91("bgm075.snd", 24, 2, 100);
                break;
              case 10:
                script_22C91("bgm010.snd", 25, 2, 100);
                break;
            }
            break;
        }
        SCNF_WaitFrames_0000(40);
        script_23C12();
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_23A8F(void) {
  return;
}

static void script_23A92(int32_t arg0) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_E4;
  float local_E0;
  int32_t local_DC;
  local_E4 = -0.0015f;
  local_E0 = -0.00069f;
  local_DC = task_50F - 1;
  if ((local_DC > 20) | (local_DC < 0)) {
  } else {
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), *(int16_t *)((int8_t *)&data_421F0 + (local_DC << 1)), &local_F4, &local_E8, CHARA_PART_Get);
    if (arg0) {
      (&local_F4)->y = (&local_F4)->y + local_E4;
      (&local_F4)->z = (&local_F4)->z + local_E0;
    } else {
      (&local_F4)->y = (&local_F4)->y - local_E4;
      (&local_F4)->z = (&local_F4)->z - local_E0;
    }
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), *(int16_t *)((int8_t *)&data_421F0 + (local_DC << 1)), &local_F4, &local_E8, CHARA_PART_Set);
  }
  return;
}

static void script_23B44(void) {
  Script_executeAicaCommand(0x1200A0, 2, 0);
  task_4D1 = 0;
  task_4D2 = 0;
  task_4D3 = 0;
  task_4D5 = 0;
  task_4DC = Script_scenCreateTask(script_23D10, 0);
  task_4E0 = Script_scenCreateTask(script_23E2F, 0);
  task_4E8 = Script_scenCreateTask(script_24019, 0);
  task_4F0 = Script_scenCreateTask(script_24218, 0);
  task_4F4 = Script_scenCreateTask(script_242D2, 0);
  task_4F8 = Script_scenCreateTask(script_24372, 0);
  if (task_50C == 1) {
    task_4D4 = 1;
  } else {
    task_4D1 = 1;
    Script_executeAicaCommand(394409, 0, 0);
    task_4D2 = 1;
    Script_executeAicaCommand(66729, 0, 0);
  }
  return;
}

static void script_23C12(void) {
  Script_srchControl((void *)task_4DC);
  Script_srchControl((void *)task_4E0);
  Script_srchControl((void *)task_4E8);
  Script_srchControl((void *)task_4F0);
  Script_srchControl((void *)task_4F4);
  Script_srchControl((void *)task_4F8);
  {
    Vector3 local_F4;
    Vector3i local_E8;
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), 159, &local_F4, &local_E8, CHARA_PART_Get);
    (&local_E8)->z = (&local_E8)->z + 5825;
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), 159, &local_F4, &local_E8, CHARA_PART_Set);
  }
  {
    Vector3 local_F4;
    Vector3i local_E8;
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), 160, &local_F4, &local_E8, CHARA_PART_Get);
    (&local_E8)->z = (&local_E8)->z + 5825;
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), 160, &local_F4, &local_E8, CHARA_PART_Set);
  }
  {
    Vector3 local_F4;
    Vector3i local_E8;
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), 161, &local_F4, &local_E8, CHARA_PART_Get);
    (&local_E8)->z = (&local_E8)->z + 5825;
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), 161, &local_F4, &local_E8, CHARA_PART_Set);
  }
  Script_charaPosRotUtils(SHEN_NAME('JBOX'), 161, &data_41E70, 0, CHARA_PART_SetScale);
  Script_charaPosRotUtils(SHEN_NAME('JBOX'), 160, &data_41E7C, 0, CHARA_PART_SetScale);
  return;
}

static int32_t script_23D10(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_E4 = 0;
  for (;;) {
    if (Script_scenCheckCharaExist(SHEN_NAME('JBOX')) == 0) {
      break;
    }
    local_E0 = task_4D1;
    switch (local_E0) {
      case 1:
        if (local_E4 < 90) {
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 153, &local_F4, &local_E8, CHARA_PART_Get);
          local_E8.x = (&local_E8)->x - 364;
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 153, &local_F4, &local_E8, CHARA_PART_Set);
          local_E4 = local_E4 + 1;
        } else {
          local_E4 = 0;
          task_4D6 = 1;
          Script_executeAicaCommand(525481, 0, 0);
          task_4D7 = 1;
          task_4D8 = 1;
          task_4D1 = 0;
        }
        break;
      case 2:
        if (local_E4 < 90) {
          if (local_E4 == 44) {
            task_4D2 = 2;
          }
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 153, &local_F4, &local_E8, CHARA_PART_Get);
          local_E8.x = (&local_E8)->x + 364;
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 153, &local_F4, &local_E8, CHARA_PART_Set);
          local_E4 = local_E4 + 1;
        } else {
          local_E4 = 0;
          task_4D1 = 0;
        }
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_23E2F(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_E4 = 0;
  for (;;) {
    if (Script_scenCheckCharaExist(SHEN_NAME('JBOX')) == 0) {
      break;
    }
    local_E0 = task_4D2;
    switch (local_E0) {
      case 1:
        if (local_E4 < 45) {
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 154, &local_F4, &local_E8, CHARA_PART_Get);
          (&local_E8)->z = (&local_E8)->z + 364;
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 154, &local_F4, &local_E8, CHARA_PART_Set);
          local_E4 = local_E4 + 1;
        } else {
          local_E4 = 0;
          task_4D2 = 0;
        }
        break;
      case 2:
        if (local_E4 < 45) {
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 154, &local_F4, &local_E8, CHARA_PART_Get);
          (&local_E8)->z = (&local_E8)->z - 364;
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 154, &local_F4, &local_E8, CHARA_PART_Set);
          local_E4 = local_E4 + 1;
        } else {
          local_E4 = 0;
          task_4D4 = 3;
          task_4D2 = 0;
        }
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_23F2F(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t unused_E4;
  int32_t local_E0;
  for (;;) {
    if (Script_scenCheckCharaExist(SHEN_NAME('JBOX')) == 0) {
      break;
    }
    local_E0 = task_4D3;
    switch (local_E0) {
      case 1:
        Script_charaPosRotUtils(SHEN_NAME('JBOX'), 155, &local_F4, &local_E8, CHARA_PART_Get);
        local_E8.x = (&local_E8)->x - 1601;
        Script_charaPosRotUtils(SHEN_NAME('JBOX'), 155, &local_F4, &local_E8, CHARA_PART_Set);
        break;
      case 2:
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_23FA1(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t unused_E4;
  int32_t local_E0;
  for (;;) {
    if (Script_scenCheckCharaExist(SHEN_NAME('JBOX')) == 0) {
      break;
    }
    local_E0 = task_4D5;
    switch (local_E0) {
      case 1:
        Script_charaPosRotUtils(SHEN_NAME('JBOX'), 157, &local_F4, &local_E8, CHARA_PART_Get);
        (&local_E8)->y = (&local_E8)->y - 1601;
        Script_charaPosRotUtils(SHEN_NAME('JBOX'), 157, &local_F4, &local_E8, CHARA_PART_Set);
        break;
      case 2:
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_24019(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_E4 = 0;
  for (;;) {
    if (Script_scenCheckCharaExist(SHEN_NAME('JBOX')) == 0) {
      break;
    }
    local_E0 = task_4D4;
    switch (local_E0) {
      case 1:
        task_4D3 = 0;
        task_4D5 = 0;
        if (local_E4 < 13) {
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, CHARA_PART_Get);
          local_E8.x = (&local_E8)->x - 18;
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, CHARA_PART_Set);
          local_E4 = local_E4 + 1;
        } else {
          local_E4 = 0;
          task_4D4 = 2;
        }
        break;
      case 2:
        if (local_E4 < 17) {
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, CHARA_PART_Get);
          (&local_E8)->y = (&local_E8)->y + 182;
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, CHARA_PART_Set);
          local_E4 = local_E4 + 1;
        } else {
          local_E4 = 0;
          task_4D4 = 0;
          task_4D1 = 1;
          Script_executeAicaCommand(394409, 0, 0);
          task_4D2 = 1;
          Script_executeAicaCommand(66729, 0, 0);
        }
        break;
      case 3:
        if (local_E4 < 17) {
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, CHARA_PART_Get);
          (&local_E8)->y = (&local_E8)->y - 182;
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, CHARA_PART_Set);
          local_E4 = local_E4 + 1;
        } else {
          local_E4 = 0;
          task_4D4 = 0;
          task_4FC = 1;
        }
        break;
      case 4:
        if (local_E4 < 13) {
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, CHARA_PART_Get);
          local_E8.x = (&local_E8)->x + 18;
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, CHARA_PART_Set);
          local_E4 = local_E4 + 1;
        } else {
          local_E4 = 0;
          Script_executeAicaCommand(197801, 0, 0);
          task_4D4 = 0;
        }
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_24218(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_E4 = 0;
  for (;;) {
    if (Script_scenCheckCharaExist(SHEN_NAME('JBOX')) == 0) {
      break;
    }
    local_E0 = task_4D6;
    switch (local_E0) {
      case 1:
        if (local_E4 < 32) {
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 159, &local_F4, &local_E8, CHARA_PART_Get);
          (&local_E8)->z = (&local_E8)->z - 182;
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 159, &local_F4, &local_E8, CHARA_PART_Set);
          local_E4 = local_E4 + 1;
        } else {
          local_E4 = 0;
          task_4D6 = 0;
          Script_executeAicaCommand(591017, 0, 0);
          Script_executeAicaCommand(459945, 0, 0);
        }
        break;
      case 2:
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_242D2(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_E4 = 0;
  for (;;) {
    if (Script_scenCheckCharaExist(SHEN_NAME('JBOX')) == 0) {
      break;
    }
    local_E0 = task_4D7;
    switch (local_E0) {
      case 1:
        if (local_E4 < 32) {
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 160, &local_F4, &local_E8, CHARA_PART_Get);
          (&local_E8)->z = (&local_E8)->z - 182;
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 160, &local_F4, &local_E8, CHARA_PART_Set);
          local_E4 = local_E4 + 1;
        } else {
          local_E4 = 0;
          task_4D7 = 0;
        }
        break;
      case 2:
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_24372(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_E4 = 0;
  for (;;) {
    if (Script_scenCheckCharaExist(SHEN_NAME('JBOX')) == 0) {
      break;
    }
    local_E0 = task_4D8;
    switch (local_E0) {
      case 1:
        Script_charaPosRotUtils(SHEN_NAME('JBOX'), 161, &data_41E7C, 0, CHARA_PART_SetScale);
        Script_charaPosRotUtils(SHEN_NAME('JBOX'), 155, &data_41E70, 0, CHARA_PART_SetScale);
        if (local_E4 < 32) {
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 161, &local_F4, &local_E8, CHARA_PART_Get);
          (&local_E8)->z = (&local_E8)->z - 182;
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 161, &local_F4, &local_E8, CHARA_PART_Set);
          local_E4 = local_E4 + 1;
        } else {
          local_E4 = 0;
          task_4D8 = 0;
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 160, &data_41E70, 0, CHARA_PART_SetScale);
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 155, &data_41E7C, 0, CHARA_PART_SetScale);
          task_4D1 = 2;
          Script_executeAicaCommand(66729, 0, 0);
        }
        break;
      case 2:
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_2447E(void) {
  int32_t local_FC;
  Script_aicaGetOrSetState(8);
  for (;;) {
    local_FC = Script_aicaGetOrSetState(8);
    if (local_FC == 1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_ReturnTimeDate(&task_4C8);
  if ((task_4C8.hours >= 19) & (task_4C8.minutes >= 0)) {
    Script_executeAicaCommand(0x1200A0, 2, 0);
    while (Script_aicaGetOrSetState(2)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_srchControl(0);
  return;
}

__attribute__((scn_string_bindings(0, data_42218, 1, data_4221E)))
static void script_244F3(void) {
  Script_LoadVoiceFile(0, "E1067");
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('D_TE'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('ASTD'), 0);
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  Script_FreeTaskData(task_4C0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "ALP_JUKE");
  return;
}

static void script_2455C(void) {
  Script_SetShouldSleepFlag(1);
  Script_TimedTransitionUtils(TIMED_TRANSITION_SET_ENABLED, 1);
  return;
}

static void script_2456E(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  if (task_50C == 0) {
  } else {
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, CHARA_PART_Get);
    local_E8.x = (&local_E8)->y = (&local_E8)->z = 0;
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, CHARA_PART_Set);
    task_4D3 = 0;
    task_4D5 = 0;
    task_50C = 0;
  }
  return;
}

static int32_t script_245CB(void) {
  int32_t local_FC;
  Vector3 local_F0;
  Vector3i local_E4;
  Vector3 local_D8;
  Vector3i local_CC;
  Vector3 local_C0;
  local_FC = 0;
  if (task_50E & 2) {
    (void)0;
  } else {
    if (Script_IsPlayerAllowed(0)) {
      Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F0, ENTITY_XFORM_WORLD_SPACE);
      Script_mtwkGetSetRotation(CHARA_AKIR, &local_E4, 0x42000000);
      Script_EVT_GetPosition(SHEN_NAME('JCNT'), -1, &local_D8, ENTITY_XFORM_WORLD_SPACE);
      Script_mtwkGetSetRotation(SHEN_NAME('JCNT'), &local_CC, 0x40000000);
      local_C0.x = (&local_F0)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E4)->y) * 0.3f;
      (&local_C0)->y = (&local_F0)->y;
      (&local_C0)->z = (&local_F0)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E4)->y) * 0.3f;
      if (Script_CheckTargetWithinAngleRange(&local_C0, 1.4f, (&local_E4)->y, 7281, &local_D8)) {
        if (Script_CheckTargetWithinAngleRange(&local_D8, 1.5f, (&local_CC)->y + 0x8000, 12743, &local_F0)) {
          local_FC = 1;
        }
      }
    }
    (void)local_FC;
  }
  end:;
}

static int32_t script_246BC(int32_t arg0) {
  int32_t local_FC;
  int32_t unused_F8;
  GameDateTime local_F0;
  int32_t local_EC;
  local_FC = 1;
  Script_ReturnTimeDate(&local_F0);
  local_EC = arg0;
  if (local_EC == 0) {
    if (((&local_F0)->hours >= 18) & ((&local_F0)->minutes >= 45)) {
      local_FC = 0;
    }
  } else {
    local_FC = 0;
  }
  return local_FC;
}

__attribute__((scn_string_bindings(0, data_42227, 1, data_42235)))
static void script_24703(void) {
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  script_EAA8("scene/02/MFSY", "ALPHA");
  script_109B5();
  Script_mapmSetCamFlags(1, 19, 1);
  Script_mapmSetRenderFlag(19, 1);
  script_2EEA6(95, 0);
  Script_mapmSetCamFlags(1, 9, 0);
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
  Script_SetGlobalSceneSlotValue34(9, 1);
  Script_SetGlobalSceneSlotValue34(10, 1);
  Script_SetGlobalSceneSlotValue34(11, 1);
  Script_SetGlobalSceneSlotValue34(12, 1);
  Script_SetGlobalSceneSlotValue34(13, 1);
  Script_SetGlobalSceneSlotValue34(14, 1);
  Script_SetGlobalSceneSlotValue34(15, 1);
  Script_SetGlobalSceneSlotValue34(16, 1);
  Script_PREP_Related(-1, 0);
  Script_SetupLightFromTable(11);
  Script_PREP_Related(19, 1);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_GetCurrentDrawMode(1, 1);
  Script_MT5Loader_TRWV_TRWAVE(2, task_254);
  (void)((task_2A1 = -1) & 4);
  Script_HATO_Funcs(StopTask);
  Script_GULLProgram_Utils(2);
  script_A671(CHARA_AKIR, -26.1f, 0.0f, 74.5f);
  script_A699(CHARA_AKIR, 0.0f, 135.0f, 0.0f);
  script_24A28();
  (void)((task_284 = 0) & 4);
  (void)((task_283 = 0) & 4);
  if (task_4BE == -1) {
    script_248D7();
  } else {
    Script_ryoControl(RYO_RestoreControl);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_4223B)))
static void script_248D7(void) {
  script_2E9F6("ALPHA", 95);
  script_A671(CHARA_AKIR, -23.0f, 0.0f, 73.0f);
  script_A699(CHARA_AKIR, 0.0f, 315.0f, 0.0f);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  if (task_26C.hours >= 19) {
    (void)((task_283 = 0) & 8);
  } else {
    (void)((task_283 = -1) & 8);
  }
  Script_ryoSleepUtils(3, 1, 8, 0);
  task_279 = 0;
  Script_convSetCamInactiveFlag(-1);
  Script_SetGameStateTo(AUTH_None);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_11125();
  return;
}

static int32_t script_249B5(void) {
  script_24A28();
  if (task_4BE == -1) {
    script_248D7();
  }
  end:;
}

static void script_249CC(void) {
  Script_executeAicaCommand(2720, 1, 30);
  Script_executeAicaCommand(2720, 2, 30);
  Script_executeAicaCommand(2720, 3, 30);
  if (~task_4BE) {
    Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  script_24B45();
  return;
}

static void script_24A28(void) {
  (void)((task_28A = 0) & 8);
  task_4B0 = -1;
  task_4B4 = -1;
  task_4BD = 0;
  (void)((task_4BE = 0) & 8);
  script_2EFCB(95);
  script_2216F(1, 4);
  Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, 2);
  while (task_7C.id == 0x52494B41) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(10);
  script_110D8();
  script_24D19();
  if (~task_4BE) {
    script_24E03();
    Script_Ambient1SoundUtils(2, 17);
    script_EB20();
    Script_LoadSND(0, 1, "amb048.snd", 0);
    script_D89F();
    Script_LoadSND(0, 2, "rim059.snd", 0);
    Script_executeAicaCommand(16296, 0, 0);
    Script_executeAicaCommand(1184, 2, 80);
    Script_LoadSND(0, 4, "f1alfabo.snd", 0);
    Script_LoadSND(0, 5, "e1jukbox.snd", 0);
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    script_11125();
  }
  return;
}

__attribute__((scn_string_bindings(0, data_42271, 1, data_4227C, 2, data_42283, 3, data_42290)))
static void script_24B45(void) {
  (void)((task_28A = -1) & 2);
  script_2F0D2();
  script_244F3();
  script_24E2D();
  Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, 0);
  Script_cdttLoadScrUtils(CDTT_CMD_SetMode, 0x5953464D);
  SCNF_WaitFrames_0000(30);
  Script_scenCreateTask(script_2E00E, 0);
  if (task_4BD == 0) {
    Script_Ambient1SoundUtils(2, 17);
    script_EB20();
    Script_LoadSND(0, 1, "amb013.snd", 0);
    script_D9D6();
    Script_LoadSND(0, 2, "SELECT", 0);
    Script_executeAicaCommand(16296, 0, 0);
    Script_executeAicaCommand(1184, 2, 100);
    Script_LoadSND(0, 4, "f1yokosu.snd", 0);
  } else {
    Script_Ambient1SoundUtils(2, 17);
    script_EB20();
    Script_LoadSND(0, 1, "amb013.snd", 0);
    script_D9D6();
  }
  task_4BC = 40;
  (void)((task_4BE = 0) & 2);
  return;
}

static void script_24C23(void) {
  DSWK_DoorUtils(DOOR_CMD_CLEAR_STATE);
  SCNF_WaitFrames_0000(1);
  Script_charaDisplay(0x30726F64, CDM_Hide);
  Script_charaDisplay(0x31726F64, CDM_Hide);
  Script_charaDisplay(0x32726F64, CDM_Hide);
  Script_charaDisplay(0x33726F64, CDM_Hide);
  Script_charaDisplay(0x34726F64, CDM_Hide);
  Script_charaDisplay(0x35726F64, CDM_Hide);
  Script_charaDisplay(0x36726F64, CDM_Hide);
  Script_charaDisplay(0x37726F64, CDM_Hide);
  Script_charaDisplay(0x38726F64, CDM_Hide);
  Script_charaDisplay(0x39726F64, CDM_Hide);
  return;
}

static void script_24CA2(void) {
  Script_charaDisplay(0x30726F64, CDM_Show);
  Script_charaDisplay(0x31726F64, CDM_Show);
  Script_charaDisplay(0x32726F64, CDM_Show);
  Script_charaDisplay(0x33726F64, CDM_Show);
  Script_charaDisplay(0x34726F64, CDM_Show);
  Script_charaDisplay(0x35726F64, CDM_Show);
  Script_charaDisplay(0x36726F64, CDM_Show);
  Script_charaDisplay(0x37726F64, CDM_Show);
  Script_charaDisplay(0x38726F64, CDM_Show);
  Script_charaDisplay(0x39726F64, CDM_Show);
  DSWK_DoorUtils(DOOR_CMD_SET_STATE_1);
  return;
}

static void script_24D19(void) {
  return;
}

static void script_24D1C(void) {
  if (task_4B4 == -1) {
    task_4B4 = script_E948(CHARA_MARK);
  }
  script_A671(CHARA_MARK, -1.73f, 0.0f, -2.48f);
  script_A699(CHARA_MARK, 0.0f, 188.0f, 0.0f);
  Script_PlayCharaMotion(CHARA_MARK, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ccowUtils(CHARA_MARK, CCOW_EnableAll);
  return;
}

static void script_24D95(void) {
  if (task_4B0 == -1) {
    task_4B0 = script_E948(CHARA_TMRA);
  }
  script_A671(CHARA_TMRA, -2.0f, 0.0f, -4.0f);
  script_A699(CHARA_TMRA, 0.0f, 200.0f, 0.0f);
  Script_PlayCharaMotion(CHARA_TMRA, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static void script_24E03(void) {
  if (Script_StepUtils(GET_STEP_HIGHER, 0) == 21) {
    if (Script_StepUtils(GET_STEP_LOWER, 0) == 6) {
      script_24D1C();
    }
  }
  return;
}

static void script_24E2D(void) {
  if (task_4B0 != -1) {
    script_EA10(CHARA_TMRA, task_4B0);
    task_4B0 = -1;
  }
  if (task_4B4 != -1) {
    script_EA10(CHARA_MARK, task_4B4);
    task_4B4 = -1;
  }
  return;
}

static int32_t script_24E70(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  while (SCN_GetIsFighting() != 2) {
    SCNF_WaitFrames_0000(1);
    if (Script_GetFightingStats(-1, 2) != 1) {
      local_FC = local_FC + 1;
    }
  }
  local_F8 = Script_PlayerStatsUtils(PS_GetShugyoCourseActiveFramesSlot, 4);
  local_F8 = local_F8 + local_FC;
  return Script_PlayerStatsUtils(PS_SetShugyoCourseActiveFramesSlot, 4, local_F8);
}

__attribute__((scn_string_bindings(0, data_4229B, 1, data_422A1, 2, data_422AB, 3, data_422B5, 4, data_422BB, 5, data_422C5, 6, data_422CF, 7, data_422D9, 8, data_422DF, 9, data_422E9, 10, data_422F3, 11, data_422FD, 12, data_42307, 13, data_42311, 14, data_4231B)))
static int32_t script_24ECB(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t unused_F0;
  GameDateTime local_E8;
  local_F4 = 0;
  local_FC = Script_StepUtils(GET_STEP_HIGHER, 0) * 100 + Script_StepUtils(GET_STEP_LOWER, 0);
  Script_ReturnTimeDate(&local_E8);
  local_F8 = (&local_E8)->hours * 100 + (&local_E8)->minutes;
  {
    int32_t local_E4;
    local_E4 = local_FC;
    if (local_E4 == 1901) {
      if (local_F8 > 1100) {
        Script_LoadVoiceFile(1, "E1040");
        if ((int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f) > 0) {
          Script_ShowUIMessageFromID("E1040A028");
        } else {
          Script_ShowUIMessageFromID("E1040A029");
        }
        local_F4 = 1;
      }
    } else {
      if ((local_E4 == 2202) | (local_E4 == 2203)) {
        Script_LoadVoiceFile(1, "E1040");
        local_E4 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
        if (local_E4 == 0) {
          Script_ShowUIMessageFromID("E1040A030");
        } else if (local_E4 == 1) {
          Script_ShowUIMessageFromID("E1040A031");
        } else {
          Script_ShowUIMessageFromID("E1040A032");
        }
        local_F4 = 1;
        goto L024FCB;
      }
    }
  }
  L024FCB:;
  if (~local_F4) {
    if ((local_FC > 2099) & (local_FC < 2200)) {
      if ((local_F8 >= 1330) & (local_F8 < 1700)) {
        Script_LoadVoiceFile(1, "E1040");
        if (local_F8 < 1400) {
          int32_t local_E4;
          local_E4 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
          if (local_E4 == 0) {
            Script_ShowUIMessageFromID("E1040A028");
          } else if (local_E4 == 1) {
            Script_ShowUIMessageFromID("E1040A029");
          } else {
            Script_ShowUIMessageFromID("E1040A032");
          }
        } else {
          int32_t local_E4;
          local_E4 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
          if (local_E4 == 0) {
            Script_ShowUIMessageFromID("E1040A030");
          } else if (local_E4 == 1) {
            Script_ShowUIMessageFromID("E1040A031");
          } else {
            Script_ShowUIMessageFromID("E1040A032");
          }
        }
        local_F4 = 1;
      }
    }
  }
  if (local_F4) {
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_LoadVoiceFile(0, "E1040");
  }
  if ((local_F8 >= 2200) | (local_F8 <= 829)) {
    local_F4 = 1;
  }
  return local_F4;
}

static int32_t script_250DD(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t unused_F0;
  GameDateTime local_E8;
  int32_t local_E4;
  local_F4 = 0;
  local_FC = Script_StepUtils(GET_STEP_HIGHER, 0) * 100 + Script_StepUtils(GET_STEP_LOWER, 0);
  Script_ReturnTimeDate(&local_E8);
  local_F8 = (&local_E8)->hours * 100 + (&local_E8)->minutes;
  local_E4 = local_FC;
  switch (local_E4) {
    case 602:
      if (local_F8 > 1430) {
        local_F4 = 1;
      }
      break;
    case 1901:
      if (local_F8 > 1200) {
        local_F4 = 1;
      }
      break;
  }
  if ((local_FC >= 2100) & (local_FC < 2200)) {
    if ((local_F8 >= 1330) & (local_F8 < 1700)) {
      local_F4 = 1;
    }
  }
  if (local_F8 >= 2200) {
    local_F4 = 1;
  }
  if (~local_F4 & -(local_F8 == 1900)) {
    local_F4 = 1;
  }
  return local_F4;
}

static void script_251AB(void) {
  int32_t local_FC;
  local_FC = 0;
  Script_wazaUtils(MOVE_UI_CMD_OPEN_MOVES_SCREEN, 0, 0, 0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_GetControllerInput(1) == 0x200) {
      break;
    }
    if (local_FC < 1200) {
      local_FC = local_FC + 1;
    } else {
      break;
    }
  }
  Script_wazaUtils(MOVE_UI_CMD_REQUEST_CLOSE_MOVES_SCREEN, 0, 0, 0);
  return;
}

static int32_t script_25204(int32_t arg0) {
  int32_t unused_E8[6];
  int32_t local_E4;
  local_E4 = 0;
  Script_shenClearFreeState(0);
  if (arg0 > 0) {
    L02521C:;
    local_E4 = local_E4 + 1;
    SCNF_WaitFrames_0000(1);
    if (!(local_E4 >= arg0)) goto L02521C;
  }
  Script_seqEnterEvtCam(2902, CHARA_AKIR, CHARA_AKIR);
  Script_shenClearFreeState(1);
  end:;
}

static int32_t script_2524F(int32_t arg0) {
  Script_EVT_SetPosition(arg0, 0x78000000, &data_42324);
  Script_charaDisplay(arg0, CDM_Hide);
  return Script_ccowUtils(arg0, CCOW_DisableAll);
}

static void script_25272(void) {
  int32_t local_FC;
  local_FC = 0x52494B41;
  Script_charaToggleMotEnabled(local_FC, 0);
  Script_PlayCharaMotion(local_FC, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, local_FC, &data_42330, 0x8000, 64);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, local_FC, 0.06f);
    if (Script_LineWalkUtils(LNWK_SCRIPT_GET_STATUS, local_FC) & 5) {
      SCNF_WaitFrames_0000(1);
      break;
    }
  }
  Script_charaToggleMotEnabled(local_FC, 1);
  Script_PlayCharaMotion(local_FC, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static void script_2531B(void) {
  SCNF_WaitFrames_0000(2);
  while (Script_IsInEventCam() != 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_2533B(void) {
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, 5, "e1shugyo.snd", 0x8000);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_LoadSND(4, 0, 0, 0);
}

static void script_25386(void) {
  int32_t local_FC;
  local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
  if (local_FC == 0) {
    Script_ShowUIMessageFromID("E1040A001");
  } else {
    Script_ShowUIMessageFromID("E1040A003");
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_253CD(void) {
  int32_t local_FC;
  local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
  switch (local_FC) {
    case 0:
      Script_ShowUIMessageFromID("E1040A009");
      break;
    case 1:
      Script_ShowUIMessageFromID("E1040A010");
      break;
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_25420(void) {
  int32_t local_FC;
  local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
  if (local_FC == 0) {
    Script_ShowUIMessageFromID("E1040A012");
  } else if (local_FC == 1) {
    Script_ShowUIMessageFromID("E1040A013");
  } else {
    Script_ShowUIMessageFromID("E1040A014");
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2547C(void) {
  int32_t local_FC;
  local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
  if (local_FC == 0) {
    Script_ShowUIMessageFromID("E1040A018");
  } else {
    Script_ShowUIMessageFromID("E1040A020");
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_254C3(void) {
  int32_t local_FC;
  local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
  if (local_FC == 0) {
    Script_ShowUIMessageFromID("E1040A015");
  } else if (local_FC == 1) {
    Script_ShowUIMessageFromID("E1040A016");
  } else {
    Script_ShowUIMessageFromID("E1040A017");
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_423C9)))
static void script_2551F(void) {
  int32_t unused_F0[4];
  Script_ryoControl(PCCTL_DisablePlayerControl);
  while (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_ryoControl(PCCTL_UpdateRyoNavYaw, 0x52494B41);
  Script_shenClearFreeState(0);
  script_25272();
  Script_scenCreateTask(script_25204, 1, 0);
  SCNF_WaitFrames_0000(1);
  Script_LoadVoiceFile(1, "E1040");
  Script_scenCreateTask(script_2524F, 1, SHEN_NAME('LUV2'));
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  Script_scenCreateTask(script_2533B, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_MIMAWASU_K, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_423CF)))
static void script_255FD(void) {
  Script_vfToggleBattleState(scnPostResultsSetup);
  while (SCN_GetIsFighting() != 2) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadVoiceFile(0, "E1040");
  SCNF_WaitFrames_0000(1);
  while (Script_GetConversationDetails(CONV_DETAILS_MODE_GET_GAME_STATE, 0) != 7) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  return;
}

static void script_25675(void) {
  int32_t local_FC;
  local_FC = 0;
  while (Script_GetFightingStats(-3, 2)) {
    if (local_FC > 1200) {
      Script_vfToggleBattleState(scnExitConfirmPending);
      break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_256B2(void) {
  int32_t local_FC;
  local_FC = 0;
  while (Script_GetFightingStats(-4, 2)) {
    if (local_FC > 300) {
      Script_vfToggleBattleState(scnExitConfirmPending);
      break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_256EF(int32_t arg0) {
  int32_t local_FC;
  int8_t local_FB;
  int8_t local_FA;
  int8_t local_F9;
  int32_t local_F4;
  local_F4 = arg0;
  if (local_F4 == 4) {
    local_FB = 58;
    local_FA = 34;
    local_F9 = local_FB;
  } else {
    local_FB = 37;
    local_FA = 34;
    local_F9 = local_FA;
  }
  if (SCNF_RequestActionSelectorFromParams_0008(0xFFFF, local_FB, local_FA, 0xFFFF, 300, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL) == local_F9) {
    local_FC = 1;
  } else {
    local_FC = -1;
  }
  return local_FC;
}

static void script_25760(int32_t arg0) {
  Vector3i local_F4;
  Script_scenSetAssignCVWK(CHARA_AKIR, 0, 1);
  Script_uiUtils(0, 15, 0);
  Script_vfSetup(0, 20, 20, 0, 0);
  task_0 = 0x52494B41;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 0;
  *(&task_0 + 3) = 0;
  *(&task_0 + 4) = 5;
  *(&task_0 + 5) = 0;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  task_0 = 0x3256554C;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 5;
  *(&task_0 + 5) = 21;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(SHEN_NAME('LUV2'), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  Script_filmCheckOrSetTransitionUtil(15, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_423D8);
  (&local_F4)->y = 0x2000;
  Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F4, 0);
  SCNF_WaitFrames_0000(1);
  Script_seqEnterEvtCam(2903, CHARA_AKIR, CHARA_AKIR);
  SCNF_WaitFrames_0000(1);
  script_2531B();
  SCN_Enqueue_VFGI(2, 99, 1.0f, 4, -1);
  Script_filmCheckOrSetTransitionUtil(15, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_scenCreateTask(script_24E70, 1, arg0);
  Script_shenClearFreeState(1);
  return;
}

static int32_t script_25972(void) {
  int32_t local_FC;
  local_FC = 0;
  if (Script_GetFightingStats(-1, 2) == 1) {
    local_FC = 1;
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE) == 1) {
    local_FC = 2;
  }
  if (script_250DD()) {
    local_FC = 2;
  }
  return local_FC;
}

static void script_259BA(void) {
  script_2547C();
  return;
}

static void script_259C2(void) {
  SCN_Enqueue_VFGI(2, 99, 1.0f, 99, -1);
  return;
}

static void script_259D6(int32_t arg0) {
  int8_t local_FF;
  int8_t local_FE;
  int32_t local_F8;
  local_FE = 0;
  local_FF = 0;
  if (Script_CycleManagerUtils(CYCLE_CMD_GET_NEAR_NPC_COUNT_CLAMPED) > 0) {
    local_FE = 1;
  }
  if (script_24ECB()) {
    local_FE = 1;
  }
  if (local_FE == 0) {
    script_2551F();
    local_FF = 1;
  }
  for (;;) {
    script_25675();
    script_256B2();
    if (local_FE == 1) {
      break;
      local_FF = 14;
    }
    SCNF_WaitFrames_0000(1);
    local_F8 = local_FF;
    if (local_F8 == 1) {
      script_25386();
      local_FF = 2;
    } else if (local_F8 == 2) {
      script_253CD();
      local_FF = 4;
    } else if ((local_F8 == 3) | (local_F8 == 13) | (local_F8 == 14)) {
      break;
    } else if ((local_F8 == 4) | (local_F8 == 12)) {
      local_FF = local_FF + script_256EF(local_FF);
    } else if (local_F8 == 5) {
      local_FE = 2;
      script_25420();
      script_25760(0);
      local_FF = 6;
    } else if (local_F8 == 6) {
      local_FF = local_FF + script_25972();
    } else if ((local_F8 == 7) | (local_F8 == 8)) {
      local_FF = local_FF - 7 + 9;
    } else {
      switch (local_F8) {
        case 9:
          if (Script_GetFightingStats(-1, 2) == 1) {
            script_259BA();
            local_FF = 12;
          }
          break;
        case 10:
          if (Script_GetFightingStats(-1, 2) == 1) {
            script_259BA();
            local_FF = 14;
          }
          break;
        case 11:
          script_259C2();
          local_FF = 6;
          break;
      }
    }
  }
  if (local_FE != 1) {
    script_255FD();
  }
  if (local_FE == 2) {
    Script_scenSetAssignCVWK(CHARA_AKIR, 0, 0);
    script_251AB();
    Script_PlayerStatsUtils(PS_SetShugyoCourseClearCountSlot, 4, Script_PlayerStatsUtils(PS_GetShugyoCourseClearCountSlot, 4) + 1);
  }
  if (local_FE != 1) {
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
    Script_SetGameStateTo(AUTH_None);
    Script_SetCPAMFlags(CHARA_AKIR, 1);
    Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
    Script_shenClearFreeState(1);
    Script_ryoControl(RYO_RestoreControl);
    SCNF_WaitFrames_0000(1);
    Script_uiUtils(0, 0, 0);
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  }
  return;
}

static void script_25C34(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_SPEND_BUDGET_BONUS_ACTIVE) == 0) {
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_SA1116_DOJO_TALK_DONE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_01LET_EVENT_DONE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_NINJIN_EVENT_DONE, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_HIHY_EVENT_DONE, 1);
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
  }
  return;
}

static void script_25F18(int32_t arg0) {
  int32_t unused_FC;
  int32_t local_F8;
  Vector3 local_EC;
  Vector3 local_E0;
  Vector3 local_D4;
  int32_t local_D0;
  script_26873();
  local_EC.x = (&local_EC)->y = (&local_EC)->z = 0.0f;
  local_E0.x = (&local_E0)->y = (&local_E0)->z = 0.0f;
  local_D4.x = (&local_D4)->y = (&local_D4)->z = 0.0f;
  Script_shenSeqUtils(-9, 0, 0);
  if (arg0) {
    Script_scenCreateTask(script_A39C, 0);
  }
  Script_shenSeqUtils(0);
  local_F8 = 0;
  while (Script_shenSeqUtils(-1)) {
    local_D0 = local_F8;
    switch (local_D0) {
      case 0:
        Script_FACE(CHARA_AKIR, 0, 0, 0);
        Script_FACE(CHARA_HISA, 0, 0, 0);
        Script_FACE(CHARA_HINA, 0, 0, 0);
        local_EC.x = 1.1f;
        (&local_EC)->y = 1.45f;
        (&local_EC)->z = 101.829002f;
        Script_FaceLookAtTargetUtils(CHARA_HINA, FACE_LookAtPosition, &local_EC, 0);
        if (-1) {
          Script_charaDisplay(CHARA_AKIR, CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(CHARA_AKIR, CDM_Show);
            goto L02601D;
          }
        }
        L02601D:;
        Script_scenToggleVisibleFor(CHARA_AKIR, 0);
        if (-1) {
          Script_charaDisplay(CHARA_JOE_, CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(CHARA_JOE_, CDM_Show);
            goto L026051;
          }
        }
        L026051:;
        Script_scenToggleVisibleFor(CHARA_JOE_, 0);
        if (-1) {
          Script_charaDisplay(CHARA_MIHO, CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(CHARA_MIHO, CDM_Show);
            goto L026085;
          }
        }
        L026085:;
        Script_scenToggleVisibleFor(CHARA_MIHO, 0);
        break;
      case 80:
        Script_FaceLookAtTargetUtils(CHARA_HINA, FACE_LookReset);
        break;
      case 265:
        Script_FACE(CHARA_HINA, 2, 0, 14);
        break;
      case 341:
        Script_FACE(CHARA_HISA, 2, 0, 10);
        break;
      case 461:
        Script_FACE(CHARA_HINA, 5, 0, 0);
        break;
      case 550:
        Script_FaceLookAtTargetUtils(CHARA_HISA, FACE_LookReset);
        break;
      case 600:
        if (0) {
          Script_charaDisplay(CHARA_AKIR, CDM_Hide);
        } else {
          if (-1) {
            Script_charaDisplay(CHARA_AKIR, CDM_Show);
            goto L02614E;
          }
        }
        L02614E:;
        Script_scenToggleVisibleFor(CHARA_AKIR, 1);
        break;
      case 660:
        Script_ResetHandMotion(CHARA_HISA, 0, 1, 0);
        Script_ResetHandMotion(CHARA_HISA, 0, 0, 0);
        Script_FaceLookAtTargetUtils(CHARA_HISA, FACE_LookReset);
        Script_FaceEx(CHARA_HINA, FACE_ExpressionDisabled, 0, 1);
        Script_FaceEx(CHARA_HISA, FACE_ExpressionDisabled, 0, 1);
        Script_scenToggleActiveFor(CHARA_HISA, 0);
        Script_FaceLookAtTargetUtils(CHARA_HINA, FACE_LookReset);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 50.0f);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 50.0f);
        break;
      case 661:
        Script_FACE(CHARA_HISA, 3, 0, 40);
        break;
      case 700:
        Script_scenToggleActiveFor(CHARA_HISA, 1);
        Script_charaDisplay(CHARA_JOE_, CDM_Show);
        Script_charaDisplay(CHARA_MIHO, CDM_Show);
        Script_ResetHandMotion(CHARA_HISA, 1, 1, 0);
        Script_ResetHandMotion(CHARA_HISA, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_HISA, 1, data_3D3BC, 1);
        Script_handSetFloatsFor(CHARA_HISA, 0, data_3D3BC, 1);
        Script_FaceEx(CHARA_HISA, FACE_ExpressionEnabled, 0, 1);
        Script_FACE(CHARA_HISA, 3, 0, 1);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 120.0f);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 120.0f);
        break;
      case 770:
        Script_FACE_LCG(CHARA_HISA, FACE_BlinkOpen, 0, 0);
        break;
      case 780:
        local_E0.x = 0.3452f;
        (&local_E0)->y = 1.5234f;
        (&local_E0)->z = 102.561401f;
        Script_FaceLookAtTargetUtils(CHARA_HISA, FACE_LookAtPosition, &local_E0, 5);
        Script_FACE_LCG(CHARA_HISA, FACE_BlinkClose, 3, 0);
        break;
      case 781:
        Script_FACE(CHARA_HISA, 7, 0, 40);
        if (-1) {
          Script_charaDisplay(CHARA_HINA, CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(CHARA_HINA, CDM_Show);
            goto L026354;
          }
        }
        L026354:;
        Script_scenToggleVisibleFor(CHARA_HINA, 0);
        if (-1) {
          Script_charaDisplay(CHARA_NMNO, CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(CHARA_NMNO, CDM_Show);
            goto L026388;
          }
        }
        L026388:;
        Script_scenToggleVisibleFor(CHARA_NMNO, 0);
        if (-1) {
          Script_charaDisplay(CHARA_ISYM, CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(CHARA_ISYM, CDM_Show);
            goto L0263BC;
          }
        }
        L0263BC:;
        Script_scenToggleVisibleFor(CHARA_ISYM, 0);
        break;
      case 784:
        Script_FACE_LCG(CHARA_HISA, FACE_BlinkOpen, 3, 0);
        break;
      case 786:
        Script_FaceLookAtTargetUtils(CHARA_HISA, FACE_LookReset);
        break;
      case 790:
        Script_FACE_LCG(CHARA_HISA, FACE_BlinkAutomatic, 6, 0);
        break;
      case 800:
        if (0) {
          Script_charaDisplay(CHARA_JOE_, CDM_Hide);
        } else {
          if (-1) {
            Script_charaDisplay(CHARA_JOE_, CDM_Show);
            goto L026450;
          }
        }
        L026450:;
        Script_scenToggleVisibleFor(CHARA_JOE_, 1);
        if (0) {
          Script_charaDisplay(CHARA_MIHO, CDM_Hide);
        } else {
          if (-1) {
            Script_charaDisplay(CHARA_MIHO, CDM_Show);
            goto L026484;
          }
        }
        L026484:;
        Script_scenToggleVisibleFor(CHARA_MIHO, 1);
        break;
      case 871:
        Script_FACE(CHARA_HISA, 0, 0, 30);
        break;
      case 1200:
        local_E0.x = 0.97f;
        (&local_E0)->y = 1.54f;
        (&local_E0)->z = 102.698952f;
        Script_FaceLookAtTargetUtils(CHARA_HISA, FACE_LookAtPosition, &local_E0, 0);
        break;
      case 1415:
        Script_FACE(CHARA_AKIR, 7, 0, 18);
        break;
      case 1491:
        Script_FACE(CHARA_HISA, 3, 0, 30);
        break;
      case 1649:
        Script_FaceLookAtTargetUtils(CHARA_HISA, FACE_LookReset);
        break;
    }
    if ((local_F8 >= 81) & (local_F8 < 550)) {
      Script_EVT_GetPosition(CHARA_HINA, 5, &local_E0, 0);
      Script_FaceLookAtTargetUtils(CHARA_HISA, FACE_LookAtPosition, &local_E0, 0);
    }
    if ((local_F8 >= 81) & (local_F8 < 660)) {
      Script_EVT_GetPosition(CHARA_HISA, 5, &local_D4, 0);
      Script_FaceLookAtTargetUtils(CHARA_HINA, FACE_LookAtPosition, &local_D4, 0);
    }
    local_F8 = local_F8 + 1;
    SCNF_WaitFrames_0000(1);
  }
  if (Script_shenSeqUtils(-7) == 1) {
    if (-1) {
      Script_charaDisplay(CHARA_HINA, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(CHARA_HINA, CDM_Show);
        goto L0265E9;
      }
    }
    L0265E9:;
    Script_scenToggleVisibleFor(CHARA_HINA, 0);
    if (-1) {
      Script_charaDisplay(CHARA_NMNO, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(CHARA_NMNO, CDM_Show);
        goto L02661D;
      }
    }
    L02661D:;
    Script_scenToggleVisibleFor(CHARA_NMNO, 0);
    if (-1) {
      Script_charaDisplay(CHARA_ISYM, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(CHARA_ISYM, CDM_Show);
        goto L026651;
      }
    }
    L026651:;
    Script_scenToggleVisibleFor(CHARA_ISYM, 0);
  }
  return;
}

static void script_2665E(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  script_269D5();
  Script_shenSeqUtils(-9, 1, 0);
  Script_shenSeqUtils(1);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_F8 = local_FC;
    switch (local_F8) {
      case 0:
        Script_ResetHandMotion(CHARA_HISA, 1, 1, 0);
        Script_ResetHandMotion(CHARA_HISA, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_HISA, 1, data_3D3BC, 0);
        Script_handSetFloatsFor(CHARA_HISA, 0, data_3D3BC, 0);
        Script_FACE(CHARA_HISA, 0, 0, 0);
        break;
      case 73:
        Script_FACE(CHARA_HISA, 1, 0, 17);
        break;
      case 175:
        Script_handSetFloatsFor(CHARA_HISA, 1, data_3D4A0, 5);
        break;
      case 255:
        Script_handSetFloatsFor(CHARA_HISA, 1, data_3D3BC, 10);
        break;
      case 300:
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        Script_executeAicaCommand(2720, 2, 30);
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_26788(int32_t arg0) {
  int32_t unused_FC;
  int32_t local_F8;
  int32_t unused_D8[8];
  int32_t local_D4;
  script_269D5();
  Script_shenSeqUtils(-9, 2, 0);
  Script_shenSeqUtils(2);
  local_F8 = 0;
  while (Script_shenSeqUtils(-1)) {
    local_D4 = local_F8;
    switch (local_D4) {
      case 0:
        Script_FACE(CHARA_HISA, 0, 0, 0);
        break;
      case 81:
        Script_FACE(CHARA_HISA, 6, 0, 19);
        break;
      case 140:
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        Script_executeAicaCommand(2720, 2, 30);
        break;
    }
    local_F8 = local_F8 + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_26830(int32_t arg0) {
  script_269D5();
  Script_shenSeqUtils(-9, 3, 0);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_shenSeqUtils(3);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_26873(void) {
  script_269D5();
  Script_handSetFloatsFor(CHARA_HISA, 1, data_3D3BC, 1);
  Script_handSetFloatsFor(CHARA_HISA, 0, data_3D3BC, 1);
  script_A614(CHARA_HINA);
  script_A614(CHARA_NMNO);
  script_A614(CHARA_ISYM);
  script_A614(CHARA_JOE_);
  script_A614(CHARA_MIHO);
  script_A2BF(CHARA_HINA);
  script_A2BF(CHARA_NMNO);
  script_A2BF(CHARA_ISYM);
  script_A2BF(CHARA_MIHO);
  script_A2BF(CHARA_JOE_);
  Script_SetCharaFlags(CHARA_HISA, 4);
  Script_SetCharaFlags(CHARA_NMNO, 4);
  Script_SetCharaFlags(CHARA_ISYM, 4);
  Script_SetCharaFlags(CHARA_MIHO, 4);
  Script_SetCharaFlags(CHARA_JOE_, 4);
  if (-1) {
    Script_charaDisplay(CHARA_JOE_, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_JOE_, CDM_Show);
      goto L02697A;
    }
  }
  L02697A:;
  Script_scenToggleVisibleFor(CHARA_JOE_, 0);
  if (-1) {
    Script_charaDisplay(CHARA_MIHO, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_MIHO, CDM_Show);
      goto L0269AE;
    }
  }
  L0269AE:;
  Script_scenToggleVisibleFor(CHARA_MIHO, 0);
  Script_ccowSetHeightMode(CHARA_HINA, CCOW_KeepHeight);
  Script_FaceEx(CHARA_HINA, FACE_ExpressionEnabled, 0, 1);
  return;
}

static void script_269D5(void) {
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 110);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 120.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 120.0f);
  script_A614(CHARA_AKIR);
  script_A614(CHARA_HISA);
  script_A2BF(CHARA_AKIR);
  script_A2BF(CHARA_HISA);
  script_A62C(CHARA_AKIR);
  script_A62C(CHARA_HISA);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetCharaFlags(CHARA_HISA, 4);
  Script_SetMotnFlag(CHARA_HISA, -1);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMaxInterpSpeed(0.0f);
  return;
}

static void script_26A83(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_SetCPAMFlags(arg0, 0);
  Script_CharHandlers(arg0, 2, 8, 16);
  return;
}

static void script_26AB6(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_ccowSetHeightMode(arg0, CCOW_FollowGround);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_CharHandlers(arg0, -1, 0, 0);
  Script_SetCPAMFlags(arg0, 1);
  return;
}

static void script_26AF2(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_SetCPAMFlags(arg0, 0);
  Script_SetMotnFlag(arg0, -1);
  Script_facemotUtils(arg0, 0, 0);
  Script_facemotUtils(arg0, 1, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 1);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 1, data_39ED0, 1);
  Script_handSetFloatsFor(arg0, 0, data_39ED0, 1);
  Script_FACE_LCG(arg0, FACE_BlinkAutomatic, 6, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  return;
}

static void script_26B8F(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_ccowSetHeightMode(arg0, CCOW_FollowGround);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  Script_FACE(CHARA_AKIR, 0, 1, 0);
  Script_FaceEx(arg0, FACE_ExpressionDisabled, 0, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_CharHandlers(arg0, -1, 0, 0);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  return;
}

static int32_t script_26BF8(void) {
  task_518 = 0;
  for (;;) {
    switch (task_518) {
      case 1:
        Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
        Script_ResetHandMotion(CHARA_SYZU, 1, 1, 0);
        Script_ResetHandMotion(CHARA_SYZU, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_SYZU, 1, data_39ED0, 0);
        Script_handSetFloatsFor(CHARA_SYZU, 0, data_39ED0, 0);
        Script_SetHandRootRotation(CHARA_SYZU, 1, 0x15, &data_425E8);
        Script_SetHandRootRotation(CHARA_SYZU, 0, 0x15, &data_425F4);
        Script_scenToggleActiveFor(CHARA_SYZU, 0);
        Script_scenToggleActiveFor(CHARA_KEBB, 0);
        Script_scenToggleActiveFor(CHARA_KEBC, 0);
        break;
      case 81:
        Script_scenToggleActiveFor(CHARA_SYZU, 1);
        Script_scenToggleActiveFor(CHARA_KEBB, 1);
        Script_scenToggleActiveFor(CHARA_KEBC, 1);
        break;
      case 391:
        Script_scenToggleActiveFor(CHARA_KEBB, 0);
        Script_scenToggleActiveFor(CHARA_KEBC, 0);
        break;
    }
    task_518 = task_518 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static float script_26D21(void) {
  task_518 = 0;
  Script_SetShadowRange(50.0f);
  for (;;) {
    switch (task_518) {
      case 1:
        Script_ResetHandMotion(CHARA_SYZU, 1, 1, 0);
        Script_ResetHandMotion(CHARA_SYZU, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_SYZU, 1, data_39ED0, 0);
        Script_handSetFloatsFor(CHARA_SYZU, 0, data_39ED0, 0);
        break;
      case 760:
        Script_executeAicaCommand(2720, 2, 30);
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        break;
    }
    task_518 = task_518 + 1;
    SCNF_WaitFrames_0000(1);
  }
  return Script_SetShadowRange(20.0f);
}

static int32_t script_26DEA(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  task_518 = 0;
  for (;;) {
    switch (task_518) {
      case 1:
        Script_ResetHandMotion(CHARA_SYZU, 1, 1, 0);
        Script_ResetHandMotion(CHARA_SYZU, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_SYZU, 1, data_39ED0, 0);
        Script_handSetFloatsFor(CHARA_SYZU, 0, data_39ED0, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_423E4, 0);
        break;
      case 150:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_39ED0, 16);
        Script_handSetFloatsFor(CHARA_SYZU, 1, data_424C8, 1);
        Script_SetHandRootRotation(CHARA_SYZU, 1, 0x15, &data_425AC);
        break;
    }
    if (task_518 < 150) {
      Script_charaDisplay(SHEN_NAME('CAN1'), CDM_Show);
      Script_fixoAttachEx(SHEN_NAME('CAN1'), CHARA_AKIR, 18, &data_425D0, &data_425B8);
    } else {
      Script_charaDisplay(SHEN_NAME('CAN1'), CDM_Show);
      Script_fixoAttachEx(SHEN_NAME('CAN1'), CHARA_SYZU, 18, &data_425DC, &data_425C4);
    }
    if (task_518 < 151) {
      Script_EVT_GetPosition(CHARA_SYZU, 18, &local_F4, 0);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F4, 0);
    }
    if (task_518 >= 871) {
      local_E8.x = 60.0f;
      (&local_E8)->y = -10.0f;
      (&local_E8)->z = 32.0f;
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_E8, 4);
    }
    switch (task_518) {
      case 876:
        Script_FACE(CHARA_AKIR, 1, 0, 4);
        break;
      case 1270:
        Script_executeAicaCommand(2720, 2, 30);
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        break;
    }
    task_518 = task_518 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_26FD4(void) {
  Vector3 local_F4;
  task_518 = 0;
  for (;;) {
    switch (task_518) {
      case 1:
        Script_ResetHandMotion(CHARA_SYZU, 1, 1, 0);
        Script_ResetHandMotion(CHARA_SYZU, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_SYZU, 1, data_39ED0, 0);
        Script_handSetFloatsFor(CHARA_SYZU, 0, data_39ED0, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_423E4, 0);
        break;
      case 150:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_39ED0, 16);
        Script_handSetFloatsFor(CHARA_SYZU, 1, data_424C8, 1);
        Script_SetHandRootRotation(CHARA_SYZU, 1, 0x15, &data_425AC);
        break;
    }
    if (task_518 < 150) {
      Script_charaDisplay(task_514, CDM_Show);
      Script_fixoAttachEx(task_514, CHARA_AKIR, 18, &data_425D0, &data_425B8);
    } else {
      Script_charaDisplay(task_514, CDM_Show);
      Script_fixoAttachEx(task_514, CHARA_SYZU, 18, &data_425DC, &data_425C4);
    }
    if (task_518 >= 381) {
      local_F4.x = 60.0f;
      (&local_F4)->y = -10.0f;
      (&local_F4)->z = 32.0f;
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F4, 4);
    }
    if (task_518 == 700) {
      Script_executeAicaCommand(2720, 2, 30);
      Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
    }
    task_518 = task_518 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_2716B(void) {
  script_2717E(1);
  task_524 = 0;
  return;
}

static void script_2717E(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  Script_executeAicaCommand(2720, 2, 30);
  Script_ReturnTimeDate(&local_F4);
  script_27EEC();
  Script_ryoControl(PCCTL_RequestNewMode, 1);
  if (arg0 == 1) {
    Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  script_272EE(arg0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  if ((&local_F4)->hours > 18) {
    script_27514();
  }
  script_2753A(arg0);
  script_276C5(arg0);
  script_2763D(arg0);
  if ((&local_F4)->hours > 18) {
    script_27527();
  }
  script_27450(arg0);
  script_27E10();
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  script_27F3E(arg0);
  return;
}

__attribute__((scn_string_bindings(0, data_42600, 1, data_42607)))
static void script_27254(void) {
  script_EB20();
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, 2, "SELECT", 0x8000);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  Script_LoadSND(1, 4, "f1yokosu.snd", 0x8000);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadSND(4, 0, 0, 0);
  Script_srchControl(0);
  return;
}

__attribute__((scn_string_bindings(2, data_4262C, 3, data_42631, 4, data_42636, 5, data_4263B, 6, data_4263C, 7, data_42641, 8, data_42642, 9, data_42647, 10, data_42648, 11, data_4264D, 12, data_4264E)))
static void script_272EE(int32_t arg0) {
  script_EB20();
  Script_LoadSND(0, 2, "bgm073.snd", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 94);
  Script_LoadSND(0, 4, "a1_hurou.snd", 0);
  script_27DEC("HAKO");
  script_27DEC("FSJH");
  {
    int32_t local_FC;
    local_FC = arg0;
    switch (local_FC) {
      case 1:
        Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
        Script_AuthorUtils(0, 0, "FSJH", "");
        Script_AuthorUtils(0, 1, "FSJH", "");
        Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
        break;
      case 2:
        Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
        Script_AuthorUtils(0, 2, "FSJH", "");
        Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
        break;
      case 3:
        Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
        Script_AuthorUtils(0, 3, "FSJH", "");
        Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
        break;
    }
  }
  {
    int32_t local_FC;
    local_FC = arg0;
    if (local_FC == 1) {
      task_268 = script_E948(CHARA_SYZU);
      task_51C = script_E948(CHARA_KEBB);
      task_520 = script_E948(CHARA_KEBC);
      goto L027443;
    }
  }
  L027443:;
  Script_LoadVoiceFile(1, "01FRO");
  return;
}

__attribute__((scn_string_bindings(0, data_42654, 1, data_4265A, 2, data_4265F, 3, data_42666)))
static void script_27450(int32_t arg0) {
  int32_t local_FC;
  Script_LoadVoiceFile(0, "01FRO");
  local_FC = arg0;
  switch (local_FC) {
    case 1:
      script_EA10(CHARA_KEBC, task_520);
      script_EA10(CHARA_KEBB, task_51C);
      Script_AuthorUtils(AUTH_FREE, 1);
      Script_AuthorUtils(AUTH_FREE, 0);
      break;
    case 2:
      Script_AuthorUtils(AUTH_FREE, 2);
      break;
    case 3:
      Script_AuthorUtils(AUTH_FREE, 3);
      break;
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "FSJH");
  script_EB20();
  Script_LoadSND(0, 2, "SELECT", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  Script_LoadSND(0, 4, "f1yokosu.snd", 0);
  return;
}

static void script_27514(void) {
  Script_PREP_Related(-1, 0);
  Script_SetupLightFromTable(12);
  return;
}

static void script_27527(void) {
  Script_SetupLightFromTable(0);
  Script_PREP_Related(-1, 1);
  return;
}

static void script_2753A(int32_t arg0) {
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_42674);
  Script_SetCharaRefBoardPos(CHARA_SYZU, &data_42674);
  Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetCharaFlags(CHARA_AKIR, 0);
  Script_SetShadowFlags(CHARA_SYZU, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_scenToggleActiveFor(SHEN_NAME('DOOR'), 0);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(CHARA_SYZU, 1);
  script_26AF2(CHARA_AKIR);
  script_26AF2(CHARA_SYZU);
  if (arg0 == 1) {
    Script_SetCharaRefBoardPos(CHARA_KEBB, &data_42674);
    Script_SetCharaRefBoardPos(CHARA_KEBC, &data_42674);
    Script_SetShadowFlags(CHARA_KEBB, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
    Script_SetShadowFlags(CHARA_KEBC, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
    Script_charaSetRefboardFlagTo(CHARA_KEBB, 1);
    Script_charaSetRefboardFlagTo(CHARA_KEBC, 1);
    script_26A83(CHARA_KEBB);
    script_26A83(CHARA_KEBC);
  }
  return;
}

static void script_2763D(int32_t arg0) {
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_charaSetRefboardFlagTo(CHARA_SYZU, 2);
  script_26B8F(CHARA_AKIR);
  script_26B8F(CHARA_SYZU);
  Script_faceUpdateFor(CHARA_SYZU, 1, 0);
  Script_faceUpdateFor(CHARA_SYZU, 0, 0);
  if (arg0 == 1) {
    Script_charaSetRefboardFlagTo(CHARA_KEBB, 2);
    Script_charaSetRefboardFlagTo(CHARA_KEBC, 2);
    script_26AB6(CHARA_KEBB);
    script_26AB6(CHARA_KEBC);
  }
  return;
}

static void script_276C5(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = arg0;
  switch (local_F8) {
    case 1:
      {
        {
          Vector3 local_EC;
          local_EC.x = 0.0f;
          (&local_EC)->z = 0.0f;
          Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, &local_EC);
        }
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        local_FC = Script_scenCreateTask(script_26BF8, 0);
        Script_shenSeqUtils(0);
        while (Script_shenSeqUtils(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_srchControl((void *)local_FC);
        local_FC = Script_scenCreateTask(script_26D21, 0);
        Script_charaDisplay(CHARA_KEBB, CDM_Hide);
        Script_charaDisplay(CHARA_KEBC, CDM_Hide);
        Script_charaDisplay(SHEN_NAME('JHAN'), CDM_Hide);
        Script_shenSeqUtils(1);
        while (Script_shenSeqUtils(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_srchControl((void *)local_FC);
        Script_InitializeOsag(CHARA_AKIR);
        {
          Vector3 local_EC;
          local_EC.x = 63.97f;
          (&local_EC)->z = 32.28f;
          Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, &local_EC);
        }
        {
          Vector3i local_EC;
          (&local_EC)->y = 27488;
          Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_EC, 0);
        }
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        break;
      }
    case 2:
      {
        Script_charaPosRotUtils(SHEN_NAME('CAN1'), 153, &data_42694, &data_426A0, CHARA_PART_Set);
        Script_charaSetRefboardFlagTo(SHEN_NAME('CAN1'), 1);
        Script_SetCharaRefBoardPos(SHEN_NAME('CAN1'), &data_42684);
        local_FC = Script_scenCreateTask(script_26DEA, 0);
        Script_shenSeqUtils(2);
        Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        while (Script_shenSeqUtils(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_srchControl((void *)local_FC);
        Script_InitializeOsag(CHARA_AKIR);
        {
          Vector3 local_EC;
          local_EC.x = 65.43f;
          (&local_EC)->z = 35.43f;
          Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, &local_EC);
        }
        {
          Vector3i local_EC;
          (&local_EC)->y = 54613;
          Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_EC, 0);
        }
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        break;
      }
    case 3:
      {
        Script_charaPosRotUtils(task_514, 153, &data_42694, &data_426A0, CHARA_PART_Set);
        Script_charaSetRefboardFlagTo(task_514, 1);
        Script_SetCharaRefBoardPos(task_514, &data_42684);
        local_FC = Script_scenCreateTask(script_26FD4, 0);
        Script_shenSeqUtils(3);
        Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        while (Script_shenSeqUtils(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_srchControl((void *)local_FC);
        Script_InitializeOsag(CHARA_AKIR);
        {
          Vector3 local_EC;
          local_EC.x = 65.43f;
          (&local_EC)->z = 35.43f;
          Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, &local_EC);
        }
        {
          Vector3i local_EC;
          (&local_EC)->y = 54613;
          Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_EC, 0);
        }
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        break;
      }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_426AC, 1, data_426B1, 2, data_426B7)))
static void script_27A17(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t unused_F4;
  GameDateTime local_EC;
  int32_t local_E8;
  int8_t local_E7;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  (void)((local_E7 = 0) & 0x20);
  local_FC = Script_shenGetStats(STATS_CASH);
  Script_ReturnTimeDate(&local_EC);
  if (Script_scenUtils(SCENARIO_CMD_QUERY_FOCUS_OR_TIME_LIMIT) == 0) {
    if (local_FC > 99) {
      uint8_t unused_E4[3];
      int32_t local_E0;
      if ((&local_EC)->hours < 19) {
        script_10529();
      }
      script_C474(arg0);
      Script_scenCreateTask(script_BFD9, 1, local_FC);
      Script_scenCreateTask(script_B896, 2, arg2, "VEND");
      script_CA3D(arg0, arg1);
      script_C54F(arg0, arg1, arg2, arg3);
      local_F8 = script_C6F9(arg0, arg1, arg2, arg3, 2);
      local_E0 = local_F8;
      switch (local_E0) {
        case 13:
          Script_LoadVoiceFile(1, "E1042");
          local_E8 = script_27C43(arg0, arg1);
          (void)((local_E7 = -1) & 1);
          Script_LoadVoiceFile(0, "E1042");
          break;
        case 16:
          local_FC = Script_shenGetStats(STATS_CASH);
          Script_scenCreateTask(script_BFFF, 1, local_FC);
          break;
      }
      script_C5F0(arg0, arg1, arg2, arg3);
      if ((&local_EC)->hours < 19) {
        script_104AA();
      }
    }
  }
  if (local_E7 & 0x80) {
    uint8_t unused_E4[3];
    int32_t local_E0;
    local_E0 = local_E8;
    switch (local_E0) {
      case 2:
        task_514 = 0x314E4143;
        script_2717E(3);
        break;
      case 1:
        task_514 = 0x314E4143;
        script_2717E(3);
        break;
      case 3:
        task_514 = 0x314E4143;
        script_2717E(3);
        break;
      case 0:
        task_514 = 0x314E4143;
        script_2717E(3);
        break;
      case 5:
        task_514 = 0x314E4143;
        script_2717E(3);
        break;
      case 4:
        script_2717E(2);
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_COFFEE_SPECIAL_VARIANT, 1);
        break;
    }
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_COFFEE_EVENT_COMPLETE, 1);
    script_B90B(task_220);
    Script_ryoControl(RYO_RestoreControl);
    Script_uiUtils(0, 0, 0);
    (void)1;
  } else {
    Script_ryoControl(RYO_RestoreControl);
    (void)0;
  }
  return;
}

static int32_t script_27C43(int32_t arg0, int32_t arg1) {
  Script_shenClearFreeState(0);
  script_B896(task_21C, task_220);
  if (task_224 == 4) {
    script_BA3B(task_21C, arg0, 18, (int32_t)&data_426D8, (int32_t)&data_426E4, 82);
    script_BD03(arg0, arg1, (int32_t)&data_426F0, -1);
    script_BD68(arg0, arg1, (int32_t)data_426FC, -1);
    script_BC57(arg0, 1, (int32_t)data_42714, -1);
    script_BC10(arg0, 1, 544, -1);
  } else {
    script_BA3B(task_21C, arg0, 18, (int32_t)&data_426C0, (int32_t)&data_426CC, 82);
    script_BD03(arg0, arg1, (int32_t)&data_426F0, -1);
    script_BD68(arg0, arg1, (int32_t)data_426FC, -1);
    script_BC57(arg0, 1, (int32_t)&data_42708, -1);
    script_BC10(arg0, 1, 545, -1);
  }
  script_BC10(arg0, 0, 546, -1);
  script_BBA0(arg0, 45059, 0, -0x80000000, -1);
  script_B9B3(0x6405A9, 80);
  script_BDAC(4014, arg1, arg1, -1);
  SCNF_WaitFrames_0000(10);
  script_BA8F(arg0, 0);
  Script_seqEnterEvtCam(3101, CHARA_AKIR, CHARA_AKIR);
  Script_ShowUIMessageFromID("E1042A012");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(15);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_fixoClearAttachment(task_21C);
  return task_224;
}

__attribute__((scn_string_bindings(0, data_4272A)))
static void script_27DEC(const char *arg0) {
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg0) == 0) {
    script_EAA8("scene/02/MFSY", arg0);
  }
  return;
}

static void script_27E10(void) {
  {
    Vector3 local_F4;
    local_F4.x = 61.78f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 31.91f;
    Script_EVT_SetPosition(CHARA_SYZU, POS_XYZ, &local_F4);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    (&local_E8)->y = 0x2000;
    Script_EVT_SetRotation(CHARA_SYZU, MATRIX_XFORM_Y, &local_E8, 0);
  }
  {
    Vector3 local_F4;
    local_F4.x = 61.7f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 31.9f;
    Script_EVT_SetPosition(SHEN_NAME('KBAK'), POS_XYZ, &local_F4);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    (&local_E8)->y = 0x2000;
    Script_EVT_SetRotation(SHEN_NAME('KBAK'), MATRIX_XFORM_Y, &local_E8, 0);
  }
  Script_charaToggleMotEnabled(CHARA_SYZU, 0);
  Script_PlayCharaMotion(CHARA_SYZU, 29953, 800.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmGetCharaFlags(CHARA_SYZU, MOTM_PoseNone, SCEN_Write);
  Script_SetCharaMotionPlaybackRate(CHARA_SYZU, 0.0f);
  return;
}

static void script_27EEC(void) {
  Script_SetShouldSleepFlag(0);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_convSetCamInactiveFlag(1);
  return;
}

static void script_27F3E(int32_t arg0) {
  int32_t local_FC;
  Script_seqEnterEvtCam(3120, 0, AUTH_None);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  local_FC = arg0;
  if ((local_FC == 1) | (local_FC == 2) | (local_FC == 3)) {
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    goto L027FA1;
  }
  L027FA1:;
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_SetShouldSleepFlag(1);
  return;
}

__attribute__((scn_string_bindings(0, data_42748, 1, data_4274E)))
static void script_27FEA(void) {
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_42738);
  Script_SetCharaRefBoardPos(CHARA_SYZU, &data_42738);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(CHARA_SYZU, 1);
  script_27EEC();
  SCNF_RequestIconNotification(0, 0, 0, 2);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  Script_LoadVoiceFile(1, "E1042");
  script_2809C();
  Script_LoadVoiceFile(0, "E1042");
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  SCNF_RequestIconNotification(0x100, 0, 0, 1);
  script_27F3E(0);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_charaSetRefboardFlagTo(CHARA_SYZU, 2);
  return;
}

static void script_2809C(void) {
  Vector3i local_F4;
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  Script_ccowUtils(CHARA_SYZU, CCOW_DisableAll);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_motmToggleFlags(CHARA_SYZU, SCEN_Disable, 0x40);
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_SetCPAMFlags(CHARA_SYZU, 0);
  Script_InitializeOsag(CHARA_AKIR);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_42754);
  (&local_F4)->y = 0x2000;
  Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F4, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &data_42760, 0);
  Script_SetCharaLookAtPosition(CHARA_SYZU, 2, &data_4276C, 0);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_COFFEE_EVENT_COMPLETE) == 0) {
    script_28267(task_524 * 3);
    script_28267(task_524 * 3 + 1);
    script_28267(task_524 * 3 + 2);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_COFFEE_SPECIAL_VARIANT) == 1) {
    script_28267(task_524 * 3 + 18);
  } else {
    script_28267(task_524 * 3 + 9);
  }
  task_524 = task_524 + 1;
  if (task_524 == 3) {
    task_524 = 0;
  }
  Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
  Script_SetCharaLookAtPosition(CHARA_SYZU, -20, 0, 0);
  Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
  Script_ccowUtils(CHARA_SYZU, CCOW_EnableAll);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_motmToggleFlags(CHARA_SYZU, SCEN_Enable, 0x40);
  Script_SetCPAMFlags(CHARA_AKIR, 1);
  Script_SetCPAMFlags(CHARA_SYZU, 1);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
  return;
}

static void script_28267(int32_t arg0) {
  {
    int32_t local_FC;
    local_FC = arg0;
    switch (local_FC) {
      case 0:
        Script_ShowUIMessageFromID("E1042B030");
        break;
      case 1:
        Script_ShowUIMessageFromID("E1042A010");
        break;
      case 2:
        Script_ShowUIMessageFromID("E1042B031");
        break;
      case 3:
        Script_ShowUIMessageFromID("E1042B032");
        break;
      case 4:
        Script_ShowUIMessageFromID("E1042A011");
        break;
      case 6:
        Script_ShowUIMessageFromID("E1042B033");
        break;
      case 9:
        Script_ShowUIMessageFromID("E1042B041");
        break;
      case 12:
        Script_ShowUIMessageFromID("E1042B043");
        break;
      case 15:
        Script_ShowUIMessageFromID("E1042B044");
        break;
      case 18:
        Script_ShowUIMessageFromID("E1042B038");
        break;
      case 21:
        Script_ShowUIMessageFromID("E1042B039");
        break;
      case 24:
        Script_ShowUIMessageFromID("E1042B040");
        break;
    }
  }
  {
    int32_t local_FC;
    local_FC = arg0;
    switch (local_FC) {
      case 0:
        Script_seqEnterEvtCam(3120, 0, AUTH_None);
        break;
      case 2:
        Script_seqEnterEvtCam(3121, 0, AUTH_None);
        break;
      case 3:
        Script_scenCreateTask(script_2846D, 0);
        break;
      case 6:
        Script_seqEnterEvtCam(3124, 0, AUTH_None);
        break;
      case 9:
        Script_seqEnterEvtCam(3130, 0, AUTH_None);
        break;
      case 12:
        Script_seqEnterEvtCam(3131, 0, AUTH_None);
        break;
      case 15:
        Script_seqEnterEvtCam(3132, 0, AUTH_None);
        break;
      case 18:
        Script_seqEnterEvtCam(3140, 0, AUTH_None);
        break;
      case 21:
        Script_seqEnterEvtCam(3141, 0, AUTH_None);
        break;
      case 24:
        Script_seqEnterEvtCam(3142, 0, AUTH_None);
        break;
    }
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_2846D(void) {
  Script_seqEnterEvtCam(3122, 0, AUTH_None);
  SCNF_WaitFrames_0000(75);
  Script_seqEnterEvtCam(3123, 0, AUTH_None);
  return Script_srchControl(0);
}

static int32_t script_28494(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  if (local_FC == 0) {
    Script_ShowUIMessageFromID("F1040A061");
  } else if (local_FC == 1) {
    Script_ShowUIMessageFromID("F1040B002");
  } else if (local_FC == 2) {
    Script_ShowUIMessageFromID("F1040A062");
  } else if (local_FC == 3) {
    Script_ShowUIMessageFromID("F1040B090");
  } else if (local_FC == 4) {
    Script_ShowUIMessageFromID("F1040B009");
  } else if (local_FC == 5) {
    Script_ShowUIMessageFromID("F1040B008");
  } else if (local_FC == 6) {
    Script_ShowUIMessageFromID("F1040B091");
  } else if (local_FC == 7) {
    Script_ShowUIMessageFromID("F1040A063");
  } else if (local_FC == 8) {
    Script_ShowUIMessageFromID("F1040B092");
  } else if (local_FC == 9) {
    Script_ShowUIMessageFromID("F1040A064");
  } else if (local_FC == 10) {
    Script_ShowUIMessageFromID("F1040A065");
  } else if (local_FC == 11) {
    Script_ShowUIMessageFromID("F1040B093");
  } else if (local_FC == 12) {
    Script_ShowUIMessageFromID("F1040A014");
  } else if (local_FC == 13) {
    Script_ShowUIMessageFromID("F1040B094");
  } else if (local_FC == 14) {
    Script_ShowUIMessageFromID("F1040A027");
  } else if (local_FC == 15) {
    Script_ShowUIMessageFromID("F1040B095");
  } else if (local_FC == 16) {
    Script_ShowUIMessageFromID("F1040B096");
  } else if (local_FC == 17) {
    Script_ShowUIMessageFromID("F1040A066");
  } else if (local_FC == 18) {
    Script_ShowUIMessageFromID("F1040B097");
  } else {
    SCNT_ShowSubtitle("もうだめだめくん");
  }
  end:;
}

static void script_28631(int32_t arg0) {
  Script_scenCreateTask(script_28494, 1, arg0);
  return;
}

static void script_28641(void) {
  task_528 = 0;
  Script_scenCreateTask(script_28798, 0);
  Script_scenCreateTask(script_28AC4, 0);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_FaceEx(CHARA_HISA, FACE_ExpressionEnabled, 0, 2);
  Script_SetCPAMFlags(CHARA_HISA, 0);
  Script_charaSetRenderOpacity(CHARA_HISA, 1.0f);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(CHARA_HISA, 1);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  {
    Vector3 local_F4;
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
    (&local_F4)->y = 0.0f;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  }
  {
    Vector3 local_F4;
    Script_EVT_GetPosition(CHARA_HISA, -1, &local_F4, 0);
    (&local_F4)->y = 0.0f;
    Script_EVT_SetPosition(CHARA_HISA, POS_XYZ, &local_F4);
  }
  for (;;) {
    if (task_528 > 1410) {
      break;
    }
    task_528 = task_528 + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 798, 1);
  Script_filmCheckOrSetTransitionUtil(10, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_28798(void) {
  int32_t local_FC;
  Vector3 local_F0;
  for (;;) {
    local_FC = task_528;
    switch (local_FC) {
      case 1:
        if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x41534948)) {
          Script_EVT_GetPosition(CHARA_HISA, 4, &local_F0, ENTITY_XFORM_WORLD_SPACE);
          Script_SetLookPointDetails(CHARA_AKIR, 16, &local_F0, 2);
          Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F0, 30);
        }
        break;
      case 30:
        if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x41534948) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
          Script_EVT_GetPosition(CHARA_AKIR, 4, &local_F0, ENTITY_XFORM_WORLD_SPACE);
          Script_SetLookPointDetails(CHARA_HISA, 16, &local_F0, 2);
          Script_FaceLookAtTargetUtils(CHARA_HISA, FACE_LookAtPosition, &local_F0, 30);
        }
        break;
      case 240:
        Script_HandleLookPoint(CHARA_HISA, 15, &data_428C0, 2);
        Script_CharHandlers(CHARA_HISA, 0, 4, 16);
        break;
      case 270:
        Script_SetCharaLookAtPosition(CHARA_AKIR, 15, &data_428C0, 2);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
        break;
      case 390:
        if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x41534948)) {
          Script_EVT_GetPosition(CHARA_HISA, 4, &local_F0, ENTITY_XFORM_WORLD_SPACE);
          Script_SetLookPointDetails(CHARA_AKIR, 16, &local_F0, 2);
          Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F0, 30);
        }
        break;
      case 400:
        Script_HandleLookPoint(CHARA_HISA, -98312, &data_428C0, 2);
        Script_CharHandlers(CHARA_HISA, -1, 0, 0);
        break;
      case 530:
        if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
          Script_SetCharaLookAtPosition(CHARA_AKIR, -10, 0, 0);
          Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
        }
        break;
      case 660:
        if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x41534948)) {
          Script_EVT_GetPosition(CHARA_HISA, 4, &local_F0, ENTITY_XFORM_WORLD_SPACE);
          Script_SetLookPointDetails(CHARA_AKIR, 16, &local_F0, 2);
          Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F0, 30);
        }
        break;
      case 700:
        if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x41534948) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
          Script_EVT_GetPosition(CHARA_AKIR, 4, &local_F0, ENTITY_XFORM_WORLD_SPACE);
          Script_SetLookPointDetails(CHARA_HISA, 16, &local_F0, 2);
          Script_FaceLookAtTargetUtils(CHARA_HISA, FACE_LookAtPosition, &local_F0, 30);
        }
        break;
      case 1230:
        if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
          Script_SetCharaLookAtPosition(CHARA_AKIR, -10, 0, 0);
          Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
        }
        break;
      case 1400:
        if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x41534948)) {
          Script_SetCharaLookAtPosition(CHARA_HISA, -10, 0, 0);
          Script_FaceLookAtTargetUtils(CHARA_HISA, FACE_LookReset);
        }
        break;
      case 1401:
        goto L028AC3;
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  L028AC3:;
}

static int32_t script_28AC4(void) {
  int32_t local_FC;
  for (;;) {
    local_FC = task_528;
    switch (local_FC) {
      case 1:
        script_28631(0);
        Script_seqEnterEvtCam(450, SHEN_NAME('FOOD'), SHEN_NAME('FOOD'));
        break;
      case 60:
        script_28631(1);
        break;
      case 100:
        script_28631(2);
        Script_seqEnterEvtCam(451, SHEN_NAME('FOOD'), SHEN_NAME('FOOD'));
        break;
      case 180:
        script_28631(3);
        Script_seqEnterEvtCam(452, SHEN_NAME('FOOD'), SHEN_NAME('FOOD'));
        break;
      case 240:
        script_28631(4);
        break;
      case 290:
        script_28631(5);
        break;
      case 330:
        script_28631(6);
        break;
      case 420:
        script_28631(7);
        Script_seqEnterEvtCam(453, SHEN_NAME('FOOD'), SHEN_NAME('FOOD'));
        break;
      case 470:
        script_28631(8);
        Script_seqEnterEvtCam(454, SHEN_NAME('FOOD'), SHEN_NAME('FOOD'));
        break;
      case 560:
        script_28631(9);
        break;
      case 680:
        script_28631(10);
        Script_seqEnterEvtCam(455, SHEN_NAME('FOOD'), SHEN_NAME('FOOD'));
        break;
      case 790:
        script_28631(11);
        break;
      case 950:
        script_28631(12);
        Script_seqEnterEvtCam(456, SHEN_NAME('FOOD'), SHEN_NAME('FOOD'));
        break;
      case 1000:
        script_28631(13);
        break;
      case 1040:
        script_28631(14);
        break;
      case 1070:
        script_28631(15);
        break;
      case 1190:
        script_28631(16);
        Script_seqEnterEvtCam(457, SHEN_NAME('FOOD'), SHEN_NAME('FOOD'));
        break;
      case 1250:
        script_28631(17);
        break;
      case 1310:
        script_28631(18);
        break;
      case 1401:
        goto L028D0F;
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  L028D0F:;
}

__attribute__((scn_string_bindings(0, data_4291C, 1, data_4292A, 2, data_42930, 6, data_42954, 7, data_4295A)))
static void script_28D10(int32_t arg0) {
  Vector3 local_F4;
  Script_charaToggleMotEnabled(arg0, 0);
  script_EAA8("scene/02/MFSY", "BENTO");
  Script_LoadVoiceFile(1, "E1061");
  script_A671(CHARA_HISA, 1.28f, 0.0f, 103.3f);
  script_A699(CHARA_HISA, 0.0f, 50.0f, 0.0f);
  Script_charaToggleMotEnabled(CHARA_HISA, 0);
  Script_SetCPAMFlags(CHARA_HISA, 0);
  Script_charaSetRenderOpacity(CHARA_HISA, 1.0f);
  if (-1) {
    Script_charaDisplay(CHARA_HISA, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_HISA, CDM_Show);
      goto L028DB2;
    }
  }
  L028DB2:;
  Script_scenToggleVisibleFor(CHARA_HISA, 0);
  task_276 = 0;
  script_A671(CHARA_KEBA, -128.05101f, 0.861f, -8.641899f);
  script_A699(CHARA_KEBA, 0.0f, 267.0f, 0.0f);
  Script_SetCPAMFlags(CHARA_KEBA, 0);
  script_A671(arg0, -123.74f, 0.0f, -10.6349945f);
  script_A699(arg0, 0.0f, 90.0f, 0.0f);
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_charaSetRefboardFlagTo(CHARA_KEBA, 1);
  Script_charaSetRefboardFlagTo(CHARA_KEBC, 1);
  Script_charaSetRefboardFlagTo(CHARA_KEBD, 1);
  if (-1) {
    Script_charaDisplay(CHARA_KEBC, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_KEBC, CDM_Show);
      goto L028E7D;
    }
  }
  L028E7D:;
  Script_scenToggleVisibleFor(CHARA_KEBC, 0);
  if (-1) {
    Script_charaDisplay(CHARA_KEBD, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_KEBD, CDM_Show);
      goto L028EB1;
    }
  }
  L028EB1:;
  Script_scenToggleVisibleFor(CHARA_KEBD, 0);
  Script_charaVisUtils(SHEN_NAME('GROM'), 1);
  Script_charaVisUtils(SHEN_NAME('ISU_'), 1);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_ccowUtils(arg0, CCOW_DisableAll);
  script_2987A();
  script_298BF();
  SCNF_WaitFrames_0000(1);
  Script_PlayCharaMotion(CHARA_KEBA, 24582, 0.0f, 100.0f, 0.0f, 1.0f, 0);
  Script_RequestFightAnimation(arg0, 24577, 0.0f, 1, 0);
  Script_seqEnterEvtCam(400, 0, AUTH_None);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, arg0, &data_428CC, 0, 0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, arg0, 0.035f) == -1) {
      Script_PlayCharaMotion(arg0, MN_AKI_AKI_STAND_DOWN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      break;
    }
  }
  Script_charaToggleMotEnabled(arg0, -1);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TURN022_WAD_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x4142454B)) {
    Script_EVT_GetPosition(CHARA_KEBA, 4, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(arg0, 16, &local_F4, 2);
    Script_FaceLookAtTargetUtils(arg0, FACE_LookAtPosition, &local_F4, 30);
  }
  Script_seqEnterEvtCam(401, 0, AUTH_None);
  Script_ShowUIMessageFromID("E1061A001");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x4142454B) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
    Script_EVT_GetPosition(CHARA_AKIR, 4, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_KEBA, 16, &local_F4, 0);
    Script_FaceLookAtTargetUtils(CHARA_KEBA, FACE_LookAtPosition, &local_F4, 30);
  }
  Script_seqEnterEvtCam(402, SHEN_NAME('GROM'), SHEN_NAME('GROM'));
  Script_ShowUIMessageFromID("E1061B001");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(403, SHEN_NAME('GROM'), SHEN_NAME('GROM'));
  Script_ShowUIMessageFromID("E1061A002");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  script_2912A(CHARA_AKIR);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "BENTO");
  Script_LoadVoiceFile(0, "E1061");
  Script_ccowUtils(arg0, CCOW_EnableAll);
  return;
}

static void script_2912A(int32_t arg0) {
  Vector3 local_F4;
  Vector3 local_E8;
  script_A699(arg0, 0.0f, 158.76f, 0.0f);
  Script_seqEnterEvtCam(404, 0, AUTH_None);
  Script_ShowUIMessageFromID("E1061B002");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x4142454B)) {
    Script_SetCharaLookAtPosition(CHARA_KEBA, -13, 0, 0);
    Script_FaceLookAtTargetUtils(CHARA_KEBA, FACE_LookReset);
  }
  Script_ShowUIMessageFromID("E1061A003");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    Script_SetCharaLookAtPosition(arg0, -13, 0, 0);
    Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  }
  Script_SetRyoMotionDetails(arg0);
  SCNF_WaitFrames_0000(1);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_scenCreateTask(script_29904, 1, CHARA_AKIR);
  script_A671(CHARA_KEBC, -123.59f, 0.0f, -11.504997f);
  script_A699(CHARA_KEBC, 0.0f, 90.0f, 0.0f);
  script_A671(CHARA_KEBD, -123.59f, 0.0f, -12.684998f);
  script_A699(CHARA_KEBD, 0.0f, 90.0f, 0.0f);
  Script_EVT_GetPosition(CHARA_KEBC, -1, &local_F4, 0);
  Script_EVT_GetPosition(CHARA_KEBD, -1, &local_E8, 0);
  SCNF_WaitFrames_0000(290);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_SetCharaLookAtPosition(CHARA_AKIR, 98316, &local_F4, 0);
  SCNF_WaitFrames_0000(100);
  Script_seqEnterEvtCam(407, 0, AUTH_None);
  if (0) {
    Script_charaDisplay(CHARA_KEBC, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_KEBC, CDM_Show);
      goto L0292CF;
    }
  }
  L0292CF:;
  Script_scenToggleVisibleFor(CHARA_KEBC, 1);
  if (0) {
    Script_charaDisplay(CHARA_KEBD, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_KEBD, CDM_Show);
      goto L029303;
    }
  }
  L029303:;
  Script_scenToggleVisibleFor(CHARA_KEBD, 1);
  SCNF_WaitFrames_0000(120);
  Script_scenCreateTask(script_29A7E, 1, CHARA_KEBC);
  Script_scenCreateTask(script_29AED, 1, CHARA_KEBD);
  script_29B5C();
  SCNF_WaitFrames_0000(60);
  {
    int32_t unused_E4;
    Vector3 local_D8;
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x4142454B) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
      Script_EVT_GetPosition(CHARA_AKIR, 4, &local_D8, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(CHARA_KEBA, 16, &local_D8, 0);
      Script_FaceLookAtTargetUtils(CHARA_KEBA, FACE_LookAtPosition, &local_D8, 30);
    }
    SCNF_WaitFrames_0000(30);
    Script_ShowUIMessageFromID("E1061B003");
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x4142454B)) {
      Script_EVT_GetPosition(CHARA_KEBA, 4, &local_D8, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(CHARA_AKIR, 16, &local_D8, 2);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_D8, 30);
    }
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x4142454B)) {
    Script_SetCharaLookAtPosition(CHARA_KEBA, -13, 0, 0);
    Script_FaceLookAtTargetUtils(CHARA_KEBA, FACE_LookReset);
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1061A004");
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
    Script_SetCharaLookAtPosition(CHARA_AKIR, -13, 0, 0);
    Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_charaToggleMotEnabled(arg0, -1);
  {
    int32_t unused_E4;
    int32_t local_D8[3];
    local_D8[0] = 0;
    local_D8[1] = 0x4000;
    local_D8[2] = 0;
    local_D8[1] = Script_EVT_SetRotation(arg0, 0x14000000, (const Vector3i *)local_D8, (int32_t)&data_3D59C);
    for (;;) {
      SCNF_WaitFrames_0000(1);
      if (Script_IsCharaMotionFinished(arg0)) {
        Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, (const Vector3i *)local_D8, 0);
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        break;
      }
    }
    Script_scenCreateTask(script_29C0B, 0);
    SCNF_WaitFrames_0000(50);
    Script_seqEnterEvtCam(410, 0, AUTH_None);
    Script_ShowUIMessageFromID("E1061A005");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    SCNF_WaitFrames_0000(30);
    Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_A671(arg0, 0.0f, 0.0f, 101.8f);
    script_A699(arg0, 0.0f, 229.99f, 0.0f);
    if (0) {
      Script_charaDisplay(CHARA_HISA, CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(CHARA_HISA, CDM_Show);
        goto L0295D7;
      }
    }
    L0295D7:;
    Script_scenToggleVisibleFor(CHARA_HISA, 1);
    SCNF_WaitFrames_0000(1);
    SCNF_WaitFrames_0000(1);
    SCNF_WaitFrames_0000(45);
    Script_CycleManagerUtils(CYCLE_CMD_SET_CYCLE_MANAGER_ENABLED, 1);
    Script_seqEnterEvtCam(420, SHEN_NAME('FOOD'), SHEN_NAME('FOOD'));
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_charaSetRefboardFlagTo(CHARA_HISA, 1);
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x41534948)) {
      Script_EVT_GetPosition(CHARA_HISA, 4, (Vector3 *)local_D8, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(arg0, 16, (const Vector3 *)local_D8, 0);
      Script_FaceLookAtTargetUtils(arg0, FACE_LookAtPosition, (const Vector3 *)local_D8, 30);
    }
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x41534948) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
      Script_EVT_GetPosition(CHARA_AKIR, 4, (Vector3 *)local_D8, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(CHARA_HISA, 16, (const Vector3 *)local_D8, 0);
      Script_FaceLookAtTargetUtils(CHARA_HISA, FACE_LookAtPosition, (const Vector3 *)local_D8, 30);
    }
    SCNF_WaitFrames_0000(30);
    Script_ShowUIMessageFromID("E1061A006");
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x41534948) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
      Script_EVT_GetPosition(CHARA_AKIR, 4, (Vector3 *)local_D8, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(CHARA_HISA, 16, (const Vector3 *)local_D8, 0);
      Script_FaceLookAtTargetUtils(CHARA_HISA, FACE_LookAtPosition, (const Vector3 *)local_D8, 30);
    }
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(423, SHEN_NAME('FOOD'), SHEN_NAME('FOOD'));
  Script_ShowUIMessageFromID("E1061E001");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(422, SHEN_NAME('FOOD'), SHEN_NAME('FOOD'));
  Script_ShowUIMessageFromID("E1061A007");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1061E002");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(421, SHEN_NAME('FOOD'), SHEN_NAME('FOOD'));
  Script_ShowUIMessageFromID("E1061E003");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1061A008");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
    Script_SetCharaLookAtPosition(CHARA_AKIR, -13, 0, 0);
    Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x41534948)) {
    Script_SetCharaLookAtPosition(CHARA_HISA, -13, 0, 0);
    Script_FaceLookAtTargetUtils(CHARA_HISA, FACE_LookReset);
  }
  return;
}

static void script_2987A(void) {
  Vector3 local_F4;
  SCNF_WaitFrames_0000(1);
  Script_EVT_GetPosition(CHARA_AKIR, 18, &local_F4, 0);
  Script_EVT_SetPosition(SHEN_NAME('BNT1'), POS_XYZ, &local_F4);
  Script_fixoAttachEx(SHEN_NAME('BNT1'), CHARA_AKIR, 18, &data_429D0, &data_429DC);
  return;
}

static void script_298BF(void) {
  Vector3 local_F4;
  SCNF_WaitFrames_0000(1);
  Script_EVT_GetPosition(CHARA_AKIR, 12, &local_F4, 0);
  Script_EVT_SetPosition(SHEN_NAME('BNT2'), POS_XYZ, &local_F4);
  Script_fixoAttachEx(SHEN_NAME('BNT2'), CHARA_AKIR, 12, &data_429E8, &data_429F4);
  return;
}

static int32_t script_29904(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  Script_PlayCharaMotion(arg0, 24581, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    local_FC = local_FC + 1;
    local_F8 = local_FC;
    switch (local_F8) {
      case 2:
        Script_fixoAttachEx(SHEN_NAME('BNT1'), CHARA_AKIR, 18, &data_42A00, &data_42A0C);
        break;
      case 100:
        Script_seqEnterEvtCam(405, 0, AUTH_None);
        break;
      case 150:
        Script_executeAicaCommand(0x1C05A9, 0, 0);
        break;
      case 155:
        Script_fixoClearAttachment(SHEN_NAME('BNT1'));
        Script_scenToggleActiveFor(SHEN_NAME('BNT1'), 0);
        break;
      case 230:
        Script_executeAicaCommand(0x1C05A9, 0, 0);
        break;
      case 236:
        Script_fixoClearAttachment(SHEN_NAME('BNT2'));
        script_A671(SHEN_NAME('BNT2'), -128.541f, 1.3452f, -9.276497f);
        script_A699(SHEN_NAME('BNT2'), 180.0f, 281.9f, 180.0f);
        Script_scenToggleActiveFor(SHEN_NAME('BNT2'), 0);
        break;
      case 265:
        Script_seqEnterEvtCam(406, 0, AUTH_None);
        script_A671(SHEN_NAME('BNT1'), -129.473206f, 1.3365f, -9.3823f);
        break;
    }
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    Script_RotateNodeUtils(arg0, &data_39DC0, 0);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_29A7E(int32_t arg0) {
  Script_PlayCharaMotion(arg0, 24593, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaToggleMotEnabled(CHARA_KEBD, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, arg0, &data_428F4, 0, 0);
  for (;;) {
    if (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, arg0, 0.035f) == -1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_charaToggleMotEnabled(arg0, -1);
  end:;
}

static int32_t script_29AED(int32_t arg0) {
  Script_PlayCharaMotion(arg0, 24590, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaToggleMotEnabled(CHARA_KEBD, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, arg0, &data_42908, 0, 0);
  for (;;) {
    if (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, arg0, 0.035f) == -1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_charaToggleMotEnabled(arg0, -1);
  end:;
}

static void script_29B5C(void) {
  Script_seqEnterEvtCam(408, SHEN_NAME('GROM'), SHEN_NAME('GROM'));
  Script_ShowUIMessageFromID("E1061C001");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1061D001");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1061C002");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(409, SHEN_NAME('GROM'), SHEN_NAME('GROM'));
  Script_ShowUIMessageFromID("E1061C003");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaLookAtPosition(CHARA_AKIR, -98310, 0, 0);
  return;
}

static int32_t script_29C0B(void) {
  Vector3i local_F4;
  Script_charaToggleMotEnabled(CHARA_AKIR, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 24579, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    Script_RotateNodeUtils(CHARA_AKIR, &data_39DC0, 0);
    SCNF_WaitFrames_0000(1);
  }
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_F4, 0x42000000);
  Script_EVT_SetRotation(CHARA_AKIR, 0x78000000, &local_F4, 0);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  end:;
}

__attribute__((scn_string_bindings(0, data_42A68, 1, data_42A76, 2, data_42A7B, 16, data_42B03, 17, data_42B0D, 18, data_42B17, 23, data_42B49, 25, data_42B5D, 26, data_42B67, 31, data_42B99, 32, data_42BA3, 33, data_42BAD, 43, data_42C11, 44, data_42C17)))
static void script_29CB3(void) {
  int8_t local_FF;
  int8_t local_FE;
  local_FE = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 20) - 1;
  if (local_FE < 8) {
    local_FF = data_42A58[local_FE];
  } else {
    local_FF = 0;
  }
  if (local_FF) {
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
    Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
    script_EAA8("scene/02/MFSY", "FSJH");
    Script_LoadVoiceFile(1, "E1042");
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_ccowUtils(CHARA_SYZU, CCOW_DisableAll);
    Script_EVT_SetPosition(CHARA_SYZU, POS_XYZ, &data_42A40);
    {
      uint8_t unused_FC[2];
      Vector3i local_F0;
      (&local_F0)->y = 0xC000;
      Script_EVT_SetRotation(CHARA_SYZU, MATRIX_XFORM_Y, &local_F0, 0);
    }
    Script_charaDisplay(CHARA_SYZU, CDM_Show);
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_42A4C);
    {
      uint8_t unused_FC[2];
      Vector3i local_F0;
      (&local_F0)->y = 0x4000;
      Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F0, 0);
    }
    {
      uint8_t unused_FC[2];
      int32_t local_F8;
      local_F8 = local_FF;
      switch (local_F8) {
        case 1:
          Script_ShowUIMessageFromID("E1042B001");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042B002");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042B003");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042A001");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042B004");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042B005");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042B006");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042A003");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042B007");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042B008");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042B009");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042A004");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042B010");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042A005");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          break;
        case 2:
          Script_ShowUIMessageFromID("E1042B011");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042A006");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042B012");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042B013");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042A007");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042B014");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042A005");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042B015");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          break;
        case 3:
          Script_ShowUIMessageFromID("E1042B011");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042A006");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042B016");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042B017");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042B018");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042B019");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042A005");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          break;
        case 4:
          Script_ShowUIMessageFromID("E1042B011");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042A006");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042B020");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042A008");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042B021");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042B022");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042B023");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042B024");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042B025");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042A009");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1042B026");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          break;
      }
    }
    Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 23, local_FF);
    Script_charaDisplay(CHARA_SYZU, CDM_Hide);
    Script_ryoControl(RYO_RestoreControl);
    Script_LoadVoiceFile(0, "E1042");
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "FSJH");
  }
  SCNF_WaitFrames_0000(1);
  return;
}

__attribute__((scn_string_bindings(0, data_42C1C, 1, data_42C21, 2, data_42C26, 3, data_42C2B)))
static void script_2A2E2(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 3, "GKYU") == 0) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "GKYU");
  }
  Script_AuthorUtils(0, 0, "GKYU", "");
  Script_scenCreateTask(script_A39C, 0);
  script_A2A7();
  script_2A420();
  Script_shenSeqUtils(arg0);
  Script_scenToggleActiveFor(CHARA_AKIR, 0);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_F8 = local_FC;
    switch (local_F8) {
      case 723:
        Script_scenToggleActiveFor(CHARA_AKIR, 1);
        break;
      case 740:
        Script_FACE(CHARA_AKIR, 1, 0, 8);
        break;
      case 760:
        Script_FACE(CHARA_AKIR, 2, 0, 8);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_3C8FC, 8);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_3C8FC, 8);
        break;
      case 799:
        script_2A6BF(CHARA_AKIR);
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_AuthorUtils(AUTH_FREE, 0);
  script_2A5D1();
  return;
}

static void script_2A420(void) {
  script_A2BF(CHARA_AKIR);
  script_A2BF(CHARA_NGSM);
  script_A2BF(CHARA_KEBJ);
  script_A2BF(CHARA_KEBK);
  script_A2BF(CHARA_GORO);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_NGSM, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_KEBJ, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_KEBK, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_GORO, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(CHARA_NGSM, -1);
  Script_SetMotnFlag(CHARA_GORO, -1);
  Script_SetMotnFlag(CHARA_KEBJ, -1);
  Script_SetMotnFlag(CHARA_KEBK, -1);
  Script_SetMaxInterpSpeed(0.4f);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  script_2A684(CHARA_AKIR);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_42C2C);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_NGSM, &data_42C2C);
  Script_charaSetRefboardFlagTo(CHARA_NGSM, 1);
  Script_SetCharaRefBoardPos(CHARA_KEBJ, &data_42C2C);
  Script_charaSetRefboardFlagTo(CHARA_KEBJ, 1);
  Script_SetCharaRefBoardPos(CHARA_KEBK, &data_42C2C);
  Script_charaSetRefboardFlagTo(CHARA_KEBK, 1);
  Script_SetCharaRefBoardPos(CHARA_GORO, &data_42C2C);
  Script_charaSetRefboardFlagTo(CHARA_GORO, 1);
  Script_SetShadowRange(30.0f);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetCharaFlags(CHARA_NGSM, 0);
  Script_SetCharaFlags(CHARA_KEBJ, 0);
  Script_SetCharaFlags(CHARA_KEBK, 0);
  Script_SetCharaFlags(CHARA_GORO, 0);
  return;
}

static void script_2A5D1(void) {
  script_2A6BF(CHARA_AKIR);
  Script_SetMotnFlag(CHARA_AKIR, 0);
  Script_scenToggleActiveFor(CHARA_AKIR, 1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_FACE(CHARA_AKIR, 0, 0, 1);
  script_A34C(CHARA_AKIR);
  script_A34C(CHARA_NGSM);
  script_A34C(CHARA_KEBJ);
  script_A34C(CHARA_KEBK);
  script_A34C(CHARA_GORO);
  Script_facemotUtils(CHARA_AKIR, 0, 0);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetShadowRange(20.0f);
  return;
}

static void script_2A684(int32_t arg0) {
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 1, data_3CE54, 8);
  Script_handSetFloatsFor(arg0, 0, data_3CE54, 8);
  return;
}

static void script_2A6BF(int32_t arg0) {
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_CharHandlers(arg0, 2, 8, 16);
  return;
}

static void script_2A6E9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_2A703, 6, arg1, arg5, arg2, arg3, arg0, arg4);
  return;
}

static int32_t script_2A703(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_motmGetCharaFlags(arg4, MOTM_PoseLow, SCEN_Write);
  Script_charaToggleMotEnabled(arg4, 1);
  Script_motmToggleFlags(arg4, SCEN_Enable, 0x40);
  Script_SetCharaMotionPlaybackRate(arg4, arg3);
  Script_PlayCharaMotion(arg4, arg0, arg2, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(arg4, arg3);
  if (arg5 == 1) {
    Script_motmToggleFlags(arg4, SCEN_Enable, 0x80);
  }
  if (arg5 == 2) {
    Script_motmToggleFlags(arg4, SCEN_Disable, 0x40);
  }
  return Script_srchControl(0);
}

static void script_2A78B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_2A7A5, 6, arg0, arg1, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_2A7A5(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, int32_t arg5) {
  int32_t local_FC;
  Vector3i local_F0;
  Vector3 local_E4;
  if (arg5 >= 0) {
    SCNF_WaitFrames_0000(arg5);
  }
  local_FC = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  local_E4.x = arg2;
  (&local_E4)->z = arg3;
  Script_EVT_SetPosition(arg0, 0x28000000, &local_E4);
  local_F0.x = 0;
  (&local_F0)->y = (int32_t)(arg4 / 360.0f * (float)0x10000);
  (&local_F0)->z = 0;
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F0, 0);
  Script_InitializeOsag(arg0);
  return Script_srchControl(0);
}

static void script_2A82B(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_2A83F, 3, arg0, arg1, arg2);
  return;
}

static int32_t script_2A83F(int32_t arg0, float arg1, int32_t arg2) {
  Vector3i local_F4;
  if (arg2 >= 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  local_F4.x = 0;
  (&local_F4)->y = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->z = 0;
  return Script_EVT_SetRotation(arg0, -0x48000000, &local_F4, 0);
}

static void script_2A888(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_2A89E, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_2A89E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_EVT_GetPosition(arg1, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(arg0, arg2, &local_F4, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookAtPosition, &local_F4, 8);
  return Script_srchControl(0);
}

static void script_2A8E1(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_2A8F5, 3, arg0, arg1, arg2);
  return;
}

static int32_t script_2A8F5(int32_t arg0, int32_t arg1, int32_t arg2) {
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  Script_SetCharaLookAtPosition(arg0, -arg1, 0, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  return Script_srchControl(0);
}

static void script_2A927(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_2A93D, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_2A93D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (arg0 == -1) {
    Script_SetGameStateTo(AUTH_None);
  } else if (arg1 == -1) {
    Script_seqEnterEvtCam(arg0, 0, AUTH_None);
  } else {
    Script_seqEnterEvtCam(arg0, arg1, arg2);
  }
  return Script_srchControl(0);
}

static void script_2A989(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_2A99B, 2, arg0, arg1);
  return;
}

static int32_t script_2A99B(const char *arg0, int32_t arg1) {
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_StartOrFreeSEQ(0);
  Script_ShowUIMessageFromID(arg0);
  return Script_srchControl(0);
}

static void script_2A9C2(void) {
  float local_FC;
  float local_F8;
  local_FC = Script_LODUtils(LOD_UTILS_GET_DISTANCE_0, 0);
  local_F8 = Script_LODUtils(LOD_UTILS_GET_DISTANCE_2, 0);
  script_2AA1D();
  script_2AD5B();
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 100.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 100.0f);
  script_2A2E2(0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, local_FC);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, local_F8);
  script_2AC1C();
  return;
}

__attribute__((scn_string_bindings(0, data_42C3C, 1, data_42C41, 2, data_42C46, 3, data_42C4B, 4, data_42C50, 6, data_42C62, 8, data_42C7D, 9, data_42C82, 10, data_42C88)))
static void script_2AA1D(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
  script_E887();
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  task_52C = script_E948(CHARA_GORO);
  *(&task_52C + 1) = script_E948(CHARA_KEBJ);
  *(&task_52C + 2) = script_E948(CHARA_KEBK);
  *(&task_52C + 3) = script_E948(CHARA_NGSM);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_LoadSND(0, 6, "FREE", 0);
  Script_LoadSND(0, 5, "FREE", 0);
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 3, "FREE", 0);
  Script_LoadSND(0, 2, "FREE", 0);
  Script_LoadSND(0, 2, "a1_sokos.snd", 0);
  Script_LoadSND(0, 4, "bgm090.snd", 0);
  SCNF_WaitFrames_0000(1);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 4, 80);
  script_EAA8("/scene/02/MFSY/", "GKYU");
  Script_LoadVoiceFile(1, "E1081");
  Script_LoadVoiceFile(1, "01GAR");
  Script_facemotUtils(CHARA_GORO, 0, 0);
  Script_FaceEx(CHARA_GORO, FACE_ExpressionEnabled, 0, 1);
  Script_FaceLookAtTargetUtils(CHARA_GORO, FACE_LookReset);
  Script_charaSetRefboardFlagTo(CHARA_GORO, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_42C8E, 1, data_42C93, 2, data_42C99)))
static void script_2AC1C(void) {
  Script_executeAicaCommand(2720, 1, 30);
  Script_executeAicaCommand(2720, 2, 30);
  Script_executeAicaCommand(2720, 3, 30);
  Script_filmCheckOrSetTransitionUtil(15, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_E866();
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "GKYU");
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  script_EA10(CHARA_GORO, *&task_52C);
  script_EA10(CHARA_KEBJ, *(&task_52C + 1));
  script_EA10(CHARA_KEBK, *(&task_52C + 2));
  script_EA10(CHARA_NGSM, *(&task_52C + 3));
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 192, 1);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_BOX_RESCUE_ROUTE_COMPLETE, 1);
  Script_LoadVoiceFile(0, "E1081");
  Script_LoadVoiceFile(0, "01GAR");
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_shenRequestGameModuleTransition(2, SHEN_NAME('MS8S'), 1);
  SCNF_WaitFrames_0000(1);
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  Script_ryoControl(RYO_RestoreControl);
  return;
}

static void script_2AD5B(void) {
  Script_motmSetBlendFrames(CHARA_GORO, MOTM_BLEND_FRAMES_8);
  Script_ccowUtils(CHARA_GORO, CCOW_DisableAll);
  script_2A927(8250, -1, -1, -1);
  script_2A927(8251, CHARA_AKIR, CHARA_AKIR, 110);
  script_2A927(8252, -1, -1, 170);
  script_2A927(8253, -1, -1, 200);
  script_2A78B(CHARA_AKIR, -1, -0x3D1ECCCD, 0x42DA999A, 0x43910000, -1);
  script_2A78B(CHARA_GORO, -1, -0x3D2D3333, 0x42D53333, 0x42F00000, -1);
  script_2A78B(CHARA_GORO, -1, -0x3D263333, 0x42D76666, 0x42E60000, 170);
  script_2A6E9(CHARA_AKIR, 89, 0, 0x3F800000, 2, -1);
  script_2A6E9(CHARA_GORO, 30977, 0, 0x3F800000, 2, -1);
  script_2A6E9(CHARA_GORO, 65, 0, 0x3F400000, 2, 170);
  script_2A6E9(CHARA_GORO, 63, 0, 0x3F400000, -1, 183);
  script_2A6E9(CHARA_GORO, 30980, 0, 0x3F800000, -1, 203);
  script_2A6E9(CHARA_GORO, 30979, 0, 0x3F800000, -1, 260);
  script_2A989((int32_t)"E1081B001", -1);
  script_2A989((int32_t)"E1081A001", 100);
  script_2A989((int32_t)"E1081B002", 230);
  script_2A989((int32_t)"E1081A002", 360);
  SCNF_WaitFrames_0000(440);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_42CE0)))
static void script_2AF3D(void) {
  task_547 = 0;
  task_545 = 0;
  task_546 = 0;
  (void)((task_54B = 0) & 1);
  (void)((task_54A = 0) & 1);
  task_548 = 0;
  task_549 = 0;
  (void)((task_54C = -1) & 1);
  Script_LoadVoiceFile(1, "E1008");
  Script_MoveObjectToObject(SHEN_NAME('B180'), 0x67726174);
  Script_MoveObjectToObject(SHEN_NAME('B184'), 0x67726174);
  Script_MoveObjectToObject(SHEN_NAME('B185'), 0x67726174);
  Script_MoveObjectToObject(SHEN_NAME('B120'), 0x67726174);
  Script_MoveObjectToObject(SHEN_NAME('B60_'), 0x67726174);
  Script_MoveObjectToObject(0x30323164, 0x67726174);
  SCNF_WaitFrames_0000(1);
  Script_EVT_SetPosition(0x67726174, POS_XYZ, &data_42CC8);
  Script_EVT_SetRotation(0x67726174, 0x38000000, &data_42CD4, 0);
  SCNF_WaitFrames_0000(1);
  Script_MoveObjectToObject(SHEN_NAME('B180'), -1);
  Script_MoveObjectToObject(SHEN_NAME('B184'), -1);
  Script_MoveObjectToObject(SHEN_NAME('B185'), -1);
  Script_MoveObjectToObject(SHEN_NAME('B120'), -1);
  Script_MoveObjectToObject(SHEN_NAME('B60_'), -1);
  return;
}

static int32_t script_2B050(void) {
  (void)((task_54A = -1) & 1);
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, 5, "e1hakotu.snd", 0x2000);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadSND(4, 0, 0, 0);
  return (task_54A = 0) & 1;
}

__attribute__((scn_string_bindings(0, data_42CF3, 1, data_42CFC, 2, data_42D0A, 3, data_42D13, 4, data_42D1C, 5, data_42D2A)))
static void script_2B0AD(int32_t arg0) {
  Vector3 local_F4;
  script_2AF3D();
  task_544 = Script_ryoSleepUtils(3, 0, 4);
  task_547 = Script_ryoSleepUtils(3, 0, 5);
  task_549 = Script_ryoSleepUtils(3, 0, 6);
  task_548 = Script_ryoSleepUtils(3, 0, 7);
  SCNF_WaitFrames_0000(1);
  Script_MoveObjectToObject(SHEN_NAME('B120'), 0x67726174);
  SCNF_WaitFrames_0000(1);
  Script_EVT_GetPosition(SHEN_NAME('B120'), -1, &local_F4, 0);
  *(int32_t *)&(&local_F4)->z = Script_ryoSleepUtils(3, 2, 0);
  Script_EVT_SetPosition(SHEN_NAME('B120'), POS_XYZ, &local_F4);
  Script_EVT_SetPosition(0x30323164, POS_XYZ, &local_F4);
  Script_MoveObjectToObject(SHEN_NAME('B120'), -1);
  SCNF_WaitFrames_0000(1);
  if (task_549 == -1) {
    if (~Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, "COLIMBUI")) {
      script_EAA8("scene/02/MFSY", "COLIMBUI");
    }
    if (~Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, "SOUKMBUI")) {
      script_EAA8("scene/02/MFSY", "SOUKMBUI");
    }
    if (task_26C.hours >= 19) {
      Script_mapmSetRenderFlag(16, 1);
    }
    script_2CC1B(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_42D33)))
static void script_2B1E0(int32_t arg0) {
  if (task_27A != 5) {
    if (~task_27C) {
      task_544 = 1;
    }
    for (;;) {
      if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, "MBUI")) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    Script_scenCreateTask(script_2B378, 1, arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_42D38, 1, data_42D3D, 2, data_42D4B, 3, data_42D50, 4, data_42D59, 5, data_42D67, 6, data_42D70, 7, data_42D79, 8, data_42D87)))
static void script_2B228(int32_t arg0) {
  int32_t unused_FC;
  if (~task_27C | (task_27C & -(Script_ryoSleepUtils(3, 0, 7) != 2))) {
    task_544 = 2;
    if (~Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, "MBUI")) {
      script_EAA8("scene/02/MFSY", "MBUI");
    }
    if (~Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, "COLIMBUI")) {
      script_EAA8("scene/02/MFSY", "COLIMBUI");
    }
    if (~Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, "SOUKMBUI")) {
      script_EAA8("scene/02/MFSY", "SOUKMBUI");
    }
    Script_EVT_GetPosition(arg0, -1, &task_23C, ENTITY_XFORM_WORLD_SPACE);
    Script_scenCreateTask(script_2B378, 1, arg0);
    if (-1) {
      Script_charaDisplay(SHEN_NAME('B120'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('B120'), CDM_Show);
        goto L02B301;
      }
    }
    L02B301:;
    Script_scenToggleVisibleFor(SHEN_NAME('B120'), 0);
    if (-1) {
      Script_charaDisplay(SHEN_NAME('B60_'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('B60_'), CDM_Show);
        goto L02B335;
      }
    }
    L02B335:;
    Script_scenToggleVisibleFor(SHEN_NAME('B60_'), 0);
    Script_scenToggleCharaFlag(0, 213, 0);
    script_2BCD5(arg0, task_544);
    for (;;) {
      if (task_544 == 8) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    task_548 = 1;
  }
  return;
}

__attribute__((scn_string_bindings(0, data_42D9C, 1, data_42DA5, 2, data_42DAE, 3, data_42DB7)))
static int32_t script_2B378(int32_t arg0) {
  Vector3 local_F4;
  int32_t local_F0;
  float local_EC;
  int8_t local_EB;
  int8_t local_EA;
  int32_t local_E4;
  local_EB = 0;
  local_F0 = Script_scenCreateTask(script_2BB32, 0);
  for (;;) {
    task_545 = 0;
    local_EC = Script_MathUtils(SCRIPT_MATH_TASK_VEC3_DISTANCE, &task_23C, &data_42D90);
    if (-!(local_EC >= 15.0f) & task_54C & ~task_54A) {
      Script_scenCreateTask(script_2B050, 0);
      (void)((task_54C = 0) & 0x80);
    }
    if (!(local_EC <= 20.0f)) {
      (void)((task_54C = -1) & 0x80);
    }
    local_E4 = task_544;
    switch (local_E4) {
      case 1:
        if (~task_27C) {
          script_2AF3D();
          if (task_547 == 0) {
            task_544 = 3;
          } else {
            task_544 = 4;
          }
        }
        break;
      case 2:
        script_2AF3D();
        script_2CC1B(arg0);
        task_544 = 8;
        break;
      case 3:
        if (task_547 == 0) {
          script_2CCB7(arg0);
        } else {
          task_544 = 4;
        }
        break;
      case 4:
        if ((local_EB = script_2C106(arg0)) != 0) {
          Script_ryoControl(PCCTL_DisablePlayerControl);
          Script_shenClearFreeState(0);
          task_544 = 5;
        }
        break;
      case 5:
        if (script_2BD3C(arg0, local_EB)) {
          Script_ryoControl(RYO_RestoreControl);
          Script_shenClearFreeState(1);
          task_544 = 6;
        } else {
          Script_ryoControl(RYO_RestoreControl);
          Script_shenClearFreeState(1);
          task_544 = 4;
        }
        break;
      case 6:
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, "COLIMBUI")) {
          Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "COLIMBUI");
          Script_EV_PAK_AFS_Utils(EVPAK_MODE_PROCESS_COLS, 0);
        }
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, "SOUKMBUI")) {
          Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "SOUKMBUI");
        }
        if (task_26C.hours >= 19) {
          Script_mapmSetRenderFlag(16, 0);
        }
        if (task_549 == -1) {
          script_2CC67(arg0);
        }
        if (task_548 == 1) {
          script_2BAD6();
          Script_ryoSleepUtils(3, 1, 7, 2);
          goto L02BACF;
        }
        if (script_2C43A(arg0, task_544)) {
          Script_ryoControl(PCCTL_DisablePlayerControl);
          Script_shenClearFreeState(0);
          Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          Script_ccowUtils(arg0, CCOW_DisableAll);
          Script_ccowUtils(SHEN_NAME('B120'), CCOW_DisableAll);
          script_2CC1B(arg0);
          script_2C1C2(arg0, 6);
          task_544 = 7;
          Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          script_2BCD5(arg0, task_544);
          SCNF_WaitFrames_0000(1);
          Script_SetGameStateTo(AUTH_None);
          Script_ccowUtils(arg0, CCOW_EnableAll);
          Script_shenClearFreeState(1);
          Script_ryoControl(RYO_RestoreControl);
        }
        break;
      case 7:
        if (script_2C43A(arg0, task_544)) {
          Script_ryoControl(PCCTL_DisablePlayerControl);
          Script_shenClearFreeState(0);
          Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          Script_ccowUtils(arg0, CCOW_DisableAll);
          Script_scenToggleCharaFlag(0, 213, 0);
          script_2C1C2(arg0, 7);
          task_544 = 8;
          script_2BCD5(arg0, task_544);
          Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          Script_convSetCamInactiveFlag(0);
          Script_SetGameStateTo(AUTH_None);
          Script_ccowUtils(arg0, CCOW_EnableAll);
          Script_shenClearFreeState(1);
          Script_ryoControl(RYO_RestoreControl);
        }
        if ((local_EA = script_2CA24(arg0)) != 0) {
          if (Script_IsPlayerAllowed(0) & -(Script_GetControllerInput(1) == 0x400)) {
            script_2C56F(arg0, local_EA, task_544);
            task_544 = 6;
            script_2BCD5(arg0, task_544);
            Script_convSetCamInactiveFlag(1);
            Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
            Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
            Script_SetGameStateTo(AUTH_None);
            SCNF_WaitFrames_0000(1);
            Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
            Script_shenClearFreeState(1);
            Script_ryoControl(RYO_RestoreControl);
          }
        }
        break;
      case 8:
        if (task_548 == 0) {
          local_F4.x = 17.043699f;
          (&local_F4)->y = 3.6f;
          (&local_F4)->z = 152.180099f;
          if (script_2BCB2(arg0, &local_F4, 2.0f, 0xA000)) {
            if (script_2BC06(arg0, 2.0f, &local_F4)) {
              task_545 = 2;
              if (Script_IsPlayerAllowed(0) & -(Script_GetControllerInput(1) == 0x400)) {
                script_2CEA3(arg0);
              }
            }
          }
        }
        if ((local_EA = script_2CA24(arg0)) != 0) {
          if (Script_IsPlayerAllowed(0) & -(Script_GetControllerInput(1) == 0x400)) {
            if (local_EA == 1) {
              script_2C56F(arg0, local_EA, task_544);
              task_544 = 6;
              script_2BCD5(arg0, task_544);
              task_545 = 0;
              Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
              Script_PlayCharaMotion(arg0, 8202, 254.0f, 256.0f, 254.0f, 0.2f, 1);
              script_2CBC3(arg0);
              Script_motmToggleFlags(arg0, SCEN_Disable, 0x80);
              Script_convSetCamInactiveFlag(1);
              Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
              Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
              Script_SetGameStateTo(AUTH_None);
              SCNF_WaitFrames_0000(1);
              Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
              Script_ryoControl(RYO_RestoreControl);
              Script_shenClearFreeState(1);
            } else {
              if (local_EA != 0) {
                script_2C56F(arg0, local_EA, task_544);
                task_544 = 6;
                script_2BCD5(arg0, task_544);
                task_545 = 0;
                Script_PlayCharaMotion(arg0, 8202, 254.0f, 256.0f, 254.0f, 0.2f, 1);
                script_2CBC3(arg0);
                Script_convSetCamInactiveFlag(1);
                Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
                Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
                Script_SetGameStateTo(AUTH_None);
                SCNF_WaitFrames_0000(1);
                Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
                Script_ryoControl(RYO_RestoreControl);
                Script_shenClearFreeState(1);
                goto L02B9C3;
              }
            }
          }
        }
        L02B9C3:;
        break;
      case 0:
        break;
      case 9:
        goto L02BACF;
        break;
    }
    if (task_545) {
      if (task_546 == 0) {
        if (task_545 == 1) {
          SCNF_RequestIconNotification(1, 0, 1, 1);
          Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
          (void)((task_54B = -1) & 1);
        } else {
          SCNF_RequestIconNotification(0x100, 0, 1, 1);
          Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
          (void)((task_54B = -1) & 1);
        }
      }
    } else {
      if (task_546) {
        SCNF_RequestIconNotification(0, 0, 0, 2);
        Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
        (void)((task_54B = 0) & 1);
      }
    }
    task_546 = task_545;
    Script_EVT_GetPosition(0x30323164, -1, &local_F4, 0);
    Script_ryoSleepUtils(3, 3, 0, ((scn3_bits){.f = (&local_F4)->z}).i);
    Script_ryoSleepUtils(3, 1, 4, task_544);
    Script_ryoSleepUtils(3, 1, 5, task_547);
    Script_ryoSleepUtils(3, 1, 6, task_549);
    Script_ryoSleepUtils(3, 1, 7, task_548);
    SCNF_WaitFrames_0000(1);
  }
  L02BACF:;
  return Script_srchControl((void *)local_F0);
}

__attribute__((scn_string_bindings(0, data_42DC0, 1, data_42DC5, 2, data_42DCE, 3, data_42DD7, 4, data_42DDD)))
static void script_2BAD6(void) {
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "MBUI");
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, "COLIMBUI")) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "COLIMBUI");
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PROCESS_COLS, 0);
  Script_LoadVoiceFile(0, "E1008");
  Script_LoadSND(0, 5, "FREE", 0);
  task_544 = 0;
  return;
}

static int32_t script_2BB32(void) {
  for (;;) {
    if (~Script_IsPlayerAllowed(0) & task_54B) {
      task_545 = 0;
      task_546 = 0;
      SCNF_RequestIconNotification(0, 0, 0, 2);
      Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
      (void)((task_54B = 0) & 1);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_2BB7D(int32_t arg0, int32_t arg1, float arg2) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3i local_D0;
  int8_t local_CF;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(arg0, &local_D0, 0x2000000);
  Script_EVT_GetPosition(arg1, -1, &local_E8, 0);
  local_DC.x = (&local_F4)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_D0)->y) * 0.3f;
  (&local_DC)->y = (&local_F4)->y;
  (&local_DC)->z = (&local_F4)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_D0)->y) * 0.3f;
  local_CF = Script_CheckTargetWithinAngleRange(&local_DC, arg2, (&local_D0)->y, 5461, &local_E8);
}

static int32_t script_2BC06(int32_t arg0, float arg1, const Vector3 *arg2) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int8_t local_DB;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(arg0, &local_DC, 0x2000000);
  local_E8.x = (&local_F4)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_DC)->y) * 0.3f;
  (&local_E8)->y = (&local_F4)->y;
  (&local_E8)->z = (&local_F4)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_DC)->y) * 0.3f;
  local_DB = Script_CheckTargetWithinAngleRange(&local_E8, arg1, (&local_DC)->y, 5461, arg2);
}

static int32_t script_2BC83(int32_t arg0, int32_t arg1, float arg2, int32_t arg3) {
  Vector3 local_F4;
  Vector3 local_E8;
  int8_t local_E7;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  Script_EVT_GetPosition(arg1, -1, &local_E8, 0);
  local_E7 = Script_CheckTargetWithinAngleRange(&local_E8, arg2, arg3, 5461, &local_F4);
}

static int32_t script_2BCB2(int32_t arg0, const Vector3 *arg1, float arg2, int32_t arg3) {
  Vector3 local_F4;
  int8_t local_F3;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  local_F3 = Script_CheckTargetWithinAngleRange(arg1, arg2, arg3, 5461, &local_F4);
}

static void script_2BCD5(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  int32_t local_F0;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  local_F0 = arg1;
  switch (local_F0) {
    case 6:
      (&local_F4)->y = 0.0f;
      break;
    case 7:
      (&local_F4)->y = 1.2f;
      break;
    case 8:
      (&local_F4)->y = 3.6f;
      break;
  }
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  return;
}

static int8_t script_2BD3C(int32_t arg0, int8_t arg1) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  float local_D8;
  int8_t local_D7;
  int8_t local_D6;
  int8_t local_D5;
  local_D7 = 0;
  (void)((local_D5 = -1) & 0x40);
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_MoveObjectToObject(arg0, 0x67726174);
  Script_MoveObjectToObject(SHEN_NAME('B120'), 0x67726174);
  script_2CBDD(arg0);
  SCNF_WaitFrames_0000(1);
  Script_EVT_GetPosition(SHEN_NAME('B120'), -1, &local_E8, 0);
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(arg0, &local_DC, 0);
  switch (arg1) {
    case 1:
      Script_seqEnterEvtCam(2000, SHEN_NAME('B120'), SHEN_NAME('B120'));
      (&local_DC)->y = 0;
      if (!((&local_F4)->x < (&local_E8)->x)) {
        local_F4.x = (&local_E8)->x;
      }
      (&local_F4)->z = (&local_E8)->z + 1.3f;
      break;
    case 2:
      Script_seqEnterEvtCam(2005, SHEN_NAME('B120'), SHEN_NAME('B120'));
      (&local_DC)->y = 0x8000;
      local_F4.x = (&local_E8)->x;
      (&local_F4)->z = (&local_E8)->z - 1.3f;
      break;
  }
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(arg0, 0x38000000, &local_DC, 0);
  SCNF_WaitFrames_0000(1);
  Script_PlayCharaMotion(arg0, 8200, 30.0f, 56.0f, 30.0f, 1.0f, 0);
  script_2CBC3(arg0);
  local_D6 = 15;
  Script_ShowUIMessageFromID("E1008A004");
  Script_executeAicaCommand(656553, 0, 0);
  while ((Script_GetControllerInput(0) == 0x1000) | (local_D6 != 0)) {
    if (local_D5 & 0x20) {
      Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
      if (arg1) {
        (&local_F4)->z = (&local_F4)->z - 0.1f;
      } else {
        (&local_F4)->z = (&local_F4)->z + 0.1f;
      }
      Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
      Script_EVT_GetPosition(SHEN_NAME('B120'), -1, &local_E8, 0);
      local_D8 = (&local_F4)->z - (&local_E8)->z;
      Script_ccowUtils(arg0, CCOW_DisableAll);
      Script_PlayCharaMotion(arg0, 8199, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_charaToggleMotEnabled(arg0, 1);
      (void)((local_D5 = 0) & 1);
    }
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    (&local_E8)->z = (&local_F4)->z - local_D8;
    switch (arg1) {
      case 2:
        if (!((&local_E8)->z < -58.852798f)) {
          (&local_E8)->z = -58.852798f;
          (&local_F4)->z = local_D8 + -58.852798f;
          Script_EVT_SetPosition(SHEN_NAME('B120'), POS_XYZ, &local_E8);
          goto L02C038;
        }
        break;
      case 1:
        if (!((&local_E8)->z >= -69.0f)) {
          (&local_E8)->z = -68.947998f;
          (&local_F4)->z = local_D8 + -69.0f;
          Script_EVT_SetPosition(SHEN_NAME('B120'), POS_XYZ, &local_E8);
          local_D7 = 1;
          goto L02C038;
        }
        break;
    }
    Script_EVT_SetPosition(SHEN_NAME('B120'), POS_XYZ, &local_E8);
    if (local_D6) {
      local_D6 = local_D6 - 1;
    }
    SCNF_WaitFrames_0000(1);
  }
  L02C038:;
  Script_executeAicaCommand(722089, 0, 0);
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_ShowUIMessageFromID(data_42DEC);
  Script_SetGameStateTo(AUTH_None);
  Script_PlayCharaMotion(arg0, 8198, 60.0f, 108.0f, 60.0f, 1.0f, 0);
  script_2CBC3(arg0);
  if (local_D5 & 0x20) {
    local_D8 = (&local_F4)->z;
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    (&local_F4)->z = local_D8;
    Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  }
  Script_EVT_GetPosition(SHEN_NAME('B120'), -1, &local_F4, 0);
  Script_EVT_SetPosition(0x30323164, POS_XYZ, &local_F4);
  Script_MoveObjectToObject(arg0, -1);
  Script_MoveObjectToObject(SHEN_NAME('B120'), -1);
  SCNF_WaitFrames_0000(1);
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_convSetCamInactiveFlag(1);
  return local_D7;
}

static int8_t script_2C106(int32_t arg0) {
  int8_t local_FF;
  local_FF = 0;
  if (script_2BC83(arg0, SHEN_NAME('B120'), 1.6f, 0xE000)) {
    if (script_2BB7D(arg0, SHEN_NAME('B120'), 1.6f)) {
      task_545 = 1;
      if (Script_IsPlayerAllowed(0) & -(Script_GetControllerInput(0) == 0x1000)) {
        local_FF = 1;
      }
      goto L02C167;
    }
  }
  L02C167:;
  if (script_2BC83(arg0, SHEN_NAME('B120'), 1.6f, 0x6000)) {
    if (script_2BB7D(arg0, SHEN_NAME('B120'), 1.6f)) {
      task_545 = 1;
      if (Script_IsPlayerAllowed(0) & -(Script_GetControllerInput(0) == 0x1000)) {
        local_FF = 2;
      }
      goto L02C1BE;
    }
  }
  L02C1BE:;
  return local_FF;
}

__attribute__((scn_string_bindings(0, data_42E04, 1, data_42E0D, 2, data_42E1B, 3, data_42E24, 4, data_42E2D, 5, data_42E3B)))
static void script_2C1C2(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  script_2CBDD(arg0);
  {
    Vector3 local_EC;
    int32_t local_E8;
    int32_t local_E4;
    local_E4 = arg1;
    switch (local_E4) {
      case 6:
        Script_seqEnterEvtCam(2010, SHEN_NAME('B120'), SHEN_NAME('B120'));
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, "COLIMBUI") == 0) {
          script_EAA8("scene/02/MFSY", "COLIMBUI");
        }
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, "SOUKMBUI") == 0) {
          script_EAA8("scene/02/MFSY", "SOUKMBUI");
          if (task_26C.hours >= 19) {
            Script_mapmSetRenderFlag(16, 1);
          }
        }
        if (script_2BC83(arg0, SHEN_NAME('B120'), 2.0f, 0xE000)) {
          local_EC.x = 22.2946f;
          (&local_EC)->y = 0.0f;
          (&local_EC)->z = 149.109406f;
          {
            local_E8 = 0x6000;
          }
        } else {
          local_EC.x = 22.2166f;
          (&local_EC)->y = 0.0f;
          (&local_EC)->z = 150.6f;
          {
            local_E8 = 0x2000;
          }
        }
        break;
      case 7:
        Script_seqEnterEvtCam(2020, SHEN_NAME('B120'), SHEN_NAME('B120'));
        local_EC.x = 21.6166f;
        (&local_EC)->y = 1.3f;
        (&local_EC)->z = 149.808594f;
        {
          local_E8 = 0x6000;
        }
        break;
    }
    Script_scenCharaSubmitMoveTargetRequest(arg0, &local_EC, local_E8, MN_AKI_AKI_STAND_LP, 0);
  }
  while (Script_IsMoveTargetRequestActive(arg0) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  {
    int32_t local_F4;
    local_F4 = arg1;
    if ((local_F4 == 6) | (local_F4 == 8)) {
      local_FC = 0x41700000;
      local_F8 = 0x41B00000;
      Script_PlayCharaMotion(arg0, MN_AKI_AKI_NORU_BOX_120, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    } else if (local_F4 == 7) {
      local_FC = 0x42840000;
      local_F8 = 0x42700000;
      Script_PlayCharaMotion(arg0, 8196, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    } else {
      local_FC = 0x42640000;
      local_F8 = 0x42500000;
      Script_PlayCharaMotion(arg0, 8195, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    }
  }
  Script_charaToggleMotEnabled(arg0, 1);
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    if (((scn3_bits){.f = Script_GetCharaMotionRemainingFramesScaled(arg0)}).i == local_F8) {
      Script_ShowUIMessageFromID("E1008A006");
    } else {
      if (((scn3_bits){.f = Script_GetCharaMotionRemainingFramesScaled(arg0)}).i == local_FC) {
        Script_executeAicaCommand(1193, 0, 0);
        goto L02C42E;
      }
    }
    L02C42E:;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int8_t script_2C43A(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int8_t local_FB;
  int32_t local_F4;
  local_FB = 0;
  local_F4 = arg1;
  switch (local_F4) {
    case 6:
      local_FC = 0x30323142;
      if (script_2BC83(arg0, local_FC, 2.0f, 0xE000)) {
        if (script_2BB7D(arg0, local_FC, 2.0f)) {
          task_545 = 2;
          if (Script_IsPlayerAllowed(0) & -(Script_GetControllerInput(1) == 0x400)) {
            local_FB = 1;
          }
          goto L02C4AD;
        }
      }
      L02C4AD:;
      if (script_2BC83(arg0, local_FC, 2.0f, 0xA000)) {
        if (script_2BB7D(arg0, local_FC, 2.0f)) {
          task_545 = 2;
          if (Script_IsPlayerAllowed(0) & -(Script_GetControllerInput(1) == 0x400)) {
            local_FB = 1;
          }
          goto L02C500;
        }
      }
      L02C500:;
      break;
    case 7:
      local_FC = 0x34383142;
      if (script_2BC83(arg0, local_FC, 2.5f, 0xE000)) {
        if (script_2BB7D(arg0, local_FC, 2.5f)) {
          task_545 = 2;
          if (Script_IsPlayerAllowed(0) & -(Script_GetControllerInput(1) == 0x400)) {
            local_FB = 1;
          }
          goto L02C568;
        }
      }
      L02C568:;
      break;
  }
  return local_FB;
}

static void script_2C56F(int32_t arg0, int8_t arg1, int8_t arg2) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  float local_D8;
  float local_D4;
  float local_D0;
  int32_t local_CC;
  local_D4 = 0.2f;
  local_DC.x = 0;
  (&local_DC)->y = 0;
  (&local_DC)->z = 0;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_MoveObjectToObject(arg0, 0x67726174);
  Script_MoveObjectToObject(SHEN_NAME('B180'), 0x67726174);
  Script_MoveObjectToObject(SHEN_NAME('B184'), 0x67726174);
  Script_MoveObjectToObject(SHEN_NAME('B120'), 0x67726174);
  SCNF_WaitFrames_0000(1);
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(arg0, &local_DC, 0x42000000);
  local_D8 = (float)((&local_DC)->y * 360 / 0x10000);
  local_CC = arg2;
  switch (local_CC) {
    case 7:
      Script_EVT_GetPosition(SHEN_NAME('B120'), -1, &local_E8, 0);
      local_D0 = 0.4f;
      break;
    case 8:
      if (arg1 == 1) {
        Script_EVT_GetPosition(SHEN_NAME('B180'), -1, &local_E8, 0);
      } else {
        Script_EVT_GetPosition(SHEN_NAME('B184'), -1, &local_E8, 0);
      }
      local_D0 = 0.55f;
      break;
  }
  switch (arg1) {
    case 1:
      (&local_F4)->z = (&local_E8)->z - local_D0 + local_D4;
      (&local_DC)->y = 0;
      Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
      Script_EVT_SetRotation(arg0, 0x38000000, &local_DC, 0);
      break;
    case 3:
      if (arg2 == 7) {
        local_F4.x = (&local_E8)->x;
      }
      (&local_F4)->z = (&local_E8)->z + local_D0 - local_D4;
      (&local_DC)->y = 0x8000;
      Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
      Script_EVT_SetRotation(arg0, 0x38000000, &local_DC, 0);
      break;
    case 4:
      local_F4.x = (&local_E8)->x - local_D0 + local_D4;
      if (arg2 == 7) {
        (&local_F4)->z = (&local_E8)->z;
      }
      (&local_DC)->y = 0x4000;
      Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
      Script_EVT_SetRotation(arg0, 0x38000000, &local_DC, 0);
      break;
  }
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  if (arg2 == 7) {
    if (arg1 == 3) {
      Script_seqEnterEvtCam(2040, arg0, arg0);
    } else {
      Script_seqEnterEvtCam(2045, arg0, arg0);
    }
    script_2C839(arg0, arg1);
  } else {
    if (arg1 == 1) {
      Script_seqEnterEvtCam(2051, arg0, arg0);
    } else {
      Script_seqEnterEvtCam(2050, arg0, arg0);
    }
    script_2C946(arg0, arg1);
  }
  Script_ccowUtils(SHEN_NAME('B120'), CCOW_EnableAll);
  Script_MoveObjectToObject(arg0, -1);
  Script_MoveObjectToObject(SHEN_NAME('B180'), -1);
  Script_MoveObjectToObject(SHEN_NAME('B184'), -1);
  Script_MoveObjectToObject(SHEN_NAME('B120'), -1);
  SCNF_WaitFrames_0000(1);
  return;
}

__attribute__((scn_string_bindings(0, data_42E4E)))
static void script_2C839(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3 local_E8;
  Script_ccowUtils(arg0, CCOW_DisableAll);
  if (arg1 == 3) {
    Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
  }
  Script_charaToggleMotEnabled(arg0, 0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_ORIRU_BOX_120, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  local_F4.x = 1.5f;
  (&local_F4)->y = 1.0f;
  (&local_F4)->z = 1.4f;
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    Script_RotateNodeUtils(arg0, &local_F4, 0);
    if (((scn3_bits){.f = Script_GetCharaMotionRemainingFramesScaled(arg0)}).i == 0x41C80000) {
      Script_ShowUIMessageFromID("E1008A009");
    }
    if (((scn3_bits){.f = Script_GetCharaMotionRemainingFramesScaled(arg0)}).i == 0x424C0000) {
      Script_executeAicaCommand(918697, 0, 0);
    }
    Script_EVT_GetPosition(arg0, -1, &local_E8, 0);
    if (!((&local_E8)->x >= -68.8f)) {
      local_E8.x = -68.8f;
      Script_EVT_SetPosition(arg0, POS_XYZ, &local_E8);
    }
    SCNF_WaitFrames_0000(1);
  }
  if (arg1 == 1) {
    Script_motmToggleFlags(arg0, SCEN_Disable, 0x80);
  }
  Script_ccowUtils(arg0, CCOW_EnableAll);
  return;
}

__attribute__((scn_string_bindings(0, data_42E58)))
static void script_2C946(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Script_ccowUtils(arg0, CCOW_DisableAll);
  if (arg1 == 1) {
    Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
  }
  Script_charaToggleMotEnabled(arg0, 0);
  Script_PlayCharaMotion(arg0, 8201, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    Script_RotateNodeUtils(arg0, &data_39DC0, 0);
    if (((scn3_bits){.f = Script_GetCharaMotionRemainingFramesScaled(arg0)}).i == 0x423C0000) {
      Script_ShowUIMessageFromID("E1008A009");
    }
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    if (!((&local_F4)->x >= -68.8f)) {
      local_F4.x = -68.8f;
      Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
    }
    SCNF_WaitFrames_0000(1);
  }
  if (arg1 == 1) {
    Script_motmToggleFlags(arg0, SCEN_Disable, 0x80);
  }
  Script_executeAicaCommand(918697, 0, 0);
  Script_ccowUtils(arg0, CCOW_EnableAll);
  return;
}

static int8_t script_2CA24(int32_t arg0) {
  Vector3i local_F4;
  float local_F0;
  int8_t local_EF;
  int32_t local_E8;
  local_EF = 0;
  Script_mtwkGetSetRotation(arg0, &local_F4, 0x42000000);
  local_F0 = (float)((&local_F4)->y * 360 / 0x10000);
  local_E8 = task_544;
  switch (local_E8) {
    case 7:
      if (-!((float)((scn3_bits){.f = script_EC08(arg0, 21.1448f, 149.069107f, 22.2054f, 150.1297f)}).i >= 0.0f) & -!(local_F0 <= 275.0f) & -!(local_F0 >= 355.0f)) {
        local_EF = 3;
      } else {
        if (-!((float)((scn3_bits){.f = script_EC08(arg0, 22.2054f, 149.422699f, 21.2155f, 150.412598f)}).i >= 0.0f) & -!(local_F0 <= 185.0f) & -!(local_F0 >= 265.0f)) {
          local_EF = 4;
          goto L02CAE0;
        }
      }
      L02CAE0:;
      break;
    case 8:
      if (-!((float)((scn3_bits){.f = script_EC08(arg0, 20.1534f, 150.060501f, 21.709f, 151.616104f)}).i >= 0.0f) & -!(local_F0 <= 265.0f) & -!(local_F0 >= 355.0f)) {
        local_EF = 3;
      } else if (-!((float)((scn3_bits){.f = script_EC08(arg0, 21.6383f, 150.626099f, 17.5371f, 154.727295f)}).i >= 0.0f) & -!(local_F0 <= 175.0f) & -!(local_F0 >= 275.0f)) {
        local_EF = 4;
      } else {
        if (-!((float)((scn3_bits){.f = script_EC08(arg0, 18.6684f, 154.656601f, 17.112801f, 153.100998f)}).i >= 0.0f) & -!(local_F0 <= 85.0f) & -!(local_F0 >= 195.0f)) {
          local_EF = 1;
        }
      }
      break;
  }
  if (local_EF != 0) {
    task_545 = 2;
  }
  return local_EF;
}

static void script_2CBC3(int32_t arg0) {
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2CBDD(int32_t arg0) {
  Vector3i local_F4;
  Script_mtwkGetSetRotation(arg0, &local_F4, 0x2000000);
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static void script_2CC1B(int32_t arg0) {
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND);
  Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, 4.1f);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_scenToggleActiveFor(arg0, 0);
  Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 19, 0);
  Script_mapmSetCamFlags(2, 5, 0);
  task_549 = -1;
  return;
}

static void script_2CC67(int32_t arg0) {
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, 3.1f);
  Script_ccowSetHeightMode(arg0, CCOW_FollowGround);
  Script_scenToggleActiveFor(arg0, 1);
  Script_scenToggleCharaFlag(0, 213, 1);
  Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 19, 1);
  Script_mapmSetCamFlags(2, 5, 1);
  task_549 = 0;
  return;
}

__attribute__((scn_string_bindings(0, data_42E88, 1, data_42E96, 4, data_42EB3)))
static void script_2CCB7(int32_t arg0) {
  Vector3 local_F4;
  if (Script_IsPlayerAllowed(0) & -(Script_CheckTargetWithinAngleRange(&data_42E64, 2.0f, 0, 0x8000, &task_23C) == 1)) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
    Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
    Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
    Script_shenClearFreeState(0);
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
    Script_ccowUtils(arg0, CCOW_DisableAll);
    Script_ryoControl(PCCTL_RequestNewMode, 5);
    while (~Script_ryoControl(PCCTL_QueryRequestIdle)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_convSetCamInactiveFlag(1);
    Script_seqEnterEvtCam(2105, 0, AUTH_None);
    Script_scenCharaSubmitMoveTargetRequest(arg0, &data_42E70, 12743, MN_AKI_AKI_STAND_LP, 0);
    script_EAA8("scene/02/MFSY", "SOUKMBUI");
    while (Script_IsMoveTargetRequestActive(arg0) != 0) {
      SCNF_WaitFrames_0000(1);
    }
    local_F4.x = 17.62f;
    (&local_F4)->y = 5.21f;
    (&local_F4)->z = 152.65f;
    Script_SetCharaLookAtPosition(arg0, 98308, &local_F4, 2);
    SCNF_WaitFrames_0000(10);
    Script_ShowUIMessageFromID("E1008A001");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_seqEnterEvtCam(2110, 0, AUTH_None);
    Script_ShowUIMessageFromID("E1008A002");
    Script_SetCharaLookAtPosition(arg0, -13, 0, 0);
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    script_2CBDD(arg0);
    SCNF_WaitFrames_0000(1);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "SOUKMBUI");
    task_547 = -1;
    Script_ccowUtils(arg0, CCOW_EnableAll);
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
    Script_SetGameStateTo(AUTH_None);
    Script_ryoControl(RYO_RestoreControl);
    Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
    Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
    Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
    Script_shenClearFreeState(1);
  }
  return;
}

static void script_2CEA3(int32_t arg0) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_seqEnterEvtCam(2030, 0, AUTH_None);
  Script_scenCharaSubmitMoveTargetRequest(CHARA_AKIR, &data_42EBC, 0x2000, MN_AKI_AKI_STAND_LP, 0);
  while (Script_IsMoveTargetRequestActive(CHARA_AKIR) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(arg0, 8194, 0.0f, 96.0f, 0.0f, 1.0f, 0);
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    if (((scn3_bits){.f = Script_GetCharaMotionRemainingFramesScaled(arg0)}).i == 0x42500000) {
      Script_seqEnterEvtCam(2035, 0, AUTH_None);
    } else {
      if (((scn3_bits){.f = Script_GetCharaMotionRemainingFramesScaled(arg0)}).i == 0x42AA0000) {
        Script_executeAicaCommand(2720, 1, 25);
        Script_executeAicaCommand(2720, 2, 25);
        Script_filmCheckOrSetTransitionUtil(20, 0, 0, 0, 0, 255, 0, 0, 0);
        goto L02CFCD;
      }
    }
    L02CFCD:;
    SCNF_WaitFrames_0000(1);
  }
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_BOX_RESCUE_ROUTE_COMPLETE, 1);
  script_EBF3(2, SHEN_NAME('MS8S'), 0);
  return;
}

void script_2CFF4(void) {
  Script_ryoSleepUtils(2, 1);
  if (Script_ryoSleepUtils(0) == -1) {
    (void)((task_27B = -1) & 0x40);
    if (Script_GetCurrentEntryPoint() == 5) {
      if (Script_ryoSleepUtils(3, 0, 7) != 2) {
        task_27A = 5;
      } else {
        task_27A = 0;
      }
    } else {
      task_27A = 0;
    }
  } else {
    (void)((task_27B = 0) & 0x40);
    task_27A = Script_GetCurrentEntryPoint();
  }
  (void)((task_27C = task_27B) & 0x40);
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_scenCreateTask(script_2D880, 1, CHARA_AKIR);
  script_2D09D();
  return;
}

__attribute__((scn_string_bindings(0, data_42ED4, 1, data_42EE2, 3, data_42EF2, 5, data_42F0D)))
static void script_2D09D(void) {
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_COFFEE_ROUTE_ENABLED, 1);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 880, 1);
  script_25C34();
  task_260 = 0;
  task_264 = 0;
  (void)((task_28A = -1) & 0x40);
  (void)((task_27D = -1) & 0x40);
  (void)((task_27E = 0) & 0x40);
  (void)((task_27F = -1) & 0x40);
  (void)((task_282 = 0) & 0x40);
  task_4AC = 0;
  (void)((task_284 = -1) & 0x40);
  (void)((task_285 = 0) & 0x40);
  (void)((task_286 = -1) & 0x40);
  task_277 = -1;
  task_278 = -1;
  task_258 = -1;
  (void)((task_288 = 0) & 0x40);
  (void)((task_289 = 0) & 0x40);
  if (task_27B & 0x80) {
    (void)((task_289 = -(Script_ryoSleepUtils(3, 0, 9) != 0)) & 4);
  }
  task_524 = 0;
  Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 100);
  Script_SetMapFlags(0, 1);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 180.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 100.0f);
  Script_LODUtils(LOD_UTILS_SET_NPC_DISPLAY_RADIUS, 48.0f);
  Script_shenSetLightingMode(1);
  Script_LightUtils(0, 0, 2);
  Script_lightSetSlotEnabled(0, 1);
  Script_SetCameraSettings(5, 6, 1, 0);
  Script_ReturnTimeDate(&task_26C);
  script_112DD();
  script_10EE9();
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS8S_MAIN_EVENT_COMPLETE) == 0) {
    task_276 = 1;
  } else {
    task_276 = 0;
  }
  script_2E033();
  task_254 = script_E708();
  Script_scenCreateTask(script_E7D9, 1, task_254);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  Script_scenToggleCharaFlag(0, 202, 0);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PARTTIME_JOB_WAKE_ROUTE_ACTIVE) == 0) {
    Script_charaDisplay(SHEN_NAME('GHAK'), CDM_Show);
    Script_hmdlApplyScale(SHEN_NAME('GHAK'), &data_42EC8);
    Script_scenToggleCharaFlag(0, 202, 1);
  }
  Script_scenToggleVisibleFor(SHEN_NAME('JIH0'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('JIH1'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('SHT1'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('SHT2'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('SHT3'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('SHT4'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('OTO1'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('OTO8'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('8MAD'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('FOD_'), 0);
  Script_scenToggleVisibleFor(0x31786264, 0);
  Script_scenToggleVisibleFor(0x32786264, 0);
  Script_scenToggleVisibleFor(SHEN_NAME('TOMD'), 0);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 887) == 0) {
    Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 14, 0);
  }
  if (-(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 798) == 1) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_AFTER_8PM_GUARD_SHIFT) == 0) & script_A7E4(15, 1)) {
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_AFTER_8PM_GUARD_SHIFT, 1);
  }
  if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_BOX_RESCUE_ROUTE_UNLOCKED) == 1) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_BOX_RESCUE_ROUTE_COMPLETE) == 0)) {
    script_EAA8("scene/02/MFSY", "MBUI");
    if (task_27C & 1) {
      script_2B0AD(CHARA_AKIR);
    }
  }
  script_109B5();
  if (task_27A != 4) {
    script_2F8A1();
  }
  script_D58A();
  script_DDFC();
  if (task_27A != 33) {
    script_D9DE();
  }
  script_136E4(0x4C455442, 9997, -0x3D125FB1, 0x3F8CCCCD, 0x42F38F5C, 0x42B40000);
  script_E087();
  Script_tmnmUtils(SHEN_NAME('FOOD'), 1535, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
  task_4BC = task_27A;
  Script_motiLoad(2, SHEN_NAME('FREE'), "M_MOBJ.BIN");
  Script_CycleManagerUtils(CYCLE_CMD_SETUP, "scene/02/MFSY", (int32_t)"CYCLEMAN.BIN", 0x5953464D);
  Script_scenCreateTask(script_2E00E, 0);
  if (task_27B & 0x80) {
    Script_ryoSleepUtils(1);
  }
  Script_EVT_GetPosition(CHARA_AKIR, -1, &task_23C, ENTITY_XFORM_WORLD_SPACE);
  if (task_26C.hours >= 19) {
    if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS8S_MAIN_EVENT_COMPLETE) == 1) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS08_SCENE_EXIT_COMPLETE) == 0)) {
      task_276 = 2;
    } else {
      task_276 = 1;
    }
    (void)((task_283 = 0) & 8);
    script_2F1B4(0);
    script_2F0FA();
  } else {
    (void)((task_283 = -1) & 8);
  }
  script_2F793();
  task_279 = 0;
  if (task_27B & 0x80) {
    task_279 = Script_ryoSleepUtils(3, 0, 8);
    if (-(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_COFFEE_EVENT_STARTED) == 1) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS08_SCENE_EXIT_COMPLETE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_COFFEE_EVENT_COMPLETE) == 0)) {
      task_268 = script_E948(CHARA_SYZU);
      Script_scenUtils(SCENARIO_CMD_SET_CHARA_RADIUS_SQUARED, 0x555A5953, 0.0f);
      script_27DEC("HAKO");
      script_27E10();
      (void)((task_288 = -1) & 8);
    }
    if (task_279 != 0) {
      int32_t local_FC;
      int32_t local_F8;
      int32_t local_F4;
      int32_t local_F0;
      (void)((task_284 = 0) & 8);
      (void)((task_283 = 0) & 8);
      local_F0 = task_279;
      switch (local_F0) {
        case 80:
          local_FC = (int32_t)data_42F12;
          {
            local_F8 = 3;
          }
          {
            local_F4 = 8;
          }
          break;
        case 81:
          local_FC = (int32_t)data_42F1A;
          {
            local_F8 = 4;
          }
          {
            local_F4 = 9;
          }
          break;
        case 82:
          local_FC = (int32_t)data_42F22;
          {
            local_F8 = 5;
          }
          {
            local_F4 = 10;
          }
          break;
        case 86:
          local_FC = (int32_t)data_42F2A;
          {
            local_F8 = 6;
          }
          {
            local_F4 = 4;
          }
          break;
        case 87:
          local_FC = (int32_t)data_42F32;
          {
            local_F8 = 7;
          }
          {
            local_F4 = 5;
          }
          break;
        case 88:
          local_FC = (int32_t)data_42F3A;
          {
            local_F8 = 8;
          }
          {
            local_F4 = 6;
          }
          break;
        case 98:
          local_FC = (int32_t)data_42F42;
          {
            local_F8 = 9;
          }
          {
            local_F4 = 7;
          }
          break;
        case 95:
          local_FC = (int32_t)data_42F4A;
          {
            local_F8 = 11;
          }
          {
            local_F4 = 1;
          }
          break;
        case 96:
          local_FC = (int32_t)data_42F50;
          {
            local_F8 = 10;
          }
          {
            local_F4 = 3;
          }
          break;
      }
      script_83B8(local_F4);
      Script_mapmSetRenderFlag(12, 0);
      Script_mapmSetRenderFlag(14, 0);
      script_2E454(local_FC, local_F8, task_279);
    }
  } else {
    task_279 = 0;
    Script_ryoSleepUtils(3, 1, 8, task_279);
  }
  (void)((task_27B = 0) & 8);
  {
    int32_t local_FC;
    local_FC = task_27A;
    if (local_FC == 4) {
      script_1131C();
      script_2F8A1();
      (void)((task_27D = 0) & 8);
      Script_scenCreateTask(script_EA82, 0);
    } else if (local_FC == 5) {
      script_2B228(CHARA_AKIR);
      (void)((task_27D = 0) & 8);
      Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
      Script_scenCreateTask(script_EA82, 0);
    } else if ((local_FC == 41) | (local_FC == 51) | (local_FC == 52) | (local_FC == 53) | (local_FC == 54)) {
      script_24703();
      (void)((task_27D = 0) & 8);
      Script_scenCreateTask(script_EA82, 0);
    } else {
      Script_CycleManagerUtils(CYCLE_CMD_RESET_LOAD_SETTLE);
      for (;;) {
        if (Script_CycleManagerUtils(CYCLE_CMD_TICK_LOAD_SETTLE_COUNT) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      (void)((task_27D = 0) & 8);
      if (task_279 == 0) {
        Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
      }
      Script_scenCreateTask(script_EA82, 0);
    }
  }
  script_1058B();
  script_112DA();
  script_B2F5();
  Script_PreserveAccumulator_0182(0, 0, 0);
  return;
}

__attribute__((scn_string_bindings(1, data_42FCB, 2, data_42FD0, 3, data_42FD5, 4, data_42FDA, 5, data_42FDF)))
static int32_t script_2D880(int32_t arg0) {
  int32_t unused_F8[2];
  int32_t local_F4;
  int32_t unused_F0;
  int32_t local_EC;
  int32_t local_E8;
  int32_t local_E4;
  int8_t local_E3;
  int8_t local_E2;
  int8_t local_E1;
  int8_t local_E0;
  int8_t local_DF;
  int8_t local_DE;
  uint8_t unused_DC[2];
  int32_t local_D8;
  int8_t local_D7;
  uint8_t unused_D4[3];
  int32_t local_D0;
  (void)((local_E2 = -1) & 1);
  (void)((local_E1 = -1) & 1);
  (void)((local_E0 = -1) & 1);
  (void)((local_DF = -1) & 1);
  (void)((local_DE = 0) & 1);
  script_9FF6(arg0);
  while (task_27D & 2) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(1);
  Script_ryoControl(RYO_RestoreControl);
  Script_scenCreateTask(script_F244, 0);
  script_ECF5();
  script_ED5C();
  for (;;) {
    Script_EVT_GetPosition(arg0, -1, &task_23C, ENTITY_XFORM_WORLD_SPACE);
    Script_mtwkGetSetRotation(arg0, &task_248, 0x42000000);
    local_E8 = Script_UpdateTaskCollisionCache(0, 1, &task_23C);
    if (local_E8 != -1) {
      Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, local_E8);
    }
    local_E4 = script_B347();
    if (local_E4 != -1) {
      if ((script_B32C() == 0x4242454B) & (local_E4 == 130)) {
        Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
        Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
        Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
        Script_shenClearFreeState(0);
        Script_executeAicaCommand(2720, 1, 30);
        Script_executeAicaCommand(2720, 2, 30);
        Script_executeAicaCommand(2720, 3, 30);
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        script_EBF3(2, SHEN_NAME('MKSG'), 0);
      } else {
        script_B31F();
      }
    }
    Script_ReturnTimeDate(&task_26C);
    task_275 = Script_WeatherUtils_0(WeatherDataUtil_GetWeatherClassForDateTime, (&task_26C)->year, task_26C.month, task_26C.day, task_26C.hours, task_26C.minutes);
    if ((local_DF == -1) & (task_26C.hours >= 19)) {
      Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 25, 0);
      (void)((local_DF = 0) & 4);
      Script_GetConversationDetails(CONV_DETAILS_MODE_STOP_CASSETTES, 0);
    }
    if (Script_CycleManagerUtils(CYCLE_CMD_GET_MOBJ_PRIMARY_DOOR_OPEN_PERCENT, 0x5F535542) | Script_CycleManagerUtils(CYCLE_CMD_GET_MOBJ_FIELD_148_OR_13C_PERCENT, 0x5F535542)) {
      if (local_DE == 0) {
        Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 27, 1);
        (void)((local_DE = -1) & 4);
      }
    } else {
      if (local_DE == -1) {
        Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 27, 0);
        (void)((local_DE = 0) & 4);
      }
    }
    if (((&task_7C)->mode != 0) & (task_7C.id == 0x52494B41)) {
      (void)((local_D7 = -1) & 1);
      local_E3 = 0;
      local_D0 = task_7C.entry_idx;
      if (local_D0 == 8) {
        task_25C = (char *)data_42F64;
        local_D8 = 3;
        task_279 = 80;
      } else if (local_D0 == 9) {
        task_25C = (char *)data_42F6C;
        local_D8 = 4;
        task_279 = 81;
      } else if (local_D0 == 10) {
        task_25C = (char *)data_42F74;
        local_D8 = 5;
        task_279 = 82;
      } else if (local_D0 == 4) {
        task_25C = (char *)data_42F7C;
        local_D8 = 6;
        task_279 = 86;
      } else if (local_D0 == 5) {
        task_25C = (char *)data_42F84;
        local_D8 = 7;
        task_279 = 87;
      } else if (local_D0 == 6) {
        task_25C = (char *)data_42F8C;
        local_D8 = 8;
        task_279 = 88;
      } else if (local_D0 == 7) {
        task_25C = (char *)data_42F94;
        local_D8 = 9;
        task_279 = 98;
      } else if (local_D0 == 3) {
        task_25C = (char *)data_42F9C;
        local_D8 = 10;
        task_279 = 96;
      } else if (local_D0 == 0) {
        task_25C = (char *)data_42FA4;
        (void)((local_D7 = 0) & 4);
        task_279 = 97;
        local_EC = 0x55594B4D;
      } else if (local_D0 == 1) {
        task_25C = (char *)data_42FAC;
        local_D8 = 11;
        task_279 = 95;
      } else {
        Script_sprintf("Illegal DOOR!! == %d ==\012", task_7C.entry_idx);
        (void)((local_D7 = 0) & 4);
        task_279 = 0;
        local_E3 = -1;
      }
      if (local_D7 & 0x80) {
        if (task_284 & 1) {
          Script_ryoSleepUtils(3, 1, 8, task_279);
          (void)((task_284 = 0) & 4);
          (void)((task_283 = 0) & 4);
          script_2E454((int32_t)task_25C, local_D8, task_279);
          Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, task_279);
        } else {
          Script_ryoSleepUtils(3, 1, 8, 0);
          (void)((task_284 = -1) & 4);
          (void)((task_283 = -1) & 4);
          script_2E93C(task_25C, task_279);
          task_279 = 0;
        }
      } else {
        if (local_E3 != -1) {
          if (task_279 > 0) {
            Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, task_279);
          }
          Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
          SCNF_WaitFrames_0000(105);
          Script_shenClearFreeState(0);
          Script_executeAicaCommand(2720, 1, 30);
          Script_executeAicaCommand(2720, 2, 30);
          Script_executeAicaCommand(2720, 3, 30);
          Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
          while (Script_filmCheckOrSetTransitionUtil(-1)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_LoadSND(0, 5, "FREE", 0);
          Script_LoadSND(0, 4, "FREE", 0);
          Script_LoadSND(0, 3, "FREE", 0);
          Script_LoadSND(0, 2, "FREE", 0);
          Script_LoadSND(0, 1, "FREE", 0);
          script_EBF3(2, local_EC, local_E3);
        }
      }
    }
    if (local_E2 & 4) {
      Script_GetDateTimeUpdateHistory((uint8_t *)&local_F4);
      if (*((int8_t *)&local_F4 + 4) & -(task_279 == 0)) {
        if ((task_26C.hours == 12) | (task_26C.hours == 17)) {
          Script_executeAicaCommand(0x5105A9, 0, 0);
        }
      }
      if (-(task_26C.hours >= 19) & Script_IsPlayerAllowed(0) & -(task_3A1 == 0)) {
        if (task_284 & 1) {
          script_82BB();
          if (task_26C.hours < 22) {
            script_83B8(0);
          }
          if (task_26C.hours < 20) {
            script_83B8(3);
          }
          if (task_283 & 0x80) {
            script_2F1B4(-1);
          }
        }
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS8S_MAIN_EVENT_COMPLETE) == 1) {
          task_276 = 2;
        }
        if (task_277 == 3) {
          script_100B0();
        }
        script_2F0FA();
        (void)((local_E2 = 0) & 0x10);
      }
    }
    if (local_E1 & -(task_26C.hours >= 20)) {
      script_8381(3);
      (void)((local_E1 = 0) & 0x10);
    }
    if (local_E0 & -(task_26C.hours >= 22)) {
      script_8381(0);
      (void)((local_E0 = 0) & 0x10);
    }
    script_2F793();
    if (task_286 & 4) {
      if (Script_tmnmCheckState(SHEN_NAME('FOOD'))) {
        local_D8 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f);
        switch (local_D8) {
          case 0:
            Script_tmnmUtils(SHEN_NAME('FOOD'), 1535, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
            break;
          case 1:
            Script_tmnmUtils(SHEN_NAME('FOOD'), 0x600, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
            break;
          case 2:
            Script_tmnmUtils(SHEN_NAME('FOOD'), 1537, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
            break;
          case 3:
            Script_tmnmUtils(SHEN_NAME('FOOD'), 1538, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
            break;
        }
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_2E00E(void) {
  while (Script_filmCheckOrSetTransitionUtil(-1) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_cdttLoadScrUtils(CDTT_CMD_Init, 0);
  end:;
}

static void script_2E033(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  script_7FBB(CHARA_AKIR, (DSWKDoorEntry *)data_3BF5C, (int32_t *)&data_3C268, (DoorResourceDescriptor *)&data_3C344, (MapObjectRecord *)data_3C3A4, 10, 0);
  SCNF_WaitFrames_0000(2);
  DSWK_DoorUtils(DOOR_CMD_SET_STATE_1);
  Script_ReturnTimeDate(&local_F4);
  script_82BB();
  script_8834(3, 7, -1, 0x2302AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(1, 3, 6, -1, 0x2302AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(3, 3, 3, -1, 0x2302AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(0, 3, 3, -1, 0x2302AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(12, 0, -1, -1, -1, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(2, 0, -1, -1, -1, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(13, 0, -1, -1, -1, -1, -1, -1);
  local_F0 = Script_ScenDataUtil(task_6C, SCND_ReadU32, 1);
  local_F0 = local_F0 | 0x20000;
  Script_ScenDataUtil(task_6C, SCND_WriteU32, 1, local_F0);
  if ((&local_F4)->hours < 22) {
    script_83B8(0);
  }
  if ((&local_F4)->hours < 20) {
    script_83B8(3);
  }
  if ((&local_F4)->hours < 19) {
    script_83B8(8);
    script_83B8(9);
    script_83B8(10);
    script_83B8(4);
    script_83B8(5);
    script_83B8(6);
    script_83B8(7);
    script_83B8(1);
  }
  (void)((task_282 = -1) & 8);
  return;
}

__attribute__((scn_string_bindings(0, data_42FE4)))
static int32_t script_2E1C2(const char *arg0) {
  script_EAFC("scene/02/MFSY", arg0);
  Script_mapmSetCamFlags(1, 19, 0);
  Script_mapmSetCamFlags(2, 19, 0);
  return (task_281 = 0) & 8;
}

__attribute__((scn_string_bindings(0, data_43034, 2, data_4304B)))
static int32_t script_2E1F3(const char *arg0, int32_t arg1) {
  float local_FC;
  float local_F8;
  float local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  int32_t local_E4;
  {
    int32_t local_E0;
    local_E0 = arg1;
    if (local_E0 == 95) {
      local_F0 = 19;
      local_FC = -24.4f;
      local_F8 = 72.6f;
      local_F4 = 315.0f;
      local_EC = (int32_t)data_42FF2;
      local_E8 = (int32_t)data_42FFD;
      local_E4 = (int32_t)data_43008;
    } else if (local_E0 == 96) {
      local_F0 = 20;
      local_FC = -81.9f;
      local_F8 = 70.8f;
      local_F4 = 135.0f;
      local_EC = (int32_t)data_43013;
      local_E8 = (int32_t)data_4301E;
      local_E4 = (int32_t)data_43029;
    } else {
      goto L02E453;
    }
  }
  for (;;) {
    if (task_284 & 1) goto L02E453;
    if (task_26C.hours >= local_F0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
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
  Script_LoadVoiceFile(1, "SA1091");
  {
    int32_t local_E0;
    local_E0 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
    if (local_E0 == 0) {
      Script_ShowUIMessageFromID((const char *)local_EC);
    } else if (local_E0 == 1) {
      Script_ShowUIMessageFromID((const char *)local_E8);
    } else if (local_E0 == 2) {
      Script_ShowUIMessageFromID((const char *)local_E4);
    } else {
      Script_sprintf("Illegal number\012");
    }
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  script_2E9F6(arg0, arg1);
  script_A671(CHARA_AKIR, local_FC, 0.0f, local_F8);
  script_A699(CHARA_AKIR, 0.0f, local_F4, 0.0f);
  SCNF_WaitFrames_0000(1);
  Script_LoadVoiceFile(0, "SA1091");
  SCNF_WaitFrames_0000(30);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_ryoSleepUtils(3, 1, 8, 0);
  task_279 = 0;
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  Script_convSetCamInactiveFlag(-1);
  L02E453:;
}

static void script_2E454(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  (void)((task_1F0 = 0) & 4);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  {
    int32_t local_F8;
    local_F8 = arg2;
    switch (local_F8) {
      case 80:
        Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 0, 0);
        break;
      case 81:
        Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 1, 0);
        Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 3, 0);
        break;
      case 82:
        Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 16, 0);
        break;
      case 86:
        Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 6, 0);
        Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 15, 0);
        Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 17, 0);
        Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 18, 0);
        break;
      case 87:
        Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 17, 0);
        break;
      case 96:
        Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 11, 0);
        break;
      case 97:
        Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 10, 0);
        break;
    }
  }
  if (-(arg2 == 95) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_SPEAK_TO_YADA_BY_2PM) == 1) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_FORKLIFT_JOB_ACTIVE) == 0)) {
    local_FC = 1;
  } else {
    local_FC = 3;
  }
  if (~task_27B) {
    SCNF_WaitFrames_0000(local_FC * 60 / 2);
  }
  if ((arg2 == 96) | (arg2 == 95)) {
    Script_executeAicaCommand(2720, 1, 30);
    Script_executeAicaCommand(2720, 2, 30);
    Script_Ambient1SoundUtils(2, 17);
    script_EB20();
  } else {
    Script_executeAicaCommand(1184, 1, 96);
  }
  if (~task_27B) {
    Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  (void)((task_281 = -1) & 0x10);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  Script_scenCreateTask(script_2E1C2, 1, arg0);
  script_2EDA6(arg2);
  while (task_281 & 0x20) {
    SCNF_WaitFrames_0000(1);
  }
  {
    int32_t local_F8;
    local_F8 = arg2;
    switch (local_F8) {
      case 96:
        Script_mapmSetRenderFlag(14, 0);
        Script_mapmSetRenderFlag(12, 0);
        script_2EFCB(96);
        Script_LoadSND(0, 1, "amb034.snd", 0);
        script_D86E();
        Script_LoadSND(0, 2, "bgm130.snd", 0);
        Script_executeAicaCommand(16296, 0, 0);
        Script_executeAicaCommand(1184, 2, 100);
        Script_LoadSND(0, 3, "amb049.snd", 0);
        Script_Ambient1SoundUtils(1, 24232);
        Script_LoadSND(0, 4, "f1wshoku.snd", 0);
        script_109B5();
        break;
      case 95:
        script_109B5();
        Script_scenCreateTask(script_249B5, 0);
        break;
    }
  }
  Script_mapmSetCamFlags(1, 19, 1);
  Script_mapmSetRenderFlag(19, 1);
  script_2EEA6(arg2, 0);
  Script_mapmSetCamFlags(1, 9, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
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
  Script_SetGlobalSceneSlotValue34(9, 1);
  Script_SetGlobalSceneSlotValue34(10, 1);
  Script_SetGlobalSceneSlotValue34(11, 1);
  Script_SetGlobalSceneSlotValue34(12, 1);
  Script_SetGlobalSceneSlotValue34(13, 1);
  Script_SetGlobalSceneSlotValue34(14, 1);
  Script_SetGlobalSceneSlotValue34(15, 1);
  Script_SetGlobalSceneSlotValue34(16, 1);
  Script_PREP_Related(-1, 0);
  Script_SetupLightFromTable(arg1);
  Script_PREP_Related(19, 1);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_GetCurrentDrawMode(1, 1);
  Script_MT5Loader_TRWV_TRWAVE(2, task_254);
  script_E05F();
  Script_GULLProgram_Utils(2);
  if (~task_27B) {
    while (task_7C.id == 0x52494B41) {
      SCNF_WaitFrames_0000(1);
    }
  }
  SCNF_WaitFrames_0000(10);
  if (-(arg2 == 82) & ~task_27B) {
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Script_scenCreateTask(script_2E8F6, 0);
  } else {
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
    if (arg2 != 95) {
      Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ryoControl(RYO_RestoreControl);
    }
    Script_scenCreateTask(script_2E1F3, 2, arg0, arg2);
  }
  (void)((task_1F0 = -1) & 2);
  return;
}

static int32_t script_2E8F6(void) {
  script_259D6(1);
  Script_ryoControl(RYO_RestoreControl);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('LUV2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('LUV2'), CDM_Show);
      goto L02E92F;
    }
  }
  L02E92F:;
  return Script_scenToggleVisibleFor(SHEN_NAME('LUV2'), 0);
}

static void script_2E93C(const char *arg0, int32_t arg1) {
  (void)((task_1F0 = 0) & 2);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 110.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 70.0f);
  Script_LODUtils(LOD_UTILS_SET_NPC_DISPLAY_RADIUS, 48.0f);
  if (arg1 == 88) {
    Script_MT5Loader_TRWV_TRWAVE(1, task_254);
  }
  SCNF_WaitFrames_0000(120);
  script_2E9F6(arg0, arg1);
  while (task_7C.id == 0x52494B41) {
    SCNF_WaitFrames_0000(1);
  }
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  SCNF_WaitFrames_0000(10);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ryoControl(RYO_RestoreControl);
  (void)((task_1F0 = -1) & 2);
  return;
}

static void script_2E9F6(const char *arg0, int32_t arg1) {
  (void)((task_284 = -1) & 2);
  Script_MT5Loader_TRWV_TRWAVE(1, task_254);
  {
    int32_t local_FC;
    local_FC = arg1;
    if (local_FC == 96) {
      script_2ECE5();
    } else if (local_FC == 95) {
      script_249CC();
    } else {
      Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      script_2EDA6(0x5953464D);
    }
  }
  {
    int32_t local_FC;
    local_FC = arg1;
    switch (local_FC) {
      case 80:
        Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 0, 1);
        break;
      case 81:
        Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 1, 1);
        Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 3, 1);
        break;
      case 82:
        Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 16, 1);
        break;
      case 86:
        Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 6, 1);
        Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 15, 1);
        Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 17, 1);
        Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 18, 1);
        break;
      case 87:
        Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 17, 1);
        break;
      case 96:
        Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 11, 1);
        break;
      case 97:
        Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 10, 1);
        break;
    }
  }
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
  Script_SetGlobalSceneSlotValue34(9, 0);
  Script_SetGlobalSceneSlotValue34(10, 0);
  Script_SetGlobalSceneSlotValue34(11, 0);
  Script_SetGlobalSceneSlotValue34(12, 0);
  Script_SetGlobalSceneSlotValue34(13, 0);
  Script_SetGlobalSceneSlotValue34(14, 0);
  Script_SetGlobalSceneSlotValue34(15, 0);
  Script_SetGlobalSceneSlotValue34(16, 0);
  Script_PREP_Related(-1, 0);
  if (task_26C.hours >= 19) {
    script_2F1B4(0);
    script_82BB();
    if (task_26C.hours < 22) {
      script_83B8(0);
    }
    if (task_26C.hours < 20) {
      script_83B8(3);
    }
  } else {
    Script_SetCharaFlags(CHARA_AKIR, 4);
    Script_GetCurrentDrawMode(1, 0);
    Script_shenSetLightingMode(1);
    Script_LightUtils(0, 0, 2);
    Script_lightSetSlotEnabled(0, 1);
  }
  Script_executeAicaCommand(1184, 1, 127);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 180.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 100.0f);
  Script_LODUtils(LOD_UTILS_SET_NPC_DISPLAY_RADIUS, 48.0f);
  script_1058B();
  (void)((task_2A1 = 0) & 0x10);
  script_E07A();
  script_D9DE();
  Script_mapmSetCamFlags(1, 9, 1);
  script_2EEA6(arg1, 1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, arg0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PROCESS_COLS, 0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

__attribute__((scn_string_bindings(0, data_43080, 1, data_4308B, 2, data_43092)))
static void script_2ECE5(void) {
  Script_executeAicaCommand(2720, 2, 30);
  Script_executeAicaCommand(2720, 3, 30);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_cdttLoadScrUtils(CDTT_CMD_SetMode, 0x5953464D);
  SCNF_WaitFrames_0000(30);
  Script_scenCreateTask(script_2E00E, 0);
  script_EB20();
  Script_Ambient1SoundUtils(2, 17);
  Script_LoadSND(0, 1, "amb013.snd", 0);
  script_D9D6();
  Script_LoadSND(0, 2, "SELECT", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  Script_LoadSND(0, 4, "f1yokosu.snd", 0);
  script_2F0D2();
  return;
}

static void script_2EDA6(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  if (local_FC == 80) {
    Script_cdttLoadScrUtils(CDTT_CMD_SetMode, 0x32304E4D);
  } else if (local_FC == 81) {
    Script_cdttLoadScrUtils(CDTT_CMD_SetMode, 0x33304E4D);
  } else if (local_FC == 82) {
    Script_cdttLoadScrUtils(CDTT_CMD_SetMode, 0x34304E4D);
  } else if (local_FC == 86) {
    Script_cdttLoadScrUtils(CDTT_CMD_SetMode, 0x32314E4D);
  } else if (local_FC == 87) {
    Script_cdttLoadScrUtils(CDTT_CMD_SetMode, 0x33314E4D);
  } else if (local_FC == 88) {
    Script_cdttLoadScrUtils(CDTT_CMD_SetMode, 0x34314E4D);
  } else if (local_FC == 98) {
    Script_cdttLoadScrUtils(CDTT_CMD_SetMode, 0x38314E4D);
  } else if (local_FC == 95) {
    Script_cdttLoadScrUtils(CDTT_CMD_SetMode, 0x31304F4D);
  } else if (local_FC == 96) {
    Script_cdttLoadScrUtils(CDTT_CMD_SetMode, 0x4F59534D);
  } else {
    Script_cdttLoadScrUtils(CDTT_CMD_SetMode, 0x5953464D);
  }
  SCNF_WaitFrames_0000(30);
  Script_scenCreateTask(script_2E00E, 0);
  return;
}

static void script_2EEA6(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = arg0;
  if (local_FC == 80) {
    arg0 = 8;
  } else if (local_FC == 81) {
    arg0 = 9;
  } else if (local_FC == 82) {
    arg0 = 10;
  } else if (local_FC == 86) {
    arg0 = 4;
  } else if (local_FC == 87) {
    arg0 = 5;
  } else if (local_FC == 88) {
    arg0 = 6;
  } else if (local_FC == 98) {
    arg0 = 7;
  } else if (local_FC == 95) {
    arg0 = 1;
  } else if (local_FC == 96) {
    arg0 = 3;
  } else {
    goto L02EFCA;
  }
  if (arg1) {
    int32_t local_F8;
    local_F8 = Script_ScenDataUtil(task_6C, SCND_ReadU32, arg0 * 13 + 1);
    local_F8 = local_F8 & -131073;
    Script_ScenDataUtil(task_6C, SCND_WriteU32, arg0 * 13 + 1, local_F8);
  } else {
    int32_t local_F8;
    local_F8 = Script_ScenDataUtil(task_6C, SCND_ReadU32, arg0 * 13 + 1);
    local_F8 = local_F8 | 0x20000;
    Script_ScenDataUtil(task_6C, SCND_WriteU32, arg0 * 13 + 1, local_F8);
  }
  L02EFCA:;
  return;
}

static void script_2EFCB(int32_t arg0) {
  Vector3 local_F4;
  Vector3 local_E8;
  int32_t local_E4;
  (&local_E8)->y = 1.0f;
  local_E4 = arg0;
  switch (local_E4) {
    case 96:
      local_E8.x = -82.7967f;
      (&local_E8)->z = 67.709f;
      break;
    case 95:
      local_E8.x = -26.090599f;
      (&local_E8)->z = 76.883102f;
      break;
  }
  local_F4.x = (&local_F4)->z = 0.0f;
  (&local_F4)->y = 0.00980392192f;
  task_258 = Script_EffectUtils(0, &local_E8, &data_430A0, 50, 0.08f, 0, 0);
  Script_EffectUtils(12, task_258, 8);
  Script_EffectUtils(11, task_258, 255);
  Script_EffectUtils(9, task_258, 193, 193, 191);
  Script_EffectUtils(10, task_258, 0.356862754f);
  Script_EffectUtils(8, task_258, 0.392156869f, 0.431372553f);
  Script_EffectUtils(7, task_258, 0.052941177f);
  Script_EffectUtils(6, task_258, &local_F4, &data_430AC);
  Script_EffectUtils(1, task_258);
  return;
}

static void script_2F0D2(void) {
  if (task_258 != -1) {
    Script_EffectUtils(2, task_258);
    Script_EffectUtils(3, task_258);
    task_258 = -1;
  }
  return;
}

static void script_2F0FA(void) {
  script_10529();
  if (-1) {
    Script_charaDisplay(SHEN_NAME('TOMD'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('TOMD'), CDM_Show);
      goto L02F129;
    }
  }
  L02F129:;
  Script_scenToggleVisibleFor(SHEN_NAME('TOMD'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('OTO1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('OTO1'), CDM_Show);
      goto L02F15D;
    }
  }
  L02F15D:;
  Script_scenToggleVisibleFor(SHEN_NAME('OTO1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('OTO8'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('OTO8'), CDM_Show);
      goto L02F191;
    }
  }
  L02F191:;
  Script_scenToggleVisibleFor(SHEN_NAME('OTO8'), 0);
  Script_charaDisplay(SHEN_NAME('OTC1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('OTC8'), CDM_Show);
  return;
}

static void script_2F1B4(int32_t arg0) {
  if (arg0) {
    script_2F265();
  }
  script_10EE9();
  Script_SetCharaFlags(CHARA_AKIR, 0);
  Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 2);
  Script_InitializeOsag(CHARA_AKIR);
  Script_PREP_Related(-1, 0);
  Script_SetupLightFromTable(0);
  Script_PREP_Related(-1, 1);
  Script_GetCurrentDrawMode(1, 1);
  script_2F50A();
  while (~task_282) {
    SCNF_WaitFrames_0000(1);
  }
  if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_HISA_REQUEST_UNLOCKED) == 1) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MKSG_HELP_ROUTE_ACTIVE) == 0)) {
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x4242454B, 1);
    SCNF_WaitFrames_0000(1);
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x4242454B, 0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_430B8, 1, data_430C6, 3, data_430D9, 5, data_430EA, 6, data_430F3, 7, data_430FC)))
static void script_2F265(void) {
  int32_t unused_FC;
  script_110D8();
  Script_SetActionSelectorHUDElementVisibility(1);
  Script_executeAicaCommand(2720, 2, 30);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (-1) {
    Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_AKIR, CDM_Show);
      goto L02F2D1;
    }
  }
  L02F2D1:;
  Script_scenToggleVisibleFor(CHARA_AKIR, 0);
  (void)((task_2A2 = -1) & 1);
  Script_Ambient1SoundUtils(2, 17);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  script_EAA8("scene/02/MFSY", "NIGHT");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 0, "SEQDATA4.BIN", "NIGHT");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  script_EBBF("bgm109.snd", 0);
  if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 2) | Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 3)) {
    Script_mapmSetRenderFlag(14, 1);
  } else {
    Script_mapmSetRenderFlag(12, 1);
  }
  Script_mapmSetRenderFlag(13, 1);
  Script_AddCharacterHandler(3, "DR02_021", SHEN_NAME('YDR0'));
  Script_AddCharacterHandler(3, "DR02_021", SHEN_NAME('YDR1'));
  script_A671(SHEN_NAME('YDR0'), -90.242859f, 0.0f, 18.416651f);
  script_A671(SHEN_NAME('YDR1'), -99.591637f, 0.0f, 9.065865f);
  script_A699(SHEN_NAME('YDR0'), 0.0f, -45.0f, 0.0f);
  script_A699(SHEN_NAME('YDR1'), 0.0f, -45.0f, 0.0f);
  Script_charaDisplay(SHEN_NAME('YDR0'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('YDR1'), CDM_Show);
  Script_LODSnapshotUtils(DISP_CMD_SaveDistancesAndCaptureLodState);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  script_21A9F();
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(1);
  Script_SetMapControlTaskFlag(SHEN_NAME('YDR0'), 1);
  Script_SetMapControlTaskFlag(SHEN_NAME('YDR1'), 1);
  Script_AuthorUtils(AUTH_FREE, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "NIGHT");
  Script_LODSnapshotUtils(DISP_CMD_RestoreDistancesAndApplyLodState);
  script_D9D6();
  script_EB73();
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  Script_SetActionSelectorHUDElementVisibility(0);
  if (0) {
    Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_AKIR, CDM_Show);
      goto L02F4C4;
    }
  }
  L02F4C4:;
  Script_scenToggleVisibleFor(CHARA_AKIR, 1);
  (void)((task_54C = -1) & 1);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  script_11125();
  Script_convSetCamInactiveFlag(-1);
  Script_SetGameStateTo(AUTH_None);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  return;
}

static void script_2F50A(void) {
  int32_t local_F4[3];
  int32_t local_E8[3];
  int32_t local_DC[3];
  int32_t local_D0[3];
  Vector3 local_C4;
  int32_t local_B8[3];
  Vector3 local_AC;
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_AC, ENTITY_XFORM_WORLD_SPACE);
  if (!((&local_AC)->x >= -124.5f) & !((&local_AC)->z >= 0.0f)) {
    local_AC.x = -124.5f;
    Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &local_AC);
  }
  local_F4[0] = 0x4207999A;
  local_F4[1] = 0;
  local_F4[2] = 0x42AA3333;
  local_E8[0] = 0x4211999A;
  local_E8[1] = 0;
  local_E8[2] = 0x42A53333;
  local_DC[0] = 0x421B999A;
  local_DC[1] = 0;
  local_DC[2] = 0x42AA3333;
  local_D0[0] = 0x4212CCCD;
  local_D0[1] = 0;
  local_D0[2] = 0x42AF6666;
  if (script_EC53(CHARA_AKIR, (int32_t)local_F4, (int32_t)local_E8, (int32_t)local_DC, (int32_t)local_D0)) {
    local_C4.x = 35.134499f;
    (&local_C4)->y = 0.0f;
    (&local_C4)->z = 86.284599f;
    local_B8[0] = 0;
    local_B8[1] = 0xE000;
    local_B8[2] = 0;
    Script_SetCycleManagerData(31, (const uint32_t *)&local_C4, (const uint32_t *)local_B8, 1, 0x30303030);
    Script_MoveObjectToObject(CHARA_AKIR, 31);
    SCNF_WaitFrames_0000(1);
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_C4, 0);
    (&local_C4)->z = 0.6f;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_C4);
    Script_MoveObjectToObject(CHARA_AKIR, -1);
  }
  local_F4[0] = 0x41073333;
  local_F4[1] = 0;
  local_F4[2] = 0x43054000;
  local_E8[0] = 0x41366666;
  local_E8[1] = 0;
  local_E8[2] = 0x43086666;
  local_DC[0] = 0x410B3333;
  local_DC[1] = 0;
  local_DC[2] = 0x430B199A;
  local_D0[0] = 0x40B1999A;
  local_D0[1] = 0;
  local_D0[2] = 0x4308428F;
  if (script_EC53(CHARA_AKIR, (int32_t)local_F4, (int32_t)local_E8, (int32_t)local_DC, (int32_t)local_D0)) {
    local_C4.x = 7.257f;
    (&local_C4)->y = 0.0f;
    (&local_C4)->z = 135.15f;
    local_B8[0] = 0;
    local_B8[1] = 39139;
    local_B8[2] = 0;
    Script_SetCycleManagerData(31, (const uint32_t *)&local_C4, (const uint32_t *)local_B8, 1, 0x30303030);
    Script_MoveObjectToObject(CHARA_AKIR, 31);
    SCNF_WaitFrames_0000(1);
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_C4, 0);
    (&local_C4)->z = 0.6f;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_C4);
    Script_MoveObjectToObject(CHARA_AKIR, -1);
  }
  return;
}

static void script_2F793(void) {
  if (!((&task_23C)->x <= 66.0f)) {
    Script_mapmSetRenderFlag(16, 1);
  } else {
    Script_mapmSetRenderFlag(16, 0);
  }
  if (!((&task_23C)->x <= 25.0f)) {
    if (task_277 != 2) {
      script_1031E();
      script_1044D();
      task_277 = 2;
      task_278 = 2;
    }
  } else {
    if (!(task_23C.z <= 35.0f)) {
      if (task_277 != 1) {
        script_102A9();
        task_277 = 1;
      }
    } else if (!(task_23C.z <= 2.0f)) {
      if (task_277 != 0) {
        script_10234();
        task_277 = 0;
      }
    } else {
      if (task_277 != 3) {
        script_100B0();
        task_277 = 3;
      }
    }
    if (!((&task_23C)->x >= -40.0f) & !(task_23C.z >= 60.0f)) {
      if (task_278 != 0) {
        script_103F0();
        task_278 = 0;
      }
    } else {
      if (task_278 != 1) {
        script_10393();
        task_278 = 1;
      }
    }
  }
  return;
}

static void script_2F8A1(void) {
  SCNF_WaitFrames_0000(1);
  Script_WeatherUtils(WeatherScript_LoadWccpClipSet0FromTaskOffset, &data_43104, 28);
  Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 26, 0);
  Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 27, 0);
  Script_WeatherUtils(WeatherScript_LoadWccpClipSet1FromTaskOffset, &data_43724, 5);
  return;
}

static int32_t script_2F8DE(int32_t arg0, int32_t arg1) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_executeAicaCommand(arg0, 0, 0);
  return Script_srchControl(0);
}

static int32_t script_2F902(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  task_574[arg1] = 0;
  task_584[arg1] = 0;
  task_594[arg1] = 0;
  task_5A4[arg1] = arg2;
  task_5B4[arg1] = arg3;
  task_5C4 = 0;
  Script_CharHandlers(arg0, 0, task_5A4[arg1], 16);
  Script_CharHandlers(arg0, 1, task_5B4[arg1], 16);
  for (;;) {
    task_574[arg1] = 0;
    while (task_574[arg1] == 0) {
      SCNF_WaitFrames_0000(1);
    }
    task_5C4 = task_594[arg1] | 0x40000000;
    Script_scenCreateTask(arg4, 3, arg0, arg1, task_574[arg1] + task_584[arg1]);
  }
  end:;
}

static int32_t script_2F9D9(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  local_FC = task_5C4;
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_motmGetCharaFlags(arg4, MOTM_PoseNone, SCEN_Write);
  Script_motmToggleFlags(arg4, SCEN_Enable, 0x40);
  Script_PlayCharaMotion(arg4, arg0, arg2, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(arg4, arg3);
  if (local_FC & -0x80000000) {
    Script_motmToggleFlags(arg4, SCEN_Enable, 0x80);
  }
  if (local_FC & 0x40000000) {
    Script_charaToggleMotEnabled(arg4, 1);
  }
  if (local_FC & 0x20000000) {
    Script_charaToggleMotEnabled(arg4, 0);
  }
  return Script_srchControl(0);
}

static void script_2FA70(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_CharHandlers(arg0, 2, arg2, 16);
  if (arg4 > 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  Script_CharHandlers(arg0, 0, task_5A4[arg1], 16);
  Script_CharHandlers(arg0, 1, task_5B4[arg1], 16);
  Script_srchControl(0);
  return;
}

static void script_2FAD2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  local_FC = task_5C4;
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  if (local_FC & 0x10000000) {
    Script_GetDataFromFightProgram(arg0, arg1);
  }
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (local_FC & 0x10000000) {
    Script_GetDataFromFightProgram(arg0, 0);
  }
  Script_srchControl(0);
  return;
}

static void script_2FB2E(int32_t arg0, float arg1, int32_t arg2) {
  int32_t local_FC;
  Vector3i local_F0;
  local_FC = task_5C4;
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  if (local_FC & -0x80000000) {
    arg1 = (float)0 - arg1;
  }
  (&local_F0)->y = (int32_t)(arg1 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, -0x70000000, &local_F0, 0);
  Script_srchControl(0);
  return;
}

static void script_2FB8B(int32_t arg0) {
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_2FBA7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, void *arg4, int32_t arg5) {
  int32_t local_FC;
  task_5F8[arg1] = Script_scenCreateTask(script_2F902, 5, arg0, arg1, arg2, arg3, arg5);
  task_5C8[arg1] = arg0;
  task_5D8[arg1] = 0;
  task_5E8[arg1] = 0;
  task_608 = 4;
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (task_5D8[arg1]) {
      local_FC = task_5E8[arg1] * 12;
      {
        int32_t local_F8;
        local_F8 = Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 5);
        if (local_F8 > 0) {
          SCNF_WaitFrames_0000(local_F8);
        }
      }
      {
        int32_t local_F8;
        local_F8 = Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 8);
        if (local_F8 > 0) {
          task_574[arg1] = local_F8;
        }
      }
      {
        int32_t local_F8;
        local_F8 = Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 6);
        if (local_F8 > 0) {
          SCNF_WaitFrames_0000(local_F8);
        }
      }
      if (task_584[arg1] == 0) {
        Script_scenCreateTask(script_2FF17, 4, Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC), Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 1), Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 2), Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 3));
      }
      {
        int32_t local_F8;
        local_F8 = Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 7);
        if (local_F8 > 0) {
          SCNF_WaitFrames_0000(local_F8);
        }
      }
      {
        int32_t local_F8;
        local_F8 = Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 9);
        if (local_F8 > 0) {
          task_574[arg1] = local_F8;
        }
      }
      script_2FE15();
      {
        int32_t local_F8;
        if (task_614 == Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC)) {
          task_610 = 1;
          local_F8 = Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 10);
        } else {
          if (task_584[arg1] == 0) {
            task_61C = task_61C + Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 4);
          }
          task_610 = 0;
          local_F8 = Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 11);
        }
        if (local_F8 > 0) {
          task_574[arg1] = local_F8;
        }
      }
      script_2FE05(arg1);
    }
  }
  end:;
}

static void script_2FDCE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  task_5D8[arg0] = 1;
  task_5E8[arg0] = arg1;
  task_584[arg0] = arg2;
  task_594[arg0] = arg3;
  return;
}

static void script_2FE05(int32_t arg0) {
  task_5D8[arg0] = 0;
  return;
}

static void script_2FE15(void) {
  SCNF_WaitFrames_0000(2);
  while (task_614 == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2FE35(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  script_2FDCE(arg0, arg1, 0, arg3);
  script_2FDCE(arg2, arg1, 1, arg3);
  while (task_5D8[arg0]) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2FE6F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  script_2FDCE(arg0, arg1, 0, arg3);
  script_2FDCE(arg2, arg1, 1, arg3);
  script_2FDCE(arg2 + 1, arg1, 2, arg3);
  while (task_5D8[arg0]) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2FEBB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  script_2FDCE(arg0, arg1, 0, arg4);
  script_2FDCE(arg2, arg1, 1, arg4);
  script_2FDCE(arg3, arg1, 2, arg4);
  while (task_5D8[arg0]) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2FF04(int32_t arg0) {
  task_608 = arg0;
  Script_SetQteOverlayTimeoutTuning(arg0, QTE_SetTimeoutStep);
  return;
}

static int32_t script_2FF17(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = arg1 + arg3;
  local_F8 = arg2 + arg3;
  Script_scenCreateTask(script_2FF7A, 2, arg1, arg3);
  Script_SetQteOverlayTimeoutTuning(local_FC, QTE_SetTimeoutMaximum);
  if (arg0 == 68) {
    SCNF_UIUtils(0x44, local_F8, 5, 0, 1, 0);
  } else {
    SCNF_UIUtils(arg0, local_F8, 2, 0, 1, 0);
  }
  return Script_srchControl(0);
}

static int32_t script_2FF7A(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  task_618 = arg0;
  task_614 = 0;
  task_60C = 0;
  local_F8 = 0xFFFF;
  for (;;) {
    local_FC = SCNF_UIUtils(0, 0, 0, 0, 0, -1);
    task_60C = task_60C + 1;
    if (local_FC != 0xFFFF) {
      if (local_FC != 0xA000) {
        local_F8 = local_FC;
      }
    }
    if (arg0 == -1) {
      if (local_F8 != 0xFFFF) {
        break;
      }
    } else {
      if (task_60C >= arg0 + arg1 + 4) {
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  if ((local_F8 & 0xFFF) == 0) {
    local_F8 = 0xFFFF;
  }
  task_614 = local_F8 & 0xFFF;
  return Script_srchControl(0);
}

static int32_t script_3003B(int32_t arg0, int32_t arg1, int32_t arg2) {
  {
    Vector3 local_F4;
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    task_620[arg1] = arg0;
    task_630[arg1][0] = (&local_F4)->x;
    task_630[arg1][1] = (&local_F4)->y;
    task_630[arg1][2] = (&local_F4)->z;
  }
  task_660[arg1] = 0.0f;
  for (;;) {
    Vector3 local_F4;
    SCNF_WaitFrames_0000(1);
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    task_630[arg1][0] = (&local_F4)->x;
    task_630[arg1][1] = (&local_F4)->y;
    task_630[arg1][2] = (&local_F4)->z;
    task_660[arg1] = Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, task_630[arg1][0] - task_630[arg2][0], task_630[arg1][2] - task_630[arg2][2]);
  }
  end:;
}

static void script_30129(void) {
  task_670 = 0.0f;
  task_674 = 0.0f;
  task_678 = 0.0f;
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_GetInputData(2, 0) & 4) {
      if (Script_GetInputData(0, 2) & 0x4000) {
        task_670 = task_670 - 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x8000) {
        task_670 = task_670 + 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x1000) {
        task_674 = task_674 - 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x2000) {
        task_674 = task_674 + 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x40) {
        task_678 = task_678 - 1.0f;
      }
      if (Script_GetInputData(0, 2) & 0x80) {
        task_678 = task_678 + 1.0f;
      }
    }
  }
  return;
}

static void script_30217(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(arg0, &local_E8, 0);
  Script_MoveObjectToObject(arg0, -1);
  return;
}

static void script_30240(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4) {
  float local_F4[3];
  Vector3i local_E8;
  int32_t local_E4;
  Vector3 local_D8;
  local_E4 = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  local_F4[0] = arg2;
  local_F4[1] = 0.0f;
  local_F4[2] = arg3;
  local_D8.x = arg2;
  (&local_D8)->z = arg3;
  Script_EVT_SetPosition(arg0, 0x28000000, &local_D8);
  local_E8.x = 0;
  (&local_E8)->y = (int32_t)(arg4 / 360.0f * (float)0x10000);
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
  Script_MoveObjectToObject(arg0, local_E4);
  Script_InitializeOsag(arg0);
  return;
}

static void script_302CF(int32_t arg0, int32_t arg1, const Vector3 *arg2) {
  int32_t local_FC;
  local_FC = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_SetPosition(arg0, POS_XYZ, arg2);
  Script_MoveObjectToObject(arg0, local_FC);
  Script_InitializeOsag(arg0);
  return;
}

static void script_30302(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_3031C, 6, arg0, arg1, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_3031C(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, int32_t arg5) {
  if (arg5 >= 0) {
    SCNF_WaitFrames_0000(arg5);
  }
  script_30240(arg0, arg1, arg2, arg3, arg4);
  return Script_srchControl(0);
}

static void script_30347(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, float arg5) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  local_F4.x = arg2;
  (&local_F4)->y = arg3;
  (&local_F4)->z = arg4;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  local_E8.x = 0;
  (&local_E8)->y = (int32_t)(arg5 / 360.0f * (float)0x10000);
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
  Script_MoveObjectToObject(arg0, local_E4);
  Script_InitializeOsag(arg0);
  return;
}

static void script_303C7(int32_t arg0, int32_t arg1, float arg2) {
  int32_t unused_F4[3];
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  local_E8.x = 0;
  (&local_E8)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
  Script_MoveObjectToObject(arg0, local_E4);
  return;
}

static void script_3041B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_30431, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_30431(int32_t arg0, int32_t arg1, float arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  script_303C7(arg0, arg1, arg2);
  return Script_srchControl(0);
}

static void script_30458(int32_t arg0, float arg1) {
  Vector3i local_F4;
  (&local_F4)->y = (int32_t)(arg1 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, -0x70000000, &local_F4, 0);
  return;
}

static void script_30481(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, -0x48000000, &local_F4, 0);
  return;
}

static void script_304D5(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_304E9, 3, arg0, arg1, arg2);
  return;
}

static int32_t script_304E9(int32_t arg0, float arg1, int32_t arg2) {
  if (arg2 >= 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  script_30458(arg0, arg1);
  return Script_srchControl(0);
}

static void script_3050E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_30526, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_30526(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4) {
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  script_30481(arg0, arg1, arg2, arg3);
  return Script_srchControl(0);
}

static void script_3054F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_30567, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_30567(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4) {
  float local_FC;
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  local_FC = (arg2 - arg1) / arg3;
  for (;;) {
    if (arg3 <= 0.0f) {
      break;
    }
    script_30481(arg0, 0.0f, 0.0f, local_FC);
    arg3 = arg3 - 1.0f;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_305BC(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = arg0;
  while (local_E4 <= arg1) {
    Script_EVT_GetPosition(task_5C8[local_E4], -1, &local_F4, 0);
    Script_mtwkGetSetRotation(task_5C8[local_E4], &local_E8, 0);
    task_67C[local_E4][0] = (&local_F4)->x;
    task_67C[local_E4][1] = (&local_F4)->y;
    task_67C[local_E4][2] = (&local_F4)->z;
    task_6AC[local_E4][0] = (&local_E8)->x;
    task_6AC[local_E4][1] = (&local_E8)->y;
    task_6AC[local_E4][2] = (&local_E8)->z;
    task_6DC[local_E4] = Script_GetCharaMotionRemainingFramesScaled(task_5C8[local_E4]);
    task_6EC[local_E4] = Script_scenCharaGetMotAnimId(task_5C8[local_E4]);
    local_E4 = local_E4 + 1;
  }
  return;
}

static void script_306A1(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = arg0;
  while (local_E4 <= arg1) {
    local_F4.x = task_67C[local_E4][0];
    (&local_F4)->y = task_67C[local_E4][1];
    (&local_F4)->z = task_67C[local_E4][2];
    local_E8.x = task_6AC[local_E4][0];
    (&local_E8)->y = task_6AC[local_E4][1];
    (&local_E8)->z = task_6AC[local_E4][2];
    Script_EVT_SetPosition(task_5C8[local_E4], POS_XYZ, &local_F4);
    Script_EVT_SetRotation(task_5C8[local_E4], 0x38000000, &local_E8, 0);
    Script_PlayCharaMotion(task_5C8[local_E4], task_6EC[local_E4], task_6DC[local_E4], -1.0f, -1.0f, 1.0f, 0);
    local_E4 = local_E4 + 1;
  }
  return;
}

static void script_3078A(int32_t arg0) {
  task_700 = arg0;
  task_6FC = 1;
  Script_PlayerStatsUtils(PS_Set70MBTimeCapSlot, task_700, task_6FC);
  return;
}

static void script_307A8(void) {
  int32_t local_FC;
  if (task_6FC == 1) {
    local_FC = Script_PlayerStatsUtils(PS_Get70MBCounter1);
    Script_PlayerStatsUtils(PS_Set70MBCounter1, local_FC + 1);
  }
  if (task_6FC < 255) {
    task_6FC = task_6FC + 1;
  }
  Script_PlayerStatsUtils(PS_Set70MBTimeCapSlot, task_700, task_6FC);
  return;
}

static void script_307F0(void) {
  int32_t local_FC;
  local_FC = Script_PlayerStatsUtils(PS_Get70MBCounter0);
  Script_PlayerStatsUtils(PS_Set70MBCounter0, local_FC + 1);
  return;
}

static void script_3080B(void) {
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_30825(const char *arg0, const char *arg1) {
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg1) == 0) {
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, arg0, arg1) == 0) {
      SCNF_WaitFrames_0000(1);
    }
    script_3080B();
  }
  return;
}

static void script_30863(const char *arg0) {
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg0) != 0) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, arg0);
  }
  return;
}

static void script_30884(int32_t arg0) {
  L030886:;
  SCNF_WaitFrames_0000(1);
  if (!Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, arg0)) goto L030886;
  return;
}

static void script_3089B(int32_t arg0) {
  int32_t local_FC;
  local_FC = 0;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) != 0) {
    local_FC = Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT_INTERNAL_HUMAN, arg0);
  } else {
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 1);
    while (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
      local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
      while (local_FC == 0) {
        SCNF_WaitFrames_0000(1);
        local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
      }
      script_30884(local_FC);
    }
  }
  (void)local_FC;
  return;
}

static void script_30921(int32_t arg0, int32_t arg1) {
  if (arg1) {
    Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, arg1);
  }
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 0);
  arg1 = 0;
  return;
}

static int32_t script_30944(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  local_FC = arg2;
  switch (local_FC) {
    case 1:
      Script_scenCreateTask(script_2F9D9, 6, 22286, -1, 0x41200000, 0x3F800000, arg0, arg1);
      break;
    case 2:
      Script_scenCreateTask(script_2F9D9, 6, 22299, -1, 0x41200000, 0x3F800000, arg0, arg1);
      break;
    case 3:
      Script_scenCreateTask(script_2F8DE, 2, 0x2076AB, -1);
      script_31E81(arg0, 2, 3, 7, -1);
      script_31E81(arg0, 2, 1, 7, 30);
      break;
    case 4:
      break;
    case 5:
      Script_scenCreateTask(script_2F8DE, 2, 0x2476AB, -1);
      Script_scenCreateTask(script_2F9D9, 6, 22278, -1, 0x420C0000, 0x3F800000, arg0, arg1);
      script_31E81(arg0, 7, 3, 7, -1);
      script_31E81(arg0, 2, 1, 7, 20);
      break;
    case 6:
      Script_scenCreateTask(script_2F9D9, 6, 22292, -1, 0x420C0000, 0x3F800000, arg0, arg1);
      break;
    case 7:
      Script_scenCreateTask(script_2F9D9, 6, 22287, 12, 0, 0x3F800000, arg0, arg1);
      break;
    case 8:
      Script_scenCreateTask(script_2F9D9, 6, 22300, 12, 0, 0x3F800000, arg0, arg1);
      break;
    case 9:
      Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
      Script_scenCreateTask(script_2F8DE, 2, 0x1376AB, 5);
      Script_scenCreateTask(script_2F8DE, 2, 0x2576AB, 40);
      Script_scenCreateTask(script_2F8DE, 2, 459945, 65);
      break;
    case 10:
      Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
      break;
    case 11:
      Script_scenCreateTask(script_2F9D9, 6, 22279, -1, 0x41800000, 0x3F800000, arg0, arg1);
      Script_scenCreateTask(script_2F8DE, 2, 66729, 10);
      Script_scenCreateTask(script_2F8DE, 2, 161451, 30);
      script_31E81(arg0, 7, 1, 7, -1);
      script_31E81(arg0, 4, 5, 7, 30);
      break;
    case 12:
      Script_scenCreateTask(script_2F9D9, 6, 22293, -1, 0x41800000, 0x3F800000, arg0, arg1);
      break;
    case 13:
      Script_scenCreateTask(script_2F9D9, 6, 22288, -1, 0x41F00000, 0x3F800000, arg0, arg1);
      break;
    case 14:
      Script_scenCreateTask(script_2F9D9, 6, 22316, -1, 0x41F00000, 0x3F800000, arg0, arg1);
      break;
    case 15:
      Script_scenCreateTask(script_2F8DE, 2, 0x2576AB, 180);
      Script_scenCreateTask(script_2F8DE, 2, 459945, 200);
      break;
    case 16:
      break;
    case 17:
      Script_scenCreateTask(script_2F9D9, 6, 22274, -1, 0x42340000, 0x3FC00000, arg0, arg1);
      Script_scenCreateTask(script_2F8DE, 2, 1193, 5);
      break;
    case 18:
      Script_scenCreateTask(script_2F9D9, 6, 22304, -1, 0x42480000, 0x3FC00000, arg0, arg1);
      break;
    case 19:
      Script_scenCreateTask(script_2F9D9, 6, 22288, -1, 0x41F00000, 0x3F800000, arg0, arg1);
      break;
    case 20:
      Script_scenCreateTask(script_2F9D9, 6, 22316, -1, 0x41F00000, 0x3F800000, arg0, arg1);
      break;
    case 21:
      Script_scenCreateTask(script_2F8DE, 2, 0x2576AB, 180);
      Script_scenCreateTask(script_2F8DE, 2, 459945, 210);
      break;
    case 22:
      break;
    case 23:
      Script_scenCreateTask(script_2F9D9, 6, 22275, -1, 0x41200000, 0x3FC00000, arg0, arg1);
      Script_scenCreateTask(script_2F8DE, 2, 1193, 5);
      break;
    case 24:
      Script_scenCreateTask(script_2F9D9, 6, 22305, -1, 0x41A00000, 0x3FC00000, arg0, arg1);
      break;
    case 25:
      Script_scenCreateTask(script_2F9D9, 6, 22288, -1, 0x41F00000, 0x3F800000, arg0, arg1);
      break;
    case 26:
      Script_scenCreateTask(script_2F9D9, 6, 22316, -1, 0x41F00000, 0x3F800000, arg0, arg1);
      break;
    case 27:
      Script_scenCreateTask(script_2F8DE, 2, 0x2576AB, 180);
      Script_scenCreateTask(script_2F8DE, 2, 459945, 210);
      break;
    case 28:
      break;
    case 29:
      Script_scenCreateTask(script_2F9D9, 6, 22276, -1, 0x41200000, 0x3FC00000, arg0, arg1);
      Script_scenCreateTask(script_2F8DE, 2, 1193, 5);
      break;
    case 30:
      Script_scenCreateTask(script_2F9D9, 6, 22306, -1, 0x41A00000, 0x3FC00000, arg0, arg1);
      break;
  }
  return Script_srchControl(0);
}

static int32_t script_30EA9(int32_t arg0, int32_t arg1, float arg2, int32_t arg3, const void *arg4) {
  SCNF_WaitFrames_0000(1);
  task_704[arg1] = arg0;
  task_714[arg1] = arg3;
  script_30F16(arg1, arg2, arg4);
  for (;;) {
    if (task_724[arg1]) {
      task_734[arg1] = Script_walk3D(ActivateGetIdx, arg0, task_744[arg1]);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_30F16(int32_t arg0, float arg1, const void *arg2) {
  int32_t local_FC;
  task_724[arg0] = 0;
  task_734[arg0] = 0;
  task_744[arg0] = arg1;
  local_FC = task_714[arg0];
  if ((local_FC == 0) | (local_FC == 1)) {
    Script_PlayCharaMotion(task_704[arg0], MN_AKI_AKI_L_STAND, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  } else {
    if (local_FC == 2) {
      Script_PlayCharaMotion(task_704[arg0], MN_AKI_AKI_L_STAND, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      goto L030FB7;
    }
  }
  L030FB7:;
  Script_walk3D(SetupPath, task_704[arg0], arg2, 0);
  return;
}

static void script_30FD0(int32_t arg0, float arg1) {
  int32_t local_FC;
  task_744[arg0] = arg1;
  task_724[arg0] = 1;
  local_FC = task_714[arg0];
  switch (local_FC) {
    case 0:
      Script_charaToggleMotEnabled(task_704[arg0], 0);
      Script_SetCharaMotionPlaybackRate(task_704[arg0], 1.0f);
      Script_PlayCharaMotion(task_704[arg0], MN_AKI_AKI_WALK_UP1_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      break;
    case 1:
      Script_motmToggleFlags(task_704[arg0], SCEN_Enable, 4);
      Script_charaToggleMotEnabled(task_704[arg0], 0);
      Script_SetCharaMotionPlaybackRate(task_704[arg0], 1.0f);
      Script_PlayCharaMotion(task_704[arg0], 22290, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      break;
    case 2:
      Script_charaToggleMotEnabled(task_704[arg0], 0);
      Script_SetCharaMotionPlaybackRate(task_704[arg0], 0.5f);
      Script_PlayCharaMotion(task_704[arg0], MN_AKI_AKI_WALK_SLOW_K_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      break;
  }
  return;
}

static void script_3110F(int32_t arg0) {
  int32_t local_FC;
  task_724[arg0] = 0;
  local_FC = task_714[arg0];
  switch (local_FC) {
    case 0:
      Script_charaToggleMotEnabled(task_704[arg0], 1);
      Script_SetCharaMotionPlaybackRate(task_704[arg0], 1.0f);
      break;
    case 1:
      Script_motmToggleFlags(task_704[arg0], SCEN_Disable, 4);
      Script_charaToggleMotEnabled(task_704[arg0], 1);
      Script_SetCharaMotionPlaybackRate(task_704[arg0], 1.0f);
      break;
    case 2:
      Script_charaToggleMotEnabled(task_704[arg0], 1);
      Script_SetCharaMotionPlaybackRate(task_704[arg0], 1.0f);
      break;
  }
  return;
}

static void script_311CC(void) {
  Script_scenCreateTask(script_31228, 2, 0, -0x80000000);
  Script_shenSetLightingMode(2);
  Script_lightSetGlobalSceneLighting(&data_43A54);
  Script_SetFogFlags(0, data_43A94);
  Script_SetFogColour(&data_43A74);
  (void)1;
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 25.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 25.0f);
  for (;;) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_31228(int32_t arg0, int32_t arg1) {
  Vector4 local_F0;
  Vector3 local_E4;
  float local_E0;
  int32_t local_DC;
  local_DC = arg0 * 14;
  local_F0.x = *(float *)(&data_4392C + local_DC);
  (&local_F0)->y = *(float *)(&data_4392C + (local_DC + 1));
  (&local_F0)->z = *(float *)(&data_4392C + (local_DC + 2));
  (&local_F0)->w = *(float *)(&data_4392C + (local_DC + 3));
  local_E4.x = *(float *)(&data_4392C + (local_DC + 10));
  (&local_E4)->y = *(float *)(&data_4392C + (local_DC + 11));
  (&local_E4)->z = *(float *)(&data_4392C + (local_DC + 12));
  local_E0 = *(float *)(&data_4392C + (local_DC + 9));
  Script_LightUtils(arg0, 0, 2);
  Script_LightUtils(arg0, 2, &local_F0);
  Script_LightUtils(arg0, 8, &local_E4);
  Script_LightUtils(arg0, 4, local_E0);
  if (arg1 & -0x80000000) {
    Script_lightSetSlotEnabled(arg0, 1);
  }
  if (arg1 & 0x40000000) {
    Script_lightSetSlotEnabled(arg0, 0);
  }
  for (;;) {
    if (arg1 & 0x20000000) {
      if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.1f)) {
        Script_lightSetSlotEnabled(arg0, 0);
      } else {
        Script_lightSetSlotEnabled(arg0, 1);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_31368(int32_t arg0, int32_t arg1) {
  Vector4 local_F0;
  Vector3 local_E4;
  int32_t local_DC[2];
  float local_D8;
  float local_D4;
  int32_t local_D0;
  local_D0 = arg0 * 14;
  local_F0.x = *(float *)(&data_4392C + local_D0);
  (&local_F0)->y = *(float *)(&data_4392C + (local_D0 + 1));
  (&local_F0)->z = *(float *)(&data_4392C + (local_D0 + 2));
  (&local_F0)->w = *(float *)(&data_4392C + (local_D0 + 3));
  local_E4.x = *(float *)(&data_4392C + (local_D0 + 4));
  (&local_E4)->y = *(float *)(&data_4392C + (local_D0 + 5));
  (&local_E4)->z = *(float *)(&data_4392C + (local_D0 + 6));
  local_D8 = *(float *)(&data_4392C + (local_D0 + 7));
  local_DC[0] = *(&data_4392C + (local_D0 + 8));
  local_DC[1] = 0;
  local_D4 = *(float *)(&data_4392C + (local_D0 + 9));
  Script_LightUtils(arg0, 0, 3);
  Script_LightUtils(arg0, 2, &local_F0);
  Script_LightUtils(arg0, 7, &local_E4);
  Script_LightUtils(arg0, 5, local_D8);
  Script_LightUtils(arg0, 6, (const float *)local_DC);
  Script_LightUtils(arg0, 4, local_D4);
  if (arg1 & -0x80000000) {
    Script_lightSetSlotEnabled(arg0, 1);
  }
  if (arg1 & 0x40000000) {
    Script_lightSetSlotEnabled(arg0, 0);
  }
  for (;;) {
    if (arg1 & 0x20000000) {
      if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.1f)) {
        Script_lightSetSlotEnabled(arg0, 0);
      } else {
        Script_lightSetSlotEnabled(arg0, 1);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_314EB(int32_t arg0, int32_t arg1) {
  Vector4 local_F0;
  Vector3 local_E4;
  Vector3 local_D8;
  int32_t local_D0[2];
  float local_CC;
  int32_t local_C8;
  float local_C4;
  int32_t local_C0;
  local_C0 = arg0 * 14;
  local_F0.x = *(float *)(&data_4392C + local_C0);
  (&local_F0)->y = *(float *)(&data_4392C + (local_C0 + 1));
  (&local_F0)->z = *(float *)(&data_4392C + (local_C0 + 2));
  (&local_F0)->w = *(float *)(&data_4392C + (local_C0 + 3));
  local_E4.x = *(float *)(&data_4392C + (local_C0 + 4));
  (&local_E4)->y = *(float *)(&data_4392C + (local_C0 + 5));
  (&local_E4)->z = *(float *)(&data_4392C + (local_C0 + 6));
  local_CC = *(float *)(&data_4392C + (local_C0 + 7));
  local_D0[0] = *(&data_4392C + (local_C0 + 8));
  local_D0[1] = 0;
  local_C4 = *(float *)(&data_4392C + (local_C0 + 9));
  local_D8.x = *(float *)(&data_4392C + (local_C0 + 10));
  (&local_D8)->y = *(float *)(&data_4392C + (local_C0 + 11));
  (&local_D8)->z = *(float *)(&data_4392C + (local_C0 + 12));
  local_C8 = *(&data_4392C + (local_C0 + 13));
  Script_LightUtils(arg0, 0, 4);
  Script_LightUtils(arg0, 2, &local_F0);
  Script_LightUtils(arg0, 7, &local_E4);
  Script_LightUtils(arg0, 5, local_CC);
  Script_LightUtils(arg0, 6, (const float *)local_D0);
  Script_LightUtils(arg0, 8, &local_D8);
  Script_LightUtils(arg0, 10, local_C8);
  Script_LightUtils(arg0, 4, local_C4);
  if (arg1 & -0x80000000) {
    Script_lightSetSlotEnabled(arg0, 1);
  }
  if (arg1 & 0x40000000) {
    Script_lightSetSlotEnabled(arg0, 0);
  }
  for (;;) {
    if (arg1 & 0x20000000) {
      if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.1f)) {
        Script_lightSetSlotEnabled(arg0, 0);
      } else {
        Script_lightSetSlotEnabled(arg0, 1);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_316D4(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t unused_F0;
  GameDateTime local_E8;
  Script_ReturnTimeDate(&local_F4);
  Script_ReturnTimeDate(&local_E8);
  (&local_F4)->hours = 22;
  (&local_F4)->minutes = 0;
  (&local_E8)->hours = 23;
  (&local_E8)->minutes = 0;
  Script_SpriteUtils(Scroll_Read1, (int32_t)"scroll", "SCROLL07.SPR");
  Script_SetNewDateTimeWrapper(&local_F4, &local_E8, 30);
  Script_PreserveAccumulator_00ED(1);
  Script_PreserveAccumulator_011D(0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_3173B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_31755, 6, arg1, arg5, arg2, arg3, arg0, arg4);
  return;
}

static int32_t script_31755(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  if (arg5 == 3) {
    Script_motmToggleFlags(arg4, SCEN_Disable, 0x40);
  } else {
    Script_motmToggleFlags(arg4, SCEN_Enable, 0x40);
  }
  if (arg5 == 2) {
    Script_charaToggleMotEnabled(arg4, 0);
  } else {
    Script_charaToggleMotEnabled(arg4, 1);
  }
  Script_PlayCharaMotion(arg4, arg0, arg2, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(arg4, arg3);
  if (arg5 == 1) {
    Script_motmToggleFlags(arg4, SCEN_Enable, 0x80);
  }
  return Script_srchControl(0);
}

static void script_317E2(int32_t arg0, int32_t arg1) {
  task_754 = arg0;
  task_550 = 0;
  return;
}

__attribute__((scn_string_bindings(0, data_43E44, 1, data_43E4B, 2, data_43E52, 3, data_43E59, 4, data_43E60, 5, data_43E67, 6, data_43E6E, 7, data_43E75, 8, data_43E7C, 9, data_43E83, 10, data_43E8A, 11, data_43E91, 12, data_43E98, 13, data_43E9F, 14, data_43EA6, 15, data_43EAD, 16, data_43EB4, 17, data_43EBB, 18, data_43EC2, 19, data_43EC9, 20, data_43ED0, 21, data_43ED7, 22, data_43EDE, 23, data_43EE5, 24, data_43EEC, 25, data_43EF3, 26, data_43EFA, 27, data_43F01, 28, data_43F08, 29, data_43F0F, 30, data_43F16, 31, data_43F1F, 32, data_43F28, 33, data_43F31, 34, data_43F3A, 35, data_43F43, 36, data_43F4C, 37, data_43F55, 38, data_43F5E, 39, data_43F67, 40, data_43F70, 41, data_43F79)))
static void script_317F3(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      SCNT_ShowSubtitle("開始０");
      break;
    case 1:
      SCNT_ShowSubtitle("成功０");
      break;
    case 2:
      SCNT_ShowSubtitle("失敗０");
      break;
    case 3:
      SCNT_ShowSubtitle("開始１");
      break;
    case 4:
      SCNT_ShowSubtitle("成功１");
      break;
    case 5:
      SCNT_ShowSubtitle("失敗１");
      break;
    case 6:
      SCNT_ShowSubtitle("開始２");
      break;
    case 7:
      SCNT_ShowSubtitle("成功２");
      break;
    case 8:
      SCNT_ShowSubtitle("失敗２");
      break;
    case 9:
      SCNT_ShowSubtitle("開始３");
      break;
    case 10:
      SCNT_ShowSubtitle("成功３");
      break;
    case 11:
      SCNT_ShowSubtitle("失敗３");
      break;
    case 12:
      SCNT_ShowSubtitle("開始４");
      break;
    case 13:
      SCNT_ShowSubtitle("成功４");
      break;
    case 14:
      SCNT_ShowSubtitle("失敗４");
      break;
    case 15:
      SCNT_ShowSubtitle("開始５");
      break;
    case 16:
      SCNT_ShowSubtitle("成功５");
      break;
    case 17:
      SCNT_ShowSubtitle("失敗５");
      break;
    case 18:
      SCNT_ShowSubtitle("開始６");
      break;
    case 19:
      SCNT_ShowSubtitle("成功６");
      break;
    case 20:
      SCNT_ShowSubtitle("失敗６");
      break;
    case 21:
      SCNT_ShowSubtitle("開始７");
      break;
    case 22:
      SCNT_ShowSubtitle("成功７");
      break;
    case 23:
      SCNT_ShowSubtitle("失敗７");
      break;
    case 24:
      SCNT_ShowSubtitle("開始８");
      break;
    case 25:
      SCNT_ShowSubtitle("成功８");
      break;
    case 26:
      SCNT_ShowSubtitle("失敗８");
      break;
    case 27:
      SCNT_ShowSubtitle("開始９");
      break;
    case 28:
      SCNT_ShowSubtitle("成功９");
      break;
    case 29:
      SCNT_ShowSubtitle("失敗９");
      break;
    case 30:
      SCNT_ShowSubtitle("開始１０");
      break;
    case 31:
      SCNT_ShowSubtitle("成功１０");
      break;
    case 32:
      SCNT_ShowSubtitle("失敗１０");
      break;
    case 33:
      SCNT_ShowSubtitle("開始１１");
      break;
    case 34:
      SCNT_ShowSubtitle("成功１１");
      break;
    case 35:
      SCNT_ShowSubtitle("失敗１１");
      break;
    case 36:
      SCNT_ShowSubtitle("開始１２");
      break;
    case 37:
      SCNT_ShowSubtitle("成功１２");
      break;
    case 38:
      SCNT_ShowSubtitle("失敗１２");
      break;
    case 39:
      SCNT_ShowSubtitle("開始１３");
      break;
    case 40:
      SCNT_ShowSubtitle("成功１３");
      break;
    case 41:
      SCNT_ShowSubtitle("失敗１３");
      break;
  }
  Script_srchControl(0);
  return;
}

static void script_31ADD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_31AF3, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_31AF3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (arg1 == -1) {
    Script_seqEnterEvtCam(arg0, 0, AUTH_None);
  } else {
    Script_seqEnterEvtCam(arg0, arg1, arg2);
  }
  return Script_srchControl(0);
}

static void script_31B2D(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_31B3F, 2, arg0, arg1);
  return;
}

static int32_t script_31B3F(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_StartOrFreeSEQ(0);
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      Script_ShowUIMessageFromID("E1036A001");
      break;
    case 1:
      Script_ShowUIMessageFromID("E1036A002");
      break;
    case 2:
      Script_ShowUIMessageFromID("E1036A003");
      break;
    case 3:
      Script_ShowUIMessageFromID("E1036A004");
      break;
    case 4:
      Script_ShowUIMessageFromID("E1036A005");
      break;
    case 5:
      Script_ShowUIMessageFromID("E1036A006");
      break;
    case 6:
      Script_ShowUIMessageFromID("E1036A007");
      break;
    case 7:
      Script_ShowUIMessageFromID("E1036A008");
      break;
    case 8:
      Script_ShowUIMessageFromID("E1036A009");
      break;
    case 9:
      Script_ShowUIMessageFromID("E1036A010");
      break;
    case 10:
      Script_ShowUIMessageFromID("E1036A011");
      break;
    case 11:
      Script_ShowUIMessageFromID("E1036A012");
      break;
    case 12:
      Script_ShowUIMessageFromID("E1036B001");
      break;
    case 13:
      Script_ShowUIMessageFromID("E1036B002");
      break;
    case 14:
      Script_ShowUIMessageFromID("E1036C001");
      break;
    case 15:
      Script_ShowUIMessageFromID("E1036C002");
      break;
    case 16:
      Script_ShowUIMessageFromID("E1036C003");
      break;
    case 17:
      Script_ShowUIMessageFromID("E1036C004");
      break;
    case 18:
      Script_ShowUIMessageFromID("E1036C005");
      break;
    case 19:
      Script_ShowUIMessageFromID("E1036D001");
      break;
    case 20:
      Script_ShowUIMessageFromID("E1036D002");
      break;
    case 21:
      Script_ShowUIMessageFromID("E1036D003");
      break;
    case 22:
      Script_ShowUIMessageFromID("E1036D004");
      break;
    case 23:
      Script_ShowUIMessageFromID("E1036D005");
      break;
    case 24:
      Script_ShowUIMessageFromID("E1036D006");
      break;
    case 25:
      Script_ShowUIMessageFromID("E1036D007");
      break;
    case 26:
      Script_ShowUIMessageFromID("E1036D008");
      break;
    case 27:
      Script_ShowUIMessageFromID("E1036D009");
      break;
    case 28:
      Script_ShowUIMessageFromID("E1036E001");
      break;
  }
  return Script_srchControl(0);
}

static void script_31DC7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_31DDD, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_31DDD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_EVT_GetPosition(arg1, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(arg0, arg2, &local_F4, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookAtPosition, &local_F4, 5);
  return Script_srchControl(0);
}

static void script_31E20(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_31E34, 3, arg0, arg1, arg2);
  return;
}

static int32_t script_31E34(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t unused_F4[3];
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  Script_SetCharaLookAtPosition(arg0, -arg1, 0, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  return Script_srchControl(0);
}

static void script_31E66(int32_t arg0, int32_t arg1) {
  Script_SetCharaLookAtPosition(arg0, -arg1, 0, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  return;
}

static void script_31E81(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_31E99, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_31E99(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  Script_FACE(arg0, arg1, arg2, arg3);
  return Script_srchControl(0);
}

static void script_31EC0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_31EDA, 6, arg0, arg1, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_31EDA(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4, int32_t arg5) {
  Vector3 local_F4;
  int32_t local_F0;
  if (arg5 >= 0) {
    SCNF_WaitFrames_0000(arg5);
  }
  local_F4.x = 1.0f;
  (&local_F4)->y = 1.0f;
  (&local_F4)->z = 1.0f;
  local_F0 = 0;
  while (local_F0 < arg4) {
    local_F4.x = (&local_F4)->x + arg1;
    (&local_F4)->y = (&local_F4)->y + arg2;
    (&local_F4)->z = (&local_F4)->z + arg3;
    Script_hmdlApplyScale(arg0, &local_F4);
    local_F0 = local_F0 + 1;
  }
  return Script_srchControl(0);
}

static int32_t script_31F62(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t unused_F8;
  int32_t local_EC[3];
  int32_t unused_E8;
  GameDateTime local_E0;
  int32_t local_DC;
  int32_t local_D8;
  SCNF_WaitFrames_0000(5);
  Script_SetQteOverlayTimeoutTuning(0, QTE_TimeoutSession);
  local_EC[0] = Script_scenCreateTask(script_30EA9, 5, arg0, arg1, 0x3D343958, 0, &data_43AC8);
  local_EC[1] = Script_scenCreateTask(script_30EA9, 5, task_5C8[1], 1, 0x3D343958, 1, &data_43AF8);
  local_EC[2] = Script_scenCreateTask(script_30EA9, 5, task_5C8[2], 2, 0x3D343958, 1, &data_43AE0);
  script_3324F();
  for (;;) {
    local_FC = Script_scenCreateTask(script_322AB, 2, arg0, arg1);
    task_61C = 0;
    for (;;) {
      if (task_61C >= 12) {
        Script_srchControl((void *)local_FC);
        local_D8 = task_754;
        switch (local_D8) {
          case 11:
            script_33D04(arg0, arg1);
            break;
          case 20:
            script_340FF(arg0, arg1);
            break;
        }
        Script_SetQteOverlayTimeoutTuning(2, QTE_TimeoutSession);
        Script_uiUtils(0, 0, 0);
        Script_srchControl((void *)*(int32_t *)local_EC);
        Script_srchControl((void *)local_EC[1]);
        Script_srchControl((void *)local_EC[2]);
        Script_executeAicaCommand(2720, 1, 30);
        Script_executeAicaCommand(2720, 2, 30);
        Script_executeAicaCommand(2720, 3, 30);
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_ReturnTimeDate(&local_E0);
        Script_PlayerStatsUtils(PS_Set70MBTimeCapSlot, 8, 255);
        local_DC = Script_PlayerStatsUtils(PS_Get70MBCounter1);
        Script_PlayerStatsUtils(PS_Set70MBCounter1, local_DC + 1);
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_HISA_QTE_RESOLVED, 1);
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_WHAT_HAPPENED_TO_MAI, 1);
        Script_shenRequestGameModuleTransition(2, SHEN_NAME('JOMO'), 19);
        script_3474E(task_554);
        SCNF_WaitFrames_0000(1);
        Script_srchControl((void *)local_FC);
        Script_SetQteOverlayTimeoutTuning(1, QTE_TimeoutSession);
        break;
      }
      if ((task_61C == -1) & (task_554 == 1)) {
        Script_PlayerStatsUtils(PS_Set70MBTimeCapSlot, 8, 1);
        script_307F0();
        Script_SetQteOverlayTimeoutTuning(2, QTE_TimeoutSession);
        Script_uiUtils(0, 0, 0);
        Script_srchControl((void *)*(int32_t *)local_EC);
        Script_srchControl((void *)local_EC[1]);
        Script_srchControl((void *)local_EC[2]);
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_REPORT_MAI_TO_HISAKA, 1);
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_HISA_QTE_RESOLVED, 1);
        Script_srchControl((void *)local_FC);
        SCNF_WaitFrames_0000(1);
        script_3474E(task_554);
        break;
      } else {
        if (task_61C == -1) {
          Script_SetQteOverlayTimeoutTuning(2, QTE_TimeoutSession);
          Script_uiUtils(0, 0, 0);
          Script_srchControl((void *)local_FC);
          break;
        }
      }
      SCNF_WaitFrames_0000(1);
    }
    Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    if (task_554 == 0) {
      for (;;) {
        Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 255, 0, 0, 0);
        Script_PrintDebugString(18, 14, data_440A4);
        if (Script_GetInputData(0, 2) & 0x400) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    }
    Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  }
  end:;
}

static int32_t script_322AB(int32_t arg0, int32_t arg1) {
  int32_t unused_FC;
  SCNF_WaitFrames_0000(2);
  script_334F9(arg0, arg1);
  Script_scenCreateTask(script_33C74, 0);
  script_31DC7(arg0, CHARA_NMNO, 1, -1);
  script_31DC7(CHARA_ISYM, CHARA_AKIR, 1, -1);
  script_31DC7(CHARA_NMNO, CHARA_AKIR, 1, -1);
  script_3173B(CHARA_ISYM, 22303, 0, 0x3F800000, -1, -1);
  script_31B2D(14, -1);
  script_31B2D(19, 90);
  script_31DC7(CHARA_NMNO, arg0, 1, 110);
  SCNF_WaitFrames_0000(45);
  Script_clmdSetFlags(CHARA_ISYM, 4, 0);
  script_2FB8B(CHARA_ISYM);
  script_3173B(CHARA_ISYM, 22302, 0, 0x3F800000, -1, -1);
  SCNF_WaitFrames_0000(30);
  script_30F16(2, 0.044f, &data_43AE0);
  script_30FD0(2, 0.044f);
  while (task_734[2] != 1) {
    SCNF_WaitFrames_0000(1);
  }
  script_3110F(2);
  script_3173B(CHARA_ISYM, 22302, 0, 0x3F800000, -1, -1);
  script_31ADD(8005, SHEN_NAME('DMY0'), SHEN_NAME('DMY0'), -1);
  script_30240(arg0, -1, 136.1f, 53.4f, 160.0f);
  script_30240(task_5C8[2], arg0, -0.9f, -1.0f, 240.0f);
  script_30240(CHARA_HINA, -1, 134.8f, 56.2f, 0.0f);
  script_31DC7(CHARA_ISYM, arg0, 1, -1);
  script_31E20(CHARA_AKIR, 1, -1);
  script_31E20(CHARA_HINA, 2, -1);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_SetCharaFlags(arg0, 0);
  script_31B2D(15, 5);
  script_317E2(3, -1);
  script_2FE35(arg1, 0, 2, 0);
  if (task_610 == 0) {
    script_317E2(5, 10);
    script_31ADD(8033, -1, -1, -1);
    script_303C7(arg0, -1, 120.0f);
    script_30240(task_5C8[2], arg0, -0.6f, -0.1f, 250.0f);
    script_3173B(CHARA_HINA, 22319, 0, 0x3F800000, -1, 10);
    script_31E20(CHARA_AKIR, 1, -1);
    script_31E20(CHARA_ISYM, 2, -1);
    SCNF_WaitFrames_0000(50);
  } else {
    script_317E2(4, 10);
    script_31ADD(8006, -1, -1, -1);
    script_31DC7(arg0, CHARA_ISYM, 1, 20);
    script_31E20(CHARA_AKIR, 1, -1);
    script_31E20(CHARA_ISYM, 2, -1);
    script_31E20(CHARA_AKIR, 1, 70);
    script_31B2D(2, 30);
    script_31E81(arg0, 0, -1, 7, 70);
    SCNF_WaitFrames_0000(80);
  }
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_43C7C, 8);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_43C7C, 8);
  script_30240(arg0, -1, 136.1f, 53.4f, 340.0f);
  script_30240(task_5C8[2], arg0, 0.0f, -1.4f, 0.0f);
  script_3041B(task_5C8[2], arg0, 0x43340000, 12);
  script_31B2D(16, -1);
  script_3173B(CHARA_AKIR, 22273, 0, 0x3F800000, -1, -1);
  script_31E81(arg0, 0, -1, 7, -1);
  Script_charaDisplay(CHARA_NMNO, CDM_Hide);
  script_317E2(6, -1);
  script_31ADD(8007, -1, -1, -1);
  script_2FE35(arg1, 1, 2, 0);
  if (task_610 == 0) {
    Script_charaDisplay(CHARA_NMNO, CDM_Show);
    script_317E2(8, 20);
    script_317E2(9, 45);
    script_30302(task_5C8[2], arg0, 0x3E4CCCCD, -0x40B33333, 0x43340000, -1);
    script_30302(task_5C8[2], arg0, -0x42333333, -0x40733333, 0x43340000, 45);
    script_3041B(CHARA_HINA, -1, 0x43870000, 45);
    script_31ADD(8015, -1, -1, -1);
    script_31ADD(8017, -1, -1, 45);
    script_3173B(CHARA_HINA, 22319, 0, 0x3F800000, -1, 10);
    script_31B2D(17, -1);
    script_31B2D(7, 25);
    script_31E81(arg0, 0, -1, 7, 45);
    SCNF_WaitFrames_0000(45);
    script_2FE35(arg1, 1, 2, 0);
    if (task_610 == 0) {
      script_317E2(11, -1);
      script_31ADD(8019, -1, -1, -1);
      script_30240(task_5C8[2], arg0, 0.0f, -0.8f, 180.0f);
      script_3041B(CHARA_NMNO, -1, 0x43660000, 105);
      script_31B2D(17, -1);
      script_31B2D(7, 25);
      script_3173B(CHARA_HINA, 22319, 0, 0x3F800000, -1, -1);
      script_30F16(1, 0.044f, &data_43AE0);
      script_30FD0(1, 0.044f);
      while (task_734[1] != 1) {
        SCNF_WaitFrames_0000(1);
      }
      script_3110F(1);
      script_31E20(CHARA_NMNO, 2, -1);
      script_3173B(CHARA_NMNO, 22312, 0, 0x3F800000, -1, -1);
      SCNF_WaitFrames_0000(5);
      task_61C = 20;
      SCNF_WaitFrames_0000(1);
    } else {
      script_317E2(10, 10);
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_43B98, 8);
      script_31ADD(8021, -1, -1, -1);
      script_3041B(CHARA_HINA, -1, 0x43870000, -1);
      script_3173B(CHARA_HINA, 22320, 0, 0x3F800000, -1, -1);
      script_31E20(CHARA_NMNO, 2, -1);
      script_31B2D(20, 30);
      SCNF_WaitFrames_0000(75);
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_43C7C, 8);
    }
  } else {
    script_317E2(7, 10);
    Script_handSetFloatsFor(CHARA_AKIR, 1, data_43B98, 8);
    script_3041B(CHARA_HINA, -1, 0x43870000, -1);
    script_3173B(CHARA_HINA, 22320, 0, 0x3F800000, -1, -1);
    script_31E20(CHARA_NMNO, 2, -1);
    script_31B2D(20, 30);
    SCNF_WaitFrames_0000(75);
    Script_handSetFloatsFor(CHARA_AKIR, 1, data_43C7C, 8);
  }
  Script_charaDisplay(CHARA_NMNO, CDM_Show);
  script_31E20(CHARA_NMNO, 2, -1);
  script_30F16(1, 0.044f, &data_43AE0);
  script_30FD0(1, 0.044f);
  while (task_734[1] != 1) {
    SCNF_WaitFrames_0000(1);
  }
  script_3110F(1);
  script_33162();
  Script_SetCharaFlags(arg0, 4);
  script_31ADD(8008, SHEN_NAME('DMY0'), SHEN_NAME('DMY0'), -1);
  script_30302(task_5C8[1], arg0, 0x3E2E147B, -0x405AE148, 0x42B40000, -1);
  script_30302(arg0, -1, 0x4308199A, 0x4255999A, 0x43520000, -1);
  script_3173B(CHARA_NMNO, 22313, 0, 0x3F800000, -1, -1);
  script_3173B(CHARA_ISYM, 22291, 0, 0x3F800000, -1, -1);
  script_317E2(12, -1);
  script_2FE35(arg1, 2, 1, 0);
  if (task_610 == 0) {
    script_317E2(14, 10);
    script_317E2(15, 30);
    script_31ADD(8022, -1, -1, -1);
    script_31ADD(8025, -1, -1, 30);
    script_30302(task_5C8[1], arg0, 0x3F19999A, -0x40800000, 0x42B40000, -1);
    script_30302(task_5C8[1], arg0, 0x3E2E147B, -0x405AE148, 0x42B40000, 30);
    script_30302(arg0, -1, 0x4308199A, 0x4255999A, 0x43520000, 30);
    script_3173B(CHARA_HINA, 22319, 0, 0x3F800000, -1, 30);
    script_3041B(CHARA_HINA, -1, 0x435C0000, 30);
    SCNF_WaitFrames_0000(30);
    script_2FE35(arg1, 3, 1, 0);
    if (task_610 == 0) {
      script_317E2(17, 10);
      script_317E2(18, 30);
      script_31ADD(8034, -1, -1, -1);
      script_31ADD(8026, -1, -1, 30);
      script_30302(task_5C8[1], arg0, 0x3F19999A, -0x40800000, 0x42B40000, -1);
      script_30302(task_5C8[1], arg0, 0x3E2E147B, -0x405AE148, 0x42B40000, 30);
      script_30302(arg0, -1, 0x4308199A, 0x4255999A, 0x43520000, 30);
      script_3173B(CHARA_HINA, 22319, 0, 0x3F800000, -1, 30);
      script_3041B(CHARA_HINA, -1, 0x435C0000, 30);
      SCNF_WaitFrames_0000(30);
      script_2FE35(arg1, 4, 1, 0);
      if (task_610 == 0) {
        script_317E2(20, 10);
        script_31ADD(8035, -1, -1, -1);
        script_30302(task_5C8[1], arg0, 0x3F000000, -0x40666666, 0x42B40000, -1);
        script_30302(CHARA_ISYM, -1, 0x43090000, 0x42526666, 0x42340000, -1);
        script_3173B(CHARA_HINA, 22319, 0, 0x3F800000, -1, 30);
        script_3041B(CHARA_HINA, -1, 0x435C0000, 30);
        SCNF_WaitFrames_0000(45);
        task_61C = 20;
        SCNF_WaitFrames_0000(1);
      } else {
        script_317E2(19, -1);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_43D60, 8);
        script_31ADD(8009, -1, -1, 10);
        script_31ADD(8010, SHEN_NAME('DMY0'), SHEN_NAME('DMY0'), 235);
        script_30302(arg0, -1, 0x4308B5C3, 0x42533D71, 0x43660000, 235);
        script_30302(CHARA_HINA, -1, 0x43070000, 0x425C6666, 0x43A28000, 235);
        script_31B2D(21, 70);
        script_31B2D(3, 120);
        script_31B2D(22, 180);
        script_3173B(CHARA_HINA, 22321, 0, 0x3F800000, -1, 150);
        script_3173B(CHARA_NMNO, 22315, 0, 0x3F800000, -1, 300);
        script_3041B(CHARA_HINA, -1, 0x43A78000, 150);
        SCNF_WaitFrames_0000(95);
        script_331DF();
        SCNF_WaitFrames_0000(55);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_43B98, 8);
        Script_scenCreateTask(script_33335, 0);
        SCNF_WaitFrames_0000(50);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_43C7C, 8);
        SCNF_WaitFrames_0000(100);
      }
    } else {
      script_317E2(16, -1);
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_43D60, 8);
      script_31ADD(8009, -1, -1, 10);
      script_31ADD(8010, SHEN_NAME('DMY0'), SHEN_NAME('DMY0'), 235);
      script_30302(arg0, -1, 0x4308B5C3, 0x42533D71, 0x43660000, 235);
      script_30302(CHARA_HINA, -1, 0x43070000, 0x425C6666, 0x43A28000, 235);
      script_31B2D(21, 70);
      script_31B2D(3, 120);
      script_31B2D(22, 180);
      script_3173B(CHARA_HINA, 22321, 0, 0x3F800000, -1, 150);
      script_3041B(CHARA_HINA, -1, 0x43A78000, 150);
      script_3173B(CHARA_NMNO, 22315, 0, 0x3F800000, -1, 300);
      SCNF_WaitFrames_0000(95);
      script_331DF();
      SCNF_WaitFrames_0000(55);
      Script_scenCreateTask(script_33335, 0);
      Script_handSetFloatsFor(CHARA_AKIR, 0, data_43B98, 8);
      SCNF_WaitFrames_0000(50);
      Script_handSetFloatsFor(CHARA_AKIR, 0, data_43C7C, 8);
      SCNF_WaitFrames_0000(100);
    }
  } else {
    script_317E2(13, 10);
    Script_handSetFloatsFor(CHARA_AKIR, 1, data_43D60, 8);
    script_31ADD(8009, -1, -1, 10);
    script_31ADD(8010, SHEN_NAME('DMY0'), SHEN_NAME('DMY0'), 235);
    script_30302(arg0, -1, 0x4308B5C3, 0x42533D71, 0x43660000, 235);
    script_30302(CHARA_HINA, -1, 0x43070000, 0x425C6666, 0x43A28000, 235);
    script_31B2D(21, 70);
    script_31B2D(3, 120);
    script_31B2D(22, 180);
    script_3173B(CHARA_HINA, 22321, 0, 0x3F800000, -1, 150);
    script_3041B(CHARA_HINA, -1, 0x43A78000, 150);
    script_3173B(CHARA_NMNO, 22315, 0, 0x3F800000, -1, 300);
    SCNF_WaitFrames_0000(95);
    script_331DF();
    SCNF_WaitFrames_0000(55);
    Script_scenCreateTask(script_33335, 0);
    Script_handSetFloatsFor(CHARA_AKIR, 0, data_43B98, 8);
    SCNF_WaitFrames_0000(50);
    Script_handSetFloatsFor(CHARA_AKIR, 0, data_43C7C, 8);
    SCNF_WaitFrames_0000(100);
  }
  script_3394F(arg0, arg1);
  task_61C = -1;
  SCNF_WaitFrames_0000(30);
  end:;
}

static void script_33162(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_CharHandlers(CHARA_NMNO, 0, 0, 16);
  local_E8.x = 0;
  (&local_E8)->y = 28763;
  (&local_E8)->z = 13817;
  local_E8.x = 0;
  (&local_E8)->y = 28763;
  (&local_E8)->z = 12906;
  local_F4.x = 0.034f;
  (&local_F4)->y = 0.015f;
  (&local_F4)->z = -0.041f;
  Script_fixoAttachEx(SHEN_NAME('KAKU'), CHARA_NMNO, 18, &local_F4, &local_E8);
  return;
}

static void script_331DF(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_fixoClearAttachment(SHEN_NAME('KAKU'));
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_43C7C, 8);
  local_E8.x = 0;
  (&local_E8)->y = 43326;
  (&local_E8)->z = 7736;
  local_F4.x = -0.011f;
  (&local_F4)->y = -0.089f;
  (&local_F4)->z = 0.186f;
  Script_fixoAttachEx(SHEN_NAME('KAKU'), CHARA_AKIR, 18, &local_F4, &local_E8);
  return;
}

static void script_3324F(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_CharHandlers(CHARA_ISYM, 0, 0, 16);
  Script_CharHandlers(CHARA_ISYM, 1, -1, 16);
  local_E8.x = 0;
  (&local_E8)->y = 28763;
  (&local_E8)->z = 12906;
  local_F4.x = 0.081f;
  (&local_F4)->y = 0.054f;
  (&local_F4)->z = -0.036f;
  Script_fixoAttachEx(SHEN_NAME('SORI'), CHARA_ISYM, 18, &local_F4, &local_E8);
  return;
}

static void script_332C2(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_CharHandlers(CHARA_ISYM, 1, 0, 16);
  Script_CharHandlers(CHARA_ISYM, 0, -1, 16);
  local_E8.x = 0;
  (&local_E8)->y = 28763;
  (&local_E8)->z = 12906;
  local_F4.x = 0.081f;
  (&local_F4)->y = 0.054f;
  (&local_F4)->z = 0.031f;
  Script_fixoAttachEx(SHEN_NAME('SORI'), CHARA_ISYM, 12, &local_F4, &local_E8);
  return;
}

static int32_t script_33335(void) {
  Script_fixoClearAttachment(SHEN_NAME('KAKU'));
  {
    Vector3 local_F4;
    int32_t unused_E8[3];
    int8_t local_E7;
    Script_EVT_GetPosition(SHEN_NAME('KAKU'), -1, &local_F4, 0);
    local_E7 = 0;
    for (;;) {
      local_F4.x = (&local_F4)->x + 0.02f;
      (&local_F4)->z = (&local_F4)->z + 0.02f;
      (&local_F4)->y = (&local_F4)->y - 0.05f - (float)local_E7 * 0.008f;
      if (!((&local_F4)->y >= 0.2f)) {
        (&local_F4)->y = 0.2f;
      }
      Script_EVT_SetPosition(SHEN_NAME('KAKU'), POS_XYZ, &local_F4);
      local_E7 = local_E7 + 1;
      if (((scn3_bits){.f = (&local_F4)->y}).i == 0x3E4CCCCD) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_scenCreateTask(script_2F8DE, 2, 197801, -1);
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    int8_t local_E7;
    local_E7 = 0;
    Script_mtwkGetSetRotation(SHEN_NAME('KAKU'), &local_E8, 0);
    while (local_E7 < 5) {
      (&local_E8)->y = (&local_E8)->y - 691;
      (&local_E8)->z = (&local_E8)->z - 3342;
      Script_EVT_SetRotation(SHEN_NAME('KAKU'), 0x38000000, &local_E8, 0);
      {
        local_E7 = local_E7 + 1;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_scenCreateTask(script_2F8DE, 2, 132265, -1);
  {
    Vector3 local_F4;
    Vector3i local_E8;
    int8_t local_E7;
    local_E7 = 0;
    Script_mtwkGetSetRotation(SHEN_NAME('KAKU'), &local_E8, 0);
    Script_EVT_GetPosition(SHEN_NAME('KAKU'), -1, &local_F4, 0);
    while (local_E7 < 5) {
      (&local_F4)->y = (&local_F4)->y - 0.037f;
      Script_EVT_SetPosition(SHEN_NAME('KAKU'), POS_XYZ, &local_F4);
      {
        (&local_E8)->z = (&local_E8)->z + 1055;
        Script_EVT_SetRotation(SHEN_NAME('KAKU'), 0x38000000, &local_E8, 0);
      }
      {
        local_E7 = local_E7 + 1;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  return Script_srchControl(0);
}

static void script_334F9(int32_t arg0, int32_t arg1) {
  SCNF_WaitFrames_0000(1);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_FaceEx(arg0, FACE_ExpressionDisabled, 0, 1);
  Script_FaceEx(CHARA_HINA, FACE_ExpressionDisabled, 0, 1);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 60.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 60.0f);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  script_30240(arg0, -1, 133.1f, 45.2f, 160.0f);
  script_31E20(CHARA_AKIR, 1, 60);
  script_31DC7(CHARA_HINA, CHARA_AKIR, 1, 140);
  script_31ADD(8027, -1, -1, -1);
  script_31ADD(8037, -1, -1, 60);
  script_31ADD(8038, SHEN_NAME('DMY0'), SHEN_NAME('DMY0'), 120);
  script_3173B(CHARA_HINA, 22325, 0, 0x3F800000, 3, -1);
  script_3173B(CHARA_ISYM, 22301, 0, 0x3F800000, 3, -1);
  script_3173B(CHARA_NMNO, 22317, 0, 0x3F800000, 3, -1);
  script_30F16(arg1, 0.044f, &data_43AC8);
  script_30FD0(arg1, 0.044f);
  SCNF_WaitFrames_0000(30);
  while (task_734[arg1] != 1) {
    SCNF_WaitFrames_0000(1);
  }
  script_3110F(arg1);
  script_3324F();
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 1);
  Script_FaceEx(CHARA_HINA, FACE_ExpressionEnabled, 0, 1);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_43C7C, 8);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_43D60, 8);
  Script_clmdSetFlags(CHARA_ISYM, 4, 1);
  Script_clmdSetFlags(CHARA_NMNO, 4, 1);
  script_31DC7(CHARA_AKIR, CHARA_HINA, 20, -1);
  script_31DC7(CHARA_HINA, CHARA_AKIR, 1, -1);
  script_31DC7(CHARA_HINA, CHARA_AKIR, 1, 160);
  script_31DC7(CHARA_ISYM, CHARA_AKIR, 20, -1);
  script_31DC7(CHARA_ISYM, CHARA_AKIR, 1, 165);
  script_31DC7(CHARA_NMNO, CHARA_AKIR, 20, -1);
  script_31DC7(CHARA_NMNO, CHARA_AKIR, 1, 165);
  script_31ADD(8000, -1, -1, -1);
  script_31ADD(8001, -1, -1, 60);
  script_31ADD(8036, -1, -1, 150);
  script_31ADD(8002, -1, -1, 280);
  script_31ADD(8003, -1, -1, 340);
  script_30302(arg0, -1, 0x43070000, 0x42573333, 0x43390000, -1);
  script_30302(arg0, -1, 0x4308199A, 0x4255999A, 0x43200000, 280);
  script_30302(CHARA_HINA, -1, 0x4306E666, 0x425C6666, 0, -1);
  script_31B2D(1, -1);
  script_31B2D(13, 60);
  script_31B2D(0, 150);
  script_31E81(CHARA_HINA, 7, 2, 7, 150);
  script_31E81(CHARA_HINA, 10, 2, 20, 160);
  script_31E81(CHARA_HINA, 0, -1, 7, 360);
  script_3173B(arg0, 22284, 0, 0x3F800000, -1, -1);
  script_3173B(CHARA_HINA, 22323, 0, 0x3F800000, -1, 45);
  script_3173B(CHARA_HINA, 22325, 0, 0x3F800000, -1, 105);
  Script_scenCreateTask(script_2F8DE, 2, 263337, 70);
  SCNF_WaitFrames_0000(100);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_43C7C, 8);
  SCNF_WaitFrames_0000(180);
  script_31E20(CHARA_AKIR, 1, -1);
  script_31E20(CHARA_HINA, 1, -1);
  script_31E20(CHARA_ISYM, 1, -1);
  script_31E20(CHARA_NMNO, 1, -1);
  return;
}

static void script_3394F(int32_t arg0, int32_t arg1) {
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_31ADD(8011, -1, -1, -1);
  script_31ADD(8012, -1, -1, 148);
  script_3173B(arg0, 22285, 0, 0x3F800000, -1, 80);
  script_3173B(CHARA_HINA, 22318, 0, 0x3F800000, -1, 80);
  script_31DC7(arg0, CHARA_HINA, 1, 1);
  script_31DC7(CHARA_HINA, arg0, 1, 1);
  script_31DC7(arg0, CHARA_HINA, 1, 80);
  script_31DC7(CHARA_HINA, arg0, 1, 80);
  script_31DC7(arg0, CHARA_HINA, 1, 148);
  script_31DC7(CHARA_HINA, arg0, 1, 148);
  script_30302(arg0, -1, 0x43076148, 0x425A3333, 0x43110000, -1);
  script_30302(CHARA_HINA, -1, 0x43070000, 0x425C6666, 0x43A28000, -1);
  script_31B2D(13, -1);
  script_31B2D(4, 60);
  script_31B2D(5, 153);
  script_31B2D(6, 273);
  script_31E81(CHARA_HINA, 7, 0, 7, -1);
  script_31E81(arg0, 3, 0, 7, 208);
  script_31E81(CHARA_HINA, 2, 0, 7, 153);
  script_31E81(CHARA_HINA, 6, 0, 14, 333);
  SCNF_WaitFrames_0000(323);
  script_31E20(CHARA_AKIR, 2, -1);
  script_31E20(CHARA_HINA, 2, -1);
  script_3173B(arg0, 22283, 0x41200000, 0x3F800000, -1, -1);
  script_303C7(arg0, -1, 180.0f);
  script_2FB8B(arg0);
  script_303C7(arg0, -1, 30.0f);
  script_3173B(arg0, 655, 0, 0x3F800000, -1, -1);
  Script_FACE_LCG(arg0, FACE_BlinkClose, 8, 4);
  script_31B2D(12, 90);
  SCNF_WaitFrames_0000(165);
  script_31ADD(8014, -1, -1, -1);
  script_30302(CHARA_HINA, -1, 0x4306999A, 0x4257999A, 0x43820000, -1);
  script_30302(arg0, -1, 0x4305199A, 0x423E6666, 0x43AF0000, -1);
  script_3173B(CHARA_HINA, 22322, 0x42200000, 0x3F800000, -1, -1);
  script_2FB8B(CHARA_HINA);
  script_3173B(CHARA_HINA, 22324, 0, 0x3F800000, -1, -1);
  script_303C7(CHARA_HINA, -1, 290.0f);
  Script_executeAicaCommand(2720, 2, 60);
  SCNF_WaitFrames_0000(60);
  Script_FACE_LCG(arg0, FACE_BlinkOpen, 8, 1);
  script_31E81(CHARA_HINA, 0, -1, 7, -1);
  script_31E81(arg0, 0, -1, 7, -1);
  return;
}

static int32_t script_33C74(void) {
  Script_clmdSetFlags(CHARA_NMNO, 4, 0);
  SCNF_WaitFrames_0000(60);
  script_3173B(CHARA_NMNO, 22314, 0, 0x3F800000, -1, -1);
  SCNF_WaitFrames_0000(33);
  script_33162();
  SCNF_WaitFrames_0000(20);
  script_31DC7(CHARA_AKIR, CHARA_ISYM, 1, -1);
  script_2FB8B(CHARA_NMNO);
  Script_PlayCharaMotion(CHARA_NMNO, 22312, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return Script_srchControl(0);
}

static void script_33D04(int32_t arg0, int32_t arg1) {
  script_332C2();
  Script_charaDisplay(CHARA_HINA, CDM_Hide);
  script_31EC0(SHEN_NAME('POTI'), 0, 0, -0x4270A3D7, 10, 120);
  script_31ADD(8016, -1, -1, -1);
  script_31ADD(8020, -1, -1, 70);
  script_31DC7(CHARA_AKIR, arg0, 1, -1);
  script_31E20(CHARA_NMNO, 2, 30);
  script_30302(task_5C8[1], arg0, 0x3F800000, 0x3E99999A, -0x3E600000, -1);
  script_30302(task_5C8[1], -1, 0x4307199A, 0x42573333, 0x43700000, 70);
  script_30302(task_5C8[3], -1, 0x4306CCCD, 0x4260CCCD, 0, -1);
  script_30302(task_5C8[2], arg0, -0x40800000, 0, 0x43910000, -1);
  script_30302(arg0, -1, 0x43080000, 0x4255999A, 0x438C0000, -1);
  Script_scenCreateTask(script_2F8DE, 2, 1193, 15);
  Script_scenCreateTask(script_2F8DE, 2, 66729, 65);
  script_31B2D(8, 15);
  script_31B2D(7, 85);
  script_3173B(arg0, 22282, 0x41C80000, 0x3F800000, -1, 15);
  script_3173B(arg0, 22282, 0x425C0000, 0x3F800000, -1, 45);
  script_3173B(CHARA_HINA, 22319, 0, 0x3F800000, -1, -1);
  script_3173B(CHARA_NMNO, 22311, 0, 0x3F800000, -1, -1);
  script_3173B(CHARA_ISYM, 22297, 0, 0x3F800000, -1, -1);
  script_3173B(CHARA_ISYM, 22296, 0, 0x3F800000, -1, 35);
  SCNF_WaitFrames_0000(130);
  Script_charaDisplay(CHARA_HINA, CDM_Show);
  script_31ADD(8018, -1, -1, -1);
  script_30240(task_5C8[2], arg0, -0.7f, -1.0f, 160.0f);
  script_30240(task_5C8[1], arg0, -0.2f, 1.0f, 0.0f);
  script_3041B(task_5C8[2], -1, 0x42200000, 40);
  script_3173B(arg0, 22281, 0, 0x3F800000, 3, -1);
  script_3173B(CHARA_ISYM, 22294, 0x42340000, 0x3F800000, -1, -1);
  script_3173B(CHARA_NMNO, 22309, 0x42200000, 0x3F800000, -1, 10);
  script_3173B(CHARA_NMNO, 22308, 0, 0x3F800000, -1, 25);
  script_3173B(CHARA_ISYM, 22298, 0, 0x3F800000, -1, 40);
  script_3173B(CHARA_NMNO, 22310, 0x420C0000, 0x3F800000, -1, 105);
  script_31B2D(23, -1);
  script_31B2D(24, 60);
  SCNF_WaitFrames_0000(150);
  Script_motmSetBlendFrames(CHARA_NMNO, MOTM_BLEND_FRAMES_10);
  Script_motmSetBlendFrames(CHARA_ISYM, MOTM_BLEND_FRAMES_15);
  script_3041B(task_5C8[1], -1, 0x41200000, -1);
  script_30F16(1, 0.044f, &data_43B10);
  script_30FD0(1, 0.044f);
  SCNF_WaitFrames_0000(10);
  script_30F16(2, 0.044f, &data_43B10);
  script_30FD0(2, 0.044f);
  script_3173B(CHARA_HINA, 22289, 0, 0x3F800000, -1, 30);
  script_31B2D(12, 160);
  SCNF_WaitFrames_0000(300);
  Script_motmSetBlendFrames(CHARA_NMNO, MOTM_BLEND_FRAMES_7);
  Script_motmSetBlendFrames(CHARA_ISYM, MOTM_BLEND_FRAMES_7);
  script_3110F(1);
  script_3110F(2);
  script_31E81(arg0, 0, -1, 7, -1);
  script_3324F();
  return;
}

static void script_340FF(int32_t arg0, int32_t arg1) {
  Script_InitializeOsag(CHARA_ISYM);
  Script_InitializeOsag(CHARA_NMNO);
  script_31ADD(8023, -1, -1, -1);
  script_31ADD(8024, -1, -1, 450);
  script_3173B(CHARA_NMNO, 22307, 0, 0x3F800000, -1, -1);
  script_30302(task_5C8[1], -1, 0x43070000, 0x4256999A, 0x43480000, -1);
  script_30302(arg0, -1, 0x4308AB85, 0x4259851F, 0x43910000, -1);
  script_3041B(CHARA_ISYM, -1, 0x42340000, -1);
  script_31B2D(26, 90);
  SCNF_WaitFrames_0000(90);
  script_31DC7(CHARA_ISYM, arg0, 1, 270);
  script_3041B(arg0, -1, 0x42DC0000, 95);
  script_3041B(CHARA_ISYM, -1, 0x43020000, -1);
  script_3173B(arg0, 22280, 0, 0x3F800000, -1, 95);
  script_3173B(CHARA_ISYM, 22295, 0, 0x3F800000, -1, -1);
  script_31B2D(18, 200);
  Script_scenCreateTask(script_2F8DE, 2, 66729, 245);
  SCNF_WaitFrames_0000(300);
  Script_motmSetBlendFrames(CHARA_NMNO, MOTM_BLEND_FRAMES_15);
  Script_motmSetBlendFrames(CHARA_ISYM, MOTM_BLEND_FRAMES_15);
  script_31B2D(24, -1);
  script_31E20(CHARA_NMNO, 2, 45);
  script_30F16(1, 0.044f, &data_43B10);
  script_30FD0(1, 0.044f);
  SCNF_WaitFrames_0000(60);
  script_31E20(CHARA_ISYM, 2, -1);
  script_30F16(2, 0.044f, &data_43B10);
  script_30FD0(2, 0.044f);
  script_30240(task_5C8[3], -1, 134.8f, 56.2f, 0.0f);
  script_3173B(CHARA_HINA, 22289, 0, 0x3F800000, -1, 30);
  script_31B2D(12, 160);
  SCNF_WaitFrames_0000(300);
  Script_motmSetBlendFrames(CHARA_NMNO, MOTM_BLEND_FRAMES_7);
  Script_motmSetBlendFrames(CHARA_ISYM, MOTM_BLEND_FRAMES_7);
  script_3110F(1);
  script_3110F(2);
  script_31E81(arg0, 0, -1, 7, -1);
  return;
}

static void script_3435C(void) {
  task_554 = 1;
  script_343A3(task_554);
  task_550 = 0;
  for (;;) {
    if (task_55C == 1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_440F8, 1, data_44107)))
static void script_3438F(void) {
  script_EAA8("scene/02/MFSY/", "MSQB");
  return;
}

__attribute__((scn_string_bindings(0, data_44148, 1, data_44157, 2, data_4415C, 3, data_44162, 4, data_44167, 5, data_4416C, 6, data_44171, 7, data_44176, 8, data_4417B, 9, data_44187)))
static void script_343A3(int32_t arg0) {
  Script_SetCameraFlags(0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_SetActionSelectorHUDElementVisibility(1);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_WALK_UP1_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  script_E887();
  task_55C = 0;
  script_3173B(CHARA_AKIR, 89, 0, 0x3F800000, 3, 45);
  script_31ADD(8040, CHARA_AKIR, CHARA_AKIR, 45);
  SCNF_WaitFrames_0000(45);
  Script_SetLookPointDetails(CHARA_AKIR, 1, &data_4413C, 0);
  SCNF_WaitFrames_0000(25);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_31E20(CHARA_AKIR, 1, -1);
  script_EAA8("scene/02/MFSY/", "MSQB");
  task_564 = script_E948(CHARA_HINA);
  *(&task_564 + 1) = script_E948(CHARA_ISYM);
  *(&task_564 + 2) = script_E948(CHARA_NMNO);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  script_30240(CHARA_HINA, -1, 135.0f, 55.1f, 325.0f);
  script_30240(CHARA_ISYM, -1, 136.2f, 56.3f, 355.0f);
  script_30240(CHARA_NMNO, -1, 137.4f, 55.5f, 30.0f);
  script_3173B(CHARA_HINA, 22325, 0, 0x3F800000, 3, -1);
  script_3173B(CHARA_ISYM, 22301, 0, 0x3F800000, 3, -1);
  script_3173B(CHARA_NMNO, 22317, 0, 0x3F800000, 3, -1);
  Script_LoadVoiceFile(1, "E1036");
  Script_SetCameraSettings(5, 6, 1, 0);
  Script_LoadSND(0, 6, "FREE", 0);
  Script_LoadSND(0, 5, "FREE", 0);
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 3, "FREE", 0);
  Script_LoadSND(0, 2, "FREE", 0);
  Script_LoadSND(0, 0, "system1.snd", 0);
  Script_LoadSND(0, 2, "bgm067.snd", 0);
  Script_LoadSND(0, 4, "e1skeqte.snd", 0);
  Script_LoadSND(0, 5, data_4419F, 0);
  Script_executeAicaCommand(222376, 0, 0);
  Script_executeAicaCommand(1184, 2, 127);
  SCNF_WaitFrames_0000(1);
  Script_charaDisplay(SHEN_NAME('KAKU'), CDM_Show);
  Script_hmdlApplyScale(SHEN_NAME('KAKU'), &data_4410C);
  Script_hmdlApplyScale(SHEN_NAME('COLA'), &data_44118);
  Script_hmdlApplyScale(SHEN_NAME('SPRT'), &data_44118);
  Script_hmdlApplyScale(SHEN_NAME('GRAP'), &data_44118);
  Script_facemotUtils(CHARA_HINA, 0, 0);
  Script_FaceEx(CHARA_HINA, FACE_ExpressionEnabled, 0, 1);
  Script_FaceLookAtTargetUtils(CHARA_HINA, FACE_LookReset);
  Script_CharHandlers(CHARA_HINA, 2, -1, 16);
  SCNF_WaitFrames_0000(1);
  Script_scenCreateTask(script_34A97, 4, CHARA_HINA, 3, 2, 2);
  Script_scenCreateTask(script_34A97, 4, CHARA_ISYM, 2, 2, 2);
  Script_scenCreateTask(script_34A97, 4, CHARA_NMNO, 1, 2, 0);
  Script_scenCreateTask(script_34895, 2, CHARA_AKIR, 0);
  task_558 = Script_scenCreateTask(script_31F62, 2, CHARA_AKIR, 0);
  SCNF_WaitFrames_0000(5);
  return;
}

__attribute__((scn_string_bindings(0, data_441C4, 1, data_441CA)))
static void script_3474E(int32_t arg0) {
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  task_55C = 1;
  SCNF_WaitFrames_0000(1);
  Script_SetCameraFlags(1);
  Script_SetActionSelectorHUDElementVisibility(0);
  script_E866();
  Script_MoveObjectToObject(SHEN_NAME('DMY0'), -1);
  Script_MoveObjectToObject(SHEN_NAME('DMY1'), -1);
  Script_MoveObjectToObject(SHEN_NAME('DMY2'), -1);
  Script_MoveObjectToObject(SHEN_NAME('DMY3'), -1);
  Script_faceUpdateFor(CHARA_HINA, 0, 0);
  Script_LoadVoiceFile(0, "E1036");
  Script_CharHandlers(CHARA_AKIR, 2, 0, 16);
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  script_EA10(CHARA_HINA, *&task_564);
  script_EA10(CHARA_ISYM, *(&task_564 + 1));
  script_EA10(CHARA_NMNO, *(&task_564 + 2));
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "MSQB");
  Script_srchControl((void *)task_558);
  Script_scenToggleActiveFor(CHARA_AKIR, 1);
  script_D9D6();
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  SCNF_WaitFrames_0000(1);
  return;
}

static int32_t script_34895(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = Script_scenCreateTask(script_3003B, 3, arg0, arg1, 0);
  local_F8 = Script_scenCreateTask(script_2FBA7, 6, arg0, arg1, 2, 2, &data_4383C, script_30944);
  Script_charaDisplay(arg0, CDM_Show);
  Script_ryoSetChara(arg0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_7);
  Script_SetCPAMFlags(arg0, 0);
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 1);
  Script_MoveObjectToObject(SHEN_NAME('DMY0'), arg0);
  Script_EVT_SetPosition(SHEN_NAME('DMY0'), POS_XYZ, &data_441D0);
  Script_EVT_SetRotation(SHEN_NAME('DMY0'), 0x38000000, &data_441DC, 0);
  Script_MoveObjectToObject(SHEN_NAME('DMY1'), 0x414E4948);
  Script_EVT_SetPosition(SHEN_NAME('DMY1'), POS_XYZ, &data_441D0);
  Script_EVT_SetRotation(SHEN_NAME('DMY1'), 0x38000000, &data_441DC, 0);
  Script_MoveObjectToObject(SHEN_NAME('DMY2'), 0x4F4E4D4E);
  Script_EVT_SetPosition(SHEN_NAME('DMY2'), POS_XYZ, &data_441D0);
  Script_EVT_SetRotation(SHEN_NAME('DMY2'), 0x38000000, &data_441DC, 0);
  Script_MoveObjectToObject(SHEN_NAME('DMY3'), 0x4D595349);
  Script_EVT_SetPosition(SHEN_NAME('DMY3'), POS_XYZ, &data_441D0);
  Script_EVT_SetRotation(SHEN_NAME('DMY3'), 0x38000000, &data_441DC, 0);
  Script_SetCharaFlags(arg0, 4);
  Script_ccowUtils(arg0, CCOW_DisableAll);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (task_55C == 1) {
      Script_ccowUtils(arg0, CCOW_EnableAll);
      Script_charaSetRefboardFlagTo(arg0, 2);
      Script_srchControl((void *)local_F8);
      Script_srchControl((void *)local_FC);
      Script_srchControl((void *)task_5F8[arg1]);
      Script_srchControl(0);
    }
  }
  end:;
}

static int32_t script_34A97(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = Script_scenCreateTask(script_3003B, 3, arg0, arg1, 0);
  local_F8 = Script_scenCreateTask(script_2FBA7, 6, arg0, arg1, 0, 2, &data_4383C, script_30944);
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_charaDisplay(arg0, CDM_Show);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_7);
  Script_SetCharaFlags(arg0, 0);
  Script_SetCPAMFlags(arg0, 0);
  Script_charaSetRefboardFlagTo(arg0, 1);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (task_55C == 1) {
      Script_charaSetRefboardFlagTo(arg0, 2);
      Script_charaDisplay(arg0, CDM_Hide);
      Script_srchControl((void *)local_F8);
      Script_srchControl((void *)local_FC);
      Script_srchControl((void *)task_5F8[arg1]);
      Script_srchControl(0);
    }
  }
  end:;
}

static int32_t script_34B5E(int32_t arg0, int32_t arg1) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_executeAicaCommand(arg0, 0, 0);
  return Script_srchControl(0);
}

static int32_t script_34B82(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  task_778[arg1] = 0;
  task_788[arg1] = 0;
  task_798[arg1] = 0;
  task_7A8[arg1] = arg2;
  task_7B8[arg1] = arg3;
  task_7C8 = 0;
  Script_CharHandlers(arg0, 0, task_7A8[arg1], 16);
  Script_CharHandlers(arg0, 1, task_7B8[arg1], 16);
  for (;;) {
    task_778[arg1] = 0;
    while (task_778[arg1] == 0) {
      SCNF_WaitFrames_0000(1);
    }
    task_7C8 = task_798[arg1] | 0x40000000;
    Script_scenCreateTask(arg4, 3, arg0, arg1, task_778[arg1] + task_788[arg1]);
  }
  end:;
}

static int32_t script_34C59(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  local_FC = task_7C8;
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_motmGetCharaFlags(arg4, MOTM_PoseNone, SCEN_Write);
  Script_motmToggleFlags(arg4, SCEN_Enable, 0x40);
  Script_PlayCharaMotion(arg4, arg0, arg2, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(arg4, arg3);
  if (local_FC & -0x80000000) {
    Script_motmToggleFlags(arg4, SCEN_Enable, 0x80);
  }
  if (local_FC & 0x40000000) {
    Script_charaToggleMotEnabled(arg4, 1);
  }
  if (local_FC & 0x20000000) {
    Script_charaToggleMotEnabled(arg4, 0);
  }
  return Script_srchControl(0);
}

static int32_t script_34CF0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_CharHandlers(arg0, 2, arg2, 16);
  if (arg4 > 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  Script_CharHandlers(arg0, 0, task_7A8[arg1], 16);
  Script_CharHandlers(arg0, 1, task_7B8[arg1], 16);
  return Script_srchControl(0);
}

static int32_t script_34D52(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  local_FC = task_7C8;
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  if (local_FC & 0x10000000) {
    Script_GetDataFromFightProgram(arg0, arg1);
  }
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (local_FC & 0x10000000) {
    Script_GetDataFromFightProgram(arg0, 0);
  }
  return Script_srchControl(0);
}

static void script_34DAE(int32_t arg0, float arg1, int32_t arg2) {
  int32_t local_FC;
  Vector3i local_F0;
  local_FC = task_7C8;
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  if (local_FC & -0x80000000) {
    arg1 = (float)0 - arg1;
  }
  (&local_F0)->y = (int32_t)(arg1 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, -0x70000000, &local_F0, 0);
  Script_srchControl(0);
  return;
}

static void script_34E0B(int32_t arg0) {
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_34E27(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, void *arg4, int32_t arg5) {
  int32_t local_FC;
  task_7FC[arg1] = Script_scenCreateTask(script_34B82, 5, arg0, arg1, arg2, arg3, arg5);
  task_7CC[arg1] = arg0;
  task_7DC[arg1] = 0;
  task_7EC[arg1] = 0;
  task_80C = 4;
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (task_7DC[arg1]) {
      local_FC = task_7EC[arg1] * 12;
      {
        int32_t local_F8;
        local_F8 = Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 5);
        if (local_F8 > 0) {
          SCNF_WaitFrames_0000(local_F8);
        }
      }
      {
        int32_t local_F8;
        local_F8 = Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 8);
        if (local_F8 > 0) {
          task_778[arg1] = local_F8;
        }
      }
      {
        int32_t local_F8;
        local_F8 = Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 6);
        if (local_F8 > 0) {
          SCNF_WaitFrames_0000(local_F8);
        }
      }
      if (task_788[arg1] == 0) {
        Script_scenCreateTask(script_35197, 4, Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC), Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 1), Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 2), Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 3));
      }
      {
        int32_t local_F8;
        local_F8 = Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 7);
        if (local_F8 > 0) {
          SCNF_WaitFrames_0000(local_F8);
        }
      }
      {
        int32_t local_F8;
        local_F8 = Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 9);
        if (local_F8 > 0) {
          task_778[arg1] = local_F8;
        }
      }
      script_35095();
      {
        int32_t local_F8;
        if (task_818 == Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC)) {
          task_814 = 1;
          local_F8 = Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 10);
        } else {
          if (task_788[arg1] == 0) {
            task_820 = task_820 + Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 4);
          }
          task_814 = 0;
          local_F8 = Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 11);
        }
        if (local_F8 > 0) {
          task_778[arg1] = local_F8;
        }
      }
      script_35085(arg1);
    }
  }
  end:;
}

static void script_3504E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  task_7DC[arg0] = 1;
  task_7EC[arg0] = arg1;
  task_788[arg0] = arg2;
  task_798[arg0] = arg3;
  return;
}

static void script_35085(int32_t arg0) {
  task_7DC[arg0] = 0;
  return;
}

static void script_35095(void) {
  SCNF_WaitFrames_0000(2);
  while (task_818 == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_350B5(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  script_3504E(arg0, arg1, 0, arg3);
  script_3504E(arg2, arg1, 1, arg3);
  while (task_7DC[arg0]) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_350EF(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  script_3504E(arg0, arg1, 0, arg3);
  script_3504E(arg2, arg1, 1, arg3);
  script_3504E(arg2 + 1, arg1, 2, arg3);
  while (task_7DC[arg0]) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_3513B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  script_3504E(arg0, arg1, 0, arg4);
  script_3504E(arg2, arg1, 1, arg4);
  script_3504E(arg3, arg1, 2, arg4);
  while (task_7DC[arg0]) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_35184(int32_t arg0) {
  task_80C = arg0;
  Script_SetQteOverlayTimeoutTuning(arg0, QTE_SetTimeoutStep);
  return;
}

static int32_t script_35197(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = arg1 + arg3;
  local_F8 = arg2 + arg3;
  Script_scenCreateTask(script_351FA, 2, arg1, arg3);
  Script_SetQteOverlayTimeoutTuning(local_FC, QTE_SetTimeoutMaximum);
  if (arg0 == 68) {
    SCNF_UIUtils(0x44, local_F8, 5, 0, 1, 0);
  } else {
    SCNF_UIUtils(arg0, local_F8, 2, 0, 1, 0);
  }
  return Script_srchControl(0);
}

static int32_t script_351FA(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  task_81C = arg0;
  task_818 = 0;
  task_810 = 0;
  local_F8 = 0xFFFF;
  for (;;) {
    local_FC = SCNF_UIUtils(0, 0, 0, 0, 0, -1);
    task_810 = task_810 + 1;
    if (local_FC != 0xFFFF) {
      if (local_FC != 0xA000) {
        local_F8 = local_FC;
      }
    }
    if (arg0 == -1) {
      if (local_F8 != 0xFFFF) {
        break;
      }
    } else {
      if (task_810 >= arg0 + arg1 + 4) {
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  if ((local_F8 & 0xFFF) == 0) {
    local_F8 = 0xFFFF;
  }
  task_818 = local_F8 & 0xFFF;
  return Script_srchControl(0);
}

static int32_t script_352BB(int32_t arg0, int32_t arg1, int32_t arg2) {
  {
    Vector3 local_F4;
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    task_824[arg1] = arg0;
    task_834[arg1][0] = (&local_F4)->x;
    task_834[arg1][1] = (&local_F4)->y;
    task_834[arg1][2] = (&local_F4)->z;
  }
  task_864[arg1] = 0.0f;
  for (;;) {
    Vector3 local_F4;
    SCNF_WaitFrames_0000(1);
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    task_834[arg1][0] = (&local_F4)->x;
    task_834[arg1][1] = (&local_F4)->y;
    task_834[arg1][2] = (&local_F4)->z;
    task_864[arg1] = Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, task_834[arg1][0] - task_834[arg2][0], task_834[arg1][2] - task_834[arg2][2]);
  }
  end:;
}

static void script_353A9(void) {
  task_874 = 0.0f;
  task_878 = 0.0f;
  task_87C = 0.0f;
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_GetInputData(2, 0) & 4) {
      if (Script_GetInputData(0, 2) & 0x4000) {
        task_874 = task_874 - 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x8000) {
        task_874 = task_874 + 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x1000) {
        task_878 = task_878 - 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x2000) {
        task_878 = task_878 + 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x40) {
        task_87C = task_87C - 1.0f;
      }
      if (Script_GetInputData(0, 2) & 0x80) {
        task_87C = task_87C + 1.0f;
      }
    }
  }
  return;
}

static void script_35497(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(arg0, &local_E8, 0);
  Script_MoveObjectToObject(arg0, -1);
  return;
}

static void script_354C0(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4) {
  float local_F4[3];
  Vector3i local_E8;
  int32_t local_E4;
  Vector3 local_D8;
  local_E4 = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  local_F4[0] = arg2;
  local_F4[1] = 0.0f;
  local_F4[2] = arg3;
  local_D8.x = arg2;
  (&local_D8)->z = arg3;
  Script_EVT_SetPosition(arg0, 0x28000000, &local_D8);
  local_E8.x = 0;
  (&local_E8)->y = (int32_t)(arg4 / 360.0f * (float)0x10000);
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
  Script_MoveObjectToObject(arg0, local_E4);
  Script_InitializeOsag(arg0);
  return;
}

static void script_3554F(int32_t arg0, int32_t arg1, const Vector3 *arg2) {
  int32_t local_FC;
  local_FC = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_SetPosition(arg0, POS_XYZ, arg2);
  Script_MoveObjectToObject(arg0, local_FC);
  Script_InitializeOsag(arg0);
  return;
}

static void script_35582(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_3559C, 6, arg0, arg1, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_3559C(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, int32_t arg5) {
  if (arg5 >= 0) {
    SCNF_WaitFrames_0000(arg5);
  }
  script_354C0(arg0, arg1, arg2, arg3, arg4);
  return Script_srchControl(0);
}

static void script_355C7(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, float arg5) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  local_F4.x = arg2;
  (&local_F4)->y = arg3;
  (&local_F4)->z = arg4;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  local_E8.x = 0;
  (&local_E8)->y = (int32_t)(arg5 / 360.0f * (float)0x10000);
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
  Script_MoveObjectToObject(arg0, local_E4);
  Script_InitializeOsag(arg0);
  return;
}

static void script_35647(int32_t arg0, int32_t arg1, float arg2) {
  int32_t unused_F4[3];
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  local_E8.x = 0;
  (&local_E8)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
  Script_MoveObjectToObject(arg0, local_E4);
  return;
}

static void script_3569B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_356B1, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_356B1(int32_t arg0, int32_t arg1, float arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  script_35647(arg0, arg1, arg2);
  return Script_srchControl(0);
}

static void script_356D8(int32_t arg0, float arg1) {
  Vector3i local_F4;
  (&local_F4)->y = (int32_t)(arg1 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, -0x70000000, &local_F4, 0);
  return;
}

static void script_35701(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, -0x48000000, &local_F4, 0);
  return;
}

static void script_35755(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_35769, 3, arg0, arg1, arg2);
  return;
}

static int32_t script_35769(int32_t arg0, float arg1, int32_t arg2) {
  if (arg2 >= 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  script_356D8(arg0, arg1);
  return Script_srchControl(0);
}

static void script_3578E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_357A6, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_357A6(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4) {
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  script_35701(arg0, arg1, arg2, arg3);
  return Script_srchControl(0);
}

static void script_357CF(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_357E7, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_357E7(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4) {
  float local_FC;
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  local_FC = (arg2 - arg1) / arg3;
  for (;;) {
    if (arg3 <= 0.0f) {
      break;
    }
    script_35701(arg0, 0.0f, 0.0f, local_FC);
    arg3 = arg3 - 1.0f;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_3583C(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = arg0;
  while (local_E4 <= arg1) {
    Script_EVT_GetPosition(task_7CC[local_E4], -1, &local_F4, 0);
    Script_mtwkGetSetRotation(task_7CC[local_E4], &local_E8, 0);
    task_880[local_E4][0] = (&local_F4)->x;
    task_880[local_E4][1] = (&local_F4)->y;
    task_880[local_E4][2] = (&local_F4)->z;
    task_8B0[local_E4][0] = (&local_E8)->x;
    task_8B0[local_E4][1] = (&local_E8)->y;
    task_8B0[local_E4][2] = (&local_E8)->z;
    task_8E0[local_E4] = Script_GetCharaMotionRemainingFramesScaled(task_7CC[local_E4]);
    task_8F0[local_E4] = Script_scenCharaGetMotAnimId(task_7CC[local_E4]);
    local_E4 = local_E4 + 1;
  }
  return;
}

static void script_35921(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = arg0;
  while (local_E4 <= arg1) {
    local_F4.x = task_880[local_E4][0];
    (&local_F4)->y = task_880[local_E4][1];
    (&local_F4)->z = task_880[local_E4][2];
    local_E8.x = task_8B0[local_E4][0];
    (&local_E8)->y = task_8B0[local_E4][1];
    (&local_E8)->z = task_8B0[local_E4][2];
    Script_EVT_SetPosition(task_7CC[local_E4], POS_XYZ, &local_F4);
    Script_EVT_SetRotation(task_7CC[local_E4], 0x38000000, &local_E8, 0);
    Script_PlayCharaMotion(task_7CC[local_E4], task_8F0[local_E4], task_8E0[local_E4], -1.0f, -1.0f, 1.0f, 0);
    local_E4 = local_E4 + 1;
  }
  return;
}

static void script_35A0A(int32_t arg0) {
  task_904 = arg0;
  task_900 = 1;
  Script_PlayerStatsUtils(PS_Set70MBTimeCapSlot, task_904, task_900);
  return;
}

static void script_35A28(void) {
  int32_t local_FC;
  if (task_900 == 1) {
    local_FC = Script_PlayerStatsUtils(PS_Get70MBCounter1);
    Script_PlayerStatsUtils(PS_Set70MBCounter1, local_FC + 1);
  }
  if (task_900 < 255) {
    task_900 = task_900 + 1;
  }
  Script_PlayerStatsUtils(PS_Set70MBTimeCapSlot, task_904, task_900);
  return;
}

static void script_35A70(void) {
  int32_t local_FC;
  local_FC = Script_PlayerStatsUtils(PS_Get70MBCounter0);
  Script_PlayerStatsUtils(PS_Set70MBCounter0, local_FC + 1);
  return;
}

static void script_35A8B(void) {
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_35AA5(const char *arg0, const char *arg1) {
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg1) == 0) {
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, arg0, arg1) == 0) {
      SCNF_WaitFrames_0000(1);
    }
    script_35A8B();
  }
  return;
}

static void script_35AE3(const char *arg0) {
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg0) != 0) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, arg0);
  }
  return;
}

static void script_35B04(int32_t arg0) {
  L035B06:;
  SCNF_WaitFrames_0000(1);
  if (!Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, arg0)) goto L035B06;
  return;
}

static void script_35B1B(int32_t arg0) {
  int32_t local_FC;
  local_FC = 0;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) != 0) {
    local_FC = Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT_INTERNAL_HUMAN, arg0);
  } else {
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 1);
    while (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
      local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
      while (local_FC == 0) {
        SCNF_WaitFrames_0000(1);
        local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
      }
      script_35B04(local_FC);
    }
  }
  (void)local_FC;
  return;
}

static void script_35BA1(int32_t arg0, int32_t arg1) {
  if (arg1) {
    Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, arg1);
  }
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 0);
  arg1 = 0;
  return;
}

static int32_t script_35BC4(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  local_FC = arg2;
  switch (local_FC) {
    case 1:
      break;
    case 2:
      Script_scenCreateTask(script_34C59, 6, 20747, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 3:
      Script_scenCreateTask(script_34B5E, 2, 30379, 8);
      Script_scenCreateTask(script_34C59, 6, 20742, -1, 0, 0x3F800000, arg0, arg1);
      script_36868(CHARA_AKIR, 4, 0, 1, -1);
      script_36868(CHARA_AKIR, 4, 1, 5, 30);
      script_36868(CHARA_AKIR, 0, -1, 5, 90);
      break;
    case 4:
      Script_scenCreateTask(script_34CF0, 5, arg0, arg1, 0, -1, 20);
      Script_scenCreateTask(script_34C59, 6, 20746, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 5:
      Script_scenCreateTask(script_34D52, 4, arg0, 1, 10, 10);
      Script_scenCreateTask(script_34C59, 6, 20741, -1, 0x41A00000, 0x3F4CCCCD, arg0, arg1);
      break;
    case 6:
      Script_scenCreateTask(script_34B5E, 2, 0x1976AB, 5);
      Script_scenCreateTask(script_34C59, 6, 20745, -1, 0x41A00000, 0x3F4CCCCD, arg0, arg1);
      break;
    case 7:
      Script_scenCreateTask(script_34C59, 6, 20762, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 8:
      Script_scenCreateTask(script_34C59, 6, 20778, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 9:
      break;
    case 10:
      Script_scenCreateTask(script_34B5E, 2, 0x2376AB, 20);
      Script_scenCreateTask(script_34B5E, 2, 0x2676AB, 40);
      Script_scenCreateTask(script_34B5E, 2, 0x2676AB, 130);
      break;
    case 11:
      Script_scenCreateTask(script_34C59, 6, 20763, -1, 0x41200000, 0x3F800000, arg0, arg1);
      script_36868(CHARA_AKIR, 4, 0, 1, -1);
      script_36868(CHARA_AKIR, 4, 1, 5, 30);
      script_36868(CHARA_AKIR, 0, -1, 5, 90);
      break;
    case 12:
      Script_scenCreateTask(script_34B5E, 2, 30379, 4);
      Script_scenCreateTask(script_34C59, 6, 20779, -1, 0x41200000, 0x3F800000, arg0, arg1);
      break;
    case 13:
      break;
    case 14:
      Script_scenCreateTask(script_34C59, 6, 20747, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 15:
      Script_scenCreateTask(script_34C59, 6, 20749, -1, 0x41400000, 0x3F800000, arg0, arg1);
      break;
    case 16:
      Script_scenCreateTask(script_34C59, 6, 20756, -1, 0x41400000, 0x3F800000, arg0, arg1);
      break;
    case 17:
      break;
    case 18:
      Script_scenCreateTask(script_34C59, 6, 20757, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 19:
      Script_scenCreateTask(script_34B5E, 2, 30379, 20);
      Script_scenCreateTask(script_34CF0, 5, arg0, arg1, 0, 20, 15);
      Script_scenCreateTask(script_34C59, 6, 20751, -1, 0x41000000, 0x3F800000, arg0, arg1);
      break;
    case 20:
      Script_scenCreateTask(script_34B5E, 2, 0x2876AB, 42);
      Script_scenCreateTask(script_34C59, 6, 20757, -1, 0x41200000, 0x3F800000, arg0, arg1);
      break;
    case 21:
      Script_scenCreateTask(script_34C59, 6, 787, -1, 0, 0x3F800000, arg0, arg1);
      Script_scenCreateTask(script_34C59, 6, 20752, 20, 0, 0x3F800000, arg0, arg1);
      script_36868(CHARA_AKIR, 4, 0, 1, -1);
      script_36868(CHARA_AKIR, 4, 1, 5, 30);
      script_36868(CHARA_AKIR, 0, -1, 5, 90);
      break;
    case 22:
      Script_scenCreateTask(script_34C59, 6, 20758, -1, 0, 0x3F800000, arg0, arg1);
      Script_scenCreateTask(script_34C59, 6, 20759, 31, 0, 0x3F800000, arg0, arg1);
      break;
  }
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_442CC, 1, data_442D1, 2, data_442D6)))
static int32_t script_36070(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  Script_charaDisplay(arg0, CDM_Show);
  Script_charaDisplay(arg1, CDM_Hide);
  local_E4 = 2;
  local_E0 = 2;
  Script_scenCreateTask(script_34B5E, 2, 1193, -1);
  Script_scenCreateTask(script_34B5E, 2, 1193, 20);
  Script_scenCreateTask(script_34B5E, 2, 1193, 25);
  for (;;) {
    local_DC = 0;
    if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 3, "MGQB") == 0) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MGQB");
    }
    while (local_DC < 6) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
      Script_InterpolateAndScaleAngles(local_DC, local_E4, &local_F4, &local_E8, "DRUM");
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_charaPosRotUtils(arg0, *(&data_442A8 + local_DC), &local_F4, &local_E8, CHARA_PART_Set);
      local_DC = local_DC + 1;
    }
    local_E4 = local_E4 + local_E0;
    if (local_E4 >= 150) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_361A7(int32_t arg0, int32_t arg1, float arg2, int32_t arg3, const void *arg4) {
  SCNF_WaitFrames_0000(1);
  task_908[arg1] = arg0;
  task_918[arg1] = arg3;
  script_36214(arg1, arg2, arg4);
  for (;;) {
    if (task_928[arg1]) {
      task_938[arg1] = Script_walk3D(ActivateGetIdx, arg0, task_948[arg1]);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_36214(int32_t arg0, float arg1, const void *arg2) {
  int32_t local_FC;
  task_928[arg0] = 0;
  task_938[arg0] = 0;
  task_948[arg0] = arg1;
  local_FC = task_918[arg0];
  if ((local_FC == 0) | (local_FC == 1)) {
    Script_PlayCharaMotion(task_908[arg0], MN_AKI_AKI_L_STAND, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  } else {
    if (local_FC == 2) {
      Script_PlayCharaMotion(task_908[arg0], MN_AKI_AKI_L_STAND, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      goto L0362B5;
    }
  }
  L0362B5:;
  Script_walk3D(SetupPath, task_908[arg0], arg2, 0);
  return;
}

static void script_362CE(int32_t arg0, float arg1) {
  int32_t local_FC;
  task_948[arg0] = arg1;
  task_928[arg0] = 1;
  local_FC = task_918[arg0];
  switch (local_FC) {
    case 0:
      Script_motmToggleFlags(task_908[arg0], SCEN_Enable, 0x40);
      Script_charaToggleMotEnabled(task_908[arg0], 0);
      Script_SetCharaMotionPlaybackRate(task_908[arg0], 1.0f);
      Script_PlayCharaMotion(task_908[arg0], MN_AKI_AKI_WALK_UP1_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      break;
    case 1:
      Script_motmToggleFlags(task_908[arg0], SCEN_Enable, 0x40);
      Script_motmToggleFlags(task_908[arg0], SCEN_Enable, 4);
      Script_charaToggleMotEnabled(task_908[arg0], 0);
      Script_SetCharaMotionPlaybackRate(task_908[arg0], 1.0f);
      Script_PlayCharaMotion(task_908[arg0], 20760, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      break;
    case 2:
      Script_motmToggleFlags(task_908[arg0], SCEN_Enable, 0x40);
      Script_charaToggleMotEnabled(task_908[arg0], 0);
      Script_SetCharaMotionPlaybackRate(task_908[arg0], 0.5f);
      Script_PlayCharaMotion(task_908[arg0], MN_AKI_AKI_WALK_SLOW_K_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      break;
  }
  return;
}

static void script_36443(int32_t arg0) {
  int32_t local_FC;
  task_928[arg0] = 0;
  local_FC = task_918[arg0];
  switch (local_FC) {
    case 0:
      Script_charaToggleMotEnabled(task_908[arg0], 1);
      Script_SetCharaMotionPlaybackRate(task_908[arg0], 1.0f);
      break;
    case 1:
      Script_motmToggleFlags(task_908[arg0], SCEN_Disable, 4);
      Script_charaToggleMotEnabled(task_908[arg0], 1);
      Script_SetCharaMotionPlaybackRate(task_908[arg0], 1.0f);
      break;
    case 2:
      Script_charaToggleMotEnabled(task_908[arg0], 1);
      Script_SetCharaMotionPlaybackRate(task_908[arg0], 1.0f);
      break;
  }
  return;
}

static void script_36500(int32_t arg0, int32_t arg1) {
  task_95C = arg0;
  task_758 = 0;
  return;
}

__attribute__((scn_string_bindings(0, data_44350, 1, data_44357, 2, data_4435E, 3, data_44365, 4, data_4436C, 5, data_44373, 6, data_4437A, 7, data_44381, 8, data_44388, 9, data_4438F, 10, data_44396, 11, data_4439D, 12, data_443A4, 13, data_443AB, 14, data_443B2, 15, data_443B9, 16, data_443C0, 17, data_443C7, 18, data_443CE, 19, data_443D5, 20, data_443DC, 21, data_443E3, 22, data_443EA, 23, data_443F1, 24, data_443F8, 25, data_443FF, 26, data_44406, 27, data_4440D, 28, data_44414, 29, data_4441B, 30, data_44422, 31, data_4442B, 32, data_44434, 33, data_4443D, 34, data_44446, 35, data_4444F, 36, data_44458, 37, data_44461, 38, data_4446A, 39, data_44473, 40, data_4447C, 41, data_44485)))
static void script_36511(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      SCNT_ShowSubtitle("開始０");
      break;
    case 1:
      SCNT_ShowSubtitle("成功０");
      break;
    case 2:
      SCNT_ShowSubtitle("失敗０");
      break;
    case 3:
      SCNT_ShowSubtitle("開始１");
      break;
    case 4:
      SCNT_ShowSubtitle("成功１");
      break;
    case 5:
      SCNT_ShowSubtitle("失敗１");
      break;
    case 6:
      SCNT_ShowSubtitle("開始２");
      break;
    case 7:
      SCNT_ShowSubtitle("成功２");
      break;
    case 8:
      SCNT_ShowSubtitle("失敗２");
      break;
    case 9:
      SCNT_ShowSubtitle("開始３");
      break;
    case 10:
      SCNT_ShowSubtitle("成功３");
      break;
    case 11:
      SCNT_ShowSubtitle("失敗３");
      break;
    case 12:
      SCNT_ShowSubtitle("開始４");
      break;
    case 13:
      SCNT_ShowSubtitle("成功４");
      break;
    case 14:
      SCNT_ShowSubtitle("失敗４");
      break;
    case 15:
      SCNT_ShowSubtitle("開始５");
      break;
    case 16:
      SCNT_ShowSubtitle("成功５");
      break;
    case 17:
      SCNT_ShowSubtitle("失敗５");
      break;
    case 18:
      SCNT_ShowSubtitle("開始６");
      break;
    case 19:
      SCNT_ShowSubtitle("成功６");
      break;
    case 20:
      SCNT_ShowSubtitle("失敗６");
      break;
    case 21:
      SCNT_ShowSubtitle("開始７");
      break;
    case 22:
      SCNT_ShowSubtitle("成功７");
      break;
    case 23:
      SCNT_ShowSubtitle("失敗７");
      break;
    case 24:
      SCNT_ShowSubtitle("開始８");
      break;
    case 25:
      SCNT_ShowSubtitle("成功８");
      break;
    case 26:
      SCNT_ShowSubtitle("失敗８");
      break;
    case 27:
      SCNT_ShowSubtitle("開始９");
      break;
    case 28:
      SCNT_ShowSubtitle("成功９");
      break;
    case 29:
      SCNT_ShowSubtitle("失敗９");
      break;
    case 30:
      SCNT_ShowSubtitle("開始１０");
      break;
    case 31:
      SCNT_ShowSubtitle("成功１０");
      break;
    case 32:
      SCNT_ShowSubtitle("失敗１０");
      break;
    case 33:
      SCNT_ShowSubtitle("開始１１");
      break;
    case 34:
      SCNT_ShowSubtitle("成功１１");
      break;
    case 35:
      SCNT_ShowSubtitle("失敗１１");
      break;
    case 36:
      SCNT_ShowSubtitle("開始１２");
      break;
    case 37:
      SCNT_ShowSubtitle("成功１２");
      break;
    case 38:
      SCNT_ShowSubtitle("失敗１２");
      break;
    case 39:
      SCNT_ShowSubtitle("開始１３");
      break;
    case 40:
      SCNT_ShowSubtitle("成功１３");
      break;
    case 41:
      SCNT_ShowSubtitle("失敗１３");
      break;
    case 42:
      SCNT_ShowSubtitle("オート１");
      break;
    case 43:
      SCNT_ShowSubtitle("オート２");
      break;
    case 44:
      SCNT_ShowSubtitle("オート３");
      break;
  }
  Script_srchControl(0);
  return;
}

static void script_3682E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_36844, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_36844(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_seqEnterEvtCam(arg0, arg1, arg2);
  return Script_srchControl(0);
}

static void script_36868(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_36880, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_36880(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  Script_FACE(arg0, arg1, arg2, arg3);
  return Script_srchControl(0);
}

static void script_368A7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_368BD, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_368BD(float arg0, float arg1, float arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_SetCamShake(arg0, arg1, arg2);
  return Script_srchControl(0);
}

static void script_368E2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  Script_scenCreateTask(script_368FE, 7, arg2, arg6, arg3, arg4, arg0, arg1, arg5);
  return;
}

static int32_t script_368FE(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_PlayCharaMotion(arg4, arg0, arg2, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(arg4, arg3);
  if (arg6 == 3) {
    Script_motmToggleFlags(arg4, SCEN_Disable, 0x40);
  } else {
    Script_motmToggleFlags(arg4, SCEN_Enable, 0x40);
  }
  if (arg6 == 4) {
    Script_charaToggleMotEnabled(arg4, 0);
  } else {
    Script_charaToggleMotEnabled(arg4, 1);
  }
  if (arg6 == 5) {
    Script_motmToggleFlags(arg4, SCEN_Enable, 0x80);
    Script_motmToggleFlags(arg4, SCEN_Disable, 0x40);
  }
  if (arg6 == 1) {
    Script_motmToggleFlags(arg4, SCEN_Enable, 0x80);
  }
  return Script_srchControl(0);
}

static void script_369A9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_369BF, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_369BF(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_EVT_GetPosition(arg1, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(arg0, arg2, &local_F4, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookAtPosition, &local_F4, 8);
  return Script_srchControl(0);
}

static void script_36A02(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_36A16, 3, arg0, arg1, arg2);
  return;
}

static int32_t script_36A16(int32_t arg0, int32_t arg1, int32_t arg2) {
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  Script_SetCharaLookAtPosition(arg0, -arg1, 0, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  return Script_srchControl(0);
}

static void script_36A48(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_36A5C, 3, arg0, arg1, arg2);
  return;
}

static int32_t script_36A5C(int32_t arg0, int32_t arg1, int32_t arg2) {
  while (arg1 > 0) {
    Script_SetFightProgramStateBytes(arg0, arg1, 1);
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  return Script_srchControl(0);
}

static void script_36A96(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_36AA8, 2, arg0, arg1);
  return;
}

static int32_t script_36AA8(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_StartOrFreeSEQ(0);
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      Script_ShowUIMessageFromID("E1046A001");
      break;
    case 1:
      Script_ShowUIMessageFromID("E1046A002");
      break;
    case 2:
      Script_ShowUIMessageFromID("E1046A003");
      break;
    case 3:
      Script_ShowUIMessageFromID("E1046A004");
      break;
    case 4:
      Script_ShowUIMessageFromID("E1046A005");
      break;
    case 5:
      Script_ShowUIMessageFromID("E1046A006");
      break;
    case 6:
      Script_ShowUIMessageFromID("E1046A007");
      break;
    case 7:
      Script_ShowUIMessageFromID("E1046D001");
      break;
    case 8:
      Script_ShowUIMessageFromID("E1046D002");
      break;
    case 9:
      Script_ShowUIMessageFromID("E1046D003");
      break;
    case 10:
      Script_ShowUIMessageFromID("E1046D004");
      break;
    case 11:
      Script_ShowUIMessageFromID("E1046D005");
      break;
    case 12:
      Script_ShowUIMessageFromID("E1046D006");
      break;
    case 13:
      Script_ShowUIMessageFromID("E1046D007");
      break;
    case 14:
      Script_ShowUIMessageFromID("E1046D008");
      break;
    case 15:
      Script_ShowUIMessageFromID("E1046D009");
      break;
    case 16:
      Script_ShowUIMessageFromID("E1046D010");
      break;
    case 17:
      Script_ShowUIMessageFromID("E1046D011");
      break;
    case 18:
      Script_ShowUIMessageFromID("E1046D012");
      break;
    case 19:
      Script_ShowUIMessageFromID("E1046D013");
      break;
    case 20:
      Script_ShowUIMessageFromID("E1046D014");
      break;
    case 21:
      Script_ShowUIMessageFromID("E1046D015");
      break;
    case 22:
      Script_ShowUIMessageFromID("E1046D016");
      break;
    case 23:
      Script_ShowUIMessageFromID("E1046D017");
      break;
    case 24:
      Script_ShowUIMessageFromID("E1046D018");
      break;
    case 25:
      Script_ShowUIMessageFromID("E1046B001");
      break;
    case 26:
      Script_ShowUIMessageFromID("E1046B002");
      break;
    case 27:
      Script_ShowUIMessageFromID("E1046B003");
      break;
    case 28:
      Script_ShowUIMessageFromID("E1046B004");
      break;
    case 29:
      Script_ShowUIMessageFromID("E1046B005");
      break;
    case 30:
      Script_ShowUIMessageFromID("E1046B006");
      break;
    case 31:
      Script_ShowUIMessageFromID("E1046B007");
      break;
    case 32:
      Script_ShowUIMessageFromID("E1046B008");
      break;
    case 33:
      Script_ShowUIMessageFromID("E1046B009");
      break;
    case 34:
      Script_ShowUIMessageFromID("E1046B010");
      break;
    case 35:
      Script_ShowUIMessageFromID("E1046B011");
      break;
    case 36:
      Script_ShowUIMessageFromID("E1046C001");
      break;
    case 37:
      Script_ShowUIMessageFromID("E1046C002");
      break;
  }
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_44625)))
static int32_t script_36DED(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F4[2];
  int32_t local_F0;
  SCNF_WaitFrames_0000(5);
  local_F4[0] = Script_scenCreateTask(script_361A7, 5, arg0, arg1, 0x3D343958, 0, &data_442DC);
  local_F4[1] = Script_scenCreateTask(script_361A7, 5, task_7CC[1], 1, 0x3D343958, 1, &data_4430C);
  Script_SetQteOverlayTimeoutTuning(0, QTE_TimeoutSession);
  script_35A0A(7);
  script_37C86(arg0, arg1);
  Script_ReturnTimeDate(&task_960);
  for (;;) {
    Script_charaDisplay(SHEN_NAME('DRM1'), CDM_Show);
    Script_charaDisplay(SHEN_NAME('DRM2'), CDM_Hide);
    local_FC = Script_scenCreateTask(script_3726F, 2, arg0, arg1);
    task_820 = 0;
    for (;;) {
      if (task_820 >= 12) {
        Script_srchControl((void *)local_FC);
        Script_SetQteOverlayTimeoutTuning(1, QTE_TimeoutSession);
        local_F0 = task_95C;
        switch (local_F0) {
          case 8:
            SCNF_WaitFrames_0000(10);
            script_35647(arg0, 0x45534142, 60.0f);
            script_35647(CHARA_NGSM, 0x45534142, 330.0f);
            script_36868(CHARA_GORO, 9, 5, 8, -1);
            script_36868(CHARA_GORO, 9, 3, 8, 30);
            script_36868(CHARA_GORO, 9, 2, 8, 60);
            script_36868(CHARA_GORO, 0, -1, 5, 120);
            Script_motmSetBlendFrames(CHARA_GORO, MOTM_BLEND_FRAMES_14);
            script_368E2(arg0, arg1, 20753, 0x420C0000, 0x3F800000, 0, -1);
            script_368E2(CHARA_GORO, 1, 20737, 0, 0x3F800000, 0, 15);
            script_368E2(CHARA_NGSM, 2, 20760, 0, 0x3F800000, 3, -1);
            script_368E2(CHARA_NGSM, 2, 20737, 0, 0x3F800000, 0, 140);
            script_36A96(35, -1);
            script_36A96(24, 75);
            SCNF_WaitFrames_0000(120);
            break;
          case 12:
            script_35647(arg0, 0x45534142, 60.0f);
            Script_scenCreateTask(script_34C59, 6, 20753, -1, 0x420C0000, 0x3F800000, arg0, arg1);
            break;
          case 20:
            SCNF_WaitFrames_0000(20);
            script_36868(CHARA_GORO, 9, 5, 8, -1);
            script_36868(CHARA_GORO, 0, -1, 5, 120);
            script_35647(arg0, 0x45534142, 270.0f);
            Script_motmSetBlendFrames(CHARA_GORO, MOTM_BLEND_FRAMES_14);
            Script_scenCreateTask(script_34C59, 6, 20737, 15, 0, 0x3F800000, CHARA_GORO, 1);
            Script_scenCreateTask(script_34C59, 6, 20753, -1, 0x420C0000, 0x3F800000, arg0, arg1);
            script_36A96(35, -1);
            script_36A96(24, 75);
            SCNF_WaitFrames_0000(120);
            break;
        }
        break;
      }
      if ((task_820 == -1) & (task_75C == 1)) {
        Script_SetQteOverlayTimeoutTuning(2, QTE_TimeoutSession);
        script_35A70();
        Script_uiUtils(0, 0, 0);
        Script_srchControl((void *)*(int32_t *)local_F4);
        Script_srchControl((void *)local_F4[1]);
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_GORO_QTE_COMPLETE, 1);
        Script_srchControl((void *)local_FC);
        SCNF_WaitFrames_0000(1);
        script_39932(task_75C);
        break;
      } else {
        if (task_820 == -1) {
          Script_SetQteOverlayTimeoutTuning(2, QTE_TimeoutSession);
          Script_srchControl((void *)local_FC);
          break;
        }
      }
      SCNF_WaitFrames_0000(1);
    }
    Script_executeAicaCommand(2720, 2, 30);
    Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_motmSetBlendFrames(CHARA_GORO, MOTM_BLEND_FRAMES_7);
    if (task_75C == 0) {
      Script_LoadSND(0, 4, "e1katkyu.snd", 0);
      for (;;) {
        Script_PrintDebugString(18, 14, "PRESS A");
        if (Script_GetInputData(0, 2) & 0x400) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    }
    if (task_75C == 1) {
      Script_uiUtils(0, 21, 0);
      Script_sicnUtils(0, 1, 1);
      SCNF_WaitFrames_0000(90);
      Script_sicnUtils(1, 0, 0);
      Script_uiUtils(0, 0, 0);
    }
    Script_SetNewDateTime(&task_960);
    Script_WeatherTaskUtil(FreeTaskData);
    Script_WeatherTaskUtil(InitTaskData);
    script_35A28();
    Script_executeAicaCommand(222376, 0, 0);
    Script_executeAicaCommand(1184, 2, 127);
    Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  }
  end:;
}

static int32_t script_3726F(int32_t arg0, int32_t arg1) {
  int8_t local_FF;
  uint8_t unused_F8[7];
  Script_charaDisplay(CHARA_MURI, CDM_Show);
  script_36500(43, -1);
  script_354C0(arg0, 0x45534142, 3.28f, 155.63f, 150.0f);
  script_354C0(task_7CC[1], 0x45534142, 3.0f, 158.0f, 350.0f);
  script_354C0(task_7CC[2], 0x45534142, 0.9f, 159.3f, 80.0f);
  script_354C0(CHARA_MURI, 0x45534142, 0.3f, 159.3f, 170.0f);
  script_36A96(1, -1);
  script_368E2(arg0, arg1, 89, 0, 0x3F800000, 3, -1);
  script_368E2(CHARA_GORO, 1, 20737, 0, 0x3F800000, 3, -1);
  script_368E2(CHARA_MURI, 3, 20771, 0, 0x3F800000, 3, -1);
  script_368E2(CHARA_NGSM, 2, 20774, 0x43C80000, 0x3F800000, 0, -1);
  script_3682E(8172, SHEN_NAME('BASE'), SHEN_NAME('BASE'), -1);
  script_38702();
  SCNF_WaitFrames_0000(60);
  script_369A9(CHARA_NGSM, CHARA_AKIR, 1, 100);
  script_36A02(CHARA_NGSM, 6, 300);
  script_36A02(CHARA_GORO, 6, -1);
  script_368E2(CHARA_AKIR, arg1, 904, 0, 0x3F800000, 0, 200);
  script_368E2(CHARA_NGSM, 2, 20773, 0, 0x3F800000, 3, 330);
  script_368E2(CHARA_MURI, 3, 20767, 0, 0x3F800000, 0, 370);
  script_368E2(CHARA_MURI, 3, 20766, 0, 0x3F800000, 0, 400);
  script_368E2(CHARA_MURI, 3, 20769, 0, 0x3F800000, 0, 460);
  script_368E2(CHARA_MURI, 3, 20768, 0, 0x3F800000, 0, 540);
  script_3682E(8167, SHEN_NAME('BASE'), SHEN_NAME('BASE'), -1);
  script_3682E(8168, SHEN_NAME('BASE'), SHEN_NAME('BASE'), 150);
  script_3682E(8175, SHEN_NAME('BASE'), SHEN_NAME('BASE'), 330);
  script_36A96(7, -1);
  script_36A96(2, 60);
  script_36A96(8, 150);
  script_36A96(28, 330);
  script_36A96(3, 360);
  SCNF_WaitFrames_0000(200);
  script_36214(1, 0.044f, &data_4430C);
  script_362CE(1, 0.044f);
  while (task_938[1] != 1) {
    SCNF_WaitFrames_0000(1);
  }
  script_36443(1);
  script_369A9(CHARA_AKIR, CHARA_GORO, 1, 1);
  script_3682E(8150, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
  script_3682E(8152, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 300);
  script_368E2(arg0, arg1, 89, 0, 0x3F800000, 3, -1);
  script_368E2(CHARA_GORO, 1, 20737, 0, 0x3F800000, 3, -1);
  script_368E2(CHARA_MURI, 3, 20771, 0, 0x3F800000, 3, -1);
  script_36868(arg0, 2, 0, 8, -1);
  script_36868(CHARA_GORO, 2, 1, 8, -1);
  script_35582(arg0, 0x45534142, 0x4053D70A, 0x431BA148, 0x4315E666, -1);
  script_35582(task_7CC[1], 0x45534142, 0x4047AE14, 0x431C0000, 0x43A48000, -1);
  script_35582(CHARA_MURI, 0x45534142, 0x3E4CCCCD, 0x431F4CCD, 0x432A0000, -1);
  SCNF_WaitFrames_0000(160);
  script_36868(CHARA_GORO, 0, -1, 5, -1);
  script_36868(CHARA_GORO, 0, -1, 5, -1);
  script_3682E(8176, SHEN_NAME('BASE'), SHEN_NAME('BASE'), -1);
  script_368E2(CHARA_AKIR, arg1, 904, 0, 0x3F800000, 0, 20);
  script_368E2(CHARA_GORO, 1, 36, 0, 0x3F800000, 0, -1);
  script_368E2(CHARA_NGSM, 2, 20768, 0, 0x3F800000, 3, -1);
  script_354C0(task_7CC[2], 0x45534142, 1.3f, 160.5f, 180.0f);
  SCNF_WaitFrames_0000(80);
  script_369A9(arg0, CHARA_MURI, 1, -1);
  script_368E2(CHARA_GORO, 1, 20760, 0, 0x3F800000, 0, -1);
  script_35647(task_7CC[1], 0x45534142, 160.0f);
  script_36A96(9, -1);
  script_369A9(task_7CC[2], task_7CC[1], 1, -1);
  script_369A9(task_7CC[2], arg0, 1, 45);
  script_369A9(task_7CC[2], CHARA_MURI, 1, 90);
  script_36A02(task_7CC[2], 1, 130);
  SCNF_WaitFrames_0000(60);
  script_368E2(CHARA_GORO, 1, 20737, 0, 0x3F800000, 0, -1);
  SCNF_WaitFrames_0000(60);
  Script_PlayCharaMotion(CHARA_GORO, MN_AKI_AKI_KIDUKU_BACK, 20.0f, -1.0f, -1.0f, 1.0f, 0);
  script_34E0B(CHARA_GORO);
  script_36A02(arg0, 6, -1);
  script_354C0(arg0, 0x45534142, 3.28f, 155.63f, 150.0f);
  script_354C0(task_7CC[1], 0x45534142, 3.8f, 157.6f, 335.0f);
  script_368E2(arg0, arg1, 787, 0, 0x3F800000, 1, -1);
  Script_PlayCharaMotion(CHARA_GORO, 20737, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_354C0(arg0, 0x45534142, 4.58f, 154.43f, 150.0f);
  script_354C0(task_7CC[1], arg0, -0.4f, -2.2f, 185.0f);
  script_3682E(8174, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
  script_36500(3, -1);
  script_350B5(arg1, 0, 1, 0);
  if (task_814 == 0) {
    script_36500(5, 10);
    script_3682E(0x1FFF, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 18);
    SCNF_WaitFrames_0000(20);
    Script_scenCreateTask(script_34B5E, 2, 161451, 10);
    script_368E2(arg0, arg1, 20748, 0, 0x3F800000, 0, -1);
    script_368E2(CHARA_GORO, 1, 20755, 0x41C80000, 0x3F800000, 1, -1);
    script_36A96(11, -1);
    SCNF_WaitFrames_0000(50);
    script_36A96(12, 10);
    script_354C0(arg0, 0x45534142, 4.56f, 154.62f, 150.0f);
    script_354C0(task_7CC[1], arg0, -0.4f, -2.05f, 180.0f);
    script_3682E(8174, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
    script_36500(6, -1);
    script_350B5(arg1, 0, 1, 0);
    if (task_814 == 0) {
      script_36868(CHARA_GORO, 9, 5, 8, -1);
      script_36500(8, 10);
      SCNF_WaitFrames_0000(6);
      script_3682E(0x2000, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
      Script_motmToggleFlags(arg0, SCEN_Enable, 8);
      Script_motmToggleFlags(CHARA_GORO, SCEN_Enable, 8);
      local_FF = 0;
      while (local_FF < 3) {
        SCNF_WaitFrames_0000(14);
        Script_scenCreateTask(script_34B5E, 2, 30379, 11);
        Script_scenCreateTask(script_34C59, 6, 20752, -1, 0x41400000, 0x3F800000, arg0, arg1);
        Script_scenCreateTask(script_34C59, 6, 20759, -1, 0x41400000, 0x3F800000, CHARA_GORO, 1);
        local_FF = local_FF + 1;
      }
      Script_motmToggleFlags(arg0, SCEN_Disable, 8);
      Script_motmToggleFlags(CHARA_GORO, SCEN_Disable, 8);
      task_820 = 15;
      SCNF_WaitFrames_0000(1);
    } else {
      script_36500(7, 10);
      script_354C0(arg0, 0x45534142, 5.8f, 153.8f, 150.0f);
      script_354C0(task_7CC[1], arg0, 0.0f, -0.9f, 230.0f);
      script_3682E(8164, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
      script_36A96(10, -1);
      Script_scenCreateTask(script_36070, 3, SHEN_NAME('DRM2'), SHEN_NAME('DRM1'), 10);
      script_34E0B(arg0);
    }
  } else {
    script_36500(4, 10);
    script_354C0(arg0, 0x45534142, 5.8f, 153.8f, 150.0f);
    script_354C0(task_7CC[1], arg0, 0.0f, -0.9f, 230.0f);
    script_3682E(8164, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
    script_36A96(10, -1);
    Script_scenCreateTask(script_36070, 3, SHEN_NAME('DRM2'), SHEN_NAME('DRM1'), 10);
    script_34E0B(arg0);
  }
  script_387FB(arg0, arg1);
  script_388E3(arg0, arg1);
  script_368E2(arg0, arg1, 787, 0, 0x3F800000, 1, -1);
  SCNF_WaitFrames_0000(30);
  script_3818D(arg0, arg1);
  task_820 = -1;
  SCNF_WaitFrames_0000(30);
  end:;
}

static void script_37C86(int32_t arg0, int32_t arg1) {
  int32_t unused_FC;
  script_3682E(8200, SHEN_NAME('BASE'), SHEN_NAME('BASE'), -1);
  script_354C0(arg0, 0x45534142, -7.4f, 153.8f, 280.0f);
  script_354C0(task_7CC[1], 0x45534142, 3.1f, 158.0f, 90.0f);
  script_354C0(task_7CC[2], 0x45534142, 1.7f, 159.6f, 80.0f);
  script_354C0(CHARA_MURI, 0x45534142, 0.6f, 159.6f, 170.0f);
  script_354C0(SHEN_NAME('DRM1'), -1, -109.7f, 102.5f, 44.0f);
  script_354C0(SHEN_NAME('DRM2'), -1, -109.7f, 102.5f, 44.0f);
  script_368E2(CHARA_GORO, 1, 20737, 0, 0x3F800000, 3, -1);
  script_368E2(CHARA_NGSM, 2, 20774, 0, 0x3F800000, 3, -1);
  script_368E2(CHARA_MURI, 3, 20770, 0, 0x3F800000, 3, -1);
  Script_EVT_SetRotation(task_7CC[1], 0x38000000, &data_4463C, 0);
  SCNF_WaitFrames_0000(2);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  script_36500(42, -1);
  script_36A96(25, -1);
  script_36A96(26, 40);
  Script_PlayCharaMotion(CHARA_GORO, 20737, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_MURI, 20770, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_NGSM, 20774, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  script_369A9(arg0, CHARA_GORO, 1, 80);
  script_36214(arg1, 0.044f, &data_442DC);
  script_362CE(arg1, 0.044f);
  SCNF_WaitFrames_0000(120);
  script_36443(arg1);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_38702();
  script_36A96(0, -1);
  script_36A96(36, 50);
  script_36A96(27, 160);
  script_3682E(8200, SHEN_NAME('BASE'), SHEN_NAME('BASE'), -1);
  script_3682E(8166, SHEN_NAME('BASE'), SHEN_NAME('BASE'), 106);
  script_35582(arg0, 0x45534142, -0x3FD9999A, 0x4317999A, 0x437AB333, -1);
  script_368E2(arg0, arg1, 89, 0, 0x3F800000, 3, -1);
  script_368E2(CHARA_NGSM, 2, 20774, 0x42A00000, 0x3F800000, 0, 200);
  script_368E2(CHARA_MURI, 3, 20770, 0, 0x3F800000, 3, -1);
  script_369A9(arg0, CHARA_MURI, 1, -1);
  SCNF_WaitFrames_0000(215);
  SCNF_WaitFrames_0000(1);
  script_368E2(CHARA_NGSM, 2, 20774, 0x42A00000, 0x3F800000, 0, -1);
  script_354C0(arg0, 0x45534142, -0.7f, 153.42f, 204.4f);
  script_36A02(arg0, 6, 50);
  script_36214(arg1, 0.044f, &data_442F4);
  script_362CE(arg1, 0.044f);
  while (task_938[arg1] != 1) {
    SCNF_WaitFrames_0000(1);
  }
  script_36443(arg1);
  script_36A02(arg0, 6, -1);
  SCNF_WaitFrames_0000(30);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(20);
  return;
}

__attribute__((scn_string_bindings(0, data_44648, 1, data_44654, 2, data_4465F, 3, data_4466C)))
static void script_3809A(void) {
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, 0, "system1.snd", 0x8000);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadSND(1, 2, "bgm067.snd", 0x8000);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(222376, 0, 0);
  Script_executeAicaCommand(1184, 2, 127);
  Script_LoadSND(1, 4, "e1katkyu.snd", 0x8000);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadSND(1, 5, "qtebat.snd", 0x8000);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadSND(4, 0, 0, 0);
  Script_srchControl(0);
  return;
}

static void script_3818D(int32_t arg0, int32_t arg1) {
  float local_FC;
  float local_F8;
  local_FC = Script_LODUtils(LOD_UTILS_GET_DISTANCE_0, 0);
  local_F8 = Script_LODUtils(LOD_UTILS_GET_DISTANCE_2, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 150.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 150.0f);
  script_36500(44, -1);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_36868(CHARA_GORO, 11, 0, -1, -1);
  Script_charaDisplay(arg0, CDM_Hide);
  script_36A02(CHARA_GORO, 20, -1);
  script_369A9(arg0, CHARA_GORO, 1, 200);
  script_369A9(CHARA_GORO, arg0, 6, 800);
  script_36A02(CHARA_GORO, 1, 1050);
  script_369A9(CHARA_NGSM, CHARA_GORO, 1, 1080);
  script_369A9(CHARA_GORO, CHARA_NGSM, 1, 1140);
  script_36A02(CHARA_GORO, 1, 1230);
  script_36A02(CHARA_NGSM, 1, 1230);
  script_354C0(arg0, 0x45534142, 8.3f, 167.1f, 280.0f);
  script_354C0(task_7CC[1], arg0, -1.4f, -0.35f, 90.0f);
  script_35582(task_7CC[1], 0x45534142, 0x40F33333, 0x43230000, 0x435C0000, 985);
  script_35582(task_7CC[2], 0x45534142, 0x40B9999A, 0x43234CCD, 0x428C0000, 985);
  script_35582(task_7CC[2], 0x45534142, 0x406CCCCD, 0x431F999A, 0x42200000, 1080);
  script_3569B(arg0, 0x45534142, 0x41F00000, 1200);
  script_3569B(arg0, 0x45534142, 0x43520000, 1240);
  script_35755(CHARA_GORO, 0x43340000, 1080);
  script_35755(CHARA_NGSM, 0x43340000, 1080);
  script_3682E(8163, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
  script_3682E(8160, SHEN_NAME('BASE'), SHEN_NAME('BASE'), 125);
  script_3682E(8161, SHEN_NAME('BASE'), SHEN_NAME('BASE'), 325);
  script_3682E(8201, SHEN_NAME('BASE'), SHEN_NAME('BASE'), 540);
  script_3682E(8202, SHEN_NAME('BASE'), SHEN_NAME('BASE'), 700);
  script_3682E(8203, SHEN_NAME('BASE'), SHEN_NAME('BASE'), 985);
  script_3682E(8204, SHEN_NAME('BASE'), SHEN_NAME('BASE'), 1080);
  script_368E2(arg0, arg1, 20739, 0x43340000, 0x3F800000, 3, -1);
  script_368E2(arg0, arg1, 1076, 0, 0x3F800000, 3, 1200);
  script_368E2(arg0, arg1, 89, 0, 0x3F800000, -1, 1240);
  script_368E2(CHARA_GORO, 1, 20740, 0x43340000, 0x3F800000, 3, -1);
  script_368E2(CHARA_GORO, 1, 20783, 0x41200000, 0x3ECCCCCD, 3, 985);
  script_368E2(CHARA_GORO, 1, 724, 0, 0x3F4CCCCD, 3, 1080);
  script_368E2(CHARA_GORO, 1, 655, 0, 0x3F99999A, 0, 1110);
  script_368E2(CHARA_NGSM, 2, 20768, 0, 0x3F800000, 3, 985);
  script_368E2(CHARA_NGSM, 2, 655, 0, 0x3F4CCCCD, 3, 1080);
  script_36A02(arg0, 1, 980);
  Script_scenCreateTask(script_3868C, 1, 830);
  script_36A96(15, 80);
  script_36A96(16, 230);
  script_36A96(4, 330);
  script_36A96(17, 450);
  script_36A96(5, 560);
  script_36A96(18, 650);
  script_36A96(19, 700);
  script_36A96(20, 800);
  script_36A96(6, 880);
  script_36A96(21, 950);
  script_36868(CHARA_GORO, 7, 3, 230, -1);
  script_36868(CHARA_GORO, 9, 4, 450, 180);
  Script_ReadModelFormat7(CHARA_GORO, 0, data_44678, 10);
  SCNF_WaitFrames_0000(125);
  Script_charaDisplay(arg0, CDM_Show);
  SCNF_WaitFrames_0000(1100);
  script_36868(CHARA_GORO, 0, -1, 5, -1);
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  Script_executeAicaCommand(2720, 2, 60);
  SCNF_WaitFrames_0000(60);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, local_FC);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, local_F8);
  return;
}

static void script_38669(void) {
  for (;;) {
    script_369A9(CHARA_AKIR, CHARA_GORO, 1, -1);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_3868C(int32_t arg0) {
  if (arg0 > 0) {
    SCNF_WaitFrames_0000(arg0);
  }
  Script_SetCharaLookAtPosition(CHARA_AKIR, 2, &data_446A0, 1);
  return Script_srchControl(0);
}

static int32_t script_386B8(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  int16_t local_F2;
  local_F2 = 0;
  while (local_F2 < arg1) {
    Script_EVT_GetPosition(arg0, 17, &local_F4, 0);
    Script_SetMotnFlags(CHARA_GORO, 17, &local_F4, 0x38000000);
    local_F2 = local_F2 + 1;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_38702(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  Script_ccowUtils(SHEN_NAME('KEIB'), CCOW_DisableCharacters);
  Script_CharHandlers(CHARA_NGSM, 0, 0, 16);
  local_F4.x = 0.083f;
  (&local_F4)->y = 0.015f;
  (&local_F4)->z = -0.036f;
  local_E8.x = 0;
  (&local_E8)->y = 0;
  (&local_E8)->z = 0;
  Script_fixoAttachEx(SHEN_NAME('KEIB'), CHARA_NGSM, 18, &local_F4, &local_E8);
  return;
}

static void script_38786(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  SCNF_WaitFrames_0000(1);
  Script_ccowUtils(SHEN_NAME('KEIB'), CCOW_DisableCharacters);
  Script_CharHandlers(CHARA_NGSM, 0, 0, 16);
  local_F4.x = 0.083f;
  (&local_F4)->y = 0.015f;
  (&local_F4)->z = -0.036f;
  local_E8.x = 0;
  (&local_E8)->y = 0;
  (&local_E8)->z = 5097;
  Script_fixoAttachEx(SHEN_NAME('KEIB'), CHARA_NGSM, 18, &local_F4, &local_E8);
  return;
}

static void script_387FB(int32_t arg0, int32_t arg1) {
  script_3682E(8169, SHEN_NAME('BASE'), SHEN_NAME('BASE'), -1);
  script_354C0(task_7CC[2], 0x45534142, 2.6f, 158.1f, 320.0f);
  script_354C0(CHARA_MURI, 0x45534142, 1.2f, 158.1f, 45.0f);
  script_36A96(29, -1);
  script_369A9(task_7CC[2], task_7CC[3], 1, 90);
  script_368E2(CHARA_NGSM, 2, 20776, 0, 0x3F666666, 5, -1);
  script_368E2(CHARA_MURI, 3, 20772, 0, 0x3F800000, 5, -1);
  script_36A96(37, 40);
  script_36A96(30, 110);
  SCNF_WaitFrames_0000(160);
  script_36A02(task_7CC[2], 1, -1);
  return;
}

static void script_388E3(int32_t arg0, int32_t arg1) {
  int8_t local_FF;
  int32_t local_F8;
  Script_charaDisplay(CHARA_MURI, CDM_Hide);
  script_36A96(31, -1);
  script_369A9(task_7CC[2], arg0, 1, 1);
  script_3682E(8153, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
  script_368E2(arg0, arg1, 787, 0, 0x3F800000, 1, -1);
  script_368E2(CHARA_NGSM, 2, 20737, 0, 0x3F800000, 3, -1);
  script_354C0(task_7CC[2], arg0, -1.0f, -2.1f, 180.0f);
  script_36500(9, -1);
  script_350B5(arg1, 1, 2, 0);
  if (task_814 == 0) {
    script_36A02(task_7CC[2], 1, -1);
    script_354C0(task_7CC[2], arg0, -0.5f, -1.4f, 160.0f);
    script_3682E(8156, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
    script_36500(11, 10);
    script_36A96(33, -1);
    SCNF_WaitFrames_0000(50);
    script_369A9(task_7CC[2], arg0, 1, -1);
    script_36A96(34, 10);
    script_354C0(task_7CC[2], arg0, -1.0f, -2.1f, 180.0f);
    script_368E2(CHARA_NGSM, 2, 20737, 0, 0x3F800000, 3, -1);
    script_3682E(8153, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
    script_36500(12, -1);
    script_350B5(arg1, 1, 2, 0);
    if (task_814 == 0) {
      script_36A02(task_7CC[2], 1, -1);
      script_354C0(task_7CC[2], arg0, -0.5f, -1.4f, 160.0f);
      Script_motmSetBlendFrames(CHARA_NGSM, MOTM_BLEND_FRAMES_10);
      script_3569B(CHARA_NGSM, 0x45534142, 0x41F00000, 40);
      script_368E2(CHARA_NGSM, 2, 20775, 0, 0x3F800000, 0, 40);
      script_368E2(arg0, arg1, 20765, 0x41A00000, 0x3F800000, 0, 10);
      script_3682E(8194, SHEN_NAME('BASE'), SHEN_NAME('BASE'), -1);
      script_36500(14, 10);
      Script_scenCreateTask(script_34B5E, 2, 0x2676AB, 60);
      SCNF_WaitFrames_0000(60);
      Script_motmSetBlendFrames(CHARA_NGSM, MOTM_BLEND_FRAMES_7);
      task_820 = 15;
      SCNF_WaitFrames_0000(1);
    } else {
      script_36A02(task_7CC[2], 1, -1);
      script_38786();
      script_36500(13, -1);
      script_3682E(8154, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
      script_354C0(task_7CC[1], 0x45534142, 4.8f, 152.7f, 180.0f);
    }
  } else {
    script_36A02(task_7CC[2], 1, -1);
    script_38786();
    script_36500(10, -1);
    script_3682E(8154, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
    script_354C0(task_7CC[1], 0x45534142, 4.8f, 152.7f, 180.0f);
  }
  Script_PlayCharaMotion(CHARA_GORO, 20737, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_34E0B(arg0);
  script_368E2(arg0, arg1, 787, 0, 0x3F800000, 1, -1);
  script_3682E(8157, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
  script_36868(CHARA_GORO, 5, 2, -1, 130);
  script_36A96(13, 45);
  SCNF_WaitFrames_0000(50);
  if (Script_PlayerStatsUtils(PS_GetStrayCatAffinityPoints) >= 1000) {
    local_F8 = Script_scenCreateTask(script_39446, 0);
  }
  SCNF_WaitFrames_0000(70);
  script_354C0(arg0, 0x45534142, 3.3f, 157.3f, 0.0f);
  script_354C0(task_7CC[1], arg0, 0.37f, -2.2f, 165.0f);
  script_354C0(task_7CC[2], 0x45534142, 4.5f, 159.4f, 330.0f);
  if (Script_PlayerStatsUtils(PS_GetStrayCatAffinityPoints) >= 1000) {
    Script_srchControl((void *)local_F8);
  }
  script_3682E(8190, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
  script_36500(15, -1);
  script_350B5(arg1, 2, 1, 0);
  if (task_814 == 0) {
    script_36500(17, 10);
    script_3682E(8158, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 10);
    SCNF_WaitFrames_0000(20);
    Script_scenCreateTask(script_34B5E, 2, 161451, 10);
    Script_scenCreateTask(script_34C59, 6, 20748, -1, 0, 0x3F800000, arg0, arg1);
    Script_scenCreateTask(script_34C59, 6, 20755, -1, 0x41C80000, 0x3F800000, CHARA_GORO, 1);
    script_36A96(22, -1);
    SCNF_WaitFrames_0000(80);
    script_368E2(arg0, arg1, 787, 0, 0x3F800000, 1, -1);
    script_368E2(CHARA_GORO, 1, 20737, 0, 0x3F800000, 1, -1);
    script_36A96(23, 10);
    script_354C0(task_7CC[1], arg0, -0.2f, -2.0f, 175.0f);
    script_3682E(8190, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
    script_36500(18, -1);
    script_350B5(arg1, 3, 1, 0);
    if (task_814 == 0) {
      script_36500(20, 10);
      script_36868(CHARA_GORO, 9, 2, 8, -1);
      script_3682E(8193, SHEN_NAME('BASE'), SHEN_NAME('BASE'), 10);
      SCNF_WaitFrames_0000(16);
      Script_motmToggleFlags(arg0, SCEN_Enable, 8);
      Script_motmToggleFlags(CHARA_GORO, SCEN_Enable, 8);
      local_FF = 0;
      while (local_FF < 3) {
        SCNF_WaitFrames_0000(14);
        Script_scenCreateTask(script_34B5E, 2, 30379, 11);
        Script_scenCreateTask(script_34C59, 6, 20752, -1, 0x41400000, 0x3F800000, arg0, arg1);
        Script_scenCreateTask(script_34C59, 6, 20759, -1, 0x41400000, 0x3F800000, CHARA_GORO, 1);
        local_FF = local_FF + 1;
      }
      Script_motmToggleFlags(arg0, SCEN_Disable, 8);
      Script_motmToggleFlags(CHARA_GORO, SCEN_Disable, 8);
      task_820 = 15;
      SCNF_WaitFrames_0000(1);
    } else {
      script_36500(19, 10);
      script_3682E(8162, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 30);
      script_36A96(14, 15);
      script_368A7(0x3E4CCCCD, 0x3DCCCCCD, 0x3F333333, 40);
      SCNF_WaitFrames_0000(60);
    }
  } else {
    script_36500(16, 10);
    if (task_958 == -1) {
      script_3909B(arg0, arg1);
    } else {
      Script_scenCreateTask(script_34B5E, 2, 161451, 96);
      Script_scenCreateTask(script_34B5E, 2, 0x1976AB, 6);
      Script_scenCreateTask(script_34B5E, 2, 0x2876AB, 56);
      script_3682E(8159, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 30);
      script_36A96(14, 15);
      script_368A7(0x3E4CCCCD, 0x3DCCCCCD, 0x3F333333, 57);
      SCNF_WaitFrames_0000(30);
      Script_scenCreateTask(script_386B8, 2, arg0, 70);
      SCNF_WaitFrames_0000(100);
    }
  }
  return;
}

static void script_3909B(int32_t arg0, int32_t arg1) {
  int32_t unused_EC[5];
  Script_CharHandlers(arg0, 0, 0, 16);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_motmToggleFlags(task_7CC[1], SCEN_Disable, 0x40);
  Script_SetFightProgramStateBytes(arg0, 8, 0);
  Script_GetDataFromFightProgram(arg0, 1);
  script_35582(task_7CC[1], arg0, 0, -0x3FFCCCCD, 0x43340000, -1);
  script_35582(arg0, 0x45534142, 0x40533333, 0x431D999A, 0, -1);
  script_3682E(8195, SHEN_NAME('BASE'), SHEN_NAME('BASE'), -1);
  script_3682E(8196, SHEN_NAME('BASE'), SHEN_NAME('BASE'), 10);
  script_3682E(8197, SHEN_NAME('BASE'), SHEN_NAME('BASE'), 23);
  script_368E2(arg0, arg1, 483, 0, 0x3F4CCCCD, 3, -1);
  script_368E2(arg0, arg1, 837, 0x41000000, 0x3F19999A, 4, 10);
  script_368E2(arg0, arg1, 20782, 0x40400000, 0x3F000000, 1, 23);
  script_368E2(task_7CC[1], 1, 20756, 0x41400000, 0x3F800000, 3, -1);
  script_368E2(task_7CC[1], 1, 803, 0, 0x3F4CCCCD, 3, 6);
  script_368E2(task_7CC[1], 1, 320, 0, 0x3F19999A, 0, 16);
  script_368E2(task_7CC[1], 1, 858, 0, 0x3F000000, 0, 28);
  Script_scenCreateTask(script_34B5E, 2, 30379, 6);
  Script_scenCreateTask(script_34B5E, 2, 95915, 16);
  Script_scenCreateTask(script_34B5E, 2, 0x1276AB, 28);
  SCNF_WaitFrames_0000(40);
  Script_SetFightProgramStateBytes(arg0, 8, 0);
  Script_GetDataFromFightProgram(arg0, 1);
  script_35582(task_7CC[1], arg0, 0, -0x4099999A, 0x43340000, -1);
  script_35582(arg0, 0x45534142, 0x404CCCCD, 0x431C199A, 0, -1);
  script_3682E(8198, SHEN_NAME('BASE'), SHEN_NAME('BASE'), -1);
  script_368E2(arg0, arg1, 20782, 0x40A00000, 0x3F800000, 3, -1);
  script_368E2(task_7CC[1], 1, 320, 0x41200000, 0x3FA8F5C3, 0, -1);
  script_368E2(task_7CC[1], 1, 858, 0, 0x3F800000, 3, 6);
  Script_scenCreateTask(script_34B5E, 2, 0x1276AB, 2);
  SCNF_WaitFrames_0000(9);
  Script_SetFightProgramStateBytes(arg0, 8, 0);
  Script_GetDataFromFightProgram(arg0, 1);
  script_35582(task_7CC[1], arg0, 0, -0x40666666, 0x43340000, -1);
  script_35582(arg0, 0x45534142, 0x404CCCCD, 0x431C199A, 0, -1);
  script_3682E(8199, SHEN_NAME('BASE'), SHEN_NAME('BASE'), -1);
  Script_scenCreateTask(script_34B5E, 2, 0x1276AB, 10);
  script_368E2(arg0, arg1, 20782, 0x40400000, 0x3E800000, 3, -1);
  script_368E2(task_7CC[1], 1, 320, 0x41200000, 0x3EA8F5C3, 3, -1);
  script_368E2(task_7CC[1], 1, 858, 0, 0x3E800000, 0, 20);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  Script_motmToggleFlags(task_7CC[1], SCEN_Enable, 0x40);
  SCNF_WaitFrames_0000(85);
  Script_GetDataFromFightProgram(arg0, 0);
  script_34E0B(arg0);
  Script_CharHandlers(arg0, 2, 0, 16);
  return;
}

static int32_t script_39446(void) {
  int8_t local_FF;
  uint8_t unused_FC[3];
  int32_t local_F8;
  int32_t local_F4;
  int8_t local_F3;
  uint8_t unused_E4[15];
  int32_t local_E0;
  int32_t local_DC;
  local_E0 = 0;
  local_FF = 0;
  (void)((task_958 = 0) & 1);
  local_F3 = 0;
  for (;;) {
    local_F8 = Script_GetControllerInput(0);
    if ((local_F8 != 0) & (local_F8 != local_F4)) {
      if (task_958 != -1) {
        if (local_F8 == *(&data_446AC + local_FF)) {
          local_F4 = local_F8;
          local_F3 = 0;
          local_FF = local_FF + 1;
          if (*(&data_446AC + local_FF) == -1) {
            (void)((task_958 = -1) & 0x80);
          }
        } else if (local_F8 & local_F4) {
        } else {
          local_F3 = 0;
          local_F4 = -1;
          local_FF = 0;
        }
      } else if (local_F8 & local_F4) {
      } else {
        (void)((task_958 = 0) & 0x80);
        break;
      }
      if (local_F8) {
        local_DC = local_F8;
      }
    }
    local_F3 = local_F3 + 1;
    if ((local_F3 > 4) & (task_958 == 0)) {
      local_F4 = -1;
      local_FF = 0;
      local_F3 = 0;
    }
    SCNF_WaitFrames_0000(1);
  }
  for (;;) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_3955A(void) {
  task_75C = 1;
  script_39666(task_75C);
  task_758 = 0;
  for (;;) {
    if (task_764 == 1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_446E0, 1, data_446EF, 2, data_446F4, 3, data_44703)))
static void script_3958D(void) {
  script_EAA8("scene/02/MFSY/", "MGQB");
  script_EAA8("scene/02/MFSY/", "DRUM");
  Script_EVT_SetPosition(SHEN_NAME('BASE'), POS_XYZ, &data_446C8);
  Script_EVT_SetRotation(SHEN_NAME('BASE'), 0x38000000, &data_446D4, 0);
  script_354C0(SHEN_NAME('DRM1'), -1, -108.0f, 102.5f, 28.0f);
  script_354C0(SHEN_NAME('DRM2'), -1, -108.0f, 102.5f, 28.0f);
  script_354C0(SHEN_NAME('C_D1'), 0x324D5244, -0.08f, -0.56f, 0.0f);
  script_354C0(SHEN_NAME('C_D2'), 0x324D5244, -2.19f, -0.72f, 0.0f);
  Script_scenToggleVisibleFor(SHEN_NAME('DRM1'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('C_D2'), 0);
  return;
}

__attribute__((scn_string_bindings(0, data_44720, 1, data_44726, 2, data_4472B, 3, data_44730, 4, data_44735, 5, data_4473A, 6, data_4473F, 7, data_4474B, 8, data_44756, 9, data_44763)))
static void script_39666(int32_t arg0) {
  Script_SetCameraFlags(0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_WALK_UP1_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetActionSelectorHUDElementVisibility(1);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  task_764 = 0;
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  task_76C = script_E948(CHARA_GORO);
  *(&task_76C + 1) = script_E948(CHARA_NGSM);
  *(&task_76C + 2) = script_E948(CHARA_MURI);
  Script_LoadVoiceFile(1, "E1046");
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  SCNF_WaitFrames_0000(1);
  Script_scenCreateTask(script_39C68, 4, CHARA_MURI, 3, 2, 2);
  Script_scenCreateTask(script_39C68, 4, CHARA_GORO, 1, 2, 2);
  Script_scenCreateTask(script_39C68, 4, CHARA_NGSM, 2, 2, 0);
  Script_scenCreateTask(script_39AC8, 2, CHARA_AKIR, 0);
  script_354C0(SHEN_NAME('DRM1'), -1, -109.7f, 102.5f, 44.0f);
  script_354C0(SHEN_NAME('DRM2'), -1, -109.7f, 102.5f, 44.0f);
  Script_charaDisplay(CHARA_MURI, CDM_Show);
  Script_charaDisplay(SHEN_NAME('KEIB'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('DRM1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('DRM2'), CDM_Hide);
  Script_LoadSND(0, 6, "FREE", 0);
  Script_LoadSND(0, 5, "FREE", 0);
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 3, "FREE", 0);
  Script_LoadSND(0, 2, "FREE", 0);
  Script_LoadSND(0, 0, "system1.snd", 0);
  Script_LoadSND(0, 2, "bgm067.snd", 0);
  Script_LoadSND(0, 4, "e1katkyu.snd", 0);
  Script_LoadSND(0, 5, "qtebat.snd", 0);
  Script_executeAicaCommand(222376, 0, 0);
  Script_executeAicaCommand(1184, 2, 127);
  Script_facemotUtils(CHARA_GORO, 0, 0);
  Script_FaceEx(CHARA_GORO, FACE_ExpressionEnabled, 0, 1);
  Script_FaceLookAtTargetUtils(CHARA_GORO, FACE_LookReset);
  task_760 = Script_scenCreateTask(script_36DED, 2, CHARA_AKIR, 0);
  Script_SetCameraSettings(5, 6, 1, 0);
  SCNF_WaitFrames_0000(1);
  return;
}

__attribute__((scn_string_bindings(0, data_4476E, 1, data_44774)))
static void script_39932(int32_t arg0) {
  script_354C0(SHEN_NAME('DRM2'), -1, -107.4f, 102.5f, 28.0f);
  script_354C0(SHEN_NAME('C_D1'), 0x324D5244, -0.08f, -0.56f, 0.0f);
  script_354C0(SHEN_NAME('C_D2'), 0x324D5244, -2.19f, -0.72f, 0.0f);
  Script_MoveObjectToObject(SHEN_NAME('DUM0'), -1);
  Script_MoveObjectToObject(SHEN_NAME('DUM1'), -1);
  Script_MoveObjectToObject(SHEN_NAME('DUM2'), -1);
  Script_MoveObjectToObject(CHARA_AKIR, -1);
  Script_MoveObjectToObject(CHARA_GORO, -1);
  Script_MoveObjectToObject(CHARA_NGSM, -1);
  Script_scenToggleVisibleFor(SHEN_NAME('DRM2'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('C_D2'), 1);
  SCNF_WaitFrames_0000(1);
  task_764 = 1;
  SCNF_WaitFrames_0000(1);
  Script_SetCameraFlags(1);
  Script_SetActionSelectorHUDElementVisibility(0);
  Script_CharHandlers(CHARA_AKIR, 2, 0, 16);
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  Script_LoadVoiceFile(0, "E1046");
  Script_FaceEx(CHARA_GORO, FACE_ExpressionDisabled, 0, 1);
  Script_faceUpdateFor(CHARA_GORO, 0, 0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  script_EA10(CHARA_GORO, *&task_76C);
  script_EA10(CHARA_NGSM, *(&task_76C + 1));
  script_EA10(CHARA_MURI, *(&task_76C + 2));
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "MGQB");
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_srchControl((void *)task_760);
  SCNF_WaitFrames_0000(1);
  return;
}

static int32_t script_39AC8(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = Script_scenCreateTask(script_352BB, 3, arg0, arg1, 0);
  local_F8 = Script_scenCreateTask(script_34E27, 6, arg0, arg1, 0, 2, &data_441E8, script_35BC4);
  Script_charaDisplay(arg0, CDM_Show);
  Script_ryoSetChara(arg0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_7);
  Script_SetCPAMFlags(arg0, 0);
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 1);
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_EVT_SetPosition(SHEN_NAME('BASE'), POS_XYZ, &data_44794);
  Script_EVT_SetRotation(SHEN_NAME('BASE'), 0x38000000, &data_447A0, 0);
  Script_MoveObjectToObject(SHEN_NAME('DUM0'), arg0);
  Script_EVT_SetPosition(SHEN_NAME('DUM0'), POS_XYZ, &data_4477C);
  Script_EVT_SetRotation(SHEN_NAME('DUM0'), 0x38000000, &data_44788, 0);
  Script_MoveObjectToObject(SHEN_NAME('DUM1'), 0x4F524F47);
  Script_EVT_SetPosition(SHEN_NAME('DUM1'), POS_XYZ, &data_4477C);
  Script_EVT_SetRotation(SHEN_NAME('DUM1'), 0x38000000, &data_44788, 0);
  Script_MoveObjectToObject(SHEN_NAME('DUM2'), 0x4D53474E);
  Script_EVT_SetPosition(SHEN_NAME('DUM2'), POS_XYZ, &data_4477C);
  Script_EVT_SetRotation(SHEN_NAME('DUM2'), 0x38000000, &data_44788, 0);
  Script_SetCharaFlags(arg0, 4);
  Script_MoveObjectToObject(arg0, 0x45534142);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (task_764 == 1) {
      Script_ccowUtils(arg0, CCOW_EnableAll);
      Script_srchControl((void *)local_F8);
      Script_srchControl((void *)local_FC);
      Script_srchControl((void *)task_7FC[arg1]);
      Script_srchControl(0);
    }
  }
  end:;
}

static int32_t script_39C68(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_FC = Script_scenCreateTask(script_352BB, 3, arg0, arg1, 0);
  local_F8 = Script_scenCreateTask(script_34E27, 6, arg0, arg1, 0, 2, &data_441E8, script_35BC4);
  Script_charaDisplay(arg0, CDM_Show);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_7);
  Script_SetCharaFlags(arg0, 4);
  Script_SetCPAMFlags(arg0, 0);
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_MoveObjectToObject(arg0, 0x45534142);
  Script_charaSetRefboardFlagTo(arg0, 1);
  local_F4 = Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Read);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (task_764 == 1) {
      Script_motmGetCharaFlags(arg0, local_F4, SCEN_Write);
      Script_MoveObjectToObject(arg0, -1);
      Script_charaDisplay(arg0, CDM_Hide);
      Script_srchControl((void *)local_F8);
      Script_srchControl((void *)local_FC);
      Script_srchControl((void *)task_7FC[arg1]);
      Script_srchControl(0);
    }
  }
  end:;
}

static void script_39D5C(void) {
  for (;;) {
    Script_CycleManagerUtils(CYCLE_CMD_QUEUE_INTERSECTION_ENTRY, -0x3D246666, 0, 0x42CCCCCD, 0x42300000, 0x3F99999A, 0x3F866666, 0);
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_GORO_QTE_COMPLETE)) {
      Script_CycleManagerUtils(CYCLE_CMD_QUEUE_INTERSECTION_ENTRY, -0x3D206666, 0, 0x42CF0000, 0x42300000, 0x3ECCCCCD, 0x3F4CCCCD, 0);
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}
