#include "act80_sypf.h"

void script_0030(void) {
  Script_seqEnterEvtCam(700, 0, AUTH_None);
  Script_SetCurrentScrollEnabled(0);
  Script_SetFogFlags(2, 0);
  task_8 = 10000;
  Script_motiLoad(80, SHEN_NAME('SYPF'), "M_SYP.BIN");
  Script_LoadVoiceFile(1, "NET");
  Script_scenToggleActiveFor(SHEN_NAME('SYUE'), 0);
  Script_PREP_Related(-1, 1);
  Script_charaDisplay(SHEN_NAME('SYUE'), CDM_Show);
  Script_facemotUtils(SHEN_NAME('SYUE'), 0, 0);
  Script_FaceEx(SHEN_NAME('SYUE'), FACE_ExpressionEnabled, 0, 2);
  Script_PlayCharaMotion(SHEN_NAME('SYUE'), 58893, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  task_20 = Script_scenCreateTask(script_0174, 1, 0x45555953);
  task_24 = Script_scenCreateTask(script_0B29, 0);
  Script_scenCreateTask(script_0C49, 0);
  Script_scenCreateTask(script_0BF6, 1, SHEN_NAME('SYUE'));
  task_1C = Script_scenCreateTask(script_08C8, 1, 0x45555953);
  Script_scenCreateTask(script_0554, 1, SHEN_NAME('SYUE'));
  Script_scenCreateTask(script_0413, 1, 0x45555953);
  Script_filmCheckOrSetTransitionUtil(20, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_0174(int32_t arg0) {
  task_C = 5;
  task_10 = 3;
  script_0EE3(1);
  for (;;) {
    task_4 = 0;
    Script_SetupLightFromTable(task_10);
    task_8 = 0;
    Script_ShowUIMessageFromID("NETB001");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("NETB002");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    SCNF_WaitFrames_0000(60);
    Script_ShowUIMessageFromID("NETB003");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("NETB004");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    SCNF_WaitFrames_0000(30);
    Script_ShowUIMessageFromID("NETB005");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    SCNF_WaitFrames_0000(30);
    Script_ShowUIMessageFromID("NETB006");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    SCNF_WaitFrames_0000(15);
    Script_ShowUIMessageFromID("NETB007");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    SCNF_WaitFrames_0000(30);
    Script_ShowUIMessageFromID("NETB008");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    SCNF_WaitFrames_0000(30);
    Script_ShowUIMessageFromID("NETB009");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("NETB010");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    SCNF_WaitFrames_0000(15);
    Script_ShowUIMessageFromID("NETB011");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("NETB012");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    SCNF_WaitFrames_0000(42);
    Script_ShowUIMessageFromID("NETB013");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("NETB014");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("NETB015");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    SCNF_WaitFrames_0000(60);
    SCNF_WaitFrames_0000(1);
    if (task_14 == 0) {
      Script_filmCheckOrSetTransitionUtil(20, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_shenRequestGameModuleTransition(80, SHEN_NAME('CHAF'), 0);
    }
    task_C = 5;
    if (task_10 == 4) {
      task_10 = 0;
    }
  }
  end:;
}

static int32_t script_0413(int32_t arg0) {
  int32_t local_FC;
  for (;;) {
    SCNF_WaitFrames_0000(1);
    local_FC = task_8;
    if ((local_FC == 0) | (local_FC == 2) | (local_FC == 720) | (local_FC == 1630) | (local_FC == 1960) | (local_FC == 2300) | (local_FC == 3600)) {
      Script_FACE(SHEN_NAME('SYUE'), 0, 0, 10);
    } else if ((local_FC == 4200) | (local_FC == 2860)) {
      Script_FACE(SHEN_NAME('SYUE'), 1, 0, 10);
    } else if ((local_FC == 1430) | (local_FC == 2510)) {
      Script_FACE(SHEN_NAME('SYUE'), 2, 0, 10);
    } else if (local_FC == 3400) {
      Script_FACE(SHEN_NAME('SYUE'), 3, 0, 10);
    } else if ((local_FC == 1050) | (local_FC == 1800)) {
      Script_FACE(SHEN_NAME('SYUE'), 10, 0, 10);
    } else if ((local_FC == 520) | (local_FC == 3830)) {
      Script_FACE(SHEN_NAME('SYUE'), 9, 0, 10);
    } else {
      switch (local_FC) {
        case 1950:
          Script_FACE(SHEN_NAME('SYUE'), 7, 0, 10);
          break;
        case 2150:
          Script_FACE(SHEN_NAME('SYUE'), 5, 0, 30);
          break;
      }
    }
  }
  end:;
}

static int32_t script_0554(int32_t arg0) {
  int32_t local_FC;
  for (;;) {
    SCNF_WaitFrames_0000(1);
    local_FC = task_8;
    if ((local_FC == 0) | (local_FC == 2150) | (local_FC == 2510)) {
      Script_FACE_LCG(arg0, FACE_BlinkClose, 4, 0);
      Script_FaceLookAtTargetUtils(arg0, FACE_LookAtPosition, &data_1064, 0);
      SCNF_WaitFrames_0000(4);
      Script_FACE_LCG(arg0, FACE_BlinkOpen, 2, 0);
      SCNF_WaitFrames_0000(1);
      Script_FACE_LCG(arg0, FACE_BlinkAutomatic, 0, 0);
    } else if ((local_FC == 502) | (local_FC == 720)) {
      Script_FACE_LCG(arg0, FACE_BlinkClose, 4, 0);
      Script_FaceLookAtTargetUtils(arg0, FACE_LookAtPosition, &data_1070, 0);
      SCNF_WaitFrames_0000(4);
      Script_FACE_LCG(arg0, FACE_BlinkOpen, 2, 0);
      SCNF_WaitFrames_0000(1);
      Script_FACE_LCG(arg0, FACE_BlinkAutomatic, 0, 0);
    } else {
      switch (local_FC) {
        case 1052:
          Script_FACE_LCG(arg0, FACE_BlinkClose, 4, 0);
          Script_FaceLookAtTargetUtils(arg0, FACE_LookAtPosition, &data_107C, 0);
          SCNF_WaitFrames_0000(4);
          Script_FACE_LCG(arg0, FACE_BlinkOpen, 2, 0);
          SCNF_WaitFrames_0000(1);
          Script_FACE_LCG(arg0, FACE_BlinkAutomatic, 0, 0);
          break;
        case 1430:
          Script_FACE_LCG(arg0, FACE_BlinkClose, 4, 0);
          Script_FaceLookAtTargetUtils(arg0, FACE_LookAtPosition, &data_1088, 0);
          SCNF_WaitFrames_0000(4);
          task_4 = 1;
          Script_FACE_LCG(arg0, FACE_BlinkOpen, 2, 0);
          SCNF_WaitFrames_0000(1);
          Script_FACE_LCG(arg0, FACE_BlinkAutomatic, 0, 0);
          break;
        case 1630:
          task_4 = 0;
          Script_FACE_LCG(arg0, FACE_BlinkClose, 4, 0);
          Script_FaceLookAtTargetUtils(arg0, FACE_LookAtPosition, &data_107C, 0);
          SCNF_WaitFrames_0000(4);
          Script_FACE_LCG(arg0, FACE_BlinkOpen, 2, 0);
          SCNF_WaitFrames_0000(1);
          Script_FACE_LCG(arg0, FACE_BlinkAutomatic, 0, 0);
          break;
        case 2500:
          Script_FACE_LCG(arg0, FACE_BlinkClose, 4, 0);
          SCNF_WaitFrames_0000(4);
          task_4 = 1;
          Script_FACE_LCG(arg0, FACE_BlinkOpen, 2, 0);
          SCNF_WaitFrames_0000(1);
          Script_FACE_LCG(arg0, FACE_BlinkAutomatic, 0, 0);
          break;
        case 3200:
          Script_FACE_LCG(arg0, FACE_BlinkClose, 4, 0);
          Script_FaceLookAtTargetUtils(arg0, FACE_LookAtPosition, &data_10A0, 0);
          task_4 = 0;
          SCNF_WaitFrames_0000(4);
          Script_FACE_LCG(arg0, FACE_BlinkOpen, 2, 0);
          SCNF_WaitFrames_0000(1);
          Script_FACE_LCG(arg0, FACE_BlinkAutomatic, 0, 0);
          break;
        case 3400:
          Script_FACE_LCG(arg0, FACE_BlinkClose, 8, 0);
          Script_FaceLookAtTargetUtils(arg0, FACE_LookAtPosition, &data_107C, 0);
          SCNF_WaitFrames_0000(8);
          Script_FACE_LCG(arg0, FACE_BlinkOpen, 4, 0);
          SCNF_WaitFrames_0000(1);
          Script_FACE_LCG(arg0, FACE_BlinkAutomatic, 0, 0);
          break;
        case 3600:
          Script_FACE_LCG(arg0, FACE_BlinkClose, 4, 0);
          Script_FaceLookAtTargetUtils(arg0, FACE_LookAtPosition, &data_10A0, 0);
          SCNF_WaitFrames_0000(4);
          Script_FACE_LCG(arg0, FACE_BlinkOpen, 2, 0);
          SCNF_WaitFrames_0000(1);
          Script_FACE_LCG(arg0, FACE_BlinkAutomatic, 0, 0);
          break;
        case 3830:
          Script_FACE_LCG(arg0, FACE_BlinkClose, 4, 0);
          SCNF_WaitFrames_0000(4);
          task_4 = 1;
          Script_FACE_LCG(arg0, FACE_BlinkOpen, 2, 0);
          SCNF_WaitFrames_0000(1);
          Script_FACE_LCG(arg0, FACE_BlinkAutomatic, 0, 0);
          break;
      }
    }
  }
  end:;
}

static int32_t script_08C8(int32_t arg0) {
  for (;;) {
    if (task_C == 0) {
      int32_t local_FC;
      local_FC = task_8;
      switch (local_FC) {
        case 0:
          Script_seqEnterEvtCam(300, 0, AUTH_None);
          break;
        case 1084:
          Script_seqEnterEvtCam(301, 0, AUTH_None);
          break;
        case 1676:
          Script_seqEnterEvtCam(302, 0, AUTH_None);
          break;
        case 2276:
          Script_seqEnterEvtCam(303, 0, AUTH_None);
          break;
        case 3034:
          Script_seqEnterEvtCam(304, 0, AUTH_None);
          break;
        case 3724:
          Script_seqEnterEvtCam(305, 0, AUTH_None);
          break;
      }
    }
    if (task_C == 1) {
      int32_t local_FC;
      local_FC = task_8;
      switch (local_FC) {
        case 0:
          Script_seqEnterEvtCam(100, 0, AUTH_None);
          break;
        case 1682:
          Script_seqEnterEvtCam(101, 0, AUTH_None);
          break;
        case 2252:
          Script_seqEnterEvtCam(102, 0, AUTH_None);
          break;
        case 3002:
          Script_seqEnterEvtCam(103, 0, AUTH_None);
          break;
        case 3902:
          Script_seqEnterEvtCam(104, 0, AUTH_None);
          break;
      }
    }
    if ((task_C == 2) & (task_8 == 0)) {
      Script_scenCreateTask(script_0B42, 0);
    }
    if (task_C == 3) {
      int32_t local_FC;
      local_FC = task_8;
      switch (local_FC) {
        case 0:
          Script_seqEnterEvtCam(200, 0, AUTH_None);
          break;
        case 1034:
          Script_seqEnterEvtCam(201, 0, AUTH_None);
          break;
        case 1676:
          Script_seqEnterEvtCam(202, 0, AUTH_None);
          break;
        case 2276:
          Script_seqEnterEvtCam(203, 0, AUTH_None);
          break;
        case 3724:
          Script_seqEnterEvtCam(204, 0, AUTH_None);
          break;
      }
    }
    if ((task_C == 4) & (task_8 == 0)) {
      Script_seqEnterEvtCam(1000, 0, AUTH_None);
    }
    if (task_C == 5) {
      int32_t local_FC;
      local_FC = task_8;
      switch (local_FC) {
        case 0:
          Script_seqEnterEvtCam(700, 0, AUTH_None);
          break;
        case 1050:
          Script_seqEnterEvtCam(701, 0, AUTH_None);
          break;
        case 2300:
          Script_seqEnterEvtCam(702, 0, AUTH_None);
          break;
        case 3300:
          Script_seqEnterEvtCam(703, 0, AUTH_None);
          break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_0B29(void) {
  for (;;) {
    SCNF_WaitFrames_0000(1);
    task_8 = task_8 + 2;
  }
  end:;
}

static int32_t script_0B42(void) {
  for (;;) {
    Script_seqEnterEvtCam(602, 0, AUTH_None);
    SCNF_WaitFrames_0000(400);
    Script_seqEnterEvtCam(600, 0, AUTH_None);
    SCNF_WaitFrames_0000(350);
    Script_seqEnterEvtCam(601, 0, AUTH_None);
    SCNF_WaitFrames_0000(200);
    Script_seqEnterEvtCam(608, 0, AUTH_None);
    SCNF_WaitFrames_0000(300);
    Script_seqEnterEvtCam(604, 0, AUTH_None);
    SCNF_WaitFrames_0000(300);
    Script_seqEnterEvtCam(605, 0, AUTH_None);
    SCNF_WaitFrames_0000(200);
    Script_seqEnterEvtCam(606, 0, AUTH_None);
    SCNF_WaitFrames_0000(200);
    Script_seqEnterEvtCam(607, 0, AUTH_None);
    SCNF_WaitFrames_0000(200);
    Script_srchControl(0);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_0BF6(int32_t arg0) {
  Vector3 local_F4;
  int32_t local_F0;
  for (;;) {
    SCNF_WaitFrames_0000(1);
    local_F0 = task_4;
    switch (local_F0) {
      case 0:
        Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
        task_4 = -1;
        break;
      case 1:
        Script_GetAuthorState_And_SetCamPos(&local_F4, 0);
        Script_FaceLookAtTargetUtils(arg0, FACE_LookAtPosition, &local_F4, 0);
        break;
    }
  }
  end:;
}

static int32_t script_0C49(void) {
  int32_t local_FC;
  SCNF_WaitFrames_0000(20);
  for (;;) {
    local_FC = task_14;
    switch (local_FC) {
      case 0:
        if ((Script_GetInputData(0, 4) - 128) | (Script_GetInputData(0, 5) - 128) | Script_GetInputData(0, 0)) {
          task_28 = 0;
          Script_filmCheckOrSetTransitionUtil(20, 0, 0, 0, 0, 255, 0, 0, 0);
          while (Script_filmCheckOrSetTransitionUtil(-1)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_srchControl((void *)task_1C);
          Script_srchControl((void *)task_24);
          Script_srchControl((void *)task_20);
          while (task_28 > -199) {
            task_28 = task_28 - 10;
            Script_SetSequenceAudioVolume(0, 0, task_28);
            SCNF_WaitFrames_0000(1);
          }
          Script_StartOrFreeSEQ(0);
          SCNF_WaitFrames_0000(40);
          Script_SetNewCameraState(1);
          Script_filmCheckOrSetTransitionUtil(20, 255, 0, 0, 0, 0, 0, 0, 0);
          while (Script_filmCheckOrSetTransitionUtil(-1)) {
            SCNF_WaitFrames_0000(1);
          }
          task_20 = Script_scenCreateTask(script_0174, 1, 0x45555953);
          task_24 = Script_scenCreateTask(script_0B29, 0);
          task_18 = 0;
          task_14 = 1;
        }
        break;
      case 1:
        if ((Script_GetInputData(0, 4) - 128) | (Script_GetInputData(0, 5) - 128) | Script_GetInputData(0, 0)) {
          task_18 = 0;
        }
        if (task_18 > 900) {
          task_28 = 0;
          Script_filmCheckOrSetTransitionUtil(20, 0, 0, 0, 0, 255, 0, 0, 0);
          while (Script_filmCheckOrSetTransitionUtil(-1)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_srchControl((void *)task_24);
          Script_srchControl((void *)task_20);
          while (task_28 > -199) {
            task_28 = task_28 - 10;
            Script_SetSequenceAudioVolume(0, 0, task_28);
            SCNF_WaitFrames_0000(1);
          }
          Script_StartOrFreeSEQ(0);
          SCNF_WaitFrames_0000(40);
          SCNF_WaitFrames_0000(1);
          Script_SetNewCameraState(0);
          task_20 = Script_scenCreateTask(script_0174, 1, 0x45555953);
          task_24 = Script_scenCreateTask(script_0B29, 0);
          task_1C = Script_scenCreateTask(script_08C8, 1, 0x45555953);
          script_1028(1);
          Script_filmCheckOrSetTransitionUtil(20, 255, 0, 0, 0, 0, 0, 0, 0);
          while (Script_filmCheckOrSetTransitionUtil(-1)) {
            SCNF_WaitFrames_0000(1);
          }
          task_14 = 0;
        }
        task_18 = task_18 + 1;
        script_0F26(1);
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_0EE3(int32_t arg0) {
  Script_GetLightPosition(arg0, &task_2C);
  task_38.x = (&task_2C)->x;
  task_38.y = task_2C.y;
  task_38.z = task_2C.z;
  *(&task_44 + 1) = task_2C.y;
  return;
}

static void script_0F26(int32_t arg0) {
  *(&task_44 + 2) = task_2C.z - Script_MathUtils(SCRIPT_MATH_INT_TO_FLOAT, Script_GetInputData(0, 4) - 128) / 400.0f;
  *(&task_44 + 1) = task_2C.y - Script_MathUtils(SCRIPT_MATH_INT_TO_FLOAT, Script_GetInputData(0, 5) - 128) / 400.0f;
  if (!(*(&task_44 + 2) <= task_38.z)) {
    task_38.z = task_38.z + 0.01f;
  }
  if (!(*(&task_44 + 2) >= task_38.z)) {
    task_38.z = task_38.z - 0.01f;
  }
  if (!(*(&task_44 + 1) <= task_38.y)) {
    task_38.y = task_38.y + 0.01f;
  }
  if (!(*(&task_44 + 1) >= task_38.y)) {
    task_38.y = task_38.y - 0.01f;
  }
  Script_LightUtils(arg0, 7, &task_38);
  return;
}

static void script_1028(int32_t arg0) {
  Script_LightUtils(arg0, 7, &task_2C);
  return;
}
