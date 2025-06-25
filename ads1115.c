#include "ads1115.h"
#include <assert.h>
#include <string.h>

static ads1115_err_t ads1115_bus_init(ads1115_t const* ads1115)
{
    return ads1115->interface.bus_init ? ads1115->interface.bus_init(ads1115->interface.bus_user)
                                       : ADS1115_ERR_NULL;
}

static ads1115_err_t ads1115_bus_deinit(ads1115_t const* ads1115)
{
    return ads1115->interface.bus_deinit
               ? ads1115->interface.bus_deinit(ads1115->interface.bus_user)
               : ADS1115_ERR_NULL;
}

static ads1115_err_t ads1115_bus_write(ads1115_t const* ads1115,
                                       uint8_t address,
                                       uint8_t const* data,
                                       size_t data_size)
{
    return ads1115->interface.bus_write
               ? ads1115->interface.bus_write(ads1115->interface.bus_user, address, data, data_size)
               : ADS1115_ERR_NULL;
}

static ads1115_err_t ads1115_bus_read(ads1115_t const* ads1115,
                                      uint8_t address,
                                      uint8_t* data,
                                      size_t data_size)
{
    return ads1115->interface.bus_read
               ? ads1115->interface.bus_read(ads1115->interface.bus_user, address, data, data_size)
               : ADS1115_ERR_NULL;
}

ads1115_err_t ads1115_initialize(ads1115_t* ads1115,
                                 ads1115_config_t* config,
                                 ads1115_interface_t* interface)
{
    assert(ads1115 && config && interface);

    memset(ads1115, 0, sizeof(*ads1115));
    memcpy(&ads1115->config, config, sizeof(*config));
    memcpy(&ads1115->interface, interface, sizeof(*interface));

    return ads1115_bus_init(ads1115);
}

ads1115_err_t ads1115_deinitialize(ads1115_t* ads1115)
{
    assert(ads1115);

    ads1115_err_t err = ads1115_bus_deinit(ads1115);

    memset(ads1115, 0, sizeof(*ads1115));

    return err;
}

ads1115_err_t ads1115_get_conversion_scaled(ads1115_t const* ads1115, float32_t* scaled)
{
    assert(ads1115);

    int16_t raw = {};

    ads1115_err_t err = ads1115_get_conversion_raw(ads1115, &raw);

    *scaled = (float32_t)raw * ads1115->config.scale;

    return err;
}

ads1115_err_t ads1115_get_mux_conversion_scaled(ads1115_t const* ads1115,
                                                ads1115_mux_t mux,
                                                float32_t* scaled)
{
    assert(ads1115 && scaled);

    ads1115_err_t err = ads1115_select_channel(ads1115, mux);
    err |= ads1115_get_conversion_scaled(ads1115, scaled);

    return err;
}

ads1115_err_t ads1115_get_conversion_raw(ads1115_t const* ads1115, int16_t* raw)
{
    assert(ads1115);

    ads1115_conversion_reg_t reg = {};

    ads1115_err_t err = ads1115_get_conversion_reg(ads1115, &reg);

    *raw = reg.conversion;

    return err;
}

ads1115_err_t ads1115_get_mux_conversion_raw(ads1115_t const* ads1115,
                                             ads1115_mux_t mux,
                                             int16_t* raw)
{
    assert(ads1115 && raw);

    ads1115_err_t err = ads1115_select_channel(ads1115, mux);
    err |= ads1115_get_conversion_raw(ads1115, raw);

    return err;
}

ads1115_err_t ads1115_trigger_oneshot_conversion(ads1115_t const* ads1115)
{
    assert(ads1115);

    ads1115_config_reg_t reg = {};

    ads1115_err_t err = ads1115_get_config_reg(ads1115, &reg);
    reg.os = 1U;
    err |= ads1115_set_config_reg(ads1115, &reg);

    return err;
}

ads1115_err_t ads1115_select_channel(ads1115_t const* ads1115, ads1115_mux_t mux)
{
    assert(ads1115);

    ads1115_config_reg_t reg = {};

    ads1115_err_t err = ads1115_get_config_reg(ads1115, &reg);
    reg.mux = mux;
    err |= ads1115_set_config_reg(ads1115, &reg);

    return err;
}

ads1115_err_t ads1115_get_conversion_reg(ads1115_t const* ads1115, ads1115_conversion_reg_t* reg)
{
    assert(ads1115 && reg);

    uint8_t data[2] = {};

    ads1115_err_t err = ads1115_bus_read(ads1115, ADS1115_REG_ADDR_CONVERSION, data, sizeof(data));

    reg->conversion = (int16_t)(((data[0] << 8) & 0xFF) | (data[1] & 0xFF));

    return err;
}

ads1115_err_t ads1115_get_config_reg(ads1115_t const* ads1115, ads1115_config_reg_t* reg)
{
    assert(ads1115 && reg);

    uint8_t data[2] = {};

    ads1115_err_t err = ads1115_bus_read(ads1115, ADS1115_REG_ADDR_CONFIG, data, sizeof(data));

    reg->os = (data[0] >> 7U) & 0x01U;
    reg->mux = (data[0] >> 4U) & 0x07U;
    reg->pga = (data[0] >> 1U) & 0x07U;
    reg->mode = data[0] & 0x01U;
    reg->dr = (data[1] >> 5U) & 0x07U;
    reg->comp_mode = (data[1] >> 4U) & 0x01U;
    reg->comp_pol = (data[1] >> 3U) & 0x01U;
    reg->comp_lat = (data[1] >> 2U) & 0x01U;
    reg->comp_que = data[1] & 0x03U;

    return err;
}

ads1115_err_t ads1115_set_config_reg(ads1115_t const* ads1115, ads1115_config_reg_t const* reg)
{
    assert(ads1115 && reg);

    uint8_t data[2] = {};

    data[0] |= (reg->os & 0x01U) << 7U;
    data[0] |= (reg->mux & 0x07U) << 4U;
    data[0] |= (reg->pga & 0x07U) << 1U;
    data[0] |= reg->mode & 0x01U;
    data[1] |= (reg->dr & 0x07U) << 5U;
    data[1] |= (reg->comp_mode & 0x01U) << 4U;
    data[1] |= (reg->comp_pol & 0x01U) << 3U;
    data[1] |= (reg->comp_lat & 0x01U) << 2U;
    data[1] |= reg->comp_que & 0x03U;

    return ads1115_bus_write(ads1115, ADS1115_REG_ADDR_CONFIG, data, sizeof(data));
}

ads1115_err_t ads1115_get_lo_thresh_reg(ads1115_t const* ads1115, ads1115_lo_thresh_reg_t* reg)
{
    assert(ads1115 && reg);

    uint8_t data[2] = {};

    ads1115_err_t err = ads1115_bus_read(ads1115, ADS1115_REG_ADDR_LO_THRESH, data, sizeof(data));

    reg->lo_thresh = (int16_t)(((data[0] & 0xFF) << 8) | (data[1] & 0xFF));

    return err;
}

ads1115_err_t ads1115_set_lo_thresh_reg(ads1115_t const* ads1115,
                                        ads1115_lo_thresh_reg_t const* reg)
{
    assert(ads1115 && reg);

    uint8_t data[2] = {};

    data[0] = (uint8_t)((reg->lo_thresh >> 8) & 0xFF);
    data[1] = (uint8_t)(reg->lo_thresh & 0xFF);

    return ads1115_bus_write(ads1115, ADS1115_REG_ADDR_LO_THRESH, data, sizeof(data));
}

ads1115_err_t ads1115_get_hi_thresh_reg(ads1115_t const* ads1115, ads1115_hi_thresh_reg_t* reg)
{
    assert(ads1115 && reg);

    uint8_t data[2] = {};

    ads1115_err_t err = ads1115_bus_read(ads1115, ADS1115_REG_ADDR_HI_THRESH, data, sizeof(data));

    reg->hi_thresh = (int16_t)(((data[0] & 0xFF) << 8) | (data[1] & 0xFF));

    return err;
}

ads1115_err_t ads1115_set_hi_thresh_reg(ads1115_t const* ads1115,
                                        ads1115_hi_thresh_reg_t const* reg)
{
    assert(ads1115 && reg);

    uint8_t data[2] = {};

    data[0] = (uint8_t)((reg->hi_thresh >> 8) & 0xFF);
    data[1] = (uint8_t)(reg->hi_thresh & 0xFF);

    return ads1115_bus_write(ads1115, ADS1115_REG_ADDR_HI_THRESH, data, sizeof(data));
}
