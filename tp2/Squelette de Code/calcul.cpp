#include "calcul.hpp"
#include <vector>

float C(float i) {
    if(i == 0) {
        return 1./sqrt(2);
    }
    return 1.0;
}

CImg<float> DCT(CImg<float> & sub_image) {
    
    CImg<float> res(sub_image.width(),sub_image.height());
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

CImg<float> DCT_inverse(CImg<float> & image_encodee) {
    
    CImg<float> res(image_encodee.width(),image_encodee.height());
    float dct_inv = 0;
    float somme = 0;

    for(float x=0; x < res.height(); x++) {
        for(float y=0; y < res.width(); y++){

            somme = 0;
            for(float i = 0; i < N; i++){
                for(float j = 0; j < N; j++){

                    double coss = cos(((2*x + 1) * i * M_PI) / (2.0 * N));
                    double coss_b = cos(((2*y + 1) * j * M_PI) / (2.0 * N));
                    somme = somme + (C(i) * C(j) * image_encodee(i,j) * coss * coss_b);
                }
            }

            dct_inv = (2.0 / N) * somme;
            res(x,y) = dct_inv;
        }
    }

    return res;
}


CImg<float> JPEGEncoder(CImg<float> image, float quality) {
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
    
    CImg<float> comp(image.width(),image.height());
    CImg<float> compress(8,8);
    division div_op{};

    cimg_forXY(image, x, y) {
        image(x,y) -= 128;
    }

    int step = 8;
    for(int y=0; y < image.height() - 1; y+=8) {
        for(int x=0; x < image.width() - 1; x+=8){
            int offx = x + (step - 1);
            int offy = y + (step - 1);

            CImg<float> sub_image = image.get_crop(x, y, offx, offy);

            compress = DCT(sub_image); 
            Quantification(compress, Q, div_op);

            for(int l = 0; l < 8 ; ++l) {
                for(int k = 0; k < 8 ; ++k ) {
                    comp(x + l, y + k) = compress(l, k);
                }
            }
        }
    }
    return comp;
}


CImg<float> JPEGDecoder(CImg<float> image, float quality) {
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
    
    
    CImg<float> comp(image.width(),image.height());
    CImg<float> compress(8,8);
    multiplication mul{};

    int step = 8;
    for(int y=0; y < image.height() - 1; y+=8) {
        for(int x=0; x < image.width() - 1; x+=8){

            int offx = x + (step - 1);
            int offy = y + (step - 1);

            CImg<float> sub_image = image.get_crop(x, y, offx, offy);
            Quantification(sub_image, Q, mul); 
            compress = DCT_inverse(sub_image); 

            for(int l = 0; l < 8 ; ++l) {
                for(int k = 0; k < 8 ; ++k ) {
                    comp(x + l, y + k) = compress(l, k);
                }
            }
        }
    }
    return comp;
}


float distorsion(CImg<float> & original_image, CImg<float> & encoded_image) {

    int width = original_image.width() - 1;
    int height = original_image.height() - 1;
    float NM = original_image.width() * original_image.height();
    
    float somme = 0;

    for(int x = 0; x < width; ++x) {
        for(int y = 0; y < height; ++y) {
            float diff = std::pow(original_image(x, y) - encoded_image(x, y), 2);
            somme += diff;
        }
    }  

    
    return (1.0/NM) * somme;
}

void drawDistorsion(float qmin, float qmax, float pas, CImg<float> & image) {

    std::vector<float> v;
    CImg<float> comp;
    //Create plot data.
    CImg<double> values(1, (qmax-qmin)/pas, 1, 1, 0);
    double dist;
    int i=0;

    for (float q = qmin; q <= qmax; q+=pas){
        comp = JPEGEncoder(image,q);
        comp = JPEGDecoder(comp, q);
        dist = distorsion(image,comp);
        values(0,i) = dist; 
        i++;
    }

    CImgDisplay disp;
    //values.display_graph(disp, plot_type, vertex_type, "X Axis", x0, x1, "Y Axis", y0, y1);
    values.display_graph(disp, 1, 1, "X Axis", qmin, qmax, "Y Axis");
}