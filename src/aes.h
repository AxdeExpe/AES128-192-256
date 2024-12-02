#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


// Gibt die Anzahl der Runden zur Schlüssellänge in Bit zurück, bei ungültiger Schlüssellänge wird -1 zurückgegeben
unsigned int numRounds(unsigned int keySize);

// Gibt die Anzahl der Schlüsselwörter zur Schlüssellänge in Bit zurück, bei ungültiger Schlüssellänge wird -1 zurückgegeben
unsigned int numKeyWords(unsigned int keySize);

// Gibt die Ergebnisse der SBox für die Eingabe zurück. Die S-Box ist in der Datei aes.c bereits bereitgestellt.
u_int8_t getSBoxValue(u_int8_t num);

// Gibt die Ergebnisse der inversen SBox für die Eingabe zurück. Die inverse S-Box ist in der Datei aes.c bereits bereitgestellt.
u_int8_t getSBoxInvert(u_int8_t num);

// Gibt den Rundenkonstantenwert rc für die Runde zurück.
u_int8_t rc(u_int8_t num);

// Substituiert die Bytes im Zustand state mit der S-Box.
void subBytes(u_int8_t* state, size_t length);

// Führt die Schlüsselerweiterung durch und speichert die Rundenschlüssel in roundKeys.
void keyExpansion(u_int8_t *key, u_int8_t *roundKeys, unsigned int keySize);



// Gibt den Rundenschlüssel für die Runde round zurück.
void getRoundKey(u_int8_t* roundKeys, u_int8_t* roundKey, u_int8_t round);

// Addiert den Rundenschlüssel roundKey zum Zustand state.
void addRoundKey(u_int8_t* state, u_int8_t* roundKey, unsigned int rounds);

// Verschiebt die Zeilen im Zustand state.
void shiftRows(u_int8_t* state, size_t length);

// Multipliziert die Spalten im Zustand state mit zwei.
// Beachten Sie die spezielle Addition in der Galois-Field-Arithmetik.
// Für die Multiplikation mit zwei ist der Ausgangswert um eins nach links zu shiften, und anschließend mit dem Produkt von 0x1b und dem größten Bit des Ausgangswertes zu XORen, wenn das größte Bit des Ausgangswertes 1 ist.
void multiply2(u_int8_t* state, size_t length);

// Multipliziert die Spalten im Zustand state mit drei.
// Beachten Sie die spezielle Addition in der Galois-Field-Arithmetik.
// Nutzen Sie die bekannten Multiplikation mit zwei und Addieren Sie anschließend einmal den Ausgangswert durch Verwendung von xor auf.
void multiply3(u_int8_t* state, size_t length);


// Führt die Berechnung von mixColumns auf dem Zustand state durch
void mixColumns(uint8_t *state, size_t length);


// Verschlüsselt den Block mit den expandierten Schlüssern roundKeys und der Anzahl der Runden rounds.
void encrypt(u_int8_t* block, size_t length, u_int8_t* roundKeys, unsigned int rounds);
void multiply9(u_int8_t* state, size_t length);

uint8_t multiply11(u_int8_t* state, size_t length);

uint8_t multiply13(u_int8_t* state, size_t length);

uint8_t multiply14(u_int8_t* state, size_t length);

// TODO:
// Umkehrung von mixColumns. Entnehmen Sie die MixColumns-Matrix aus dem bereitgestellten Buch. 
// Für die Multiplikation mit 9, 11, 13 und 14 können Sie die Funktion Multiply verwenden, die in der Datei aes.c bereitgestellt ist.
void invMixColumns(u_int8_t* state, size_t length);

// Umkehrung von subBytes. Nutzen Sie die inverse S-Box, die in der Datei aes.c bereitgestellt ist.
void invSubBytes(u_int8_t* state, size_t length);

// Umkehrung von shiftRows.
void invShiftRows(u_int8_t* state, size_t length);
// Entschlüsselt den Block mit den expandierten Schlüsseln roundKeys und der Anzahl der Runden rounds.
void decrypt(u_int8_t* block, size_t length, u_int8_t* roundKeys, unsigned int rounds); 

// Verschlüsselt den Inhalt mit dem Schlüssel key und der Schlüssellänge keySize unter Verwendung des ECB-Verfahrens.
void ecb_encrypt (u_int8_t* content, u_int8_t* key, unsigned int keySize, size_t length);

// Entschlüsselt den Inhalt mit dem Schlüssel key und der Schlüssellänge keySize unter Verwendung des ECB-Verfahrens.
void ecb_decrypt (u_int8_t* content, u_int8_t* key, unsigned int keySize, size_t length);

// Verschlüsselt den Inhalt mit dem Schlüssel key und der Schlüssellänge keySize unter Verwendung des CBC-Verfahrens mit dem Initialisierungsvektor iv.
void cbc_encrypt (u_int8_t* content, u_int8_t* key, unsigned int keySize, u_int8_t* iv, size_t length);

// Entschlüsselt den Inhalt mit dem Schlüssel key und der Schlüssellänge keySize unter Verwendung des CBC-Verfahrens mit dem Initialisierungsvektor iv.
void cbc_decrypt(u_int8_t* content, u_int8_t* key, unsigned int keySize, uint8_t* iv, size_t length);