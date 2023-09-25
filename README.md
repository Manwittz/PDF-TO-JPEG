# PDF to Image Converter in C++

## Overview
This project reads a folder containing PDF files and converts each page in the PDFs into JPEG images.

## Libraries Used
1. Boost Filesystem - For directory traversal and file manipulation.
2. Poppler - For reading PDF files.
3. OpenCV - For saving images.

## Installation

### Ubuntu
1. Install Boost:
    ```bash
    sudo apt-get install libboost-all-dev
    ```
2. Install Poppler:
    ```bash
    sudo apt-get install libpoppler-cpp-dev
    ```
3. Install OpenCV:
    ```bash
    sudo apt-get install libopencv-dev
    ```

### macOS
1. Install Boost:
    ```bash
    brew install boost
    ```
2. Install Poppler:
    ```bash
    brew install poppler
    ```
3. Install OpenCV:
    ```bash
    brew install opencv
    ```
4. Install pkg-config (if needed):
    ```bash
    brew install pkg-config
    ```

## Building the Project

### Ubuntu
1. Navigate to the project root directory.
2. Create a build directory and navigate into it:
    ```bash
    mkdir build
    cd build
    ```
3. Run CMake to generate the Makefile:
    ```bash
    cmake ..
    ```
4. Build the project:
    ```bash
    make
    ```

### macOS
1. Navigate to the project root directory.
2. Create a build directory and navigate into it:
    ```bash
    mkdir build
    cd build
    ```
3. Run CMake to generate the Makefile:
    ```bash
    cmake ..
    ```
4. If you encounter a 'pkg-config not found' error, make sure to install pkg-config:
    ```bash
    brew install pkg-config
    ```
5. Build the project:
    ```bash
    make
    ```

## Running the Project
Place some PDF files in the `pdf_folder/` directory and then run the generated executable:
```bash
./PDFtoImageConverter
```
This will save each page of the PDFs as JPEG files in the directory where the executable is located.
