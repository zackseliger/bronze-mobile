#ifndef FILEASSET_H
#define FILEASSET_H

#ifdef __ANDROID__
#include <android/asset_manager.h>
void initAssetManager(AAssetManager* manager); // ios doesn't need this function
#endif

typedef struct FileData {
  long size;
  void* data;
  void* handle;
} FileData;

FileData getAsset(const char* filename);
void freeAsset(FileData asset);

#endif // FILEASSET_H
