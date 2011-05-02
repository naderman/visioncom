#include "VisionComSender.h"
#include "../NetworkConfig.h"

#include <iostream>

using namespace vision;

int VisionComSender::start(const std::string& endpoint, int argc, char* argv[], ImageProvider* provider)
{
    thisPtr = this;
    thisPtr->__setNoDelete(true);

    int status = 0;

    try
    {
        imageProvider = provider;
        appName = argv[0];

        // create a communicator
        communicator = Ice::initialize(argc, argv);

        // TODO: generate a unique identifier for this machine
        hardwareId = 4567;

        // start up Ice server
        Ice::ObjectAdapterPtr adapter = communicator->createObjectAdapterWithEndpoints("ImageProvider", endpoint);
        Ice::Identity id = communicator->stringToIdentity("ImageProvider");
        adapter->add(this, id);
        adapter->activate();
        thisProxy = adapter->createProxy(id);

        // get topic manager
        topicManager = IceStorm::TopicManagerPrx::checkedCast(communicator->stringToProxy(TOPIC_MANAGER));

        // get topic proxies to publish to
        IceStorm::TopicPrx embeddedTopicPrx;
        IceStorm::TopicPrx sharedMemoryTopicPrx;
        try
        {
            embeddedTopicPrx = topicManager->retrieve("ImageReceiverEmbedded");
        }
        catch (const IceStorm::NoSuchTopic&)
        {
            embeddedTopicPrx = topicManager->create("ImageReceiverEmbedded");
        }
        try
        {
            sharedMemoryTopicPrx = topicManager->retrieve("ImageReceiverSharedMemory");
        }
        catch (const IceStorm::NoSuchTopic&)
        {
            sharedMemoryTopicPrx = topicManager->create("ImageReceiverSharedMemory");
        }

        embeddedTopic = ImageReceiverEmbeddedPrx::uncheckedCast(embeddedTopicPrx->getPublisher()->ice_oneway());
        sharedMemoryTopic = ImageReceiverSharedMemoryPrx::uncheckedCast(sharedMemoryTopicPrx->getPublisher()->ice_oneway());

    }
    catch (const std::exception& ex)
    {
        std::cerr << appName << ": " << ex.what() << std::endl;
        status = EXIT_FAILURE;
    }
    catch (const std::string& msg)
    {
        std::cerr << appName << ": " << msg << std::endl;
        status = EXIT_FAILURE;
    }
    catch (const char* msg)
    {
        std::cerr << appName << ": " << msg << std::endl;
        status = EXIT_FAILURE;
    }
    catch (...)
    {
        std::cerr << appName << ": unknown exception" << std::endl;
        status = EXIT_FAILURE;
    }

    return status;
}

void VisionComSender::stop()
{
    if (communicator)
    {
        communicator->destroy();
    }
}

void VisionComSender::broadcastImage(const Blob& image, ImageType imageType)
{
    IceUtil::Mutex::Lock lock(broadcastMutex); // unlocks at end of scope
    EnabledMap::iterator it;

    it = broadcastEnabled.find(ProvisionType(Embedded, imageType));
    if (it != broadcastEnabled.end() && it->second > 0)
    {
        embeddedTopic->receiveImageEmbedded(image, it->first.second);
    }

    it = broadcastEnabled.find(ProvisionType(SharedMemory, imageType));
    if (it != broadcastEnabled.end() && it->second > 0)
    {
        // TODO: convert image to SharedMemorySegment
        sharedMemoryTopic->receiveImageSharedMemory(SharedMemorySegment(), it->first.second);
    }
}

std::set<ImageType> VisionComSender::requestedImageTypes()
{
    IceUtil::Mutex::Lock broadcastLock(broadcastMutex); // unlocks at end of scope
    IceUtil::Mutex::Lock pollingLock(pollingMutex); // unlocks at end of scope
    EnabledMap::iterator it;
    std::set<ImageType> types;

    for (it = broadcastEnabled.begin(); it != broadcastEnabled.end(); ++it)
    {
        types.insert(it->first.second);
    }
    for (it = pollingEnabled.begin(); it != pollingEnabled.end(); ++it)
    {
        types.insert(it->first.second);
    }

    return types;
}

bool VisionComSender::isShutdown()
{
    return communicator->isShutdown();
}

void VisionComSender::enableBroadcast(StorageType store, ImageType iType, const Ice::Current& ctx)
{
    IceUtil::Mutex::Lock lock(broadcastMutex); // unlocks at end of scope

    ProvisionType type(store, iType);
    EnabledMap::iterator it = broadcastEnabled.find(type);

    if (it == broadcastEnabled.end())
    {
        broadcastEnabled[type] = 1;
    }
    else
    {
        it->second++;
    }
    std::cout << "enabled broadcast" << std::endl;
}

void VisionComSender::disableBroadcast(StorageType store, ImageType iType, const Ice::Current& ctx)
{
    IceUtil::Mutex::Lock lock(broadcastMutex); // unlocks at end of scope

    ProvisionType type(store, iType);
    EnabledMap::iterator it = broadcastEnabled.find(type);

    if (it == broadcastEnabled.end())
    {
        // nothing to do, already disabled
    }
    else if (it->second > 0)
    {
        it->second--;
    }
    std::cout << "disabled broadcast" << std::endl;
}

void VisionComSender::enablePolling(StorageType store, ImageType iType, const Ice::Current& ctx)
{
    IceUtil::Mutex::Lock lock(pollingMutex); // unlocks at end of scope

    ProvisionType type(store, iType);
    EnabledMap::iterator it = pollingEnabled.find(type);

    if (it == pollingEnabled.end())
    {
        pollingEnabled[type] = 1;
    }
    else
    {
        it->second++;
    }
    std::cout << "enabled polling" << std::endl;
}

void VisionComSender::disablePolling(StorageType store, ImageType iType, const Ice::Current& ctx)
{
    IceUtil::Mutex::Lock lock(pollingMutex); // unlocks at end of scope

    ProvisionType type(store, iType);
    EnabledMap::iterator it = pollingEnabled.find(type);

    if (it == pollingEnabled.end())
    {
        // nothing to do, already disabled
    }
    else if (it->second > 0)
    {
        it->second--;
    }
    std::cout << "disabled polling" << std::endl;
}

Blob VisionComSender::getImageBlob(ImageType iType, const Ice::Current& ctx)
{
    IceUtil::Mutex::Lock lock(pollingMutex); // unlocks at end of scope

    return imageProvider->getImage(iType);
}

SharedMemorySegment VisionComSender::getImageSharedMemory(ImageType iType, const Ice::Current& ctx)
{
    IceUtil::Mutex::Lock lock(pollingMutex); // unlocks at end of scope

    Blob image = imageProvider->getImage(iType);
    // TODO: turn image into shared memory

    return SharedMemorySegment();
}

int VisionComSender::getHardwareId(const Ice::Current& ctx)
{
    return hardwareId;
}

