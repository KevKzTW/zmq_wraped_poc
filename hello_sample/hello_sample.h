#pragma once

#include "ipc.h"

typedef struct sequence
{
	uint32_t _length;
	double *_buffer;
} sequence;

typedef struct hello_sample
{
	uint32_t id;
	double data;
	char *msg;
	sequence payload;
} hello_sample;

extern void hello_free(hello_sample *sample);

extern int hello_get_size(void *sample);

extern const ipc_topic_descriptor_t hello_descriptor;
