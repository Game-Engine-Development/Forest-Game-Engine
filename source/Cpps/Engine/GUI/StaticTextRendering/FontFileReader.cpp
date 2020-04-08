#include "Headers/Engine/GUI/StaticTextRendering/FontFileReader.h"

FontFileReader::FontFileReader()  = default;

FontFileReader::FontFileReader(const char *filename) {
    std::ifstream file (filename);

    std::array<int, 8> numbersForChar{};
    int numbersForCharIndex;
    std::string numberAsString;

    std::string line;
    while(getline(file, line, '\n')) {
        numbersForCharIndex = 0;
        numberAsString = "";
        for(int i = 0; i < line.size(); ++i) {
            if(isIntegral(line[i])) {
                numberAsString += line[i];

                if(!isIntegral(line[i+1])) {
                    numbersForChar[numbersForCharIndex] = std::stoi(numberAsString);

                    numberAsString = "";
                    ++numbersForCharIndex;
                }
            }

            if(numbersForCharIndex >= numbersForChar.size()) {
                fontInfo.insert(std::make_pair(static_cast<char>(numbersForChar[0]), Character(numbersForChar)));
                break;
            }
        }
    }

    file.close();
}

Character FontFileReader::getChar(char id) {
    return fontInfo[id];
}

FontFileReader::~FontFileReader() = default;
