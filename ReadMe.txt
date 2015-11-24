# obfuscation
////////////////////////////////////////////////
Content
1. Parser  of files
2. Encoder/Decoder
3. Usage of obfuscation in MS VC++ project

////////////////////////////////////////
1. Parser of files

Usage
 Parser <operation>

Where is
  operation:
    o - parses the code files and obfuscation of them
    r - restores original codes


Example:

Parser o

1) Create the temp directory TEMP_OBFUSCATION for each a code directory.
2) Copy all codes files from the original directory to the temp directory
3) Obfuscate the codes files in the original directory
4) Create the archive file (arch_obfuscate.obf) in the root directory, where it  wrote all changes 

Parser r

1) Read arch_obfuscate.obf with all changes 
2) Delete all the code obfuscated files in the original directory
3) Move the original code from the temp directory to the original directory
4) Delete the temporary directory
5) Delete arch_obfuscate.obf

Return values:
0 - OK
1 - error, see console/output messages for details

///////////////////////////////////////////
2. Encoder/Decoder

Use of AES 256

Demo project is DemoAES.

//Encrypt a string
//Input: a plain text in strIn
//Return: an encrypted string by AES or empty string and throw by error  
static void encryptString(const string& strIn, string& strOut);

//Decrypt a string
//Input: a decrypted text by AES in strIn
//Return: a plain text or empty string and throw by error  
static void decryptString(wstring strInW, wstring& strOutW); 

Usage:

#include "AES.h"

	string strTextA = CStringA(m_strText);
	string strOutTextA;
	CAES::encryptString(strTextA, strOutTextA);
	m_strEncode = CString(strOutTextA.c_str());
	wstring strTextOutW;
	CAES::decryptString(m_strEncode.GetBuffer(), strTextOutW);
	m_strDecode = strTextOutW.c_str();

Schema of operations:

string ->(encrypt)->string->(compiler)->wstring->(encrypt)->wstring

Test of AES 256

Use \trunk\DemoAES\Release\DemosAES.exe for test of Encrypt/Decrypt.

///////////////////////////////////////////
3. Usage of obfuscation in MS VC++ project

For example see the MSVC project: Test

For obfuscation the project must go three stages: 

-Obfuscation by using parser o
-Build the project by MSVC.
-Restore original codes by using parser r.

Steps for setting:

1)Move parser.exe from Parser (or \Test\Test) project to the root of your project (not Solution). 
If your solution has several projects for obfuscation, then parser should be in those projects.

2)Open your project in MSVC.

3)Open the project property.

4)Select Configuration: Release (recommended) or Debug.

5)Open Build Events/Pre-Build Event.  Set "parser o" in Command Line. Set "Yes" in Use In Build if you wish 
to enable the obfuscation or set "No" for its disabling.   

6)Open Build Events/Post-Build Event.  Set "parser r" in Command Line. Set "Yes" in Use In Build if you wish 
to enable the code restoring or set "No" for its disabling.

8)Save changes in the project properties. 

7)Open your code files and set the macros of obfuscation:

#define ENCRYPT(x) x

and ENCRYPT(....) for obfuscated strings, for example, see Test project.

8)For decrypt operations:

- Move AES.cpp and AES.h from DemoAES project to your project.
- Create the get() method for text decrypting by CAES::decryptString(), see above. 
- add #include "AES.h" to .cpp where will be get().

Sample of get():

#include "AES.h"

#define DECRYP_ENABLE

wchar_t* CSetting::get( wchar_t* strKey) 
{
#ifdef DECRYP_ENABLE
	CAES::decryptString(strKey, m_str[m_nIndexStr]);
#else
	m_str[m_nIndexStr] = strKey;
#endif
	int nIndex = m_nIndexStr;
	if (m_nIndexStr == MAX_STRING_NUMBER)
		m_nIndexStr = 0;
	else
		m_nIndexStr++;
	return (wchar_t*)m_str[nIndex].c_str();
}


9)Build the project, use the output log for control.


Notes before usage:
  - Parser will skip the file if it will meet "#define __NO__OBFUSCATION". You should not use that directive 
	in the following combinations: "//#define __NO__OBFUSCATION" or "/*#define __NO__OBFUSCATION*/".
  - Parser doesn't make an obfuscation for single character, only for a string. 	
  - Before start ot the project make its backup copy (optional). 
  - Before run of "Parser o" the old temp directory TEMP_OBFUSCATION doesn't must exist in the code directory.
  - If "Parser o" returns the error ("1"), then MS Visual Studio stops the build. 
  - If "Parser r" returns the error ("1"), then the build will be incomplete. 
  - Builded project cannot be used for debuging, because the source code will not fit the builded code. 			 


Example of the output log:

1>------ Rebuild All started: Project: Test, Configuration: Debug Win32 ------
1>  
1>  ***** Obfuscation *****
1>  Obfuscate operation.
1>  Parsing files in D:\Work\obfuscation\trunk\Test\Test\:
1>  Resource.h -> passed
1>  Setting.h -> passed
1>  stdafx.h -> passed
1>  targetver.h -> passed
1>  Test.h -> passed
1>  Setting.cpp -> passed
1>  stdafx.cpp -> passed
1>  Temp directory:
1>  D:\Work\obfuscation\trunk\Test\Test\TEMP_OBFUSCATION\
1>  Test.cpp was copied to Temp directory
1>  Test.cpp -> obfuscated
1>  Parsing files in D:\Work\obfuscation\trunk\Test\Test\Other_codes\:
1>  Temp directory:
1>  D:\Work\obfuscation\trunk\Test\Test\Other_codes\TEMP_OBFUSCATION\
1>  Setting2.h was copied to Temp directory
1>  Setting2.h -> obfuscated
1>  Setting2.cpp was copied to Temp directory
1>  Setting2.cpp -> obfuscated
1>  Arch file was created: D:\Work\obfuscation\trunk\Test\Test\arch_obfuscate.obf
1>  Obfuscate operation - completed!
1>  Obfuscated files number: 3
1>  
1>  Obfuscated finish: Normal
1>  
1>  stdafx.cpp
1>  Test.cpp
1>  Setting.cpp
1>  Setting2.cpp
1>  Generating Code...
1>  Test.vcxproj -> D:\Work\obfuscation\trunk\Test\Debug\Test.exe
1>  
1>  ***** Obfuscation *****
1>  Restore operation.
1>  Restore the archive of obfuscation
1>  Arch file arch_obfuscate.obf was deleted
1>  Restore the original files
1>  Original direrctory: D:\Work\obfuscation\trunk\Test\Test\
1>  Test.cpp -> restored.
1>  Original direrctory: D:\Work\obfuscation\trunk\Test\Test\Other_codes\
1>  Setting2.h -> restored.
1>  Setting2.cpp -> restored.
1>  Resore operation - completed!
1>  
1>  Obfuscated finish: Normal
1>  
========== Rebuild All: 1 succeeded, 0 failed, 0 skipped ==========

After Text.exe run:
 
key1 value1
key2 value2
key3 value3

key4 value4
key5 value5
key6 value6


Encoded values of  Test.exe via Process Explorer:

g5C9B929D863458C0DA2DFB0CC29C9EFF2B38B7A3896E73F8FEEC7C3A39F12003
A0B2CDE89AFB3A2DC8B26A9848371F8D31893EFA860E3838F1E7995619B82069
FDE709B5A3B56353003AFF5BEFD058520022D80419D62EDB8BA2B115F00BCF9A
1C4A173DDF424C0BB6443CAACADFF6773A2C206359D7542C1B79698FEC45454C
3A537BF49FB0F7B6A6C546C0520AC78C89E8095E1E7F2CE5C47965A8A34716D9
406CE6FF34D6114C1F77967C93443B8AFD1603E1944A4AC3F413A50ABCE52875
Fatal Error: MFC initialization failed
Fatal Error: GetModuleHandle failed
DEC5B08F9B1AFDF2F4CC7B702B6F56006DC74346FF08E24C8AB0705318A09DC7
5E0E3784B7487714150E5C8663F1FA40E7334339B3C7CD8031F14755AD6AC070
214CE2F5A0D3A8F57008F6B4BDFF7ACD80303D784FEFC307F466329EE7E42BA1
C08D1F8C0B9D6FF484CDA4398C2212925EE18DCADC449CAC7E6D0AF9A37D08E4
7063E8F3E50A89433AA2FD99259C1DA7E1796F9803A81D122048BF0A737CC75F
D5E03AC03F589D56DE65BA95DADFED1197435E0820B678ECF300AF0FC3093680
491805428AD98BDD431A3B7BEBCD3AB25153C3EE63883CC510D186E6CBCC0DDC
............


Google Tests:

[==========] Running 34 tests from 1 test case.
[----------] Global test environment set-up.
[----------] 34 tests from EngineTestParser
[ RUN      ] EngineTestParser.BasicTest
[       OK ] EngineTestParser.BasicTest (0 ms)
[ RUN      ] EngineTestParser.findSubDirs_noParentDirectory
[       OK ] EngineTestParser.findSubDirs_noParentDirectory (0 ms)
[ RUN      ] EngineTestParser.findSubDirs_findAllSubDirectories
[       OK ] EngineTestParser.findSubDirs_findAllSubDirectories (0 ms)
[ RUN      ] EngineTestParser.findFileByType_setInvalidParameters1
[       OK ] EngineTestParser.findFileByType_setInvalidParameters1 (0 ms)
[ RUN      ] EngineTestParser.findFileByType_setInvalidParameters2
[       OK ] EngineTestParser.findFileByType_setInvalidParameters2 (0 ms)
[ RUN      ] EngineTestParser.findFileByType_noFilesByType
[       OK ] EngineTestParser.findFileByType_noFilesByType (1 ms)
[ RUN      ] EngineTestParser.findFileByType_setIncorrectCurrentDirectory
[       OK ] EngineTestParser.findFileByType_setIncorrectCurrentDirectory (0 ms)

[ RUN      ] EngineTestParser.findFileByType_checkFilesFound
[       OK ] EngineTestParser.findFileByType_checkFilesFound (0 ms)
[ RUN      ] EngineTestParser.findCodesFiles_noParentDirectory
[       OK ] EngineTestParser.findCodesFiles_noParentDirectory (0 ms)
[ RUN      ] EngineTestParser.findCodesFiles_noCodesFiles
[       OK ] EngineTestParser.findCodesFiles_noCodesFiles (1 ms)
[ RUN      ] EngineTestParser.findCodesFiles_haveCodesFiles
[       OK ] EngineTestParser.findCodesFiles_haveCodesFiles (1 ms)
[ RUN      ] EngineTestParser.getFileError_genericException
[       OK ] EngineTestParser.getFileError_genericException (0 ms)
[ RUN      ] EngineTestParser.createTempDir
[       OK ] EngineTestParser.createTempDir (15 ms)
[ RUN      ] EngineTestParser.encodeText_encode
[       OK ] EngineTestParser.encodeText_encode (0 ms)
[ RUN      ] EngineTestParser.parseFile_noPath
[       OK ] EngineTestParser.parseFile_noPath (0 ms)
[ RUN      ] EngineTestParser.parseFile_noFileName
[       OK ] EngineTestParser.parseFile_noFileName (0 ms)
[ RUN      ] EngineTestParser.parseFile_cannotOpenFile
[       OK ] EngineTestParser.parseFile_cannotOpenFile (1037 ms)
[ RUN      ] EngineTestParser.parseFile_haveNullFileLength
[       OK ] EngineTestParser.parseFile_haveNullFileLength (1025 ms)
[ RUN      ] EngineTestParser.parseFile_define__NO__OBFUSCATION
[       OK ] EngineTestParser.parseFile_define__NO__OBFUSCATION (1030 ms)
[ RUN      ] EngineTestParser.parseFile_parseWithCreateTempDir
[       OK ] EngineTestParser.parseFile_parseWithCreateTempDir (1044 ms)
[ RUN      ] EngineTestParser.parseFile_notFileForParsing
[       OK ] EngineTestParser.parseFile_notFileForParsing (1028 ms)
[ RUN      ] EngineTestParser.parseFile_createTempDirError
[       OK ] EngineTestParser.parseFile_createTempDirError (1026 ms)
[ RUN      ] EngineTestParser.parseFiles_noCCodeDirectories
[       OK ] EngineTestParser.parseFiles_noCCodeDirectories (0 ms)
[ RUN      ] EngineTestParser.parseFiles_emptyCCodeDirectories
[       OK ] EngineTestParser.parseFiles_emptyCCodeDirectories (0 ms)
[ RUN      ] EngineTestParser.parseFiles_noFilesForObfuscation
[       OK ] EngineTestParser.parseFiles_noFilesForObfuscation (1028 ms)
[ RUN      ] EngineTestParser.parseFiles_hasFilesForObfuscation
[       OK ] EngineTestParser.parseFiles_hasFilesForObfuscation (1077 ms)
[ RUN      ] EngineTestParser.parseFiles_errorNotExistDir
[       OK ] EngineTestParser.parseFiles_errorNotExistDir (1 ms)
[ RUN      ] EngineTestParser.obfuscate_noPathDirectory
[       OK ] EngineTestParser.obfuscate_noPathDirectory (0 ms)
[ RUN      ] EngineTestParser.obfuscate_noFilesForObfuscation
[       OK ] EngineTestParser.obfuscate_noFilesForObfuscation (1064 ms)
[ RUN      ] EngineTestParser.obfuscate_haveFilesForObfuscation
[       OK ] EngineTestParser.obfuscate_haveFilesForObfuscation (1115 ms)
[ RUN      ] EngineTestParser.restore_noPathDirectory
[       OK ] EngineTestParser.restore_noPathDirectory (0 ms)
[ RUN      ] EngineTestParser.restore_invalidPathDirectory
[       OK ] EngineTestParser.restore_invalidPathDirectory (1 ms)
[ RUN      ] EngineTestParser.restore_noArchFile
[       OK ] EngineTestParser.restore_noArchFile (1069 ms)
[ RUN      ] EngineTestParser.restore_fullOperation
[       OK ] EngineTestParser.restore_fullOperation (1085 ms)
[----------] 34 tests from EngineTestParser (12672 ms total)

[----------] Global test environment tear-down
[==========] 34 tests from 1 test case ran. (12674 ms total)
[  PASSED  ] 34 tests.

///////////////////////////////////////////























