#include <yaml++.hh>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

template<typename T>
string dump(T t)
{
    cerr << "FATAL:Don't call the generic template." << endl;
    exit(-1);
}

template<>
string dump<string>(string str)
{
    return str;
}

template<>
string dump<vector<string> >(vector<string> vec)
{
    string result;
    for( vector<string>::const_iterator iter(vec.begin());
        iter != vec.end();
        ++iter ) {
        result += "- ";
        result += dump(*iter);
        result += "\n";
    }
    return result;
}

int main(int argc, char* argv[])
{
    vector<string> v;
    v.push_back("a");
    v.push_back("b");
    v.push_back("c");
    cout << dump(v) << endl;

    return 0;
}
