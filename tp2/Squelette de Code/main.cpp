#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

#include "CImg.h" //tuto : https://cimg.eu/reference/tutorial.html

#define ROUND( a ) ( ( (a) < 0 ) ? (int) ( (a) - 0.5 ) : (int) ( (a) + 0.5 ) )

float N = 8;

using namespace std;
using namespace cimg_library;


float C(float i) {
    if(i == 0) {
        return 1./sqrt(2);
    }
    return 1.0;
}

CImg<float> DCT(CImg<float> & sub_image)
{
    
    CImg<float> res(sub_image.width(),sub_image.height(),1,1,0);
    float dct = 0;
    float somme = 0;

    for(float i=0; i < res.height(); i++) {
        for(float j=0; j < res.width(); j++){
            somme = 0;

            for(float x = 0; x < N; x++){
                for(float y = 0; y < N; y++){

                    double coss = cos(((2*x + 1) * i * M_PI) / (2.0 * N));
                    double coss_b = cos(((2*y + 1) * j * M_PI) / (2.0 * N));
                    somme = somme + ( sub_image(x,y) * coss * coss_b );
                }
            }
            dct = (2.0 * C(i) * C(j) * somme) / N ;
            res(i,j) = dct;
        }
    }

    return res;
}

void Quantification(CImg<float> & sub_image, CImg<float> & Q) {

    for(int i=0; i < sub_image.height(); i++) {
        for(int j=0; j < sub_image.width(); j++){
            sub_image(i,j) = round (sub_image(i,j) / ((float)Q(i,j)) );
        }
    }
}


void copyImg(CImg<float> & dest, CImg<float> & src, int x, int y, int offset) {
    int i = 0, j = 0;
    for(int yy= y; yy < y + offset - 1; yy++) {
        for(int xx= x; xx < x + offset - 1; xx++){
            dest(xx, yy) = src(i, j);
            if(i > 8) {
                i = 0;
            } else {
                i++;
            }
        }
        if(j > 8) {
            j = 0;
        } else {
            j++;
        }
    }
}


CImg<float> JPEGEncoder(CImg<float> image, float quality)
{
    CImg<float> Q(8,8);
    Q(0,0)=16;   Q(0,1)=11;   Q(0,2)=10;   Q(0,3)=16;   Q(0,4)=24;   Q(0,5)=40;   Q(0,6)=51;   Q(0,7)=61;
    Q(1,0)=12;   Q(1,1)=12;   Q(1,2)=14;   Q(1,3)=19;   Q(1,4)=26;   Q(1,5)=58;   Q(1,6)=60;   Q(1,7)=55;
    Q(2,0)=14;   Q(2,1)=13;   Q(2,2)=16;   Q(2,3)=24;   Q(2,4)=40;   Q(2,5)=57;   Q(2,6)=69;   Q(2,7)=56;
    Q(3,0)=14;   Q(3,1)=17;   Q(3,2)=22;   Q(3,3)=29;   Q(3,4)=51;   Q(3,5)=87;   Q(3,6)=80;   Q(3,7)=62;
    Q(4,0)=18;   Q(4,1)=22;   Q(4,2)=37;   Q(4,3)=56;   Q(4,4)=68;   Q(4,5)=109;  Q(4,6)=103;  Q(4,7)=77;
    Q(5,0)=24;   Q(5,1)=35;   Q(5,2)=55;   Q(5,3)=64;   Q(5,4)=81;   Q(5,5)=104;  Q(5,6)=113;  Q(5,7)=92;
    Q(6,0)=49;   Q(6,1)=64;   Q(6,2)=78;   Q(6,3)=87;   Q(6,4)=103;  Q(6,5)=121;  Q(6,6)=120;  Q(6,7)=101;
    Q(7,0)=72;   Q(7,1)=92;   Q(7,2)=95;   Q(7,3)=98;   Q(7,4)=112;  Q(7,5)=100;  Q(7,6)=103;  Q(7,7)=99;
    Q *= quality; 
    
    CImg<float> comp(image.width(),image.height(),1,1,0);
    CImg<float> compress(8,8,1,1,0);

    cimg_forXY(image, x, y) {
        image(x,y) -= 128;
    }

    //image = image.get_crop(0, 0, 16, 16);
    //CImg<float> comp(16,16,1,1,0);
    /*
        7 : 7
        15 : 7
        23 : 7
        7 : 15
        15 : 15
        23 : 15
        7 : 23
        15 : 23
        23 : 23
    */

    int step = 8;
    for(int y=0; y < image.height() - 1; y+=8) {
        for(int x=0; x < image.width() - 1; x+=8){
            int offx = x + (step - 1);
            int offy = y + (step - 1);
            CImg<float> sub_image = image.get_crop(x, y, offx, offy);
            compress = DCT(sub_image); 
            Quantification(compress, Q);
            copyImg(comp, compress, x, y, step);
        }
    }
    //comp = image;
    return comp;
}

int main()
{
 // Read the image "lena.bmp"
 CImg<float> my_image("lena.bmp");

 // Take the luminance information 
 my_image.channel(0);
 
 float quality=1.;
 CImg<float> comp_image = JPEGEncoder(my_image,quality);


 /////////// TO DELETE /////////// 
 // this part of code does a zooming on 8*8 pixels (pretty print)
 // CImg Library => basic operations
 int step = 8;
 int x0 = 16;
 int y0 = 32;
 CImg<float> sub_image = comp_image.get_crop(x0,y0,x0+(step-1),y0+(step-1));
 sub_image.resize(-3200,-3200);
 CImgDisplay sub_disp(sub_image,"Sub Image"); 
 /////////// TO DELETE ///////////


 // Display the bmp file
 CImgDisplay main_disp(my_image,"Initial Image");

 // Display the compressed file (by dct)
 //comp_image.resize(-3200,-3200);
 CImgDisplay comp_disp(comp_image,"Compressed Image");

 while (!main_disp.is_closed());
 {
  main_disp.wait();
 }
}

