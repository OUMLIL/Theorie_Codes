#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

#include "CImg.h" //tuto : https://cimg.eu/reference/tutorial.html

#define ROUND( a ) ( ( (a) < 0 ) ? (int) ( (a) - 0.5 ) : (int) ( (a) + 0.5 ) )
#define N 8

using namespace std;
using namespace cimg_library;


class division {
    public:
        float operator()(float a, float b) {
            return ROUND(a/b);
        }
};

class multiplication {
    public:
        float operator()(float a, float b) {
            return a*b;
        }
};

template <typename BinaryFunction>
void Quantification(CImg<float> & sub_image, CImg<float> & Q, BinaryFunction arithmetic_operator) {
    for(int i=0; i < sub_image.height(); i++) {
        for(int j=0; j < sub_image.width(); j++){
            sub_image(i,j) = arithmetic_operator(sub_image(i,j), ((float)Q(i,j)));
        }
    }
}


float C(float i);
float distorsion(CImg<float> & original_image, CImg<float> & encoded_image);

CImg<float> DCT(CImg<float> & sub_image);
CImg<float> DCT_inverse(CImg<float> & image_encodee);
CImg<float> JPEGEncoder(CImg<float> image, float quality);
CImg<float> JPEGDecoder(CImg<float> image, float quality);
void drawDistorsion(float qmin, float qmax, float pas, CImg<float> & image);