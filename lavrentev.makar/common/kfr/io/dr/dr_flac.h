#ifndef dr_flac_h
#define dr_flac_h

#include <stddef.h>

#if defined(_MSC_VER) && _MSC_VER < 1600
typedef signed char drflac_int8;
typedef unsigned char drflac_uint8;
typedef signed short drflac_int16;
typedef unsigned short drflac_uint16;
typedef signed int drflac_int32;
typedef unsigned int drflac_uint32;
typedef signed __int64 drflac_int64;
typedef unsigned __int64 drflac_uint64;
#else
#include <stdint.h>
typedef int8_t drflac_int8;
typedef uint8_t drflac_uint8;
typedef int16_t drflac_int16;
typedef uint16_t drflac_uint16;
typedef int32_t drflac_int32;
typedef uint32_t drflac_uint32;
typedef int64_t drflac_int64;
typedef uint64_t drflac_uint64;
#endif
typedef drflac_uint8 drflac_bool8;
typedef drflac_uint32 drflac_bool32;
#define DRFLAC_TRUE 1
#define DRFLAC_FALSE 0

#ifndef DR_FLAC_BUFFER_SIZE
#define DR_FLAC_BUFFER_SIZE 4096
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN64) || defined(_LP64) || defined(__LP64__)
#define DRFLAC_64BIT
#endif

#ifdef DRFLAC_64BIT
typedef drflac_uint64 drflac_cache_t;
#else
typedef drflac_uint32 drflac_cache_t;
#endif

#define DRFLAC_METADATA_BLOCK_TYPE_STREAMINFO 0
#define DRFLAC_METADATA_BLOCK_TYPE_PADDING 1
#define DRFLAC_METADATA_BLOCK_TYPE_APPLICATION 2
#define DRFLAC_METADATA_BLOCK_TYPE_SEEKTABLE 3
#define DRFLAC_METADATA_BLOCK_TYPE_VORBIS_COMMENT 4
#define DRFLAC_METADATA_BLOCK_TYPE_CUESHEET 5
#define DRFLAC_METADATA_BLOCK_TYPE_PICTURE 6
#define DRFLAC_METADATA_BLOCK_TYPE_INVALID 127

#define DRFLAC_PICTURE_TYPE_OTHER 0
#define DRFLAC_PICTURE_TYPE_FILE_ICON 1
#define DRFLAC_PICTURE_TYPE_OTHER_FILE_ICON 2
#define DRFLAC_PICTURE_TYPE_COVER_FRONT 3
#define DRFLAC_PICTURE_TYPE_COVER_BACK 4
#define DRFLAC_PICTURE_TYPE_LEAFLET_PAGE 5
#define DRFLAC_PICTURE_TYPE_MEDIA 6
#define DRFLAC_PICTURE_TYPE_LEAD_ARTIST 7
#define DRFLAC_PICTURE_TYPE_ARTIST 8
#define DRFLAC_PICTURE_TYPE_CONDUCTOR 9
#define DRFLAC_PICTURE_TYPE_BAND 10
#define DRFLAC_PICTURE_TYPE_COMPOSER 11
#define DRFLAC_PICTURE_TYPE_LYRICIST 12
#define DRFLAC_PICTURE_TYPE_RECORDING_LOCATION 13
#define DRFLAC_PICTURE_TYPE_DURING_RECORDING 14
#define DRFLAC_PICTURE_TYPE_DURING_PERFORMANCE 15
#define DRFLAC_PICTURE_TYPE_SCREEN_CAPTURE 16
#define DRFLAC_PICTURE_TYPE_BRIGHT_COLORED_FISH 17
#define DRFLAC_PICTURE_TYPE_ILLUSTRATION 18
#define DRFLAC_PICTURE_TYPE_BAND_LOGOTYPE 19
#define DRFLAC_PICTURE_TYPE_PUBLISHER_LOGOTYPE 20

typedef enum { drflac_container_native, drflac_container_ogg, drflac_container_unknown } drflac_container;

typedef enum { drflac_seek_origin_start, drflac_seek_origin_current } drflac_seek_origin;

#pragma pack(2)
typedef struct
{
  drflac_uint64 firstSample;
  drflac_uint64 frameOffset;
  drflac_uint16 sampleCount;
} drflac_seekpoint;
#pragma pack()

typedef struct
{
  drflac_uint16 minBlockSize;
  drflac_uint16 maxBlockSize;
  drflac_uint32 minFrameSize;
  drflac_uint32 maxFrameSize;
  drflac_uint32 sampleRate;
  drflac_uint8 channels;
  drflac_uint8 bitsPerSample;
  drflac_uint64 totalSampleCount;
  drflac_uint8 md5[16];
} drflac_streaminfo;

typedef struct
{

  drflac_uint32 type;

  const void *pRawData;

  drflac_uint32 rawDataSize;

  union
  {
    drflac_streaminfo streaminfo;

    struct
    {
      int unused;
    } padding;

    struct
    {
      drflac_uint32 id;
      const void *pData;
      drflac_uint32 dataSize;
    } application;

    struct
    {
      drflac_uint32 seekpointCount;
      const drflac_seekpoint *pSeekpoints;
    } seektable;

    struct
    {
      drflac_uint32 vendorLength;
      const char *vendor;
      drflac_uint32 commentCount;
      const void *pComments;
    } vorbis_comment;

    struct
    {
      char catalog[128];
      drflac_uint64 leadInSampleCount;
      drflac_bool32 isCD;
      drflac_uint8 trackCount;
      const void *pTrackData;
    } cuesheet;

    struct
    {
      drflac_uint32 type;
      drflac_uint32 mimeLength;
      const char *mime;
      drflac_uint32 descriptionLength;
      const char *description;
      drflac_uint32 width;
      drflac_uint32 height;
      drflac_uint32 colorDepth;
      drflac_uint32 indexColorCount;
      drflac_uint32 pictureDataSize;
      const drflac_uint8 *pPictureData;
    } picture;
  } data;
} drflac_metadata;

typedef size_t (*drflac_read_proc)(void *pUserData, void *pBufferOut, size_t bytesToRead);

typedef drflac_bool32 (*drflac_seek_proc)(void *pUserData, int offset, drflac_seek_origin origin);

typedef void (*drflac_meta_proc)(void *pUserData, drflac_metadata *pMetadata);

typedef struct
{
  const drflac_uint8 *data;
  size_t dataSize;
  size_t currentReadPos;
} drflac__memory_stream;

typedef struct
{

  drflac_read_proc onRead;

  drflac_seek_proc onSeek;

  void *pUserData;
  size_t unalignedByteCount;

  drflac_cache_t unalignedCache;

  drflac_uint32 nextL2Line;

  drflac_uint32 consumedBits;

  drflac_cache_t cacheL2[DR_FLAC_BUFFER_SIZE / sizeof(drflac_cache_t)];
  drflac_cache_t cache;

  drflac_uint16 crc16;
  drflac_cache_t crc16Cache;
  drflac_uint32 crc16CacheIgnoredBytes;
} drflac_bs;

typedef struct
{

  drflac_uint8 subframeType;

  drflac_uint8 wastedBitsPerSample;

  drflac_uint8 lpcOrder;

  drflac_uint32 bitsPerSample;

  drflac_int32 *pDecodedSamples;
} drflac_subframe;

typedef struct
{

  drflac_uint64 sampleNumber;

  drflac_uint32 frameNumber;

  drflac_uint32 sampleRate;

  drflac_uint16 blockSize;

  drflac_uint8 channelAssignment;

  drflac_uint8 bitsPerSample;

  drflac_uint8 crc8;
} drflac_frame_header;

typedef struct
{

  drflac_frame_header header;

  drflac_uint32 samplesRemaining;

  drflac_subframe subframes[8];
} drflac_frame;

typedef struct
{

  drflac_meta_proc onMeta;

  void *pUserDataMD;

  drflac_uint32 sampleRate;

  drflac_uint8 channels;

  drflac_uint8 bitsPerSample;

  drflac_uint16 maxBlockSize;

  drflac_uint64 totalSampleCount;

  drflac_container container;

  drflac_uint32 seekpointCount;

  drflac_frame currentFrame;

  drflac_uint64 currentSample;

  drflac_uint64 firstFramePos;

  drflac__memory_stream memoryStream;

  drflac_int32 *pDecodedSamples;

  drflac_seekpoint *pSeekpoints;

  void *_oggbs;

  drflac_bs bs;

  drflac_uint8 pExtraData[1];
} drflac;

drflac *drflac_open(drflac_read_proc onRead, drflac_seek_proc onSeek, void *pUserData);

drflac *drflac_open_relaxed(drflac_read_proc onRead, drflac_seek_proc onSeek, drflac_container container,
                            void *pUserData);

drflac *drflac_open_with_metadata(drflac_read_proc onRead, drflac_seek_proc onSeek, drflac_meta_proc onMeta,
                                  void *pUserData);

drflac *drflac_open_with_metadata_relaxed(drflac_read_proc onRead, drflac_seek_proc onSeek, drflac_meta_proc onMeta,
                                          drflac_container container, void *pUserData);

void drflac_close(drflac *pFlac);

drflac_uint64 drflac_read_s32(drflac *pFlac, drflac_uint64 samplesToRead, drflac_int32 *pBufferOut);

drflac_uint64 drflac_read_s16(drflac *pFlac, drflac_uint64 samplesToRead, drflac_int16 *pBufferOut);

drflac_uint64 drflac_read_f32(drflac *pFlac, drflac_uint64 samplesToRead, float *pBufferOut);

drflac_bool32 drflac_seek_to_sample(drflac *pFlac, drflac_uint64 sampleIndex);

#ifndef DR_FLAC_NO_STDIO

drflac *drflac_open_file(const char *filename);

drflac *drflac_open_file_with_metadata(const char *filename, drflac_meta_proc onMeta, void *pUserData);
#endif

drflac *drflac_open_memory(const void *data, size_t dataSize);

drflac *drflac_open_memory_with_metadata(const void *data, size_t dataSize, drflac_meta_proc onMeta, void *pUserData);

drflac_int32 *drflac_open_and_decode_s32(drflac_read_proc onRead, drflac_seek_proc onSeek, void *pUserData,
                                         unsigned int *channels, unsigned int *sampleRate,
                                         drflac_uint64 *totalSampleCount);

drflac_int16 *drflac_open_and_decode_s16(drflac_read_proc onRead, drflac_seek_proc onSeek, void *pUserData,
                                         unsigned int *channels, unsigned int *sampleRate,
                                         drflac_uint64 *totalSampleCount);

float *drflac_open_and_decode_f32(drflac_read_proc onRead, drflac_seek_proc onSeek, void *pUserData,
                                  unsigned int *channels, unsigned int *sampleRate, drflac_uint64 *totalSampleCount);

#ifndef DR_FLAC_NO_STDIO

drflac_int32 *drflac_open_and_decode_file_s32(const char *filename, unsigned int *channels, unsigned int *sampleRate,
                                              drflac_uint64 *totalSampleCount);

drflac_int16 *drflac_open_and_decode_file_s16(const char *filename, unsigned int *channels, unsigned int *sampleRate,
                                              drflac_uint64 *totalSampleCount);

float *drflac_open_and_decode_file_f32(const char *filename, unsigned int *channels, unsigned int *sampleRate,
                                       drflac_uint64 *totalSampleCount);
#endif

drflac_int32 *drflac_open_and_decode_memory_s32(const void *data, size_t dataSize, unsigned int *channels,
                                                unsigned int *sampleRate, drflac_uint64 *totalSampleCount);

drflac_int16 *drflac_open_and_decode_memory_s16(const void *data, size_t dataSize, unsigned int *channels,
                                                unsigned int *sampleRate, drflac_uint64 *totalSampleCount);

float *drflac_open_and_decode_memory_f32(const void *data, size_t dataSize, unsigned int *channels,
                                         unsigned int *sampleRate, drflac_uint64 *totalSampleCount);

void drflac_free(void *p);

typedef struct
{
  drflac_uint32 countRemaining;
  const char *pRunningData;
} drflac_vorbis_comment_iterator;

void drflac_init_vorbis_comment_iterator(drflac_vorbis_comment_iterator *pIter, drflac_uint32 commentCount,
                                         const void *pComments);

const char *drflac_next_vorbis_comment(drflac_vorbis_comment_iterator *pIter, drflac_uint32 *pCommentLengthOut);

typedef struct
{
  drflac_uint32 countRemaining;
  const char *pRunningData;
} drflac_cuesheet_track_iterator;

#pragma pack(4)
typedef struct
{
  drflac_uint64 offset;
  drflac_uint8 index;
  drflac_uint8 reserved[3];
} drflac_cuesheet_track_index;
#pragma pack()

typedef struct
{
  drflac_uint64 offset;
  drflac_uint8 trackNumber;
  char ISRC[12];
  drflac_bool8 isAudio;
  drflac_bool8 preEmphasis;
  drflac_uint8 indexCount;
  const drflac_cuesheet_track_index *pIndexPoints;
} drflac_cuesheet_track;

void drflac_init_cuesheet_track_iterator(drflac_cuesheet_track_iterator *pIter, drflac_uint32 trackCount,
                                         const void *pTrackData);

drflac_bool32 drflac_next_cuesheet_track(drflac_cuesheet_track_iterator *pIter, drflac_cuesheet_track *pCuesheetTrack);

#ifdef __cplusplus
}
#endif
#endif

#ifdef DR_FLAC_IMPLEMENTATION
#ifdef __linux__
#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif
#ifndef __USE_BSD
#define __USE_BSD
#endif
#include <endian.h>
#endif

#include <stdlib.h>
#include <string.h>

#if defined(__x86_64__) || defined(_M_X64)
#define DRFLAC_X64
#elif defined(__i386) || defined(_M_IX86)
#define DRFLAC_X86
#elif defined(__arm__) || defined(_M_ARM)
#define DRFLAC_ARM
#endif

#if !defined(DR_FLAC_NO_SIMD) && (defined(DRFLAC_X86) || defined(DRFLAC_X64))
#if defined(_MSC_VER) && !defined(__clang__)
#if _MSC_VER >= 1400
#include <intrin.h>
static void drflac__cpuid(int info[4], int fid)
{
  __cpuid(info, fid);
}
#else
#define DRFLAC_NO_CPUID
#endif
#else
#if defined(__GNUC__) || defined(__clang__)
static void drflac__cpuid(int info[4], int fid)
{

#if defined(DRFLAC_X86) && defined(__PIC__)
  __asm__ __volatile__("xchg{l} {%%}ebx, %k1;"
                       "cpuid;"
                       "xchg{l} {%%}ebx, %k1;"
                       : "=a"(info[0]), "=&r"(info[1]), "=c"(info[2]), "=d"(info[3])
                       : "a"(fid), "c"(0));
#else
  __asm__ __volatile__("cpuid" : "=a"(info[0]), "=b"(info[1]), "=c"(info[2]), "=d"(info[3]) : "a"(fid), "c"(0));
#endif
}
#else
#define DRFLAC_NO_CPUID
#endif
#endif
#else
#define DRFLAC_NO_CPUID
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1500 && (defined(DRFLAC_X86) || defined(DRFLAC_X64))
#define DRFLAC_HAS_LZCNT_INTRINSIC
#elif (defined(__GNUC__) && ((__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)))
#define DRFLAC_HAS_LZCNT_INTRINSIC
#elif defined(__clang__)
#if __has_builtin(__builtin_clzll) || __has_builtin(__builtin_clzl)
#define DRFLAC_HAS_LZCNT_INTRINSIC
#endif
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1300
#define DRFLAC_HAS_BYTESWAP16_INTRINSIC
#define DRFLAC_HAS_BYTESWAP32_INTRINSIC
#define DRFLAC_HAS_BYTESWAP64_INTRINSIC
#elif defined(__clang__)
#if __has_builtin(__builtin_bswap16)
#define DRFLAC_HAS_BYTESWAP16_INTRINSIC
#endif
#if __has_builtin(__builtin_bswap32)
#define DRFLAC_HAS_BYTESWAP32_INTRINSIC
#endif
#if __has_builtin(__builtin_bswap64)
#define DRFLAC_HAS_BYTESWAP64_INTRINSIC
#endif
#elif defined(__GNUC__)
#if ((__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 3))
#define DRFLAC_HAS_BYTESWAP32_INTRINSIC
#define DRFLAC_HAS_BYTESWAP64_INTRINSIC
#endif
#if ((__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8))
#define DRFLAC_HAS_BYTESWAP16_INTRINSIC
#endif
#endif

#ifndef DRFLAC_ASSERT
#include <assert.h>
#define DRFLAC_ASSERT(expression) assert(expression)
#endif
#ifndef DRFLAC_MALLOC
#define DRFLAC_MALLOC(sz) malloc((sz))
#endif
#ifndef DRFLAC_REALLOC
#define DRFLAC_REALLOC(p, sz) realloc((p), (sz))
#endif
#ifndef DRFLAC_FREE
#define DRFLAC_FREE(p) free((p))
#endif
#ifndef DRFLAC_COPY_MEMORY
#define DRFLAC_COPY_MEMORY(dst, src, sz) memcpy((dst), (src), (sz))
#endif
#ifndef DRFLAC_ZERO_MEMORY
#define DRFLAC_ZERO_MEMORY(p, sz) memset((p), 0, (sz))
#endif

#define DRFLAC_MAX_SIMD_VECTOR_SIZE 64

#ifdef _MSC_VER
#define DRFLAC_INLINE __forceinline
#else
#ifdef __GNUC__
#define DRFLAC_INLINE inline __attribute__((always_inline))
#else
#define DRFLAC_INLINE inline
#endif
#endif

typedef drflac_int32 drflac_result;
#define DRFLAC_SUCCESS 0
#define DRFLAC_ERROR -1
#define DRFLAC_INVALID_ARGS -2
#define DRFLAC_END_OF_STREAM -128
#define DRFLAC_CRC_MISMATCH -129

#define DRFLAC_SUBFRAME_CONSTANT 0
#define DRFLAC_SUBFRAME_VERBATIM 1
#define DRFLAC_SUBFRAME_FIXED 8
#define DRFLAC_SUBFRAME_LPC 32
#define DRFLAC_SUBFRAME_RESERVED 255

#define DRFLAC_RESIDUAL_CODING_METHOD_PARTITIONED_RICE 0
#define DRFLAC_RESIDUAL_CODING_METHOD_PARTITIONED_RICE2 1

#define DRFLAC_CHANNEL_ASSIGNMENT_INDEPENDENT 0
#define DRFLAC_CHANNEL_ASSIGNMENT_LEFT_SIDE 8
#define DRFLAC_CHANNEL_ASSIGNMENT_RIGHT_SIDE 9
#define DRFLAC_CHANNEL_ASSIGNMENT_MID_SIDE 10

#define drflac_align(x, a) ((((x) + (a) - 1) / (a)) * (a))
#define drflac_assert DRFLAC_ASSERT
#define drflac_copy_memory DRFLAC_COPY_MEMORY
#define drflac_zero_memory DRFLAC_ZERO_MEMORY

static drflac_bool32 drflac__gIsLZCNTSupported = DRFLAC_FALSE;
#ifndef DRFLAC_NO_CPUID
static drflac_bool32 drflac__gIsSSE42Supported = DRFLAC_FALSE;
static void drflac__init_cpu_caps()
{
  int info[4] = {0};

  drflac__cpuid(info, 0x80000001);
  drflac__gIsLZCNTSupported = (info[2] & (1 << 5)) != 0;

  drflac__cpuid(info, 1);
  drflac__gIsSSE42Supported = (info[2] & (1 << 19)) != 0;
}
#endif

static DRFLAC_INLINE drflac_bool32 drflac__is_little_endian()
{
#if defined(DRFLAC_X86) || defined(DRFLAC_X64)
  return DRFLAC_TRUE;
#else
  int n = 1;
  return (*(char *)&n) == 1;
#endif
}

static DRFLAC_INLINE drflac_uint16 drflac__swap_endian_uint16(drflac_uint16 n)
{
#ifdef DRFLAC_HAS_BYTESWAP16_INTRINSIC
#if defined(_MSC_VER)
  return _byteswap_ushort(n);
#elif defined(__GNUC__) || defined(__clang__)
  return __builtin_bswap16(n);
#else
#error "This compiler does not support the byte swap intrinsic."
#endif
#else
  return ((n & 0xFF00) >> 8) | ((n & 0x00FF) << 8);
#endif
}

static DRFLAC_INLINE drflac_uint32 drflac__swap_endian_uint32(drflac_uint32 n)
{
#ifdef DRFLAC_HAS_BYTESWAP32_INTRINSIC
#if defined(_MSC_VER)
  return _byteswap_ulong(n);
#elif defined(__GNUC__) || defined(__clang__)
  return __builtin_bswap32(n);
#else
#error "This compiler does not support the byte swap intrinsic."
#endif
#else
  return ((n & 0xFF000000) >> 24) | ((n & 0x00FF0000) >> 8) | ((n & 0x0000FF00) << 8) | ((n & 0x000000FF) << 24);
#endif
}

static DRFLAC_INLINE drflac_uint64 drflac__swap_endian_uint64(drflac_uint64 n)
{
#ifdef DRFLAC_HAS_BYTESWAP64_INTRINSIC
#if defined(_MSC_VER)
  return _byteswap_uint64(n);
#elif defined(__GNUC__) || defined(__clang__)
  return __builtin_bswap64(n);
#else
#error "This compiler does not support the byte swap intrinsic."
#endif
#else
  return ((n & (drflac_uint64)0xFF00000000000000) >> 56) | ((n & (drflac_uint64)0x00FF000000000000) >> 40)
         | ((n & (drflac_uint64)0x0000FF0000000000) >> 24) | ((n & (drflac_uint64)0x000000FF00000000) >> 8)
         | ((n & (drflac_uint64)0x00000000FF000000) << 8) | ((n & (drflac_uint64)0x0000000000FF0000) << 24)
         | ((n & (drflac_uint64)0x000000000000FF00) << 40) | ((n & (drflac_uint64)0x00000000000000FF) << 56);
#endif
}

static DRFLAC_INLINE drflac_uint16 drflac__be2host_16(drflac_uint16 n)
{
#ifdef __linux__
  return be16toh(n);
#else
  if (drflac__is_little_endian()) {
    return drflac__swap_endian_uint16(n);
  }

  return n;
#endif
}

static DRFLAC_INLINE drflac_uint32 drflac__be2host_32(drflac_uint32 n)
{
#ifdef __linux__
  return be32toh(n);
#else
  if (drflac__is_little_endian()) {
    return drflac__swap_endian_uint32(n);
  }

  return n;
#endif
}

static DRFLAC_INLINE drflac_uint64 drflac__be2host_64(drflac_uint64 n)
{
#ifdef __linux__
  return be64toh(n);
#else
  if (drflac__is_little_endian()) {
    return drflac__swap_endian_uint64(n);
  }

  return n;
#endif
}

static DRFLAC_INLINE drflac_uint32 drflac__le2host_32(drflac_uint32 n)
{
#ifdef __linux__
  return le32toh(n);
#else
  if (!drflac__is_little_endian()) {
    return drflac__swap_endian_uint32(n);
  }

  return n;
#endif
}

static DRFLAC_INLINE drflac_uint32 drflac__unsynchsafe_32(drflac_uint32 n)
{
  drflac_uint32 result = 0;
  result |= (n & 0x7F000000) >> 3;
  result |= (n & 0x007F0000) >> 2;
  result |= (n & 0x00007F00) >> 1;
  result |= (n & 0x0000007F) >> 0;

  return result;
}

static drflac_uint8 drflac__crc8_table[] = {
    0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15, 0x38, 0x3F, 0x36, 0x31, 0x24, 0x23, 0x2A, 0x2D, 0x70, 0x77, 0x7E,
    0x79, 0x6C, 0x6B, 0x62, 0x65, 0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D, 0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB,
    0xF2, 0xF5, 0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD, 0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85, 0xA8,
    0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA, 0xBD, 0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2, 0xFF, 0xF8, 0xF1, 0xF6,
    0xE3, 0xE4, 0xED, 0xEA, 0xB7, 0xB0, 0xB9, 0xBE, 0xAB, 0xAC, 0xA5, 0xA2, 0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D,
    0x9A, 0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32, 0x1F, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0D, 0x0A, 0x57, 0x50,
    0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42, 0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A, 0x89, 0x8E, 0x87, 0x80, 0x95,
    0x92, 0x9B, 0x9C, 0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4, 0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC,
    0xC1, 0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4, 0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C, 0x51, 0x56, 0x5F,
    0x58, 0x4D, 0x4A, 0x43, 0x44, 0x19, 0x1E, 0x17, 0x10, 0x05, 0x02, 0x0B, 0x0C, 0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A,
    0x33, 0x34, 0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B, 0x76, 0x71, 0x78, 0x7F, 0x6A, 0x6D, 0x64, 0x63, 0x3E,
    0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B, 0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13, 0xAE, 0xA9, 0xA0, 0xA7,
    0xB2, 0xB5, 0xBC, 0xBB, 0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83, 0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC,
    0xCB, 0xE6, 0xE1, 0xE8, 0xEF, 0xFA, 0xFD, 0xF4, 0xF3};

static drflac_uint16 drflac__crc16_table[] = {
    0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011, 0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D,
    0x8027, 0x0022, 0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072, 0x0050, 0x8055, 0x805F, 0x005A,
    0x804B, 0x004E, 0x0044, 0x8041, 0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2, 0x00F0, 0x80F5,
    0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1, 0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1,
    0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082, 0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D,
    0x8197, 0x0192, 0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1, 0x01E0, 0x81E5, 0x81EF, 0x01EA,
    0x81FB, 0x01FE, 0x01F4, 0x81F1, 0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2, 0x0140, 0x8145,
    0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151, 0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162,
    0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132, 0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E,
    0x0104, 0x8101, 0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312, 0x0330, 0x8335, 0x833F, 0x033A,
    0x832B, 0x032E, 0x0324, 0x8321, 0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371, 0x8353, 0x0356,
    0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342, 0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1,
    0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2, 0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD,
    0x83B7, 0x03B2, 0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381, 0x0280, 0x8285, 0x828F, 0x028A,
    0x829B, 0x029E, 0x0294, 0x8291, 0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2, 0x82E3, 0x02E6,
    0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2, 0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1,
    0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252, 0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E,
    0x0264, 0x8261, 0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231, 0x8213, 0x0216, 0x021C, 0x8219,
    0x0208, 0x820D, 0x8207, 0x0202};

static DRFLAC_INLINE drflac_uint8 drflac_crc8_byte(drflac_uint8 crc, drflac_uint8 data)
{
  return drflac__crc8_table[crc ^ data];
}

static DRFLAC_INLINE drflac_uint8 drflac_crc8(drflac_uint8 crc, drflac_uint32 data, drflac_uint32 count)
{
  drflac_assert(count <= 32);

#ifdef DR_FLAC_NO_CRC
  (void)crc;
  (void)data;
  (void)count;
  return 0;
#else
#if 0
    drflac_uint8 p = 0x07;
    for (int i = count-1; i >= 0; --i) {
        drflac_uint8 bit = (data & (1 << i)) >> i;
        if (crc & 0x80) {
            crc = ((crc << 1) | bit) ^ p;
        } else {
            crc = ((crc << 1) | bit);
        }
    }
    return crc;
#else
  drflac_uint32 wholeBytes = count >> 3;
  drflac_uint32 leftoverBits = count - (wholeBytes * 8);

  static drflac_uint64 leftoverDataMaskTable[8] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F};
  drflac_uint64 leftoverDataMask = leftoverDataMaskTable[leftoverBits];

  switch (wholeBytes) {
  case 4:
    crc = drflac_crc8_byte(crc, (drflac_uint8)((data & (0xFF000000UL << leftoverBits)) >> (24 + leftoverBits)));
  case 3:
    crc = drflac_crc8_byte(crc, (drflac_uint8)((data & (0x00FF0000UL << leftoverBits)) >> (16 + leftoverBits)));
  case 2:
    crc = drflac_crc8_byte(crc, (drflac_uint8)((data & (0x0000FF00UL << leftoverBits)) >> (8 + leftoverBits)));
  case 1:
    crc = drflac_crc8_byte(crc, (drflac_uint8)((data & (0x000000FFUL << leftoverBits)) >> (0 + leftoverBits)));
  case 0:
    if (leftoverBits > 0)
      crc = (crc << leftoverBits) ^ drflac__crc8_table[(crc >> (8 - leftoverBits)) ^ (data & leftoverDataMask)];
  }
  return crc;
#endif
#endif
}

static DRFLAC_INLINE drflac_uint16 drflac_crc16_byte(drflac_uint16 crc, drflac_uint8 data)
{
  return (crc << 8) ^ drflac__crc16_table[(drflac_uint8)(crc >> 8) ^ data];
}

static DRFLAC_INLINE drflac_uint16 drflac_crc16_bytes(drflac_uint16 crc, drflac_cache_t data, drflac_uint32 byteCount)
{
  switch (byteCount) {
#ifdef DRFLAC_64BIT
  case 8:
    crc = drflac_crc16_byte(crc, (drflac_uint8)((data >> 56) & 0xFF));
  case 7:
    crc = drflac_crc16_byte(crc, (drflac_uint8)((data >> 48) & 0xFF));
  case 6:
    crc = drflac_crc16_byte(crc, (drflac_uint8)((data >> 40) & 0xFF));
  case 5:
    crc = drflac_crc16_byte(crc, (drflac_uint8)((data >> 32) & 0xFF));
#endif
  case 4:
    crc = drflac_crc16_byte(crc, (drflac_uint8)((data >> 24) & 0xFF));
  case 3:
    crc = drflac_crc16_byte(crc, (drflac_uint8)((data >> 16) & 0xFF));
  case 2:
    crc = drflac_crc16_byte(crc, (drflac_uint8)((data >> 8) & 0xFF));
  case 1:
    crc = drflac_crc16_byte(crc, (drflac_uint8)((data >> 0) & 0xFF));
  }

  return crc;
}

static DRFLAC_INLINE drflac_uint16 drflac_crc16__32bit(drflac_uint16 crc, drflac_uint32 data, drflac_uint32 count)
{
  drflac_assert(count <= 64);

#ifdef DR_FLAC_NO_CRC
  (void)crc;
  (void)data;
  (void)count;
  return 0;
#else
#if 0
    drflac_uint16 p = 0x8005;
    for (int i = count-1; i >= 0; --i) {
        drflac_uint16 bit = (data & (1ULL << i)) >> i;
        if (r & 0x8000) {
            r = ((r << 1) | bit) ^ p;
        } else {
            r = ((r << 1) | bit);
        }
    }

    return crc;
#else
  drflac_uint32 wholeBytes = count >> 3;
  drflac_uint32 leftoverBits = count - (wholeBytes * 8);

  static drflac_uint64 leftoverDataMaskTable[8] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F};
  drflac_uint64 leftoverDataMask = leftoverDataMaskTable[leftoverBits];

  switch (wholeBytes) {
  default:
  case 4:
    crc = drflac_crc16_byte(crc, (drflac_uint8)((data & (0xFF000000UL << leftoverBits)) >> (24 + leftoverBits)));
  case 3:
    crc = drflac_crc16_byte(crc, (drflac_uint8)((data & (0x00FF0000UL << leftoverBits)) >> (16 + leftoverBits)));
  case 2:
    crc = drflac_crc16_byte(crc, (drflac_uint8)((data & (0x0000FF00UL << leftoverBits)) >> (8 + leftoverBits)));
  case 1:
    crc = drflac_crc16_byte(crc, (drflac_uint8)((data & (0x000000FFUL << leftoverBits)) >> (0 + leftoverBits)));
  case 0:
    if (leftoverBits > 0)
      crc = (crc << leftoverBits) ^ drflac__crc16_table[(crc >> (16 - leftoverBits)) ^ (data & leftoverDataMask)];
  }
  return crc;
#endif
#endif
}

static DRFLAC_INLINE drflac_uint16 drflac_crc16__64bit(drflac_uint16 crc, drflac_uint64 data, drflac_uint32 count)
{
  drflac_assert(count <= 64);

#ifdef DR_FLAC_NO_CRC
  (void)crc;
  (void)data;
  (void)count;
  return 0;
#else
  drflac_uint32 wholeBytes = count >> 3;
  drflac_uint32 leftoverBits = count - (wholeBytes * 8);

  static drflac_uint64 leftoverDataMaskTable[8] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F};
  drflac_uint64 leftoverDataMask = leftoverDataMaskTable[leftoverBits];

  switch (wholeBytes) {
  default:
  case 8:
    crc = drflac_crc16_byte(
        crc, (drflac_uint8)((data & ((drflac_uint64)0xFF00000000000000 << leftoverBits)) >> (56 + leftoverBits)));
  case 7:
    crc = drflac_crc16_byte(
        crc, (drflac_uint8)((data & ((drflac_uint64)0x00FF000000000000 << leftoverBits)) >> (48 + leftoverBits)));
  case 6:
    crc = drflac_crc16_byte(
        crc, (drflac_uint8)((data & ((drflac_uint64)0x0000FF0000000000 << leftoverBits)) >> (40 + leftoverBits)));
  case 5:
    crc = drflac_crc16_byte(
        crc, (drflac_uint8)((data & ((drflac_uint64)0x000000FF00000000 << leftoverBits)) >> (32 + leftoverBits)));
  case 4:
    crc = drflac_crc16_byte(
        crc, (drflac_uint8)((data & ((drflac_uint64)0x00000000FF000000 << leftoverBits)) >> (24 + leftoverBits)));
  case 3:
    crc = drflac_crc16_byte(
        crc, (drflac_uint8)((data & ((drflac_uint64)0x0000000000FF0000 << leftoverBits)) >> (16 + leftoverBits)));
  case 2:
    crc = drflac_crc16_byte(
        crc, (drflac_uint8)((data & ((drflac_uint64)0x000000000000FF00 << leftoverBits)) >> (8 + leftoverBits)));
  case 1:
    crc = drflac_crc16_byte(
        crc, (drflac_uint8)((data & ((drflac_uint64)0x00000000000000FF << leftoverBits)) >> (0 + leftoverBits)));
  case 0:
    if (leftoverBits > 0)
      crc = (crc << leftoverBits) ^ drflac__crc16_table[(crc >> (16 - leftoverBits)) ^ (data & leftoverDataMask)];
  }
  return crc;
#endif
}

static DRFLAC_INLINE drflac_uint16 drflac_crc16(drflac_uint16 crc, drflac_cache_t data, drflac_uint32 count)
{
#ifdef DRFLAC_64BIT
  return drflac_crc16__64bit(crc, data, count);
#else
  return drflac_crc16__32bit(crc, data, count);
#endif
}

#ifdef DRFLAC_64BIT
#define drflac__be2host__cache_line drflac__be2host_64
#else
#define drflac__be2host__cache_line drflac__be2host_32
#endif

#define DRFLAC_CACHE_L1_SIZE_BYTES(bs) (sizeof((bs)->cache))
#define DRFLAC_CACHE_L1_SIZE_BITS(bs) (sizeof((bs)->cache) * 8)
#define DRFLAC_CACHE_L1_BITS_REMAINING(bs) (DRFLAC_CACHE_L1_SIZE_BITS(bs) - (bs)->consumedBits)
#define DRFLAC_CACHE_L1_SELECTION_MASK(_bitCount) (~((~(drflac_cache_t)0) >> (_bitCount)))
#define DRFLAC_CACHE_L1_SELECTION_SHIFT(bs, _bitCount) (DRFLAC_CACHE_L1_SIZE_BITS(bs) - (_bitCount))
#define DRFLAC_CACHE_L1_SELECT(bs, _bitCount) (((bs)->cache) & DRFLAC_CACHE_L1_SELECTION_MASK(_bitCount))
#define DRFLAC_CACHE_L1_SELECT_AND_SHIFT(bs, _bitCount)                                                                \
  (DRFLAC_CACHE_L1_SELECT((bs), (_bitCount)) >> DRFLAC_CACHE_L1_SELECTION_SHIFT((bs), (_bitCount)))
#define DRFLAC_CACHE_L1_SELECT_AND_SHIFT_SAFE(bs, _bitCount)                                                           \
  (DRFLAC_CACHE_L1_SELECT((bs), (_bitCount))                                                                           \
   >> (DRFLAC_CACHE_L1_SELECTION_SHIFT((bs), (_bitCount)) & (DRFLAC_CACHE_L1_SIZE_BITS(bs) - 1)))
#define DRFLAC_CACHE_L2_SIZE_BYTES(bs) (sizeof((bs)->cacheL2))
#define DRFLAC_CACHE_L2_LINE_COUNT(bs) (DRFLAC_CACHE_L2_SIZE_BYTES(bs) / sizeof((bs)->cacheL2[0]))
#define DRFLAC_CACHE_L2_LINES_REMAINING(bs) (DRFLAC_CACHE_L2_LINE_COUNT(bs) - (bs)->nextL2Line)

#ifndef DR_FLAC_NO_CRC
static DRFLAC_INLINE void drflac__reset_crc16(drflac_bs *bs)
{
  bs->crc16 = 0;
  bs->crc16CacheIgnoredBytes = bs->consumedBits >> 3;
}

static DRFLAC_INLINE void drflac__update_crc16(drflac_bs *bs)
{
  bs->crc16 =
      drflac_crc16_bytes(bs->crc16, bs->crc16Cache, DRFLAC_CACHE_L1_SIZE_BYTES(bs) - bs->crc16CacheIgnoredBytes);
  bs->crc16CacheIgnoredBytes = 0;
}

static DRFLAC_INLINE drflac_uint16 drflac__flush_crc16(drflac_bs *bs)
{

  drflac_assert((DRFLAC_CACHE_L1_BITS_REMAINING(bs) & 7) == 0);

  if (DRFLAC_CACHE_L1_BITS_REMAINING(bs) == 0) {
    drflac__update_crc16(bs);
  } else {

    bs->crc16 = drflac_crc16_bytes(bs->crc16, bs->crc16Cache >> DRFLAC_CACHE_L1_BITS_REMAINING(bs),
                                   (bs->consumedBits >> 3) - bs->crc16CacheIgnoredBytes);

    bs->crc16CacheIgnoredBytes = bs->consumedBits >> 3;
  }

  return bs->crc16;
}
#endif

static DRFLAC_INLINE drflac_bool32 drflac__reload_l1_cache_from_l2(drflac_bs *bs)
{

  if (bs->nextL2Line < DRFLAC_CACHE_L2_LINE_COUNT(bs)) {
    bs->cache = bs->cacheL2[bs->nextL2Line++];
    return DRFLAC_TRUE;
  }

  if (bs->unalignedByteCount > 0) {
    return DRFLAC_FALSE;
  }

  size_t bytesRead = bs->onRead(bs->pUserData, bs->cacheL2, DRFLAC_CACHE_L2_SIZE_BYTES(bs));

  bs->nextL2Line = 0;
  if (bytesRead == DRFLAC_CACHE_L2_SIZE_BYTES(bs)) {
    bs->cache = bs->cacheL2[bs->nextL2Line++];
    return DRFLAC_TRUE;
  }

  size_t alignedL1LineCount = bytesRead / DRFLAC_CACHE_L1_SIZE_BYTES(bs);

  bs->unalignedByteCount = bytesRead - (alignedL1LineCount * DRFLAC_CACHE_L1_SIZE_BYTES(bs));
  if (bs->unalignedByteCount > 0) {
    bs->unalignedCache = bs->cacheL2[alignedL1LineCount];
  }

  if (alignedL1LineCount > 0) {
    size_t offset = DRFLAC_CACHE_L2_LINE_COUNT(bs) - alignedL1LineCount;
    for (size_t i = alignedL1LineCount; i > 0; --i) {
      bs->cacheL2[i - 1 + offset] = bs->cacheL2[i - 1];
    }

    bs->nextL2Line = (drflac_uint32)offset;
    bs->cache = bs->cacheL2[bs->nextL2Line++];
    return DRFLAC_TRUE;
  } else {

    bs->nextL2Line = DRFLAC_CACHE_L2_LINE_COUNT(bs);
    return DRFLAC_FALSE;
  }
}

static drflac_bool32 drflac__reload_cache(drflac_bs *bs)
{
#ifndef DR_FLAC_NO_CRC
  drflac__update_crc16(bs);
#endif

  if (drflac__reload_l1_cache_from_l2(bs)) {
    bs->cache = drflac__be2host__cache_line(bs->cache);
    bs->consumedBits = 0;
#ifndef DR_FLAC_NO_CRC
    bs->crc16Cache = bs->cache;
#endif
    return DRFLAC_TRUE;
  }

  size_t bytesRead = bs->unalignedByteCount;
  if (bytesRead == 0) {
    bs->consumedBits = DRFLAC_CACHE_L1_SIZE_BITS(bs);
    return DRFLAC_FALSE;
  }

  drflac_assert(bytesRead < DRFLAC_CACHE_L1_SIZE_BYTES(bs));
  bs->consumedBits = (drflac_uint32)(DRFLAC_CACHE_L1_SIZE_BYTES(bs) - bytesRead) * 8;

  bs->cache = drflac__be2host__cache_line(bs->unalignedCache);
  bs->cache &= DRFLAC_CACHE_L1_SELECTION_MASK(DRFLAC_CACHE_L1_BITS_REMAINING(bs));
  bs->unalignedByteCount = 0;

#ifndef DR_FLAC_NO_CRC
  bs->crc16Cache = bs->cache >> bs->consumedBits;
  bs->crc16CacheIgnoredBytes = bs->consumedBits >> 3;
#endif
  return DRFLAC_TRUE;
}

static void drflac__reset_cache(drflac_bs *bs)
{
  bs->nextL2Line = DRFLAC_CACHE_L2_LINE_COUNT(bs);
  bs->consumedBits = DRFLAC_CACHE_L1_SIZE_BITS(bs);
  bs->cache = 0;
  bs->unalignedByteCount = 0;
  bs->unalignedCache = 0;

#ifndef DR_FLAC_NO_CRC
  bs->crc16Cache = 0;
  bs->crc16CacheIgnoredBytes = 0;
#endif
}

static DRFLAC_INLINE drflac_bool32 drflac__read_uint32(drflac_bs *bs, unsigned int bitCount, drflac_uint32 *pResultOut)
{
  drflac_assert(bs != NULL);
  drflac_assert(pResultOut != NULL);
  drflac_assert(bitCount > 0);
  drflac_assert(bitCount <= 32);

  if (bs->consumedBits == DRFLAC_CACHE_L1_SIZE_BITS(bs)) {
    if (!drflac__reload_cache(bs)) {
      return DRFLAC_FALSE;
    }
  }

  if (bitCount <= DRFLAC_CACHE_L1_BITS_REMAINING(bs)) {

#ifdef DRFLAC_64BIT
    *pResultOut = (drflac_uint32)DRFLAC_CACHE_L1_SELECT_AND_SHIFT(bs, bitCount);
    bs->consumedBits += bitCount;
    bs->cache <<= bitCount;
#else
    if (bitCount < DRFLAC_CACHE_L1_SIZE_BITS(bs)) {
      *pResultOut = (drflac_uint32)DRFLAC_CACHE_L1_SELECT_AND_SHIFT(bs, bitCount);
      bs->consumedBits += bitCount;
      bs->cache <<= bitCount;
    } else {

      *pResultOut = (drflac_uint32)bs->cache;
      bs->consumedBits = DRFLAC_CACHE_L1_SIZE_BITS(bs);
      bs->cache = 0;
    }
#endif

    return DRFLAC_TRUE;
  } else {

    drflac_uint32 bitCountHi = DRFLAC_CACHE_L1_BITS_REMAINING(bs);
    drflac_uint32 bitCountLo = bitCount - bitCountHi;
    drflac_uint32 resultHi = (drflac_uint32)DRFLAC_CACHE_L1_SELECT_AND_SHIFT(bs, bitCountHi);

    if (!drflac__reload_cache(bs)) {
      return DRFLAC_FALSE;
    }

    *pResultOut = (resultHi << bitCountLo) | (drflac_uint32)DRFLAC_CACHE_L1_SELECT_AND_SHIFT(bs, bitCountLo);
    bs->consumedBits += bitCountLo;
    bs->cache <<= bitCountLo;
    return DRFLAC_TRUE;
  }
}

static drflac_bool32 drflac__read_int32(drflac_bs *bs, unsigned int bitCount, drflac_int32 *pResult)
{
  drflac_assert(bs != NULL);
  drflac_assert(pResult != NULL);
  drflac_assert(bitCount > 0);
  drflac_assert(bitCount <= 32);

  drflac_uint32 result;
  if (!drflac__read_uint32(bs, bitCount, &result)) {
    return DRFLAC_FALSE;
  }

  drflac_uint32 signbit = ((result >> (bitCount - 1)) & 0x01);
  result |= (~signbit + 1) << bitCount;

  *pResult = (drflac_int32)result;
  return DRFLAC_TRUE;
}

#ifdef DRFLAC_64BIT
static drflac_bool32 drflac__read_uint64(drflac_bs *bs, unsigned int bitCount, drflac_uint64 *pResultOut)
{
  drflac_assert(bitCount <= 64);
  drflac_assert(bitCount > 32);

  drflac_uint32 resultHi;
  if (!drflac__read_uint32(bs, bitCount - 32, &resultHi)) {
    return DRFLAC_FALSE;
  }

  drflac_uint32 resultLo;
  if (!drflac__read_uint32(bs, 32, &resultLo)) {
    return DRFLAC_FALSE;
  }

  *pResultOut = (((drflac_uint64)resultHi) << 32) | ((drflac_uint64)resultLo);
  return DRFLAC_TRUE;
}
#endif

#if 0
static drflac_bool32 drflac__read_int64(drflac_bs* bs, unsigned int bitCount, drflac_int64* pResultOut)
{
    drflac_assert(bitCount <= 64);

    drflac_uint64 result;
    if (!drflac__read_uint64(bs, bitCount, &result)) {
        return DRFLAC_FALSE;
    }

    drflac_uint64 signbit = ((result >> (bitCount-1)) & 0x01);
    result |= (~signbit + 1) << bitCount;

    *pResultOut = (drflac_int64)result;
    return DRFLAC_TRUE;
}
#endif

static drflac_bool32 drflac__read_uint16(drflac_bs *bs, unsigned int bitCount, drflac_uint16 *pResult)
{
  drflac_assert(bs != NULL);
  drflac_assert(pResult != NULL);
  drflac_assert(bitCount > 0);
  drflac_assert(bitCount <= 16);

  drflac_uint32 result;
  if (!drflac__read_uint32(bs, bitCount, &result)) {
    return DRFLAC_FALSE;
  }

  *pResult = (drflac_uint16)result;
  return DRFLAC_TRUE;
}

#if 0
static drflac_bool32 drflac__read_int16(drflac_bs* bs, unsigned int bitCount, drflac_int16* pResult)
{
    drflac_assert(bs != NULL);
    drflac_assert(pResult != NULL);
    drflac_assert(bitCount > 0);
    drflac_assert(bitCount <= 16);

    drflac_int32 result;
    if (!drflac__read_int32(bs, bitCount, &result)) {
        return DRFLAC_FALSE;
    }

    *pResult = (drflac_int16)result;
    return DRFLAC_TRUE;
}
#endif

static drflac_bool32 drflac__read_uint8(drflac_bs *bs, unsigned int bitCount, drflac_uint8 *pResult)
{
  drflac_assert(bs != NULL);
  drflac_assert(pResult != NULL);
  drflac_assert(bitCount > 0);
  drflac_assert(bitCount <= 8);

  drflac_uint32 result;
  if (!drflac__read_uint32(bs, bitCount, &result)) {
    return DRFLAC_FALSE;
  }

  *pResult = (drflac_uint8)result;
  return DRFLAC_TRUE;
}

static drflac_bool32 drflac__read_int8(drflac_bs *bs, unsigned int bitCount, drflac_int8 *pResult)
{
  drflac_assert(bs != NULL);
  drflac_assert(pResult != NULL);
  drflac_assert(bitCount > 0);
  drflac_assert(bitCount <= 8);

  drflac_int32 result;
  if (!drflac__read_int32(bs, bitCount, &result)) {
    return DRFLAC_FALSE;
  }

  *pResult = (drflac_int8)result;
  return DRFLAC_TRUE;
}

static drflac_bool32 drflac__seek_bits(drflac_bs *bs, size_t bitsToSeek)
{
  if (bitsToSeek <= DRFLAC_CACHE_L1_BITS_REMAINING(bs)) {
    bs->consumedBits += (drflac_uint32)bitsToSeek;
    bs->cache <<= bitsToSeek;
    return DRFLAC_TRUE;
  } else {

    bitsToSeek -= DRFLAC_CACHE_L1_BITS_REMAINING(bs);
    bs->consumedBits += DRFLAC_CACHE_L1_BITS_REMAINING(bs);
    bs->cache = 0;

#ifdef DRFLAC_64BIT
    while (bitsToSeek >= DRFLAC_CACHE_L1_SIZE_BITS(bs)) {
      drflac_uint64 bin;
      if (!drflac__read_uint64(bs, DRFLAC_CACHE_L1_SIZE_BITS(bs), &bin)) {
        return DRFLAC_FALSE;
      }
      bitsToSeek -= DRFLAC_CACHE_L1_SIZE_BITS(bs);
    }
#else
    while (bitsToSeek >= DRFLAC_CACHE_L1_SIZE_BITS(bs)) {
      drflac_uint32 bin;
      if (!drflac__read_uint32(bs, DRFLAC_CACHE_L1_SIZE_BITS(bs), &bin)) {
        return DRFLAC_FALSE;
      }
      bitsToSeek -= DRFLAC_CACHE_L1_SIZE_BITS(bs);
    }
#endif

    while (bitsToSeek >= 8) {
      drflac_uint8 bin;
      if (!drflac__read_uint8(bs, 8, &bin)) {
        return DRFLAC_FALSE;
      }
      bitsToSeek -= 8;
    }

    if (bitsToSeek > 0) {
      drflac_uint8 bin;
      if (!drflac__read_uint8(bs, (drflac_uint32)bitsToSeek, &bin)) {
        return DRFLAC_FALSE;
      }
      bitsToSeek = 0;
    }

    drflac_assert(bitsToSeek == 0);
    return DRFLAC_TRUE;
  }
}

static drflac_bool32 drflac__find_and_seek_to_next_sync_code(drflac_bs *bs)
{
  drflac_assert(bs != NULL);

  if (!drflac__seek_bits(bs, DRFLAC_CACHE_L1_BITS_REMAINING(bs) & 7)) {
    return DRFLAC_FALSE;
  }

  for (;;) {
#ifndef DR_FLAC_NO_CRC
    drflac__reset_crc16(bs);
#endif

    drflac_uint8 hi;
    if (!drflac__read_uint8(bs, 8, &hi)) {
      return DRFLAC_FALSE;
    }

    if (hi == 0xFF) {
      drflac_uint8 lo;
      if (!drflac__read_uint8(bs, 6, &lo)) {
        return DRFLAC_FALSE;
      }

      if (lo == 0x3E) {
        return DRFLAC_TRUE;
      } else {
        if (!drflac__seek_bits(bs, DRFLAC_CACHE_L1_BITS_REMAINING(bs) & 7)) {
          return DRFLAC_FALSE;
        }
      }
    }
  }
}

#if !defined(DR_FLAC_NO_SIMD) && defined(DRFLAC_HAS_LZCNT_INTRINSIC)
#define DRFLAC_IMPLEMENT_CLZ_LZCNT
#endif
#if defined(_MSC_VER) && _MSC_VER >= 1400 && (defined(DRFLAC_X64) || defined(DRFLAC_X86))
#define DRFLAC_IMPLEMENT_CLZ_MSVC
#endif

static DRFLAC_INLINE drflac_uint32 drflac__clz_software(drflac_cache_t x)
{
  static drflac_uint32 clz_table_4[] = {0, 4, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1};

  drflac_uint32 n = clz_table_4[x >> (sizeof(x) * 8 - 4)];
  if (n == 0) {
#ifdef DRFLAC_64BIT
    if ((x & 0xFFFFFFFF00000000ULL) == 0) {
      n = 32;
      x <<= 32;
    }
    if ((x & 0xFFFF000000000000ULL) == 0) {
      n += 16;
      x <<= 16;
    }
    if ((x & 0xFF00000000000000ULL) == 0) {
      n += 8;
      x <<= 8;
    }
    if ((x & 0xF000000000000000ULL) == 0) {
      n += 4;
      x <<= 4;
    }
#else
    if ((x & 0xFFFF0000) == 0) {
      n = 16;
      x <<= 16;
    }
    if ((x & 0xFF000000) == 0) {
      n += 8;
      x <<= 8;
    }
    if ((x & 0xF0000000) == 0) {
      n += 4;
      x <<= 4;
    }
#endif
    n += clz_table_4[x >> (sizeof(x) * 8 - 4)];
  }

  return n - 1;
}

#ifdef DRFLAC_IMPLEMENT_CLZ_LZCNT
static DRFLAC_INLINE drflac_bool32 drflac__is_lzcnt_supported()
{

#ifdef DRFLAC_HAS_LZCNT_INTRINSIC
  return drflac__gIsLZCNTSupported;
#else
  return DRFLAC_FALSE;
#endif
}

static DRFLAC_INLINE drflac_uint32 drflac__clz_lzcnt(drflac_cache_t x)
{
#if defined(_MSC_VER) && !defined(__clang__)
#ifdef DRFLAC_64BIT
  return (drflac_uint32)__lzcnt64(x);
#else
  return (drflac_uint32)__lzcnt(x);
#endif
#else
#if defined(__GNUC__) || defined(__clang__)
#ifdef DRFLAC_64BIT
  return (drflac_uint32)__builtin_clzll((unsigned long long)x);
#else
  return (drflac_uint32)__builtin_clzl((unsigned long)x);
#endif
#else

#error "This compiler does not support the lzcnt intrinsic."
#endif
#endif
}
#endif

#ifdef DRFLAC_IMPLEMENT_CLZ_MSVC
#include <intrin.h>

static DRFLAC_INLINE drflac_uint32 drflac__clz_msvc(drflac_cache_t x)
{
  drflac_uint32 n;
#ifdef DRFLAC_64BIT
  _BitScanReverse64((unsigned long *)&n, x);
#else
  _BitScanReverse((unsigned long *)&n, x);
#endif
  return sizeof(x) * 8 - n - 1;
}
#endif

static DRFLAC_INLINE drflac_uint32 drflac__clz(drflac_cache_t x)
{

#ifdef DRFLAC_IMPLEMENT_CLZ_LZCNT
  if (drflac__is_lzcnt_supported()) {
    return drflac__clz_lzcnt(x);
  } else
#endif
  {
#ifdef DRFLAC_IMPLEMENT_CLZ_MSVC
    return drflac__clz_msvc(x);
#else
    return drflac__clz_software(x);
#endif
  }
}

static inline drflac_bool32 drflac__seek_past_next_set_bit(drflac_bs *bs, unsigned int *pOffsetOut)
{
  drflac_uint32 zeroCounter = 0;
  while (bs->cache == 0) {
    zeroCounter += (drflac_uint32)DRFLAC_CACHE_L1_BITS_REMAINING(bs);
    if (!drflac__reload_cache(bs)) {
      return DRFLAC_FALSE;
    }
  }

  drflac_uint32 setBitOffsetPlus1 = drflac__clz(bs->cache);
  setBitOffsetPlus1 += 1;

  bs->consumedBits += setBitOffsetPlus1;
  bs->cache <<= setBitOffsetPlus1;

  *pOffsetOut = zeroCounter + setBitOffsetPlus1 - 1;
  return DRFLAC_TRUE;
}

static drflac_bool32 drflac__seek_to_byte(drflac_bs *bs, drflac_uint64 offsetFromStart)
{
  drflac_assert(bs != NULL);
  drflac_assert(offsetFromStart > 0);

  if (offsetFromStart > 0x7FFFFFFF) {
    drflac_uint64 bytesRemaining = offsetFromStart;
    if (!bs->onSeek(bs->pUserData, 0x7FFFFFFF, drflac_seek_origin_start)) {
      return DRFLAC_FALSE;
    }
    bytesRemaining -= 0x7FFFFFFF;

    while (bytesRemaining > 0x7FFFFFFF) {
      if (!bs->onSeek(bs->pUserData, 0x7FFFFFFF, drflac_seek_origin_current)) {
        return DRFLAC_FALSE;
      }
      bytesRemaining -= 0x7FFFFFFF;
    }

    if (bytesRemaining > 0) {
      if (!bs->onSeek(bs->pUserData, (int)bytesRemaining, drflac_seek_origin_current)) {
        return DRFLAC_FALSE;
      }
    }
  } else {
    if (!bs->onSeek(bs->pUserData, (int)offsetFromStart, drflac_seek_origin_start)) {
      return DRFLAC_FALSE;
    }
  }

  drflac__reset_cache(bs);
  return DRFLAC_TRUE;
}

static drflac_result drflac__read_utf8_coded_number(drflac_bs *bs, drflac_uint64 *pNumberOut, drflac_uint8 *pCRCOut)
{
  drflac_assert(bs != NULL);
  drflac_assert(pNumberOut != NULL);

  drflac_uint8 crc = *pCRCOut;

  unsigned char utf8[7] = {0};
  if (!drflac__read_uint8(bs, 8, utf8)) {
    *pNumberOut = 0;
    return DRFLAC_END_OF_STREAM;
  }
  crc = drflac_crc8(crc, utf8[0], 8);

  if ((utf8[0] & 0x80) == 0) {
    *pNumberOut = utf8[0];
    *pCRCOut = crc;
    return DRFLAC_SUCCESS;
  }

  int byteCount = 1;
  if ((utf8[0] & 0xE0) == 0xC0) {
    byteCount = 2;
  } else if ((utf8[0] & 0xF0) == 0xE0) {
    byteCount = 3;
  } else if ((utf8[0] & 0xF8) == 0xF0) {
    byteCount = 4;
  } else if ((utf8[0] & 0xFC) == 0xF8) {
    byteCount = 5;
  } else if ((utf8[0] & 0xFE) == 0xFC) {
    byteCount = 6;
  } else if ((utf8[0] & 0xFF) == 0xFE) {
    byteCount = 7;
  } else {
    *pNumberOut = 0;
    return DRFLAC_CRC_MISMATCH;
  }

  drflac_assert(byteCount > 1);

  drflac_uint64 result = (drflac_uint64)(utf8[0] & (0xFF >> (byteCount + 1)));
  for (int i = 1; i < byteCount; ++i) {
    if (!drflac__read_uint8(bs, 8, utf8 + i)) {
      *pNumberOut = 0;
      return DRFLAC_END_OF_STREAM;
    }
    crc = drflac_crc8(crc, utf8[i], 8);

    result = (result << 6) | (utf8[i] & 0x3F);
  }

  *pNumberOut = result;
  *pCRCOut = crc;
  return DRFLAC_SUCCESS;
}

static DRFLAC_INLINE drflac_int32 drflac__calculate_prediction_32(drflac_uint32 order, drflac_int32 shift,
                                                                  const drflac_int32 *coefficients,
                                                                  drflac_int32 *pDecodedSamples)
{
  drflac_assert(order <= 32);

  drflac_int32 prediction = 0;

  switch (order) {
  case 32:
    prediction += coefficients[31] * pDecodedSamples[-32];
  case 31:
    prediction += coefficients[30] * pDecodedSamples[-31];
  case 30:
    prediction += coefficients[29] * pDecodedSamples[-30];
  case 29:
    prediction += coefficients[28] * pDecodedSamples[-29];
  case 28:
    prediction += coefficients[27] * pDecodedSamples[-28];
  case 27:
    prediction += coefficients[26] * pDecodedSamples[-27];
  case 26:
    prediction += coefficients[25] * pDecodedSamples[-26];
  case 25:
    prediction += coefficients[24] * pDecodedSamples[-25];
  case 24:
    prediction += coefficients[23] * pDecodedSamples[-24];
  case 23:
    prediction += coefficients[22] * pDecodedSamples[-23];
  case 22:
    prediction += coefficients[21] * pDecodedSamples[-22];
  case 21:
    prediction += coefficients[20] * pDecodedSamples[-21];
  case 20:
    prediction += coefficients[19] * pDecodedSamples[-20];
  case 19:
    prediction += coefficients[18] * pDecodedSamples[-19];
  case 18:
    prediction += coefficients[17] * pDecodedSamples[-18];
  case 17:
    prediction += coefficients[16] * pDecodedSamples[-17];
  case 16:
    prediction += coefficients[15] * pDecodedSamples[-16];
  case 15:
    prediction += coefficients[14] * pDecodedSamples[-15];
  case 14:
    prediction += coefficients[13] * pDecodedSamples[-14];
  case 13:
    prediction += coefficients[12] * pDecodedSamples[-13];
  case 12:
    prediction += coefficients[11] * pDecodedSamples[-12];
  case 11:
    prediction += coefficients[10] * pDecodedSamples[-11];
  case 10:
    prediction += coefficients[9] * pDecodedSamples[-10];
  case 9:
    prediction += coefficients[8] * pDecodedSamples[-9];
  case 8:
    prediction += coefficients[7] * pDecodedSamples[-8];
  case 7:
    prediction += coefficients[6] * pDecodedSamples[-7];
  case 6:
    prediction += coefficients[5] * pDecodedSamples[-6];
  case 5:
    prediction += coefficients[4] * pDecodedSamples[-5];
  case 4:
    prediction += coefficients[3] * pDecodedSamples[-4];
  case 3:
    prediction += coefficients[2] * pDecodedSamples[-3];
  case 2:
    prediction += coefficients[1] * pDecodedSamples[-2];
  case 1:
    prediction += coefficients[0] * pDecodedSamples[-1];
  }

  return (drflac_int32)(prediction >> shift);
}

static DRFLAC_INLINE drflac_int32 drflac__calculate_prediction_64(drflac_uint32 order, drflac_int32 shift,
                                                                  const drflac_int32 *coefficients,
                                                                  drflac_int32 *pDecodedSamples)
{
  drflac_assert(order <= 32);

#ifndef DRFLAC_64BIT
  drflac_int64 prediction;
  if (order == 8) {
    prediction = coefficients[0] * (drflac_int64)pDecodedSamples[-1];
    prediction += coefficients[1] * (drflac_int64)pDecodedSamples[-2];
    prediction += coefficients[2] * (drflac_int64)pDecodedSamples[-3];
    prediction += coefficients[3] * (drflac_int64)pDecodedSamples[-4];
    prediction += coefficients[4] * (drflac_int64)pDecodedSamples[-5];
    prediction += coefficients[5] * (drflac_int64)pDecodedSamples[-6];
    prediction += coefficients[6] * (drflac_int64)pDecodedSamples[-7];
    prediction += coefficients[7] * (drflac_int64)pDecodedSamples[-8];
  } else if (order == 7) {
    prediction = coefficients[0] * (drflac_int64)pDecodedSamples[-1];
    prediction += coefficients[1] * (drflac_int64)pDecodedSamples[-2];
    prediction += coefficients[2] * (drflac_int64)pDecodedSamples[-3];
    prediction += coefficients[3] * (drflac_int64)pDecodedSamples[-4];
    prediction += coefficients[4] * (drflac_int64)pDecodedSamples[-5];
    prediction += coefficients[5] * (drflac_int64)pDecodedSamples[-6];
    prediction += coefficients[6] * (drflac_int64)pDecodedSamples[-7];
  } else if (order == 3) {
    prediction = coefficients[0] * (drflac_int64)pDecodedSamples[-1];
    prediction += coefficients[1] * (drflac_int64)pDecodedSamples[-2];
    prediction += coefficients[2] * (drflac_int64)pDecodedSamples[-3];
  } else if (order == 6) {
    prediction = coefficients[0] * (drflac_int64)pDecodedSamples[-1];
    prediction += coefficients[1] * (drflac_int64)pDecodedSamples[-2];
    prediction += coefficients[2] * (drflac_int64)pDecodedSamples[-3];
    prediction += coefficients[3] * (drflac_int64)pDecodedSamples[-4];
    prediction += coefficients[4] * (drflac_int64)pDecodedSamples[-5];
    prediction += coefficients[5] * (drflac_int64)pDecodedSamples[-6];
  } else if (order == 5) {
    prediction = coefficients[0] * (drflac_int64)pDecodedSamples[-1];
    prediction += coefficients[1] * (drflac_int64)pDecodedSamples[-2];
    prediction += coefficients[2] * (drflac_int64)pDecodedSamples[-3];
    prediction += coefficients[3] * (drflac_int64)pDecodedSamples[-4];
    prediction += coefficients[4] * (drflac_int64)pDecodedSamples[-5];
  } else if (order == 4) {
    prediction = coefficients[0] * (drflac_int64)pDecodedSamples[-1];
    prediction += coefficients[1] * (drflac_int64)pDecodedSamples[-2];
    prediction += coefficients[2] * (drflac_int64)pDecodedSamples[-3];
    prediction += coefficients[3] * (drflac_int64)pDecodedSamples[-4];
  } else if (order == 12) {
    prediction = coefficients[0] * (drflac_int64)pDecodedSamples[-1];
    prediction += coefficients[1] * (drflac_int64)pDecodedSamples[-2];
    prediction += coefficients[2] * (drflac_int64)pDecodedSamples[-3];
    prediction += coefficients[3] * (drflac_int64)pDecodedSamples[-4];
    prediction += coefficients[4] * (drflac_int64)pDecodedSamples[-5];
    prediction += coefficients[5] * (drflac_int64)pDecodedSamples[-6];
    prediction += coefficients[6] * (drflac_int64)pDecodedSamples[-7];
    prediction += coefficients[7] * (drflac_int64)pDecodedSamples[-8];
    prediction += coefficients[8] * (drflac_int64)pDecodedSamples[-9];
    prediction += coefficients[9] * (drflac_int64)pDecodedSamples[-10];
    prediction += coefficients[10] * (drflac_int64)pDecodedSamples[-11];
    prediction += coefficients[11] * (drflac_int64)pDecodedSamples[-12];
  } else if (order == 2) {
    prediction = coefficients[0] * (drflac_int64)pDecodedSamples[-1];
    prediction += coefficients[1] * (drflac_int64)pDecodedSamples[-2];
  } else if (order == 1) {
    prediction = coefficients[0] * (drflac_int64)pDecodedSamples[-1];
  } else if (order == 10) {
    prediction = coefficients[0] * (drflac_int64)pDecodedSamples[-1];
    prediction += coefficients[1] * (drflac_int64)pDecodedSamples[-2];
    prediction += coefficients[2] * (drflac_int64)pDecodedSamples[-3];
    prediction += coefficients[3] * (drflac_int64)pDecodedSamples[-4];
    prediction += coefficients[4] * (drflac_int64)pDecodedSamples[-5];
    prediction += coefficients[5] * (drflac_int64)pDecodedSamples[-6];
    prediction += coefficients[6] * (drflac_int64)pDecodedSamples[-7];
    prediction += coefficients[7] * (drflac_int64)pDecodedSamples[-8];
    prediction += coefficients[8] * (drflac_int64)pDecodedSamples[-9];
    prediction += coefficients[9] * (drflac_int64)pDecodedSamples[-10];
  } else if (order == 9) {
    prediction = coefficients[0] * (drflac_int64)pDecodedSamples[-1];
    prediction += coefficients[1] * (drflac_int64)pDecodedSamples[-2];
    prediction += coefficients[2] * (drflac_int64)pDecodedSamples[-3];
    prediction += coefficients[3] * (drflac_int64)pDecodedSamples[-4];
    prediction += coefficients[4] * (drflac_int64)pDecodedSamples[-5];
    prediction += coefficients[5] * (drflac_int64)pDecodedSamples[-6];
    prediction += coefficients[6] * (drflac_int64)pDecodedSamples[-7];
    prediction += coefficients[7] * (drflac_int64)pDecodedSamples[-8];
    prediction += coefficients[8] * (drflac_int64)pDecodedSamples[-9];
  } else if (order == 11) {
    prediction = coefficients[0] * (drflac_int64)pDecodedSamples[-1];
    prediction += coefficients[1] * (drflac_int64)pDecodedSamples[-2];
    prediction += coefficients[2] * (drflac_int64)pDecodedSamples[-3];
    prediction += coefficients[3] * (drflac_int64)pDecodedSamples[-4];
    prediction += coefficients[4] * (drflac_int64)pDecodedSamples[-5];
    prediction += coefficients[5] * (drflac_int64)pDecodedSamples[-6];
    prediction += coefficients[6] * (drflac_int64)pDecodedSamples[-7];
    prediction += coefficients[7] * (drflac_int64)pDecodedSamples[-8];
    prediction += coefficients[8] * (drflac_int64)pDecodedSamples[-9];
    prediction += coefficients[9] * (drflac_int64)pDecodedSamples[-10];
    prediction += coefficients[10] * (drflac_int64)pDecodedSamples[-11];
  } else {
    prediction = 0;
    for (int j = 0; j < (int)order; ++j) {
      prediction += coefficients[j] * (drflac_int64)pDecodedSamples[-j - 1];
    }
  }
#endif

#ifdef DRFLAC_64BIT
  drflac_int64 prediction = 0;

  switch (order) {
  case 32:
    prediction += coefficients[31] * (drflac_int64)pDecodedSamples[-32];
  case 31:
    prediction += coefficients[30] * (drflac_int64)pDecodedSamples[-31];
  case 30:
    prediction += coefficients[29] * (drflac_int64)pDecodedSamples[-30];
  case 29:
    prediction += coefficients[28] * (drflac_int64)pDecodedSamples[-29];
  case 28:
    prediction += coefficients[27] * (drflac_int64)pDecodedSamples[-28];
  case 27:
    prediction += coefficients[26] * (drflac_int64)pDecodedSamples[-27];
  case 26:
    prediction += coefficients[25] * (drflac_int64)pDecodedSamples[-26];
  case 25:
    prediction += coefficients[24] * (drflac_int64)pDecodedSamples[-25];
  case 24:
    prediction += coefficients[23] * (drflac_int64)pDecodedSamples[-24];
  case 23:
    prediction += coefficients[22] * (drflac_int64)pDecodedSamples[-23];
  case 22:
    prediction += coefficients[21] * (drflac_int64)pDecodedSamples[-22];
  case 21:
    prediction += coefficients[20] * (drflac_int64)pDecodedSamples[-21];
  case 20:
    prediction += coefficients[19] * (drflac_int64)pDecodedSamples[-20];
  case 19:
    prediction += coefficients[18] * (drflac_int64)pDecodedSamples[-19];
  case 18:
    prediction += coefficients[17] * (drflac_int64)pDecodedSamples[-18];
  case 17:
    prediction += coefficients[16] * (drflac_int64)pDecodedSamples[-17];
  case 16:
    prediction += coefficients[15] * (drflac_int64)pDecodedSamples[-16];
  case 15:
    prediction += coefficients[14] * (drflac_int64)pDecodedSamples[-15];
  case 14:
    prediction += coefficients[13] * (drflac_int64)pDecodedSamples[-14];
  case 13:
    prediction += coefficients[12] * (drflac_int64)pDecodedSamples[-13];
  case 12:
    prediction += coefficients[11] * (drflac_int64)pDecodedSamples[-12];
  case 11:
    prediction += coefficients[10] * (drflac_int64)pDecodedSamples[-11];
  case 10:
    prediction += coefficients[9] * (drflac_int64)pDecodedSamples[-10];
  case 9:
    prediction += coefficients[8] * (drflac_int64)pDecodedSamples[-9];
  case 8:
    prediction += coefficients[7] * (drflac_int64)pDecodedSamples[-8];
  case 7:
    prediction += coefficients[6] * (drflac_int64)pDecodedSamples[-7];
  case 6:
    prediction += coefficients[5] * (drflac_int64)pDecodedSamples[-6];
  case 5:
    prediction += coefficients[4] * (drflac_int64)pDecodedSamples[-5];
  case 4:
    prediction += coefficients[3] * (drflac_int64)pDecodedSamples[-4];
  case 3:
    prediction += coefficients[2] * (drflac_int64)pDecodedSamples[-3];
  case 2:
    prediction += coefficients[1] * (drflac_int64)pDecodedSamples[-2];
  case 1:
    prediction += coefficients[0] * (drflac_int64)pDecodedSamples[-1];
  }
#endif

  return (drflac_int32)(prediction >> shift);
}

#if 0


static drflac_bool32 drflac__decode_samples_with_residual__rice__reference(
    drflac_bs* bs,
    drflac_uint32 bitsPerSample,
    drflac_uint32 count,
    drflac_uint8 riceParam,
    drflac_uint32 order,
    drflac_int32 shift,
    const drflac_int32* coefficients,
    drflac_int32* pSamplesOut)
{
    drflac_assert(bs != NULL);
    drflac_assert(count > 0);
    drflac_assert(pSamplesOut != NULL);

    for (drflac_uint32 i = 0; i < count; ++i) {
        drflac_uint32 zeroCounter = 0;
        for (;;) {
            drflac_uint8 bit;
            if (!drflac__read_uint8(bs, 1, &bit)) {
                return DRFLAC_FALSE;
            }

            if (bit == 0) {
                zeroCounter += 1;
            } else {
                break;
            }
        }

        drflac_uint32 decodedRice;
        if (riceParam > 0) {
            if (!drflac__read_uint32(bs, riceParam, &decodedRice)) {
                return DRFLAC_FALSE;
            }
        } else {
            decodedRice = 0;
        }

        decodedRice |= (zeroCounter << riceParam);
        if ((decodedRice & 0x01)) {
            decodedRice = ~(decodedRice >> 1);
        } else {
            decodedRice =  (decodedRice >> 1);
        }


        if (bitsPerSample > 16) {
            pSamplesOut[i] = decodedRice + drflac__calculate_prediction_64(order, shift, coefficients, pSamplesOut + i);
        } else {
            pSamplesOut[i] = decodedRice + drflac__calculate_prediction_32(order, shift, coefficients, pSamplesOut + i);
        }
    }

    return DRFLAC_TRUE;
}
#endif

#if 0
static drflac_bool32 drflac__read_rice_parts__reference(
    drflac_bs* bs, drflac_uint8 riceParam, drflac_uint32* pZeroCounterOut, drflac_uint32* pRiceParamPartOut)
{
    drflac_uint32 zeroCounter = 0;
    for (;;) {
        drflac_uint8 bit;
        if (!drflac__read_uint8(bs, 1, &bit)) {
            return DRFLAC_FALSE;
        }

        if (bit == 0) {
            zeroCounter += 1;
        } else {
            break;
        }
    }

    drflac_uint32 decodedRice;
    if (riceParam > 0) {
        if (!drflac__read_uint32(bs, riceParam, &decodedRice)) {
            return DRFLAC_FALSE;
        }
    } else {
        decodedRice = 0;
    }

    *pZeroCounterOut = zeroCounter;
    *pRiceParamPartOut = decodedRice;
    return DRFLAC_TRUE;
}
#endif

static DRFLAC_INLINE drflac_bool32 drflac__read_rice_parts(drflac_bs *bs, drflac_uint8 riceParam,
                                                           drflac_uint32 *pZeroCounterOut,
                                                           drflac_uint32 *pRiceParamPartOut)
{
  drflac_assert(riceParam > 0);

  drflac_cache_t riceParamMask = DRFLAC_CACHE_L1_SELECTION_MASK(riceParam);

  drflac_uint32 zeroCounter = 0;
  while (bs->cache == 0) {
    zeroCounter += (drflac_uint32)DRFLAC_CACHE_L1_BITS_REMAINING(bs);
    if (!drflac__reload_cache(bs)) {
      return DRFLAC_FALSE;
    }
  }

  drflac_uint32 setBitOffsetPlus1 = drflac__clz(bs->cache);
  zeroCounter += setBitOffsetPlus1;
  setBitOffsetPlus1 += 1;

  drflac_uint32 riceParamPart;
  drflac_uint32 riceLength = setBitOffsetPlus1 + riceParam;
  if (riceLength < DRFLAC_CACHE_L1_BITS_REMAINING(bs)) {
    riceParamPart = (drflac_uint32)((bs->cache & (riceParamMask >> setBitOffsetPlus1))
                                    >> DRFLAC_CACHE_L1_SELECTION_SHIFT(bs, riceLength));

    bs->consumedBits += riceLength;
    bs->cache <<= riceLength;
  } else {
    bs->consumedBits += riceLength;
    bs->cache <<= setBitOffsetPlus1 & (DRFLAC_CACHE_L1_SIZE_BITS(bs) - 1);

    drflac_uint32 bitCountLo = bs->consumedBits - DRFLAC_CACHE_L1_SIZE_BITS(bs);
    drflac_cache_t resultHi = DRFLAC_CACHE_L1_SELECT_AND_SHIFT(bs, riceParam);

    if (bs->nextL2Line < DRFLAC_CACHE_L2_LINE_COUNT(bs)) {
#ifndef DR_FLAC_NO_CRC
      drflac__update_crc16(bs);
#endif
      bs->cache = drflac__be2host__cache_line(bs->cacheL2[bs->nextL2Line++]);
      bs->consumedBits = 0;
#ifndef DR_FLAC_NO_CRC
      bs->crc16Cache = bs->cache;
#endif
    } else {

      if (!drflac__reload_cache(bs)) {
        return DRFLAC_FALSE;
      }
    }

    riceParamPart = (drflac_uint32)(resultHi | DRFLAC_CACHE_L1_SELECT_AND_SHIFT_SAFE(bs, bitCountLo));

    bs->consumedBits += bitCountLo;
    bs->cache <<= bitCountLo;
  }

  *pZeroCounterOut = zeroCounter;
  *pRiceParamPartOut = riceParamPart;
  return DRFLAC_TRUE;
}

static DRFLAC_INLINE drflac_bool32 drflac__read_rice_parts__param_equals_zero(drflac_bs *bs,
                                                                              drflac_uint32 *pZeroCounterOut,
                                                                              drflac_uint32 *pRiceParamPartOut)
{
  drflac_cache_t riceParamMask = DRFLAC_CACHE_L1_SELECTION_MASK(0);

  drflac_uint32 zeroCounter = 0;
  while (bs->cache == 0) {
    zeroCounter += (drflac_uint32)DRFLAC_CACHE_L1_BITS_REMAINING(bs);
    if (!drflac__reload_cache(bs)) {
      return DRFLAC_FALSE;
    }
  }

  drflac_uint32 setBitOffsetPlus1 = drflac__clz(bs->cache);
  zeroCounter += setBitOffsetPlus1;
  setBitOffsetPlus1 += 1;

  drflac_uint32 riceParamPart;
  drflac_uint32 riceLength = setBitOffsetPlus1;
  if (riceLength < DRFLAC_CACHE_L1_BITS_REMAINING(bs)) {
    riceParamPart = (drflac_uint32)((bs->cache & (riceParamMask >> setBitOffsetPlus1))
                                    >> DRFLAC_CACHE_L1_SELECTION_SHIFT(bs, riceLength));

    bs->consumedBits += riceLength;
    bs->cache <<= riceLength;
  } else {

    drflac_uint32 bitCountLo = riceLength + bs->consumedBits - DRFLAC_CACHE_L1_SIZE_BITS(bs);

    if (bs->nextL2Line < DRFLAC_CACHE_L2_LINE_COUNT(bs)) {
#ifndef DR_FLAC_NO_CRC
      drflac__update_crc16(bs);
#endif
      bs->cache = drflac__be2host__cache_line(bs->cacheL2[bs->nextL2Line++]);
      bs->consumedBits = 0;
#ifndef DR_FLAC_NO_CRC
      bs->crc16Cache = bs->cache;
#endif
    } else {

      if (!drflac__reload_cache(bs)) {
        return DRFLAC_FALSE;
      }
    }

    riceParamPart = (drflac_uint32)(DRFLAC_CACHE_L1_SELECT_AND_SHIFT_SAFE(bs, bitCountLo));

    bs->consumedBits += bitCountLo;
    bs->cache <<= bitCountLo;
  }

  *pZeroCounterOut = zeroCounter;
  *pRiceParamPartOut = riceParamPart;
  return DRFLAC_TRUE;
}

static drflac_bool32 drflac__decode_samples_with_residual__rice__simple(drflac_bs *bs, drflac_uint32 bitsPerSample,
                                                                        drflac_uint32 count, drflac_uint8 riceParam,
                                                                        drflac_uint32 order, drflac_int32 shift,
                                                                        const drflac_int32 *coefficients,
                                                                        drflac_int32 *pSamplesOut)
{
  drflac_assert(bs != NULL);
  drflac_assert(count > 0);
  drflac_assert(pSamplesOut != NULL);

  static drflac_uint32 t[2] = {0x00000000, 0xFFFFFFFF};

  drflac_uint32 zeroCountPart0;
  drflac_uint32 zeroCountPart1;
  drflac_uint32 zeroCountPart2;
  drflac_uint32 zeroCountPart3;
  drflac_uint32 riceParamPart0;
  drflac_uint32 riceParamPart1;
  drflac_uint32 riceParamPart2;
  drflac_uint32 riceParamPart3;
  drflac_uint32 i4 = 0;
  drflac_uint32 count4 = count >> 2;
  while (i4 < count4) {

    if (!drflac__read_rice_parts(bs, riceParam, &zeroCountPart0, &riceParamPart0)
        || !drflac__read_rice_parts(bs, riceParam, &zeroCountPart1, &riceParamPart1)
        || !drflac__read_rice_parts(bs, riceParam, &zeroCountPart2, &riceParamPart2)
        || !drflac__read_rice_parts(bs, riceParam, &zeroCountPart3, &riceParamPart3)) {
      return DRFLAC_FALSE;
    }

    riceParamPart0 |= (zeroCountPart0 << riceParam);
    riceParamPart1 |= (zeroCountPart1 << riceParam);
    riceParamPart2 |= (zeroCountPart2 << riceParam);
    riceParamPart3 |= (zeroCountPart3 << riceParam);

    riceParamPart0 = (riceParamPart0 >> 1) ^ t[riceParamPart0 & 0x01];
    riceParamPart1 = (riceParamPart1 >> 1) ^ t[riceParamPart1 & 0x01];
    riceParamPart2 = (riceParamPart2 >> 1) ^ t[riceParamPart2 & 0x01];
    riceParamPart3 = (riceParamPart3 >> 1) ^ t[riceParamPart3 & 0x01];

    if (bitsPerSample > 16) {
      pSamplesOut[0] = riceParamPart0 + drflac__calculate_prediction_64(order, shift, coefficients, pSamplesOut + 0);
      pSamplesOut[1] = riceParamPart1 + drflac__calculate_prediction_64(order, shift, coefficients, pSamplesOut + 1);
      pSamplesOut[2] = riceParamPart2 + drflac__calculate_prediction_64(order, shift, coefficients, pSamplesOut + 2);
      pSamplesOut[3] = riceParamPart3 + drflac__calculate_prediction_64(order, shift, coefficients, pSamplesOut + 3);
    } else {
      pSamplesOut[0] = riceParamPart0 + drflac__calculate_prediction_32(order, shift, coefficients, pSamplesOut + 0);
      pSamplesOut[1] = riceParamPart1 + drflac__calculate_prediction_32(order, shift, coefficients, pSamplesOut + 1);
      pSamplesOut[2] = riceParamPart2 + drflac__calculate_prediction_32(order, shift, coefficients, pSamplesOut + 2);
      pSamplesOut[3] = riceParamPart3 + drflac__calculate_prediction_32(order, shift, coefficients, pSamplesOut + 3);
    }

    i4 += 1;
    pSamplesOut += 4;
  }

  drflac_uint32 i = i4 << 2;
  while (i < count) {

    if (!drflac__read_rice_parts(bs, riceParam, &zeroCountPart0, &riceParamPart0)) {
      return DRFLAC_FALSE;
    }

    riceParamPart0 |= (zeroCountPart0 << riceParam);
    riceParamPart0 = (riceParamPart0 >> 1) ^ t[riceParamPart0 & 0x01];

    if (bitsPerSample > 16) {
      pSamplesOut[0] = riceParamPart0 + drflac__calculate_prediction_64(order, shift, coefficients, pSamplesOut + 0);
    } else {
      pSamplesOut[0] = riceParamPart0 + drflac__calculate_prediction_32(order, shift, coefficients, pSamplesOut + 0);
    }

    i += 1;
    pSamplesOut += 1;
  }

  return DRFLAC_TRUE;
}

static drflac_bool32 drflac__decode_samples_with_residual__rice__param_equals_zero(
    drflac_bs *bs, drflac_uint32 bitsPerSample, drflac_uint32 count, drflac_uint32 order, drflac_int32 shift,
    const drflac_int32 *coefficients, drflac_int32 *pSamplesOut)
{
  drflac_assert(bs != NULL);
  drflac_assert(count > 0);
  drflac_assert(pSamplesOut != NULL);

  static drflac_uint32 t[2] = {0x00000000, 0xFFFFFFFF};

  drflac_uint32 zeroCountPart0;
  drflac_uint32 zeroCountPart1;
  drflac_uint32 zeroCountPart2;
  drflac_uint32 zeroCountPart3;
  drflac_uint32 riceParamPart0;
  drflac_uint32 riceParamPart1;
  drflac_uint32 riceParamPart2;
  drflac_uint32 riceParamPart3;
  drflac_uint32 i4 = 0;
  drflac_uint32 count4 = count >> 2;
  while (i4 < count4) {

    if (!drflac__read_rice_parts__param_equals_zero(bs, &zeroCountPart0, &riceParamPart0)
        || !drflac__read_rice_parts__param_equals_zero(bs, &zeroCountPart1, &riceParamPart1)
        || !drflac__read_rice_parts__param_equals_zero(bs, &zeroCountPart2, &riceParamPart2)
        || !drflac__read_rice_parts__param_equals_zero(bs, &zeroCountPart3, &riceParamPart3)) {
      return DRFLAC_FALSE;
    }

    riceParamPart0 |= zeroCountPart0;
    riceParamPart1 |= zeroCountPart1;
    riceParamPart2 |= zeroCountPart2;
    riceParamPart3 |= zeroCountPart3;

    riceParamPart0 = (riceParamPart0 >> 1) ^ t[riceParamPart0 & 0x01];
    riceParamPart1 = (riceParamPart1 >> 1) ^ t[riceParamPart1 & 0x01];
    riceParamPart2 = (riceParamPart2 >> 1) ^ t[riceParamPart2 & 0x01];
    riceParamPart3 = (riceParamPart3 >> 1) ^ t[riceParamPart3 & 0x01];

    if (bitsPerSample > 16) {
      pSamplesOut[0] = riceParamPart0 + drflac__calculate_prediction_64(order, shift, coefficients, pSamplesOut + 0);
      pSamplesOut[1] = riceParamPart1 + drflac__calculate_prediction_64(order, shift, coefficients, pSamplesOut + 1);
      pSamplesOut[2] = riceParamPart2 + drflac__calculate_prediction_64(order, shift, coefficients, pSamplesOut + 2);
      pSamplesOut[3] = riceParamPart3 + drflac__calculate_prediction_64(order, shift, coefficients, pSamplesOut + 3);
    } else {
      pSamplesOut[0] = riceParamPart0 + drflac__calculate_prediction_32(order, shift, coefficients, pSamplesOut + 0);
      pSamplesOut[1] = riceParamPart1 + drflac__calculate_prediction_32(order, shift, coefficients, pSamplesOut + 1);
      pSamplesOut[2] = riceParamPart2 + drflac__calculate_prediction_32(order, shift, coefficients, pSamplesOut + 2);
      pSamplesOut[3] = riceParamPart3 + drflac__calculate_prediction_32(order, shift, coefficients, pSamplesOut + 3);
    }

    i4 += 1;
    pSamplesOut += 4;
  }

  drflac_uint32 i = i4 << 2;
  while (i < count) {

    if (!drflac__read_rice_parts__param_equals_zero(bs, &zeroCountPart0, &riceParamPart0)) {
      return DRFLAC_FALSE;
    }

    riceParamPart0 |= zeroCountPart0;
    riceParamPart0 = (riceParamPart0 >> 1) ^ t[riceParamPart0 & 0x01];

    if (bitsPerSample > 16) {
      pSamplesOut[0] = riceParamPart0 + drflac__calculate_prediction_64(order, shift, coefficients, pSamplesOut + 0);
    } else {
      pSamplesOut[0] = riceParamPart0 + drflac__calculate_prediction_32(order, shift, coefficients, pSamplesOut + 0);
    }

    i += 1;
    pSamplesOut += 1;
  }

  return DRFLAC_TRUE;
}

static drflac_bool32 drflac__decode_samples_with_residual__rice(drflac_bs *bs, drflac_uint32 bitsPerSample,
                                                                drflac_uint32 count, drflac_uint8 riceParam,
                                                                drflac_uint32 order, drflac_int32 shift,
                                                                const drflac_int32 *coefficients,
                                                                drflac_int32 *pSamplesOut)
{
#if 0
    return drflac__decode_samples_with_residual__rice__reference(
        bs, bitsPerSample, count, riceParam, order, shift, coefficients, pSamplesOut);
#else
  if (riceParam != 0) {
    return drflac__decode_samples_with_residual__rice__simple(bs, bitsPerSample, count, riceParam, order, shift,
                                                              coefficients, pSamplesOut);
  } else {
    return drflac__decode_samples_with_residual__rice__param_equals_zero(bs, bitsPerSample, count, order, shift,
                                                                         coefficients, pSamplesOut);
  }
#endif
}

static drflac_bool32 drflac__read_and_seek_residual__rice(drflac_bs *bs, drflac_uint32 count, drflac_uint8 riceParam)
{
  drflac_assert(bs != NULL);
  drflac_assert(count > 0);

  drflac_uint32 zeroCountPart;
  drflac_uint32 riceParamPart;

  if (riceParam != 0) {
    for (drflac_uint32 i = 0; i < count; ++i) {
      if (!drflac__read_rice_parts(bs, riceParam, &zeroCountPart, &riceParamPart)) {
        return DRFLAC_FALSE;
      }
    }
  } else {
    for (drflac_uint32 i = 0; i < count; ++i) {
      if (!drflac__read_rice_parts__param_equals_zero(bs, &zeroCountPart, &riceParamPart)) {
        return DRFLAC_FALSE;
      }
    }
  }

  return DRFLAC_TRUE;
}

static drflac_bool32 drflac__decode_samples_with_residual__unencoded(
    drflac_bs *bs, drflac_uint32 bitsPerSample, drflac_uint32 count, drflac_uint8 unencodedBitsPerSample,
    drflac_uint32 order, drflac_int32 shift, const drflac_int32 *coefficients, drflac_int32 *pSamplesOut)
{
  drflac_assert(bs != NULL);
  drflac_assert(count > 0);
  drflac_assert(unencodedBitsPerSample <= 31);
  drflac_assert(pSamplesOut != NULL);

  for (unsigned int i = 0; i < count; ++i) {
    if (unencodedBitsPerSample > 0) {
      if (!drflac__read_int32(bs, unencodedBitsPerSample, pSamplesOut + i)) {
        return DRFLAC_FALSE;
      }
    } else {
      pSamplesOut[i] = 0;
    }

    if (bitsPerSample > 16) {
      pSamplesOut[i] += drflac__calculate_prediction_64(order, shift, coefficients, pSamplesOut + i);
    } else {
      pSamplesOut[i] += drflac__calculate_prediction_32(order, shift, coefficients, pSamplesOut + i);
    }
  }

  return DRFLAC_TRUE;
}

static drflac_bool32 drflac__decode_samples_with_residual(drflac_bs *bs, drflac_uint32 bitsPerSample,
                                                          drflac_uint32 blockSize, drflac_uint32 order,
                                                          drflac_int32 shift, const drflac_int32 *coefficients,
                                                          drflac_int32 *pDecodedSamples)
{
  drflac_assert(bs != NULL);
  drflac_assert(blockSize != 0);
  drflac_assert(pDecodedSamples != NULL);

  drflac_uint8 residualMethod;
  if (!drflac__read_uint8(bs, 2, &residualMethod)) {
    return DRFLAC_FALSE;
  }

  if (residualMethod != DRFLAC_RESIDUAL_CODING_METHOD_PARTITIONED_RICE
      && residualMethod != DRFLAC_RESIDUAL_CODING_METHOD_PARTITIONED_RICE2) {
    return DRFLAC_FALSE;
  }

  pDecodedSamples += order;

  drflac_uint8 partitionOrder;
  if (!drflac__read_uint8(bs, 4, &partitionOrder)) {
    return DRFLAC_FALSE;
  }

  if (partitionOrder > 8) {
    return DRFLAC_FALSE;
  }

  if ((blockSize / (1 << partitionOrder)) <= order) {
    return DRFLAC_FALSE;
  }

  drflac_uint32 samplesInPartition = (blockSize / (1 << partitionOrder)) - order;
  drflac_uint32 partitionsRemaining = (1 << partitionOrder);
  for (;;) {
    drflac_uint8 riceParam = 0;
    if (residualMethod == DRFLAC_RESIDUAL_CODING_METHOD_PARTITIONED_RICE) {
      if (!drflac__read_uint8(bs, 4, &riceParam)) {
        return DRFLAC_FALSE;
      }
      if (riceParam == 15) {
        riceParam = 0xFF;
      }
    } else if (residualMethod == DRFLAC_RESIDUAL_CODING_METHOD_PARTITIONED_RICE2) {
      if (!drflac__read_uint8(bs, 5, &riceParam)) {
        return DRFLAC_FALSE;
      }
      if (riceParam == 31) {
        riceParam = 0xFF;
      }
    }

    if (riceParam != 0xFF) {
      if (!drflac__decode_samples_with_residual__rice(bs, bitsPerSample, samplesInPartition, riceParam, order, shift,
                                                      coefficients, pDecodedSamples)) {
        return DRFLAC_FALSE;
      }
    } else {
      unsigned char unencodedBitsPerSample = 0;
      if (!drflac__read_uint8(bs, 5, &unencodedBitsPerSample)) {
        return DRFLAC_FALSE;
      }

      if (!drflac__decode_samples_with_residual__unencoded(bs, bitsPerSample, samplesInPartition,
                                                           unencodedBitsPerSample, order, shift, coefficients,
                                                           pDecodedSamples)) {
        return DRFLAC_FALSE;
      }
    }

    pDecodedSamples += samplesInPartition;

    if (partitionsRemaining == 1) {
      break;
    }

    partitionsRemaining -= 1;

    if (partitionOrder != 0) {
      samplesInPartition = blockSize / (1 << partitionOrder);
    }
  }

  return DRFLAC_TRUE;
}

static drflac_bool32 drflac__read_and_seek_residual(drflac_bs *bs, drflac_uint32 blockSize, drflac_uint32 order)
{
  drflac_assert(bs != NULL);
  drflac_assert(blockSize != 0);

  drflac_uint8 residualMethod;
  if (!drflac__read_uint8(bs, 2, &residualMethod)) {
    return DRFLAC_FALSE;
  }

  if (residualMethod != DRFLAC_RESIDUAL_CODING_METHOD_PARTITIONED_RICE
      && residualMethod != DRFLAC_RESIDUAL_CODING_METHOD_PARTITIONED_RICE2) {
    return DRFLAC_FALSE;
  }

  drflac_uint8 partitionOrder;
  if (!drflac__read_uint8(bs, 4, &partitionOrder)) {
    return DRFLAC_FALSE;
  }

  if (partitionOrder > 8) {
    return DRFLAC_FALSE;
  }

  if ((blockSize / (1 << partitionOrder)) <= order) {
    return DRFLAC_FALSE;
  }

  drflac_uint32 samplesInPartition = (blockSize / (1 << partitionOrder)) - order;
  drflac_uint32 partitionsRemaining = (1 << partitionOrder);
  for (;;) {
    drflac_uint8 riceParam = 0;
    if (residualMethod == DRFLAC_RESIDUAL_CODING_METHOD_PARTITIONED_RICE) {
      if (!drflac__read_uint8(bs, 4, &riceParam)) {
        return DRFLAC_FALSE;
      }
      if (riceParam == 15) {
        riceParam = 0xFF;
      }
    } else if (residualMethod == DRFLAC_RESIDUAL_CODING_METHOD_PARTITIONED_RICE2) {
      if (!drflac__read_uint8(bs, 5, &riceParam)) {
        return DRFLAC_FALSE;
      }
      if (riceParam == 31) {
        riceParam = 0xFF;
      }
    }

    if (riceParam != 0xFF) {
      if (!drflac__read_and_seek_residual__rice(bs, samplesInPartition, riceParam)) {
        return DRFLAC_FALSE;
      }
    } else {
      unsigned char unencodedBitsPerSample = 0;
      if (!drflac__read_uint8(bs, 5, &unencodedBitsPerSample)) {
        return DRFLAC_FALSE;
      }

      if (!drflac__seek_bits(bs, unencodedBitsPerSample * samplesInPartition)) {
        return DRFLAC_FALSE;
      }
    }

    if (partitionsRemaining == 1) {
      break;
    }

    partitionsRemaining -= 1;
    samplesInPartition = blockSize / (1 << partitionOrder);
  }

  return DRFLAC_TRUE;
}

static drflac_bool32 drflac__decode_samples__constant(drflac_bs *bs, drflac_uint32 blockSize,
                                                      drflac_uint32 bitsPerSample, drflac_int32 *pDecodedSamples)
{

  drflac_int32 sample;
  if (!drflac__read_int32(bs, bitsPerSample, &sample)) {
    return DRFLAC_FALSE;
  }

  for (drflac_uint32 i = 0; i < blockSize; ++i) {
    pDecodedSamples[i] = sample;
  }

  return DRFLAC_TRUE;
}

static drflac_bool32 drflac__decode_samples__verbatim(drflac_bs *bs, drflac_uint32 blockSize,
                                                      drflac_uint32 bitsPerSample, drflac_int32 *pDecodedSamples)
{
  for (drflac_uint32 i = 0; i < blockSize; ++i) {
    drflac_int32 sample;
    if (!drflac__read_int32(bs, bitsPerSample, &sample)) {
      return DRFLAC_FALSE;
    }

    pDecodedSamples[i] = sample;
  }

  return DRFLAC_TRUE;
}

static drflac_bool32 drflac__decode_samples__fixed(drflac_bs *bs, drflac_uint32 blockSize, drflac_uint32 bitsPerSample,
                                                   drflac_uint8 lpcOrder, drflac_int32 *pDecodedSamples)
{
  drflac_int32 lpcCoefficientsTable[5][4] = {{0, 0, 0, 0}, {1, 0, 0, 0}, {2, -1, 0, 0}, {3, -3, 1, 0}, {4, -6, 4, -1}};

  for (drflac_uint32 i = 0; i < lpcOrder; ++i) {
    drflac_int32 sample;
    if (!drflac__read_int32(bs, bitsPerSample, &sample)) {
      return DRFLAC_FALSE;
    }

    pDecodedSamples[i] = sample;
  }

  if (!drflac__decode_samples_with_residual(bs, bitsPerSample, blockSize, lpcOrder, 0, lpcCoefficientsTable[lpcOrder],
                                            pDecodedSamples)) {
    return DRFLAC_FALSE;
  }

  return DRFLAC_TRUE;
}

static drflac_bool32 drflac__decode_samples__lpc(drflac_bs *bs, drflac_uint32 blockSize, drflac_uint32 bitsPerSample,
                                                 drflac_uint8 lpcOrder, drflac_int32 *pDecodedSamples)
{
  drflac_uint8 i;

  for (i = 0; i < lpcOrder; ++i) {
    drflac_int32 sample;
    if (!drflac__read_int32(bs, bitsPerSample, &sample)) {
      return DRFLAC_FALSE;
    }

    pDecodedSamples[i] = sample;
  }

  drflac_uint8 lpcPrecision;
  if (!drflac__read_uint8(bs, 4, &lpcPrecision)) {
    return DRFLAC_FALSE;
  }
  if (lpcPrecision == 15) {
    return DRFLAC_FALSE;
  }
  lpcPrecision += 1;

  drflac_int8 lpcShift;
  if (!drflac__read_int8(bs, 5, &lpcShift)) {
    return DRFLAC_FALSE;
  }

  drflac_int32 coefficients[32];
  for (i = 0; i < lpcOrder; ++i) {
    if (!drflac__read_int32(bs, lpcPrecision, coefficients + i)) {
      return DRFLAC_FALSE;
    }
  }

  if (!drflac__decode_samples_with_residual(bs, bitsPerSample, blockSize, lpcOrder, lpcShift, coefficients,
                                            pDecodedSamples)) {
    return DRFLAC_FALSE;
  }

  return DRFLAC_TRUE;
}

static drflac_bool32 drflac__read_next_frame_header(drflac_bs *bs, drflac_uint8 streaminfoBitsPerSample,
                                                    drflac_frame_header *header)
{
  drflac_assert(bs != NULL);
  drflac_assert(header != NULL);

  const drflac_uint32 sampleRateTable[12] = {0,     88200, 176400, 192000, 8000,  16000,
                                             22050, 24000, 32000,  44100,  48000, 96000};
  const drflac_uint8 bitsPerSampleTable[8] = {0, 8, 12, (drflac_uint8)-1, 16, 20, 24, (drflac_uint8)-1};

  for (;;) {
    if (!drflac__find_and_seek_to_next_sync_code(bs)) {
      return DRFLAC_FALSE;
    }

    drflac_uint8 crc8 = 0xCE;

    drflac_uint8 reserved = 0;
    if (!drflac__read_uint8(bs, 1, &reserved)) {
      return DRFLAC_FALSE;
    }
    if (reserved == 1) {
      continue;
    }
    crc8 = drflac_crc8(crc8, reserved, 1);

    drflac_uint8 blockingStrategy = 0;
    if (!drflac__read_uint8(bs, 1, &blockingStrategy)) {
      return DRFLAC_FALSE;
    }
    crc8 = drflac_crc8(crc8, blockingStrategy, 1);

    drflac_uint8 blockSize = 0;
    if (!drflac__read_uint8(bs, 4, &blockSize)) {
      return DRFLAC_FALSE;
    }
    if (blockSize == 0) {
      continue;
    }
    crc8 = drflac_crc8(crc8, blockSize, 4);

    drflac_uint8 sampleRate = 0;
    if (!drflac__read_uint8(bs, 4, &sampleRate)) {
      return DRFLAC_FALSE;
    }
    crc8 = drflac_crc8(crc8, sampleRate, 4);

    drflac_uint8 channelAssignment = 0;
    if (!drflac__read_uint8(bs, 4, &channelAssignment)) {
      return DRFLAC_FALSE;
    }
    if (channelAssignment > 10) {
      continue;
    }
    crc8 = drflac_crc8(crc8, channelAssignment, 4);

    drflac_uint8 bitsPerSample = 0;
    if (!drflac__read_uint8(bs, 3, &bitsPerSample)) {
      return DRFLAC_FALSE;
    }
    if (bitsPerSample == 3 || bitsPerSample == 7) {
      continue;
    }
    crc8 = drflac_crc8(crc8, bitsPerSample, 3);

    if (!drflac__read_uint8(bs, 1, &reserved)) {
      return DRFLAC_FALSE;
    }
    if (reserved == 1) {
      continue;
    }
    crc8 = drflac_crc8(crc8, reserved, 1);

    drflac_bool32 isVariableBlockSize = blockingStrategy == 1;
    if (isVariableBlockSize) {
      drflac_uint64 sampleNumber;
      drflac_result result = drflac__read_utf8_coded_number(bs, &sampleNumber, &crc8);
      if (result != DRFLAC_SUCCESS) {
        if (result == DRFLAC_END_OF_STREAM) {
          return DRFLAC_FALSE;
        } else {
          continue;
        }
      }
      header->frameNumber = 0;
      header->sampleNumber = sampleNumber;
    } else {
      drflac_uint64 frameNumber = 0;
      drflac_result result = drflac__read_utf8_coded_number(bs, &frameNumber, &crc8);
      if (result != DRFLAC_SUCCESS) {
        if (result == DRFLAC_END_OF_STREAM) {
          return DRFLAC_FALSE;
        } else {
          continue;
        }
      }
      header->frameNumber = (drflac_uint32)frameNumber;
      header->sampleNumber = 0;
    }

    if (blockSize == 1) {
      header->blockSize = 192;
    } else if (blockSize >= 2 && blockSize <= 5) {
      header->blockSize = 576 * (1 << (blockSize - 2));
    } else if (blockSize == 6) {
      if (!drflac__read_uint16(bs, 8, &header->blockSize)) {
        return DRFLAC_FALSE;
      }
      crc8 = drflac_crc8(crc8, header->blockSize, 8);
      header->blockSize += 1;
    } else if (blockSize == 7) {
      if (!drflac__read_uint16(bs, 16, &header->blockSize)) {
        return DRFLAC_FALSE;
      }
      crc8 = drflac_crc8(crc8, header->blockSize, 16);
      header->blockSize += 1;
    } else {
      header->blockSize = 256 * (1 << (blockSize - 8));
    }

    if (sampleRate <= 11) {
      header->sampleRate = sampleRateTable[sampleRate];
    } else if (sampleRate == 12) {
      if (!drflac__read_uint32(bs, 8, &header->sampleRate)) {
        return DRFLAC_FALSE;
      }
      crc8 = drflac_crc8(crc8, header->sampleRate, 8);
      header->sampleRate *= 1000;
    } else if (sampleRate == 13) {
      if (!drflac__read_uint32(bs, 16, &header->sampleRate)) {
        return DRFLAC_FALSE;
      }
      crc8 = drflac_crc8(crc8, header->sampleRate, 16);
    } else if (sampleRate == 14) {
      if (!drflac__read_uint32(bs, 16, &header->sampleRate)) {
        return DRFLAC_FALSE;
      }
      crc8 = drflac_crc8(crc8, header->sampleRate, 16);
      header->sampleRate *= 10;
    } else {
      continue;
    }

    header->channelAssignment = channelAssignment;

    header->bitsPerSample = bitsPerSampleTable[bitsPerSample];
    if (header->bitsPerSample == 0) {
      header->bitsPerSample = streaminfoBitsPerSample;
    }

    if (!drflac__read_uint8(bs, 8, &header->crc8)) {
      return DRFLAC_FALSE;
    }

#ifndef DR_FLAC_NO_CRC
    if (header->crc8 != crc8) {
      continue;
    }
#endif
    return DRFLAC_TRUE;
  }
}

static drflac_bool32 drflac__read_subframe_header(drflac_bs *bs, drflac_subframe *pSubframe)
{
  drflac_uint8 header;
  if (!drflac__read_uint8(bs, 8, &header)) {
    return DRFLAC_FALSE;
  }

  if ((header & 0x80) != 0) {
    return DRFLAC_FALSE;
  }

  int type = (header & 0x7E) >> 1;
  if (type == 0) {
    pSubframe->subframeType = DRFLAC_SUBFRAME_CONSTANT;
  } else if (type == 1) {
    pSubframe->subframeType = DRFLAC_SUBFRAME_VERBATIM;
  } else {
    if ((type & 0x20) != 0) {
      pSubframe->subframeType = DRFLAC_SUBFRAME_LPC;
      pSubframe->lpcOrder = (type & 0x1F) + 1;
    } else if ((type & 0x08) != 0) {
      pSubframe->subframeType = DRFLAC_SUBFRAME_FIXED;
      pSubframe->lpcOrder = (type & 0x07);
      if (pSubframe->lpcOrder > 4) {
        pSubframe->subframeType = DRFLAC_SUBFRAME_RESERVED;
        pSubframe->lpcOrder = 0;
      }
    } else {
      pSubframe->subframeType = DRFLAC_SUBFRAME_RESERVED;
    }
  }

  if (pSubframe->subframeType == DRFLAC_SUBFRAME_RESERVED) {
    return DRFLAC_FALSE;
  }

  pSubframe->wastedBitsPerSample = 0;
  if ((header & 0x01) == 1) {
    unsigned int wastedBitsPerSample;
    if (!drflac__seek_past_next_set_bit(bs, &wastedBitsPerSample)) {
      return DRFLAC_FALSE;
    }
    pSubframe->wastedBitsPerSample = (unsigned char)wastedBitsPerSample + 1;
  }

  return DRFLAC_TRUE;
}

static drflac_bool32 drflac__decode_subframe(drflac_bs *bs, drflac_frame *frame, int subframeIndex,
                                             drflac_int32 *pDecodedSamplesOut)
{
  drflac_assert(bs != NULL);
  drflac_assert(frame != NULL);

  drflac_subframe *pSubframe = frame->subframes + subframeIndex;
  if (!drflac__read_subframe_header(bs, pSubframe)) {
    return DRFLAC_FALSE;
  }

  pSubframe->bitsPerSample = frame->header.bitsPerSample;
  if ((frame->header.channelAssignment == DRFLAC_CHANNEL_ASSIGNMENT_LEFT_SIDE
       || frame->header.channelAssignment == DRFLAC_CHANNEL_ASSIGNMENT_MID_SIDE)
      && subframeIndex == 1) {
    pSubframe->bitsPerSample += 1;
  } else if (frame->header.channelAssignment == DRFLAC_CHANNEL_ASSIGNMENT_RIGHT_SIDE && subframeIndex == 0) {
    pSubframe->bitsPerSample += 1;
  }

  if (pSubframe->wastedBitsPerSample >= pSubframe->bitsPerSample) {
    return DRFLAC_FALSE;
  }
  pSubframe->bitsPerSample -= pSubframe->wastedBitsPerSample;
  pSubframe->pDecodedSamples = pDecodedSamplesOut;

  switch (pSubframe->subframeType) {
  case DRFLAC_SUBFRAME_CONSTANT: {
    drflac__decode_samples__constant(bs, frame->header.blockSize, pSubframe->bitsPerSample, pSubframe->pDecodedSamples);
  } break;

  case DRFLAC_SUBFRAME_VERBATIM: {
    drflac__decode_samples__verbatim(bs, frame->header.blockSize, pSubframe->bitsPerSample, pSubframe->pDecodedSamples);
  } break;

  case DRFLAC_SUBFRAME_FIXED: {
    drflac__decode_samples__fixed(bs, frame->header.blockSize, pSubframe->bitsPerSample, pSubframe->lpcOrder,
                                  pSubframe->pDecodedSamples);
  } break;

  case DRFLAC_SUBFRAME_LPC: {
    drflac__decode_samples__lpc(bs, frame->header.blockSize, pSubframe->bitsPerSample, pSubframe->lpcOrder,
                                pSubframe->pDecodedSamples);
  } break;

  default:
    return DRFLAC_FALSE;
  }

  return DRFLAC_TRUE;
}

static drflac_bool32 drflac__seek_subframe(drflac_bs *bs, drflac_frame *frame, int subframeIndex)
{
  drflac_assert(bs != NULL);
  drflac_assert(frame != NULL);

  drflac_subframe *pSubframe = frame->subframes + subframeIndex;
  if (!drflac__read_subframe_header(bs, pSubframe)) {
    return DRFLAC_FALSE;
  }

  pSubframe->bitsPerSample = frame->header.bitsPerSample;
  if ((frame->header.channelAssignment == DRFLAC_CHANNEL_ASSIGNMENT_LEFT_SIDE
       || frame->header.channelAssignment == DRFLAC_CHANNEL_ASSIGNMENT_MID_SIDE)
      && subframeIndex == 1) {
    pSubframe->bitsPerSample += 1;
  } else if (frame->header.channelAssignment == DRFLAC_CHANNEL_ASSIGNMENT_RIGHT_SIDE && subframeIndex == 0) {
    pSubframe->bitsPerSample += 1;
  }

  if (pSubframe->wastedBitsPerSample >= pSubframe->bitsPerSample) {
    return DRFLAC_FALSE;
  }
  pSubframe->bitsPerSample -= pSubframe->wastedBitsPerSample;
  pSubframe->pDecodedSamples = NULL;

  switch (pSubframe->subframeType) {
  case DRFLAC_SUBFRAME_CONSTANT: {
    if (!drflac__seek_bits(bs, pSubframe->bitsPerSample)) {
      return DRFLAC_FALSE;
    }
  } break;

  case DRFLAC_SUBFRAME_VERBATIM: {
    unsigned int bitsToSeek = frame->header.blockSize * pSubframe->bitsPerSample;
    if (!drflac__seek_bits(bs, bitsToSeek)) {
      return DRFLAC_FALSE;
    }
  } break;

  case DRFLAC_SUBFRAME_FIXED: {
    unsigned int bitsToSeek = pSubframe->lpcOrder * pSubframe->bitsPerSample;
    if (!drflac__seek_bits(bs, bitsToSeek)) {
      return DRFLAC_FALSE;
    }

    if (!drflac__read_and_seek_residual(bs, frame->header.blockSize, pSubframe->lpcOrder)) {
      return DRFLAC_FALSE;
    }
  } break;

  case DRFLAC_SUBFRAME_LPC: {
    unsigned int bitsToSeek = pSubframe->lpcOrder * pSubframe->bitsPerSample;
    if (!drflac__seek_bits(bs, bitsToSeek)) {
      return DRFLAC_FALSE;
    }

    unsigned char lpcPrecision;
    if (!drflac__read_uint8(bs, 4, &lpcPrecision)) {
      return DRFLAC_FALSE;
    }
    if (lpcPrecision == 15) {
      return DRFLAC_FALSE;
    }
    lpcPrecision += 1;

    bitsToSeek = (pSubframe->lpcOrder * lpcPrecision) + 5;
    if (!drflac__seek_bits(bs, bitsToSeek)) {
      return DRFLAC_FALSE;
    }

    if (!drflac__read_and_seek_residual(bs, frame->header.blockSize, pSubframe->lpcOrder)) {
      return DRFLAC_FALSE;
    }
  } break;

  default:
    return DRFLAC_FALSE;
  }

  return DRFLAC_TRUE;
}

static DRFLAC_INLINE drflac_uint8 drflac__get_channel_count_from_channel_assignment(drflac_int8 channelAssignment)
{
  drflac_assert(channelAssignment <= 10);

  drflac_uint8 lookup[] = {1, 2, 3, 4, 5, 6, 7, 8, 2, 2, 2};
  return lookup[channelAssignment];
}

static drflac_result drflac__decode_frame(drflac *pFlac)
{

  drflac_zero_memory(pFlac->currentFrame.subframes, sizeof(pFlac->currentFrame.subframes));

  if (pFlac->currentFrame.header.blockSize > pFlac->maxBlockSize) {
    return DRFLAC_ERROR;
  }

  int channelCount = drflac__get_channel_count_from_channel_assignment(pFlac->currentFrame.header.channelAssignment);
  if (channelCount != (int)pFlac->channels) {
    return DRFLAC_ERROR;
  }

  for (int i = 0; i < channelCount; ++i) {
    if (!drflac__decode_subframe(&pFlac->bs, &pFlac->currentFrame, i,
                                 pFlac->pDecodedSamples + (pFlac->currentFrame.header.blockSize * i))) {
      return DRFLAC_ERROR;
    }
  }

  drflac_uint8 paddingSizeInBits = DRFLAC_CACHE_L1_BITS_REMAINING(&pFlac->bs) & 7;
  if (paddingSizeInBits > 0) {
    drflac_uint8 padding = 0;
    if (!drflac__read_uint8(&pFlac->bs, paddingSizeInBits, &padding)) {
      return DRFLAC_END_OF_STREAM;
    }
  }

#ifndef DR_FLAC_NO_CRC
  drflac_uint16 actualCRC16 = drflac__flush_crc16(&pFlac->bs);
#endif
  drflac_uint16 desiredCRC16;
  if (!drflac__read_uint16(&pFlac->bs, 16, &desiredCRC16)) {
    return DRFLAC_END_OF_STREAM;
  }

#ifndef DR_FLAC_NO_CRC
  if (actualCRC16 != desiredCRC16) {
    return DRFLAC_CRC_MISMATCH;
  }
#endif

  pFlac->currentFrame.samplesRemaining = pFlac->currentFrame.header.blockSize * channelCount;

  return DRFLAC_SUCCESS;
}

static drflac_result drflac__seek_frame(drflac *pFlac)
{
  int channelCount = drflac__get_channel_count_from_channel_assignment(pFlac->currentFrame.header.channelAssignment);
  for (int i = 0; i < channelCount; ++i) {
    if (!drflac__seek_subframe(&pFlac->bs, &pFlac->currentFrame, i)) {
      return DRFLAC_ERROR;
    }
  }

  if (!drflac__seek_bits(&pFlac->bs, DRFLAC_CACHE_L1_BITS_REMAINING(&pFlac->bs) & 7)) {
    return DRFLAC_ERROR;
  }

#ifndef DR_FLAC_NO_CRC
  drflac_uint16 actualCRC16 = drflac__flush_crc16(&pFlac->bs);
#endif
  drflac_uint16 desiredCRC16;
  if (!drflac__read_uint16(&pFlac->bs, 16, &desiredCRC16)) {
    return DRFLAC_END_OF_STREAM;
  }

#ifndef DR_FLAC_NO_CRC
  if (actualCRC16 != desiredCRC16) {
    return DRFLAC_CRC_MISMATCH;
  }
#endif

  return DRFLAC_SUCCESS;
}

static drflac_bool32 drflac__read_and_decode_next_frame(drflac *pFlac)
{
  drflac_assert(pFlac != NULL);

  for (;;) {
    if (!drflac__read_next_frame_header(&pFlac->bs, pFlac->bitsPerSample, &pFlac->currentFrame.header)) {
      return DRFLAC_FALSE;
    }

    drflac_result result = drflac__decode_frame(pFlac);
    if (result != DRFLAC_SUCCESS) {
      if (result == DRFLAC_CRC_MISMATCH) {
        continue;
      } else {
        return DRFLAC_FALSE;
      }
    }

    return DRFLAC_TRUE;
  }
}

static void drflac__get_current_frame_sample_range(drflac *pFlac, drflac_uint64 *pFirstSampleInFrameOut,
                                                   drflac_uint64 *pLastSampleInFrameOut)
{
  drflac_assert(pFlac != NULL);

  unsigned int channelCount =
      drflac__get_channel_count_from_channel_assignment(pFlac->currentFrame.header.channelAssignment);

  drflac_uint64 firstSampleInFrame = pFlac->currentFrame.header.sampleNumber;
  if (firstSampleInFrame == 0) {
    firstSampleInFrame = pFlac->currentFrame.header.frameNumber * pFlac->maxBlockSize * channelCount;
  }

  drflac_uint64 lastSampleInFrame = firstSampleInFrame + (pFlac->currentFrame.header.blockSize * channelCount);
  if (lastSampleInFrame > 0) {
    lastSampleInFrame -= 1;
  }

  if (pFirstSampleInFrameOut)
    *pFirstSampleInFrameOut = firstSampleInFrame;
  if (pLastSampleInFrameOut)
    *pLastSampleInFrameOut = lastSampleInFrame;
}

static drflac_bool32 drflac__seek_to_first_frame(drflac *pFlac)
{
  drflac_assert(pFlac != NULL);

  drflac_bool32 result = drflac__seek_to_byte(&pFlac->bs, pFlac->firstFramePos);

  drflac_zero_memory(&pFlac->currentFrame, sizeof(pFlac->currentFrame));
  pFlac->currentSample = 0;

  return result;
}

static DRFLAC_INLINE drflac_result drflac__seek_to_next_frame(drflac *pFlac)
{

  drflac_assert(pFlac != NULL);
  return drflac__seek_frame(pFlac);
}

static drflac_bool32 drflac__seek_to_sample__brute_force(drflac *pFlac, drflac_uint64 sampleIndex)
{
  drflac_assert(pFlac != NULL);

  drflac_bool32 isMidFrame = DRFLAC_FALSE;

  drflac_uint64 runningSampleCount;
  if (sampleIndex >= pFlac->currentSample) {

    runningSampleCount = pFlac->currentSample;

    if (pFlac->currentSample == 0 && pFlac->currentFrame.samplesRemaining == 0) {
      if (!drflac__read_next_frame_header(&pFlac->bs, pFlac->bitsPerSample, &pFlac->currentFrame.header)) {
        return DRFLAC_FALSE;
      }
    } else {
      isMidFrame = DRFLAC_TRUE;
    }
  } else {

    runningSampleCount = 0;

    if (!drflac__seek_to_first_frame(pFlac)) {
      return DRFLAC_FALSE;
    }

    if (!drflac__read_next_frame_header(&pFlac->bs, pFlac->bitsPerSample, &pFlac->currentFrame.header)) {
      return DRFLAC_FALSE;
    }
  }

  for (;;) {
    drflac_uint64 firstSampleInFrame = 0;
    drflac_uint64 lastSampleInFrame = 0;
    drflac__get_current_frame_sample_range(pFlac, &firstSampleInFrame, &lastSampleInFrame);

    drflac_uint64 sampleCountInThisFrame = (lastSampleInFrame - firstSampleInFrame) + 1;
    if (sampleIndex < (runningSampleCount + sampleCountInThisFrame)) {

      drflac_uint64 samplesToDecode = sampleIndex - runningSampleCount;

      if (!isMidFrame) {
        drflac_result result = drflac__decode_frame(pFlac);
        if (result == DRFLAC_SUCCESS) {

          return drflac_read_s32(pFlac, samplesToDecode, NULL) == samplesToDecode;
        } else {
          if (result == DRFLAC_CRC_MISMATCH) {
            goto next_iteration;
          } else {
            return DRFLAC_FALSE;
          }
        }
      } else {

        return drflac_read_s32(pFlac, samplesToDecode, NULL) == samplesToDecode;
      }
    } else {

      if (!isMidFrame) {
        drflac_result result = drflac__seek_to_next_frame(pFlac);
        if (result == DRFLAC_SUCCESS) {
          runningSampleCount += sampleCountInThisFrame;
        } else {
          if (result == DRFLAC_CRC_MISMATCH) {
            goto next_iteration;
          } else {
            return DRFLAC_FALSE;
          }
        }
      } else {

        runningSampleCount += pFlac->currentFrame.samplesRemaining;
        pFlac->currentFrame.samplesRemaining = 0;
        isMidFrame = DRFLAC_FALSE;
      }
    }

  next_iteration:

    if (!drflac__read_next_frame_header(&pFlac->bs, pFlac->bitsPerSample, &pFlac->currentFrame.header)) {
      return DRFLAC_FALSE;
    }
  }
}

static drflac_bool32 drflac__seek_to_sample__seek_table(drflac *pFlac, drflac_uint64 sampleIndex)
{
  drflac_assert(pFlac != NULL);

  if (pFlac->pSeekpoints == NULL || pFlac->seekpointCount == 0) {
    return DRFLAC_FALSE;
  }

  drflac_uint32 iClosestSeekpoint = 0;
  for (drflac_uint32 iSeekpoint = 0; iSeekpoint < pFlac->seekpointCount; ++iSeekpoint) {
    if (pFlac->pSeekpoints[iSeekpoint].firstSample * pFlac->channels >= sampleIndex) {
      break;
    }

    iClosestSeekpoint = iSeekpoint;
  }

  drflac_bool32 isMidFrame = DRFLAC_FALSE;

  drflac_uint64 runningSampleCount;
  if ((sampleIndex >= pFlac->currentSample)
      && (pFlac->pSeekpoints[iClosestSeekpoint].firstSample * pFlac->channels <= pFlac->currentSample)) {

    runningSampleCount = pFlac->currentSample;

    if (pFlac->currentSample == 0 && pFlac->currentFrame.samplesRemaining == 0) {
      if (!drflac__read_next_frame_header(&pFlac->bs, pFlac->bitsPerSample, &pFlac->currentFrame.header)) {
        return DRFLAC_FALSE;
      }
    } else {
      isMidFrame = DRFLAC_TRUE;
    }
  } else {

    runningSampleCount = pFlac->pSeekpoints[iClosestSeekpoint].firstSample * pFlac->channels;

    if (!drflac__seek_to_byte(&pFlac->bs, pFlac->firstFramePos + pFlac->pSeekpoints[iClosestSeekpoint].frameOffset)) {
      return DRFLAC_FALSE;
    }

    if (!drflac__read_next_frame_header(&pFlac->bs, pFlac->bitsPerSample, &pFlac->currentFrame.header)) {
      return DRFLAC_FALSE;
    }
  }

  for (;;) {
    drflac_uint64 firstSampleInFrame = 0;
    drflac_uint64 lastSampleInFrame = 0;
    drflac__get_current_frame_sample_range(pFlac, &firstSampleInFrame, &lastSampleInFrame);

    drflac_uint64 sampleCountInThisFrame = (lastSampleInFrame - firstSampleInFrame) + 1;
    if (sampleIndex < (runningSampleCount + sampleCountInThisFrame)) {

      drflac_uint64 samplesToDecode = sampleIndex - runningSampleCount;

      if (!isMidFrame) {
        drflac_result result = drflac__decode_frame(pFlac);
        if (result == DRFLAC_SUCCESS) {

          return drflac_read_s32(pFlac, samplesToDecode, NULL) == samplesToDecode;
        } else {
          if (result == DRFLAC_CRC_MISMATCH) {
            goto next_iteration;
          } else {
            return DRFLAC_FALSE;
          }
        }
      } else {

        return drflac_read_s32(pFlac, samplesToDecode, NULL) == samplesToDecode;
      }
    } else {

      if (!isMidFrame) {
        drflac_result result = drflac__seek_to_next_frame(pFlac);
        if (result == DRFLAC_SUCCESS) {
          runningSampleCount += sampleCountInThisFrame;
        } else {
          if (result == DRFLAC_CRC_MISMATCH) {
            goto next_iteration;
          } else {
            return DRFLAC_FALSE;
          }
        }
      } else {

        runningSampleCount += pFlac->currentFrame.samplesRemaining;
        pFlac->currentFrame.samplesRemaining = 0;
        isMidFrame = DRFLAC_FALSE;
      }
    }

  next_iteration:

    if (!drflac__read_next_frame_header(&pFlac->bs, pFlac->bitsPerSample, &pFlac->currentFrame.header)) {
      return DRFLAC_FALSE;
    }
  }
}

#ifndef DR_FLAC_NO_OGG
typedef struct
{
  drflac_uint8 capturePattern[4];
  drflac_uint8 structureVersion;
  drflac_uint8 headerType;
  drflac_uint64 granulePosition;
  drflac_uint32 serialNumber;
  drflac_uint32 sequenceNumber;
  drflac_uint32 checksum;
  drflac_uint8 segmentCount;
  drflac_uint8 segmentTable[255];
} drflac_ogg_page_header;
#endif

typedef struct
{
  drflac_read_proc onRead;
  drflac_seek_proc onSeek;
  drflac_meta_proc onMeta;
  drflac_container container;
  void *pUserData;
  void *pUserDataMD;
  drflac_uint32 sampleRate;
  drflac_uint8 channels;
  drflac_uint8 bitsPerSample;
  drflac_uint64 totalSampleCount;
  drflac_uint16 maxBlockSize;
  drflac_uint64 runningFilePos;
  drflac_bool32 hasStreamInfoBlock;
  drflac_bool32 hasMetadataBlocks;
  drflac_bs bs;
  drflac_frame_header firstFrameHeader;

#ifndef DR_FLAC_NO_OGG
  drflac_uint32 oggSerial;
  drflac_uint64 oggFirstBytePos;
  drflac_ogg_page_header oggBosHeader;
#endif
} drflac_init_info;

static DRFLAC_INLINE void drflac__decode_block_header(drflac_uint32 blockHeader, drflac_uint8 *isLastBlock,
                                                      drflac_uint8 *blockType, drflac_uint32 *blockSize)
{
  blockHeader = drflac__be2host_32(blockHeader);
  *isLastBlock = (blockHeader & (0x01 << 31)) >> 31;
  *blockType = (blockHeader & (0x7F << 24)) >> 24;
  *blockSize = (blockHeader & 0xFFFFFF);
}

static DRFLAC_INLINE drflac_bool32 drflac__read_and_decode_block_header(drflac_read_proc onRead, void *pUserData,
                                                                        drflac_uint8 *isLastBlock,
                                                                        drflac_uint8 *blockType,
                                                                        drflac_uint32 *blockSize)
{
  drflac_uint32 blockHeader;
  if (onRead(pUserData, &blockHeader, 4) != 4) {
    return DRFLAC_FALSE;
  }

  drflac__decode_block_header(blockHeader, isLastBlock, blockType, blockSize);
  return DRFLAC_TRUE;
}

drflac_bool32 drflac__read_streaminfo(drflac_read_proc onRead, void *pUserData, drflac_streaminfo *pStreamInfo)
{

  drflac_uint32 blockSizes;
  if (onRead(pUserData, &blockSizes, 4) != 4) {
    return DRFLAC_FALSE;
  }

  drflac_uint64 frameSizes = 0;
  if (onRead(pUserData, &frameSizes, 6) != 6) {
    return DRFLAC_FALSE;
  }

  drflac_uint64 importantProps;
  if (onRead(pUserData, &importantProps, 8) != 8) {
    return DRFLAC_FALSE;
  }

  drflac_uint8 md5[16];
  if (onRead(pUserData, md5, sizeof(md5)) != sizeof(md5)) {
    return DRFLAC_FALSE;
  }

  blockSizes = drflac__be2host_32(blockSizes);
  frameSizes = drflac__be2host_64(frameSizes);
  importantProps = drflac__be2host_64(importantProps);

  pStreamInfo->minBlockSize = (blockSizes & 0xFFFF0000) >> 16;
  pStreamInfo->maxBlockSize = blockSizes & 0x0000FFFF;
  pStreamInfo->minFrameSize = (drflac_uint32)((frameSizes & (drflac_uint64)0xFFFFFF0000000000) >> 40);
  pStreamInfo->maxFrameSize = (drflac_uint32)((frameSizes & (drflac_uint64)0x000000FFFFFF0000) >> 16);
  pStreamInfo->sampleRate = (drflac_uint32)((importantProps & (drflac_uint64)0xFFFFF00000000000) >> 44);
  pStreamInfo->channels = (drflac_uint8)((importantProps & (drflac_uint64)0x00000E0000000000) >> 41) + 1;
  pStreamInfo->bitsPerSample = (drflac_uint8)((importantProps & (drflac_uint64)0x000001F000000000) >> 36) + 1;
  pStreamInfo->totalSampleCount = (importantProps & (drflac_uint64)0x0000000FFFFFFFFF) * pStreamInfo->channels;
  drflac_copy_memory(pStreamInfo->md5, md5, sizeof(md5));

  return DRFLAC_TRUE;
}

drflac_bool32 drflac__read_and_decode_metadata(drflac_read_proc onRead, drflac_seek_proc onSeek,
                                               drflac_meta_proc onMeta, void *pUserData, void *pUserDataMD,
                                               drflac_uint64 *pFirstFramePos, drflac_uint64 *pSeektablePos,
                                               drflac_uint32 *pSeektableSize)
{

  drflac_uint64 runningFilePos = 42;
  drflac_uint64 seektablePos = 0;
  drflac_uint32 seektableSize = 0;

  for (;;) {
    drflac_uint8 isLastBlock = 0;
    drflac_uint8 blockType;
    drflac_uint32 blockSize;
    if (!drflac__read_and_decode_block_header(onRead, pUserData, &isLastBlock, &blockType, &blockSize)) {
      return DRFLAC_FALSE;
    }
    runningFilePos += 4;

    drflac_metadata metadata;
    metadata.type = blockType;
    metadata.pRawData = NULL;
    metadata.rawDataSize = 0;

    switch (blockType) {
    case DRFLAC_METADATA_BLOCK_TYPE_APPLICATION: {
      if (blockSize < 4) {
        return DRFLAC_FALSE;
      }

      if (onMeta) {
        void *pRawData = DRFLAC_MALLOC(blockSize);
        if (pRawData == NULL) {
          return DRFLAC_FALSE;
        }

        if (onRead(pUserData, pRawData, blockSize) != blockSize) {
          DRFLAC_FREE(pRawData);
          return DRFLAC_FALSE;
        }

        metadata.pRawData = pRawData;
        metadata.rawDataSize = blockSize;
        metadata.data.application.id = drflac__be2host_32(*(drflac_uint32 *)pRawData);
        metadata.data.application.pData = (const void *)((drflac_uint8 *)pRawData + sizeof(drflac_uint32));
        metadata.data.application.dataSize = blockSize - sizeof(drflac_uint32);
        onMeta(pUserDataMD, &metadata);

        DRFLAC_FREE(pRawData);
      }
    } break;

    case DRFLAC_METADATA_BLOCK_TYPE_SEEKTABLE: {
      seektablePos = runningFilePos;
      seektableSize = blockSize;

      if (onMeta) {
        void *pRawData = DRFLAC_MALLOC(blockSize);
        if (pRawData == NULL) {
          return DRFLAC_FALSE;
        }

        if (onRead(pUserData, pRawData, blockSize) != blockSize) {
          DRFLAC_FREE(pRawData);
          return DRFLAC_FALSE;
        }

        metadata.pRawData = pRawData;
        metadata.rawDataSize = blockSize;
        metadata.data.seektable.seekpointCount = blockSize / sizeof(drflac_seekpoint);
        metadata.data.seektable.pSeekpoints = (const drflac_seekpoint *)pRawData;

        for (drflac_uint32 iSeekpoint = 0; iSeekpoint < metadata.data.seektable.seekpointCount; ++iSeekpoint) {
          drflac_seekpoint *pSeekpoint = (drflac_seekpoint *)pRawData + iSeekpoint;
          pSeekpoint->firstSample = drflac__be2host_64(pSeekpoint->firstSample);
          pSeekpoint->frameOffset = drflac__be2host_64(pSeekpoint->frameOffset);
          pSeekpoint->sampleCount = drflac__be2host_16(pSeekpoint->sampleCount);
        }

        onMeta(pUserDataMD, &metadata);

        DRFLAC_FREE(pRawData);
      }
    } break;

    case DRFLAC_METADATA_BLOCK_TYPE_VORBIS_COMMENT: {
      if (blockSize < 8) {
        return DRFLAC_FALSE;
      }

      if (onMeta) {
        void *pRawData = DRFLAC_MALLOC(blockSize);
        if (pRawData == NULL) {
          return DRFLAC_FALSE;
        }

        if (onRead(pUserData, pRawData, blockSize) != blockSize) {
          DRFLAC_FREE(pRawData);
          return DRFLAC_FALSE;
        }

        metadata.pRawData = pRawData;
        metadata.rawDataSize = blockSize;

        const char *pRunningData = (const char *)pRawData;
        const char *const pRunningDataEnd = (const char *)pRawData + blockSize;

        metadata.data.vorbis_comment.vendorLength = drflac__le2host_32(*(const drflac_uint32 *)pRunningData);
        pRunningData += 4;

        if ((pRunningDataEnd - pRunningData) - 4 < (drflac_int64)metadata.data.vorbis_comment.vendorLength) {
          DRFLAC_FREE(pRawData);
          return DRFLAC_FALSE;
        }
        metadata.data.vorbis_comment.vendor = pRunningData;
        pRunningData += metadata.data.vorbis_comment.vendorLength;
        metadata.data.vorbis_comment.commentCount = drflac__le2host_32(*(const drflac_uint32 *)pRunningData);
        pRunningData += 4;

        if ((pRunningDataEnd - pRunningData) / sizeof(drflac_uint32) < metadata.data.vorbis_comment.commentCount) {
          DRFLAC_FREE(pRawData);
          return DRFLAC_FALSE;
        }
        metadata.data.vorbis_comment.pComments = pRunningData;

        for (drflac_uint32 i = 0; i < metadata.data.vorbis_comment.commentCount; ++i) {
          if (pRunningDataEnd - pRunningData < 4) {
            DRFLAC_FREE(pRawData);
            return DRFLAC_FALSE;
          }
          const drflac_uint32 commentLength = drflac__le2host_32(*(const drflac_uint32 *)pRunningData);
          pRunningData += 4;
          if (pRunningDataEnd - pRunningData < (drflac_int64)commentLength) {
            DRFLAC_FREE(pRawData);
            return DRFLAC_FALSE;
          }
          pRunningData += commentLength;
        }

        onMeta(pUserDataMD, &metadata);

        DRFLAC_FREE(pRawData);
      }
    } break;

    case DRFLAC_METADATA_BLOCK_TYPE_CUESHEET: {
      if (blockSize < 396) {
        return DRFLAC_FALSE;
      }

      if (onMeta) {
        void *pRawData = DRFLAC_MALLOC(blockSize);
        if (pRawData == NULL) {
          return DRFLAC_FALSE;
        }

        if (onRead(pUserData, pRawData, blockSize) != blockSize) {
          DRFLAC_FREE(pRawData);
          return DRFLAC_FALSE;
        }

        metadata.pRawData = pRawData;
        metadata.rawDataSize = blockSize;

        char *pRunningData = (char *)pRawData;
        const char *const pRunningDataEnd = (const char *)pRawData + blockSize;

        drflac_copy_memory(metadata.data.cuesheet.catalog, pRunningData, 128);
        pRunningData += 128;
        metadata.data.cuesheet.leadInSampleCount = drflac__be2host_64(*(const drflac_uint64 *)pRunningData);
        pRunningData += 8;
        metadata.data.cuesheet.isCD = (pRunningData[0] & 0x80) != 0;
        pRunningData += 259;
        metadata.data.cuesheet.trackCount = pRunningData[0];
        pRunningData += 1;
        metadata.data.cuesheet.pTrackData = pRunningData;

        for (drflac_uint8 i = 0; i < metadata.data.cuesheet.trackCount; ++i) {
          if (pRunningDataEnd - pRunningData < 36) {
            DRFLAC_FREE(pRawData);
            return DRFLAC_FALSE;
          }

          pRunningData += 35;
          const drflac_uint8 indexCount = pRunningData[0];
          pRunningData += 1;
          const drflac_uint32 indexPointSize = indexCount * sizeof(drflac_cuesheet_track_index);
          if (pRunningDataEnd - pRunningData < (drflac_int64)indexPointSize) {
            DRFLAC_FREE(pRawData);
            return DRFLAC_FALSE;
          }

          for (drflac_uint8 index = 0; index < indexCount; ++index) {
            drflac_cuesheet_track_index *pTrack = (drflac_cuesheet_track_index *)pRunningData;
            pRunningData += sizeof(drflac_cuesheet_track_index);
            pTrack->offset = drflac__be2host_64(pTrack->offset);
          }
        }

        onMeta(pUserDataMD, &metadata);

        DRFLAC_FREE(pRawData);
      }
    } break;

    case DRFLAC_METADATA_BLOCK_TYPE_PICTURE: {
      if (blockSize < 32) {
        return DRFLAC_FALSE;
      }

      if (onMeta) {
        void *pRawData = DRFLAC_MALLOC(blockSize);
        if (pRawData == NULL) {
          return DRFLAC_FALSE;
        }

        if (onRead(pUserData, pRawData, blockSize) != blockSize) {
          DRFLAC_FREE(pRawData);
          return DRFLAC_FALSE;
        }

        metadata.pRawData = pRawData;
        metadata.rawDataSize = blockSize;

        const char *pRunningData = (const char *)pRawData;
        const char *const pRunningDataEnd = (const char *)pRawData + blockSize;

        metadata.data.picture.type = drflac__be2host_32(*(const drflac_uint32 *)pRunningData);
        pRunningData += 4;
        metadata.data.picture.mimeLength = drflac__be2host_32(*(const drflac_uint32 *)pRunningData);
        pRunningData += 4;

        if ((pRunningDataEnd - pRunningData) - 24 < (drflac_int64)metadata.data.picture.mimeLength) {
          DRFLAC_FREE(pRawData);
          return DRFLAC_FALSE;
        }
        metadata.data.picture.mime = pRunningData;
        pRunningData += metadata.data.picture.mimeLength;
        metadata.data.picture.descriptionLength = drflac__be2host_32(*(const drflac_uint32 *)pRunningData);
        pRunningData += 4;

        if ((pRunningDataEnd - pRunningData) - 20 < (drflac_int64)metadata.data.picture.descriptionLength) {
          DRFLAC_FREE(pRawData);
          return DRFLAC_FALSE;
        }
        metadata.data.picture.description = pRunningData;
        pRunningData += metadata.data.picture.descriptionLength;
        metadata.data.picture.width = drflac__be2host_32(*(const drflac_uint32 *)pRunningData);
        pRunningData += 4;
        metadata.data.picture.height = drflac__be2host_32(*(const drflac_uint32 *)pRunningData);
        pRunningData += 4;
        metadata.data.picture.colorDepth = drflac__be2host_32(*(const drflac_uint32 *)pRunningData);
        pRunningData += 4;
        metadata.data.picture.indexColorCount = drflac__be2host_32(*(const drflac_uint32 *)pRunningData);
        pRunningData += 4;
        metadata.data.picture.pictureDataSize = drflac__be2host_32(*(const drflac_uint32 *)pRunningData);
        pRunningData += 4;
        metadata.data.picture.pPictureData = (const drflac_uint8 *)pRunningData;

        if (pRunningDataEnd - pRunningData < (drflac_int64)metadata.data.picture.pictureDataSize) {
          DRFLAC_FREE(pRawData);
          return DRFLAC_FALSE;
        }

        onMeta(pUserDataMD, &metadata);

        DRFLAC_FREE(pRawData);
      }
    } break;

    case DRFLAC_METADATA_BLOCK_TYPE_PADDING: {
      if (onMeta) {
        metadata.data.padding.unused = 0;

        if (!onSeek(pUserData, blockSize, drflac_seek_origin_current)) {
          isLastBlock = DRFLAC_TRUE;
        } else {
          onMeta(pUserDataMD, &metadata);
        }
      }
    } break;

    case DRFLAC_METADATA_BLOCK_TYPE_INVALID: {

      if (onMeta) {
        if (!onSeek(pUserData, blockSize, drflac_seek_origin_current)) {
          isLastBlock = DRFLAC_TRUE;
        }
      }
    } break;

    default: {

      if (onMeta) {
        void *pRawData = DRFLAC_MALLOC(blockSize);
        if (pRawData == NULL) {
          return DRFLAC_FALSE;
        }

        if (onRead(pUserData, pRawData, blockSize) != blockSize) {
          DRFLAC_FREE(pRawData);
          return DRFLAC_FALSE;
        }

        metadata.pRawData = pRawData;
        metadata.rawDataSize = blockSize;
        onMeta(pUserDataMD, &metadata);

        DRFLAC_FREE(pRawData);
      }
    } break;
    }

    if (onMeta == NULL && blockSize > 0) {
      if (!onSeek(pUserData, blockSize, drflac_seek_origin_current)) {
        isLastBlock = DRFLAC_TRUE;
      }
    }

    runningFilePos += blockSize;
    if (isLastBlock) {
      break;
    }
  }

  *pSeektablePos = seektablePos;
  *pSeektableSize = seektableSize;
  *pFirstFramePos = runningFilePos;

  return DRFLAC_TRUE;
}

drflac_bool32 drflac__init_private__native(drflac_init_info *pInit, drflac_read_proc onRead, drflac_seek_proc onSeek,
                                           drflac_meta_proc onMeta, void *pUserData, void *pUserDataMD,
                                           drflac_bool32 relaxed)
{
  (void)onSeek;

  pInit->container = drflac_container_native;

  drflac_uint8 isLastBlock;
  drflac_uint8 blockType;
  drflac_uint32 blockSize;
  if (!drflac__read_and_decode_block_header(onRead, pUserData, &isLastBlock, &blockType, &blockSize)) {
    return DRFLAC_FALSE;
  }

  if (blockType != DRFLAC_METADATA_BLOCK_TYPE_STREAMINFO || blockSize != 34) {
    if (!relaxed) {

      return DRFLAC_FALSE;
    } else {

      pInit->hasStreamInfoBlock = DRFLAC_FALSE;
      pInit->hasMetadataBlocks = DRFLAC_FALSE;

      if (!drflac__read_next_frame_header(&pInit->bs, 0, &pInit->firstFrameHeader)) {
        return DRFLAC_FALSE;
      }

      if (pInit->firstFrameHeader.bitsPerSample == 0) {
        return DRFLAC_FALSE;
      }

      pInit->sampleRate = pInit->firstFrameHeader.sampleRate;
      pInit->channels = drflac__get_channel_count_from_channel_assignment(pInit->firstFrameHeader.channelAssignment);
      pInit->bitsPerSample = pInit->firstFrameHeader.bitsPerSample;
      pInit->maxBlockSize = 65535;
      return DRFLAC_TRUE;
    }
  } else {
    drflac_streaminfo streaminfo;
    if (!drflac__read_streaminfo(onRead, pUserData, &streaminfo)) {
      return DRFLAC_FALSE;
    }

    pInit->hasStreamInfoBlock = DRFLAC_TRUE;
    pInit->sampleRate = streaminfo.sampleRate;
    pInit->channels = streaminfo.channels;
    pInit->bitsPerSample = streaminfo.bitsPerSample;
    pInit->totalSampleCount = streaminfo.totalSampleCount;
    pInit->maxBlockSize = streaminfo.maxBlockSize;
    pInit->hasMetadataBlocks = !isLastBlock;

    if (onMeta) {
      drflac_metadata metadata;
      metadata.type = DRFLAC_METADATA_BLOCK_TYPE_STREAMINFO;
      metadata.pRawData = NULL;
      metadata.rawDataSize = 0;
      metadata.data.streaminfo = streaminfo;
      onMeta(pUserDataMD, &metadata);
    }

    return DRFLAC_TRUE;
  }
}

#ifndef DR_FLAC_NO_OGG
#define DRFLAC_OGG_MAX_PAGE_SIZE 65307
#define DRFLAC_OGG_CAPTURE_PATTERN_CRC32 1605413199

typedef enum { drflac_ogg_recover_on_crc_mismatch, drflac_ogg_fail_on_crc_mismatch } drflac_ogg_crc_mismatch_recovery;

static drflac_uint32 drflac__crc32_table[] = {
    0x00000000L, 0x04C11DB7L, 0x09823B6EL, 0x0D4326D9L, 0x130476DCL, 0x17C56B6BL, 0x1A864DB2L, 0x1E475005L, 0x2608EDB8L,
    0x22C9F00FL, 0x2F8AD6D6L, 0x2B4BCB61L, 0x350C9B64L, 0x31CD86D3L, 0x3C8EA00AL, 0x384FBDBDL, 0x4C11DB70L, 0x48D0C6C7L,
    0x4593E01EL, 0x4152FDA9L, 0x5F15ADACL, 0x5BD4B01BL, 0x569796C2L, 0x52568B75L, 0x6A1936C8L, 0x6ED82B7FL, 0x639B0DA6L,
    0x675A1011L, 0x791D4014L, 0x7DDC5DA3L, 0x709F7B7AL, 0x745E66CDL, 0x9823B6E0L, 0x9CE2AB57L, 0x91A18D8EL, 0x95609039L,
    0x8B27C03CL, 0x8FE6DD8BL, 0x82A5FB52L, 0x8664E6E5L, 0xBE2B5B58L, 0xBAEA46EFL, 0xB7A96036L, 0xB3687D81L, 0xAD2F2D84L,
    0xA9EE3033L, 0xA4AD16EAL, 0xA06C0B5DL, 0xD4326D90L, 0xD0F37027L, 0xDDB056FEL, 0xD9714B49L, 0xC7361B4CL, 0xC3F706FBL,
    0xCEB42022L, 0xCA753D95L, 0xF23A8028L, 0xF6FB9D9FL, 0xFBB8BB46L, 0xFF79A6F1L, 0xE13EF6F4L, 0xE5FFEB43L, 0xE8BCCD9AL,
    0xEC7DD02DL, 0x34867077L, 0x30476DC0L, 0x3D044B19L, 0x39C556AEL, 0x278206ABL, 0x23431B1CL, 0x2E003DC5L, 0x2AC12072L,
    0x128E9DCFL, 0x164F8078L, 0x1B0CA6A1L, 0x1FCDBB16L, 0x018AEB13L, 0x054BF6A4L, 0x0808D07DL, 0x0CC9CDCAL, 0x7897AB07L,
    0x7C56B6B0L, 0x71159069L, 0x75D48DDEL, 0x6B93DDDBL, 0x6F52C06CL, 0x6211E6B5L, 0x66D0FB02L, 0x5E9F46BFL, 0x5A5E5B08L,
    0x571D7DD1L, 0x53DC6066L, 0x4D9B3063L, 0x495A2DD4L, 0x44190B0DL, 0x40D816BAL, 0xACA5C697L, 0xA864DB20L, 0xA527FDF9L,
    0xA1E6E04EL, 0xBFA1B04BL, 0xBB60ADFCL, 0xB6238B25L, 0xB2E29692L, 0x8AAD2B2FL, 0x8E6C3698L, 0x832F1041L, 0x87EE0DF6L,
    0x99A95DF3L, 0x9D684044L, 0x902B669DL, 0x94EA7B2AL, 0xE0B41DE7L, 0xE4750050L, 0xE9362689L, 0xEDF73B3EL, 0xF3B06B3BL,
    0xF771768CL, 0xFA325055L, 0xFEF34DE2L, 0xC6BCF05FL, 0xC27DEDE8L, 0xCF3ECB31L, 0xCBFFD686L, 0xD5B88683L, 0xD1799B34L,
    0xDC3ABDEDL, 0xD8FBA05AL, 0x690CE0EEL, 0x6DCDFD59L, 0x608EDB80L, 0x644FC637L, 0x7A089632L, 0x7EC98B85L, 0x738AAD5CL,
    0x774BB0EBL, 0x4F040D56L, 0x4BC510E1L, 0x46863638L, 0x42472B8FL, 0x5C007B8AL, 0x58C1663DL, 0x558240E4L, 0x51435D53L,
    0x251D3B9EL, 0x21DC2629L, 0x2C9F00F0L, 0x285E1D47L, 0x36194D42L, 0x32D850F5L, 0x3F9B762CL, 0x3B5A6B9BL, 0x0315D626L,
    0x07D4CB91L, 0x0A97ED48L, 0x0E56F0FFL, 0x1011A0FAL, 0x14D0BD4DL, 0x19939B94L, 0x1D528623L, 0xF12F560EL, 0xF5EE4BB9L,
    0xF8AD6D60L, 0xFC6C70D7L, 0xE22B20D2L, 0xE6EA3D65L, 0xEBA91BBCL, 0xEF68060BL, 0xD727BBB6L, 0xD3E6A601L, 0xDEA580D8L,
    0xDA649D6FL, 0xC423CD6AL, 0xC0E2D0DDL, 0xCDA1F604L, 0xC960EBB3L, 0xBD3E8D7EL, 0xB9FF90C9L, 0xB4BCB610L, 0xB07DABA7L,
    0xAE3AFBA2L, 0xAAFBE615L, 0xA7B8C0CCL, 0xA379DD7BL, 0x9B3660C6L, 0x9FF77D71L, 0x92B45BA8L, 0x9675461FL, 0x8832161AL,
    0x8CF30BADL, 0x81B02D74L, 0x857130C3L, 0x5D8A9099L, 0x594B8D2EL, 0x5408ABF7L, 0x50C9B640L, 0x4E8EE645L, 0x4A4FFBF2L,
    0x470CDD2BL, 0x43CDC09CL, 0x7B827D21L, 0x7F436096L, 0x7200464FL, 0x76C15BF8L, 0x68860BFDL, 0x6C47164AL, 0x61043093L,
    0x65C52D24L, 0x119B4BE9L, 0x155A565EL, 0x18197087L, 0x1CD86D30L, 0x029F3D35L, 0x065E2082L, 0x0B1D065BL, 0x0FDC1BECL,
    0x3793A651L, 0x3352BBE6L, 0x3E119D3FL, 0x3AD08088L, 0x2497D08DL, 0x2056CD3AL, 0x2D15EBE3L, 0x29D4F654L, 0xC5A92679L,
    0xC1683BCEL, 0xCC2B1D17L, 0xC8EA00A0L, 0xD6AD50A5L, 0xD26C4D12L, 0xDF2F6BCBL, 0xDBEE767CL, 0xE3A1CBC1L, 0xE760D676L,
    0xEA23F0AFL, 0xEEE2ED18L, 0xF0A5BD1DL, 0xF464A0AAL, 0xF9278673L, 0xFDE69BC4L, 0x89B8FD09L, 0x8D79E0BEL, 0x803AC667L,
    0x84FBDBD0L, 0x9ABC8BD5L, 0x9E7D9662L, 0x933EB0BBL, 0x97FFAD0CL, 0xAFB010B1L, 0xAB710D06L, 0xA6322BDFL, 0xA2F33668L,
    0xBCB4666DL, 0xB8757BDAL, 0xB5365D03L, 0xB1F740B4L};

static DRFLAC_INLINE drflac_uint32 drflac_crc32_byte(drflac_uint32 crc32, drflac_uint8 data)
{
#ifndef DR_FLAC_NO_CRC
  return (crc32 << 8) ^ drflac__crc32_table[(drflac_uint8)((crc32 >> 24) & 0xFF) ^ data];
#else
  (void)data;
  return crc32;
#endif
}

#if 0
static DRFLAC_INLINE drflac_uint32 drflac_crc32_uint32(drflac_uint32 crc32, drflac_uint32 data)
{
    crc32 = drflac_crc32_byte(crc32, (drflac_uint8)((data >> 24) & 0xFF));
    crc32 = drflac_crc32_byte(crc32, (drflac_uint8)((data >> 16) & 0xFF));
    crc32 = drflac_crc32_byte(crc32, (drflac_uint8)((data >>  8) & 0xFF));
    crc32 = drflac_crc32_byte(crc32, (drflac_uint8)((data >>  0) & 0xFF));
    return crc32;
}

static DRFLAC_INLINE drflac_uint32 drflac_crc32_uint64(drflac_uint32 crc32, drflac_uint64 data)
{
    crc32 = drflac_crc32_uint32(crc32, (drflac_uint32)((data >> 32) & 0xFFFFFFFF));
    crc32 = drflac_crc32_uint32(crc32, (drflac_uint32)((data >>  0) & 0xFFFFFFFF));
    return crc32;
}
#endif

static DRFLAC_INLINE drflac_uint32 drflac_crc32_buffer(drflac_uint32 crc32, drflac_uint8 *pData, drflac_uint32 dataSize)
{

  for (drflac_uint32 i = 0; i < dataSize; ++i) {
    crc32 = drflac_crc32_byte(crc32, pData[i]);
  }
  return crc32;
}

static DRFLAC_INLINE drflac_bool32 drflac_ogg__is_capture_pattern(drflac_uint8 pattern[4])
{
  return pattern[0] == 'O' && pattern[1] == 'g' && pattern[2] == 'g' && pattern[3] == 'S';
}

static DRFLAC_INLINE drflac_uint32 drflac_ogg__get_page_header_size(drflac_ogg_page_header *pHeader)
{
  return 27 + pHeader->segmentCount;
}

static DRFLAC_INLINE drflac_uint32 drflac_ogg__get_page_body_size(drflac_ogg_page_header *pHeader)
{
  drflac_uint32 pageBodySize = 0;
  for (int i = 0; i < pHeader->segmentCount; ++i) {
    pageBodySize += pHeader->segmentTable[i];
  }

  return pageBodySize;
}

drflac_result drflac_ogg__read_page_header_after_capture_pattern(drflac_read_proc onRead, void *pUserData,
                                                                 drflac_ogg_page_header *pHeader,
                                                                 drflac_uint32 *pBytesRead, drflac_uint32 *pCRC32)
{
  drflac_assert(*pCRC32 == DRFLAC_OGG_CAPTURE_PATTERN_CRC32);

  drflac_uint8 data[23];
  if (onRead(pUserData, data, 23) != 23) {
    return DRFLAC_END_OF_STREAM;
  }
  *pBytesRead += 23;

  pHeader->structureVersion = data[0];
  pHeader->headerType = data[1];
  drflac_copy_memory(&pHeader->granulePosition, &data[2], 8);
  drflac_copy_memory(&pHeader->serialNumber, &data[10], 4);
  drflac_copy_memory(&pHeader->sequenceNumber, &data[14], 4);
  drflac_copy_memory(&pHeader->checksum, &data[18], 4);
  pHeader->segmentCount = data[22];

  data[18] = 0;
  data[19] = 0;
  data[20] = 0;
  data[21] = 0;

  drflac_uint32 i;
  for (i = 0; i < 23; ++i) {
    *pCRC32 = drflac_crc32_byte(*pCRC32, data[i]);
  }

  if (onRead(pUserData, pHeader->segmentTable, pHeader->segmentCount) != pHeader->segmentCount) {
    return DRFLAC_END_OF_STREAM;
  }
  *pBytesRead += pHeader->segmentCount;

  for (i = 0; i < pHeader->segmentCount; ++i) {
    *pCRC32 = drflac_crc32_byte(*pCRC32, pHeader->segmentTable[i]);
  }

  return DRFLAC_SUCCESS;
}

drflac_result drflac_ogg__read_page_header(drflac_read_proc onRead, void *pUserData, drflac_ogg_page_header *pHeader,
                                           drflac_uint32 *pBytesRead, drflac_uint32 *pCRC32)
{
  *pBytesRead = 0;

  drflac_uint8 id[4];
  if (onRead(pUserData, id, 4) != 4) {
    return DRFLAC_END_OF_STREAM;
  }
  *pBytesRead += 4;

  for (;;) {
    if (drflac_ogg__is_capture_pattern(id)) {
      *pCRC32 = DRFLAC_OGG_CAPTURE_PATTERN_CRC32;

      drflac_result result =
          drflac_ogg__read_page_header_after_capture_pattern(onRead, pUserData, pHeader, pBytesRead, pCRC32);
      if (result == DRFLAC_SUCCESS) {
        return DRFLAC_SUCCESS;
      } else {
        if (result == DRFLAC_CRC_MISMATCH) {
          continue;
        } else {
          return result;
        }
      }
    } else {

      id[0] = id[1];
      id[1] = id[2];
      id[2] = id[3];
      if (onRead(pUserData, &id[3], 1) != 1) {
        return DRFLAC_END_OF_STREAM;
      }
      *pBytesRead += 1;
    }
  }
}

typedef struct
{
  drflac_read_proc onRead;
  drflac_seek_proc onSeek;
  void *pUserData;
  drflac_uint64 currentBytePos;
  drflac_uint64 firstBytePos;
  drflac_uint32 serialNumber;
  drflac_ogg_page_header bosPageHeader;
  drflac_ogg_page_header currentPageHeader;
  drflac_uint32 bytesRemainingInPage;
  drflac_uint32 pageDataSize;
  drflac_uint8 pageData[DRFLAC_OGG_MAX_PAGE_SIZE];
} drflac_oggbs;

static size_t drflac_oggbs__read_physical(drflac_oggbs *oggbs, void *bufferOut, size_t bytesToRead)
{
  size_t bytesActuallyRead = oggbs->onRead(oggbs->pUserData, bufferOut, bytesToRead);
  oggbs->currentBytePos += bytesActuallyRead;

  return bytesActuallyRead;
}

static drflac_bool32 drflac_oggbs__seek_physical(drflac_oggbs *oggbs, drflac_uint64 offset, drflac_seek_origin origin)
{
  if (origin == drflac_seek_origin_start) {
    if (offset <= 0x7FFFFFFF) {
      if (!oggbs->onSeek(oggbs->pUserData, (int)offset, drflac_seek_origin_start)) {
        return DRFLAC_FALSE;
      }
      oggbs->currentBytePos = offset;

      return DRFLAC_TRUE;
    } else {
      if (!oggbs->onSeek(oggbs->pUserData, 0x7FFFFFFF, drflac_seek_origin_start)) {
        return DRFLAC_FALSE;
      }
      oggbs->currentBytePos = offset;

      return drflac_oggbs__seek_physical(oggbs, offset - 0x7FFFFFFF, drflac_seek_origin_current);
    }
  } else {
    while (offset > 0x7FFFFFFF) {
      if (!oggbs->onSeek(oggbs->pUserData, 0x7FFFFFFF, drflac_seek_origin_current)) {
        return DRFLAC_FALSE;
      }
      oggbs->currentBytePos += 0x7FFFFFFF;
      offset -= 0x7FFFFFFF;
    }

    if (!oggbs->onSeek(oggbs->pUserData, (int)offset, drflac_seek_origin_current)) {
      return DRFLAC_FALSE;
    }
    oggbs->currentBytePos += offset;

    return DRFLAC_TRUE;
  }
}

static drflac_bool32 drflac_oggbs__goto_next_page(drflac_oggbs *oggbs, drflac_ogg_crc_mismatch_recovery recoveryMethod)
{
  drflac_ogg_page_header header;
  for (;;) {
    drflac_uint32 crc32 = 0;
    drflac_uint32 bytesRead;
    if (drflac_ogg__read_page_header(oggbs->onRead, oggbs->pUserData, &header, &bytesRead, &crc32) != DRFLAC_SUCCESS) {
      return DRFLAC_FALSE;
    }
    oggbs->currentBytePos += bytesRead;

    drflac_uint32 pageBodySize = drflac_ogg__get_page_body_size(&header);
    if (pageBodySize > DRFLAC_OGG_MAX_PAGE_SIZE) {
      continue;
    }

    if (header.serialNumber != oggbs->serialNumber) {

      if (pageBodySize > 0 && !drflac_oggbs__seek_physical(oggbs, pageBodySize, drflac_seek_origin_current)) {
        return DRFLAC_FALSE;
      }
      continue;
    }

    if (drflac_oggbs__read_physical(oggbs, oggbs->pageData, pageBodySize) != pageBodySize) {
      return DRFLAC_FALSE;
    }
    oggbs->pageDataSize = pageBodySize;

#ifndef DR_FLAC_NO_CRC
    drflac_uint32 actualCRC32 = drflac_crc32_buffer(crc32, oggbs->pageData, oggbs->pageDataSize);
    if (actualCRC32 != header.checksum) {
      if (recoveryMethod == drflac_ogg_recover_on_crc_mismatch) {
        continue;
      } else {

        drflac_oggbs__goto_next_page(oggbs, drflac_ogg_recover_on_crc_mismatch);
        return DRFLAC_FALSE;
      }
    }
#else
    (void)recoveryMethod;
#endif

    oggbs->currentPageHeader = header;
    oggbs->bytesRemainingInPage = pageBodySize;
    return DRFLAC_TRUE;
  }
}

#if 0
static drflac_uint8 drflac_oggbs__get_current_segment_index(drflac_oggbs* oggbs, drflac_uint8* pBytesRemainingInSeg)
{
    drflac_uint32 bytesConsumedInPage = drflac_ogg__get_page_body_size(
        &oggbs->currentPageHeader) - oggbs->bytesRemainingInPage;
    drflac_uint8 iSeg = 0;
    drflac_uint32 iByte = 0;
    while (iByte < bytesConsumedInPage) {
        drflac_uint8 segmentSize = oggbs->currentPageHeader.segmentTable[iSeg];
        if (iByte + segmentSize > bytesConsumedInPage) {
            break;
        } else {
            iSeg += 1;
            iByte += segmentSize;
        }
    }

    *pBytesRemainingInSeg = oggbs->currentPageHeader.segmentTable[iSeg] - (drflac_uint8)(bytesConsumedInPage - iByte);
    return iSeg;
}

static drflac_bool32 drflac_oggbs__seek_to_next_packet(drflac_oggbs* oggbs)
{
    for (;;) {
        drflac_bool32 atEndOfPage = DRFLAC_FALSE;

        drflac_uint8 bytesRemainingInSeg;
        drflac_uint8 iFirstSeg = drflac_oggbs__get_current_segment_index(oggbs, &bytesRemainingInSeg);

        drflac_uint32 bytesToEndOfPacketOrPage = bytesRemainingInSeg;
        for (drflac_uint8 iSeg = iFirstSeg; iSeg < oggbs->currentPageHeader.segmentCount; ++iSeg) {
            drflac_uint8 segmentSize = oggbs->currentPageHeader.segmentTable[iSeg];
            if (segmentSize < 255) {
                if (iSeg == oggbs->currentPageHeader.segmentCount-1) {
                    atEndOfPage = DRFLAC_TRUE;
                }

                break;
            }

            bytesToEndOfPacketOrPage += segmentSize;
        }

        drflac_oggbs__seek_physical(oggbs, bytesToEndOfPacketOrPage, drflac_seek_origin_current);
        oggbs->bytesRemainingInPage -= bytesToEndOfPacketOrPage;

        if (atEndOfPage) {
            if (!drflac_oggbs__goto_next_page(oggbs)) {
                return DRFLAC_FALSE;
            }

            if ((oggbs->currentPageHeader.headerType & 0x01) == 0) {
                return DRFLAC_TRUE;
            }
        } else {
            return DRFLAC_TRUE;
        }
    }
}

static drflac_bool32 drflac_oggbs__seek_to_next_frame(drflac_oggbs* oggbs)
{
    return drflac_oggbs__seek_to_next_packet(oggbs);
}
#endif

static size_t drflac__on_read_ogg(void *pUserData, void *bufferOut, size_t bytesToRead)
{
  drflac_oggbs *oggbs = (drflac_oggbs *)pUserData;
  drflac_assert(oggbs != NULL);

  drflac_uint8 *pRunningBufferOut = (drflac_uint8 *)bufferOut;

  size_t bytesRead = 0;
  while (bytesRead < bytesToRead) {
    size_t bytesRemainingToRead = bytesToRead - bytesRead;

    if (oggbs->bytesRemainingInPage >= bytesRemainingToRead) {
      drflac_copy_memory(pRunningBufferOut, oggbs->pageData + (oggbs->pageDataSize - oggbs->bytesRemainingInPage),
                         bytesRemainingToRead);
      bytesRead += bytesRemainingToRead;
      oggbs->bytesRemainingInPage -= (drflac_uint32)bytesRemainingToRead;
      break;
    }

    if (oggbs->bytesRemainingInPage > 0) {
      drflac_copy_memory(pRunningBufferOut, oggbs->pageData + (oggbs->pageDataSize - oggbs->bytesRemainingInPage),
                         oggbs->bytesRemainingInPage);
      bytesRead += oggbs->bytesRemainingInPage;
      pRunningBufferOut += oggbs->bytesRemainingInPage;
      oggbs->bytesRemainingInPage = 0;
    }

    drflac_assert(bytesRemainingToRead > 0);
    if (!drflac_oggbs__goto_next_page(oggbs, drflac_ogg_recover_on_crc_mismatch)) {
      break;
    }
  }

  return bytesRead;
}

static drflac_bool32 drflac__on_seek_ogg(void *pUserData, int offset, drflac_seek_origin origin)
{
  drflac_oggbs *oggbs = (drflac_oggbs *)pUserData;
  drflac_assert(oggbs != NULL);
  drflac_assert(offset >= 0);

  if (origin == drflac_seek_origin_start) {
    if (!drflac_oggbs__seek_physical(oggbs, (int)oggbs->firstBytePos, drflac_seek_origin_start)) {
      return DRFLAC_FALSE;
    }

    if (!drflac_oggbs__goto_next_page(oggbs, drflac_ogg_fail_on_crc_mismatch)) {
      return DRFLAC_FALSE;
    }

    return drflac__on_seek_ogg(pUserData, offset, drflac_seek_origin_current);
  }

  drflac_assert(origin == drflac_seek_origin_current);

  int bytesSeeked = 0;
  while (bytesSeeked < offset) {
    int bytesRemainingToSeek = offset - bytesSeeked;
    drflac_assert(bytesRemainingToSeek >= 0);

    if (oggbs->bytesRemainingInPage >= (size_t)bytesRemainingToSeek) {
      bytesSeeked += bytesRemainingToSeek;
      oggbs->bytesRemainingInPage -= bytesRemainingToSeek;
      break;
    }

    if (oggbs->bytesRemainingInPage > 0) {
      bytesSeeked += (int)oggbs->bytesRemainingInPage;
      oggbs->bytesRemainingInPage = 0;
    }

    drflac_assert(bytesRemainingToSeek > 0);
    if (!drflac_oggbs__goto_next_page(oggbs, drflac_ogg_fail_on_crc_mismatch)) {

      return DRFLAC_FALSE;
    }
  }

  return DRFLAC_TRUE;
}

drflac_bool32 drflac_ogg__seek_to_sample(drflac *pFlac, drflac_uint64 sampleIndex)
{
  drflac_oggbs *oggbs = (drflac_oggbs *)pFlac->_oggbs;

  drflac_uint64 originalBytePos = oggbs->currentBytePos;

  if (!drflac__seek_to_byte(&pFlac->bs, pFlac->firstFramePos)) {
    return DRFLAC_FALSE;
  }
  oggbs->bytesRemainingInPage = 0;

  drflac_uint64 runningGranulePosition = 0;
  drflac_uint64 runningFrameBytePos = oggbs->currentBytePos;
  for (;;) {
    if (!drflac_oggbs__goto_next_page(oggbs, drflac_ogg_recover_on_crc_mismatch)) {
      drflac_oggbs__seek_physical(oggbs, originalBytePos, drflac_seek_origin_start);
      return DRFLAC_FALSE;
    }

    runningFrameBytePos =
        oggbs->currentBytePos - drflac_ogg__get_page_header_size(&oggbs->currentPageHeader) - oggbs->pageDataSize;
    if (oggbs->currentPageHeader.granulePosition * pFlac->channels >= sampleIndex) {
      break;
    }

    if ((oggbs->currentPageHeader.headerType & 0x01) == 0) {
      if (oggbs->currentPageHeader.segmentTable[0] >= 2) {
        drflac_uint8 firstBytesInPage[2];
        firstBytesInPage[0] = oggbs->pageData[0];
        firstBytesInPage[1] = oggbs->pageData[1];

        if ((firstBytesInPage[0] == 0xFF) && (firstBytesInPage[1] & 0xFC) == 0xF8) {
          runningGranulePosition = oggbs->currentPageHeader.granulePosition * pFlac->channels;
        }

        continue;
      }
    }
  }

  if (!drflac_oggbs__seek_physical(oggbs, runningFrameBytePos, drflac_seek_origin_start)) {
    return DRFLAC_FALSE;
  }
  if (!drflac_oggbs__goto_next_page(oggbs, drflac_ogg_recover_on_crc_mismatch)) {
    return DRFLAC_FALSE;
  }

  drflac_uint64 runningSampleCount = runningGranulePosition;
  for (;;) {

    if (!drflac__read_next_frame_header(&pFlac->bs, pFlac->bitsPerSample, &pFlac->currentFrame.header)) {
      return DRFLAC_FALSE;
    }

    drflac_uint64 firstSampleInFrame = 0;
    drflac_uint64 lastSampleInFrame = 0;
    drflac__get_current_frame_sample_range(pFlac, &firstSampleInFrame, &lastSampleInFrame);

    drflac_uint64 sampleCountInThisFrame = (lastSampleInFrame - firstSampleInFrame) + 1;
    if (sampleIndex < (runningSampleCount + sampleCountInThisFrame)) {

      drflac_result result = drflac__decode_frame(pFlac);
      if (result == DRFLAC_SUCCESS) {

        drflac_uint64 samplesToDecode = (size_t)(sampleIndex - runningSampleCount);
        if (samplesToDecode == 0) {
          return DRFLAC_TRUE;
        }
        return drflac_read_s32(pFlac, samplesToDecode, NULL) != 0;
      } else {
        if (result == DRFLAC_CRC_MISMATCH) {
          continue;
        } else {
          return DRFLAC_FALSE;
        }
      }
    } else {

      drflac_result result = drflac__seek_to_next_frame(pFlac);
      if (result == DRFLAC_SUCCESS) {
        runningSampleCount += sampleCountInThisFrame;
      } else {
        if (result == DRFLAC_CRC_MISMATCH) {
          continue;
        } else {
          return DRFLAC_FALSE;
        }
      }
    }
  }
}

drflac_bool32 drflac__init_private__ogg(drflac_init_info *pInit, drflac_read_proc onRead, drflac_seek_proc onSeek,
                                        drflac_meta_proc onMeta, void *pUserData, void *pUserDataMD,
                                        drflac_bool32 relaxed)
{

  (void)relaxed;

  pInit->container = drflac_container_ogg;
  pInit->oggFirstBytePos = 0;

  drflac_ogg_page_header header;

  drflac_uint32 crc32 = DRFLAC_OGG_CAPTURE_PATTERN_CRC32;
  drflac_uint32 bytesRead = 0;
  if (drflac_ogg__read_page_header_after_capture_pattern(onRead, pUserData, &header, &bytesRead, &crc32)
      != DRFLAC_SUCCESS) {
    return DRFLAC_FALSE;
  }
  pInit->runningFilePos += bytesRead;

  for (;;) {

    if ((header.headerType & 0x02) == 0) {
      return DRFLAC_FALSE;
    }

    int pageBodySize = drflac_ogg__get_page_body_size(&header);
    if (pageBodySize == 51) {

      drflac_uint32 bytesRemainingInPage = pageBodySize;

      drflac_uint8 packetType;
      if (onRead(pUserData, &packetType, 1) != 1) {
        return DRFLAC_FALSE;
      }

      bytesRemainingInPage -= 1;
      if (packetType == 0x7F) {

        drflac_uint8 sig[4];
        if (onRead(pUserData, sig, 4) != 4) {
          return DRFLAC_FALSE;
        }

        bytesRemainingInPage -= 4;
        if (sig[0] == 'F' && sig[1] == 'L' && sig[2] == 'A' && sig[3] == 'C') {

          drflac_uint8 mappingVersion[2];
          if (onRead(pUserData, mappingVersion, 2) != 2) {
            return DRFLAC_FALSE;
          }

          if (mappingVersion[0] != 1) {
            return DRFLAC_FALSE;
          }

          if (!onSeek(pUserData, 2, drflac_seek_origin_current)) {
            return DRFLAC_FALSE;
          }

          if (onRead(pUserData, sig, 4) != 4) {
            return DRFLAC_FALSE;
          }

          if (sig[0] == 'f' && sig[1] == 'L' && sig[2] == 'a' && sig[3] == 'C') {

            drflac_uint8 isLastBlock;
            drflac_uint8 blockType;
            drflac_uint32 blockSize;
            if (!drflac__read_and_decode_block_header(onRead, pUserData, &isLastBlock, &blockType, &blockSize)) {
              return DRFLAC_FALSE;
            }

            if (blockType != DRFLAC_METADATA_BLOCK_TYPE_STREAMINFO || blockSize != 34) {
              return DRFLAC_FALSE;
            }

            drflac_streaminfo streaminfo;
            if (drflac__read_streaminfo(onRead, pUserData, &streaminfo)) {

              pInit->hasStreamInfoBlock = DRFLAC_TRUE;
              pInit->sampleRate = streaminfo.sampleRate;
              pInit->channels = streaminfo.channels;
              pInit->bitsPerSample = streaminfo.bitsPerSample;
              pInit->totalSampleCount = streaminfo.totalSampleCount;
              pInit->maxBlockSize = streaminfo.maxBlockSize;
              pInit->hasMetadataBlocks = !isLastBlock;

              if (onMeta) {
                drflac_metadata metadata;
                metadata.type = DRFLAC_METADATA_BLOCK_TYPE_STREAMINFO;
                metadata.pRawData = NULL;
                metadata.rawDataSize = 0;
                metadata.data.streaminfo = streaminfo;
                onMeta(pUserDataMD, &metadata);
              }

              pInit->runningFilePos += pageBodySize;
              pInit->oggFirstBytePos = pInit->runningFilePos - 79;
              pInit->oggSerial = header.serialNumber;
              pInit->oggBosHeader = header;
              break;
            } else {

              return DRFLAC_FALSE;
            }
          } else {

            return DRFLAC_FALSE;
          }
        } else {

          if (!onSeek(pUserData, bytesRemainingInPage, drflac_seek_origin_current)) {
            return DRFLAC_FALSE;
          }
        }
      } else {

        if (!onSeek(pUserData, bytesRemainingInPage, drflac_seek_origin_current)) {
          return DRFLAC_FALSE;
        }
      }
    } else {
      if (!onSeek(pUserData, pageBodySize, drflac_seek_origin_current)) {
        return DRFLAC_FALSE;
      }
    }

    pInit->runningFilePos += pageBodySize;

    if (drflac_ogg__read_page_header(onRead, pUserData, &header, &bytesRead, &crc32) != DRFLAC_SUCCESS) {
      return DRFLAC_FALSE;
    }
    pInit->runningFilePos += bytesRead;
  }

  pInit->hasMetadataBlocks = DRFLAC_TRUE;
  return DRFLAC_TRUE;
}
#endif

drflac_bool32 drflac__init_private(drflac_init_info *pInit, drflac_read_proc onRead, drflac_seek_proc onSeek,
                                   drflac_meta_proc onMeta, drflac_container container, void *pUserData,
                                   void *pUserDataMD)
{
  if (pInit == NULL || onRead == NULL || onSeek == NULL) {
    return DRFLAC_FALSE;
  }

  drflac_zero_memory(pInit, sizeof(*pInit));
  pInit->onRead = onRead;
  pInit->onSeek = onSeek;
  pInit->onMeta = onMeta;
  pInit->container = container;
  pInit->pUserData = pUserData;
  pInit->pUserDataMD = pUserDataMD;

  pInit->bs.onRead = onRead;
  pInit->bs.onSeek = onSeek;
  pInit->bs.pUserData = pUserData;
  drflac__reset_cache(&pInit->bs);

  drflac_bool32 relaxed = container != drflac_container_unknown;

  drflac_uint8 id[4];

  for (;;) {
    if (onRead(pUserData, id, 4) != 4) {
      return DRFLAC_FALSE;
    }
    pInit->runningFilePos += 4;

    if (id[0] == 'I' && id[1] == 'D' && id[2] == '3') {
      drflac_uint8 header[6];
      if (onRead(pUserData, header, 6) != 6) {
        return DRFLAC_FALSE;
      }
      pInit->runningFilePos += 6;

      drflac_uint8 flags = header[1];
      drflac_uint32 headerSize;
      drflac_copy_memory(&headerSize, header + 2, 4);
      headerSize = drflac__unsynchsafe_32(drflac__be2host_32(headerSize));
      if (flags & 0x10) {
        headerSize += 10;
      }

      if (!onSeek(pUserData, headerSize, drflac_seek_origin_current)) {
        return DRFLAC_FALSE;
      }
      pInit->runningFilePos += headerSize;
    } else {
      break;
    }
  }

  if (id[0] == 'f' && id[1] == 'L' && id[2] == 'a' && id[3] == 'C') {
    return drflac__init_private__native(pInit, onRead, onSeek, onMeta, pUserData, pUserDataMD, relaxed);
  }
#ifndef DR_FLAC_NO_OGG
  if (id[0] == 'O' && id[1] == 'g' && id[2] == 'g' && id[3] == 'S') {
    return drflac__init_private__ogg(pInit, onRead, onSeek, onMeta, pUserData, pUserDataMD, relaxed);
  }
#endif

  if (relaxed) {
    if (container == drflac_container_native) {
      return drflac__init_private__native(pInit, onRead, onSeek, onMeta, pUserData, pUserDataMD, relaxed);
    }
#ifndef DR_FLAC_NO_OGG
    if (container == drflac_container_ogg) {
      return drflac__init_private__ogg(pInit, onRead, onSeek, onMeta, pUserData, pUserDataMD, relaxed);
    }
#endif
  }

  return DRFLAC_FALSE;
}

void drflac__init_from_info(drflac *pFlac, drflac_init_info *pInit)
{
  drflac_assert(pFlac != NULL);
  drflac_assert(pInit != NULL);

  drflac_zero_memory(pFlac, sizeof(*pFlac));
  pFlac->bs = pInit->bs;
  pFlac->onMeta = pInit->onMeta;
  pFlac->pUserDataMD = pInit->pUserDataMD;
  pFlac->maxBlockSize = pInit->maxBlockSize;
  pFlac->sampleRate = pInit->sampleRate;
  pFlac->channels = (drflac_uint8)pInit->channels;
  pFlac->bitsPerSample = (drflac_uint8)pInit->bitsPerSample;
  pFlac->totalSampleCount = pInit->totalSampleCount;
  pFlac->container = pInit->container;
}

drflac *drflac_open_with_metadata_private(drflac_read_proc onRead, drflac_seek_proc onSeek, drflac_meta_proc onMeta,
                                          drflac_container container, void *pUserData, void *pUserDataMD)
{
#ifndef DRFLAC_NO_CPUID

  drflac__init_cpu_caps();
#endif

  drflac_init_info init;
  if (!drflac__init_private(&init, onRead, onSeek, onMeta, container, pUserData, pUserDataMD)) {
    return NULL;
  }

  drflac_uint32 allocationSize = sizeof(drflac);

  drflac_uint32 wholeSIMDVectorCountPerChannel;
  if ((init.maxBlockSize % (DRFLAC_MAX_SIMD_VECTOR_SIZE / sizeof(drflac_int32))) == 0) {
    wholeSIMDVectorCountPerChannel = (init.maxBlockSize / (DRFLAC_MAX_SIMD_VECTOR_SIZE / sizeof(drflac_int32)));
  } else {
    wholeSIMDVectorCountPerChannel = (init.maxBlockSize / (DRFLAC_MAX_SIMD_VECTOR_SIZE / sizeof(drflac_int32))) + 1;
  }

  drflac_uint32 decodedSamplesAllocationSize =
      wholeSIMDVectorCountPerChannel * DRFLAC_MAX_SIMD_VECTOR_SIZE * init.channels;

  allocationSize += decodedSamplesAllocationSize;
  allocationSize += DRFLAC_MAX_SIMD_VECTOR_SIZE;

#ifndef DR_FLAC_NO_OGG

  drflac_uint32 oggbsAllocationSize = 0;
  if (init.container == drflac_container_ogg) {
    oggbsAllocationSize = sizeof(drflac_oggbs);
    allocationSize += oggbsAllocationSize;
  }

  drflac_oggbs oggbs;
  drflac_zero_memory(&oggbs, sizeof(oggbs));
  if (init.container == drflac_container_ogg) {
    oggbs.onRead = onRead;
    oggbs.onSeek = onSeek;
    oggbs.pUserData = pUserData;
    oggbs.currentBytePos = init.oggFirstBytePos;
    oggbs.firstBytePos = init.oggFirstBytePos;
    oggbs.serialNumber = init.oggSerial;
    oggbs.bosPageHeader = init.oggBosHeader;
    oggbs.bytesRemainingInPage = 0;
  }
#endif

  drflac_uint64 firstFramePos = 42;
  drflac_uint64 seektablePos = 0;
  drflac_uint32 seektableSize = 0;
  if (init.hasMetadataBlocks) {
    drflac_read_proc onReadOverride = onRead;
    drflac_seek_proc onSeekOverride = onSeek;
    void *pUserDataOverride = pUserData;

#ifndef DR_FLAC_NO_OGG
    if (init.container == drflac_container_ogg) {
      onReadOverride = drflac__on_read_ogg;
      onSeekOverride = drflac__on_seek_ogg;
      pUserDataOverride = (void *)&oggbs;
    }
#endif

    if (!drflac__read_and_decode_metadata(onReadOverride, onSeekOverride, onMeta, pUserDataOverride, pUserDataMD,
                                          &firstFramePos, &seektablePos, &seektableSize)) {
      return NULL;
    }

    allocationSize += seektableSize;
  }

  drflac *pFlac = (drflac *)DRFLAC_MALLOC(allocationSize);
  drflac__init_from_info(pFlac, &init);
  pFlac->pDecodedSamples = (drflac_int32 *)drflac_align((size_t)pFlac->pExtraData, DRFLAC_MAX_SIMD_VECTOR_SIZE);

#ifndef DR_FLAC_NO_OGG
  if (init.container == drflac_container_ogg) {
    drflac_oggbs *pInternalOggbs =
        (drflac_oggbs *)((drflac_uint8 *)pFlac->pDecodedSamples + decodedSamplesAllocationSize + seektableSize);
    *pInternalOggbs = oggbs;

    pFlac->bs.onRead = drflac__on_read_ogg;
    pFlac->bs.onSeek = drflac__on_seek_ogg;
    pFlac->bs.pUserData = (void *)pInternalOggbs;
    pFlac->_oggbs = (void *)pInternalOggbs;
  }
#endif

  pFlac->firstFramePos = firstFramePos;

#ifndef DR_FLAC_NO_OGG
  if (init.container == drflac_container_ogg) {
    pFlac->pSeekpoints = NULL;
    pFlac->seekpointCount = 0;
  } else
#endif
  {

    if (seektablePos != 0) {
      pFlac->seekpointCount = seektableSize / sizeof(*pFlac->pSeekpoints);
      pFlac->pSeekpoints = (drflac_seekpoint *)((drflac_uint8 *)pFlac->pDecodedSamples + decodedSamplesAllocationSize);

      if (pFlac->bs.onSeek(pFlac->bs.pUserData, (int)seektablePos, drflac_seek_origin_start)) {
        if (pFlac->bs.onRead(pFlac->bs.pUserData, pFlac->pSeekpoints, seektableSize) == seektableSize) {

          for (drflac_uint32 iSeekpoint = 0; iSeekpoint < pFlac->seekpointCount; ++iSeekpoint) {
            pFlac->pSeekpoints[iSeekpoint].firstSample = drflac__be2host_64(pFlac->pSeekpoints[iSeekpoint].firstSample);
            pFlac->pSeekpoints[iSeekpoint].frameOffset = drflac__be2host_64(pFlac->pSeekpoints[iSeekpoint].frameOffset);
            pFlac->pSeekpoints[iSeekpoint].sampleCount = drflac__be2host_16(pFlac->pSeekpoints[iSeekpoint].sampleCount);
          }
        } else {

          pFlac->pSeekpoints = NULL;
          pFlac->seekpointCount = 0;
        }

        if (!pFlac->bs.onSeek(pFlac->bs.pUserData, (int)pFlac->firstFramePos, drflac_seek_origin_start)) {
          DRFLAC_FREE(pFlac);
          return NULL;
        }
      } else {

        pFlac->pSeekpoints = NULL;
        pFlac->seekpointCount = 0;
      }
    }
  }

  if (!init.hasStreamInfoBlock) {
    pFlac->currentFrame.header = init.firstFrameHeader;
    do {
      drflac_result result = drflac__decode_frame(pFlac);
      if (result == DRFLAC_SUCCESS) {
        break;
      } else {
        if (result == DRFLAC_CRC_MISMATCH) {
          if (!drflac__read_next_frame_header(&pFlac->bs, pFlac->bitsPerSample, &pFlac->currentFrame.header)) {
            DRFLAC_FREE(pFlac);
            return NULL;
          }
          continue;
        } else {
          DRFLAC_FREE(pFlac);
          return NULL;
        }
      }
    } while (1);
  }

  return pFlac;
}

#ifndef DR_FLAC_NO_STDIO
#include <stdio.h>

static size_t drflac__on_read_stdio(void *pUserData, void *bufferOut, size_t bytesToRead)
{
  return fread(bufferOut, 1, bytesToRead, (FILE *)pUserData);
}

static drflac_bool32 drflac__on_seek_stdio(void *pUserData, int offset, drflac_seek_origin origin)
{
  drflac_assert(offset >= 0);

  return fseek((FILE *)pUserData, offset, (origin == drflac_seek_origin_current) ? SEEK_CUR : SEEK_SET) == 0;
}

static FILE *drflac__fopen(const char *filename)
{
  FILE *pFile;
#ifdef _MSC_VER
  if (fopen_s(&pFile, filename, "rb") != 0) {
    return NULL;
  }
#else
  pFile = fopen(filename, "rb");
  if (pFile == NULL) {
    return NULL;
  }
#endif

  return pFile;
}

drflac *drflac_open_file(const char *filename)
{
  FILE *file = drflac__fopen(filename);
  if (file == NULL) {
    return NULL;
  }

  drflac *pFlac = drflac_open(drflac__on_read_stdio, drflac__on_seek_stdio, (void *)file);
  if (pFlac == NULL) {
    fclose(file);
    return NULL;
  }

  return pFlac;
}

drflac *drflac_open_file_with_metadata(const char *filename, drflac_meta_proc onMeta, void *pUserData)
{
  FILE *file = drflac__fopen(filename);
  if (file == NULL) {
    return NULL;
  }

  drflac *pFlac = drflac_open_with_metadata_private(drflac__on_read_stdio, drflac__on_seek_stdio, onMeta,
                                                    drflac_container_unknown, (void *)file, pUserData);
  if (pFlac == NULL) {
    fclose(file);
    return pFlac;
  }

  return pFlac;
}
#endif

static size_t drflac__on_read_memory(void *pUserData, void *bufferOut, size_t bytesToRead)
{
  drflac__memory_stream *memoryStream = (drflac__memory_stream *)pUserData;
  drflac_assert(memoryStream != NULL);
  drflac_assert(memoryStream->dataSize >= memoryStream->currentReadPos);

  size_t bytesRemaining = memoryStream->dataSize - memoryStream->currentReadPos;
  if (bytesToRead > bytesRemaining) {
    bytesToRead = bytesRemaining;
  }

  if (bytesToRead > 0) {
    drflac_copy_memory(bufferOut, memoryStream->data + memoryStream->currentReadPos, bytesToRead);
    memoryStream->currentReadPos += bytesToRead;
  }

  return bytesToRead;
}

static drflac_bool32 drflac__on_seek_memory(void *pUserData, int offset, drflac_seek_origin origin)
{
  drflac__memory_stream *memoryStream = (drflac__memory_stream *)pUserData;
  drflac_assert(memoryStream != NULL);
  drflac_assert(offset >= 0);

  if (offset > (drflac_int64)memoryStream->dataSize) {
    return DRFLAC_FALSE;
  }

  if (origin == drflac_seek_origin_current) {
    if (memoryStream->currentReadPos + offset <= memoryStream->dataSize) {
      memoryStream->currentReadPos += offset;
    } else {
      return DRFLAC_FALSE;
    }
  } else {
    if ((drflac_uint32)offset <= memoryStream->dataSize) {
      memoryStream->currentReadPos = offset;
    } else {
      return DRFLAC_FALSE;
    }
  }

  return DRFLAC_TRUE;
}

drflac *drflac_open_memory(const void *data, size_t dataSize)
{
  drflac__memory_stream memoryStream;
  memoryStream.data = (const unsigned char *)data;
  memoryStream.dataSize = dataSize;
  memoryStream.currentReadPos = 0;
  drflac *pFlac = drflac_open(drflac__on_read_memory, drflac__on_seek_memory, &memoryStream);
  if (pFlac == NULL) {
    return NULL;
  }

  pFlac->memoryStream = memoryStream;

#ifndef DR_FLAC_NO_OGG
  if (pFlac->container == drflac_container_ogg) {
    drflac_oggbs *oggbs = (drflac_oggbs *)pFlac->_oggbs;
    oggbs->pUserData = &pFlac->memoryStream;
  } else
#endif
  {
    pFlac->bs.pUserData = &pFlac->memoryStream;
  }

  return pFlac;
}

drflac *drflac_open_memory_with_metadata(const void *data, size_t dataSize, drflac_meta_proc onMeta, void *pUserData)
{
  drflac__memory_stream memoryStream;
  memoryStream.data = (const unsigned char *)data;
  memoryStream.dataSize = dataSize;
  memoryStream.currentReadPos = 0;
  drflac *pFlac = drflac_open_with_metadata_private(drflac__on_read_memory, drflac__on_seek_memory, onMeta,
                                                    drflac_container_unknown, &memoryStream, pUserData);
  if (pFlac == NULL) {
    return NULL;
  }

  pFlac->memoryStream = memoryStream;

#ifndef DR_FLAC_NO_OGG
  if (pFlac->container == drflac_container_ogg) {
    drflac_oggbs *oggbs = (drflac_oggbs *)pFlac->_oggbs;
    oggbs->pUserData = &pFlac->memoryStream;
  } else
#endif
  {
    pFlac->bs.pUserData = &pFlac->memoryStream;
  }

  return pFlac;
}

drflac *drflac_open(drflac_read_proc onRead, drflac_seek_proc onSeek, void *pUserData)
{
  return drflac_open_with_metadata_private(onRead, onSeek, NULL, drflac_container_unknown, pUserData, pUserData);
}
drflac *drflac_open_relaxed(drflac_read_proc onRead, drflac_seek_proc onSeek, drflac_container container,
                            void *pUserData)
{
  return drflac_open_with_metadata_private(onRead, onSeek, NULL, container, pUserData, pUserData);
}

drflac *drflac_open_with_metadata(drflac_read_proc onRead, drflac_seek_proc onSeek, drflac_meta_proc onMeta,
                                  void *pUserData)
{
  return drflac_open_with_metadata_private(onRead, onSeek, onMeta, drflac_container_unknown, pUserData, pUserData);
}
drflac *drflac_open_with_metadata_relaxed(drflac_read_proc onRead, drflac_seek_proc onSeek, drflac_meta_proc onMeta,
                                          drflac_container container, void *pUserData)
{
  return drflac_open_with_metadata_private(onRead, onSeek, onMeta, container, pUserData, pUserData);
}

void drflac_close(drflac *pFlac)
{
  if (pFlac == NULL) {
    return;
  }

#ifndef DR_FLAC_NO_STDIO

  if (pFlac->bs.onRead == drflac__on_read_stdio) {
    fclose((FILE *)pFlac->bs.pUserData);
  }

#ifndef DR_FLAC_NO_OGG

  if (pFlac->container == drflac_container_ogg) {
    drflac_assert(pFlac->bs.onRead == drflac__on_read_ogg);
    drflac_oggbs *oggbs = (drflac_oggbs *)pFlac->_oggbs;
    if (oggbs->onRead == drflac__on_read_stdio) {
      fclose((FILE *)oggbs->pUserData);
    }
  }
#endif
#endif

  DRFLAC_FREE(pFlac);
}

drflac_uint64 drflac__read_s32__misaligned(drflac *pFlac, drflac_uint64 samplesToRead, drflac_int32 *bufferOut)
{
  unsigned int channelCount =
      drflac__get_channel_count_from_channel_assignment(pFlac->currentFrame.header.channelAssignment);

  drflac_assert(samplesToRead < channelCount);
  drflac_assert(pFlac->currentFrame.samplesRemaining > 0 && samplesToRead <= pFlac->currentFrame.samplesRemaining);

  drflac_uint64 samplesRead = 0;
  while (samplesToRead > 0) {
    drflac_uint64 totalSamplesInFrame = pFlac->currentFrame.header.blockSize * channelCount;
    drflac_uint64 samplesReadFromFrameSoFar = totalSamplesInFrame - pFlac->currentFrame.samplesRemaining;
    drflac_uint64 channelIndex = samplesReadFromFrameSoFar % channelCount;

    drflac_uint64 nextSampleInFrame = samplesReadFromFrameSoFar / channelCount;

    int decodedSample = 0;
    switch (pFlac->currentFrame.header.channelAssignment) {
    case DRFLAC_CHANNEL_ASSIGNMENT_LEFT_SIDE: {
      if (channelIndex == 0) {
        decodedSample = pFlac->currentFrame.subframes[channelIndex + 0].pDecodedSamples[nextSampleInFrame]
                        << pFlac->currentFrame.subframes[channelIndex + 0].wastedBitsPerSample;
      } else {
        int side = pFlac->currentFrame.subframes[channelIndex + 0].pDecodedSamples[nextSampleInFrame]
                   << pFlac->currentFrame.subframes[channelIndex + 0].wastedBitsPerSample;
        int left = pFlac->currentFrame.subframes[channelIndex - 1].pDecodedSamples[nextSampleInFrame]
                   << pFlac->currentFrame.subframes[channelIndex - 1].wastedBitsPerSample;
        decodedSample = left - side;
      }
    } break;

    case DRFLAC_CHANNEL_ASSIGNMENT_RIGHT_SIDE: {
      if (channelIndex == 0) {
        int side = pFlac->currentFrame.subframes[channelIndex + 0].pDecodedSamples[nextSampleInFrame]
                   << pFlac->currentFrame.subframes[channelIndex + 0].wastedBitsPerSample;
        int right = pFlac->currentFrame.subframes[channelIndex + 1].pDecodedSamples[nextSampleInFrame]
                    << pFlac->currentFrame.subframes[channelIndex + 1].wastedBitsPerSample;
        decodedSample = side + right;
      } else {
        decodedSample = pFlac->currentFrame.subframes[channelIndex + 0].pDecodedSamples[nextSampleInFrame]
                        << pFlac->currentFrame.subframes[channelIndex + 0].wastedBitsPerSample;
      }
    } break;

    case DRFLAC_CHANNEL_ASSIGNMENT_MID_SIDE: {
      int mid;
      int side;
      if (channelIndex == 0) {
        mid = pFlac->currentFrame.subframes[channelIndex + 0].pDecodedSamples[nextSampleInFrame]
              << pFlac->currentFrame.subframes[channelIndex + 0].wastedBitsPerSample;
        side = pFlac->currentFrame.subframes[channelIndex + 1].pDecodedSamples[nextSampleInFrame]
               << pFlac->currentFrame.subframes[channelIndex + 1].wastedBitsPerSample;

        mid = (((unsigned int)mid) << 1) | (side & 0x01);
        decodedSample = (mid + side) >> 1;
      } else {
        mid = pFlac->currentFrame.subframes[channelIndex - 1].pDecodedSamples[nextSampleInFrame]
              << pFlac->currentFrame.subframes[channelIndex - 1].wastedBitsPerSample;
        side = pFlac->currentFrame.subframes[channelIndex + 0].pDecodedSamples[nextSampleInFrame]
               << pFlac->currentFrame.subframes[channelIndex + 0].wastedBitsPerSample;

        mid = (((unsigned int)mid) << 1) | (side & 0x01);
        decodedSample = (mid - side) >> 1;
      }
    } break;

    case DRFLAC_CHANNEL_ASSIGNMENT_INDEPENDENT:
    default: {
      decodedSample = pFlac->currentFrame.subframes[channelIndex + 0].pDecodedSamples[nextSampleInFrame]
                      << pFlac->currentFrame.subframes[channelIndex + 0].wastedBitsPerSample;
    } break;
    }

    decodedSample <<= (32 - pFlac->bitsPerSample);

    if (bufferOut) {
      *bufferOut++ = decodedSample;
    }

    samplesRead += 1;
    pFlac->currentFrame.samplesRemaining -= 1;
    samplesToRead -= 1;
  }

  return samplesRead;
}

drflac_uint64 drflac__seek_forward_by_samples(drflac *pFlac, drflac_uint64 samplesToRead)
{
  drflac_uint64 samplesRead = 0;
  while (samplesToRead > 0) {
    if (pFlac->currentFrame.samplesRemaining == 0) {
      if (!drflac__read_and_decode_next_frame(pFlac)) {
        break;
      }
    } else {
      if (pFlac->currentFrame.samplesRemaining > samplesToRead) {
        samplesRead += samplesToRead;
        pFlac->currentFrame.samplesRemaining -= (drflac_uint32)samplesToRead;
        samplesToRead = 0;
      } else {
        samplesRead += pFlac->currentFrame.samplesRemaining;
        samplesToRead -= pFlac->currentFrame.samplesRemaining;
        pFlac->currentFrame.samplesRemaining = 0;
      }
    }
  }

  pFlac->currentSample += samplesRead;
  return samplesRead;
}

drflac_uint64 drflac_read_s32(drflac *pFlac, drflac_uint64 samplesToRead, drflac_int32 *bufferOut)
{

  if (pFlac == NULL || samplesToRead == 0) {
    return 0;
  }

  if (bufferOut == NULL) {
    return drflac__seek_forward_by_samples(pFlac, samplesToRead);
  }

  drflac_uint64 samplesRead = 0;
  while (samplesToRead > 0) {

    if (pFlac->currentFrame.samplesRemaining == 0) {
      if (!drflac__read_and_decode_next_frame(pFlac)) {
        break;
      }
    } else {

      unsigned int channelCount =
          drflac__get_channel_count_from_channel_assignment(pFlac->currentFrame.header.channelAssignment);
      drflac_uint64 totalSamplesInFrame = pFlac->currentFrame.header.blockSize * channelCount;
      drflac_uint64 samplesReadFromFrameSoFar = totalSamplesInFrame - pFlac->currentFrame.samplesRemaining;

      drflac_uint64 misalignedSampleCount = samplesReadFromFrameSoFar % channelCount;
      if (misalignedSampleCount > 0) {
        drflac_uint64 misalignedSamplesRead = drflac__read_s32__misaligned(pFlac, misalignedSampleCount, bufferOut);
        samplesRead += misalignedSamplesRead;
        samplesReadFromFrameSoFar += misalignedSamplesRead;
        bufferOut += misalignedSamplesRead;
        samplesToRead -= misalignedSamplesRead;
        pFlac->currentSample += misalignedSamplesRead;
      }

      drflac_uint64 alignedSampleCountPerChannel = samplesToRead / channelCount;
      if (alignedSampleCountPerChannel > pFlac->currentFrame.samplesRemaining / channelCount) {
        alignedSampleCountPerChannel = pFlac->currentFrame.samplesRemaining / channelCount;
      }

      drflac_uint64 firstAlignedSampleInFrame = samplesReadFromFrameSoFar / channelCount;
      unsigned int unusedBitsPerSample = 32 - pFlac->bitsPerSample;

      switch (pFlac->currentFrame.header.channelAssignment) {
      case DRFLAC_CHANNEL_ASSIGNMENT_LEFT_SIDE: {
        const drflac_int32 *pDecodedSamples0 =
            pFlac->currentFrame.subframes[0].pDecodedSamples + firstAlignedSampleInFrame;
        const drflac_int32 *pDecodedSamples1 =
            pFlac->currentFrame.subframes[1].pDecodedSamples + firstAlignedSampleInFrame;

        for (drflac_uint64 i = 0; i < alignedSampleCountPerChannel; ++i) {
          int left =
              pDecodedSamples0[i] << (unusedBitsPerSample + pFlac->currentFrame.subframes[0].wastedBitsPerSample);
          int side =
              pDecodedSamples1[i] << (unusedBitsPerSample + pFlac->currentFrame.subframes[1].wastedBitsPerSample);
          int right = left - side;

          bufferOut[i * 2 + 0] = left;
          bufferOut[i * 2 + 1] = right;
        }
      } break;

      case DRFLAC_CHANNEL_ASSIGNMENT_RIGHT_SIDE: {
        const drflac_int32 *pDecodedSamples0 =
            pFlac->currentFrame.subframes[0].pDecodedSamples + firstAlignedSampleInFrame;
        const drflac_int32 *pDecodedSamples1 =
            pFlac->currentFrame.subframes[1].pDecodedSamples + firstAlignedSampleInFrame;

        for (drflac_uint64 i = 0; i < alignedSampleCountPerChannel; ++i) {
          int side =
              pDecodedSamples0[i] << (unusedBitsPerSample + pFlac->currentFrame.subframes[0].wastedBitsPerSample);
          int right = pDecodedSamples1[i]
                      << (unusedBitsPerSample + pFlac->currentFrame.subframes[1].wastedBitsPerSample);
          int left = right + side;

          bufferOut[i * 2 + 0] = left;
          bufferOut[i * 2 + 1] = right;
        }
      } break;

      case DRFLAC_CHANNEL_ASSIGNMENT_MID_SIDE: {
        const drflac_int32 *pDecodedSamples0 =
            pFlac->currentFrame.subframes[0].pDecodedSamples + firstAlignedSampleInFrame;
        const drflac_int32 *pDecodedSamples1 =
            pFlac->currentFrame.subframes[1].pDecodedSamples + firstAlignedSampleInFrame;

        for (drflac_uint64 i = 0; i < alignedSampleCountPerChannel; ++i) {
          int mid = pDecodedSamples0[i] << pFlac->currentFrame.subframes[0].wastedBitsPerSample;
          int side = pDecodedSamples1[i] << pFlac->currentFrame.subframes[1].wastedBitsPerSample;

          mid = (((drflac_uint32)mid) << 1) | (side & 0x01);

          bufferOut[i * 2 + 0] = ((mid + side) >> 1) << (unusedBitsPerSample);
          bufferOut[i * 2 + 1] = ((mid - side) >> 1) << (unusedBitsPerSample);
        }
      } break;

      case DRFLAC_CHANNEL_ASSIGNMENT_INDEPENDENT:
      default: {
        if (pFlac->currentFrame.header.channelAssignment == 1) {

          const drflac_int32 *pDecodedSamples0 =
              pFlac->currentFrame.subframes[0].pDecodedSamples + firstAlignedSampleInFrame;
          const drflac_int32 *pDecodedSamples1 =
              pFlac->currentFrame.subframes[1].pDecodedSamples + firstAlignedSampleInFrame;

          for (drflac_uint64 i = 0; i < alignedSampleCountPerChannel; ++i) {
            bufferOut[i * 2 + 0] = pDecodedSamples0[i]
                                   << (unusedBitsPerSample + pFlac->currentFrame.subframes[0].wastedBitsPerSample);
            bufferOut[i * 2 + 1] = pDecodedSamples1[i]
                                   << (unusedBitsPerSample + pFlac->currentFrame.subframes[1].wastedBitsPerSample);
          }
        } else {

          for (drflac_uint64 i = 0; i < alignedSampleCountPerChannel; ++i) {
            for (unsigned int j = 0; j < channelCount; ++j) {
              bufferOut[(i * channelCount) + j] =
                  (pFlac->currentFrame.subframes[j].pDecodedSamples[firstAlignedSampleInFrame + i])
                  << (unusedBitsPerSample + pFlac->currentFrame.subframes[j].wastedBitsPerSample);
            }
          }
        }
      } break;
      }

      drflac_uint64 alignedSamplesRead = alignedSampleCountPerChannel * channelCount;
      samplesRead += alignedSamplesRead;
      samplesReadFromFrameSoFar += alignedSamplesRead;
      bufferOut += alignedSamplesRead;
      samplesToRead -= alignedSamplesRead;
      pFlac->currentSample += alignedSamplesRead;
      pFlac->currentFrame.samplesRemaining -= (unsigned int)alignedSamplesRead;

      if (samplesToRead > 0 && pFlac->currentFrame.samplesRemaining > 0) {
        drflac_uint64 excessSamplesRead = 0;
        if (samplesToRead < pFlac->currentFrame.samplesRemaining) {
          excessSamplesRead = drflac__read_s32__misaligned(pFlac, samplesToRead, bufferOut);
        } else {
          excessSamplesRead = drflac__read_s32__misaligned(pFlac, pFlac->currentFrame.samplesRemaining, bufferOut);
        }

        samplesRead += excessSamplesRead;
        samplesReadFromFrameSoFar += excessSamplesRead;
        bufferOut += excessSamplesRead;
        samplesToRead -= excessSamplesRead;
        pFlac->currentSample += excessSamplesRead;
      }
    }
  }

  return samplesRead;
}

drflac_uint64 drflac_read_s16(drflac *pFlac, drflac_uint64 samplesToRead, drflac_int16 *pBufferOut)
{

  drflac_uint64 totalSamplesRead = 0;

  while (samplesToRead > 0) {
    drflac_int32 samples32[4096];
    drflac_uint64 samplesJustRead = drflac_read_s32(pFlac, (samplesToRead > 4096) ? 4096 : samplesToRead, samples32);
    if (samplesJustRead == 0) {
      break;
    }

    for (drflac_uint64 i = 0; i < samplesJustRead; ++i) {
      pBufferOut[i] = (drflac_int16)(samples32[i] >> 16);
    }

    totalSamplesRead += samplesJustRead;
    samplesToRead -= samplesJustRead;
    pBufferOut += samplesJustRead;
  }

  return totalSamplesRead;
}

drflac_uint64 drflac_read_f32(drflac *pFlac, drflac_uint64 samplesToRead, float *pBufferOut)
{

  drflac_uint64 totalSamplesRead = 0;

  while (samplesToRead > 0) {
    drflac_int32 samples32[4096];
    drflac_uint64 samplesJustRead = drflac_read_s32(pFlac, (samplesToRead > 4096) ? 4096 : samplesToRead, samples32);
    if (samplesJustRead == 0) {
      break;
    }

    for (drflac_uint64 i = 0; i < samplesJustRead; ++i) {
      pBufferOut[i] = (float)(samples32[i] / 2147483648.0);
    }

    totalSamplesRead += samplesJustRead;
    samplesToRead -= samplesJustRead;
    pBufferOut += samplesJustRead;
  }

  return totalSamplesRead;
}

drflac_bool32 drflac_seek_to_sample(drflac *pFlac, drflac_uint64 sampleIndex)
{
  if (pFlac == NULL) {
    return DRFLAC_FALSE;
  }

  if (pFlac->firstFramePos == 0) {
    return DRFLAC_FALSE;
  }

  if (sampleIndex == 0) {
    pFlac->currentSample = 0;
    return drflac__seek_to_first_frame(pFlac);
  } else {
    drflac_bool32 wasSuccessful = DRFLAC_FALSE;

    if (sampleIndex >= pFlac->totalSampleCount) {
      sampleIndex = pFlac->totalSampleCount - 1;
    }

    if (sampleIndex > pFlac->currentSample) {

      drflac_uint32 offset = (drflac_uint32)(sampleIndex - pFlac->currentSample);
      if (pFlac->currentFrame.samplesRemaining > offset) {
        pFlac->currentFrame.samplesRemaining -= offset;
        pFlac->currentSample = sampleIndex;
        return DRFLAC_TRUE;
      }
    } else {

      drflac_uint32 offsetAbs = (drflac_uint32)(pFlac->currentSample - sampleIndex);
      drflac_uint32 currentFrameSampleCount =
          pFlac->currentFrame.header.blockSize
          * drflac__get_channel_count_from_channel_assignment(pFlac->currentFrame.header.channelAssignment);
      drflac_uint32 currentFrameSamplesConsumed =
          (drflac_uint32)(currentFrameSampleCount - pFlac->currentFrame.samplesRemaining);
      if (currentFrameSamplesConsumed > offsetAbs) {
        pFlac->currentFrame.samplesRemaining += offsetAbs;
        pFlac->currentSample = sampleIndex;
        return DRFLAC_TRUE;
      }
    }

#ifndef DR_FLAC_NO_OGG
    if (pFlac->container == drflac_container_ogg) {
      wasSuccessful = drflac_ogg__seek_to_sample(pFlac, sampleIndex);
    } else
#endif
    {

      wasSuccessful = drflac__seek_to_sample__seek_table(pFlac, sampleIndex);
      if (!wasSuccessful) {
        wasSuccessful = drflac__seek_to_sample__brute_force(pFlac, sampleIndex);
      }
    }

    pFlac->currentSample = sampleIndex;
    return wasSuccessful;
  }
}

#if defined(SIZE_MAX)
#define DRFLAC_SIZE_MAX SIZE_MAX
#else
#if defined(DRFLAC_64BIT)
#define DRFLAC_SIZE_MAX ((drflac_uint64)0xFFFFFFFFFFFFFFFF)
#else
#define DRFLAC_SIZE_MAX 0xFFFFFFFF
#endif
#endif

#define DRFLAC_DEFINE_FULL_DECODE_AND_CLOSE(extension, type)                                                           \
  static type *drflac__full_decode_and_close_##extension(                                                              \
      drflac *pFlac, unsigned int *channelsOut, unsigned int *sampleRateOut, drflac_uint64 *totalSampleCountOut)       \
  {                                                                                                                    \
    drflac_assert(pFlac != NULL);                                                                                      \
                                                                                                                       \
    type *pSampleData = NULL;                                                                                          \
    drflac_uint64 totalSampleCount = pFlac->totalSampleCount;                                                          \
                                                                                                                       \
    if (totalSampleCount == 0) {                                                                                       \
      type buffer[4096];                                                                                               \
                                                                                                                       \
      size_t sampleDataBufferSize = sizeof(buffer);                                                                    \
      pSampleData = (type *)DRFLAC_MALLOC(sampleDataBufferSize);                                                       \
      if (pSampleData == NULL) {                                                                                       \
        goto on_error;                                                                                                 \
      }                                                                                                                \
                                                                                                                       \
      drflac_uint64 samplesRead;                                                                                       \
      while ((samplesRead = (drflac_uint64)drflac_read_##extension(pFlac, sizeof(buffer) / sizeof(buffer[0]), buffer)) \
             > 0) {                                                                                                    \
        if (((totalSampleCount + samplesRead) * sizeof(type)) > sampleDataBufferSize) {                                \
          sampleDataBufferSize *= 2;                                                                                   \
          type *pNewSampleData = (type *)DRFLAC_REALLOC(pSampleData, sampleDataBufferSize);                            \
          if (pNewSampleData == NULL) {                                                                                \
            DRFLAC_FREE(pSampleData);                                                                                  \
            goto on_error;                                                                                             \
          }                                                                                                            \
                                                                                                                       \
          pSampleData = pNewSampleData;                                                                                \
        }                                                                                                              \
                                                                                                                       \
        drflac_copy_memory(pSampleData + totalSampleCount, buffer, (size_t)(samplesRead * sizeof(type)));              \
        totalSampleCount += samplesRead;                                                                               \
      }                                                                                                                \
                                                                                                                       \
      drflac_zero_memory(pSampleData + totalSampleCount,                                                               \
                         (size_t)(sampleDataBufferSize - totalSampleCount * sizeof(type)));                            \
    } else {                                                                                                           \
      drflac_uint64 dataSize = totalSampleCount * sizeof(type);                                                        \
      if (dataSize > DRFLAC_SIZE_MAX) {                                                                                \
        goto on_error;                                                                                                 \
      }                                                                                                                \
                                                                                                                       \
      pSampleData = (type *)DRFLAC_MALLOC((size_t)dataSize);                                                           \
      if (pSampleData == NULL) {                                                                                       \
        goto on_error;                                                                                                 \
      }                                                                                                                \
                                                                                                                       \
      totalSampleCount = drflac_read_##extension(pFlac, pFlac->totalSampleCount, pSampleData);                         \
    }                                                                                                                  \
                                                                                                                       \
    if (sampleRateOut)                                                                                                 \
      *sampleRateOut = pFlac->sampleRate;                                                                              \
    if (channelsOut)                                                                                                   \
      *channelsOut = pFlac->channels;                                                                                  \
    if (totalSampleCountOut)                                                                                           \
      *totalSampleCountOut = totalSampleCount;                                                                         \
                                                                                                                       \
    drflac_close(pFlac);                                                                                               \
    return pSampleData;                                                                                                \
                                                                                                                       \
  on_error:                                                                                                            \
    drflac_close(pFlac);                                                                                               \
    return NULL;                                                                                                       \
  }

DRFLAC_DEFINE_FULL_DECODE_AND_CLOSE(s32, drflac_int32)
DRFLAC_DEFINE_FULL_DECODE_AND_CLOSE(s16, drflac_int16)
DRFLAC_DEFINE_FULL_DECODE_AND_CLOSE(f32, float)

drflac_int32 *drflac_open_and_decode_s32(drflac_read_proc onRead, drflac_seek_proc onSeek, void *pUserData,
                                         unsigned int *channels, unsigned int *sampleRate,
                                         drflac_uint64 *totalSampleCount)
{

  if (sampleRate)
    *sampleRate = 0;
  if (channels)
    *channels = 0;
  if (totalSampleCount)
    *totalSampleCount = 0;

  drflac *pFlac = drflac_open(onRead, onSeek, pUserData);
  if (pFlac == NULL) {
    return NULL;
  }

  return drflac__full_decode_and_close_s32(pFlac, channels, sampleRate, totalSampleCount);
}

drflac_int16 *drflac_open_and_decode_s16(drflac_read_proc onRead, drflac_seek_proc onSeek, void *pUserData,
                                         unsigned int *channels, unsigned int *sampleRate,
                                         drflac_uint64 *totalSampleCount)
{

  if (sampleRate)
    *sampleRate = 0;
  if (channels)
    *channels = 0;
  if (totalSampleCount)
    *totalSampleCount = 0;

  drflac *pFlac = drflac_open(onRead, onSeek, pUserData);
  if (pFlac == NULL) {
    return NULL;
  }

  return drflac__full_decode_and_close_s16(pFlac, channels, sampleRate, totalSampleCount);
}

float *drflac_open_and_decode_f32(drflac_read_proc onRead, drflac_seek_proc onSeek, void *pUserData,
                                  unsigned int *channels, unsigned int *sampleRate, drflac_uint64 *totalSampleCount)
{

  if (sampleRate)
    *sampleRate = 0;
  if (channels)
    *channels = 0;
  if (totalSampleCount)
    *totalSampleCount = 0;

  drflac *pFlac = drflac_open(onRead, onSeek, pUserData);
  if (pFlac == NULL) {
    return NULL;
  }

  return drflac__full_decode_and_close_f32(pFlac, channels, sampleRate, totalSampleCount);
}

#ifndef DR_FLAC_NO_STDIO
drflac_int32 *drflac_open_and_decode_file_s32(const char *filename, unsigned int *channels, unsigned int *sampleRate,
                                              drflac_uint64 *totalSampleCount)
{
  if (sampleRate)
    *sampleRate = 0;
  if (channels)
    *channels = 0;
  if (totalSampleCount)
    *totalSampleCount = 0;

  drflac *pFlac = drflac_open_file(filename);
  if (pFlac == NULL) {
    return NULL;
  }

  return drflac__full_decode_and_close_s32(pFlac, channels, sampleRate, totalSampleCount);
}

drflac_int16 *drflac_open_and_decode_file_s16(const char *filename, unsigned int *channels, unsigned int *sampleRate,
                                              drflac_uint64 *totalSampleCount)
{
  if (sampleRate)
    *sampleRate = 0;
  if (channels)
    *channels = 0;
  if (totalSampleCount)
    *totalSampleCount = 0;

  drflac *pFlac = drflac_open_file(filename);
  if (pFlac == NULL) {
    return NULL;
  }

  return drflac__full_decode_and_close_s16(pFlac, channels, sampleRate, totalSampleCount);
}

float *drflac_open_and_decode_file_f32(const char *filename, unsigned int *channels, unsigned int *sampleRate,
                                       drflac_uint64 *totalSampleCount)
{
  if (sampleRate)
    *sampleRate = 0;
  if (channels)
    *channels = 0;
  if (totalSampleCount)
    *totalSampleCount = 0;

  drflac *pFlac = drflac_open_file(filename);
  if (pFlac == NULL) {
    return NULL;
  }

  return drflac__full_decode_and_close_f32(pFlac, channels, sampleRate, totalSampleCount);
}
#endif

drflac_int32 *drflac_open_and_decode_memory_s32(const void *data, size_t dataSize, unsigned int *channels,
                                                unsigned int *sampleRate, drflac_uint64 *totalSampleCount)
{
  if (sampleRate)
    *sampleRate = 0;
  if (channels)
    *channels = 0;
  if (totalSampleCount)
    *totalSampleCount = 0;

  drflac *pFlac = drflac_open_memory(data, dataSize);
  if (pFlac == NULL) {
    return NULL;
  }

  return drflac__full_decode_and_close_s32(pFlac, channels, sampleRate, totalSampleCount);
}

drflac_int16 *drflac_open_and_decode_memory_s16(const void *data, size_t dataSize, unsigned int *channels,
                                                unsigned int *sampleRate, drflac_uint64 *totalSampleCount)
{
  if (sampleRate)
    *sampleRate = 0;
  if (channels)
    *channels = 0;
  if (totalSampleCount)
    *totalSampleCount = 0;

  drflac *pFlac = drflac_open_memory(data, dataSize);
  if (pFlac == NULL) {
    return NULL;
  }

  return drflac__full_decode_and_close_s16(pFlac, channels, sampleRate, totalSampleCount);
}

float *drflac_open_and_decode_memory_f32(const void *data, size_t dataSize, unsigned int *channels,
                                         unsigned int *sampleRate, drflac_uint64 *totalSampleCount)
{
  if (sampleRate)
    *sampleRate = 0;
  if (channels)
    *channels = 0;
  if (totalSampleCount)
    *totalSampleCount = 0;

  drflac *pFlac = drflac_open_memory(data, dataSize);
  if (pFlac == NULL) {
    return NULL;
  }

  return drflac__full_decode_and_close_f32(pFlac, channels, sampleRate, totalSampleCount);
}

void drflac_free(void *pSampleDataReturnedByOpenAndDecode)
{
  DRFLAC_FREE(pSampleDataReturnedByOpenAndDecode);
}

void drflac_init_vorbis_comment_iterator(drflac_vorbis_comment_iterator *pIter, drflac_uint32 commentCount,
                                         const void *pComments)
{
  if (pIter == NULL) {
    return;
  }

  pIter->countRemaining = commentCount;
  pIter->pRunningData = (const char *)pComments;
}

const char *drflac_next_vorbis_comment(drflac_vorbis_comment_iterator *pIter, drflac_uint32 *pCommentLengthOut)
{

  if (pCommentLengthOut)
    *pCommentLengthOut = 0;

  if (pIter == NULL || pIter->countRemaining == 0 || pIter->pRunningData == NULL) {
    return NULL;
  }

  drflac_uint32 length = drflac__le2host_32(*(const drflac_uint32 *)pIter->pRunningData);
  pIter->pRunningData += 4;

  const char *pComment = pIter->pRunningData;
  pIter->pRunningData += length;
  pIter->countRemaining -= 1;

  if (pCommentLengthOut)
    *pCommentLengthOut = length;
  return pComment;
}

void drflac_init_cuesheet_track_iterator(drflac_cuesheet_track_iterator *pIter, drflac_uint32 trackCount,
                                         const void *pTrackData)
{
  if (pIter == NULL) {
    return;
  }

  pIter->countRemaining = trackCount;
  pIter->pRunningData = (const char *)pTrackData;
}

drflac_bool32 drflac_next_cuesheet_track(drflac_cuesheet_track_iterator *pIter, drflac_cuesheet_track *pCuesheetTrack)
{
  if (pIter == NULL || pIter->countRemaining == 0 || pIter->pRunningData == NULL) {
    return DRFLAC_FALSE;
  }

  drflac_cuesheet_track cuesheetTrack;

  const char *pRunningData = pIter->pRunningData;

  drflac_uint64 offsetHi = drflac__be2host_32(*(const drflac_uint32 *)pRunningData);
  pRunningData += 4;
  drflac_uint64 offsetLo = drflac__be2host_32(*(const drflac_uint32 *)pRunningData);
  pRunningData += 4;
  cuesheetTrack.offset = offsetLo | (offsetHi << 32);
  cuesheetTrack.trackNumber = pRunningData[0];
  pRunningData += 1;
  drflac_copy_memory(cuesheetTrack.ISRC, pRunningData, sizeof(cuesheetTrack.ISRC));
  pRunningData += 12;
  cuesheetTrack.isAudio = (pRunningData[0] & 0x80) != 0;
  cuesheetTrack.preEmphasis = (pRunningData[0] & 0x40) != 0;
  pRunningData += 14;
  cuesheetTrack.indexCount = pRunningData[0];
  pRunningData += 1;
  cuesheetTrack.pIndexPoints = (const drflac_cuesheet_track_index *)pRunningData;
  pRunningData += cuesheetTrack.indexCount * sizeof(drflac_cuesheet_track_index);

  pIter->pRunningData = pRunningData;
  pIter->countRemaining -= 1;

  if (pCuesheetTrack)
    *pCuesheetTrack = cuesheetTrack;
  return DRFLAC_TRUE;
}
#endif
