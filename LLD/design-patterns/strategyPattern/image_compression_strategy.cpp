#include <iostream>
#include <memory>
#include <string>
using namespace std;

// Strategy interface
class CompressionStrategy {
public:
    virtual void compress(const string& filename) = 0;
    virtual ~CompressionStrategy() {}
};

class ZipCompression : public CompressionStrategy {
public:
    void compress(const string& filename) override {
        cout << "Compressing " << filename << " using ZIP algorithm\n";
    }
};

class RarCompression : public CompressionStrategy {
public:
    void compress(const string& filename) override {
        cout << "Compressing " << filename << " using RAR algorithm\n";
    }
};

class PngCompression : public CompressionStrategy {
public:
    void compress(const string& filename) override {
        cout << "Compressing " << filename << " using PNG lossless algorithm\n";
    }
};

// Context
class ImageCompressor {
    unique_ptr<CompressionStrategy> strategy;
public:
    void setStrategy(CompressionStrategy* s) {
        strategy.reset(s);
    }
    void compressImage(const string& filename) {
        if (strategy)
            strategy->compress(filename);
        else
            cout << "No compression strategy set!\n";
    }
};

int main() {
    ImageCompressor compressor;
    compressor.setStrategy(new ZipCompression());
    compressor.compressImage("photo.jpg");

    compressor.setStrategy(new RarCompression());
    compressor.compressImage("archive.tar");

    compressor.setStrategy(new PngCompression());
    compressor.compressImage("diagram.bmp");
    return 0;
}
