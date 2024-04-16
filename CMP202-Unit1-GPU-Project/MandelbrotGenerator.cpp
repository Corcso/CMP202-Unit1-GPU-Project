#include "MandelbrotGenerator.h"


MandelbrotGenerator::ComplexD MandelbrotGenerator::c_add(ComplexD c1, ComplexD c2)
{
	ComplexD tmp;
	double a = c1.x;
	double b = c1.y;
	double c = c2.x;
	double d = c2.y;
	tmp.x = a + c;
	tmp.y = b + d;
	return tmp;
}

MandelbrotGenerator::ComplexD MandelbrotGenerator::c_mul(ComplexD c1, ComplexD c2)
{
	ComplexD tmp;
	double a = c1.x;
	double b = c1.y;
	double c = c2.x;
	double d = c2.y;
	tmp.x = a * c - b * d;
	tmp.y = b * c + a * d;
	return tmp;
}

double MandelbrotGenerator::c_abs(ComplexD c)
{
	return sycl::sqrt(c.x * c.x + c.y * c.y);
}

void MandelbrotGenerator::GenerateBasic(sycl::queue* q, uint32_t* imageBuffer, int width, int height, double left, double right, double top, double bottom, int max_iterations)
{

	// Get a device allocation for the image array
	uint32_t* imageUSM = sycl::malloc_device<uint32_t>(width * height, *q);
	
	// Start the kernels (blank image array already on device)
	q->submit([&](sycl::handler& cgh) {
		cgh.parallel_for(sycl::range<1>(width * height), [=](sycl::id<1> i) {
			// Calculate the row and column from the index
			int row = i[0] / width;
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
			while (c_abs(z) < 2.0 && iterations < max_iterations)
			{
				z = c_add(c_mul(z, z), c);

				++iterations;
			}

			if (iterations == max_iterations)
			{
				// z didn't escape from the circle.
				// This point is in the Mandelbrot set.
				//image[y][x] = 0x000000; // black

				//uint8_t red = (iterations % 256);  // Example for red gradient
				//uint8_t green = 0; ((iterations + 100) % 256);                  // Set green to 0
				//uint8_t blue = (iterations % 256);                   // Set blue to 0

				//imageUSM[i[0]] = (red << 24) | (green << 16) | (blue << 16) | 0xFF;
				imageUSM[i[0]] = 0x000000FF;
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

				imageUSM[i[0]] = (red << 24) | (green << 16) | (blue << 8) | 0xFF;
				//imageUSM[i[0]] = 0xFFFFFFFF;

			}
		});
	}).wait();

	// Copy device memory image to the host memory for rendering on SFML
	q->memcpy(imageBuffer, imageUSM, sizeof(uint32_t) * width * height);

	// Free up the device memory
	sycl::free(imageUSM, *q);

	return;
}

void MandelbrotGenerator::GenerateBasicSequentialCPU(uint32_t* imageBuffer, int width, int height, double left, double right, double top, double bottom, int max_iterations)
{
	for (int i = 0; i < width * height; i++) {
		if (i == 400500) {
			std::cout << "midway";
		}

		// Calculate the row and column from the index
		int row = i / width;
		int col = i - (row * width);

		// Work out the point in the complex plane that
				// corresponds to this pixel in the output image.
		ComplexD c{ left + (col * (right - left) / width),
			top + (row * (bottom - top) / height) };

		// Start off z at (0, 0).
		ComplexD z{ 0.0, 0.0 };

		

		// Iterate z = z^2 + c until z moves more than 2 units
		// away from (0, 0), or we've iterated too many times.
		int iterations = 0;
		while (c_abs(z) < 2.0 && iterations < max_iterations)
		{
			z = c_add(c_mul(z, z), c);

			++iterations;
		}

		if (iterations >= max_iterations)
		{
			// z didn't escape from the circle.
			// This point is in the Mandelbrot set.
			//image[y][x] = 0x000000; // black

			uint8_t red = (iterations % 256);  // Example for red gradient
			uint8_t green = 0; ((iterations + 100) % 256);                  // Set green to 0
			uint8_t blue = (iterations % 256);                   // Set blue to 0

			//imageUSM[i[0]] = (red << 24) | (green << 16) | (blue << 16) | 0xFF;
			imageBuffer[i] = 0x000000FF;
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

			//imageUSM[i[0]] = (red << 24) | (green << 16) | (blue << 16) | 0xFF;
			imageBuffer[i] = 0xFFFFFFFF;

		}
	}
}

void MandelbrotGenerator::GenerateSubgroupAutoprecision(sycl::queue* q, uint32_t* imageBuffer, int width, int height, double left, double right, double top, double bottom, int minIterations)
{
	// Get a device allocation for the image array
	uint32_t* imageUSM = sycl::malloc_device<uint32_t>(width * height, *q);

	//std::cout << "Allowed " << q->get_device().get_info<sycl::info::device::max_work_group_size>() << " work groups, trying " << floor(sqrt(q->get_device().get_info<sycl::info::device::max_work_group_size>())) << "\n";

	// Start the kernels (blank image array already on device)
	q->submit([&](sycl::handler& cgh) {
		cgh.parallel_for(
			sycl::nd_range<2>{sycl::range<2>(width, height), 
			sycl::range<2>(
				8,
				8
			)
		},
			[=](sycl::nd_item<2> it) {
			// Get the subgroup
			auto subGroup = it.get_sub_group();
			
			// Calculate the row and column from the index
			int row = it.get_global_id(0);
			int col = it.get_global_id(1);

			// Stores wether we will keep generating with a higher increment count
			// If all points in the sub goup escape we will stop
			// If all points in the sub group don't escape we will also stop 
			// If only some points escape and some don't we will increase the iterations and recalculate this subgroup
			bool continueGenerating = true;

			// Currently on max iterations, number of max iterations to go up to. 
			int currentMaxIterations = minIterations;

			while (continueGenerating) {

				// Work out the point in the complex plane that
						// corresponds to this pixel in the output image.
				ComplexD c{ left + (col * (right - left) / width),
					top + (row * (bottom - top) / height) };

				// Start off z at (0, 0).
				ComplexD z{ 0.0, 0.0 };

				// Iterate z = z^2 + c until z moves more than 2 units
				// away from (0, 0), or we've iterated too many times.
				int iterations = 0;
				while (c_abs(z) < 2.0 && iterations < currentMaxIterations)
				{
					z = c_add(c_mul(z, z), c);

					++iterations;
				}
				bool escaped = !(iterations == currentMaxIterations);
				if (!escaped)
				{
					// z didn't escape from the circle.
					// This point is in the Mandelbrot set.
					//image[y][x] = 0x000000; // black

					//uint8_t red = (iterations % 256);  // Example for red gradient
					//uint8_t green = 0; ((iterations + 100) % 256);                  // Set green to 0
					//uint8_t blue = (iterations % 256);                   // Set blue to 0

					//imageUSM[i[0]] = (red << 24) | (green << 16) | (blue << 16) | 0xFF;
					imageUSM[width * row + col] = 0x000000FF;
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

					imageUSM[width * row + col] = (red << 24) | (green << 16) | (blue << 8) | 0xFF;
					//imageUSM[i[0]] = 0xFFFFFFFF;

				}

				// Tempoary
				switch (currentMaxIterations) {
				case 125:
					imageUSM[width * row + col] = 0xFFFF00FF;
					break;
				case 250:
					imageUSM[width * row + col] = 0x00FFFFFF;
					break;
				case 500:
					imageUSM[width * row + col] = 0xFF00FFFF;
					break;
				case 1000:
					imageUSM[width * row + col] = 0xFFFFFFFF;
					break;
				case 2000:
					imageUSM[width * row + col] = 0xFF0000FF;
					break;
				case 4000:
					imageUSM[width * row + col] = 0x00FF00FF;
					break;
				case 8000:
					imageUSM[width * row + col] = 0x0000FFFF;
					break;
				case 16000:
					imageUSM[width * row + col] = 0x000000FF;
					break;
				}

				// Only continue generating if there was both escapes and non escapes in the subgroup
				continueGenerating = sycl::any_of_group(subGroup, escaped) && sycl::any_of_group(subGroup, !escaped);
				currentMaxIterations *= 2;
				// Put a hard stop if the number of iterations gets too high 
				if (currentMaxIterations > 16000) continueGenerating = false;
			}
			});
		}).wait();

		// Copy device memory image to the host memory for rendering on SFML
		q->memcpy(imageBuffer, imageUSM, sizeof(uint32_t) * width * height);

		// Free up the device memory
		sycl::free(imageUSM, *q);

		return;
}
