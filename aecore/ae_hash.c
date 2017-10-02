/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
--------------------------------------------------------------------------------
--- stb.h v2.27 - Sean's Tool Box -- public domain -- http://nothings.org/stb.h
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

u32 ae_hash_combine(u32 lhs, u32 rhs)
{
   lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
   return lhs; // boost::hash_combine implementation
}

// these string hashing functions could probably get a good speedup from
// SSE2, but they're often called on really small strings (<= 8 chars),
// to the point where even this unrolling hurts a bit on some workloads.
// TODO: could we hash 32 bits at a time in the same way directly here?
// TODO: we throw away valuable string length information! write it out!

u32 ae_hash_str(char *str) // strhash
{
   u32 hash = 0; // speedups via charles bloom's blog
   ae_assert(str, "null input for string hash func");

   while (!zero_byte_in_u32(*((u32*)str))) // unsafe?
   {
      hash = ae_barrel_rotate_left(hash, 7) + str[0];
      hash = ae_barrel_rotate_left(hash, 7) + str[1];
      hash = ae_barrel_rotate_left(hash, 7) + str[2];
      hash = ae_barrel_rotate_left(hash, 7) + str[3];

      str += sizeof(u32);
   }

   while (*str)
   {
      hash = ae_barrel_rotate_left(hash, 7) + *str++;
   }

   return hash + (hash >> 16);
}

u32 ae_hash_len(char *str, int len) // strnhash
{
   u32 hash = 0; // speedups via charles bloom's blog
   ae_assert(str, "null input for string hash func");

   // take the faster unrolled path for non-substrings
   if (!str[len]) return ae_hash_str(str);

   while (len-- > 0 && *str)
   {
      hash = ae_barrel_rotate_left(hash, 7) + *str++;
   }

   return hash + (hash >> 16);
}

void ae_hash64_str(char* str, u32 hash[2])
{
   u32 hash1 = 0x3141592c;
   u32 hash2 = 0x77f044ed;

   while (*str)
   {
      hash1 = (hash1 <<  7) + (hash1 >> 25) + *str;
      hash2 = (hash2 << 11) + (hash2 >> 21) + *str;
      ++str;
   }

   hash[0] = hash1 + (hash2 >> 16);
   hash[1] = hash2 + (hash1 >> 16);
}

void ae_hash64_len(char* str, int len, u32 hash[2])
{
   u32 hash1 = 0x3141592c;
   u32 hash2 = 0x77f044ed;

   while (len-- > 0 && *str)
   {
      hash1 = (hash1 <<  7) + (hash1 >> 25) + *str;
      hash2 = (hash2 << 11) + (hash2 >> 21) + *str;
      ++str;
   }

   hash[0] = hash1 + (hash2 >> 16);
   hash[1] = hash2 + (hash1 >> 16);
}

// TODO: on 64-bit platforms, we should probably hash the entire pointer.
// when rehash_new and hash_ptr implementations are swapped after testing,
// we can hash both halves of the ptr and use hash_combine on the hashes.
// note that we'll also need to check for a 64-bit CPU at compile time.

u32 ae_hash_ptr(void *p)
{
   // grab lower 32 bits of the pointer for mixing
   u32 x = (u32)(size_t)p;

   // typically lacking in low bits and high bits
   x = ae_rehash_old(x);
   x += x << 16;

   // do the rest of pearson's shuffling algorithm
   x ^= x <<  3;
   x += x >>  5;
   x ^= x <<  2;
   x += x >> 15;
   x ^= x << 10;

   return ae_rehash_old(x);
}

u32 ae_rehash_new(u32 v)
{
   /* TODO: swapping this implementation with hash_ptr
    * would make a lot more sense on 64-bit platforms.
    */
   return ae_hash_ptr((void *)(size_t)v);
}

u32 ae_hash_num(u32 hash)
{
   // original stb_hash_number (12 operations)
   #if 0

      hash ^= hash <<  3; hash += hash >>  5;
      hash ^= hash <<  4; hash += hash >> 17;
      hash ^= hash << 25; hash += hash >>  6;

   // tom wang's hash algorithm (9 operations)
   #else

      hash  = (hash ^ 61) ^ (hash >> 16);
      hash *= 9;
      hash  =        hash ^ (hash >>  4);
      hash *= 0x27d4eb2d;
      hash  =        hash ^ (hash >> 15);

   #endif
   return hash;
}

/* ===== [ block hashing ] ================================================== */

#if 0

// TODO: random hash functions, where the initial hash value is "salted"
// by a pseudo-random value to protect against denial-of-service attacks.
// some commonly-used algorithms are SipHash, CityHash, and MurmerHash.

static u32 ae_hash_rotating(const void* const p, u32 size)
{
   const u8* const data = (const u8* const)p;
   u32 i = 0, h = 0;

   while (i < size)
   {
      h = (h << 4) ^ (h >> 28) ^ (u32)data[i++];
   }
   return h;
}

static u32 ae_hash_sax(const void* const p, u32 size)
{
   const u8* const data = (const u8* const)p;
   u32 i = 0, h = 0;

   while (i < size)
   {
      h ^= (h << 5) + (h >> 2) + (u32)data[i++];
   }
   return h;
}

static u32 ae_hash_sdbm(const void* const p, u32 size)
{
   const u8* const data = (const u8* const)p;
   u32 i = 0, h = 5381;

   while (i < size)
   {
      h = (h << 16) + (h << 6) - h + (u32)data[i++];
   }
   return h;
}

static u32 ae_hash_djb(const void* const p, u32 size)
{
   const u8* const data = (const u8* const)p;
   u32 i = 0, h = 0;

   while (i < size)
   {
   // h = 33 * h + (u32)data[i++];
      h = 33 * h ^ (u32)data[i++];
   }
   return h;
}

static u32 ae_hash_djb2(const void* const p, u32 size)
{
   const u8* const data = (const u8* const)p;
   u32 i = 0, h = 5381;

   while (i < size)
   {
      h = ((h << 5) + h) + (u32)data[i++];
   }
   return h;
}

static u32 ae_hash_fnv(const void* const p, u32 size)
{
   const u8* const data = (const u8* const)p;
   u32 i = 0, h = 2166136261;

   while (i < size)
   {
      h = (h * 16777619) ^ (u32)data[i++];
   }
   return h;
}

static u32 ae_hash_elf(const void* const p, u32 size)
{
   const u8* const data = (const u8* const)p;
   u32 i = 0, h = 0, g;

   while (i < size)
   {
      h = (h << 4) + (u32)data[i++];
      g = h & 0xf0000000L;

      if (g != 0) { h ^= g >> 24; }
      h &= ~g;
   }

   return h;
}

static u32 ae_hash_oat(const void* const p, u32 size)
{
   const u8* const data = (const u8* const)p;
   u32 i = 0, h = 0;

   while (i < size)
   {
      h += (u32)data[i++];
      h += (h << 10);
      h ^= (h >>  6);
   }

   h += (h <<  3);
   h ^= (h >> 11);
   h += (h << 15);

   return h;
}

static u32 ae_hash_lose(const void* const p, u32 size)
{
   const u8* const data = (const u8* const)p;
   u32 i = 0, h = 5381; // crappy test hash!

   while (i < size)
   {
   // h += (u32)data[i++];
   // h *= (u32)data[i++];
      h ^= (u32)data[i++];
   }
   return h;
}

static u32 ae_hash_zero(const void* const p, u32 size)
{
   return 0;
}

#endif

static u32 ae_hash_hsieh(const void* const p, u32 size)
{
   #define ae_get16_slow(p) ((p)[0] + ((p)[1] << 8))

   #if defined(_MSC_VER)
      #define ae_get16(p) (*((u16 *) (p)))
   #else
      #define ae_get16(p) ae_get16_slow(p)
   #endif

   unsigned char * q = (unsigned char *)p;
   unsigned int hash = size;

   if (size <= 0 || q == NULL) { return 0; }
   if (( (size_t)q & 1 ) == 0) // main loop
   {
      for (; size > 3; size -= 4) // even
      {
         unsigned int val;

         hash += (ae_get16(q));
         val   = (ae_get16(q + 2) << 11);
         hash  = (hash << 16) ^ hash ^ val;
         q    += 4;
         hash += hash >> 11;
      }
   }
   else
   {
      for (; size > 3; size -= 4) // odd
      {
         unsigned int val;

         hash += (ae_get16_slow(q));
         val   = (ae_get16_slow(q + 2) << 11);
         hash  = (hash << 16) ^ hash ^ val;
         q    += 4;
         hash += hash >> 11;
      }
   }

   switch (size) // handle the end cases
   {
      case 3: hash += ae_get16_slow(q);
              hash ^= hash << 16;
              hash ^= q[2] << 18;
              hash += hash >> 11;
              break;
      case 2: hash += ae_get16_slow(q);
              hash ^= hash << 11;
              hash += hash >> 17;
              break;
      case 1: hash += q[0];
              hash ^= hash << 10;
              hash += hash >> 1;
              break;
      case 0: break;
   }

   // force "avalanching" of final 127 bits
   hash ^= hash <<  3;
   hash += hash >>  5;
   hash ^= hash <<  4;
   hash += hash >> 17;
   hash ^= hash << 25;
   hash += hash >>  6;

   return hash;
}

u32 ae_hash_mem(void* p, int len) // memhash
{
   /* TODO: this is the implementation we chose, because it's what came with
    * stb.h. the other functions may be better; test them with a large vertex
    * welding workload or something to measure both speed and collision rate.
    */
   return ae_hash_hsieh(p, len);
}

/*
================================================================================
 * ~~ [ streaming block checksums ] ~~ *
--------------------------------------------------------------------------------
NOTE: this is compatible with zlib (i believe SSE4.2's CRC32C would break that)
TODO: crc32c function that uses intel's polynomial + SSE4 hardware instructions
--------------------------------------------------------------------------------
*/

static u32 ae_crc_table[256];

u32 ae_crc32_block(u32 crc, const void* const buffer, const size_t len)
{
   u8* bufptr = (u8*)buffer;
   size_t i = 0;

   ae_assert(ae_crc_table[1], "empty crc table (call ae_init)");
   for (crc = ~crc; i < len; ++i)
   {
      crc = (crc >> 8) ^ ae_crc_table[bufptr[i] ^ (crc & 0xff)];
   }

   return ~crc;
}

u32 ae_crc32(const void* const buffer, const size_t len)
{
   return ae_crc32_block(0, buffer, len);
}

u32 ae_adler32_block(u32 adler32, const void* const buffer, size_t len)
{
   const unsigned long ADLER_MOD = 65521;
   u8* bufptr = (u8*)buffer;

   unsigned long s1 = adler32 & 0xffff, s2 = adler32 >> 16;
   unsigned long i, blocklen = len % 5552;

   while (len)
   {
      for (i = 0; i + 7 < blocklen; i += 8)
      {
         s1 += bufptr[0], s2 += s1;
         s1 += bufptr[1], s2 += s1;
         s1 += bufptr[2], s2 += s1;
         s1 += bufptr[3], s2 += s1;
         s1 += bufptr[4], s2 += s1;
         s1 += bufptr[5], s2 += s1;
         s1 += bufptr[6], s2 += s1;
         s1 += bufptr[7], s2 += s1;

         bufptr += 8;
      }

      for (; i < blocklen; ++i)
      {
         s1 += *bufptr++, s2 += s1;
      }

      s1 %= ADLER_MOD,
      s2 %= ADLER_MOD;

      len -= blocklen;
      blocklen = 5552;
   }

   return (s2 << 16) + s1;
}

u32 ae_adler32(const void* const buffer, size_t len)
{
   return ae_adler32_block(1, buffer, len);
}

/*
================================================================================
 * ~~ [ initialization ] ~~ *
--------------------------------------------------------------------------------
*/

AE_STRMAP_APPLY_ARGS(AE_HASH_IMPL);
AE_PTRMAP_APPLY_ARGS(AE_HASH_IMPL);

void ae_hash_init(int argc, char** argv)
{
   u32 i, j, s;

   for (i = 0; i < 256; i++) // build the crc table
   {
      for (s = i, j = 0; j < 8; ++j)
      {
         s = (s >> 1) ^ (s & 1 ? 0xedb88320 : 0);
      }
      ae_crc_table[i] = s;
   }
}

void ae_hash_quit(void)
{
   //
}
