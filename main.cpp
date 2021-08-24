#include<bits/stdc++.h>

using namespace std;
enum cell_t {zero=0, one, two, three, four, five, six, seven, eight, bomb};

class Cell {
    bool is_uncovered;
    bool is_flagged;
    cell_t value;

    public:
        Cell(): is_uncovered(false), is_flagged(false), value(zero) {};
        bool is_uncovered () { return is_uncovered; }
        bool is_flagged () { return is_flagged; }
        bool get_value () {return value; } 

};

class Grid {
    int width;
    int height;
    vector<vector<Cell>> field;

    public:
        Grid (int, int);

};

Grid::Grid (int width, int height){
    this->width = width;
    this->height = height;
    for (int h=0; h<this->height; ++h){
        vector<Cell> row;
        for (int w=0; w<this->width; ++w){
            row.push_back(Cell());
        }
        this->field.push_back(row);
    }
}

int main(){


    return 0;
}