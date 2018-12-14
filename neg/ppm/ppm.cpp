#include <iostream>
#include <fstream>
#include <string>
#include "ppm.h"

namespace imaging {

	float * ReadPPM(const char * filename, int * w, int * h) {
		
		std::ifstream iFile(filename, std::ios::in | std::ios::binary); //opening file for reading

		//checking if the file's opened correctly
		if (!iFile.is_open()) {
			std::cerr << "Error: Opening of File Failed\n\n";
			return nullptr;
		}

		//reading the values of the file's header
		std::string formatOfImage;
		unsigned int width, height, max;

		iFile >> formatOfImage >> width >> height >> max;

		//checking if format, width and height are correct
		if (formatOfImage != "P6") {
			std::cerr << "Error: Format Is Not P6\n\n";
			iFile.close();
			return nullptr;
		}

		if (width == NULL || height == NULL) {
			std::cerr << "Error: Width Or Height Missing\n\n";
			iFile.close();
			return nullptr;
		}

		//since width and height exist we pass them into w and h pointers
		*w = width;
		*h = height;

		if (max > 255 || max != 255) {
			std::cerr << "Error: Maximum Range Of Colors Is Greater Than Or Not Equal To 255\n\n";
			iFile.close();
			return nullptr;
		}

		//reading the rest of the file
		unsigned int sizeOfBuffer = height * width * 3; //finding out how much space our buffer will take up
		unsigned char * buffer = new unsigned char[sizeOfBuffer]; //array that we'll use later to convert our data to floats
		iFile.get(); //skips blanks after header
		iFile.read((char *)buffer, sizeOfBuffer); //initializing array buffer with data from our file

		float *f_buffer = new float[sizeOfBuffer]; //array containing float values

		/* converting integers values of array "buffer" ranging between 0 and  255
		   to normalized range of float numbers ranging between 0 and 1
		*/
		for (unsigned int i = 0; i < sizeOfBuffer; i++) {
			f_buffer[i] = buffer[i] / (float)max;
		}

		iFile.close(); //closing ifstream "iFile"

		delete[] buffer; //deleting array "buffer" since it takes up useless space

		return f_buffer; //returning pointer to first element of f_Buffer array
	}

	bool WritePPM(const float * data, int w, int h, const char * filename) {

		std::ofstream oFile(filename, std::ios::out | std::ios::binary); //opening file for writing
		
		//checking if the file's opened correctly
		if (!oFile.is_open()) {
			std::cerr << "Error: Opening File Failed\n\n";
			return false;
		}

		//writing the values of the file's header
		std::string formatOfImage = "P6";
		oFile << formatOfImage << "\n" << w << "\n" << h << "\n" << 255 << "\n";

		//writing the body of the file
		unsigned int sizeOfBuffer = h * w * 3; //finding out how much space our buffer will take up

		unsigned char * buffer = new unsigned char[sizeOfBuffer]; //array that we'll use to write data

		/* converting normalized range of float numbers ranging between 0 and 1 to
			integers values of array "buffer" ranging between 0 and  255 
		*/
		for (unsigned int i = 0; i < sizeOfBuffer; i++) {
			buffer[i] = (unsigned char) (data[i] * 255.0F);
		}

		oFile.write((char *)buffer, sizeOfBuffer); //writes the buffer inside the image,after the header
		
		oFile.flush();
		oFile.close(); //closing ofstream "oFile"

		delete[] buffer; //deleting array "buffer" since it takes up useless space

		return true;
	}
}