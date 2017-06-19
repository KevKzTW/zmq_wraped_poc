#pragma once

#define _POSIX_C_SOURCE 199309L

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "zmq.h"
#include "ipc_stream.h"

#define IPC_NSECS_IN_SEC 1000000000LL
#define IPC_NSECS_IN_MSEC 1000000LL
#define IPC_NSECS_IN_USEC 1000LL

#define IPC_SECS(n) ((n)*IPC_NSECS_IN_SEC)
#define IPC_MSECS(n) ((n)*IPC_NSECS_IN_MSEC)
#define IPC_USECS(n) ((n)*IPC_NSECS_IN_USEC)

#define NO_ERROR			0
#define ERROR_DATA_WRITING -1001
#define ERROR_DATA_READING -1002

#define IPC_MODE_SERVER 0
#define IPC_MODE_CLIENT 1

typedef int (*get_size_fn)(void *);

typedef struct ipc_topic_descriptor
{
	uint32_t sample_size;
	const uint32_t *topic_ops;
	get_size_fn get_size;
} ipc_topic_descriptor_t;

typedef struct ipc_entity
{
	void *entity;
	const ipc_topic_descriptor_t *topic;
} ipc_entity;

typedef struct ipc_entity *ipc_entity_t;

extern void ipc_init(void **context);

extern int32_t ipc_create_publisher(void *context, ipc_entity_t *zmq_pub, const char *addr, const ipc_topic_descriptor_t *topic, uint8_t mode);

extern int32_t ipc_write(ipc_entity_t zmq_pub, void *sample, int flags);

extern int32_t ipc_create_subscriber(void *context, ipc_entity_t *zmq_sub, const char *addr, const ipc_topic_descriptor_t *topic, uint8_t mode);

extern int32_t ipc_read(ipc_entity_t zmq_sub, void **sample, bool *valid_data);

extern int32_t ipc_entity_delete(ipc_entity_t entity);

extern int32_t ipc_fini(void *context);

extern void ipc_sleepfor(uint64_t nanosec);
