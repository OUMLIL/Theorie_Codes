#include "calcul.hpp"

int main()
{
/*
 // Read the image "lena.bmp"
 CImg<float> my_image("lena.bmp");

 // Take the luminance information 
 my_image.channel(0);
 
 float quality=1.0;
 CImg<float> comp_image = JPEGEncoder(my_image, quality);
 CImg<float> decoded_image = JPEGDecoder(comp_image, quality);


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

 //Distortion
 std::cout << "Distortion: " << distorsion(my_image, decoded_image) << std::endl;


 // Display the bmp file
 CImgDisplay main_disp(my_image,"Initial Image");

 // Display the compressed file (by dct)
 //comp_image.resize(-3200,-3200);
 //CImgDisplay comp_disp(comp_image,"Compressed Image");
 //CImgDisplay decode_disp(decoded_image, "Decoded Image");

 while (!main_disp.is_closed());
 {
  main_disp.wait();
 }
 */
 // Read the image "lena.bmp"
 CImg<float> my_image("lena.bmp");

 // Take the luminance information 
 my_image.channel(0);
 
 float quality,qmin,qmax,pas;
 CImg<float> comp_image;
 CImg<float> decoded_image;
 int input;
 std::cout << "Choisissez une action a effectuer :" << std::endl;
 std::cout << "1. Appliquer la DCT et afficher le resultat. Vous devez fournir un facteur de qualite." << std::endl;
 std::cout << "2. Appliquer la DCT puis la DCT inverse et afficher l'image decompressee. Vous devez fournir un facteur de qualite." << std::endl;
 std::cout << "3. Calculer le taux de distorsion en fonction du facteur de qualite. Vous devez fournir un facteur de qualite minimum, un facteur de qualite maximum, et un pas." << std::endl;
 std::cin >> input;
 if (input == 1) {
    std::cout << "Veuillez fournir un facteur de qualité :" << std::endl;
    std::cin >> quality;
    comp_image = JPEGEncoder(my_image, quality);
    CImgDisplay main_disp(my_image,"Initial Image");
    CImgDisplay comp_disp(comp_image,"Compressed Image");
    while (!main_disp.is_closed());
    {
        main_disp.wait();
    }
 }
if (input == 2) {
    std::cout << "Veuillez fournir un facteur de qualité :" << std::endl;
    std::cin >> quality;
    comp_image = JPEGEncoder(my_image, quality);
    decoded_image = JPEGDecoder(comp_image, quality);
    std::cout << "Distortion: " << distorsion(my_image, comp_image) << std::endl;
    CImgDisplay main_disp(my_image,"Initial Image");
    CImgDisplay comp_disp(comp_image,"Compressed Image");
    CImgDisplay decoded_disp(decoded_image,"Decoded Image");
    while (!main_disp.is_closed());
    {
        main_disp.wait();
    }
 }
 if (input == 3) {
    std::cout << "Veuillez fournir un facteur de qualité minimal :" << std::endl;
    std::cin >> qmin;
    std::cout << "Veuillez fournir un facteur de qualité maximal :" << std::endl;
    std::cin >> qmax;
    std::cout << "Veuillez fournir un pas :" << std::endl;
    std::cin >> pas;
    drawDistorsion(qmin,qmax,pas,my_image);
 }
}

