/*The search pattern is given by a string of length m, in which, in addition to ordinary characters, “?”
 * Characters can occur. Find the positions of all occurrences of the template in text of length n. Each occurrence of
 * the template assumes that all ordinary characters coincide with the corresponding ones from the text, and instead of
 * the “?” Symbol, an arbitrary character occurs in the text.
The operating time is O (n + m + Z), where Z is the total number of occurrences of the substrings of the pattern
 “between questions” in the source text. m ≤ 5000, n ≤ 2,000,000.*/

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

using std::shared_ptr;
using std::weak_ptr;

struct Node {
    Node() : go(std::vector < weak_ptr < Node >> (26)) {};

    Node(Node &) = delete;

    Node(Node &&) = delete;

    Node &operator=(Node &) = delete;

    Node &operator=(Node &&) = delete;

    ~Node() = default;

    std::unordered_map<char, shared_ptr<Node>> son;
    std::vector <weak_ptr<Node>> go;
    weak_ptr <Node> parent;
    weak_ptr <Node> suffLin;
    weak_ptr <Node> up;
    weak_ptr <Node> root;
    char charToParent;
    bool terminal;
    std::vector<int> leafPatternNumber;
};


class AhoCorasick {
public:
    AhoCorasick(std::string &pattern, std::string &text);

    AhoCorasick(AhoCorasick &) = delete;

    AhoCorasick(AhoCorasick &&) = delete;

    AhoCorasick &operator=(AhoCorasick &) = delete;

    AhoCorasick &operator=(AhoCorasick &&) = delete;

    ~AhoCorasick() = default;

    void print();

    weak_ptr <Node> &get_link(weak_ptr <Node> &node, char symb);

    weak_ptr <Node> &get_up(weak_ptr <Node> &node);

    std::vector<int> patternMasklessPos;
    std::vector<int> auxiliary_vector;
    std::string text;
    shared_ptr <Node> root;

private:
    void add_pattern(std::string &pattern, int number);


    weak_ptr <Node> &get_suff_link(weak_ptr <Node> &node);


    //void delete_node(Node* node);

    std::vector <std::string> patternMasklessString;
    std::string pattern;
};

weak_ptr <Node> &AhoCorasick::get_suff_link(weak_ptr <Node> &node) {
    shared_ptr <Node> current_node = node.lock();
    if (current_node->suffLin.expired()) {
        if (current_node == root || current_node->parent.lock() == root) {
            current_node->suffLin = root;
        } else {
            current_node->suffLin = get_link(get_suff_link(current_node->parent), current_node->charToParent);
        }
    }
    return current_node->suffLin;
}

weak_ptr <Node> &AhoCorasick::get_link(weak_ptr <Node> &node, char symb) {
    char index = symb - 'a';
    shared_ptr <Node> current_node = node.lock();
    if (current_node->go[index].expired()) {
        if (current_node->son.find(symb) != current_node->son.end()) {
            current_node->go[index] = current_node->son[symb];
        } else if (current_node == root) {
            current_node->go[index] = root;
        } else {
            current_node->go[index] = get_link(get_suff_link(node), symb);
        }
    }
    return current_node->go[index];
}

weak_ptr <Node> &AhoCorasick::get_up(weak_ptr <Node> &node) {
    shared_ptr <Node> current_node = node.lock();
    if (current_node->up.expired()) {

        if (get_suff_link(node).lock() == current_node->root.lock()) {
            current_node->up = current_node->root;
        } else if (get_suff_link(node).lock()->terminal) {
            current_node->up = get_suff_link(node);
        } else {
            current_node->up = get_up(get_suff_link(node));
        }
    }
    return current_node->up;
}

void AhoCorasick::add_pattern(std::string &pattern, int number) {
    shared_ptr <Node> current_node = root;
    for (int i = 0; i < pattern.size(); i++) {
        char symb = pattern[i];
        if (current_node->son[symb] == nullptr) {
            current_node->son[symb] = std::make_shared<Node>();
            current_node->son[symb]->parent = current_node;
            current_node->son[symb]->charToParent = symb;
            current_node->son[symb]->terminal = false;
            current_node->son[symb]->root = current_node->root;
        }
        current_node = current_node->son[symb];
    }
    current_node->terminal = true;
    current_node->leafPatternNumber.push_back(number);
}

void work_with_text(AhoCorasick &aho_corasik) {
    aho_corasik.auxiliary_vector.resize(aho_corasik.text.size(), 0);
    weak_ptr <Node> current_node = aho_corasik.root;
    for (int i = 0; i < aho_corasik.text.size(); ++i) {
        char symb = aho_corasik.text[i];
        weak_ptr <Node> next = aho_corasik.get_link(current_node, symb);
        current_node = next;
        do {
            if (!current_node.lock()->leafPatternNumber.empty()) {
                for (auto s: current_node.lock()->leafPatternNumber) {
                    if (i >= aho_corasik.patternMasklessPos[s]) {
                        ++aho_corasik.auxiliary_vector[i - aho_corasik.patternMasklessPos[s]];
                    }
                }
            }
            current_node = aho_corasik.get_up(current_node);
        } while (current_node.lock() != aho_corasik.root);
        current_node = next;
    }
}

AhoCorasick::AhoCorasick(std::string &pattern, std::string &text) : pattern(pattern), text(text),
                                                                    root(std::make_shared<Node>()) {
    int count = 0;
    bool curr_pattern_process = false;
    for (size_t i = 0; i < pattern.size(); ++i) {
        if (curr_pattern_process) {
            if (pattern[i] == '?') {
                curr_pattern_process = false;
                ++count;
                patternMasklessPos.push_back(i - 1);
            } else {
                patternMasklessString[count] += pattern[i];
            }
        } else {
            if (pattern[i] != '?') {
                curr_pattern_process = true;
                patternMasklessString.push_back("");
                patternMasklessString[count] += pattern[i];
            }
        }
    }
    if (curr_pattern_process && pattern[pattern.size() - 1] != '?')
        patternMasklessPos.push_back(pattern.size() - 1);
    root->parent = root;
    root->root = root;
    for (int i = 0; i < patternMasklessString.size(); ++i) {
        add_pattern(patternMasklessString[i], i);
    }


}

int main() {
    std::string pattern, text;
    std::cin >> pattern >> text;
    AhoCorasick aho_corasik(pattern, text);
    work_with_text(aho_corasik);
    for (int i = 0; i < aho_corasik.auxiliary_vector.size(); ++i) {
        if (aho_corasik.auxiliary_vector[i] == aho_corasik.patternMasklessPos.size() &&
            i <= text.size() - pattern.size()) {
            std::cout << i << " ";
        }
    }
    return 0;
}