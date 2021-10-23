#include "Wave_finder.hpp"
#include <iostream>
using namespace std;

int main ()
{
    try {
    
    // /Users/gratchuvalsky/Desktop/labir1.txt
      //  /Users/gratchuvalsky/Desktop/output.txt
    char str[128];// = "/Users/gratchuvalsky/Desktop/input.txt";
    cout<<" Enter the path to the file:\n";
    gets(str);
    //cout<<str;
    
    WayMatrix mat;
    mat.get_data(str);
    finder a(mat);
    a.find();
    cout<<" Enter the path to the file:\n";
    gets(str);
    a.file_output(str);
    //a.find_start_finish(mat);
    //mat.get(1,1);
    }
    
    catch (const char* s){
        cout<<s;
        cout<<"\n";
    }
    
    
    
}
