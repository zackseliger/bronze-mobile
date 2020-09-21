#ifndef FILEASSET_H
#define FILEASSET_H

#ifdef __ANDROID__
#include <android/asset_manager.h>
void initAssetManager(AAssetManager* manager); // ios doesn't need this function
#endif

typedef struct FileData {
  const long size;
  const void* data;
  const void* handle;
  
//  FileData();
//  FileData(long, void*, void*);
//  FileData& operator=(FileData);
} FileData;

FileData getAsset(const char* filename);
void freeAsset(FileData asset);

#endif // FILEASSET_H
