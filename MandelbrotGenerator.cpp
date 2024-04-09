#include "MandelbrotGenerator.h"


MandelbrotGenerator::ComplexD MandelbrotGenerator::c_add(ComplexD c1, ComplexD c2)
{
	ComplexD tmp;
	float a = c1.x;
	float b = c1.y;
	float c = c2.x;
	float d = c2.y;
	tmp.x = a + c;
	tmp.y = b + d;
	return tmp;
}

MandelbrotGenerator::ComplexD MandelbrotGenerator::c_mul(ComplexD c1, ComplexD c2)
{
	ComplexD tmp;
	float a = c1.x;
	float b = c1.y;
	float c = c2.x;
	float d = c2.y;
	tmp.x = a * c - b * d;
	tmp.y = b * c + a * d;
	return tmp;
}

double MandelbrotGenerator::c_abs(ComplexD c)
{
	return sycl::sqrt(c.x * c.x + c.y * c.y);
}

void MandelbrotGenerator::GenerateBasic(sycl::queue* q, uint32_t* imageBuffer, int width, int height, double left, double right, double top, double bottom)
{

	// Get a device allocation for the image array
	uint32_t* imageUSM = sycl::malloc_device<uint32_t>(width * height, *q);
	
	// Start the kernels (blank image array already on device)
	q->submit([&](sycl::handler& cgh) {
		cgh.parallel_for(sycl::range<1>(width * height), [=](sycl::id<1> i) {
			// Calculate the row and column from the index
			int row = i[0];
			int col = i[0] - (row * width);

			// Work out the point in the complex plane that
					// corresponds to this pixel in the output image.
			ComplexD c{ left + (col * (right - left) / width),
				top + (row * (bottom - top) / height) };

			// Start off z at (0, 0).
			ComplexD z{ 0.0, 0.0 };

			// Iterate z = z^2 + c until z moves more than 2 units
			// away from (0, 0), or we've iterated too many times.
			int iterations = 0;
			while (c_abs(z) < 2.0 && iterations < 500)
			{
				z = c_add(c_mul(z, z), c);

				++iterations;
			}

			if (iterations == 500)
			{
				// z didn't escape from the circle.
				// This point is in the Mandelbrot set.
				//image[y][x] = 0x000000; // black

				uint8_t red = (iterations % 256);  // Example for red gradient
				uint8_t green = 0; ((iterations + 100) % 256);                  // Set green to 0
				uint8_t blue = (iterations % 256);                   // Set blue to 0

				imageUSM[i[0]] = (red << 16) | (green << 8) | blue; // Combine channels
			}
			else
			{
				// z escaped within less than MAX_ITERATIONS
				// iterations. This point isn't in the set.
				//image[y][x] = 0xFFFFFF; // white
				// 
				// Create a colorful palette based on the number of iterations
				uint8_t red = static_cast<uint8_t>(128.0f + sin(iterations * 0.15f) * 128.0f);
				uint8_t green = static_cast<uint8_t>(128.0f + cos(iterations * 0.16f) * 128.0f); //0;// static_cast<uint8_t>(128.0f + sin(iterations * 0.16f) * 128.0f);
				uint8_t blue = static_cast<uint8_t>(128.0f + sin(iterations * 0.17f) * 128.0f);

				imageUSM[i[0]] = (red << 16) | (green << 8) | blue;

			}
		});
	}).wait();
}
