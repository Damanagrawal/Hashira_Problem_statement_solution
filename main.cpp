#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
using json = nlohmann::json;
using boost::multiprecision::cpp_int;

// Convert base-N string to cpp_int
cpp_int convertToDecimal(const string& numStr, int base) {
    cpp_int result = 0;
    for (char c : numStr) {
        int digit;
        if (isdigit(c)) digit = c - '0';
        else if (isalpha(c)) digit = tolower(c) - 'a' + 10;
        else throw runtime_error("Invalid character in number");
        if (digit >= base) throw runtime_error("Digit out of range for base");
        result = result * base + digit;
    }
    return result;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <input.json>" << endl;
        return 1;
    }

    ifstream file(argv[1]);
    if (!file.is_open()) {
        cerr << "Error opening file: " << argv[1] << endl;
        return 1;
    }

    json j;
    file >> j;

    int n = j["keys"]["n"];
    int k = j["keys"]["k"];

    cpp_int product = 1;

    // Only first k roots matter
    for (int i = 1; i <= k; i++) {
        int base = stoi(j[to_string(i)]["base"].get<string>());
        string value = j[to_string(i)]["value"].get<string>();
        cpp_int root = convertToDecimal(value, base);
        product *= root;
    }

    // Apply (-1)^k
    if (k % 2 != 0) product = -product;

    cout << product << endl;

    return 0;
}
