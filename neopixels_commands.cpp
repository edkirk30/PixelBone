#include "./pixel.hpp"

#include <string>

#define PIXELS_PER_LINE 7
#define PIXELS_ROWS 4

/*
g++  -std=c++0x -c -fPIC -o neopixels_commands.o neopixels_commands.cpp
g++  -std=c++0x -c -fPIC -o pixel.o pixel.cpp
g++  -std=c++0x -c -fPIC -o gfx.o gfx.cpp
g++  -std=c++0x -c -fPIC -o matrix.o matrix.cpp

g++ -shared -Wl,-soname,libneopixels_commands.so -o libneopixels_commands.so neopixels_commands.o pixel.o gfx.o matrix.o pru.o util.o am335x/app_loader/lib/libprussdrv.so -lpthread  ./am335x/app_loader/lib/libprussdrv.so
*/


//row state

class LedSignalInterface {

    private:
    
        PixelBone_Pixel* strip;

    public:
        
        LedSignalInterface() {

            printf("LedSignalInterface");

            strip = new PixelBone_Pixel(PIXELS_PER_LINE*
                                                    PIXELS_ROWS);

        }

        void setRow(int row, char* state) {

            std::string stateString(state);

            setRow(row, stateString);

        }

        void setRow(int row, std::string state) {

            printf("setRow");
        
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

            strip->wait();
            strip->show();

        }

        ~LedSignalInterface() {

            delete strip;

        }


};


extern "C" {

    LedSignalInterface* LedSignalInterface_new() {
        return new LedSignalInterface();
    }

    void LedSignalInterface_setRow(LedSignalInterface* ledSignalInterface,
                                    int row,
                                    char* state) {
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

    LedSignalInterface ledSignalInterface;

    ledSignalInterface.setRow(row, state);

}



