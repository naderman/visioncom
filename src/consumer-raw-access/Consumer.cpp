#include "Consumer.h"
#include "../NetworkConfig.h"

#include <iostream>

using namespace vision;

int Consumer::run(int argc, char* argv[])
{
    // retrieve the producer
    Ice::ObjectPrx base = communicator()->stringToProxy(PRODUCER);
    ImageProviderGenericPrx producer = ImageProviderGenericPrx::checkedCast(base);

    // start up Ice server
    Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapterWithEndpoints("ImageReceiver", CONSUMER_ENDPOINT);
    Ice::Identity id = communicator()->stringToIdentity("ImageReceiver");
    adapter->add(this, id);
    adapter->activate();
    Ice::ObjectPrx thisProxy = adapter->createProxy(id);

    // get topic manager
    topicManager = IceStorm::TopicManagerPrx::checkedCast(communicator()->stringToProxy(TOPIC_MANAGER));

    // get topic proxies to publish to
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

    // subscribe to both types of broadcast
    IceStorm::QoS qos;
    embeddedTopicPrx->subscribeAndGetPublisher(qos, thisProxy->ice_oneway());
    sharedMemoryTopicPrx->subscribeAndGetPublisher(qos, thisProxy->ice_oneway());

    producer->enableBroadcast(Embedded, Default);
    producer->enablePolling(SharedMemory, Default);

    // test polling
    Blob i1 = producer->getImageBlob(Default);
    std::cout << "Polled embedded image of size " << i1.size() << std::endl;

    SharedMemorySegment i2 = producer->getImageSharedMemory(Default);
    std::cout << "Polled shared memory image of size " << i2.size << std::endl;

    // receive a few blob images
    IceUtil::ThreadControl::sleep(IceUtil::Time::seconds(1));

    // enable shared memory as well and receive a few more
    producer->enableBroadcast(SharedMemory, Default);
    IceUtil::ThreadControl::sleep(IceUtil::Time::seconds(1));

    // disable embedded and receive only shared memory
    producer->disableBroadcast(Embedded, Default);
    IceUtil::ThreadControl::sleep(IceUtil::Time::seconds(1));

    // all tests done! now clean up

    // unsubscribe from topics before shutdown
    embeddedTopicPrx->unsubscribe(thisProxy->ice_oneway());
    sharedMemoryTopicPrx->unsubscribe(thisProxy->ice_oneway());

    // disable broadcast & polling on producer
    producer->disableBroadcast(SharedMemory, Default);
    producer->disablePolling(Embedded, Default);
    producer->disablePolling(SharedMemory, Default);

    return 0;
}

void Consumer::receiveImageEmbedded(const Blob& image, ImageType iType, const Ice::Current& ctx)
{
    std::cout << "Received Blob Image of size " << image.size() << " bytes." << std::endl;
}

void Consumer::receiveImageSharedMemory(const SharedMemorySegment& image, ImageType iType, const Ice::Current& ctx)
{
    std::cout << "Received Shared Memory Image of size " << image.size
        << " bytes. (key = " << image.key << ", flags = " << image.flags << ")"
        << std::endl;
}

