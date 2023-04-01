#include <string>
#include <iostream>
#include <fstream>

using namespace std;

string new_HWID;
string current_HWID;

void spoofHWID(string new_hwid)
{
    new_HWID = new_hwid;
    current_HWID = new_HWID;
}

int main()
{
    string input_HWID;

    cout << "Enter the new HWID: ";
    cin >> input_HWID;

    spoofHWID(input_HWID);

    cout << "New HWID is: " << current_HWID << endl;

    ofstream tempFile("temp.txt"); 
    system("WMIC CSProduct Get UUID >> temp.txt");
    tempFile.close(); 

    ifstream tempFileRead("temp.txt");
    ofstream currentFile("csp_sig_current.txt");
    string line;
    while (getline(tempFileRead, line)) {
        if (line.find("UUID") != string::npos) {
            currentFile << line.substr(line.find_first_of("0123456789ABCDEF")) << endl; 
            break;
        }
    }
    tempFileRead.close();
    currentFile.close();

    ofstream newFile("csp_sig_new.txt");
    newFile << new_HWID << endl;
    newFile.close();

    system("copy /b csp_sig_new.txt + csp_sig_current.txt csp_sig_fused.txt"); 
    system("WMIC CSProduct where name=\"%%\" call Identify csp_sig_fused.txt"); 
    system("del temp.txt");
    system("del csp_sig_new.txt");
    system("del csp_sig_current.txt");
    system("del csp_sig_fused.txt");

    return 0;
}
