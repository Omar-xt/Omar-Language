#include <iostream>
#include <map>
#include <string>
using namespace std;

int main()
{
    map<string, string> CapitalCity;

    // Adding the elements
    CapitalCity["New Delhi"] = "India";
    CapitalCity["Bangalore"] = "Karnataka";
    CapitalCity["Mumbai"] = "Maharashtra";

    // Traversing through the map elements
    for (auto element : CapitalCity)
    {

        // element.first represents key
        cout << element.first << " is the capital of ";

        // element.second represents value
        cout << element.second << endl;
    }
    return 0;
}