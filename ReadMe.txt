# obfuscation

Content
1. Parser file
2. Encoder/Decoder
3. Usage of obfuscation in MS VC++ project


1. Parser file

Usage
 Parser <operation>

Where is
  operation:
    o - parse code files and obfuscation them
    r - restore original codes


Example:

Parser c

1. Create the temp directory: temp_original
2. Copy all codes files from the original directory to the temp directory
3. Obfuscate codes files in the original directory

Parser r

1. Delete all the code obfuscated files in the original directory
2. Move the original code from the temp directory to the original directory
3. Delete the temporary directory


























