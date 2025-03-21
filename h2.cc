// Program 2 - HW3
// Author: <Christopher Pawlus>

#include "image.h"
#include <iostream>
#include <string>

using namespace ComputerVisionProjects;

int main(int argc, char **argv)
{
  if (argc != 4) {
    std::cout << "Usage: " <<
      argv[0] << " {input_image_name} {threshold} {output_image_name}" << std::endl;
    return 0;
  }
  const std::string input_file(argv[1]);
  const int threshold = std::stoi(argv[2]);
  const std::string output_file(argv[3]);

  std::cout << "Running h2 " << input_file << " " 
  << threshold << " " << output_file << std::endl;

  // Reading the image
  Image img;
  if (!ReadImage(input_file, &img)) {
    std::cout <<"Can't open file " << input_file << std::endl;
    return 0;
  }

  // Thresholding
  for(int i = 0; i < img.num_rows(); i++) 
  {
    for(int k = 0; k < img.num_columns(); k++) 
    {
      if(img.GetPixel(i,k) >= threshold) 
      {
        img.SetPixel(i,k, 255);
      }
      else 
      {
        img.SetPixel(i,k, 0);
      }
    }
  }
  // Writing the image
  if (!WriteImage(output_file, img)){
    std::cout << "Can't write to file " << output_file << std::endl;
    return 0;
  }

  return 0;
}
