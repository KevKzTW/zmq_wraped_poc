//  Pubsub envelope subscriber

#include "hello_sample.h"

int main(void)
{
    //  Prepare our context and subscriber
    void *context = NULL;
    ipc_entity_t subscriber;

    ipc_init(&context);
    ipc_create_subscriber(context, &subscriber, "ipc:///hello", &hello_descriptor);

    hello_sample *sample_ptr;
    void *samples[1] = {0};
    bool valid_data;

    while (1)
    {
        int ret = ipc_read(subscriber, samples, &valid_data);
        if ((ret > 0) && valid_data)
        {

            sample_ptr = samples[0];

            hello_free(sample_ptr);
        }
        else if (ret == -1)
            break;
        ipc_sleepfor(IPC_USECS(100));
    }
    //  We never get here, but clean up anyhow
    ipc_entity_delete(subscriber);
    ipc_fini(context);
    return 0;
}
