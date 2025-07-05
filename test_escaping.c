/* test_escaping.c - Comprehensive test for mkjson string escaping
 *
 * Copyright (C) 2025 Ben Roeder
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "mkjson.h"

void test_basic_escaping() {
    printf("Testing basic string escaping:\n");
    
    // Test quotes
    char* json = mkjson(MKJSON_OBJ, 1, MKJSON_STRING, "field", "value with \"quotes\"");
    assert(strstr(json, "\\\"quotes\\\"") != NULL);
    printf("✓ Quotes escaped correctly\n");
    free(json);
    
    // Test backslashes
    json = mkjson(MKJSON_OBJ, 1, MKJSON_STRING, "path", "C:\\Windows\\System32");
    assert(strstr(json, "C:\\\\Windows\\\\System32") != NULL);
    printf("✓ Backslashes escaped correctly\n");
    free(json);
    
    // Test newlines
    json = mkjson(MKJSON_OBJ, 1, MKJSON_STRING, "text", "line1\nline2");
    assert(strstr(json, "line1\\nline2") != NULL);
    printf("✓ Newlines escaped correctly\n");
    free(json);
    
    // Test tabs
    json = mkjson(MKJSON_OBJ, 1, MKJSON_STRING, "text", "\tindented");
    assert(strstr(json, "\\tindented") != NULL);
    printf("✓ Tabs escaped correctly\n");
    free(json);
}

void test_key_escaping() {
    printf("\nTesting key escaping:\n");
    
    // Test key with quotes
    char* json = mkjson(MKJSON_OBJ, 1, MKJSON_STRING, "key\"with\"quotes", "value");
    assert(strstr(json, "\"key\\\"with\\\"quotes\"") != NULL);
    printf("✓ Keys with quotes escaped correctly\n");
    free(json);
    
    // Test key with newline
    json = mkjson(MKJSON_OBJ, 1, MKJSON_STRING, "key\nwith\nnewlines", "value");
    assert(strstr(json, "key\\nwith\\nnewlines") != NULL);
    printf("✓ Keys with newlines escaped correctly\n");
    free(json);
}

void test_control_characters() {
    printf("\nTesting control character escaping:\n");
    
    // Test various control characters (excluding NULL which terminates C strings)
    char test_str[32];
    test_str[0] = '\x01';  // SOH
    test_str[1] = '\x02';  // STX
    test_str[2] = '\b';    // Backspace
    test_str[3] = '\t';    // Tab
    test_str[4] = '\n';    // Newline
    test_str[5] = '\f';    // Form feed
    test_str[6] = '\r';    // Carriage return
    test_str[7] = '\x1f';  // Unit separator
    test_str[8] = '\0';    // Terminate string
    
    char* json = mkjson(MKJSON_OBJ, 1, MKJSON_STRING, "ctrl", test_str);
    
    // Check that control characters are escaped properly
    assert(strstr(json, "\\u0001") != NULL);  // SOH
    assert(strstr(json, "\\u0002") != NULL);  // STX
    assert(strstr(json, "\\b") != NULL);      // Backspace (\x08)
    assert(strstr(json, "\\t") != NULL);      // Tab (\x09)
    assert(strstr(json, "\\n") != NULL);      // Newline (\x0a)
    assert(strstr(json, "\\f") != NULL);      // Form feed (\x0c)
    assert(strstr(json, "\\r") != NULL);      // Carriage return (\x0d)
    assert(strstr(json, "\\u001f") != NULL);  // Unit separator
    
    printf("✓ Control characters escaped correctly\n");
    free(json);
}

void test_edge_cases() {
    printf("\nTesting edge cases:\n");
    
    // Test NULL string
    char* json = mkjson(MKJSON_OBJ, 1, MKJSON_STRING, "null_field", NULL);
    assert(strstr(json, "\"null_field\": null") != NULL);
    printf("✓ NULL strings handled correctly\n");
    free(json);
    
    // Test empty string
    json = mkjson(MKJSON_OBJ, 1, MKJSON_STRING, "empty", "");
    assert(strstr(json, "\"empty\": \"\"") != NULL);
    printf("✓ Empty strings handled correctly\n");
    free(json);
    
    // Test very long string with escapes
    char long_str[1024];
    for (int i = 0; i < 512; i++) {
        long_str[i*2] = '"';
        long_str[i*2+1] = '\\';
    }
    long_str[1023] = '\0';
    
    json = mkjson(MKJSON_OBJ, 1, MKJSON_STRING, "long", long_str);
    assert(json != NULL);
    assert(strlen(json) > 2048); // Should be much longer due to escaping
    printf("✓ Long strings with many escapes handled correctly\n");
    free(json);
}

void test_real_world_examples() {
    printf("\nTesting real-world USB device names:\n");
    
    // Test various problematic device names
    char* json;
    
    // Device with quotes in name
    json = mkjson(MKJSON_OBJ, 1, MKJSON_STRING, "device", "USB \"High-Speed\" Hub");
    assert(strstr(json, "USB \\\"High-Speed\\\" Hub") != NULL);
    printf("✓ Device name with quotes\n");
    free(json);
    
    // Device with backslashes (Windows path)
    json = mkjson(MKJSON_OBJ, 1, MKJSON_STRING, "path", "\\\\?\\USB#VID_2109&PID_2817#5&1c8b2a89&0&1");
    assert(strstr(json, "\\\\\\\\?\\\\USB#VID_2109") != NULL);
    printf("✓ Windows device path\n");
    free(json);
    
    // Device with non-ASCII but valid UTF-8
    json = mkjson(MKJSON_OBJ, 1, MKJSON_STRING, "device", "USB Hub (中文)");
    assert(strstr(json, "USB Hub (中文)") != NULL);
    printf("✓ UTF-8 device names preserved\n");
    free(json);
}

int main() {
    printf("=== mkjson String Escaping Test Suite ===\n\n");
    
    test_basic_escaping();
    test_key_escaping();
    test_control_characters();
    test_edge_cases();
    test_real_world_examples();
    
    printf("\n✅ All tests passed!\n");
    return 0;
}