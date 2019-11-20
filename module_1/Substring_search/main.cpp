/*Find all occurrences of the pattern in a string. The length of the pattern is p, the length of the string is n.
 * Time O (n + p), add. memory is O (p).
p <= 30000, n <= 300000.
Use one of the methods:
- Using the prefix function;
- Using the z-function.*/

#include <iostream>
#include <string>
#include <vector>


const std::vector<int> prefix_function_create(std::string &string) {
    size_t n = string.length();
    std::vector<int> prefix_function(n);
    prefix_function[0] = 0;
    for (int i = 1; i < n; ++i) {
        int j = prefix_function[i - 1];
        while (j > 0 && string[i] != string[j])
            j = prefix_function[j - 1];
        if (string[i] == string[j]) ++j;
        prefix_function[i] = j;
    }
    return prefix_function;

}


void search_for_substring_in_string(std::vector<int> &entries, std::string &substring, char symbol, int &number,
                                    const std::vector<int> &prefix_function) {
    size_t n = substring.length() - 1;
    static int prev_val = 0;
    int j = prev_val;
    while (j > 0 && symbol != substring[j]) {
        j = prefix_function[j - 1];
    }
    j = (symbol == substring[j] ? j + 1 : 0);
    if (j == n)
        entries.push_back(1 + number - n);
    prev_val = j;
    ++number;
}


int main() {
    std::string substring;
    int ans = 0;
    std::cin >> substring;
    substring += "#";
    size_t n = substring.length();
    const std::vector<int> prefix_function = prefix_function_create(substring);
    std::vector<int> entries;
    char symbol;
    int number = 0;
    while (std::cin >> symbol)
        search_for_substring_in_string(entries, substring, symbol, number, prefix_function);
    for (int i:entries)
        std::cout << i << " ";
    return 0;
}

