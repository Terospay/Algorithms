/*Given two lines s, t and an integer k. Consider the set of all such non-empty strings that occur as substrings in s
 * and t at the same time. Find the kth lexicographic line from this set.*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class SuffixArray {
public:
    SuffixArray(const std::string &s);

    std::vector<int> Get() const {
        return permutation;
    }

private:
    std::string string;
    size_t size;
    std::vector<int> permutation;
    std::vector<int> color;

    bool DifferentString(int i, int j, int current_sort_size) const;

};

SuffixArray::SuffixArray(const std::string &s) : string(s), size(string.size()) {
    // First step: k = 1
    color.resize(size);
    permutation.resize(size);
    for (int i = 0; i < size; ++i) {
        permutation[i] = i;
    }
    std::sort(permutation.begin(), permutation.end(),
              [this](int i, int j) { return this->string[i] < this->string[j]; });
    int current_color = 0;
    color[permutation[0]] = 0;
    for (int i = 1; i < size; ++i) {
        if (string[permutation[i]] != string[permutation[i - 1]])
            ++current_color;
        color[permutation[i]] = current_color;
    }
    // Transition
    int curr_sort_size = 1;
    std::vector<int> next_permutation(size);
    std::vector<int> next_color(size);
    std::vector<int> possition_for_count(size + 1);
    while (curr_sort_size < size) {
        possition_for_count.assign(size, 0);
        for (int i = 0; i < size; ++i) {
            ++possition_for_count[color[i] + 1];
        }
        for (int i = 1; i < size; ++i) {
            possition_for_count[i] += possition_for_count[i - 1];
        }
        for (int i = 0; i < size; ++i) {
            int first_part_poss = (permutation[i] - curr_sort_size + size) % size;
            next_permutation[possition_for_count[color[first_part_poss]]++] = first_part_poss;
        }
        current_color = 0;
        next_color[next_permutation[0]] = 0;
        for (int i = 1; i < size; ++i) {
            if (DifferentString(next_permutation[i], next_permutation[i - 1], curr_sort_size))
                ++current_color;
            next_color[next_permutation[i]] = current_color;
        }
        color = next_color;
        permutation = next_permutation;
        curr_sort_size *= 2;
    }


}

bool SuffixArray::DifferentString(int i, int j, int current_sort_size) const {
    return color[i] != color[j] || color[(i + current_sort_size) % size] != color[(j + current_sort_size) % size];
}

class LCP {
public:
    LCP(const std::string &s, const std::vector<int> &suffix_array);

    std::vector<int> Get() const {
        return lcp;
    }

private:
    std::string string;
    int size;
    std::vector<int> lcp;
    std::vector<int> suffix_array;

};

LCP::LCP(const std::string &s, const std::vector<int> &suffix_array) : string(s), size(string.size()),
                                                                       suffix_array(suffix_array) {
    lcp.assign(size, 0);
    std::vector<int> inverse_suffix_array(size);
    for (int i = 0; i < size; ++i) {
        inverse_suffix_array[suffix_array[i]] = i;
    }
    int k = 0;
    for (int i = 0; i < size; ++i) {
        if (k > 0)
            --k;
        if (inverse_suffix_array[i] == size - 1) {
            lcp[size - 1] = -1;
            k = 0;
            continue;
        } else {
            int j = suffix_array[inverse_suffix_array[i] + 1];
            while (std::max(i + k, j + k) < size && string[i + k] == string[j + k]) {
                ++k;
            }
            lcp[inverse_suffix_array[i]] = k;
        }
    }

}

std::string
K_common_substring(std::string &s, std::vector<int> &suffix_array, std::vector<int> &lcp, long long k) {
    int size = s.size();
    int min_lcp = 0;
    for (int i = 2; i < size - 1; ++i) {
        int curr_numb = suffix_array[i];
        int next_numb = suffix_array[i + 1];
        if ((curr_numb < suffix_array[0] && next_numb > suffix_array[0]) ||
            (curr_numb > suffix_array[0] && next_numb < suffix_array[0]) && lcp[i] > 0) {
            if (k <= lcp[i] - min_lcp) {
                std::string ans;
                for (int j = 0; j < min_lcp + k; ++j) {
                    ans += s[curr_numb + j];
                }
                return ans;
            } else {
                if (min_lcp <= lcp[i]) {
                    k += min_lcp - lcp[i];
                }
                min_lcp = lcp[i];

            }
        }
        if (min_lcp > lcp[i]) {
            min_lcp = lcp[i];
        }
    }
    return "-1";
}

int main() {
    std::string s;
    std::string t;
    long long k;
    std::cin >> s >> t >> k;
    s += '#' + t + '$';
    std::vector<int> suffix_array = SuffixArray(s).Get();
    std::vector<int> lcp = LCP(s, suffix_array).Get();
    std::string ans = K_common_substring(s, suffix_array, lcp, k);
    std::cout << ans;
}