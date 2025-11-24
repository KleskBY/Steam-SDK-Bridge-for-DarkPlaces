// utf8lib_wtf8.c.h


#include <stdio.h>
#include <stdlib.h>

// Assuming a maximum WTF-8 sequence length of 4 bytes for simplicity
#define MAX_UTF8_SEQUENCE_LENGTH 4

// Function to convert a single WTF-8 character to Unicode
int wtf8_to_unicode(const unsigned char* wtf8_char, int* unicode_char) {
    if ((wtf8_char[0] & 0x80) == 0x00) { // Single-byte character (ASCII)
        *unicode_char = wtf8_char[0];
        return 1;
    } else if ((wtf8_char[0] & 0xE0) == 0xC0) { // Two-byte character
        if ((wtf8_char[1] & 0xC0) != 0x80) return -1; // Invalid format
        *unicode_char = ((wtf8_char[0] & 0x1F) << 6) | (wtf8_char[1] & 0x3F);
        return 2;
    } else if ((wtf8_char[0] & 0xF0) == 0xE0) { // Three-byte character
         if ((wtf8_char[1] & 0xC0) != 0x80 || (wtf8_char[2] & 0xC0) != 0x80) return -1; // Invalid format
        *unicode_char = ((wtf8_char[0] & 0x0F) << 12) | ((wtf8_char[1] & 0x3F) << 6) | (wtf8_char[2] & 0x3F);
        
        // Check for surrogate code points (U+D800 to U+DFFF)
        if (*unicode_char >= 0xD800 && *unicode_char <= 0xDFFF) {
            // Handle surrogate - in WTF-8, it's represented directly
            return 3;
        }
        return 3;
    } else if ((wtf8_char[0] & 0xF8) == 0xF0) { // Four-byte character
        if ((wtf8_char[1] & 0xC0) != 0x80 || (wtf8_char[2] & 0xC0) != 0x80 || (wtf8_char[3] & 0xC0) != 0x80) return -1; // Invalid format
        *unicode_char = ((wtf8_char[0] & 0x07) << 18) | ((wtf8_char[1] & 0x3F) << 12) | ((wtf8_char[2] & 0x3F) << 6) | (wtf8_char[3] & 0x3F);
        return 4;
    } else {
        return -1; // Invalid WTF-8 start byte
    }
}

// Function to convert a WTF-8 string to a Unicode string
int convert_wtf8_to_unicode(const unsigned char* wtf8_string, int* unicode_string, int max_unicode_length) {
    int wtf8_index = 0;
    int unicode_index = 0;

    while (wtf8_string[wtf8_index] != '\0' && unicode_index < max_unicode_length) {
        int unicode_char;
        int bytes_read = wtf8_to_unicode(wtf8_string + wtf8_index, &unicode_char);

        if (bytes_read > 0) {
            unicode_string[unicode_index++] = unicode_char;
            wtf8_index += bytes_read;
        } else {
           return -1; //Error in conversion
        }
    }
    unicode_string[unicode_index] = 0; // Null-terminate the Unicode string
    return unicode_index;
}

//int main() {
//    unsigned char wtf8_string[] = "Hello \xF0\x9D\x93\x98 world!"; //UTF-8 for "Mathematical Bold Capital H" (U+1D5D8)
//    int unicode_string[100]; // Assuming a maximum of 100 Unicode characters
//
//    int result_length = convert_wtf8_toonsunicode(wtf8_string, unicode_string, 100);
//
//    if (result_length >= 0) {
//        printf("Unicode: ");
//        for (int i = 0; i < result_length; i++) {
//            printf("%X ", unicode_string[i]);
//        }
//        printf("\n");
//    } else {
//        printf("Conversion error.\n");
//    }
//
//    return 0;
//}
//
//
//

//#include <stdio.h>
//#include <stdlib.h>

// Function to convert a single WTF-8 character to UTF-8, handling surrogates
int wtf8_to_utf8_char(const unsigned char* wtf8_char, unsigned char* utf8_char) {
    if ((wtf8_char[0] & 0x80) == 0x00) {
        // Single-byte character (ASCII)
        utf8_char[0] = wtf8_char[0];
        return 1;
    } else if ((wtf8_char[0] & 0xE0) == 0xC0) {
        // Two-byte character
        utf8_char[0] = wtf8_char[0];
        utf8_char[1] = wtf8_char[1];
        return 2;
    } else if ((wtf8_char[0] & 0xF0) == 0xE0) {
        // Three-byte character or surrogate
      if ((wtf8_char[0] == 0xED) && (wtf8_char[1] >= 0xA0) && (wtf8_char[1] <= 0xBF)){
            //Unpaired surrogate, replace with U+FFFD
            utf8_char[0] = 0xEF;
            utf8_char[1] = 0xBF;
            utf8_char[2] = 0xBD;
            return 3;
        } else {
            //Regular three-byte UTF-8 character
            utf8_char[0] = wtf8_char[0];
            utf8_char[1] = wtf8_char[1];
            utf8_char[2] = wtf8_char[2];
            return 3;
        }
    } else if ((wtf8_char[0] & 0xF8) == 0xF0) {
        // Four-byte character
        utf8_char[0] = wtf8_char[0];
        utf8_char[1] = wtf8_char[1];
        utf8_char[2] = wtf8_char[2];
        utf8_char[3] = wtf8_char[3];
        return 4;
    }
    return 0;
}

// Function to convert a WTF-8 string to UTF-8
char* wtf8_to_utf8(const char* wtf8_string) {
    size_t wtf8_len = 0;
    while (wtf8_string[wtf8_len] != '\0') {
        wtf8_len++;
    }
    
    size_t utf8_max_len = wtf8_len * 3 + 1; // Maximum possible length if all chars are surrogates
    char* utf8_string_result = (char*)malloc(utf8_max_len);
    if (utf8_string_result == NULL) {
        return NULL; // Allocation failed
    }
    
    size_t utf8_index = 0;
    size_t wtf8_index = 0;

    while (wtf8_index < wtf8_len) {
        unsigned char wtf8_char[4] = {0};
        int char_size;

        if ((wtf8_string[wtf8_index] & 0x80) == 0x00) {
            char_size = 1;
        } else if ((wtf8_string[wtf8_index] & 0xE0) == 0xC0) {
            char_size = 2;
        } else if ((wtf8_string[wtf8_index] & 0xF0) == 0xE0) {
            char_size = 3;
        } else if ((wtf8_string[wtf8_index] & 0xF8) == 0xF0) {
            char_size = 4;
        } else {
            // Invalid character, skip it
            wtf8_index++;
            continue;
        }

        for(int i = 0; i < char_size; i++){
            wtf8_char[i] = wtf8_string[wtf8_index + i];
        }

        unsigned char utf8_char[4];
        int converted_size = wtf8_to_utf8_char(wtf8_char, utf8_char);
        for(int i = 0; i < converted_size; i++){
             utf8_string_result[utf8_index + i] = utf8_char[i];
        }

        utf8_index += converted_size;
        wtf8_index += char_size;
    }

    utf8_string_result[utf8_index] = '\0';
    return utf8_string_result;
}