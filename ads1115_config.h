#ifndef ADS1115_ADS115_CONFIG_H
#define ADS1115_ADS115_CONFIG_H

#include "ads1115_registers.h"
#include <stddef.h>
#include <stdint.h>

typedef float float32_t;

typedef enum {
    ADS1115_ERR_OK = 0,
    ADS1115_ERR_FAIL = 1 << 0,
    ADS1115_ERR_NULL = 1 << 1,
} ads1115_err_t;

typedef enum {
    ADS1115_SLAVE_ADDRESS_AD0_GND = 0b1001000,
    ADS1115_SLAVE_ADDRESS_AD0_VDD = 0b1001001,
    ADS1115_SLAVE_ADDRESS_AD0_SDA = 0b1001010,
    ADS1115_SLAVE_ADDRESS_AD0_SCL = 0b1001011,
} ads1115_slave_address_t;

typedef enum {
    ADS1115_REG_ADDRESS_CONVERSION = 0b00,
    ADS1115_REG_ADDRESS_CONFIG = 0b01,
    ADS1115_REG_ADDRESS_LO_THRESH = 0b10,
    ADS1115_REG_ADDRESS_HI_THRESH = 0b11,
} ads1115_reg_address_t;

typedef enum {
    ADS1115_CHANNEL_0 = 0b000,
    ADS1115_CHANNEL_1 = 0b001,
    ADS1115_CHANNEL_2 = 0b010,
    ADS1115_CHANNEL_3 = 0b011,
    ADS1115_CHANNEL_4 = 0b100,
    ADS1115_CHANNEL_5 = 0b101,
    ADS1115_CHANNEL_6 = 0b110,
    ADS1115_CHANNEL_7 = 0b111,
} ads1115_channel_t;

typedef enum {
    ADS1115_MODE_CONTINUOUS = 0b00,
    ADS1115_MODE_SINGLE_SHOT = 0b01,
} ads1115_mode_t;

typedef enum {
    ADS1115_FULL_SCALE_RANGE_6V144 = 0b000,
    ADS1115_FULL_SCALE_RANGE_4V096 = 0b001,
    ADS1115_FULL_SCALE_RANGE_2V048 = 0b010,
    ADS1115_FULL_SCALE_RANGE_1V024 = 0b011,
    ADS1115_FULL_SCALE_RANGE_0V512 = 0b100,
    ADS1115_FULL_SCALE_RANGE_0V256 = 0b110,
} ads1115_full_scale_range_t;

typedef enum {
    ADS1115_OUTPUT_DATA_RATE_8HZ = 0b000,
    ADS1115_OUTPUT_DATA_RATE_16HZ = 0b001,
    ADS1115_OUTPUT_DATA_RATE_32HZ = 0b010,
    ADS1115_OUTPUT_DATA_RATE_64HZ = 0b011,
    ADS1115_OUTPUT_DATA_RATE_128HZ = 0b100,
    ADS1115_OUTPUT_DATA_RATE_250HZ = 0b101,
    ADS1115_OUTPUT_DATA_RATE_475HZ = 0b110,
    ADS1115_OUTPUT_DATA_RATE_860HZ = 0b111,
} ads1115_output_data_rate_t;

typedef enum {
    ADS1115_COMP_MODE_TRADITIONAL = 0b00,
    ADS1115_COMP_MODE_WINDOW = 0b01,
} ads1115_comp_mode_t;

typedef enum {
    ADS1115_COMP_POLARITY_ACTIVE_LOW = 0b00,
    ADS1115_COMP_POLARITY_ACTIVE_HIGH = 0b01,
} ads1115_comp_polarity_t;

typedef enum {
    ADS1115_COMP_LATCH_EN = 0b01,
    ADS1115_COMP_LATCH_DIS = 0b00,
} ads1115_comp_latch_t;

typedef enum {
    ADS1115_COMP_QUEUE_ASSERT_1 = 0b00,
    ADS1115_COMP_QUEUE_ASSERT_2 = 0b01,
    ADS1115_COMP_QUEUE_ASSERT_4 = 0b10,
    ADS1115_COMP_QUEUE_DISABLE = 0b11,
} ads1115_comp_queue_t;

typedef struct {
    float32_t scale;
} ads1115_config_t;

typedef struct {
    void* bus_user;
    ads1115_err_t (*bus_initialize)(void*);
    ads1115_err_t (*bus_deinitialize)(void*);
    ads1115_err_t (*bus_write_data)(void*, uint8_t, uint8_t const*, size_t);
    ads1115_err_t (*bus_read_data)(void*, uint8_t, uint8_t*, size_t);
} ads1115_interface_t;

inline float ads1115_range_to_scale(ads1115_full_scale_range_t range)
{
    switch (range) {
        case ADS1115_FULL_SCALE_RANGE_6V144:
            return 187.5F;
        case ADS1115_FULL_SCALE_RANGE_4V096:
            return 125.0F;
        case ADS1115_FULL_SCALE_RANGE_2V048:
            return 62.5F;
        case ADS1115_FULL_SCALE_RANGE_1V024:
            return 31.25F;
        case ADS1115_FULL_SCALE_RANGE_0V512:
            return 15.625F;
        case ADS1115_FULL_SCALE_RANGE_0V256:
            return 7.8125F;
        default:
            return 0.0F;
    }
}

#endif // ADS1115_ADS115_CONFIG_H
