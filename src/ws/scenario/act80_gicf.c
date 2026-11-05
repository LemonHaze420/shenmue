#include "act80_gicf.h"

void script_0030(void) {
  task_C = 100;
  task_34 = 0;
  Script_SetCurrentScrollEnabled(0);
  Script_SetFogFlags(0, data_1344);
  Script_SetFogFlags(4, 1);
  Script_FACE_LCG(CHARA_MARK, FACE_BlinkAutomatic, 0, 0);
  Script_charaDisplay(CHARA_MARK, CDM_Show);
  Script_facemotUtils(CHARA_MARK, 0, 0);
  Script_FaceEx(CHARA_MARK, FACE_ExpressionEnabled, 0, 0);
  Script_SetClipperFlag(0x4B52414D);
  Script_SetCPAMFlags(CHARA_MARK, 0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_LoadVoiceFile(1, "NET");
  Script_PREP_Related(-1, 1);
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  script_0F42(1);
  Script_scenCreateTask(script_00E6, 0);
  return;
}

static int32_t script_00E6(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  while (1) {
    local_F4 = task_34;
    switch (local_F4) {
      case 0:
        task_34 = 1;
        Script_seqEnterEvtCam(task_C, CHARA_MARK, CHARA_MARK);
        task_4 = Script_scenCreateTask(script_036F, 3, 0x4B52414D);
        Script_filmCheckOrSetTransitionUtil(20, 255, 0, 0, 0, 0, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        SCNF_WaitFrames_0000(20);
        break;
      case 1:
        if (Script_GetInputData(0, 1) | (Script_GetInputData(0, 4) - 128) | (Script_GetInputData(0, 5) - 128)) {
          task_34 = 2;
          local_FC = 0;
          while (local_FC > -199) {
            Script_SetSequenceAudioVolume(0, 0, local_FC = local_FC - 10);
            SCNF_WaitFrames_0000(1);
          }
          Script_StartOrFreeSEQ(0);
          Script_filmCheckOrSetTransitionUtil(20, 0, 0, 0, 0, 255, 0, 0, 0);
          while (Script_filmCheckOrSetTransitionUtil(-1)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_srchControl((void *)task_4);
          SCNF_WaitFrames_0000(40);
        }
        break;
      case 2:
        task_34 = 3;
        local_F8 = 0;
        Script_SetNewCameraState(1);
        task_8 = Script_scenCreateTask(script_036F, 3, 0x4B52414D);
        Script_filmCheckOrSetTransitionUtil(20, 255, 0, 0, 0, 0, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        break;
      case 3:
        while (1) {
          if (Script_GetInputData(0, 1) | (Script_GetInputData(0, 4) - 128) | (Script_GetInputData(0, 5) - 128)) {
            local_F8 = 0;
          } else {
            if (local_F8 / 30 >= 30) {
              local_FC = 0;
              task_34 = 0;
              while (local_FC > -199) {
                Script_SetSequenceAudioVolume(0, 0, local_FC = local_FC - 10);
                SCNF_WaitFrames_0000(1);
              }
              Script_StartOrFreeSEQ(0);
              Script_filmCheckOrSetTransitionUtil(20, 0, 0, 0, 0, 255, 0, 0, 0);
              while (Script_filmCheckOrSetTransitionUtil(-1)) {
                SCNF_WaitFrames_0000(1);
              }
              Script_srchControl((void *)task_8);
              Script_SetNewCameraState(0);
              script_1069(1);
              SCNF_WaitFrames_0000(40);
              break;
              goto L000343;
            }
          }
          L000343:;
          script_0F85(1);
          local_F8 = local_F8 + 1;
          SCNF_WaitFrames_0000(1);
        }
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_036F(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  while (1) {
    if ((task_34 == 0) | (task_34 == 2)) {
      break;
    }
    task_0 = 0;
    local_FC = Script_ShowUIMessageFromID("NETD001");
    while (task_0 < 10) {
      int8_t local_FB;
      local_FB = 0;
      Script_FACE(CHARA_MARK, data_10BC[task_0], 0, data_10C8[task_0]);
      Script_FACE_LCG(CHARA_MARK, data_10A4[task_0], data_10C8[task_0], data_10B0[task_0]);
      while (local_FB <= data_10C8[task_0]) {
        local_FB = local_FB + 1;
        SCNF_WaitFrames_0000(1);
      }
      task_0 = task_0 + 1;
    }
    SCNF_WaitFrames_0000(1);
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    if ((task_34 == 0) | (task_34 == 2)) {
      break;
    }
    task_0 = 0;
    local_FC = Script_ShowUIMessageFromID("NETD002");
    while (task_0 < 10) {
      int8_t local_FB;
      local_FB = 0;
      Script_FACE(CHARA_MARK, data_10EC[task_0], 0, data_10F8[task_0]);
      Script_FACE_LCG(CHARA_MARK, data_10D4[task_0], data_10F8[task_0], data_10E0[task_0]);
      while (local_FB <= data_10F8[task_0]) {
        local_FB = local_FB + 1;
        SCNF_WaitFrames_0000(1);
      }
      task_0 = task_0 + 1;
    }
    SCNF_WaitFrames_0000(1);
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    if ((task_34 == 0) | (task_34 == 2)) {
      break;
    }
    task_0 = 0;
    local_FC = Script_ShowUIMessageFromID("NETD003");
    while (task_0 < 10) {
      int8_t local_FB;
      local_FB = 0;
      Script_FACE(CHARA_MARK, data_111C[task_0], 0, data_1128[task_0]);
      Script_FACE_LCG(CHARA_MARK, data_1104[task_0], data_1128[task_0], data_1110[task_0]);
      while (local_FB <= data_1128[task_0]) {
        local_FB = local_FB + 1;
        SCNF_WaitFrames_0000(1);
      }
      task_0 = task_0 + 1;
    }
    SCNF_WaitFrames_0000(1);
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    if ((task_34 == 0) | (task_34 == 2)) {
      break;
    }
    task_0 = 0;
    local_FC = Script_ShowUIMessageFromID("NETD004");
    while (task_0 < 10) {
      int8_t local_FB;
      local_FB = 0;
      Script_FACE(CHARA_MARK, data_114C[task_0], 0, data_1158[task_0]);
      Script_FACE_LCG(CHARA_MARK, data_1134[task_0], data_1158[task_0], data_1140[task_0]);
      while (local_FB <= data_1158[task_0]) {
        local_FB = local_FB + 1;
        SCNF_WaitFrames_0000(1);
      }
      task_0 = task_0 + 1;
    }
    SCNF_WaitFrames_0000(1);
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    if ((task_34 == 0) | (task_34 == 2)) {
      break;
    }
    task_0 = 0;
    local_FC = Script_ShowUIMessageFromID("NETD005");
    while (task_0 < 10) {
      int8_t local_FB;
      local_FB = 0;
      Script_FACE(CHARA_MARK, data_117C[task_0], 0, data_1188[task_0]);
      Script_FACE_LCG(CHARA_MARK, data_1164[task_0], data_1188[task_0], data_1170[task_0]);
      while (local_FB <= data_1188[task_0]) {
        local_FB = local_FB + 1;
        SCNF_WaitFrames_0000(1);
      }
      task_0 = task_0 + 1;
    }
    SCNF_WaitFrames_0000(1);
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    if ((task_34 == 0) | (task_34 == 2)) {
      break;
    }
    task_0 = 0;
    local_FC = Script_ShowUIMessageFromID("NETD006");
    while (task_0 < 10) {
      int8_t local_FB;
      local_FB = 0;
      Script_FACE(CHARA_MARK, data_11AC[task_0], 0, data_11B8[task_0]);
      Script_FACE_LCG(CHARA_MARK, data_1194[task_0], data_11B8[task_0], data_11A0[task_0]);
      while (local_FB <= data_11B8[task_0]) {
        local_FB = local_FB + 1;
        SCNF_WaitFrames_0000(1);
      }
      task_0 = task_0 + 1;
    }
    SCNF_WaitFrames_0000(1);
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    if ((task_34 == 0) | (task_34 == 2)) {
      break;
    }
    task_0 = 0;
    local_FC = Script_ShowUIMessageFromID("NETD007");
    while (task_0 < 10) {
      int8_t local_FB;
      local_FB = 0;
      Script_FACE(CHARA_MARK, data_11DC[task_0], 0, data_11E8[task_0]);
      Script_FACE_LCG(CHARA_MARK, data_11C4[task_0], data_11E8[task_0], *((int8_t *)data_11D0 + task_0));
      while (local_FB <= data_11E8[task_0]) {
        local_FB = local_FB + 1;
        SCNF_WaitFrames_0000(1);
      }
      task_0 = task_0 + 1;
    }
    SCNF_WaitFrames_0000(1);
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    if ((task_34 == 0) | (task_34 == 2)) {
      break;
    }
    task_0 = 0;
    local_FC = Script_ShowUIMessageFromID("NETD008");
    while (task_0 < 10) {
      int8_t local_FB;
      local_FB = 0;
      Script_FACE(CHARA_MARK, data_120C[task_0], 0, data_1218[task_0]);
      Script_FACE_LCG(CHARA_MARK, *((int8_t *)data_11F4 + task_0), data_1218[task_0], data_1200[task_0]);
      while (local_FB <= data_1218[task_0]) {
        local_FB = local_FB + 1;
        SCNF_WaitFrames_0000(1);
      }
      task_0 = task_0 + 1;
    }
    SCNF_WaitFrames_0000(1);
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    if ((task_34 == 0) | (task_34 == 2)) {
      break;
    }
    task_0 = 0;
    local_FC = Script_ShowUIMessageFromID("NETD009");
    while (task_0 < 10) {
      int8_t local_FB;
      local_FB = 0;
      Script_FACE(CHARA_MARK, data_123C[task_0], 0, data_1248[task_0]);
      Script_FACE_LCG(CHARA_MARK, data_1224[task_0], data_1248[task_0], *((int8_t *)data_1230 + task_0));
      while (local_FB <= data_1248[task_0]) {
        local_FB = local_FB + 1;
        SCNF_WaitFrames_0000(1);
      }
      task_0 = task_0 + 1;
    }
    SCNF_WaitFrames_0000(1);
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    if ((task_34 == 0) | (task_34 == 2)) {
      break;
    }
    task_0 = 0;
    local_FC = Script_ShowUIMessageFromID("NETD010");
    while (task_0 < 10) {
      int8_t local_FB;
      local_FB = 0;
      Script_FACE(CHARA_MARK, data_126C[task_0], 0, data_1278[task_0]);
      Script_FACE_LCG(CHARA_MARK, data_1254[task_0], data_1278[task_0], data_1260[task_0]);
      while (local_FB <= data_1278[task_0]) {
        local_FB = local_FB + 1;
        SCNF_WaitFrames_0000(1);
      }
      task_0 = task_0 + 1;
    }
    SCNF_WaitFrames_0000(1);
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    if ((task_34 == 0) | (task_34 == 2)) {
      break;
    }
    task_0 = 0;
    local_FC = Script_ShowUIMessageFromID("NETD011");
    while (task_0 < 10) {
      int8_t local_FB;
      local_FB = 0;
      Script_FACE(CHARA_MARK, data_129C[task_0], 0, data_12A8[task_0]);
      Script_FACE_LCG(CHARA_MARK, data_1284[task_0], data_12A8[task_0], data_1290[task_0]);
      while (local_FB <= data_12A8[task_0]) {
        local_FB = local_FB + 1;
        SCNF_WaitFrames_0000(1);
      }
      task_0 = task_0 + 1;
    }
    SCNF_WaitFrames_0000(1);
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    if ((task_34 == 0) | (task_34 == 2)) {
      break;
    }
    task_0 = 0;
    local_FC = Script_ShowUIMessageFromID("NETD012");
    while (task_0 < 10) {
      int8_t local_FB;
      local_FB = 0;
      Script_FACE(CHARA_MARK, data_12CC[task_0], 0, data_12D8[task_0]);
      Script_FACE_LCG(CHARA_MARK, data_12B4[task_0], data_12D8[task_0], data_12C0[task_0]);
      while (local_FB <= data_12D8[task_0]) {
        local_FB = local_FB + 1;
        SCNF_WaitFrames_0000(1);
      }
      task_0 = task_0 + 1;
    }
    SCNF_WaitFrames_0000(1);
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    if ((task_34 == 0) | (task_34 == 2)) {
      break;
    }
    task_0 = 0;
    local_FC = Script_ShowUIMessageFromID("NETD013");
    while (task_0 < 10) {
      int8_t local_FB;
      local_FB = 0;
      Script_FACE(CHARA_MARK, data_12FC[task_0], 0, data_1308[task_0]);
      Script_FACE_LCG(CHARA_MARK, data_12E4[task_0], data_1308[task_0], data_12F0[task_0]);
      while (local_FB <= data_1308[task_0]) {
        local_FB = local_FB + 1;
        SCNF_WaitFrames_0000(1);
      }
      task_0 = task_0 + 1;
    }
    SCNF_WaitFrames_0000(1);
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    if ((task_34 == 0) | (task_34 == 2)) {
      break;
    }
    task_0 = 0;
    local_FC = Script_ShowUIMessageFromID("NETD014");
    while (task_0 < 10) {
      int8_t local_FB;
      local_FB = 0;
      Script_FACE(CHARA_MARK, data_132C[task_0], 0, data_1338[task_0]);
      Script_FACE_LCG(CHARA_MARK, *((int8_t *)data_1314 + task_0), data_1338[task_0], data_1320[task_0]);
      while (local_FB <= data_1338[task_0]) {
        local_FB = local_FB + 1;
        SCNF_WaitFrames_0000(1);
      }
      task_0 = task_0 + 1;
    }
    SCNF_WaitFrames_0000(1);
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    if (task_34 == 1) {
      Script_filmCheckOrSetTransitionUtil(20, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_shenRequestGameModuleTransition(80, SHEN_NAME('SYPF'), 0);
    }
    Script_AllocSpline(0);
    Script_seqEnterEvtCam(task_C, CHARA_MARK, CHARA_MARK);
  }
  end:;
}

static void script_0F42(int32_t arg0) {
  Script_GetLightPosition(arg0, &task_10);
  task_1C.x = (&task_10)->x;
  task_1C.y = task_10.y;
  task_1C.z = task_10.z;
  *(&task_28 + 1) = task_10.y;
  return;
}

static void script_0F85(int32_t arg0) {
  task_28 = (&task_10)->x + Script_MathUtils(SCRIPT_MATH_INT_TO_FLOAT, Script_GetInputData(0, 4) - 128) / 300.0f;
  *(&task_28 + 2) = task_10.z + Script_MathUtils(SCRIPT_MATH_INT_TO_FLOAT, Script_GetInputData(0, 5) - 128) / 300.0f;
  if (!(*&task_28 <= (&task_1C)->x)) {
    task_1C.x = (&task_1C)->x + 0.01f;
  }
  if (!(*&task_28 >= (&task_1C)->x)) {
    task_1C.x = (&task_1C)->x - 0.01f;
  }
  if (!(*(&task_28 + 2) <= task_1C.z)) {
    task_1C.z = task_1C.z + 0.01f;
  }
  if (!(*(&task_28 + 2) >= task_1C.z)) {
    task_1C.z = task_1C.z - 0.01f;
  }
  Script_LightUtils(arg0, 7, &task_1C);
  return;
}

static void script_1069(int32_t arg0) {
  Script_LightUtils(arg0, 7, &task_10);
  return;
}
