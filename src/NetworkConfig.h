#ifndef VISIONCOM_NETWORKCONFIG_H
#define VISIONCOM_NETWORKCONFIG_H

namespace vision
{
    const char* const TOPIC_MANAGER = "IceStorm/TopicManager:tcp -p 10000";
    const char* const PRODUCER = "ImageProvider:default -p 9830";
    const char* const PRODUCER_ENDPOINT = "default -p 9830";
    const char* const CONSUMER = "ImageReceiver:default -p 9831";
    const char* const CONSUMER_ENDPOINT = "default -p 9831";
}

#endif