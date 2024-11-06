# include "header.h"

int main(){
    while(true){
    string file_name = "";
    cout << "Choose what to do : " << endl;
    cout << "1) Load file using default name \"vole.txt\". "<< endl;
    cout << "2) Enter file name to load." << endl;
    cout << "3) Quit" << endl;
    string choice;
    getline(cin, choice);
    if(choice == "3"){
        cout << "thank you\n";
        break;}

    else if(choice == "1"){file_name = "vole.txt";}
    else if(choice == "2"){
        cin.clear();
        fflush(stdin);
        cout << "Enter file name : ";
        getline(cin, file_name);}

    else{
        cout << "Invalid input\n";
        continue;}

    cin.clear();
    fflush(stdin);
    Machine mac;
    Ui THE_UI;
    THE_UI.load_instructions(file_name, mac);
    mac.run_instructions();
    THE_UI.output(mac);}
}
