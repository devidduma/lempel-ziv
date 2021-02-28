#include <iostream>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

class LZ78 {
private:

public:
    map<string, long> table;
    vector<tuple<long, char>> compressed_data;

    LZ78() {
    }

    void compress(const string& data) {

        long pos = 0;
        while(pos < data.length()) {

            // find if string exists in dictionary
            long length = 0;
            long id = 0;

            while(pos + length + 1 < data.length() && this->table.find(data.substr(pos, length + 1)) != this->table.end()) {
                length++;
                id = this->table[data.substr(pos, length)] + 1;
            }

            // save next substring to table
            this->table[data.substr(pos, length + 1)] = this->table.size();

            // move pos to += length + 1
            pos += length + 1;
            char next_char;
            // next_char of tuple <id, next_char> is at new position
            if(pos > data.length()) {
                next_char = NULL;
            } else {
                next_char = data[pos - 1];
            }

            this->compressed_data.emplace_back(tuple(id, next_char));
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

    static string writePair(long id, char next_char) {
        string id_base = writeSegment(id);

        string result;
        if(next_char != NULL) {
            result = id_base + next_char;
        } else {
            result = id_base;
        }

        return result;
    }

    string printCompressed() {
        string compressed_data_string;

        for(tuple pair : this->compressed_data) {
            compressed_data_string += LZ78::writePair(get<0>(pair), get<1>(pair));
        }

        return compressed_data_string;
    }
};

int main() {
    ifstream fin("dna.50MB.lines1");
    string data((istreambuf_iterator<char>(fin)),
                           istreambuf_iterator<char>());

    ofstream fout("dna.50MB.lines1.lz78", ofstream::binary);

    LZ78 lz78 = LZ78();
    lz78.compress(data);

    fout << lz78.printCompressed();

    cout << "Length of data: " << lz78.printCompressed().length() << endl
        << "Number of tuples: " << lz78.compressed_data.size() <<endl
        << "Size of lookup array: " << lz78.table.size() << endl;
    fin.close();
    fout.close();
}