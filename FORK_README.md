# mkjson Fork for uhubctl

This is a fork of [mkjson](https://github.com/Jacajack/mkjson) maintained for use in [uhubctl](https://github.com/mvp/uhubctl).

## Why This Fork?

The original mkjson repository was archived on April 19, 2025. Additionally, we discovered a critical bug that needed to be fixed for proper uhubctl operation.

## Bug Fix

**Fixed incorrect format specifier for 64-bit integers**

- **File**: mkjson.c
- **Line**: 205
- **Change**: `%Ld` → `%lld`
- **Issue**: The incorrect format specifier `%Ld` caused truncation of large 64-bit values
- **Example**: USB 3.0 speeds of 5,000,000,000 bps were being displayed as 705,032,704

## Original Project

- **Author**: Jacek Wieczorek (Jacajack)
- **License**: MIT
- **Original Repository**: https://github.com/Jacajack/mkjson

## Usage in uhubctl

This fork is used as a lightweight JSON generation library in uhubctl, providing a 10x size reduction compared to cJSON (357 lines vs 3,443 lines).