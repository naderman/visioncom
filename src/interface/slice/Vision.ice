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
        Default,
     };

     interface ImageReceiverSharedMemory
     {
         void receiveImageSharedMemory(SharedMemorySegment image, ImageType iType);
     };

     interface ImageReceiverEmbedded
     {
         void receiveImageEmbedded(Blob image, ImageType iType);
     };

     interface ImageReceiverGeneric extends ImageReceiverSharedMemory, ImageReceiverEmbedded {
     };

     interface ImageProvider
     {
         void enableBroadcast(StorageType store, ImageType iType);
         void disableBroadcast(StorageType store, ImageType iType);

         void enablePolling(StorageType store, ImageType iType);
         void disablePolling(StorageType store, ImageType iType);

         Blob getImageBlob(ImageType iType);
         SharedMemorySegment getImageSharedMemory(ImageType iType);

         int getHardwareId();
     };
};

