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
3) Obfuscate codes files in the original directory
4) Create the archive file (arch_obfuscate.obf) in the root directory, where were  wrote all changes 

Parser r

1) Read Serial_obfuscate.obf with all changes 
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
  - Before start ot the project make its backup copy. 
  - Before run of "Parser o" the old temp directory TEMP_OBFUSCATION doesn't must exist in the code directory.
  - If "Parser o" returns the error ("1"), then MS Visual Studio stops the build. 		 


Example for log:


D:\Work\obfuscation\trunk\Test\Test>parser o

***** Obfuscation *****
Obfuscate operation.
Parsing files in D:\Work\obfuscation\trunk\Test\Test\:
Resource.h -> passed
Setting.h -> passed
stdafx.h -> passed
targetver.h -> passed
Test.h -> passed
Setting.cpp -> passed
stdafx.cpp -> passed
Temp directory:
D:\Work\obfuscation\trunk\Test\Test\TEMP_OBFUSCATION\
Test.cpp was copied to Temp directory
Test.cpp -> obfuscated
Parsing files in D:\Work\obfuscation\trunk\Test\Test\Other_codes\:
Temp directory:
D:\Work\obfuscation\trunk\Test\Test\Other_codes\TEMP_OBFUSCATION\
Setting2.h was copied to Temp directory
Setting2.h -> obfuscated
Setting2.cpp was copied to Temp directory
Setting2.cpp -> obfuscated
Arch file was created: D:\Work\obfuscation\trunk\Test\Test\arch_obfuscate.obf
Obfuscate operation - completed!
Obfuscated files number: 3

Obfuscated finish: Normal


D:\Work\obfuscation\trunk\Test\Test>parser r

***** Obfuscation *****
Restore operation.
Restore the archive of obfuscation
Arch file arch_obfuscate.obf was deleted
Restore the original files
Original direrctory: D:\Work\obfuscation\trunk\Test\Test\
Test.cpp -> restored.
Original direrctory: D:\Work\obfuscation\trunk\Test\Test\Other_codes\
Setting2.h -> restored.
Setting2.cpp -> restored.
Resore operation - completed!

Obfuscated finish: Normal


D:\Work\obfuscation\trunk\Test\Test>


///////////////////////////////////////////
2. Encoder/Decoder

- Encoder (obfuscater)

Adds "_BLA_BLA" to the original string.


///////////////////////////////////////////
3. Usage of obfuscation in MS VC++ project

For example see the MSVC project: Test

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
///////////////////////////////////////////























