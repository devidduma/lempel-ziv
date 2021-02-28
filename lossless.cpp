#include <iostream>
#include <fstream>
using namespace std;

int main() {
    ifstream fin1("input1", ios_base::binary);
    string data1((istreambuf_iterator<char>(fin1)),
                istreambuf_iterator<char>());

    ifstream fin2("input1lzwdecompressed", ios_base::binary);
    string data2((istreambuf_iterator<char>(fin2)),
                 istreambuf_iterator<char>());

    data1 == data2 ? cout << "Files are equal." << endl : cout << "Files are different." << endl;
}