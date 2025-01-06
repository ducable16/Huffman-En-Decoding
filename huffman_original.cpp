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
    double probability;
    string symbol;
    vector<Node *> children;
    string code;

    Node(double p = 0.0, string s = "") : probability(p), symbol(s) {}
};

struct CompareNodes
{
    bool operator()(Node *a, Node *b)
    {
        return a->probability > b->probability;
    }
};

vector<Node *> nodes;
Node *root;
map<string, string> codes;
int r, q;

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
        pq.push(new Node(0.0, "_pad" + to_string(i)));
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
            parent->probability += pq.top()->probability;
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

bool input()
{
    cout << "Nhap co so r (2-6): ";
    cin >> r;
    if (r < 2 || r > 6)
    {
        cout << "Du lieu khong hop le!\n";
        return 0;
    }
    cout << "Nhap so luong tin q (<12): ";
    cin >> q;
    if (q >= 12)
    {
        cout << "Du lieu khong hop le!\n";
        return 0;
    }

    
    // Nhập xác suất cho các symbol
    while(true) {
        double totalProb = 0.0;                              
        for (int i = 0; i < q; i++) {
            string symbol = "s" + to_string(i + 1);
            double prob = 0;
            // Yêu cầu nhập xác suất trong khoảng (0,1)
            while(prob >= 1.0 || prob <= 0.0)
            {
                cout << "Nhap xac suat cho " << symbol << ": ";
                cin >> prob;
                if (prob >= 1.0 || prob <= 0.0)
                    cout << "Xac suat phai trong khoang (0,1)!\n";
            }

            totalProb += prob;
            nodes.push_back(new Node(prob, symbol));
        }
        // Kiểm tra tổng xác suất
        if (totalProb != 1.0) {     
            cout << totalProb << '\n';    
            cout << "Tong xac suat phai bang 1!\n";
            nodes.clear();
        }
        // Nếu tổng xác suất đúng thì thoát
        else break;
    }

    return 1;
}

void encode()
{
    buildTree();
    generateCodes(root);
}

void printCodes()
{
    cout << "\nMa Huffman co so " << r << ":\n";
    for (const auto &pair : codes)
    {
        cout << pair.first << ": " << pair.second << '\n';
    }
}

int main()
{
    if(!input()) return 0;
    encode();
    printCodes();
    cleanup(root);
    return 0;
}