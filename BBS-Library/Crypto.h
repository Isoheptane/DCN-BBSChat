#pragma once

#include <cstdio>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstdint>

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

            void KeyExpansion(const unsigned char key[], unsigned char w[]);

            void EncryptBlock(const unsigned char in[], unsigned char out[],
                unsigned char* roundKeys);

            void DecryptBlock(const unsigned char in[], unsigned char out[],
                unsigned char* roundKeys);

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
    }
}
