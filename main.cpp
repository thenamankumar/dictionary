#include <iostream>
#include <string>
#include <map>
#include <set>

using namespace std;

class dictionary {
private:
    class trie {
    private:
        class node {
        private:
            char letter;
            string definition;
            map<char, node *> *children;
            bool terminal;
        public:
            node(char letter, string definition, bool terminal) {
                this->letter = letter;
                this->definition = definition;
                this->terminal = terminal;
                this->children = new map<char, node *>();
            };

            char getLetter() { return this->letter; };

            void setLetter(char letter) { this->letter = letter; };

            string getDefinition() { return this->definition; };

            void setDefinition(string definition) { this->definition = definition; };

            void setTerminal(bool terminal) { this->terminal = terminal; };

            bool getTerminal() { return this->terminal; };

            set<node *> getChildren() {
                set<node *> children;
                map<char, node *>::iterator itr;
                for (itr = this->children->begin(); itr != this->children->end(); itr++) {
                    children.insert(itr->second);
                }
                return children;
            };

            void addChild(char letter, node *child) { this->children->insert(pair<char, node *>(letter, child)); };

            node *getChild(char letter) {
                node *child;
                try {
                    child = this->children->at(letter);
                } catch (...) {
                    return NULL;
                }
                return child;
            };

            void removeChild(char letter) { this->children->erase(letter); };

            bool ifNoChild() { return this->children->empty(); };

            int getTotalChildren() { return this->children->size(); };
        };

        int totalWords;

        node *root;

        void addWord(node *parent, string word, string definition) {
            if (word.length() == 0) {
                if (!parent->getTerminal()) {
                    parent->setTerminal(true);
                    parent->setDefinition(definition);
                    this->totalWords++;
                }
                return;
            }

            char ch = word[0];
            string remainingWord = word.substr(1);
            node *child = parent->getChild(ch);
            if (child == NULL) {
                child = new node(ch, "\0", false);
                parent->addChild(ch, child);
            }

            this->addWord(child, remainingWord, definition);
        };

        bool find(node *parent, string word) {
            if (word.length() == 0) {
                return parent->getTerminal();
            }

            char ch = word[0];
            string remainingWord = word.substr(1);

            node *child = parent->getChild(ch);
            if (child == NULL) {
                return false;
            }

            return this->find(child, remainingWord);
        };

        string getWordDefinition(node *parent, string word) {
            if (word.length() == 0) {
                if (parent->getTerminal())
                    return parent->getDefinition();
                else
                    return "";
            }

            char ch = word[0];
            string remainingWord = word.substr(1);

            node *child = parent->getChild(ch);
            if (child == NULL) {
                return "";
            }

            return this->getWordDefinition(child, remainingWord);
        };

        void removeWord(node *parent, string word) {
            if (word.length() == 0) {
                if (parent->getTerminal()) {
                    parent->setTerminal(false);
                    this->totalWords--;
                }
                return;
            }

            char ch = word[0];
            string remainingWord = word.substr(1);

            node *child = parent->getChild(ch);
            if (child == NULL) {
                return;
            }

            this->removeWord(child, remainingWord);

            if (!child->getTerminal() && child->ifNoChild()) {
                parent->removeChild(ch);
            }
        };

        node *getPrefixEnd(node *parent, string prefix) {
            if (prefix.length() == 0) {
                return parent;
            }

            char ch = prefix[0];
            string remainingPrefix = prefix.substr(1);

            node *child = parent->getChild(ch);
            if (child == NULL) {
                return NULL;
            }

            return this->getPrefixEnd(child, remainingPrefix);
        };

        set<string> wordsFrom(node *treeNode, string word, set<string> words) {
            if (treeNode == NULL) {
                return words;
            }

            set<node *> children = treeNode->getChildren();

            for (auto child : children) {
                char ch = child->getLetter();
                string currentWord = word + ch;
                if (child->getTerminal()) {
                    words.insert(words.end(), currentWord);
                }
                words = this->wordsFrom(child, currentWord, words);
            }
            return words;
        };
    public:
        trie() {
            this->root = new node('\0', "\0", false);
            this->totalWords = 0;
        }

        int getTotalWords() { return this->totalWords; };

        bool isEmpty() { return getTotalWords() == 0; };

        void addWord(string word, string definition) { this->addWord(this->root, word, definition); };

        bool find(string word) { return this->find(this->root, word); };

        string getWordDefinition(string word) { return this->getWordDefinition(this->root, word); };

        void removeWord(string word) { this->removeWord(this->root, word); };

        set<string> wordsFrom(string prefix) {
            node *prefixEnd = this->getPrefixEnd(this->root, prefix);
            if (prefixEnd == NULL)
                return set<string>();
            return this->wordsFrom(prefixEnd, prefix, set<string>());
        };
    };

    trie *tree;
public:
    dictionary() {
        this->tree = new trie();
    }

    void add() {
        string word, definition;
        cout << "Word: ";
        cin >> word;
        cout << "Definition: ";
        cin >> definition;
        this->tree->addWord(word, definition);
        cout << "Word Added\n";
    };

    void update() {
        string word, definition;
        cout << "Word: ";
        cin >> word;
        cout << "Difination: ";
        cin >> definition;
        this->tree->addWord(word, definition);
        cout << "Word Definition Updated\n";
    };

    void remove() {
        string word;
        cout << "Word: ";
        cin >> word;
        this->tree->removeWord(word);
        cout << "Word Removed\n";
    };

    void search() {
        string word;
        cout << "Word: ";
        cin >> word;
        bool found = this->tree->find(word);
        if (!found) {
            cout << "Not Found!\n";
            return;
        }
        cout << "Definition: ";
        cout << this->tree->getWordDefinition(word);
    };

    void prefixSearch() {
        string prefix;
        cout << "Prefix: ";
        cin >> prefix;
        set<string> results = this->tree->wordsFrom(prefix);

        for (auto word : results) {
            cout << word << endl;
        }
    };
};

int main() {
    dictionary myDictionary;
    menu:
    cout << "1) Add Word\n";
    cout << "2) Update Word\n";
    cout << "3) Search Word\n";
    cout << "4) Remove Word\n";
    cout << "5) Prefix Search\n";
    cout << "6) Exit\n";

    int opt;
    option:
    cin >> opt;
    switch (opt) {
        case 1:
            myDictionary.add();
            break;
        case 2:
            myDictionary.update();
            break;
        case 3:
            myDictionary.search();
            break;
        case 4:
            myDictionary.remove();
            break;
        case 5:
            myDictionary.prefixSearch();
            break;
        case 6:
            exit(0);
        default:
            goto option;
    }
    cout << endl;
    goto menu;
}