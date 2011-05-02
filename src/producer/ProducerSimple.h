#ifndef VISIONCOM_CONSUMER_H
#define VISIONCOM_CONSUMER_H

#include <interface/cpp/Vision.h>

#include <map>
#include <Ice/Ice.h>
#include <IceStorm/IceStorm.h>

namespace vision
{
    class VisionCom_EXPORT Producer :
        virtual public Ice::Application,
        virtual public ImageProviderGeneric
    {
        public:
            typedef std::pair<StorageType, ImageType> ProvisionType;
            typedef std::map<ProvisionType, int> EnabledMap;

            int run(int argc, char* argv[]);

            void enableBroadcast(StorageType store, ImageType image, const Ice::Current& ctx);
            void disableBroadcast(StorageType store, ImageType image, const Ice::Current& ctx);

            void enablePolling(StorageType store, ImageType image, const Ice::Current& ctx);
            void disablePolling(StorageType store, ImageType image, const Ice::Current& ctx);

            Blob getImageBlob(ImageType image, const Ice::Current& ctx);
            SharedMemorySegment getImageSharedMemory(ImageType image, const Ice::Current& ctx);

            int getHardwareId(const Ice::Current& ctx);

        protected:
            EnabledMap broadcastEnabled;
            EnabledMap pollingEnabled;

            IceUtil::Mutex broadcastMutex;
            IceUtil::Mutex pollingMutex;

            IceStorm::TopicManagerPrx topicManager;
            ImageReceiverEmbeddedPrx embeddedTopic;
            ImageReceiverSharedMemoryPrx sharedMemoryTopic;
    };
}

#endif
