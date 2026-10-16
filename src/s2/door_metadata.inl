
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_AB00_23{{{.flag = 940, .answer = 3, .dialogue = 24}, {}}};
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_AK00_14{{{.flag = 819, .answer = 2, .dialogue = 15}, {}}};
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_AK00_24{{{.flag = 820, .answer = 2, .dialogue = 15}, {}}};
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_AR02_23{{{.flag = 142, .answer = 2, .dialogue = 13}, {}}};
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_AR03_06{{{.flag = 144, .answer = 2, .dialogue = 7}, {}}};
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_AR03_23{{{.flag = 142, .answer = 2, .dialogue = 13}, {}}};
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_KSH1_01{{{.flag = 944, .answer = 3, .dialogue = 64}, {}}};
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_QA00_40{{{.flag = 821, .answer = 3, .dialogue = 64}, {}}};
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_QA00_54{{{.flag = 822, .answer = 1, .dialogue = 9}, {}}};
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_QB00_01{{{.flag = 151, .answer = 3, .dialogue = 42}, {}}};
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_QB00_02{{{.flag = 151, .answer = 3, .dialogue = 42}, {}}};
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_QB00_04{{{.flag = 1153, .answer = 3, .dialogue = 56}, {}}};
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_QB00_05{{{.flag = 151, .answer = 3, .dialogue = 41}, {}}};
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_QB00_06{{{.flag = 151, .answer = 3, .dialogue = 41}, {}}};
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_QC00_08{{{.flag = 1050, .answer = 2, .dialogue = 15}, {}}};
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_QC00_09{{{.flag = 1050, .answer = 2, .dialogue = 15}, {}}};
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_QD00_40{{{.flag = 826, .answer = 0, .dialogue = 15}, {}}};
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_QE00_41{{{.flag = 823, .answer = 2, .dialogue = 15}, {}}};
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_QE00_46{{{.flag = 824, .answer = 0, .dialogue = 15}, {}}};
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_QEDJ_41{{{.flag = 823, .answer = 3, .dialogue = 40}, {}}};
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_QR00_01{{{.flag = 987, .answer = 3, .dialogue = 43}, {}}};
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_QRR0_01{{{.flag = 848, .answer = 3, .dialogue = 60}, {}}};
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_QUG0_05{{{.flag = 739, .answer = 3, .dialogue = 39}, {}}};
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_QUG0_06{{{.flag = 977, .answer = 2, .dialogue = 61}, {}}};
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_QUG0_07{{{.flag = 977, .answer = 2, .dialogue = 61}, {}}};
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_QUG0_08{{{.flag = 861, .answer = 2, .dialogue = 13}, {}}};
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_WKA0_03{{{.flag = 818, .answer = 0, .dialogue = 15}, {}}};
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_WN00_24{{{.flag = 825, .answer = 2, .dialogue = 15}, {}}};
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_WSY0_14{{{.flag = 615, .answer = 3, .dialogue = 22}, {}}};
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_WSY0_15{{{.flag = 615, .answer = 3, .dialogue = 22}, {}}};
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_WT00_01{{{.flag = 148, .answer = 2, .dialogue = 14}, {}}};
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_WT00_02{{{.flag = 148, .answer = 2, .dialogue = 14}, {}}};
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_WT00_09{{{.flag = 1167, .answer = 3, .dialogue = 0}, {}}};
constexpr std::array<DoorFlagRule, 2> DOOR_FLAG_RULES_WTA0_32{{{.flag = 138, .answer = 0, .dialogue = 15}, {}}};
constexpr std::array<DoorAreaMetadata, 3> DOOR_METADATA_AB00{
    {{.door = 23, .flags = 1, .opens = 0, .closes = 2400, .answer = 2, .rules = DOOR_FLAG_RULES_AB00_23.data()},
        {.door = 1, .flags = 1, .opens = 0, .closes = 0, .answer = 2, .dialogues = {13, 14, 15, 0}}, {}}};
constexpr std::array<DoorAreaMetadata, 10> DOOR_METADATA_AK00{
    {{.door = 24, .flags = 0, .opens = 0, .closes = 2400, .answer = 2, .rules = DOOR_FLAG_RULES_AK00_24.data()},
        {.door = 5, .flags = 0, .opens = 0, .closes = 2200, .answer = 2, .dialogues = {6, 3, 7, 0}},
        {.door = 18, .flags = 0, .opens = 0, .closes = 2400, .answer = 3}, {.door = 7, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 10, .flags = 1, .opens = 0, .closes = 2400, .answer = 2}, {.door = 12, .flags = 1, .opens = 0, .closes = 2400, .answer = 2},
        {.door = 13, .flags = 1, .opens = 0, .closes = 2400, .answer = 2},
        {.door = 14, .flags = 1, .opens = 0, .closes = 2400, .answer = 2, .rules = DOOR_FLAG_RULES_AK00_14.data()},
        {.door = 30, .flags = 1, .opens = 0, .closes = 0, .answer = 2, .dialogues = {13, 14, 15, 0}}, {}}};
constexpr std::array<DoorAreaMetadata, 2> DOOR_METADATA_AKA3{{{.door = 14, .flags = 0, .opens = 0, .closes = 2400, .answer = 2}, {}}};
constexpr std::array<DoorAreaMetadata, 2> DOOR_METADATA_AKS0{{{.door = 5, .flags = 1, .opens = 0, .closes = 2400, .answer = 3}, {}}};
constexpr std::array<DoorAreaMetadata, 2> DOOR_METADATA_AKS1{{{.door = 1, .flags = 1, .opens = 0, .closes = 2400, .answer = 3}, {}}};
constexpr std::array<DoorAreaMetadata, 2> DOOR_METADATA_AKT0{{{.door = 10, .flags = 0, .opens = 0, .closes = 2400, .answer = 2}, {}}};
constexpr std::array<DoorAreaMetadata, 2> DOOR_METADATA_AKT2{{{.door = 12, .flags = 0, .opens = 0, .closes = 2400, .answer = 2}, {}}};
constexpr std::array<DoorAreaMetadata, 2> DOOR_METADATA_AKT3{{{.door = 13, .flags = 0, .opens = 0, .closes = 2400, .answer = 2}, {}}};
constexpr std::array<DoorAreaMetadata, 3> DOOR_METADATA_AKY0{
    {{.door = 2, .flags = 0, .opens = 0, .closes = 2400, .answer = 2}, {.door = 1, .flags = 1, .opens = 0, .closes = 2400, .answer = 2}, {}}};
constexpr std::array<DoorAreaMetadata, 10> DOOR_METADATA_AR02{{{.door = 20, .flags = 1, .opens = 0, .closes = 2400, .answer = 3},
    {.door = 21, .flags = 0, .opens = 0, .closes = 2400, .answer = 3}, {.door = 2, .flags = 1, .opens = 0, .closes = 2400, .answer = 3},
    {.door = 1, .flags = 1, .opens = 0, .closes = 0, .answer = 3}, {.door = 22, .flags = 1, .opens = 0, .closes = 0, .answer = 2, .dialogues = {13, 14, 15, 0}},
    {.door = 4, .flags = 1, .opens = 0, .closes = 2000, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 5, .flags = 0, .opens = 0, .closes = 2000, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 23, .flags = 0, .opens = 0, .closes = 2400, .answer = 2, .rules = DOOR_FLAG_RULES_AR02_23.data(), .dialogues = {7, 14, 62, 0}},
    {.door = 12, .flags = 4, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}}, {}}};
constexpr std::array<DoorAreaMetadata, 14> DOOR_METADATA_AR03{
    {{.door = 23, .flags = 0, .opens = 0, .closes = 2400, .answer = 2, .rules = DOOR_FLAG_RULES_AR03_23.data(), .dialogues = {7, 14, 62, 0}},
        {.door = 8, .flags = 1, .opens = 0, .closes = 1900, .answer = 2, .dialogues = {4, 5, 6, 0}},
        {.door = 9, .flags = 0, .opens = 0, .closes = 1900, .answer = 2, .dialogues = {4, 5, 6, 0}},
        {.door = 10, .flags = 1, .opens = 0, .closes = 2400, .answer = 3}, {.door = 11, .flags = 0, .opens = 0, .closes = 2400, .answer = 3},
        {.door = 6, .flags = 1, .opens = 0, .closes = 2400, .answer = 3, .rules = DOOR_FLAG_RULES_AR03_06.data()},
        {.door = 13, .flags = 4, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 14, .flags = 4, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 15, .flags = 4, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 16, .flags = 4, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 17, .flags = 4, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 18, .flags = 4, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 19, .flags = 4, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}}, {}}};
constexpr std::array<DoorAreaMetadata, 3> DOOR_METADATA_ARA0{
    {{.door = 21, .flags = 1, .opens = 0, .closes = 2400, .answer = 3}, {.door = 20, .flags = 0, .opens = 0, .closes = 2400, .answer = 3}, {}}};
constexpr std::array<DoorAreaMetadata, 2> DOOR_METADATA_ARC0{{{.door = 2, .flags = 1, .opens = 0, .closes = 2400, .answer = 3}, {}}};
constexpr std::array<DoorAreaMetadata, 3> DOOR_METADATA_ARM0{
    {{.door = 5, .flags = 1, .opens = 0, .closes = 2400, .answer = 3}, {.door = 4, .flags = 0, .opens = 0, .closes = 2400, .answer = 3}, {}}};
constexpr std::array<DoorAreaMetadata, 3> DOOR_METADATA_ARZ0{
    {{.door = 11, .flags = 1, .opens = 0, .closes = 2400, .answer = 3}, {.door = 10, .flags = 0, .opens = 0, .closes = 2400, .answer = 3}, {}}};
constexpr std::array<DoorAreaMetadata, 3> DOOR_METADATA_WB00{{{.door = 7, .flags = 0, .opens = 0, .closes = 2400, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 8, .flags = 1, .opens = 0, .closes = 2400, .answer = 2, .dialogues = {6, 3, 7, 0}}, {}}};
constexpr std::array<DoorAreaMetadata, 15> DOOR_METADATA_WE00{
    {{.door = 36, .flags = 1, .opens = 0, .closes = 2100, .answer = 3}, {.door = 11, .flags = 1, .opens = 0, .closes = 2100, .answer = 3},
        {.door = 5, .flags = 1, .opens = 0, .closes = 2100, .answer = 3}, {.door = 25, .flags = 1, .opens = 0, .closes = 2400, .answer = 3},
        {.door = 9, .flags = 1, .opens = 0, .closes = 2000, .answer = 2, .dialogues = {6, 3, 7, 0}},
        {.door = 10, .flags = 0, .opens = 0, .closes = 2000, .answer = 2, .dialogues = {6, 3, 7, 0}},
        {.door = 38, .flags = 1, .opens = 0, .closes = 2400, .answer = 3}, {.door = 39, .flags = 0, .opens = 0, .closes = 2400, .answer = 3},
        {.door = 15, .flags = 1, .opens = 0, .closes = 2400, .answer = 3},
        {.door = 21, .flags = 1, .opens = 0, .closes = 2100, .answer = 2, .dialogues = {6, 3, 7, 0}},
        {.door = 22, .flags = 0, .opens = 0, .closes = 2100, .answer = 2, .dialogues = {6, 3, 7, 0}},
        {.door = 19, .flags = 1, .opens = 0, .closes = 2400, .answer = 3}, {.door = 34, .flags = 1, .opens = 0, .closes = 2400, .answer = 3},
        {.door = 40, .flags = 1, .opens = 0, .closes = 2000, .answer = 2, .dialogues = {6, 3, 7, 0}}, {}}};
constexpr std::array<DoorAreaMetadata, 2> DOOR_METADATA_WECF{{{.door = 19, .flags = 0, .opens = 0, .closes = 2400, .answer = 3}, {}}};
constexpr std::array<DoorAreaMetadata, 2> DOOR_METADATA_WEG0{{{.door = 34, .flags = 0, .opens = 0, .closes = 2400, .answer = 3}, {}}};
constexpr std::array<DoorAreaMetadata, 3> DOOR_METADATA_WEM1{
    {{.door = 39, .flags = 1, .opens = 0, .closes = 2400, .answer = 3}, {.door = 38, .flags = 0, .opens = 0, .closes = 2400, .answer = 3}, {}}};
constexpr std::array<DoorAreaMetadata, 2> DOOR_METADATA_WES1{{{.door = 25, .flags = 0, .opens = 0, .closes = 2400, .answer = 3}, {}}};
constexpr std::array<DoorAreaMetadata, 2> DOOR_METADATA_WESM{{{.door = 40, .flags = 1, .opens = 0, .closes = 2400, .answer = 3}, {}}};
constexpr std::array<DoorAreaMetadata, 6> DOOR_METADATA_WK00{{{.door = 1, .flags = 1, .opens = 0, .closes = 2400, .answer = 3},
    {.door = 19, .flags = 1, .opens = 0, .closes = 2300, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 23, .flags = 1, .opens = 0, .closes = 2000, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 3, .flags = 5, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
    {.door = 4, .flags = 5, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}}, {}}};
constexpr std::array<DoorAreaMetadata, 20> DOOR_METADATA_WKA0{{{.door = 1, .flags = 0, .opens = 0, .closes = 2400, .answer = 3},
    {.door = 3, .flags = 1, .opens = 0, .closes = 2400, .knock = 1, .answer = 1, .rules = DOOR_FLAG_RULES_WKA0_03.data(), .dialogues = {8, 9, 10, 0}},
    {.door = 4, .flags = 1, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
    {.door = 5, .flags = 1, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
    {.door = 6, .flags = 1, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {11, 9, 12, 0}},
    {.door = 7, .flags = 1, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
    {.door = 9, .flags = 1, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {11, 9, 12, 0}},
    {.door = 10, .flags = 1, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
    {.door = 11, .flags = 1, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {11, 9, 12, 0}},
    {.door = 12, .flags = 1, .opens = 0, .closes = 0, .knock = 1, .dialogues = {15, 0, 0, 0}},
    {.door = 13, .flags = 1, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
    {.door = 14, .flags = 1, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {11, 9, 12, 0}},
    {.door = 15, .flags = 1, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {11, 9, 12, 0}},
    {.door = 16, .flags = 1, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {11, 9, 12, 0}},
    {.door = 17, .flags = 1, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
    {.door = 18, .flags = 1, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {11, 9, 12, 0}},
    {.door = 28, .flags = 1, .opens = 0, .closes = 2400, .answer = 1}, {.door = 29, .flags = 1, .opens = 0, .closes = 2400, .answer = 1},
    {.door = 30, .flags = 1, .opens = 0, .closes = 2400, .answer = 1}, {}}};
constexpr std::array<DoorAreaMetadata, 35> DOOR_METADATA_WN00{{{.door = 50, .flags = 0, .opens = 0, .closes = 2100, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 51, .flags = 1, .opens = 0, .closes = 2100, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 62, .flags = 0, .opens = 0, .closes = 2100, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 63, .flags = 1, .opens = 0, .closes = 2100, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 66, .flags = 0, .opens = 0, .closes = 2100, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 67, .flags = 1, .opens = 0, .closes = 2100, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 82, .flags = 1, .opens = 0, .closes = 2400, .answer = 2}, {.door = 27, .flags = 0, .opens = 0, .closes = 2400, .answer = 2},
    {.door = 38, .flags = 0, .opens = 0, .closes = 2100, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 39, .flags = 1, .opens = 0, .closes = 2100, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 42, .flags = 0, .opens = 0, .closes = 2100, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 43, .flags = 1, .opens = 0, .closes = 2100, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 46, .flags = 0, .opens = 0, .closes = 2100, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 47, .flags = 1, .opens = 0, .closes = 2100, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 58, .flags = 0, .opens = 0, .closes = 2400, .answer = 3}, {.door = 60, .flags = 0, .opens = 0, .closes = 2400, .answer = 3},
    {.door = 24, .flags = 0, .opens = 0, .closes = 2400, .answer = 3, .rules = DOOR_FLAG_RULES_WN00_24.data()},
    {.door = 54, .flags = 0, .opens = 0, .closes = 2100, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 55, .flags = 1, .opens = 0, .closes = 2100, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 80, .flags = 1, .opens = 0, .closes = 2400, .answer = 3}, {.door = 18, .flags = 0, .opens = 0, .closes = 2400, .answer = 3},
    {.door = 83, .flags = 1, .opens = 0, .closes = 2000, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 29, .flags = 0, .opens = 0, .closes = 2000, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 81, .flags = 1, .opens = 0, .closes = 2400, .answer = 3}, {.door = 70, .flags = 0, .opens = 0, .closes = 2400, .answer = 3},
    {.door = 84, .flags = 1, .opens = 0, .closes = 2400, .answer = 3}, {.door = 31, .flags = 0, .opens = 0, .closes = 2400, .answer = 3},
    {.door = 20, .flags = 1, .opens = 0, .closes = 2200, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 6, .flags = 1, .opens = 0, .closes = 2200, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 14, .flags = 0, .opens = 0, .closes = 2200, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 10, .flags = 1, .opens = 0, .closes = 2200, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 2, .flags = 1, .opens = 0, .closes = 2200, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 3, .flags = 4, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
    {.door = 4, .flags = 4, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}}, {}}};
constexpr std::array<DoorAreaMetadata, 4> DOOR_METADATA_WR00{{{.door = 2, .flags = 1, .opens = 0, .closes = 2400, .answer = 3},
    {.door = 3, .flags = 1, .opens = 0, .closes = 2400, .answer = 3}, {.door = 4, .flags = 1, .opens = 0, .closes = 2400, .answer = 3}, {}}};
constexpr std::array<DoorAreaMetadata, 2> DOOR_METADATA_WRS2{{{.door = 4, .flags = 0, .opens = 0, .closes = 2400, .answer = 3}, {}}};
constexpr std::array<DoorAreaMetadata, 14> DOOR_METADATA_WS00{{{.door = 6, .flags = 1, .opens = 0, .closes = 2200, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 10, .flags = 1, .opens = 0, .closes = 2400, .answer = 3}, {.door = 14, .flags = 1, .opens = 0, .closes = 2400, .answer = 3},
    {.door = 15, .flags = 0, .opens = 0, .closes = 2400, .answer = 1}, {.door = 25, .flags = 1, .opens = 0, .closes = 2400, .answer = 3},
    {.door = 41, .flags = 0, .opens = 0, .closes = 2400, .answer = 3}, {.door = 27, .flags = 1, .opens = 0, .closes = 2400, .answer = 3},
    {.door = 34, .flags = 0, .opens = 0, .closes = 2200, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 35, .flags = 1, .opens = 0, .closes = 2400, .answer = 3}, {.door = 37, .flags = 1, .opens = 0, .closes = 2400, .answer = 3},
    {.door = 39, .flags = 1, .opens = 0, .closes = 2200, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 3, .flags = 5, .opens = 0, .closes = 0, .knock = 1, .answer = 2, .dialogues = {13, 14, 15, 0}},
    {.door = 4, .flags = 5, .opens = 0, .closes = 0, .knock = 1, .answer = 2, .dialogues = {13, 14, 15, 0}}, {}}};
constexpr std::array<DoorAreaMetadata, 8> DOOR_METADATA_WSY0{{{.door = 15, .flags = 1, .opens = 0, .closes = 2400, .rules = DOOR_FLAG_RULES_WSY0_15.data()},
    {.door = 14, .flags = 0, .opens = 0, .closes = 2400, .rules = DOOR_FLAG_RULES_WSY0_14.data()},
    {.door = 16, .flags = 1, .opens = 0, .closes = 0, .answer = 1},
    {.door = 18, .flags = 1, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
    {.door = 19, .flags = 1, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
    {.door = 20, .flags = 1, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
    {.door = 21, .flags = 1, .opens = 0, .closes = 2400, .answer = 1}, {}}};
constexpr std::array<DoorAreaMetadata, 23> DOOR_METADATA_WT00{{{.door = 40, .flags = 0, .opens = 0, .closes = 2200, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 43, .flags = 0, .opens = 0, .closes = 2200, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 42, .flags = 0, .opens = 0, .closes = 2200, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 38, .flags = 0, .opens = 0, .closes = 0, .answer = 2, .dialogues = {13, 14, 15, 0}},
    {.door = 49, .flags = 0, .opens = 0, .closes = 2200, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 41, .flags = 0, .opens = 0, .closes = 2200, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 9, .flags = 1, .opens = 0, .closes = 2400, .answer = 3, .rules = DOOR_FLAG_RULES_WT00_09.data()},
    {.door = 39, .flags = 0, .opens = 0, .closes = 2200, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 55, .flags = 1, .opens = 0, .closes = 2200, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 37, .flags = 0, .opens = 0, .closes = 2200, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 13, .flags = 0, .opens = 0, .closes = 0, .answer = 1, .dialogues = {8, 9, 10, 0}},
    {.door = 36, .flags = 0, .opens = 0, .closes = 2200, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 35, .flags = 0, .opens = 0, .closes = 2200, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 51, .flags = 0, .opens = 0, .closes = 2400, .answer = 3},
    {.door = 50, .flags = 0, .opens = 0, .closes = 2200, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 46, .flags = 0, .opens = 0, .closes = 1800, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 44, .flags = 0, .opens = 0, .closes = 2400, .answer = 3},
    {.door = 45, .flags = 0, .opens = 0, .closes = 1900, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 48, .flags = 0, .opens = 0, .closes = 2100, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 47, .flags = 0, .opens = 0, .closes = 2100, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 1, .flags = 0, .opens = 0, .closes = 2400, .answer = 3, .rules = DOOR_FLAG_RULES_WT00_01.data()},
    {.door = 2, .flags = 1, .opens = 0, .closes = 2400, .answer = 3, .rules = DOOR_FLAG_RULES_WT00_02.data()}, {}}};
constexpr std::array<DoorAreaMetadata, 16> DOOR_METADATA_WTA0{{{.door = 1, .flags = 1, .opens = 0, .closes = 2400, .answer = 3},
    {.door = 2, .flags = 0, .opens = 0, .closes = 2400, .answer = 3},
    {.door = 20, .flags = 0, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
    {.door = 21, .flags = 0, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
    {.door = 22, .flags = 0, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
    {.door = 23, .flags = 0, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
    {.door = 24, .flags = 0, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
    {.door = 25, .flags = 0, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
    {.door = 26, .flags = 0, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
    {.door = 27, .flags = 0, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
    {.door = 28, .flags = 0, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
    {.door = 29, .flags = 0, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
    {.door = 30, .flags = 0, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
    {.door = 31, .flags = 0, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
    {.door = 32, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1, .rules = DOOR_FLAG_RULES_WTA0_32.data(), .dialogues = {8, 9, 10, 0}}, {}}};
constexpr std::array<DoorAreaMetadata, 6> DOOR_METADATA_Q100{{{.door = 6, .flags = 1, .opens = 0, .closes = 2400, .answer = 1},
    {.door = 5, .flags = 0, .opens = 0, .closes = 2400, .answer = 1}, {.door = 11, .flags = 1, .opens = 0, .closes = 2400, .answer = 2},
    {.door = 12, .flags = 1, .opens = 0, .closes = 2400, .answer = 2}, {.door = 13, .flags = 1, .opens = 0, .closes = 2400, .answer = 2}, {}}};
constexpr std::array<DoorAreaMetadata, 5> DOOR_METADATA_Q300{
    {{.door = 7, .flags = 1, .opens = 0, .closes = 2400, .answer = 3}, {.door = 8, .flags = 0, .opens = 0, .closes = 2400, .answer = 3},
        {.door = 9, .flags = 1, .opens = 0, .closes = 2400, .answer = 3}, {.door = 10, .flags = 0, .opens = 0, .closes = 2400, .answer = 3}, {}}};
constexpr std::array<DoorAreaMetadata, 39> DOOR_METADATA_QA00{
    {{.door = 40, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 2, .rules = DOOR_FLAG_RULES_QA00_40.data()},
        {.door = 88, .flags = 0, .opens = 0, .closes = 0, .answer = 3, .dialogues = {64, 0, 0, 0}},
        {.door = 41, .flags = 0, .opens = 0, .closes = 0, .knock = 1, .dialogues = {13, 14, 15, 0}},
        {.door = 48, .flags = 0, .opens = 0, .closes = 0, .knock = 1, .answer = 2, .dialogues = {13, 14, 15, 0}},
        {.door = 77, .flags = 0, .opens = 0, .closes = 0},
        {.door = 54, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1, .rules = DOOR_FLAG_RULES_QA00_54.data(), .dialogues = {8, 9, 10, 0}},
        {.door = 1, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 2, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 3, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 4, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 5, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 6, .flags = 4, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 7, .flags = 4, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 8, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 9, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 31, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 32, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 10, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 11, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 12, .flags = 4, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 13, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 14, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 15, .flags = 4, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 16, .flags = 4, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 17, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 18, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 19, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 20, .flags = 4, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 21, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 22, .flags = 4, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 23, .flags = 4, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 24, .flags = 4, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 25, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 26, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 27, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 28, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 29, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 30, .flags = 4, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}}, {}}};
constexpr std::array<DoorAreaMetadata, 3> DOOR_METADATA_QA22{
    {{.door = 51, .flags = 0, .opens = 0, .closes = 2400, .answer = 3}, {.door = 45, .flags = 1, .opens = 0, .closes = 2400, .answer = 3}, {}}};
constexpr std::array<DoorAreaMetadata, 5> DOOR_METADATA_QAE1{
    {{.door = 8, .flags = 1, .opens = 0, .closes = 2400, .answer = 3}, {.door = 7, .flags = 0, .opens = 0, .closes = 2400, .answer = 3},
        {.door = 10, .flags = 1, .opens = 0, .closes = 2400, .answer = 3}, {.door = 9, .flags = 0, .opens = 0, .closes = 2400, .answer = 3}, {}}};
constexpr std::array<DoorAreaMetadata, 7> DOOR_METADATA_QAW1{
    {{.door = 44, .flags = 0, .opens = 0, .closes = 2400, .answer = 3}, {.door = 50, .flags = 1, .opens = 0, .closes = 2400, .answer = 3},
        {.door = 45, .flags = 0, .opens = 0, .closes = 2400, .answer = 3}, {.door = 51, .flags = 1, .opens = 0, .closes = 2400, .answer = 3},
        {.door = 48, .flags = 0, .opens = 0, .closes = 2400, .answer = 3}, {.door = 49, .flags = 0, .opens = 0, .closes = 2400, .answer = 3}, {}}};
constexpr std::array<DoorAreaMetadata, 5> DOOR_METADATA_QAW6{
    {{.door = 53, .flags = 1, .opens = 0, .closes = 2400, .answer = 3}, {.door = 47, .flags = 0, .opens = 0, .closes = 2400, .answer = 3},
        {.door = 52, .flags = 1, .opens = 0, .closes = 2400, .answer = 3}, {.door = 46, .flags = 0, .opens = 0, .closes = 2400, .answer = 3}, {}}};
constexpr std::array<DoorAreaMetadata, 8> DOOR_METADATA_QB00{
    {{.door = 5, .flags = 1, .opens = 0, .closes = 2400, .answer = 1, .rules = DOOR_FLAG_RULES_QB00_05.data()},
        {.door = 6, .flags = 0, .opens = 0, .closes = 2400, .answer = 1, .rules = DOOR_FLAG_RULES_QB00_06.data()},
        {.door = 1, .flags = 0, .opens = 0, .closes = 2400, .answer = 3, .rules = DOOR_FLAG_RULES_QB00_01.data()},
        {.door = 2, .flags = 0, .opens = 0, .closes = 2400, .answer = 3, .rules = DOOR_FLAG_RULES_QB00_02.data()},
        {.door = 3, .flags = 0, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 4, .flags = 0, .opens = 0, .closes = 2400, .answer = 3, .rules = DOOR_FLAG_RULES_QB00_04.data()},
        {.door = 5, .flags = 1, .opens = 0, .closes = 0, .answer = 3}, {}}};
constexpr std::array<DoorAreaMetadata, 2> DOOR_METADATA_QBAA{{{.door = 4, .flags = 1, .opens = 0, .closes = 2400, .answer = 3}, {}}};
constexpr std::array<DoorAreaMetadata, 13> DOOR_METADATA_QC00{
    {{.door = 8, .flags = 1, .opens = 0, .closes = 2400, .answer = 1, .rules = DOOR_FLAG_RULES_QC00_08.data()},
        {.door = 9, .flags = 0, .opens = 0, .closes = 2400, .answer = 1, .rules = DOOR_FLAG_RULES_QC00_09.data()},
        {.door = 44, .flags = 0, .opens = 0, .closes = 2400, .answer = 2}, {.door = 43, .flags = 0, .opens = 0, .closes = 2400, .answer = 2},
        {.door = 45, .flags = 0, .opens = 0, .closes = 2400, .answer = 2},
        {.door = 1, .flags = 4, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 2, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 3, .flags = 4, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 4, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 5, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 6, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 7, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1}, {}}};
constexpr std::array<DoorAreaMetadata, 4> DOOR_METADATA_QC01{{{.door = 5, .flags = 0, .opens = 0, .closes = 2400, .answer = 2},
    {.door = 6, .flags = 0, .opens = 0, .closes = 2400, .answer = 2}, {.door = 7, .flags = 0, .opens = 0, .closes = 2400, .answer = 2}, {}}};
constexpr std::array<DoorAreaMetadata, 4> DOOR_METADATA_QC06{{{.door = 40, .flags = 0, .opens = 0, .closes = 2400, .answer = 1},
    {.door = 41, .flags = 0, .opens = 0, .closes = 2400, .answer = 1}, {.door = 42, .flags = 0, .opens = 0, .closes = 2400, .answer = 1}, {}}};
constexpr std::array<DoorAreaMetadata, 7> DOOR_METADATA_QCAE{
    {{.door = 13, .flags = 0, .opens = 0, .closes = 2400, .answer = 2}, {.door = 12, .flags = 0, .opens = 0, .closes = 2400, .answer = 2},
        {.door = 11, .flags = 0, .opens = 0, .closes = 2400, .answer = 2}, {.door = 5, .flags = 1, .opens = 0, .closes = 2400, .answer = 2},
        {.door = 6, .flags = 1, .opens = 0, .closes = 2400, .answer = 2}, {.door = 7, .flags = 1, .opens = 0, .closes = 2400, .answer = 2}, {}}};
constexpr std::array<DoorAreaMetadata, 9> DOOR_METADATA_QD00{
    {{.door = 40, .flags = 0, .opens = 0, .closes = 2400, .answer = 2, .rules = DOOR_FLAG_RULES_QD00_40.data(), .dialogues = {13, 14, 15, 0}},
        {.door = 1, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 2, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 3, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 4, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 5, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 6, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 7, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1}, {}}};
constexpr std::array<DoorAreaMetadata, 5> DOOR_METADATA_QD01{{{.door = 1, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
    {.door = 2, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1}, {.door = 3, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
    {.door = 4, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1}, {}}};
constexpr std::array<DoorAreaMetadata, 2> DOOR_METADATA_QDKJ{{{.door = 40, .flags = 1, .opens = 0, .closes = 2400, .answer = 3}, {}}};
constexpr std::array<DoorAreaMetadata, 21> DOOR_METADATA_QE00{
    {{.door = 41, .flags = 0, .opens = 0, .closes = 2400, .answer = 2, .rules = DOOR_FLAG_RULES_QE00_41.data()},
        {.door = 43, .flags = 0, .opens = 0, .closes = 2400, .knock = 1},
        {.door = 46, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1, .rules = DOOR_FLAG_RULES_QE00_46.data(), .dialogues = {8, 9, 10, 0}},
        {.door = 5, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 6, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 7, .flags = 4, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 8, .flags = 4, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 9, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 10, .flags = 0, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 11, .flags = 0, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 12, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 13, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 14, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 15, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 16, .flags = 4, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 17, .flags = 4, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 18, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 19, .flags = 4, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 20, .flags = 0, .opens = 0, .closes = 2400, .knock = 1, .answer = 1},
        {.door = 21, .flags = 4, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}}, {}}};
constexpr std::array<DoorAreaMetadata, 8> DOOR_METADATA_QE01{
    {{.door = 5, .flags = 0, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 6, .flags = 0, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 7, .flags = 0, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 8, .flags = 0, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 9, .flags = 0, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 10, .flags = 0, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 11, .flags = 0, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}}, {}}};
constexpr std::array<DoorAreaMetadata, 13> DOOR_METADATA_QE06{
    {{.door = 5, .flags = 0, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 6, .flags = 0, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 7, .flags = 0, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 8, .flags = 0, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 9, .flags = 0, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 10, .flags = 0, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 11, .flags = 0, .opens = 0, .closes = 0, .knock = 1, .answer = 1, .dialogues = {8, 9, 10, 0}},
        {.door = 17, .flags = 0, .opens = 0, .closes = 2400, .answer = 2}, {.door = 18, .flags = 0, .opens = 0, .closes = 2400, .answer = 2},
        {.door = 19, .flags = 0, .opens = 0, .closes = 2400, .answer = 2}, {.door = 20, .flags = 0, .opens = 0, .closes = 2400, .answer = 2},
        {.door = 21, .flags = 0, .opens = 0, .closes = 2400, .answer = 2}, {}}};
constexpr std::array<DoorAreaMetadata, 2> DOOR_METADATA_QEDJ{
    {{.door = 41, .flags = 1, .opens = 0, .closes = 2400, .answer = 3, .rules = DOOR_FLAG_RULES_QEDJ_41.data()}, {}}};
constexpr std::array<DoorAreaMetadata, 4> DOOR_METADATA_QEHI{{{.door = 43, .flags = 1, .opens = 0, .closes = 2400},
    {.door = 44, .flags = 0, .opens = 0, .closes = 2400, .knock = 1}, {.door = 45, .flags = 0, .opens = 0, .closes = 2400, .knock = 1}, {}}};
constexpr std::array<DoorAreaMetadata, 25> DOOR_METADATA_QF00{
    {{.door = 1, .flags = 0, .opens = 0, .closes = 2400, .answer = 2}, {.door = 2, .flags = 0, .opens = 0, .closes = 2400, .answer = 2},
        {.door = 3, .flags = 0, .opens = 0, .closes = 2400, .answer = 2}, {.door = 4, .flags = 0, .opens = 0, .closes = 2400, .answer = 2},
        {.door = 5, .flags = 0, .opens = 0, .closes = 2400, .answer = 2}, {.door = 6, .flags = 0, .opens = 0, .closes = 2400, .answer = 2},
        {.door = 7, .flags = 0, .opens = 0, .closes = 2400, .answer = 2}, {.door = 8, .flags = 0, .opens = 0, .closes = 2400, .answer = 2},
        {.door = 9, .flags = 0, .opens = 0, .closes = 2400, .answer = 2}, {.door = 10, .flags = 0, .opens = 0, .closes = 2400, .answer = 2},
        {.door = 11, .flags = 0, .opens = 0, .closes = 2400, .answer = 2}, {.door = 12, .flags = 0, .opens = 0, .closes = 2400, .answer = 2},
        {.door = 13, .flags = 0, .opens = 0, .closes = 2400, .answer = 2}, {.door = 14, .flags = 0, .opens = 0, .closes = 2400, .answer = 2},
        {.door = 15, .flags = 0, .opens = 0, .closes = 2400, .answer = 2}, {.door = 16, .flags = 0, .opens = 0, .closes = 2400, .answer = 2},
        {.door = 17, .flags = 0, .opens = 0, .closes = 2400, .answer = 2}, {.door = 18, .flags = 0, .opens = 0, .closes = 2400, .answer = 2},
        {.door = 19, .flags = 0, .opens = 0, .closes = 2400, .answer = 2}, {.door = 20, .flags = 0, .opens = 0, .closes = 2400, .answer = 2},
        {.door = 21, .flags = 0, .opens = 0, .closes = 2400, .answer = 2}, {.door = 22, .flags = 0, .opens = 0, .closes = 2400, .answer = 2},
        {.door = 23, .flags = 0, .opens = 0, .closes = 2400, .answer = 2}, {.door = 24, .flags = 0, .opens = 0, .closes = 2400, .answer = 2}, {}}};
constexpr std::array<DoorAreaMetadata, 5> DOOR_METADATA_QF01{
    {{.door = 20, .flags = 0, .opens = 0, .closes = 2400, .answer = 2}, {.door = 21, .flags = 0, .opens = 0, .closes = 2400, .answer = 2},
        {.door = 22, .flags = 0, .opens = 0, .closes = 0, .answer = 3, .dialogues = {39, 0, 0, 0}},
        {.door = 23, .flags = 0, .opens = 0, .closes = 0, .answer = 3, .dialogues = {39, 0, 0, 0}}, {}}};
constexpr std::array<DoorAreaMetadata, 5> DOOR_METADATA_QF02{
    {{.door = 20, .flags = 0, .opens = 0, .closes = 2400, .answer = 2}, {.door = 21, .flags = 0, .opens = 0, .closes = 2400, .answer = 2},
        {.door = 22, .flags = 0, .opens = 0, .closes = 2400, .answer = 2}, {.door = 23, .flags = 0, .opens = 0, .closes = 2400, .answer = 2}, {}}};
constexpr std::array<DoorAreaMetadata, 25> DOOR_METADATA_QF06{{{.door = 1, .flags = 0, .opens = 0, .closes = 0, .answer = 3, .dialogues = {38, 0, 0, 0}},
    {.door = 2, .flags = 0, .opens = 0, .closes = 0, .answer = 3, .dialogues = {40, 0, 0, 0}},
    {.door = 3, .flags = 0, .opens = 0, .closes = 0, .answer = 3, .dialogues = {37, 0, 0, 0}},
    {.door = 4, .flags = 0, .opens = 0, .closes = 0, .answer = 3, .dialogues = {38, 0, 0, 0}},
    {.door = 5, .flags = 0, .opens = 0, .closes = 0, .answer = 3, .dialogues = {40, 0, 0, 0}},
    {.door = 6, .flags = 0, .opens = 0, .closes = 0, .answer = 3, .dialogues = {37, 0, 0, 0}},
    {.door = 7, .flags = 0, .opens = 0, .closes = 0, .answer = 3, .dialogues = {38, 0, 0, 0}},
    {.door = 8, .flags = 0, .opens = 0, .closes = 0, .answer = 3, .dialogues = {40, 0, 0, 0}},
    {.door = 9, .flags = 0, .opens = 0, .closes = 0, .answer = 3, .dialogues = {37, 0, 0, 0}},
    {.door = 10, .flags = 0, .opens = 0, .closes = 0, .answer = 3, .dialogues = {38, 0, 0, 0}},
    {.door = 11, .flags = 0, .opens = 0, .closes = 0, .answer = 3, .dialogues = {40, 0, 0, 0}},
    {.door = 12, .flags = 0, .opens = 0, .closes = 0, .answer = 3, .dialogues = {37, 0, 0, 0}},
    {.door = 13, .flags = 0, .opens = 0, .closes = 0, .answer = 3, .dialogues = {38, 0, 0, 0}},
    {.door = 14, .flags = 0, .opens = 0, .closes = 0, .answer = 3, .dialogues = {40, 0, 0, 0}},
    {.door = 15, .flags = 0, .opens = 0, .closes = 0, .answer = 3, .dialogues = {37, 0, 0, 0}},
    {.door = 16, .flags = 0, .opens = 0, .closes = 0, .answer = 3, .dialogues = {38, 0, 0, 0}},
    {.door = 17, .flags = 0, .opens = 0, .closes = 0, .answer = 3, .dialogues = {40, 0, 0, 0}},
    {.door = 18, .flags = 0, .opens = 0, .closes = 0, .answer = 3, .dialogues = {37, 0, 0, 0}},
    {.door = 19, .flags = 0, .opens = 0, .closes = 0, .answer = 3, .dialogues = {38, 0, 0, 0}},
    {.door = 20, .flags = 0, .opens = 0, .closes = 0, .answer = 3, .dialogues = {40, 0, 0, 0}},
    {.door = 21, .flags = 0, .opens = 0, .closes = 0, .answer = 3, .dialogues = {37, 0, 0, 0}},
    {.door = 22, .flags = 0, .opens = 0, .closes = 0, .answer = 3, .dialogues = {38, 0, 0, 0}},
    {.door = 23, .flags = 0, .opens = 0, .closes = 0, .answer = 3, .dialogues = {40, 0, 0, 0}},
    {.door = 24, .flags = 0, .opens = 0, .closes = 0, .answer = 3, .dialogues = {37, 0, 0, 0}}, {}}};
constexpr std::array<DoorAreaMetadata, 3> DOOR_METADATA_QF39{
    {{.door = 41, .flags = 1, .opens = 0, .closes = 2400, .answer = 2}, {.door = 40, .flags = 0, .opens = 0, .closes = 2400, .answer = 2}, {}}};
constexpr std::array<DoorAreaMetadata, 4> DOOR_METADATA_QF40{{{.door = 40, .flags = 1, .opens = 0, .closes = 0, .answer = 3, .dialogues = {40, 0, 0, 0}},
    {.door = 41, .flags = 0, .opens = 0, .closes = 0, .answer = 3, .dialogues = {40, 0, 0, 0}},
    {.door = 42, .flags = 1, .opens = 0, .closes = 2400, .answer = 2}, {}}};
constexpr std::array<DoorAreaMetadata, 2> DOOR_METADATA_QR00{
    {{.door = 1, .flags = 0, .opens = 0, .closes = 2400, .rules = DOOR_FLAG_RULES_QR00_01.data()}, {}}};
constexpr std::array<DoorAreaMetadata, 2> DOOR_METADATA_QRC0{{{.door = 54, .flags = 1, .opens = 0, .closes = 2400, .answer = 3}, {}}};
constexpr std::array<DoorAreaMetadata, 2> DOOR_METADATA_QRR0{
    {{.door = 1, .flags = 1, .opens = 0, .closes = 2400, .rules = DOOR_FLAG_RULES_QRR0_01.data()}, {}}};
constexpr std::array<DoorAreaMetadata, 8> DOOR_METADATA_QUG0{
    {{.door = 8, .flags = 0, .opens = 0, .closes = 2400, .answer = 2, .rules = DOOR_FLAG_RULES_QUG0_08.data(), .dialogues = {13, 14, 15, 0}},
        {.door = 9, .flags = 0, .opens = 0, .closes = 0, .answer = 2, .dialogues = {13, 14, 15, 0}},
        {.door = 5, .flags = 1, .opens = 0, .closes = 2400, .answer = 2, .rules = DOOR_FLAG_RULES_QUG0_05.data()},
        {.door = 6, .flags = 2, .opens = 0, .closes = 0, .answer = 2, .rules = DOOR_FLAG_RULES_QUG0_06.data(), .dialogues = {13, 14, 15, 0}},
        {.door = 7, .flags = 2, .opens = 0, .closes = 0, .answer = 2, .rules = DOOR_FLAG_RULES_QUG0_07.data(), .dialogues = {13, 14, 15, 0}},
        {.door = 22, .flags = 1, .opens = 0, .closes = 2400, .answer = 2}, {.door = 23, .flags = 1, .opens = 0, .closes = 2400, .answer = 2}, {}}};
constexpr std::array<DoorAreaMetadata, 3> DOOR_METADATA_KRH1{{{.door = 1, .flags = 1, .opens = 9000, .closes = 2300, .answer = 2, .dialogues = {6, 3, 7, 0}},
    {.door = 2, .flags = 0, .opens = 9000, .closes = 2300, .answer = 2, .dialogues = {6, 3, 7, 0}}, {}}};
constexpr std::array<DoorAreaMetadata, 3> DOOR_METADATA_KSH1{
    {{.door = 1, .flags = 0, .opens = 0, .closes = 2400, .answer = 2, .rules = DOOR_FLAG_RULES_KSH1_01.data()},
        {.door = 2, .flags = 2, .opens = 0, .closes = 0, .answer = 3, .dialogues = {3, 0, 0, 0}}, {}}};
constexpr std::array<DoorAreaMetadataMap, 31> DOOR_METADATA_ACTS_1_2{{{.map = SHEN_NAME(AB00), .entries = DOOR_METADATA_AB00.data()},
    {.map = SHEN_NAME(AK00), .entries = DOOR_METADATA_AK00.data()}, {.map = SHEN_NAME(AKA3), .entries = DOOR_METADATA_AKA3.data()},
    {.map = SHEN_NAME(AKS0), .entries = DOOR_METADATA_AKS0.data()}, {.map = SHEN_NAME(AKS1), .entries = DOOR_METADATA_AKS1.data()},
    {.map = SHEN_NAME(AKT0), .entries = DOOR_METADATA_AKT0.data()}, {.map = SHEN_NAME(AKT2), .entries = DOOR_METADATA_AKT2.data()},
    {.map = SHEN_NAME(AKT3), .entries = DOOR_METADATA_AKT3.data()}, {.map = SHEN_NAME(AKY0), .entries = DOOR_METADATA_AKY0.data()},
    {.map = SHEN_NAME(AR02), .entries = DOOR_METADATA_AR02.data()}, {.map = SHEN_NAME(AR03), .entries = DOOR_METADATA_AR03.data()},
    {.map = SHEN_NAME(ARA0), .entries = DOOR_METADATA_ARA0.data()}, {.map = SHEN_NAME(ARC0), .entries = DOOR_METADATA_ARC0.data()},
    {.map = SHEN_NAME(ARM0), .entries = DOOR_METADATA_ARM0.data()}, {.map = SHEN_NAME(ARZ0), .entries = DOOR_METADATA_ARZ0.data()},
    {.map = SHEN_NAME(WB00), .entries = DOOR_METADATA_WB00.data()}, {.map = SHEN_NAME(WE00), .entries = DOOR_METADATA_WE00.data()},
    {.map = SHEN_NAME(WECF), .entries = DOOR_METADATA_WECF.data()}, {.map = SHEN_NAME(WEG0), .entries = DOOR_METADATA_WEG0.data()},
    {.map = SHEN_NAME(WEM1), .entries = DOOR_METADATA_WEM1.data()}, {.map = SHEN_NAME(WES1), .entries = DOOR_METADATA_WES1.data()},
    {.map = SHEN_NAME(WESM), .entries = DOOR_METADATA_WESM.data()}, {.map = SHEN_NAME(WK00), .entries = DOOR_METADATA_WK00.data()},
    {.map = SHEN_NAME(WKA0), .entries = DOOR_METADATA_WKA0.data()}, {.map = SHEN_NAME(WN00), .entries = DOOR_METADATA_WN00.data()},
    {.map = SHEN_NAME(WR00), .entries = DOOR_METADATA_WR00.data()}, {.map = SHEN_NAME(WRS2), .entries = DOOR_METADATA_WRS2.data()},
    {.map = SHEN_NAME(WS00), .entries = DOOR_METADATA_WS00.data()}, {.map = SHEN_NAME(WSY0), .entries = DOOR_METADATA_WSY0.data()},
    {.map = SHEN_NAME(WT00), .entries = DOOR_METADATA_WT00.data()}, {.map = SHEN_NAME(WTA0), .entries = DOOR_METADATA_WTA0.data()}}};
constexpr std::array<DoorAreaMetadataMap, 31> DOOR_METADATA_ACT_3{{{.map = SHEN_NAME(Q100), .entries = DOOR_METADATA_Q100.data()},
    {.map = SHEN_NAME(Q300), .entries = DOOR_METADATA_Q300.data()}, {.map = SHEN_NAME(QA00), .entries = DOOR_METADATA_QA00.data()},
    {.map = SHEN_NAME(QA22), .entries = DOOR_METADATA_QA22.data()}, {.map = SHEN_NAME(QAE1), .entries = DOOR_METADATA_QAE1.data()},
    {.map = SHEN_NAME(QAW1), .entries = DOOR_METADATA_QAW1.data()}, {.map = SHEN_NAME(QAW6), .entries = DOOR_METADATA_QAW6.data()},
    {.map = SHEN_NAME(QB00), .entries = DOOR_METADATA_QB00.data()}, {.map = SHEN_NAME(QBAA), .entries = DOOR_METADATA_QBAA.data()},
    {.map = SHEN_NAME(QC00), .entries = DOOR_METADATA_QC00.data()}, {.map = SHEN_NAME(QC01), .entries = DOOR_METADATA_QC01.data()},
    {.map = SHEN_NAME(QC06), .entries = DOOR_METADATA_QC06.data()}, {.map = SHEN_NAME(QCAE), .entries = DOOR_METADATA_QCAE.data()},
    {.map = SHEN_NAME(QD00), .entries = DOOR_METADATA_QD00.data()}, {.map = SHEN_NAME(QD01), .entries = DOOR_METADATA_QD01.data()},
    {.map = SHEN_NAME(QDKJ), .entries = DOOR_METADATA_QDKJ.data()}, {.map = SHEN_NAME(QE00), .entries = DOOR_METADATA_QE00.data()},
    {.map = SHEN_NAME(QE01), .entries = DOOR_METADATA_QE01.data()}, {.map = SHEN_NAME(QE06), .entries = DOOR_METADATA_QE06.data()},
    {.map = SHEN_NAME(QEDJ), .entries = DOOR_METADATA_QEDJ.data()}, {.map = SHEN_NAME(QEHI), .entries = DOOR_METADATA_QEHI.data()},
    {.map = SHEN_NAME(QF00), .entries = DOOR_METADATA_QF00.data()}, {.map = SHEN_NAME(QF01), .entries = DOOR_METADATA_QF01.data()},
    {.map = SHEN_NAME(QF02), .entries = DOOR_METADATA_QF02.data()}, {.map = SHEN_NAME(QF06), .entries = DOOR_METADATA_QF06.data()},
    {.map = SHEN_NAME(QF39), .entries = DOOR_METADATA_QF39.data()}, {.map = SHEN_NAME(QF40), .entries = DOOR_METADATA_QF40.data()},
    {.map = SHEN_NAME(QR00), .entries = DOOR_METADATA_QR00.data()}, {.map = SHEN_NAME(QRC0), .entries = DOOR_METADATA_QRC0.data()},
    {.map = SHEN_NAME(QRR0), .entries = DOOR_METADATA_QRR0.data()}, {.map = SHEN_NAME(QUG0), .entries = DOOR_METADATA_QUG0.data()}}};
constexpr std::array<DoorAreaMetadataMap, 2> DOOR_METADATA_ACT_4{
    {{.map = SHEN_NAME(KRH1), .entries = DOOR_METADATA_KRH1.data()}, {.map = SHEN_NAME(KSH1), .entries = DOOR_METADATA_KSH1.data()}}};
