#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <cmath>
using namespace std;

map<string, string> codes;
set<string> setCode;
string fileContent;
int r;
char toChar(string s) {
    int n = 0;
    for(int i = 0; i < s.length(); i++) {
        n = n*10 + (s[i] - '0');
    }
    return (char)n;
}
void printDecodedContent(std::ostream& out)
{
    // Mở file
    std::ifstream file1("map.txt");
    if (!file1.is_open()) {
        std::cerr << "Không thể mở file!" << std::endl;
        return;
    }
    // Đọc dữ liệu mã hóa từ file
    string line;
    getline(file1, line);
    r = line[line.size() - 2] - '0';
    while(getline(file1, line)) {
        for(int i = 0; i < line.size(); i++) {
            if(line[i] == ':') {
                string symbol = line.substr(0, i);
                string code = line.substr(i + 2);
                codes[code] = symbol;
                setCode.insert(code);
                break;
            }
        }
    }
    file1.close();


    // Mở file
    std::ifstream file2("content.txt");
    if (!file2.is_open()) {
        std::cerr << "Không thể mở file!" << std::endl;
        return;
    }

    // Đọc nội dung file và giải mã
    getline(file2, fileContent);
    string code = "";
    for(int i = 0; i < fileContent.size(); i++) {
        code += fileContent[i];
        if(setCode.find(code) != setCode.end()) {
            out << toChar(codes[code]);
            code = "";
        }
    }
    file2.close();
}

int main()
{
    // Ghi kết quả vào file
    std::ofstream outFile("decode.txt");
    if (outFile.is_open()) {
        printDecodedContent(outFile);
        outFile.close();
    } else {
        std::cerr << "Không thể mở file để ghi kết quả!" << std::endl;
    }
    return 0;
}
