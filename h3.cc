// Program 3 - HW3
// Author: <Christopher Pawlus>

#include "image.h"
#include <cmath>
#include <iostream>
#include <string>

using namespace ComputerVisionProjects;

int main(int argc, char **argv){
  if (argc != 4) {
      std::cout << "Usage: " <<
        argv[0] << " {input_binary_image_name} {output_hough_image_name} {output_hough_array_name}" << std::endl;
      return 0;
  }
  const std::string input_filenm(argv[1]);
  const std::string output_pgm_filenm(argv[2]);
  const std::string output_txt_filenm(argv[3]);

  std::cout << "Running h3: " << input_filenm << " " << output_pgm_filenm << " " << output_txt_filenm << std::endl;
  
  Image img;
  Image img2;
  if (!ReadImage(input_filenm, &img)) {
    std::cout <<"Can't open file " << input_filenm << std::endl;
    return 0;
  }
  
  // Accumulator array
  int rho = sqrt((img.num_rows()*img.num_rows()) + (img.num_columns()*img.num_columns()));
  int delrho = 1;
  int deltheta = 180/delrho;
  int acc[deltheta][rho*2] = {0};
  float rhoc = 0;
  int index = 0;
  // x*cos(theta) + y*sin(theta); did not work, I switched the x and y around.

  for(int i = 0; i < img.num_rows(); i++) 
  {
    for(int k = 0; k < img.num_columns(); k++) 
    {
      if(img.GetPixel(i,k) > 0) 
      { 
        for(int h = 0; h < deltheta; h++) 
        {
          // Degrees
          float t = h * (M_PI/180);
          
          // Finding rho as the point to plot in acc
          rhoc = i*cos(t) + k*sin(t);
          index = round(rhoc) + rho;
          
          // Boundary checkking
          if(index >= 0 && index < rho*2) 
          {
            acc[h][index]++;
          }
        }
      }
    }
  }

  if (!WriteImage(output_pgm_filenm, img)) {
    std::cout << "Can't write to file " << output_txt_filenm << std::endl;
    return 0;
  }
  
  if (!ReadImage(input_filenm, &img2)) {
    std::cout <<"Can't open file " << input_filenm << std::endl;
    return 0;
  }

  // Bounds of accumulator array onto new image
  img2.AllocateSpaceAndSetSize(deltheta,rho*2);
  for(int i = 0; i < img2.num_rows(); i++) 
  {
    for(int k = 0; k < img2.num_columns(); k++) 
    {
      img2.SetPixel(i,k,acc[i][k]);
    }
  }
  
  // Writing need image with accumulator array bounds
  if (!WriteImage(output_pgm_filenm, img2)) {
    std::cout << "Can't write to file " << output_txt_filenm << std::endl;
    return 0;
  }
  
  return 0;
}
