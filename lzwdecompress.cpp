#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <fstream>
#include <string>
using namespace std;

class LZWDec {
private:
    string data;

public:
    vector<string> table;
    int counter = 0;

    explicit LZWDec(string data) {
        this->data = std::move(data);

        // initialize lookup table
        for(int i = 0; i < 256; i++) {
            string charToString;
            charToString += char(i);
            this->table.emplace_back(charToString);
        }
    }

    string decompress() {
        string decompressed_data;
        string old;

        while(!this->data.empty()) {
            // read id from data
            long id = readIdFromData();

            string neu;
            
            // watch out for the case when id points to the not yet written old id
            if(id == this->table.size()) {
                neu = old + old[0];
            } else {
                neu = this->table[id];
            }

            // if first character read, then no need to add to table
            if(!old.empty()) {
                // add to table
                this->table.emplace_back(old + neu[0]);
            }

            // cout << id << " " << this->table.back() << " " << neu << endl;

            // add sequence to decompressed_data
            decompressed_data += this->table[id];

            // update old tuple
            old = neu;
        }

        return decompressed_data;
    }

    long readIdFromData() {
        long base = 128;

        long pos = 0;

        // build up id from characters
        long id = 0;
        long power_up = 1;
        while(pos < this->data.length()) {
            if(this->data[pos] < 0) {
                id += (this->data[pos] + 1) * (-1) * power_up;
                break;
            } else {
                id += (this->data[pos]) * power_up;
            }

            power_up *= base;
            pos += 1;
        }

        // remove id
        if(pos < this->data.length()) {
            this->data = string(this->data.begin() + pos + 1, this->data.end());
        } else {
            this->data = "";
        }

        this->counter++;
        /*
        if(id > this->table.size())
            cout << endl << "ID is bigger than table size: " << data.length() << " " << pos << " " << id << " " << this->table.size() << endl;
        */
        return id;
    }
};

int main() {
    fstream fin("dna.50MB.lines1.lzw", ifstream::in | ifstream::binary);

    string data((istreambuf_iterator<char>(fin)),
                istreambuf_iterator<char>());
    long datasize = data.size();

    ofstream fout("dna.50MB.lines1.lzw.decompressed");

    LZWDec lzwdec = LZWDec(data);

    string decompressed_data = lzwdec.decompress();
    fout << decompressed_data;

    cout << "Data size is: " << datasize << endl;
    cout << "Number of tuples: " << lzwdec.counter << endl
    << "Size of lookup array: " << lzwdec.table.size() << endl;

    fin.close();
    fout.close();
}
