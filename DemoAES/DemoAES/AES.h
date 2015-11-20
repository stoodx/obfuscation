#pragma once

/*
Class CAES for AES 256 encoding/decoding strings in Unicode(UTF-16)
Environment: MS Windows x86/x64  
Use library: STL C++11

by http://www.codeproject.com/Articles/1380/A-C-Implementation-of-the-Rijndael-Encryption-Decr

*/
#include <string>
#include <stdint.h>
#include <codecvt>

using namespace std;


class CAES
{
	CAES();

public:
	~CAES(void);

	//Encrypt a string
	//Input: a plain text in strIn
	//Return: an encrypted string by AES or empty string and throw by error  
	static const string encryptString(const string& strIn);

	//Decrypt a string
	//Input: a decrypted text by AES in strIn
	//Return: a plain text or empty string and throw by error  
	static const string decryptString(const string& strIn); 
private:
	const string encryptInternalString(const string& strIn); 
	const string decryptInternalString(const string& strIn); 

	enum { MAX_BLOCK_SIZE=32, MAX_ROUNDS=14, MAX_KC=8, MAX_BC=8 };

	//Key Initialization Flag
	bool m_bKeyInit; 
	//Block Size
	int32_t m_iBlockSize;

	//Point to AES kye
	uint8_t* m_pKey;
	//Number of Rounds
	int32_t m_iROUNDS;

	//Expand a user-supplied key material into a session key.
	void makeKey(void);
	
	//Encryption (m_Ke) round key
	int32_t m_Ke[MAX_ROUNDS+1][MAX_BC];
	//Decryption (m_Kd) round key
    int32_t m_Kd[MAX_ROUNDS+1][MAX_BC];
	//Auxiliary private use buffers
	int32_t m_tk[MAX_KC];
	int32_t m_a[MAX_BC];
	int32_t m_t[MAX_BC];
	//default keys
	//static const  uint8_t m_KEY_16_DEFAULT[16];
	//static const  uint8_t m_KEY_24_DEFAULT[24]; 
	static const  uint8_t m_KEY_32_DEFAULT[32]; 

	static const int8_t m_sm_S[256];
	static const int8_t m_sm_Si[256];

	static const int8_t m_sm_rcon[30];
	static const int32_t m_sm_U1[256];
    static const int32_t m_sm_U2[256];
    static const int32_t m_sm_U3[256];
    static const int32_t m_sm_U4[256];
    static const int32_t m_sm_T1[256];
    static const int32_t m_sm_T2[256];
    static const int32_t m_sm_T3[256];
    static const int32_t m_sm_T4[256];
    static const int32_t m_sm_T5[256];
    static const int32_t m_sm_T6[256];
    static const int32_t m_sm_T7[256];
    static const int32_t m_sm_T8[256];

	static const int32_t m_sm_shifts[3][4][2];

	//Function to convert string of unsigned chars to string of chars
	void charStr2HexStr(unsigned char const* pucCharStr, string& pszHexStr, int32_t iSize);
	
	//Function to convert unsigned char to string of length 2
	void char2Hex(unsigned char ch, char* szHex);
	
	//Function to convert string of chars to string of unsigned chars
	void hexStr2CharStr(const string& pszHexStr, unsigned char* pucCharStr, int iSize);
	
	//Function to convert string of length 2 to unsigned char
	void hex2Char(char const* szHex, unsigned char& rch);

};

