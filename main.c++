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

    void initializeGridRandom()
    {
        sort(cells_tobeplaced.begin(), cells_tobeplaced.end());
        cells_tobeplaced.erase(unique(cells_tobeplaced.begin(), cells_tobeplaced.end()), cells_tobeplaced.end());

        grid.resize(num_rows, vector<int>(num_cols, -1));

        // Create a vector with both cells and empty sites
        vector<int> allcells(num_rows * num_cols, -1);
        for (int cell : cells_tobeplaced)
        {
            allcells[cell] = cell;
        }

        random_shuffle(allcells.begin(), allcells.end());

        int index = 0;
        for (int row = 0; row < num_rows; ++row)
        {
            for (int col = 0; col < num_cols; ++col)
            {
                grid[row][col] = allcells[index++];
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

    pair<int, int> findpos(int cell)
    {
        for (int row = 0; row < num_rows; ++row)
        {
            for (int col = 0; col < num_cols; ++col)
            {
                if (grid[row][col] == cell)
                {
                    return make_pair(row, col);
                }
            }
        }
        cerr << "cell not on the grid" << endl;
        return make_pair(-1, -1);
    }

    double HPWLcalc(vector<int> &net)
    {
        double res_hpwl = 0.0;

        for (int i = 1; i < net.size(); ++i)
        {
            // pos of cells on the grid
            pair<int, int> pos1 = findpos(net[i - 1]);
            pair<int, int> pos2 = findpos(net[i]);

            // coord of rows
            double row1 = pos1.first + 0.5;
            double row2 = pos2.first + 0.5;

            // coord of columns
            double col1 = pos1.second + 0.5;
            double col2 = pos2.second + 0.5;

            // Euclidean distance from the center
            res_hpwl = res_hpwl + sqrt(pow(row1 - row2, 2) + pow(col1 - col2, 2));
        }

        return res_hpwl;
    }

    int calcTotalWireLenghth()
    {
        int totalLength = 0;

        for (auto &net : nets)
        {
            totalLength += HPWLcalc(net);
        }

        return totalLength;
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
    string netlistFileName = "d2.txt"; // Provide the correct file name

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
        cout << "Net " << i + 1 << " components: ";
        for (int j = 0; j < nets[i].size(); ++j)
        {
            cout << nets[i][j] << " ";
        }
        cout << endl;
    }

    placement.initializeGridRandom();

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

    cout << "\n\n HPWL for each net:" << endl;
    // Calculate and display HPWL for each net
    for (int i = 0; i < nets.size(); ++i)
    {
        cout << "Net " << i + 1 << " HPWL: " << placement.HPWLcalc(nets[i]) << endl;
    }

    cout << "\n Total Wire Length:" << placement.calcTotalWireLenghth() << endl;
    cout << "----------------------------------------------------" << endl;

    // Set initial temperature and cooling rate
    //  double initialTemperature = 500.0 * placement.calcTotalWireLenghth();

    // Display the final floor plan
    cout << "\nFinal Floor Plan:" << std::endl;
    placement.displayfloor();

    cout << "\nFinal Floor Plan IN BINARY:" << endl;
    placement.binaryfloorplan();
    // cout << "\nFinal HPWL: " << placement.calculateTotalHPWL() << endl;
}