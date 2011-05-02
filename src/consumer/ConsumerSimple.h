#ifndef VISIONCOM_CONSUMERSIMPLE_H
#define VISIONCOM_CONSUMERSIMPLE_H

#include <interface/VisionComReceiver.h>

namespace vision
{
    class VisionCom_EXPORT ConsumerSimple :
        virtual public ImageReceiver
    {
        public:
            int main(int argc, char* argv[]);

            void receiveImage(const Blob& image, ImageType imageType);

        protected:
            VisionComReceiver receiver;
    };
}

#endif
