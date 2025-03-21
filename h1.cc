// Program 1 - HW3
// Author: <Christopher Pawlus>

#include "image.h"
#include <iostream>
#include <vector>
#include <cmath>
using namespace ComputerVisionProjects;

int main(int argc, char **argv)
{
  if (argc != 3) {
    std::cout << "Usage: " <<
      argv[0] << " {input_image_name} {output_edge_image_name}" << std::endl;
      return 0;
  }

  const std::string input_file(argv[1]);
  const std::string output_file(argv[2]);

  std::cout << "Running h1 " << input_file << " " << output_file << std::endl;

  // Reading the image
  Image img;
  if (!ReadImage(input_file, &img)) {
    std::cout <<"Can't open file " << input_file << std::endl;
    return 0;
  }

  // X SOBEL
  int delx[9] = {-1, 0, 1,
                 -2, 0, 2,
                 -1, 0, 1};
  // Y SOBEL
  int dely[9] = { 1, 2, 1,
                  0, 0, 0,
                 -1,-2,-1};
  int x = 0, y = 0;
  std::vector<std::vector<int>> arr(img.num_rows(), std::vector<int>(img.num_columns(), 0)); // Magnitude array
  for(int i = 0; i < img.num_rows(); i++) 
  {
    for(int k = 0; k < img.num_columns(); k++) 
    {
      if(k-1 < 0 || i-1 < 0 || k+1 > img.num_columns()-1 || i+1 > img.num_rows()-1) 
      {
        // Boundary checking
      }
      else 
      {
        // Since Zeros are negligible, I picked the non-zero values from the 3x3 Soble filter
        // X
        // x = ((img.GetPixel(i+1,k+1) - img.GetPixel(i, k+1)) + (img.GetPixel(i+1,k) - img.GetPixel(i,k)));
        x += delx[0] * img.GetPixel(i-1,k-1);
        x += delx[2] * img.GetPixel(i-1,k);
        x += delx[3] * img.GetPixel(i-1,k+1);
        x += delx[5] * img.GetPixel(i+1,k-1);
        x += delx[6] * img.GetPixel(i+1,k);
        x += delx[8] * img.GetPixel(i+1,k+1);
        // Y
        // y = ((img.GetPixel(i+1,k+1) - img.GetPixel(i+1, k)) + (img.GetPixel(i,k+1) - img.GetPixel(i,k)));
        y += dely[0] * img.GetPixel(i-1,k-1);
        y += dely[1] * img.GetPixel(i,k-1);
        y += dely[2] * img.GetPixel(i+1,k-1);
        y += dely[6] * img.GetPixel(i-1,k+1);
        y += dely[7] * img.GetPixel(i,k+1);
        y += dely[8] * img.GetPixel(i+1,k+1);
        // Storing
        arr[i][k] = sqrt((x*x) + (y*y));
        x = 0, y = 0;
      }
    }
  }

  // Combining both
  for(int i = 0; i < img.num_rows(); i++) 
  {
    for(int k = 0; k < img.num_columns(); k++) 
    {
      img.SetPixel(i,k, arr[i][k]);
    }
  }

  // Writing the image
  if (!WriteImage(output_file, img)){
    std::cout << "Can't write to file " << output_file << std::endl;
    return 0;
  }

  return 0;
}
