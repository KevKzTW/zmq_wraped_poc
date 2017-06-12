#pragma once

#include <stdio.h>
#include <string.h>

extern uint8_t *serialize(uint8_t *buffer, void *value, uint32_t length, uint32_t *size);

extern uint8_t *dserialize(void *value, uint8_t *buffer, uint32_t length, uint32_t *size);

extern uint8_t *serialize_u8(uint8_t *buffer, uint8_t *value, uint32_t array_size, uint32_t *size);

extern uint8_t *serialize_u16(uint8_t *buffer, uint16_t *value, uint32_t array_size, uint32_t *size);

extern uint8_t *serialize_u32(uint8_t *buffer, uint32_t *value, uint32_t array_size, uint32_t *size);

extern uint8_t *serialize_f64(uint8_t *buffer, double *value, uint32_t array_size, uint32_t *size);

extern uint8_t *serialize_str(uint8_t *buffer, char *str, int *size);

extern uint8_t *deserialize_u8(uint8_t *buffer, uint8_t *value, uint32_t array_size, uint32_t *size, uint32_t total_size);

extern uint8_t *deserialize_u16(uint8_t *buffer, uint16_t *value, uint32_t array_size, uint32_t *size, uint32_t total_size);

extern uint8_t *deserialize_u32(uint8_t *buffer, uint32_t *value, uint32_t array_size, uint32_t *size, uint32_t total_size);

extern uint8_t *deserialize_f64(uint8_t *buffer, double *value, uint32_t array_size, uint32_t *size, uint32_t total_size);

extern uint8_t *deserialize_str(uint8_t *buffer, char **str, uint32_t *size, uint32_t total_size);
