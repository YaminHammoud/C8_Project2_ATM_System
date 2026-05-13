#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include <fstream>

using namespace std;

const string ClientsFileName = "Clients.txt" ;

enum enMainMenuOptions
{
    eQuickWithdraw = 1,
    eNormalWithdraw = 2,
    eDeposit = 3,
    eCheckBalance = 4,
    eLogout = 5
}; 

struct stClient
{
    string AccountNumber ; 
    string PinCode ; 
    string Name ; 
    string Phone ; 
    double AccountBalance ; 
    bool IsMarkedForDelete = false ; 
};

void Login() ; 
void ShowMainMenu() ; 
void ShowQuickWithdrawScreen();
void ShowNormalWithdrawScreen();

stClient CurrentClient ; 

char ReadChar (string Message)
{
    char c = 'n' ; 
    cout << Message ;
    cin >> c;
    return c;
}

vector<string> SplitString(string S1, const string &Delim)
{
    vector<string> vString;
    size_t pos = 0;
    string sWord;

    while ((pos = S1.find(Delim)) != string::npos)
    {
        sWord = S1.substr(0, pos);
        if (!sWord.empty())
        {
            vString.push_back(sWord);
        }
        S1.erase(0, pos + Delim.length());
    }

    if (!S1.empty())
    {
        vString.push_back(S1);
    }

    return vString;
}

stClient ConvertLineToRecord (string Line , string Seperator = "#//#")
{
    vector<string> vClients = SplitString(Line,Seperator) ; 

    stClient Client ;

    Client.AccountNumber = vClients[0];
    Client.PinCode = vClients[1];
    Client.Name = vClients[2];
    Client.Phone = vClients[3];
    Client.AccountBalance = stod(vClients[4]);

    return Client ; 
}

vector<stClient> LoadDataFromFile(string FileName)
{
    fstream MyFile ; 
    MyFile.open(FileName,ios::in) ; 

    stClient Client ; 
    string Line; 
    vector<stClient> vClients ;

    if (MyFile.is_open())
    {
        while (getline(MyFile,Line))
        {
            Client = ConvertLineToRecord(Line) ; 
            vClients.push_back(Client) ; 
        }

        MyFile.close()  ;

    }

    return vClients ; 

}

void GoBackToMainMenu ()
{
    cout << "\nPlease enter any key to go back to main menu ........";
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');//clear the input buffer
    cin.get();//wait for user input 
    ShowMainMenu() ; 
}

bool FindClientByAccountNumberAndPin(string AccountNumber ,string PinCode , stClient & Client)
{
    vector<stClient> vClients = LoadDataFromFile(ClientsFileName) ; 

    for (stClient & C : vClients)
    {
        if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
        {
            Client = C ;
            return true; 
        }
    }

    return false ; 
}

bool LoadClientInfo(string AccountNumber, string PinCode)
{
    if (FindClientByAccountNumberAndPin(AccountNumber,PinCode,CurrentClient))
        return true;
    
    else
        return false;
}

enMainMenuOptions ReadMainMenuOption ()
{
    short c = 0 ;
    cout <<  "Choose what do you want to do? [1 to 5]? " ; 
    cin >> c ; 

    return enMainMenuOptions(c) ; 
}

int GetQuickWithdrawAmount(short UserChoice)
{
    switch (UserChoice)
    {
        case 1 : 
        return 20 ; 

        case 2 : 
        return 50; 

        case 3 : 
        return 100 ;

        case 4 : 
        return 200 ; 

        case 5 : 
        return 400 ;

        case 6 : 
        return 600 ; 

        case 7 : 
        return 8000 ; 

        case 8 : 
        return 1000  ;

        default:
        return 0; 
    }
}

short ReadQuickWithdrawOption ()
{
    short c = 0 ;
    
    do 
    {
        cout << "\nChoose what to do from [1] to [9]? ";
        cin >> c;
        
    }while (c < 1 || c > 9); 

    return c ; 
}

string ConvertRecordToLine (stClient Client , string Seperator = "#//#")
{
    string sLine = "" ; 
    
    sLine += Client.AccountNumber + Seperator ;
    sLine += Client.PinCode + Seperator; 
    sLine += Client.Name + Seperator;
    sLine += Client.Phone + Seperator ;
    sLine +=  to_string(Client.AccountBalance) ;

    return sLine ; 
    
}

void SaveVectorToFile (string FileName ,vector<stClient> &vClients)
{
    fstream MyFile ;

    MyFile.open(FileName, ios::out) ; 

    if (MyFile.is_open())
    {
        for (  stClient & C : vClients)
        {
            if (C.IsMarkedForDelete == false )
            {
                MyFile << ConvertRecordToLine(C) << endl ;
            }
        }
    }
}

int ReadNormalWithdrawAmount()
{
    int amount = 0 ;
    do 
    {
        cout << "\nEnter an amount multiple of 5's ? " ; 
        cin >> amount; 
    }while (amount % 5 != 0 ) ;

    return amount ;
}

void DepositBalanceToClientByAccountNumber(string AccountNumber , double DepositAmount , vector <stClient> & vClients)
{

    for (stClient &C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C.AccountBalance += DepositAmount;
            SaveVectorToFile(ClientsFileName, vClients);
            cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance << endl;
            break;
        }
    }
}

void PerformQuickWithdrawOption (short QuickWithdrawOption)
{
    if (QuickWithdrawOption == 9)
        return;

    short WithdrawAmount = GetQuickWithdrawAmount(QuickWithdrawOption); 

    if (WithdrawAmount > CurrentClient.AccountBalance)
    {
        cout << "\nThe amount exceeds your balance, make another choice.\n"; 
        cout << "Press any key to continue........... " ;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear the input buffer
        cin.get(); // wait for user input
        system("clear");
        ShowQuickWithdrawScreen();    
    }

    char answer = ReadChar("\nAre you sure you want make this transaction? y/n? ");

    if (toupper(answer ==  'Y'))
    {
        vector<stClient> vClients = LoadDataFromFile(ClientsFileName);
        DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithdrawAmount * -1, vClients);
        CurrentClient.AccountBalance -= WithdrawAmount;
    }
    
    else
        cout << "\n\nTransaction Canceled.\n";
    
}

void ShowQuickWithdrawScreen()
{
    cout << "=================================================\n";
    cout << "\t\tQuick Withdraw\n";
    cout << "=================================================\n";
    cout << "\t[1] 20\t\t" << "[2] 50\n" ;
    cout << "\t[3] 100\t\t" << "[4] 200\n" ;
    cout << "\t[5] 400\t\t" << "[6] 600\n" ; 
    cout << "\t[7] 800\t\t" << "[8] 1000\n" ;
    cout << "\t[9] Exit\n" ;
    cout << "=================================================\n" ; 
    cout << "You Balance is " << CurrentClient.AccountBalance << " $\n" ;    
    
   
    PerformQuickWithdrawOption(ReadQuickWithdrawOption()) ; 
    
}

void PerformNormalWithdraw( int WithDrawAmount)
{
    if (WithDrawAmount > CurrentClient.AccountBalance)
    {

        cout << "\nThe amount exceeds your balance.\n";
        cout << "Press any key to continue........... ";
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear the input buffer
        cin.get();                                                     // wait for user input
        system("clear");
        ShowNormalWithdrawScreen();
    }

    char answer = ReadChar("\nAre you sure you want make this transaction? y/n? ");

    if (toupper(answer == 'Y'))
    {
        vector<stClient> vClients = LoadDataFromFile(ClientsFileName);
        DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithDrawAmount * -1, vClients);
        CurrentClient.AccountBalance -= WithDrawAmount;
    }

    else
        cout << "\n\nTransaction Canceled.\n";
 
}

void ShowNormalWithdrawScreen() 
{
    cout << "=================================================\n";
    cout << "\t\tNormal Withdraw Screen\n";
    cout << "=================================================\n";

    PerformNormalWithdraw(ReadNormalWithdrawAmount());
}

double ReadDepositAmount ()
{
    double amount = 0 ;
    do 
    {
        cout << "\nEnter a positive deposit amount? " ;
        cin >> amount;
    }while (amount <= 0); 

    return amount ; 
}

void PerformDeposit(double DepositAmount)
{
    char answer = ReadChar("\nAre you sure you want make this transaction? y/n? ");

    if (toupper(answer == 'Y'))
    {
        vector<stClient> vClients = LoadDataFromFile(ClientsFileName);
        DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, DepositAmount, vClients);
        CurrentClient.AccountBalance += DepositAmount;
    }
    
    else
        cout << "\n\nTransaction Canceled.\n";
}

void ShowDepositScreen()
{
    cout << "=================================================\n";
    cout << "\t\t  Deposit Screen\n";
    cout << "=================================================\n";
    
    PerformDeposit(ReadDepositAmount());

    
}

void ShowCheckBalanceScreen()
{
    cout << "=================================================\n";
    cout << "\t\tCheck Balance Screen\n";
    cout << "=================================================\n";
    cout << "Your Balance is: " << CurrentClient.AccountBalance <<"\n"; 
}

void PerformMainMenuOption(enMainMenuOptions MainMenuOption)
{
    switch (MainMenuOption)
    {
    case enMainMenuOptions::eQuickWithdraw:
        system("clear");
        ShowQuickWithdrawScreen();
        GoBackToMainMenu();
        break;

    case enMainMenuOptions::eNormalWithdraw:
        system("clear");
        ShowNormalWithdrawScreen();
        GoBackToMainMenu();
        break;

    case enMainMenuOptions::eDeposit:
        system("clear");
        ShowDepositScreen();
        GoBackToMainMenu();
        break;

    case enMainMenuOptions::eCheckBalance:
        system("clear");
        ShowCheckBalanceScreen();
        GoBackToMainMenu();
        break;

    case enMainMenuOptions::eLogout:
        system("clear");
        Login();
    }

}

void ShowMainMenu()
{
    system("clear") ;

    cout << "=================================================\n" ; 
    cout << "\t\tATM Main Menu Screen\n" ; 
    cout << "=================================================\n" ; 
    cout << "\t[1] Quick Withdraw.\n" ; 
    cout << "\t[2] Normal Withdraw.\n" ; 
    cout << "\t[3] Deposit.\n" ;
    cout << "\t[4] Check Balance.\n" ; 
    cout << "\t[5] Logout.\n" ; 
    cout << "=================================================\n" ;

    PerformMainMenuOption(ReadMainMenuOption());
}

void Login()
{
    bool LoginFailed = false ; 
    string AccountNumber , PinCode ; 
    do 
    {
        system ("clear") ; 
        cout << "---------------------------------------------\n" ;
        cout << "\t\tLogin Screen\n" ;
        cout << "---------------------------------------------\n";

        if (LoginFailed)
            cout << "Invalid Account Number/PinCode!\n";

        cout << "Enter Account Number? " ;
        cin >> AccountNumber ;
        cout << "Enter Pin? " ; 
        cin >> PinCode ; 

        LoginFailed = !LoadClientInfo(AccountNumber,PinCode) ;
    }while (LoginFailed) ; 

     ShowMainMenu() ; 

}

int main() 
{
    Login();

    

    return 0;
}