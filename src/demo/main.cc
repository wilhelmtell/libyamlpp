#include <iostream>

using namespace std;

int line_number = 1;

void ws() {
    while( true ) {
        if( ! cin ) return;
        if( cin.peek() == '\n' )
            ++line_number;
        else if( cin.peek() != ' ' && cin.peek() != '\t' )
            return;
        cin.get();
    }
}

int main(int argc, char* argv[])
{
    ws();
    char ch = cin.peek();
    cout << ch << line_number << endl;
    return 0;
}
