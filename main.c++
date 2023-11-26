#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <ctime>
#include <string>
#include <vector>
using namespace std;

struct Placement
{
    int num_rows;
    int num_cols;

    int num_comptobeplaced;
    int num_connectionBWcomponets;
    vector<int> cells_tobeplaced;
    vector<vector<int>> grid;
    vector<vector<int>> nets;

    void initialRandomPlacement()
    {
        sort(cells_tobeplaced.begin(), cells_tobeplaced.end());
        cells_tobeplaced.erase(unique(cells_tobeplaced.begin(), cells_tobeplaced.end()), cells_tobeplaced.end());

        grid.resize(num_rows, vector<int>(num_cols, -1));

        vector<int> cells_and_empty_sites(num_rows * num_cols, -1);
        for (int cell : cells_tobeplaced)
        {
            cells_and_empty_sites[cell] = cell;
        }

        random_shuffle(cells_and_empty_sites.begin(), cells_and_empty_sites.end());

        int index = 0;
        for (int row = 0; row < num_rows; ++row)
        {
            for (int col = 0; col < num_cols; ++col)
            {
                grid[row][col] = cells_and_empty_sites[index++];
            }
        }
    }

    vector<vector<int>> parseNetlist(string &netlistFileName, Placement &P)
    {
        ifstream file(netlistFileName);
        if (!file.is_open())
        {
            cerr << "Error: Could not open netlist file." << endl;
            exit(1);
        }

        file >> P.num_comptobeplaced >> P.num_connectionBWcomponets >> P.num_rows >> P.num_cols;

        nets.resize(P.num_connectionBWcomponets);
        for (int i = 0; i < P.num_connectionBWcomponets; ++i)
        {
            int comp;
            file >> comp;
            nets[i].resize(comp);

            for (int j = 0; j < comp; ++j)
            {
                file >> nets[i][j];
                cells_tobeplaced.push_back(nets[i][j]);
            }
        }

        file.close();
        return nets;
    }

    void displayfloor()
    {
        for (int i = 0; i < num_rows; ++i)
        {
            for (int j = 0; j < num_cols; ++j)
            {
                if (grid[i][j] == -1)
                {
                    cout << "-- ";
                }
                else
                {
                    cout << setw(2) << setfill('0') << grid[i][j] << " ";
                }
            }
            cout << endl;
        }
    }

    void binaryfloorplan()
    {
        cout << "Floor Plan in Binary:" << endl;
        for (int i = 0; i < num_rows; ++i)
        {
            for (int j = 0; j < num_cols; ++j)
            {
                cout << (grid[i][j] == -1 ? '0' : '1');
            }
            cout << endl;
        }
    }
};

int main()
{
    double coolingRate = 0.95;

    srand(static_cast<unsigned>(time(0)));
    string netlistFileName = "d0.txt"; // Provide the correct file name

    Placement placement;

    vector<vector<int>> nets = placement.parseNetlist(netlistFileName, placement);
    cout << "----------------------------------------------------" << endl;
    cout << endl;
    cout << endl;
    cout << "Number of cells: " << placement.num_comptobeplaced << endl;
    cout << "Number of nets: " << placement.num_connectionBWcomponets << endl;
    cout << "Number of rows: " << placement.num_rows << endl;
    cout << "Number of columns: " << placement.num_cols << endl;
    cout << endl;
    cout << "NETLIST" << endl;
    cout << "----------------------------------------------------" << endl;

    for (int i = 0; i < nets.size(); ++i)
    {
        cout << "Net " << i + 1 << " cells: ";
        for (int j = 0; j < nets[i].size(); ++j)
        {
            cout << nets[i][j] << " ";
        }
        cout << endl;
    }

    placement.initialRandomPlacement();

    // Display the initial floor plan
    cout << "All CELLS: ";
    for (int i = 0; i < placement.cells_tobeplaced.size(); i++)
    {
        cout << placement.cells_tobeplaced[i] << ", ";
    }

    cout << "\nInitial Floor Plan random placement:\n"
         << endl;
    placement.displayfloor();

    cout << "\n\nInitial Floor Plan IN BINARY:" << endl;
    placement.binaryfloorplan();
}
