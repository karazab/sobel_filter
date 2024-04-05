#include <iostream>
#include<fstream>
#include<vector>
using namespace std;

const int masks[8][3][3] = {
{
        { 1,  2,  1},
        { 0,  0,  0},
        {-1, -2, -1}
    },
{
        { 2,  1, 0 },
        { 1, 0, -1 },
        { 0, -1, -2}
    },
{
        { 1, 0, -1},
        { 2, 0, -2},
        { 1, 0, -1}
    },
{
        { 0, -1, -2},
        { 1,  0, -1},
        { 2,  1,  0}
    },
{
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    },
 {
        {-2, -1, 0},
        {-1, 0,  1},
        { 0, 1,  2}
    },
{
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    },
{
        { 0, 1,  2},
        {-1, 0,  1},
        {-2, -1, 0}
    }

};

struct color {
    float r;
    float g;
    float b;
};
struct BITMAPFILEHEADER {
    char bfType[2];
    unsigned int bfSize, bfOffBits;
    unsigned short bfReserved1, bfReserved2;
};
struct BITMAPINFOHEADER {
    unsigned int biSize, biWidth, biHeight, biCompression, biSizeImage, biXpelsPerMeter, biYpelsPerMeter, biCrlUses, biCrlImportant, biRealWidth, biPadding;
    unsigned short biPlanes, biBitCount;
};


void read_BITMAPFILEHEADER(ifstream& ifs, BITMAPFILEHEADER& bfh) {
    ifs.read(reinterpret_cast<char*> (&bfh.bfType), 2);
    ifs.read(reinterpret_cast<char*> (&bfh.bfSize), 4);
    ifs.read(reinterpret_cast<char*> (&bfh.bfReserved1), 2);
    ifs.read(reinterpret_cast<char*> (&bfh.bfReserved2), 2);
    ifs.read(reinterpret_cast<char*> (&bfh.bfOffBits), 4);

    cout << "BITMAPFILEHEADER: " << endl << endl;
    cout << "Sygnatura pliku (bfType):" << bfh.bfType[0] << bfh.bfType[1] << endl;
    cout << "Dlugosc calego pliku w bajtach (bfSize): " << bfh.bfSize << endl;
    cout << "Pole zarezerwowane 1 (bfReserved1): " << bfh.bfReserved1 << endl;
    cout << "Pole zarezerwowane 2 (bfReserved2): " << bfh.bfReserved2 << endl;
    cout << "Pozycja danych obrazowych w pliku (bfOffBits): " << bfh.bfOffBits << endl;
}

void read_BITMAPINFOHEADER(ifstream& ifs, BITMAPINFOHEADER& bih) {
    ifs.read(reinterpret_cast<char*>(&bih.biSize), 4);
    ifs.read(reinterpret_cast<char*>(&bih.biWidth), 4);
    ifs.read(reinterpret_cast<char*>(&bih.biHeight), 4);
    ifs.read(reinterpret_cast<char*>(&bih.biPlanes), 2);
    ifs.read(reinterpret_cast<char*>(&bih.biBitCount), 2);
    ifs.read(reinterpret_cast<char*>(&bih.biCompression), 4);
    ifs.read(reinterpret_cast<char*>(&bih.biSizeImage), 4);
    ifs.read(reinterpret_cast<char*>(&bih.biXpelsPerMeter), 4);
    ifs.read(reinterpret_cast<char*>(&bih.biYpelsPerMeter), 4);
    ifs.read(reinterpret_cast<char*>(&bih.biCrlUses), 4);
    ifs.read(reinterpret_cast<char*>(&bih.biCrlImportant), 4);
    bih.biRealWidth = (bih.biWidth * 3 + 3) / 4 * 4;
    bih.biPadding = bih.biRealWidth - bih.biWidth * 3;

    cout << endl << "BITMAPINFOHEADER: " << endl << endl;
    cout << "Rozmiar naglowka informacyjnego (biSize): " << bih.biSize << endl;
    cout << "Szerokosc obrazu w pikselach (biWidth): " << bih.biWidth << endl;
    cout << "Wysokosc obrazu w pikselach (biHeight): " << bih.biHeight << endl;
    cout << "Liczba platow (biPlanes): " << bih.biPlanes << endl;
    cout << "Liczba bitow na piksel (biBitCount): " << bih.biBitCount << endl;
    cout << "Algorytm kompresji (biCompression): " << bih.biCompression << endl;
    cout << "Rozmiar rysunku (biSizeImage): " << bih.biSizeImage << endl;
    cout << "Rozdzielczosc pozioma (biXpelsPerMeter): " << bih.biXpelsPerMeter << endl;
    cout << "Rozdzielczosc pionowa (biYpelsPerMeter): " << bih.biYpelsPerMeter << endl;
    cout << "Liczba kolorow w palecie (biCrlUses): " << bih.biCrlUses << endl;
    cout << "Liczba waznych kolorow w palecie (biCrlImportant): " << bih.biCrlImportant << endl;
}

void read_pixels(ifstream& ifs, BITMAPINFOHEADER& bih, BITMAPFILEHEADER& bfh, vector<vector<color>>& pixels) {
    pixels.resize(bih.biHeight);
    for (int i = 0; i < bih.biHeight; i++) {
        pixels[i].resize(bih.biWidth);
    }
    for (int i = 0; i < bih.biHeight; i++) {
        for (int j = 0; j < bih.biWidth; j++) {
            unsigned char color[3];
            ifs.read(reinterpret_cast<char*>(color), 3);
            pixels[i][j].r = static_cast<float>(color[2]);
            pixels[i][j].g = static_cast<float>(color[1]);
            pixels[i][j].b = static_cast<float>(color[0]);
        }
        ifs.ignore(bih.biPadding);
    }
}

void convolution(vector<vector<color>>& pixels, vector<vector<color>>& new_pixels) {
    int max = 225;
    int min = 0;

    new_pixels.resize(pixels.size());
    for (int i = 0; i < pixels.size(); i++) {
        new_pixels[i].resize(pixels[i].size());
    }

    for (int i = 0; i < new_pixels.size(); i++) {
        for (int j = 0; j < new_pixels[i].size(); j++) {
            new_pixels[i][j].r = 0;
            new_pixels[i][j].g = 0;
            new_pixels[i][j].b = 0;
        }
    }

    for (int i = 1; i < pixels.size() - 1; i++) {
        for (int j = 1; j < pixels[i].size() - 1; j++) {

            float sum = 0;

            for (int k = 0; k < 8; k++) {

                sum = 0;
                for (int a = -1; a < 2; a++) {
                    for (int b = -1; b < 2; b++) {
                        sum = sum + (pixels[i + a][j + b].r * masks[k][a + 1][b + 1]);
                    }
                }
                if (sum > max) sum = max;
                if (sum < min) sum = min;
                if (sum > new_pixels[i][j].r) new_pixels[i][j].r = sum;

                sum = 0;
                for (int a = -1; a < 2; a++) {
                    for (int b = -1; b < 2; b++) {
                        sum = sum + (pixels[i + a][j + b].g * masks[k][a + 1][b + 1]);
                    }
                }
                if (sum > max) sum = max;
                if (sum < min) sum = min;
                if (sum > new_pixels[i][j].g) new_pixels[i][j].g = sum;

                sum = 0;
                for (int a = -1; a < 2; a++) {
                    for (int b = -1; b < 2; b++) {
                        sum = sum + (pixels[i + a][j + b].b * masks[k][a + 1][b + 1]);
                    }
                }
                if (sum > max) sum = max;
                if (sum < min) sum = min;
                if (sum > new_pixels[i][j].b) new_pixels[i][j].b = sum;
            }
        }
    }
}

void new_BITMAPFILEHEADER(ofstream& f, BITMAPFILEHEADER& bfh) {
    f.write(reinterpret_cast<char*> (&bfh.bfType), 2);
    f.write(reinterpret_cast<char*> (&bfh.bfSize), 4);
    f.write(reinterpret_cast<char*> (&bfh.bfReserved1), 2);
    f.write(reinterpret_cast<char*> (&bfh.bfReserved2), 2);
    f.write(reinterpret_cast<char*> (&bfh.bfOffBits), 4);
}

void new_BITMAPINFOHEADER(ofstream& f, BITMAPINFOHEADER& bih) {
    f.write(reinterpret_cast<char*>(&bih.biSize), 4);
    f.write(reinterpret_cast<char*>(&bih.biWidth), 4);
    f.write(reinterpret_cast<char*>(&bih.biHeight), 4);
    f.write(reinterpret_cast<char*>(&bih.biPlanes), 2);
    f.write(reinterpret_cast<char*>(&bih.biBitCount), 2);
    f.write(reinterpret_cast<char*>(&bih.biCompression), 4);
    f.write(reinterpret_cast<char*>(&bih.biSizeImage), 4);
    f.write(reinterpret_cast<char*>(&bih.biXpelsPerMeter), 4);
    f.write(reinterpret_cast<char*>(&bih.biYpelsPerMeter), 4);
    f.write(reinterpret_cast<char*>(&bih.biCrlUses), 4);
    f.write(reinterpret_cast<char*>(&bih.biCrlImportant), 4);
}

void save(ofstream& f, BITMAPINFOHEADER& bih, BITMAPFILEHEADER& bfh, vector<vector<color>>& new_pixels) {
    new_BITMAPFILEHEADER(f, bfh);
    new_BITMAPINFOHEADER(f, bih);

    for (int i = 0; i < bih.biHeight; i++) {
        for (int j = 0; j < bih.biWidth; j++) {

            unsigned char r = static_cast<unsigned char>(new_pixels[i][j].r);
            unsigned char g = static_cast<unsigned char>(new_pixels[i][j].g);
            unsigned char b = static_cast<unsigned char>(new_pixels[i][j].b);

            unsigned char color[] = { b, g ,r };
            f.write(reinterpret_cast<char*>(color), 3);
        }
        unsigned char zero[3] = { 0, 0, 0 };
        f.write(reinterpret_cast<char*>(zero), bih.biPadding);
    }
    f.close();
}


int main()
{
    BITMAPINFOHEADER bih;
    BITMAPFILEHEADER bfh;
    ifstream ifs;
    ofstream f;
    string file_name, new_file_name;
    vector<vector<color>>pixels;
    vector<vector<color>>new_pixels;

    cout << "Podaj nazwe pliku: ";
    cin >> file_name;

    ifs.open(file_name, ifstream::binary);
    if (!ifs.is_open()) {
        cout << "Nie mozna otworzyc pliku." << endl;
        return 1;
    }

    read_BITMAPFILEHEADER(ifs, bfh);
    read_BITMAPINFOHEADER(ifs, bih);

    read_pixels(ifs, bih, bfh, pixels);
    convolution(pixels, new_pixels);

    cout << "Podaj nazwe do zapisania pliku: ";
    cin >> new_file_name;
    f.open(new_file_name, ofstream::out | ofstream::binary);
    save(f, bih, bfh, new_pixels);
    return 0;
}