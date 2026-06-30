#ifndef HASHER_HPP
#define HASHER_HPP
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>

namespace alisov
{
  inline uint32_t leftRotate(uint32_t value, unsigned int shift)
  {
    return (value << shift) | (value >> (32 - shift));
  }

  inline size_t hashBytes(const uint8_t *message, size_t len)
  {
    uint32_t h0 = 0x67452301, h1 = 0xEFCDAB89, h2 = 0x98BADCFE, h3 = 0x10325476, h4 = 0xC3D2E1F0;
    size_t new_len = ((len + 8) / 64 + 1) * 64;
    uint8_t *msg = new uint8_t[new_len]();
    std::memcpy(msg, message, len);
    msg[len] = 0x80;
    uint64_t bits_len = len * 8;
    for (int i = 0; i < 8; ++i)
      msg[new_len - 1 - i] = (bits_len >> (i * 8)) & 0xFF;

    for (size_t chunk = 0; chunk < new_len; chunk += 64) {
      uint32_t w[80];
      for (int i = 0; i < 16; ++i) {
        w[i] = (msg[chunk + i * 4] << 24) | (msg[chunk + i * 4 + 1] << 16) | (msg[chunk + i * 4 + 2] << 8)
               | msg[chunk + i * 4 + 3];
      }
      for (int i = 16; i < 80; ++i)
        w[i] = leftRotate(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
      uint32_t a = h0, b = h1, c = h2, d = h3, e = h4;
      for (int i = 0; i < 80; ++i) {
        uint32_t f, k;
        if (i < 20) {
          f = (b & c) | ((~b) & d);
          k = 0x5A827999;
        } else if (i < 40) {
          f = b ^ c ^ d;
          k = 0x6ED9EBA1;
        } else if (i < 60) {
          f = (b & c) | (b & d) | (c & d);
          k = 0x8F1BBCDC;
        } else {
          f = b ^ c ^ d;
          k = 0xCA62C1D6;
        }
        uint32_t temp = leftRotate(a, 5) + f + e + k + w[i];
        e = d;
        d = c;
        c = leftRotate(b, 30);
        b = a;
        a = temp;
      }
      h0 += a;
      h1 += b;
      h2 += c;
      h3 += d;
      h4 += e;
    }
    delete[] msg;
    return static_cast< size_t >(h0 ^ h1 ^ h2 ^ h3 ^ h4);
  }

  template < class T >
  struct SHA1
  {
    size_t operator()(const T &key) const
    {
      return hashBytes(reinterpret_cast< const uint8_t * >(&key), sizeof(T));
    }
  };
  template <>
  struct SHA1< std::string >
  {
    size_t operator()(const std::string &key) const
    {
      return hashBytes(reinterpret_cast< const uint8_t * >(key.data()), key.size());
    }
  };

  template < class T1, class T2 >
  struct SHA1< std::pair< T1, T2 > >
  {
    size_t operator()(const std::pair< T1, T2 > &key) const
    {
      size_t h1 = SHA1< T1 >{}(key.first);
      size_t h2 = SHA1< T2 >{}(key.second);
      return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
  };
}
#endif
