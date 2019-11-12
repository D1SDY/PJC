#include "trie.hpp"

#include <utility>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <stack>

using namespace std;

int i = 0;

trie::trie(const vector<string> &strings) {
    m_root = new trie_node;
    for (int i = 0; i < strings.size(); i++) {
        insert(strings[i]);
    }
}

trie::trie() {
    m_root = new trie_node;
}

trie::~trie() {
    if (m_root) {
        stack<trie_node*> nodes;
        nodes.push(m_root);

        while (!nodes.empty()) {
            auto current = nodes.top();
            nodes.pop();
            for(int i=0;i<128;i++){
                if(current->children[i]){
                    nodes.push(current->children[i]);
                }
            }
            delete current;
        }
    }
    m_root= nullptr;
    m_size=0;
}

trie &trie::operator=(const trie &rhs) {
    //return <#initializer#>;
}

trie::trie(trie &&rhs) {
    m_root = rhs.m_root;
    rhs.m_root = nullptr;
}

trie::trie(const trie &rhs) {
    vector<string> words=rhs.search_by_prefix("");

    if (rhs.m_root->payload==' '){
        words.push_back("");
    }
    m_root=new trie_node();
    m_size=0;
    for(auto value : words){
        insert(value);
    }
}

trie &trie::operator=(trie &&rhs) {
    //return <#initializer#>;
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
            int pos = 0;
            if (current->children) {
                isEmpty = false;
            } else {
                isEmpty = true;
            }
            if (isEmpty == true) {
                trie_node *parrent = current->parent;
                for (int i; i < 128; i++) {
                    if (parrent->children[i]) {
                        if (parrent->children[i] == current) {
                            pos = i;
                            break;
                        }
                    }
                }
                parrent->children[pos] = nullptr;
                delete (current);
            } else {
                current->is_terminal = false;
            }
            m_size--;
            return true;
        }

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
                if (current->children[j]) {
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
    vector<string> words={};
    string word="";
    bool isFound=false;
    int i=0;
    trie_node* current=m_root;

    while(prefix[i] != '\0'){
        for (int j=0;j<128;j++){
            if(current->children[j]){
                if(current->children[j]->payload==prefix[i]){
                    current=current->children[j];
                    word.push_back(prefix[i]);
                    isFound=true;
                    break;
                }
            }
        }
        if(isFound==true){
            isFound=false;
            i++;
        }else{
            return words;
        }
    }
    return gelAllWords(words,current,word);
}

vector<string> trie::gelAllWords(vector<string> vector, trie_node *pNode, string basicString) const {
    if (pNode->is_terminal){
        vector.push_back(basicString);
    }
    for (int i = 0; i < 128; i++){
        if (pNode->children[i] != nullptr){
            trie_node* current=pNode->children[i];
            basicString.push_back(current->payload);
            vector = gelAllWords(vector, current, basicString);
            basicString.pop_back();
        }
    }

    return vector;
}

vector<string> trie::get_prefixes(const string &str) const {
    return vector<string>();
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

bool trie::operator==(const trie &rhs) const {
    return false;
}

bool trie::operator<(const trie &rhs) const {
    return false;
}

trie trie::operator&(trie const &rhs) const {
    return trie();
}

trie trie::operator|(trie const &rhs) const {
    return trie();
}

void trie::swap(trie &rhs) {

}

bool operator!=(const trie &lhs, const trie &rhs) {
    return false;
}

bool operator<=(const trie &lhs, const trie &rhs) {
    return false;
}

bool operator>(const trie &lhs, const trie &rhs) {
    return false;
}

bool operator>=(const trie &lhs, const trie &rhs) {
    return false;
}

void swap(trie &lhs, trie &rhs) {

}

ostream &operator<<(ostream &out, trie const &trie) {
    return out;
}

trie::const_iterator::const_iterator(const trie_node *node) {
    current_node = node;
}

trie::const_iterator &trie::const_iterator::operator++() {
    const_iterator temp = *this;
    const_iterator root = *this;
    int position = 0;
    while (root.current_node->parent) {
        root.current_node = root.current_node->parent;
    }
    while (true) {
        temp.current_node = temp.current_node->parent;
        if (temp.current_node->parent == root.current_node) {
            break;
        }
    }
    for (int i = 0; i < 128; i++) {
        if (root.current_node->children[i]) {
            if (root.current_node->children[i]->payload == temp.current_node->payload) {
                position = i;
                break;
            }
        }
    }
    position++;
    if (root.current_node->children[position]) {
        root = root.current_node->children[position];
        while (true) {
            if (root.current_node->children[0]) {
                root.current_node = root.current_node->children[0];
            }
            if (root.current_node->is_terminal == true) {
                break;
            }
        }
    } else {
        root = nullptr;
    }

    return *this = root;
}

trie::const_iterator trie::const_iterator::operator++(int) {
    const_iterator temp = *this;
    const_iterator root = *this;
    int position = 0;
    while (root.current_node->parent) {
        root.current_node = root.current_node->parent;
    }
    while (true) {
        temp.current_node = temp.current_node->parent;
        if (temp.current_node->parent == root.current_node) {
            break;
        }
    }
    for (int i = 0; i < 128; i++) {
        if (root.current_node->children[i]) {
            if (root.current_node->children[i]->payload == temp.current_node->payload) {
                position = i;
                break;
            }
        }
    }
    position++;
    if (root.current_node->children[position]) {
        root = root.current_node->children[position];
        while (true) {
            if (root.current_node->children[0]) {
                root.current_node = root.current_node->children[0];
            }
            if (root.current_node->is_terminal == true) {
                break;
            }
        }
    } else {
        root = nullptr;
    }
    return *this = root;

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





