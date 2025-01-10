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
{
    if(r > m_rows || c > m_cols || r < 0 || c < 0){return false;}
    if(!isalpha(m_grid[r][c]))
    {
        m_grid[r][c] = 'A';
    } else if(m_grid[r][c] < 'Z')
    {
        m_grid[r][c]++;
    } else {
        m_grid[r][c] = 'Z';
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

