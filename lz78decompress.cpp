#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <fstream>
#include <string>
using namespace std;

class LZ78Dec {
private:
    string data;

public:
    vector<string> table;
    int counter = 0;

    explicit LZ78Dec(string data) {
        this->data = std::move(data);
    }

    string decompress() {
        string decompressed_data;

        while(!this->data.empty()) {
            tuple pair = readPairFromData();
            long id = get<0>(pair);
            char next_char = get<1>(pair);

            // if id = 0, then no need for table lookup
            if(id > 0) {
                decompressed_data += this->table[id - 1];
                // add to table
                this->table.emplace_back(this->table[id - 1] + next_char);
            } else {
                // add to table
                this->table.emplace_back(string(1, next_char));
            }

            if(next_char != NULL) {
                decompressed_data += next_char;
            }
        }

        return decompressed_data;
    }

    tuple<long, char> readPairFromData() {
        long base = 128;

        long pos = 0;

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
        pos += 1;

        char next_char;
        // next_char and remove tuple
        if(pos < this->data.length()) {
            next_char = this->data[pos];
            this->data = string(this->data.begin() + pos + 1, this->data.end());
        } else {
            next_char = NULL;
            this->data = "";
        }

        this->counter++;
        /*
        if(id > this->table.size())
            cout << endl << "ID is bigger than table size: " << data.length() << " " << pos << " " << id << " " << this->table.size() << " "<< next_char << endl;
        */
        return tuple(id, next_char);
    }
};

int main() {
    fstream fin("dna.50MB.lines1.lz78", ifstream::in | ifstream::binary);

    string data((istreambuf_iterator<char>(fin)),
                istreambuf_iterator<char>());
    long datasize = data.size();

    ofstream fout("dna.50MB.lines1.lz78.decompressed");

    LZ78Dec lz78dec = LZ78Dec(data);

    fout << lz78dec.decompress();

    cout << "Data size is: " << datasize << endl;
    cout << "Number of tuples: " << lz78dec.counter << endl
    << "Size of lookup array: " << lz78dec.table.size() << endl;
    fin.close();
    fout.close();
}