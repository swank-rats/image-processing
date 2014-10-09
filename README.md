image-processing
================

Installation and project setup
-------------------
To get this project running you need [OpenCV 2.4.9](http://opencv.org/). We used [Eclipse](http://www.eclipse.org) as IDE and [MinGW](http://www.mingw.org/). The installation instructions are for [Eclipse](http://www.eclipse.org). You may also need to install [CMake](http://www.cmake.org/).

1. Download [OpenCV](http://opencv.org/)
2. Install it on your computer: [installation instructions](http://docs.opencv.org/doc/tutorials/introduction/table_of_content_introduction/table_of_content_introduction.html) 3. You have to build OpenCV with [MinGW](http://www.mingw.org/). Just follow the following instructions.
  * [Install OpenCV on Windows](http://seevisionc.blogspot.co.uk/2011/09/linux-like-installation-of-opencv-230.html)
  * [Build OpenCV under MinGW](http://sourceforge.net/p/opencvmingw/wiki/Build%20OpenCV%20under%20MinGW/)
4. Clone this repository
5. Create an Eclipse project with the sources of this repository.
6. Go to the Properties page of your project and navigate to C/C++ Build -> Settings
 1. Go to GCC C++ Compiler -> Includes. Here you need to add "PATH_TO_OPENCV\build\include\"
 ![Compiler includes](https://raw.githubusercontent.com/swank-rats/image-processing/master/doc/images/compiler_includes.png)
 2. Go to MinGW C++ Linker -> Libraries. Here you add opencv_imgproc249, opencv_highgui249^, opencv_core249 (one by one!) to Libraries (-I) and "PATH_TO_OPENCV\build\x86\mingw\lib" to Library search path (-L).
![Linker includes](https://raw.githubusercontent.com/swank-rats/image-processing/master/doc/images/linker_includes.png)
 *Maybe you get some help for troubleshooting here:
  *[Using OpenCV with Eclipse (plugin CDT)](http://docs.opencv.org/doc/tutorials/introduction/linux_eclipse/linux_eclipse.html#linux-eclipse-usage)
  * [stackoverflow: see the second and third post](http://stackoverflow.com/questions/10860352/getting-started-with-opencv-2-4-and-mingw-on-windows-7)
