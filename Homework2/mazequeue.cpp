#include <queue>
#include <iostream>
using namespace std;

class Coord
{
    public:
    Coord(int r, int c) : m_row(r), m_col(c) {}
    int r() const { return m_row; }
    int c() const { return m_col; }
    private:
    int m_row;
    int m_col;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    // Return true if there is a path from (sr,sc) to (er,ec)
    // through the maze; return false otherwise
    queue<Coord> coordQueue;
    Coord a(sr, sc);
    coordQueue.push(a);
    maze[sr][sc] = 'O';
    while(!coordQueue.empty())
    {  
        Coord temp = coordQueue.front();
        coordQueue.pop();
        if(temp.r() == er && temp.c() == ec){return true;}
        if(temp.r()-1 > 0 && maze[temp.r()-1][temp.c()] == '.')
        {
            coordQueue.push(Coord(temp.r()-1, temp.c()));
            maze[temp.r()-1][temp.c()] = 'O';
        }
        if(temp.c()-1 > 0 && maze[temp.r()][temp.c()-1] == '.')
        {
            coordQueue.push(Coord(temp.r(), temp.c()-1));
            maze[temp.r()][temp.c()-1] = 'O';
        }
        if(temp.c()+1 < nCols && maze[temp.r()][temp.c()+1] == '.')
        {
            coordQueue.push(Coord(temp.r(), temp.c()+1));
            maze[temp.r()][temp.c()+1] = 'O';
        }
        if(temp.r()+1 < nRows && maze[temp.r()+1][temp.c()] == '.')
        {
            coordQueue.push(Coord(temp.r()+1, temp.c()));
            maze[temp.r()+1][temp.c()] = 'O';
        }
    }
    return false;
}

int main()
{
    string maze[10] = {
        "XXXXXXXXXX",
        "X........X",
        "XX.X.XXXXX",
        "X..X.X...X",
        "X..X...X.X",
        "XXXX.XXX.X",
        "X.X....XXX",
        "X..XX.XX.X",
        "X...X....X",
        "XXXXXXXXXX"
    };

    if (pathExists(maze, 10,10, 6,4, 1,1))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
}


