// Темный Даниил Александрович
// 15.01.2023, 8 часов
// Сериализайция и десериализация списка

#include <iostream>
#include <vector>
#include <ctime>
#include <map>

// структуру ListNode модифицировать нельзя
struct ListNode {
    ListNode* prev = nullptr; // указатель на предыдущий элемент списка, либо `nullptr` в случае начала списка
    ListNode* next = nullptr;
    ListNode* rand = nullptr; // указатель на произвольный элемент данного списка, либо `nullptr`
    std::string data; // произвольные пользовательские данные
};

class List {
public:
    void Serialize(FILE* file); // сохранение списка в файл, файл открыт с помощью `fopen(path, "wb")`
    void Deserialize(FILE* file); // восстановление списка из файла, файл открыт с помощью `fopen(path, "rb")`
    
    // ... ваши методы для заполнения списка
    List(int size) {
        std::map<int, ListNode*> nodeMap;
        std::string str;

        for (int i = 0; i < size; i++) {
            std::cin >> str;
            appendNode(str, nodeMap);
        }

        std::srand(std::time(nullptr));
        
        int randomNum = 0;
        ListNode* p = this->head;

        if (size > 0) {
            do {
                randomNum = std::rand() % (size + 1);

                p->rand = linkRandomNode(randomNum, nodeMap);

                p = p->next;
            } while (p != nullptr);
        }
    }

    List(const std::vector<std::string>& nodes) {
        std::map<int, ListNode*> nodeMap;

        int size = nodes.size();

        for (int i = 0; i < size; i++) {
            appendNode(nodes[i], nodeMap);
        }

        std::srand(std::time(nullptr));
        
        int randomNum = 0;
        ListNode* p = this->head;

        if (size > 0) {
            do {
                randomNum = std::rand() % (size + 1);

                p->rand = linkRandomNode(randomNum, nodeMap);

                p = p->next;
            } while (p != nullptr);
        }
    }

    ~List() {
        removeNodes();
    }

    void removeNodes() {
        removeNode(this->head);
        this->head = nullptr;
        this->tail = nullptr;
        this->count = 0;
    }

    void removeNode(ListNode* p) {
        if (p->next != nullptr) {
            removeNode(p->next);
        }
        delete p;
    }

    void printOut() {
        ListNode* p = this->head;
        int curPosition = 0;
        
        do {
            std::cout << ++curPosition << ". \"" << p->data << "\"";
            if (p->rand != nullptr)
                std::cout << ". Random node: \"" << p->rand->data << "\"" << std::endl;
            else
                std::cout << ". Don't have random node" << std::endl;
            
            p = p->next;
        } while (p != nullptr);
    }

    void appendNode(std::string str, std::map<int, ListNode*>& nodeMap) {
        ListNode* p;

        p = this->tail;
        if (this->count == 0) {
            p = new ListNode;
            this->head = p;
        } else {
            p->next = new ListNode;
            p->next->prev = p;
            p = p->next;
        }

        nodeMap[(this->count)++] = p;

        p->next = nullptr;
        p->data = str;

        this->tail = p;
    }

    ListNode* linkRandomNode(int randomNum, std::map<int, ListNode*>& nodeMap) {
        return((randomNum != -1) ? nodeMap[randomNum] : nullptr);
    }

private:
    ListNode* head = nullptr;
    ListNode* tail = nullptr;
    int count = 0;
};

void List::Serialize(FILE* file) {
    ListNode* p = this->head;

    fwrite(&(this->count), sizeof(int), 1, file);
    
    std::map<ListNode*, int> nodeMap;
    int curNode = 0;
    int dataLen = 0;
    
    do {
        nodeMap[p] = curNode++;

        dataLen = p->data.length();
        fwrite(&dataLen, sizeof(dataLen), 1, file);
        fwrite(&(p->data[0]), sizeof(char), dataLen, file);

        p = p->next;
    } while (p != nullptr);

    p = this->head;
    int curRand = -1;

    do {
        curRand = ((p->rand != nullptr) ? nodeMap[p->rand] : -1);

        fwrite(&curRand, sizeof(int), 1, file);

        p = p->next;
    } while (p != nullptr);
}

void List::Deserialize(FILE* file) {
    if (this->count > 0)
        removeNodes();

    std::map<int, ListNode*> nodeMap;
    int dataLen = 0, size = 0;

    fread(&size, sizeof(int), 1, file);
    
    for (int i = 0; i < size; i++) {
        fread(&dataLen, sizeof(int), 1, file);
        std::string str(dataLen, '\0');
        fread(&(str[0]), sizeof(char), dataLen, file);
        appendNode(str, nodeMap);
    }
    
    ListNode* p = this->head;
    int randomNum = 0;

    for (int i = 0; i < size; i++) {
        fread(&randomNum, sizeof(int), 1, file);

        p->rand = linkRandomNode(randomNum, nodeMap);

        p = p->next;
    }
}

int main() {
    std::vector<std::string> arr1 = {"one", "two", "three", "four", 
        "five", "six", "seven", "eight"};
    std::vector<std::string> arr2 = {};
    
    List listToWrite(arr1);
    
    std::cout << "Before serialization:" << std::endl;
    listToWrite.printOut();

    FILE* file;
    if ((file = fopen("task3List.bin", "wb")) == NULL) {
        std::cout << "Cannot open file for writing." << std::endl;
        return 0;
    }

    listToWrite.Serialize(file);

    fclose(file);


    if ((file = fopen("task3List.bin", "rb")) == NULL) {
        std::cout << "Cannot open file for reading." << std::endl;
        return 0;
    }
    std::cout << std::endl << "Deserialized list: " << std::endl;

    List listToRead(arr2);

    listToRead.Deserialize(file);
    listToRead.printOut();

    fclose(file);
}