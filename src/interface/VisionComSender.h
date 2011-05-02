#ifndef VISIONCOM_VISIONCOMSENDER_H
#define VISIONCOM_VISIONCOMSENDER_H

#include <interface/cpp/Vision.h>

#include <Ice/Ice.h>
#include <IceStorm/IceStorm.h>

namespace vision
{
    class VisionCom_EXPORT ImageProvider
    {
        public:
            virtual void enableBroadcast(ImageType) {};
            virtual void disableBroadcast(ImageType) {};

            virtual void enablePolling(ImageType) {};
            virtual void disablePolling(ImageType) {};
            virtual Blob getImage(ImageType) = 0;
    };

    class VisionCom_EXPORT VisionComSender :
        virtual public ImageProviderGeneric
    {
        public:
            typedef std::pair<StorageType, ImageType> ProvisionType;
            typedef std::map<ProvisionType, int> EnabledMap;

            int start(const std::string& endpoint, int argc, char* argv[], ImageProvider* provider);
            void stop();

            void broadcastImage(const Blob& image, ImageType imageType);
            std::set<ImageType> requestedImageTypes();

            bool isShutdown();

            void enableBroadcast(StorageType store, ImageType iType, const Ice::Current& ctx);
            void disableBroadcast(StorageType store, ImageType iType, const Ice::Current& ctx);

            void enablePolling(StorageType store, ImageType iType, const Ice::Current& ctx);
            void disablePolling(StorageType store, ImageType iType, const Ice::Current& ctx);

            Blob getImageBlob(ImageType iType, const Ice::Current& ctx);
            SharedMemorySegment getImageSharedMemory(ImageType iType, const Ice::Current& ctx);

            int getHardwareId(const Ice::Current& ctx);

            Ice::CommunicatorPtr communicator;

        protected:
            int hardwareId;

            ImageProvider* imageProvider;

            EnabledMap broadcastEnabled;
            EnabledMap pollingEnabled;

            IceUtil::Mutex broadcastMutex;
            IceUtil::Mutex pollingMutex;

            std::string appName;
            Ice::ObjectPrx thisProxy;
            Ice::ObjectPtr thisPtr;

            IceStorm::TopicManagerPrx topicManager;
            ImageReceiverEmbeddedPrx embeddedTopic;
            ImageReceiverSharedMemoryPrx sharedMemoryTopic;
    };
}

#endif
