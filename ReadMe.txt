# obfuscation
////////////////////////////////////////////////
Content
1. Parser file
2. Encoder/Decoder
3. Usage of obfuscation in MS VC++ project

////////////////////////////////////////
1. Parser file

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

Notes for usage:
  - Parser will skip the file if it will meet "#define __NO__OBFUSCATION".
  - Parser doesn't make an obfuscation for single character, only for a string. 	
  - Before run of "Parser o" the old temp directory TEMP_OBFUSCATION doesn't must exist in the code directory.


///////////////////////////////////////////
2. Encoder/Decoder

- Encoder (obfuscater)

CStringA Encoder(CStringA strText) in Parser project. It adds 2 to each character of string.

-Decoder

wchar_t* CSetting::get( wchar_t* strKey) in Test project. It deducts 2 from each character of string.


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

4)Select Configuration: Release or Debug.

5)Open Build Events/Pre-Build Event.  Set "parser o" in Command Line. Set "Yes" in Use In Build if you wish 
to enable the obfuscation or set "No" for its disabling.   

6)Open Build Events/Post-Build Event.  Set "parser r" in Command Line. Set "Yes" in Use In Build if you wish 
to enable the code restoring or set "No" for its disabling.

8)Save changes in the project properties. 

7)Open your code files and set the macros of obfuscation:

#define ENCRYPT(x) x

and ENCRYPT(....) for obfuscated strings, for example, see Test project.

8)Build the project, use the building log for control.


Notes before usage:
  - Parser will skip the file if it will meet "#define __NO__OBFUSCATION". You should not use that directive 
	in the following combinations: "//#define __NO__OBFUSCATION" or "/*#define __NO__OBFUSCATION*/".
  - Parser doesn't make an obfuscation for single character, only for a string. 	
  - Before start ot the project make its backup copy (recommend). 
  - Before run of "Parser o" the old temp directory TEMP_OBFUSCATION doesn't must exist in the code directory.
  - If "Parser o" returns the error ("1"), then MS Visual Studio stops the build. 
  - If "Parser r" returns the error ("1"), then the build will be incomplete. 
  - Builded project cannot be used for debuging, because the source code will not fit the builded code. 			 


Example of building log:

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
Для продолжения нажмите любую клавишу . . .

///////////////////////////////////////////























