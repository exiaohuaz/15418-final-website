
// C++ Program to implement
// Read Screen Pixels 
#include <windows.h>
#include <iostream>
  
using namespace std;
  
int main()
{
      // Assigning the device context to the current output device
      HDC dng = GetDC(NULL);
    
      // Calling the getpixel function and passing in the device context and coordinates
      // The coordinates should be inside the max resolution of the device context 
      // ex. screen coordinate X < 1919; in case of a full HD display
      COLORREF c = GetPixel(dng, 10, 150);
    
      // Beautifying the data to output as a tuple containing the RGB values
      // The GetRValue function returns the Red color value of the pixel
      // The GetGValue function returns the Green color value of the pixel
      // The GetBValue function returns the Blue color value of the pixel
      cout<<"(";
      cout<<(int)GetRValue(c)<<", ";
      cout<<(int)GetGValue(c)<<", ";
      cout<<(int)GetBValue(c)<<")";
    
      // Releasing the Handle
      ReleaseDC(NULL, dng);
}