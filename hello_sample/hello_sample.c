#include "hello_sample.h"

static const uint32_t hello_ops[] =
	{
		IPC_OP_ADR | IPC_OP_TYPE_4BY, offsetof(hello_sample, id),
		IPC_OP_ADR | IPC_OP_TYPE_8BY, offsetof(hello_sample, data),
		IPC_OP_ADR | IPC_OP_TYPE_STR, offsetof(hello_sample, msg),
		IPC_OP_ADR | IPC_OP_TYPE_SEQ | IPC_OP_SUBTYPE_8BY, offsetof(hello_sample, payload),
		IPC_OP_RTS};

void hello_free(hello_sample *sample)
{
	free(sample->msg);
	free(sample->payload._buffer);
	free(sample);
	sample = NULL;
}

int hello_get_size(void *sample)
{
	hello_sample *this = (hello_sample *)sample;

	int size = 0;

	size += sizeof(this->id);
	size += sizeof(this->data);

	size += sizeof(uint16_t);
	size += strlen(this->msg);

	size += sizeof(this->payload._length);
	size += sizeof(double) * this->payload._length;

	return size;
}

const ipc_topic_descriptor_t hello_descriptor =
	{
		sizeof(hello_sample),
		hello_ops,
		hello_get_size,
};
