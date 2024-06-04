#include <stdio.h>
#include <stdint.h>

#include "ntt.h"
#include "reduce.h"
#include "randombytes.h"
// #include "symmetric.h"

int16_t abc[1] = {0};
const int16_t zetas[128] = {
  2285, 2571, 2970, 1812, 1493, 1422, 287, 202, 3158, 622, 1577, 182, 962,
  2127, 1855, 1468, 573, 2004, 264, 383, 2500, 1458, 1727, 3199, 2648, 1017,
  732, 608, 1787, 411, 3124, 1758, 1223, 652, 2777, 1015, 2036, 1491, 3047,
  1785, 516, 3321, 3009, 2663, 1711, 2167, 126, 1469, 2476, 3239, 3058, 830,
  107, 1908, 3082, 2378, 2931, 961, 1821, 2604, 448, 2264, 677, 2054, 2226,
  430, 555, 843, 2078, 871, 1550, 105, 422, 587, 177, 3094, 3038, 2869, 1574,
  1653, 3083, 778, 1159, 3182, 2552, 1483, 2727, 1119, 1739, 644, 2457, 349,
  418, 329, 3173, 3254, 817, 1097, 603, 610, 1322, 2044, 1864, 384, 2114, 3193,
  1218, 1994, 2455, 220, 2142, 1670, 2144, 1799, 2051, 794, 1819, 2475, 2459,
  478, 3221, 3021, 996, 991, 958, 1869, 1522, 1628
};

const int16_t zetas_inv[128] = {
  1701, 1807, 1460, 2371, 2338, 2333, 308, 108, 2851, 870, 854, 1510, 2535,
  1278, 1530, 1185, 1659, 1187, 3109, 874, 1335, 2111, 136, 1215, 2945, 1465,
  1285, 2007, 2719, 2726, 2232, 2512, 75, 156, 3000, 2911, 2980, 872, 2685,
  1590, 2210, 602, 1846, 777, 147, 2170, 2551, 246, 1676, 1755, 460, 291, 235,
  3152, 2742, 2907, 3224, 1779, 2458, 1251, 2486, 2774, 2899, 1103, 1275, 2652,
  1065, 2881, 725, 1508, 2368, 398, 951, 247, 1421, 3222, 2499, 271, 90, 853,
  1860, 3203, 1162, 1618, 666, 320, 8, 2813, 1544, 282, 1838, 1293, 2314, 552,
  2677, 2106, 1571, 205, 2918, 1542, 2721, 2597, 2312, 681, 130, 1602, 1871,
  829, 2946, 3065, 1325, 2756, 1861, 1474, 1202, 2367, 3147, 1752, 2707, 171,
  3127, 3042, 1907, 1836, 1517, 359, 758, 1441
};

uint8_t multi_ordered_array[7][128] = {
  {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127}
};

uint8_t single_ordered_array[128] = 
  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127};

static const uint8_t index_array[8][128] = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127},
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191},
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223},
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239},
    {0, 1, 2, 3, 4, 5, 6, 7, 16, 17, 18, 19, 20, 21, 22, 23, 32, 33, 34, 35, 36, 37, 38, 39, 48, 49, 50, 51, 52, 53, 54, 55, 64, 65, 66, 67, 68, 69, 70, 71, 80, 81, 82, 83, 84, 85, 86, 87, 96, 97, 98, 99, 100, 101, 102, 103, 112, 113, 114, 115, 116, 117, 118, 119, 128, 129, 130, 131, 132, 133, 134, 135, 144, 145, 146, 147, 148, 149, 150, 151, 160, 161, 162, 163, 164, 165, 166, 167, 176, 177, 178, 179, 180, 181, 182, 183, 192, 193, 194, 195, 196, 197, 198, 199, 208, 209, 210, 211, 212, 213, 214, 215, 224, 225, 226, 227, 228, 229, 230, 231, 240, 241, 242, 243, 244, 245, 246, 247},
    {0, 1, 2, 3, 8, 9, 10, 11, 16, 17, 18, 19, 24, 25, 26, 27, 32, 33, 34, 35, 40, 41, 42, 43, 48, 49, 50, 51, 56, 57, 58, 59, 64, 65, 66, 67, 72, 73, 74, 75, 80, 81, 82, 83, 88, 89, 90, 91, 96, 97, 98, 99, 104, 105, 106, 107, 112, 113, 114, 115, 120, 121, 122, 123, 128, 129, 130, 131, 136, 137, 138, 139, 144, 145, 146, 147, 152, 153, 154, 155, 160, 161, 162, 163, 168, 169, 170, 171, 176, 177, 178, 179, 184, 185, 186, 187, 192, 193, 194, 195, 200, 201, 202, 203, 208, 209, 210, 211, 216, 217, 218, 219, 224, 225, 226, 227, 232, 233, 234, 235, 240, 241, 242, 243, 248, 249, 250, 251},
    {0, 1, 4, 5, 8, 9, 12, 13, 16, 17, 20, 21, 24, 25, 28, 29, 32, 33, 36, 37, 40, 41, 44, 45, 48, 49, 52, 53, 56, 57, 60, 61, 64, 65, 68, 69, 72, 73, 76, 77, 80, 81, 84, 85, 88, 89, 92, 93, 96, 97, 100, 101, 104, 105, 108, 109, 112, 113, 116, 117, 120, 121, 124, 125, 128, 129, 132, 133, 136, 137, 140, 141, 144, 145, 148, 149, 152, 153, 156, 157, 160, 161, 164, 165, 168, 169, 172, 173, 176, 177, 180, 181, 184, 185, 188, 189, 192, 193, 196, 197, 200, 201, 204, 205, 208, 209, 212, 213, 216, 217, 220, 221, 224, 225, 228, 229, 232, 233, 236, 237, 240, 241, 244, 245, 248, 249, 252, 253}
};
static const uint8_t twiddle_factor_index_array[8][128] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
    {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
    {8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15},
    {16, 16, 16, 16, 16, 16, 16, 16, 17, 17, 17, 17, 17, 17, 17, 17, 18, 18, 18, 18, 18, 18, 18, 18, 19, 19, 19, 19, 19, 19, 19, 19, 20, 20, 20, 20, 20, 20, 20, 20, 21, 21, 21, 21, 21, 21, 21, 21, 22, 22, 22, 22, 22, 22, 22, 22, 23, 23, 23, 23, 23, 23, 23, 23, 24, 24, 24, 24, 24, 24, 24, 24, 25, 25, 25, 25, 25, 25, 25, 25, 26, 26, 26, 26, 26, 26, 26, 26, 27, 27, 27, 27, 27, 27, 27, 27, 28, 28, 28, 28, 28, 28, 28, 28, 29, 29, 29, 29, 29, 29, 29, 29, 30, 30, 30, 30, 30, 30, 30, 30, 31, 31, 31, 31, 31, 31, 31, 31},
    {32, 32, 32, 32, 33, 33, 33, 33, 34, 34, 34, 34, 35, 35, 35, 35, 36, 36, 36, 36, 37, 37, 37, 37, 38, 38, 38, 38, 39, 39, 39, 39, 40, 40, 40, 40, 41, 41, 41, 41, 42, 42, 42, 42, 43, 43, 43, 43, 44, 44, 44, 44, 45, 45, 45, 45, 46, 46, 46, 46, 47, 47, 47, 47, 48, 48, 48, 48, 49, 49, 49, 49, 50, 50, 50, 50, 51, 51, 51, 51, 52, 52, 52, 52, 53, 53, 53, 53, 54, 54, 54, 54, 55, 55, 55, 55, 56, 56, 56, 56, 57, 57, 57, 57, 58, 58, 58, 58, 59, 59, 59, 59, 60, 60, 60, 60, 61, 61, 61, 61, 62, 62, 62, 62, 63, 63, 63, 63},
    {64, 64, 65, 65, 66, 66, 67, 67, 68, 68, 69, 69, 70, 70, 71, 71, 72, 72, 73, 73, 74, 74, 75, 75, 76, 76, 77, 77, 78, 78, 79, 79, 80, 80, 81, 81, 82, 82, 83, 83, 84, 84, 85, 85, 86, 86, 87, 87, 88, 88, 89, 89, 90, 90, 91, 91, 92, 92, 93, 93, 94, 94, 95, 95, 96, 96, 97, 97, 98, 98, 99, 99, 100, 100, 101, 101, 102, 102, 103, 103, 104, 104, 105, 105, 106, 106, 107, 107, 108, 108, 109, 109, 110, 110, 111, 111, 112, 112, 113, 113, 114, 114, 115, 115, 116, 116, 117, 117, 118, 118, 119, 119, 120, 120, 121, 121, 122, 122, 123, 123, 124, 124, 125, 125, 126, 126, 127, 127}
};


uint8_t ordered_array[128] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44,
    45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
    70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94,
    95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115,
    116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127};

unsigned char random_buffer[RANDOM_BUFFER_SIZE_NEW];
unsigned int total_rand_bytes_remaining = 0;
unsigned char seed[32];

uint8_t dummy(uint8_t s1,uint8_t s2){
  return s1+s2;
}

// A function to generate a random
// permutation of arr[]
static void randomize (unsigned char arr[], int n)
{
  unsigned char temp;

    // Start from the last element and swap
    // one by one. We don't need to run for
    // the first element that's why i > 0

    for (int i = n - 1; i > 0; i--)
    {
        if(total_rand_bytes_remaining <= 0)
        {

          #if(CALC_NTT_RAND_TIME_RAND == 1)
            t0_ntt = hal_get_time();
          #endif

          randombytes(random_buffer,RANDOM_BUFFER_SIZE_NEW);
          // prf(random_buffer, RANDOM_BUFFER_SIZE_NEW, seed, 0);
          total_rand_bytes_remaining = RANDOM_BUFFER_SIZE_NEW;

          #if(CALC_NTT_RAND_TIME_RAND == 1)

            t1_ntt = hal_get_time();
            randomize_ntt_time = randomize_ntt_time + t1_ntt - t0_ntt;

          #endif
        }
        temp = random_buffer[RANDOM_BUFFER_SIZE_NEW-total_rand_bytes_remaining];
        total_rand_bytes_remaining = total_rand_bytes_remaining-1;

        // Pick a random index from 0 to i
        int j = temp % (i + 1);

        // Swap arr[i] with the element
        // at random iRANDOM_BUFFER_SIZEndex
        temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

/*************************************************
* Name:        fqmul
*
* Description: Multiplication followed by Montgomery reduction
*
* Arguments:   - int16_t a: first factor
*              - int16_t b: second factor
*
* Returns 16-bit integer congruent to a*b*R^{-1} mod q
**************************************************/
static int16_t fqmul(int16_t a, int16_t b) {
  return montgomery_reduce((int32_t)a*b);
}

/*************************************************
* Name:        ntt
*
* Description: Inplace number-theoretic transform (NTT) in Rq
*              input is in standard order, output is in bitreversed order
*
* Arguments:   - int16_t r[256]: pointer to input/output vector of elements
*                                of Zq
**************************************************/

#if (NTT_PROTECTION_MODE == NO_PROTECTION)

void ntt(int16_t r[256])
{
  asm volatile(
    "addi t5, x0, 1\n"
    "li t3, 0x82001000\n"
    "sw t5, 0(t3)\n"
  );
  unsigned int len, start, j, k;
  int16_t t, zeta;

  k = 1;
  // int i = 0;
  for(len = 128; len >= 2; len >>= 1)
  {
    for(start = 0; start < 256; start = j + len)
    {
      // printf("k: %d\n",k);
      zeta = zetas[k++];
      for(j = start; j < start + len; j++)
      {
        // Loading r[j+len]
        // printf("%d %d\n",len,start);
        t = fqmul(zeta, r[j + len]);
        // i++;
        r[j + len] = r[j] - t;
        r[j] = r[j] + t;
      }
    }
  }
  asm volatile(
    "li t3, 0x82001000\n"
    "sw x0, 0(t3)\n"
    
  );
  // printf("k:%d",i);
}

void ntt_h(int16_t r[256]){
  unsigned int len, start, j, k;
  int16_t t, zeta;

  k = 1;
  for(len = 128; len >= 2; len >>= 1)
  {
    for(start = 0; start < 256; start = j + len)
    {
      zeta = zetas[k++];
      for(j = start; j < start + len; j++)
      {
        // Loading r[j+len]
        t = fqmul(zeta, r[j + len]);
        r[j + len] = r[j] - t;
        r[j] = r[j] + t;
      }
    }
  }
}

#elif (NTT_PROTECTION_MODE == GROUP_COARSE_SHUFFLING)

// Coarse shuffing within groups...

void __attribute__ ((noinline)) ntt(int16_t r[256])
{
  unsigned int len, start, j, k;
  int16_t t, zeta;
  uint8_t len1,temp;

  // #if(CALC_NTT_RAND_TIME == 1 || CALC_NTT_RAND_TIME_RAND == 1)
  //   randomize_ntt_time = 0;
  // #endif
  int i = 0;
  multi_ordered_array[0][0] = 0;
  multi_ordered_array[0][1] = 1;
  k = 1;
  for(len = 7; len >= 1; len--)
  {
    len1 = (1 << len);
    for(start = 0; start < 256; start = j + len1)
    {
      zeta = zetas[k++];
      // #if(CALC_NTT_RAND_TIME == 1)
      // t0_ntt = hal_get_time();
      // #endif
      // printf("%d %d\n",len-1,len1);
      // i++;
      randomize(&(multi_ordered_array[len-1][0]), len1);
      // #if(CALC_NTT_RAND_TIME == 1)
      // t1_ntt = hal_get_time();
      // randomize_ntt_time = randomize_ntt_time + t1_ntt - t0_ntt;
      // #endif 

      for(j = start; j < start + len1; ++j)
      {
        temp = multi_ordered_array[len-1][j-start] + start;

        t = fqmul(zeta, r[temp + len1]);
        r[temp + len1] = r[temp] - t;
        r[temp] = r[temp] + t;
      }
    }
  }
  // printf("i: %d\n",i);
}

void ntt_h(int16_t r[256])
{
  unsigned int len, start, j, k;
  int16_t t, zeta;
  uint8_t len1,temp,tt,index;

  // #if(CALC_NTT_RAND_TIME == 1 || CALC_NTT_RAND_TIME_RAND == 1)
  //   randomize_ntt_time = 0;
  // #endif
  int i = 0;
  // multi_ordered_array[0][0] = 0;
  // multi_ordered_array[0][1] = 1;
  k = 1;
  for(len = 7; len >= 1; len--)
  {
    len1 = (1 << len);
    index = len1;
    // index = 64;
      // printf("len: %d len1: %d index: %d\n",len,len1,index);
    for(start = 0; start < 128; start = start + len1)
    {
      index = ((k+1)>>(8-len))?index>>1:index;
    // printf("k,%d index,%d 9-len, %d %d\n",k,index, (7-len),((k+1)>>(8-len)));
      printf("index: %d\n",index);
      // printf("start: %d\n",start);
      zeta = zetas[k++];

      // #if(CALC_NTT_RAND_TIME == 1)
      // t0_ntt = hal_get_time();
      // #endif
      // printf("len-1: %d len1: %d\n",len-1,len1);
      // i++;
      // randomize(&(multi_ordered_array[len-1][0]), len1);
      // #if(CALC_NTT_RAND_TIME == 1)
      // t1_ntt = hal_get_time();
      // randomize_ntt_time = randomize_ntt_time + t1_ntt - t0_ntt;
      // #endif 
      // j = start;
      printf("start: %d, len1: %d\n",start,len1);
      asm volatile(
        // "la t6, multi_ordered_array\n"
        // "addi t5, %[len], -1\n"
        // "li t4, 128\n"
        // "mul t5, t5, t4\n"
        // "add t6, t6, t5\n"
        // "add t4, %[j],%[start]\n"
        "update.index %[index]\n"
        "hwlp.count x0,%[len1]\n"
        "hwlp.starti x0,nttstart\n"
        "hwlp.endi x0,nttend\n"
        // "mv %[j],%[start]\n"
        "nttstart:;\n"
        "addtoba %[tt],x0,x0\n"
        "nop\n"
        :[len1] "+r" (len1),
        // [len] "+r" (len)
        // [start] "+r" (start)
        // [j] "+r" (j),
        // [j] "=r" (j),
        // [start] "+r" (start),
        [index] "+r" (index),
        [tt] "=r" (tt)
      );
      // for(j = start; j < start + len1; ++j)
      // {
        printf("%d\n",tt);
        // tt = multi_ordered_array[len-1][j-start];
        temp = tt + start*2;
        i++;
        t = fqmul(zeta, r[temp + len1]);
        r[temp + len1] = r[temp] - t;
        r[temp] = r[temp] + t;
        // j = j+1;
        asm volatile(
          // "addi %[j],%[j],1\n"
          "nttend:;\n"
          "nop\n"
          "mv %[j],%[len1]"
          :[j] "=r" (j),
          [len1] "+r" (len1)
        );
      
      // }
      // printf("start: %d\n",start);
    }
  }
  printf("i: %d k: %d\n",i,k);
  // printf("i: %d\n",i);
}
void ntt_h2(int16_t r[256]){
  unsigned int len, start, j, k;
  int16_t t, zeta;
  uint8_t len1,temp,tt;
  uint8_t index;
  
  multi_ordered_array[0][0] = 0;
  multi_ordered_array[0][1] = 1;
  k = 1;
  for (len = 0; len < 7; len++){
    len1 = (128 >> len);
    index = len1 >> 1;
    for (start = 0; start < (1<<len); start++){
      zeta = zetas[k++];
      // printf("%d\n",zeta);
      // printf("start: %d k: %d\n",start*len1*2,k);
      // randomize(&(multi_ordered_array[6-len][0]), len1);
      // for (j = 0; j < len1; j++){
        tt = start*len1*2;
        asm volatile(
        "update.index %[index]\n"
        "hwlp.count x0,%[len1]\n"
        "hwlp.starti x0,nttstart\n"
        "hwlp.endi x0,nttend\n"      
        // "add %[j], x0, x0\n"
        "nttstart:;\n"
        "addtoba %[temp],x0,x0\n"
        "nop\n"    
        :[len1] "+r" (len1),
        [temp] "+r" (temp),
        [index] "+r" (index)
        );
        printf("temp: %d, len1: %d",temp,len1);
        // temp = multi_ordered_array[6-len][j];
        // printf("start: %d",start);
        
        temp = temp + tt;

        t = fqmul(zeta, r[temp + len1]);
        r[temp + len1] = r[temp] - t;
        r[temp] = r[temp] + t;

        asm volatile(
          "nop\n"
          // "addi %[j],%[j],1\n"
          "nttend:;\n"
          "nop\n"
          // :[j] "=r" (j)
        );        
      // }
    }
    
  }
}
void __attribute__ ((noinline)) ntt_h3(int16_t r[256])
{
  unsigned int len, start, j, k;
  int16_t t, zeta;
  uint8_t len1,temp,tt,index;

  // #if(CALC_NTT_RAND_TIME == 1 || CALC_NTT_RAND_TIME_RAND == 1)
  //   randomize_ntt_time = 0;
  // #endif
  k = 1;
  for(len = 7; len >= 1; len--)
  {
    len1 = (1 << len);
    index = len1;
    for(start = 0; start < 128; start = start + len1)
    {
      index = ((k+1)>>(8-len))?index>>1:index;
    
      zeta = zetas[k++];

      // #if(CALC_NTT_RAND_TIME == 1)
      // t0_ntt = hal_get_time();
      // #endif
      // #if(CALC_NTT_RAND_TIME == 1)
      // t1_ntt = hal_get_time();
      // randomize_ntt_time = randomize_ntt_time + t1_ntt - t0_ntt;
      // #endif 
        // printf("%d\n",start*2);
      asm volatile(
        "update.index %[index]\n"
        "hwlp.count x0,%[len1]\n"
        "hwlp.starti x0,nttstart\n"
        "hwlp.endi x0,nttend\n"
        "nttstart:;\n"
        "addtoba %[tt],x0,x0\n"
        :[len1] "+r" (len1),
        [index] "+r" (index),
        [tt] "=r" (tt)
      );
        // printf("here2");
        temp = tt + start*2;
        // printf("%d\n",temp);
        t = fqmul(zeta, r[temp + len1]);
        r[temp + len1] = r[temp] - t;
        r[temp] = r[temp] + t;
        
        asm volatile(
          "nttend:;\n"
          "nop\n"
          "mv %[j],%[len1]"
          :[j] "=r" (j),
          [len1] "+r" (len1)
        );
     }
  }
  // printf("%d\n",i);
}
#elif (NTT_PROTECTION_MODE == GROUP_SHUFFLING)

void ntt(int16_t r[256])
{
  #if(CALC_NTT_RAND_TIME == 1 || CALC_NTT_RAND_TIME_RAND == 1)
    randomize_ntt_time = 0;
  #endif
  multi_ordered_array[0][0] = 0;
  multi_ordered_array[0][1] = 1;
      
  unsigned int len, start, j, k, g;
  int16_t t, zeta;
  uint8_t len1,temp,no_groups,row_no,stage;
  int i = 0;
  stage = 0;
  for(len = 7; len >= 1; len--)
  {
    len1 = (1 << len); 
    no_groups = 128/len1;
    row_no = 6 - len;
    if(no_groups > 1)
    {
      #if(CALC_NTT_RAND_TIME == 1)
      t0_ntt = hal_get_time();
      #endif
      // printf("row_no: %d no_groups: %d\n",row_no,no_groups);
      randomize(&(multi_ordered_array[row_no][0]), no_groups);
      // i++;
      #if(CALC_NTT_RAND_TIME == 1)
      t1_ntt = hal_get_time(); 
      randomize_ntt_time = randomize_ntt_time + t1_ntt - t0_ntt;
      #endif

      for(g = 0; g < no_groups; g++)
      {
        start = (multi_ordered_array[row_no][g])*2*len1;
        zeta = zetas[twiddle_factor_index_array[stage][start/2]];
        for(j = start; j < start + len1; ++j)
        {
          // printf("%d %d\n",g, j);
          t = fqmul(zeta, r[j + len1]);
          r[j + len1] = r[j] - t;
          r[j] = r[j] + t;
        }
      }
  
    }

    else
    {

      #if(CALC_NTT_RAND_TIME == 1)
      t0_ntt = hal_get_time();
      #endif
      randomize(ordered_array, 128);
      // i++;
      #if(CALC_NTT_RAND_TIME == 1)
      t1_ntt = hal_get_time();
      randomize_ntt_time = randomize_ntt_time + t1_ntt - t0_ntt;
      #endif

      for(start = 0; start < 128; start++)
      {
        temp = ordered_array[start];
        zeta = zetas[twiddle_factor_index_array[stage][temp]];
        temp = index_array[stage][temp];
        t = fqmul(zeta, r[temp + len1]);
        r[temp + len1] = r[temp] - t;
        r[temp] = r[temp] + t;
      }
    }
    stage = stage+1;
  }
  // printf("i: %d\n",i);
}

#elif (NTT_PROTECTION_MODE == COARSE_SHUFFLING)

// Coarse Shuffling...

void __attribute__ ((noinline)) ntt(int16_t r[256])
{
  // asm volatile(
  //   "addi t5, x0, 1\n"
  //   "li t3, 0x82001000\n"
  //   "sw t5, 0(t3)\n"
  // );
  unsigned int len, start, j, k;
  int16_t t, zeta;
  uint8_t temp;
  uint8_t stage;

  k = 1;

  stage = 0;
  for(len = 128; len >= 2; len >>= 1)
  {
    randomize(&multi_ordered_array[6][0], 128);

    for(start = 0; start < 128; start++)
    {
      temp = multi_ordered_array[6][start];
      zeta = zetas[twiddle_factor_index_array[stage][temp]];
      temp = index_array[stage][temp];
      // printf("%d %d\n",len,start);
      t = fqmul(zeta, r[temp + len]);
      // k++;
      r[temp + len] = r[temp] - t;
      r[temp] = r[temp] + t;
      // printf("start: %d\n",start);
    }
    stage++;
  }
  // asm volatile(
  //   "li t3, 0x82001000\n"
  //   "sw x0, 0(t3)\n"
    
  // );
}

void ntt_h(int16_t r[256])
{
  unsigned int len, start, j;
  int16_t t, zeta;
  uint8_t temp;
  uint8_t stage;


  stage = 0;
  for(len = 128; len >= 2; len >>= 1)
  {
    
      asm volatile(
        "hwlp.counti x0, 128\n"
    	  "hwlp.starti x0, nttstart\n"
    	  "hwlp.endi   x0, nttend\n"
        "nttstart:;\n"
        // "nop\n"
        "addtoba %[temp], x0, x0\n"
        : [temp]"=r" (temp)
      );
      // temp=start;
      zeta = zetas[twiddle_factor_index_array[stage][temp]];
      temp = index_array[stage][temp];

      t = fqmul(zeta, r[temp + len]);
      r[temp + len] = r[temp] - t;
      r[temp] = r[temp] + t;
      asm volatile(
        "nttend:;\n"
		    "nop\n"
      );
      // printf("%d\n",temp);
    stage++;
  }

}
void ntt_h2(int16_t r[256])
{
  unsigned int len;
  int16_t t, zeta;
  uint8_t temp;
  uint8_t stage;



  stage = 0;
  // for(len = 128; len >= 2; len >>= 1)
  asm volatile(
    ".balign 4;"
    "hwlp.counti x1, 7\n"
    "hwlp.starti x1, nttstartout\n"
    "hwlp.endi   x1, nttendout\n"
    "mv t6, x0\n"
    "li t5, 128\n"
    "nttstartout:;\n"
    "mv %[len], t5\n"
    "mv %[stage], t6\n"
    "nop\n"
    :[stage] "=r"(stage), [len] "=r" (len)
  );
  // {
    // randomize(&multi_ordered_array[6][0], 128);
    
      asm volatile(
        ".balign 4;"
        "hwlp.counti x0, 128\n"
    	  "hwlp.starti x0, nttstart2\n"
    	  "hwlp.endi   x0, nttend2\n"
        "nttstart2:;\n"
        // "nop\n"
        "addtoba %[temp], x0, x0\n"
        : [temp]"=r" (temp)
      );
      // temp=start;
      // printf("%x\n",len);
      zeta = zetas[twiddle_factor_index_array[stage][temp]];
      temp = index_array[stage][temp];
      t = fqmul(zeta, r[temp + len]);
      r[temp + len] = r[temp] - t;
      r[temp] = r[temp] + t;
      asm volatile(
        "nttend2:;\n"
		    "nop\n"
      );
      asm volatile(
        "addi t6, t6, 1\n"
        "srli t5, t5, 1\n"
        "nttendout:;\n"
        "nop\n"
      );
      // printf("%d\n",temp);
    // stage++;
  // }

}
void __attribute__ ((noinline)) ntt_h3(int16_t r[256])
{
  asm volatile(
    "addi t5, x0, 1\n"
    "li t3, 0x82001000\n"
    "sw t5, 0(t3)\n"
  );
  unsigned int len, start, j;
  int16_t t, zeta;
  uint8_t temp;
  uint8_t stage;


  stage = 0;
  for(len = 128; len >= 2; len >>= 1)
  { 
    
      asm volatile(
        "update.perm\n"
        "hwlp.counti x0, 128\n"
    	  "hwlp.starti x0, nttstart3\n"
    	  "hwlp.endi   x0, nttend3\n"
        "nttstart3:;\n"
        "addtoba %[temp], x0, x0\n"
        : [temp]"=r" (temp)
      );
      // printf("%d\n",temp);
      zeta = zetas[twiddle_factor_index_array[stage][temp]];
      temp = index_array[stage][temp];

      t = fqmul(zeta, r[temp + len]);
      r[temp + len] = r[temp] - t;
      r[temp] = r[temp] + t;
      asm volatile(
        "nttend3:;\n"
		    "nop\n"
      );
    stage++;
  }
  asm volatile(
    "li t3, 0x82001000\n"
    "sw x0, 0(t3)\n"
    
  );
}
// void ntt_h4(int16_t r[256])
// {
//   unsigned int len, start, j;
//   int16_t t, zeta;
//   uint8_t temp;
//   uint8_t stage;
//   uint8_t d;


//   stage = 0;
//   for(len = 128; len >= 2; len >>= 1)
//   { 
//       // d = dummy(stage,stage);
    
//       asm volatile(
//         "hwlp.counti x0, 128\n"
//     	  "hwlp.starti x0, nttstart3%0=\n"
//     	  "hwlp.endi   x0, nttend3%0=\n"
//         "nttstart3%0=:;\n"
//         // "nop\n"
//         "addtoba %[temp], x0, x0\n"
//         : [temp]"=r" (temp) 
//       );
//       // temp=start;
//       zeta = zetas[twiddle_factor_index_array[stage][temp]];
//       temp = index_array[stage][temp];

//       t = fqmul(zeta, r[temp + len]);
//       r[temp + len] = r[temp] - t;
//       r[temp] = r[temp] + t;
//       asm volatile(
//         "nttend3%0=:;\n"
// 		    "nop\n"
//       ); 
//       // printf("%d\n",temp);
//     stage++;
//   }

// }
#elif (NTT_PROTECTION_MODE == FINE_SHUFFLING)

// Fine Shulffing...

void ntt(int16_t r[256])
{
  #if(CALC_NTT_RAND_TIME == 1 || CALC_NTT_RAND_TIME_RAND == 1)
    randomize_ntt_time = 0;
  #endif

  unsigned int len, start, j, k;
  int16_t t, zeta;
  int16_t op1, op2, temp, out1, out2, temp_new;
  uint8_t randi, bitrand;
  int16_t mask;

  k = 1;
  int count = 32;

  total_rand_bytes_remaining = 0;
  count = 32;

  int16_t op1_new, op2_new;
  uint8_t op1_new_bit, op2_new_bit, bit;

  for(len = 128; len >= 2; len >>= 1)
  {
    for(start = 0; start < 256; start = j + len)
    {
      zeta = zetas[k++];
      for(j = start; j < start + len; ++j)
      {

        if(count == 32)
        {

          #if(CALC_NTT_RAND_TIME_RAND == 1)
            t0_ntt = hal_get_time();
          #endif

          randombytes(random_buffer,RANDOM_BUFFER_SIZE);
          total_rand_bytes_remaining = RANDOM_BUFFER_SIZE;

          #if(CALC_NTT_RAND_TIME_RAND == 1)

            t1_ntt = hal_get_time();
            randomize_ntt_time = randomize_ntt_time + t1_ntt - t0_ntt;

          #endif

          count = 0;
        }

        randi = random_buffer[RANDOM_BUFFER_SIZE - total_rand_bytes_remaining];

        if(count > 0 && (count & 0xFF) == 0)
          total_rand_bytes_remaining = total_rand_bytes_remaining-1;

        bitrand = randi>>(count&0x7)&0x1;

        op1 = r[j+bitrand*len];
        op2 = r[j+(1-bitrand)*len];

        #if(CALC_NTT_RAND_TIME == 1)
        t0_ntt = hal_get_time();
        #endif

        count = count+1;
        temp = (op1 ^ op2);

        temp_new = 0;

        for(int cc = 0; cc < 16; cc++)
        {
          bit = ((temp >> cc)) & bitrand;
          temp_new |= (bit << cc);
        }

        op1 = op1 ^ temp_new;
        op2 = op2 ^ temp_new;

        #if(CALC_NTT_RAND_TIME == 1)
        t1_ntt = hal_get_time();
        randomize_ntt_time = randomize_ntt_time + t1_ntt - t0_ntt;
        #endif

        t = fqmul(zeta, op2);
        out1 = op1 + t;
        out2 = op1 - t;


        if(count == 32)
        {

          #if(CALC_NTT_RAND_TIME_RAND == 1)
            t0_ntt = hal_get_time();
          #endif

          randombytes(random_buffer,RANDOM_BUFFER_SIZE);
          total_rand_bytes_remaining = RANDOM_BUFFER_SIZE;

          #if(CALC_NTT_RAND_TIME_RAND == 1)

            t1_ntt = hal_get_time();
            randomize_ntt_time = randomize_ntt_time + t1_ntt - t0_ntt;

          #endif

          count = 0;
        }

        randi = random_buffer[RANDOM_BUFFER_SIZE - total_rand_bytes_remaining];

        if(count > 0 && (count & 0xFF) == 0)
          total_rand_bytes_remaining = total_rand_bytes_remaining-1;

        #if(CALC_NTT_RAND_TIME == 1)
        t0_ntt = hal_get_time();
        #endif

        bitrand = randi>>(count&0x7)&0x1;
        count = count+1;
        temp = (out1 ^ out2);

        temp_new = 0;

        for(int cc = 0; cc < 16; cc++)
        {
          bit = ((temp >> cc)) & bitrand;
          temp_new |= (bit << cc);
        }

        out1 = out1 ^ temp_new;
        out2 = out2 ^ temp_new;

        #if(CALC_NTT_RAND_TIME == 1)
        t1_ntt = hal_get_time();
        randomize_ntt_time = randomize_ntt_time + t1_ntt - t0_ntt;
        #endif

        r[j+(bitrand)*len] = out1;
        r[j+(1-bitrand)*len] = out2;

      }
    }
  }
}

#elif (NTT_PROTECTION_MODE == COARSE_MULTIPLICATIVE_MASKING)

// Coarse Multiplicative masking...

void ntt(int16_t r[256])
{
  #if(CALC_NTT_RAND_TIME == 1 || CALC_NTT_RAND_TIME_RAND == 1)
    randomize_ntt_time = 0;
  #endif

  unsigned int len, start, j, k;
  int16_t t, zeta, zeta1, zeta2, t1, t2;
  uint8_t temp;
  uint8_t stage = 0;

  k = 1;

  uint8_t random_index;

  uint8_t random_twiddle_exponent[7]; //Generating random exponents for each stage...
  uint16_t sum_random_twiddle_exponent = 0;

  uint8_t index1, index2;

  for(int i=0;i<(NO_STAGES-1);i++)
  {

    if(total_rand_bytes_remaining <= 0)
    {

      #if(CALC_NTT_RAND_TIME_RAND == 1)
        t0_ntt = hal_get_time();
      #endif

      randombytes(random_buffer,RANDOM_BUFFER_SIZE);
      total_rand_bytes_remaining = RANDOM_BUFFER_SIZE;

      #if(CALC_NTT_RAND_TIME_RAND == 1)

        t1_ntt = hal_get_time();
        randomize_ntt_time = randomize_ntt_time + t1_ntt - t0_ntt;

      #endif
    }

    random_index = random_buffer[RANDOM_BUFFER_SIZE - total_rand_bytes_remaining];
    total_rand_bytes_remaining = total_rand_bytes_remaining-1;

    random_twiddle_exponent[i] = (random_index)&((KYBER_N)-1);
    sum_random_twiddle_exponent += random_twiddle_exponent[i];
  }

  random_twiddle_exponent[6] = (KYBER_N) - (sum_random_twiddle_exponent&((KYBER_N)-1));

  k = 1;
  for(len = 128; len >= 2; len >>= 1)
  {
    for(start = 0; start < 256; start = j + len)
    {
      index1 = (bitrev_order[k] + random_twiddle_exponent[stage])&0xFF; // Correct...
      index2 = random_twiddle_exponent[stage];

      zeta1 = zetas_normal_order[index1];
      zeta2 = zetas_normal_order[index2];

      for(j = start; j < start + len; ++j)
      {
        t1 = fqmul(zeta1, r[j + len]);
        t2 = fqmul(zeta2, r[j]);
        r[j + len] = t2 - t1;
        r[j] = t2 + t1;
      }
      k = k+1;
    }
    stage = stage+1;
  }
}

#elif (NTT_PROTECTION_MODE == FINE_MULTIPLICATIVE_MASKING)

// Fine Multiplicative masking...

void ntt(int16_t r[256])
{
  #if(CALC_NTT_RAND_TIME == 1 || CALC_NTT_RAND_TIME_RAND == 1)
    randomize_ntt_time = 0;
  #endif

  unsigned int len, start, j, k;
  int16_t t, zeta, zeta1, zeta2, zeta3, zeta4, t1, t2, t3, t4;
  uint8_t index1, index2, index3, index4;
  uint8_t temp;
  uint8_t stage = 0;
  uint8_t new_random_twiddle_index_1, new_random_twiddle_index_2;

  k = 1;

  uint8_t random_index, ii1, ii2;

  // Performing side channel resistant Inverse NTT structure with different twiddle factors...
  uint8_t random_twiddle_exponent[KYBER_N]; //Generating random exponents for each stage...

  for(int i=0;i<KYBER_N;i++)
  {
    random_twiddle_exponent[i] = 0;
  }

  k = 1;
  for(len = 128; len >= 2; len >>= 1)
  {
    for(start = 0; start < 256; start = j + len)
    {
      random_index = bitrev_order[k];
      for(j = start; j < start + len; ++j)
      {
        ii1 = 256 - random_twiddle_exponent[j];
        ii2 = 256 - random_twiddle_exponent[j+len];

        if(len == 2)
        {
          index1 = ii1; //A1
          index2 = (random_index + ii2)&0xFF; //B1
          index3 = ii1; //A2
          index4 = (random_index + ii2)&0xFF; //B2
        }
        else
        {

          if(total_rand_bytes_remaining <= 0)
          {
            #if(CALC_NTT_RAND_TIME_RAND == 1)
              t0_ntt = hal_get_time();
            #endif

            randombytes(random_buffer,RANDOM_BUFFER_SIZE);
            total_rand_bytes_remaining = RANDOM_BUFFER_SIZE;

            #if(CALC_NTT_RAND_TIME_RAND == 1)
              t1_ntt = hal_get_time();
              randomize_ntt_time = randomize_ntt_time + t1_ntt - t0_ntt;
            #endif
          }

          random_twiddle_exponent[j] = random_buffer[RANDOM_BUFFER_SIZE - total_rand_bytes_remaining];
          total_rand_bytes_remaining = total_rand_bytes_remaining-1;

          random_twiddle_exponent[j+len] = random_buffer[RANDOM_BUFFER_SIZE - total_rand_bytes_remaining];
          total_rand_bytes_remaining = total_rand_bytes_remaining-1;

          index1 = (ii1 + random_twiddle_exponent[j])&0xFF; //A1
          index2 = (random_index + ii2 + random_twiddle_exponent[j])&0xFF; //B1
          index3 = (ii1 + random_twiddle_exponent[j+len])&0xFF; //A2
          index4 = (random_index + ii2 + random_twiddle_exponent[j+len])&0xFF; //B2
        }

        zeta1 = zetas_normal_order[index1];
        zeta2 = zetas_normal_order[index2];
        zeta3 = zetas_normal_order[index3];
        zeta4 = zetas_normal_order[index4];

        t1 = fqmul(zeta1, r[j]);
        t2 = fqmul(zeta2, r[j+len]);
        t3 = fqmul(zeta3, r[j]);
        t4 = fqmul(zeta4, r[j + len]);

        r[j] = t1 + t2;
        r[j + len] = t3 - t4;

      }
      k = k+1;
    }
  }
}

#elif (NTT_PROTECTION_MODE == GENERIC_MULTIPLICATIVE_MASKING)

void ntt(int16_t r[256])
{

  #if(CALC_NTT_RAND_TIME == 1 || CALC_NTT_RAND_TIME_RAND == 1)
    randomize_ntt_time = 0;
  #endif

  unsigned int len, start, j, k, k_prev;
  int16_t t, zeta, zeta1, zeta2, zeta3, zeta4, t1, t2, t3, t4;
  uint8_t index1, index2, index3, index4;
  uint8_t temp;
  uint8_t stage = 0;
  uint8_t new_random_twiddle_index_1, new_random_twiddle_index_2;
  uint8_t ii1, ii2, ii3;

  k = 1;
  k_prev = 0;

  uint8_t random_index;

  uint8_t random_twiddle_exponent[KYBER_N]; //Generating random exponents for each stage...
  uint8_t new_random_twiddle_exponent[KYBER_N];

  uint8_t no_masks_per_stage_shift = NO_MASKS_IN_STAGE;
  int no_masks_per_stage = (1 << no_masks_per_stage_shift);

  // Lets just ignore the first stage masking for now, as it might add some un-necessary overhead...

  unsigned int max_length_of_group = KYBER_N/(no_masks_per_stage);
  uint8_t max_length_of_group_shift = 8 - no_masks_per_stage_shift;

  uint8_t mask_refresh_no = 0;
  uint8_t mask_no = 0;
  uint8_t mask_j_exponent, mask_j_len_exponent;
  uint8_t mask_j_exponent_prev, mask_j_len_exponent_prev;

  mask_j_exponent = 0x00;
  mask_j_len_exponent = 0x00;
  mask_j_exponent_prev = 0xFF;
  mask_j_len_exponent_prev = 0xFF;

  for(int i=0;i<no_masks_per_stage;i++)
  {
      random_twiddle_exponent[i] = 0;
  }

  k = 1;
  for(len = 128; len >= 2; len >>= 1)
  {
    for(int i=0;i<no_masks_per_stage;i++)
    {
      if(len > 2)
      {
        if(total_rand_bytes_remaining <= 0)
        {
          #if(CALC_NTT_RAND_TIME_RAND == 1)
            t0_ntt = hal_get_time();
          #endif

          randombytes(random_buffer,RANDOM_BUFFER_SIZE);
          total_rand_bytes_remaining = RANDOM_BUFFER_SIZE;

          #if(CALC_NTT_RAND_TIME_RAND == 1)
            t1_ntt = hal_get_time();
            randomize_ntt_time = randomize_ntt_time + t1_ntt - t0_ntt;
          #endif
        }

        random_index = random_buffer[RANDOM_BUFFER_SIZE - total_rand_bytes_remaining];
        total_rand_bytes_remaining = total_rand_bytes_remaining-1;

        new_random_twiddle_exponent[i] = (random_index)&((KYBER_N)-1);
      }
      else
      {
        new_random_twiddle_exponent[i] = (KYBER_N - random_twiddle_exponent[i])&((KYBER_N)-1);
      }
    }

    if((2*len) > max_length_of_group) // DIDO...
    {
      for(start = 0; start < 256; start = j + len)
      {
        ii3 = bitrev_order[k];

        for(j = start; j < start + len; ++j)
        {

          mask_j_exponent = j >> (max_length_of_group_shift);
          mask_j_len_exponent = (j+len) >> (max_length_of_group_shift);

          if((mask_j_exponent_prev != mask_j_exponent) || (k_prev != k))
          {
            k_prev = k;
            mask_j_exponent_prev = mask_j_exponent;
            mask_j_len_exponent_prev = mask_j_len_exponent;

            ii1 = 256 - random_twiddle_exponent[mask_j_exponent];
            ii2 = 256 - random_twiddle_exponent[mask_j_len_exponent];

            if(len != 2)
            {
              index1 = (ii1 + new_random_twiddle_exponent[mask_j_exponent])&0xFF; //A1
              index2 = (ii3 + ii2 + new_random_twiddle_exponent[mask_j_exponent])&0xFF; //B1
              index3 = (ii1 + new_random_twiddle_exponent[mask_j_len_exponent])&0xFF; //A2
              index4 = (ii3 + ii2 + new_random_twiddle_exponent[mask_j_len_exponent])&0xFF; //B2


              zeta1 = zetas_normal_order[index1];
              zeta2 = zetas_normal_order[index2];
              zeta3 = zetas_normal_order[index3];
              zeta4 = zetas_normal_order[index4];
            }

            else
            {
              index1 = (ii1) & 0xFF; //A1
              index2 = (ii3 + ii2) & 0xFF; //B1

              zeta1 = zetas_normal_order[index1];
              zeta2 = zetas_normal_order[index2];
            }
          }

          if(len != 2)
          {
            t1 = fqmul(zeta1, r[j]);
            t2 = fqmul(zeta2, r[j+len]);
            t3 = fqmul(zeta3, r[j]);
            t4 = fqmul(zeta4, r[j + len]);

            r[j] = t1 + t2;
            r[j + len] = t3 - t4;
          }
          else
          {
            t1 = fqmul(zeta2, r[j + len]);
            t2 = fqmul(zeta1, r[j]);
            r[j + len] = t2 - t1;
            r[j] = t2 + t1;
          }
        }
        k = k+1;
      }
      for(int i=0;i<no_masks_per_stage;i++)
      {
          random_twiddle_exponent[i] = new_random_twiddle_exponent[i];
      }
    }

    else if((2*len) <= max_length_of_group) // SISO...
    {
      for(start = 0; start < 256; start = j + len)
      {
        ii3 = bitrev_order[k];

        for(j = start; j < start + len; ++j)
        {

          mask_j_exponent = j >> (max_length_of_group_shift);

          if((mask_j_exponent_prev != mask_j_exponent) || (k_prev != k))
          {
            mask_j_exponent_prev = mask_j_exponent;
            k_prev = k;

            ii1 = 256 - random_twiddle_exponent[mask_j_exponent];

            if(len != 2)
            {
              index1 = (ii3 + ii1 + new_random_twiddle_exponent[mask_j_exponent])&0xFF; // Correct...
              index2 = (ii1 + new_random_twiddle_exponent[mask_j_exponent])&0xFF;
            }
            else
            {
              index1 = (ii3 + new_random_twiddle_exponent[mask_j_exponent])&0xFF; // Correct...
              index2 = (new_random_twiddle_exponent[mask_j_exponent])&0xFF;
            }

            zeta1 = zetas_normal_order[index1];
            zeta2 = zetas_normal_order[index2];
          }

          t1 = fqmul(zeta1, r[j + len]);
          t2 = fqmul(zeta2, r[j]);
          r[j + len] = t2 - t1;
          r[j] = t2 + t1;
        }
        k = k+1;
      }
      for(int i=0;i<no_masks_per_stage;i++)
      {
          random_twiddle_exponent[i] = new_random_twiddle_exponent[i];
      }
    }
  }
}

#endif

