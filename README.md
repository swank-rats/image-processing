Image processing
================

Installation and project setup
-------------------
To get this project running you need [OpenCV 2.4.9](http://opencv.org/) and [Poco C++ Libraries 1.5.4](http://pocoproject.org/). We developed on Windows 7 by using Visual Studio 2013 as IDE and the Microsoft Visual C++ Compiler 18.00.21005.1 for x86 platform. The installation instructions are for Visual Studio 2013 and Windows. 

It is necessary to add new system environment variables. So do not close the window, if you have opened it during the installation process.

1. Install OpenSSL
  1. Download [OpenSSL Installer for Windows](https://slproweb.com/products/Win32OpenSSL.html)
  2. Run installer
  3. Add a new system environment variable. To do so open Control Panel -> System -> Advanced system settings -> Environment variables.
  4. At system variables press the "new" button and add a variable with name "OPENSSL" and path to e.g. "C:\OpenSSL\" (or to your new location) (with "\" at the end!)
  5. OpenSSL installation is finished
2. Install Poco C++ Libraries
  1. Download [Poco C++ Libraries 1.5.4 (development version) all](http://pocoproject.org/download/index.html) - [direct link - download poco-1.5.4-all.zip](http://pocoproject.org/releases/poco-1.5.4/)
  2. Unpack the archive file to e.g. C:\Poco
  3. Navigate to the folder
  4. Open the file "components" and remove "CppUnit", "Data", "Data/SQLite", "Data/ODBC", "Data/MySQL", "Zip"
  5. You have to edit the path to your OpenSSL installation in the file "buildwin.cmd", if it is not "C:\OpenSSL"
  6. Double click build_vs120.cmd -> this command will build the needed files
  7. Make sure that the folder e.g."C:\Poco" contains a folder "bin" and "lib". 
  8. Again add a new system environment variable. 
  9. At system variables press the "new" button and add a variable with name "POCO" and path to "C:\Poco\" (or to your new location) (with "\" at the end!)
  10. Edit the variable PATH
  11. Add "C:\Poco\bin" at the end (between the last and the new entry must be a ";"!)
  12. Poco installation is finished.
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
6. Click right on the solution and go to -> properties -> debugging -> additional command line parameters
7. add /uri=ws://127.0.0.1:3001/ where the IP and port should be the address of the NodeJS server
6. Finish - now you can run the application!

Troubleshooting
-------------------
  * [OpenCV: install instructions windows](http://docs.opencv.org/doc/tutorials/introduction/windows_install/windows_install.html#windows-installation)
  * [OpenCV: instruction for setting up the environment variables](http://docs.opencv.org/doc/tutorials/introduction/windows_install/windows_install.html#windowssetpathandenviromentvariable)
  * [OpenCV: general install instructions](http://docs.opencv.org/doc/tutorials/introduction/table_of_content_introduction/table_of_content_introduction.html)
  * [OpenCV: Installing & Configuring OpenCV with Visual Studio](http://opencv-srf.blogspot.co.at/2013/05/installing-configuring-opencv-with-vs.html)
  * [Poco: Building On Windows](http://pocoproject.org/docs/00200-GettingStarted.html#7)

Project documentation
-------------------
The whole project documentation can be found [here](http://swank-rats.github.io/docs): http://swank-rats.github.io/docs/

Build & Start application
-------------------
1. Open the solution with VS 2013
2. Right click on the solution image-processing -> Properties
3. Now navigate to Configuration properties -> Debugging -> Command line arguments
4. Enter /uri=wss://IP:3001/ (replace the IP with the IP of your game server)
5. Build it
6. Run it

When you run the .exe manually do not forget to pass the /uri parameter. 

```
//to get help:
image-processing.exe /help

//to start the image processing server (important use wss and add a "/" at the end!)
image-processing.exe /uri=wss://127.0.0.1:3001/
```
