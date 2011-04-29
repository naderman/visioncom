#include "Producer.h"
#include "../NetworkConfig.h"

#include <iostream>

using namespace vision;

int Producer::run(int argc, char* argv[])
{
    EnabledMap::iterator it;

    IceUtil::Mutex::Lock broadcastLock(broadcastMutex);
    broadcastLock.release();

    // start up Ice server
    Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapterWithEndpoints("ImageProvider", PRODUCER_ENDPOINT);
    Ice::Identity id = communicator()->stringToIdentity("ImageProvider");
    adapter->add(this, id);
    adapter->activate();

    // get topic manager
    topicManager = IceStorm::TopicManagerPrx::checkedCast(communicator()->stringToProxy(TOPIC_MANAGER));

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

    while (!communicator()->isShutdown())
    {
        // broadcast data
        broadcastLock.acquire();

        for (it = broadcastEnabled.begin(); it != broadcastEnabled.end(); ++it)
        {
            if (it->second > 0)
            {
                // TODO: broadcast correct image type based on it->first.second
                switch (it->first.first)
                {
                    case Embedded:
                        embeddedTopic->receiveImageEmbedded(Blob());
                    break;

                    case SharedMemory:
                        sharedMemoryTopic->receiveImageSharedMemory(SharedMemorySegment());
                    break;
                }
            }
        }

        broadcastLock.release();

        // "wait" for next image
        IceUtil::ThreadControl::sleep(IceUtil::Time::milliSeconds(30)); // ~30 images/second
    }

    return 0;
}

void Producer::enableBroadcast(StorageType store, ImageType image, const Ice::Current& ctx)
{
    IceUtil::Mutex::Lock lock(broadcastMutex); // unlocks at end of scope

    ProvisionType type(store, image);
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

void Producer::disableBroadcast(StorageType store, ImageType image, const Ice::Current& ctx)
{
    IceUtil::Mutex::Lock lock(broadcastMutex); // unlocks at end of scope

    ProvisionType type(store, image);
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

void Producer::enablePolling(StorageType store, ImageType image, const Ice::Current& ctx)
{
    IceUtil::Mutex::Lock lock(pollingMutex); // unlocks at end of scope

    ProvisionType type(store, image);
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

void Producer::disablePolling(StorageType store, ImageType image, const Ice::Current& ctx)
{
    IceUtil::Mutex::Lock lock(pollingMutex); // unlocks at end of scope

    ProvisionType type(store, image);
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

Blob Producer::getImageBlob(ImageType image, const Ice::Current& ctx)
{
    IceUtil::Mutex::Lock lock(pollingMutex); // unlocks at end of scope

    Blob data; // TODO: retrieve image from somewhere

    return data;
}

SharedMemorySegment Producer::getImageSharedMemory(ImageType image, const Ice::Current& ctx)
{
    IceUtil::Mutex::Lock lock(pollingMutex); // unlocks at end of scope

    SharedMemorySegment data; // TODO: retrieve image from somewhere
    data.size = 1;
    data.flags = 1;
    data.key = 1;

    return data;
}

int Producer::getHardwareId(const Ice::Current& ctx)
{
    // TODO: generate a unique identifier for this machine
    return 0;
}

