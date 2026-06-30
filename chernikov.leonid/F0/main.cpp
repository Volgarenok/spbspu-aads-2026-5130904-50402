#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <unistd.h>
#include "fits_file.h"
#include "quad_tree.h"

double getFitsPixel(void *context, int x, int y)
{
  FitsFile *fits = static_cast< FitsFile * >(context);
  return fits->getPixelAsDouble(x, y);
}

void printHistogram(FitsFile &fits, int bins = 20)
{
  if (fits.totalPixels() == 0)
  {
    std::cout << "No data to display histogram." << std::endl;
    return;
  }

  double minVal = fits.computeMin();
  double maxVal = fits.computeMax();

  if (minVal == maxVal)
  {
    std::cout << "Histogram: All pixels have value " << minVal << std::endl;
    return;
  }

  double binWidth = (maxVal - minVal) / bins;

  std::vector< int > histogram(bins, 0);
  int maxCount = 0;

  for (int y = 0; y < fits.height; y++)
  {
    for (int x = 0; x < fits.width; x++)
    {
      double val = fits.getPixelAsDouble(x, y);
      int bin = static_cast< int >((val - minVal) / binWidth);
      if (bin >= bins)
        bin = bins - 1;
      if (bin < 0)
        bin = 0;
      histogram[bin]++;
      if (histogram[bin] > maxCount)
        maxCount = histogram[bin];
    }
  }

  int maxBarWidth = 60;
  std::cout << "=== Histogram (" << bins << " bins) ===" << std::endl;
  std::cout << "Range: [" << minVal << ", " << maxVal << "]" << std::endl;
  std::cout << "Total pixels: " << fits.totalPixels() << std::endl;

  for (int i = 0; i < bins; i++)
  {
    double binStart = minVal + i * binWidth;
    double binEnd = binStart + binWidth;
    std::cout << std::fixed << std::setprecision(1) << "[" << std::setw(8) << binStart << " - " << std::setw(8)
              << binEnd << "] | ";
    int barWidth = (maxCount > 0) ? (histogram[i] * maxBarWidth / maxCount) : 0;
    for (int j = 0; j < barWidth; j++)
      std::cout << "#";
    std::cout << " " << histogram[i] << std::endl;
  }
}

void printHelp()
{
  std::cout << "Available commands:" << std::endl;
  std::cout << "  LOAD <file>        - Load FITS file" << std::endl;
  std::cout << "  SAVE <file>        - Save current data to FITS file" << std::endl;
  std::cout << "  HEADER             - Print all header keywords" << std::endl;
  std::cout << "  INFO               - Print file information" << std::endl;
  std::cout << "  STAT               - Print statistics" << std::endl;
  std::cout << "  HISTOGRAM          - Display ASCII histogram" << std::endl;
  std::cout << "  BUILD_TREE         - Build Quad-Tree" << std::endl;
  std::cout << "  GET_PIXEL <x> <y>  - Get pixel value via Quad-Tree" << std::endl;
  std::cout << "  FIND_SOURCES <s>   - Find bright pixels" << std::endl;
  std::cout << "  CALIBRATE <dark>   - Subtract dark frame" << std::endl;
  std::cout << "  HELP               - Show this message" << std::endl;
  std::cout << "  EXIT               - Exit program" << std::endl;
}

int main()
{
  FitsFile currentFile;
  QuadTree quadTree;
  bool treeBuilt = false;
  bool isInteractive = isatty(fileno(stdin));

  if (isInteractive)
  {
    std::cout << "FITS Image Analyzer v1.0" << std::endl;
    std::cout << "Type HELP for available commands." << std::endl;
  }

  std::string line;
  while (true)
  {
    if (isInteractive)
    {
      std::cout << "> ";
    }
    if (!std::getline(std::cin, line))
    {
      break;
    }
    if (line.empty())
      continue;

    std::istringstream iss(line);
    std::string cmd;
    iss >> cmd;

    for (size_t i = 0; i < cmd.length(); i++)
    {
      cmd[i] = static_cast< char >(std::toupper(cmd[i]));
    }

    if (cmd == "LOAD")
    {
      std::string filename;
      iss >> filename;
      if (!filename.empty())
      {
        if (currentFile.load(filename))
          treeBuilt = false;
      } else
      {
        std::cout << "Usage: LOAD <filename>" << std::endl;
      }
    } else if (cmd == "HEADER")
    {
      if (!currentFile.loaded)
      {
        std::cout << "No file loaded." << std::endl;
        continue;
      }
      std::cout << "=== FITS Header ===" << std::endl;
      currentFile.header.printInOrder();
      std::cout << "=== End of Header ===" << std::endl;
    } else if (cmd == "INFO")
    {
      currentFile.printInfo();
    } else if (cmd == "STAT")
    {
      if (!currentFile.loaded)
      {
        std::cout << "No file loaded." << std::endl;
        continue;
      }
      std::cout << "=== Statistics ===" << std::endl;
      std::cout << std::fixed << std::setprecision(6);
      std::cout << "  Min:    " << std::setw(15) << currentFile.computeMin() << std::endl;
      std::cout << "  Max:    " << std::setw(15) << currentFile.computeMax() << std::endl;
      std::cout << "  Mean:   " << std::setw(15) << currentFile.computeMean() << std::endl;
      std::cout << "  StdDev: " << std::setw(15) << currentFile.computeStdDev() << std::endl;
      std::cout << "==================" << std::endl;
    } else if (cmd == "HISTOGRAM" || cmd == "HIST")
    {
      if (!currentFile.loaded)
      {
        std::cout << "No file loaded." << std::endl;
        continue;
      }
      printHistogram(currentFile);
    } else if (cmd == "BUILD_TREE" || cmd == "BUILD")
    {
      if (!currentFile.loaded)
      {
        std::cout << "No file loaded." << std::endl;
        continue;
      }
      if (currentFile.totalPixels() == 0)
      {
        std::cout << "No pixel data to build tree." << std::endl;
        continue;
      }
      std::cout << "Building Quad-Tree... ";
      quadTree.build(&currentFile, getFitsPixel, currentFile.width, currentFile.height);
      treeBuilt = true;
    } else if (cmd == "GET_PIXEL" || cmd == "GET")
    {
      if (!treeBuilt)
      {
        std::cout << "Build Quad-Tree first with BUILD_TREE." << std::endl;
        continue;
      }
      int x, y;
      if (iss >> x >> y)
      {
        if (x >= 0 && x < currentFile.width && y >= 0 && y < currentFile.height)
        {
          double pixel = quadTree.getPixel(&currentFile, getFitsPixel, x, y);
          std::cout << "Pixel(" << x << ", " << y << ") = " << pixel << std::endl;
        } else
        {
          std::cout << "Coordinates out of range. Image is " << currentFile.width << "x" << currentFile.height
                    << std::endl;
        }
      } else
      {
        std::cout << "Usage: GET_PIXEL <x> <y>" << std::endl;
      }
    } else if (cmd == "FIND_SOURCES" || cmd == "FIND")
    {
      if (!treeBuilt)
      {
        std::cout << "Build Quad-Tree first with BUILD_TREE." << std::endl;
        continue;
      }
      double sigma;
      if (iss >> sigma)
      {
        if (sigma < 0)
        {
          std::cout << "Sigma must be non-negative." << std::endl;
          continue;
        }
        double mean = currentFile.computeMean();
        double stddev = currentFile.computeStdDev();
        double threshold = mean + sigma * stddev;

        std::cout << "=== Source Detection ===" << std::endl;
        std::cout << "  Mean:      " << mean << std::endl;
        std::cout << "  StdDev:    " << stddev << std::endl;
        std::cout << "  Sigma:     " << sigma << std::endl;
        std::cout << "  Threshold: " << threshold << std::endl;
        std::cout << "  Searching..." << std::endl;

        std::vector< Source > sources = quadTree.findSources(&currentFile, getFitsPixel, threshold);

        std::cout << "  Found " << sources.size() << " bright pixels above threshold." << std::endl;

        int show = std::min(30, static_cast< int >(sources.size()));
        if (show > 0)
        {
          std::cout << "  Top " << show << " brightest sources:" << std::endl;
          std::cout << "  " << std::setw(6) << "X" << std::setw(6) << "Y" << std::setw(16) << "Brightness" << std::endl;
          std::cout << "  " << std::string(28, '-') << std::endl;
          for (int i = 0; i < show; i++)
          {
            std::cout << "  " << std::setw(6) << sources[i].x << std::setw(6) << sources[i].y << std::setw(16)
                      << std::fixed << std::setprecision(2) << sources[i].brightness << std::endl;
          }
          if (static_cast< int >(sources.size()) > show)
            std::cout << "  ... and " << (sources.size() - show) << " more." << std::endl;
        }
        std::cout << "========================" << std::endl;
      } else
      {
        std::cout << "Usage: FIND_SOURCES <sigma>" << std::endl;
      }
    } else if (cmd == "CALIBRATE" || cmd == "CAL")
    {
      if (!currentFile.loaded)
      {
        std::cout << "Load main image first with LOAD." << std::endl;
        continue;
      }
      std::string darkFile;
      iss >> darkFile;
      if (!darkFile.empty())
      {
        FitsFile darkFrame;
        if (darkFrame.load(darkFile))
        {
          if (currentFile.calibrate(darkFrame))
          {
            treeBuilt = false;
            std::cout << "Calibration successful. Rebuild Quad-Tree if needed." << std::endl;
          }
        }
      } else
      {
        std::cout << "Usage: CALIBRATE <dark_frame_file>" << std::endl;
      }
    } else if (cmd == "SAVE")
    {
      if (!currentFile.loaded)
      {
        std::cout << "No file loaded to save." << std::endl;
        continue;
      }
      std::string filename;
      iss >> filename;
      if (!filename.empty())
        currentFile.save(filename);
      else
        std::cout << "Usage: SAVE <filename>" << std::endl;
    } else if (cmd == "HELP" || cmd == "?")
    {
      printHelp();
    } else if (cmd == "EXIT" || cmd == "QUIT" || cmd == "Q")
    {
      std::cout << "Goodbye!" << std::endl;
      break;
    } else
    {
      std::cout << "Unknown command: " << cmd << std::endl;
    }
  }

  return 0;
}
