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


CImg<float> DCT(CImg<float> sub_image, int quality)
{
    
    for(int x=0; x<N; x++){
        for(int y=0; y<N;y++){
            sub_image(x,y) -= 128;
        }
    }
     CImg<> Q(8,8);
    Q(0,0)=16;   Q(0,1)=11;   Q(0,2)=10;   Q(0,3)=16;   Q(0,4)=24;   Q(0,5)=40;   Q(0,6)=51;   Q(0,7)=61;
    Q(1,0)=12;   Q(1,1)=12;   Q(1,2)=14;   Q(1,3)=19;   Q(1,4)=26;   Q(1,5)=58;   Q(1,6)=60;   Q(1,7)=55;
    Q(2,0)=14;   Q(2,1)=13;   Q(2,2)=16;   Q(2,3)=24;   Q(2,4)=40;   Q(2,5)=57;   Q(2,6)=69;   Q(2,7)=56;
    Q(3,0)=14;   Q(3,1)=17;   Q(3,2)=22;   Q(3,3)=29;   Q(3,4)=51;   Q(3,5)=87;   Q(3,6)=80;   Q(3,7)=62;
    Q(4,0)=18;   Q(4,1)=22;   Q(4,2)=37;   Q(4,3)=56;   Q(4,4)=68;   Q(4,5)=109;  Q(4,6)=103;  Q(4,7)=77;
    Q(5,0)=24;   Q(5,1)=35;   Q(5,2)=55;   Q(5,3)=64;   Q(5,4)=81;   Q(5,5)=104;  Q(5,6)=113;  Q(5,7)=92;
    Q(6,0)=49;   Q(6,1)=64;   Q(6,2)=78;   Q(6,3)=87;   Q(6,4)=103;  Q(6,5)=121;  Q(6,6)=120;  Q(6,7)=101;
    Q(7,0)=72;   Q(7,1)=92;   Q(7,2)=95;   Q(7,3)=98;   Q(7,4)=112;  Q(7,5)=100;  Q(7,6)=103;  Q(7,7)=99;
    Q *= quality; 
    
    CImg<float> res(sub_image.width(),sub_image.height(),1,1,0);
    float dct=0;
    float somme=0;
    for(int i=0; i < res.height(); i++) {
        for(int j=0; j < res.width(); j++){
            //std::cout << (float)sub_image(i,j) << std::endl;
            somme = 0;
            for(int x=0; x<N; x++){
                for(int y=0; y<N;y++){
                    somme += sub_image(x,y) *cos((2*x+1)*i*M_PI/(2*8))*cos((2*y+1)*j*M_PI/(2*8));
                }
            }
            dct = (2.0/N)*(i>0?1:1.0/sqrt(2))*(j>0?1:1.0/sqrt(2))*somme;
            
            res(i,j) = dct;
        }
    }

    // for(int i=0; i < res.height(); i++) {
    //     for(int j=0; j < res.width(); j++){
    //         res(i,j) = res(i,j) / (float)Q(i,j);
    //         std::cout << res(i,j)  << std::endl;
    //     }
    // }
    return res;
}

void copy(CImg<float> & dest, CImg<float> & src, int x, int y, int offset) {
    int i = 0, j = 0;
    for(int yy= y; yy < y + offset; yy+=1) {
        for(int xx= x; xx < x + offset; xx+=1){
            dest(xx, yy) = src(i, j);
            i++; 
        }
        j++;
    }
}


CImg<float> JPEGEncoder(CImg<unsigned char> image, float quality)
{
    CImg<> Q(8,8);
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

    int step = 8;
    for(int y=0; y < image.height(); y+=8) {
        for(int x=0; x < image.width(); x+=8){
            CImg<float> sub_image = image.get_crop(x,y,x+(step-1),y+(step-1));
            compress = DCT(sub_image, quality);
            
            copy(comp, compress, x, y, step);
        }
    }

 // Quantization matrix


 // TODO: code to insert

 return comp;
}

int main()
{
 // Read the image "lena.bmp"
 CImg<unsigned char> my_image("lena.bmp");

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
 CImgDisplay comp_disp(comp_image,"Compressed Image");

 while (!main_disp.is_closed());
 {
  main_disp.wait();
 }
}

