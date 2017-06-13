#pragma once

#include <stdio.h>
#include <string.h>

#define IPC_OP_RTS 0x00000000
#define IPC_OP_ADR 0x01000000

#define IPC_OP_VAL_1BY 0x01
#define IPC_OP_VAL_2BY 0x02
#define IPC_OP_VAL_4BY 0x03
#define IPC_OP_VAL_8BY 0x04
#define IPC_OP_VAL_STR 0x05
#define IPC_OP_VAL_SEQ 0x06

#define IPC_OP_TYPE_1BY (IPC_OP_VAL_1BY << 16)
#define IPC_OP_TYPE_2BY (IPC_OP_VAL_2BY << 16)
#define IPC_OP_TYPE_4BY (IPC_OP_VAL_4BY << 16)
#define IPC_OP_TYPE_8BY (IPC_OP_VAL_8BY << 16)
#define IPC_OP_TYPE_STR (IPC_OP_VAL_STR << 16)
#define IPC_OP_TYPE_SEQ (IPC_OP_VAL_SEQ << 16)

#define IPC_OP_SUBTYPE_1BY (IPC_OP_VAL_1BY << 8)
#define IPC_OP_SUBTYPE_2BY (IPC_OP_VAL_2BY << 8)
#define IPC_OP_SUBTYPE_4BY (IPC_OP_VAL_4BY << 8)
#define IPC_OP_SUBTYPE_8BY (IPC_OP_VAL_8BY << 8)
#define IPC_OP_SUBTYPE_STR (IPC_OP_VAL_STR << 8)
#define IPC_OP_SUBTYPE_SEQ (IPC_OP_VAL_SEQ << 8)

extern uint32_t ipc_serialize(uint8_t *buffer, void *sample, const uint32_t *data_ops);

extern uint32_t ipc_deserialize(uint8_t *buffer, void **sample, const uint32_t *data_ops, uint32_t recv_bytes, uint32_t sample_size);

extern uint8_t *serialize(uint8_t *buffer, void *value, uint32_t length, uint32_t *size);

extern uint8_t *deserialize(void *value, uint8_t *buffer, uint32_t length);

extern uint8_t *serialize_u8(uint8_t *buffer, uint8_t *value, uint32_t array_size, uint32_t *size);

extern uint8_t *serialize_u16(uint8_t *buffer, uint16_t *value, uint32_t array_size, uint32_t *size);

extern uint8_t *serialize_u32(uint8_t *buffer, uint32_t *value, uint32_t array_size, uint32_t *size);

extern uint8_t *serialize_str(uint8_t *buffer, char *str, int *size);

extern uint8_t *deserialize_u8(uint8_t *buffer, uint8_t *value, uint32_t array_size);

extern uint8_t *deserialize_u16(uint8_t *buffer, uint16_t *value, uint32_t array_size);

extern uint8_t *deserialize_u32(uint8_t *buffer, uint32_t *value, uint32_t array_size);

extern uint8_t *deserialize_str(uint8_t *buffer, char **str);
