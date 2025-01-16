#include "History.h"
#include <cctype>
#include <iostream>
using namespace std;

History::History(int nRows, int nCols)
{
    m_rows = nRows;
    m_cols = nCols;
    for (int r = 0; r < nRows; r++)
        for (int c = 0; c < nCols; c++)
            m_grid[r][c] = '.';
}

bool History::record(int r, int c)
{   int rows = r-1;
    int cols = c-1;

    if(r > m_rows || c > m_cols || r <= 0 || c <= 0){return false;}
    if(m_grid[rows][cols] == '.')
    {
        m_grid[rows][cols] = 'A';
    }else if(m_grid[rows][cols] < 'Z')
    {
        m_grid[rows][cols]++;
    }
    
    return true;
}

void History::display() const
{
    clearScreen();
    // Displays history
    for (int r = 0; r < m_rows; r++)
    {
        for (int c = 0; c < m_cols; c++)
            cout << m_grid[r][c];
        cout << endl;
    }
    cout << endl;
}

