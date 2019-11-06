//=============================================================================
//                                                                            
//   Exercise code for the lecture
//   "Introduction to Computer Graphics"
//   by Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) Computer Graphics Group, Bielefeld University.
//                                                                            
//=============================================================================

#ifndef IMAGE_H
#define IMAGE_H


//== INCLUDES =================================================================

#include "vec3.h"
#include <vector>
#include <assert.h>
#include <fstream>
#include <lodepng.h>


//== CLASS DEFINITION =========================================================


/// \class Image Image.h
/// This class stores an image as a big array of colors, which are represented
/// as vec3 values for RGB.
class Image
{
public:

	/// Construct an image of size _width times _height
	/// \param _width Width of the image in pixels
	/// \param _height Height of the image in pixels
    Image(unsigned int _width=0, unsigned int _height=0)
    {
        resize(_width, _height);
    }
    
	/// Resize the image
	/// \param _width New width of the image in pixels
	/// \param _height New height of the image in pixels
    void resize(unsigned int _width, unsigned int _height)
    {
        width_  = _width;
        height_ = _height;
        pixels_.resize(width_ * height_);
    }

    /// Returns image width in pixels.
    unsigned int width() const
    {
        return width_;
    }

    /// Returns image height in pixels.
    unsigned int height() const
    {
        return height_;
    }

    /// Read/write access to pixel (_x,_y). Use this to set the color by
    /// image(x,y) = color;
    vec3& operator()(unsigned int _x, unsigned int _y)
    {
        assert(_x < width_);
        assert(_y < height_);
        unsigned int el =_y*static_cast<unsigned int>(width_) + _x;
        return pixels_[el];
    }
    
    /// Read access to pixel (_x,_y).
    const vec3& operator()(unsigned int _x, unsigned int _y) const
    {
        assert(_x < width_);
        assert(_y < height_);
        return pixels_[_y*static_cast<unsigned int>(width_) + _x];
    }

    /// Writes the image in PNG format to a file.
    /// \param[in] _filename Filename to save the image to.
    bool write_png(const char* _filename)
    {
            // convert 4d float vectors to RGB (unsigned char) (bottom to top)
            std::vector<unsigned char> rgb(3 * width_ * height_);
            for (unsigned int y = 0; y < height_; ++y)
            {
                for (unsigned int x = 0; x < width_; ++x)
                {
                    for (unsigned int c = 0; c < 3; ++c)
                    {
                            rgb[3 * ((height_ - y - 1)* width_ + x) + c] = 255.0*pixels_[y*width_ + x][c];
                    }
                }
            }

            // write RGBA to file
            unsigned error = lodepng::encode(_filename, rgb, width_, height_, LCT_RGB);
            if (error)
            {
                std::cout << _filename <<" write error: " << lodepng_error_text(error) << std::endl;
                return false;
            }
            return true;
    }

    /// Reads an image out of a file in PNG format.
    /// \param[in] _filename Filename to read the image off.
    bool read(const char* _filename)
    {
        // read block of RGBA values

        std::vector<unsigned char> rgb;
        unsigned int w = 0,h = 0;
        unsigned error = lodepng::decode(rgb, w, h, _filename, LCT_RGB);
        if (error)
        {
            std::cout << _filename << " read error: " << lodepng_error_text(error) << std::endl;
            width_ = height_ = 0;
            return false;
        }

        // allocate memory
        resize(w, h);
        vec3 color;
        // convert RGB (unsigned char) to float colors (top to bottom)
        for (unsigned int y = 0; y < height_; ++y)
        {
            for (unsigned int x = 0; x < width_; ++x)
            {
                for (unsigned int c = 0; c < 3; ++c)
                {
                    color[c] = rgb[3 * ((height_- y - 1) * width_ + x) + c] / 255.0;

                }
                (*this)(x,y) = color;
            }
        }
        return true;
    }

private:

	/// vector with all pixels in the image
    std::vector<vec3> pixels_;
	
    /// image width in pixels
    unsigned short int width_;
	
    /// image height in pixels
	unsigned short int height_;
};


//=============================================================================
#endif // IMAGE_H defined
//=============================================================================
