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

[[deprecated]]
void convertPdfToImageOld(const std::string& pdfPath) {
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

        // Set resolution to 300 dpi and enable text anti-aliasing
        renderer.set_render_hint(poppler::page_renderer::text_antialiasing, true);
        poppler::image img = renderer.render_page(p, 300, 300, 0, 0, p->page_rect().width(), p->page_rect().height());

        saveImage(img, folder_name, "page_" + std::to_string(i) + ".jpg");

        delete p;
    }
    delete doc;
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
        renderer.set_render_hint(poppler::page_renderer::text_antialiasing, true);

        // Get PDF page dimensions
        poppler::rectf page_rect = p->page_rect();
        double page_width = page_rect.width();
        double page_height = page_rect.height();

        // Check if dimensions are reasonable
        if (page_width <= 0 || page_height <= 0) {
            std::cout << "Skipping page " << i << " due to invalid dimensions." << std::endl;
            continue;
        }

        // Calculate resolution dynamically based on PDF page dimensions
        double dpi_x = page_width * 72.0;  // PDF units are in 1/72 inches
        double dpi_y = page_height * 72.0; // PDF units are in 1/72 inches

        // Log the calculated DPI for debugging
        std::cout << "Calculated DPI for page " << i << ": (" << dpi_x << ", " << dpi_y << ")" << std::endl;

        // Render the page into an image
        poppler::image img = renderer.render_page(p, dpi_x, dpi_y, 0, 0, page_width, page_height);

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
