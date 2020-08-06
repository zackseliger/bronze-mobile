#import <UIKit/UIKit.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"

FileData get_file_data(const char* path) {
    assert(path != NULL);
        
    FILE* stream = fopen(path, "r");
    assert (stream != NULL);

    fseek(stream, 0, SEEK_END);
    long stream_size = ftell(stream);
//    fseek(stream, 0, SEEK_SET);
    rewind(stream);

    void* buffer = malloc(stream_size);
    fread(buffer, stream_size, 1, stream);

    assert(ferror(stream) == 0);
    fclose(stream);

    return (FileData){stream_size, buffer, nullptr};
}

FileData getAsset(const char* filename) {
    // load some kind of file
//    NSString* path=[[NSBundle mainBundle] resourcePath];
//    NSMutableString* relativePath = [[NSMutableString alloc] initWithString:@"assets/"];
//    [relativePath appendString:[[NSString alloc] initWithCString:filename encoding:NSASCIIStringEncoding]];
//
//    NSString* test = [path stringByAppendingPathComponent:relativePath];
//
//    const char* pathAsChar = [test UTF8String];

    NSMutableString* adjusted_relative_path = [[NSMutableString alloc] initWithString:@"/assets/"];
    [adjusted_relative_path appendString:[[NSString alloc] initWithCString:filename encoding:NSASCIIStringEncoding]];

    const char* pathAsChar = [[[NSBundle mainBundle] pathForResource:adjusted_relative_path ofType:nil] cStringUsingEncoding:NSASCIIStringEncoding];

//    return get_file_data(pathAsChar);

    NSString* thing = [[NSBundle mainBundle] pathForResource:adjusted_relative_path ofType:nil];

    NSFileManager* fileManager = [[NSFileManager alloc] init];

//    unsigned long long fileSize = [[[NSFileManager defaultManager] attributesOfItemAtPath:thing error:nil] fileSize];
    NSData* payload = [fileManager contentsAtPath:thing];
    const long fileSize = [payload length];
    void* buffer = malloc(fileSize);
    memcpy(buffer, [payload bytes], fileSize);
//    pubmsg.payloadlen = typedData;

    return (FileData){(const long)fileSize, buffer, nullptr};
}

void freeAsset(FileData asset) {
//    free((void*)asset.data);
}
