How to install x86-64 variant of homebrew:
(This link should be all the info you need)
https://gist.github.com/progrium/b286cd8c82ce0825b2eb3b0b3a0720a0


How to get packages once you have x86-64 variant of homebrew installed: 
https://stackoverflow.com/questions/72970363/how-do-you-install-the-x86-64-version-of-a-brew-package-on-a-m1-mac


After getting homebrew, download the module libpng. This would be done via `arch -x86_64 /usr/local/bin/brew install libpng` from the command line

Then you will need to install png++ which uses libpng (which was originally made for standard C to deal with image processing files) but png++ makes use of those nice quick syntaxes to simplify our convolution.

This may work installing for png++:
`brew install png++`

If everything is done correctly, you should be able to compile the `testRay.cpp` file in the repo via `g++ -lpng16 testRay.cpp`
