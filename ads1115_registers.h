#ifndef ADS1115_ADS1115_REGISTERS_H
#define ADS1115_ADS1115_REGISTERS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int16_t conversion : 16;
} ads1115_conversion_reg_t;

typedef struct {
    uint8_t os : 1;
    uint8_t mux : 3;
    uint8_t pga : 3;
    uint8_t mode : 1;
    uint8_t dr : 3;
    uint8_t comp_mode : 1;
    uint8_t comp_pol : 1;
    uint8_t comp_lat : 1;
    uint8_t comp_que : 2;
} ads1115_config_reg_t;

typedef struct {
    int16_t lo_thresh : 16;
} ads1115_lo_thresh_reg_t;

typedef struct {
    int16_t hi_thresh : 16;
} ads1115_hi_thresh_reg_t;

#ifdef __cplusplus
}
#endif

#endif // ADS1115_ADS1115_REGISTERS_H