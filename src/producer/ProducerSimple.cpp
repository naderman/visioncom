#include "ProducerSimple.h"
#include "../NetworkConfig.h"

#include <iostream>

using namespace vision;

int ProducerSimple::main(int argc, char* argv[])
{
    IceUtil::Mutex::Lock bufferLock(bufferMutex);
    bufferLock.release();

    ImageMap::iterator image;
    std::set<ImageType> imageTypes;
    std::set<ImageType>::iterator imageType;
    Blob* newImage;

    int status;

    if ((status = sender.start(PRODUCER_ENDPOINT, argc, argv, this)))
    {
        return status;
    }

    while (!sender.isShutdown())
    {
        lastRawImage = Blob(); // TODO: getImageFromCamera(...)

        bufferLock.acquire();
        {
            // free all old images
            for (image = lastImages.begin(); image != lastImages.end(); ++image)
            {
                delete image->second;
            }

            imageTypes = sender.requestedImageTypes();

            // transform into all necessary types
            for (imageType = imageTypes.begin(); imageType != imageTypes.end(); ++imageType)
            {
                newImage = new Blob;
                // TODO: call a method to convert, e.g. prepare(lastRawImage, *imageType);
                *newImage = lastRawImage;
                lastImages[*imageType] = newImage;

                sender.broadcastImage(*newImage, *imageType);
            }
        }
        bufferLock.release();

        // "wait" for next image
        IceUtil::ThreadControl::sleep(IceUtil::Time::milliSeconds(30)); // ~30 images/second
    }

    sender.stop();

    return 0;
}

Blob ProducerSimple::getImage(ImageType imageType)
{
    IceUtil::Mutex::Lock lock(bufferMutex); // unlocks when going out of scope

    ImageMap::iterator image = lastImages.find(imageType);
    if (image == lastImages.end() || image->second == NULL)
    {
        throw ImageTypeNotEnabled();
    }

    return *(image->second);
}
