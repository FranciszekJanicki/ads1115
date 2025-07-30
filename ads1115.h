#ifndef ADS1115_ADS1115_H
#define ADS1115_ADS1115_H

#include "ads1115_config.h"
#include "ads1115_registers.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    ads1115_config_t config;
    ads1115_interface_t interface;
} ads1115_t;

ads1115_err_t ads1115_initialize(ads1115_t* ads1115,
                                 ads1115_config_t const* config,
                                 ads1115_interface_t const* interface);
ads1115_err_t ads1115_deinitialize(ads1115_t* ads1115);

ads1115_err_t ads1115_get_current_channel_voltage_data_scaled(
    ads1115_t const* ads1115,
    float32_t* scaled);
ads1115_err_t ads1115_get_channel_voltage_data_scaled(ads1115_t const* ads1115,
                                                      ads1115_channel_t channel,
                                                      float32_t* scaled);

ads1115_err_t ads1115_get_current_channel_voltage_data_raw(
    ads1115_t const* ads1115,
    int16_t* raw);
ads1115_err_t ads1115_get_channel_voltage_data_raw(ads1115_t const* ads1115,
                                                   ads1115_channel_t channel,
                                                   int16_t* raw);

ads1115_err_t ads1115_trigger_oneshot_conversion(ads1115_t const* ads1115);
ads1115_err_t ads1115_select_channel(ads1115_t const* ads1115,
                                     ads1115_channel_t channel);

ads1115_err_t ads1115_get_conversion_reg(ads1115_t const* ads1115,
                                         ads1115_conversion_reg_t* reg);

ads1115_err_t ads1115_get_config_reg(ads1115_t const* ads1115,
                                     ads1115_config_reg_t* reg);
ads1115_err_t ads1115_set_config_reg(ads1115_t const* ads1115,
                                     ads1115_config_reg_t const* reg);

ads1115_err_t ads1115_get_lo_thresh_reg(ads1115_t const* ads1115,
                                        ads1115_lo_thresh_reg_t* reg);
ads1115_err_t ads1115_set_lo_thresh_reg(ads1115_t const* ads1115,
                                        ads1115_lo_thresh_reg_t const* reg);

ads1115_err_t ads1115_get_hi_thresh_reg(ads1115_t const* ads1115,
                                        ads1115_hi_thresh_reg_t* reg);
ads1115_err_t ads1115_set_hi_thresh_reg(ads1115_t const* ads1115,
                                        ads1115_hi_thresh_reg_t const* reg);

#ifdef __cplusplus
}
#endif

#endif // ADS1115_ADS1115_H
