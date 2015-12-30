/*
Ali Atlı
21302442
Sec2
*/
#include "AirportNet.h"
#include <map>
#include <fstream>
#include <cstdlib>

AirportNet::AirportNet(const string aname)
{
//    fileName = aname;
    ifstream myFile(aname.c_str());
    string line("");

    if ( !myFile.is_open() )
    {
        cout << "Empty Net" << endl;
        numVertices = 0;

    }
    else
    {
        getline(myFile, line);
        int size = atoi( line.c_str() ); //get size from first line
        int count = 0;

        numVertices = 0;

        map<string, map < string, int> > dists;
        //this loop simply parses each line and generates the list of cities adn count.
        while ( count < size && getline(myFile, line) )
        {
            string temp = " ";
            int blankPos = line.find_first_of(" "); // get the origin
            temp = line.substr(0, blankPos);
            if (m.find(temp) == m.end()){
                n[numVertices] = temp;
                m[temp] = numVertices++;
            }
            line = line.substr(blankPos + 1);

            blankPos = line.find_first_of(" "); // get the destination
             string temp1 = line.substr(0, blankPos);
            if (m.find(temp1) == m.end()){
                n[numVertices] = temp1;
                m[temp1] = numVertices++;
            }
            line = line.substr(blankPos + 1);


            blankPos = line.find_first_of(" ");
            string temp2 = line.substr(0, blankPos);
            int distance =  atoi( temp2.c_str() );
            dists[temp][temp1]=distance;
            count++;

        }
        dist = new int*[numVertices];
        for(int i=0;i<numVertices;i++)
            dist[i] = new int[numVertices];
             // initializes the adjacency matrix
        for(int i=0;i<numVertices;i++)
            for(int j =0;j<numVertices;j++)
                if(i==j)
                    dist[i][j] = 0;
                else
                    dist[i][j] = 214748364;

        // creates the adjacency matrix


        for(map<string, map<string, int> >::iterator outer_iter=dists.begin(); outer_iter!=dists.end(); ++outer_iter) {
            for(map<string, int>::iterator inner_iter=outer_iter->second.begin(); inner_iter!=outer_iter->second.end(); ++inner_iter) {
                dist[m[outer_iter->first]][m[inner_iter->first]] = dists[outer_iter->first][inner_iter->first];
                dist[m[inner_iter->first]][m[outer_iter->first]] = dists[outer_iter->first][inner_iter->first];
            }
        }


    }
}

AirportNet::~AirportNet()
{
    //dtor
    if(connectivity != NULL)
        delete [] connectivity;
    if(dist != NULL){
        for(int i=0;i< numVertices;i++)
            delete [] dist[i];
        delete [] dist;
    }
}

void AirportNet::listRoutesOnHubAirport(const string aname){
    if(numVertices == 0)
        cout << "No city in this networks\n"<<endl;

    if(m[aname] == 0 && aname != n[0]){
        cout << endl;
        cout << aname <<" does not exist in the network." << endl;
        return;
    }

    int count = 0;
    for(int i=0;i<numVertices;i++)
        if(dist[m[aname]][i] < 214748364)
            count ++;

    //cout << count << endl;

    string * ar = new string[count];
    int index = 0;
    for(int i=0;i<numVertices;i++)
        if(dist[m[aname]][i] < 214748364)
            ar[index++] = n[i];


    cout <<"If " <<aname<<" is considered as hub these routes are possible:\n";
    for(int i=0;i<count;i++){
        for(int j=i+1;j<count;j++){
            if(/*iki şehir arasında mesafe sonsuz ise*/dist[m[ar[i]]][m[ar[j]]] == 214748364)
                cout << "<" << ar[i] << " , " << ar[j] << ">\n";
                // yazdır ikisini.
        }
    }
}

int AirportNet::root(int i){
    while (i != connectivity[i]) i = connectivity[i];
        return i;
}
bool AirportNet::connected(int p, int q){
   return root(q) == root(p);
}
void AirportNet::uni(int p, int q)
{
    int i = root(p);
    int j = root(q);
    connectivity[i] = j;
}

void AirportNet::findConnectedComponents(){
    if(numVertices == 0)
        cout << "No city in this networks\n"<<endl;
    connectivity = new int[numVertices];
    // initialize the connectivity array for ********************union-find algorithms***********************
    for(int i=0;i<numVertices;i++){
        connectivity[i] = i;
    }
    for(int i=0;i<numVertices;i++){
        for(int j=i;j<numVertices;j++){
            if(i==j)continue;
            if(dist[i][j] < 214748364)
                uni(i, j);
        }
    }

    int count = 0;
    for(int i=0;i<numVertices;i++)
        if(i==connectivity[i])
            count++;

    if(count == 1)
        cout << "Graph is connected with " << numVertices << " nodes."<< endl;


    else{
        int * ar = new int[numVertices];
        for(int i=0;i<numVertices;i++)
            ar[i] = 0;
        int index = 0;
        for(int i=0;i<numVertices;i++){
            ar[root(i)]++;
        }

        int ind = 0;
        for(int i=0;i<numVertices;i++){
            if(ar[i]>0){
                cout << "For connected component " << ++ind   << " there are " << ar[i] << " number of cities." << endl;
            }
        }
        delete [] ar;
    }

}


void AirportNet::listDirectFlights(const string aname){
    if(numVertices == 0)
        cout << "No city in this networks\n"<<endl;
    if(m[aname] == 0 && aname != n[0]){
        cout << aname << " does not exist in this airport." << endl;
        return;
    }

    cout << "Connected cities to: " << aname << endl;
    int index = m[aname];
    for(int i=0;i<numVertices;i++)
        if(dist[index][i]< 214748364 )
            cout << n[i] << " ";
    cout << "." << endl;
}
// changing floyd-warshall algorithm to reconstruct the path
// allows us to construct all shortest path from one node to other.
void AirportNet::displayMostCentralAirport(){
if(numVertices == 0)
        cout << "No city in this networks\n"<<endl;

   // initialization begins
    int ** floyd = new int*[numVertices];
    for(int i=0;i<numVertices;i++){
        floyd[i] = new int[numVertices];
    }

    int * c = new int[numVertices];
    for(int i=0;i<numVertices;i++)
        c[i] = 0;

     for(int i=0;i<numVertices;i++)
        for(int j=0;j<numVertices;j++)
            floyd[i][j] = dist[i][j];



    // initializations end


    // after this three nested for loop floyd will have the
    // length of the shortest paths from all cities to other cities
    // http://stackoverflow.com/questions/11370041/floyd-warshall-all-shortest-paths
    // see above link for the explanation.
    // a couple of extra info:
    // instead of strorin all number of shortest paths from one point to other.
    // this calculates the number of betweenness.
    for(int i=0;i<numVertices;i++){
        for(int j=0;j<numVertices;j++){
            for(int k=0;k<numVertices;k++){
                if (floyd[i][k] + floyd[k][j] < floyd[i][j] ){
                    floyd[i][j] = floyd[i][k] + floyd[k][j];
                    c[k]++;
                }
            }
        }
    }


    int minimum = 0;
    int minIndex = -1;
    for(int i=0;i<numVertices;i++){
        if(c[i]>minimum){
            minIndex = i;
            minimum = c[i];
        }
    }

    // the rest is to find the biggest betweenness centrality.
    cout << "Most cental airport in the network is: " << n[minIndex] <<endl;



}



