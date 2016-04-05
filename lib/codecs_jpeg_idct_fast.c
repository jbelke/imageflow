#include <stdio.h>
#include "imageflow_private.h"

#define JPEG_INTERNALS
#include "jpeglib.h"
#include "jdct.h" /* Private declarations for DCT subsystem */
#include "codecs_jpeg.h"
#include "fastapprox.h"

void jpeg_idct_downscale_wrap_islow_fast(j_decompress_ptr cinfo, jpeg_component_info * compptr, JCOEFPTR coef_block,
                                         JSAMPARRAY output_buf, JDIMENSION output_col);

void jpeg_idct_downscale_wrap_islow_fast_1x1(j_decompress_ptr cinfo, jpeg_component_info * compptr, JCOEFPTR coef_block,
                                             JSAMPARRAY output_buf, JDIMENSION output_col);

void jpeg_idct_downscale_wrap_islow_fast_2x2(j_decompress_ptr cinfo, jpeg_component_info * compptr, JCOEFPTR coef_block,
                                             JSAMPARRAY output_buf, JDIMENSION output_col);

void jpeg_idct_downscale_wrap_islow_fast_3x3(j_decompress_ptr cinfo, jpeg_component_info * compptr, JCOEFPTR coef_block,
                                             JSAMPARRAY output_buf, JDIMENSION output_col);

void jpeg_idct_downscale_wrap_islow_fast_4x4(j_decompress_ptr cinfo, jpeg_component_info * compptr, JCOEFPTR coef_block,
                                             JSAMPARRAY output_buf, JDIMENSION output_col);

void jpeg_idct_downscale_wrap_islow_fast_5x5(j_decompress_ptr cinfo, jpeg_component_info * compptr, JCOEFPTR coef_block,
                                             JSAMPARRAY output_buf, JDIMENSION output_col);

void jpeg_idct_downscale_wrap_islow_fast_6x6(j_decompress_ptr cinfo, jpeg_component_info * compptr, JCOEFPTR coef_block,
                                             JSAMPARRAY output_buf, JDIMENSION output_col);

void jpeg_idct_downscale_wrap_islow_fast_7x7(j_decompress_ptr cinfo, jpeg_component_info * compptr, JCOEFPTR coef_block,
                                             JSAMPARRAY output_buf, JDIMENSION output_col);
static const uint8_t lut_linear_short_to_srgb[3584] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 11, 12, 13, 14,
    15, 15, 16, 17, 17, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 23,
    24, 24, 25, 25, 26, 26, 27, 27, 28, 28, 28, 29, 29, 30, 30, 30,
    31, 31, 32, 32, 32, 33, 33, 33, 34, 34, 34, 35, 35, 35, 36, 36,
    36, 37, 37, 37, 38, 38, 38, 39, 39, 39, 40, 40, 40, 40, 41, 41,
    41, 42, 42, 42, 42, 43, 43, 43, 44, 44, 44, 44, 45, 45, 45, 45,
    46, 46, 46, 46, 47, 47, 47, 47, 48, 48, 48, 48, 49, 49, 49, 49,
    50, 50, 50, 50, 51, 51, 51, 51, 51, 52, 52, 52, 52, 53, 53, 53,
    53, 53, 54, 54, 54, 54, 54, 55, 55, 55, 55, 56, 56, 56, 56, 56,
    57, 57, 57, 57, 57, 58, 58, 58, 58, 58, 59, 59, 59, 59, 59, 60,
    60, 60, 60, 60, 60, 61, 61, 61, 61, 61, 62, 62, 62, 62, 62, 63,
    63, 63, 63, 63, 63, 64, 64, 64, 64, 64, 64, 65, 65, 65, 65, 65,
    66, 66, 66, 66, 66, 66, 67, 67, 67, 67, 67, 67, 68, 68, 68, 68,
    68, 68, 69, 69, 69, 69, 69, 69, 70, 70, 70, 70, 70, 70, 70, 71,
    71, 71, 71, 71, 71, 72, 72, 72, 72, 72, 72, 73, 73, 73, 73, 73,
    73, 73, 74, 74, 74, 74, 74, 74, 74, 75, 75, 75, 75, 75, 75, 75,
    76, 76, 76, 76, 76, 76, 77, 77, 77, 77, 77, 77, 77, 78, 78, 78,
    78, 78, 78, 78, 78, 79, 79, 79, 79, 79, 79, 79, 80, 80, 80, 80,
    80, 80, 80, 81, 81, 81, 81, 81, 81, 81, 81, 82, 82, 82, 82, 82,
    82, 82, 83, 83, 83, 83, 83, 83, 83, 83, 84, 84, 84, 84, 84, 84,
    84, 84, 85, 85, 85, 85, 85, 85, 85, 86, 86, 86, 86, 86, 86, 86,
    86, 87, 87, 87, 87, 87, 87, 87, 87, 87, 88, 88, 88, 88, 88, 88,
    88, 88, 89, 89, 89, 89, 89, 89, 89, 89, 90, 90, 90, 90, 90, 90,
    90, 90, 90, 91, 91, 91, 91, 91, 91, 91, 91, 92, 92, 92, 92, 92,
    92, 92, 92, 92, 93, 93, 93, 93, 93, 93, 93, 93, 93, 94, 94, 94,
    94, 94, 94, 94, 94, 94, 95, 95, 95, 95, 95, 95, 95, 95, 95, 96,
    96, 96, 96, 96, 96, 96, 96, 96, 97, 97, 97, 97, 97, 97, 97, 97,
    97, 98, 98, 98, 98, 98, 98, 98, 98, 98, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 100, 100, 100, 100, 100, 100, 100, 100, 100, 101, 101, 101,
    101, 101, 101, 101, 101, 101, 101, 102, 102, 102, 102, 102, 102, 102, 102, 102,
    102, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 104, 104, 104, 104, 104,
    104, 104, 104, 104, 104, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 106,
    106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 107, 107, 107, 107, 107, 107,
    107, 107, 107, 107, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 109,
    109, 109, 109, 109, 109, 109, 109, 109, 109, 110, 110, 110, 110, 110, 110, 110,
    110, 110, 110, 110, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 112,
    112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 113, 113, 113, 113, 113, 113,
    113, 113, 113, 113, 113, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114,
    114, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 116, 116, 116, 116,
    116, 116, 116, 116, 116, 116, 116, 116, 117, 117, 117, 117, 117, 117, 117, 117,
    117, 117, 117, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 119,
    119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 120, 120, 120, 120, 120,
    120, 120, 120, 120, 120, 120, 120, 121, 121, 121, 121, 121, 121, 121, 121, 121,
    121, 121, 121, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122,
    123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 124, 124, 124, 124,
    124, 124, 124, 124, 124, 124, 124, 124, 124, 125, 125, 125, 125, 125, 125, 125,
    125, 125, 125, 125, 125, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126,
    126, 126, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 129, 129, 129, 129,
    129, 129, 129, 129, 129, 129, 129, 129, 129, 130, 130, 130, 130, 130, 130, 130,
    130, 130, 130, 130, 130, 130, 130, 131, 131, 131, 131, 131, 131, 131, 131, 131,
    131, 131, 131, 131, 132, 132, 132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
    132, 132, 133, 133, 133, 133, 133, 133, 133, 133, 133, 133, 133, 133, 133, 134,
    134, 134, 134, 134, 134, 134, 134, 134, 134, 134, 134, 134, 134, 135, 135, 135,
    135, 135, 135, 135, 135, 135, 135, 135, 135, 135, 135, 136, 136, 136, 136, 136,
    136, 136, 136, 136, 136, 136, 136, 136, 136, 137, 137, 137, 137, 137, 137, 137,
    137, 137, 137, 137, 137, 137, 137, 137, 138, 138, 138, 138, 138, 138, 138, 138,
    138, 138, 138, 138, 138, 138, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
    139, 139, 139, 139, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
    140, 140, 140, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141,
    141, 141, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142,
    142, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,
    144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 144, 145,
    145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 146, 146,
    146, 146, 146, 146, 146, 146, 146, 146, 146, 146, 146, 146, 146, 146, 147, 147,
    147, 147, 147, 147, 147, 147, 147, 147, 147, 147, 147, 147, 147, 148, 148, 148,
    148, 148, 148, 148, 148, 148, 148, 148, 148, 148, 148, 148, 148, 149, 149, 149,
    149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 150, 150, 150,
    150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 151, 151, 151,
    151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 152, 152, 152,
    152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 153, 153, 153,
    153, 153, 153, 153, 153, 153, 153, 153, 153, 153, 153, 153, 153, 153, 154, 154,
    154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 155, 155,
    155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 156,
    156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
    157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157,
    157, 158, 158, 158, 158, 158, 158, 158, 158, 158, 158, 158, 158, 158, 158, 158,
    158, 158, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159,
    159, 159, 159, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
    160, 160, 160, 160, 161, 161, 161, 161, 161, 161, 161, 161, 161, 161, 161, 161,
    161, 161, 161, 161, 161, 161, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162,
    162, 162, 162, 162, 162, 162, 162, 162, 163, 163, 163, 163, 163, 163, 163, 163,
    163, 163, 163, 163, 163, 163, 163, 163, 163, 164, 164, 164, 164, 164, 164, 164,
    164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 165, 165, 165, 165, 165,
    165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 166, 166, 166,
    166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
    167, 167, 167, 167, 167, 167, 167, 167, 167, 167, 167, 167, 167, 167, 167, 167,
    167, 167, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168,
    168, 168, 168, 168, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169,
    169, 169, 169, 169, 169, 169, 169, 170, 170, 170, 170, 170, 170, 170, 170, 170,
    170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 171, 171, 171, 171, 171, 171,
    171, 171, 171, 171, 171, 171, 171, 171, 171, 171, 171, 171, 171, 172, 172, 172,
    172, 172, 172, 172, 172, 172, 172, 172, 172, 172, 172, 172, 172, 172, 172, 172,
    173, 173, 173, 173, 173, 173, 173, 173, 173, 173, 173, 173, 173, 173, 173, 173,
    173, 173, 173, 174, 174, 174, 174, 174, 174, 174, 174, 174, 174, 174, 174, 174,
    174, 174, 174, 174, 174, 174, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175,
    175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 176, 176, 176, 176, 176, 176,
    176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 177, 177,
    177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177,
    177, 178, 178, 178, 178, 178, 178, 178, 178, 178, 178, 178, 178, 178, 178, 178,
    178, 178, 178, 178, 178, 179, 179, 179, 179, 179, 179, 179, 179, 179, 179, 179,
    179, 179, 179, 179, 179, 179, 179, 179, 179, 180, 180, 180, 180, 180, 180, 180,
    180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 181, 181,
    181, 181, 181, 181, 181, 181, 181, 181, 181, 181, 181, 181, 181, 181, 181, 181,
    181, 181, 182, 182, 182, 182, 182, 182, 182, 182, 182, 182, 182, 182, 182, 182,
    182, 182, 182, 182, 182, 182, 182, 183, 183, 183, 183, 183, 183, 183, 183, 183,
    183, 183, 183, 183, 183, 183, 183, 183, 183, 183, 183, 184, 184, 184, 184, 184,
    184, 184, 184, 184, 184, 184, 184, 184, 184, 184, 184, 184, 184, 184, 184, 184,
    185, 185, 185, 185, 185, 185, 185, 185, 185, 185, 185, 185, 185, 185, 185, 185,
    185, 185, 185, 185, 185, 186, 186, 186, 186, 186, 186, 186, 186, 186, 186, 186,
    186, 186, 186, 186, 186, 186, 186, 186, 186, 186, 187, 187, 187, 187, 187, 187,
    187, 187, 187, 187, 187, 187, 187, 187, 187, 187, 187, 187, 187, 187, 187, 188,
    188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188,
    188, 188, 188, 188, 188, 189, 189, 189, 189, 189, 189, 189, 189, 189, 189, 189,
    189, 189, 189, 189, 189, 189, 189, 189, 189, 189, 190, 190, 190, 190, 190, 190,
    190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190,
    191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191,
    191, 191, 191, 191, 191, 191, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192,
    192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 193, 193, 193, 193,
    193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 193, 193,
    193, 193, 194, 194, 194, 194, 194, 194, 194, 194, 194, 194, 194, 194, 194, 194,
    194, 194, 194, 194, 194, 194, 194, 194, 195, 195, 195, 195, 195, 195, 195, 195,
    195, 195, 195, 195, 195, 195, 195, 195, 195, 195, 195, 195, 195, 195, 195, 196,
    196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196,
    196, 196, 196, 196, 196, 197, 197, 197, 197, 197, 197, 197, 197, 197, 197, 197,
    197, 197, 197, 197, 197, 197, 197, 197, 197, 197, 197, 197, 198, 198, 198, 198,
    198, 198, 198, 198, 198, 198, 198, 198, 198, 198, 198, 198, 198, 198, 198, 198,
    198, 198, 198, 199, 199, 199, 199, 199, 199, 199, 199, 199, 199, 199, 199, 199,
    199, 199, 199, 199, 199, 199, 199, 199, 199, 199, 200, 200, 200, 200, 200, 200,
    200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200,
    200, 201, 201, 201, 201, 201, 201, 201, 201, 201, 201, 201, 201, 201, 201, 201,
    201, 201, 201, 201, 201, 201, 201, 201, 201, 202, 202, 202, 202, 202, 202, 202,
    202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202,
    203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203, 203,
    203, 203, 203, 203, 203, 203, 203, 203, 204, 204, 204, 204, 204, 204, 204, 204,
    204, 204, 204, 204, 204, 204, 204, 204, 204, 204, 204, 204, 204, 204, 204, 204,
    205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205,
    205, 205, 205, 205, 205, 205, 205, 205, 206, 206, 206, 206, 206, 206, 206, 206,
    206, 206, 206, 206, 206, 206, 206, 206, 206, 206, 206, 206, 206, 206, 206, 206,
    207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207,
    207, 207, 207, 207, 207, 207, 207, 207, 208, 208, 208, 208, 208, 208, 208, 208,
    208, 208, 208, 208, 208, 208, 208, 208, 208, 208, 208, 208, 208, 208, 208, 208,
    208, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
    209, 209, 209, 209, 209, 209, 209, 209, 209, 210, 210, 210, 210, 210, 210, 210,
    210, 210, 210, 210, 210, 210, 210, 210, 210, 210, 210, 210, 210, 210, 210, 210,
    210, 210, 211, 211, 211, 211, 211, 211, 211, 211, 211, 211, 211, 211, 211, 211,
    211, 211, 211, 211, 211, 211, 211, 211, 211, 211, 211, 212, 212, 212, 212, 212,
    212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212,
    212, 212, 212, 212, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213,
    213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 214, 214, 214,
    214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214,
    214, 214, 214, 214, 214, 214, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215,
    215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215,
    216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216,
    216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 217, 217, 217, 217, 217, 217,
    217, 217, 217, 217, 217, 217, 217, 217, 217, 217, 217, 217, 217, 217, 217, 217,
    217, 217, 217, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218,
    218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 219, 219, 219,
    219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219,
    219, 219, 219, 219, 219, 219, 219, 219, 220, 220, 220, 220, 220, 220, 220, 220,
    220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220,
    220, 220, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221,
    221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 221, 222, 222, 222, 222,
    222, 222, 222, 222, 222, 222, 222, 222, 222, 222, 222, 222, 222, 222, 222, 222,
    222, 222, 222, 222, 222, 222, 222, 223, 223, 223, 223, 223, 223, 223, 223, 223,
    223, 223, 223, 223, 223, 223, 223, 223, 223, 223, 223, 223, 223, 223, 223, 223,
    223, 223, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224,
    224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 225, 225, 225,
    225, 225, 225, 225, 225, 225, 225, 225, 225, 225, 225, 225, 225, 225, 225, 225,
    225, 225, 225, 225, 225, 225, 225, 225, 226, 226, 226, 226, 226, 226, 226, 226,
    226, 226, 226, 226, 226, 226, 226, 226, 226, 226, 226, 226, 226, 226, 226, 226,
    226, 226, 226, 227, 227, 227, 227, 227, 227, 227, 227, 227, 227, 227, 227, 227,
    227, 227, 227, 227, 227, 227, 227, 227, 227, 227, 227, 227, 227, 227, 228, 228,
    228, 228, 228, 228, 228, 228, 228, 228, 228, 228, 228, 228, 228, 228, 228, 228,
    228, 228, 228, 228, 228, 228, 228, 228, 228, 228, 229, 229, 229, 229, 229, 229,
    229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229, 229,
    229, 229, 229, 229, 229, 229, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230,
    230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230,
    230, 230, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231,
    231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 232, 232,
    232, 232, 232, 232, 232, 232, 232, 232, 232, 232, 232, 232, 232, 232, 232, 232,
    232, 232, 232, 232, 232, 232, 232, 232, 232, 232, 233, 233, 233, 233, 233, 233,
    233, 233, 233, 233, 233, 233, 233, 233, 233, 233, 233, 233, 233, 233, 233, 233,
    233, 233, 233, 233, 233, 233, 234, 234, 234, 234, 234, 234, 234, 234, 234, 234,
    234, 234, 234, 234, 234, 234, 234, 234, 234, 234, 234, 234, 234, 234, 234, 234,
    234, 234, 234, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235,
    235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 236,
    236, 236, 236, 236, 236, 236, 236, 236, 236, 236, 236, 236, 236, 236, 236, 236,
    236, 236, 236, 236, 236, 236, 236, 236, 236, 236, 236, 236, 237, 237, 237, 237,
    237, 237, 237, 237, 237, 237, 237, 237, 237, 237, 237, 237, 237, 237, 237, 237,
    237, 237, 237, 237, 237, 237, 237, 237, 237, 238, 238, 238, 238, 238, 238, 238,
    238, 238, 238, 238, 238, 238, 238, 238, 238, 238, 238, 238, 238, 238, 238, 238,
    238, 238, 238, 238, 238, 238, 239, 239, 239, 239, 239, 239, 239, 239, 239, 239,
    239, 239, 239, 239, 239, 239, 239, 239, 239, 239, 239, 239, 239, 239, 239, 239,
    239, 239, 239, 239, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240,
    240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240,
    240, 241, 241, 241, 241, 241, 241, 241, 241, 241, 241, 241, 241, 241, 241, 241,
    241, 241, 241, 241, 241, 241, 241, 241, 241, 241, 241, 241, 241, 241, 241, 242,
    242, 242, 242, 242, 242, 242, 242, 242, 242, 242, 242, 242, 242, 242, 242, 242,
    242, 242, 242, 242, 242, 242, 242, 242, 242, 242, 242, 242, 242, 243, 243, 243,
    243, 243, 243, 243, 243, 243, 243, 243, 243, 243, 243, 243, 243, 243, 243, 243,
    243, 243, 243, 243, 243, 243, 243, 243, 243, 243, 243, 244, 244, 244, 244, 244,
    244, 244, 244, 244, 244, 244, 244, 244, 244, 244, 244, 244, 244, 244, 244, 244,
    244, 244, 244, 244, 244, 244, 244, 244, 244, 245, 245, 245, 245, 245, 245, 245,
    245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245, 245,
    245, 245, 245, 245, 245, 245, 245, 246, 246, 246, 246, 246, 246, 246, 246, 246,
    246, 246, 246, 246, 246, 246, 246, 246, 246, 246, 246, 246, 246, 246, 246, 246,
    246, 246, 246, 246, 246, 246, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247,
    247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247,
    247, 247, 247, 247, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248,
    248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248,
    248, 248, 248, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249,
    249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249,
    249, 249, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250,
    250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250,
    250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251,
    251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251,
    251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252,
    252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252,
    253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253,
    253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253,
    254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254,
    254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
};
static const int16_t lut_srgb_to_linear_short[256] = {
    0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 66, 72, 78,
    85, 92, 99, 107, 115, 123, 131, 140, 150, 159, 169, 180, 190, 201, 213, 224,
    237, 249, 262, 275, 289, 303, 318, 332, 348, 363, 379, 396, 413, 430, 448, 466,
    484, 503, 523, 542, 563, 583, 604, 626, 648, 670, 693, 717, 740, 765, 789, 814,
    840, 866, 893, 920, 947, 975, 1003, 1032, 1062, 1092, 1122, 1153, 1184, 1216, 1248, 1281,
    1314, 1348, 1382, 1417, 1453, 1488, 1525, 1562, 1599, 1637, 1675, 1714, 1753, 1793, 1834, 1875,
    1916, 1959, 2001, 2044, 2088, 2132, 2177, 2222, 2268, 2314, 2361, 2409, 2457, 2506, 2555, 2604,
    2655, 2706, 2757, 2809, 2861, 2915, 2968, 3022, 3077, 3133, 3189, 3245, 3302, 3360, 3418, 3477,
    3537, 3597, 3657, 3719, 3780, 3843, 3906, 3970, 4034, 4099, 4164, 4230, 4297, 4364, 4432, 4500,
    4569, 4639, 4709, 4780, 4852, 4924, 4997, 5070, 5144, 5219, 5294, 5370, 5447, 5524, 5602, 5680,
    5760, 5839, 5920, 6001, 6082, 6165, 6248, 6331, 6416, 6500, 6586, 6672, 6759, 6847, 6935, 7024,
    7113, 7203, 7294, 7386, 7478, 7571, 7664, 7758, 7853, 7949, 8045, 8142, 8239, 8338, 8436, 8536,
    8636, 8737, 8839, 8941, 9044, 9148, 9252, 9357, 9463, 9570, 9677, 9785, 9893, 10002, 10112, 10223,
    10334, 10446, 10559, 10673, 10787, 10902, 11017, 11134, 11251, 11368, 11487, 11606, 11726, 11847, 11968, 12090,
    12213, 12336, 12460, 12585, 12711, 12837, 12965, 13093, 13221, 13350, 13481, 13611, 13743, 13875, 14008, 14142,
    14276, 14412, 14548, 14684, 14822, 14960, 15099, 15239, 15379, 15521, 15663, 15805, 15949, 16093, 16238, 16384,
};
static const int16_t jpeg_scale_to_7_x_7_weights[7][8] = {
    { 14810, 1574, 0, 0, 0, 0, 0, 0 },
    { -261, 13184, 3461, 0, 0, 0, 0, 0 },
    { 0, -505, 11018, 5871, 0, 0, 0, 0 },
    { 0, 0, -497, 8689, 8689, -497, 0, 0 },
    { 0, 0, 0, 0, 5871, 11018, -505, 0 },
    { 0, 0, 0, 0, 0, 3461, 13184, -261 },
    { 0, 0, 0, 0, 0, 0, 1574, 14810 },
};
static const int16_t jpeg_scale_to_6_x_6_weights[6][8] = {
    { 12274, 4110, 0, 0, 0, 0, 0, 0 },
    { -368, 8560, 8560, -368, 0, 0, 0, 0 },
    { 0, 0, 3927, 11725, 732, 0, 0, 0 },
    { 0, 0, 0, 732, 11725, 3927, 0, 0 },
    { 0, 0, 0, 0, -368, 8560, 8560, -368 },
    { 0, 0, 0, 0, 0, 0, 4110, 12274 },
};
static const int16_t jpeg_scale_to_5_x_5_weights[5][8] = {
    { 10025, 6558, -199, 0, 0, 0, 0, 0 },
    { -360, 4186, 10119, 2438, 0, 0, 0, 0 },
    { 0, 0, 264, 7928, 7928, 264, 0, 0 },
    { 0, 0, 0, 0, 2438, 10119, 4186, -360 },
    { 0, 0, 0, 0, 0, -199, 6558, 10025 },
};
static const int16_t jpeg_scale_to_4_x_4_weights[4][8] = {
    { 7460, 7460, 1464, 0, 0, 0, 0, 0 },
    { 0, 1344, 6848, 6848, 1344, 0, 0, 0 },
    { 0, 0, 0, 1344, 6848, 6848, 1344, 0 },
    { 0, 0, 0, 0, 0, 1464, 7460, 7460 },
};
static const int16_t jpeg_scale_to_3_x_3_weights[3][8] = {
    { 5123, 6599, 3961, 701, 0, 0, 0, 0 },
    { 0, 156, 2497, 5539, 5539, 2497, 156, 0 },
    { 0, 0, 0, 0, 701, 3961, 6599, 5123 },
};
static const int16_t jpeg_scale_to_2_x_2_weights[2][8] = {
    { 3059, 4283, 4283, 3059, 1434, 267, 0, 0 },
    { 0, 0, 267, 1434, 3059, 4283, 4283, 3059 },
};
static const int16_t jpeg_scale_to_1_x_1_weights[1][8] = {
    { 1493, 1931, 2282, 2487, 2487, 2282, 1931, 1493 },
};
#define FLOW_SHORT_MAX (256 * 64)
#define FLOW_SHORT_REVERSE_LUT_DIVISOR
#define FLOW_SHORTS_PER_LINE 16
#define REVERSE_LUT_SIZE (256 * 14)
#define FLOW_WEIGHT_DIVISOR FLOW_SHORT_MAX

#define jpeg_idct_downscale_wrap_islow_fast_generic(cinfo, compptr, coef_block, output_buf, output_col, scaled, summation) \
JSAMPLE result[DCTSIZE2];\
JSAMPROW rows[DCTSIZE] = {&result[0], &result[DCTSIZE], &result[DCTSIZE * 2], &result[DCTSIZE * 3], &result[DCTSIZE * 4], &result[DCTSIZE *5], &result[DCTSIZE * 6], &result[DCTSIZE *7]};\
int i; \
jpeg_idct_islow(cinfo, compptr, coef_block, &rows[0], 0); \
int16_t linearized[DCTSIZE2];\
for (i = 0; i < DCTSIZE2; i++)\
    linearized[i] = lut_srgb_to_linear_short[result[i]]; \
int16_t scaled_h[DCTSIZE2];\
for (int row = 0; row < DCTSIZE; row++) {\
    int16_t * r = &linearized[row * DCTSIZE];\
    int16_t * dest = &scaled_h[row]; \
    summation \
}\
for (int row = 0; row < scaled; row++) {\
    int16_t * r = &scaled_h[row * DCTSIZE]; \
    int16_t * dest = &linearized[row]; \
    summation \
} \
for (int row = 0; row < scaled; row++) {\
    for (int col = 0; col < scaled; col++) {\
        size_t reverse_lut_index = (size_t)linearized[row * DCTSIZE + col] * (REVERSE_LUT_SIZE -1) / FLOW_SHORT_MAX; \
        *(output_buf[row] + output_col + col) = reverse_lut_index > (REVERSE_LUT_SIZE -1) ? 255 : lut_linear_short_to_srgb[reverse_lut_index];\
    }\
} \

#define DEFAULT_WEIGHTED_SUM(scaled, matrix) \
  for (int to = 0; to < scaled; to++) {\
        int64_t sum = 0;\
        for (int from = 0; from < DCTSIZE; from++) {\
            sum += (int64_t)r[from] * matrix[to][from];\
        }\
        dest[to * DCTSIZE] = (int16_t)(sum / FLOW_WEIGHT_DIVISOR); \
    }\

void jpeg_idct_downscale_wrap_islow_fast_1x1(j_decompress_ptr cinfo, jpeg_component_info * compptr, JCOEFPTR coef_block,
                                         JSAMPARRAY output_buf, JDIMENSION output_col){
    jpeg_idct_downscale_wrap_islow_fast_generic(cinfo, compptr, coef_block, output_buf, output_col, 1, DEFAULT_WEIGHTED_SUM(1, jpeg_scale_to_1_x_1_weights));
}

void jpeg_idct_downscale_wrap_islow_fast_2x2(j_decompress_ptr cinfo, jpeg_component_info * compptr, JCOEFPTR coef_block,
                                             JSAMPARRAY output_buf, JDIMENSION output_col){
    jpeg_idct_downscale_wrap_islow_fast_generic(cinfo, compptr, coef_block, output_buf, output_col, 2, DEFAULT_WEIGHTED_SUM(2, jpeg_scale_to_2_x_2_weights));
}

void jpeg_idct_downscale_wrap_islow_fast_3x3(j_decompress_ptr cinfo, jpeg_component_info * compptr, JCOEFPTR coef_block,
                                             JSAMPARRAY output_buf, JDIMENSION output_col){
    jpeg_idct_downscale_wrap_islow_fast_generic(cinfo, compptr, coef_block, output_buf, output_col, 3, DEFAULT_WEIGHTED_SUM(3, jpeg_scale_to_3_x_3_weights));
}

void jpeg_idct_downscale_wrap_islow_fast_4x4(j_decompress_ptr cinfo, jpeg_component_info * compptr, JCOEFPTR coef_block,
                                             JSAMPARRAY output_buf, JDIMENSION output_col){
    jpeg_idct_downscale_wrap_islow_fast_generic(cinfo, compptr, coef_block, output_buf, output_col, 4, DEFAULT_WEIGHTED_SUM(4, jpeg_scale_to_4_x_4_weights));
}

void jpeg_idct_downscale_wrap_islow_fast_5x5(j_decompress_ptr cinfo, jpeg_component_info * compptr, JCOEFPTR coef_block,
                                             JSAMPARRAY output_buf, JDIMENSION output_col){
    jpeg_idct_downscale_wrap_islow_fast_generic(cinfo, compptr, coef_block, output_buf, output_col, 5, DEFAULT_WEIGHTED_SUM(5, jpeg_scale_to_5_x_5_weights));
}

void jpeg_idct_downscale_wrap_islow_fast_6x6(j_decompress_ptr cinfo, jpeg_component_info * compptr, JCOEFPTR coef_block,
                                             JSAMPARRAY output_buf, JDIMENSION output_col){
    jpeg_idct_downscale_wrap_islow_fast_generic(cinfo, compptr, coef_block, output_buf, output_col, 6, DEFAULT_WEIGHTED_SUM(6, jpeg_scale_to_6_x_6_weights));
}

void jpeg_idct_downscale_wrap_islow_fast_7x7(j_decompress_ptr cinfo, jpeg_component_info * compptr, JCOEFPTR coef_block,
                                             JSAMPARRAY output_buf, JDIMENSION output_col){

    jpeg_idct_downscale_wrap_islow_fast_generic(cinfo, compptr, coef_block, output_buf, output_col, 7, DEFAULT_WEIGHTED_SUM(7, jpeg_scale_to_7_x_7_weights));
}


// void jpeg_idct_downscale_wrap_islow_fast(j_decompress_ptr cinfo, jpeg_component_info * compptr, JCOEFPTR coef_block,
//                                              JSAMPARRAY output_buf, JDIMENSION output_col){

// #if JPEG_LIB_VERSION >= 70
//     int scaled = compptr->DCT_h_scaled_size;
// #else
//     int scaled = compptr->DCT_scaled_size;
// #endif


//     jpeg_idct_downscale_wrap_islow_fast_generic(cinfo, compptr, coef_block, output_buf, output_col, scaled, DEFAULT_WEIGHTED_SUM(scaled,&weights_by_target[scaled - 1]));
// }

