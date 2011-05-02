#include "VisionCom.h"
#include "../NetworkConfig.h"

#include <iostream>
#include <algorithm>

using namespace vision;

void VisionCom::start(const std::string& endpoint, int argc, char* argv[])
{
    int status;

    try
    {
        appName = argv[0];

        // create a communicator
        communicator = Ice::initialize(argc, argv);

        // retrieve the producer
        Ice::ObjectPrx base = communicator->stringToProxy(PRODUCER);
        producer = ImageProviderPrx::checkedCast(base);

        int myHardwareId = 1234; // TODO: figure out hardware id
        storageType = (producer->getHardwareId() == myHardwareId) ? SharedMemory : Embedded;

        // start up Ice server
        Ice::ObjectAdapterPtr adapter = communicator->createObjectAdapterWithEndpoints("ImageReceiver", endpoint);
        Ice::Identity id = communicator->stringToIdentity("ImageReceiver");
        adapter->add(this, id);
        adapter->activate();
        thisProxy = adapter->createProxy(id);

        // get topic manager
        topicManager = IceStorm::TopicManagerPrx::checkedCast(communicator->stringToProxy(TOPIC_MANAGER));
    }
    catch(const std::exception& ex)
    {
        std::cerr << appName << ": " << ex.what() << std::endl;
        status = EXIT_FAILURE;
    }
    catch(const std::string& msg)
    {
        std::cerr << appName << ": " << msg << std::endl;
        status = EXIT_FAILURE;
    }
    catch(const char* msg)
    {
        std::cerr << appName << ": " << msg << std::endl;
        status = EXIT_FAILURE;
    }
    catch(...)
    {
        std::cerr << appName << ": unknown exception" << std::endl;
        status = EXIT_FAILURE;
    }
}

void VisionCom::stop()
{
    if (communicator)
    {
        communicator->destroy();
        communicator = NULL;
    }
}

void VisionCom::enableBroadcast(ImageType imageType, ImageReceiver* receiver)
{
    std::string topicName = (storageType == Embedded) ? "ImageReceiverEmbedded" : "ImageReceiverSharedMemory";
    try
    {
        topicPrx = topicManager->retrieve(topicName);
    }
    catch (const IceStorm::NoSuchTopic&)
    {
        topicPrx = topicManager->create(topicName);
    }

    IceStorm::QoS qos;
    topicPrx->subscribeAndGetPublisher(qos, thisProxy->ice_oneway());

    BroadcastReceiverMap::iterator it = broadcastCallbacks.find(imageType);
    if (it == broadcastCallbacks.end())
    {
        ReceiverVector v;
        v.push_back(receiver);

        broadcastCallbacks[imageType] = v;
    }
    else
    {
        it->second.push_back(receiver);
    }

    producer->enableBroadcast(storageType, imageType);
}

void VisionCom::disableBroadcast(ImageType imageType, ImageReceiver* receiver)
{
    BroadcastReceiverMap::iterator it = broadcastCallbacks.find(imageType);
    if (it != broadcastCallbacks.end())
    {
        ReceiverVector::iterator vit = std::find(it->second.begin(), it->second.end(), receiver);
        if (vit != it->second.end())
        {
            it->second.erase(vit);
        }

        if (it->second.empty())
        {
            broadcastCallbacks.erase(it);
        }
    }

    if (broadcastCallbacks.empty())
    {
        topicPrx->unsubscribe(thisProxy->ice_oneway());
    }

    producer->disableBroadcast(storageType, imageType);
}

void VisionCom::enablePolling(ImageType imageType)
{
    producer->enablePolling(storageType, imageType);
}

void VisionCom::disablePolling(ImageType imageType)
{
    producer->disablePolling(storageType, imageType);
}

Blob VisionCom::getImage(ImageType imageType)
{
    switch (storageType)
    {
        case Embedded:
            return producer->getImageBlob(imageType);

        case SharedMemory:
        {
            SharedMemorySegment segment = producer->getImageSharedMemory(imageType);
            Blob image;
            // TODO: retrieve image from shared memory
            return image;
        }
    }
}

void VisionCom::receiveImageEmbedded(const Blob& image, ImageType iType, const Ice::Current& ctx)
{
    BroadcastReceiverMap::iterator it = broadcastCallbacks.find(iType);
    if (it != broadcastCallbacks.end())
    {
        ReceiverVector::iterator vit;
        for (vit = it->second.begin(); vit != it->second.end(); ++vit)
        {
            (*vit)->receiveImage(image, iType);
        }
    }
}

void VisionCom::receiveImageSharedMemory(const SharedMemorySegment& segment, ImageType iType, const Ice::Current& ctx)
{
    Blob image;
    // TODO: retrieve image from shared memory

    BroadcastReceiverMap::iterator it = broadcastCallbacks.find(iType);
    if (it != broadcastCallbacks.end())
    {
        ReceiverVector::iterator vit;
        for (vit = it->second.begin(); vit != it->second.end(); ++vit)
        {
            (*vit)->receiveImage(image, iType);
        }
    }
}

