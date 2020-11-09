#ifndef SIN1024X16_H_
#define SIN1024X16_H_
 
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "mozzi_pgmspace.h"
 
#define SIN1024X16_NUM_CELLS 1024
#define SIN1024X16_SAMPLERATE 1024X16
 
CONSTTABLE_STORAGE(int16_t) SIN1024X16_DATA [] = {-32768, -32768, -32766, -32763,
-32759, -32753, -32746, -32738, -32729, -32719, -32707, -32694, -32680, -32664,
-32648, -32630, -32611, -32590, -32569, -32546, -32522, -32497, -32470, -32443,
-32414, -32384, -32352, -32320, -32286, -32251, -32215, -32177, -32139, -32099,
-32058, -32016, -31972, -31928, -31882, -31835, -31786, -31737, -31686, -31635,
-31582, -31527, -31472, -31415, -31358, -31299, -31238, -31177, -31115, -31051,
-30986, -30920, -30853, -30785, -30715, -30645, -30573, -30500, -30426, -30351,
-30274, -30197, -30118, -30038, -29957, -29875, -29792, -29708, -29622, -29536,
-29448, -29360, -29270, -29179, -29087, -28994, -28899, -28804, -28708, -28610,
-28512, -28412, -28311, -28209, -28107, -28003, -27898, -27792, -27685, -27577,
-27467, -27357, -27246, -27134, -27021, -26906, -26791, -26675, -26558, -26439,
-26320, -26200, -26078, -25956, -25833, -25709, -25584, -25458, -25331, -25202,
-25074, -24944, -24813, -24681, -24548, -24415, -24280, -24145, -24008, -23871,
-23733, -23594, -23454, -23313, -23171, -23028, -22885, -22741, -22595, -22449,
-22302, -22155, -22006, -21857, -21707, -21556, -21404, -21251, -21098, -20943,
-20788, -20632, -20476, -20319, -20160, -20002, -19842, -19682, -19520, -19359,
-19196, -19033, -18869, -18704, -18538, -18372, -18205, -18038, -17870, -17701,
-17531, -17361, -17190, -17019, -16847, -16674, -16500, -16326, -16152, -15977,
-15801, -15624, -15447, -15270, -15091, -14913, -14733, -14553, -14373, -14192,
-14011, -13829, -13646, -13463, -13279, -13095, -12911, -12726, -12540, -12354,
-12168, -11981, -11794, -11606, -11417, -11229, -11040, -10850, -10660, -10470,
-10279, -10088, -9897, -9705, -9513, -9320, -9127, -8934, -8740, -8546, -8352,
-8157, -7962, -7767, -7572, -7376, -7180, -6984, -6787, -6590, -6393, -6196,
-5998, -5801, -5603, -5404, -5206, -5007, -4809, -4610, -4410, -4211, -4012,
-3812, -3612, -3412, -3212, -3012, -2812, -2612, -2411, -2211, -2010, -1809,
-1608, -1408, -1207, -1006, -805, -604, -403, -202, -1, 201, 402, 603, 804,
1005, 1206, 1407, 1607, 1808, 2009, 2210, 2410, 2611, 2811, 3011, 3211, 3411,
3611, 3811, 4011, 4210, 4409, 4609, 4808, 5006, 5205, 5403, 5602, 5800, 5997,
6195, 6392, 6589, 6786, 6983, 7179, 7375, 7571, 7766, 7961, 8156, 8351, 8545,
8739, 8933, 9126, 9319, 9512, 9704, 9896, 10087, 10278, 10469, 10659, 10849,
11039, 11228, 11416, 11605, 11793, 11980, 12167, 12353, 12539, 12725, 12910,
13094, 13278, 13462, 13645, 13828, 14010, 14191, 14372, 14552, 14732, 14912,
15090, 15269, 15446, 15623, 15800, 15976, 16151, 16325, 16499, 16673, 16846,
17018, 17189, 17360, 17530, 17700, 17869, 18037, 18204, 18371, 18537, 18703,
18868, 19032, 19195, 19358, 19519, 19680, 19841, 20001, 20159, 20318, 20475,
20631, 20787, 20942, 21097, 21250, 21403, 21555, 21706, 21856, 22005, 22154,
22301, 22448, 22594, 22740, 22884, 23027, 23170, 23312, 23453, 23593, 23732,
23870, 24007, 24144, 24279, 24414, 24547, 24680, 24812, 24943, 25073, 25201,
25330, 25457, 25583, 25708, 25832, 25955, 26077, 26199, 26319, 26438, 26557,
26674, 26790, 26905, 27020, 27133, 27245, 27356, 27466, 27576, 27684, 27791,
27897, 28002, 28106, 28208, 28310, 28411, 28511, 28609, 28707, 28803, 28898,
28993, 29086, 29178, 29269, 29359, 29447, 29535, 29621, 29707, 29791, 29874,
29956, 30037, 30117, 30196, 30273, 30350, 30425, 30499, 30572, 30644, 30714,
30784, 30852, 30919, 30985, 31050, 31114, 31176, 31237, 31298, 31357, 31414,
31471, 31526, 31581, 31634, 31685, 31736, 31785, 31834, 31881, 31927, 31971,
32015, 32057, 32098, 32138, 32176, 32214, 32250, 32285, 32319, 32351, 32383,
32413, 32442, 32469, 32496, 32521, 32545, 32568, 32589, 32610, 32629, 32647,
32663, 32679, 32693, 32706, 32718, 32728, 32737, 32745, 32752, 32758, 32762,
32765, 32767, 32767, 32767, 32765, 32762, 32758, 32752, 32745, 32737, 32728,
32718, 32706, 32693, 32679, 32663, 32647, 32629, 32610, 32589, 32568, 32545,
32521, 32496, 32469, 32442, 32413, 32383, 32351, 32319, 32285, 32250, 32214,
32176, 32138, 32098, 32057, 32015, 31971, 31927, 31881, 31834, 31785, 31736,
31685, 31634, 31581, 31526, 31471, 31414, 31357, 31298, 31237, 31176, 31114,
31050, 30985, 30919, 30852, 30784, 30714, 30644, 30572, 30499, 30425, 30350,
30273, 30196, 30117, 30037, 29956, 29874, 29791, 29707, 29621, 29535, 29447,
29359, 29269, 29178, 29086, 28993, 28898, 28803, 28707, 28609, 28511, 28411,
28310, 28208, 28106, 28002, 27897, 27791, 27684, 27576, 27466, 27356, 27245,
27133, 27020, 26905, 26790, 26674, 26557, 26438, 26319, 26199, 26077, 25955,
25832, 25708, 25583, 25457, 25330, 25201, 25073, 24943, 24812, 24680, 24547,
24414, 24279, 24144, 24007, 23870, 23732, 23593, 23453, 23312, 23170, 23027,
22884, 22740, 22594, 22448, 22301, 22154, 22005, 21856, 21706, 21555, 21403,
21250, 21097, 20942, 20787, 20631, 20475, 20318, 20159, 20001, 19841, 19680,
19519, 19358, 19195, 19032, 18868, 18703, 18537, 18371, 18204, 18037, 17869,
17700, 17530, 17360, 17189, 17018, 16846, 16673, 16499, 16325, 16151, 15976,
15800, 15623, 15446, 15269, 15090, 14912, 14732, 14552, 14372, 14191, 14010,
13828, 13645, 13462, 13278, 13094, 12910, 12725, 12539, 12353, 12167, 11980,
11793, 11605, 11416, 11228, 11039, 10849, 10659, 10469, 10278, 10087, 9896,
9704, 9512, 9319, 9126, 8933, 8739, 8545, 8351, 8156, 7961, 7766, 7571, 7375,
7179, 6983, 6786, 6589, 6392, 6195, 5997, 5800, 5602, 5403, 5205, 5006, 4808,
4609, 4409, 4210, 4011, 3811, 3611, 3411, 3211, 3011, 2811, 2611, 2410, 2210,
2009, 1808, 1607, 1407, 1206, 1005, 804, 603, 402, 201, -1, -202, -403, -604,
-805, -1006, -1207, -1408, -1608, -1809, -2010, -2211, -2411, -2612, -2812,
-3012, -3212, -3412, -3612, -3812, -4012, -4211, -4410, -4610, -4809, -5007,
-5206, -5404, -5603, -5801, -5998, -6196, -6393, -6590, -6787, -6984, -7180,
-7376, -7572, -7767, -7962, -8157, -8352, -8546, -8740, -8934, -9127, -9320,
-9513, -9705, -9897, -10088, -10279, -10470, -10660, -10850, -11040, -11229,
-11417, -11606, -11794, -11981, -12168, -12354, -12540, -12726, -12911, -13095,
-13279, -13463, -13646, -13829, -14011, -14192, -14373, -14553, -14733, -14913,
-15091, -15270, -15447, -15624, -15801, -15977, -16152, -16326, -16500, -16674,
-16847, -17019, -17190, -17361, -17531, -17701, -17870, -18038, -18205, -18372,
-18538, -18704, -18869, -19033, -19196, -19359, -19520, -19682, -19842, -20002,
-20160, -20319, -20476, -20632, -20788, -20943, -21098, -21251, -21404, -21556,
-21707, -21857, -22006, -22155, -22302, -22449, -22595, -22741, -22885, -23028,
-23171, -23313, -23454, -23594, -23733, -23871, -24008, -24145, -24280, -24415,
-24548, -24681, -24813, -24944, -25074, -25202, -25331, -25458, -25584, -25709,
-25833, -25956, -26078, -26200, -26320, -26439, -26558, -26675, -26791, -26906,
-27021, -27134, -27246, -27357, -27467, -27577, -27685, -27792, -27898, -28003,
-28107, -28209, -28311, -28412, -28512, -28610, -28708, -28804, -28899, -28994,
-29087, -29179, -29270, -29360, -29448, -29536, -29622, -29708, -29792, -29875,
-29957, -30038, -30118, -30197, -30274, -30351, -30426, -30500, -30573, -30645,
-30715, -30785, -30853, -30920, -30986, -31051, -31115, -31177, -31238, -31299,
-31358, -31415, -31472, -31527, -31582, -31635, -31686, -31737, -31786, -31835,
-31882, -31928, -31972, -32016, -32058, -32099, -32139, -32177, -32215, -32251,
-32286, -32320, -32352, -32384, -32414, -32443, -32470, -32497, -32522, -32546,
-32569, -32590, -32611, -32630, -32648, -32664, -32680, -32694, -32707, -32719,
-32729, -32738, -32746, -32753, -32759, -32763, -32766, -32768,
 }; 
 
 #endif /* SIN1024X16_H_ */
