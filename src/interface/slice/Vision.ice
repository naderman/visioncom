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
        default,
     };

     interface VisionReceiverSharedMemory
     {
         void receiveImage(SharedMemorySegment image);
     };

     interface VisionReceiverEmbedded
     {
         void receiveImage(Blob image);
     };

     interface CaptureModule
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

