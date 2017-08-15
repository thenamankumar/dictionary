#include <iostream>
#include <string>
#include <map>

using namespace std;

class dictionary {
private:
    class trie {
    private:
        class node {
        private:
            char letter;
            string definition;
            map<char, node *> *childern;
            bool terminal;
        public:
            node(char letter, string definition, bool terminal) {
                this->letter = letter;
                this->definition = definition;
                this->terminal = terminal;
                this->childern = new map<char, node *>();
            };

            char getLetter() { return this->letter; };

            void setLetter(char letter) { this->letter = letter; };

            string getDefinition() { return this->definition; };

            void setDefinition(string definition) { this->definition = definition; };

            void setTerminal(bool terminal) { this->terminal = terminal; };

            bool getTerminal() { return this->terminal; };

            void addChild(char letter, node *child) { this->childern->insert(pair<char, node *>(letter, child)); };

            node *getChild(char letter) { return this->childern->at(letter); };

            void removeChild(char letter) { this->childern->erase(letter); };

            bool ifNoChild() { return this->childern->empty(); };

            int getTotalChildern() { return this->childern->size(); };
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
};

int main() {
    dictionary myDictionary;
    return 0;
}