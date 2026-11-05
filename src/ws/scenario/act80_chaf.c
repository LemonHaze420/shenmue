#include "act80_chaf.h"

void script_0030(void) {
  task_2C = 1;
  task_30 = 1;
  task_34 = 1;
  Script_PREP_Related(-1, 1);
  Script_LoadVoiceFile(1, data_12AC);
  Script_scenCreateTask(script_00D3, 1, CHARA_CHAI);
  Script_scenCreateTask(script_078C, 1, 0x49414843);
  Script_scenCreateTask(script_074D, 1, 0x49414843);
  Script_scenCreateTask(script_0F0C, 1, 0x49414843);
  Script_scenCreateTask(script_1149, 0);
  Script_filmCheckOrSetTransitionUtil(80, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_12C8, 15, data_1340)))
static int32_t script_00D3(int32_t arg0) {
  int32_t local_FC;
  Script_charaDisplay(arg0, CDM_Show);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 2);
  Script_SetCameraFlags(1);
  Script_camSetRotationStepSize(3);
  Script_refbSetActive(arg0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    local_FC = task_2C;
    switch (local_FC) {
      case 1:
        Script_AllocSpline(0);
        if (task_34 == 1) {
          Script_AllocSpline(0);
          Script_seqEnterEvtCam(10, 0, AUTH_None);
        }
        task_24 = Script_ShowUIMessageFromID("NETC019");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        task_2C = task_2C + 1;
        task_30 = 1;
        break;
      case 2:
        task_24 = Script_ShowUIMessageFromID("NETC001");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        task_2C = task_2C + 1;
        task_30 = 1;
        break;
      case 3:
        if (task_34 == 1) {
          Script_seqEnterEvtCam(30, 0, AUTH_None);
          Script_AllocSpline(0);
        }
        task_24 = Script_ShowUIMessageFromID("NETC002");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        task_2C = task_2C + 1;
        task_30 = 1;
        break;
      case 4:
        if (task_34 == 1) {
          Script_seqEnterEvtCam(40, 0, AUTH_None);
          Script_AllocSpline(0);
        }
        task_24 = Script_ShowUIMessageFromID("NETC003");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        task_2C = task_2C + 1;
        task_30 = 1;
        break;
      case 5:
        if (task_34 == 1) {
          Script_seqEnterEvtCam(50, 0, AUTH_None);
          Script_AllocSpline(0);
        }
        task_24 = Script_ShowUIMessageFromID("NETC004");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        task_2C = task_2C + 1;
        task_30 = 1;
        break;
      case 6:
        if (task_34 == 1) {
          Script_seqEnterEvtCam(60, 0, AUTH_None);
          Script_AllocSpline(0);
        }
        task_24 = Script_ShowUIMessageFromID("NETC005");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        task_2C = task_2C + 1;
        task_30 = 1;
        break;
      case 7:
        if (task_34 == 1) {
          Script_seqEnterEvtCam(70, 0, AUTH_None);
          Script_AllocSpline(0);
        }
        task_24 = Script_ShowUIMessageFromID("NETC006");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        task_2C = task_2C + 1;
        task_30 = 1;
        break;
      case 8:
        if (task_34 == 1) {
          Script_seqEnterEvtCam(80, 0, AUTH_None);
          Script_AllocSpline(0);
        }
        task_24 = Script_ShowUIMessageFromID("NETC007");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        task_2C = task_2C + 1;
        task_30 = 1;
        break;
      case 9:
        if (task_34 == 1) {
          Script_seqEnterEvtCam(90, 0, AUTH_None);
          Script_AllocSpline(0);
        }
        task_24 = Script_ShowUIMessageFromID("NETC008");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        task_2C = task_2C + 1;
        task_30 = 1;
        break;
      case 10:
        if (task_34 == 1) {
          Script_seqEnterEvtCam(100, 0, AUTH_None);
          Script_AllocSpline(0);
        }
        task_24 = Script_ShowUIMessageFromID("NETC009");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        task_2C = task_2C + 1;
        task_30 = 1;
        break;
      case 11:
        if (task_34 == 1) {
          Script_seqEnterEvtCam(110, 0, AUTH_None);
          Script_AllocSpline(0);
        }
        task_24 = Script_ShowUIMessageFromID("NETC010");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        task_2C = task_2C + 1;
        task_30 = 1;
        break;
      case 12:
        if (task_34 == 1) {
          Script_seqEnterEvtCam(120, 0, AUTH_None);
          Script_AllocSpline(0);
        }
        task_24 = Script_ShowUIMessageFromID("NETC011");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        task_2C = task_2C + 1;
        task_30 = 1;
        break;
      case 13:
        if (task_34 == 1) {
          Script_seqEnterEvtCam(130, 0, AUTH_None);
          Script_AllocSpline(0);
        }
        task_24 = Script_ShowUIMessageFromID("NETC012");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        task_2C = task_2C + 1;
        task_30 = 1;
        break;
      case 14:
        if (task_34 == 1) {
          Script_seqEnterEvtCam(140, 0, AUTH_None);
          Script_AllocSpline(0);
        }
        task_24 = Script_ShowUIMessageFromID("NETC013");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        task_2C = task_2C + 1;
        task_30 = 1;
        break;
      case 15:
        if (task_34 == 1) {
          Script_seqEnterEvtCam(150, 0, AUTH_None);
          Script_AllocSpline(0);
        }
        task_24 = Script_ShowUIMessageFromID("NETC014");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        task_2C = task_2C + 1;
        task_30 = 1;
        break;
      case 16:
        if (task_34 == 1) {
          Script_seqEnterEvtCam(11, 0, AUTH_None);
          Script_AllocSpline(0);
        }
        task_24 = Script_ShowUIMessageFromID("NETC019");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        SCNF_WaitFrames_0000(60);
        task_30 = 1;
        task_2C = 1;
        if (task_38 == 0) {
          Script_filmCheckOrSetTransitionUtil(40, 0, 0, 0, 0, 255, 0, 0, 0);
          while (Script_filmCheckOrSetTransitionUtil(-1)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_shenRequestGameModuleTransition(80, SHEN_NAME('KOTF'), 0);
        }
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_0718(void) {
  for (;;) {
    SCNF_WaitFrames_0000(1);
    task_28 = task_28 + 2;
    if (task_28 == 2) {
      task_2C = task_2C + 1;
      task_30 = 1;
    }
  }
  return;
}

static int32_t script_074D(int32_t arg0) {
  for (;;) {
    SCNF_WaitFrames_0000(1);
    Script_scenCreateTask(script_0773, 1, CHARA_CHAI);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_0773(int32_t arg0) {
  Vector3 local_F4;
  Script_GetAuthorState_And_SetCamPos(&local_F4, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookAtPosition, &local_F4, 1);
  end:;
}

static int32_t script_078C(int32_t arg0) {
  int32_t local_FC;
  for (;;) {
    SCNF_WaitFrames_0000(1);
    local_FC = task_2C;
    switch (local_FC) {
      case 1:
        if (task_30 == 1) {
          task_30 = 0;
          Script_scenCreateTask(script_0A79, 1, CHARA_CHAI);
        }
        break;
      case 2:
        if (task_30 == 1) {
          task_30 = 0;
          Script_scenCreateTask(script_0B46, 1, 0x49414843);
        }
        break;
      case 3:
        if (task_30 == 1) {
          task_30 = 0;
          Script_scenCreateTask(script_0B49, 1, CHARA_CHAI);
        }
        break;
      case 4:
        if (task_30 == 1) {
          task_30 = 0;
          Script_scenCreateTask(script_0B62, 1, CHARA_CHAI);
        }
        break;
      case 5:
        if (task_30 == 1) {
          task_30 = 0;
          Script_scenCreateTask(script_0BA4, 1, CHARA_CHAI);
        }
        break;
      case 6:
        if (task_30 == 1) {
          task_30 = 0;
          Script_scenCreateTask(script_0C3A, 1, CHARA_CHAI);
        }
        break;
      case 7:
        if (task_30 == 1) {
          task_30 = 0;
          Script_scenCreateTask(script_0C67, 1, CHARA_CHAI);
        }
        break;
      case 8:
        if (task_30 == 1) {
          task_30 = 0;
          Script_scenCreateTask(script_0D3C, 1, CHARA_CHAI);
        }
        break;
      case 9:
        if (task_30 == 1) {
          task_30 = 0;
          Script_scenCreateTask(script_0D7E, 1, CHARA_CHAI);
        }
        break;
      case 10:
        if (task_30 == 1) {
          task_30 = 0;
          Script_scenCreateTask(script_0D96, 1, CHARA_CHAI);
        }
        break;
      case 11:
        if (task_30 == 1) {
          task_30 = 0;
          Script_scenCreateTask(script_0DAE, 1, CHARA_CHAI);
        }
        break;
      case 12:
        if (task_30 == 1) {
          task_30 = 0;
          Script_scenCreateTask(script_0DF1, 1, CHARA_CHAI);
        }
        break;
      case 13:
        if (task_30 == 1) {
          task_30 = 0;
          Script_scenCreateTask(script_0E1F, 1, CHARA_CHAI);
        }
        break;
      case 14:
        if (task_30 == 1) {
          task_30 = 0;
          Script_scenCreateTask(script_0E77, 1, CHARA_CHAI);
        }
        break;
      case 15:
        if (task_30 == 1) {
          task_30 = 0;
          Script_scenCreateTask(script_0EB9, 1, CHARA_CHAI);
        }
        break;
      case 16:
        if (task_30 == 1) {
          task_30 = 0;
          Script_scenCreateTask(script_0EFC, 1, CHARA_CHAI);
        }
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_0A79(int32_t arg0) {
  Script_FACE(arg0, 9, 1, 75);
  SCNF_WaitFrames_0000(75);
  Script_FACE(arg0, 1, 1, 5);
  SCNF_WaitFrames_0000(5);
  Script_FACE(arg0, 4, 1, 20);
  SCNF_WaitFrames_0000(20);
  Script_FACE(arg0, 2, 1, 5);
  SCNF_WaitFrames_0000(5);
  Script_FACE(arg0, 1, 1, 35);
  SCNF_WaitFrames_0000(35);
  Script_FACE(arg0, 7, 1, 5);
  SCNF_WaitFrames_0000(5);
  Script_FACE(arg0, 1, 1, 10);
  SCNF_WaitFrames_0000(10);
  Script_FACE(arg0, 2, 1, 50);
  SCNF_WaitFrames_0000(50);
  Script_FACE(arg0, 7, 1, 5);
  SCNF_WaitFrames_0000(5);
  Script_FACE(arg0, 8, 1, 30);
  end:;
}

static int32_t script_0B46(int32_t arg0) {
  end:;
}

static int32_t script_0B49(int32_t arg0) {
  SCNF_WaitFrames_0000(140);
  Script_FACE(arg0, 7, 1, 5);
  end:;
}

static int32_t script_0B62(int32_t arg0) {
  SCNF_WaitFrames_0000(5);
  Script_FACE(arg0, 8, 1, 5);
  SCNF_WaitFrames_0000(95);
  Script_FACE(arg0, 1, 1, 40);
  SCNF_WaitFrames_0000(40);
  Script_FACE(arg0, 4, 1, 10);
  end:;
}

static int32_t script_0BA4(int32_t arg0) {
  SCNF_WaitFrames_0000(10);
  Script_FACE(arg0, 9, 1, 20);
  SCNF_WaitFrames_0000(20);
  Script_FACE(arg0, 4, 1, 30);
  SCNF_WaitFrames_0000(80);
  Script_FACE(arg0, 7, 1, 5);
  SCNF_WaitFrames_0000(55);
  Script_FACE(arg0, 1, 1, 30);
  SCNF_WaitFrames_0000(25);
  Script_FACE(arg0, 7, 1, 30);
  SCNF_WaitFrames_0000(50);
  Script_FACE(arg0, 1, 1, 30);
  SCNF_WaitFrames_0000(30);
  Script_FACE(arg0, 7, 1, 30);
  end:;
}

static int32_t script_0C3A(int32_t arg0) {
  SCNF_WaitFrames_0000(45);
  Script_FACE(arg0, 9, 1, 25);
  SCNF_WaitFrames_0000(25);
  Script_FACE(arg0, 4, 1, 30);
  end:;
}

static int32_t script_0C67(int32_t arg0) {
  SCNF_WaitFrames_0000(50);
  Script_FACE(arg0, 1, 1, 30);
  SCNF_WaitFrames_0000(50);
  Script_FACE(arg0, 9, 1, 5);
  SCNF_WaitFrames_0000(5);
  Script_FACE(arg0, 7, 1, 3);
  SCNF_WaitFrames_0000(2);
  Script_FACE(arg0, 2, 1, 1);
  SCNF_WaitFrames_0000(48);
  Script_FACE(arg0, 4, 1, 10);
  SCNF_WaitFrames_0000(10);
  Script_FACE(arg0, 2, 1, 20);
  SCNF_WaitFrames_0000(30);
  Script_FACE(arg0, 9, 1, 15);
  SCNF_WaitFrames_0000(20);
  Script_FACE(arg0, 7, 1, 5);
  SCNF_WaitFrames_0000(5);
  Script_FACE(arg0, 4, 1, 5);
  SCNF_WaitFrames_0000(5);
  Script_FACE(arg0, 9, 1, 30);
  end:;
}

static int32_t script_0D3C(int32_t arg0) {
  SCNF_WaitFrames_0000(45);
  Script_FACE(arg0, 2, 1, 5);
  SCNF_WaitFrames_0000(5);
  Script_FACE(arg0, 7, 1, 4);
  SCNF_WaitFrames_0000(4);
  Script_FACE(arg0, 2, 1, 5);
  end:;
}

static int32_t script_0D7E(int32_t arg0) {
  SCNF_WaitFrames_0000(100);
  Script_FACE(arg0, 9, 1, 30);
  end:;
}

static int32_t script_0D96(int32_t arg0) {
  SCNF_WaitFrames_0000(100);
  Script_FACE(arg0, 8, 1, 30);
  end:;
}

static int32_t script_0DAE(int32_t arg0) {
  SCNF_WaitFrames_0000(150);
  Script_FACE(arg0, 7, 1, 30);
  SCNF_WaitFrames_0000(50);
  Script_FACE(arg0, 8, 1, 5);
  SCNF_WaitFrames_0000(5);
  Script_FACE(arg0, 2, 1, 30);
  end:;
}

static int32_t script_0DF1(int32_t arg0) {
  SCNF_WaitFrames_0000(145);
  Script_FACE(arg0, 7, 1, 30);
  SCNF_WaitFrames_0000(50);
  Script_FACE(arg0, 2, 1, 30);
  end:;
}

static int32_t script_0E1F(int32_t arg0) {
  SCNF_WaitFrames_0000(75);
  Script_FACE(arg0, 7, 1, 5);
  SCNF_WaitFrames_0000(25);
  Script_FACE(arg0, 5, 1, 5);
  SCNF_WaitFrames_0000(5);
  Script_FACE(arg0, 1, 1, 30);
  SCNF_WaitFrames_0000(145);
  Script_FACE(arg0, 7, 1, 2);
  end:;
}

static int32_t script_0E77(int32_t arg0) {
  SCNF_WaitFrames_0000(5);
  Script_FACE(arg0, 9, 1, 5);
  SCNF_WaitFrames_0000(40);
  Script_FACE(arg0, 2, 1, 20);
  SCNF_WaitFrames_0000(50);
  Script_FACE(arg0, 9, 1, 30);
  end:;
}

static int32_t script_0EB9(int32_t arg0) {
  SCNF_WaitFrames_0000(55);
  Script_FACE(arg0, 2, 1, 30);
  SCNF_WaitFrames_0000(145);
  Script_FACE(arg0, 9, 1, 5);
  SCNF_WaitFrames_0000(5);
  Script_FACE(arg0, 4, 1, 30);
  end:;
}

static int32_t script_0EFC(int32_t arg0) {
  Script_FACE(arg0, 9, 1, 15);
  end:;
}

static int32_t script_0F0C(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  task_38 = 0;
  local_FC = 0;
  local_F8 = 0;
  SCNF_WaitFrames_0000(20);
  while (1) {
    SCNF_WaitFrames_0000(1);
    local_F4 = task_38;
    switch (local_F4) {
      case 0:
        break;
      case 1:
        task_38 = 2;
        while (local_FC > -999) {
          Script_SetSequenceAudioVolume(0, (void *)task_24, local_FC = local_FC - 50);
          SCNF_WaitFrames_0000(1);
        }
        Script_StartOrFreeSEQ(0);
        Script_filmCheckOrSetTransitionUtil(20, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        break;
      case 2:
        task_38 = 3;
        local_FC = 0;
        local_F8 = 0;
        Script_SetNewCameraState(1);
        Script_filmCheckOrSetTransitionUtil(20, 255, 0, 0, 0, 0, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        break;
      case 3:
        task_2C = 1;
        while (1) {
          if ((Script_GetInputData(0, 4) - 128) | (Script_GetInputData(0, 5) - 128) | Script_GetInputData(0, 0)) {
            local_F8 = 0;
          } else {
            if (local_F8 / 30 >= 30) {
              task_2C = -1;
              task_38 = 0;
              while (local_FC > -999) {
                Script_SetSequenceAudioVolume(0, (void *)task_24, local_FC = local_FC - 50);
                SCNF_WaitFrames_0000(1);
              }
              Script_StartOrFreeSEQ(0);
              Script_filmCheckOrSetTransitionUtil(20, 0, 0, 0, 0, 255, 0, 0, 0);
              while (Script_filmCheckOrSetTransitionUtil(-1)) {
                SCNF_WaitFrames_0000(1);
              }
              Script_SetNewCameraState(0);
              task_2C = 1;
              Script_scenCreateTask(script_1270, 0);
              task_34 = 1;
              SCNF_WaitFrames_0000(40);
              Script_filmCheckOrSetTransitionUtil(20, 255, 0, 0, 0, 0, 0, 0, 0);
              while (Script_filmCheckOrSetTransitionUtil(-1)) {
                SCNF_WaitFrames_0000(1);
              }
              break;
              goto L00111B;
            }
          }
          L00111B:;
          Script_scenCreateTask(script_118C, 0);
          local_F8 = local_F8 + 1;
          SCNF_WaitFrames_0000(1);
        }
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static float script_1149(void) {
  Script_GetLightPosition(1, &task_0);
  task_C.x = (&task_0)->x;
  task_C.y = task_0.y;
  task_C.z = task_0.z;
  return *(&task_18 + 1) = task_0.y;
}

static int32_t script_118C(void) {
  task_18 = (&task_0)->x + Script_MathUtils(SCRIPT_MATH_INT_TO_FLOAT, Script_GetInputData(0, 4) - 128) / 300.0f;
  *(&task_18 + 2) = task_0.z + Script_MathUtils(SCRIPT_MATH_INT_TO_FLOAT, Script_GetInputData(0, 5) - 128) / 300.0f;
  if (!(*&task_18 <= (&task_C)->x)) {
    task_C.x = (&task_C)->x + 0.01f;
  }
  if (!(*&task_18 >= (&task_C)->x)) {
    task_C.x = (&task_C)->x - 0.01f;
  }
  if (!(*(&task_18 + 2) <= task_C.z)) {
    task_C.z = task_C.z + 0.01f;
  }
  if (!(*(&task_18 + 2) >= task_C.z)) {
    task_C.z = task_C.z - 0.01f;
  }
  Script_LightUtils(1, 7, &task_C);
  end:;
}

static int32_t script_1270(void) {
  Script_LightUtils(1, 7, &task_0);
  end:;
}
