#include <iostream>

using namespace std;

int line_number = 1;
char peek = ' ';

void ws() {
    while( true ) {
        if( peek == '\n' )
            ++line_number;
        else if( peek != ' ' && peek != '\t' )
            return;
        peek = cin.get();
        if( ! cin ) return;
    }
}

int main(int argc, char* argv[])
{
    ws();
    char ch = peek;
    cout << ch << line_number << endl;
    return 0;
}
