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
    int num_cells;
    int num_nets;
};

vector<vector<int>> parseNetlist(const string &netlistFileName, Placement &P)
{
    vector<vector<int>> netlist;
    ifstream file(netlistFileName);
    if (!file.is_open())
    {
        cerr << "Error: Could not open netlist file." << endl;
        exit(1);
    }

    file >> P.num_cells >> P.num_nets >> P.num_rows >> P.num_cols;
    netlist.resize(P.num_nets);

    for (int i = 0; i < P.num_nets; ++i)
    {
        int comp;
        file >> comp;
        netlist[i].resize(comp);

        for (int j = 0; j < comp; ++j)
        {
            file >> netlist[i][j];
        }
    }

    file.close();
    return netlist;
}

int main()
{
    srand(static_cast<unsigned>(time(0)));
    string netlistFileName = "d2.txt"; // Provide the correct file name
    Placement placement;
    vector<vector<int>> netlist = parseNetlist(netlistFileName, placement);
    cout << "----------------------------------------------------" << endl;
    cout << endl;
    cout << endl;
    cout << "Number of cells: " << placement.num_cells << endl;
    cout << "Number of nets: " << placement.num_nets << endl;
    cout << "Number of rows: " << placement.num_rows << endl;
    cout << "Number of columns: " << placement.num_cols << endl;
    cout << endl;
    cout << "NETLIST" << endl;
    cout << "----------------------------------------------------" << endl;

    for (int i = 0; i < netlist.size(); ++i)
    {
        cout << "Net " << i + 1 << " components: ";
        for (int j = 0; j < netlist[i].size(); ++j)
        {
            cout << netlist[i][j] << " ";
        }
        cout << endl;
    }
}
