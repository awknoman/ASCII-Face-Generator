#include <Magick++.h>
#include <iostream>
#include <cmath>
#define ui unsigned int

using namespace std;
using namespace Magick;

//Functions List
ui brightnessCriteria(ui, ui, ui);

//Program.
int main(int argc,char **argv) 
{
    //Change value of this variable {true, false} to enable/disable Image Inversion respectively.
    bool invertImage = false;

    //Required.
    InitializeMagick(*argv);

    string brightToAscii = "`^\",:;Il!i~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$"; 

    // Construct the image object. Seperating image construction from the 
    // the read operation ensures that a failure to read the image file 
    // doesn't render the image object useless. 
    Image testImage;
    
    try {
        //This command takes a capture from the webcam.
        system("fswebcam -r 640x480 --jpeg 85 -D 1 --no-banner --save testImage.jpg");

        // Read a file into image object.
        testImage.read("testImage.jpg");
        cout << "Image Loaded\n";

        //Resizing image so as to see the complete image on the terminal.
        testImage.resize(Geometry(150, 100));

        ui width = testImage.columns(); // returns an unsigned int representing the my_image width
        ui height = testImage.rows(); // returns an unsigned int representing the my_image heigth

        cout << "Brightness Matrix Size: " << width << ' ' << height << '\n';

        ui brightnessLvl;

        //Below statement required if magick API works on the previous cache of image.
        testImage.modifyImage();

        Pixels imageCache(testImage);

        for(ui x = 0; x < height; x++)
        {
            for(ui y = 0; y < width; y++)
            {
                Color c = testImage.pixelColor(y, x);
                ui r = c.redQuantum() / 256;
                ui g = c.greenQuantum() / 256;
                ui b = c.blueQuantum() / 256;

                brightnessLvl = brightnessCriteria(r, g, b);
                brightnessLvl = brightnessLvl / ( 256 / brightToAscii.size()  + 0.5);

                if(invertImage)
                    brightnessLvl = brightToAscii.size() - 1 - brightnessLvl;
                
                if(brightnessLvl >= 0 && brightnessLvl < brightToAscii.size())
                {
                    cout << brightToAscii[brightnessLvl];
                    cout << brightToAscii[brightnessLvl];
                }
                
            }
            cout << '\n';
        }
    } 
    catch( Exception &error_ ) 
    { 
        cout << "Caught exception: " << error_.what() << endl; 
        return 1; 
    } 
  return 0; 
}

ui brightnessCriteria(ui red, ui green, ui blue)
{
    //Change the mode value between {1, 2, 3} to set different parameters to calculate brightness.
    int mode = 2;

    ui brightLvl;
    switch(mode)
    {
        case 1: brightLvl = (red + green + blue) / 3;
                break;
        case 2: brightLvl = ( max(red, max(green, blue)) + min(red, min(green, blue)) ) / 2;
                break;
        case 3: brightLvl = ( 21*red + 72*green + 7*blue) / 100;
                break;
    }
    return brightLvl;
}