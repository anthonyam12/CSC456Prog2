#include <iostream>

using namespace std;

int main (int argc, char* argv[])
{
    if (argc < 3) // or 4 or 5 or whatever
    {
        cout << "\nSimulation requires the following parameters: .......\n " << endl;
        return 0;
    }    

    for (int i = 1; i < argc; i++)
        cout << argv[i] << endl;

    return 0;
}
