// Testing_Feature_Detection.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <array>
// TODO: Reference additional headers your program requires here.

#include "portability.hpp"
#include "detection.hpp"
#include "isa_detection.hpp"

using simd_type_128 = int32_t;

struct simd_dispatcher_base {
	virtual simd_type_128 opAnd(simd_type_128, simd_type_128) = 0;
};

#if JSONIFIER_IS_ARM64

class arm_simd :public simd_dispatcher_base {};

#else

class avx_simd :public simd_dispatcher_base {
	simd_type_128 opAnd(simd_type_128, simd_type_128) override {
		std::cout << "AVX" << std::endl;
		return {};
	}
};

class avx2_simd :public simd_dispatcher_base {
	simd_type_128 opAnd(simd_type_128, simd_type_128) override {
		std::cout << "AVX2" << std::endl;
		return {};
	}
};

class avx512_simd :public simd_dispatcher_base {
	simd_type_128 opAnd(simd_type_128, simd_type_128) override {
		std::cout << "AVX512" << std::endl;
		return {};
	}
};

constexpr avx_simd avx{};
constexpr avx2_simd avx2{};
constexpr avx512_simd avx512{};

template<size_t index> struct get_simd {
	using type = std::conditional_t<index == 0, avx_simd, std::conditional_t<index == 1, avx2_simd, avx512_simd>>;
};

template<size_t index> using get_simd_t = get_simd<index>::type;

auto getSimdType(size_t mask) {
	if (mask & instruction_set::AVX512F) {
		return static_cast<simd_dispatcher_base*>(new get_simd_t<2>{});
	}
	if (mask & instruction_set::AVX2) {
		return static_cast<simd_dispatcher_base*>(new get_simd_t<1>{});
	}
	if (mask & instruction_set::SSE42) {
		return static_cast<simd_dispatcher_base*>(new get_simd_t<0>{});
	}
}

std::array<const simd_dispatcher_base*, 3> simdTypes{ &avx,&avx2,&avx512 };

#endif

struct simd_dispatcher {

	inline static std::unique_ptr<simd_dispatcher_base> base{ getSimdType(simdjson::internal::detect_supported_architectures()) };

	virtual simd_type_128 opAnd(simd_type_128 value01, simd_type_128 value02) {
		return base->opAnd(value01, value02);
	};

	static void initialize() {
		using simd_type = decltype(simdTypes[0]);
		std::cout << "CURRENT TYPE: " << typeid(simd_type).name() << std::endl;
		//base.reset(new std::remove_reference_t<std::remove_const_t<std::remove_pointer_t<decltype(simdTypes[0])>>>);
	}

};
