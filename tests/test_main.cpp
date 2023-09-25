#include <gtest/gtest.h>
#include "../main.cpp"

TEST(ConvertPdfToImageTest, HandlesValidInput) {
    // Call convertPdfToImage with a known PDF and check the results
}

TEST(ConvertPdfToImageTest, HandlesInvalidInput) {
    // Test how convertPdfToImage handles invalid input
}

TEST(SaveImageTest, SavesCorrectly) {
    //  test  saveImage
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
