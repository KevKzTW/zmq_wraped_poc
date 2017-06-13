//  Pubsub envelope publisher
//  Note that the zhelpers.h file also provides s_sendmore

#include "hello_sample.h"

int main(void)
{
    //  Prepare our context and publisher
    hello_sample sample;
    void *context;
    ipc_entity_t publisher;

    ipc_init(&context);
    ipc_create_publisher(context, &publisher, "ipc:///hello", &hello_descriptor);

    sample.id = 1337;
    sample.data = 1.2323232;
    sample.msg = "hello";
    sample.payload._length = 32768;
    sample.payload._buffer = calloc(32768, sizeof(double));

    for (int i = 0; i < 32768; i++)
        sample.payload._buffer[i] = (double)i;

    ipc_sleepfor(IPC_SECS(1));
    while (1)
    {
        ipc_sleepfor(IPC_MSECS(10));
        int ret = ipc_write(publisher, &sample, 0);
        printf("%d\n", ret);
        sample.id++;
    }
    //  We never get here, but clean up anyhow
    ipc_entity_delete(publisher);
    ipc_fini(context);

    return 0;
}
