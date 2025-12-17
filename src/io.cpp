#include <iostream>
#include <fstream>
#include "io.h"
#include <cstdint>

namespace hardware
{
    namespace pipeline
    {
        using namespace std;

        // FrameReader implementation (same as before)
        pixel *FrameReader::loadImage(const char *filename, int &width, int &height)
        {
            std::cout << "[DEBUG] Loading image: " << filename << "\n";

            std::ifstream file(filename);
            if (!file.is_open())
            {
                std::cerr << "[ERROR] Could not open file " << filename << std::endl;
                return nullptr;
            }

            string magic;
            file >> magic;
            if (magic != "P3")
            {
                cerr << "Error: Unsupported format (" << magic << "). Expected P3." << endl;
                return nullptr;
            }

            int maxVal;
            file >> width >> height >> maxVal;

            // Allocate memory for the image buffer
            pixel *buffer = new pixel[width * height];

            for (int i = 0; i < width * height; i++)
            {
                int r, g, b;
                file >> r >> g >> b;
                buffer[i].r = static_cast<uint8_t>(r);
                buffer[i].g = static_cast<uint8_t>(g);
                buffer[i].b = static_cast<uint8_t>(b);
            }

            file.close();
            std::cout << "[DEBUG] Image loaded successfully\n";
            return buffer;
        }

        // FrameWriter implementation - NOW RETURNS BOOL
        bool FrameWriter::saveImage(const char *filename, pixel *buffer, int width, int height)
        {
            ofstream file(filename);
            if (!file.is_open())
            {
                cerr << "Error: could not write to file " << filename << endl;
                return false; // Return false on failure
            }

            file << "P3\n"
                 << width << " " << height << "\n255\n";

            for (int i = 0; i < width * height; i++)
            {
                file << (int)buffer[i].r << " "
                     << (int)buffer[i].g << " "
                     << (int)buffer[i].b << "\n";
            }

            file.close();

            // Check if write was successful
            if (file.fail())
            {
                cerr << "Error: Failed to write to file " << filename << endl;
                return false;
            }

            return true; // Return true on success
        }

        // Optional overload for std::string
        bool FrameWriter::saveImage(const std::string &filename, pixel *buffer, int width, int height)
        {
            return saveImage(filename.c_str(), buffer, width, height);
        }
    }
}