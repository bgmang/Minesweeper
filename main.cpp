#include<bits/stdc++.h>

using namespace std;

enum cell_t {zero=0, one, two, three, four, five, six, seven, eight, mine};
enum move_t {flag=true, step};

struct Move{
    move_t type;
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
        void set_value (cell_t val) {this->value = val;}
        void set_flag (bool flag) {this->flag = flag;}
 
        void display ();
        void final_display();
        void uncover () {this->cover = false;}
        
};

void Cell::display (){
    if (is_flagged()) cout<<"f ";
    else if (is_covered()) cout<<". ";
    else if (value <= 8) cout<<value<<" ";
    else cout<<"* ";
}
void Cell::final_display (){
    if (is_flagged()){
        if (value == mine) cout<<"Z ";
        else cout<<"X ";
    } 
    else if (value == mine) cout<<"* ";
    else if (is_covered()) cout<<". ";
    else cout<<value<<" ";    
}

class Grid {
    int width;
    int height;
    vector<vector<Cell>> field;

    public:
        Grid (int, int);

        void display ();
        void final_display ();
        void setup (int);
        bool is_move_valid (Move);
        bool apply_move (Move);
        bool is_complete(); 
        // Used for debugging only
        void uncover();

};

void add_cells(int, int, vector<vector<Cell>>&);
void place_mines (int, vector<vector<Cell>>&);

Grid::Grid (int width, int height){
    this->width = width;
    this->height = height;
    add_cells (height, width, this->field);
}
void Grid::display (){
    for (auto row : field){
        for (auto cell : row){
            cell.display();
        }
        cout<<endl;
    }
}
void Grid::final_display (){
    for (auto row : field){
        for (auto cell : row){
            cell.final_display();
        }
        cout<<endl;
    }
}
void Grid::setup (int num_mines){

    // Place the mines first
    place_mines (num_mines, this->field);

    // random_device rd;
    // mt19937 rgen ( rd() );

    // for (int k=0; k<num_mines; ++k){
    //     // Attempt to place mine
    //     int h, w;
    //     do{
    //         h = rgen() % height;
    //         w = rgen() % width;
    //     }while (field[h][w].get_value() == mine);
    //     field[h][w].set_value(mine);
    // }
    
    // Mark cells with num of adjacent mines
    vector<int> dh {-1, -1, -1, 0, 0, 1, 1, 1};
    vector<int> dw {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int h=0; h<height; ++h){
        for (int w=0; w<width; ++w){
            if (field[h][w].get_value() == mine) continue;
            
            unsigned int num_adj_mines = 0;
            // Visit all neighbouring cells
            for (int i=0; i<dh.size(); ++i){
                int neighbour_h = h + dh[i];
                int neighbour_w = w + dw[i];
                if ((neighbour_h >= 0 && neighbour_h < height) && 
                    (neighbour_w >=0 && neighbour_w < width) && 
                    field[neighbour_h][neighbour_w].get_value() == mine) 
                    num_adj_mines++;
            }
            field[h][w].set_value(static_cast<cell_t>(num_adj_mines));
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
    if (move.type == flag){
        if (field[move.h][move.w].is_covered()) 
            field[move.h][move.w].set_flag(! field[move.h][move.w].is_flagged());
        return false;
    } else {
        Cell cell = field[move.h][move.w];
        if (cell.is_flagged()) return false;
        if (! cell.is_covered()) return false;
        if (cell.get_value() == mine) return true;
        
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
            if (field[h][w].get_value() != mine && field[h][w].is_covered())
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
    if (c == 'f') move.type = flag;
    else move.type = step;

    move.h--; move.w--;

    return move;
}


void add_cells (int height, int width, vector<vector<Cell>>& field){
    for (int h=0; h<height; ++h){
        vector<Cell> row;
        for (int w=0; w<width; ++w)
            row.push_back(Cell());
        
        field.push_back(row);
    }
}
void place_mines (int num_mines, vector<vector<Cell>>& field){
    random_device rd;
    mt19937 rgen (rd());
    
    while(num_mines--){
        int h, w;
        do{
            h = rgen() % field.size();
            w = rgen() % field[0].size();
        }while (field[h][w].get_value() == mine);
        field[h][w].set_value(mine);
    }
}

int main(){
    const int WIDTH = 8;
    const int HEIGHT = 8;
    const int num_mines = 10;
    
    bool game_over = false;
    bool game_won = false;

    Grid Game_grid (WIDTH, HEIGHT);
    Game_grid.setup(num_mines);

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