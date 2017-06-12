#include "ipc.h"

void ipc_init(void **context)
{
	*context = zmq_ctx_new();
}

int ipc_create_publisher(void *context, ipc_entity_t *zmq_pub, const char *addr, ipc_topic_descriptor_t *topic)
{
	ipc_entity_t pub = malloc(sizeof(ipc_entity));

	pub->entity = zmq_socket(context, ZMQ_PUB);

	zmq_bind(pub->entity, addr);

	pub->topic = topic;

	*zmq_pub = pub;
}

int ipc_write(ipc_entity_t zmq_pub, void *sample, int flags)
{
	uint32_t _size = zmq_pub->topic->get_size(sample);

	uint8_t *buff = calloc(_size, 1);

	uint32_t size = zmq_pub->topic->serialize(buff, sample);

	uint8_t head[4];
	serialize_u32(head, &size, 1, NULL);
	zmq_send(zmq_pub->entity, head, 4, ZMQ_SNDMORE);

	uint32_t ret = zmq_send(zmq_pub->entity, buff, size, 0);

	free(buff);

	if (ret != _size)
		return -1;

	return ret;
}

int ipc_create_subscriber(void *context, ipc_entity_t *zmq_sub, const char *addr, ipc_topic_descriptor_t *topic)
{
	ipc_entity_t sub = malloc(sizeof(ipc_entity));

	sub->entity = zmq_socket(context, ZMQ_SUB);

	zmq_connect(sub->entity, addr);

	zmq_setsockopt(sub->entity, ZMQ_SUBSCRIBE, "", 0);

	sub->topic = topic;

	*zmq_sub = sub;
}

int ipc_read(ipc_entity_t zmq_sub, void **sample, bool *valid_data)
{
	uint8_t head[4];
	zmq_recv(zmq_sub->entity, head, 4, 0);

	uint32_t _size;
	deserialize_u32(head, &_size, 1, NULL, 4);

	uint8_t *buff = calloc(_size, 1);

	int ret = zmq_recv(zmq_sub->entity, buff, _size, 0);

	if (ret != _size)
	{
		*valid_data = false;
		return -1;
	}
	*valid_data = true;

	void *temp;
	zmq_sub->topic->deserialize(buff, &temp, ret);

	free(buff);
	*sample = temp;

	return ret;
}

int ipc_entity_delete(ipc_entity_t entity)
{
	zmq_close(entity->entity);

	entity->topic = NULL;

	free(entity);
	entity = NULL;
}

int ipc_fini(void *context)
{
	zmq_ctx_term(context);
}

void ipc_sleepfor(uint64_t nanosec)
{
	struct timespec t;
	t.tv_sec = nanosec / IPC_NSECS_IN_SEC;
	t.tv_nsec = (nanosec % IPC_NSECS_IN_SEC);
	nanosleep(&t, NULL);
}
