#include <algorithm>
#include "image.h"
#include <stdlib.h>
#include <math.h>
#include <Util/exceptions.h>
#include <random>

using namespace Util;
using namespace Image;

unsigned char clamp(double val) {
	return val < 0 ? 0 : val > 255 ? 255 : static_cast<unsigned char>(val);
}

int clampIndex(int index, int size) {
	return index > size - 1 ? size - 1 : index < 0 ? 0 : index;
}

double clampDouble(double val, int size) {
	return val > size - 1 ? static_cast<double>(size - 1) : val < 0 ? 0.0 : val;
}

bool checkBounds(double val, int size)
{
	return val > size - 1 ? false : val < 0 ? false : true;
}

unsigned char mean(Image32 img)
{
	long sum = 0;
	for (int i = 0; i < img.width(); i++) {
		for (int j = 0; j < img.height(); j++) {
			sum += clamp((static_cast<double>((img)(i, j).r) * 0.3) + (static_cast<double>((img)(i, j).b) * 0.11) + (static_cast<double>((img)(i, j).g) * 0.59));
		}
	}
	return static_cast<unsigned char>((sum / static_cast<long>((img.width() * img.height()))));
}

/////////////
// Image32 //
/////////////
Image32 Image32::addRandomNoise(double noise) const
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> distr(-noise, noise);

	Image32 newImg;
	newImg.setSize(_width, _height);
	for (int i = 0; i < _width; i++) {
		for (int j = 0; j < _height; j++) {
			newImg(i, j).a = (*this)(i, j).a;
			newImg(i, j).r = clamp(static_cast<double>((*this)(i, j).r) + (distr(gen) * 255.0));
			newImg(i, j).b = clamp(static_cast<double>((*this)(i, j).b) + (distr(gen) * 255.0));
			newImg(i, j).g = clamp(static_cast<double>((*this)(i, j).g) + (distr(gen) * 255.0));
		}
	}
	return newImg;
}

Image32 Image32::brighten(double brightness) const
{
	Image32 newImg;
	newImg.setSize(_width, _height);
	for (int i = 0; i < _width; i++) {
		for (int j = 0; j < _height; j++) {
			newImg(i, j).a = (*this)(i, j).a;
			newImg(i, j).r = clamp(static_cast<double>((*this)(i, j).r) * brightness);
			newImg(i, j).b = clamp(static_cast<double>((*this)(i, j).b) * brightness);
			newImg(i, j).g = clamp(static_cast<double>((*this)(i, j).g) * brightness);
		}
	}
	return newImg;
}

Image32 Image32::luminance(void) const
{
	Image32 newImg;
	newImg.setSize(_width, _height);
	for (int i = 0; i < _width; i++) {
		for (int j = 0; j < _height; j++) {
			newImg(i, j).a = (*this)(i, j).a;
			unsigned char avg = clamp((static_cast<double>((*this)(i, j).r) * 0.3) + (static_cast<double>((*this)(i, j).b) * 0.11) + (static_cast<double>((*this)(i, j).g) * 0.59));
			newImg(i, j).r = avg;
			newImg(i, j).b = avg;
			newImg(i, j).g = avg;
		}
	}
	return newImg;
}

Image32 Image32::contrast(double contrast) const
{
	unsigned char avg = mean(luminance());

	Image32 newImg;
	newImg.setSize(_width, _height);
	for (int i = 0; i < _width; i++) {
		for (int j = 0; j < _height; j++) {
			newImg(i, j).a = (*this)(i, j).a;
			newImg(i, j).r = clamp(((1 - contrast) * static_cast<double>(avg)) + (contrast * static_cast<double>((*this)(i, j).r)));
			newImg(i, j).b = clamp(((1 - contrast) * static_cast<double>(avg)) + (contrast * static_cast<double>((*this)(i, j).b)));
			newImg(i, j).g = clamp(((1 - contrast) * static_cast<double>(avg)) + (contrast * static_cast<double>((*this)(i, j).g)));
		}
	}
	return newImg;
}

Image32 Image32::saturate(double saturation) const
{
	Image32 newImg;
	newImg.setSize(_width, _height);
	for (int i = 0; i < _width; i++) {
		for (int j = 0; j < _height; j++) {
			newImg(i, j).a = (*this)(i, j).a;
			unsigned char avg = clamp(((double)(*this)(i, j).r * 0.3) + ((double)(*this)(i, j).b * 0.11) + ((double)(*this)(i, j).g * 0.59));
			newImg(i, j).r = clamp(((1 - saturation) * static_cast<double>(avg)) + (saturation * static_cast<double>((*this)(i, j).r)));
			newImg(i, j).b = clamp(((1 - saturation) * static_cast<double>(avg)) + (saturation * static_cast<double>((*this)(i, j).b)));
			newImg(i, j).g = clamp(((1 - saturation) * static_cast<double>(avg)) + (saturation * static_cast<double>((*this)(i, j).g)));
		}
	}
	return newImg;
}

Image32 Image32::quantize(int bits) const
{
	Image32 newImg;
	newImg.setSize(_width, _height);
	for (int i = 0; i < _width; i++) {
		for (int j = 0; j < _height; j++) {
			newImg(i, j).a = clamp((255.0 / (pow(2.0, bits) - 1)) * (floor((double)((*this)(i, j).a / 255.0) * (pow(2.0, bits)))));
			newImg(i, j).r = clamp((255.0 / (pow(2.0, bits) - 1)) * (floor((double)((*this)(i, j).r / 255.0) * (pow(2.0, bits)))));
			newImg(i, j).b = clamp((255.0 / (pow(2.0, bits) - 1)) * (floor((double)((*this)(i, j).b / 255.0) * (pow(2.0, bits)))));
			newImg(i, j).g = clamp((255.0 / (pow(2.0, bits) - 1)) * (floor((double)((*this)(i, j).g / 255.0) * (pow(2.0, bits)))));
		}
	}
	return newImg;
}

Image32 Image32::randomDither(int bits) const
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> distr(-1.0, 1.0);

	Image32 newImg;
	newImg.setSize(_width, _height);
	for (int i = 0; i < _width; i++) {
		for (int j = 0; j < _height; j++) {
			newImg(i, j).a = clamp(255.0 * (((double)((*this)(i, j).a) / 255.0) + (distr(gen) / pow(2.0, bits))));
			newImg(i, j).r = clamp(255.0 * (((double)((*this)(i, j).r) / 255.0) + (distr(gen) / pow(2.0, bits))));
			newImg(i, j).b = clamp(255.0 * (((double)((*this)(i, j).b) / 255.0) + (distr(gen) / pow(2.0, bits))));
			newImg(i, j).g = clamp(255.0 * (((double)((*this)(i, j).g) / 255.0) + (distr(gen) / pow(2.0, bits))));
		}
	}
	return newImg;
}

Image32 Image32::orderedDither2X2(int bits) const
{
	double thresholds[2][2] = { {1, 3}, {4, 2} };

	Image32 newImg;
	newImg.setSize(_width, _height);
	for (int i = 0; i < _width; i++) {
		for (int j = 0; j < _height; j++) {
			int x = i % 2;
			int y = j % 2;

			double c = ((*this)(i, j).a / 255.0) * (pow(2.0, bits) - 1);
			double e = c - floor(c);

			if (e > thresholds[x][y] / 5) {
				newImg(i, j).a = clamp((255.0 / (pow(2.0, bits) - 1)) * ceil(c));
			}
			else {
				newImg(i, j).a = clamp((255.0 / (pow(2.0, bits) - 1)) * floor(c));
			}

			c = ((*this)(i, j).r / 255.0) * (pow(2.0, bits) - 1);
			e = c - floor(c);

			if (e > thresholds[x][y] / 5) {
				newImg(i, j).r = clamp((255.0 / (pow(2.0, bits) - 1)) * ceil(c));
			}
			else {
				newImg(i, j).r = clamp((255.0 / (pow(2.0, bits) - 1)) * floor(c));
			}

			c = ((*this)(i, j).b / 255.0) * (pow(2.0, bits) - 1);
			e = c - floor(c);

			if (e > thresholds[x][y] / 5) {
				newImg(i, j).b = clamp((255.0 / (pow(2.0, bits) - 1)) * ceil(c));
			}
			else {
				newImg(i, j).b = clamp((255.0 / (pow(2.0, bits) - 1)) * floor(c));
			}

			c = ((*this)(i, j).g / 255.0) * (pow(2.0, bits) - 1);
			e = c - floor(c);

			if (e > thresholds[x][y] / 5) {
				newImg(i, j).g = clamp((255.0 / (pow(2.0, bits) - 1)) * ceil(c));
			}
			else {
				newImg(i, j).g = clamp((255.0 / (pow(2.0, bits) - 1)) * floor(c));
			}
		}
	}
	return newImg;
}

Image32 Image32::floydSteinbergDither(int bits) const
{
	Image32 oldImg(*this);
	Image32 newImg;
	newImg.setSize(_width, _height);
	for (int j = 0; j < _height; j++) {
		for (int i = 0; i < _width; i++) {
			newImg(i, j).a = clamp((255.0 / (pow(2.0, bits) - 1)) * (floor((double)(oldImg(i, j).a / 255.0) * pow(2.0, bits))));
			newImg(i, j).r = clamp((255.0 / (pow(2.0, bits) - 1)) * (floor((double)(oldImg(i, j).r / 255.0) * pow(2.0, bits))));
			newImg(i, j).b = clamp((255.0 / (pow(2.0, bits) - 1)) * (floor((double)(oldImg(i, j).b / 255.0) * pow(2.0, bits))));
			newImg(i, j).g = clamp((255.0 / (pow(2.0, bits) - 1)) * (floor((double)(oldImg(i, j).g / 255.0) * pow(2.0, bits))));

			double e = oldImg(i, j).a - newImg(i, j).a;
			oldImg(i, clampIndex(j + 1, _height)).a = clamp((double)oldImg(i, clampIndex(j + 1, _height)).a + (7.0 / 16.0) * e);
			oldImg(clampIndex(i + 1, _width), clampIndex(j - 1, _height)).a = clamp((double)oldImg(clampIndex(i + 1, _width), clampIndex(j - 1, _height)).a + (3.0 / 16.0) * e);
			oldImg(clampIndex(i + 1, _width), j).a = clamp((double)oldImg(clampIndex(i + 1, _width), j).a + (5.0 / 16.0) * e);
			oldImg(clampIndex(i + 1, _width), clampIndex(j + 1, _height)).a = clamp((double)oldImg(clampIndex(i + 1, _width), clampIndex(j + 1, _height)).a + (1.0 / 16.0) * e);

			e = oldImg(i, j).r - newImg(i, j).r;
			oldImg(i, clampIndex(j + 1, _height)).r = clamp((double)oldImg(i, clampIndex(j + 1, _height)).r + (7.0 / 16.0) * e);
			oldImg(clampIndex(i + 1, _width), clampIndex(j - 1, _height)).r = clamp((double)oldImg(clampIndex(i + 1, _width), clampIndex(j - 1, _height)).r + (3.0 / 16.0) * e);
			oldImg(clampIndex(i + 1, _width), j).r = clamp((double)oldImg(clampIndex(i + 1, _width), j).r + (5.0 / 16.0) * e);
			oldImg(clampIndex(i + 1, _width), clampIndex(j + 1, _height)).r = clamp((double)oldImg(clampIndex(i + 1, _width), clampIndex(j + 1, _height)).r + (1.0 / 16.0) * e);

			e = oldImg(i, j).b - newImg(i, j).b;
			oldImg(i, clampIndex(j + 1, _height)).b = clamp((double)oldImg(i, clampIndex(j + 1, _height)).b + (7.0 / 16.0) * e);
			oldImg(clampIndex(i + 1, _width), clampIndex(j - 1, _height)).b = clamp((double)oldImg(clampIndex(i + 1, _width), clampIndex(j - 1, _height)).b + (3.0 / 16.0) * e);
			oldImg(clampIndex(i + 1, _width), j).b = clamp((double)oldImg(clampIndex(i + 1, _width), j).b + (5.0 / 16.0) * e);
			oldImg(clampIndex(i + 1, _width), clampIndex(j + 1, _height)).b = clamp((double)oldImg(clampIndex(i + 1, _width), clampIndex(j + 1, _height)).b + (1.0 / 16.0) * e);

			e = oldImg(i, j).g - newImg(i, j).g;
			oldImg(i, clampIndex(j + 1, _height)).g = clamp((double)oldImg(i, clampIndex(j + 1, _height)).g + (7.0 / 16.0) * e);
			oldImg(clampIndex(i + 1, _width), clampIndex(j - 1, _height)).g = clamp((double)oldImg(clampIndex(i + 1, _width), clampIndex(j - 1, _height)).g + (3.0 / 16.0) * e);
			oldImg(clampIndex(i + 1, _width), j).g = clamp((double)oldImg(clampIndex(i + 1, _width), j).g + (5.0 / 16.0) * e);
			oldImg(clampIndex(i + 1, _width), clampIndex(j + 1, _height)).g = clamp((double)oldImg(clampIndex(i + 1, _width), clampIndex(j + 1, _height)).g + (1.0 / 16.0) * e);
		}
	}
	return newImg;
}

Image32 Image32::blur3X3(void) const
{
	double mask[9] =
	{
		1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0,
		2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0,
		1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0
	};

	double* ptr = &mask[4];

	Image32 newImg;
	newImg.setSize(_width, _height);
	for (int i = 0; i < _width; i++) {
		for (int j = 0; j < _height; j++) {
			double newRed = 0, newBlue = 0, newGreen = 0, newAlpha = 0;
			for (int x = -1; x < 2; x++) {
				for (int y = -1; y < 2; y++) {
					newRed += (*this)(clampIndex(i + x, _width), clampIndex(j + y, _height)).r * ptr[(x * 3) + y];
					newBlue += (*this)(clampIndex(i + x, _width), clampIndex(j + y, _height)).b * ptr[(x * 3) + y];
					newGreen += (*this)(clampIndex(i + x, _width), clampIndex(j + y, _height)).g * ptr[(x * 3) + y];
				}
			}
			newImg(i, j).a = (*this)(i, j).a;
			newImg(i, j).r = clamp(newRed);
			newImg(i, j).b = clamp(newBlue);
			newImg(i, j).g = clamp(newGreen);
		}
	}
	return newImg;
}

Image32 Image32::edgeDetect3X3(void) const
{
	double threshold = 20.0;

	double mask[9] =
	{
		-1.0 / 8.0, -1.0 / 8.0, -1.0 / 8.0,
		-1.0 / 8.0,        1.0, -1.0 / 8.0,
		-1.0 / 8.0, -1.0 / 8.0, -1.0 / 8.0
	};

	double* ptr = &mask[4];

	double maxRedErr = 0;
	double minRedErr = 0;
	double maxGreenErr = 0;
	double minGreenErr = 0;
	double maxBlueErr = 0;
	double minBlueErr = 0;

	for (int i = 0; i < _width; i++) {
		for (int j = 0; j < _height; j++) {
			double redErr = 0, blueErr = 0, greenErr = 0;
			for (int x = -1; x < 2; x++) {
				for (int y = -1; y < 2; y++) {
					redErr += (*this)(clampIndex(i + x, _width), clampIndex(j + y, _height)).r * ptr[(x * 3) + y];
					blueErr += (*this)(clampIndex(i + x, _width), clampIndex(j + y, _height)).b * ptr[(x * 3) + y];
					greenErr += (*this)(clampIndex(i + x, _width), clampIndex(j + y, _height)).g * ptr[(x * 3) + y];
				}
			}
			if (redErr > maxRedErr) {
				maxRedErr = redErr;
			}
			else if (redErr < minRedErr) {
				minRedErr = redErr;
			}
			if (greenErr > maxGreenErr) {
				maxGreenErr = greenErr;
			}
			else if (greenErr < minGreenErr) {
				minGreenErr = greenErr;
			}
			if (blueErr > maxBlueErr) {
				maxBlueErr = blueErr;
			}
			else if (blueErr < minBlueErr) {
				minBlueErr = blueErr;
			}
		}
	}

	Image32 newImg;
	newImg.setSize(_width, _height);
	for (int i = 0; i < _width; i++) {
		for (int j = 0; j < _height; j++) {
			double redErr = 0, blueErr = 0, greenErr = 0;
			for (int x = -1; x < 2; x++) {
				for (int y = -1; y < 2; y++) {
					redErr += (*this)(clampIndex(i + x, _width), clampIndex(j + y, _height)).r * ptr[(x * 3) + y];
					blueErr += (*this)(clampIndex(i + x, _width), clampIndex(j + y, _height)).b * ptr[(x * 3) + y];
					greenErr += (*this)(clampIndex(i + x, _width), clampIndex(j + y, _height)).g * ptr[(x * 3) + y];
				}
			}
			newImg(i, j).r = clamp(((redErr - minRedErr) / (maxRedErr - minRedErr) * 255));
			newImg(i, j).b = clamp(((blueErr - minBlueErr) / (maxBlueErr - minBlueErr) * 255));
			newImg(i, j).g = clamp(((greenErr - minGreenErr) / (maxGreenErr - minGreenErr) * 255));

			newImg(i, j).a = (*this)(i, j).a;

			//Uncomment 356-358 and comment 349-351 for Method 1. Currently using method 2.
			//newImg(i, j).r = abs(redErr) > threshold ? 255 : 0;
			//newImg(i, j).b = abs(blueErr) > threshold ? 255 : 0;
			//newImg(i, j).g = abs(greenErr) > threshold ? 255 : 0;
		}
	}
	return newImg;
}

Image32 Image32::scaleNearest(double scaleFactor) const
{
	int width = static_cast<int>(_width * scaleFactor);
	int height = static_cast<int>(_height * scaleFactor);

	Image32 newImg;
	newImg.setSize(width, height);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			unsigned char u = floor((i / scaleFactor) + 0.5);
			unsigned char v = floor((j / scaleFactor) + 0.5);

			if (!checkBounds(u, _width) || !checkBounds(v, _height))
			{
				newImg(i, j).a = 0;
				newImg(i, j).r = 0;
				newImg(i, j).b = 0;
				newImg(i, j).g = 0;
			}
			else
			{
				newImg(i, j).a = (*this)(u, v).a;
				newImg(i, j).r = (*this)(u, v).r;
				newImg(i, j).b = (*this)(u, v).b;
				newImg(i, j).g = (*this)(u, v).g;
			}
		}
	}
	return newImg;
}

Image32 Image32::scaleBilinear(double scaleFactor) const
{
	int width = static_cast<int>(_width * scaleFactor);
	int height = static_cast<int>(_height * scaleFactor);

	Image32 newImg;
	newImg.setSize(width, height);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			double u1 = clampIndex(floor(i / scaleFactor), _width);
			double u2 = clampIndex(u1 + 1, _width);
			double v1 = clampIndex(floor(j / scaleFactor), _height);
			double v2 = clampIndex(v1 + 1, _height);
			double du = (i / scaleFactor) - u1;
			double dv = (j / scaleFactor) - v1;

			double a = (*this)(u1, v1).a * (1 - du) + (*this)(u2, v1).a * du;
			double b = (*this)(u1, v2).a * (1 - du) + (*this)(u2, v2).a * du;
			newImg(i, j).a = a * (1 - dv) + b * dv;

			a = (*this)(u1, v1).r * (1 - du) + (*this)(u2, v1).r * du;
			b = (*this)(u1, v2).r * (1 - du) + (*this)(u2, v2).r * du;
			newImg(i, j).r = a * (1 - dv) + b * dv;

			a = (*this)(u1, v1).g * (1 - du) + (*this)(u2, v1).g * du;
			b = (*this)(u1, v2).g * (1 - du) + (*this)(u2, v2).g * du;
			newImg(i, j).g = a * (1 - dv) + b * dv;

			a = (*this)(u1, v1).b * (1 - du) + (*this)(u2, v1).b * du;
			b = (*this)(u1, v2).b * (1 - du) + (*this)(u2, v2).b * du;
			newImg(i, j).b = a * (1 - dv) + b * dv;
		}
	}
	return newImg;
}

Image32 Image32::scaleGaussian(double scaleFactor) const
{
	int w = 1.0 / scaleFactor;
	if (w < 1) w++;

	int width = static_cast<int>(_width * scaleFactor);
	int height = static_cast<int>(_height * scaleFactor);

	Image32 newImg;
	newImg.setSize(width, height);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			double u = (double)i / scaleFactor;
			double v = (double)j / scaleFactor;

			double dstRed = 0;
			double dstBlue = 0;
			double dstGreen = 0;
			double weight = 0;

			int ulo = floor(u - w);
			int uhi = ceil(u + w);
			int vlo = floor(v - w);
			int vhi = ceil(v + w);

			for (int iu = ulo; iu < uhi; iu++) {
				for (int iv = vlo; iv < vhi; iv++) {
					double d = sqrt(pow(u - iu, 2) + pow(v - iv, 2));
					double a = (double)iu - u;
					double b = (double)iv - v;
					if (a * a + b * b <= w * w) {
						if (!checkBounds(iu, _width) || !checkBounds(iv, _height))
						{
							weight += exp(-(pow(d, 2.0)) / (2.0 * pow((double)w / 3.0, 2.0)));
						}
						else
						{
							weight += exp(-(pow(d, 2.0)) / (2.0 * pow((double)w / 3.0, 2.0)));
							dstRed += exp(-(pow(d, 2.0)) / (2.0 * pow((double)w / 3.0, 2.0))) * (double)(*this)(iu, iv).r;
							dstBlue += exp(-(pow(d, 2.0)) / (2.0 * pow((double)w / 3.0, 2.0))) * (double)(*this)(iu, iv).b;
							dstGreen += exp(-(pow(d, 2.0)) / (2.0 * pow((double)w / 3.0, 2.0))) * (double)(*this)(iu, iv).g;
						}
					}
				}
			}
			newImg(i, j).a = 255;
			newImg(i, j).r = clamp(dstRed / weight);
			newImg(i, j).b = clamp(dstBlue / weight);
			newImg(i, j).g = clamp(dstGreen / weight);
		}
	}
	return newImg;
}

Image32 Image32::rotateNearest(double angle) const
{
	double a = -angle;

	int height = static_cast<int>((double)_width * abs(cos(a * (Pi / 180.0))) + (double)_height * abs(sin(a * (Pi / 180.0))));
	int width = static_cast<int>((double)_width * abs(sin(a * (Pi / 180.0))) + (double)_height * abs(cos(a * (Pi / 180.0))));

	Image32 newImg;
	newImg.setSize(width, height);
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			double u = (double)(i - ((double)width / 2.0)) * cos(-a * (Pi / 180.0)) - (double)(j - ((double)height / 2.0)) * sin(-a * (Pi / 180.0)) + ((double)_width / 2.0);
			double v = (double)(i - ((double)width / 2.0)) * sin(-a * (Pi / 180.0)) + (double)(j - ((double)height / 2.0)) * cos(-a * (Pi / 180.0)) + ((double)_height / 2.0);
			int iu = floor(u + 0.5);
			int iv = floor(v + 0.5);
			if (!checkBounds(iu, _width) || !checkBounds(iv, _height)) {
				newImg(i, j).a = 0;
				newImg(i, j).r = 0;
				newImg(i, j).b = 0;
				newImg(i, j).g = 0;
			}
			else
			{
				newImg(i, j).a = (*this)(iu, iv).a;
				newImg(i, j).r = (*this)(iu, iv).r;
				newImg(i, j).b = (*this)(iu, iv).b;
				newImg(i, j).g = (*this)(iu, iv).g;
			}
		}
	}
	return newImg;
}

Image32 Image32::rotateBilinear(double angle) const
{
	double a = -angle;

	int height = static_cast<int>((double)_width * abs(cos(a * (Pi / 180.0))) + (double)_height * abs(sin(a * (Pi / 180.0))));
	int width = static_cast<int>((double)_width * abs(sin(a * (Pi / 180.0))) + (double)_height * abs(cos(a * (Pi / 180.0))));

	Image32 newImg;
	newImg.setSize(width, height);
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			double u = (double)(i - ((double)width / 2.0)) * cos(-a * (Pi / 180.0)) - (double)(j - ((double)height / 2.0)) * sin(-a * (Pi / 180.0)) + ((double)_width / 2.0);
			double v = (double)(i - ((double)width / 2.0)) * sin(-a * (Pi / 180.0)) + (double)(j - ((double)height / 2.0)) * cos(-a * (Pi / 180.0)) + ((double)_height / 2.0);

			double u1 = floor(u);
			double u2 = u1 + 1;
			double v1 = floor(v);
			double v2 = v1 + 1;
			double du = u - u1;
			double dv = v - v1;

			Pixel32 bl;
			Pixel32 br;
			Pixel32 tl;
			Pixel32 tr;

			if (!checkBounds(u1, _width) || !checkBounds(v1, _height)) {
				bl.a = 0;
				bl.r = 0;
				bl.b = 0;
				bl.g = 0;
			}
			else {
				bl.a = (*this)(u1, v1).a;
				bl.r = (*this)(u1, v1).r;
				bl.b = (*this)(u1, v1).b;
				bl.g = (*this)(u1, v1).g;
			}

			if (!checkBounds(u2, _width) || !checkBounds(v1, _height)) {
				br.a = 0;
				br.r = 0;
				br.b = 0;
				br.g = 0;
			}
			else {
				br.a = (*this)(u2, v1).a;
				br.r = (*this)(u2, v1).r;
				br.b = (*this)(u2, v1).b;
				br.g = (*this)(u2, v1).g;
			}

			if (!checkBounds(u1, _width) || !checkBounds(v2, _height)) {
				tl.a = 0;
				tl.r = 0;
				tl.b = 0;
				tl.g = 0;
			}
			else {
				tl.a = (*this)(u1, v2).a;
				tl.r = (*this)(u1, v2).r;
				tl.b = (*this)(u1, v2).b;
				tl.g = (*this)(u1, v2).g;
			}

			if (!checkBounds(u2, _width) || !checkBounds(v2, _height)) {
				tr.a = 0;
				tr.r = 0;
				tr.b = 0;
				tr.g = 0;
			}
			else {
				tr.a = (*this)(u2, v2).a;
				tr.r = (*this)(u2, v2).r;
				tr.b = (*this)(u2, v2).b;
				tr.g = (*this)(u2, v2).g;
			}

			double a = bl.a * (1 - du) + br.a * du;
			double b = tl.a * (1 - du) + tr.a * du;
			newImg(i, j).a = a * (1 - dv) + b * dv;

			a = bl.r * (1 - du) + br.r * du;
			b = tl.r * (1 - du) + tr.r * du;
			newImg(i, j).r = a * (1 - dv) + b * dv;

			a = bl.g * (1 - du) + br.g * du;
			b = tl.g * (1 - du) + tr.g * du;
			newImg(i, j).g = a * (1 - dv) + b * dv;

			a = bl.b * (1 - du) + br.b * du;
			b = tl.b * (1 - du) + tr.b * du;
			newImg(i, j).b = a * (1 - dv) + b * dv;
		}
	}
	return newImg;
}

Image32 Image32::rotateGaussian(double angle) const
{
	double a = -angle;

	int height = static_cast<int>((double)_width * abs(cos(a * (Pi / 180.0))) + (double)_height * abs(sin(a * (Pi / 180.0))));
	int width = static_cast<int>((double)_width * abs(sin(a * (Pi / 180.0))) + (double)_height * abs(cos(a * (Pi / 180.0))));

	int w = 1;

	Image32 newImg;
	newImg.setSize(width, height);
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			double u = (double)(i - ((double)width / 2.0)) * cos(-a * (Pi / 180.0)) - (double)(j - ((double)height / 2.0)) * sin(-a * (Pi / 180.0)) + ((double)_width / 2.0);
			double v = (double)(i - ((double)width / 2.0)) * sin(-a * (Pi / 180.0)) + (double)(j - ((double)height / 2.0)) * cos(-a * (Pi / 180.0)) + ((double)_height / 2.0);

			double dstRed = 0;
			double dstBlue = 0;
			double dstGreen = 0;

			double weight = 0;

			int ulo = floor(u - w);
			int uhi = ceil(u + w);
			int vlo = floor(v - w);
			int vhi = ceil(v + w);

			for (int iu = ulo; iu < uhi; iu++) {
				for (int iv = vlo; iv < vhi; iv++) {
					double d = sqrt(pow(u - iu, 2) + pow(v - iv, 2));
					double a = (double)iu - u;
					double b = (double)iv - v;
					if (a * a + b * b <= w * w) {
						if (!checkBounds(iu, _width) || !checkBounds(iv, _height)) {
							weight += exp(-(pow(d, 2.0)) / (2.0 * pow((double)w / 3.0, 2.0)));
						}
						else {
							weight += exp(-(pow(d, 2.0)) / (2.0 * pow((double)w / 3.0, 2.0)));
							dstRed += exp(-(pow(d, 2.0)) / (2.0 * pow((double)w / 3.0, 2.0))) * (double)(*this)(iu, iv).r;
							dstBlue += exp(-(pow(d, 2.0)) / (2.0 * pow((double)w / 3.0, 2.0))) * (double)(*this)(iu, iv).b;
							dstGreen += exp(-(pow(d, 2.0)) / (2.0 * pow((double)w / 3.0, 2.0))) * (double)(*this)(iu, iv).g;
						}
					}
				}
			}
			newImg(i, j).a = 255;
			newImg(i, j).r = clamp(dstRed / weight);
			newImg(i, j).b = clamp(dstBlue / weight);
			newImg(i, j).g = clamp(dstGreen / weight);
		}
	}
	return newImg;
}

void Image32::setAlpha(const Image32& matte)
{
	for (int j = 0; j < _height; j++) {
		for (int i = 0; i < _width; i++) {
			(*this)(i, j).a = clamp((double)matte(i, j).r);
		}
	}
}

Image32 Image32::composite(const Image32& overlay) const
{
	Image32 newImg;
	newImg.setSize(_width, _height);
	if (_width != overlay.width() || _height != overlay.height()) {
		ErrorOut("image.todo.cpp", 746, "composite", "overlay and image are different sizes", _width, overlay.width(), _height, overlay.height());
	}
	for (int j = 0; j < _height; j++) {
		for (int i = 0; i < _width; i++) {
			double bAlpha = 0;
			if (!(*this)(i, j).a)
			{
				bAlpha = 1.0;
			}
			else
			{
				bAlpha = static_cast<double>((*this)(i, j).a) / 255.0;
			}
			newImg(i, j).a = clamp((static_cast<double>(overlay(i, j).a) / 255.0)
				+ (1.0 - (static_cast<double>(overlay(i, j).a) / 255.0))
				* bAlpha);

			newImg(i, j).b = clamp((static_cast<double>(overlay(i, j).b)
				* (static_cast<double>(overlay(i, j).a) / 255.0)
				+ static_cast<double>((*this)(i, j).b)
				* (1.0 - (static_cast<double>(overlay(i, j).a) / 255.0)) * bAlpha)
				/ ((static_cast<double>(overlay(i, j).a) / 255.0)
					+ (1.0 - (static_cast<double>(overlay(i, j).a) / 255.0))
					* bAlpha));

			newImg(i, j).g = clamp((static_cast<double>(overlay(i, j).g)
				* (static_cast<double>(overlay(i, j).a) / 255.0)
				+ static_cast<double>((*this)(i, j).g)
				* (1.0 - (static_cast<double>(overlay(i, j).a) / 255.0)) * bAlpha)
				/ ((static_cast<double>(overlay(i, j).a) / 255.0)
					+ (1.0 - (static_cast<double>(overlay(i, j).a) / 255.0))
					* bAlpha));

			newImg(i, j).r = clamp((static_cast<double>(overlay(i, j).r)
				* (static_cast<double>(overlay(i, j).a) / 255.0)
				+ static_cast<double>((*this)(i, j).r)
				* (1.0 - (static_cast<double>(overlay(i, j).a) / 255.0)) * bAlpha)
				/ ((static_cast<double>(overlay(i, j).a) / 255.0)
					+ (1.0 - (static_cast<double>(overlay(i, j).a) / 255.0))
					* bAlpha));
		}
	}
	return newImg;
}

Image32 Image32::CrossDissolve(const Image32& source, const Image32& destination, double blendWeight)
{
	int width = destination.width();
	int height = destination.height();
	Image32 newImg;
	newImg.setSize(width, height);
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			newImg(i, j).a = source(i, j).a + blendWeight * (destination(i, j).a - source(i, j).a);
			newImg(i, j).r = source(i, j).r + blendWeight * (destination(i, j).r - source(i, j).r);
			newImg(i, j).b = source(i, j).b + blendWeight * (destination(i, j).b - source(i, j).b);
			newImg(i, j).g = source(i, j).g + blendWeight * (destination(i, j).g - source(i, j).g);
		}
	}

	return newImg;
}

Image32 Image32::warp(const OrientedLineSegmentPairs& olsp) const
{
	Image32 newImg;
	newImg.setSize(this->width(), this->height());
	for (int j = 0; j < this->height(); j++)
	{
		for (int i = 0; i < this->width(); i++)
		{
			Point2D X(i, j);
			Point2D DSUM(0, 0);
			double weightSUM = 0;
			for (auto pair : olsp)
			{
				Point2D P = pair.second.endPoints[0];
				Point2D Q = pair.second.endPoints[1];

				Point2D Pi = pair.first.endPoints[0];
				Point2D Qi = pair.first.endPoints[1];

				Point2D XP = X - P;
				Point2D QP = Q - P;
				double u = XP.dot(QP) / pow(QP.length(), 2);

				Point2D QPperp = pair.second.perpendicular();
				double v = XP.dot(QPperp) / QPperp.length();

				Point2D QIPI = Qi - Pi;
				Point2D QIPIperp = pair.first.perpendicular();
				Point2D Xpi = Pi + u * QIPI + (v * QIPIperp) / QIPI.length();

				Point2D Di = Xpi - X;
				double dist = pair.second.distance(X);
				double weight = pow((pow(pair.second.length(), pair.second.P) / (pair.second.A + dist)), pair.second.B);
				DSUM += (Di * weight);
				weightSUM += weight;
			}
			Point2D Xp = X + (DSUM / weightSUM);
			if (!checkBounds(Xp[0], this->width()) || !checkBounds(Xp[1], this->height())) {}
			else {
				double u1 = clampIndex(Xp[0], _width);
				double u2 = clampIndex(u1 + 1, _width);
				double v1 = clampIndex(Xp[1], _height);
				double v2 = clampIndex(v1 + 1, _height);
				double du = (Xp[0]) - u1;
				double dv = (Xp[1]) - v1;

				double a = (*this)(u1, v1).a * (1 - du) + (*this)(u2, v1).a * du;
				double b = (*this)(u1, v2).a * (1 - du) + (*this)(u2, v2).a * du;
				newImg(i, j).a = a * (1 - dv) + b * dv;

				a = (*this)(u1, v1).r * (1 - du) + (*this)(u2, v1).r * du;
				b = (*this)(u1, v2).r * (1 - du) + (*this)(u2, v2).r * du;
				newImg(i, j).r = a * (1 - dv) + b * dv;

				a = (*this)(u1, v1).g * (1 - du) + (*this)(u2, v1).g * du;
				b = (*this)(u1, v2).g * (1 - du) + (*this)(u2, v2).g * du;
				newImg(i, j).g = a * (1 - dv) + b * dv;

				a = (*this)(u1, v1).b * (1 - du) + (*this)(u2, v1).b * du;
				b = (*this)(u1, v2).b * (1 - du) + (*this)(u2, v2).b * du;
				newImg(i, j).b = a * (1 - dv) + b * dv;
			}
		}
	}
	return newImg;
}

Image32 Image32::blurNXN(double n, double sigma) const
{
	Image32 newImg;
	newImg.setSize(_width, _height);

	int center = (int)n % 2 != 1 ? n++ / 2 : n / 2;

	for (int j = 0; j < _height; j++) {
		for (int i = 0; i < _width; i++) {
			double dstRed = 0;
			double dstBlue = 0;
			double dstGreen = 0;
			double weight = 0;

			int xlo = i - center;
			int xhi = i + center;
			int ylo = j - center;
			int yhi = j + center;

			for (int x = xlo; x < xhi; x++) {
				for (int y = ylo; y < yhi; y++) {
					if (!checkBounds(x, _width) || !checkBounds(y, _height)) {
					}
					else {
						weight += (1.0 / (2.0 * Pi * pow(sigma, 2))) * exp((-pow(x - i, 2) + pow(y - j, 2)) / (2 * pow(sigma, 2)));
						dstRed += (1.0 / (2.0 * Pi * pow(sigma, 2))) * exp((-pow(x - i, 2) + pow(y - j, 2)) / (2 * pow(sigma, 2))) * (double)(*this)(x, y).r;
						dstBlue += (1.0 / (2.0 * Pi * pow(sigma, 2))) * exp((-pow(x - i, 2) + pow(y - j, 2)) / (2 * pow(sigma, 2))) * (double)(*this)(x, y).b;
						dstGreen += (1.0 / (2.0 * Pi * pow(sigma, 2))) * exp((-pow(x - i, 2) + pow(y - j, 2)) / (2 * pow(sigma, 2))) * (double)(*this)(x, y).g;
					}
				}
			}
			newImg(i, j).a = 255;
			newImg(i, j).r = clamp(dstRed / weight);
			newImg(i, j).b = clamp(dstBlue / weight);
			newImg(i, j).g = clamp(dstGreen / weight);
		}
	}
	return newImg;
}

Image32 Image32::funFilter(int numBuckets, int radius) const
{
	Image32 newImg;
	newImg.setSize(_width, _height);

	for (int j = 0; j < newImg.height(); j++) {
		for (int i = 0; i < newImg.width(); i++) {
			int* rAvg = new int[numBuckets];
			int* bAvg = new int[numBuckets];
			int* gAvg = new int[numBuckets];
			int* intensityCount = new int[numBuckets];

			std::fill_n(rAvg, numBuckets, 0);
			std::fill_n(bAvg, numBuckets, 0);
			std::fill_n(gAvg, numBuckets, 0);
			std::fill_n(intensityCount, numBuckets, 0);

			for (int y = j - radius; y < j + radius; y++) {
				for (int x = i - radius; x < i + radius; x++) {
					if (checkBounds(x, newImg.width()) && checkBounds(y, newImg.height())) {
						int curIntensity = (int)((((double)(*this)(x, y).r * 0.3) + ((double)(*this)(x, y).b * 0.11) + ((double)(*this)(x, y).g * 0.59)) * (double)numBuckets / 255.0);
						intensityCount[curIntensity]++;
						rAvg[curIntensity] += (int)(*this)(x, y).r;
						gAvg[curIntensity] += (int)(*this)(x, y).g;
						bAvg[curIntensity] += (int)(*this)(x, y).b;
					}
				}
			}
			int max = 0;
			int maxIndex = 0;
			for (int n = 0; n < numBuckets; n++)
			{
				if (intensityCount[n] > max)
				{
					max = intensityCount[n];
					maxIndex = n;
				}
			}
			newImg(i, j).a = 255;
			newImg(i, j).r = clamp(rAvg[maxIndex] / (double)max);
			newImg(i, j).b = clamp(bAvg[maxIndex] / (double)max);
			newImg(i, j).g = clamp(gAvg[maxIndex] / (double)max);
		}
	}
	return newImg;
}

Image32 Image32::crop(int x1, int y1, int x2, int y2) const
{
	int width = x2 - x1;
	int height = y2 - y1;

	Image32 newImg;
	newImg.setSize(width, height);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			newImg(i, j).a = (*this)(i + x1, j + y1).a;
			newImg(i, j).r = (*this)(i + x1, j + y1).r;
			newImg(i, j).b = (*this)(i + x1, j + y1).b;
			newImg(i, j).g = (*this)(i + x1, j + y1).g;
		}
	}
	return newImg;
}

Pixel32 Image32::nearestSample(Point2D p) const
{
	Pixel32 newPix;
	newPix.a = (*this)(floor(p[0] + 0.5), floor(p[1] + 0.5)).a;
	newPix.r = (*this)(floor(p[0] + 0.5), floor(p[1] + 0.5)).r;
	newPix.b = (*this)(floor(p[0] + 0.5), floor(p[1] + 0.5)).b;
	newPix.g = (*this)(floor(p[0] + 0.5), floor(p[1] + 0.5)).g;
	return newPix;
}

Pixel32 Image32::bilinearSample(Point2D p) const
{
	Pixel32 newPix;
	double u = p[0];
	double v = p[1];

	double u1 = floor(u);
	double u2 = u1 + 1;
	double v1 = floor(v);
	double v2 = v1 + 1;
	double du = u - u1;
	double dv = v - v1;

	Pixel32 bl;
	Pixel32 br;
	Pixel32 tl;
	Pixel32 tr;

	if (!checkBounds(u1, _width) || !checkBounds(v1, _height)) {
		bl.a = 0;
		bl.r = 0;
		bl.b = 0;
		bl.g = 0;
	}
	else {
		bl.a = (*this)(u1, v1).a;
		bl.r = (*this)(u1, v1).r;
		bl.b = (*this)(u1, v1).b;
		bl.g = (*this)(u1, v1).g;
	}

	if (!checkBounds(u2, _width) || !checkBounds(v1, _height)) {
		br.a = 0;
		br.r = 0;
		br.b = 0;
		br.g = 0;
	}
	else {
		br.a = (*this)(u2, v1).a;
		br.r = (*this)(u2, v1).r;
		br.b = (*this)(u2, v1).b;
		br.g = (*this)(u2, v1).g;
	}

	if (!checkBounds(u1, _width) || !checkBounds(v2, _height)) {
		tl.a = 0;
		tl.r = 0;
		tl.b = 0;
		tl.g = 0;
	}
	else {
		tl.a = (*this)(u1, v2).a;
		tl.r = (*this)(u1, v2).r;
		tl.b = (*this)(u1, v2).b;
		tl.g = (*this)(u1, v2).g;
	}

	if (!checkBounds(u2, _width) || !checkBounds(v2, _height)) {
		tr.a = 0;
		tr.r = 0;
		tr.b = 0;
		tr.g = 0;
	}
	else {
		tr.a = (*this)(u2, v2).a;
		tr.r = (*this)(u2, v2).r;
		tr.b = (*this)(u2, v2).b;
		tr.g = (*this)(u2, v2).g;
	}

	double a = bl.a * (1 - du) + br.a * du;
	double b = tl.a * (1 - du) + tr.a * du;
	newPix.a = a * (1 - dv) + b * dv;

	a = bl.r * (1 - du) + br.r * du;
	b = tl.r * (1 - du) + tr.r * du;
	newPix.r = a * (1 - dv) + b * dv;

	a = bl.g * (1 - du) + br.g * du;
	b = tl.g * (1 - du) + tr.g * du;
	newPix.g = a * (1 - dv) + b * dv;

	a = bl.b * (1 - du) + br.b * du;
	b = tl.b * (1 - du) + tr.b * du;
	newPix.b = a * (1 - dv) + b * dv;

	return newPix;
}

Pixel32 Image32::gaussianSample(Point2D p, double variance, double radius) const
{
	Pixel32 newPix;
	double u = p[0];
	double v = p[1];

	double dstRed = 0;
	double dstBlue = 0;
	double dstGreen = 0;
	double dstAlpha = 0;
	double weight = 0;

	int ulo = floor(u - radius);
	int uhi = ceil(u + radius);
	int vlo = floor(v - radius);
	int vhi = ceil(v + radius);

	for (int iu = ulo; iu < uhi; iu++) {
		for (int iv = vlo; iv < vhi; iv++) {
			double d = sqrt(pow(u - iu, 2) + pow(v - iv, 2));
			double a = (double)iu - u;
			double b = (double)iv - v;
			if (a * a + b * b <= pow(radius, 2)) {
				if (!checkBounds(iu, _width) || !checkBounds(iv, _height)) {
					weight += exp(-(pow(d, 2.0)) / (2.0 * pow(variance, 2.0)));
				}
				else {
					weight += exp(-(pow(d, 2.0)) / (2.0 * pow(variance, 2.0)));
					dstAlpha += exp(-(pow(d, 2.0)) / (2.0 * pow(variance, 2.0))) * (double)(*this)(iu, iv).a;
					dstRed += exp(-(pow(d, 2.0)) / (2.0 * pow(variance, 2.0))) * (double)(*this)(iu, iv).r;
					dstBlue += exp(-(pow(d, 2.0)) / (2.0 * pow(variance, 2.0))) * (double)(*this)(iu, iv).b;
					dstGreen += exp(-(pow(d, 2.0)) / (2.0 * pow(variance, 2.0))) * (double)(*this)(iu, iv).g;
				}
			}
		}
	}
	newPix.a = clamp(dstAlpha / weight);
	newPix.r = clamp(dstRed / weight);
	newPix.b = clamp(dstBlue / weight);
	newPix.g = clamp(dstGreen / weight);
	return newPix;
}

Image32 Image32::shiftChannel(int channel, int amount)
{
	Image32 newImg;
	newImg.setSize(_width, _height);
	for (int j = 0; j < newImg.height(); j++) {
		for (int i = 0; i < newImg.width(); i++) {
			switch(channel)
			{
			case 0:
				newImg(i, j).a = clamp((*this)(i, j).a + amount);
				newImg(i, j).r = (*this)(i, j).r;
				newImg(i, j).b = (*this)(i, j).b;
				newImg(i, j).g = (*this)(i, j).g;
				break;
			case 1:
				newImg(i, j).a = (*this)(i, j).a;
				newImg(i, j).r = clamp((*this)(i, j).r + amount);
				newImg(i, j).b = (*this)(i, j).b;
				newImg(i, j).g = (*this)(i, j).g;
				break;
			case 2:
				newImg(i, j).a = (*this)(i, j).a;
				newImg(i, j).r = (*this)(i, j).r;
				newImg(i, j).b = (*this)(i, j).b;
				newImg(i, j).g = clamp((*this)(i, j).g + amount);
				break;
			case 3:
				newImg(i, j).a = (*this)(i, j).a;
				newImg(i, j).r = (*this)(i, j).r;
				newImg(i, j).b = clamp((*this)(i, j).b + amount);
				newImg(i, j).g = (*this)(i, j).g;
				break;
			default:
				newImg(i, j).a = (*this)(i, j).a;
				newImg(i, j).r = (*this)(i, j).r;
				newImg(i, j).b = (*this)(i, j).b;
				newImg(i, j).g = (*this)(i, j).g;
			}
		}
	}
	return newImg;
}