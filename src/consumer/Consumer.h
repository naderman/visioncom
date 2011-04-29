#ifndef VISIONCOM_CONSUMER_H
#define VISIONCOM_CONSUMER_H

#include <interface/cpp/Vision.h>

#include <Ice/Ice.h>
#include <IceStorm/IceStorm.h>

namespace vision
{
    class VisionCom_EXPORT Consumer :
        virtual public Ice::Application,
        virtual public ImageReceiver
    {
        public:
            int run(int argc, char* argv[]);

            void receiveImageEmbedded(const Blob& image, const Ice::Current& ctx);
            void receiveImageSharedMemory(const SharedMemorySegment& image, const Ice::Current& ctx);

        protected:
            IceStorm::TopicManagerPrx topicManager;

            IceStorm::TopicPrx embeddedTopicPrx;
            IceStorm::TopicPrx sharedMemoryTopicPrx;
    };
}

#endif
