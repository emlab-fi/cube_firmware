/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.5 */

#ifndef PB_CUBE_PB_H_INCLUDED
#define PB_CUBE_PB_H_INCLUDED
#include "pb.h"

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Enum definitions */
typedef enum _instruction { 
    instruction_nop = 0, 
    instruction_status_i = 1, 
    instruction_move_to = 2, 
    instruction_get_abs_pos = 3, 
    instruction_get_rel_pos = 4, 
    instruction_set_zero_pos = 5, 
    instruction_reset_zero_pos = 6, 
    instruction_set_coordinate_mode = 7, 
    instruction_home = 8, 
    instruction_spi_transfer = 9, 
    instruction_i2c_transfer = 10, 
    instruction_set_gpio_mode = 11, 
    instruction_set_gpio = 12, 
    instruction_get_gpio = 13, 
    instruction_set_parameter = 14, 
    instruction_get_paramete = 15 
} instruction;

typedef enum _coordinate_mode { 
    coordinate_mode_cartesian = 0, 
    coordinate_mode_cylindrical = 1, 
    coordinate_mode_spherical = 2 
} coordinate_mode;

/* Struct definitions */
typedef struct _data_reply_submsg { 
    uint32_t length; 
    pb_byte_t data[64]; 
} data_reply_submsg;

typedef struct _gpio_config_submsg { 
    uint32_t index; 
    bool value; 
} gpio_config_submsg;

typedef struct _i2c_transfer_submsg { 
    uint32_t rx_length; 
    uint32_t tx_length; 
    uint32_t address; 
    pb_byte_t data[64]; 
} i2c_transfer_submsg;

typedef struct _param_config_submsg { 
    uint32_t id; 
    uint32_t value; 
} param_config_submsg;

/* we use a, b, c as the position can be both cartesian
 or spherical/cylindrical */
typedef struct _position { 
    float a; 
    float b; 
    float c; 
} position;

typedef struct _spi_transfer_submsg { 
    uint32_t cs; 
    uint32_t length; 
    pb_byte_t data[64]; 
} spi_transfer_submsg;

typedef struct _status_msg { 
    uint32_t error_id; 
    coordinate_mode mode; 
    bool has_pos;
    position pos; /* more stuff to follow */
} status_msg;

/* main message types */
typedef struct _command_msg { 
    uint32_t id; 
    instruction inst; 
    bool has_stat;
    status_msg stat; 
    pb_size_t which_payload;
    union {
        coordinate_mode mode;
        spi_transfer_submsg spi;
        i2c_transfer_submsg i2c;
        gpio_config_submsg gpio;
        param_config_submsg param;
    } payload; 
} command_msg;

typedef struct _reply_msg { 
    uint32_t id; 
    bool has_stat;
    status_msg stat; 
    pb_size_t which_payload;
    union {
        data_reply_submsg data;
        bool gpio_status;
        uint32_t param_value;
    } payload; 
} reply_msg;


/* Helper constants for enums */
#define _instruction_MIN instruction_nop
#define _instruction_MAX instruction_get_paramete
#define _instruction_ARRAYSIZE ((instruction)(instruction_get_paramete+1))

#define _coordinate_mode_MIN coordinate_mode_cartesian
#define _coordinate_mode_MAX coordinate_mode_spherical
#define _coordinate_mode_ARRAYSIZE ((coordinate_mode)(coordinate_mode_spherical+1))


#ifdef __cplusplus
extern "C" {
#endif

/* Initializer values for message structs */
#define position_init_default                    {0, 0, 0}
#define spi_transfer_submsg_init_default         {0, 0, {0}}
#define i2c_transfer_submsg_init_default         {0, 0, 0, {0}}
#define data_reply_submsg_init_default           {0, {0}}
#define gpio_config_submsg_init_default          {0, 0}
#define param_config_submsg_init_default         {0, 0}
#define command_msg_init_default                 {0, _instruction_MIN, false, status_msg_init_default, 0, {_coordinate_mode_MIN}}
#define reply_msg_init_default                   {0, false, status_msg_init_default, 0, {data_reply_submsg_init_default}}
#define status_msg_init_default                  {0, _coordinate_mode_MIN, false, position_init_default}
#define position_init_zero                       {0, 0, 0}
#define spi_transfer_submsg_init_zero            {0, 0, {0}}
#define i2c_transfer_submsg_init_zero            {0, 0, 0, {0}}
#define data_reply_submsg_init_zero              {0, {0}}
#define gpio_config_submsg_init_zero             {0, 0}
#define param_config_submsg_init_zero            {0, 0}
#define command_msg_init_zero                    {0, _instruction_MIN, false, status_msg_init_zero, 0, {_coordinate_mode_MIN}}
#define reply_msg_init_zero                      {0, false, status_msg_init_zero, 0, {data_reply_submsg_init_zero}}
#define status_msg_init_zero                     {0, _coordinate_mode_MIN, false, position_init_zero}

/* Field tags (for use in manual encoding/decoding) */
#define data_reply_submsg_length_tag             1
#define data_reply_submsg_data_tag               2
#define gpio_config_submsg_index_tag             1
#define gpio_config_submsg_value_tag             2
#define i2c_transfer_submsg_rx_length_tag        1
#define i2c_transfer_submsg_tx_length_tag        2
#define i2c_transfer_submsg_address_tag          3
#define i2c_transfer_submsg_data_tag             4
#define param_config_submsg_id_tag               1
#define param_config_submsg_value_tag            2
#define position_a_tag                           1
#define position_b_tag                           2
#define position_c_tag                           3
#define spi_transfer_submsg_cs_tag               1
#define spi_transfer_submsg_length_tag           2
#define spi_transfer_submsg_data_tag             3
#define status_msg_error_id_tag                  1
#define status_msg_mode_tag                      2
#define status_msg_pos_tag                       3
#define command_msg_id_tag                       1
#define command_msg_inst_tag                     2
#define command_msg_stat_tag                     3
#define command_msg_mode_tag                     4
#define command_msg_spi_tag                      5
#define command_msg_i2c_tag                      6
#define command_msg_gpio_tag                     7
#define command_msg_param_tag                    8
#define reply_msg_id_tag                         1
#define reply_msg_stat_tag                       2
#define reply_msg_data_tag                       3
#define reply_msg_gpio_status_tag                4
#define reply_msg_param_value_tag                5

/* Struct field encoding specification for nanopb */
#define position_FIELDLIST(X, a_) \
X(a_, STATIC,   SINGULAR, FLOAT,    a,                 1) \
X(a_, STATIC,   SINGULAR, FLOAT,    b,                 2) \
X(a_, STATIC,   SINGULAR, FLOAT,    c,                 3)
#define position_CALLBACK NULL
#define position_DEFAULT NULL

#define spi_transfer_submsg_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   cs,                1) \
X(a, STATIC,   SINGULAR, UINT32,   length,            2) \
X(a, STATIC,   SINGULAR, FIXED_LENGTH_BYTES, data,              3)
#define spi_transfer_submsg_CALLBACK NULL
#define spi_transfer_submsg_DEFAULT NULL

#define i2c_transfer_submsg_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   rx_length,         1) \
X(a, STATIC,   SINGULAR, UINT32,   tx_length,         2) \
X(a, STATIC,   SINGULAR, UINT32,   address,           3) \
X(a, STATIC,   SINGULAR, FIXED_LENGTH_BYTES, data,              4)
#define i2c_transfer_submsg_CALLBACK NULL
#define i2c_transfer_submsg_DEFAULT NULL

#define data_reply_submsg_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   length,            1) \
X(a, STATIC,   SINGULAR, FIXED_LENGTH_BYTES, data,              2)
#define data_reply_submsg_CALLBACK NULL
#define data_reply_submsg_DEFAULT NULL

#define gpio_config_submsg_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   index,             1) \
X(a, STATIC,   SINGULAR, BOOL,     value,             2)
#define gpio_config_submsg_CALLBACK NULL
#define gpio_config_submsg_DEFAULT NULL

#define param_config_submsg_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   id,                1) \
X(a, STATIC,   SINGULAR, UINT32,   value,             2)
#define param_config_submsg_CALLBACK NULL
#define param_config_submsg_DEFAULT NULL

#define command_msg_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   id,                1) \
X(a, STATIC,   SINGULAR, UENUM,    inst,              2) \
X(a, STATIC,   OPTIONAL, MESSAGE,  stat,              3) \
X(a, STATIC,   ONEOF,    UENUM,    (payload,mode,payload.mode),   4) \
X(a, STATIC,   ONEOF,    MESSAGE,  (payload,spi,payload.spi),   5) \
X(a, STATIC,   ONEOF,    MESSAGE,  (payload,i2c,payload.i2c),   6) \
X(a, STATIC,   ONEOF,    MESSAGE,  (payload,gpio,payload.gpio),   7) \
X(a, STATIC,   ONEOF,    MESSAGE,  (payload,param,payload.param),   8)
#define command_msg_CALLBACK NULL
#define command_msg_DEFAULT NULL
#define command_msg_stat_MSGTYPE status_msg
#define command_msg_payload_spi_MSGTYPE spi_transfer_submsg
#define command_msg_payload_i2c_MSGTYPE i2c_transfer_submsg
#define command_msg_payload_gpio_MSGTYPE gpio_config_submsg
#define command_msg_payload_param_MSGTYPE param_config_submsg

#define reply_msg_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   id,                1) \
X(a, STATIC,   OPTIONAL, MESSAGE,  stat,              2) \
X(a, STATIC,   ONEOF,    MESSAGE,  (payload,data,payload.data),   3) \
X(a, STATIC,   ONEOF,    BOOL,     (payload,gpio_status,payload.gpio_status),   4) \
X(a, STATIC,   ONEOF,    UINT32,   (payload,param_value,payload.param_value),   5)
#define reply_msg_CALLBACK NULL
#define reply_msg_DEFAULT NULL
#define reply_msg_stat_MSGTYPE status_msg
#define reply_msg_payload_data_MSGTYPE data_reply_submsg

#define status_msg_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   error_id,          1) \
X(a, STATIC,   SINGULAR, UENUM,    mode,              2) \
X(a, STATIC,   OPTIONAL, MESSAGE,  pos,               3)
#define status_msg_CALLBACK NULL
#define status_msg_DEFAULT NULL
#define status_msg_pos_MSGTYPE position

extern const pb_msgdesc_t position_msg;
extern const pb_msgdesc_t spi_transfer_submsg_msg;
extern const pb_msgdesc_t i2c_transfer_submsg_msg;
extern const pb_msgdesc_t data_reply_submsg_msg;
extern const pb_msgdesc_t gpio_config_submsg_msg;
extern const pb_msgdesc_t param_config_submsg_msg;
extern const pb_msgdesc_t command_msg_msg;
extern const pb_msgdesc_t reply_msg_msg;
extern const pb_msgdesc_t status_msg_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define position_fields &position_msg
#define spi_transfer_submsg_fields &spi_transfer_submsg_msg
#define i2c_transfer_submsg_fields &i2c_transfer_submsg_msg
#define data_reply_submsg_fields &data_reply_submsg_msg
#define gpio_config_submsg_fields &gpio_config_submsg_msg
#define param_config_submsg_fields &param_config_submsg_msg
#define command_msg_fields &command_msg_msg
#define reply_msg_fields &reply_msg_msg
#define status_msg_fields &status_msg_msg

/* Maximum encoded size of messages (where known) */
#define command_msg_size                         121
#define data_reply_submsg_size                   72
#define gpio_config_submsg_size                  8
#define i2c_transfer_submsg_size                 84
#define param_config_submsg_size                 12
#define position_size                            15
#define reply_msg_size                           107
#define spi_transfer_submsg_size                 78
#define status_msg_size                          25

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
