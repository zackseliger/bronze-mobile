#import <UIKit/UIKit.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "fileasset.h"

FileData get_file_data(const char* path) {
    assert(path != NULL);
        
    FILE* stream = fopen(path, "rb");
    assert (stream != NULL);

    fseek(stream, 0, SEEK_END);
    long stream_size = ftell(stream);
    rewind(stream);

    void* buffer = malloc(stream_size+1);
    fread(buffer, 1, stream_size, stream);
    ((char*)buffer)[stream_size] = 0; // null-terminated

    assert(ferror(stream) == 0);
    fclose(stream);

    return (FileData){stream_size, buffer, nullptr};
}

FileData getAsset(const char* filename) {
    // load some kind of file
    NSString* path=[[NSBundle mainBundle] resourcePath];
    NSMutableString* relativePath = [[NSMutableString alloc] initWithString:@"assets/"];
    [relativePath appendString:[[NSString alloc] initWithCString:filename encoding:NSASCIIStringEncoding]];

    NSString* test = [path stringByAppendingPathComponent:relativePath];
    const char* pathAsChar = [test UTF8String];

    return get_file_data(pathAsChar);
}

void freeAsset(FileData asset) {
    free((void*)asset.data);
}
