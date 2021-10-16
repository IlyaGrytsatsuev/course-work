#include "Wave_finder.hpp"
#include <iostream>
using namespace std;

int main ()
{
    char str[128];// = "/Users/gratchuvalsky/Desktop/input.txt";
    cout<<" Enter the path to the file:\n";
    gets(str);
    //cout<<str;
    
    WayMatrix mat(str);
    finder a(mat);
    a.find();
    //a.find_start_finish(mat);
    //mat.get(1,1);
    
    
    
}
