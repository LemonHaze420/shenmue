#ifndef SHEN_PLATFORM_H
#define SHEN_PLATFORM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int64_t shen_platform_counter_frequency(void);
int64_t shen_platform_counter(void);
void shen_platform_sleep_nanoseconds(int64_t nanoseconds);

#ifdef __cplusplus
}
#endif

#endif
