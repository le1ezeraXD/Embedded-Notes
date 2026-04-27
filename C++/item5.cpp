#include<iostream>
#include<vector>
#include<unordered_map>

using namespace std;

int main() {
    unordered_map<string,int> m = {
        {"1",1},
        {"2",2},
        {"3",3},
    };
    // The pair in unordered_map is const
    // Such as std::pair<const std:: string, int>
    // So if used "const std::pair<std::string, int>& p : m"
    // IDE has to change the pair which in unordered_map
    // To the type what u designed (const std::pair<std::string, int>&)"
    for (const auto& p : m) {
        cout << p.first << endl;
    }
}