// Program 4 - HW3
// Author: <Christopher Pawlus>

#include <iostream>
#include <string>
#include <cmath>
#include <set>
#include <fstream>
#include <unordered_map>
#include "image.h"
#include "DisjSets.h"
using namespace ComputerVisionProjects;

void labeling(Image &an_image) 
{
  int rows = an_image.num_rows();
  int cols = an_image.num_columns();

  int label_count = 1;
  DisjSets ds(rows*cols);
  vector<vector<int>> labels(rows, vector<int>(cols,0));
  float c = 0;

  // First Pass
  for(int i = 0; i < rows; i++) 
  {
    for(int j = 0; j < cols; j++) 
    {
      if(an_image.GetPixel(i,j) == 0) 
      {
        // Ignore background
        continue;
      }

      int left = (j > 0) ? labels[i][j - 1] : 0;
      int top = (i > 0) ? labels[i - 1][j] : 0;

      if(left == 0 && top == 0) 
      {
        labels[i][j] = label_count;
        label_count++;
      }
      else if(left != 0 && top == 0) 
      {
        labels[i][j] = left;
      }
      else if(left == 0 && top != 0) 
      {
        labels[i][j] = top;
      }
      else 
      {
        labels[i][j] = min(left, top);
        if(ds.find(left) != ds.find(top)) 
        {
          ds.unionSets(ds.find(left), ds.find(top));
        }
      }
    }
  }
}

void lines(const Image &houghImage, Image &baseImage, int threshold) 
{
  int numTheta = houghImage.num_rows();  // T
  int numRho = houghImage.num_columns(); // R
  int width = baseImage.num_columns();
  int height = baseImage.num_rows();
  int diag = sqrt(width * width + height * height);  // Max possible rho

  for (int thetaIdx = 0; thetaIdx < numTheta; thetaIdx++) {
    double theta = 0.5 * thetaIdx * M_PI / 180.0;  // Conversion with the dt from h3.cc
    for (int rhoIdx = 0; rhoIdx < numRho; rhoIdx++) 
    {
      int votes = houghImage.GetPixel(thetaIdx, rhoIdx);
      if (votes < threshold)
      {
        continue;  // Ignore 
      }
      double rho = 0.5 * rhoIdx;  // Rho with dr from h3.cc

      // Compute endpoints of the detected line
      int x0, y0, x1, y1;
      std::vector<std::pair<int, int>> vec;
      
      //#1 Case
      double t = rho/cos(theta);
      if(t >= 0 && t < width) 
      {
        vec.push_back({t, 0});
      } 
      //#2 Case
      t = (rho-(width-1)*sin(theta))/cos(theta);
      if(t >= 0 && t < width) 
      {
        vec.push_back({t, width-1});
      }
      //#3 Case
      t = (rho-(height-1)*cos(theta))/sin(theta);
      if(t >= 0 && t < height) 
      {
        vec.push_back({height-1, t});
      }
      //#4 Case
      t = rho/sin(theta);
      if(t >= 0 && t < height) 
      {
        vec.push_back({0, t});
      } 
      if(vec.size() >= 2) 
      {
        x0 = vec[0].first;
        y0 = vec[0].second;
        x1 = vec[1].first;
        y1 = vec[1].second;
        // Check again
        if (x0 >= 0 && x0 < height && y0 >= 0 && y0 < width && x1 >= 0 && x1 < height && y1 >= 0 && y1 < width) {
          DrawLine(x0, y0, x1, y1, 255, &baseImage);
        } 
      }
    }
  }
}

int main(int argc, char **argv)
{
  if (argc != 5) {
    std::cout << " Usage: " <<
      argv[0] << " input_image input_hough_array input_threshold output_line_image" << std::endl;
    return 0;
  }
  const std::string input_filenm(argv[1]);
  const std::string input_array_filenm(argv[2]);
  const int threshold = std::stoi(argv[3]);
  const std::string output_line_filenm(argv[4]);

  std::cout << "Running h4: " << input_filenm << " " << input_array_filenm << " " << threshold
            << "  " << output_line_filenm << std::endl;

  // Reading the image
  Image img;
  Image img2;
  if (!ReadImage(input_filenm, &img)) {
    std::cout <<"Can't open file " << input_filenm << std::endl;
    return 0;
  }

  // Text to image conversion for the hough array
  std::ifstream file(input_array_filenm);
  std::string a = " ", b = " ";
  std::getline(file, a);
  std::getline(file, b);
  int T = std::stoi(a);
  int R = std::stoi(b);
  int rows = 0;
  int cols = 0;
  img2.AllocateSpaceAndSetSize(T,R);
  img2.SetNumberGrayLevels(255);
  std::string f = "";
  // Copying
  while(std::getline(file, f)) 
  {
    std::string s = "";
    for(int i = 0; i < f.size(); i++) 
    {
      if(f[i] == ' ') 
      {
        img2.SetPixel(rows,cols,std::stoi(s));
        cols++;
        s = "";
      }
      else 
      {
        s += f[i];
      }
    }
    cols = 0;
    rows++;
  }

  for(int i = 0; i < img2.num_rows(); i++) 
  {
    for(int k = 0; k < img2.num_columns(); k++)
    {
      if(img2.GetPixel(i,k) < threshold) 
      { 
        img2.SetPixel(i,k,0);
      }
    }
  }
  
  labeling(img2);
  lines(img2, img, threshold);

  // Writing the image
  if (!WriteImage(output_line_filenm, img)){
    std::cout << "Can't write to file " << output_line_filenm << std::endl;
    return 0;
  }

  return 0;
}