Image processing
================

Installation and project setup
-------------------
To get this project running you need [OpenCV 2.4.9](http://opencv.org/) and [Boost 1.56.0](http://www.boost.org/). We developed on Windows 7 by using Visual Studio 2013 as IDE and the Microsoft Visual C++ Compiler 18.00.21005.1 for x86 platform. The installation instructions are for Visual Studio 2013 and Windows. 

1. Install Boost
  1. Download [Boost 1.56.0](http://www.boost.org/)
  2. Unpack the archive file
  3. Navigate into the unpacked folder
  4. Open command window and execute the following command
    1. This command will build the Boost.Build tool
      * bootstrap.bat
    2. This one will build Boost and copy the file to "C:\Boost". The second command will take some time.
      *b2 --toolset=msvc-12.0 --build-type=complete --abbreviate-paths architecture=x86 address-model=32 install -j4
  5. Make sure that the folder "C:\Boost" contains a folder "include" and "lib".
  6. You can move the "C:\Boost" folder if you like. Just remember the location.
  7. Add a new system environment variable. To do so open Control Panel -> System -> Advanced system settings -> Environment variables.
  8. At system variables press the "new" button and add a variable with name "BOOST" and path to "C:\Boost\" (or to your new location) (with "\" at the end!)
  9. Boost installation is finished.
2. Install OpenCV
  1. Download [OpenCV 2.4.9](http://opencv.org/)
  2. Unpack the archive file
  3. Copy all the files to the location where you want it to have on your compute
  4. OpenCV is already delivered with prebuild VS120 libs. So we have nothing to build.
  5. Again add a new system environment variable. To do so open Control Panel -> System -> Advanced system settings -> Environment variables.
  6. At system variables press the "new" button and add a variable with name "OPENCV" and path to e.g. "C:\opencv\build\" (with "\" at the end!). This is the path to the OpenCV installation including the folder "build". The folder "build" must contain the folder "include" and "x86\vc12\lib". 
  7. Modify the PATH variable. Add "%OPENCV%\x86\vc12\bin;" (without ") at the end of the value of your PATH variable.
  8. OpenCV installation is finished.,
3. Clone this repository
4. Open the solution with VS 2013
5. Build the project
6. Finish!
  
Troubleshooting
-------------------
  * [OpenCV: install instructions windows](http://docs.opencv.org/doc/tutorials/introduction/windows_install/windows_install.html#windows-installation)
  * [OpenCV: instruction for setting up the environment variables](http://docs.opencv.org/doc/tutorials/introduction/windows_install/windows_install.html#windowssetpathandenviromentvariable)
  * [OpenCV: general install instructions](http://docs.opencv.org/doc/tutorials/introduction/table_of_content_introduction/table_of_content_introduction.html)
  * [OpenCV: Installing & Configuring OpenCV with Visual Studio](http://opencv-srf.blogspot.co.at/2013/05/installing-configuring-opencv-with-vs.html)
  * [Boost: How to build Boost for Visual Studio 2013](http://choorucode.com/2014/06/06/how-to-build-boost-for-visual-studio-2013/)
  * [Boost: How to use Boost in Visual Studio 2010 - see 2nd and 3rd post](http://stackoverflow.com/questions/2629421/how-to-use-boost-in-visual-studio-2010)


Project documentation
-------------------
The whole project documentation can be found [here](http://swank-rats.github.io/docs): http://swank-rats.github.io/docs/
