#include "ConsumerSimple.h"
#include "../NetworkConfig.h"

#include <iostream>

using namespace vision;

int ConsumerSimple::main(int argc, char* argv[])
{
    visionCom.start(CONSUMER_ENDPOINT, argc, argv);

    visionCom.enableBroadcast(Default, this);
    IceUtil::ThreadControl::sleep(IceUtil::Time::seconds(1));
    visionCom.disableBroadcast(Default, this);

    visionCom.enablePolling(Default);
    Blob image = visionCom.getImage(Default);
    std::cout << "Requested Image of size " << image.size() << " bytes." << std::endl;
    visionCom.disablePolling(Default);

    return 0;
}

void ConsumerSimple::receiveImage(const Blob& image, ImageType imageType)
{
    std::cout << "Received Image of size " << image.size() << " bytes." << std::endl;
}
