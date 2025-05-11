#pragma once

#include <cstdio>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstdint>
#include <array>

namespace Crypto
{
    namespace AES
    {
        enum class AESKeyLength { AES_128, AES_192, AES_256 };

        class AES {
        private:
            static constexpr unsigned int Nb = 4;
            static constexpr unsigned int blockBytesLen = 4 * Nb * sizeof(unsigned char);

            unsigned int Nk;
            unsigned int Nr;

            void SubBytes(unsigned char state[4][Nb]);

            void ShiftRow(unsigned char state[4][Nb], unsigned int i,
                unsigned int n);  // shift row i on n positions

            void ShiftRows(unsigned char state[4][Nb]);

            unsigned char xtime(unsigned char b);  // multiply on x

            void MixColumns(unsigned char state[4][Nb]);

            void AddRoundKey(unsigned char state[4][Nb], unsigned char* key);

            void SubWord(unsigned char* a);

            void RotWord(unsigned char* a);

            void XorWords(unsigned char* a, unsigned char* b, unsigned char* c);

            void Rcon(unsigned char* a, unsigned int n);

            void InvSubBytes(unsigned char state[4][Nb]);

            void InvMixColumns(unsigned char state[4][Nb]);

            void InvShiftRows(unsigned char state[4][Nb]);

            void CheckLength(unsigned int len);

            void XorBlocks(const unsigned char* a, const unsigned char* b,
                unsigned char* c, unsigned int len);

            std::vector<unsigned char> ArrayToVector(unsigned char* a, unsigned int len);

            unsigned char* VectorToArray(std::vector<unsigned char>& a);

        public:
            explicit AES(const AESKeyLength keyLength = AESKeyLength::AES_256);

            unsigned char* EncryptECB(const unsigned char in[], unsigned int inLen,
                const unsigned char key[]);

            unsigned char* DecryptECB(const unsigned char in[], unsigned int inLen,
                const unsigned char key[]);

            unsigned char* EncryptCBC(const unsigned char in[], unsigned int inLen,
                const unsigned char key[], const unsigned char* iv);

            unsigned char* DecryptCBC(const unsigned char in[], unsigned int inLen,
                const unsigned char key[], const unsigned char* iv);

            unsigned char* EncryptCFB(const unsigned char in[], unsigned int inLen,
                const unsigned char key[], const unsigned char* iv);

            unsigned char* DecryptCFB(const unsigned char in[], unsigned int inLen,
                const unsigned char key[], const unsigned char* iv);

            std::vector<unsigned char> EncryptECB(std::vector<unsigned char> in,
                std::vector<unsigned char> key);

            std::vector<unsigned char> DecryptECB(std::vector<unsigned char> in,
                std::vector<unsigned char> key);

            std::vector<unsigned char> EncryptCBC(std::vector<unsigned char> in,
                std::vector<unsigned char> key,
                std::vector<unsigned char> iv);

            std::vector<unsigned char> DecryptCBC(std::vector<unsigned char> in,
                std::vector<unsigned char> key,
                std::vector<unsigned char> iv);

            std::vector<unsigned char> EncryptCFB(std::vector<unsigned char> in,
                std::vector<unsigned char> key,
                std::vector<unsigned char> iv);

            std::vector<unsigned char> DecryptCFB(std::vector<unsigned char> in,
                std::vector<unsigned char> key,
                std::vector<unsigned char> iv);

            void KeyExpansion(const unsigned char key[], unsigned char w[]);

            void EncryptBlock(const unsigned char in[], unsigned char out[],
                unsigned char* roundKeys);

            void DecryptBlock(const unsigned char in[], unsigned char out[],
                unsigned char* roundKeys);

            void printHexArray(unsigned char a[], unsigned int n);

            void printHexVector(std::vector<unsigned char> a);
        };
    }

    namespace ECDH
    {
        const static size_t PRIVATE_KEY_SIZE = 72;
        const static size_t PUBLIC_KEY_SIZE = 144;

        int ecdh_generate_keys(uint8_t* public_key, uint8_t* private_key);
        /* input: own private key + other party's public key, output: shared secret */
        int ecdh_shared_secret(const uint8_t* private_key, const uint8_t* others_pub, uint8_t* output);
    }

    namespace SHA256
    {
        class SHA256 {

        public:
            SHA256();
            void update(const uint8_t* data, size_t length);
            void update(const std::string& data);
            std::array<uint8_t, 32> digest();

            static std::string toString(const std::array<uint8_t, 32>& digest);

        private:
            uint8_t  m_data[64];
            uint32_t m_blocklen;
            uint64_t m_bitlen;
            uint32_t m_state[8]; //A, B, C, D, E, F, G, H

            static constexpr std::array<uint32_t, 64> K = {
                0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,
                0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
                0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,
                0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
                0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,
                0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
                0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,
                0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
                0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,
                0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
                0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,
                0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
                0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,
                0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
                0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,
                0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
            };

            static uint32_t rotr(uint32_t x, uint32_t n);
            static uint32_t choose(uint32_t e, uint32_t f, uint32_t g);
            static uint32_t majority(uint32_t a, uint32_t b, uint32_t c);
            static uint32_t sig0(uint32_t x);
            static uint32_t sig1(uint32_t x);
            void transform();
            void pad();
            void revert(std::array<uint8_t, 32>& hash);
        };
    }

    namespace Random{
        int generate(uint8_t* data, size_t length);
    }
}
