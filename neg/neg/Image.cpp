#include "Image.h"
#include "../ppm/ppm.h"
#include <ostream>
#include <string>

namespace imaging {

	bool isPPM(std::string& filename);
	bool areEqual(const std::string& a, const std::string& b);

	//returns pointer to internal data
	Color * Image::getRawDataPtr() {
		return buffer;
	}

	//retrieves the color of the image at location (x, y)
	Color Image::getPixel(unsigned int x, unsigned int y) const {

		Color colorObj;

		//if x,y pair is out-of-bounds return a black (0,0,0) color
		if ((x > height - 1) || (y > width - 1)) {
			
			colorObj.r = 0;
			colorObj.g = 0;
			colorObj.b = 0;
			return colorObj;
		}

		unsigned int pixel = (x * width + y); //location of pixel inside array "buffer"

		colorObj.r = buffer[pixel].r;
		colorObj.g = buffer[pixel].g;
		colorObj.b = buffer[pixel].b;

		return colorObj;
	}
	
	//sets the color of the image at location (x, y)
	void Image::setPixel(unsigned int x, unsigned int y, Color & value) {
		
		if ((x > height - 1) || (y > width - 1)) return;
		
		unsigned int pixel = (x * width + y);//location of pixel inside array "buffer"

		buffer[pixel].r = value.r;
		buffer[pixel].g = value.g;
		buffer[pixel].b = value.b;
	}
	
	//Copies the image data from an external raw buffer to the internal image buffer
	void Image::setData(const Color * & data_ptr) {

		buffer = new Color[width * height];

		//check if x,y pair is out-of-bounds
		if ((height == 0) || (width == 0) || (data_ptr == nullptr)) return;
			
		for (unsigned int pixel = 0; pixel < width * height; pixel++) {
			buffer[pixel].r = data_ptr[pixel].r;
			buffer[pixel].g = data_ptr[pixel].g;
			buffer[pixel].b = data_ptr[pixel].b;
		}
	}
	
	//Default constructor
	Image::Image() {

		buffer = nullptr;
		width = 0;
		height = 0;
	}

	Image::Image(unsigned int width, unsigned int height) {
		
		buffer = new Color[width * height];
		this->width = width;
		this->height = height;
	}
	
	Image::Image(unsigned int width, unsigned int height, const Color * data_ptr){
		
		setData(data_ptr);
		this->width = width;
		this->height = height;		
	}
	
	//Copy constructor
	Image::Image(const Image &src) {
		
		const Color * data_ptr = src.buffer;
		setData(data_ptr);
		this->width = src.width;
		this->height = src.height;
	}

	//destructor
	Image::~Image() {
		delete[] buffer;
	}

	Image & Image::operator = (const Image & right){
		
		const Color * data_ptr = right.buffer;
		setData(data_ptr);
		this->width = right.width;
		this->height = right.height;		
		return *this; //returns the object created inside here
	}

	//Loads the image data from the specified file, if the extension of the filename matches the format string
	bool Image::load(const std::string & filename, const std::string & format) {

		std::string filenameNotConst = filename;
		if (!isPPM(filenameNotConst)) return false;

		if (format != "ppm") return false;

		int w, h;
		float *f_buffer;

		f_buffer = ReadPPM(filename.c_str(), &w, &h);//read image data and pass into f_buffer

		if (f_buffer == nullptr) {
			return false;
		}

		width = w;
		height = h;

		buffer = new Color[width * height];

		for (unsigned int i = 0; i < width * height; i ++) {
		
			buffer[i].r = f_buffer[i * 3];
			buffer[i].g = f_buffer[i * 3 + 1];
			buffer[i].b = f_buffer[i * 3 + 2];
		}

		delete[] f_buffer;//deleting array "f_buffer" since it takes up useless space

		return true;
	}

	//Stores the image data to the specified file, if the extension of the filename matches the format string.
	bool Image::save(const std::string & filename, const std::string & format) {
		
		std::string filenameNotConst = filename;
		if (!isPPM(filenameNotConst)) return false;
		
		if (format != "ppm") return false;

		float *f_buffer = new float[width * height * 3];

		int w = width;
		int h = height;

		for (int i = 0; i < w * h; i++) {
			
			f_buffer[i*3] = buffer[i].r;
			f_buffer[i*3+1] = buffer[i].g;
			f_buffer[i*3+2] = buffer[i].b;
		}

		return WritePPM(f_buffer, width, height, filename.c_str());
	}

	//Checks if format="ppm"
	bool isPPM(std::string& filename) {
		return (areEqual(filename.substr(filename.find_last_of(".") + 1), "ppm") ? true : false);
	}

	//checks if 2 strings are equal
	bool areEqual(const std::string& a, const std::string& b) {

		if (b.size() != a.size()) return false;

		for (unsigned int i = 0; i < a.size(); ++i) {
			if (tolower(a[i]) != tolower(b[i])) {
				return false;
			}
		}

		return true;
	}
}