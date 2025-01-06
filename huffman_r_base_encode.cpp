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

struct Node
{
    int occurences;
    string symbol;
    vector<Node *> children;
    string code;

    Node(int occ, string ch) : occurences(occ), symbol(ch) {}
};
struct CompareNodes
{
    bool operator()(Node *a, Node *b)
    {
        return a->occurences > b->occurences;
    }
};

vector<Node *> nodes;
Node *root;
map<string, string> codes;
map<char, int> occurences;
set<char> setChar;
string fileContent;
int r, q;
long totalOccurences = 0;

// Tính số node padding cần thêm vào cây
int calculatePaddingNodes()
{
    double a = static_cast<double>(q - r) / (r - 1);
    int b = ceil(a);
    return r + b * (r - 1) - q;
}

void buildTree()
{
    priority_queue<Node *, vector<Node *>, CompareNodes> pq;
    for (auto node : nodes)
    {
        pq.push(node);
    }

    // Thêm node padding nếu cần
    int paddingCount = calculatePaddingNodes();
    for (int i = 0; i < paddingCount; i++)
    {
        pq.push(new Node(0, "_pad" + to_string(i)));
    }

    // Xây dựng cây cho đến khi còn 1 node
    while (pq.size() > 1)
    {

        // Tạo node cha mới
        Node *parent = new Node(0.0, "internal");

        // Lấy r node có xác suất nhỏ nhất
        for (int i = 0; i < r && !pq.empty(); i++)
        {
            parent->children.push_back(pq.top());
            parent->occurences += pq.top()->occurences;
            pq.pop();
        }

        pq.push(parent);
    }

    root = pq.top();
}

void generateCodes(Node *node, string currentCode = "")
{
    // Nếu node là node ảo thì không cần xét
    if (node->symbol[0] == '_')  return;

    // Nếu node là leaf và không phải là node ảo thì đặt code cho nó
    if (node->children.empty())
    {
        codes[node->symbol] = currentCode;
        return;
    }
    // Đệ quy cho các node con
    for (int i = 0; i < r; i++)
    {
        generateCodes(node->children[i], currentCode + to_string(i));
    }
}

void cleanup(Node *node)
{
    if (!node)
        return;
    for (auto child : node->children)
    {
        cleanup(child);
    }
    delete node;
}

void input()
{
    cout << "Nhap co so r (2-6): ";
    cin >> r;

    // Mở file
    std::ifstream file("test.inp");

    if (!file.is_open()) {
        std::cerr << "Không thể mở file!" << std::endl;
        return;
    }

    // Đọc toàn bộ dữ liệu từ file vào một chuỗi
    std::stringstream buffer;
    buffer << file.rdbuf();

    file.close();

    // Lấy dữ liệu dưới dạng chuỗi
    fileContent = buffer.str();
    

    totalOccurences = fileContent.size();

    // Đếm số lần xuất hiện của các ký tự
    for(int i = 0; i < fileContent.size(); i++) {
        setChar.insert(fileContent[i]);
        occurences[fileContent[i]]++;
    }
    q = setChar.size();

    // Tạo các node từ các ký tự
    for(auto ch : setChar) {
        nodes.push_back(new Node(occurences[ch], string(1, ch)));
    }
}

void encode()
{
    buildTree();
    generateCodes(root);
}

void printEncodedMap(std::ostream& out) {
    out << "Ma Huffman co so " << r << ":\n";
    for (const auto &pair : codes) {
        out << (int)pair.first[0] << ": " << pair.second << '\n';
    }
}

void printEncodedContent(std::ostream& out, const std::string& fileContent) {
    for (auto ch : fileContent) {
        out << codes[string(1, ch)];
    }
}
int main()
{
    input();
    encode();

    // Ghi kết quả ra file
    std::ofstream outFile1("map.txt");
    if (outFile1.is_open()) {
        printEncodedMap(outFile1);
        outFile1.close();
    } else {
        std::cerr << "Không thể mở file để ghi kết quả!" << std::endl;
    }
    std::ofstream outFile2("content.txt");
    if (outFile2.is_open()) {
        printEncodedContent(outFile2, fileContent);
        outFile2.close();
    } else {
        std::cerr << "Không thể mở file để ghi kết quả!" << std::endl;
    }
    cleanup(root);
    return 0;
}
