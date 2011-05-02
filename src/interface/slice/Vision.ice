module vision
{
    sequence<byte> Blob;

    struct SharedMemorySegment
    {
        int key;
        int size;
        int flags;
    };

    enum StorageType
    {
        SharedMemory,
        Embedded
    };

    enum ImageType
    {
        Default
    };

    exception ImageTypeNotEnabled {};

    exception ImageTypeNotSupported {};

    interface ImageReceiverSharedMemory
    {
        void receiveImageSharedMemory(SharedMemorySegment image, ImageType iType);
    };

    interface ImageReceiverEmbedded
    {
        void receiveImageEmbedded(Blob image, ImageType iType);
    };

    interface ImageReceiverGeneric extends ImageReceiverSharedMemory, ImageReceiverEmbedded {};

    interface ImageProviderGeneric
    {
        void enableBroadcast(StorageType store, ImageType iType) throws ImageTypeNotSupported;
        void disableBroadcast(StorageType store, ImageType iType);

        void enablePolling(StorageType store, ImageType iType) throws ImageTypeNotSupported;
        void disablePolling(StorageType store, ImageType iType);

        Blob getImageBlob(ImageType iType) throws ImageTypeNotSupported, ImageTypeNotEnabled;
        SharedMemorySegment getImageSharedMemory(ImageType iType) throws ImageTypeNotSupported, ImageTypeNotEnabled;

        int getHardwareId();
    };
};

