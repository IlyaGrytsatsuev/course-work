#include "Wave_finder.hpp"
#include <iostream>
using namespace std;

int main ()
{
    try {
    // FILES:
        
    // /Users/gratchuvalsky/Desktop/mat1.txt
      //  /Users/gratchuvalsky/Desktop/output1.txt
        
       // /Users/gratchuvalsky/Desktop/mat2.txt
        // /Users/gratchuvalsky/Desktop/output2.txt
        
        // /Users/gratchuvalsky/Desktop/mat3.txt
        // /Users/gratchuvalsky/Desktop/output3.txt
        
        
        char str[128];
        cout<<" Enter the path to the file for downloading:\n";
        fgets(str, 127, stdin);

        WayMatrix mat;
        mat.get_data(str);
        finder a(mat);
        a.find();
        cout<<" \nWay is found\n\n";
            
        cout<<" Enter the path to the file for uploading:\n";
        fgets(str, 127, stdin);
        
        a.file_output(str);
        cout<<"\nUpload is success !\n\n";
    }
    
    catch (const char* s){
        cout<<"\n";
        cout<<s;
        cout<<"\n";
    }
    
    
    return 0;
}
