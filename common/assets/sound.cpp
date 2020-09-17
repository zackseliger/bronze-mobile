#include "sound.h"
#include "../utils.h"
#include <map>
#include <limits.h>

std::map<const char*, ALuint> sounds; // all the sounds we'll load

STREAM_DATA::STREAM_DATA(FileData fd) : asset(fd) {
  STREAM_DATA::offset = 0;
}

static sf_count_t stream_len(void *stream) {
  STREAM_DATA *vf = (STREAM_DATA *)stream;
  return (sf_count_t)vf->asset.size;
}

static sf_count_t stream_seek(sf_count_t offset, int whence, void *stream) {
  STREAM_DATA *vf = (STREAM_DATA *)stream;

  switch (whence)
  {
    case SEEK_SET:
      vf->offset = offset;
          break;

    case SEEK_CUR:
      vf->offset = vf->offset + offset;
          break;

    case SEEK_END:
      vf->offset = vf->asset.size + offset;
          break;
    default:
      break;
  };

  return vf->offset;
} // stream_seek

static sf_count_t stream_read(void *ptr, sf_count_t count, void *stream) {
  STREAM_DATA *vf = (STREAM_DATA *)stream;

  // don't read more than there is
  if (vf->offset + count > vf->asset.size)
    count = vf->asset.size - vf->offset;

  // copy info from asset to ptr
  memcpy( ptr, &((char*)vf->asset.data)[vf->offset], count);
  vf->offset += count;

  return count;
}

static sf_count_t stream_tell(void *stream) {
  STREAM_DATA *vf = (STREAM_DATA *)stream;
  return vf->offset;
}

ALuint loadSound(const char* filename, const char* name) {
  ALenum err, format;
  SNDFILE *sndfile;
  SF_INFO sfinfo;
  SF_VIRTUAL_IO sfvirtual;
  short *membuf;
  sf_count_t num_frames;
  ALsizei num_bytes;

  // Open the audio file and check that it's usable.
  FileData fileData = getAsset(filename);
  STREAM_DATA stream(fileData);

  sfvirtual.get_filelen = stream_len;
  sfvirtual.read = stream_read;
  sfvirtual.seek = stream_seek;
  sfvirtual.tell = stream_tell;

  sndfile = sf_open_virtual(&sfvirtual, SFM_READ, &sfinfo, &stream);
  if(!sndfile) {

    LOG("Couldn't open audio file %s: %s", filename, sf_strerror(sndfile));
    return 0;
  }
  if(sfinfo.frames < 1 || sfinfo.frames > (sf_count_t)(INT_MAX / sizeof(short)) / sfinfo.channels) {
//    fprintf(stderr, "Bad sample count in %s (%" PRId64 ")\n", filename, sfinfo.frames);
    LOG("Bad sample count in %s (%lld)", filename, sfinfo.frames);
    sf_close(sndfile);
    return 0;
  }

  // Get the sound format, and figure out the OpenAL format
  if(sfinfo.channels == 1)
    format = AL_FORMAT_MONO16;
  else if(sfinfo.channels == 2)
    format = AL_FORMAT_STEREO16;
  else
  {
    LOG("Unsupported channel count: %d", sfinfo.channels);
    sf_close(sndfile);
    return 0;
  }

  // Decode the whole audio file to a buffer.
  membuf = (short*)malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short));

  num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
  if(num_frames < 1)
  {
    free(membuf);
    sf_close(sndfile);
    LOG("Failed to read samples in %s (%lld)", filename, num_frames);
    return 0;
  }
  num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);

  // Buffer the audio data into a new buffer object, then free the data and
  // close the file.
  ALuint buffer = 0;
  alGenBuffers(1, &buffer);
  alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);

  // free memory
  free(membuf);
  sf_close(sndfile);
  freeAsset(fileData);

  // Check if an error occured, and clean up if so.
  err = alGetError();
  if(err != AL_NO_ERROR)
  {
    LOG("OpenAL Error: %s", alGetString(err));
    if(buffer && alIsBuffer(buffer)) alDeleteBuffers(1, &buffer);
    return 0;
  }
  
  // put buffer in sounds map
  sounds[name] = buffer;

  return buffer;
}

void playSound(const char* name) {
  std::map<const char*, ALuint>::iterator it;
  ALuint soundBuffer;
  
  it = sounds.find(name);
  if (it == sounds.end()) {
    LOG("COULDN'T FIND SOUND \"%s\"", name);
    return;
  }
  
  soundBuffer = it->second;
  alSourcePlay(soundBuffer);
}

