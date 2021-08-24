#include<bits/stdc++.h>

using namespace std;
enum cell_t {zero=0, one, two, three, four, five, six, seven, eight, bomb};

class Cell {
    bool cover;
    bool flag;
    cell_t value;

    public:
        Cell(): cover(true), flag(false), value(zero) {};
        bool is_covered () { return cover; }
        bool is_flagged () { return flag; }
        cell_t get_value () {return value; } 

};

class Grid {
    int width;
    int height;
    vector<vector<Cell>> field;

    public:
        Grid (int, int);
        int get_width() {return width;}
        int get_height() {return height;}
        void display ();
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
void Grid::display (){
    for (int h=0; h<height; ++h){
        for (int w=0; w<width; ++w){
            if (field[h][w].is_flagged()) cout<<'f';
            else if (field[h][w].is_covered()) cout<<'.';
            else if (field[h][w].get_value() <= 8) cout<<field[h][w].get_value();
            else cout<<"*";
        }
        cout<<endl;
    }
}

int main(){
    const int WIDTH = 10;
    const int HEIGHT = 10;

    Grid Game_grid (WIDTH, HEIGHT);
    Game_grid.display();


    return 0;
}