#include "fileasset.h"

#ifdef __ANDROID__
#include <assert.h>
#include <stdlib.h>

AAssetManager* assetManager;
FileData getAsset(const char* filename) {
  AAsset* asset = AAssetManager_open(assetManager, filename, AASSET_MODE_STREAMING);
  assert(asset != nullptr);

  // read file, manually adding EOF
  long size = AAsset_getLength(asset);
  void* buffer = malloc(size+1);
  AAsset_read(asset, buffer, size);
  ((char*)buffer)[size] = 0;

  return (FileData){size+1, buffer, asset};
}
void freeAsset(FileData asset) {
    AAsset_close((AAsset*)asset.handle);
    free((void*)asset.data);
}
void initAssetManager(AAssetManager* manager) {
  assetManager = manager;
}
// ios implementation is in AssetsUtils.mm
#endif
