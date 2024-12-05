// Testing_Feature_Detection.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <array>
// TODO: Reference additional headers your program requires here.

#include "portability.hpp"
#include "detection.hpp"
#include "isa_detection.hpp"

enum class avx_type {
	avx = 0,
	avx2 = 1,
	avx512 = 2
};

class simd_dispatcher_base {};

class arm_simd :public simd_dispatcher_base {};

class avx_simd :public simd_dispatcher_base {};

class avx2_simd :public simd_dispatcher_base {};

class avx3_simd :public simd_dispatcher_base {};


std::array<simd_dispatcher_base, 5> arrayOfImlemenations{};