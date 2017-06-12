#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "ipc_stream.h"

static bool is_buff_valid(uint32_t length, uint32_t *size, uint32_t total_size)
{
	uint32_t remain = 0;
	if (size)
		remain = total_size - *size;
	else
		remain = total_size;

	if (remain >= length)
		return true;
	return false;
}

uint8_t *serialize(uint8_t *buffer, void *value, uint32_t length, uint32_t *size)
{
	memcpy(buffer, value, length);
	if (size)
		*size += length;
	return buffer + length;
}

uint8_t *dserialize(void *value, uint8_t *buffer, uint32_t length, uint32_t *size)
{
	memcpy(value, buffer, length);
	if (size)
		*size += length;
	return buffer + length;
}

uint8_t *serialize_u8(uint8_t *buffer, uint8_t *value, uint32_t array_size, uint32_t *size)
{
	return serialize(buffer, value, 1 * array_size, size);
}

uint8_t *serialize_u16(uint8_t *buffer, uint16_t *value, uint32_t array_size, uint32_t *size)
{
	return serialize(buffer, value, 2 * array_size, size);
}

uint8_t *serialize_u32(uint8_t *buffer, uint32_t *value, uint32_t array_size, uint32_t *size)
{
	return serialize(buffer, value, 4 * array_size, size);
}

uint8_t *serialize_f64(uint8_t *buffer, double *value, uint32_t array_size, uint32_t *size)
{
	return serialize(buffer, value, 8 * array_size, size);
}

uint8_t *serialize_str(uint8_t *buffer, char *str, int *size)
{
	uint16_t length = strlen(str);

	buffer = serialize_u16(buffer, &length, 1, size);

	buffer = serialize_u8(buffer, str, length, size);

	return buffer;
}

uint8_t *deserialize_u8(uint8_t *buffer, uint8_t *value, uint32_t array_size, uint32_t *size, uint32_t total_size)
{
	uint32_t length = 1 * array_size;
	if (is_buff_valid(length, size, total_size))
		return dserialize(value, buffer, length, size);
	return buffer;
}

uint8_t *deserialize_u16(uint8_t *buffer, uint16_t *value, uint32_t array_size, uint32_t *size, uint32_t total_size)
{
	uint32_t length = 2 * array_size;
	if (is_buff_valid(length, size, total_size))
		return dserialize(value, buffer, length, size);
	return buffer;
}

uint8_t *deserialize_u32(uint8_t *buffer, uint32_t *value, uint32_t array_size, uint32_t *size, uint32_t total_size)
{
	uint32_t length = 4 * array_size;
	if (is_buff_valid(length, size, total_size))
		return dserialize(value, buffer, length, size);
	return buffer;
}

uint8_t *deserialize_f64(uint8_t *buffer, double *value, uint32_t array_size, uint32_t *size, uint32_t total_size)
{
	uint32_t length = 8 * array_size;
	if (is_buff_valid(length, size, total_size))
		return dserialize(value, buffer, length, size);
	return buffer;
}

uint8_t *deserialize_str(uint8_t *buffer, char **str, uint32_t *size, uint32_t total_size)
{
	uint16_t length = 0;
	buffer = deserialize_u16(buffer, &length, 1, size, total_size);

	if (length > 0)
	{
		char *msg = calloc(length, 1);
		buffer = deserialize_u8(buffer, msg, length, size, total_size);

		*str = msg;
	}

	return buffer;
}
