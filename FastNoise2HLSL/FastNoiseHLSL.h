// MIT License
//
// Copyright(c) 2024 Jordan Peck (jordan.me2@gmail.com)
// Copyright(c) 2024 Contributors
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// .'',;:cldxkO00KKXXNNWWWNNXKOkxdollcc______;__:ccllloooolllllllllooollc:,'...        ...........',;cldxkO000Okxdlc__;;;,,;;;__cclllllll
// ..',;:ldxO0KXXNNNNNNNNXXK0kxdolcc______;;;,,,,,,;;;;;;;;;;__:cclllllc:;'....       ...........',;:ldxO0KXXXK0Okxdolc__;;;;__cllodddddo
// ...',:loxO0KXNNNNNXXKK0Okxdolc__;________;;;,,'''''.....''',;:clllllc:;,'............''''''''',;:loxO0KXNNNNNXK0Okxdollccccllodxxxxxxd
// ....';:ldkO0KXXXKK00Okxdolcc:;;;;;__cclllcc:;;,''..... ....',;clooddolcc:;;;;,,;;;;;____;;;;;;:cloxk0KXNWWWWWWNXKK0Okxddoooddxxkkkkkxx
// .....';:ldxkOOOOOkxxdolcc:;;;,,,;;:cllooooolcc:;'...      ..,:codxkkkxddooollloooooooollcc____:clodkO0KXNWWWWWWNNXK00Okxxxxxxxxkkkkxxx
// . ....';:cloddddo___________,,,,;;:clooddddoolc:,...      ..,:ldx__00OOOkkk___kkkkkkxxdollc____cclodkO0KXXNNNNNNXXK0OOkxxxxxxxxxxxxddd
// .......',;:cccc:|           |,,,;;:cclooddddoll:;'..     ..';cox|  \KKK000|   |KK00OOkxdocc___;__clldxxkO0KKKKK00Okkxdddddddddddddddoo
// .......'',,,,,''|   ________|',,;;__cclloooooolc:;'......___:ldk|   \KK000|   |XKKK0Okxolc|   |;;__cclodxxkkkkxxdoolllcclllooodddooooo
// ''......''''....|   |  ....'',,,,;;;__cclloooollc:;,''.'|   |oxk|    \OOO0|   |KKK00Oxdoll|___|;;;;;__ccllllllcc__;;,,;;;:cclloooooooo
// ;;,''.......... |   |_____',,;;;____:___cllo________.___|   |___|     \xkk|   |KK_______ool_____:;________;;;_______...'',;;:ccclllloo
// c:;,''......... |         |__:/     '   |lo/        |           |      \dx|   |0/       \d|   |cc/        |'/       \......',,;;:ccllo
// ol:;,'..........|    _____|ll/    __    |o/   ______|____    ___|   |   \o|   |/   ___   \|   |o/   ______|/   ___   \ .......'',;:clo
// dlc;,...........|   |__clooo|    /  |   |x\___   \KXKKK0|   |dol|   |\   \|   |   |   |   |   |d\___   \..|   |  /   /       ....',:cl
// xoc;'...  .....'|   |llodddd|    \__|   |_____\   \KKK0O|   |lc:|   |'\       |   |___|   |   |_____\   \.|   |_/___/...      ...',;:c
// dlc;'... ....',;|   |oddddddo\          |          |Okkx|   |__;|   |..\      |\         /|   |          | \         |...    ....',;:c
// ol:,'.......',:c|___|xxxddollc\_____,___|_________/ddoll|___|,,,|___|...\_____|:\ ______/l|___|_________/...\________|'........',;__cc
// c:;'.......';:codxxkkkkxxolc__;__clodxkOO0OOkkxdollc__;;,,''''',,,,''''''''''',,'''''',;:loxkkOOkxol:;,'''',,;:ccllcc:;,'''''',;__ccll
// ;,'.......',:codxkOO0OOkxdlc:;,,;;:cldxxkkxxdolc:;;,,''.....'',;;__:;;,,,'''''........,;cldkO0KK0Okdoc__;;__cloodddoolc:;;;;;__ccllooo
// .........',;:lodxOO0000Okdoc:,,',,;:clloddoolc:;,''.......'',;:clooollc:;;,,''.......',:ldkOKXNNXX0Oxdolllloddxxxxxxdolccccccllooodddd
// .    .....';:cldxkO0000Okxol:;,''',,;__cccc:;,,'.......'',;:cldxxkkxxdolc:;;,'.......';coxOKXNWWWNXKOkxddddxxkkkkkkxdoollllooddxxxxkkk
//       ....',;:codxkO000OOxdoc:;,''',,,;;;;,''.......',,;:clodkO00000Okxolc__;,,''..',;:ldxOKXNWWWNNK0OkkkkkkkkkkkxxddooooodxxkOOOOO000
//       ....',;;clodxkkOOOkkdolc:;,,,,,,,,'..........,;:clodxkO0KKXKK0Okxdolcc__;;,,,;;:codkO0XXNNNNXKK0OOOOOkkkkxxdoollloodxkO0KKKXXXXX
//
// VERSION: 1.1.1
// https://github.com/Auburn/FastNoiseLite

#ifndef FASTNOISEHLSL_H
#define FASTNOISEHLSL_H

/* Usage
All the macros will expand the right function to use
int seed = 1337;
float freq = 0.004f;
float noise2d = PerlinNoise(seed, x * freq, y * freq);
float noise3d = PerlinNoise(seed, x * freq, y * freq, z * freq);
float noise4d = PerlinNoise(seed, x * freq, y * freq, z * freq, w * freq);
*/

/* Diffing GPU vs SIMD using FS_Abs_f32(hlsl[idx] - simd[idx]) > 0.000001f
Noises 2D on a grid 8192 * 8192 (67 108 864)
- Perlin 0.0217% (14 537)
- Value
*/

// The Forge use a customized version of HLSL to be able to deploy on any platform
// Then convert some of the language abstraction if not in use
// Define that property if you using The Forge
#ifndef THE_FORGE_SHADING_LANGUAGE
#define STATIC static
#define INLINE inline
#endif

#ifdef __cplusplus
template<typename T>
static inline float asfloatCpp(T x) {
  auto y = &x;
  return *((float*)y);
}
#define asfloat(X) asfloatCpp(X)
template<typename T>
static inline unsigned int asuintCpp(T x) {
  auto y = &x;
  return *((unsigned int*)y);
}
#define asuint(X) asuintCpp(X)
template<typename T>
static inline unsigned int asintCpp(T x) {
  auto y = &x;
  return *((int*)y);
}
#define asint(X) asintCpp(X)
#define REF(TYPE, VAR) TYPE & VAR
#else
#define REF(TYPE, VAR) inout TYPE VAR
#define asint(X) int(asuint(X))
#endif

#include "./NodeTreeProcessing.h"

#endif //FASTNOISEHLSL_H
