#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "ipc_stream.h"

static uint32_t get_member_size(uint32_t key, uint32_t sub_key)
{
	if (key == IPC_OP_VAL_1BY)
		return sizeof(uint8_t);
	else if (key == IPC_OP_VAL_2BY)
		return sizeof(uint16_t);
	else if (key == IPC_OP_VAL_4BY)
		return sizeof(uint32_t);
	else if (key == IPC_OP_VAL_8BY)
		return sizeof(uint64_t);
	else if (key == IPC_OP_VAL_SEQ)
		return get_member_size(sub_key, 0);
	else
		return 0;
}

uint32_t ipc_serialize(uint8_t *buffer, void *sample, const uint32_t *data_ops)
{
	uint32_t send_bytes = 0;
	uint32_t i = 0;

	do
	{
		uint32_t data_type = (data_ops[i] & 0x000F0000) >> 16;
		uint32_t sub_type = (data_ops[i] & 0x00000F00) >> 8;

		if (data_type == IPC_OP_VAL_STR)
		{
			/*
				pack the string type
			*/
			uint32_t member_offset = data_ops[i + 1];
			char **msg = (char *)sample + member_offset;
			buffer = serialize_str(buffer, *msg, &send_bytes);
		}
		else if (data_type == IPC_OP_VAL_SEQ)
		{
			/*
				pack the _length of a sequence type
			*/
			uint32_t member_offset = data_ops[i + 1];
			uint8_t *_length = (uint8_t *)sample + member_offset;
			buffer = serialize(buffer, _length, sizeof(uint32_t), &send_bytes);
			/*
				pack the _buffer of a sequence type
			*/
			uint8_t **_buffer = (uint8_t *)sample + member_offset + sizeof(uint8_t *);
			uint32_t member_size = get_member_size(data_type, sub_type);
			uint32_t L = *((uint32_t *)_length);
			buffer = serialize(buffer, *_buffer, member_size * L, &send_bytes);
		}
		else
		{
			uint32_t member_size = get_member_size(data_type, sub_type);

			if (member_size)
			{
				/*
					pack any single variable type
				*/
				uint32_t member_offset = data_ops[i + 1];
				uint8_t *member = (uint8_t *)sample + member_offset;
				buffer = serialize(buffer, member, member_size, &send_bytes);
			}
			else
				break;
		}
		i += 2;
	} while (1);

	return send_bytes;
}

uint32_t ipc_deserialize(uint8_t *buffer, void **sample, const uint32_t *data_ops, uint32_t recv_bytes, uint32_t sample_size)
{
	uint32_t i = 0;

	uint8_t *this = calloc(1, sample_size);
	do
	{
		uint32_t data_type = (data_ops[i] & 0x000F0000) >> 16;
		uint32_t sub_type = (data_ops[i] & 0x00000F00) >> 8;

		if (data_type == IPC_OP_VAL_STR)
		{
			/*
				unpack the string type
			*/
			uint32_t member_offset = data_ops[i + 1];
			char **msg = this + member_offset;
			buffer = deserialize_str(buffer, msg);
		}
		else if (data_type == IPC_OP_VAL_SEQ)
		{
			/*
				unpack the _length of a sequence type
			*/
			uint32_t member_offset = data_ops[i + 1];
			uint8_t *_length = this + member_offset;
			buffer = deserialize(_length, buffer, sizeof(uint32_t));
			/*
				unpack the _buffer of a sequence type
			*/
			uint32_t member_size = get_member_size(data_type, sub_type);
			uint32_t L = *((uint32_t *)_length);
			uint8_t *temp = calloc(L, member_size);
			buffer = deserialize(temp, buffer, member_size * L);

			uint8_t **_buffer = this + member_offset + sizeof(void *);
			*_buffer = temp;
		}
		else
		{
			uint32_t member_size = get_member_size(data_type, sub_type);

			if (member_size)
			{
				/*
					unpack any single variable type
				*/
				uint32_t member_offset = data_ops[i + 1];
				uint8_t *member = this + member_offset;
				buffer = deserialize(member, buffer, member_size);
			}
			else
				break;
		}
		i += 2;
	} while (1);

	*sample = this;

	return recv_bytes;
}

uint8_t *serialize(uint8_t *buffer, void *value, uint32_t length, uint32_t *size)
{
	memcpy(buffer, value, length);
	if (size)
		*size += length;
	return buffer + length;
}

uint8_t *deserialize(void *value, uint8_t *buffer, uint32_t length)
{
	memcpy(value, buffer, length);
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

uint8_t *serialize_str(uint8_t *buffer, char *str, int *size)
{
	uint16_t length = strlen(str);

	buffer = serialize_u16(buffer, &length, 1, size);

	buffer = serialize_u8(buffer, str, length, size);

	return buffer;
}

uint8_t *deserialize_u8(uint8_t *buffer, uint8_t *value, uint32_t array_size)
{
	uint32_t length = 1 * array_size;
	return deserialize(value, buffer, length);
}

uint8_t *deserialize_u16(uint8_t *buffer, uint16_t *value, uint32_t array_size)
{
	uint32_t length = 2 * array_size;
	return deserialize(value, buffer, length);
}

uint8_t *deserialize_u32(uint8_t *buffer, uint32_t *value, uint32_t array_size)
{
	uint32_t length = 4 * array_size;
	return deserialize(value, buffer, length);
}

uint8_t *deserialize_str(uint8_t *buffer, char **str)
{
	uint16_t length = 0;
	buffer = deserialize_u16(buffer, &length, 1);

	char *msg = calloc(length, sizeof(char));
	buffer = deserialize_u8(buffer, msg, length);

	*str = msg;

	return buffer;
}
