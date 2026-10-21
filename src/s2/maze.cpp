#include "s2/maze.hpp"
#include "s2/world_render.hpp"

#include "s2/hlib.hpp"
#include "s2/game_runtime.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/pak.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/pause.hpp"
#include "s2/character.hpp"
#include "s2/fish.hpp"
#include "s2/mt7.hpp"
#include "s2/camera.hpp"
#include "s2/date_time.hpp"
#include "s2/weather.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/navi.hpp"
#include "d3t/object_store.hpp"

#include <array>
#include <bit>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>

namespace shenmue::s2 {
std::uint32_t g_mazePropDrawCount = 0;
std::uint32_t g_mazeUnitWallHitCount = 0;
MazeUnit *MUZT_Data = nullptr;
namespace {

constexpr Vector2 unit2Path0[] = {
    {0.9559339880943298f, 13.85644245147705f},
    {0.17282700538635254f, 15.684398651123047f},
    {-0.0011970000341534615f, 17.251218795776367f},
    {-0.0011970000341534615f, 18.46985626220703f},
};
constexpr Vector2 unit2Path1[] = {
    {1.2169690132141113f, 12.115531921386719f},
    {0.7095990180969238f, 10.068550109863281f},
    {0.31189200282096863f, 8.043073654174805f},
    {0.02264999970793724f, 6.632472991943359f},
    {-1.098162055015564f, 4.82401180267334f},
    {-1.4235730171203613f, 3.232564926147461f},
    {-1.2789520025253296f, 2.111319065093994f},
    {-0.664313018321991f, 1.243257999420166f},
    {1.9027080535888672f, -0.5290340185165405f},
    {3.5676960945129395f, -1.4694340229034424f},
    {4.688508987426758f, -1.758787989616394f},
    {8.014789581298828f, -1.975803017616272f},
    {10.473345756530762f, -1.7226179838180542f},
    {12.283597946166992f, -0.9992340207099915f},
    {14.452912330627441f, -0.16734200716018677f},
    {15.899121284484863f, 0.12201199680566788f},
    {17.417640686035156f, 0.15818099677562714f},
    {18.683074951171875f, 0.15818099677562714f},
};
constexpr Vector2 unit2Path2[] = {
    {2.3296759128570557f, 12.802322387695312f},
    {3.829200029373169f, 12.644415855407715f},
    {5.635041236877441f, 12.439910888671875f},
    {6.927080154418945f, 11.885965347290039f},
    {8.1268310546875f, 11.193532943725586f},
    {9.280437469482422f, 9.947155952453613f},
    {9.8803129196167f, 8.469968795776367f},
    {9.972600936889648f, 7.038942813873291f},
    {9.14134407043457f, 5.561756134033203f},
    {7.710872173309326f, 3.992244005203247f},
    {6.463959217071533f, 2.145759105682373f},
    {6.048661231994629f, 0.25311198830604553f},
    {6.094804763793945f, -2.424290895462036f},
    {6.0022478103637695f, -3.9476420879364014f},
    {5.1716508865356445f, -4.778560161590576f},
    {3.7873239517211914f, -5.240180969238281f},
    {2.1722750663757324f, -5.147857189178467f},
    {-0.9206590056419373f, -4.224614143371582f},
    {-3.320159912109375f, -3.4398579597473145f},
    {-6.829052925109863f, -2.2858049869537354f},
    {-10.153417587280273f, -1.2240760326385498f},
    {-12.045331954956055f, -0.6239680051803589f},
    {-14.029535293579102f, -0.023861000314354897f},
    {-15.690728187561035f, 0.16078799962997437f},
    {-17.16734504699707f, 0.2992739975452423f},
    {-18.413238525390625f, 0.2992739975452423f},
};
constexpr AuthoredPath unit2Paths[] = {{unit2Path0}, {unit2Path1}, {unit2Path2}};
constexpr Vector2 unit5Path0[] = {
    {-0.01992099918425083f, 1.3089890480041504f},
    {-1.122668981552124f, 2.6485579013824463f},
    {-1.8315789699554443f, 4.697311878204346f},
    {-2.225416898727417f, 7.455248832702637f},
    {-1.8315789699554443f, 10.449580192565918f},
    {-1.122668981552124f, 12.73472785949707f},
    {-0.4137600064277649f, 15.177472114562988f},
    {-0.01992099918425083f, 18.093006134033203f},
};
constexpr Vector2 unit5Path1[] = {
    {-1.122668981552124f, -0.5821679830551147f},
    {-2.382952928543091f, -1.606544017791748f},
    {-4.352146148681641f, -2.47332501411438f},
    {-6.242570877075195f, -2.5521230697631836f},
    {-8.054227828979492f, -2.079334020614624f},
    {-10.180956840515137f, -1.0549570322036743f},
    {-12.15014934539795f, -0.10937900096178055f},
    {-13.961807250976562f, -0.10937900096178055f},
    {-16.403606414794922f, -0.030579999089241028f},
    {-18.372798919677734f, 0.048218000680208206f},
};
constexpr Vector2 unit5Path2[] = {
    {1.0040589570999146f, 0.048218000680208206f},
    {2.4218780994415283f, -0.6609659790992737f},
    {4.233534812927246f, -1.0549570322036743f},
    {6.517798900604248f, -1.2913520336151123f},
    {9.510972023010254f, -1.0549570322036743f},
    {11.71646785736084f, -0.6609659790992737f},
    {14.47333812713623f, -0.34577301144599915f},
};
constexpr AuthoredPath unit5Paths[] = {{unit5Path0}, {unit5Path1}, {unit5Path2}};
constexpr Vector2 unit6Path0[] = {
    {-0.5f, 1.3860000371932983f},
    {1.0f, -0.35100001096725464f},
    {1.899999976158142f, -2.109999895095825f},
    {3.200000047683716f, -4.8680009841918945f},
    {5.300000190734863f, -6.126999855041504f},
    {7.5f, -5.997000217437744f},
    {10.699999809265137f, -4.454999923706055f},
    {13.300000190734863f, -1.4160000085830688f},
    {15.800000190734863f, -0.026000000536441803f},
    {18.19999885559082f, 0.25600001215934753f},
};
constexpr Vector2 unit6Path1[] = {
    {-0.800000011920929f, 3.7070000171661377f},
    {-0.20000000298023224f, 5.791999816894531f},
    {0.20000000298023224f, 8.9350004196167f},
    {0.30000001192092896f, 11.791000366210938f},
    {0.30000001192092896f, 14.358000755310059f},
    {0.30000001192092896f, 17.084001541137695f},
};
constexpr Vector2 unit6Path2[] = {
    {-3.0f, 1.878000020980835f},
    {-5.0f, 1.0119999647140503f},
    {-7.5f, 0.3700000047683716f},
    {-10.59999942779541f, -0.14300000667572021f},
    {-14.200000762939453f, -0.14300000667572021f},
    {-17.19999885559082f, -0.2070000022649765f},
};
constexpr AuthoredPath unit6Paths[] = {{unit6Path0}, {unit6Path1}, {unit6Path2}};
constexpr Vector2 unit7Path0[] = {
    {-0.7200000286102295f, 14.199999809265137f},
    {-0.30000001192092896f, 16.309999465942383f},
    {-0.05999999865889549f, 18.35999870300293f},
};
constexpr Vector2 unit7Path1[] = {
    {-2.2899999618530273f, 12.50999927520752f},
    {-4.760000228881836f, 11.180000305175781f},
    {-7.53000020980835f, 9.5f},
    {-9.279999732971191f, 7.199999809265137f},
    {-10.969999313354492f, 4.789999961853027f},
    {-12.899999618530273f, 2.259999990463257f},
    {-15.5f, 0.5f},
    {-18.0f, 0.0f},
};
constexpr Vector2 unit7Path2[] = {
    {0.49000000953674316f, 12.149999618530273f},
    {2.7200000286102295f, 10.829999923706055f},
    {4.710000038146973f, 9.739999771118164f},
    {6.460000038146973f, 7.510000228881836f},
    {7.909998893737793f, 5.340000152587891f},
    {10.020000457763672f, 3.5299999713897705f},
    {12.369999885559082f, 1.4199999570846558f},
    {15.0f, 0.0f},
    {18.5f, 0.0f},
};
constexpr AuthoredPath unit7Paths[] = {{unit7Path0}, {unit7Path1}, {unit7Path2}};
constexpr Vector2 unit8Path0[] = {
    {-0.7200000286102295f, 14.199999809265137f},
    {-0.30000001192092896f, 16.309999465942383f},
    {-0.05999999865889549f, 18.35999870300293f},
};
constexpr Vector2 unit8Path1[] = {
    {-2.2899999618530273f, 12.50999927520752f},
    {-4.760000228881836f, 11.180000305175781f},
    {-7.53000020980835f, 9.5f},
    {-9.279999732971191f, 7.199999809265137f},
    {-10.969999313354492f, 4.789999961853027f},
    {-12.899999618530273f, 2.259999990463257f},
    {-15.5f, 0.5f},
    {-18.0f, 0.0f},
};
constexpr Vector2 unit8Path2[] = {
    {0.49000000953674316f, 12.149999618530273f},
    {2.7200000286102295f, 10.829999923706055f},
    {4.710000038146973f, 9.739999771118164f},
    {6.460000038146973f, 7.510000228881836f},
    {7.909998893737793f, 5.340000152587891f},
    {10.020000457763672f, 3.5299999713897705f},
    {12.369999885559082f, 1.4199999570846558f},
    {15.0f, 0.0f},
    {18.5f, 0.0f},
};
constexpr AuthoredPath unit8Paths[] = {{unit8Path0}, {unit8Path1}, {unit8Path2}};
constexpr Vector2 unit9Path0[] = {
    {-0.30000001192092896f, 14.09999942779541f},
    {-0.4000000059604645f, 16.100000381469727f},
    {-0.10000000149011612f, 18.399999618530273f},
};
constexpr Vector2 unit9Path1[] = {
    {-1.899999976158142f, 12.0f},
    {-4.0f, 10.100000381469727f},
    {-6.400000095367432f, 8.699999809265137f},
    {-9.199999809265137f, 7.400000095367432f},
    {-11.0f, 6.099999904632568f},
    {-12.600000381469727f, 3.5f},
    {-14.40000057220459f, 1.2000000476837158f},
    {-16.399999618530273f, 0.20000000298023224f},
    {-18.299999237060547f, 0.0f},
};
constexpr Vector2 unit9Path2[] = {
    {1.2999999523162842f, 12.0f},
    {3.299999952316284f, 9.800000190734863f},
    {6.199999809265137f, 7.900000095367432f},
    {7.900001049041748f, 4.699999809265137f},
    {9.600000381469727f, 2.0999999046325684f},
    {12.09999942779541f, 0.30000001192092896f},
    {15.200000762939453f, -0.30000001192092896f},
    {18.100000381469727f, -0.20000000298023224f},
};
constexpr AuthoredPath unit9Paths[] = {{unit9Path0}, {unit9Path1}, {unit9Path2}};
constexpr Vector2 unit10Path0[] = {
    {-0.7200000286102295f, 14.199999809265137f},
    {-0.30000001192092896f, 16.309999465942383f},
    {-0.05999999865889549f, 18.35999870300293f},
};
constexpr Vector2 unit10Path1[] = {
    {-2.2899999618530273f, 12.50999927520752f},
    {-4.760000228881836f, 11.180000305175781f},
    {-7.53000020980835f, 9.5f},
    {-9.279999732971191f, 7.199999809265137f},
    {-10.969999313354492f, 4.789999961853027f},
    {-12.899999618530273f, 2.259999990463257f},
    {-15.5f, 0.5f},
    {-18.0f, 0.0f},
};
constexpr Vector2 unit10Path2[] = {
    {0.49000000953674316f, 12.149999618530273f},
    {2.7200000286102295f, 10.829999923706055f},
    {4.710000038146973f, 9.739999771118164f},
    {6.460000038146973f, 7.510000228881836f},
    {7.909998893737793f, 5.340000152587891f},
    {10.020000457763672f, 3.5299999713897705f},
    {12.369999885559082f, 1.4199999570846558f},
    {15.0f, 0.0f},
    {18.5f, 0.0f},
};
constexpr AuthoredPath unit10Paths[] = {{unit10Path0}, {unit10Path1}, {unit10Path2}};
constexpr Vector2 unit11Path0[] = {
    {0.11362899839878082f, 17.5744686126709f},
    {0.08527199923992157f, 15.617023468017578f},
    {0.3121339976787567f, 13.914895057678223f},
    {1.1345109939575195f, 12.865249633789062f},
    {2.2688241004943848f, 11.843973159790039f},
    {3.374778985977173f, 10.822696685791016f},
    {3.830336093902588f, 9.48935604095459f},
    {4.142271995544434f, 7.3900651931762695f},
    {4.227344989776611f, 4.836874008178711f},
    {3.376599073410034f, 2.368788957595825f},
    {2.0721399784088135f, 0.4680800139904022f},
    {0.25589001178741455f, -0.8936219811439514f},
    {-1.3037899732589722f, -2.368799924850464f},
    {-2.609050989151001f, -4.780147075653076f},
    {-3.176448106765747f, -7.9858222007751465f},
    {-3.034658908843994f, -10.36880111694336f},
    {-2.2119200229644775f, -12.581570625305176f},
    {-1.134323000907898f, -14.198591232299805f},
    {-0.3686619997024536f, -15.276605606079102f},
    {0.028348000720143318f, -16.723413467407227f},
    {0.05670500174164772f, -17.602846145629883f},
};
constexpr AuthoredPath unit11Paths[] = {{unit11Path0}};
constexpr Vector2 unit12Path0[] = {
    {0.04133100062608719f, 7.938899993896484f},
    {0.08983299881219864f, 3.4265151023864746f},
    {0.08983299881219864f, -0.8432689905166626f},
    {-0.24964000284671783f, -4.773410797119141f},
    {-0.007133000064641237f, -7.781668186187744f},
};
constexpr AuthoredPath unit12Paths[] = {{unit12Path0}};
constexpr Vector2 unit13Path0[] = {
    {-0.1207439973950386f, 8.045559883117676f},
    {-0.15051999688148499f, 6.16896390914917f},
    {-0.0016070000128820539f, 4.143433094024658f},
    {0.41525301337242126f, 2.1476879119873047f},
    {0.7725620269775391f, -0.5033760070800781f},
    {0.593779981136322f, -3.0352909564971924f},
    {0.20644299685955048f, -5.179973125457764f},
    {0.05757699906826019f, -6.550185203552246f},
    {0.027800999581813812f, -8.426780700683594f},
};
constexpr AuthoredPath unit13Paths[] = {{unit13Path0}};
constexpr Vector2 unit14Path0[] = {
    {0.0f, 18.0f},
    {0.0f, 17.0f},
    {0.8999999761581421f, 15.0f},
    {2.5f, 13.0f},
    {3.799999952316284f, 11.5f},
    {5.099999904632568f, 9.0f},
    {6.5f, 6.0f},
    {7.400000095367432f, 3.5f},
    {7.400001049041748f, 2.0f},
    {6.199999809265137f, 0.5f},
    {4.300000190734863f, -1.5f},
    {3.799999952316284f, -3.0f},
    {3.4000000953674316f, -6.0f},
    {2.799999952316284f, -8.5f},
    {1.5f, -11.0f},
    {-0.20000000298023224f, -13.5f},
    {-0.699999988079071f, -15.5f},
    {-0.6000000238418579f, -17.5f},
};
constexpr AuthoredPath unit14Paths[] = {{unit14Path0}};
constexpr Vector2 unit16Path0[] = {
    {0.0f, 8.899999618530273f},
    {0.0f, 7.099999904632568f},
    {0.0f, 4.699999809265137f},
    {0.0f, 2.0f},
    {0.0f, -1.0f},
    {0.0f, -3.299999952316284f},
    {0.0f, -6.0f},
    {0.0f, -8.0f},
};
constexpr AuthoredPath unit16Paths[] = {{unit16Path0}};
constexpr Vector2 unit17Path0[] = {
    {0.10000000149011612f, 9.137001037597656f},
    {0.10000000149011612f, 7.125999927520752f},
    {-0.10000000149011612f, 5.364999771118164f},
    {-0.10000000149011612f, 3.753000020980835f},
    {0.10000000149011612f, 1.4470000267028809f},
    {0.30000001192092896f, -0.04100000113248825f},
    {0.4000000059604645f, -1.5360000133514404f},
    {0.30000001192092896f, -3.494999885559082f},
    {0.20000000298023224f, -5.197000980377197f},
    {0.10000000149011612f, -7.061999797821045f},
    {0.10000000149011612f, -8.836999893188477f},
};
constexpr AuthoredPath unit17Paths[] = {{unit17Path0}};
constexpr Vector2 unit21Path0[] = {
    {0.13924099504947662f, 7.984930992126465f},
    {0.17633099853992462f, 5.944645881652832f},
    {0.02797199971973896f, 3.5704970359802246f},
    {-0.027672000229358673f, 0.6770049929618835f},
    {-0.23171000182628632f, -2.661642074584961f},
    {-0.2874009907245636f, -5.888998985290527f},
    {-0.13904200494289398f, -8.09621524810791f},
};
constexpr AuthoredPath unit21Paths[] = {{unit21Path0}};
constexpr Vector2 unit23Path0[] = {
    {0.0f, 22.0f},
    {0.0f, 21.0f},
    {0.05650800094008446f, 18.606828689575195f},
    {0.17621000111103058f, 15.568537712097168f},
    {0.6230649948120117f, 13.185556411743164f},
    {1.2483550310134888f, 10.77278995513916f},
    {1.0994759798049927f, 7.615344047546387f},
    {0.08679000288248062f, 5.768537998199463f},
    {-1.6406739950180054f, 4.0110931396484375f},
    {-3.904664993286133f, 1.8962010145187378f},
    {-5.364343166351318f, -0.48677799105644226f},
    {-5.662099838256836f, -1.8569910526275635f},
    {-5.691934108734131f, -4.299543857574463f},
    {-5.334601879119873f, -5.610180854797363f},
    {-3.8752119541168213f, -7.9633708000183105f},
    {-1.939054012298584f, -10.882519721984863f},
    {-0.7173389792442322f, -14.218689918518066f},
    {-0.09177300333976746f, -17.614431381225586f},
    {0.02738800086081028f, -20.02718734741211f},
    {-0.0023880000226199627f, -21.96335792541504f},
};
constexpr AuthoredPath unit23Paths[] = {{unit23Path0}};
constexpr Vector2 unit27Path0[] = {
    {0.0f, -3.869999885559082f},
    {-1.600000023841858f, -3.4100000858306885f},
    {-2.700000047683716f, -2.2300000190734863f},
    {-3.3399999141693115f, -1.1100000143051147f},
    {-3.640000104904175f, 0.6800000071525574f},
    {-2.5999999046325684f, 2.609999895095825f},
    {-1.1699999570846558f, 4.070000171661377f},
    {-0.12999999523162842f, 6.150000095367432f},
    {0.03999999910593033f, 8.09000015258789f},
};
constexpr AuthoredPath unit27Paths[] = {{unit27Path0}};
constexpr Vector2 unit28Path0[] = {
    {-0.9522500038146973f, 7.930092811584473f},
    {-1.4557980298995972f, 6.362894058227539f},
    {-1.6240689754486084f, 4.068066120147705f},
    {-1.5401769876480103f, 0.7657529711723328f},
    {-1.2604069709777832f, -2.480587959289551f},
    {-0.365092009305954f, -5.587000846862793f},
    {-0.08529999852180481f, -7.126214981079102f},
    {-0.029349999502301216f, -8.273628234863281f},
};
constexpr AuthoredPath unit28Paths[] = {{unit28Path0}};
constexpr Vector2 unit29Path0[] = {
    {-5.978970050811768f, -14.110830307006836f},
    {-7.628101825714111f, -11.809833526611328f},
    {-8.539463996887207f, -9.899574279785156f},
    {-10.23199462890625f, -7.8156538009643555f},
    {-11.664944648742676f, -6.0356340408325195f},
    {-12.619705200195312f, -3.560978889465332f},
    {-13.010289192199707f, -1.042907953262329f},
    {-12.489510536193848f, 1.518576979637146f},
    {-11.231013298034668f, 4.470798015594482f},
    {-9.364890098571777f, 6.728377819061279f},
    {-6.499682903289795f, 9.593769073486328f},
    {-3.4618079662323f, 12.415743827819824f},
    {-1.2476069927215576f, 15.628458976745605f},
    {-0.5966339707374573f, 17.538719177246094f},
};
constexpr AuthoredPath unit29Paths[] = {{unit29Path0}};
constexpr Vector2 unit30Path0[] = {
    {0.16825300455093384f, 4.084362030029297f},
    {0.15165899693965912f, 5.826985836029053f},
    {0.12224899977445602f, 7.894902229309082f},
    {0.13506600260734558f, 10.042478561401367f},
    {0.0696600005030632f, 12.781872749328613f},
};
constexpr AuthoredPath unit30Paths[] = {{unit30Path0}};
constexpr Vector2 unit31Path0[] = {
    {3.4000000953674316f, -2.9570000171661377f},
    {3.299999952316284f, -0.039000000804662704f},
    {2.299999952316284f, 2.7980000972747803f},
    {0.8999999761581421f, 5.392000198364258f},
    {0.0f, 7.90500020980835f},
    {-0.6000000238418579f, 10.905000686645508f},
    {-0.800000011920929f, 13.039999961853027f},
};
constexpr AuthoredPath unit31Paths[] = {{unit31Path0}};
constexpr Vector2 unit32Path0[] = {
    {0.0f, 22.0f},
    {0.0f, 20.0f},
    {0.0f, 18.0f},
    {0.18047699332237244f, 15.699812889099121f},
    {0.9443749785423279f, 11.980733871459961f},
    {1.1480820178985596f, 9.586258888244629f},
    {0.5369629859924316f, 7.242730140686035f},
    {-1.3485020399093628f, 4.593527793884277f},
    {-3.3346381187438965f, 2.3009450435638428f},
    {-5.0661420822143555f, -0.09352999925613403f},
    {-5.6273980140686035f, -2.589898109436035f},
    {-5.525545120239258f, -4.882481098175049f},
    {-4.455692768096924f, -7.480741024017334f},
    {-2.5204830169677734f, -10.079001426696777f},
    {-0.8897690176963806f, -13.798078536987305f},
    {-0.2786499857902527f, -16.90580177307129f},
    {-0.023899000138044357f, -19.809736251831055f},
    {0.02702699974179268f, -21.898534774780273f},
};
constexpr AuthoredPath unit32Paths[] = {{unit32Path0}};

constexpr AuthoredUnit authoredUnits[] = {
    {"", {}, 10},
    {"", {}, 10},
    {"UNTIIT2", unit2Paths, 9},
    {"", {}, 10},
    {"", {}, 10},
    {"UNTSOT2", unit5Paths, 9},
    {"UNTH3T1", unit6Paths, 9},
    {"UNTHIY1", unit7Paths, 9},
    {"UNTHIY2", unit8Paths, 9},
    {"UNTHIY3", unit9Paths, 9},
    {"UNTHIY4", unit10Paths, 9},
    {"UNTIIA1", unit11Paths, 6},
    {"UNTIIA2", unit12Paths, 4},
    {"UNTIIA3", unit13Paths, 4},
    {"UNTIIA4", unit14Paths, 8},
    {"UNTIIA5", {}, 7},
    {"UNTYI01", unit16Paths, 4},
    {"UNTY302", unit17Paths, 4},
    {"", {}, 10},
    {"", {}, 10},
    {"", {}, 10},
    {"UNTHIA1", unit21Paths, 4},
    {"", {}, 10},
    {"UNTHOA0", unit23Paths, 6},
    {"UNTSRA1", {}, 6},
    {"UNTSRA2", {}, 6},
    {"UNTRIA1", {}, 6},
    {"UNTHIE1", unit27Paths, 0},
    {"UNTH3A3", unit28Paths, 4},
    {"UNTH3E1", unit29Paths, 3},
    {"UNTH3E2", unit30Paths, 1},
    {"UNTI3E1", unit31Paths, 2},
    {"UNTHOS0", unit32Paths, 6},
    {"", {}, 10},
    {"", {}, 10},
    {"", {}, 10},
    {"", {}, 10},
    {"", {}, 10},
    {"", {}, 10},
    {"", {}, 10},
    {"QTE2RA1", {}, 4},
    {"QTE1OH0", {}, 4},
    {"QTE13T1", {}, 4},
    {"QTE1RJ1", {}, 4},
    {"QTE2OB0", {}, 4},
    {"QTE1OR0", {}, 4},
    {"QTE2OD0", {}, 4},
    {"QTE1IK1", {}, 4},
    {"QTE1OL0", {}, 4},
    {"QTE1OH1", {}, 4},
    {"QTE33A1", {}, 4},
    {"QTE33E1", {}, 4},
    {"QTE1IK2", {}, 4},
    {"QTE23C1", {}, 5},
    {"QTE3OC1", {}, 4},
    {"QTE3OB1", {}, 4},
    {"QTE3OD1", {}, 4},
    {"QTE3OF1", {}, 4},
    {"", {}, 10},
    {"", {}, 10},
    {"", {}, 10},
    {"", {}, 10},
};

std::vector<RoadBlock> roadBlocks;
std::vector<void *> allocations;
std::int32_t unitCount = 0;
std::array<MazePartition, 32> partitions{};
std::array<std::array<EndpointLinks, 2>, 32> links{};
std::int32_t partitionCount = 0;
std::span<const std::byte> markerBytes;
std::int32_t markerCount = 0;
Vector3 *splineBuffer = nullptr;
Vector3 *routeBuffer = nullptr;
std::span<const std::byte> unitAreas;
const char *terrainPackage = nullptr;
bool ownsTerrainPackage = false;
HLib::Task *mazeTask = nullptr;
std::uint32_t mazeCurrentReferenceCount = 0;
std::uint32_t mazeFrameCount = 0;

Vector3 *fishControlPoints{};
Vector3 *fishPathSamples{};
std::int32_t fishControlCount{};
std::int32_t fishPathRingLength{};
std::int32_t fishPathCursor{};
std::uint32_t fishLastReferenceCount{};
Vector3 fishBoxMin{};
Vector3 fishBoxMax{};
std::uint32_t fishCount{};
std::int32_t fishSeed{};
const MT7Model *fishModel{};
HLib::Task *fishTask{};
float fishScale{1.0f};
bool fishEnabled{};
bool fishForceYaw{};
std::uint16_t fishForcedYaw{};

struct MazeModelResource {
    MT7Model *model{};
    MazeModelReader reader;
};
std::array<MazeModelResource, 24> roadModels;
std::array<MazeModelReader *, 24> activeRoadModels{};
std::array<MazeModelResource, std::size(authoredUnits)> unitModels;
std::array<MazeModelResource, 10> authoredObjectModels;
MazeAuthoredObject authoredObjects[] = {
    {.name = SHEN_NAME(HIA1), .resource = "KAWHIA1G", .flags = 0x90004},
    {.name = SHEN_NAME(IIT2), .resource = "KAWIIT2G", .flags = 0x90004},
    {.name = SHEN_NAME(IIA4), .resource = "TAKIIA4G", .flags = 0x50012},
    {.name = SHEN_NAME(IIA4), .resource = "TUBIIA4G", .flags = 0x150012},
    {.name = SHEN_NAME(IIA4), .resource = "KAWIIA4G", .flags = 0x150014},
    {.name = SHEN_NAME(HIY1), .resource = "KAWHIEKG", .flags = 0xe0008},
    {.name = SHEN_NAME(HIY1), .resource = "IWNA2L1T", .flags = 0x8000},
    {.name = SHEN_NAME(H3E1), .resource = "KAWH3E1G", .flags = 0xe0008},
    {.name = SHEN_NAME(H3E1), .resource = "IWNA2L1T", .flags = 0x8000},
    {.name = SHEN_NAME(HIE1), .resource = "TAKHIE1G", .flags = 0x50011},
    {.flags = 0xffffffff},
};
std::array<MazeRoad *, 120> currentMazeObjects{};

std::array<MazeRoad *, 9> currentMazeUnitObjects{};


float mazeUnitDistance = 60.0f;
std::int32_t mazeTerrainDrawEnabled = 1;
std::int32_t mazeRoadDrawEnabled = 1;
std::int32_t mazeUnitDrawEnabled = 1;
std::int32_t mazeRoadPropsEnabled = 1;
std::int32_t mazeCullingSteps = 13;
constexpr float mazeBob[] = {1.0f, 0.875f, 0.75f, 0.5f, 0.0f, -0.5f, -0.75f, -0.875f, -1.0f, -0.875f, -0.75f, -0.5f, 0.0f, 0.5f, 0.75f, 0.875f};
float mazeHourBlend = 0.0000099999997f;
std::int32_t mazeHourFrame = 2;
std::int32_t mazePreviousHourFrame = 2;
DateTime mazeHourDate{};
struct MazeRoadCommandCopy {
    std::span<std::byte> bytes;
    LegacyDrawResources resources;
};
std::array<MazeRoadCommandCopy, 240> roadCommandCopies;
std::uint32_t roadCommandCopySize{};
MazeModelResource terrainModel;
Matrix4x4 mazeViewTransform{};
Vector3 mazeCameraPosition{};
bool LoadMazeModel(const LoadedMapPackage &package, const char *name, MazeModelResource &resource);
bool InitializeMazeAuthoredObjects(const LoadedMapPackage &package);
shen_name MazeUnitAuthoredName(std::int32_t modelIndex);
std::int32_t AuthoredWord(const RoadBlock &block, std::int32_t record, std::size_t offset);
void ScanMazeAuthoredObjects(shen_name name, const MazeRoad &context);
std::size_t MazeRoadModelIndex(const MazeRoad &road);
std::span<const std::byte> FindTerrainEntry(const LoadedMapPackage &package, const char *name);
void *Allocate(std::size_t bytes, shen_name name);
std::array<MazeModelResource, 35> mediumModels;
std::array<MazeModelResource, 9> treeModels;
std::array<MazeModelResource, 12> auxiliaryModels;
std::array<std::span<const MazePropRecord>, 6> propTables{};
std::array<std::array<MazePropState, 32>, 120> propStates;
std::array<std::uint8_t, 7000> mediumOccupancy{};
Matrix4x4 mazeFlutterTransform{};
Matrix4x4 mazeEffectTransform{};
bool mazeClearWeather{};
bool mazeHeavyWeather{};
constexpr std::int32_t mazeWiggle[] = {0, 16, 32, 40, 48, 52, 56, 58, 60, 58, 56, 52, 48, 40, 32, 16};

bool MazeRecordMatchesIndex(std::uint32_t flags, std::int32_t index) {
    return (!(flags & 0x400U) || !(index & 1)) && (!(flags & 0x800U) || index == 3 * (index / 3)) && (!(flags & 0x1000U) || index % 4 == 0);
}

// @verified - p
bool ShouldGenerateMazeProp(const MazePropRecord &record, const MazeRoad &road) {
    if ((record.flags & 0x2000U) != 0)
        return false;
    const float chance = IncrementSharedPrngFloat();
    return record.chance > chance && MazeRecordMatchesIndex(record.flags, road.index);
}

// @verified - p
const void *FindFreeEntryInMazeObjectPoolOfKind(std::int32_t kind) {
    for (std::size_t index = 0; index < 200; ++index) {
        std::uint8_t &occupied = mediumOccupancy[index * 35 + kind];
        if (!occupied) {
            occupied = 1;
            return &occupied;
        }
    }
    char diagnostic[256];
    std::snprintf(diagnostic, sizeof(diagnostic), "Couldn't find free entry in maze object pool of kind: %i", kind);
    return nullptr;
}

// @verified - p
MazePropState *MakeMazePropState(const MazePropRecord *record, MazePropState *output) {
    if (!record)
        return output;
    float x = IncrementSharedPrngFloat() * (record->maxX - record->minX) + record->minX;
    if (IncrementSharedPrngFloat() > 0.5f)
        x = -x;
    float z = IncrementSharedPrngFloat() * 2.0f - 1.0f;
    if (((record->flags & 1U) && x > 0.0f) || ((record->flags & 2U) && x < 0.0f))
        x = -x;
    if (record->flags & 0x40U)
        z = 0.0f;
    std::int32_t yaw = 2 * IncrementSharedPrngUInt16();
    if (record->flags & 8U)
        yaw = 0;
    if (record->flags & 0x10U)
        yaw = 0x8000;
    if (record->flags & 0x20U)
        yaw = yaw > 0x8000 ? 0x8000 : 0;
    output->kind = record->kind;
    output->flags = record->flags;
    output->drawState = record->drawState;
    output->yaw = yaw;
    output->x = x;
    output->z = z;
    output->height = record->height;
    output->placement = FindFreeEntryInMazeObjectPoolOfKind(record->kind);
    ++output;
    if (record->flags & 4U) {
        output->kind = record->kind;
        output->flags = record->flags;
        output->drawState = record->drawState;
        output->yaw = yaw;
        output->x = -x;
        output->z = z;
        output->height = record->height;
        output->placement = FindFreeEntryInMazeObjectPoolOfKind(record->kind);
        ++output;
    }
    output->kind = -1;
    return output;
}

void GenerateMazePropStates(const MazeRoad &road, std::size_t drawIndex) {
    SetSharedPrngSeed(static_cast<std::uint32_t>(road.index) + (static_cast<std::uint32_t>(road.block) << 11));
    const auto table = propTables[MazeRoadModelIndex(road) / 4];
    MazePropState *output = propStates[drawIndex].data();
    for (std::size_t index = 0; index < table.size() && table[index].flags != 0xffffffffU; ++index) {
        const MazePropRecord &record = table[index];
        if (record.flags & 0x4000U) {
            ++index;
            if (MazeRecordMatchesIndex(table[index].flags, road.index)) {
                std::size_t count = 0;
                if (!(table[index].flags & 0xa000U)) {
                    do {
                        ++count;
                    } while (!(table[index + count].flags & 0x8000U));
                }
                const MazePropRecord *selected =
                    count ? &table[index + static_cast<std::size_t>(IncrementSharedPrngFloat() * static_cast<float>(count))] : nullptr;
                output = MakeMazePropState(selected, output);
                while (!(table[index].flags & 0x8000U))
                    ++index;
            }
        } else if (ShouldGenerateMazeProp(record, road))
            output = MakeMazePropState(&record, output);
    }
}

void DrawMazePropStates(const MazeRoad &road, std::size_t drawIndex) {
    for (const MazePropState &prop : propStates[drawIndex]) {
        if (prop.kind < 0)
            break;
        Matrix4x4_Push(&transformMatrix_prePass);
        Matrix4x4::preTranslate(road.position.x, road.position.y, road.position.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(road.yaw));
        Matrix4x4::preTranslate(prop.x, prop.height, prop.z);
        if (prop.yaw)
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(prop.yaw));
        d3t::HashModel(prop.placement, GetLegacyPerspectiveProjection());
        const bool hashed = road.alpha == 1.0f ? d3t::HashModel(prop.placement, GetLegacyPerspectiveProjection()) :
                                                 d3t::HashModelWithAlpha(prop.placement, road.alpha, GetLegacyPerspectiveProjection());
        if (!hashed) {
            Matrix4x4::Pop(1);
            Matrix4x4_Push();
            Matrix4x4::preTranslate(prop.x, prop.height, prop.z);
            MT7Model &model = *mediumModels[prop.kind].model;
            model.nodes[0].rotation.y = static_cast<float>(prop.yaw);
            if (prop.drawState) {
                if (road.alpha == 1.0f)
                    charaApplyScale(&model, model.nodes.data());
                else {
                    SetLegacyBusyPreparingDraw(true);
                    charaPrepareNodesForDraw(&model, model.nodes.data(), road.alpha);
                    SetLegacyBusyPreparingDraw(false);
                }
            }
        }
        Matrix4x4::Pop(1);
        ++g_mazePropDrawCount;
    }
}

struct MazeAuxiliaryDefinition {
    shen_name area;
    const char *model;
    Vector3 position;
    std::int32_t yaw;
};
constexpr MazeAuxiliaryDefinition mazeAuxiliaryDefinitions[] = {
    {.area = SHEN_NAME(KMZA), .model = "MAZHOB1G", .position = {0.63f, 0.82f, -5.58f}, .yaw = 13747},
    {.area = SHEN_NAME(KMZA), .model = "MAZHOB2G", .position = {-6.5f, 0.83f, 3.2f}, .yaw = 57317},
    {.area = SHEN_NAME(KMZA), .model = "MAZHOB3G", .position = {-6.96f, 1.14f, 3.38f}, .yaw = 2545},
    {.area = SHEN_NAME(KMZA), .model = "MAZHOP3G", .position = {-0.2f, 0.0f, -6.0f}, .yaw = 55252},
    {.area = SHEN_NAME(KMZA), .model = "MAZHOP1G", .position = {1.44f, 0.15f, -5.61f}, .yaw = 64535},
    {.area = SHEN_NAME(KMZA), .model = "MAZHOP2G", .position = {-5.59f, 0.7f, 1.88f}, .yaw = 32260},
    {.area = SHEN_NAME(KMZ1), .model = "KANSOT2G", .position = {0.0f, 0.0f, -3.5f}},
    {.area = SHEN_NAME(KMZA), .model = "KANSRB1G"},
    {.area = SHEN_NAME(KMZ4), .model = "KANIRB1G"},
    {.area = SHEN_NAME(KMZ3), .model = "ZIZHIY4G", .position = {6.48f, 1.72f, 14.08f}},
    {.area = SHEN_NAME(KMZ4), .model = "KOMHIY4G", .position = {6.48f, 1.18f, 14.08f}},
    {.area = SHEN_NAME(KMZA), .model = "IWAR001G"},
};
struct MazeRoadDecoration {
    shen_name area;
    std::int32_t marker;
    std::int32_t block;
    std::int32_t index;
    Vector3 position;
    std::int32_t yaw;
    std::int32_t kind;
};
MazeRoadDecoration mazeMarkerDecorations[] = {
    {.area = SHEN_NAME(KMZ2), .position = {-2.1f, 0.0f, 0.0f}, .kind = 7},
    {.area = SHEN_NAME(KMZ3), .position = {-2.1f, 0.0f, 0.0f}, .kind = 7},
    {.area = SHEN_NAME(KMZ4), .position = {2.1f, 0.0f, 0.0f}, .yaw = 0x8000, .kind = 8},
};
constexpr MazeRoadDecoration mazeFixedDecorations[] = {
    {.area = SHEN_NAME(KMZ4), .block = 1, .index = 88, .position = {-9.0f, 0.0f, 0.0f}, .kind = 25},
    {.area = SHEN_NAME(KMZ4), .block = 1, .index = 122, .position = {9.0f, 0.0f, 0.0f}, .yaw = 0x8000, .kind = 25},
    {.area = SHEN_NAME(KMZ4), .block = 4, .index = 3, .position = {-9.0f, 0.0f, 0.0f}, .kind = 25},
};

bool InitializeMazeEnvironmentModels(const LoadedMapPackage &package) {
    constexpr const char *tableNames[] = {"tbl_mori.BIN", "tbl_iwa.BIN", nullptr, nullptr, nullptr, "tbl_lwod.BIN"};
    std::array<bool, 35> used{};
    std::array<bool, 6> visited{};
    for (const RoadBlock &block : roadBlocks) {
        for (std::int32_t index = 0; index < block.controlCount; ++index) {
            MazeRoad authored{};
            authored.flags = static_cast<std::uint32_t>(AuthoredWord(block, index, 28)) << 16;
            const std::size_t kind = MazeRoadModelIndex(authored) / 4;
            if (visited[kind])
                continue;
            visited[kind] = true;
            if (!tableNames[kind])
                return false;
            const auto bytes = FindTerrainEntry(package, tableNames[kind]);
            if (bytes.empty())
                return false;
            propTables[kind] = {reinterpret_cast<const MazePropRecord *>(bytes.data()), bytes.size() / sizeof(MazePropRecord)};
            for (const MazePropRecord &record : propTables[kind]) {
                if (record.flags == 0xffffffffU)
                    break;
                if (!(record.flags & 0xc000U))
                    used[record.kind] = true;
            }
        }
    }
    constexpr const char *mediumNames[] = {"SRB1", "SRB2", "SRB3", "IRB1", "IRB2", "IRB3", "KRB1", "KRB2", "KRB3", "IOB0", "IOB1", "IOB2", "IRM1", "IRM2",
        "IRM3", "IRM4", "IRM5", "HOM0", "H3F1", "H3F2", "H3F3", "H3F4", "H3F5", "H3F6", "H3F7", "H3F8", "H3F9", "HOM3", "HIJ1", "HIJ2", "HIJ3", "HOJ0", "HOJ1",
        "H3J1", "H3J2"};
    for (std::size_t index = 0; index < used.size(); ++index) {
        if (!used[index])
            continue;
        char name[16];
        std::snprintf(name, sizeof(name), "MAZ%sG", mediumNames[index]);
        if (!LoadMazeModel(package, name, mediumModels[index]))
            return false;
    }
    return true;
}

bool InitializeMazeAuxiliaryModels(const LoadedMapPackage &package) {
    for (std::size_t index = 0; index < std::size(mazeAuxiliaryDefinitions); ++index) {
        const MazeAuxiliaryDefinition &definition = mazeAuxiliaryDefinitions[index];
        if (definition.area.name == g_currentMap.name || definition.area.name == SHEN_NAME(KMZA))
            if (!LoadMazeModel(package, definition.model, auxiliaryModels[index]))
                return false;
    }
    if (!InitializeMazeAuthoredObjects(package))
        return false;
    constexpr const char *treeNames[] = {"EFFHIS1G", "EFFHIM1G", "EFFHIL1G", "EFFPISSG", "EFFPIS1G", "EFFPIM1G", "EFFPIL1G", "MON02L4P", "MONY2L4P"};
    for (std::size_t index = 0; index < std::size(treeNames); ++index)
        if (!LoadMazeModel(package, treeNames[index], treeModels[index]))
            return false;
    return true;
}

void BindMazeMarkerDecorations() {
    for (MazeRoadDecoration &decoration : mazeMarkerDecorations) {
        if (decoration.area.name != g_currentMap.name)
            continue;
        Vector3 position{};
        mazeGetMarkerPosition(decoration.marker, &position);
        float closest = 1000000.0f;
        for (const RoadBlock &block : roadBlocks)
            for (std::int32_t index = 0; index < block.roadCount; ++index) {
                const MazeRoad &road = block.roads[index];
                const float x = position.x - road.position.x;
                const float z = position.z - road.position.z;
                if (closest > z * z + x * x) {
                    closest = z * z + x * x;
                    decoration.block = road.block;
                    decoration.index = road.index;
                }
            }
    }
}

void DrawMazeAuxiliaryUnit(const MazeRoad &road, shen_name name) {
    SetSharedPrngSeed(static_cast<std::uint32_t>(road.unit) + (static_cast<std::uint32_t>(road.block) << 8));
    std::int32_t variation = static_cast<std::int32_t>(IncrementSharedPrngFloat() * 3.0f);
    if (g_currentMap.name == SHEN_NAME(KMZ1) && road.unit == 1)
        variation = 3;
    if (g_currentMap.name == SHEN_NAME(KMZ3) && name.name == SHEN_NAME(HIY4))
        variation = 4;
    if (g_currentMap.name == SHEN_NAME(KMZ4) && name.name == SHEN_NAME(HIY4))
        variation = 5;
    bool selected[2]{true, false};
    if (variation < 3) {
        selected[0] = IncrementSharedPrngFloat() < 0.65f;
        selected[1] = IncrementSharedPrngFloat() < 0.65f;
    }
    constexpr std::int32_t modelIndices[][2] = {{0, 2}, {1, 3}, {4, 5}, {6, 0}, {9, 0}, {10, 0}};
    for (std::size_t index = 0; index < 2; ++index) {
        if (!selected[index])
            continue;
        const std::size_t modelIndex = modelIndices[variation][index];
        const MazeAuxiliaryDefinition &definition = mazeAuxiliaryDefinitions[modelIndex];
        MT7Model &model = *auxiliaryModels[modelIndex].model;
        Matrix4x4_Push(&transformMatrix_prePass);
        Matrix4x4::preTranslate(road.position.x, road.position.y, road.position.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(road.yaw));
        Matrix4x4::preTranslate(definition.position.x, definition.position.y, definition.position.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(definition.yaw));
        if (!d3t::HashModel(GetMT7ReaderData(&model), GetLegacyPerspectiveProjection())) {
            Matrix4x4::Pop(1);
            Matrix4x4_Push();
            Matrix4x4::preTranslate(definition.position.x, definition.position.y, definition.position.z);
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(definition.yaw));
            charaApplyScale(&model, model.nodes.data());
        }
        Matrix4x4::Pop(1);
    }
}

void ApplyMazeRoadDecoration(const MazeRoad &road, const MazeRoadDecoration &decoration, MT7Model &model, bool clearYaw) {
    if (decoration.area.name != g_currentMap.name || decoration.block != road.block || decoration.index != road.index || model.nodes.empty())
        return;
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::preTranslate(road.position.x, road.position.y, road.position.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(road.yaw));
    Matrix4x4::preTranslate(decoration.position.x, decoration.position.y, decoration.position.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(decoration.yaw));
    if (!d3t::HashModel(GetMT7ReaderData(&model), GetLegacyPerspectiveProjection())) {
        Matrix4x4::Pop(1);
        Matrix4x4_Push();
        Matrix4x4::preTranslate(decoration.position.x, decoration.position.y, decoration.position.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(decoration.yaw));
        if (clearYaw)
            model.nodes[0].rotation.y = 0.0f;
        charaApplyScale(&model, model.nodes.data());
    }
    Matrix4x4::Pop(1);
}

void SwizzledBasis3x4IntoActiveFrom(const Matrix4x4 &source) {
    for (std::size_t row = 0; row < 3; ++row) {
        workTransformMatrix.rows[row].x = source.rows[row].x;
        workTransformMatrix.rows[row].y = source.rows[row].y;
        workTransformMatrix.rows[row].z = source.rows[row].z;
        workTransformMatrix.rows[row].w = 0.0f;
    }
}

void DrawMazeEffectMesh(MT7Model &model, std::int32_t node, const float *alpha = nullptr) {
    const MT7Node &mesh = model.nodes[node];
    if (!alpha)
        ModelFlagDispatch_SetModelFlagDataOpaque(model.source, mesh.xb01Offset, mesh.borrowedMesh ? mesh.borrowedMesh->drawResources : mesh.drawResources);
    else
        SetModelFlagDataWithAlpha(model.source, mesh.xb01Offset, mesh.borrowedMesh ? mesh.borrowedMesh->drawResources : mesh.drawResources, *alpha);
}

void InitializeMazeFrameEnvironment() {
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::GetTransformMatrix(&mazeFlutterTransform);
    Matrix4x4::RotateAroundX_Wrapper(4608);
    Matrix4x4::RotateAroundZ_Wrapper(4608);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>((mazeCurrentReferenceCount & 0x1ff) << 7));
    Matrix4x4::GetTransformMatrix(&mazeEffectTransform);
    Matrix4x4::Pop(1);
    mazeClearWeather = false;
    mazeHeavyWeather = false;
    DateTime date{};
    GetCurrentDateTime(date);
    if (static_cast<std::uint8_t>(date.hour - 7) <= 9) {
        mazeHeavyWeather = GetWeatherPressure() > 0.69999999f;
        if (GetWeatherPressure() < 0.30000001f)
            mazeClearWeather = GetWeatherWindVector() < 0.5f;
    }
    camCtrlGetNextCamPos(&mazeCameraPosition);
}

void FreeNecessaryMazeObjects() {
    constexpr std::int32_t children[][2] = {
        {27, 0}, {27, 1}, {27, 2}, {27, 3}, {18, 4}, {19, 1}, {19, 3}, {20, 2}, {20, 3}, {22, 4}, {24, 3}, {26, 2}, {26, 3}};
    const std::uint32_t phase = mazeCurrentReferenceCount >> 1;
    std::uint8_t offset = 0;
    for (const auto &child : children) {
        if (!mediumModels[child[0]].model) {
            offset += 4;
            continue;
        }
        MT7Model &model = *mediumModels[child[0]].model;
        if (!model.nodes.empty()) {
            std::int32_t node = model.nodes[0].firstChild;
            for (std::int32_t index = 0; index < child[1]; ++index)
                node = model.nodes[node].nextSibling;
            const std::uint8_t angle = static_cast<std::uint8_t>(phase + offset + child[0]);
            model.nodes[node].rotation = {0, ((angle & 0x10) ? 1 : -1) * mazeWiggle[angle & 15], 0};
        }
        offset += 4;
    }
}

void ApplyMazeUnitChildWiggle() {
    for (std::int32_t modelIndex : {23, 32}) {
        if (!unitModels[modelIndex].model)
            continue;
        MT7Model &model = *unitModels[modelIndex].model;
        if (model.nodes.empty())
            continue;
        std::int32_t node = model.nodes[0].firstChild;
        node = model.nodes[node].firstChild;
        std::uint8_t phase = static_cast<std::uint8_t>(mazeCurrentReferenceCount >> 1);
        while (node >= 0) {
            const std::int32_t angle = ((phase & 0x10) ? 3 : -3) * mazeWiggle[phase & 15];
            model.nodes[node].rotation = {angle, angle, 0};
            phase += 2;
            node = model.nodes[node].nextSibling;
        }
    }
}

void DrawMazeFlutterEffects(const MazeRoad &road) {
    const std::size_t kind = MazeRoadModelIndex(road) / 4;
    if ((road.flags & 0x10000000U) || road.cameraDistance > 256.0f || !mazeClearWeather || (kind != 0 && kind != 5))
        return;
    if (kind == 0 && MazeRecordMatchesIndex(0x1000, road.index))
        return;
    const float centerX = kind == 5 ? 0.60000002f : 4.099999904632568f;
    const float centerY = kind == 5 ? 11.6f : 3.700000047683716f;
    SetSharedPrngSeed(2U * (static_cast<std::uint32_t>(road.index) + 300U * static_cast<std::uint32_t>(road.block)));
    const std::int32_t count = 3 - static_cast<std::int32_t>(IncrementSharedPrngFloat() * -3.0f);
    for (std::int32_t index = 0; index < count; ++index) {
        const std::uint16_t random = IncrementSharedPrngUInt16();
        if (((static_cast<std::uint8_t>(random) * static_cast<std::uint8_t>(mazeCurrentReferenceCount >> 3)) & 0x70) == 0)
            continue;
        MT7Model &model = *treeModels[(random & 3) + 3].model;
        float x = IncrementSharedPrngFloat() * 0.40000001f - 0.2f + centerX;
        const float y = IncrementSharedPrngFloat() * 1.6f - 0.80000001f + centerY;
        const float z = IncrementSharedPrngFloat() * 2.0f - 1.0f;
        if (IncrementSharedPrngFloat() > 0.5f)
            x = -x;
        const std::int32_t distance = static_cast<std::int32_t>(static_cast<double>(z * z + x * x + road.cameraDistance) * 100.0);
        if (!(distance & 0xe))
            continue;
        const std::int32_t shift = distance & 0xffe000 ? distance >> 12 : distance >> 10;
        if (!((random >> (shift & 7)) & 1))
            continue;
        float alpha = distance & 4 ? 0.70000005f : 0.40000001f;
        if (distance & 2)
            alpha += 0.30000001f;
        Matrix4x4_Push(&transformMatrix_prePass);
        Matrix4x4::preTranslate(road.position.x, road.position.y, road.position.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(road.yaw));
        Matrix4x4::preTranslate(x, y, z);
        if (!d3t::HashModelWithAlpha(GetMT7ReaderData(&model), alpha, GetLegacyPerspectiveProjection())) {
            Matrix4x4::Pop(1);
            Matrix4x4_Push();
            Matrix4x4::preTranslate(x, y, z);
            SwizzledBasis3x4IntoActiveFrom(mazeFlutterTransform);
            DrawMazeEffectMesh(model, 0, &alpha);
        }
        Matrix4x4::Pop(1);
    }
}

void DrawMazeTreeEffects(const MazeRoad &road) {
    const std::size_t kind = MazeRoadModelIndex(road) / 4;
    if ((road.flags & 0x10000000U) || (kind != 0 && kind != 5))
        return;
    DrawMazeFlutterEffects(road);
    SetSharedPrngSeed(static_cast<std::uint32_t>(road.index) + (static_cast<std::uint32_t>(road.block) << 9));
    if (IncrementSharedPrngFloat() > 0.5f)
        return;
    const std::int32_t count = static_cast<std::int32_t>(IncrementSharedPrngFloat() * 1.1f + 1.0f);
    for (std::int32_t index = 0; index < count; ++index) {
        const float choice = IncrementSharedPrngFloat();
        const std::size_t modelIndex = choice < 0.44999999f ? 0 : choice < 0.80000001f ? 1 : 2;
        float x = IncrementSharedPrngFloat() - 0.5f + (kind == 5 ? 2.0f : 5.0f);
        const float y = IncrementSharedPrngFloat() * 2.0f - 1.0f + (kind == 5 ? 8.0f : 5.0f);
        const float z = IncrementSharedPrngFloat() - 0.5f;
        if (IncrementSharedPrngFloat() >= 0.5f)
            x = -x;
        Matrix4x4_Push();
        Matrix4x4::preTranslate(x, y, z);
        SwizzledBasis3x4IntoActiveFrom(mazeViewTransform);
        if (!GetLegacyRenderMask()) {
            MT7Model &model = *treeModels[modelIndex].model;
            const std::int32_t child = model.nodes[0].firstChild;
            DrawMazeEffectMesh(model, model.nodes[child].nextSibling);
            if (mazeClearWeather) {
                Matrix4x4::MultiplyWithPrePassMatrix(&mazeEffectTransform);
                const float alpha = 0.40000001f;
                DrawMazeEffectMesh(model, child, &alpha);
            }
        }
        Matrix4x4::Pop(1);
    }
}

void DrawMazeButterflyEffects(const MazeRoad &road) {
    if (static_cast<double>(IncrementSharedPrngFloat()) > 0.05)
        return;
    const std::int32_t count = static_cast<std::int32_t>(IncrementSharedPrngFloat() * 1.9f + 1.0f);
    for (std::int32_t index = 0; index < count; ++index) {
        const std::uint8_t random = static_cast<std::uint8_t>(IncrementSharedPrngUInt16());
        const std::uint8_t phase = (random + mazeCurrentReferenceCount) & 15;
        const Vector3 &point = mazeButterflyPath[phase & 7];
        float x = IncrementSharedPrngFloat() * 0.80000001f - 0.40000001f + 3.0f + point.x;
        const float y = ((phase & 8) ? 1.0f : -1.0f) * point.y + (IncrementSharedPrngFloat() * 0.80000001f - 0.40000001f + 1.5f);
        const float z = IncrementSharedPrngFloat() * 0.80000001f - 0.40000001f + point.z;
        if (road.index & 1)
            x = -x;
        Matrix4x4_Push();
        Matrix4x4::preTranslate(x, y, z);
        MT7Model &model = *treeModels[(random & 1) + 7].model;
        std::int32_t node = model.nodes[0].firstChild;
        node = model.nodes[node].firstChild;
        for (std::uint32_t frame = 0; frame < (mazeCurrentReferenceCount & 3); ++frame)
            node = model.nodes[node].nextSibling;
        DrawMazeEffectMesh(model, node);
        Matrix4x4::Pop(1);
    }
}

void DrawMazeRoadButterflies(const MazeRoad &road) {
    if (MazeRoadModelIndex(road) / 4 != 0)
        return;
    SetSharedPrngSeed(3U * static_cast<std::uint32_t>(road.index) + (static_cast<std::uint32_t>(road.block) << 9));
    if (!mazeHeavyWeather)
        DrawMazeButterflyEffects(road);
}

bool CheckLineSegmentIntersection(const Vector3 &a, const Vector3 &b, const Vector3 &c, const Vector3 &d) {
    const float crossC = (c.x - b.x) * (b.z - a.z) - (c.z - b.z) * (b.x - a.x);
    const float crossD = (d.x - b.x) * (b.z - a.z) - (d.z - b.z) * (b.x - a.x);
    if (crossC * crossD > -0.0f)
        return false;
    const float crossA = (a.x - d.x) * (d.z - c.z) - (a.z - d.z) * (d.x - c.x);
    const float crossB = (b.x - d.x) * (d.z - c.z) - (b.z - d.z) * (d.x - c.x);
    return crossA * crossB <= -0.0f;
}

template <std::size_t N> void InsertMazeObject(std::array<MazeRoad *, N> &list, std::size_t capacity, MazeRoad *road) {
    std::size_t begin = 0;
    std::size_t end = capacity;
    while (begin < end) {
        const std::size_t middle = (begin + end) / 2;
        if (!list[middle] || list[middle]->cameraDistance >= road->cameraDistance)
            end = middle;
        else
            begin = middle + 1;
    }
    if (begin == capacity)
        return;
    for (std::size_t index = capacity - 1; index > begin; --index)
        list[index] = list[index - 1];
    list[begin] = road;
}

std::int32_t UpdateXFormMazeObject(MazeRoad *roads, const Vector3 &camera, float &output) {
    float nearest = 25600.0f;
    std::int32_t nearestIndex = -1;
    const float far = camCtrlGetFarDistance();
    const float farSquared = far * far;
    for (MazeRoad *road = roads; road->index >= 0; ++road) {
        const float x = camera.x - road->position.x;
        const float z = camera.z - road->position.z;
        road->cameraDistance = z * z + x * x;
        if (static_cast<std::uint16_t>(road->flags) == 0) {
            if (farSquared > road->cameraDistance) {
                InsertMazeObject(currentMazeObjects, 119, road);

                if (1.0f > road->alpha)
                    road->alphaDirection = 1;
            } else if (road->alpha > 0.0f) {
                road->alphaDirection = 2;
                InsertMazeObject(currentMazeObjects, 119, road);
            }
        } else if (static_cast<std::int32_t>(road->flags) < 0) {
            if (mazeUnitDistance * mazeUnitDistance > road->cameraDistance) {
                InsertMazeObject(currentMazeUnitObjects, 8, road);
                if (1.0f > road->alpha)
                    road->alphaDirection = 1;
            } else if (road->alpha > 0.0f) {
                road->alphaDirection = 2;
                InsertMazeObject(currentMazeUnitObjects, 8, road);
            }
        }
        if (nearest > road->cameraDistance) {
            nearest = road->cameraDistance;
            nearestIndex = road->index;
        }
    }
    output = nearest;
    return nearestIndex;
}

bool ApplyTransformationForMazeObject(const MazeRoad &road, bool unit) {
    Matrix4x4_Push();
    Matrix4x4::SetPrePassToTempTransform();
    const std::uint16_t yaw = camCtrlGetNextRotY();
    Vector3 camera{};
    camCtrlGetNextCamPos(&camera);
    Matrix4x4::preTranslate(camera.x, camera.y, camera.z);
    Matrix4x4::RotateAroundY_Wrapper(yaw);
    Matrix4x4::preTranslate(0.0f, 0.0f, 3.0f);
    Matrix4x4 matrix{};
    Matrix4x4::GetTransformMatrix(&matrix);
    const Vector3 delta{road.position.x - matrix.rows[3].x, road.position.y - matrix.rows[3].y, road.position.z - matrix.rows[3].z};
    Matrix4x4::SetPrePassToTempTransform();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-yaw));
    Matrix4x4::preTranslate(delta.x, delta.y, delta.z);
    Matrix4x4::GetTransformMatrix(&matrix);
    Matrix4x4::Pop(1);
    const Vector3 point{matrix.rows[3].x, matrix.rows[3].y, matrix.rows[3].z};
    const float distance = point.z * point.z + point.x * point.x;
    const std::int32_t angle = FixedAtan2(-point.x, -point.z);
    if (unit)
        return !(distance >= 144.0f && point.z >= 26.0f && static_cast<std::uint32_t>(angle - 16385) <= 0x7ffeU);
    if (distance < 144.0f)
        return true;
    if (point.z > 0.0f)
        return false;
    const std::int32_t cullingAngle = mazeCullingSteps << 10;
    return !(angle > cullingAngle && angle < 0x10000 - cullingAngle);
}

// @verified - p
bool CheckLineSegmentIntersection_Wrapper(const MazeRoad &road) {
    Vector3 camera{};
    camCtrlGetNextCamPos(&camera);
    const MazeUnit &unit = MUZT_Data[road.unit];
    if (unit.pointCount <= 0)
        return false;
    std::int32_t blocked = 0;
    for (std::int32_t point = 0; point < unit.pointCount; ++point) {
        for (const MazeRoad *current : currentMazeObjects) {
            if (!current)
                break;
            if (CheckLineSegmentIntersection(camera, unit.points[point], current->rightStart, current->rightEnd) ||
                CheckLineSegmentIntersection(camera, unit.points[point], current->leftStart, current->leftEnd)) {
                ++blocked;
                break;
            }
        }
    }
    return blocked < unit.pointCount;
}

// @verified - p
bool GetDistanceClosest_ForMaze(const MazeRoad &road, const Vector3 &camera, const Vector3 &target) {
    for (const MazeRoad *current : currentMazeUnitObjects) {
        if (!current)
            break;
        if (!(road.cameraDistance > current->cameraDistance))
            continue;
        const MazeUnit &unit = MUZT_Data[current->unit];
        for (std::int32_t wall = 0; wall < unit.wallCount; ++wall) {
            const Vector3 &first = unit.walls[wall][0];
            const Vector3 &second = unit.walls[wall][1];
            const float crossFirst = (first.x - target.x) * (target.z - camera.z) - (first.z - target.z) * (target.x - camera.x);
            const float crossSecond = (second.x - target.x) * (target.z - camera.z) - (second.z - target.z) * (target.x - camera.x);
            if (!(crossFirst * crossSecond <= -0.0f))
                continue;
            const float crossTarget = (target.x - second.x) * (second.z - first.z) - (target.z - second.z) * (second.x - first.x);
            const float crossCamera = (camera.x - second.x) * (second.z - first.z) - (camera.z - second.z) * (second.x - first.x);
            if (crossTarget * crossCamera <= -0.0f) {
                ++g_mazeUnitWallHitCount;
                return true;
            }
        }
    }
    return false;
}

void ApplyTransformationForCurrentMazeObjects() {
    std::array<MazeRoad *, 120> candidates{};
    std::size_t count = 0;
    Matrix4x4_Push();
    Matrix4x4::SetPrePassToTempTransform();
    for (MazeRoad *road : currentMazeObjects) {
        if (!road)
            break;
        if (ApplyTransformationForMazeObject(*road, false))
            candidates[count++] = road;
    }
    if (count <= 8)
        currentMazeObjects = candidates;
    else {
        Vector3 camera{};
        camCtrlGetNextCamPos(&camera);
        for (std::size_t index = 0; index < 8; ++index)
            currentMazeObjects[index] = candidates[index];
        std::size_t admitted = 8;
        for (std::size_t index = 8; index < count; ++index) {
            MazeRoad &road = *candidates[index];
            auto blocked = [&](const Vector3 &target, bool includeSelf) {
                std::size_t previous = index + (includeSelf ? 1 : 0);
                while (previous) {
                    const MazeRoad &obstacle = *candidates[--previous];
                    if (CheckLineSegmentIntersection(camera, target, obstacle.leftStart, obstacle.leftEnd) ||
                        CheckLineSegmentIntersection(camera, target, obstacle.rightStart, obstacle.rightEnd))
                        return true;
                }
                return false;
            };
            std::int32_t occluded = blocked(road.leftCentre, false) || GetDistanceClosest_ForMaze(road, camera, road.leftCentre);
            if (occluded) {
                occluded += blocked(road.rightCentre, false);
                if (occluded < 1)
                    occluded += GetDistanceClosest_ForMaze(road, camera, road.rightCentre);
                if (occluded >= 1) {
                    occluded += blocked(road.position, true);
                    if (occluded < 2)
                        occluded += GetDistanceClosest_ForMaze(road, camera, road.position);
                }
            }
            if (occluded >= 2) {
                road.alphaDirection = road.alpha > 0.0f ? 2 : 0;
                if (road.alpha <= 0.0f)
                    continue;
            } else
                road.alphaDirection = road.alpha < 1.0f ? 1 : 0;
            currentMazeObjects[admitted++] = &road;
        }
        currentMazeObjects[admitted] = nullptr;
    }
    Matrix4x4::Pop(1);
}

void AdvanceMazeObjectAlpha(MazeRoad &road) {
    if (road.alphaDirection == 1)
        road.alpha += 0.1f;
    if (road.alphaDirection == 2)
        road.alpha -= 0.1f;
    if (road.alpha > 1.0f)
        road.alpha = 1.0f;
    if (road.alpha < 0.0f)
        road.alpha = 0.0f;
}

std::uint32_t MazeMeshWord(const MT7Model &model, std::uint32_t offset) {
    std::uint32_t word;
    std::memcpy(&word, model.source.data() + offset, sizeof(word));
    return word;
}

bool InitMazeModels(const LoadedMapPackage &package) {
    constexpr const char *names[] = {"SRZ1", "SRZ2", "SRZ3", "SRZ4", "IRZ1", "IRZ2", "IRZ3", "IRZ4", "KRZ1", "KRZ2", "KRZ3", "KRZ4", "NULL04", "NULL05",
        "NULL06", "NULL07", "NULL00", "NULL01", "NULL02", "NULL03", "NULL00", "NULL01", "NULL02", "NULL03"};
    for (std::size_t index = 0; index < std::size(names); ++index) {
        if (std::strncmp(names[index], "NULL", 4) == 0)
            activeRoadModels[index] = activeRoadModels[names[index][5] - 96 + names[index][4]];
        else {
            char filename[16];
            std::snprintf(filename, sizeof(filename), "MAZ%sG", names[index]);
            if (!LoadMazeModel(package, filename, roadModels[index]))
                return false;
            activeRoadModels[index] = &roadModels[index].reader;
        }
        MT7Model &model = *activeRoadModels[index]->data;
        d3t::RegisterObjectPlacement(GetMT7ReaderData(&model), GetMT7ReaderData(&model), 0);
        if (!model.nodes.empty()) {
            const std::uint32_t xb = model.nodes[0].xb01Offset;
            const std::uint32_t word = MazeMeshWord(model, xb) == SHEN_NAME(XB01) ? MazeMeshWord(model, xb + 24) + 10 : 13;
            const std::uint32_t flags = 0x41002000;
            std::memcpy(const_cast<std::byte *>(model.source.data()) + xb + word * 4, &flags, 4);
        }
    }
    return true;
}

bool InitializeMazeUnitModels(const LoadedMapPackage &package) {
    for (const RoadBlock &block : roadBlocks) {
        for (std::int32_t record = 0; record < block.controlCount; ++record) {
            const std::int32_t type = AuthoredWord(block, record, 0);
            if (type >= 0)
                continue;
            const std::uint8_t index = static_cast<std::uint8_t>(type);
            if (unitModels[index].reader.data)
                continue;
            char name[16];
            std::snprintf(name, sizeof(name), "%sG", authoredUnits[index].model);
            if (!LoadMazeModel(package, name, unitModels[index]))
                return false;
        }
    }
    if (!LoadMazeModel(package, "MAZSRL9G", terrainModel))
        return false;
    return true;
}

std::size_t MazeRoadModelIndex(const MazeRoad &road) {
    std::size_t kind = 0;
    switch (road.flags & 0x3c0000U) {
    case 0x40000:
        kind = 1;
        break;
    case 0xc0000:
        kind = 2;
        break;
    case 0x80000:
        kind = 3;
        break;
    case 0x100000:
        kind = 4;
        break;
    case 0x140000:
        kind = 5;
        break;
    }
    return kind * 4 + road.index % 4;
}

bool InitializeMazeRoadCommandCopies() {
    const MT7Model &model = *activeRoadModels[MazeRoadModelIndex(roadBlocks[0].roads[0])]->data;
    const std::uint32_t xb = model.nodes[0].xb01Offset;
    if (MazeMeshWord(model, xb) != SHEN_NAME(XB01))
        return true;
    const std::uint32_t table = MazeMeshWord(model, xb + 28) * 4 + 28;
    const std::uint32_t count = MazeMeshWord(model, xb + table + 4);
    std::uint32_t size = table + 8 * (count + 1);
    for (std::uint32_t index = 0; index < count; ++index)
        size += MazeMeshWord(model, xb + table + 12 + index * 12);
    roadCommandCopySize = size + 8;
    if (roadCommandCopySize > 0x200)
        return false;
    for (MazeRoadCommandCopy &copy : roadCommandCopies) {
        void *allocation = Allocate(0x200, SHEN_NAME(XMAZ));
        if (!allocation)
            return false;
        copy.bytes = {static_cast<std::byte *>(allocation), 0x200};
        std::memcpy(copy.bytes.data(), model.source.data() + xb, roadCommandCopySize);
        if (!PrepareLegacyDrawResources(copy.bytes, 0, copy.resources))
            return false;
    }
    return true;
}

void PrepareMazeRoadCommandCopy(const MazeRoad &road, std::size_t drawIndex) {
    const MT7Model &model = *activeRoadModels[MazeRoadModelIndex(road)]->data;
    const MT7Node &node = model.nodes[0];
    if (MazeMeshWord(model, node.xb01Offset) != SHEN_NAME(XB01))
        return;
    MazeRoadCommandCopy &copy = roadCommandCopies[drawIndex * 2 + (mazeFrameCount & 1)];
    d3t::RetireObjectPlacement(roadCommandCopies[drawIndex * 2].bytes.data());
    d3t::RegisterObjectPlacement(GetMT7ReaderData(&model), roadCommandCopies[drawIndex * 2].bytes.data(), 0);
    std::memcpy(copy.bytes.data(), model.source.data() + node.xb01Offset, roadCommandCopySize);
    RefreshLegacyDrawResources(copy.bytes, 0, node.borrowedMesh ? node.borrowedMesh->drawResources : node.drawResources, copy.resources);
}

void ApplyMazeRoadJoins(const MazeRoad &road, std::size_t drawIndex) {
    MT7Model &model = *activeRoadModels[MazeRoadModelIndex(road)]->data;
    const std::uint32_t xb = model.nodes[0].xb01Offset;
    if (MazeMeshWord(model, xb) == SHEN_NAME(XB01)) {
        MazeRoadCommandCopy &copy = roadCommandCopies[drawIndex * 2 + (mazeFrameCount & 1)];
        std::uint32_t count;
        std::memcpy(&count, copy.bytes.data() + 28, 4);
        const std::uint32_t offset = 4 * count;
        std::memcpy(copy.bytes.data() + offset + 140, &road.leftJoin, sizeof(Vector3));
        std::memcpy(copy.bytes.data() + offset + 108, &road.rightJoin, sizeof(Vector3));
        std::uint32_t size;
        std::memcpy(&size, copy.bytes.data() + offset + 40, 4);
        UploadLegacyVertexBuffer(copy.resources, 0, {copy.bytes.data() + offset + 44, size});
    } else {
        std::memcpy(const_cast<std::byte *>(model.source.data()) + xb + 208, &road.leftJoin, sizeof(Vector3));
        std::memcpy(const_cast<std::byte *>(model.source.data()) + xb + 176, &road.rightJoin, sizeof(Vector3));
    }
}

void DoMazeTransformationForObject(MazeRoad &road) {
    if (!CheckLineSegmentIntersection_Wrapper(road) || !ApplyTransformationForMazeObject(road, true))
        return;
    AdvanceMazeObjectAlpha(road);
    Matrix4x4_Push();
    if (!mazeUnitDrawEnabled) {
        Matrix4x4::Pop(1);
        return;
    }
    MT7Model &model = *unitModels[MUZT_Data[road.unit].modelIndex].reader.data;
    const RoadBlock &block = roadBlocks[road.block];
    const std::uint16_t record = static_cast<std::uint16_t>(road.flags);
    const std::int32_t yaw = 0x4000 - AuthoredWord(block, record, 24);
    model.nodes[0].rotation.y = static_cast<float>(yaw);
    Matrix4x4::preTranslate(road.position.x, road.position.y + 0.02f, road.position.z);
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::preTranslate(road.position.x, road.position.y + 0.02f, road.position.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(yaw));
    const bool hashed = road.alpha == 1.0f ? d3t::HashModel(GetMT7ReaderData(&model), GetLegacyPerspectiveProjection()) :
                                             d3t::HashModelWithAlpha(GetMT7ReaderData(&model), road.alpha, GetLegacyPerspectiveProjection());
    Matrix4x4::Pop(1);
    if (!hashed) {
        if (road.alpha == 1.0f)
            charaApplyScale(&model, model.nodes.data());
        else {
            SetLegacyBusyPreparingDraw(true);
            charaPrepareNodesForDraw(&model, model.nodes.data(), road.alpha);
            SetLegacyBusyPreparingDraw(false);
        }
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(yaw));
        const shen_name name = MazeUnitAuthoredName(MUZT_Data[road.unit].modelIndex);
        if (name.name == SHEN_NAME(SOT2) || name.name == SHEN_NAME(HIY4))
            DrawMazeAuxiliaryUnit(road, name);
        else
            ScanMazeAuthoredObjects(name, road);
    }
    Matrix4x4::Pop(1);
}

void ApplyMazePositionTranslationsFromArray() {
    struct Position {
        shen_name area;
        Vector3 point;
    };
    constexpr Position positions[] = {
        {.area = SHEN_NAME(KMZ1), .point = {-357.18f, 8.0f, -138.1f}},
        {.area = SHEN_NAME(KMZ3), .point = {-91.8f, 10.0f, -499.0f}},
        {.area = SHEN_NAME(KMZ3), .point = {-112.54f, 10.0f, -471.48f}},
        {.area = SHEN_NAME(KMZ3), .point = {-205.5f, 15.0f, -577.4f}},
        {.area = SHEN_NAME(KMZ3), .point = {-209.97f, 15.0f, -577.42f}},
        {.area = SHEN_NAME(KMZ3), .point = {-224.7f, 15.0f, -558.6f}},
        {.area = SHEN_NAME(KMZ3), .point = {-373.89f, 20.0f, -551.94f}},
        {.area = SHEN_NAME(KMZ3), .point = {-368.53f, 19.9f, -517.66f}},
        {.area = SHEN_NAME(KMZ3), .point = {-506.37f, 25.0f, -470.9f}},
        {.area = SHEN_NAME(KMZ3), .point = {-500.91f, 25.0f, -470.98f}},
        {.area = SHEN_NAME(KMZ4), .point = {-194.8f, 24.0f, -762.91f}},
        {.area = SHEN_NAME(KMZ4), .point = {-188.9f, 24.0f, -768.5f}},
    };
    const float far = camCtrlGetFarDistance();
    MT7Model &model = *auxiliaryModels[11].model;
    for (const Position &position : positions) {
        const float x = position.point.x - mazeCameraPosition.x;
        const float z = position.point.z - mazeCameraPosition.z;
        if (position.area.name != g_currentMap.name || far * far <= z * z + x * x || model.nodes.empty())
            continue;
        Matrix4x4_Push(&transformMatrix_prePass);
        Matrix4x4::preTranslate(position.point.x, position.point.y, position.point.z);
        if (!d3t::HashModel(GetMT7ReaderData(&model), GetLegacyPerspectiveProjection())) {
            Matrix4x4::Pop(1);
            Matrix4x4_Push();
            Matrix4x4::preTranslate(position.point.x, position.point.y, position.point.z);
            charaApplyScale(&model, model.nodes.data());
        }
        Matrix4x4::Pop(1);
    }
}

void SetTranslationToCurrent() {
    Vector3 camera{};
    camCtrlGetNextCamPos(&camera);
    Matrix4x4_Push();
    Matrix4x4::preTranslate(camera.x, camera.y, camera.z);
    if (mazeTerrainDrawEnabled && !d3t::HashModel(GetMT7ReaderData(terrainModel.model), GetLegacyPerspectiveProjection()))
        charaApplyScale(terrainModel.model, terrainModel.model->nodes.data());
    Matrix4x4::Pop(1);
}

void DrawMazeRoad(MazeRoad &road, std::size_t drawIndex) {
    if (!mazeRoadDrawEnabled)
        return;
    Matrix4x4_Push();
    Matrix4x4::preTranslate(road.position.x, road.position.y, road.position.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(road.yaw));
    ApplyMazeRoadJoins(road, drawIndex);
    for (const MazeRoadDecoration &decoration : mazeFixedDecorations)
        ApplyMazeRoadDecoration(road, decoration, *mediumModels[decoration.kind].model, true);
    if (!(road.flags & 0x20000000U)) {
        if (mazeRoadPropsEnabled) {
            AdvanceMazeObjectAlpha(road);
            GenerateMazePropStates(road, drawIndex);
            DrawMazePropStates(road, drawIndex);
            DrawMazeTreeEffects(road);
            DrawMazeRoadButterflies(road);
        }
        for (const MazeRoadDecoration &decoration : mazeMarkerDecorations)
            ApplyMazeRoadDecoration(road, decoration, *auxiliaryModels[decoration.kind].model, false);
    }
    MT7Model &model = *activeRoadModels[MazeRoadModelIndex(road)]->data;
    const MT7Node &root = model.nodes[0];
    if (MazeMeshWord(model, root.xb01Offset) == SHEN_NAME(XB01)) {
        MazeRoadCommandCopy &copy = roadCommandCopies[drawIndex * 2 + (mazeFrameCount & 1)];
        const LegacyNodeCommandOverride override{root.sourceOffset, copy.bytes, 0, &copy.resources};
        Matrix4x4_Push();
        Matrix4x4::SetPrePassToTempTransform();
        Matrix4x4::preTranslate(road.position.x, road.position.y, road.position.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(road.yaw));
        const bool hashed = d3t::FindHashedObject(roadCommandCopies[drawIndex * 2].bytes.data(), &road.leftJoin, GetLegacyPerspectiveProjection());
        Matrix4x4::Pop(1);
        if (!hashed)
            DrawMT7Model(model, nullptr, LegacyModelShaderPath::Lambert, 1.0f, &override);
    } else
        charaApplyScale(&model, model.nodes.data());
    Matrix4x4::Pop(1);
}


void MazeFrameNullsub() {
}

void RetireMazeModels() {
    for (MazeRoadCommandCopy &copy : roadCommandCopies) {
        if (!copy.bytes.empty())
            d3t::RetireObjectPlacement(copy.bytes.data());
        ReleaseLegacyDrawResources(copy.resources);
        copy.bytes = {};
    }
    if (terrainModel.model)
        d3t::RetireObjectPlacement(GetMT7ReaderData(terrainModel.model));
    terrainModel = {};
    for (std::span<MazeModelResource> resources :
        {std::span<MazeModelResource>{mediumModels}, std::span<MazeModelResource>{treeModels}, std::span<MazeModelResource>{auxiliaryModels},
            std::span<MazeModelResource>{roadModels}, std::span<MazeModelResource>{unitModels}, std::span<MazeModelResource>{authoredObjectModels}}) {
        for (MazeModelResource &resource : resources) {
            if (resource.model)
                d3t::RetireObjectPlacement(GetMT7ReaderData(resource.model));
            resource = {};
        }
    }
    propTables = {};
    propStates = {};
    mediumOccupancy = {};
    roadCommandCopySize = 0;
    activeRoadModels = {};
    for (MazeAuthoredObject &object : authoredObjects)
        object.modelReader = nullptr;
    currentMazeObjects = {};
    currentMazeUnitObjects = {};
}

bool LoadMazeModel(const LoadedMapPackage &package, const char *name, MazeModelResource &resource) {
    PakGlobalData *const owner = FindPakByEntryName(package.name.data());
    resource.model = LoadGameModel(owner, name);
    if (!resource.model)
        return false;
    resource.reader = {.source = resource.model->source.data(), .data = resource.model};
    const MT7Node *const data = GetMT7ReaderData(resource.model);
    d3t::RegisterObjectPlacement(data, data, 0);
    return true;
}

shen_name MazeUnitAuthoredName(std::int32_t modelIndex) {
    const char *const name = authoredUnits[modelIndex].model;
    return shen_name{make_shen_name(name[3], name[4], name[5], name[6])};
}

bool InitializeMazeAuthoredObjects(const LoadedMapPackage &package) {
    for (std::size_t index = 0; index < std::size(authoredObjects) - 1; ++index) {
        MazeAuthoredObject &object = authoredObjects[index];
        if (!HasMazeUnitOfKind(object.name))
            continue;
        // LoadGameModel returns the package's existing reader for duplicate sources.
        for (std::size_t previous = 0; previous < index; ++previous)
            if (authoredObjects[previous].modelReader && std::strcmp(authoredObjects[previous].resource, object.resource) == 0)
                object.modelReader = authoredObjects[previous].modelReader;
        if (!object.modelReader) {
            if (!LoadMazeModel(package, object.resource, authoredObjectModels[index]))
                return false;
            object.modelReader = &authoredObjectModels[index].reader;
        }
    }
    return true;
}

void ApplyMazeAuthoredUV(MazeAuthoredObject &object) {
    float amount = 0.0f;
    if (object.flags & 1U)
        amount = 0.039999999f;
    if (object.flags & 2U)
        amount = 0.02f;
    if (object.flags & 4U)
        amount = 0.0012000001f;
    if (object.flags & 8U)
        amount = 0.00050000002f;
    if (object.flags & 0x100000U)
        amount = -amount;
    const float u = object.flags & 0x80000U ? amount : 0.0f;
    const float v = object.flags & 0x40000U ? amount : 0.0f;
    SetTextureAnimationUV(u, v);
    if ((g_loadingFlag & 2U) == 0) {
        MT7Model &model = *object.modelReader->data;
        for (MT7Node &node : model.nodes)
            if (node.xb01Offset)
                UpdateLegacyTextureUVs(model.source, node.xb01Offset,
                    node.borrowedMesh ? const_cast<LegacyDrawResources &>(node.borrowedMesh->drawResources) : node.drawResources, u, v);
    }
}

void DrawMazeAuthoredAnimatedObject(MazeAuthoredObject &object) {
    Matrix4x4_Push();
    ApplyMazeAuthoredUV(object);
    const std::uint32_t phase = mazeCurrentReferenceCount >> 4;
    const float bob =
        (mazeBob[phase & 15] - mazeBob[(phase - 1) & 15]) * (static_cast<float>(mazeCurrentReferenceCount & 15) * 0.0625f) + mazeBob[(phase - 1) & 15];
    if (object.flags & 0x20U)
        Matrix4x4::preTranslate(0.0f, bob * 0.5f, 0.0f);
    if (object.flags & 0x10U)
        Matrix4x4::preTranslate(0.0f, bob * 0.1f, 0.0f);
    charaApplyScale(object.modelReader->data, object.modelReader->data->nodes.data());
    Matrix4x4::Pop(1);
}

void DrawMazeAuthoredHourObject(MazeAuthoredObject &object) {
    DateTime current{};
    GetCurrentDateTime(current);
    if (current.hour != mazeHourDate.hour && current.minute == mazeHourDate.minute) {
        mazeHourBlend = 0.99998999f;
        mazePreviousHourFrame = mazeHourFrame;
        if (++mazeHourFrame >= 5)
            mazeHourFrame = 0;
        GetCurrentDateTime(mazeHourDate);
    }
    ApplyMazeAuthoredUV(object);
    if ((g_loadingFlag & 2U) == 0)
        mazeHourBlend = std::fmin(0.99998999f, std::fmax(0.0000099999997f, mazeHourBlend - 0.0125f));
    MT7Model &model = *object.modelReader->data;
    std::int32_t selected = model.nodes[0].firstChild;
    std::int32_t previous = selected;
    for (std::int32_t index = 0; index < mazeHourFrame; ++index)
        selected = model.nodes[selected].nextSibling;
    for (std::int32_t index = 0; index < mazePreviousHourFrame; ++index)
        previous = model.nodes[previous].nextSibling;
    const MT7Node &node = model.nodes[selected];
    Matrix4x4_Push();
    Matrix4x4::preTranslate(node.position.x, node.position.y + (object.name.name == SHEN_NAME(HIY1) ? -0.2f : 0.0f), node.position.z);
    ExecuteLegacyDrawCommandStream(model.source, model.nodes[previous].xb01Offset,
        model.nodes[previous].borrowedMesh ? model.nodes[previous].borrowedMesh->drawResources : model.nodes[previous].drawResources,
        LegacyModelShaderPath::Lambert, mazeHourBlend);
    ExecuteLegacyDrawCommandStream(model.source, node.xb01Offset, node.borrowedMesh ? node.borrowedMesh->drawResources : node.drawResources,
        LegacyModelShaderPath::Lambert, 1.0f - mazeHourBlend);
    Matrix4x4::Pop(1);
}

void ScanMazeAuthoredObjects(shen_name name, const MazeRoad &context) {
    for (MazeAuthoredObject &object : authoredObjects) {
        if (static_cast<std::int32_t>(object.flags) < 0)
            break;
        if (object.name.name != name.name)
            continue;
        if (object.flags & 0x10000U)
            DrawMazeAuthoredAnimatedObject(object);
        if (object.flags & 0x20000U)
            DrawMazeAuthoredHourObject(object);
        if (object.flags & 0x8000U)
            FISH_Draw(object, context);
    }
}

void ResetMazeState(bool releaseAllocations) {
    if (releaseAllocations) {
        if (mazeTask)
            HLib::DestroyTask(mazeTask);
        if (fishTask)
            HLib::DestroyTask(fishTask);
    }
    RetireMazeModels();
    fishTask = nullptr;
    fishControlPoints = nullptr;
    fishPathSamples = nullptr;
    fishModel = nullptr;
    fishEnabled = false;
    markerBytes = {};
    markerCount = 0;
    unitAreas = {};
    unitCount = 0;
    partitionCount = 0;
    partitions = {};
    links = {};
    roadBlocks.clear();
    if (releaseAllocations) {
        for (std::size_t i = allocations.size(); i > 0; --i)
            HLib::FreeTaskData(allocations[i - 1]);
        if (ownsTerrainPackage)
            UnloadMapPackage(terrainPackage);
    }
    allocations.clear();
    MUZT_Data = nullptr;
    splineBuffer = nullptr;
    routeBuffer = nullptr;
    ownsTerrainPackage = false;
    terrainPackage = nullptr;
    mazeTask = nullptr;
    mazeCurrentReferenceCount = 0;
    mazeFrameCount = 0;
}

void UpdateMazeTask(void *taskData) {
    MazeTaskData *const data = static_cast<MazeTaskData *>(taskData);
    if (data)
        data->reserved = 0;
    if (!IsWorldDrawingEnabled())
        return;
    Matrix4x4::GetTransformMatrix(&mazeViewTransform);
    InitializeMazeFrameEnvironment();
    FreeNecessaryMazeObjects();
    ApplyMazeUnitChildWiggle();
    ApplyMazePositionTranslationsFromArray();
    if ((g_loadingFlag & 2U) == 0)
        ++mazeCurrentReferenceCount;
    MazeFrameNullsub();
    Vector3 camera{};
    camCtrlGetNextCamPos(&camera);
    float closest;
    InitializeMazeObjectTransforms(&camera, &closest);
    ApplyTransformationForCurrentMazeObjects();
    SetTranslationToCurrent();
    mediumOccupancy.fill(0);
    std::size_t drawIndex = 0;
    for (MazeRoad *road : currentMazeObjects) {
        if (!road)
            break;
        PrepareMazeRoadCommandCopy(*road, drawIndex);
        DrawMazeRoad(*road, drawIndex++);
    }
    for (MazeRoad *road : currentMazeUnitObjects) {
        if (!road)
            break;
        DoMazeTransformationForObject(*road);
    }
    SetSharedPrngSeed(GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Delta));
    ++mazeFrameCount;
}

// @verified - p
void MazeTaskCallback(void *taskData) {
    HLib::ExecuteCallbackNow(UpdateMazeTask, taskData);
}

void *Allocate(std::size_t bytes, shen_name name) {
    void *data = HLib::AllocateTaskData(bytes, name);
    if (data)
        allocations.push_back(data);
    return data;
}

template <class T> T ReadValue(std::span<const std::byte> bytes, std::size_t offset) {
    T result;
    std::memcpy(&result, bytes.data() + offset, sizeof(result));
    return result;
}

std::span<const std::byte> FindTerrainEntry(const LoadedMapPackage &package, const char *name) {
    std::span<const std::byte> bytes = package.pks.Find(name);
    if (bytes.empty())
        bytes = package.pkf.Find(name);
    return bytes;
}

std::int32_t AuthoredWord(const RoadBlock &block, std::int32_t record, std::size_t offset) {
    return ReadValue<std::int32_t>(block.authored, 56 * static_cast<std::size_t>(record) + offset);
}

Vector3 CurrentTranslation() {
    Matrix4x4 matrix;
    Matrix4x4::GetTransformMatrix(&matrix);
    return {matrix.rows[3].x, matrix.rows[3].y, matrix.rows[3].z};
}

Vector3 TranslatePoint(float x, float y, float z) {
    Matrix4x4_Push();
    Matrix4x4::preTranslate(x, y, z);
    const Vector3 result = CurrentTranslation();
    Matrix4x4::Pop();
    return result;
}

void SampleControlSpline(const RoadBlock &block, std::int32_t sample, Vector3 &output) {
    const float parameter = static_cast<float>(sample) * 0.015625f;
    std::int32_t index = static_cast<std::int32_t>(parameter);
    float t = parameter - static_cast<float>(index);
    if (parameter >= static_cast<float>(block.controlCount - 3)) {
        index = block.controlCount - 4;
        t = 1.0f;
    }
    const Vector3 *points = block.controlPoints + index;
    const Matrix4x4 coordinates{{
        {points[0].x, points[1].x, points[2].x, points[3].x},
        {points[0].y, points[1].y, points[2].y, points[3].y},
        {points[0].z, points[1].z, points[2].z, points[3].z},
        {1.0f, 1.0f, 1.0f, 1.0f},
    }};
    constexpr Matrix4x4 basis{{
        {-0.1666666716337204f, 0.5f, -0.5f, 0.1666666716337204f},
        {0.5f, -1.0f, 0.0f, 0.6666666865348816f},
        {-0.5f, 0.5f, 0.5f, 0.1666666716337204f},
        {0.1666666716337204f, 0.0f, 0.0f, 0.0f},
    }};
    const float square = t * t;
    const float cube = square * t;
    const Matrix4x4 powers{{
        {cube * 6.0f, cube * 6.0f, cube, 0.0f},
        {0.0f, square + square, square, 0.0f},
        {0.0f, 0.0f, t, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
    }};
    Matrix4x4_Push(&powers);
    Matrix4x4::MultiplyWithPrePassMatrix(&basis);
    Matrix4x4::MultiplyWithPrePassMatrix(&coordinates);
    Matrix4x4 result;
    Matrix4x4::GetTransformMatrix(&result);
    Matrix4x4::Pop(0);
    output = {result.rows[0].z + result.rows[0].w, result.rows[1].z + result.rows[1].w, result.rows[2].z + result.rows[2].w};
}

float SampleSegment(const RoadBlock &block, std::int32_t segment) {
    for (std::int32_t i = 0; i <= 64; ++i)
        SampleControlSpline(block, segment * 64 + i, splineBuffer[i]);
    float length = 0.0f;
    for (std::int32_t i = 1; i <= 64; ++i) {
        const float x = splineBuffer[i - 1].x - splineBuffer[i].x;
        const float y = splineBuffer[i - 1].y - splineBuffer[i].y;
        const float z = splineBuffer[i - 1].z - splineBuffer[i].z;
        length += std::sqrt((x * x + y * y) + z * z);
    }
    return length;
}

void SetRoadFlags(MazeRoad &road, const RoadBlock &block, std::int32_t record, std::int32_t sample) {
    const std::int32_t type = AuthoredWord(block, record, 0);
    const std::int32_t next = AuthoredWord(block, record + 1, 0);
    const std::int32_t previous = AuthoredWord(block, record - 1, 0);
    std::int32_t metadata = AuthoredWord(block, record, 28);
    road.flags = 0;
    if (type) {
        if (!metadata) {
            const std::int32_t previousMetadata = AuthoredWord(block, record - 1, 28);
            const std::int32_t nextMetadata = AuthoredWord(block, record + 1, 28);
            if (nextMetadata)
                metadata = nextMetadata;
            if (previousMetadata)
                metadata = previousMetadata;
        }

        road.flags = static_cast<std::uint32_t>(record);
        if (!next)
            road.flags = sample == 0 ? 0x24000000U : 0;
        if (!previous && sample == 0)
            road.flags = 0x28000000U;
    }
    if (sample == 0)
        road.flags |= type < 0 ? 0xc0000000U : 0x40000000U;
    road.flags |= static_cast<std::uint32_t>(metadata) << 16;
}

void BuildRoadJoins(RoadBlock &block) {
    for (std::int32_t i = 0; i < block.roadCount; ++i) {
        MazeRoad &road = block.roads[i];
        if (i + 1 == block.roadCount) {
            road.leftJoin = {-5.0f, 0.0f, -1.0f};
            road.rightJoin = {5.0f, 0.0f, -1.0f};
        } else {
            const MazeRoad &next = block.roads[i + 1];
            Matrix4x4_Push();
            Matrix4x4::SetPrePassToTempTransform();
            Matrix4x4::preTranslate(next.position.x, next.position.y, next.position.z);
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(next.yaw));
            const Vector3 left = TranslatePoint(-5.0f, 0.0f, 1.0f);
            const Vector3 right = TranslatePoint(5.0f, 0.0f, 1.0f);
            Matrix4x4::Pop();

            Matrix4x4_Push();
            Matrix4x4::SetPrePassToTempTransform();
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-road.yaw));
            Matrix4x4::preTranslate(-road.position.x, -road.position.y, -road.position.z);
            road.leftJoin = TranslatePoint(left.x, left.y, left.z);
            road.rightJoin = TranslatePoint(right.x, right.y, right.z);
            Matrix4x4::Pop();

            if (road.leftJoin.z > 1.0f)
                road.leftJoin.z = 1.0f;
            if (road.rightJoin.z > 1.0f)
                road.rightJoin.z = 1.0f;
            road.leftJoin.y = next.position.y - road.position.y;
            road.rightJoin.y = road.leftJoin.y;
        }
        road.leftJoin.x = std::bit_cast<float>(std::bit_cast<std::uint32_t>(road.leftJoin.x) | 1U);
        road.rightJoin.x = std::bit_cast<float>(std::bit_cast<std::uint32_t>(road.rightJoin.x) | 1U);
    }
    Matrix4x4_Push();
    for (std::int32_t i = 0; i < block.roadCount; ++i) {
        MazeRoad &road = block.roads[i];
        const float leftX = std::bit_cast<float>(std::bit_cast<std::uint32_t>(road.leftJoin.x) & ~1U);
        const float rightX = std::bit_cast<float>(std::bit_cast<std::uint32_t>(road.rightJoin.x) & ~1U);
        const float centreX = (leftX + rightX) * 0.5f;
        const float centreZ = (road.leftJoin.z + road.rightJoin.z) * 0.5f;
        Matrix4x4::SetPrePassToTempTransform();
        Matrix4x4::preTranslate(road.position.x, road.position.y, road.position.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(road.yaw));
        road.leftStart = TranslatePoint(-7.5f, 0.0f, 1.0f);
        road.leftEnd = TranslatePoint((leftX - centreX) * 1.5f + centreX, 0.0f, (road.leftJoin.z - centreZ) * 1.5f + centreZ);
        road.rightStart = TranslatePoint(7.5f, 0.0f, 1.0f);
        road.rightEnd = TranslatePoint((rightX - centreX) * 1.5f + centreX, 0.0f, (road.rightJoin.z - centreZ) * 1.5f + centreZ);
        road.leftCentre.x = (road.leftEnd.x + road.leftStart.x) * 0.5f;
        road.leftCentre.z = (road.leftEnd.z + road.leftStart.z) * 0.5f;
        road.rightCentre.x = (road.rightEnd.x + road.rightStart.x) * 0.5f;
        road.rightCentre.z = (road.rightEnd.z + road.rightStart.z) * 0.5f;
    }
    Matrix4x4::Pop();
}

bool BuildRoads(RoadBlock &block, std::int32_t blockIndex) {
    float length = 0.0f;
    for (std::int32_t i = 0; i < block.controlCount - 3; ++i)
        length += SampleSegment(block, i);
    const std::int32_t capacity = block.controlCount - static_cast<std::int32_t>(length * -0.5f) + 4;
    block.roads = static_cast<MazeRoad *>(Allocate(sizeof(MazeRoad) * capacity, SHEN_NAME(MAZE)));
    if (!block.roads)
        return false;
    for (std::int32_t segment = 0; segment < block.controlCount - 3; ++segment) {
        const float halfLength = SampleSegment(block, segment) * 0.5f;
        std::int32_t samples = static_cast<std::int32_t>(halfLength);
        if (halfLength - static_cast<float>(samples) > 0.5f)
            ++samples;
        if (samples < 1)
            samples = 1;
        if (samples >= capacity - block.roadCount)
            return false;
        for (std::int32_t i = 0; i < samples; ++i) {
            MazeRoad &road = block.roads[block.roadCount];
            road = {};
            const std::int32_t sample = (i * 64) / samples;
            const Vector3 &first = splineBuffer[sample];
            const Vector3 &second = splineBuffer[sample + 1];
            road.position = first;
            road.yaw = static_cast<std::int16_t>(FixedAtan2(first.x - second.x, first.z - second.z));
            road.block = blockIndex;
            road.index = block.roadCount++;
            SetRoadFlags(road, block, segment + 1, i);
        }
    }
    block.roads[block.roadCount] = {};
    block.roads[block.roadCount].index = -1;
    BuildRoadJoins(block);
    return true;
}

Vector3 **AllocateMazeUnitPaths(std::span<const AuthoredPath> paths) {
    if (paths.empty())
        return nullptr;
    std::size_t totalPoints = 0;
    for (const AuthoredPath &path : paths)
        totalPoints += path.points.size();
    Vector3 **const lists = static_cast<Vector3 **>(Allocate(paths.size() * sizeof(Vector3 *), MEMORY_MAZE_ROUTES));
    Vector3 *cursor = static_cast<Vector3 *>(Allocate((totalPoints + 3) * sizeof(Vector3), MEMORY_MAZE_ROUTES));
    if (!lists || !cursor)
        return nullptr;
    for (std::size_t index = 0; index < paths.size(); ++index) {
        lists[index] = cursor;
        for (const Vector2 &point : paths[index].points)
            *cursor++ = TranslatePoint(point.x, 0.0f, point.y);
        *cursor++ = {0.0f, 1000.0f, 0.0f};
    }
    return lists;
}

bool BuildUnits(RoadBlock &block) {
    for (std::int32_t i = 0; i < block.roadCount; ++i) {
        MazeRoad &road = block.roads[i];
        if (!(road.flags & 0x80000000U))
            continue;

        if (unitCount == static_cast<std::int32_t>(MAZE_UNIT_CAPACITY)) {
            return false;
        }

        const std::uint16_t record = static_cast<std::uint16_t>(road.flags);
        if (record >= block.controlCount) {
            return false;
        }

        const std::uint8_t model = static_cast<std::uint8_t>(AuthoredWord(block, record, 0));
        if (model >= std::size(authoredUnits)) {
            return false;
        }

        const AuthoredUnit &authored = authoredUnits[model];
        const std::size_t area = static_cast<std::size_t>(authored.areaIndex) * 112;
        if (area + 112 > unitAreas.size()) {
            return false;
        }

        MazeUnit &unit = MUZT_Data[unitCount];
        unit.road = &road;
        unit.modelIndex = model;
        unit.pointCount = ReadValue<std::int16_t>(unitAreas, area + 32);
        unit.wallCount = ReadValue<std::int16_t>(unitAreas, area + 60);
        if (unit.pointCount < 0 || unit.pointCount > 3 || unit.wallCount < 0 || unit.wallCount > 3) {
            return false;
        }
        unit.numLists = static_cast<std::int32_t>(authored.paths.size());
        Matrix4x4_Push();
        Matrix4x4::SetPrePassToTempTransform();
        Matrix4x4::preTranslate(road.position.x, road.position.y, road.position.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(0x4000 - AuthoredWord(block, record, 24)));
        unit.pointLists = AllocateMazeUnitPaths(authored.paths);
        if (unit.numLists && !unit.pointLists) {
            Matrix4x4::Pop(1);
            return false;
        }
        for (std::size_t j = 0; j < 4; ++j) {
            const Vector2 point = ReadValue<Vector2>(unitAreas, area + j * 8);
            unit.quad[j] = TranslatePoint(point.x, 0.0f, point.y);
        }
        for (std::int32_t j = 0; j < unit.wallCount; ++j) {
            const Vector4 wall = ReadValue<Vector4>(unitAreas, area + 64 + j * 16);
            unit.walls[j][0] = TranslatePoint(wall.x, 0.0f, wall.y);
            unit.walls[j][1] = TranslatePoint(wall.z, 0.0f, wall.w);
        }
        for (std::int32_t j = 0; j < unit.pointCount; ++j) {
            const Vector2 point = ReadValue<Vector2>(unitAreas, area + 36 + j * 8);
            unit.points[j] = TranslatePoint(point.x, 0.0f, point.y);
        }
        Matrix4x4::Pop();
        road.unit = unitCount++;
    }
    return true;
}

bool InsideQuad(const MazeUnit &unit, const Vector3 &point) {
    const Vector3 *quad = unit.quad;
    const float first = (quad[1].x - quad[0].x) * (quad[0].z - point.z) - (quad[0].x - point.x) * (quad[1].z - quad[0].z);
    for (std::size_t i = 1; i < 4; ++i) {
        const Vector3 &a = quad[i];
        const Vector3 &b = quad[(i + 1) % 4];
        const float side = (b.x - a.x) * (a.z - point.z) - (b.z - a.z) * (a.x - point.x);
        if (!(side * first >= 0.0f))
            return false;
    }
    return true;
}

std::int32_t FindUnit(const Vector3 &point) {
    for (std::int32_t i = 0; i < unitCount; ++i)
        if (InsideQuad(MUZT_Data[i], point))
            return i;
    return -1;
}

std::int32_t PathCount(const Vector3 *points) {
    std::int32_t count = 0;
    while (points[count].y < 990.0f)
        ++count;
    return count == 0 ? -1 : count;
}

void FindNearestUnitPoint(const Vector3 &point, const MazeUnit &unit, std::int32_t &list, std::int32_t &element) {
    float minimum = 1000000000.0f;
    list = -1;
    element = -1;
    for (std::int32_t i = 0; i < unit.numLists; ++i) {
        const Vector3 *points = unit.pointLists[i];
        const std::int32_t count = PathCount(points);
        for (std::int32_t j = 0; j < count; ++j) {
            const float x = point.x - points[j].x;
            const float y = point.y - points[j].y;
            const float z = point.z - points[j].z;
            const float distance = (y * y + x * x) + z * z;
            if (minimum > distance) {
                minimum = distance;
                list = i;
                element = j;
            }
        }
    }
}

MazeRoad *SeekOutsideUnit(MazeRoad *road) {
    while (road->index >= 0 && FindUnit(road->position) >= 0)
        ++road;
    return road;
}

MazeRoad *SeekInsideUnit(MazeRoad *road) {
    while (road->index >= 0 && FindUnit(road->position) < 0)
        ++road;
    return road;
}

bool AppendPoint(const Vector3 &point, std::int32_t &count) {
    if (count >= 0x3000 / static_cast<std::int32_t>(sizeof(Vector3)) - 1)
        return false;
    routeBuffer[count++] = point;
    return true;
}

bool AppendOutsideRoads(MazeRoad *road, std::int32_t &count) {
    while (road->index >= 0 && FindUnit(road->position) < 0) {
        if (!AppendPoint(road->position, count))
            return false;
        ++road;
    }
    return true;
}

bool AppendUnitPath(const MazeUnit &unit, std::int32_t list, std::int32_t direction, std::int32_t &count) {
    if (unit.numLists <= 0)
        return true;
    if (list < 0 || list >= unit.numLists)
        return false;
    const Vector3 *points = unit.pointLists[list];
    const std::int32_t pointCount = PathCount(points);
    const std::int32_t step = direction > 0 ? 1 : -1;
    for (std::int32_t i = direction > 0 ? 0 : pointCount - 1; i >= 0 && i < pointCount; i += step) {
        if (FindUnit(points[i]) >= 0 && !AppendPoint(points[i], count))
            return false;
    }
    return true;
}

bool CollectPartition(MazeRoad *&road, std::int32_t &count) {
    count = 0;
    const std::int32_t initialUnit = FindUnit(road->position);
    if (initialUnit >= 0) {
        std::int32_t list;
        std::int32_t element;
        FindNearestUnitPoint(SeekOutsideUnit(road)->position, MUZT_Data[initialUnit], list, element);
        if (!AppendUnitPath(MUZT_Data[initialUnit], list, 1, count))
            return false;
    }
    road = SeekOutsideUnit(road);
    if (!AppendOutsideRoads(road, count))
        return false;
    road = SeekInsideUnit(road);
    while (road->index >= 0) {
        const std::int32_t unitIndex = FindUnit(road->position);
        if (unitIndex >= 0) {
            const MazeUnit &unit = MUZT_Data[unitIndex];
            std::int32_t list;
            std::int32_t element;
            FindNearestUnitPoint(road->position, unit, list, element);
            if (unit.numLists > 1) {
                if (!AppendUnitPath(unit, list, -1, count))
                    return false;
                MazeRoad *outside = SeekOutsideUnit(road);
                if (outside->index == -1)
                    road = outside;
                routeBuffer[count] = {0.0f, -1000.0f, 0.0f};
                return true;
            }
            std::int32_t direction = 0;
            if (unit.numLists > 0)
                direction = element > PathCount(unit.pointLists[list]) / 2 ? -1 : 1;
            if (!AppendUnitPath(unit, 0, direction, count))
                return false;
            road = SeekOutsideUnit(road);
        }
        if (!AppendOutsideRoads(road, count))
            return false;
        road = SeekInsideUnit(road);
    }
    routeBuffer[count] = {0.0f, -1000.0f, 0.0f};
    return true;
}

bool BuildPartitions() {
    for (RoadBlock &block : roadBlocks) {
        MazeRoad *road = block.roads;
        do {
            if (partitionCount == static_cast<std::int32_t>(partitions.size()))
                return false;
            std::int32_t count;
            if (!CollectPartition(road, count))
                return false;
            MazePartition &partition = partitions[partitionCount++];
            partition.count = count;
            partition.points = static_cast<Vector3 *>(Allocate(192 * static_cast<std::size_t>(count + 1), SHEN_NAME(MZNR)));
            if (!partition.points)
                return false;
            for (std::int32_t i = 0; i < count; ++i)
                partition.points[i] = routeBuffer[i];
        } while (road->index != -1);
    }
    return true;
}

void BuildEndpointLinks(std::int32_t partition, std::int32_t endpoint) {
    const std::int32_t element = endpoint == 0 ? 0 : partitions[partition].count - 1;
    if (element < 0)
        return;
    const std::int32_t unitIndex = FindUnit(partitions[partition].points[element]);
    if (unitIndex < 0 || MUZT_Data[unitIndex].numLists != 3)
        return;
    const MazeUnit &unit = MUZT_Data[unitIndex];
    std::int32_t adjacentPartition[3];
    std::int32_t adjacentElement[3]{};
    std::int32_t ownList = -1;
    for (std::int32_t i = 0; i < 3; ++i) {
        adjacentPartition[i] = -1;
        mazeFindClosestTarget(&adjacentPartition[i], &adjacentElement[i], unit.pointLists[i]);
        if (adjacentPartition[i] == partition && adjacentElement[i] == element)
            ownList = i;
    }
    if (ownList < 0)
        return;
    EndpointLinks &result = links[partition][endpoint];
    for (std::int32_t i = 0; i < 2; ++i) {
        const std::int32_t next = (ownList + i + 1) % 3;
        result.neighbors[i].partition = static_cast<std::int16_t>(adjacentPartition[next]);
        result.neighbors[i].element = static_cast<std::int16_t>(adjacentElement[next]);
    }
}

struct RouteSearch {
    std::int32_t partition;
    std::int32_t element;
    std::int32_t targetPartition;
    std::int32_t targetElement;
    std::int32_t nextPartition;
    std::int32_t nextElement;
};

std::int32_t SearchRoute(RouteSearch &search, std::int32_t endpoint) {
    if (search.partition < 0 || search.partition >= partitionCount || links[search.partition][0].visited)
        return 0;
    links[search.partition][0].visited = true;
    if (search.partition == search.targetPartition) {
        search.nextPartition = search.targetPartition;
        search.nextElement = search.targetElement;
        const std::int32_t difference = search.targetElement - search.element;
        return (difference < 0 ? -difference : difference) | 0x10000;
    }
    const std::int32_t partition = search.partition;
    const std::int32_t element = search.element;
    const EndpointLinks &neighbors = links[partition][endpoint];
    for (std::int32_t i = 1; i >= 0; --i) {
        search.partition = neighbors.neighbors[i].partition;
        search.element = neighbors.neighbors[i].element;
        const std::int32_t result = SearchRoute(search, search.element == 0);
        search.nextPartition = search.partition;
        search.nextElement = search.element;
        search.partition = partition;
        search.element = element;
        if (result)
            return (endpoint ? partitions[partition].count - element : element) + result + 1;
    }
    return 0;
}

void MarkRoadEnds(shen_name area) {
    for (RoadBlock &block : roadBlocks) {
        for (std::int32_t i = 1; i < block.roadCount; ++i) {
            if (!(block.roads[i].flags & 0xffffU))
                continue;
            for (std::int32_t j = i; j < block.roadCount && j < i + 5; ++j)
                block.roads[j].flags |= 0x10000000U;
        }

        for (std::int32_t i = block.roadCount - 1; i > 0 && i >= block.roadCount - 5; --i)
            block.roads[i].flags |= 0x10000000U;

        for (std::int32_t i = block.roadCount - 2; i > 0; --i) {
            if (!(block.roads[i].flags & 0xffffU))
                continue;
            for (std::int32_t j = i; j > 0 && j > i - 5; --j)
                block.roads[j].flags |= 0x10000000U;
        }
    }

    if (area.name != SHEN_NAME(KMZ3) || partitionCount <= 13)
        return;
    for (std::int32_t i = 0; i < partitions[13].count; ++i) {
        const Vector3 &point = partitions[13].points[i];
        float minimum = 25600.0f;
        MazeRoad *nearest = nullptr;
        for (RoadBlock &block : roadBlocks) {
            for (std::int32_t j = 0; j < block.roadCount; ++j) {
                const float x = point.x - block.roads[j].position.x;
                const float z = point.z - block.roads[j].position.z;
                const float distance = z * z + x * x;
                if (minimum > distance) {
                    minimum = distance;
                    nearest = &block.roads[j];
                }
            }
        }
        if (nearest)
            nearest->flags |= 0x10000000U;
    }
}

bool InitializeMazeAssets(const LoadedMapPackage &package, std::int32_t mode) {
    constexpr const char *mapNames[] = {"qte", "kmz1", "kmz2", "kmz3", "kmz4"};
    char filename[16];
    std::snprintf(filename, sizeof(filename), "%smap.BIN", mapNames[mode]);
    const std::span<const std::byte> map = FindTerrainEntry(package, filename);
    unitAreas = FindTerrainEntry(package, "unitarea.BIN");
    if (map.size() < 4 || unitAreas.empty()) {
        return false;
    }
    if (mode) {
        std::snprintf(filename, sizeof(filename), "%sev.BIN", mapNames[mode]);
        markerBytes = FindTerrainEntry(package, filename);
        if (markerBytes.size() < sizeof(Marker)) {
            return false;
        }
        std::size_t offset = 0;
        while (offset + sizeof(Marker) <= markerBytes.size() && ReadValue<std::int32_t>(markerBytes, offset) >= 0) {
            ++markerCount;
            offset += sizeof(Marker);
        }
        if (offset + sizeof(Marker) > markerBytes.size()) {
            return false;
        }
    }
    MUZT_Data = static_cast<MazeUnit *>(Allocate(sizeof(MazeUnit) * MAZE_UNIT_CAPACITY, MEMORY_MAZE_UNITS));
    splineBuffer = static_cast<Vector3 *>(Allocate(0x30c, SHEN_NAME(MZBF)));
    if (!MUZT_Data || !splineBuffer) {
        return false;
    }
    const std::uint16_t blockCount = ReadValue<std::uint16_t>(map, 0);
    if (blockCount == 0)
        return false;
    roadBlocks.resize(blockCount);
    std::size_t offset = 4;
    for (RoadBlock &block : roadBlocks) {
        if (offset + 4 > map.size())
            return false;
        const std::uint32_t count = ReadValue<std::uint32_t>(map, offset);
        if (count < 4 || count > 0xffffU || count > (map.size() - offset - 4) / 56)
            return false;
        block.controlCount = static_cast<std::int32_t>(count);
        block.authored = map.subspan(offset + 4, count * 56);
        offset += 4 + count * 56;
    }
    return true;
}

bool InitializeMazeGeometry() {
    for (RoadBlock &block : roadBlocks) {
        const std::size_t count = static_cast<std::size_t>(block.controlCount);
        block.controlPoints = static_cast<Vector3 *>(Allocate(sizeof(Vector3) * count, SHEN_NAME(MZPT)));
        if (!block.controlPoints)
            return false;
        for (std::size_t i = 0; i < count; ++i) {
            block.controlPoints[i] = ReadValue<Vector3>(block.authored, i * 56 + 12);
            block.controlPoints[i].y += 0.005f;
        }
    }
    for (std::size_t i = 0; i < roadBlocks.size(); ++i) {
        if (!BuildRoads(roadBlocks[i], static_cast<std::int32_t>(i)) || !BuildUnits(roadBlocks[i])) {
            return false;
        }
    }
    return true;
}

bool InitializeMazeRoutes(shen_name area) {
    routeBuffer = static_cast<Vector3 *>(Allocate(0x3000, SHEN_NAME(MZRT)));
    if (!routeBuffer || !BuildPartitions()) {
        return false;
    }
    for (std::int32_t i = 0; i < partitionCount; ++i) {
        BuildEndpointLinks(i, 0);
        BuildEndpointLinks(i, 1);
    }
    MarkRoadEnds(area);
    return true;
}

} // namespace

// @verified - p
void SetMazeDefaults() {
    mazeCullingSteps = 13;
    mazeTerrainDrawEnabled = 1;
    mazeUnitDrawEnabled = 1;
    mazeRoadPropsEnabled = 1;
    mazeRoadDrawEnabled = 1;
    SetNaviTaskEnabled(true);
    mazeUnitDistance = 60.0f;
}

bool InitializeMazeRuntime(std::int32_t act, shen_name area) {
    if (act != 4)
        return false;
    std::int32_t mode;
    switch (area.name) {
    case SHEN_NAME(KMZ1):
        mode = 1;
        break;
    case SHEN_NAME(KMZ2):
        mode = 2;
        break;
    case SHEN_NAME(KMZ3):
        mode = 3;
        break;
    case SHEN_NAME(KMZ4):
        mode = 4;
        break;
    case SHEN_NAME(KWQA):
        mode = 0;
        break;
    default:
        return false;
    }

    // Parent module teardown calls RetireMazeRuntime before freeing use-spaces and package buffers.
    ResetMazeState(false);
    g_mazePropDrawCount = 0;
    constexpr const char *packageNames[] = {"qte", "kmz1", "kmz2", "kmz3", "kmz4"};
    terrainPackage = packageNames[mode];
    const LoadedMapPackage *package = nullptr;
    for (const LoadedMapPackage &loaded : GetLoadedMapPackages()) {
        if (std::strncmp(loaded.name.data(), terrainPackage, loaded.name.size()) == 0) {
            package = &loaded;
            break;
        }
    }
    if (!package) {
        if (!LoadMapPackage("scene/04/maze", terrainPackage)) {
            ResetMazeState(true);
            return false;
        }
        ownsTerrainPackage = true;
        LoadPak();
        for (const LoadedMapPackage &loaded : GetLoadedMapPackages()) {
            if (std::strncmp(loaded.name.data(), terrainPackage, loaded.name.size()) == 0) {
                package = &loaded;
                break;
            }
        }
    }
    SetMazeDefaults();
    if (!package || !InitializeMazeAssets(*package, mode) || !InitMazeModels(*package) || !InitializeMazeEnvironmentModels(*package) ||
        !InitializeMazeUnitModels(*package) || !InitializeMazeGeometry()) {
        ResetMazeState(true);
        return false;
    }

    mazeTask = HLib::EnqueueTaskWithParameter(MazeTaskCallback, 13, 2, sizeof(MazeTaskData), TASK_MAZE);
    MazeTaskData *const data = mazeTask ? static_cast<MazeTaskData *>(HLib::GetTaskParameter(mazeTask)) : nullptr;
    if (!data) {
        if (mazeTask) {
            HLib::DestroyTask(mazeTask);
            mazeTask = nullptr;
        }
        ResetMazeState(true);
        return false;
    }
    data->reserved = 0;
    data->task = mazeTask;
    if (!InitializeMazeRoutes(area) || !InitializeMazeAuxiliaryModels(*package)) {
        ResetMazeState(true);
        return false;
    }
    MAZ_fishCtrl();
    BindMazeMarkerDecorations();
    mazeHourBlend = 0.0000099999997f;
    mazeHourFrame = 2;
    mazePreviousHourFrame = 2;
    GetCurrentDateTime(mazeHourDate);
    if (!InitializeMazeRoadCommandCopies()) {
        ResetMazeState(true);
        return false;
    }

    return true;
}

void RetireMazeRuntime() {
    ResetMazeState(true);
}

std::span<MazeRoad *const> GetCurrentMazeObjects() {
    std::size_t count = 0;
    while (count < currentMazeObjects.size() && currentMazeObjects[count])
        ++count;
    return {currentMazeObjects.data(), count};
}

std::span<MazeRoad *const> GetCurrentMazeUnitObjects() {
    std::size_t count = 0;
    while (count < currentMazeUnitObjects.size() && currentMazeUnitObjects[count])
        ++count;
    return {currentMazeUnitObjects.data(), count};
}

std::int32_t InitializeMazeObjectTransforms(const Vector3 *camera, float *nearestDistance) {
    currentMazeObjects = {};
    currentMazeUnitObjects = {};
    std::int32_t nearest = -1;
    float distance = 1000000.0f;
    for (std::size_t block = 0; block < roadBlocks.size(); ++block) {
        float blockDistance;
        const std::int32_t index = UpdateXFormMazeObject(roadBlocks[block].roads, *camera, blockDistance);
        if (index != -1 && distance > blockDistance) {
            distance = blockDistance;
            nearest = static_cast<std::int32_t>((block << 16) + index);
        }
    }
    *nearestDistance = distance;
    return nearest;
}

std::span<const MazeUnit> GetMazeUnits() {
    return {MUZT_Data, static_cast<std::size_t>(unitCount)};
}
std::span<const MazeRoad> GetMazeRoads(std::int32_t block) {
    if (block < 0 || static_cast<std::size_t>(block) >= roadBlocks.size())
        return {};
    return {roadBlocks[block].roads, static_cast<std::size_t>(roadBlocks[block].roadCount)};
}

// @verified - p
MazeRoad *GetMazeRoad(std::int32_t block, std::int32_t index) {
    return roadBlocks[block].roads + index;
}

// @verified - p
std::uint32_t GetMazeRoadPosition(std::int32_t block, std::int32_t index, Vector3 *output) {
    *output = roadBlocks[block].roads[index].position;
    return std::bit_cast<std::uint32_t>(output->z);
}

// @verified - p
std::uint32_t GetMazeRoadYaw(std::int32_t block, std::int32_t index) {
    return static_cast<std::uint32_t>(roadBlocks[block].roads[index].yaw);
}

// @verified - p
std::uint32_t GetMazeUnitCount() {
    return static_cast<std::uint32_t>(unitCount);
}

// @verified - p
std::uint32_t GetMazeMarkerCount() {
    return static_cast<std::uint32_t>(markerCount);
}

// @verified - p
void FindNearestMazeRoad(const Vector3 *position, std::int32_t *block, std::int32_t *index) {
    *block = -1;
    *index = -1;
    float nearest = 25600.0f;
    for (std::size_t b = 0; b < roadBlocks.size(); ++b) {
        const RoadBlock &roads = roadBlocks[b];
        for (std::int32_t i = 0; i < roads.roadCount; ++i) {
            const Vector3 &sample = roads.roads[i].position;
            const float x = position->x - sample.x, z = position->z - sample.z;
            const float distance = z * z + x * x;
            if (nearest > distance) {
                nearest = distance;
                *block = static_cast<std::int32_t>(b);
                *index = i;
            }
        }
    }
}

// @verified - p
std::uint32_t FindNearestMazeFirstBlockRoad(const Vector3 *position) {
    std::int32_t block, index;
    FindNearestMazeRoad(position, &block, &index);
    return block == 0 ? static_cast<std::uint32_t>(index) : 0;
}

// @verified - p
std::uint32_t FindCharacterNearestMazeFirstBlockRoad(Character *character) {
    const Vector3 position = GetCharacterWorldPosition(character);
    return FindNearestMazeFirstBlockRoad(&position);
}

// @verified - p
shen_name GetMazeUnitAuthoredName(std::int32_t index) {
    const MazeUnit *const unit = index < 0 || index >= unitCount ? nullptr : MUZT_Data + index;
    return MazeUnitAuthoredName(unit->modelIndex);
}

// @verified - p
bool HasMazeUnitOfKind(shen_name name) {
    for (std::int32_t index = 0; index < unitCount; ++index)
        if (MazeUnitAuthoredName(MUZT_Data[index].modelIndex).name == name.name)
            return true;
    return false;
}

// @verified - p
void SetMazeUnitDrawDistance(float distance) {
    mazeUnitDistance = distance;
}
void SetMazeTerrainDrawEnabled(std::int32_t enabled) {
    mazeTerrainDrawEnabled = enabled;
}
std::int32_t mazeGetNumPartitions() {
    return partitionCount;
}
std::int32_t GetMZNRDataForID(std::int32_t partition) {
    return partitions[partition].count;
}

void mazeFindClosestTarget(std::int32_t *partition, std::int32_t *element, const Vector3 *point) {
    const std::int32_t previousPartition = *partition;
    const std::int32_t previousElement = *element;
    float globalDistance = 1000000.0f;
    float localDistance = 1000000.0f;
    std::int32_t localElement = -1;
    for (std::int32_t i = 0; i < partitionCount; ++i) {
        for (std::int32_t j = 0; j < partitions[i].count; ++j) {
            const Vector3 &sample = partitions[i].points[j];
            const float x = sample.x - point->x;
            const float z = sample.z - point->z;
            const float distance = z * z + x * x;
            if (i == previousPartition && localDistance > distance) {
                localDistance = distance;
                localElement = j;
            }
            if (globalDistance > distance) {
                globalDistance = distance;
                *partition = i;
                *element = j;
            }
        }
    }
    if (previousPartition >= 0 && previousPartition != *partition && previousPartition < partitionCount && previousElement > 4 &&
        previousElement < partitions[previousPartition].count - 4 && localDistance < 16.0f) {
        *partition = previousPartition;
        *element = localElement;
    }
}

void mazeSamplePartition(std::int32_t partition, std::int32_t element, Vector3 *output) {
    *output = partitions[partition].points[element];
}

std::int32_t mazeFindPartitionDirection(std::int32_t sourcePartition, std::int32_t sourceElement, std::int32_t targetPartition, std::int32_t targetElement,
    std::int32_t *nextPartition, std::int32_t *nextElement) {
    RouteSearch search{sourcePartition, sourceElement, targetPartition, targetElement, -1, 0};
    for (std::array<EndpointLinks, 2> &partition : links)
        for (EndpointLinks &endpoint : partition)
            endpoint.visited = false;

    const std::int32_t backward = SearchRoute(search, 0);
    *nextPartition = search.nextPartition;
    *nextElement = search.nextElement;
    for (std::array<EndpointLinks, 2> &partition : links)
        for (EndpointLinks &endpoint : partition)
            endpoint.visited = false;

    const std::int32_t forward = SearchRoute(search, 1);
    std::int32_t distance = static_cast<std::uint16_t>(backward);
    if (sourcePartition == targetPartition) {
        if (sourceElement > targetElement)
            distance = -distance;
    } else if (backward && (!forward || static_cast<std::uint16_t>(backward) <= static_cast<std::uint16_t>(forward))) {
        distance = -distance;
    } else {
        *nextPartition = search.nextPartition;
        *nextElement = search.nextElement;
        distance = static_cast<std::uint16_t>(forward);
    }
    return distance > 0 ? 1 : distance < 0 ? -1 : 0;
}

bool mazeGetMarkerPosition(std::int32_t marker, Vector3 *output) {
    for (std::int32_t i = 0; i < markerCount; ++i) {
        const Marker entry = ReadValue<Marker>(markerBytes, static_cast<std::size_t>(i) * sizeof(Marker));
        if (entry.id != marker)
            continue;
        *output = partitions[entry.partition].points[entry.element];
        return true;
    }
    return false;
}

// @verified - p
void GetMazeMarkerRouteCoordinates(std::int32_t marker, std::int32_t *partition, std::int32_t *element) {
    for (std::int32_t index = 0; index < markerCount; ++index) {
        const Marker entry = ReadValue<Marker>(markerBytes, static_cast<std::size_t>(index) * sizeof(Marker));
        if (entry.id != marker)
            continue;
        *partition = entry.partition;
        *element = entry.element;
        return;
    }
}

// @verified - p
std::int32_t FindMazeMarkerWithinRadius(const Vector3 *position, float radius) {
    std::int32_t result = -1;
    const float squaredRadius = radius * radius;
    for (std::int32_t index = 0; index < markerCount; ++index) {
        const Marker marker = ReadValue<Marker>(markerBytes, static_cast<std::size_t>(index) * sizeof(Marker));
        const Vector3 &point = partitions[marker.partition].points[marker.element];
        const float x = position->x - point.x, z = position->z - point.z;
        if (squaredRadius > z * z + x * x)
            result = marker.id;
    }
    return result;
}

// @verified - p
std::int32_t FindCharacterMazeMarkerWithinRadius(Character *character, float radius) {
    const Vector3 position = GetCharacterWorldPosition(character);
    return FindMazeMarkerWithinRadius(&position, radius);
}

// @verified - p
bool IsCharacterWithinMazeMarkerRadius(Character *character, std::int32_t marker, float radius) {
    const Vector3 position = GetCharacterWorldPosition(character);
    Vector3 target;
    if (!mazeGetMarkerPosition(marker, &target))
        return false;
    const float dx = position.x - target.x;
    const float dz = position.z - target.z;
    return radius * radius > dz * dz + dx * dx;
}

// @verified - p
void GetMazeUnitPosition(std::int32_t index, Vector3 *output) {
    const MazeUnit *const unit = index < 0 || index >= unitCount ? nullptr : MUZT_Data + index;
    *output = unit->road->position;
}

// @verified - p
std::uint32_t GetMazeUnitYaw(std::int32_t index) {
    const MazeUnit *const unit = index < 0 || index >= unitCount ? nullptr : MUZT_Data + index;
    const MazeRoad *const road = unit->road;
    const RoadBlock &block = roadBlocks[road->block];
    const std::uint16_t record = static_cast<std::uint16_t>(road->flags);
    (void)AuthoredWord(block, record, 0);
    return 0x4000U - static_cast<std::uint32_t>(AuthoredWord(block, record, 24));
}
std::uint64_t GetCharacterMazeUnitIndex(Character *character) {
    const Vector3 position = GetCharacterWorldPosition(character);
    return static_cast<std::uint32_t>(FindUnit(position));
}

// @verified - p
std::uint32_t mazeGetCurrentRefCount() {
    return mazeCurrentReferenceCount;
}

// @verified - p
void mazeSetCurrentRefCount(std::uint32_t count) {
    mazeCurrentReferenceCount = count;
}

// @verified - p
HLib::Task *MAZ_fishCtrl() {
    std::int32_t count = 0;
    while (mazeFishControlPath[count].x < 999.0f)
        ++count;
    fishControlCount = count;
    fishControlPoints = static_cast<Vector3 *>(Allocate(12ULL * count, MEMORY_MAZE_FISH));
    for (std::int32_t index = 0; index < count; ++index)
        fishControlPoints[index] = {mazeFishControlPath[index].x, 0.0f, mazeFishControlPath[index].y};
    fishPathSamples = static_cast<Vector3 *>(Allocate(768ULL * count, MEMORY_MAZE_FISH));
    const RoadBlock spline{.controlPoints = fishControlPoints, .controlCount = fishControlCount};
    Vector3 *cursor = fishPathSamples;
    for (std::int32_t segment = 0; segment < count - 3; ++segment) {
        for (std::int32_t index = 0; index <= 64; ++index)
            SampleControlSpline(spline, segment * 64 + index, cursor[index]);
        cursor += 64;
    }
    const Vector3 &last = cursor[-1];
    for (std::int32_t index = 0; index < 16; ++index) {
        const float fraction = static_cast<float>(index + 1) / 17.0f;
        cursor[index].x = (fishPathSamples[0].x - last.x) * fraction + last.x;
        cursor[index].z = (fishPathSamples[0].z - last.z) * fraction + last.z;
    }
    cursor[16].x = 1000.0f;
    cursor[16].z = 1000.0f;
    fishPathRingLength = count * 64 - 177;
    fishScale = 1.0f;
    fishForceYaw = false;
    fishForcedYaw = 0;
    HLib::Task *task = nullptr;
    if (g_currentMap.name == AREA_MAZE_FISH.name) {
        task = HLib::EnqueueTaskWithParameter(MAZ_fishCtrl_Callback, 13, 5, 0, TASK_FISH);
        fishTask = task;
    }
    fishEnabled = false;
    return task;
}

// @verified - p
void MAZ_fishCtrl_Callback(void *) {
    if (fishEnabled)
        MAZ_fishCtrlUpdate(&fishBoxMin, &fishBoxMax, fishCount, fishSeed, fishModel);
}

// @verified - p
void MAZ_fishCtrlUpdate(const Vector3 *boxMin, const Vector3 *boxMax, std::uint32_t count, std::int32_t seed, const MT7Model *fish) {
    if (!IsWorldDrawingEnabled())
        return;
    SetSharedPrngSeed(static_cast<std::uint32_t>(seed));
    std::int32_t cursor = fishPathCursor;
    if (fishLastReferenceCount != mazeGetCurrentRefCount()) {
        fishLastReferenceCount = mazeGetCurrentRefCount();
        ++cursor;
    }
    if (cursor >= fishPathRingLength)
        cursor = 0;
    fishPathCursor = cursor;
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(count); ++index) {
        std::int32_t offset = static_cast<std::int32_t>(IncrementSharedPrngFloat() * static_cast<float>(fishPathRingLength));
        if (fishPathCursor + offset >= fishPathRingLength)
            offset -= fishPathRingLength;
        const Vector3 &point = fishPathSamples[fishPathCursor + offset];
        const Vector3 &next = fishPathCursor + offset + 1 >= fishPathRingLength ? fishPathSamples[0] : fishPathSamples[fishPathCursor + offset + 1];
        const float intensity = IncrementSharedPrngFloat();
        const float x = (boxMax->x - boxMin->x) * intensity + boxMin->x;
        const float y = (boxMax->y - boxMin->y) * intensity + boxMin->y;
        const float z = (boxMax->z - boxMin->z) * intensity + boxMin->z;
        const float jitter = IncrementSharedPrngFloat() * 0.60000002f - 0.30000001f;
        const std::uint16_t yaw = fishForceYaw ? fishForcedYaw : FixedAtan2(point.x - next.x, point.z - next.z);
        Matrix4x4_Push();
        Matrix4x4::preTranslate(x + point.x, y + jitter, z + point.z);
        Matrix4x4::RotateAroundY_Wrapper(yaw);
        Matrix4x4::Scale(fishScale, fishScale, fishScale);
        charaApplyScale(fish, fish ? fish->nodes.data() : nullptr);
        Matrix4x4::Pop(1);
    }
}

// @verified - p
void SetFish(const Vector3 *boxMin, const Vector3 *boxMax, std::int32_t count, std::int32_t seed, Character *fish) {
    if (!IsWorldDrawingEnabled())
        return;
    fishBoxMin = *boxMin;
    fishBoxMax = *boxMax;
    fishCount = static_cast<std::uint32_t>(count);
    fishSeed = seed;
    fishModel = static_cast<const MT7Model *>(fish->model);
    fishEnabled = true;
    fishForceYaw = true;
    fishForcedYaw = static_cast<std::uint16_t>(FixedAtan2(boxMin->x - boxMax->x, boxMin->z - boxMax->z));
    fishScale = 3.0f;
}

std::int64_t InitializeMazeNavigationTrail() {
    float startZ;
    float endX;
    float endZ;
    switch (g_currentMap.name) {
    case SHEN_NAME(KMZ2):
        startZ = -190.0f;
        endX = -500.0f;
        endZ = -310.0f;
        break;
    case SHEN_NAME(KMZ3):
        startZ = -400.0f;
        endX = -390.0f;
        endZ = -400.0f;
        break;
    case SHEN_NAME(KMZ4):
        startZ = -700.0f;
        endX = -400.0f;
        endZ = -810.0f;
        break;
    default:
        return static_cast<std::int64_t>(g_currentMap.name);
    }
    auto *points = static_cast<Vector3 *>(HLib::AllocateTaskData(96, SHEN_NAME(MZET)));
    SetSharedPrngSeed(g_currentMap.name);
    const float spanX = endX - -500.0f;
    const float spanZ = endZ - startZ;
    for (std::int32_t index = 0; index < 8; ++index) {
        const float jitterX = IncrementSharedPrngFloat() * 4.0f - 2.0f;
        const float jitterZ = IncrementSharedPrngFloat() * 4.0f - 2.0f;
        const float fraction = static_cast<float>(index) * 0.125f;
        points[index].y = 1.0f;
        points[index].x = (spanX * fraction - 500.0f) + jitterX;
        points[index].z = (spanZ * fraction + startZ) + jitterZ;
    }
    AddNaviTrailLocations({points + 1, 7});
    return HLib::FreeTaskData(points);
}

void ResetMazeHourAnimation(std::int32_t frame) {
    mazeHourFrame = frame;
    mazePreviousHourFrame = frame;
    mazeHourBlend = 0.0000099999997f;
    GetCurrentDateTime(mazeHourDate);
}


void SetMazeHourAnimationFrame(std::int32_t frame) {
    mazePreviousHourFrame = mazeHourFrame;
    mazeHourFrame = frame;
    mazeHourBlend = 0.99998999f;
    GetCurrentDateTime(mazeHourDate);
}

} // namespace shenmue::s2
