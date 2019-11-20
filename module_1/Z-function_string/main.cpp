/*Find the lexicographically minimal string constructed by the z-function in the alphabet a-z.*/

#include <iostream>
#include <string>
#include <vector>

void string_construction(std::string &ans, int n) {
    static int already_built = 1;
    static int already_read = 1;
    static std::vector<int> ban_on_symb(26, 0);
    ++already_read;
    if (n != 0) {
        for (; n > already_built - already_read + 1; ++already_built) {
            ans += char(ans[already_built - already_read + 1]);
        }
        ban_on_symb[ans[n] - char('a')] = n + 1;
    } else {
        if (already_read > already_built) {
            for (int i = 1; i < 26; i++) {
                if (ban_on_symb[i] != 1) {
                    ans += char('a' + i);
                    ++already_built;
                    break;
                }
            }
        }
    }
    for (int i = 0; i < 26; ++i) {
        if (ban_on_symb[i] != 0) {
            --ban_on_symb[i];
        }
    }
}

int main() {
    int n;
    std::string ans = "a";
    std::cin >> n;
    while (std::cin >> n)
        string_construction(ans, n);
    std::cout << ans;
}