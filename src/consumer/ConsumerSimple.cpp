#include "ConsumerSimple.h"
#include "../NetworkConfig.h"

#include <iostream>

using namespace vision;

int ConsumerSimple::main(int argc, char* argv[])
{
    vision.start(CONSUMER_ENDPOINT, argc, argv);

    vision.enableBroadcast(Default, this);
    IceUtil::ThreadControl::sleep(IceUtil::Time::seconds(1));
    vision.disableBroadcast(Default, this);

    vision.enablePolling(Default);
    Blob image = vision.getImage(Default);
    std::cout << "Requested Image of size " << image.size() << " bytes." << std::endl;
    vision.disablePolling(Default);

    return 0;
}

void ConsumerSimple::receiveImage(const Blob& image, ImageType imageType)
{
    std::cout << "Received Image of size " << image.size() << " bytes." << std::endl;
}
