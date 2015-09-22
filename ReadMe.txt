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

Parser c

1) Create the temp directory TEMP_OBFUSCATION for each a code directory.
2) Copy all codes files from the original directory to the temp directory
3) Obfuscate codes files in the original directory
4) Create the log file (not text) in the root directory, where are  wrote all changes 

Parser r

1) Read the log file with all changes 
2) Delete all the code obfuscated files in the original directory
3) Move the original code from the temp directory to the original directory
4) Delete the temporary directory
5) Delete the log file

Return values:
0 - OK
1 - error, see console/output messages for details

Notes for usage:
  - Parser will skip the file if it will meet "#define __NO__OBFUSCATION".
  - Parser doesn't make an obfuscation for single character, only for a string. 	
  - Before start ot the project make its backup copy. 
  - Before run of "Parser c" the old temp directory TEMP_OBFUSCATION doesn't must exist in the code directory.
  - If "Parser o" returns the error ("1"), then MS Visual Studio stops the build. 		 


///////////////////////////////////////////
2. Encoder/Decoder

- Encoder (obfuscater)

Adds "_BLA_BLA" to the original string.


///////////////////////////////////////////
3. Usage of obfuscation in MS VC++ project

For example see the MSVC project: Test

///////////////////////////////////////////























