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
        void set_value (cell_t val) {this->value = val;};
        void uncover () {this->cover = false;}
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
        void final_display ();
        void setup (int);

        // Used for debugging only
        void uncover();
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
            if (field[h][w].is_flagged()) cout<<"f ";
            else if (field[h][w].is_covered()) cout<<". ";
            else if (field[h][w].get_value() <= 8) cout<<field[h][w].get_value()<<" ";
            else cout<<"* ";
        }
        cout<<endl;
    }
}
void Grid::final_display (){
    for (int h=0; h<height; ++h){
        for (int w=0; w<width; ++w){
            if (field[h][w].is_flagged()){
                if (field[h][w].get_value() == bomb) cout<<"O ";
                else cout<<"X ";
            } 
            else if (field[h][w].get_value() == bomb) cout<<"* ";
            else if (field[h][w].is_covered()) cout<<". ";
            else cout<<field[h][w].get_value()<<" "; 
        }
        cout<<endl;
    }
}
void Grid::setup (int num_bombs){
    // Place the bombs first
    for (int k=0; k<num_bombs; ++k){
        // Attempt to place bomb
        int h, w;
        do{
            h = rand() % height;
            w = rand() % width;
        }while (field[h][w].get_value() == bomb);
        field[h][w].set_value(bomb);
    }
    // Mark cells with num of adjacent bombs
    vector<int> dh {-1, -1, -1, 0, 0, 1, 1, 1};
    vector<int> dw {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int h=0; h<height; ++h){
        for (int w=0; w<width; ++w){
            if (field[h][w].get_value() == bomb) continue;
            
            unsigned int num_adj_bombs = 0;
            // Visit all neighbouring cells
            for (int i=0; i<dh.size(); ++i){
                int neighbour_h = h + dh[i];
                int neighbour_w = w + dw[i];
                if ((neighbour_h >= 0 && neighbour_h < height) && 
                    (neighbour_w >=0 && neighbour_w < width) && 
                    field[neighbour_h][neighbour_w].get_value() == bomb) 
                    num_adj_bombs++;
            }
            field[h][w].set_value(static_cast<cell_t>(num_adj_bombs));
        }
    }
} 
void Grid::uncover(){
    for (int h=0; h<height; ++h){
        for (int w=0; w<width; ++w){
            field[h][w].uncover();
        }
    }
};

int main(){
    const int WIDTH = 8;
    const int HEIGHT = 8;
    const int num_bombs = 10;
    
    bool game_over = false;
    bool game_won = false;

    Grid Game_grid (WIDTH, HEIGHT);
    Game_grid.setup(num_bombs);

    while(!game_over){
        Game_grid.display();
        game_over = true;
    }

    if (game_won){
        cout<<"\nCongrats, you won the game!\n\n";
    } else {
        cout<<"\nOh, no... you've stumbled on a mine!\n\n";
    }

    Game_grid.final_display();
    return 0;
}