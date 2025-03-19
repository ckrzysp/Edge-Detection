// Program 4 - HW3
// Author: <Christopher Pawlus>

#include <iostream>
#include <string>
#include <cmath>
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

  // Second Pass
  std::unordered_map<int, int> labelMap;
  int labelCount = 0;

  for (int i = 0; i < rows; i++) 
  {
    for (int j = 0; j < cols; j++) 
    {
      if (labels[i][j] != 0) 
      { // Ignore background pixels
        if (labels[i][j] < 1 || labels[i][j] >= ds.size()) 
        {
          continue;  // Skip to prevent crash
        }

        int rootLabel = ds.find(labels[i][j]);
        if (rootLabel <= 0) 
        {
          continue;  // Skip this pixel to avoid crash
        }

        // Assign unique grayscale value
        if (labelMap.find(rootLabel) == labelMap.end()) 
        {
          labelCount++;
          c = c + (255/(labelMap.size()+1));
          //int t = static_cast<int>(c);
          labelMap[rootLabel] = c;
        }

        an_image.SetPixel(i, j, labelMap[rootLabel]);
      } 
    }
  }

  std::cout<<labelMap.size()<<std::endl;
}

int main(int argc, char **argv){
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
  if (!ReadImage(input_filenm, &img)) {
    std::cout <<"Can't open file " << input_filenm << std::endl;
    return 0;
  }

  // Reading the hough array
  Image img2; // HOUGH
  if (!ReadImage(input_array_filenm, &img2)) {
    std::cout <<"Can't open file " << input_filenm << std::endl;
    return 0;
  }

  // Thresholding
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

  // Non-max suppression
  for(int i = 0; i < img2.num_rows(); i++) 
  {
    for(int k = 0; k < img2.num_columns(); k++)
    {
      if(img2.GetPixel(i,k) > 0) 
      { 
        if(k-1 < 0 || i-1 < 0 || k+1 > img2.num_columns()-1 || i+1 > img2.num_rows()-1) 
        {
          // Boundaries
        }
        else 
        {
          double shift = i/45;
          int dir = round(shift);
          if(dir == 0 || (dir > 3 && dir <= 4))
          {
            if(img2.GetPixel(i,k) < img2.GetPixel(i+1,k+1) || img2.GetPixel(i,k) < img2.GetPixel(i-1,k-1)) 
              img2.SetPixel(i,k,0);
          }
          else if(dir <= 1)
          {
            if(img2.GetPixel(i,k) < img2.GetPixel(i,k+1) || img2.GetPixel(i,k) < img2.GetPixel(i,k-1)) 
              img2.SetPixel(i,k,0);
          }
          else if(dir <= 2) 
          {
            if(img2.GetPixel(i,k) < img2.GetPixel(i-1,k+1) || img2.GetPixel(i,k) < img2.GetPixel(i+1,k+1)) 
              img2.SetPixel(i,k,0);
          }
          else if(dir <= 3) 
          {
            if(img2.GetPixel(i,k) < img2.GetPixel(i-1,k) || img2.GetPixel(i,k) < img2.GetPixel(i+1,k)) 
              img2.SetPixel(i,k,0);
          }
        }
      }
    }
  }

  labeling(img2);
  
  // DrawLine(centroid[o][2], centroid[o][1], centroid[o][2] + 100 * cos(theta1 * (M_PI / 180.0)), 
  // centroid[o][1] + 100 * sin(theta1 * (M_PI / 180.0)), 255, &an_image);

  // Writing the image
  if (!WriteImage(output_line_filenm, img2)){
    std::cout << "Can't write to file " << output_line_filenm << std::endl;
    return 0;
  }

  return 0;
}
