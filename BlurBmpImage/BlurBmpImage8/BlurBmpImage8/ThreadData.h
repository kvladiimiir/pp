#pragma once
#include "bitmap_image.hpp"
struct ThreadData
{
	bitmap_image ibmp;
	bitmap_image* obmp;
	int startIndex;
	int endIndex;
};

const int BLUR_RADIUS = 10;