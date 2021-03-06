#ifndef SOUND_H
#define SOUND_H

#include "fileasset.h"
#include "../utils.h"

#include <map>
#include <sndfile.h>
#include <AL/al.h>
#include <AL/alc.h>

// our map type
typedef std::map<const char *, int, char_cmp> soundmap;

// to make sndfile happy
class STREAM_DATA {
public:
    STREAM_DATA(FileData);
    FileData asset;
    sf_count_t offset;
};

void initAudio();
ALuint loadSound(const char* filename, const char* name);
void playSound(const char* name);

#endif //SOUND_H
