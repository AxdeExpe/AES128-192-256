#include "aes.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

static const uint8_t sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b,
    0xfe, 0xd7, 0xab, 0x76, 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
    0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, 0xb7, 0xfd, 0x93, 0x26,
    0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2,
    0xeb, 0x27, 0xb2, 0x75, 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
    0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, 0x53, 0xd1, 0x00, 0xed,
    0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f,
    0x50, 0x3c, 0x9f, 0xa8, 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
    0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, 0xcd, 0x0c, 0x13, 0xec,
    0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14,
    0xde, 0x5e, 0x0b, 0xdb, 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
    0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, 0xe7, 0xc8, 0x37, 0x6d,
    0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f,
    0x4b, 0xbd, 0x8b, 0x8a, 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
    0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, 0xe1, 0xf8, 0x98, 0x11,
    0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f,
    0xb0, 0x54, 0xbb, 0x16 };

static const uint8_t rsbox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e,
    0x81, 0xf3, 0xd7, 0xfb, 0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87,
    0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, 0x54, 0x7b, 0x94, 0x32,
    0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49,
    0x6d, 0x8b, 0xd1, 0x25, 0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16,
    0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92, 0x6c, 0x70, 0x48, 0x50,
    0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05,
    0xb8, 0xb3, 0x45, 0x06, 0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02,
    0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b, 0x3a, 0x91, 0x11, 0x41,
    0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8,
    0x1c, 0x75, 0xdf, 0x6e, 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89,
    0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b, 0xfc, 0x56, 0x3e, 0x4b,
    0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59,
    0x27, 0x80, 0xec, 0x5f, 0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d,
    0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef, 0xa0, 0xe0, 0x3b, 0x4d,
    0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63,
    0x55, 0x21, 0x0c, 0x7d };

// Gibt die Anzahl der Runden zur Schlüssellänge in Bit zurück, bei ungültiger Schlüssellänge wird -1 zurückgegeben
unsigned int numRounds(unsigned int keySize){
    switch (keySize) {
        case 128: return 0b1010;
        case 192: return 0b1100;
        case 256: return 0b1110;
        default: return -1;
    }
}  

// Gibt die Anzahl der Schlüsselwörter zur Schlüssellänge in Bit zurück, bei ungültiger Schlüssellänge wird -1 zurückgegeben
unsigned int numKeyWords(unsigned int keySize){
    switch (keySize) {
        case 128: return 0b0100;
        case 192: return 0b0110;
        case 256: return 0b1000;
        default: return -1;
    }
}

// Gibt die Ergebnisse der SBox für die Eingabe zurück. Die S-Box ist in der Datei aes.c bereits bereitgestellt.
u_int8_t getSBoxValue(u_int8_t num){

    if (num < 0 || num > sizeof(sbox)) return -1;

    return sbox[num];
}

// Gibt die Ergebnisse der inversen SBox für die Eingabe zurück. Die inverse S-Box ist in der Datei aes.c bereits bereitgestellt.
u_int8_t getSBoxInvert(u_int8_t num){

    if (num < 0 || num > sizeof(rsbox)) return -1;

    return rsbox[num];
}

// Gibt den Rundenkonstantenwert rc für die Runde zurück.
u_int8_t rc(u_int8_t num){
    
    u_int8_t r = 0x01;

    if (num == 1) return r;

    for (u_int8_t i = 1; i < num; i++) {
        if (r & 0x80) {
            r = (r << 1) ^ 0x1b;
        } else {
            r <<= 1;
        }
    }

    //printf("0x%02x\n", r);
    return r;
}

// Substituiert die Bytes im Zustand state mit der S-Box.
void subBytes(u_int8_t* state, size_t length){ 

    for (int i = 0; i < length; i++) {
        //printf("subBytes2: %d\n", state[i]);
        state[i] = getSBoxValue(state[i]);
    }
}

// Führt die Schlüsselerweiterung durch und speichert die Rundenschlüssel in roundKeys.
void keyExpansion(u_int8_t *key, u_int8_t *roundKeys, unsigned int keySize) {
    unsigned int NumberKeys = numKeyWords(keySize);
    unsigned int rounds = numRounds(keySize);
    const unsigned int Blocksize = 4; 

    memcpy(roundKeys, key, NumberKeys * Blocksize);

    uint8_t temp[Blocksize];

    for (int i = NumberKeys; i < (Blocksize * (rounds + 1)); i++) { 

        memcpy(temp, &roundKeys[(i - 1) * Blocksize], Blocksize);

        if (i % NumberKeys == 0) {

            uint8_t t = temp[0];
            temp[0] = temp[1];
            temp[1] = temp[2];
            temp[2] = temp[3];
            temp[3] = t;

            for (int j = 0; j < Blocksize; j++) {
                subBytes(&temp[j], sizeof(temp[j]));
            }

            temp[0] ^= rc(i / NumberKeys);
        }

        if (NumberKeys > 6 && i % NumberKeys == 4) {
            // subbytes für aes-192 und aes-256
            for (int j = 0; j < NumberKeys; j++) {
                subBytes(&temp[j], sizeof(temp[j]));
            }
        }

        for (int j = 0; j < Blocksize; j++) {
            roundKeys[i * Blocksize + j] = roundKeys[(i - NumberKeys) * Blocksize + j] ^ temp[j];
        }
    }

    // printf("Expanded Round Keys:\n");
    // for (unsigned int i = 0; i < keySize+16; i++) {
    //     printf("%02x", roundKeys[i]);
    //     if ((i + 1) % Blocksize == 0) {
    //         printf("\n");
    //     }
    // }
}



// Gibt den Rundenschlüssel für die Runde round zurück.
void getRoundKey(u_int8_t* roundKeys, u_int8_t* roundKey, u_int8_t round){

    // 16 Bytes pro Runde
    int startIndex = round * 16;

    for (int i = 0; i < 16; i++) {
        roundKey[i] = roundKeys[startIndex + i];
    }
}

// Addiert den Rundenschlüssel roundKey zum Zustand state.
void addRoundKey(u_int8_t* state, u_int8_t* roundKey, unsigned int rounds){
    for(int i = 0; i < rounds; i++){
        state[i] ^= roundKey[i];
    }
}

// Verschiebt die Zeilen im Zustand state.
void shiftRows(u_int8_t* state, size_t length){

    u_int8_t temp[length];

    memcpy(temp, state, length);

    unsigned int mod = sqrt(length);

    for(int i = 0; i < length; i++){
        state[i] = temp[(i+4*(i%mod)) % length];
    }
}

// Multipliziert die Spalten im Zustand state mit zwei.
// Beachten Sie die spezielle Addition in der Galois-Field-Arithmetik.
// Für die Multiplikation mit zwei ist der Ausgangswert um eins nach links zu shiften, und anschließend mit dem Produkt von 0x1b und dem größten Bit des Ausgangswertes zu XORen, wenn das größte Bit des Ausgangswertes 1 ist.
void multiply2(u_int8_t* state, size_t length){

    uint8_t msb;

    for (size_t i = 0; i < length; i++) {
        msb = ((state[i] & 0x80) != 0x00) ? 0x1b : 0x00;
        state[i] = (state[i] << 1) ^ msb;
    }

}

// Multipliziert die Spalten im Zustand state mit drei.
// Beachten Sie die spezielle Addition in der Galois-Field-Arithmetik.
// Nutzen Sie die bekannten Multiplikation mit zwei und Addieren Sie anschließend einmal den Ausgangswert durch Verwendung von xor auf.
void multiply3(u_int8_t* state, size_t length){

    u_int8_t temp[length];

    memcpy(temp, state, length);

    multiply2(state, length);

    for(int i = 0; i < length; i++){
        state[i] ^= temp[i];
    }
}


// Führt die Berechnung von mixColumns auf dem Zustand state durch
void mixColumns(uint8_t *state, size_t length) {

    u_int8_t temp[length];
    unsigned int rows = sqrt(length);

    memcpy(temp, state, length);

    for (int i = 0; i < rows; i++) {
        u_int8_t col[4] = {temp[i * rows], temp[i * rows + 1], temp[i * rows + 2], temp[i * rows + 3]};
        u_int8_t result[4];

        u_int8_t col_mult2[4] = {col[0], col[1], col[2], col[3]};
        u_int8_t col_mult3[4] = {col[0], col[1], col[2], col[3]};

        multiply2(col_mult2, sizeof(col_mult2));
        multiply3(col_mult3, sizeof(col_mult3));

        result[0] = col_mult2[0] ^ col_mult3[1] ^ col[2] ^ col[3];
        result[1] = col[0] ^ col_mult2[1] ^ col_mult3[2] ^ col[3];
        result[2] = col[0] ^ col[1] ^ col_mult2[2] ^ col_mult3[3];
        result[3] = col_mult3[0] ^ col[1] ^ col[2] ^ col_mult2[3];

        state[i * rows] = result[0];
        state[i * rows + 1] = result[1];
        state[i * rows + 2] = result[2];
        state[i * rows + 3] = result[3];
    }
}

// Verschlüsselt den Block mit den expandierten Schlüssern roundKeys und der Anzahl der Runden rounds.
void encrypt(u_int8_t* block, size_t length, u_int8_t* roundKeys, unsigned int rounds){

    for (unsigned int round = 1; round < rounds; ++round) {
        u_int8_t* roundKey = &roundKeys[round * length];
        subBytes(block, length); // s-Box   

        // printf("SUBBYTES: \n");
        // for(int i = 0; i < length; i++){
        //     printf("%x ", block[i]);
        // }
        // printf("\n");
        shiftRows(block, length); // permutation
        mixColumns(block, length);  // mult
        addRoundKey(block, roundKey, length); // key

    //     printf("OUTPUT: \n");
    //     for(int i = 0; i < length; i++){
    //         printf("%x ", block[i]);
    //     }
    //     printf("\n");

    // printf("\n\n");

    }

    u_int8_t* roundKey = &roundKeys[rounds * length];

    subBytes(block, length);
    shiftRows(block, length);

    addRoundKey(block, roundKey, length);  // letzte runde

    // for(int i = 0; i < length; i++){
    //     printf("%x ", block[i]);
    // }

    // printf("\n");
    
}

void multiply9(u_int8_t* state, size_t length) {
    
    u_int8_t temp[length];
    memcpy(temp, state, length);

    multiply2(state, length);
    multiply2(state, length);
    multiply2(state, length);

    for(int i = 0; i < length; i++){
        state[i] ^= temp[i];
    }

}

uint8_t multiply11(u_int8_t* state, size_t length) {
    
    u_int8_t temp[length];
    memcpy(temp, state, length);

    multiply2(state, length);
    multiply2(state, length);

    for(int i = 0; i < length; i++){
        state[i] ^= temp[i];
    }

    multiply2(state, length);

    for(int i = 0; i < length; i++){
        state[i] ^= temp[i];
    }

}

uint8_t multiply13(u_int8_t* state, size_t length) {
    
    u_int8_t temp[length];
    memcpy(temp, state, length);

    multiply2(state, length);

    for(int i = 0; i < length; i++){
        state[i] ^= temp[i];
    }

    multiply2(state, length);
    multiply2(state, length);

    for(int i = 0; i < length; i++){
        state[i] ^= temp[i];
    }

}

uint8_t multiply14(u_int8_t* state, size_t length) {
    
    u_int8_t temp[length];
    memcpy(temp, state, length);

    multiply2(state, length);

    for(int i = 0; i < length; i++){
        state[i] ^= temp[i];
    }

    multiply2(state, length);

    for(int i = 0; i < length; i++){
        state[i] ^= temp[i];
    }

    multiply2(state, length);

}

// Umkehrung von mixColumns. Entnehmen Sie die MixColumns-Matrix aus dem bereitgestellten Buch. 
// Für die Multiplikation mit 9, 11, 13 und 14 können Sie die Funktion Multiply verwenden, die in der Datei aes.c bereitgestellt ist.
void invMixColumns(u_int8_t* state, size_t length){

    u_int8_t temp[length];
    memcpy(temp, state, length);

    unsigned int rows = sqrt(length);

    for (int i = 0; i < rows; i++) {

        u_int8_t col[4] = {temp[i * rows], temp[i * rows + 1], temp[i * rows + 2], temp[i * rows + 3]};
        u_int8_t result[rows];

        u_int8_t col_mult9[4] = {col[0], col[1], col[2], col[3]};
        u_int8_t col_mult11[4] = {col[0], col[1], col[2], col[3]};
        u_int8_t col_mult13[4] = {col[0], col[1], col[2], col[3]};
        u_int8_t col_mult14[4] = {col[0], col[1], col[2], col[3]};

        multiply9(col_mult9, sizeof(col_mult9));
        multiply11(col_mult11, sizeof(col_mult11));
        multiply13(col_mult13, sizeof(col_mult13));
        multiply14(col_mult14, sizeof(col_mult14));

        result[0] = col_mult14[0] ^ col_mult11[1] ^ col_mult13[2] ^ col_mult9[3];  
        result[1] = col_mult9[0] ^ col_mult14[1] ^ col_mult11[2] ^ col_mult13[3];
        result[2] = col_mult13[0] ^ col_mult9[1] ^ col_mult14[2] ^ col_mult11[3];
        result[3] = col_mult11[0] ^ col_mult13[1] ^ col_mult9[2] ^ col_mult14[3];      
        
        //printf("%x %x %x %x\n", result[0], result[1], result[2], result[3]);

        state[i * rows] = result[0];
        state[i * rows + 1] = result[1];
        state[i * rows + 2] = result[2];
        state[i * rows + 3] = result[3];
    }
}

// Umkehrung von subBytes. Nutzen Sie die inverse S-Box, die in der Datei aes.c bereitgestellt ist.
void invSubBytes(u_int8_t* state, size_t length){
    for (int i = 0; i < length; i++) {
        //printf("subBytes2: %d\n", state[i]);
        state[i] = getSBoxInvert(state[i]);
    }
}

// Umkehrung von shiftRows.
void invShiftRows(u_int8_t* state, size_t length){
    u_int8_t temp[length];

    memcpy(temp, state, length);
    unsigned int mod = sqrt(length);

    for(int i = 0; i < length; i++){
        state[i] = temp[(i-4*(i%mod)) % length];
    }
}

// Entschlüsselt den Block mit den expandierten Schlüsseln roundKeys und der Anzahl der Runden rounds.
void decrypt(u_int8_t* block, size_t length, u_int8_t* roundKeys, unsigned int rounds){

    uint8_t* roundKey = &roundKeys[rounds * length];

    // Letzte Runde (ohne invMixColumns)
    addRoundKey(block, roundKey, length);

    for (unsigned int round = rounds - 1; round > 0; --round) {
        
        roundKey = &roundKeys[round * length];

        invShiftRows(block, length); // per mutation
        invSubBytes(block, length); // Inverse S-Box
        addRoundKey(block, roundKey, length);
        invMixColumns(block, length);     
    } 

    roundKey = &roundKeys[0];

    // for(int i = 0; i < length; i++){
    //     printf("%x ", roundKey[i]);
    // }

    invShiftRows(block, length);
    invSubBytes(block, length);
    addRoundKey(block, roundKey, length);
}   

// Verschlüsselt den Inhalt mit dem Schlüssel key und der Schlüssellänge keySize unter Verwendung des ECB-Verfahrens.
void ecb_encrypt (u_int8_t* content, u_int8_t* key, unsigned int keySize, size_t length){

    unsigned int blocksize = 16;
    unsigned int roundKeys_size = (numRounds(keySize) + 1) * blocksize;
    uint8_t roundKeys[roundKeys_size];

    

    keyExpansion(key, roundKeys, keySize);
    //addRoundKey(content, key, length);

    for(int i = 0; i < length; i += blocksize){

        u_int8_t block[blocksize];
        memcpy(block, &content[i], blocksize);
        encrypt(block, blocksize, roundKeys, numRounds(keySize));
        memcpy(&content[i], block, blocksize);

    }

}

// Entschlüsselt den Inhalt mit dem Schlüssel key und der Schlüssellänge keySize unter Verwendung des ECB-Verfahrens.
void ecb_decrypt (u_int8_t* content, u_int8_t* key, unsigned int keySize, size_t length){
    unsigned int blocksize = 16;
    unsigned int roundKeys_size = (numRounds(keySize) + 1) * blocksize;
    uint8_t roundKeys[roundKeys_size];

    keyExpansion(key, roundKeys, keySize);

    for (int i = 0; i < length; i += blocksize) {

        u_int8_t block[blocksize];
        memcpy(block, &content[i], blocksize);
        decrypt(block, blocksize, roundKeys, numRounds(keySize));
        memcpy(&content[i], block, blocksize);

    }
}

// Verschlüsselt den Inhalt mit dem Schlüssel key und der Schlüssellänge keySize unter Verwendung des CBC-Verfahrens mit dem Initialisierungsvektor iv.
void cbc_encrypt (u_int8_t* content, u_int8_t* key, unsigned int keySize, u_int8_t* iv, size_t length){

    unsigned int NumberKeys = numKeyWords(keySize);
    unsigned int blocksize = 16;
    unsigned int rounds = numRounds(keySize);
    unsigned int roundKeys_size = (rounds + 1) * blocksize;
    u_int8_t roundKeys[roundKeys_size];

    keyExpansion(key, roundKeys, keySize);

    u_int8_t previousBlock[blocksize];

    memcpy(previousBlock, iv, blocksize);


    for (int i = 0; i < length; i += blocksize) {
        
        u_int8_t block[blocksize];
        memcpy(block, &content[i], blocksize);

        for (int j = 0; j < blocksize; j++) {
            block[j] ^= (roundKeys[j] ^ previousBlock[j]); // block[j] ^= iv[j]
        }
        
        //encrypt(block, blocksize, roundKeys, rounds);
        ecb_encrypt(block, roundKeys, keySize, blocksize);

        memcpy(previousBlock, block, blocksize);

        memcpy(&content[i], block, blocksize);
    }

    for(int i = 0; i < length; i++){
        printf("%02x ", content[i]);
    }
    printf("\n");

}


// Entschlüsselt den Inhalt mit dem Schlüssel key und der Schlüssellänge keySize unter Verwendung des CBC-Verfahrens mit dem Initialisierungsvektor iv.
void cbc_decrypt(u_int8_t* content, u_int8_t* key, unsigned int keySize, u_int8_t* iv, size_t length) {

    unsigned int blocksize = 16;
    unsigned int NumberKeys = numKeyWords(keySize);
    unsigned int rounds = numRounds(keySize);
    unsigned int roundKeys_size = (rounds + 1) * blocksize; 
    u_int8_t *roundKeys = malloc(roundKeys_size);

    keyExpansion(key, roundKeys, keySize);
    
    u_int8_t previousBlock[blocksize]; 
    u_int8_t currentBlock[blocksize];  

    memcpy(previousBlock, iv, blocksize);


    for (int i = 0; i < length; i += blocksize) {
        memcpy(currentBlock, &content[i], blocksize); 

        ecb_decrypt(currentBlock, roundKeys, keySize, blocksize);
            
        for (int j = 0; j < blocksize; j++) {
            currentBlock[j] ^= previousBlock[j];
        }

        memcpy(previousBlock, &content[i], blocksize);

        memcpy(&content[i], currentBlock, blocksize);
    }

    printf("Decrypted content:\n");
    for (int i = 0; i < length; i++) {
        printf("%02x ", content[i]);
    }
    printf("\n");

    free(roundKeys);
}
