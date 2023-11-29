#ifndef DSCSHANDLERS_H
#define DSCSHANDLERS_H

typedef void (*PacketHandlerFunc)(void *inputData);

class DSCSHandlers
{
public:
    static void bootstrapHandler(void *arg);
    static void getSensorsResponseHandler(void *arg);
    static void getIndicatorsResponseHandler(void *arg);
    static void chronoUpdateRequestHandler(void *arg);
    static void aliveResponseHandler(void *arg);
    static void getIndicatorDataResponseHandler(void *arg);
};

#endif // DSCSHANDLERS_H
