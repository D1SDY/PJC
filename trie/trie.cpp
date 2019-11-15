#include "trie.hpp"

#include <utility>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <stack>

using namespace std;

void deleteTrie(trie_node *pNode) {
    if (pNode->children) {
        for (int i = 0; i < 128; ++i) {
            if (pNode->children[i]) {
                deleteTrie(pNode->children[i]);
            }
        }
    }
    if (pNode != nullptr) {
        delete pNode;
    }
}

vector<string> gelAllWords(vector<string> vector, trie_node *pNode, string basicString) {
    if (pNode->is_terminal) {
        vector.push_back(basicString);
    }
    for (int i = 0; i < 128; i++) {
        if (pNode->children[i] != nullptr) {
            trie_node *current = pNode->children[i];
            basicString.push_back(current->payload);
            vector = gelAllWords(vector, current, basicString);
            basicString.pop_back();
        }
    }

    return vector;
}

trie::trie(const vector<string> &strings) {
    m_root = new trie_node;
    for (int i = 0; i < strings.size(); i++) {
        insert(strings[i]);
    }
}

trie::trie() {
    m_root = new trie_node();
}

trie::~trie() {
    trie_node *current = m_root;
    deleteTrie(current);
    m_root = new trie_node();
    m_size = 0;
}

trie &trie::operator=(const trie &rhs) {
    vector<string> words = rhs.search_by_prefix("");
    if (rhs.m_root->payload == ' ') {
        words.push_back("");
    }
    deleteTrie(m_root);
    m_root = new trie_node();
    m_size = 0;
    for (string ele:words) {
        this->insert(ele);
    }
    return *this;
}

trie::trie(trie &&rhs) {
    m_root = new trie_node();
    m_size = 0;
    vector<string> words = rhs.search_by_prefix("");
    if (rhs.m_root->payload == ' ') {
        words.push_back("");
    }
    for (string ele:words) {
        insert(ele);
    }
}

trie::trie(const trie &rhs) {
    vector<string> words = rhs.search_by_prefix("");

    if (rhs.m_root->payload == ' ') {
        words.push_back("");
    }
    m_root = new trie_node();
    m_size = 0;
    for (auto value : words) {
        insert(value);
    }
}

trie &trie::operator=(trie &&rhs) {
    vector<string> words = rhs.search_by_prefix("");
    if (rhs.m_root->payload == ' ') {
        words.push_back("");
    }
    deleteTrie(m_root);
    m_root = new trie_node();
    m_size = 0;
    for (string ele:words) {
        this->insert(ele);
    }
    return *this;
}

bool trie::erase(const string &str) {
    if (contains(str)) {
        int len = str.length();
        trie_node *current = m_root;
        if (len < 1) {
            len = 1;
        }
        for (int i = 0; i < len; i++) {
            for (int j = 0; j < 128; j++) {
                if (current->children[j]) {
                    if (current->children[j]->payload == str[i]) {
                        current = current->children[j];
                        break;
                    }
                }
            }
        }
        if (current->is_terminal == true) {
            bool isEmpty = false;
            while (true) {
                for (int i = 0; i < 128; i++) {
                    if (current->children[i] != nullptr) {
                        isEmpty = false;
                        break;
                    } else {
                        isEmpty = true;
                    }
                }
                if (isEmpty) {
                    trie_node *child = current;
                    for (int i = 0; i < 128; i++) {
                        if (current->parent->children[i] == current) {
                            current = current->parent;
                            current->children[i] = nullptr;
                            break;
                        }
                    }
                    delete child;
                } else {
                    current->is_terminal = false;
                    break;
                }
            }
        } else {
            current->is_terminal = false;
        }
        m_size--;
        return true;
    }

    return false;
}

bool trie::insert(const string &str) {
    if (contains(str) == true) {
        return false;
    } else {
        trie_node *current = m_root;
        int len = str.length();
        if (len < 1) {
            len = 1;
        }
        for (int i = 0; i < len; i++) {
            for (int j = 0; j < 128; j++) {
                if (current->children[j] != nullptr) {
                    if (current->children[j]->payload == str[i]) {
                        current = current->children[j];
                        break;
                    }
                } else {
                    trie_node *child = new trie_node;
                    if (len == 1) {
                        child->is_terminal = true;
                    } else {
                        child->is_terminal = false;
                    }
                    child->payload = str[i];
                    child->parent = current;
                    current->children[j] = child;
                    current = current->children[j];
                    break;
                }
            }
        }
        current->is_terminal = true;
        m_size++;
        return true;
    }

}

bool trie::contains(const string &str) const {
    if (m_size == 0) {
        return false;
    } else {
        trie_node *current = m_root;
        int len = str.length();
        if (len < 1) {
            len = 1;
        }
        for (int i = 0; i < len; i++) {
            for (int j = 0; j < 128; j++) {
                if (current->children[j]) {
                    if (len == 1) {
                        if (current->children[j]->payload == str[i] && current->children[j]->is_terminal == true) {
                            current = current->children[j];
                            break;
                        }
                    } else {
                        if (current->children[j]->payload == str[i]) {
                            current = current->children[j];
                            break;
                        }
                    }
                } else {
                    return false;
                }
            }
        }
        if (current->is_terminal == true) {
            return true;
        }
        return false;
    }
}

size_t trie::size() const {
    return m_size;
}

bool trie::empty() const {
    if (m_size == 0) {
        return true;
    }
    return false;
}

vector<string> trie::search_by_prefix(const string &prefix) const {
    vector<string> words = {};
    string word = "";
    bool isFound = false;
    int i = 0;
    trie_node *current = m_root;

    while (prefix[i] != '\0') {
        for (int j = 0; j < 128; j++) {
            if (current->children[j]) {
                if (current->children[j]->payload == prefix[i]) {
                    current = current->children[j];
                    word.push_back(prefix[i]);
                    isFound = true;
                    break;
                }
            }
        }
        if (isFound == true) {
            isFound = false;
            i++;
        } else {
            return words;
        }
    }
    return gelAllWords(words, current, word);
}

bool findAllPrefixes(trie_node *pNode, const string &str) {
    if (str.size() == 0) {
        return false;
    }
    if (str.size() == 1) {
        for (int i = 0; i < 128; i++) {
            if (pNode->children[i]) {
                if ((pNode->children[i]->payload == str[0]) && pNode->children[i]->is_terminal) {
                    return true;
                }
            }
        }
        return false;
    } else {
        for (int i = 0; i < 128; i++) {
            if (pNode->children[i]) {
                if (pNode->children[i]->payload == str[0]) {
                    return findAllPrefixes(pNode->children[i], str.substr(1, str.size() - 1));
                }
            }
        }
        return false;
    }
}

vector<string> trie::get_prefixes(const string &str) const {
    vector<string> prefixes;
    for (int i = str.size(); i > 0; i--) {
        if (findAllPrefixes(m_root, str.substr(0, i))) {
            prefixes.push_back(str.substr(0, i));
        }
    }

    return prefixes;

}

trie::const_iterator trie::begin() const {
    trie_node *currnode = m_root;

    while (true) {
        if (currnode->children[0]) {
            currnode = currnode->children[0];
        }
        if (currnode->is_terminal == true) {
            break;
        }
    }
    return currnode;

}

trie::const_iterator trie::end() const {
    return nullptr;
}

bool compare(vector<string> vec1, vector<string> vec2) {
    bool status = false;
    for (int i = 0; i < vec1.size(); i++) {
        for (int j = 0; j < vec2.size(); j++) {
            if (vec1[i] == vec2[j]) {
                status = true;
                break;
            }
            if (j == vec2.size() - 1 && status == false) {
                return false;
            }
        }
        if (status == false || i == vec2.size() - 1) {
            break;
        }
        status = false;
    }
    return status;
}

bool trie::operator==(const trie &rhs) const {
    if (this->m_size == 0 && rhs.m_size == 0) {
        return true;
    } else if (this->m_size < rhs.m_size) {
        return true;
    } else if (this->m_size > rhs.m_size) {
        return false;
    } else {
        vector<string> words1 = rhs.search_by_prefix("");
        vector<string> words2 = this->search_by_prefix("");
        bool status = compare(words1, words2);
        return status;
    }
}

bool trie::operator<(const trie &rhs) const {
    vector<string> words1 = rhs.search_by_prefix("");
    vector<string> words2 = this->search_by_prefix("");
    int result = 0;
    if (this->m_size == 0 && rhs.m_size == 0) {
        return false;
    }
    if (this->m_size < rhs.m_size) {
        return true;
    } else {
        int len = words1.size();
        for (int i = 0; i < len; i++) {
            char str1[words1[i].size() + 1];
            strcpy(str1, words2[i].c_str());
            char str2[words2[i].size() + 1];
            strcpy(str2, words1[i].c_str());
            result = +strcmp(str2, str1);
        }
        if (result > 0 || result == 0) {
            return false;
        } else if (result < 0) {
            return true;
        }
    }
}

trie trie::operator&(trie const &rhs) const {
    return trie();
}

trie trie::operator|(trie const &rhs) const {
    return trie();
}

void trie::swap(trie &rhs) {
    trie_node *current = this->m_root;
    this->m_root = rhs.m_root;
    rhs.m_root = current;
    size_t currentSize = this->m_size;
    this->m_size = rhs.m_size;
    rhs.m_size = currentSize;

}


bool operator!=(const trie &lhs, const trie &rhs) {
    vector<string> words1 = rhs.search_by_prefix("");
    vector<string> words2 = lhs.search_by_prefix("");
    if (words1.size() == 0 && words2.size() == 0) {
        return false;
    } else {
        if (compare(words1, words2) == false) {
            return true;
        } else {
            return false;
        }
    }
}

bool operator<=(const trie &lhs, const trie &rhs) {
    return !(lhs > rhs);
}

bool operator>(const trie &lhs, const trie &rhs) {
    return rhs < lhs;
}

bool operator>=(const trie &lhs, const trie &rhs) {
    return !(lhs > rhs);
}

void swap(trie &lhs, trie &rhs) {
    lhs.swap(rhs);
}

ostream &operator<<(ostream &out, trie const &trie) {
    return out;
}

trie::const_iterator::const_iterator(const trie_node *node) {
    current_node = node;
}

trie::const_iterator &trie::const_iterator::operator++() {
    const_iterator temp = *this;
    const_iterator parent = temp.current_node->parent;
    const_iterator root = temp.current_node;
    while (root.current_node->parent != nullptr) {
        root.current_node = root.current_node->parent;
    }

    int position = 0;
    for (int j = 0; j < 128; j++) {
        if (temp.current_node->children[j]) {
            while (true) {
                if (temp.current_node->children[0]) {
                    temp.current_node = temp.current_node->children[0];
                    if (temp.current_node->is_terminal == true) {
                        return *this = temp;
                    }
                } else {
                    break;
                }
            }
        } else {
            for (int i = 0; i < 128; i++) {
                if (parent.current_node->children[i]) {
                    if (parent.current_node->children[i]->payload == temp.current_node->payload) {
                        position = i;
                        break;
                    }
                }
            }
            if (temp.current_node->parent != root.current_node) {
                while (!temp.current_node->parent->children[position + 1]) {
                    temp.current_node = temp.current_node->parent;
                    for (int i = 0; i < 128; i++) {
                        if (temp.current_node->parent->children[i]) {
                            if (temp.current_node->parent->children[i]->payload == temp.current_node->payload) {
                                position = i;
                                break;
                            }
                        } else {
                            break;
                        }
                    }
                    if (temp.current_node->parent == root.current_node) {
                        break;
                    }
                }
            }
            char chr = temp.current_node->payload;
            for (int i = 0; i < 128; i++) {
                if (temp.current_node->parent->children[i]) {
                    if (temp.current_node->parent->children[i]->payload == chr) {
                        position = i;
                        break;
                    }
                }
            }
            if (temp.current_node->parent->children[position + 1]) {
                temp.current_node = temp.current_node->parent->children[position + 1];
                while (!temp.current_node->is_terminal) {
                    temp.current_node = temp.current_node->children[0];
                }
            } else {
                temp = nullptr;
            }


        }
        return *this = temp;

    }
}

trie::const_iterator trie::const_iterator::operator++(int) {
    operator++();
    return *this;
}

trie::const_iterator::reference trie::const_iterator::operator*() const {
    const_iterator temp = *this;
    string str;
    while (temp.current_node->parent) {
        if (temp.current_node->payload == '\0') {

        } else {
            str.push_back(temp.current_node->payload);
        }
        temp.current_node = temp.current_node->parent;
    }

    reverse(str.begin(), str.end());
    return str;
}

bool trie::const_iterator::operator==(const trie::const_iterator &rhs) const {
    const_iterator temp = *this;
    if (temp.current_node == rhs.current_node) {
        return true;
    }
    return false;
}

bool trie::const_iterator::operator!=(const trie::const_iterator &rhs) const {
    const_iterator temp = *this;
    if (temp.current_node == rhs.current_node) {
        return false;
    }
    return true;
}





