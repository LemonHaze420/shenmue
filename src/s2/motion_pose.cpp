#include "s2/motion_pose.hpp"
#include "s2/motion_work.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/hand.hpp"
#include "s2/mt7.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>

namespace shenmue::s2 {
namespace {

constexpr float epsilon = 0.00000011920929f;
constexpr std::uint8_t invalidRig = 17;
constexpr std::uint8_t fallbackRig = 8;




constexpr float overrideRigConstants[69] = {
    1.100000023841858f,
    0,
    -.14100000262260437f,
    .09397999942302704f,
    0,
    0,
    -.00430000014603138f,
    .03590000048279762f,
    .06539999693632126f,
    .09299000352621078f,
    .0333000011742115f,
    -.014299999922513962f,
    0,
    0,
    -.05000000074505806f,
    .057100001722574234f,
    .05000000074505806f,
    .057100001722574234f,
    .03420000150799751f,
    -.039900001138448715f,
    -.06400000303983688f,
    -.00430000014603138f,
    .07599999755620956f,
    -.01759999990463257f,
    -.07859999686479568f,
    -.026499999687075615f,
    .12606999278068542f,
    .1594499945640564f,
    0,
    0,
    -.06400000303983688f,
    -.00430000014603138f,
    -.07599999755620956f,
    -.01759999990463257f,
    -.07859999686479568f,
    .011500000022351742f,
    .12606999278068542f,
    .1594499945640564f,
    0,
    0,
    0,
    .10499999672174454f,
    .1208299994468689f,
    -.05380000174045563f,
    .04729999974370003f,
    -.0820000022649765f,
    -.027000000700354576f,
    -.1581999957561493f,
    .006000000052154064f,
    .15931999683380127f,
    .11957000195980072f,
    0,
    0,
    -.05380000174045563f,
    .04729999974370003f,
    .0820000022649765f,
    -.027000000700354576f,
    -.1581999957561493f,
    .006000000052154064f,
    .15931999683380127f,
    .11957000195980072f,
    0,
    0,
    .03610000014305115f,
    .024800000712275505f,
    .024299999698996544f,
    -.011500000022351742f,
    .03099999949336052f,
    -.009999999776482582f,
};

// Shenmue2.exe g_anim_data_ptr and g_motion_const_data, normal-rig fields.
// Rows are: six retarget scales; chest/head; right arm; left arm; hips and
// left/right leg constants. Rig 16 is the separate 96-channel override rig.
constexpr float g_rigRetargetParameters[17][46] = {
    {.81f, .3977f, .1806f, .4686f, .7225f, .09f, .13f, .39773f, -.22f, .22f, .062f, .16f, 0, .045f, 0, 0, .103f, .25645f, .21214f, .17052f, .042f, .16f, 0,
        -.045f, 0, 0, -.103f, .25647f, .21211f, .17052f, .042f, .13f, 0, -.128f, .077f, .37553f, .34694f, -.066f, -.089f, 0, -.128f, -.077f, .37554f, .34694f,
        -.066f, -.089f},
    {.936f, .3237f, .178f, .4831f, .8455f, .09f, .162f, .32372f, -.233f, .233f, .05f, .186f, 0, .048f, 0, 0, .1f, .22596f, .25717f, .192f, .065f, .186f, 0,
        -.048f, 0, 0, -.1f, .22596f, .25717f, .192f, .065f, .162f, 0, -.163f, .078f, .36999f, .47556f, -.073f, -.1f, 0, -.163f, -.078f, .36999f, .47556f,
        -.073f, -.1f},
    {.841f, .4198f, .178f, .463f, .7584f, .09f, .1179f, .41978f, -.194f, .1939f, .0854f, .1598f, 0, .0635f, -.0188f, 0, .102f, .23345f, .22958f, .175f, .0545f,
        .1598f, 0, -.0635f, -.0188f, 0, -.102f, .23345f, .22959f, .175f, .0545f, .1179f, 0, -.1109f, .078f, .36123f, .39719f, -.06f, -.097f, 0, -.1109f, -.078f,
        .36123f, .39719f, -.06f, -.097f},
    {.429f, .2657f, .125f, .2958f, .3269f, .0893f, .07f, .26566f, -.14f, .156f, .047f, .114f, 0, .02f, -.012f, 0, .094f, .14377f, .15207f, .10634f, .045f,
        .114f, 0, -.02f, -.012f, 0, -.094f, .14377f, .15206f, .10633f, .045f, .07f, 0, -.083f, .046f, .15878f, .16811f, -.064f, -.064f, 0, -.083f, -.046f,
        .15878f, .16812f, -.064f, -.064f},
    {.578f, .2815f, .139f, .3793f, .473f, .0903f, .102f, .28147f, -.1471f, .1591f, .058f, .1131f, 0, .022f, -.0129f, 0, .103f, .20225f, .17711f, .103f, .0741f,
        .1131f, 0, -.022f, -.0129f, 0, -.103f, .20225f, .1771f, .103f, .0741f, .102f, 0, -.117f, .056f, .22975f, .24326f, -.057f, -.066f, 0, -.117f, -.056f,
        .22976f, .24327f, -.0569f, -.066f},
    {.718f, .3215f, .1461f, .4071f, .6285f, .0899f, .0985f, .32146f, -.1661f, .1652f, .0747f, .1308f, 0, .026f, -.0139f, 0, .134f, .22825f, .17887f, .1223f,
        .0565f, .1308f, 0, -.026f, -.0139f, 0, -.134f, .22825f, .17887f, .1223f, .0565f, .0985f, 0, -.0985f, .064f, .3037f, .32478f, -.048f, -.059f, 0, -.0985f,
        -.064f, .3037f, .32478f, -.048f, -.059f},
    {.799f, .4223f, .1985f, .5149f, .6784f, .1199f, .1007f, .4223f, -.2129f, .2339f, .053f, .1909f, 0, .028f, .001f, 0, .138f, .2736f, .24127f, .13f, .1112f,
        .1909f, 0, -.028f, .001f, 0, -.138f, .2736f, .24127f, .13f, .1112f, .1007f, 0, -.1017f, .081f, .35295f, .32548f, -.094f, -.124f, 0, -.1017f, -.081f,
        .35296f, .32547f, -.094f, -.124f},
    {.8823f, .4088f, .2094f, .5072f, .7636f, .1202f, .17f, .40885f, -.242f, .24501f, .073f, .18201f, 0, .045f, -.01f, 0, .158f, .27797f, .22923f, .22954f, 0,
        .18201f, 0, -.045f, -.01f, 0, -.158f, .27803f, .22917f, .22943f, 0, .17f, 0, -.17f, .081f, .40565f, .35694f, -.083f, -.14f, 0, -.17f, -.081f, .40565f,
        .35694f, -.083f, -.14f},
    {.96f, .377f, .2073f, .5278f, .841f, .1195f, .189f, .37704f, -.2105f, .2105f, .057f, .1495f, 0, .055f, -.008f, 0, .167f, .28117f, .24667f, .192f, .0548f,
        .1495f, 0, -.055f, -.008f, 0, -.167f, .28117f, .24666f, .192f, .0548f, .189f, 0, -.189f, .091f, .41669f, .42432f, -.088f, -.137f, 0, -.189f, -.091f,
        .41669f, .42433f, -.088f, -.137f},
    {.965f, .446f, .2094f, .5447f, .8187f, .1198f, .1339f, .44596f, -.2526f, .2526f, .085f, .1946f, 0, .056f, -.016f, 0, .169f, .30914f, .23562f, .19081f,
        .0448f, .1946f, 0, -.056f, -.016f, 0, -.169f, .30914f, .23623f, .19081f, .0448f, .1339f, 0, -.1609f, .092f, .414f, .40469f, -.085f, -.148f, 0, -.1609f,
        -.092f, .414f, .4047f, -.085f, -.148f},
    {1.028f, .5065f, .223f, .5802f, .8796f, .12f, .1101f, .5065f, -.28f, .28f, .091f, .218f, 0, .059f, -.017f, 0, .179f, .32923f, .25093f, .20322f, .048f,
        .218f, 0, -.059f, -.017f, 0, -.179f, .32924f, .25159f, .20322f, .048f, .1101f, 0, -.139f, .097f, .44092f, .43866f, -.07187f, -.14996f, 0, -.139f,
        -.097f, .44091f, .43866f, -.07186f, -.14996f},
    {1.11f, .5776f, .2753f, .6736f, .9711f, .1395f, .232f, .57762f, -.369f, .369f, .082f, .297f, 0, .056f, -.011f, 0, .274f, .36919f, .30445f, .245f, .059f,
        .297f, 0, -.056f, -.011f, 0, -.274f, .36927f, .30437f, .245f, .059f, .16f, 0, -.16f, .18f, .4969f, .47418f, -.061f, -.1f, 0, -.16f, -.18f, .4969f,
        .47417f, -.061f, -.1f},
    {.87f, .4079f, .221f, .5354f, .8135f, .1201f, .1941f, .40793f, -.19f, .201f, .05f, .129f, 0, .045f, .013f, 0, .198f, .29344f, .24198f, .16745f, .0745f,
        .129f, 0, -.043f, .013f, 0, -.198f, .29351f, .24192f, .16733f, .0745f, .1941f, 0, -.131f, .1f, .39013f, .42339f, -.081f, -.1f, 0, -.131f, -.1f, .39013f,
        .42339f, -.081f, -.1f},
    {.81f, .3462f, .177f, .4592f, .7797f, .0908f, .185f, .34619f, -.16f, .151f, .0549f, .128f, 0, .045f, 0, 0, .098f, .25133f, .20789f, .1671f, .0407f, .128f,
        0, -.045f, 0, 0, -.098f, .25134f, .20787f, .1671f, .0407f, .185f, 0, -.125f, .062f, .3812f, .39855f, -.052f, -.071f, 0, -.125f, -.062f, .38119f,
        .39855f, -.052f, -.071f},
    {.952f, .3367f, .1851f, .5025f, .8625f, .0891f, .162f, .33667f, -.242f, .2423f, .052f, .193f, 0, .03f, 0, 0, .118f, .26337f, .23908f, .19968f, .0394f,
        .193f, 0, -.03f, 0, 0, -.118f, .26337f, .23909f, .19968f, .0394f, .162f, 0, -.163f, .071f, .38743f, .47502f, -.06f, -.1f, 0, -.163f, -.071f, .38742f,
        .47504f, -.06f, -.1f},
    {.9805f, .4561f, .2073f, .5278f, .841f, .14f, .1099f, .45613f, -.271f, .271f, .0687f, .21f, 0, .055f, -.008f, 0, .14f, .28117f, .24667f, .192f, .0548f,
        .21f, 0, -.055f, -.008f, 0, -.14f, .28117f, .24666f, .192f, .0548f, .1099f, 0, -.1099f, .091f, .41669f, .42432f, -.0964f, -.117f, 0, -.1099f, -.091f,
        .41669f, .42433f, -.0964f, -.117f},
    {1, 1, 1, 1, 1, 0, 0, .09398f, 0, 0, .0333f, -.064f, -.0043f, .076f, -.0176f, -.0786f, -.0265f, .12607f, .15945f, 0, 0, -.064f, -.0043f, -.076f, -.0176f,
        -.0786f, .0115f, .12607f, .15945f, 0, 0, 0, -.027f, -.1582f, .006f, .15932f, .11957f, 0, 0, -.027f, -.1582f, .006f, .15932f, .11957f, 0, 0},
};

constexpr std::uint8_t entityToRig[97] = {
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    9,
    10,
    11,
    12,
    13,
    14,
    15,
    16,
    17,
    17,
    17,
    17,
    17,
    17,
    17,
    17,
    17,
    17,
    17,
    17,
    17,
    17,
    17,
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    9,
    10,
    11,
    12,
    13,
    14,
    15,
    16,
    17,
    17,
    17,
    17,
    17,
    17,
    17,
    17,
    17,
    17,
    17,
    17,
    17,
    17,
    17,
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    9,
    10,
    11,
    12,
    13,
    14,
    15,
    16,
    17,
    17,
    17,
    17,
    17,
    17,
    17,
    17,
    17,
    17,
    17,
    17,
    17,
    17,
    17,
    4,
};

float &At(Matrix4x4 &m, int row, int column) {
    return m.rows[row][column];
}
float At(const Matrix4x4 &m, int row, int column) {
    return m.rows[row][column];
}

Matrix4x4 Identity() {
    return {{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};
}

void RotateZSC(Matrix4x4 &m, float sine, float cosine) {
    const Vector4 a = m.rows[0], b = m.rows[1];
    for (int c = 0; c < 4; ++c) {
        At(m, 0, c) = b[c] * sine + a[c] * cosine;
        At(m, 1, c) = b[c] * cosine - a[c] * sine;
    }
}
void RotateYSC(Matrix4x4 &m, float sine, float cosine) {
    const Vector4 a = m.rows[0], b = m.rows[2];
    for (int c = 0; c < 4; ++c) {
        At(m, 0, c) = a[c] * cosine - b[c] * sine;
        At(m, 2, c) = a[c] * sine + b[c] * cosine;
    }
}
void RotateXSC(Matrix4x4 &m, float sine, float cosine) {
    const Vector4 a = m.rows[1], b = m.rows[2];
    for (int c = 0; c < 4; ++c) {
        At(m, 1, c) = b[c] * sine + a[c] * cosine;
        At(m, 2, c) = b[c] * cosine - a[c] * sine;
    }
}

std::uint16_t FixedAngle(float radians) {
    return static_cast<std::uint16_t>(static_cast<std::int32_t>(radians * 57.29578f * 182.04443f));
}
void RotateZ(Matrix4x4 &m, float r) {
    const auto a = FixedAngle(r);
    RotateZSC(m, sin_fixed16(static_cast<std::int16_t>(a)), sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(a))));
}
void RotateY(Matrix4x4 &m, float r) {
    const auto a = FixedAngle(r);
    RotateYSC(m, sin_fixed16(static_cast<std::int16_t>(a)), sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(a))));
}
void RotateX(Matrix4x4 &m, float r) {
    const auto a = FixedAngle(r);
    RotateXSC(m, sin_fixed16(static_cast<std::int16_t>(a)), sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(a))));
}
void RotateZYX(Matrix4x4 &m, float z, float y, float x) {
    RotateZ(m, z);
    RotateY(m, y);
    RotateX(m, x);
}

Vector3 TransformPoint(const Matrix4x4 &m, Vector3 p) {
    return {p.x * At(m, 0, 0) + p.y * At(m, 1, 0) + p.z * At(m, 2, 0) + At(m, 3, 0), p.x * At(m, 0, 1) + p.y * At(m, 1, 1) + p.z * At(m, 2, 1) + At(m, 3, 1),
        p.x * At(m, 0, 2) + p.y * At(m, 1, 2) + p.z * At(m, 2, 2) + At(m, 3, 2)};
}
float AlignLocal(Matrix4x4 &m, Vector3 p) {
    const float xySquared = p.y * p.y + p.x * p.x;
    const float xy = std::sqrt(xySquared);
    const float inverseXY = 1.0f / xy;
    RotateZSC(m, inverseXY * p.y, inverseXY * p.x);
    const float d = std::sqrt(p.z * p.z + xySquared);
    const float inverseDistance = 1.0f / d;
    RotateYSC(m, -(inverseDistance * p.z), inverseDistance * xy);
    return d;
}
float AimWorld(Matrix4x4 &m, Vector3 t) {
    const float dx = t.x - At(m, 3, 0), dy = t.y - At(m, 3, 1), dz = t.z - At(m, 3, 2);
    const Vector3 p{(At(m, 0, 1) * dy + At(m, 0, 0) * dx) + At(m, 0, 2) * dz, (At(m, 1, 1) * dy + At(m, 1, 0) * dx) + At(m, 1, 2) * dz,
        (At(m, 2, 1) * dy + At(m, 2, 0) * dx) + At(m, 2, 2) * dz};
    return AlignLocal(m, p);
}
std::array<float, 2> SolveCos(float a, float b, float opposite) {
    if (a * a < epsilon || b * b < epsilon)
        return {0, 0};
    const float c = std::clamp((b * b + a * a - opposite * opposite) / (2 * a * b), -1.0f, 1.0f);
    return {std::sqrt(std::max(0.0f, 1 - c * c)), c};
}

struct Profile {
    float hipsY, chestY, chestLength, headBase, headMid;
    Vector3 leftHip, rightHip, leftFoot, rightFoot;
    float leftUpper, leftLower, rightUpper, rightLower;
    struct Arm {
        Vector3 shoulder, elbow;
        float upper, lower, wrist, hand;
    } armR, armL;
};

Profile MakeProfile(std::uint8_t rig) {
    const float *r = g_rigRetargetParameters[rig];
    return {r[31], r[6], r[7] + r[8], r[9], r[10], {r[32], r[33], r[34]}, {r[39], r[40], r[41]}, {0, r[37], r[38]}, {0, r[44], r[45]}, r[35], r[36], r[42],
        r[43], {{r[11], r[12], r[13]}, {r[14], r[15], r[16]}, r[17], r[18], r[19], r[20]},
        {{r[21], r[22], r[23]}, {r[24], r[25], r[26]}, r[27], r[28], r[29], r[30]}};
}
void Put(MotionPose &p, std::uint8_t id, const Matrix4x4 &m) {
    p.nodeMatrices[id] = m;
    p.hasNodeMatrix[id] = true;
}

void SolveArm(const std::array<float, 69> &v, int start, const Profile::Arm &p, Matrix4x4 work, bool right, MotionPose &out) {
    const std::uint8_t shoulder = right ? 4 : 9, elbow = right ? 5 : 10, forearm = right ? 6 : 11, wrist = right ? 7 : 12, hand = right ? 191 : 190;
    Matrix4x4::preTranslate(work, p.shoulder.x, p.shoulder.y, p.shoulder.z);
    RotateZYX(work, v[start], v[start + 1], v[start + 2]);
    Put(out, shoulder, work);
    Matrix4x4::preTranslate(work, p.elbow.x, p.elbow.y, p.elbow.z);
    RotateZYX(work, v[start + 3], v[start + 4], v[start + 5]);
    const float d = AlignLocal(work, {v[start + 6], v[start + 7], v[start + 8]});
    auto sc = SolveCos(p.upper, d, p.lower);
    RotateZSC(work, -sc[0], sc[1]);
    Put(out, elbow, work);
    Matrix4x4::preTranslate(work, p.upper, 0, 0);
    sc = SolveCos(p.upper, p.lower, d);
    RotateZSC(work, sc[0], -sc[1]);
    Put(out, forearm, work);
    Matrix4x4::preTranslate(work, p.wrist, 0, 0);
    RotateX(work, v[start + 9]);
    Put(out, wrist, work);
    Matrix4x4::preTranslate(work, p.hand, 0, 0);
    RotateZ(work, v[start + 10]);
    RotateY(work, v[start + 11]);
    Put(out, hand, work);
    out.armWristRotation[right ? 0 : 1] = {v[start + 9], v[start + 11], v[start + 10]};
}

void CopyBasis(Matrix4x4 &target, const Matrix4x4 &source) {
    for (int row = 0; row < 3; ++row) {
        for (int column = 0; column < 3; ++column) {
            At(target, row, column) = At(source, row, column);
        }
    }
}

void SolveOverrideLeg(Matrix4x4 work, Vector3 target, Vector3 fkOffset, Vector3 fkRotation, Vector3 hipOffset, Vector3 hipRotation, float upper, float lower,
    Vector3 ankleRotation, const Matrix4x4 &basis, bool right, MotionPose &out) {
    const std::uint8_t hipFk = right ? 20 : 15, thigh = right ? 21 : 16, shin = right ? 22 : 17, ankle = right ? 23 : 18;
    Matrix4x4::preTranslate(work, fkOffset.x, fkOffset.y, fkOffset.z);
    RotateZYX(work, fkRotation.x, fkRotation.y, fkRotation.z);
    Put(out, hipFk, work);
    Matrix4x4::preTranslate(work, hipOffset.x, hipOffset.y, hipOffset.z);
    RotateZYX(work, hipRotation.x, hipRotation.y, hipRotation.z);
    const float distance = AimWorld(work, target);
    auto sc = SolveCos(upper, distance, lower);
    RotateZSC(work, sc[0], sc[1]);
    Put(out, thigh, work);
    Matrix4x4::preTranslate(work, upper, 0, 0);
    sc = SolveCos(upper, lower, distance);
    RotateZSC(work, -sc[0], -sc[1]);
    Put(out, shin, work);
    Matrix4x4::preTranslate(work, lower, 0, 0);
    CopyBasis(work, basis);
    RotateZYX(work, ankleRotation.x, ankleRotation.y, ankleRotation.z);
    Put(out, ankle, work);
}

void SolveOverrideArm(const std::array<float, 96> &v, int start, Matrix4x4 chest, const Matrix4x4 &root, bool right, MotionPose &out) {
    const int base = right ? 20 : 30;
    const std::uint8_t shoulder = right ? 4 : 9, elbow = right ? 5 : 10, forearm = right ? 6 : 11, hand = right ? 191 : 190;
    const Vector3 target = TransformPoint(root, {v[start + 6], v[start + 7], v[start + 8]});
    Matrix4x4::preTranslate(chest, overrideRigConstants[base], overrideRigConstants[base + 1], overrideRigConstants[base + 2]);
    RotateZYX(chest, v[start], v[start + 1], v[start + 2]);
    Put(out, shoulder, chest);
    Matrix4x4::preTranslate(chest, overrideRigConstants[base + 3], overrideRigConstants[base + 4], overrideRigConstants[base + 5]);
    RotateZYX(chest, v[start + 3], v[start + 4], v[start + 5]);
    const float upper = overrideRigConstants[base + 6], lower = overrideRigConstants[base + 7];
    const float distance = AimWorld(chest, target);
    auto sc = SolveCos(upper, distance, lower);
    RotateZSC(chest, sc[0], sc[1]);
    Put(out, elbow, chest);
    Matrix4x4::preTranslate(chest, upper, 0, 0);
    sc = SolveCos(upper, lower, distance);
    RotateZSC(chest, -sc[0], -sc[1]);
    Put(out, forearm, chest);
    Matrix4x4::preTranslate(chest, lower, 0, 0);
    CopyBasis(chest, root);
    RotateZYX(chest, v[start + 9], v[start + 10], v[start + 11]);
    Put(out, hand, chest);
    out.armWristRotation[right ? 0 : 1] = {v[start + 11], v[start + 10], v[start + 9]};
}

void EvaluateOverridePose(const std::array<float, 96> &v, MotionPose &out) {
    const Matrix4x4 root = Identity();
    Matrix4x4 pelvisTransform = Identity();
    Matrix4x4::preTranslate(pelvisTransform, -v[0], 0, -v[2]);
    const Matrix4x4 pelvisInput = Matrix4x4::Multiply(pelvisTransform, root);
    const Matrix4x4 basis = pelvisInput;
    const Vector3 leftTarget = TransformPoint(pelvisInput, {v[18], v[19], v[20]});
    const Vector3 rightTarget = TransformPoint(pelvisInput, {v[30], v[31], v[32]});
    const Vector3 hipsTarget = TransformPoint(pelvisInput, {v[9], v[10], v[11]});

    Matrix4x4 pelvis = pelvisInput;
    Matrix4x4::preTranslate(pelvis, v[0], v[1], v[2]);
    RotateZYX(pelvis, v[3], v[4], v[5]);
    Put(out, 0, pelvis);
    Matrix4x4 hips = pelvis;
    Matrix4x4::preTranslate(hips, 0, 0, overrideRigConstants[41]);
    RotateZYX(hips, v[6], v[7], v[8]);
    AimWorld(hips, hipsTarget);
    Put(out, 14, hips);
    Matrix4x4::preTranslate(hips, overrideRigConstants[42], 0, 0);
    SolveOverrideLeg(hips, leftTarget, {overrideRigConstants[43], overrideRigConstants[44], overrideRigConstants[45]}, {v[12], v[13], v[14]},
        {overrideRigConstants[46], overrideRigConstants[47], 0}, {v[15], v[16], v[17]}, overrideRigConstants[49], overrideRigConstants[50],
        {v[21], v[22], v[23]}, basis, false, out);
    SolveOverrideLeg(hips, rightTarget, {overrideRigConstants[53], overrideRigConstants[54], overrideRigConstants[55]}, {v[24], v[25], v[26]},
        {overrideRigConstants[56], overrideRigConstants[57], 0}, {v[27], v[28], v[29]}, overrideRigConstants[59], overrideRigConstants[60],
        {v[33], v[34], v[35]}, basis, true, out);

    Matrix4x4 toe = hips;
    Matrix4x4::preTranslate(toe, overrideRigConstants[63], overrideRigConstants[64], 0);
    RotateZ(toe, v[36]);
    RotateY(toe, v[37]);
    Put(out, 85, toe);
    Matrix4x4::preTranslate(toe, overrideRigConstants[65], overrideRigConstants[66], 0);
    RotateZ(toe, v[38]);
    RotateY(toe, v[39]);
    Put(out, 86, toe);
    Matrix4x4::preTranslate(toe, overrideRigConstants[67], overrideRigConstants[68], 0);
    RotateZ(toe, v[40]);
    RotateY(toe, v[41]);
    Put(out, 87, toe);

    const Vector3 chestTarget = TransformPoint(pelvisInput, {v[45], v[46], v[47]});
    Matrix4x4 chest = pelvis;
    Matrix4x4::preTranslate(chest, 0, 0, overrideRigConstants[2]);
    RotateZYX(chest, v[42], v[43], v[44]);
    AimWorld(chest, chestTarget);
    Put(out, 1, chest);
    Matrix4x4::preTranslate(chest, overrideRigConstants[3], 0, 0);

    const Vector3 headTarget = TransformPoint(root, {v[54], v[55], v[56]});
    Matrix4x4 headBase = chest;
    Matrix4x4::preTranslate(headBase, 0, overrideRigConstants[6], 0);
    RotateZYX(headBase, v[48], v[49], v[50]);
    Put(out, 2, headBase);
    Matrix4x4 aim = headBase;
    Matrix4x4::preTranslate(aim, overrideRigConstants[7], overrideRigConstants[8], 0);
    RotateZYX(aim, v[51], v[52], v[53]);
    AimWorld(aim, headTarget);
    Put(out, 177, aim);
    Matrix4x4::preTranslate(aim, overrideRigConstants[9], 0, 0);
    Matrix4x4 head = aim;
    Matrix4x4::preTranslate(head, overrideRigConstants[10], overrideRigConstants[11], 0);
    RotateZYX(head, v[57], v[58], v[59]);
    AlignLocal(head, {v[60], v[61], v[62]});
    Put(out, 70, head);
    Matrix4x4::preTranslate(head, overrideRigConstants[12], 0, 0);
    Matrix4x4 node = head;
    Matrix4x4::preTranslate(node, 0, overrideRigConstants[14], overrideRigConstants[15]);
    RotateZYX(node, v[63], v[64], v[65]);
    Put(out, 58, node);
    node = head;
    Matrix4x4::preTranslate(node, 0, overrideRigConstants[16], overrideRigConstants[17]);
    RotateZYX(node, v[66], v[67], v[68]);
    Put(out, 59, node);
    node = head;
    Matrix4x4::preTranslate(node, overrideRigConstants[18], 0, overrideRigConstants[19]);
    RotateZYX(node, v[69], v[70], v[71]);
    Put(out, 119, node);
    SolveOverrideArm(v, 72, chest, root, true, out);
    SolveOverrideArm(v, 84, chest, root, false, out);
    out.rootMotion = {v[0], 0, v[2]};
    out.overrideRig = true;
}

} // namespace

const Matrix4x4 *FindMotionDrawOverrideNodeTransform(const MT7Model &model, const Matrix4x4 *frame, std::uint32_t nodeId) {
    if (!frame)
        return nullptr;
    // ReadNode preserves child-before-sibling preorder. Invisible nodes
    // still occupy a record; only an absent XB01 omits one.
    for (const MT7Node &node : model.nodes) {
        if (!node.xb01Offset)
            continue;
        if ((static_cast<std::uint32_t>(node.id) & 0xffffU) == nodeId)
            return frame;
        ++frame;
    }
    return nullptr;
}

std::int32_t GetMotionBoneIdForEntityType(std::int32_t entityType) {
    if (static_cast<std::uint32_t>(entityType) >= std::size(entityToRig))
        return -1;
    return entityToRig[entityType];
}

std::uint8_t MotionRigForEntityType(std::int32_t entityType) {
    if (entityType < 0 || static_cast<std::size_t>(entityType) >= std::size(entityToRig))
        return fallbackRig;
    const std::uint8_t rig = entityToRig[entityType];
    return rig < invalidRig ? rig : fallbackRig;
}

float GetMotionRigTranslationScale(const MotnSequence &sequence, std::uint8_t targetRig) {
    targetRig = targetRig < invalidRig ? targetRig : fallbackRig;
    const std::uint8_t authored = static_cast<std::uint8_t>(sequence.animationFlags);
    const std::uint8_t source = authored < invalidRig ? authored : fallbackRig;
    return g_rigRetargetParameters[targetRig][0] / g_rigRetargetParameters[source][0];
}
float GetMotionRigScale(std::uint32_t targetRig, std::uint32_t sourceRig, std::uint32_t region) {
    if (targetRig >= invalidRig || sourceRig >= invalidRig || region >= 6U)
        return 0.0f;
    return g_rigRetargetParameters[targetRig][region] / g_rigRetargetParameters[sourceRig][region];
}

float GetMotionRigArmAxisScale(std::uint32_t rig) {
    return rig < invalidRig ? g_rigRetargetParameters[rig][3] : 0.0f;
}

void AlignMotionFrameToTarget(Matrix4x4 &transform, const Vector3 &target) {
    const float horizontal = std::sqrt(target.x * target.x + target.y * target.y);
    RotateZSC(transform, target.y / horizontal, target.x / horizontal);
    const float distance = std::sqrt(horizontal * horizontal + target.z * target.z);
    RotateYSC(transform, -target.z / distance, horizontal / distance);
}

bool SampleMotionRootTrack(const MotnSequence &sequence, float time, std::uint8_t targetRig, Vector3 &translation) {
    if (sequence.scalars.size() < 3)
        return false;
    const float scale = GetMotionRigTranslationScale(sequence, targetRig);
    translation = {SampleMotnScalar(sequence.scalars[0], time) * scale, SampleMotnScalar(sequence.scalars[1], time) * scale,
        SampleMotnScalar(sequence.scalars[2], time) * scale};
    return true;
}

bool SampleMotionRootTrackPVA(const MotnSequence &sequence, float time, std::uint8_t targetRig, Vector3 &translation, Vector3 &incomingHalfTangent,
    Vector3 &outgoingHalfTangent, bool linear) {
    if (sequence.scalars.size() < 3)
        return false;
    const float scale = GetMotionRigTranslationScale(sequence, targetRig);
    for (std::size_t axis = 0; axis < 3; ++axis) {
        SampleMotnScalarPVA(sequence.scalars[axis], time, translation[axis], incomingHalfTangent[axis], outgoingHalfTangent[axis], linear);
        translation[axis] *= scale;
        incomingHalfTangent[axis] *= scale;
        outgoingHalfTangent[axis] *= scale;
    }
    return true;
}

bool EvaluateMotionPose(const MotnSequence &sequence, float time, std::uint8_t targetRig, MotionPose &out, const MotionWork *motionWork) {
    out = {};
    targetRig = targetRig < invalidRig ? targetRig : fallbackRig;
    const std::uint8_t sourceRig = (sequence.animationFlags & 0xff) < invalidRig ? static_cast<std::uint8_t>(sequence.animationFlags & 0xff) : fallbackRig;
    if (targetRig == 16) {
        if (sequence.scalars.size() < 96)
            return false;
        std::array<float, 96> values{};
        for (std::size_t i = 0; i < values.size(); ++i)
            values[i] = SampleMotnScalar(sequence.scalars[i], time);
        if (sourceRig != targetRig) {
            const int region0[] = {0, 1, 2, 12, 13, 14, 21, 22, 23}, region1[] = {30, 31, 32}, region2[] = {42, 43, 44}, region3[] = {51, 52, 53, 63, 64, 65};
            const auto scale = [&](int region, const int *indices, std::size_t count) {
                const float s = g_rigRetargetParameters[sourceRig][region] == 0 ? 1.0f : g_rigRetargetParameters[targetRig][region] / g_rigRetargetParameters[sourceRig][region];
                for (std::size_t i = 0; i < count; ++i)
                    values[indices[i]] *= s;
            };
            scale(0, region0, std::size(region0));
            scale(1, region1, std::size(region1));
            scale(2, region2, std::size(region2));
            scale(3, region3, std::size(region3));
        }
        EvaluateOverridePose(values, out);
        return true;
    }
    if (sequence.scalars.size() != 69)
        return false;
    std::array<float, 69> v{};
    for (std::size_t i = 0; i < v.size(); ++i)
        v[i] = SampleMotnScalar(sequence.scalars[i], time);
    const int region0[] = {0, 1, 2, 12, 13, 14, 21, 22, 23}, region1[] = {30, 31, 32}, region2[] = {42, 43, 44}, region3[] = {51, 52, 53, 63, 64, 65};
    const auto scale = [&](int region, const int *indices, std::size_t count) {
        const float s = g_rigRetargetParameters[sourceRig][region] == 0 ? 1.0f : g_rigRetargetParameters[targetRig][region] / g_rigRetargetParameters[sourceRig][region];
        for (std::size_t i = 0; i < count; ++i)
            v[indices[i]] *= s;
    };
    scale(0, region0, std::size(region0));
    scale(1, region1, std::size(region1));
    scale(2, region2, std::size(region2));
    scale(3, region3, std::size(region3));
    if (motionWork && (motionWork->rig.lowerBody.motionFlags & 2U) != 0) {
        v[0] = -v[0];
        v[3] = -v[3];
        v[4] = -v[4];
        v[6] = -v[6];
        v[7] = 3.1415927f - v[7];
        v[8] = -v[8];
        for (std::size_t index = 0; index < 9; ++index)
            std::swap(v[9 + index], v[18 + index]);
        for (const std::size_t index : {10U, 11U, 12U, 15U, 16U, 19U, 20U, 21U, 24U, 25U})
            v[index] = -v[index];
    }
    const Profile p = MakeProfile(targetRig);
    const IKRig *rig = motionWork ? &motionWork->rig : nullptr;
    Matrix4x4 work = Identity();
    Matrix4x4 pelvis;
    if (rig) {
        Matrix4x4 inverse = rig->lowerBody.rootPrepassTransform;
        inverse.rows[3] = {motionWork->runtime.rootPosition.x, motionWork->runtime.rootPosition.y, motionWork->runtime.rootPosition.z, 1.0f};
        Matrix4x4::InvertInPlace(&inverse);
        pelvis = Matrix4x4::Multiply(rig->lowerBody.pelvisWorld, inverse);
        Put(out, 14, Matrix4x4::Multiply(rig->lowerBody.hipsWorld, inverse));
        Put(out, 16, Matrix4x4::Multiply(rig->lowerBody.leftThighWorld, inverse));
        Put(out, 17, Matrix4x4::Multiply(rig->lowerBody.leftShinWorld, inverse));
        Put(out, 18, Matrix4x4::Multiply(rig->lowerBody.leftAnkleWorld, inverse));
        Put(out, 19, Matrix4x4::Multiply(rig->lowerBody.leftFootWorld, inverse));
        Put(out, 21, Matrix4x4::Multiply(rig->lowerBody.rightThighWorld, inverse));
        Put(out, 22, Matrix4x4::Multiply(rig->lowerBody.rightShinWorld, inverse));
        Put(out, 23, Matrix4x4::Multiply(rig->lowerBody.rightAnkleWorld, inverse));
        Put(out, 24, Matrix4x4::Multiply(rig->lowerBody.rightFootWorld, inverse));
    } else {
        Matrix4x4::preTranslate(work, -v[0], 0, -v[2]);
        const Matrix4x4 basis = work;
        const Vector3 targets[2] = {TransformPoint(work, {v[12], v[13], v[14]}), TransformPoint(work, {v[21], v[22], v[23]})};
        Matrix4x4::preTranslate(work, v[0], v[1], v[2]);
        RotateZYX(work, v[3], v[4], v[5]);
        pelvis = work;
        Matrix4x4::preTranslate(work, 0, p.hipsY, 0);
        RotateZYX(work, v[6], v[7], v[8]);
        Put(out, 14, work);
        const Matrix4x4 hips = work;
        for (std::size_t side = 0; side < 2; ++side) {
            work = hips;
            const Vector3 &hip = side ? p.rightHip : p.leftHip;
            const float upper = side ? p.rightUpper : p.leftUpper;
            const float lower = side ? p.rightLower : p.leftLower;
            const std::size_t first = side ? 18U : 9U;
            const std::uint8_t thigh = side ? 21U : 16U;
            Matrix4x4::preTranslate(work, 0.0f, hip.y, hip.z);
            RotateZYX(work, v[first], v[first + 1], v[first + 2]);
            const float distance = AimWorld(work, targets[side]);
            std::array<float, 2> sc = SolveCos(upper, distance, lower);
            RotateZSC(work, sc[0], sc[1]);
            Put(out, thigh, work);
            Matrix4x4::preTranslate(work, upper, 0, 0);
            sc = SolveCos(upper, lower, distance);
            RotateZSC(work, -sc[0], -sc[1]);
            Put(out, thigh + 1, work);
            Matrix4x4::preTranslate(work, lower, 0, 0);
            CopyBasis(work, basis);
            RotateZYX(work, v[first + 6], v[first + 7], v[first + 8]);
            Put(out, thigh + 2, work);
            const Vector3 &foot = side ? p.rightFoot : p.leftFoot;
            Matrix4x4::preTranslate(work, 0.0f, foot.y, foot.z);
            Put(out, thigh + 3, work);
        }
    }
    work = pelvis;
    Matrix4x4::preTranslate(work, 0, p.chestY, 0);
    RotateZYX(work, v[27], v[28], v[29]);
    AlignLocal(work, {v[30], v[31], v[32]});
    Put(out, 0, work);
    Matrix4x4::preTranslate(work, p.chestLength, 0, 0);
    RotateZYX(work, v[33], v[34], v[35]);
    Put(out, 1, work);
    const Matrix4x4 chest = work;
    Matrix4x4::preTranslate(work, p.headBase, 0, 0);
    RotateZYX(work, v[36], v[37], v[38]);
    Put(out, 2, work);
    Matrix4x4::preTranslate(work, p.headMid, 0, 0);
    RotateZYX(work, v[39], v[40], v[41]);
    AlignLocal(work, {v[42], v[43], v[44]});
    Put(out, 70, work);
    SolveArm(v, 45, p.armR, chest, true, out);
    SolveArm(v, 57, p.armL, chest, false, out);
    out.rootMotion = {v[0], 0, v[2]};
    return true;
}

} // namespace shenmue::s2
