#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_pvsj.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void PvsjEntry(void *argument)
{
    AreaPVSJMain(argument);
}
#else
static shen_name pvsjOD1Points[] = {SHEN_NAME4('G', 'O', 'L', '1'), SHEN_NAME4('S', 'T', 'R', '1'), SHEN_NAME4('S', 'T', 'R', '2'),
    SHEN_NAME4('S', 'T', 'R', '3'), 0};
static PvsjCustomer pvsjOD1Customers[] = {
    {SHEN_NAME4('O', 'D', '1', '_'), SHEN_NAME4('G', 'O', 'L', '1'), 0, 0},
    {SHEN_NAME4('A', 'H', 'E', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 1},
    {SHEN_NAME4('S', 'Y', 'O', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 1},
    {SHEN_NAME4('A', 'D', '2', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 1},
    {SHEN_NAME4('C', 'H', 'W', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 1},
    {SHEN_NAME4('C', 'K', 'N', '_'), SHEN_NAME4('S', 'T', 'R', '3'), 0, 1},
    {SHEN_NAME4('S', 'S', 'A', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 1},
    {SHEN_NAME4('S', 'N', 'N', '_'), SHEN_NAME4('S', 'T', 'R', '3'), 0, 1},
    {SHEN_NAME4('S', 'Y', 'Y', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 1},
    {SHEN_NAME4('E', 'M', 'I', '_'), SHEN_NAME4('S', 'T', 'R', '3'), 0, 1},
    {SHEN_NAME4('K', 'O', 'O', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 2},
    {SHEN_NAME4('K', 'O', 'W', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 2},
    {SHEN_NAME4('Y', 'M', '1', '_'), SHEN_NAME4('S', 'T', 'R', '3'), 0, 2},
    {SHEN_NAME4('Y', 'M', '2', '_'), SHEN_NAME4('S', 'T', 'R', '3'), 0, 2},
    {0, 0, 0, 0},
};
static char pvsjOD1Greetings[] = {1, 1, 0, 3, 1, 2, 1, 1, 0, 0, 3, 0, 1, 0, 0, 2};
static shen_name pvsjKT4Points[] = {SHEN_NAME4('G', 'O', 'L', '1'), SHEN_NAME4('S', 'T', 'R', '1'), SHEN_NAME4('S', 'T', 'R', '2'),
    SHEN_NAME4('S', 'T', 'R', '3'), 0};
static PvsjCustomer pvsjKT4Customers[] = {
    {SHEN_NAME4('K', 'T', '4', '_'), SHEN_NAME4('G', 'O', 'L', '1'), 0, 0},
    {SHEN_NAME4('A', 'H', 'E', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 1},
    {SHEN_NAME4('S', 'Y', 'O', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 1},
    {SHEN_NAME4('A', 'D', '2', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 1},
    {SHEN_NAME4('C', 'H', 'W', '_'), SHEN_NAME4('S', 'T', 'R', '3'), 0, 1},
    {SHEN_NAME4('T', 'J', 'N', '_'), SHEN_NAME4('S', 'T', 'R', '3'), 0, 1},
    {SHEN_NAME4('C', 'K', 'N', '_'), SHEN_NAME4('S', 'T', 'R', '3'), 0, 1},
    {SHEN_NAME4('S', 'S', 'A', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 1},
    {SHEN_NAME4('S', 'N', 'N', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 1},
    {SHEN_NAME4('S', 'Y', 'Y', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 1},
    {SHEN_NAME4('K', 'O', 'O', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 2},
    {SHEN_NAME4('K', 'O', 'W', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 2},
    {SHEN_NAME4('Y', 'M', '1', '_'), SHEN_NAME4('S', 'T', 'R', '3'), 0, 2},
    {SHEN_NAME4('Y', 'M', '2', '_'), SHEN_NAME4('S', 'T', 'R', '3'), 0, 2},
    {0, 0, 0, 0},
};
static char pvsjKT4Greetings[] = {1, 2, 1, 3, 0, 2, 1, 0, 2, 1, 1, 0, 2, 0, 3, 1};
static shen_name pvsjSNSPoints[] = {SHEN_NAME4('G', 'O', 'L', '3'), SHEN_NAME4('S', 'T', 'R', '3'), SHEN_NAME4('S', 'T', 'R', '4'),
    SHEN_NAME4('S', 'T', 'R', '5'), 0};
static PvsjCustomer pvsjSNSCustomers[] = {
    {SHEN_NAME4('S', 'N', 'S', '_'), SHEN_NAME4('G', 'O', 'L', '3'), 0, 0},
    {SHEN_NAME4('A', 'H', 'E', '_'), SHEN_NAME4('S', 'T', 'R', '4'), 0, 1},
    {SHEN_NAME4('S', 'Y', 'O', '_'), SHEN_NAME4('S', 'T', 'R', '4'), 0, 1},
    {SHEN_NAME4('A', 'D', '2', '_'), SHEN_NAME4('S', 'T', 'R', '4'), 0, 1},
    {SHEN_NAME4('C', 'H', 'W', '_'), SHEN_NAME4('S', 'T', 'R', '4'), 0, 1},
    {SHEN_NAME4('T', 'J', 'N', '_'), SHEN_NAME4('S', 'T', 'R', '5'), 0, 1},
    {SHEN_NAME4('C', 'K', 'N', '_'), SHEN_NAME4('S', 'T', 'R', '5'), 0, 1},
    {SHEN_NAME4('S', 'S', 'A', '_'), SHEN_NAME4('S', 'T', 'R', '3'), 0, 1},
    {SHEN_NAME4('S', 'N', 'N', '_'), SHEN_NAME4('S', 'T', 'R', '3'), 0, 1},
    {SHEN_NAME4('S', 'Y', 'Y', '_'), SHEN_NAME4('S', 'T', 'R', '3'), 0, 1},
    {SHEN_NAME4('E', 'M', 'I', '_'), SHEN_NAME4('S', 'T', 'R', '5'), 0, 1},
    {SHEN_NAME4('K', 'O', 'O', '_'), SHEN_NAME4('S', 'T', 'R', '5'), 0, 2},
    {SHEN_NAME4('K', 'O', 'W', '_'), SHEN_NAME4('S', 'T', 'R', '4'), 0, 2},
    {SHEN_NAME4('Y', 'M', '1', '_'), SHEN_NAME4('S', 'T', 'R', '3'), 0, 2},
    {SHEN_NAME4('Y', 'M', '2', '_'), SHEN_NAME4('S', 'T', 'R', '3'), 0, 2},
    {0, 0, 0, 0},
};
static char pvsjSNSGreetings[] = {0, 1, 0, 2, 1, 1, 0, 1, 3, 1, 0, 1, 2, 0, 0, 1};
static shen_name pvsjEINPoints[] = {SHEN_NAME4('G', 'O', 'L', '6'), SHEN_NAME4('S', 'T', 'R', '6'), SHEN_NAME4('S', 'T', 'R', '7'),
    SHEN_NAME4('S', 'T', 'R', '8'), 0};
static PvsjCustomer pvsjEINCustomers[] = {
    {SHEN_NAME4('E', 'I', 'N', '_'), SHEN_NAME4('G', 'O', 'L', '6'), 0, 0},
    {SHEN_NAME4('A', 'H', 'E', '_'), SHEN_NAME4('S', 'T', 'R', '8'), 0, 1},
    {SHEN_NAME4('S', 'Y', 'O', '_'), SHEN_NAME4('S', 'T', 'R', '6'), 0, 1},
    {SHEN_NAME4('A', 'D', '2', '_'), SHEN_NAME4('S', 'T', 'R', '8'), 0, 1},
    {SHEN_NAME4('C', 'H', 'W', '_'), SHEN_NAME4('S', 'T', 'R', '6'), 0, 1},
    {SHEN_NAME4('T', 'J', 'N', '_'), SHEN_NAME4('S', 'T', 'R', '6'), 0, 1},
    {SHEN_NAME4('S', 'S', 'A', '_'), SHEN_NAME4('S', 'T', 'R', '6'), 0, 1},
    {SHEN_NAME4('S', 'N', 'N', '_'), SHEN_NAME4('S', 'T', 'R', '6'), 0, 1},
    {SHEN_NAME4('S', 'Y', 'Y', '_'), SHEN_NAME4('S', 'T', 'R', '6'), 0, 1},
    {SHEN_NAME4('E', 'M', 'I', '_'), SHEN_NAME4('S', 'T', 'R', '8'), 0, 1},
    {SHEN_NAME4('K', 'O', 'O', '_'), SHEN_NAME4('S', 'T', 'R', '8'), 0, 2},
    {SHEN_NAME4('K', 'O', 'W', '_'), SHEN_NAME4('S', 'T', 'R', '6'), 0, 2},
    {SHEN_NAME4('Y', 'M', '1', '_'), SHEN_NAME4('S', 'T', 'R', '7'), 0, 2},
    {SHEN_NAME4('Y', 'M', '2', '_'), SHEN_NAME4('S', 'T', 'R', '7'), 0, 2},
    {0, 0, 0, 0},
};
static char pvsjEINGreetings[] = {3, 0, 1, 1, 2, 1, 2, 1, 3, 0, 1, 0, 1, 1, 2, 1};
static shen_name pvsjGKSPoints[] = {SHEN_NAME4('G', 'O', 'L', '1'), SHEN_NAME4('S', 'T', 'R', '1'), SHEN_NAME4('S', 'T', 'R', '2'), 0};
static PvsjCustomer pvsjGKSCustomers[] = {
    {SHEN_NAME4('G', 'K', 'S', '_'), SHEN_NAME4('G', 'O', 'L', '1'), 0, 0},
    {SHEN_NAME4('A', 'H', 'E', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 1},
    {SHEN_NAME4('S', 'Y', 'O', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 1},
    {SHEN_NAME4('A', 'D', '2', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 1},
    {SHEN_NAME4('C', 'H', 'W', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 1},
    {SHEN_NAME4('T', 'J', 'N', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 1},
    {SHEN_NAME4('C', 'K', 'N', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 1},
    {SHEN_NAME4('S', 'S', 'A', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 1},
    {SHEN_NAME4('S', 'N', 'N', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 1},
    {SHEN_NAME4('S', 'Y', 'Y', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 1},
    {SHEN_NAME4('E', 'M', 'I', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 1},
    {SHEN_NAME4('K', 'O', 'O', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 2},
    {SHEN_NAME4('K', 'O', 'W', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 2},
    {SHEN_NAME4('Y', 'M', '1', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 2},
    {SHEN_NAME4('Y', 'M', '2', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 2},
    {0, 0, 0, 0},
};
static char pvsjGKSGreetings[] = {2, 1, 1, 2, 1, 0, 3, 1, 0, 1, 2, 1, 1, 2, 3, 0};
static shen_name pvsjKNTPoints[] = {SHEN_NAME4('G', 'O', 'L', '1'), SHEN_NAME4('S', 'T', 'R', '1'), SHEN_NAME4('S', 'T', 'R', '2'),
    SHEN_NAME4('S', 'T', 'R', '3'), 0};
static PvsjCustomer pvsjKNTCustomers[] = {
    {SHEN_NAME4('K', 'N', 'T', '_'), SHEN_NAME4('G', 'O', 'L', '1'), 0, 0},
    {SHEN_NAME4('S', 'Y', 'O', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 1},
    {SHEN_NAME4('C', 'H', 'W', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 1},
    {SHEN_NAME4('T', 'J', 'N', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 1},
    {SHEN_NAME4('C', 'K', 'N', '_'), SHEN_NAME4('S', 'T', 'R', '3'), 0, 1},
    {SHEN_NAME4('S', 'S', 'A', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 1},
    {SHEN_NAME4('S', 'N', 'N', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 1},
    {SHEN_NAME4('S', 'Y', 'Y', '_'), SHEN_NAME4('S', 'T', 'R', '3'), 0, 1},
    {SHEN_NAME4('E', 'M', 'I', '_'), SHEN_NAME4('S', 'T', 'R', '3'), 0, 1},
    {SHEN_NAME4('K', 'O', 'O', '_'), SHEN_NAME4('S', 'T', 'R', '3'), 0, 2},
    {SHEN_NAME4('K', 'O', 'W', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 2},
    {SHEN_NAME4('Y', 'M', '1', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 2},
    {SHEN_NAME4('Y', 'M', '2', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 2},
    {0, 0, 0, 0},
};
static char pvsjKNTGreetings[] = {0, 1, 1, 0, 1, 2, 0, 1, 3, 0, 1, 0, 2, 0, 0, 1};
static shen_name pvsjKU4Points[] = {SHEN_NAME4('G', 'O', 'L', '1'), SHEN_NAME4('S', 'T', 'R', '1'), SHEN_NAME4('S', 'T', 'R', '2'),
    SHEN_NAME4('S', 'T', 'R', '3'), 0};
static PvsjCustomer pvsjKU4Customers[] = {
    {SHEN_NAME4('K', 'U', '4', '_'), SHEN_NAME4('G', 'O', 'L', '1'), 0, 0},
    {SHEN_NAME4('M', 'E', 'J', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 1},
    {SHEN_NAME4('T', 'S', 'Y', '_'), SHEN_NAME4('S', 'T', 'R', '3'), 0, 1},
    {SHEN_NAME4('K', 'D', '4', '_'), SHEN_NAME4('S', 'T', 'R', '3'), 0, 1},
    {SHEN_NAME4('S', 'D', '7', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 1},
    {SHEN_NAME4('K', 'K', 'K', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 1},
    {SHEN_NAME4('W', 'D', '1', '_'), SHEN_NAME4('S', 'T', 'R', '3'), 0, 1},
    {SHEN_NAME4('K', 'D', '3', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 1},
    {SHEN_NAME4('A', 'W', '6', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 1},
    {SHEN_NAME4('I', 'L', 'N', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 1},
    {SHEN_NAME4('K', 'O', 'O', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 2},
    {SHEN_NAME4('K', 'O', 'W', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 2},
    {SHEN_NAME4('Y', 'M', '1', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 2},
    {SHEN_NAME4('Y', 'M', '2', '_'), SHEN_NAME4('S', 'T', 'R', '3'), 0, 2},
    {0, 0, 0, 0},
};
static char pvsjKU4Greetings[] = {3, 1, 0, 1, 2, 0, 2, 1, 3, 0, 1, 1, 2, 1, 0, 1};
static shen_name pvsjMFGPoints[] = {SHEN_NAME4('G', 'O', 'L', '1'), SHEN_NAME4('S', 'T', 'R', '1'), SHEN_NAME4('S', 'T', 'R', '2'), 0};
static PvsjCustomer pvsjMFGCustomers[] = {
    {SHEN_NAME4('M', 'F', 'G', '_'), SHEN_NAME4('G', 'O', 'L', '1'), 0, 0},
    {SHEN_NAME4('M', 'E', 'J', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 1},
    {SHEN_NAME4('T', 'S', 'Y', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 1},
    {SHEN_NAME4('K', 'D', '4', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 1},
    {SHEN_NAME4('S', 'D', '7', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 1},
    {SHEN_NAME4('K', 'K', 'K', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 1},
    {SHEN_NAME4('W', 'D', '1', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 1},
    {SHEN_NAME4('K', 'D', '3', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 1},
    {SHEN_NAME4('A', 'W', '6', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 1},
    {SHEN_NAME4('I', 'L', 'N', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 1},
    {SHEN_NAME4('K', 'O', 'O', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 2},
    {SHEN_NAME4('K', 'O', 'W', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 2},
    {SHEN_NAME4('Y', 'M', '1', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 2},
    {SHEN_NAME4('Y', 'M', '2', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 2},
    {0, 0, 0, 0},
};
static char pvsjMFGGreetings[] = {3, 0, 0, 1, 0, 1, 2, 0, 1, 1, 0, 2, 0, 0, 1, 1};
static shen_name pvsjSD2Points[] = {SHEN_NAME4('G', 'O', 'L', '1'), SHEN_NAME4('S', 'T', 'R', '1'), SHEN_NAME4('S', 'T', 'R', '2'), 0};
static PvsjCustomer pvsjSD2Customers[] = {
    {SHEN_NAME4('S', 'D', '2', '_'), SHEN_NAME4('G', 'O', 'L', '1'), 0, 0},
    {SHEN_NAME4('M', 'E', 'J', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 1},
    {SHEN_NAME4('T', 'S', 'Y', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 1},
    {SHEN_NAME4('K', 'D', '4', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 1},
    {SHEN_NAME4('S', 'D', '7', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 1},
    {SHEN_NAME4('K', 'K', 'K', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 1},
    {SHEN_NAME4('W', 'D', '1', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 1},
    {SHEN_NAME4('K', 'D', '3', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 1},
    {SHEN_NAME4('A', 'W', '6', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 1},
    {SHEN_NAME4('I', 'L', 'N', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 1},
    {SHEN_NAME4('K', 'O', 'O', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 2},
    {SHEN_NAME4('K', 'O', 'W', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 2},
    {SHEN_NAME4('Y', 'M', '1', '_'), SHEN_NAME4('S', 'T', 'R', '2'), 0, 2},
    {SHEN_NAME4('Y', 'M', '2', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 2},
    {0, 0, 0, 0},
};
static char pvsjSD2Greetings[] = {2, 1, 3, 1, 1, 1, 2, 0, 2, 1, 1, 1, 3, 1, 2, 0};
static shen_name pvsjCHXPoints[] = {SHEN_NAME4('G', 'O', 'L', '1'), SHEN_NAME4('S', 'T', 'R', '1'), 0};
static PvsjCustomer pvsjCHXCustomers[] = {
    {SHEN_NAME4('C', 'H', 'X', '_'), SHEN_NAME4('G', 'O', 'L', '1'), 0, 0},
    {SHEN_NAME4('M', 'E', 'J', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 1},
    {SHEN_NAME4('S', 'D', '7', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 1},
    {SHEN_NAME4('K', 'K', 'K', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 1},
    {SHEN_NAME4('W', 'D', '1', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 1},
    {SHEN_NAME4('K', 'D', '3', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 1},
    {SHEN_NAME4('A', 'W', '6', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 1},
    {SHEN_NAME4('I', 'L', 'N', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 1},
    {SHEN_NAME4('K', 'O', 'O', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 2},
    {SHEN_NAME4('K', 'O', 'W', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 2},
    {SHEN_NAME4('Y', 'M', '1', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 2},
    {SHEN_NAME4('Y', 'M', '2', '_'), SHEN_NAME4('S', 'T', 'R', '1'), 0, 2},
    {0, 0, 0, 0},
};
static char pvsjCHXGreetings[] = {0, 1, 1, 3, 0, 1, 2, 1, 1, 3, 1, 2, 1, 0, 1, 2};
static int pvsjMfgMotions[] = {6036, 6038, 6034, 6035, 6033, 6037, 6040, 6042, 6041, 6043, 6044, 6039, 6046, 6055, 6045, 6052, 6054, 6047};
static int pvsjMotions[] = {6051, 6059, 6057, 6058, 6056, 6049, 6040, 6042, 6041, 6043, 6044, 6039, 6046, 6055, 6045, 6052, 6054, 6047};
PvsjBoardGroup pvsjBoardGroups[] = {
    {SHEN_NAME4('A', 'R', '0', '9'), SHEN_NAME4('O', 'D', '1', '_'), pvsjOD1Points, pvsjOD1Customers, pvsjOD1Greetings, 0x8000, pvsjMotions},
    {SHEN_NAME4('A', 'K', '0', '9'), SHEN_NAME4('K', 'N', 'T', '_'), pvsjKNTPoints, pvsjKNTCustomers, pvsjKNTGreetings, 0x8000, pvsjMotions},
    {SHEN_NAME4('W', 'S', '0', '9'), SHEN_NAME4('K', 'T', '4', '_'), pvsjKT4Points, pvsjKT4Customers, pvsjKT4Greetings, 0x0, pvsjMotions},
    {SHEN_NAME4('W', 'K', '0', '9'), SHEN_NAME4('S', 'N', 'S', '_'), pvsjSNSPoints, pvsjSNSCustomers, pvsjSNSGreetings, 0x9000, pvsjMotions},
    {SHEN_NAME4('W', 'K', '0', '9'), SHEN_NAME4('E', 'I', 'N', '_'), pvsjEINPoints, pvsjEINCustomers, pvsjEINGreetings, 0x0, pvsjMotions},
    {SHEN_NAME4('W', 'K', '0', '9'), SHEN_NAME4('G', 'K', 'S', '_'), pvsjGKSPoints, pvsjGKSCustomers, pvsjGKSGreetings, 0x2000, pvsjMotions},
    {SHEN_NAME4('Q', '1', '0', '9'), SHEN_NAME4('K', 'U', '4', '_'), pvsjKU4Points, pvsjKU4Customers, pvsjKU4Greetings, 0x8000, pvsjMotions},
    {SHEN_NAME4('Q', 'R', '0', '9'), SHEN_NAME4('M', 'F', 'G', '_'), pvsjMFGPoints, pvsjMFGCustomers, pvsjMFGGreetings, 0xf000, pvsjMfgMotions},
    {SHEN_NAME4('Q', 'E', '0', '9'), SHEN_NAME4('S', 'D', '2', '_'), pvsjSD2Points, pvsjSD2Customers, pvsjSD2Greetings, 0x4000, pvsjMotions},
    {SHEN_NAME4('Q', 'T', '0', '9'), SHEN_NAME4('C', 'H', 'X', '_'), pvsjCHXPoints, pvsjCHXCustomers, pvsjCHXGreetings, 0x9000, pvsjMotions},
    {0, 0, 0, 0, 0, 0, 0},
};
int pvsjCustomerModelIndex = 0;
PvsjCustomerPartition pvsjCustomerPartitions[] = {
    {SHEN_NAME4('A', 'D', '2', '_'), 0, 1, 0},
    {SHEN_NAME4('A', 'H', 'E', '_'), 2, 3, 1},
    {SHEN_NAME4('A', 'W', '6', '_'), 4, 5, 0},
    {SHEN_NAME4('C', 'H', 'W', '_'), 6, 7, 0},
    {SHEN_NAME4('C', 'K', 'N', '_'), 8, 9, 0},
    {SHEN_NAME4('E', 'M', 'I', '_'), 10, 11, 1},
    {SHEN_NAME4('I', 'L', 'N', '_'), 12, 13, 1},
    {SHEN_NAME4('K', 'D', '3', '_'), 14, 15, 1},
    {SHEN_NAME4('K', 'D', '4', '_'), 16, 17, 0},
    {SHEN_NAME4('K', 'K', 'K', '_'), 18, 19, 1},
    {SHEN_NAME4('M', 'E', 'J', '_'), 20, 21, 1},
    {SHEN_NAME4('S', 'D', '7', '_'), 22, 23, 0},
    {SHEN_NAME4('S', 'N', 'N', '_'), 24, 25, 0},
    {SHEN_NAME4('S', 'S', 'A', '_'), 26, 27, 1},
    {SHEN_NAME4('S', 'Y', 'O', '_'), 28, 29, 0},
    {SHEN_NAME4('S', 'Y', 'Y', '_'), 30, 31, 0},
    {SHEN_NAME4('T', 'J', 'N', '_'), 32, 33, 0},
    {SHEN_NAME4('T', 'S', 'Y', '_'), 36, 37, 1},
    {SHEN_NAME4('W', 'D', '1', '_'), 38, 39, 0},
    {0, 0, 0, 0},
};
PvsjVoicePartition pvsjVoicePartitions[] = {{"0104", 0, 1}, {"0104AA1", 2, 3}, {"0104AD2", 4, 5}, {"0104AHE", 6, 7}, {"0104ANS", 8, 9},
    {"0104AW6", 10, 11}, {"0104CHW", 12, 13}, {"0104CHX", 14, 15}, {"0104CKN", 16, 17}, {"0104CN6", 18, 19}, {"0104DHO", 20, 21}, {"0104DM1", 22, 23},
    {"0104DM2", 24, 25}, {"0104DS4", 26, 27}, {"0104DY1", 28, 29}, {"0104EIN", 30, 31}, {"0104EMI", 32, 33}, {"0104FD1", 34, 35}, {"0104GKS", 36, 37},
    {"0104HAK", 38, 39}, {"0104ILN", 40, 41}, {"0104JI8", 42, 43}, {"0104JOE", 44, 45}, {"0104KD3", 46, 47}, {"0104KD4", 48, 49}, {"0104KKK", 50, 51},
    {"0104KNH", 52, 53}, {"0104KNT", 54, 55}, {"0104KOO", 56, 57}, {"0104KOW", 58, 59}, {"0104KT4", 60, 61}, {"0104KU4", 62, 63}, {"0104LYO", 64, 65},
    {"0104MEJ", 66, 67}, {"0104MFG", 68, 69}, {"0104OD1", 70, 71}, {"0104PG5", 72, 73}, {"0104RKU", 74, 75}, {"0104SCN", 76, 77}, {"0104SD2", 78, 79},
    {"0104SD7", 80, 81}, {"0104SF7", 82, 83}, {"0104SNN", 84, 85}, {"0104SNS", 86, 87}, {"0104SOG", 88, 89}, {"0104SSA", 90, 91}, {"0104SYO", 92, 93},
    {"0104SYY", 94, 95}, {"0104TJN", 96, 97}, {"0104TNJ", 98, 99}, {"0104TSY", 100, 101}, {"0104WD1", 102, 103}, {"0104WO1", 104, 105}, {"0104WO3", 106, 107},
    {"0104YM1", 108, 109}, {"0104YM2", 110, 111}, {"0104ZKS", 112, 113}, {0, 0, 1}};
const PvsjDealer pvsjDealers[] = {
    {SHEN_NAME4('O', 'D', '1', '_'), 1138, pvsjMotions, 931},
    {SHEN_NAME4('D', 'Y', '1', '_'), 1135, pvsjMotions, 911},
    {SHEN_NAME4('S', 'O', 'G', '_'), 1136, pvsjMotions, 931},
    {SHEN_NAME4('K', 'N', 'T', '_'), 1137, pvsjMotions, 911},
    {SHEN_NAME4('C', 'N', '6', '_'), 1168, pvsjMotions, 911},
    {SHEN_NAME4('K', 'T', '4', '_'), 1169, pvsjMotions, 931},
    {SHEN_NAME4('J', 'O', 'E', '_'), 1170, pvsjMotions, 931},
    {SHEN_NAME4('S', 'C', 'N', '_'), 1172, pvsjMotions, 931},
    {SHEN_NAME4('S', 'N', 'S', '_'), 1071, pvsjMotions, 921},
    {SHEN_NAME4('A', 'A', '1', '_'), 1159, pvsjMotions, 931},
    {SHEN_NAME4('H', 'A', 'K', '_'), 1173, pvsjMotions, 911},
    {SHEN_NAME4('D', 'H', 'O', '_'), 1174, pvsjMotions, 931},
    {SHEN_NAME4('L', 'Y', 'O', '_'), 1160, pvsjMotions, 911},
    {SHEN_NAME4('A', 'N', 'S', '_'), 1171, pvsjMotions, 931},
    {SHEN_NAME4('E', 'I', 'N', '_'), 1175, pvsjMotions, 931},
    {SHEN_NAME4('K', 'N', 'H', '_'), 1176, pvsjMotions, 931},
    {SHEN_NAME4('R', 'K', 'U', '_'), 1177, pvsjMotions, 921},
    {SHEN_NAME4('G', 'K', 'S', '_'), 1165, pvsjMotions, 931},
    {SHEN_NAME4('C', 'H', 'X', '_'), 940, pvsjMotions, 921},
    {SHEN_NAME4('D', 'M', '1', '_'), 941, pvsjMotions, 921},
    {SHEN_NAME4('D', 'M', '2', '_'), 942, pvsjMotions, 931},
    {SHEN_NAME4('D', 'S', '4', '_'), 943, pvsjMotions, 931},
    {SHEN_NAME4('F', 'D', '1', '_'), 944, pvsjMotions, 931},
    {SHEN_NAME4('J', 'I', '8', '_'), 945, pvsjMotions, 921},
    {SHEN_NAME4('K', 'U', '4', '_'), 946, pvsjMotions, 911},
    {SHEN_NAME4('M', 'F', 'G', '_'), 1386, pvsjMotions, 921},
    {SHEN_NAME4('O', 'D', '3', '_'), 947, pvsjMotions, 931},
    {SHEN_NAME4('P', 'G', '5', '_'), 1325, pvsjMotions, 921},
    {SHEN_NAME4('S', 'D', '2', '_'), 948, pvsjMotions, 931},
    {SHEN_NAME4('S', 'F', '7', '_'), 1324, pvsjMotions, 911},
    {SHEN_NAME4('T', 'N', 'J', '_'), 949, pvsjMotions, 931},
    {SHEN_NAME4('W', 'O', '1', '_'), 452, pvsjMotions, 931},
    {SHEN_NAME4('W', 'O', '3', '_'), 454, pvsjMotions, 931},
    {SHEN_NAME4('A', 'H', 'E', '_'), -1, pvsjMotions, 921},
    {SHEN_NAME4('S', 'Y', 'O', '_'), -1, pvsjMotions, 931},
    {SHEN_NAME4('A', 'D', '2', '_'), -1, pvsjMotions, 931},
    {SHEN_NAME4('C', 'H', 'W', '_'), -1, pvsjMotions, 931},
    {SHEN_NAME4('T', 'J', 'N', '_'), -1, pvsjMotions, 931},
    {SHEN_NAME4('C', 'K', 'N', '_'), -1, pvsjMotions, 931},
    {SHEN_NAME4('S', 'S', 'A', '_'), -1, pvsjMotions, 921},
    {SHEN_NAME4('S', 'N', 'N', '_'), -1, pvsjMotions, 931},
    {SHEN_NAME4('S', 'Y', 'Y', '_'), -1, pvsjMotions, 931},
    {SHEN_NAME4('E', 'M', 'I', '_'), -1, pvsjMotions, 921},
    {SHEN_NAME4('M', 'E', 'J', '_'), -1, pvsjMotions, 921},
    {SHEN_NAME4('T', 'S', 'Y', '_'), -1, pvsjMotions, 921},
    {SHEN_NAME4('K', 'D', '4', '_'), -1, pvsjMotions, 921},
    {SHEN_NAME4('S', 'D', '7', '_'), -1, pvsjMotions, 931},
    {SHEN_NAME4('K', 'K', 'K', '_'), -1, pvsjMotions, 921},
    {SHEN_NAME4('W', 'D', '1', '_'), -1, pvsjMotions, 931},
    {SHEN_NAME4('K', 'D', '3', '_'), -1, pvsjMotions, 921},
    {SHEN_NAME4('A', 'W', '6', '_'), -1, pvsjMotions, 911},
    {SHEN_NAME4('I', 'L', 'N', '_'), -1, pvsjMotions, 921},
    {SHEN_NAME4('T', 'S', 'O', '_'), -1, pvsjMotions, 921},
    {SHEN_NAME4('K', 'O', 'O', '_'), -1, pvsjMotions, 931},
    {SHEN_NAME4('K', 'O', 'W', '_'), -1, pvsjMotions, 911},
    {SHEN_NAME4('Y', 'M', '1', '_'), -1, pvsjMotions, 931},
    {SHEN_NAME4('Y', 'M', '2', '_'), -1, pvsjMotions, 931},
    {0, -1, 0, 0},
};
static PvsjVoiceMessage pvsjVoiceMessages[] = {{0, 0}};
static int pvsjBoardCount;
static PvsjBoardPosition pvsjBoardPositions[32];
static char pvsjGameArgument[24];
static int pvsjPlayerCamera, pvsjCustomerCamera;
static Vector3 pvsjFilteredEye, pvsjFilteredTarget, pvsjDesiredEye, pvsjDesiredTarget;
static Vector3i pvsjSavedPlayerRotation;
static short pvsjCameraBlend;
static int g_patiCameraTime;
static int pvsjSkipMotion;
PatiMotionCacheEntry g_patiMotionCache[4];

void PvsjReleaseVoice(PvsjEventWork *work)
{
    if (work->voicePrefix[0]) {
        FTBL.EV_FreeAuthorPak(0, work->voicePrefix);
        FTBL.DebugLogToFileRemoved("PATI_FREE_VOICE_FILE:%s\n", work->voicePrefix);
        work->voicePrefix[0] = 0;
    }
}

void PvsjLoadVoice(PvsjEventWork *work, shen_name character)
{
    int i;
    void *source;
    int size;
    sprintf(work->voicePrefix, "0104%c%c%c", (unsigned char)character, (unsigned char)(character >> 8), (unsigned char)(character >> 16));
    for (i = 0; pvsjVoicePartitions[i].name && strcmp(pvsjVoicePartitions[i].name, work->voicePrefix); i++)
        ;
    if (!pvsjVoicePartitions[i].name) {
        FTBL.DebugLogToFileRemoved("NoVoiceFile:%s!!\n", work->voicePrefix);
        FTBL.EventAssertNullsub(__FILE__, 1034);
        sprintf(work->voicePrefix, "0104");
    }
    FTBL.DebugLogToFileRemoved("EV_PAKLOAD_AFS [PATI] %i,%i,%i\n", work->gamePartition, pvsjVoicePartitions[i].text, pvsjVoicePartitions[i].sound);
    FTBL.EV_ControlPackageResources(5, (int)work->voiceLoadBuffer, work->gamePartition, pvsjVoicePartitions[i].text, pvsjVoicePartitions[i].sound);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 5, work->gamePartition, pvsjVoicePartitions[i].text, pvsjVoicePartitions[i].sound);
    source = FTBL.LoadCurrentPakBinary(work->voicePrefix);
    size = FTBL.GetCurrentPakBinarySize(work->voicePrefix) / 4 * 4;
    FTBL.DebugLogToFileRemoved("EV_PAK [PATI] %08x,%08x,%08x->", work->gamePartition, source, size);
    if (size >= 0x4000) {
        FTBL.DebugLogToFileRemoved("SIZE OVER\n");
        FTBL.EventAssertNullsub(__FILE__, 1051);
    }
    FTBL.DebugLogToFileRemoved("%08x\n", work->voiceLengths);
    memcpy(work->voiceLengths, source, size);
    FTBL.DebugLogToFileRemoved("EV_FREE_PAKFILE_AFS [PATI] %i,%i,%i\n", work->gamePartition, pvsjVoicePartitions[i].text, pvsjVoicePartitions[i].sound);
    FTBL.EV_ControlPackageResources(0, 12, work->gamePartition, pvsjVoicePartitions[i].text, pvsjVoicePartitions[i].sound);
    FTBL.DebugLogToFileRemoved("PATI_VOICE_NAME:%s\n", work->voicePrefix);
    FTBL.EV_FreeAuthorPak(1, work->voicePrefix);
    FTBL.EV_ControlPackageResources(2, 2, "PATI", 0, 0);
    FTBL.EV_ControlPackageResources(2, 4, "PATI", 0, 0);
}

void SetPatiVoice(const char *prefix, const char *code, int delay)
{
    char voice[32];
    sprintf(voice, "%s%s", prefix, code);
    FTBL.DebugLogToFileRemoved("VoiceCode:%s\n", voice);
    EVT_AT_SetVoice(voice, delay);
}

void PvsjQueueVoice(PvsjEventWork *work, const char *code, int *frame)
{
    shen_name name;
    int i, duration;
    name = PVSJ_READ_NAME(code);
    i = 0;
    while (work->voiceLengths[i].code && work->voiceLengths[i].code != name)
        i++;
    duration = work->voiceLengths[i].frames;
    if (work->voiceLengths[i].code) {
        SetPatiVoice(work->voicePrefix, code, *frame);
        *frame += 10 + duration;
        FTBL.DebugLogToFileRemoved("VL:%i\n", duration);
    } else {
        FTBL.DebugLogToFileRemoved("PVSJ:No VoiceID [%s%s]\n", work->voicePrefix, code);
        i = 0;
        while (pvsjVoiceMessages[i].code && pvsjVoiceMessages[i].code != name)
            i++;
        if (pvsjVoiceMessages[i].code) {
            *frame += 20;
            EVT_RepeatCall(ShowPatiVoiceMessage, (void *)pvsjVoiceMessages[i].text, *frame, 1);
            *frame += 70;
        } else
            FTBL.EventAssertNullsub(__FILE__, 1171);
    }
}

void PvsjOfferJob(PvsjEventWork *work)
{
    int start = work->delay;
    EVT_AT_SetFaceLooks(PVSJ_PLAYER, 0, 10, 1.0f, 0);
    EVT_AT_SetFaceLooks(work->owner, 0, 10, 1.0f, 0);
    PvsjSetCamera(pvsjCustomerCamera, work->owner, work->owner, work->delay, work);
    PvsjSetMotion(work->owner, work->boards->motions[0], 0.0f, 1.0f, 0, work->delay);
    PvsjSetMotion(PVSJ_PLAYER, work->boards->motions[6], 0.0f, 1.0f, 0, work->delay + 25);
    PvsjQueueVoice(work, "B094", &work->delay);
    PvsjQueueVoice(work, "B095", &work->delay);
    PvsjSetCamera(pvsjPlayerCamera, PVSJ_PLAYER, PVSJ_PLAYER, work->delay, work);
    PvsjQueueVoice(work, "A035", &work->delay);
    if (work->delay - start < 250)
        start = work->delay;
    else
        start += 250;
    PvsjSetMotion(work->owner, work->boards->motions[5], 0.0f, 1.0f, 0, start);
    PvsjSetMotion(PVSJ_PLAYER, work->boards->motions[7], 0.0f, 1.0f, 0, start);
}

void PvsjBeginShift(PvsjEventWork *work)
{
    EVT_AT_SetFaceLooks(PVSJ_PLAYER, 0, 10, 1.0f, 0);
    EVT_AT_SetFaceLooks(work->owner, 0, 10, 1.0f, 0);
    PvsjSetCamera(pvsjCustomerCamera, work->owner, work->owner, work->delay, work);
    switch ((unsigned short)FTBL.shenGetRandomInt32(2)) {
    case 0:
    default:
        PvsjQueueVoice(work, "B096", &work->delay);
        break;
    case 1:
        PvsjQueueVoice(work, "B097", &work->delay);
        break;
    }
    PvsjSetCamera(pvsjPlayerCamera, PVSJ_PLAYER, PVSJ_PLAYER, work->delay, work);
    PvsjQueueVoice(work, "A036", &work->delay);
    PvsjSetMotion(work->owner, work->boards->motions[5], 0.0f, 1.0f, 0, work->delay);
    PvsjSetMotion(PVSJ_PLAYER, work->boards->motions[7], 0.0f, 1.0f, 0, work->delay);
}

void PvsjCallCustomer(PvsjEventWork *work)
{
    EVT_AT_SetFaceLooks(PVSJ_PLAYER, 0, 10, 1.0f, 0);
    if (work->callFrames++ & 1)
        PvsjQueueVoice(work, "A042", &work->delay);
    else {
        switch ((unsigned short)FTBL.shenGetRandomInt32(2)) {
        case 0:
        default:
            PvsjQueueVoice(work, "A043", &work->delay);
            break;
        case 1:
            PvsjQueueVoice(work, "A044", &work->delay);
            break;
        case 2:
            PvsjQueueVoice(work, "A045", &work->delay);
            break;
        }
    }
}

void PvsjWelcomeCustomer(PvsjEventWork *work)
{
    EVT_AT_SetFaceLooks(PVSJ_PLAYER, 0, 10, 1.0f, 0);
    EVT_AT_SetFaceLooks(work->customer, 0, 10, 1.0f, 0);
    PvsjSetCamera(pvsjPlayerCamera + 1, PVSJ_PLAYER, PVSJ_PLAYER, work->delay, work);
    PvsjSetMotion(work->customer, work->boards->motions[12], 0.0f, 1.0f, 0, work->delay);
    PvsjSetMotion(PVSJ_PLAYER, work->boards->motions[7], 0.0f, 1.0f, 0, work->delay);
    switch ((unsigned short)FTBL.shenGetRandomInt32(2)) {
    case 0:
    default:
        PvsjQueueVoice(work, "A046", &work->delay);
        break;
    case 1:
        PvsjQueueVoice(work, "A047", &work->delay);
        break;
    }
    PvsjSetCamera(pvsjCustomerCamera, work->customer, work->customer, work->delay, work);
    switch ((unsigned short)FTBL.shenGetRandomInt32(2)) {
    case 0:
    default:
        PvsjQueueVoice(work, "F001", &work->delay);
        break;
    case 1:
        PvsjQueueVoice(work, "F002", &work->delay);
        break;
    }
    PvsjSetCamera(pvsjPlayerCamera, PVSJ_PLAYER, PVSJ_PLAYER, work->delay, work);
    PvsjQueueVoice(work, "A048", &work->delay);
}

void PvsjExplainPartTimeJob(PvsjEventWork *work)
{
    EVT_AT_SetFaceLooks(PVSJ_PLAYER, 0, 10, 1.0f, 0);
    EVT_AT_SetFaceLooks(work->customer, 0, 10, 1.0f, 0);
    PvsjSetCamera(pvsjCustomerCamera + 1, work->customer, work->customer, work->delay, work);
    PvsjSetMotion(work->customer, work->boards->motions[17], 0.0f, 1.0f, 0, work->delay);
    PvsjSetMotion(PVSJ_PLAYER, work->boards->motions[7], 0.0f, 1.0f, 0, work->delay);
    switch ((unsigned short)FTBL.shenGetRandomInt32(2)) {
    case 0:
    default:
        PvsjQueueVoice(work, "F003", &work->delay);
        break;
    case 1:
        PvsjQueueVoice(work, "F004", &work->delay);
        break;
    }
    PvsjSetCamera(pvsjPlayerCamera + 1, PVSJ_PLAYER, PVSJ_PLAYER, work->delay, work);
    PvsjQueueVoice(work, "A049", &work->delay);
    PvsjSetCamera(pvsjCustomerCamera + 1, work->customer, work->customer, work->delay, work);
    switch ((unsigned short)FTBL.shenGetRandomInt32(2)) {
    case 0:
    default:
        PvsjQueueVoice(work, "F001", &work->delay);
        break;
    case 1:
        PvsjQueueVoice(work, "F002", &work->delay);
        break;
    }
    PvsjSetCamera(pvsjPlayerCamera, PVSJ_PLAYER, PVSJ_PLAYER, work->delay, work);
    PvsjQueueVoice(work, "A048", &work->delay);
}

void PvsjStartCustomerGame(PvsjEventWork *work)
{
    EVT_AT_SetFaceLooks(PVSJ_PLAYER, 0, 10, 1.0f, 0);
    EVT_AT_SetFaceLooks(work->customer, 0, 10, 1.0f, 0);
    PvsjSetCamera(pvsjCustomerCamera, work->customer, work->customer, work->delay, work);
    switch ((unsigned short)FTBL.shenGetRandomInt32(2)) {
    case 0:
    default:
        PvsjQueueVoice(work, "F005", &work->delay);
        break;
    case 1:
        PvsjQueueVoice(work, "F006", &work->delay);
        break;
    }
    PvsjSetCamera(pvsjPlayerCamera, PVSJ_PLAYER, PVSJ_PLAYER, work->delay, work);
    PvsjQueueVoice(work, "A072", &work->delay);
}

void PvsjCustomerDeclines(PvsjEventWork *work)
{
    EVT_AT_SetFaceLooks(PVSJ_PLAYER, 0, 10, 1.0f, 0);
    EVT_AT_SetFaceLooks(work->customer, 0, 10, 1.0f, 0);
    PvsjSetCamera(pvsjPlayerCamera + 1, PVSJ_PLAYER, PVSJ_PLAYER, work->delay, work);
    PvsjSetMotion(work->customer, work->boards->motions[12], 0.0f, 1.0f, 0, work->delay);
    PvsjSetMotion(PVSJ_PLAYER, work->boards->motions[7], 0.0f, 1.0f, 0, work->delay);
    switch ((unsigned short)FTBL.shenGetRandomInt32(2)) {
    case 0:
    default:
        PvsjQueueVoice(work, "A046", &work->delay);
        break;
    case 1:
        PvsjQueueVoice(work, "A047", &work->delay);
        break;
    }
    PvsjSetCamera(pvsjCustomerCamera + 1, work->customer, work->customer, work->delay, work);
    switch ((unsigned short)FTBL.shenGetRandomInt32(2)) {
    case 0:
    default:
        PvsjQueueVoice(work, "F007", &work->delay);
        break;
    case 1:
        PvsjQueueVoice(work, "F008", &work->delay);
        break;
    }
    PvsjSetCamera(pvsjPlayerCamera, PVSJ_PLAYER, PVSJ_PLAYER, work->delay, work);
    PvsjQueueVoice(work, "A050", &work->delay);
    switch ((unsigned short)FTBL.shenGetRandomInt32(2)) {
    case 0:
    default:
        PvsjQueueVoice(work, "A051", &work->delay);
        break;
    case 1:
        PvsjQueueVoice(work, "A052", &work->delay);
        break;
    }
}

void PvsjHandleCustomerInquiry(PvsjEventWork *work)
{
    EVT_AT_SetFaceLooks(PVSJ_PLAYER, 0, 10, 1.0f, 0);
    EVT_AT_SetFaceLooks(work->customer, 0, 10, 1.0f, 0);
    PvsjSetCamera(pvsjCustomerCamera + 1, work->customer, work->customer, work->delay, work);
    PvsjSetMotion(work->customer, work->boards->motions[17], 0.0f, 1.0f, 0, work->delay);
    PvsjSetMotion(PVSJ_PLAYER, work->boards->motions[7], 0.0f, 1.0f, 0, work->delay);
    switch ((unsigned short)FTBL.shenGetRandomInt32(2)) {
    case 0:
    default:
        PvsjQueueVoice(work, "F009", &work->delay);
        break;
    case 1:
        PvsjQueueVoice(work, "F004", &work->delay);
        break;
    }
    PvsjSetCamera(pvsjPlayerCamera + 1, PVSJ_PLAYER, PVSJ_PLAYER, work->delay, work);
    PvsjQueueVoice(work, "A049", &work->delay);
    PvsjSetCamera(pvsjCustomerCamera + 1, work->customer, work->customer, work->delay, work);
    switch ((unsigned short)FTBL.shenGetRandomInt32(2)) {
    case 0:
    default:
        PvsjQueueVoice(work, "F010", &work->delay);
        break;
    case 1:
        PvsjQueueVoice(work, "F011", &work->delay);
        break;
    }
    PvsjSetCamera(pvsjPlayerCamera, PVSJ_PLAYER, PVSJ_PLAYER, work->delay, work);
    switch ((unsigned short)FTBL.shenGetRandomInt32(2)) {
    case 0:
    default:
        PvsjQueueVoice(work, "A053", &work->delay);
        break;
    case 1:
        PvsjQueueVoice(work, "A054", &work->delay);
        break;
    }
    PvsjSetCamera(pvsjCustomerCamera, work->customer, work->customer, work->delay, work);
    switch ((unsigned short)FTBL.shenGetRandomInt32(2)) {
    case 0:
    default:
        PvsjQueueVoice(work, "F012", &work->delay);
        break;
    case 1:
        PvsjQueueVoice(work, "F013", &work->delay);
        break;
    }
}

void PvsjReportShift(PvsjEventWork *work)
{
    int start = work->delay;
    EVT_AT_SetFaceLooks(PVSJ_PLAYER, 0, 10, 1.0f, 0);
    EVT_AT_SetFaceLooks(work->owner, 3, 10, 1.0f, 0);
    PvsjSetCamera(pvsjCustomerCamera + 1, work->owner, work->owner, work->delay, work);
    PvsjSetMotion(work->owner, work->boards->motions[1], 100.0f, 1.0f, 0, work->delay);
    PvsjSetMotion(PVSJ_PLAYER, work->boards->motions[7], 0.0f, 1.0f, 0, work->delay);
    switch ((unsigned short)FTBL.shenGetRandomInt32(2)) {
    case 0:
    default:
        PvsjQueueVoice(work, "B098", &work->delay);
        break;
    case 1:
        PvsjQueueVoice(work, "B099", &work->delay);
        break;
    }
    PvsjSetCamera(pvsjPlayerCamera, PVSJ_PLAYER, PVSJ_PLAYER, work->delay, work);
    if (work->delay - start < 250)
        start = work->delay;
    else
        start += 250;
    PvsjSetMotion(work->owner, work->boards->motions[5], 0.0f, 1.0f, 0, start);
    PvsjSetMotion(PVSJ_PLAYER, work->boards->motions[7], 0.0f, 1.0f, 0, start);
}

void PvsjAskToContinue(PvsjEventWork *work)
{
    EVT_AT_SetFaceLooks(PVSJ_PLAYER, 0, 10, 1.0f, 0);
    EVT_AT_SetFaceLooks(work->owner, 0, 10, 1.0f, 0);
    PvsjSetCamera(pvsjCustomerCamera, work->owner, work->owner, work->delay, work);
    PvsjSetMotion(work->owner, work->boards->motions[5], 0.0f, 1.0f, 0, work->delay);
    PvsjSetMotion(PVSJ_PLAYER, work->boards->motions[7], 0.0f, 1.0f, 0, work->delay);
    switch ((unsigned short)FTBL.shenGetRandomInt32(2)) {
    case 0:
    default:
        PvsjQueueVoice(work, "B100", &work->delay);
        break;
    case 1:
        PvsjQueueVoice(work, "B101", &work->delay);
        break;
    }
}

void PvsjClosingReport(PvsjEventWork *work)
{
    EVT_AT_SetFaceLooks(PVSJ_PLAYER, 0, 10, 1.0f, 0);
    EVT_AT_SetFaceLooks(work->owner, 0, 10, 1.0f, 0);
    PvsjSetCamera(pvsjCustomerCamera + 1, work->owner, work->owner, work->delay, work);
    switch ((unsigned short)FTBL.shenGetRandomInt32(2)) {
    case 0:
    default:
        PvsjQueueVoice(work, "B102", &work->delay);
        break;
    case 1:
        PvsjQueueVoice(work, "B103", &work->delay);
        break;
    }
    PvsjSetCamera(pvsjPlayerCamera, PVSJ_PLAYER, PVSJ_PLAYER, work->delay, work);
    PvsjQueueVoice(work, "A035", &work->delay);
}

void PvsjTimeExpired(PvsjEventWork *work)
{
    EVT_AT_SetFaceLooks(PVSJ_PLAYER, 0, 10, 1.0f, 0);
    EVT_AT_SetFaceLooks(work->owner, 0, 10, 1.0f, 0);
    PvsjSetCamera(pvsjCustomerCamera + 1, work->owner, work->owner, work->delay, work);
    switch ((unsigned short)FTBL.shenGetRandomInt32(3)) {
    case 0:
    default:
        PvsjQueueVoice(work, "B118", &work->delay);
        break;
    case 1:
        PvsjQueueVoice(work, "B119", &work->delay);
        break;
    case 2:
        PvsjQueueVoice(work, "B120", &work->delay);
        break;
    }
}

void PvsjContinueShift(PvsjEventWork *work)
{
    EVT_AT_SetFaceLooks(PVSJ_PLAYER, 0, 10, 1.0f, 0);
    EVT_AT_SetFaceLooks(work->owner, 0, 10, 1.0f, 0);
    PvsjSetCamera(pvsjPlayerCamera, PVSJ_PLAYER, PVSJ_PLAYER, work->delay, work);
    switch ((unsigned short)FTBL.shenGetRandomInt32(2)) {
    case 0:
    default:
        PvsjQueueVoice(work, "A036", &work->delay);
        break;
    case 1:
        PvsjQueueVoice(work, "A037", &work->delay);
        break;
    }
    PvsjSetMotion(work->owner, work->boards->motions[0], 0.0f, 1.0f, 0, work->delay);
    PvsjSetMotion(PVSJ_PLAYER, work->boards->motions[6], 0.0f, 1.0f, 0, work->delay);
    PvsjSetCamera(pvsjCustomerCamera + 1, work->owner, work->owner, work->delay, work);
    PvsjQueueVoice(work, "B104", &work->delay);
    PvsjSetMotion(work->owner, work->boards->motions[5], 0.0f, 1.0f, 0, work->delay);
    PvsjSetMotion(PVSJ_PLAYER, work->boards->motions[7], 0.0f, 1.0f, 0, work->delay);
}

void PvsjOfferPay(PvsjEventWork *work)
{
    int start = work->delay;
    EVT_AT_SetFaceLooks(PVSJ_PLAYER, 0, 10, 1.0f, 0);
    EVT_AT_SetFaceLooks(work->owner, 3, 10, 1.0f, 0);
    PvsjSetCamera(pvsjCustomerCamera, work->owner, work->owner, work->delay, work);
    PvsjSetMotion(work->owner, work->boards->motions[3], 0.0f, 1.0f, 0, work->delay);
    PvsjSetMotion(PVSJ_PLAYER, work->boards->motions[8], 0.0f, 1.0f, 0, work->delay);
    switch ((unsigned short)FTBL.shenGetRandomInt32(2)) {
    case 0:
    default:
        PvsjQueueVoice(work, "B105", &work->delay);
        break;
    case 1:
        PvsjQueueVoice(work, "B106", &work->delay);
        break;
    }
    switch ((unsigned short)FTBL.shenGetRandomInt32(2)) {
    case 0:
    default:
        PvsjQueueVoice(work, "B107", &work->delay);
        break;
    case 1:
        PvsjQueueVoice(work, "B108", &work->delay);
        break;
    }
    PvsjQueueVoice(work, "B109", &work->delay);
    if (work->delay - start < 250)
        start = work->delay;
    else
        start += 250;
    PvsjSetMotion(work->owner, work->boards->motions[5], 0.0f, 1.0f, 0, start);
    PvsjSetMotion(PVSJ_PLAYER, work->boards->motions[7], 0.0f, 1.0f, 0, start);
}

void PvsjDeclineContinue(PvsjEventWork *work)
{
    EVT_AT_SetFaceLooks(PVSJ_PLAYER, 0, 10, 1.0f, 0);
    EVT_AT_SetFaceLooks(work->owner, 3, 10, 1.0f, 0);
    PvsjSetCamera(pvsjCustomerCamera + 1, work->owner, work->owner, work->delay, work);
    PvsjSetMotion(work->owner, work->boards->motions[5], 0.0f, 1.0f, 0, work->delay);
    PvsjSetMotion(PVSJ_PLAYER, work->boards->motions[7], 0.0f, 1.0f, 0, work->delay);
    switch ((unsigned short)FTBL.shenGetRandomInt32(2)) {
    case 0:
    default:
        PvsjQueueVoice(work, "B110", &work->delay);
        break;
    case 1:
        PvsjQueueVoice(work, "B111", &work->delay);
        break;
    }
}

void PvsjAskWithoutProfit(PvsjEventWork *work)
{
    int start = work->delay;
    EVT_AT_SetFaceLooks(PVSJ_PLAYER, 0, 10, 1.0f, 0);
    EVT_AT_SetFaceLooks(work->owner, 1, 10, 1.0f, 0);
    PvsjSetCamera(pvsjCustomerCamera + 1, work->owner, work->owner, work->delay, work);
    PvsjSetMotion(work->owner, work->boards->motions[2], 130.0f, 1.0f, 0, work->delay);
    PvsjSetMotion(PVSJ_PLAYER, work->boards->motions[7], 0.0f, 1.0f, 0, work->delay);
    PvsjQueueVoice(work, "B112", &work->delay);
    PvsjQueueVoice(work, "A038", &work->delay);
    PvsjQueueVoice(work, "B113", &work->delay);
    if (work->delay - start < 524)
        start = work->delay;
    else
        start += 524;
    PvsjSetMotion(work->owner, work->boards->motions[5], 0.0f, 1.0f, 0, start);
    PvsjSetMotion(PVSJ_PLAYER, work->boards->motions[7], 0.0f, 1.0f, 0, start);
}

void PvsjRestoreSpeech(PvsjEventWork *work)
{
    PvsjSetCamera(pvsjPlayerCamera, PVSJ_PLAYER, PVSJ_PLAYER, work->delay, work);
    switch ((unsigned short)FTBL.shenGetRandomInt32(2)) {
    case 0:
    default:
        PvsjQueueVoice(work, "A039", &work->delay);
        break;
    case 1:
        PvsjQueueVoice(work, "A040", &work->delay);
        break;
    }
}

void PvsjFinishWithoutProfit(PvsjEventWork *work)
{
    int start = work->delay;
    EVT_AT_SetFaceLooks(PVSJ_PLAYER, 0, 10, 1.0f, 0);
    EVT_AT_SetFaceLooks(work->owner, 0, 10, 1.0f, 0);
    PvsjSetCamera(pvsjCustomerCamera + 1, work->owner, work->owner, work->delay, work);
    PvsjSetMotion(work->owner, work->boards->motions[4], 0.0f, 1.0f, 0, work->delay);
    PvsjSetMotion(PVSJ_PLAYER, work->boards->motions[9], 0.0f, 1.0f, 0, work->delay);
    switch ((unsigned short)FTBL.shenGetRandomInt32(3)) {
    case 0:
    default:
        PvsjQueueVoice(work, "B114", &work->delay);
        break;
    case 1:
        PvsjQueueVoice(work, "B115", &work->delay);
        break;
    case 2:
        PvsjQueueVoice(work, "B116", &work->delay);
        break;
    }
    PvsjSetCamera(pvsjPlayerCamera, PVSJ_PLAYER, PVSJ_PLAYER, work->delay, work);
    PvsjQueueVoice(work, "A041", &work->delay);
    PvsjSetCamera(pvsjCustomerCamera + 1, work->owner, work->owner, work->delay, work);
    PvsjQueueVoice(work, "B117", &work->delay);
    if (work->delay - start < 200)
        start = work->delay;
    else
        start += 200;
    PvsjSetMotion(work->owner, work->boards->motions[5], 0.0f, 1.0f, 0, start);
    PvsjSetMotion(PVSJ_PLAYER, work->boards->motions[7], 0.0f, 1.0f, 0, start);
}

void PvsjShowSessionCash(PvsjEventWork *work)
{
    PvsjSetCamera(pvsjPlayerCamera, PVSJ_PLAYER, PVSJ_PLAYER, work->delay, work);
    FTBL.AnimateMoneyAmount(FTBL.GetPlayerStat(2), work->sessionCash, 0);
    FTBL.SetPlayerStat(2, work->sessionCash);
}

void PvsjShowWallet(PvsjEventWork *work)
{
    PvsjSetCamera(pvsjPlayerCamera, PVSJ_PLAYER, PVSJ_PLAYER, work->delay, work);
    FTBL.AnimateMoneyAmount(FTBL.GetPlayerStat(2), 0, 0);
    FTBL.SetPlayerStat(2, 0);
}

void AreaPVSJMain(void *argument)
{
    HLibTask *task;
    PvsjEventWork *work;
    HLibTask *hint;
    PvsjEventWork **hintData;
    task = FTBL.HLib_EnqueueTaskWithParameter(PvsjMainCallback, 5, 11, sizeof(PvsjEventWork), TASK_EVNT);
    work = (PvsjEventWork *)FTBL.HLib_GetTaskParameter(task);
    work->parent = FTBL.HLib_GetCurrentTask();
    work->task = task;
    work->argument = *(shen_name *)argument;
    work->soundScene = FTBL.GetSelectedSoundSceneIndex();
    strcpy(work->sbt, FTBL.shenGetCurrentSbt());
    strcpy(work->bgm, FTBL.GetCurrentBgm());
    work->previousCash = FTBL.GetPlayerStat(2);
    work->state = 0;
    work->sessionCash = 300;
    work->jobPartition = PVSJ_INVALID_PARTITION;
    work->customerPartition = -1;
    work->firstCustomer = 1;
    work->gamePartition = PVSJ_INVALID_PARTITION;
    PvsjSavePlayerRotation();
    ResetPatiMotionCache();
    pvsjSkipMotion = 0;
    hint = FTBL.HLib_EnqueueTaskWithParameter(PvsjHintCallback, 5, 11, sizeof(PvsjEventWork *), TASK_EVNT);
    hintData = (PvsjEventWork **)FTBL.HLib_GetTaskParameter(hint);
    work->hintTask = hint;
    *hintData = work;
    work->greetingIndex = FTBL.shenGetRandomInt32(15);
    if (!(work->voiceLoadBuffer = FTBL.HLib_AllocateTaskData(0x4000, AREA_PVSJ)))
        FTBL.EventAssertNullsub(__FILE__, 1786);
    if (!(work->voiceLengths = (PvsjVoiceLength *)FTBL.HLib_AllocateTaskData(0x4000, AREA_PVSJ)))
        FTBL.EventAssertNullsub(__FILE__, 1788);
    EventDebugRegister(PVSJ_DEBUG_ID, PvsjDebugExitCallback, work, 216000);
    EventDebugCheck(PVSJ_DEBUG_ID, 4);
    FTBL.SetPlayerControlEnabled(0);
    FTBL.DebugLogToFileRemoved("PVSJ:New-Load [PVSJ]PAK\n");
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(7), (int)"PVSJ", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.DebugLogToFileRemoved("PVSJ:New-Load [PATI]PAK\n");
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(7), (int)"PATI", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.DebugLogToFileRemoved("PVSJ:New-Load [PVSJ]afs\n");
    work->jobPartition = FTBL.EV_ControlPackageResources(1, 0, FTBL.Filepath_Gen(7), (int)"PVSJ.afs", 0);
    FTBL.DebugLogToFileRemoved("EV_PAKLOAD_AFS [PVSJ] %i\n", work->jobPartition);
    if (work->jobPartition == PVSJ_INVALID_PARTITION)
        FTBL.EventAssertNullsub(__FILE__, 1807);
    FTBL.DebugLogToFileRemoved("PVSJ:New-Load [PATI]afs\n");
    work->gamePartition = FTBL.EV_ControlPackageResources(1, 0, FTBL.Filepath_Gen(7), (int)"PATI.afs", 0);
    FTBL.DebugLogToFileRemoved("EV_PAKLOAD_AFS [PATI] %i\n", work->gamePartition);
    if (work->gamePartition == PVSJ_INVALID_PARTITION)
        FTBL.EventAssertNullsub(__FILE__, 1811);
    FTBL.LoadAreaFunc_Wrapper_0("PATI.EV3", 0);
    PvsjInitializeBoards(work);
    FTBL.CheckHelpIcon(0, 0, 0);
    FTBL.camCtrlSetState(0);
    FTBL.EV_EventSetUpHelpIcon_0(145, 0);
    FTBL.HLib_SetupTaskDelay(0);
}

void PvsjDebugExitCallback(void *data)
{
    PvsjEventWork *work = (PvsjEventWork *)data;
    work->state = 60;
    FTBL.EV_refreshPadControlData(0, 1);
}

void PvsjFinish(PvsjEventWork *work)
{
    shen_name area;
    int entry;
    void *player;
    EventDebugUnregister(PVSJ_DEBUG_ID);
    if (work->gamePartition != PVSJ_INVALID_PARTITION) {
        FTBL.DebugLogToFileRemoved("EV_UNMOUNT_AFS_PARTITION [PATI].afs %i\n", work->gamePartition);
        FTBL.EV_ControlPackageResources(1, 1, work->gamePartition, 0, 0);
        work->gamePartition = PVSJ_INVALID_PARTITION;
    }
    FTBL.HLib_FreeTaskData(work->voiceLoadBuffer);
    FTBL.HLib_FreeTaskData(work->voiceLengths);
    if (work->jobPartition != PVSJ_INVALID_PARTITION) {
        if (work->customerPartition != -1) {
            if (FTBL.EV_ControlPackageResources(0, 2, 0, 0, 0)) {
                FTBL.DebugLogToFileRemoved("EV_DESTROY_PAKFILE_AFS [PVSJ]\n");
                FTBL.EV_ControlPackageResources(0, 13, work->jobPartition, pvsjCustomerPartitions[work->customerPartition].models,
                    pvsjCustomerPartitions[work->customerPartition].hands);
            } else {
                FTBL.DebugLogToFileRemoved("EV_FREE_PAKFILE_AFS [PVSJ]\n");
                FTBL.EV_ControlPackageResources(0, 12, work->jobPartition, pvsjCustomerPartitions[work->customerPartition].models,
                    pvsjCustomerPartitions[work->customerPartition].hands);
            }
            work->customerPartition = -1;
        }
        FTBL.DebugLogToFileRemoved("EV_UNMOUNT_AFS_PARTITION [PVSJ].afs %i\n", work->jobPartition);
        FTBL.EV_ControlPackageResources(1, 1, work->jobPartition, 0, 0);
        work->jobPartition = PVSJ_INVALID_PARTITION;
    }
    FTBL.EV_ControlPackageResources(0, 6, "PATI", 0, 0);
    FTBL.EV_ControlPackageResources(0, 6, "PVSJ", 0, 0);
    PvsjReleaseVoice(work);
    FTBL.EV_LoadCutsceneFromCurrentScene(work->bgm);
    FTBL.EV_LoadAudio(work->sbt, work->soundScene);
    FTBL.CheckHelpIcon(0, 0, 0);
    FTBL.EV_EventSetUpHelpIcon_0(0, 0);
    FTBL.EV_NPC_NewLoadSw_0(1);
    FTBL.SetRefBoardParamsForCharaById(PVSJ_PLAYER, 0);
    EVT_AT_SetFaceLooks(PVSJ_PLAYER, 0, 10, 1.0f, 0);
    FTBL.ReleaseCharacterFaceCache(FTBL.ctrlGetCharaByName(PVSJ_PLAYER));
    FTBL.GetCharacterFaceCacheIndex(FTBL.ctrlGetCharaByName(PVSJ_PLAYER));
    EVT_AT_SetFaceLooks(PVSJ_PLAYER, 0, 10, 1.0f, 0);
    FTBL.EV_EnableCharacterDisplay(PVSJ_PLAYER);
    if (FTBL.DoesScenarioCharacterExist(PVSJ_BACKPACK) == 1)
        FTBL.EV_EnableCharacterDisplay(PVSJ_BACKPACK);
    FTBL.EV_SetCharaControllerFlags(PVSJ_PLAYER, 2, 1);
    FTBL.EV_SetCharaControllerFlags(PVSJ_PLAYER, 3, 1);
    SHENMUE_EVENT_BOUNDARY();
    FTBL.SetPlayerStat(2, work->previousCash);
    FTBL.HLib_MarkCurrentTaskForCleanup();
    FTBL.HLib_DestroyCurrentEventTask();
    switch (g_currentMap) {
    case SHEN_NAME4('A', 'R', '0', '9'):
        area = SHEN_NAME4('A', 'R', '0', '3');
        break;
    case SHEN_NAME4('Q', 'T', '0', '9'):
        area = SHEN_NAME4('Q', 'T', 'B', '3');
        break;
    default:
        area = (g_currentMap & 0xffffff) | 0x30000000;
        break;
    }
    entry = g_entryPoint + 1;
    FTBL.DebugLogToFileRemoved("PVSJ:Exit to (%i,%x,%i)\n", g_actNumber, area, entry);
    FTBL.EV_scenQueueLoad(g_actNumber, area, entry);
    FTBL.EVS_LoopCamera(0, 0, 0);
}

void PvsjShowChoice(int first, int second, int third, int fourth, PvsjEventWork *work)
{
    PvsjSetCamera(pvsjPlayerCamera, PVSJ_PLAYER, PVSJ_PLAYER, work->delay, work);
    EVT_AT_KsIcon_Enqueue(first, second, third, fourth, 10800, 0xffff, 2, work->delay);
    work->delay += 5;
}

void PvsjSetCamera(int id, shen_name eye, shen_name target, int delay, PvsjEventWork *work)
{
    shen_name eyeText[2], targetText[2];
    if (id == work->cameraId && eye == work->cameraEye && target == work->cameraTarget)
        return;
    FTBL.EV_ControlPackageResources(2, 4, "PATI", 0, 0);
    eyeText[0] = eye;
    targetText[0] = target;
    targetText[1] = 0;
    eyeText[1] = 0;
    FTBL.DebugLogToFileRemoved("SetCamera:%i,%s,%s\n", id, eyeText, targetText);
    EVT_AT_SetCam(id, eye, target, 0, 0, delay);
    work->cameraId = id;
    work->cameraEye = eye;
    work->cameraTarget = target;
    g_patiCameraTime = delay;
    pvsjCameraBlend = -1;
}

void ShowPatiVoiceMessage(void *text)
{
    QueueEventMessage((const char *)text, 0, 60);
}
void PvsjHintCallback(void *data)
{
    PvsjEventWork **hint = (PvsjEventWork **)data;
    switch ((*hint)->state) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 31:
    case 32:
    case 33:
    case 59:
    case 60:
    case 61:
        break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 15:
    case 16:
    case 17:
    case 34:
        if (FTBL.filmCheckFadeTrackColour())
            break;
        FTBL.GetBattleDataConfig(0, PVSJ_BATTLE_FONT);
    default:
        if (!FTBL.filmCheckFadeTrackColour())
            FTBL.CheckHelpIcon(0x100000, 0, 3);
        break;
    }
    if (pvsjSkipMotion) {
        if ((*hint)->customerActive & 1) {
            if (!FTBL.npcGetFlagFor((*hint)->customer)) {
                pvsjSkipMotion = 0;
                if (FTBL.DoesScenarioCharacterExist((*hint)->customer))
                    PvsjSetMotion((*hint)->customer, (*hint)->boards->motions[12], 0.0f, 1.0f, 0, 0);
            }
        } else if (!FTBL.npcGetFlagFor((*hint)->owner)) {
            pvsjSkipMotion = 0;
            if (FTBL.DoesScenarioCharacterExist((*hint)->owner))
                PvsjSetMotion((*hint)->owner, (*hint)->boards->motions[5], 0.0f, 1.0f, 0, 0);
        }
    }
}

int PvsjMoveCustomer(shen_name character, shen_name target, PvsjEventWork *work)
{
    char characterText[8], previousText[8], targetText[8];
    int i, result;
    shen_name previous;
    unsigned int mask = 0xffffff;
    shen_name goal = PVSJ_GOAL_POINT;
    for (i = 0; work->boards->customers[i].character && work->boards->customers[i].character != character; i++)
        ;
    if (!work->boards->customers[i].character)
        FTBL.EventAssertNullsub(__FILE__, 2057);
    previous = work->boards->customers[i].currentPoint;
    if (!previous) {
        FTBL.DebugLogToFileRemoved("PVSJ:%s Can not move.\n", characterText);
        return 0;
    }
    work->boards->customers[i].currentPoint = target;
    if ((previous & mask) == goal)
        previous = (previous & mask) | (target & ~mask);
    if ((target & mask) == goal)
        target = (target & mask) | (previous & ~mask);
    for (i = 0; i < 4; i++) {
        characterText[i] = character >> i * 8;
        previousText[i] = previous >> i * 8;
        targetText[i] = target >> i * 8;
    }
    characterText[i] = 0;
    previousText[i] = 0;
    targetText[i] = 0;
    FTBL.DebugLogToFileRemoved("PVSJ:%s (%s->%s)%08X\n", characterText, previousText, targetText, work->boards->yaw);
    result = FTBL.StartNpcScriptAreaRoute(character, previous, target, work->boards->yaw);
    FTBL.DebugLogToFileRemoved("PVSJ:ret = %i\n", i);
    return result;
}

int PvsjRestoreCustomerRoute(shen_name character, PvsjEventWork *work)
{
    char characterText[8], pointText[8];
    int i, result;
    shen_name point;
    for (i = 0; work->boards->customers[i].character && work->boards->customers[i].character != character; i++)
        ;
    if (!work->boards->customers[i].character)
        FTBL.EventAssertNullsub(__FILE__, 2105);
    point = work->boards->customers[i].currentPoint;
    if (!point) {
        FTBL.DebugLogToFileRemoved("PVSJ:%s Can not move.\n", characterText);
        return 0;
    }
    for (i = 0; i < 4; i++) {
        characterText[i] = character >> i * 8;
        pointText[i] = point >> i * 8;
    }
    characterText[i] = 0;
    pointText[i] = 0;
    FTBL.DebugLogToFileRemoved("PVSJ:%s (->%s)%08X\n", characterText, pointText, work->boards->yaw);
    result = FTBL.npcStartPlayEntityAtPos(character, point, 0, work->boards->yaw);
    FTBL.DebugLogToFileRemoved("PVSJ:ret = %i\n", i);
    return result;
}

void PvsjChooseCustomer(PvsjEventWork *work)
{
    shen_name previous = work->customer;
    int index, count;
    work->customerActive = 1;
    work->greeting = work->boards->greetings[work->greetingIndex];
    FTBL.DebugLogToFileRemoved("ComType[%i]=%i\n", work->greetingIndex, work->greeting);
    work->greetingIndex = (work->greetingIndex + 1) & 15;
    switch (work->greeting) {
    case 0:
    case 1:
        if (work->admittedCustomers) {
            index = FTBL.shenGetRandomInt32(work->admittedCustomers - 1) + 1;
            work->customer = work->boards->customers[index].character;
            if (work->customer == previous) {
                if (work->boards->customers[++index].kind != 1)
                    index = 1;
                work->customer = work->boards->customers[index].character;
            }
            return;
        }
        break;
    default:
        count = work->missingCustomers + work->admittedCustomers;
        if (count) {
            index = FTBL.shenGetRandomInt32(count - 1) + 1;
            work->customer = work->boards->customers[index].character;
            if (work->customer == previous)
                work->customer = work->boards->customers[1].character;
            return;
        }
        break;
    }
    work->customer = work->owner;
}

#define PVSJ_SELECT_CAMERAS(name) \
    { \
        for (i = 0; pvsjDealers[i].character && pvsjDealers[i].character != (name); i++) \
            ; \
        pvsjPlayerCamera = 901; \
        pvsjCustomerCamera = 911; \
        if (pvsjDealers[i].motions) \
            pvsjCustomerCamera = pvsjDealers[i].camera; \
    }
#define PVSJ_RELEASE_SUBJECTS(name) \
    { \
        EVT_AT_SetFaceLooks(PVSJ_PLAYER, 0, 10, 1.0f, 0); \
        EVT_AT_SetFaceLooks((name), 0, 10, 1.0f, 0); \
        FTBL.ReleaseCharacterFaceCache(FTBL.ctrlGetCharaByName(PVSJ_PLAYER)); \
        FTBL.ReleaseCharacterFaceCache(FTBL.ctrlGetCharaByName(name)); \
        FTBL.SetRefBoardParamsForCharaById((name), 0); \
        FTBL.EV_SetCharacterAlphaEnabled((name), 0); \
        FTBL.npcLockUnlock(name); \
    }
#define PVSJ_DIALOGUE_SUBJECTS(name) \
    { \
        FTBL.CheckHelpIcon(1, 0, 0); \
        FTBL.EV_EnableCharacterDisplay(PVSJ_PLAYER); \
        FTBL.GetCharacterFaceCacheIndex(FTBL.ctrlGetCharaByName(PVSJ_PLAYER)); \
        FTBL.GetCharacterFaceCacheIndex(FTBL.ctrlGetCharaByName(name)); \
        EVT_AT_SetFaceLooks(PVSJ_PLAYER, 0, 10, 1.0f, 0); \
        EVT_AT_SetFaceLooks((name), 0, 10, 1.0f, 0); \
        FTBL.SetRefBoardParamsForCharaById(PVSJ_PLAYER, 1); \
        FTBL.SetRefBoardParamsForCharaById((name), 1); \
    }
#define PVSJ_CUSTOMER_DOT() \
    { \
        FTBL.GetCharaPosition(PVSJ_PLAYER, -1, &player, 0x40000000); \
        FTBL.GetCharaPosition(work->customer, -1, &customer, 0x40000000); \
        customer.x -= player.x; \
        customer.y -= player.y; \
        customer.z -= player.z; \
        FTBL.Vector3_NormalizeInPlace(&customer); \
        player.x = pvsjFilteredTarget.x - pvsjFilteredEye.x; \
        player.y = pvsjFilteredTarget.y - pvsjFilteredEye.y; \
        player.z = pvsjFilteredTarget.z - pvsjFilteredEye.z; \
        FTBL.Vector3_NormalizeInPlace(&player); \
        dot = FTBL.GetVector3DotProduct(&customer, &player); \
    }

void PvsjMainCallback(void *data)
{
    PvsjEventWork *work = (PvsjEventWork *)data;
    Vector3 player, customer;
    EventDateTime date;
    Vector3 handPosition;
    Vector3i handRotation;
    int previous, close, now, partition, board;
    unsigned int i;
    float dot;
    shen_name point, oldCustomer;
    work->delay = 0;
    g_patiCameraTime = 99999;
    previous = work->state;
    if (FTBL.GetShouldRenderIcons(0))
        return;
    if (!FTBL.filmCheckFadeTrackColour()) {
        SHENMUE_EVENT_BOUNDARY();
        FTBL.GetCurrentControlInput(2);
    }
    switch (work->state) {
    case 0:
        if (FTBL.ForceLoadNpc(work->owner)) {
            FTBL.CheckWarmStartNpcRoute(work->owner);
            FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(PVSJ_PLAYER), 0, 0);
            FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(work->owner), 0, 0);
            FTBL.EV_SetCharacterAlphaEnabled(work->owner, 0);
            PvsjSetMotion(work->owner, work->boards->motions[5], 0.0f, 1.0f, 0x40000000, work->delay);
            work->state = 1;
            work->delay = 2;
        }
        break;
    case 1:
        PVSJ_SELECT_CAMERAS(work->owner);
        PvsjSetCamera(pvsjCustomerCamera, work->owner, work->owner, work->delay, work);
        FTBL.EV_NPC_NewLoadSw_0(0);
        FTBL.EventFunction869();
        PvsjLoadVoice(work, work->owner);
        work->state = 2;
        break;
    case 2:
        if (!FTBL.filmCheckFadeTrackColour()) {
            FTBL.EV_NPC_NewLoadSw_0(1);
            work->state = 3;
        }
        break;
    case 3:
        if (FTBL.ForceLoadNpc(work->owner)) {
            FTBL.CheckWarmStartNpcRoute(work->owner);
            FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(work->owner), 0, 0);
            FTBL.EV_SetCharacterAlphaEnabled(work->owner, 0);
            FTBL.GetCharaPosition(PVSJ_PLAYER, -1, &player, 0x40000000);
            FTBL.GetCharaPosition(work->owner, -1, &player, 0x40000000);
            FTBL.GetCharacterFaceCacheIndex(FTBL.ctrlGetCharaByName(PVSJ_PLAYER));
            FTBL.GetCharacterFaceCacheIndex(FTBL.ctrlGetCharaByName(work->owner));
            EVT_AT_SetFaceLooks(PVSJ_PLAYER, 0, 10, 1.0f, 0);
            EVT_AT_SetFaceLooks(work->owner, 0, 10, 1.0f, 0);
            FTBL.SetRefBoardParamsForCharaById(PVSJ_PLAYER, 1);
            FTBL.SetRefBoardParamsForCharaById(work->owner, 1);
            work->state = 4;
        }
        break;
    case 4:
        PvsjOfferJob(work);
        work->state = 5;
        break;
    case 5:
        work->previousCash = FTBL.GetPlayerStat(2);
        work->sessionCash = 300;
        FTBL.SetPlayerStat(2, 0);
        PvsjShowSessionCash(work);
        work->state = 6;
        break;
    case 6:
        PvsjBeginShift(work);
        FTBL.GetCurrentDateTime((DateTime *)&date);
        work->startMinute = date.day * 1440 + date.hour * 60 + date.minute;
        PvsjChooseCustomer(work);
        work->state = 7;
        break;
    case 7:
        FTBL.EV_NPC_NewLoadSw_0(0);
        FTBL.EventFunction869();
        PVSJ_RELEASE_SUBJECTS(work->owner);
        FTBL.CheckWarmStartNpcRoute(work->customer);
        PvsjSetOverlay(2, work);
        FTBL.SetRefBoardParamsForCharaById(PVSJ_PLAYER, 0);
        point = work->pointCount ? work->boards->points[FTBL.shenGetRandomInt32(work->pointCount - 1) + 1] : work->boards->points[0];
        work->customerActive = 1;
        if (work->pointCount) {
            PvsjMoveCustomer(work->customer, work->boards->points[0], work);
            PvsjMoveCustomer(work->owner, point, work);
            PvsjReleaseVoice(work);
            PvsjLoadVoice(work, work->customer);
            PVSJ_SELECT_CAMERAS(work->customer);
        } else {
            PvsjReleaseVoice(work);
            PvsjLoadVoice(work, work->owner);
            PVSJ_SELECT_CAMERAS(work->owner);
        }
        FTBL.EV_NPC_NewLoadSw_0(1);
        work->state = 9;
        break;
    case 8:
        FTBL.EV_NPC_NewLoadSw_0(0);
        FTBL.EventFunction869();
        PVSJ_RELEASE_SUBJECTS(work->customer);
        if (pvsjCameraBlend == -1) {
            pvsjCameraBlend = 0;
            PvsjSetOverlay(3, work);
        } else
            PvsjRefreshOverlay(3, work);
        FTBL.SetRefBoardParamsForCharaById(PVSJ_PLAYER, 0);
        point = work->pointCount ? work->boards->points[FTBL.shenGetRandomInt32(work->pointCount - 1) + 1] : work->boards->points[0];
        work->sessionCash = FTBL.GetPlayerStat(2);
        oldCustomer = work->customer;
        FTBL.GetCurrentDateTime((DateTime *)&date);
        now = date.day * 1440 + date.hour * 60 + date.minute;
        FTBL.DebugLogToFileRemoved("StartTime:%i,NowTime:%i,DifTime:%i,first:%i\n", work->startMinute, now, now - work->startMinute, work->firstCustomer);
        if ((now - work->startMinute > 90 && !work->firstCustomer) || work->sessionCash < 50 || FTBL.shenGetMinutesUntilNextFrameMinute() <= 0) {
            work->customerActive = 0;
            PvsjReleaseVoice(work);
            PvsjLoadVoice(work, work->owner);
            PVSJ_SELECT_CAMERAS(work->owner);
        } else
            PvsjChooseCustomer(work);
        work->firstCustomer = 0;
        if (work->pointCount) {
            if (!work->customerActive) {
                FTBL.CheckWarmStartNpcRoute(work->owner);
                PvsjMoveCustomer(work->customer, point, work);
                PvsjMoveCustomer(work->owner, work->boards->points[0], work);
                PvsjReleaseVoice(work);
                PvsjLoadVoice(work, work->owner);
                PVSJ_SELECT_CAMERAS(work->owner);
            } else {
                FTBL.CheckWarmStartNpcRoute(work->customer);
                PvsjMoveCustomer(oldCustomer, point, work);
                PvsjMoveCustomer(work->customer, work->boards->points[0], work);
                PvsjReleaseVoice(work);
                PvsjLoadVoice(work, work->customer);
                PVSJ_SELECT_CAMERAS(work->customer);
            }
        } else {
            PvsjReleaseVoice(work);
            PvsjLoadVoice(work, work->owner);
            PVSJ_SELECT_CAMERAS(work->owner);
        }
        FTBL.EV_NPC_NewLoadSw_0(1);
        work->state = 9;
        break;
    case 9:
        FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(PVSJ_PLAYER), 0);
        if (pvsjCameraBlend == -1) {
            pvsjCameraBlend = 0;
            PvsjSetOverlay(3, work);
            work->stateTicks = 0;
            work->state = 10;
        } else if (pvsjCameraBlend != 255) {
            PvsjRefreshOverlay(3, work);
            work->stateTicks = 0;
            work->state = 10;
        } else {
            PvsjRefreshOverlay(3, work);
            work->state = 11;
        }
        break;
    case 10:
        FTBL.SetPlayerGameFlags(5, 0);
        FTBL.SetCharacterAlpha(FTBL.ctrlGetCharaByName(PVSJ_PLAYER), (30.0f - work->stateTicks) / 30.0f);
        pvsjCameraBlend = work->stateTicks / 30.0f * 255.0f;
        PvsjRefreshOverlay(3, work);
        if (work->stateTicks < 30)
            work->stateTicks++;
        else
            work->state = 11;
        break;
    case 11:
        FTBL.SetCharacterAlpha(FTBL.ctrlGetCharaByName(PVSJ_PLAYER), 1.0f);
        FTBL.EV_DisableCharacterDisplay(PVSJ_PLAYER);
        FTBL.SetPlayerGameFlags(5, 1);
        FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(PVSJ_PLAYER), 1);
        pvsjCameraBlend = 255;
        PvsjRefreshOverlay(1, work);
        work->state = 12;
        pvsjSkipMotion = 0;
        work->stateTicks = 0;
        work->callFrames = 0;
        FTBL.EV_EventSetUpHelpIcon_0(145, 0);
        FTBL.CheckHelpIcon(129, 0, 0);
        ResetPatiMotionCache();
        break;
    case 12:
        close = 0;
        if (work->stateTicks > 16200) {
            for (i = 0; i < 10; i++)
                FTBL.DebugLogToFileRemoved("**** PVSJ:TIME OUT(NPC MoveStoS) ****\n");
            PvsjDebugExitCallback(work);
            break;
        }
        work->stateTicks++;
        if (work->customerActive & 1) {
            if (FTBL.ForceLoadNpc(work->customer)) {
                if (FTBL.DoesScenarioCharacterExist(work->customer)) {
                    FTBL.EV_SetCharacterAlphaEnabled(work->customer, 0);
                    FTBL.GetCharaPosition(PVSJ_PLAYER, -1, &player, 0x40000000);
                    FTBL.GetCharaPosition(work->customer, -1, &customer, 0x40000000);
                    customer.x -= player.x;
                    customer.y -= player.y;
                    customer.z -= player.z;
                    dot = FTBL.GetVector3DotProduct(&customer, &customer);
                    if (dot < 25.0f)
                        close = 1;
                }
                if (!FTBL.npcGetFlagFor(work->customer)) {
                    if (FTBL.DoesScenarioCharacterExist(work->owner)) {
                        FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(work->owner), 0, 0);
                        FTBL.EV_SetCharacterAlphaEnabled(work->owner, 1);
                    }
                    work->state = 15;
                    break;
                }
            }
        } else if (FTBL.ForceLoadNpc(work->owner)) {
            if (FTBL.DoesScenarioCharacterExist(work->owner))
                FTBL.EV_SetCharacterAlphaEnabled(work->owner, 0);
            if (!FTBL.npcGetFlagFor(work->owner)) {
                if (FTBL.DoesScenarioCharacterExist(work->customer)) {
                    FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(work->customer), 0, 0);
                    FTBL.EV_SetCharacterAlphaEnabled(work->customer, 1);
                }
                work->state = 35;
                work->sessionCash = FTBL.GetPlayerStat(2);
                if (FTBL.shenGetMinutesUntilNextFrameMinute() <= 0)
                    work->state = 53;
                break;
            }
        }
        if (FTBL.GetCurrentControlInput(0) >> 10 & 1) {
            if (!close)
                work->state = 13;
            else {
                PVSJ_CUSTOMER_DOT()
                if (FTBL.CalculateFixedCosine(3640) > dot)
                    work->state = 13;
                else {
                    pvsjSkipMotion = 1;
                    switch (work->greeting) {
                    case 0:
                        work->state = 29;
                        break;
                    case 1:
                        work->state = 27;
                        break;
                    case 2:
                        work->state = 24;
                        break;
                    case 3:
                        work->state = 21;
                        break;
                    default:
                        FTBL.EventAssertNullsub(__FILE__, 2633);
                        break;
                    }
                }
            }
        } else {
            if (!pvsjSkipMotion)
                PvsjRefreshOverlay(0, work);
            pvsjSkipMotion = 0;
        }
        break;
    case 13:
        FTBL.CheckHelpIcon(1, 0, 0);
        PvsjCallCustomer(work);
        work->state = 14;
        break;
    case 14:
        FTBL.CheckHelpIcon(129, 0, 0);
        work->state = 12;
        work->stateTicks = 0;
        break;
    case 15:
        if (pvsjCustomerCamera != 921)
            PvsjSetMotion(work->customer, 6061, 0.0f, 1.0f, 0, work->delay);
        else
            PvsjSetMotion(work->customer, 6060, 0.0f, 1.0f, 0, work->delay);
        PvsjRefreshOverlay(0, work);
        switch (work->greeting) {
        case 0:
            work->state = 28;
            break;
        case 1:
        case 2:
            work->state = 16;
            break;
        case 3:
            work->state = 20;
            break;
        default:
            FTBL.EventAssertNullsub(__FILE__, 2691);
            break;
        }
        break;
    case 16:
        PvsjRefreshOverlay(0, work);
        work->stateTicks = 0;
        work->state = 17;
        break;
    case 17:
        FTBL.CheckHelpIcon(129, 0, 0);
        PvsjRefreshOverlay(0, work);
        if (++work->stateTicks > 299) {
            work->state = 34;
            break;
        }
        if (FTBL.GetCurrentControlInput(0) >> 10 & 1) {
            PVSJ_CUSTOMER_DOT()
            if (FTBL.CalculateFixedCosine(3640) > dot)
                work->state = 18;
            else {
                switch (work->greeting) {
                case 1:
                    work->state = 26;
                    break;
                case 2:
                    work->state = 23;
                    break;
                default:
                    FTBL.EventAssertNullsub(__FILE__, 2759);
                    break;
                }
            }
        }
        break;
    case 18:
        FTBL.CheckHelpIcon(1, 0, 0);
        PvsjCallCustomer(work);
        work->state = 19;
        break;
    case 19:
        FTBL.CheckHelpIcon(129, 0, 0);
        work->state = 17;
        break;
    case 20:
    case 21:
        PVSJ_DIALOGUE_SUBJECTS(work->customer);
        if (work->state == 21) {
            PvsjRestoreCustomerRoute(work->customer, work);
            PvsjSetCamera(pvsjPlayerCamera, PVSJ_PLAYER, PVSJ_PLAYER, work->delay, work);
            PvsjCallCustomer(work);
        }
        PvsjHandleCustomerInquiry(work);
        work->state = 22;
        break;
    case 23:
    case 24:
        PVSJ_DIALOGUE_SUBJECTS(work->customer);
        if (work->state == 24)
            PvsjRestoreCustomerRoute(work->customer, work);
        PvsjCustomerDeclines(work);
        work->state = 25;
        break;
    case 26:
    case 27:
        PVSJ_DIALOGUE_SUBJECTS(work->customer);
        if (work->state == 27)
            PvsjRestoreCustomerRoute(work->customer, work);
        PvsjWelcomeCustomer(work);
        work->state = 30;
        break;
    case 28:
    case 29:
        PVSJ_DIALOGUE_SUBJECTS(work->customer);
        if (work->state == 29) {
            PvsjRestoreCustomerRoute(work->customer, work);
            PvsjSetCamera(pvsjPlayerCamera, PVSJ_PLAYER, PVSJ_PLAYER, work->delay, work);
            PvsjCallCustomer(work);
        }
        PvsjExplainPartTimeJob(work);
        work->state = 30;
        break;
    case 30:
        PvsjStartCustomerGame(work);
        work->state = 31;
        break;
    case 31:
        ResetPatiMotionCache();
        FTBL.CheckHelpIcon(1, 0, 0);
        FTBL.CheckWarmStartNpcRoute(work->customer);
        pvsjGameArgument[0] = 'P';
        pvsjGameArgument[1] = 'V';
        pvsjGameArgument[2] = 'S';
        pvsjGameArgument[3] = 'J';
        pvsjGameArgument[4] = work->customer;
        pvsjGameArgument[5] = work->customer >> 8;
        pvsjGameArgument[6] = work->customer >> 16;
        pvsjGameArgument[7] = work->customer >> 24;
        board = FTBL.shenGetRandomInt32(pvsjBoardCount);
        if (!pvsjBoardPositions[board].character)
            board = 0;
        FTBL.DebugLogToFileRemoved("boards = %i\n", pvsjBoardCount);
        FTBL.DebugLogToFileRemoved("board_idx = %i\n", board);
        pvsjGameArgument[8] = pvsjBoardPositions[board].character;
        pvsjGameArgument[9] = pvsjBoardPositions[board].character >> 8;
        pvsjGameArgument[10] = pvsjBoardPositions[board].character >> 16;
        pvsjGameArgument[11] = pvsjBoardPositions[board].character >> 24;
        pvsjGameArgument[12] = work->owner;
        pvsjGameArgument[13] = work->owner >> 8;
        pvsjGameArgument[14] = work->owner >> 16;
        pvsjGameArgument[15] = work->owner >> 24;
        pvsjGameArgument[16] = 0;
        PvsjReleaseVoice(work);
        if (work->gamePartition != PVSJ_INVALID_PARTITION) {
            FTBL.DebugLogToFileRemoved("EV_UNMOUNT_AFS_PARTITION [PATI].afs %i\n", work->gamePartition);
            FTBL.EV_ControlPackageResources(1, 1, work->gamePartition, 0, 0);
            work->gamePartition = PVSJ_INVALID_PARTITION;
        }
        partition = work->customerPartition;
        if (work->customer == work->owner) {
            FTBL.CheckWarmStartNpcRoute(work->customer);
            pvsjCustomerModelIndex = 0;
        } else {
            for (pvsjCustomerModelIndex = 0; pvsjCustomerPartitions[pvsjCustomerModelIndex].character &&
                 pvsjCustomerPartitions[pvsjCustomerModelIndex].character != work->customer;
                 pvsjCustomerModelIndex++)
                ;
            if (!pvsjCustomerPartitions[pvsjCustomerModelIndex].character)
                FTBL.EventAssertNullsub(__FILE__, 2926);
        }
        work->customerPartition = pvsjCustomerModelIndex;
        FTBL.EV_NPC_NewLoadSw_0(0);
        FTBL.EventFunction869();
        FTBL.EV_ResetFreeTalkFlags();
        FTBL.DebugLogToFileRemoved("EV_PAKLOAD_AFS [PVSJ] %i,%i,%i\n", work->jobPartition, pvsjCustomerPartitions[work->customerPartition].models,
            pvsjCustomerPartitions[work->customerPartition].hands);
        if (partition != -1)
            FTBL.EventAssertNullsub(__FILE__, 2934);
        FTBL.EV_ControlPackageResources(0, 8, work->jobPartition, pvsjCustomerPartitions[work->customerPartition].models,
            pvsjCustomerPartitions[work->customerPartition].hands);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        switch (pvsjCustomerPartitions[work->customerPartition].highHands) {
        case 0:
        default:
            handPosition.x = -0.11279297f;
            handPosition.y = -0.050003052f;
            handPosition.z = 0.093688965f;
            break;
        case 1:
            handPosition.x = -0.097198486f;
            handPosition.y = -0.045295715f;
            handPosition.z = 0.073486328f;
            break;
        }
        handRotation.x = 0;
        handRotation.y = 0;
        handRotation.z = 0xf1c7;
        FTBL.matrix_stack_Matrix4x4_Push(0);
        FTBL.matrix_stack_UnitCurrentMatrix();
        FTBL.Matrix4x4_RotateAroundZ_Wrapper(handRotation.z);
        FTBL.Matrix4x4_RotateAroundY_Wrapper(handRotation.y);
        FTBL.Matrix4x4_RotateAroundX_Wrapper(handRotation.x);
        FTBL.matrix_stack_TranslateVector(&handPosition);
        FTBL.Matrix4x4_GetTranslationRow(&handPosition);
        FTBL.DebugLogToFileRemoved("PVSJ:YubiBase(%f,%f,%f)\n", handPosition.x, handPosition.y, handPosition.z);
        FTBL.PopCurrentMatrix(1);
        FTBL.scenApplyPosOrRotToChildId(PVSJ_OPPONENT_HAND, -1, &handPosition, &handRotation, 0);
        handRotation.x = 0;
        handRotation.y = 20935;
        handRotation.z = 0;
        FTBL.EV_AT_SetAngY_0(PVSJ_OPPONENT_HAND, 0x38000000, &handRotation, 0);
        PvsjBeginGame(pvsjGameArgument, work);
        work->state = 32;
        break;
    case 32:
        FTBL.EV_EventSetUpHelpIcon_0(145, 0);
        FTBL.DebugLogToFileRemoved("EV_FREE_PAKFILE_AFS [PVSJ]\n");
        if (work->customerPartition == -1)
            FTBL.EventAssertNullsub(__FILE__, 2986);
        FTBL.EV_ControlPackageResources(0, 12, work->jobPartition, pvsjCustomerPartitions[work->customerPartition].models,
            pvsjCustomerPartitions[work->customerPartition].hands);
        work->customerPartition = -1;
        FTBL.SetPlayerControlEnabled(0);
        FTBL.npcLockUnlock(work->customer);
        FTBL.DebugLogToFileRemoved("PVSJ:New-Load [PATI]afs\n");
        work->gamePartition = FTBL.EV_ControlPackageResources(1, 0, FTBL.Filepath_Gen(7), (int)"PATI.afs", 0);
        FTBL.DebugLogToFileRemoved("EV_PAKLOAD_AFS [PATI] %i\n", work->gamePartition);
        if (work->gamePartition == PVSJ_INVALID_PARTITION)
            FTBL.EventAssertNullsub(__FILE__, 2996);
        partition = (int)FTBL.EV_ControlPackageResources(0, 5, "PATI", 0, 0);
        if (!partition) {
            FTBL.DebugLogToFileRemoved("PVSJ:Re-Load [PATI]PAK\n");
            i = (int)FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(7), (int)"PATI", 0);
            FTBL.DebugLogToFileRemoved("PVSJ:EV_PAKLOAD = %i\n", i);
            i = (int)FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
            FTBL.DebugLogToFileRemoved("PVSJ:EV_WAIT_PAKLOAD = %i\n", i);
        } else
            FTBL.DebugLogToFileRemoved("PVSJ:Already-Load [PATI]PAK\n");
        work->cameraId = 0;
        work->cameraEye = 0;
        work->cameraTarget = 0;
        EventDebugCheck(PVSJ_DEBUG_ID, 4);
        work->state = 34;
        break;
    case 22:
    case 25:
        work->state = 34;
        break;
    case 34:
        if (pvsjCameraBlend == -1) {
            pvsjCameraBlend = 0;
            PvsjSetOverlay(3, work);
        } else
            PvsjRefreshOverlay(3, work);
        work->state = 8;
        break;
    case 35:
        PVSJ_DIALOGUE_SUBJECTS(work->owner);
        FTBL.GetCharaPosition(PVSJ_PLAYER, -1, &player, 0x40000000);
        FTBL.GetCharaPosition(work->owner, -1, &player, 0x40000000);
        work->delay = 5;
        if (FTBL.shenGetMinutesUntilNextFrameMinute() > 0)
            work->state = 36;
        else
            work->state = 54;
        break;
    case 36:
        PvsjReportShift(work);
        work->state = 37;
        break;
    case 37:
        work->sessionCash = FTBL.GetPlayerStat(2);
        PvsjShowWallet(work);
        work->state = 38;
        break;
    case 38:
        FTBL.SetPlayerStat(2, work->previousCash);
        if (work->sessionCash > 300)
            work->state = 39;
        else
            work->state = 44;
        break;
    case 39:
        PvsjOfferPay(work);
        work->state = 40;
        break;
    case 40:
        work->newCash = (work->sessionCash - 299) / 2;
        PvsjSetCamera(pvsjPlayerCamera, PVSJ_PLAYER, PVSJ_PLAYER, work->delay, work);
        EVT_AT_StartMoneyCount(work->newCash, work->delay);
        work->delay += 5;
        work->state = 41;
        break;
    case 41:
        PvsjAskToContinue(work);
        work->state = 42;
        break;
    case 42:
        PvsjShowChoice(0xffff, 37, 34, 0xffff, work);
        work->state = 43;
        break;
    case 43:
        if (FTBL.shenGetCtrlSpeakIconFlag() == 37)
            work->state = 47;
        else
            work->state = 48;
        break;
    case 44:
        PvsjAskWithoutProfit(work);
        work->state = 45;
        break;
    case 45:
        PvsjShowChoice(0xffff, 37, 34, 0xffff, work);
        work->state = 46;
        break;
    case 46:
        if (FTBL.shenGetCtrlSpeakIconFlag() == 37)
            work->state = 47;
        else
            work->state = 50;
        break;
    case 47:
        if (FTBL.shenGetMinutesUntilNextFrameMinute() <= 0)
            work->state = 52;
        else {
            PvsjContinueShift(work);
            work->state = 5;
        }
        break;
    case 48:
        PvsjRestoreSpeech(work);
        work->state = 49;
        break;
    case 49:
        PvsjDeclineContinue(work);
        work->state = 59;
        break;
    case 50:
        PvsjRestoreSpeech(work);
        work->state = 51;
        break;
    case 51:
        PvsjFinishWithoutProfit(work);
        work->state = 59;
        break;
    case 52:
        PvsjTimeExpired(work);
        work->state = 59;
        break;
    case 53:
        PVSJ_DIALOGUE_SUBJECTS(work->owner);
        FTBL.GetCharaPosition(PVSJ_PLAYER, -1, &player, 0x40000000);
        work->delay = 5;
        work->state = 54;
        break;
    case 54:
        PvsjClosingReport(work);
        work->state = 55;
        break;
    case 55:
        work->sessionCash = FTBL.GetPlayerStat(2);
        PvsjShowWallet(work);
        work->state = 56;
        break;
    case 56:
        FTBL.SetPlayerStat(2, work->previousCash);
        if (work->sessionCash > 300)
            work->state = 57;
        else
            work->state = 51;
        break;
    case 57:
        PvsjOfferPay(work);
        work->state = 58;
        break;
    case 58:
        work->newCash = (work->sessionCash - 299) / 2;
        PvsjSetCamera(pvsjPlayerCamera, PVSJ_PLAYER, PVSJ_PLAYER, work->delay, work);
        EVT_AT_StartMoneyCount(work->newCash, work->delay);
        work->delay += 5;
        work->state = 59;
        break;
    case 59:
        work->previousCash = FTBL.GetPlayerStat(2);
        work->state = 60;
        break;
    case 60:
        FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        FTBL.ProcessAicaCommand(2720, 30);
        work->state = 61;
        break;
    case 61:
        if (!FTBL.filmCheckFadeTrackColour()) {
            FTBL.SetRefBoardParamsForCharaById(PVSJ_PLAYER, 0);
            EVT_AT_SetFaceLooks(PVSJ_PLAYER, 0, 10, 1.0f, 0);
            FTBL.ReleaseCharacterFaceCache(FTBL.ctrlGetCharaByName(PVSJ_PLAYER));
            if (FTBL.DoesScenarioCharacterExist(work->owner)) {
                EVT_AT_SetFaceLooks(work->owner, 0, 10, 1.0f, 0);
                FTBL.ReleaseCharacterFaceCache(FTBL.ctrlGetCharaByName(work->owner));
                FTBL.SetRefBoardParamsForCharaById(work->owner, 0);
                FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(work->owner), 0, 0);
                FTBL.EV_SetCharacterAlphaEnabled(work->owner, 1);
            }
            FTBL.npcLockUnlock(work->owner);
            if (work->customer) {
                if (FTBL.DoesScenarioCharacterExist(work->customer)) {
                    EVT_AT_SetFaceLooks(work->customer, 0, 10, 1.0f, 0);
                    FTBL.ReleaseCharacterFaceCache(FTBL.ctrlGetCharaByName(work->customer));
                    FTBL.SetRefBoardParamsForCharaById(work->customer, 0);
                    FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(work->customer), 0, 0);
                    FTBL.EV_SetCharacterAlphaEnabled(work->customer, 1);
                }
                FTBL.npcLockUnlock(work->customer);
            }
            PvsjFinish(work);
        }
        break;
    default:
        FTBL.DebugLogToFileRemoved("PVSJ:Illegal Main State\n");
        FTBL.EventAssertNullsub(__FILE__, 3266);
        break;
    }
    if (previous != work->state)
        FTBL.DebugLogToFileRemoved("PVSJ State = %i \n", work->state);
    if (work->delay > 0)
        FTBL.HLib_SetupTaskDelay(work->delay);
}
void PvsjInitializeBoards(PvsjEventWork *work)
{
    PatiTableData tables;
    Vector3 player, delta;
    char area[5];
    int i, j, board, nearest;
    float distance, candidate;
    shen_name owner;
    nearest = i = 0;
    for (j = 0; j < 4; j++)
        area[j] = g_currentMap >> j * 8;
    area[4] = 0;
    FTBL.GetCharaPosition(PVSJ_PLAYER, -1, &player, 0x40000000);
    PATI_TBL_Parser(&tables, "PATI_TBL");
    distance = 100000000.0f;
    FTBL.DebugLogToFileRemoved("init board tableplace[%s]\n", area);
    for (; tables.tables[i].area[0]; i++) {
        if (strcmp(area, tables.tables[i].area))
            continue;
        for (board = tables.tables[i].firstBoard; board < tables.tables[i].firstBoard + tables.tables[i].boardCount; board++) {
            delta.x = player.x - tables.tables[i].position.x;
            delta.y = player.y - tables.tables[i].position.y;
            delta.z = player.z - tables.tables[i].position.z;
            candidate = FTBL.GetVector3DotProduct(&delta, &delta);
            FTBL.DebugLogToFileRemoved("place[%i].dist %f\n", i, candidate);
            if (distance > candidate) {
                distance = candidate;
                nearest = i;
            }
        }
    }
    FTBL.DebugLogToFileRemoved("near place = [%s] %s\n", tables.tables[nearest].tableName, tables.tables[nearest].npc);
    owner = PVSJ_READ_NAME(tables.tables[nearest].npc);
    work->owner = owner;
    i = 0;
    pvsjBoardCount = 0;
    for (; tables.tables[i].area[0]; i++) {
        if (strcmp(area, tables.tables[i].area))
            continue;
        owner = PVSJ_READ_NAME(tables.tables[i].npc);
        if (owner != work->owner)
            continue;
        for (board = tables.tables[i].firstBoard; board < tables.tables[i].firstBoard + tables.tables[i].boardCount; board++) {
            pvsjBoardPositions[pvsjBoardCount].character = PVSJ_READ_NAME(tables.boards[board].character);
            FTBL.GetCharaPosition(pvsjBoardPositions[pvsjBoardCount].character, -1, &pvsjBoardPositions[pvsjBoardCount].position, 0x40000000);
            FTBL.DebugLogToFileRemoved("board = [%s] \n", tables.boards[board].character);
            pvsjBoardCount++;
        }
    }
    pvsjBoardPositions[pvsjBoardCount].character = 0;
    FTBL.DebugLogToFileRemoved("boards = %i\n", pvsjBoardCount);
    for (j = 0; pvsjBoardGroups[j].area; j++)
        if (pvsjBoardGroups[j].area == g_currentMap && pvsjBoardGroups[j].owner == work->owner) {
            work->boards = &pvsjBoardGroups[j];
            break;
        }
    if (!pvsjBoardGroups[j].area)
        FTBL.EventAssertNullsub(__FILE__, 3358);
    for (j = 0; work->boards->customers[j].character; j++)
        work->boards->customers[j].currentPoint = work->boards->customers[j].initialPoint;
    for (j = 0; work->boards->points[j + 1]; j++)
        ;
    work->pointCount = j;
    FTBL.DebugLogToFileRemoved("pos = %i\n", j);
    for (j = 0; work->boards->customers[j + 1].kind == 1; j++)
        ;
    work->admittedCustomers = j;
    FTBL.DebugLogToFileRemoved("npc = %i\n", j);
    for (j = 0; work->boards->customers[j + 1].character; j++)
        ;
    work->missingCustomers = j - work->admittedCustomers;
    FTBL.DebugLogToFileRemoved("mis = %i\n", work->missingCustomers);
}

void ResetPatiMotionCache(void)
{
    int i;
    for (i = 0; i < 4; i++) {
        g_patiMotionCache[i].character = 0;
        g_patiMotionCache[i].age = i;
    }
}

void PvsjSetMotion(shen_name character, int motion, float frame, float speed, unsigned int flags, int delay)
{
    shen_name text[2];
    int i, oldest;
    if (pvsjSkipMotion) {
        FTBL.DebugLogToFileRemoved("PVSJ:SkipMot(Closer):id=%s,mot=%i,frm=%f,flg=%x\n", text, motion, frame, flags);
        return;
    }
    text[0] = character;
    text[1] = 0;
    for (i = 0; i < 4; i++)
        if (g_patiMotionCache[i].character == character && g_patiMotionCache[i].motion == motion)
            break;
    if (i < 4) {
        g_patiMotionCache[i].age = 0;
        FTBL.DebugLogToFileRemoved("PVSJ:SkipMot:id=%s,mot=%i,frm=%f,flg=%x\n", text, motion, frame, flags);
        for (i = 0; i < 4; i++)
            if (g_patiMotionCache[i].age < 10000)
                g_patiMotionCache[i].age++;
    } else {
        if (delay == g_patiCameraTime)
            flags |= 0x40000000;
        FTBL.DebugLogToFileRemoved("PVSJ:SetMot:id=%s,mot=%i,frm=%f,flg=%x\n", text, motion, frame, flags);
        EVT_AT_SetMot(character, motion, frame, speed, flags, delay);
        oldest = 0;
        for (i = 0; i < 4; i++) {
            if (g_patiMotionCache[oldest].age > g_patiMotionCache[i].age)
                oldest = i;
            if (!g_patiMotionCache[i].character || g_patiMotionCache[i].character == character) {
                oldest = i;
                break;
            }
        }
        g_patiMotionCache[oldest].character = character;
        g_patiMotionCache[oldest].motion = motion;
        g_patiMotionCache[oldest].age = 0;
    }
    for (i = 0; i < 4; i++) {
        text[0] = g_patiMotionCache[i].character;
        text[1] = 0;
        FTBL.DebugLogToFileRemoved("cur_mot[%i]:id=%s,mot=%i,tch=%i\n", i, text, g_patiMotionCache[i].motion, g_patiMotionCache[i].age);
    }
}

void PvsjSavePlayerRotation(void)
{
    FTBL.GetScenarioCharacterEulerAngles(PVSJ_PLAYER, &pvsjSavedPlayerRotation, 0x40000000);
}

void PvsjRefreshOverlay(int mode, PvsjEventWork *work)
{
    PvsjUpdateOverlay(mode, work);
}

void PvsjSetOverlay(int mode, PvsjEventWork *work)
{
    PvsjUpdateOverlay(mode, work);
    pvsjFilteredEye.x = pvsjDesiredEye.x;
    pvsjFilteredEye.y = pvsjDesiredEye.y;
    pvsjFilteredEye.z = pvsjDesiredEye.z;
    pvsjFilteredTarget.x = pvsjDesiredTarget.x;
    pvsjFilteredTarget.y = pvsjDesiredTarget.y;
    pvsjFilteredTarget.z = pvsjDesiredTarget.z;
    PvsjUpdateOverlay(mode, work);
}

void PvsjUpdateOverlay(int mode, PvsjEventWork *work)
{
    Vector3 point, far;
    Vector3i rotation, angle;
    Vector3 position;
    float x, y, blend, trigger, eyeScale, targetScale;
    int fov, current;
    trigger = 0.0f;
    FTBL.matrix_stack_Matrix4x4_Push(0);
    FTBL.GetCharaPosition(PVSJ_PLAYER, -1, &position, 0x40000000);
    FTBL.GetScenarioCharacterEulerAngles(PVSJ_PLAYER, &rotation, 0x40000000);
    x = (unsigned char)FTBL.GetCurrentControlInput(5);
    y = (unsigned char)FTBL.GetCurrentControlInput(6);
    switch (mode) {
    case 0:
    case 1:
    default:
        blend = 255.0f;
        break;
    case 2:
        blend = 0.0f;
        break;
    case 3:
        blend = pvsjCameraBlend;
        break;
    }
    y /= 255.0f;
    x /= 255.0f;
    blend /= 255.0f;
    point.x = 0.0f;
    point.y = 0.0f;
    point.z = -0.5f;
    angle.x = (int)((0.5f - y) * 2 * 45.0f * 8192.0f / 45.0f) & 0xffff;
    angle.y = (int)((0.5f - x) * 2 * 60.0f * 8192.0f / 45.0f) & 0xffff;
    angle.z = 0;
    FTBL.matrix_stack_UnitCurrentMatrix();
    FTBL.Matrix4x4_RotateAroundY_Wrapper(rotation.z);
    FTBL.Matrix4x4_RotateAroundY_Wrapper(rotation.y);
    FTBL.Matrix4x4_RotateAroundY_Wrapper(rotation.x);
    FTBL.Matrix4x4_RotateAroundZ_Wrapper(angle.z);
    FTBL.Matrix4x4_RotateAroundY_Wrapper(angle.y);
    FTBL.Matrix4x4_RotateAroundX_Wrapper(angle.x);
    FTBL.Matrix4x4_TransformPoint(&point, &point);
    eyeScale = blend + blend - 1.0f;
    targetScale = blend + blend;
    pvsjDesiredEye.x = (position.x + 0.0f) + point.x * eyeScale;
    pvsjDesiredEye.y = (position.y + 1.6f) + point.y * eyeScale;
    pvsjDesiredEye.z = (position.z + 0.0f) + point.z * eyeScale;
    pvsjDesiredTarget.x = (position.x + 0.0f) + point.x * targetScale;
    pvsjDesiredTarget.y = (position.y + 1.6f) + point.y * targetScale;
    pvsjDesiredTarget.z = (position.z + 0.0f) + point.z * targetScale;
    far.x = position.x + point.x * 5.0f;
    far.y = position.y + point.y * 5.0f;
    far.z = position.z + point.z * 5.0f;
    FTBL.PopCurrentMatrix(1);
    pvsjFilteredEye.x += (pvsjDesiredEye.x - pvsjFilteredEye.x) * 0.125f;
    pvsjFilteredEye.y += (pvsjDesiredEye.y - pvsjFilteredEye.y) * 0.125f;
    pvsjFilteredEye.z += (pvsjDesiredEye.z - pvsjFilteredEye.z) * 0.125f;
    pvsjFilteredTarget.x += (pvsjDesiredTarget.x - pvsjFilteredTarget.x) * 0.125f;
    pvsjFilteredTarget.y += (pvsjDesiredTarget.y - pvsjFilteredTarget.y) * 0.125f;
    pvsjFilteredTarget.z += (pvsjDesiredTarget.z - pvsjFilteredTarget.z) * 0.125f;
    work->cameraId = 0;
    work->cameraEye = 0;
    work->cameraTarget = 0;
    FTBL.camCtrlSetState(26);
    FTBL.camCtrlSetNextPosition(&pvsjFilteredEye);
    FTBL.camCtrlSetTargetPosition(&pvsjFilteredTarget);
    if (mode == 0)
        trigger = (unsigned char)(FTBL.GetNormalTriggerMapping() ? FTBL.GetCurrentControlInput(4) : FTBL.GetCurrentControlInput(3));
    trigger /= 255.0f;
    if (trigger > 1.0f)
        trigger = 1.0f;
    if (0.0f > trigger)
        trigger = 0.0f;
    trigger = 60.0f - trigger * 30.0f;
    fov = (short)(int)(trigger * 8192.0f / 45.0f);
    current = FTBL.camCtrlGetPerspective();
    SHENMUE_EVENT_BOUNDARY();
    FTBL.camCtrlSetPerspective((short)(current + (fabs(trigger) >= 1.1920929e-07f ? (fov - current) / 2 : (fov - current) / 4)));
}

void PvsjBeginGame(void *argument, PvsjEventWork *work)
{
    EventDebugGetState(PVSJ_DEBUG_ID, 4);
    if (FTBL.DoesScenarioCharacterExist(work->owner)) {
        FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(work->owner), 0, 0);
        FTBL.EV_SetCharacterAlphaEnabled(work->owner, 1);
    }
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(PVSJ_PLAYER), 71, 0, 0);
    FTBL.LoadAreaFunc_Wrapper_0("PATI.EV3", 0);
    FTBL.ExecuteCurrAreaMainFunc("PATI.EV3", argument);
    FTBL.HLib_SetupTaskDelay(0);
}
#endif
#endif
