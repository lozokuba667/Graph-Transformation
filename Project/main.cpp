#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <numeric>

using namespace std;

bool checkIf1Graph(vector<vector<int>> test, int num) {
    int index;
    vector<int> singleVert;
    for (int i = 0; i < num; i++) {
        index = i;
        for (int j: test[i]) {
            singleVert.push_back(j);
            if (!singleVert.empty()) {
                sort(singleVert.begin(), singleVert.end());
                if (adjacent_find(singleVert.begin(), singleVert.end()) != singleVert.end()) {
                    return false;
                }
            }
        }
        singleVert.clear();
    }
    return true;
}

int makeTestSize(vector<vector<int>> giveSize) { ///funkcja do stworzenia rozmiaru analizowanego przy sprzężeniu (zakładamy że wierzchołki izolowane są na końcu)
    int testSize = giveSize.size();
    int isolatedCounters = 0;
    for (auto &i: giveSize) {
        for (int j: i) {
            if (j == 50) {
                isolatedCounters++;
            }
        }
    }
    testSize = testSize - isolatedCounters;
    return testSize; ///zwróć rozmiar badany
}

vector<int> findIntersection(vector<int> first, vector<int> second) { ///funkcja pomocnicza dla sprzężenia. Tworzy wektor części wspólnych
    vector<int> intersected;
    sort(first.begin(), first.end());
    sort(second.begin(), second.end());
    set_intersection(first.begin(), first.end(), second.begin(), second.end(), back_inserter(intersected));
    return intersected;
}

bool checkIfAdjoint(vector<vector<int>> ifAdjoint) { ///Sprawdź czy graf jest sprzężony
    int testedSize = makeTestSize(ifAdjoint); ///Patrz: makeTestSize()
    int vertex1;
    int vertex2;
    vector<int> storeNextFor1;
    vector<int> storeNextFor2;
    for (int i = 0; i < testedSize; i++) {
        for (int j = 0; j < testedSize; j++) {
            vertex1 = i;
            vertex2 = j;
            if (vertex1 == vertex2) {
                continue;
            }                                         /// Stwórz wszystkie możliwe pary do analizy
            for (int check1: ifAdjoint[vertex1]) {
                storeNextFor1.push_back(check1);
            }
            for (int check2: ifAdjoint[vertex2]) {
                storeNextFor2.push_back(check2);
            }
            vector<int> myIntersection = findIntersection(storeNextFor1, storeNextFor2); ///Patrz: findIntersection()
            if (!myIntersection.empty() && !storeNextFor1.empty() && !storeNextFor2.empty()) { ///Warunek sprzężenia: Jeśli zbiór bezpośrednich następników dwóch wierzchołków NIE jest rozłączny to zbiory muszą być równe
                if (storeNextFor2 != storeNextFor1) { ///Jeśli wektory przechowujące następniki nie są równe to znaczy że nie zaszła implikacja
                    return false;
                }
            }
            storeNextFor1.clear();
            storeNextFor2.clear(); ///Po zakończonej analizie pary -> wyczyść wektory następników
        }
    }
    return true;
}

bool isLinear(vector<vector<int>> prev, vector<vector<int>> next) {
    int testedSize = makeTestSize(prev); ///Patrz: makeTestSize()
    int vertex1;
    int vertex2;
    vector<int> storePrevFor1;
    vector<int> storePrevFor2;
    vector<int> storeNextFor1;
    vector<int>storeNextFor2;
    for (int i = 0; i < testedSize; i++) {
        for (int j = 0; j < testedSize; j++) {
            vertex1 = i;
            vertex2 = j;
            if (vertex1 == vertex2) {
                continue;
            }                                         /// Stwórz wszystkie możliwe pary do analizy
            for (int check1: prev[vertex1]) {
                storePrevFor1.push_back(check1);
            }
            for (int check2: prev[vertex2]) {
                storePrevFor2.push_back(check2);
            }
            for (int check3: next[vertex1]) {
                storeNextFor1.push_back(check3);
            }
            for (int check4: next[vertex2]) {
                storeNextFor2.push_back(check4);
            }
            vector<int> intersectNext = findIntersection(storeNextFor1,storeNextFor2);
            vector<int> intersectPrev = findIntersection(storePrevFor1, storePrevFor2);
            if (!intersectNext.empty() && !storeNextFor1.empty() && !storeNextFor2.empty()) { ///Warunek sprzężenia: Jeśli zbiór bezpośrednich następników dwóch wierzchołków NIE jest rozłączny to zbiory muszą być równe
                if (storeNextFor2 != storeNextFor1 || !intersectPrev.empty() ) {
                    return false;///Jeśli wektory przechowujące następniki nie są równe to znaczy że nie zaszła implikacja
                }
            }
            storePrevFor1.clear();
            storePrevFor2.clear();
            storeNextFor1.clear();
            storeNextFor2.clear();
        }
    }
    return true;
}

vector<vector<int>> previousVerts(vector<vector<int>> next, int verts) {
    vector<vector<int>> previous(verts);
    int index;
    int tempVert;
    for (int i = 0; i < verts; i++) {
        index = i;
        for (int j: next[i]) {
            if (j != 50) {
                tempVert = j;
                previous[tempVert].push_back(index);
            } else {
                previous[i].push_back(50);
            }
        }
    }
    return previous;
}

int getNumberOfVerticles() { ///Funkcja licząca ile wiechołków ma wczytywany graf (wierchołki nie posiadające następników NIE MOGĄ być na końcu)
    fstream file;
    file.open("graph.txt", ios::in | ios::out);
    string line;
    int numOfVert = 0;
    while (getline(file, line)) { ///Jako że mamy liste następników to wystarczy policzyć linie
        numOfVert++;
    }
    file.close();
    return numOfVert; ///zwróć ilość wierchołków
}

vector<vector<int>> fillVector(vector<vector<int>> myList) { ///funkcja do wypełnienia wektora (grafu w programie)
    int counter = 0;
    fstream filePartTwo;
    filePartTwo.open("graph.txt", ios::in | ios::out);
    string neuLine;

    while (getline(filePartTwo, neuLine)) {
        istringstream idk(neuLine); ///Stwórz zmienną strumienia tekstowego z pojedynczej lini
        myList[counter] = vector<int>(istream_iterator<int>(idk),
                                      istream_iterator<int>()); /// Każdą pojedynczą linie potraktuj jako wektor iterujący od początku lini aż do końca
        counter++;
    }
    filePartTwo.close();
    return myList; ///zwróć wektor wektorów (nasz graf) --> [i][j] gdzie i to wierchołek a j to następniki
}

void printTest(vector<vector<int>> testIt) { ///Zaprezentuj graf w konsoli
    for (int i = 0; i < testIt.size(); i++) {
        cout << i << " ---> ";
        for (int j: testIt[i]) {
            cout << j << " ---> ";
        }
        cout << "\n";
    }
}

void printPair (vector<pair<int,int>> printMe) {
    for (int i =0; i<printMe.size(); i++)  {
        cout << "Verticle in Adjoint: " << i  << " ---> " "(" << printMe[i].first << "," << printMe[i].second << ")";
        cout << "\n";
    }
}

int remIsolated(vector<vector<int>>vec, int size) {
    int isolatedIndex = 120;
    for (int i=0; i<size; i++) {
        for (int j: vec[i]) {
            if (j == 50) {
                isolatedIndex = i;
                return isolatedIndex;
            }
        }
    }
    return isolatedIndex;
}
void correction(vector<pair<int,int>> &pairMe, vector<vector<int>>next, int count){
    vector<int>compInd;
    vector<int>nextVal;
    for (int i=0;i<count;i++){
        compInd.push_back(i);
        for (int j: next[i]) {
            nextVal.push_back(j);
        }
        for (int c=i+1; c<count;c++) {
            if (next[c] == nextVal) {
                compInd.push_back(c);
                for (int currentInd:compInd) {
                    for (int CurrentVal: nextVal) {
                        if (pairMe[currentInd].second != pairMe[CurrentVal].first && pairMe[currentInd].second != pairMe[currentInd+1].second) {
                            pairMe[currentInd].second = pairMe[CurrentVal].first;
                        }
                    }
                }
            }
        }
        compInd.clear();
        nextVal.clear();
    }
}
void pairPairs(vector<pair<int,int>> &pairMe, vector<vector<int>>next, int count) {
    int indexInAd;
    int tempValue;
    vector<int>usedVert(count);
    for (int i = 0; i < count; i++) {
        indexInAd = i;
        for (int d: next[i]) {
            if (d != 50) {
                tempValue = d;
                pairMe[tempValue].first = pairMe[indexInAd].second;
            }
        }
        usedVert.push_back(i);
    }
    correction(pairMe,next,count);
}
//void corInd (vector<pair<int,int>> &re, int num,int isolated) {
//    int reCount = 1;
//    int max=1;
//    if (isolated != 120) {
//        num = num-1;
//    }
//    for (int i=0; i<num; i++) {
//        if (re[i].second > max) {
//            int rem = re[i].second;
//            re[i].second = max+1;
//            for (int j=i+1; j<num; j++) {
//                if (re[j].first == rem) {
//                    re[j].first = max+1;
//                } else if (re[j].second == rem) {
//                    re[j].second = max+1;
//                }
//            }
//            max = max+1;
//        }
//        if (re[i].second > max) {
//            max = re[i].second;
//        }
//    }
//    if (isolated !=120) {
//        re[isolated].first = reCount-1;
//        re[isolated].second = reCount;
//    }
//}
//void reIndex(vector<pair<int,int>> &re, int num,int isolated) {
//    int reCount = 1;
//    vector<int> usedValues;
//    if (isolated != 120) {
//        num = num-1;
//    }
//    for (int i=0; i<num; i++) {
//        if (re[i].second != reCount && reCount < re[i].second && reCount<=num) {
//            int rem = re[i].second;
//            re[i].second = reCount;
//            for (int j=i+1; j<num; j++) {
//                if (re[j].first == rem) {
//                    re[j].first = reCount;
//                } else if (re[j].second == rem) {
//                    re[j].second = reCount;
//                }
//            }
//            reCount++;
//        } else {
//            reCount++;
//        }
//
//    }
//    if (isolated !=120) {
//        re[isolated].first = reCount-1;
//        re[isolated].second = reCount;
//    }
//    corInd(re,num,isolated);
//}

void indexRe(vector<pair<int,int>> &re, int num) {
    vector<int>usedValues;
    int value = 0;
    int rem;

    for (int i=0; i<num; i++) {
        if (re[i].first > value) {
            rem = re[i].first;
            re[i].first = value;
            for (int j=i+1; j<num; j++) {
                if (re[i].second == rem) {
                    re[i].second = value;
                }
                if (re[j].first == rem) {
                    re[j].first = value;
                } if (re[j].second == rem) {
                    re[j].second = value;
                }
            }
            usedValues.push_back(value);
            value++;
        } else if (find(usedValues.begin(),usedValues.end(),re[i].first) != usedValues.end()){
            value = value;
        } else {
            value++;
            usedValues.push_back(re[i].first);
        }
        if (re[i].second > value) {
            rem = re[i].second;
            re[i].second = value;
            for (int j=i+1; j<num; j++) {
                if (re[j].first == rem) {
                    re[j].first = value;
                }
                if (re[j].second == rem) {
                    re[j].second = value;
                }
            }
            usedValues.push_back(value);
            value++;
        } else if (find(usedValues.begin(),usedValues.end(),re[i].second) != usedValues.end()){
            value = value;
        } else {
            value++;
            usedValues.push_back(re[i].second);
        }
    }
}


vector<pair<int,int>> makePair(vector<vector<int>> alpha, int alphavert) {
    vector<pair<int, int>> setOfEdges(alphavert);
    pair<int, int> singleEdge;
    int counter = 0;
    for (int i = 0; i < alphavert; i++) { ///stwórz rozłączne krawędzie (przed przeindeksowaniem)
        singleEdge = make_pair(counter, counter + 1);
        setOfEdges[i] = singleEdge;
        counter = counter + 2;
    }
    cout << "Pairs before pairing:" << endl;
    printPair(setOfEdges);
    cout << "\n";
    int isolatedIn = remIsolated(alpha,alphavert);
    if (isolatedIn != 120) {
        cout << "Isolated vert is: " << isolatedIn << endl;
    } else {
        cout << "Graph does not contain isolated vertex" << endl;
    }
    cout << "\n";
    pairPairs(setOfEdges,std::move(alpha),alphavert);
    cout << "Pairs before reindexing: " << endl;
    printPair(setOfEdges);
    cout << "\n";
    indexRe(setOfEdges,alphavert);
    cout << "Pairs after reindexing" << endl;
    printPair(setOfEdges);
    return setOfEdges;
}

int possibleSize(vector<pair<int,int>> pairs, int pairsize) {
    int size;
    vector<int>allValues;
    for (int i=0; i<pairsize; i++) {
        allValues.push_back(pairs[i].first);
        allValues.push_back(pairs[i].second);
    }
    size = *max_element(allValues.begin(),allValues.end());
    return size+1;
}

vector<vector<int>>createOriginal(vector<pair<int,int>> pair, int psize,vector<vector<int>>original) {
    for (int i=0; i<psize; i++) {
        original[pair[i].first].push_back(pair[i].second);
    }
    return original;
}

void saveToFile(vector<vector<int>> ourGraph) {
    fstream file;
    file.open("newGraph.txt", ios::out);
    ostream_iterator<int> myIter (file, " ");
    for(const auto& vt : ourGraph) {
        copy(vt.cbegin(), vt.cend(),myIter);
        file << "\n";
    }
}
int main() {
    int numOfVert = getNumberOfVerticles();
    vector<vector<int>> myList(numOfVert);
    vector<vector<int>> previousList(numOfVert);

    myList = fillVector(myList);
    previousList = previousVerts(myList, numOfVert);
    cout << "Normal list:" << endl;
    printTest(myList);
    cout << "\n";
    cout << "Prev list:" << endl;
    printTest(previousList);
    if (checkIf1Graph(myList, numOfVert)) {
        cout << "Graph is 1-Graph ";
        if (checkIfAdjoint(myList)) {
            cout << "and Adjoint ";
            if (isLinear(previousList,myList)) {
                cout << "and Linear" << endl;
            } else {
                cout << "but NOT Linear" << endl;
            }
        } else {
            cout << "but NOT Adjoint" << endl;
        }
    } else {
        cout << "Graph is NOT 1-Graph!" << endl;
    }
    cout << "\n";
    if (checkIf1Graph(myList,numOfVert) && checkIfAdjoint(myList)) {
        vector<pair<int,int>>graphAsPair(numOfVert);
        graphAsPair = makePair(myList,numOfVert);
        int originalSize = possibleSize(graphAsPair,numOfVert);
        vector<vector<int>>originalGraph(originalSize);
        originalGraph = createOriginal(graphAsPair,numOfVert,originalGraph);
        cout << "\n";
        cout << "Original Graph (after transformation): " << endl;
        printTest(originalGraph);
        cout << "\n";
        saveToFile(originalGraph);
        cout << "Saving to file completed";
    } else {
        cout << "Graph is not qualified to be transformed to original form";
    }
    return 0;
}

