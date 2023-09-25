#include <iostream>
#include <boost/filesystem.hpp>
#include <poppler/cpp/poppler-document.h>
#include <poppler/cpp/poppler-page.h>
#include <poppler/cpp/poppler-page-renderer.h>
#include <poppler/cpp/poppler-image.h>
#include <opencv2/opencv.hpp>

void saveImage(const poppler::image& const_img, const std::string& folder_name, const std::string& filename) {
    // Create a non-const copy of the image
    poppler::image img = const_img;

    // Create an OpenCV Mat object with the dimensions and type of the poppler::image
    cv::Mat mat(img.height(), img.width(), CV_8UC3, (void*)img.data());

    // Save the image using OpenCV's imwrite()
    std::string full_path = folder_name + "/" + filename;
    cv::imwrite(full_path, mat);
}

void convertPdfToImage(const std::string& pdfPath) {
    poppler::document *doc = poppler::document::load_from_file(pdfPath);
    if (doc == nullptr) {
        std::cout << "Failed to open " << pdfPath << std::endl;
        return;
    }

    // Create a directory named after the PDF file
    std::string folder_name = pdfPath.substr(pdfPath.find_last_of("/")+1);
    folder_name = folder_name.substr(0, folder_name.find_last_of("."));
    boost::filesystem::create_directory(folder_name);

    for (int i = 0; i < doc->pages(); ++i) {
        poppler::page *p = doc->create_page(i);
        if (p == nullptr) continue;

        poppler::page_renderer renderer;
        poppler::image img = renderer.render_page(p);

        saveImage(img, folder_name, "page_" + std::to_string(i) + ".jpg");

        delete p;
    }
    delete doc;
}

int main() {
    const boost::filesystem::path pdfDir("pdf_folder/");
    if (!boost::filesystem::exists(pdfDir)) {
        std::cout << "Folder does not exist\n";
        return 1;
    }

    for (const auto& entry : boost::filesystem::directory_iterator(pdfDir)) {
        if (entry.path().extension() == ".pdf") {
            convertPdfToImage(entry.path().string());
        }
    }

    return 0;
}
