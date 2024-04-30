#pragma once
#include <stdint.h>
#include <CL/sycl.hpp>

namespace MandelbrotGenerator
{
	struct ComplexD {
		double x;
		double y;
	};

	ComplexD c_add(ComplexD c1, ComplexD c2);
	ComplexD c_mul(ComplexD c1, ComplexD c2);
	double c_abs(ComplexD c);

	int GenerateBasic(sycl::queue* q, uint32_t* imageBuffer, int width, int height, double left, double right, double top, double bottom, int max_iterations);
	int GenerateBasicWithBuffers(sycl::queue* q, uint32_t* imageBuffer, int width, int height, double left, double right, double top, double bottom, int max_iterations);
	
	// An debug function used for testing, not used for Timings, CPU selector is used instead. 
	void GenerateBasicSequentialCPU(uint32_t* imageBuffer, int width, int height, double left, double right, double top, double bottom, int max_iterations);

	int GenerateSubgroupAutoprecision(sycl::queue* q, uint32_t* imageBuffer, int width, int height, double left, double right, double top, double bottom, int minIterations, int maxIterations);
}

