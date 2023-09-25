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

void convertPdfToImage(const std::string& pdfPath, const std::string& savePath, int numPagesToConvert) {
    poppler::document *doc = poppler::document::load_from_file(pdfPath);
    if (doc == nullptr) {
        std::cout << "Failed to open " << pdfPath << std::endl;
        return;
    }

    // Create a directory named after the PDF file
    std::string folder_name = savePath;
    if (boost::filesystem::exists(folder_name)) {
        boost::filesystem::remove_all(folder_name);  // Remove if already exists
    }
    boost::filesystem::create_directory(folder_name);

    int totalPages = doc->pages();
    int endPage = (numPagesToConvert == -1) ? totalPages : std::min(totalPages, numPagesToConvert);

    for (int i = 0; i < endPage; ++i) {
        poppler::page *p = doc->create_page(i);
        if (p == nullptr) continue;

        poppler::page_renderer renderer;
        renderer.set_render_hint(poppler::page_renderer::antialiasing, true);

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


int main(int argc, char* argv[]) {
    std::string pdfPath = "pdf_folder/";
    std::string savePath = "";
    int numPagesToConvert = -1;  // -1 means all pages

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        // -f or --file: To specify an exact file path for the PDF to convert. 
        // If not specified, the program will convert all PDF files in the pdf_folder directory.
        if (arg == "-f" && i + 1 < argc) {
            pdfPath = argv[++i];
        } 
        // -s or --save: To specify a path where to save the generated images. 
        // If not specified, the images will be saved in the same directory as the PDF file.
        else if (arg == "-s" && i + 1 < argc) {
            savePath = argv[++i];
        } 
        // -p or --pages: To specify the number of pages in the PDF to convert. 
        // If not specified, all pages will be converted.
        else if (arg == "-p" && i + 1 < argc) {
            numPagesToConvert = std::stoi(argv[++i]);
        }
    }

    // If no save path is specified, use the PDF path as the save path
    if (savePath.empty()) {
        savePath = pdfPath.substr(0, pdfPath.find_last_of("."));
    }

    if (boost::filesystem::is_directory(pdfPath)) {
        for (const auto& entry : boost::filesystem::directory_iterator(pdfPath)) {
            if (entry.path().extension() == ".pdf") {
                std::string individual_pdf_path = entry.path().string();
                convertPdfToImage(individual_pdf_path, savePath, numPagesToConvert);
            }
        }
    } else {
        // If pdfPath is a file, convert it directly
        convertPdfToImage(pdfPath, savePath, numPagesToConvert);
    }

    return 0;
}
