#include "ConsumerSimple.h"
#include "../NetworkConfig.h"

#include <iostream>

using namespace vision;

int ConsumerSimple::main(int argc, char* argv[])
{
    int status;

    if ((status = receiver.start(CONSUMER_ENDPOINT, argc, argv)))
    {
        return status;
    }

    receiver.enableBroadcast(Default, this);
    IceUtil::ThreadControl::sleep(IceUtil::Time::seconds(1));
    receiver.disableBroadcast(Default, this);

    receiver.enablePolling(Default);
    Blob image = receiver.getImage(Default);
    std::cout << "Requested Image of size " << image.size() << " bytes." << std::endl;
    receiver.disablePolling(Default);

    receiver.stop();

    return 0;
}

void ConsumerSimple::receiveImage(const Blob& image, ImageType imageType)
{
    std::cout << "Received Image of size " << image.size() << " bytes." << std::endl;
}
