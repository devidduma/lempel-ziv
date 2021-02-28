#include <iostream>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

class LZW {
private:

public:
    map<string, long> table;
    vector<long> compressed_data;

    LZW() {
        // initialize lookup table
        for(int i = 0; i < 256; i++) {
            string charToString;
            charToString += char(i);
            table.insert({charToString, i});
        }
    }

    void compress(const string& data) {

        long pos = 0;
        while(pos < data.length()) {

            // find if string exists in dictionary
            long length = 0;
            long id = 0;

            while(pos + length < data.length() && this->table.find(data.substr(pos, length + 1)) != this->table.end()) {
                length++;
                id = this->table[data.substr(pos, length)];
            }

            // save next substring to table
            this->table[data.substr(pos, length + 1)] = this->table.size();
            // cout << id << " " << data.substr(pos, length + 1) << endl;

            // move pos to += length
            pos += length;

            // emplace back id
            this->compressed_data.emplace_back(id);
        }
    }

    static string writeSegment(long x) {
        int base = 128;
        string x_base;

        if(x > 0) {
            while(x > 0) {
                x_base += char(x % base);
                x = x / base;
            }
        } else {
            x_base = char(0);
        }
        // Flip range [0 ; 127] to [-1 ; -128]
        x_base[x_base.length() - 1] = (x_base[x_base.length() - 1] + 1) * (-1);

        return x_base;
    }

    static string writePair(long id) {
        string result = writeSegment(id);

        return result;
    }

    string printCompressed() {
        string compressed_data_string;

        for(long id : this->compressed_data) {
            compressed_data_string += LZW::writePair(id);
        }

        return compressed_data_string;
    }
};

int main() {
    ifstream fin("dna.50MB.lines1");
    string data((istreambuf_iterator<char>(fin)),
                           istreambuf_iterator<char>());

    ofstream fout("dna.50MB.lines1.lzw", ofstream::binary);

    LZW lzw = LZW();
    lzw.compress(data);

    fout << lzw.printCompressed();

    cout << "Length of data: " << lzw.printCompressed().length() << endl
        << "Number of tuples: " << lzw.compressed_data.size() <<endl
        << "Size of lookup array: " << lzw.table.size() << endl;

    fin.close();
    fout.close();
}