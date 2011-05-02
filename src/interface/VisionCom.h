#ifndef VISIONCOM_VISIONCOM_H
#define VISIONCOM_VISIONCOM_H

#include <interface/cpp/Vision.h>

#include <Ice/Ice.h>
#include <IceStorm/IceStorm.h>

namespace vision
{
    class VisionCom_EXPORT ImageReceiver
    {
        public:
            virtual void receiveImage(const Blob&, ImageType) = 0;
    };

    class VisionCom_EXPORT VisionCom :
        virtual public ImageReceiverGeneric
    {
        public:
            typedef std::vector<ImageReceiver*> ReceiverVector;
            typedef std::map<ImageType, ReceiverVector> BroadcastReceiverMap;

            void start(const std::string& endpoint, int argc, char* argv[]);
            void stop();

            void enableBroadcast(ImageType imageType, ImageReceiver* receiver);
            void disableBroadcast(ImageType imageType, ImageReceiver* receiver);
            void enablePolling(ImageType imageType);
            void disablePolling(ImageType imageType);
            Blob getImage(ImageType imageType);

            void receiveImageEmbedded(const Blob& image, ImageType iType, const Ice::Current& ctx);
            void receiveImageSharedMemory(const SharedMemorySegment& image, ImageType iType, const Ice::Current& ctx);

            Ice::CommunicatorPtr communicator;

        protected:
            std::string appName;
            Ice::ObjectPrx thisProxy;

            IceStorm::TopicManagerPrx topicManager;
            IceStorm::TopicPrx topicPrx;
            ImageProviderGenericPrx producer;

            StorageType storageType;
            BroadcastReceiverMap broadcastCallbacks;
    };
}

#endif
