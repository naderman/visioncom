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
         void receiveImageSharedMemory(SharedMemorySegment image);
     };

     interface ImageReceiverEmbedded
     {
         void receiveImageEmbedded(Blob image);
     };

     interface ImageReceiver extends ImageReceiverSharedMemory, ImageReceiverEmbedded {
     };

     interface ImageProvider
     {
         void enableBroadcast(StorageType store, ImageType image);
         void disableBroadcast(StorageType store, ImageType image);

         void enablePolling(StorageType store, ImageType image);
         void disablePolling(StorageType store, ImageType image);

         Blob getImageBlob(ImageType image);
         SharedMemorySegment getImageSharedMemory(ImageType image);

         int getHardwareId();
     };
};

