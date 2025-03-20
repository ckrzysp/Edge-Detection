// Program 4 - HW3
// Author: <Christopher Pawlus>

#include <iostream>
#include <string>
#include <cmath>
#include <set>
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

  int max = -1;
  int prev = labels[0][0];
  std::vector<int> lset;
  for(int i = 0; i < rows; i++) 
  {
    for(int j = 0; j < cols; j++) 
    {
      //lset.insert(labels[i][j]);
      if(labels[i][j] > 0)
      {
        lset.push_back(labels[i][j]);
        cout<<labels[i][j]<<": "<<i<<","<<j<<endl;
      }
    }
  }

  /* Second Pass
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
  */
}

void ExtractLinesFromHough(const Image &houghImage, Image &baseImage) 
{

}

/*
void ExtractLinesFromHough(const Image &houghImage, Image &baseImage, int threshold) {
  int numTheta = houghImage.num_rows();       // t
  int numRho = houghImage.num_columns();      // ρ 
  int width = baseImage.num_columns();
  int height = baseImage.num_rows();
  int diag = sqrt(width * width + height * height);  // Max possible p

  cout << "Hough Image Size: " << numTheta << " x " << numRho << endl;
  cout << "Base Image Size: " << width << " x " << height << endl;

  for (int thetaIdx = 0; thetaIdx < numTheta; ++thetaIdx) {
      double theta = thetaIdx * M_PI / 180.0;  // Convert θ index to radians

      for (int rhoIdx = 0; rhoIdx < numRho; ++rhoIdx) {
        if (thetaIdx < 0 || thetaIdx >= houghImage.num_rows() ||
              rhoIdx < 0 || rhoIdx >= houghImage.num_columns()) {
              cerr << "ERROR: Out-of-bounds access in GetPixel() at ("
                  << thetaIdx << ", " << rhoIdx << ")" << endl;
              continue;
          }
          int votes = houghImage.GetPixel(thetaIdx, rhoIdx);
          if (votes < threshold) continue;  // Ignore weak peaks

          double rho = rhoIdx - (numRho / 2);  // Convert ρ index to actual range

          cout << "Processing (t=" << thetaIdx << ", p=" << rho << ", votes=" << votes << ")" << endl;

          // Compute endpoints of the detected line
          int x0, y0, x1, y1;
          double cosTheta = cos(theta);
          double sinTheta = sin(theta);

          if (fabs(sinTheta) > 1e-6) {  // Normal lines
            x0 = 0;
            y0 = static_cast<int>((rho - x0 * cosTheta) / sinTheta);
            x1 = width - 1;
            y1 = static_cast<int>((rho - x1 * cosTheta) / sinTheta);
          } else {  // Handle vertical lines
            x0 = x1 = static_cast<int>(rho);
            y0 = 0;
            y1 = height - 1;
          }

          // valid image bounds
          x0 = max(0, min(x0, width - 1));
          y0 = max(0, min(y0, height - 1));
          x1 = max(0, min(x1, width - 1));
          y1 = max(0, min(y1, height - 1));

          // Check
          if (x0 >= 0 && x0 < width && y0 >= 0 && y0 < height &&
              x1 >= 0 && x1 < width && y1 >= 0 && y1 < height) {
              cout << "Drawing line: (" << x0 << "," << y0 << ") -> (" << x1 << "," << y1 << ")" << endl;
              DrawLine(x0, y0, x1, y1, 255, &baseImage);
          } else {
              cerr << "Skipped out-of-bounds line: (" << x0 << "," << y0 << ") -> (" << x1 << "," << y1 << ")" << endl;
          }
      }
  }
}
*/

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

  //Thresholding
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
  //ExtractLinesFromHough(img2, img);


  // Writing the image
  if (!WriteImage(output_line_filenm, img2)){
    std::cout << "Can't write to file " << output_line_filenm << std::endl;
    return 0;
  }

  return 0;
}


/* Non-max suppression
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
            if(img2.GetPixel(i,k) < img2.GetPixel(i+1,k+1)-50 || img2.GetPixel(i,k) < img2.GetPixel(i-1,k-1)-50) 
              img2.SetPixel(i,k,0);
          }
          else if(dir <= 1)
          {
            if(img2.GetPixel(i,k) < img2.GetPixel(i,k+1)-50 || img2.GetPixel(i,k) < img2.GetPixel(i,k-1)-50) 
              img2.SetPixel(i,k,0);
          }
          else if(dir <= 2) 
          {
            if(img2.GetPixel(i,k) < img2.GetPixel(i-1,k+1)-50 || img2.GetPixel(i,k) < img2.GetPixel(i+1,k+1)-50) 
              img2.SetPixel(i,k,0);
          }
          else if(dir <= 3) 
          {
            if(img2.GetPixel(i,k) < img2.GetPixel(i-1,k)-50 || img2.GetPixel(i,k) < img2.GetPixel(i+1,k)-50) 
              img2.SetPixel(i,k,0);
          }
        }
      }
    }
  }
  */