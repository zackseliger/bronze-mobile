#import <UIKit/UIKit.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"

const char* get_file_data(const char* path) {
    assert(path != NULL);
        
    FILE* stream = fopen(path, "r");
    assert (stream != NULL);

    fseek(stream, 0, SEEK_END);
    long stream_size = ftell(stream);
    fseek(stream, 0, SEEK_SET);

    void* buffer = malloc(stream_size);
    fread(buffer, stream_size, 1, stream);

    assert(ferror(stream) == 0);
    fclose(stream);

    return (const char*)buffer;
}

const char* readFile(const char* filename) {
    // load some kind of file
    NSString* path=[[NSBundle mainBundle] resourcePath];
    NSMutableString* relativePath = [[NSMutableString alloc] initWithString:@"assets/"];
    [relativePath appendString:[[NSString alloc] initWithCString:filename encoding:NSASCIIStringEncoding]];
    
    NSString* test = [path stringByAppendingPathComponent:relativePath];
    
    const char* pathAsChar = [test UTF8String];
    return get_file_data(pathAsChar);
}
