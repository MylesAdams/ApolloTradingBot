# INSTRUCTIONS TO BUILD PROJECT

# UNIX Systems

1. Install [CMake](https://cmake.org/download/)
    * macOS
        * `brew install cmake`
    * Ubuntu
        * `sudo apt install cmake`
2. Install curl
    * macOS
        * `brew install curl`
    * Ubuntu
        * `sudo apt install curl`
        
3. Install [cpprestsdk](https://github.com/Microsoft/cpprestsdk)
   * `brew install cpprestsdk` on mac
   * `sudo apt-get install libcpprest-dev` on ubuntu/debian
   
4. Run commands below within ApolloTradingBot directory
    * `cd Build`
    * `cmake ../`
    * `make`
    
# Windows special instructions:
**This guide can only guarantee that these instructions work for Visual Studio 2017 Community Edition.**

1. Install [CMake](https://cmake.org/download/)
   
2. Install libcurl
      1. [Download the curl source zip](https://curl.haxx.se/download.html)
      2. Extract curl to any folder you want
      3. Go to start -> search -> "system environment variables"
      4. Click on `environment variables`
      5. Create a new System variable (note: NOT a user variable!)
         * variable should be named `CURL`
         * set `CURL` equal to the path of your curl directory
      6. Under System variables find the `PATH` variable and click EDIT...
      7. pre-pend `%CURL%;` to the value of the PATH variable. (the semi-colon is absolutely essential)
      8. Add `%CURL%\lib\;` to the `PATH` variable as well.
      9. Go to your curl directory and read the installation instructions which are found in /winbuild/BUILD.WINDOWS.TXT
         * tl;dr of how to build is to just go to start -> search -> "dev" -> click on "Developer Command Prompt for VS20xx" -> cd into /curl/winbuild and run `nmake /F mode=dll`
      10. Go to the newly created /curl/builds/ directory and open the `libcurl-vc-x86-release-dll-ipv6-sspi-winssl` directory.
      11. Inside the `bin` folder you will find **libcurl.dll** and inside of the `lib` folder you will find **libcurl.lib** 
      12. Copy both **libcurl.dll** and **libcurl.lib** into /curl/lib/
      13. Congrats you just installed curl! It really was *that hard* on windows. Luckily you have this little guide! Maybe use a UNIX system next time lol.

3. Install [vcpkg](https://github.com/Microsoft/vcpkg)

4. Install [cpprestsdk](https://github.com/Microsoft/cpprestsdk)
   * Use vcpkg to do this. Getting the dependencies for cpprestsdk manually is not a good idea.
   * `vcpkg install cpprestsdk cpprestsdk:x64-windows`
   
5. Run commands in ApolloTradingBot directory:
   * `cd Build`
   * `cmake ../ -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake` (**Note**: If you did not install vckpkg to C:\\ then specify your system's path to vcpkg's root directory.)

6. Open `ApolloTradingBot.sln` inside the build folder. After opening the ApolloTradingBot.sln, open or locate the solution explorer window. Right-click on ApolloTradingBot -> "set as start-up project" since the default start-up project is ALL_BUILD.

7. `ctrl-shift-b` to build.
