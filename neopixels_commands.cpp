#include "./pixel.hpp"

#include <string>
#include <vector>

/*
g++  -std=c++0x -c -fPIC -o neopixels_commands.o neopixels_commands.cpp
g++  -std=c++0x -c -fPIC -o pixel.o pixel.cpp
g++  -std=c++0x -c -fPIC -o gfx.o gfx.cpp
g++  -std=c++0x -c -fPIC -o matrix.o matrix.cpp

cc -std=c99 -W -Wall -D_BSD_SOURCE -fPIC -Wp,-MMD,./.pru.o.d -Wp,-MT,pru.o -I. -O2 -mtune=cortex-a8 -march=armv7-a  -I./am335x/app_loader/include -c -o pru.o pru.c
cc -std=c99 -W -Wall -D_BSD_SOURCE -fPIC -Wp,-MMD,./.util.o.d -Wp,-MT,util.o -I. -O2 -mtune=cortex-a8 -march=armv7-a  -I./am335x/app_loader/include -c -o util.o util.c

g++ -shared -Wl,-soname,libneopixels_commands.so -o libneopixels_commands.so neopixels_commands.o pixel.o gfx.o matrix.o pru.o util.o am335x/app_loader/lib/libprussdrv.so -lpthread  ./am335x/app_loader/lib/libprussdrv.so
*/


//row state

class LedSignalInterface {

    private:
    
        PixelBone_Pixel* strip;
	std::vector<std::vector<unsigned>> rowIndexes;
	unsigned pixelCount;

    public:
        
        LedSignalInterface(std::vector<std::vector<unsigned>> rowIndexes, unsigned pixelCount) : rowIndexes(rowIndexes), pixelCount(pixelCount) {

            printf("LedSignalInterface \n");

            printf("LedSignalInterface pixelCount:%d\n", pixelCount);
            strip = new PixelBone_Pixel(pixelCount);
        }

        void setRow(int row, char* state) {

            std::string stateString(state);

            setRow(row, stateString);

        }

        void setRow(int row, std::string state) {

            printf("setRow");
 
	    if (!(rowIndexes.size() > row)) {
		printf("error row out of range");
	    }

	    for (unsigned i=0; i<rowIndexes[row].size(); i++) {

                if (state == "ON_RED") {

                    //printf("ON_RED \n");

                    strip->setPixelColor(rowIndexes[row][i], 255, 0, 0);
                }
                else if (state == "ON_GREEN") {

                    //printf("ON_GREEN \n");

                    strip->setPixelColor(rowIndexes[row][i], 0, 255, 0);

                }
                else if (state == "ON_BLUE") {

                    //printf("ON_BLUE \n");

                    strip->setPixelColor(rowIndexes[row][i], 0, 0, 255);

                }
                else if (state == "ON_WHITE") {

                    //printf("ON_WHITE \n");

                    strip->setPixelColor(rowIndexes[row][i], 255, 255, 255);

                }
                else {

                    strip->setPixelColor(rowIndexes[row][i], 0, 0, 0);

                }

	    }
      
/*
 
            unsigned startPixel = row * PIXELS_PER_LINE;
            unsigned lastPixel = startPixel + PIXELS_PER_LINE;

            for (unsigned i=startPixel; i<lastPixel; i++) { 

                //printf("i: %d \n", i);

                if (state == "ON_RED") {

                    //printf("ON_RED \n");

                    strip->setPixelColor(i, 255, 0, 0);
                }
                else if (state == "ON_GREEN") {

                    //printf("ON_GREEN \n");

                    strip->setPixelColor(i, 0, 255, 0);

                }
                else if (state == "ON_BLUE") {

                    //printf("ON_BLUE \n");

                    strip->setPixelColor(i, 0, 0, 255);

                }
                else if (state == "ON_WHITE") {

                    //printf("ON_WHITE \n");

                    strip->setPixelColor(i, 255, 255, 255);

                }
                else {

                    strip->setPixelColor(i, 0, 0, 0);

                }

            }
*/
            strip->wait();
            strip->show();

        }

        ~LedSignalInterface() {

            delete strip;

        }


};


extern "C" {

    LedSignalInterface* LedSignalInterface_new(int rowIndexesC[][100], unsigned rowsSize, unsigned pixelCount) {

	std::vector<std::vector<unsigned>> rowIndexes;

	printf("length %d \n", rowsSize);
	for (unsigned i=0; i<rowsSize; i++) {

	    std::vector<unsigned> row;

	    for (unsigned j=0; j<100; j++) {

		printf("index %d \n", rowIndexesC[i][j]);

		if (rowIndexesC[i][j] < 0) {
		    break;
		}
		row.push_back(rowIndexesC[i][j]);

	    }
		rowIndexes.push_back(row);
	}


        return new LedSignalInterface(rowIndexes, pixelCount);
    }

    void LedSignalInterface_setRow(LedSignalInterface* ledSignalInterface,
                                    int row,
                                    char* state) {

	printf("LedSignalInterface_setRow \n");
        ledSignalInterface->setRow(row, state);
    }
}


int main(int argv, char **args) {

    int row = 0;
    std::string state = "OFF";

    switch (argv) {

        case 3:

            row = std::stoi(args[1]);
            state = args[2];

    }

    std::vector<std::vector<unsigned>> rowIndexes;
    rowIndexes.push_back({0, 1});
    rowIndexes.push_back({2, 3});

    LedSignalInterface ledSignalInterface(rowIndexes, 4);

    ledSignalInterface.setRow(row, state);

}



