#include<bits/stdc++.h>

using namespace std;

enum cell_t {zero=0, one, two, three, four, five, six, seven, eight, bomb};

struct Move{
    bool flag_move;
    int h;
    int w;
};

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
        void set_flag (bool flag) {this->flag = flag;}
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
        bool is_move_valid (Move);
        bool apply_move (Move);
        bool is_complete(); 
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
                if (field[h][w].get_value() == bomb) cout<<"Z ";
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
    random_device rd;
    mt19937 rgen ( rd() );

    
    // Place the bombs first
    for (int k=0; k<num_bombs; ++k){
        // Attempt to place bomb
        int h, w;
        do{
            h = rgen() % height;
            w = rgen() % width;
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
bool Grid::is_move_valid(Move move){
    if (move.h < 0 || move.h >height){
        cout<<"Row out of bounds. Please insert a number between 1 and "<<height<<'\n';
        return false;
    } else if (move.w < 0 || move.w > width){
        cout<<"Column out of bounds. Please insert a number between 1 and "<<width<<'\n';
        return false;
    }
    return true;
}
bool Grid::apply_move(Move move){
    if (move.flag_move){
        if (field[move.h][move.w].is_covered()) 
            field[move.h][move.w].set_flag(! field[move.h][move.w].is_flagged());
        return false;
    } else {
        Cell cell = field[move.h][move.w];
        if (cell.is_flagged()) return false;
        if (! cell.is_covered()) return false;
        if (cell.get_value() == bomb) return true;
        
        field[move.h][move.w].uncover();
        
        if (field[move.h][move.w].get_value() == zero){
        //   If the uncovered cell has 0 mines attached to it, run a BFS search 
        // to uncover all subsequent neighbouring cells with value 0;
            queue<pair<int, int>> bfs;
            bfs.push({move.h, move.w});
            while (! bfs.empty()){
                pair<int, int> loc = bfs.front();
                bfs.pop();

                field[loc.first][loc.second].uncover();
                vector<int> dh {-1, -1, -1, 0, 0, 1, 1, 1};
                vector<int> dw {-1, 0, 1, -1, 1, -1, 0, 1};

                for (int i=0; i<dh.size(); ++i){
                    int neighbour_h = loc.first + dh[i];
                    int neighbour_w = loc.second + dw[i];

                    if ( (neighbour_h >=0 && neighbour_h < height)  && (neighbour_w >=0 && neighbour_w < width) && field[neighbour_h][neighbour_w].is_covered()){
                        if (field[neighbour_h][neighbour_w].get_value() == zero)
                            bfs.push({neighbour_h, neighbour_w});
                        else
                            field[neighbour_h][neighbour_w].uncover();
                    }
                }
            }
        }
        
        return false;
    }
}
bool Grid::is_complete(){
    for (int h=0; h<height; ++h){
        for (int w=0; w<width; ++w){
            if (field[h][w].get_value() != bomb && field[h][w].is_covered())
                return false;
        }
    }
    return true;
}
Move get_move(){
    Move move;
    char c;
    cout<<"\nInsert Move. Move type is flag/step (f)/(s),  Row (1-8), Col (1-8); E.g. f 1 5, s 7 2, etc.\n";
    cout<<"Answer: ";
    cin>>c>>move.h>>move.w;
    if (c == 'f') move.flag_move = true;
    else move.flag_move = false;

    move.h--; move.w--;

    return move;
}



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
        Move move = get_move();
        while (!Game_grid.is_move_valid(move)){
            move = get_move();
        }
        game_over = Game_grid.apply_move(move);

        if (Game_grid.is_complete()){
            game_over = true;
            game_won = true;
        }
    }

    if (game_won){
        cout<<"\nCongrats, you won the game!\n\n";
    } else {
        cout<<"\nOh, no... you've stumbled on a mine!\n\n";
    }

    Game_grid.final_display();
    return 0;
}