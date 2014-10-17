Image processing
================

Installation and project setup
-------------------
To get this project running you need [OpenCV 2.4.9](http://opencv.org/). We developed on Windows 7 by using Visual Studio 2013 as IDE and the Microsoft Visual C++ Compiler 18.00.21005.1 for x86 platform. The installation instructions are for Visual Studio 2013 and Windows. 

1. Download [OpenCV](http://opencv.org/)
2. Install OpenCV on your computer. The installation is quite easy because in case you are also using Windows you can use the pre-build libraries.
  * [install instructions windows](http://docs.opencv.org/doc/tutorials/introduction/windows_install/windows_install.html#windows-installation)
  * [instruction for setting up the environment variables](http://docs.opencv.org/doc/tutorials/introduction/windows_install/windows_install.html#windowssetpathandenviromentvariable)
  * [general install instructions](http://docs.opencv.org/doc/tutorials/introduction/table_of_content_introduction/table_of_content_introduction.html)
3. Clone this repository
4. Create a Visual Studio 2013 project with the sources of this repository.
5. Follow the following instructions for setting up the project properties. At the point, where you have to enter the linker input, add the following libraries:
opencv_calib3d249d.lib
opencv_contrib249d.lib
opencv_core249d.lib
opencv_features2d249d.lib
opencv_flann249d.lib
opencv_gpu249d.lib
opencv_highgui249d.lib
opencv_imgproc249d.lib
opencv_legacy249d.lib
opencv_ml249d.lib
opencv_nonfree249d.lib
opencv_objdetect249d.lib
opencv_photo249d.lib
opencv_stitching249d.lib
opencv_superres249d.lib
opencv_ts249d.lib
opencv_video249d.lib
opencv_videostab249d.lib. Here the instructions: [Installing & Configuring OpenCV with Visual Studio](http://opencv-srf.blogspot.co.at/2013/05/installing-configuring-opencv-with-vs.html)
8. Now you can build the project and start it
