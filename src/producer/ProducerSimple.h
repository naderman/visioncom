#ifndef VISIONCOM_PRODUCERSIMPLE_H
#define VISIONCOM_PRODUCERSIMPLE_H

#include <interface/VisionComSender.h>

namespace vision
{
    class VisionCom_EXPORT ProducerSimple :
        virtual public ImageProvider
    {
        public:
            typedef std::map<ImageType, Blob*> ImageMap;

            int main(int argc, char* argv[]);

            Blob getImage(ImageType imageType);

        protected:
            VisionComSender sender;

            Blob lastRawImage;
            ImageMap lastImages;

            IceUtil::Mutex bufferMutex;
    };
}

#endif
