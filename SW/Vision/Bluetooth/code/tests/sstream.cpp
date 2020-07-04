#include <iostream>
#include <sstream>

using std::stringstream;
using std::cout;
using std::endl;

int main(void)
{
    stringstream ss;
    ss << "Arroz";
    ss << "Batata";
    std::string str;

    {
        ss >> str;
        cout << str << endl;
    }
}
