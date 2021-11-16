#include "Wave_finder.hpp"
#include <iostream>
using namespace std;

int main (int argc, char* argv[])
{
    try {

        char* str;
        
        for(int i = 1; i < argc; i++)
        {
            WayMatrix mat;
            str = argv[i];
            mat.get_data(str);
            finder a(mat);
            a.find();
            str = argv[i+1];
            a.file_output(str);
            i++;
            
        }
        cout<<" \nWays are found\n\n";
            
    }
    
    catch (const char* s){
        cout<<"\n";
        cout<<s;
        cout<<"\n";
    }
    
    
    return 0;
}
