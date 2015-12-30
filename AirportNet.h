/*
Ali Atlı
21302442
Sec2
*/
#include <string>
#include <iostream>
#include <map>
using namespace std;
class AirportNet{
public:
    AirportNet(const string aname);
    AirportNet(const AirportNet& aNet); // copy contructor
    ~AirportNet(); // destructor
    void listDirectFlights(const string aname);
    void listRoutesOnHubAirport(const string aname);
    void findConnectedComponents();
    void displayMostCentralAirport();
    //void init(string aname);
    //string getFileName() const;

// ...
// define
// if you
private:
    // here some functions for union-find algorithm
    int root(int i);
    bool connected(int p, int q);
    void uni(int p, int q);

    //string fileName;

    int ** dist;
    int numVertices;
    map<string, int> m;  // this map is hash table for parsing the data O(N) time. if we dont use hasth tables, then we will need to check an array
                                        // on every new line to see if the city exist in the array which results inO(n^2) time.
    map<int , string> n; // this is to have the city list in the dists's order, hopefully.
   // map<string, map < string, int>> dists;

   int * connectivity;
};
