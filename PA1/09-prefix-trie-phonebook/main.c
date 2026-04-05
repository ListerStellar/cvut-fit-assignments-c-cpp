#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

constexpr int PHONE_DIGITS = 10;

typedef struct TNode {
    char* m_Name;
    struct TNode* m_Child[PHONE_DIGITS];
} TNODE;

typedef struct {
    TNODE* m_Root;
    int m_Size;
} TPHONEBOOK;


//void printTree(TNODE* node, char path[100]) {
//    if (node) {
//        for (int i = 0; i < PHONE_DIGITS; i++) {
//            if (node->m_Child[i]) {
//                for (int j = 0; j < strlen(path); j++) {
//                    printf("%c -> ", path[j]);
//                }
//                printf("%d\n", i);
//                char newPath[100];
//                strcpy(newPath, path);
//                addChar(newPath, '0' + i);
//                printTree(node->m_Child[i], newPath);
//            }
//        }
//    }
//}

#endif /* __PROGTEST__ */

int ctoi(char number) {
    return number - '0';
}

TNODE* initNode() {
    TNODE* node = (TNODE*)malloc(sizeof(TNODE));
    node->m_Name = nullptr;
    for (int i = 0; i < PHONE_DIGITS; i++) {
        node->m_Child[i] = nullptr;
    }
    return node;
}

bool addPhone(TPHONEBOOK* book, const char* phone, const char* name) {
    if (!phone || strlen(phone) == 0 || !name || !book) return false;
    for (int i = 0; phone[i]; i++) {
        if (phone[i] < '0' || phone[i] > '9') return false;
    }
    if (!book->m_Root) {
        book->m_Root = initNode();
    }
    TNODE* node = book->m_Root;
    for (int i = 0; phone[i]; i++) {
        if (!node->m_Child[ctoi(phone[i])]) {
            node->m_Child[ctoi(phone[i])] = initNode();
        }
        node = node->m_Child[ctoi(phone[i])];
    }
    if (!node->m_Name) book->m_Size++;
    else {
        free(node->m_Name);
        node->m_Name = nullptr;
    }
    node->m_Name = (char*)malloc(strlen(name) + 1);
    strcpy(node->m_Name, name);
    return true;
}

void delUnderNodes(TNODE* node) {
    if (node) {
        for (int i = 0; i < PHONE_DIGITS; i++) {
            if (node->m_Child[i]) {
                delUnderNodes(node->m_Child[i]);
                if (node->m_Child[i]->m_Name) {
                    free(node->m_Child[i]->m_Name);
                    node->m_Child[i]->m_Name = nullptr;
                }
                free(node->m_Child[i]);
                node->m_Child[i] = nullptr;
            }
        }
    }
}

void delBook(TPHONEBOOK* book) {
    if (!book || !book->m_Root) return;
    delUnderNodes(book->m_Root);
    free(book->m_Root);
    book->m_Root = nullptr;
    book->m_Size = 0;
}

bool isNodeEmpty(TNODE* node) {
    if (node->m_Name) return false;
    for (int i = 0; i < PHONE_DIGITS; i++) {
        if (node->m_Child[i]) return false;
    }
    return true;
}

void delExtraNodesSupfunc(TNODE* node) {
    if (node) {
        for (int i = 0; i < PHONE_DIGITS; i++) {
            if (node->m_Child[i]) {
                if (isNodeEmpty(node->m_Child[i])) {
                    if (node->m_Child[i]->m_Name) {
                        free(node->m_Child[i]->m_Name);
                        node->m_Child[i]->m_Name = nullptr;
                    }
                    free(node->m_Child[i]);
                    node->m_Child[i] = nullptr;
                }
                else delExtraNodesSupfunc(node->m_Child[i]);
                if (node->m_Child[i] && isNodeEmpty(node->m_Child[i])) {
                    if (node->m_Child[i]->m_Name) {
                        free(node->m_Child[i]->m_Name);
                        node->m_Child[i]->m_Name = nullptr;
                    }
                    free(node->m_Child[i]);
                    node->m_Child[i] = nullptr;
                }
            }
        }
    }
}

void delExtraNodes(TPHONEBOOK* book) {
    if (!book || !book->m_Root) return;
    delExtraNodesSupfunc(book->m_Root);
    if (!book->m_Root) return;
    if (isNodeEmpty(book->m_Root)) book->m_Root = nullptr;
    //char path[100] = "0";
    //printTree(book->m_Root, path);
    //printf("----------------------------------\n");
}

bool delPhone(TPHONEBOOK* book, const char* phone) {
    if (!phone || strlen(phone) == 0 || !book) return false;
    for (int i = 0; phone[i]; i++) {
        if (phone[i] < '0' || phone[i] > '9') return false;
    }
    if (!book->m_Root) return false;
    TNODE* node = book->m_Root;
    for (int i = 0; phone[i]; i++) {
        if (!node->m_Child[ctoi(phone[i])]) return false;
        node = node->m_Child[ctoi(phone[i])];
    }
    if (!node->m_Name) return false;
    free(node->m_Name);
    node->m_Name = nullptr;
    book->m_Size--;
    delExtraNodes(book);
    return true;
}

const char* findPhone(TPHONEBOOK* book, const char* phone) {
    if (!phone || strlen(phone) == 0 || !book) return nullptr;
    for (int i = 0; phone[i]; i++) {
        if (phone[i] < '0' || phone[i] > '9') return nullptr;
    }
    if (!book->m_Root) return nullptr;
    const char* lastName = nullptr;
    TNODE* node = book->m_Root;
    for (int i = 0; phone[i]; i++) {
        if (node->m_Child[ctoi(phone[i])]) {
            node = node->m_Child[ctoi(phone[i])];
            if (node && node->m_Name) lastName = node->m_Name;
        }
        else return lastName;
    }
    return lastName;
}

#ifndef __PROGTEST__
int main() {
    TPHONEBOOK b = { nullptr, 0 };
    char tmpStr[100];
    /*addPhone(&b, "000", "");
    char path[100] = "0";
    printTree(b.m_Root, path);*/
    //printf("%s\n", b.m_Root->m_Name);
    //printf("%d\n", strlen(""));



    const char* name;
    assert(addPhone(&b, "420", "Czech Republic"));
    assert(addPhone(&b, "42022435", "Czech Republic CVUT"));
    assert(addPhone(&b, "421", "Slovak Republic"));
    assert(addPhone(&b, "44", "Great Britain"));
    strncpy(tmpStr, "USA", sizeof(tmpStr) - 1);
    assert(addPhone(&b, "1", tmpStr));
    strncpy(tmpStr, "Guam", sizeof(tmpStr) - 1);
    assert(addPhone(&b, "1671", tmpStr));
    assert(addPhone(&b, "44", "United Kingdom"));
    assert(b.m_Size == 6);
    assert(!b.m_Root->m_Name);
    assert(!b.m_Root->m_Child[0]);
    assert(!strcmp(b.m_Root->m_Child[1]->m_Name, "USA"));
    assert(!b.m_Root->m_Child[1]->m_Child[0]);
    assert(!b.m_Root->m_Child[1]->m_Child[1]);
    assert(!b.m_Root->m_Child[1]->m_Child[2]);
    assert(!b.m_Root->m_Child[1]->m_Child[3]);
    assert(!b.m_Root->m_Child[1]->m_Child[4]);
    assert(!b.m_Root->m_Child[1]->m_Child[5]);
    assert(!b.m_Root->m_Child[1]->m_Child[6]->m_Name);
    assert(!b.m_Root->m_Child[1]->m_Child[6]->m_Child[0]);
    assert(!b.m_Root->m_Child[1]->m_Child[6]->m_Child[1]);
    assert(!b.m_Root->m_Child[1]->m_Child[6]->m_Child[2]);
    assert(!b.m_Root->m_Child[1]->m_Child[6]->m_Child[3]);
    assert(!b.m_Root->m_Child[1]->m_Child[6]->m_Child[4]);
    assert(!b.m_Root->m_Child[1]->m_Child[6]->m_Child[5]);
    assert(!b.m_Root->m_Child[1]->m_Child[6]->m_Child[6]);
    assert(!b.m_Root->m_Child[1]->m_Child[6]->m_Child[7]->m_Name);
    assert(!b.m_Root->m_Child[1]->m_Child[6]->m_Child[7]->m_Child[0]);
    assert(!strcmp(b.m_Root->m_Child[1]->m_Child[6]->m_Child[7]->m_Child[1]->m_Name, "Guam"));
    assert(!b.m_Root->m_Child[1]->m_Child[6]->m_Child[7]->m_Child[1]->m_Child[0]);
    assert(!b.m_Root->m_Child[1]->m_Child[6]->m_Child[7]->m_Child[1]->m_Child[1]);
    assert(!b.m_Root->m_Child[1]->m_Child[6]->m_Child[7]->m_Child[1]->m_Child[2]);
    assert(!b.m_Root->m_Child[1]->m_Child[6]->m_Child[7]->m_Child[1]->m_Child[3]);
    assert(!b.m_Root->m_Child[1]->m_Child[6]->m_Child[7]->m_Child[1]->m_Child[4]);
    assert(!b.m_Root->m_Child[1]->m_Child[6]->m_Child[7]->m_Child[1]->m_Child[5]);
    assert(!b.m_Root->m_Child[1]->m_Child[6]->m_Child[7]->m_Child[1]->m_Child[6]);
    assert(!b.m_Root->m_Child[1]->m_Child[6]->m_Child[7]->m_Child[1]->m_Child[7]);
    assert(!b.m_Root->m_Child[1]->m_Child[6]->m_Child[7]->m_Child[1]->m_Child[8]);
    assert(!b.m_Root->m_Child[1]->m_Child[6]->m_Child[7]->m_Child[1]->m_Child[9]);
    assert(!b.m_Root->m_Child[1]->m_Child[6]->m_Child[7]->m_Child[2]);
    assert(!b.m_Root->m_Child[1]->m_Child[6]->m_Child[7]->m_Child[3]);
    assert(!b.m_Root->m_Child[1]->m_Child[6]->m_Child[7]->m_Child[4]);
    assert(!b.m_Root->m_Child[1]->m_Child[6]->m_Child[7]->m_Child[5]);
    assert(!b.m_Root->m_Child[1]->m_Child[6]->m_Child[7]->m_Child[6]);
    assert(!b.m_Root->m_Child[1]->m_Child[6]->m_Child[7]->m_Child[7]);
    assert(!b.m_Root->m_Child[1]->m_Child[6]->m_Child[7]->m_Child[8]);
    assert(!b.m_Root->m_Child[1]->m_Child[6]->m_Child[7]->m_Child[9]);
    assert(!b.m_Root->m_Child[1]->m_Child[6]->m_Child[8]);
    assert(!b.m_Root->m_Child[1]->m_Child[6]->m_Child[9]);
    assert(!b.m_Root->m_Child[1]->m_Child[7]);
    assert(!b.m_Root->m_Child[1]->m_Child[8]);
    assert(!b.m_Root->m_Child[1]->m_Child[9]);
    assert(!b.m_Root->m_Child[2]);
    assert(!b.m_Root->m_Child[3]);
    assert(!b.m_Root->m_Child[4]->m_Name);
    assert(!b.m_Root->m_Child[4]->m_Child[0]);
    assert(!b.m_Root->m_Child[4]->m_Child[1]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Name);
    assert(!strcmp(b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Name, "Czech Republic"));
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[0]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[1]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Name);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[0]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[1]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Name);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[0]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[1]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[2]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[3]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Name);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[0]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[1]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[2]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Name);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[0]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[1]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[2]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[3]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[4]);
    assert(!strcmp(b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[5]->m_Name, "Czech Republic CVUT"));
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[5]->m_Child[0]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[5]->m_Child[1]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[5]->m_Child[2]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[5]->m_Child[3]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[5]->m_Child[4]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[5]->m_Child[5]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[5]->m_Child[6]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[5]->m_Child[7]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[5]->m_Child[8]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[5]->m_Child[9]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[6]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[7]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[8]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[9]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[4]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[5]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[6]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[7]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[8]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[9]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[5]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[6]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[7]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[8]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[9]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[3]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[4]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[5]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[6]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[7]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[8]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[9]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[3]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[4]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[5]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[6]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[7]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[8]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[9]);
    assert(!strcmp(b.m_Root->m_Child[4]->m_Child[2]->m_Child[1]->m_Name, "Slovak Republic"));
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[1]->m_Child[0]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[1]->m_Child[1]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[1]->m_Child[2]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[1]->m_Child[3]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[1]->m_Child[4]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[1]->m_Child[5]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[1]->m_Child[6]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[1]->m_Child[7]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[1]->m_Child[8]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[1]->m_Child[9]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[2]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[3]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[4]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[5]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[6]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[7]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[8]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[9]);
    assert(!b.m_Root->m_Child[4]->m_Child[3]);
    assert(!strcmp(b.m_Root->m_Child[4]->m_Child[4]->m_Name, "United Kingdom"));
    assert(!b.m_Root->m_Child[4]->m_Child[4]->m_Child[0]);
    assert(!b.m_Root->m_Child[4]->m_Child[4]->m_Child[1]);
    assert(!b.m_Root->m_Child[4]->m_Child[4]->m_Child[2]);
    assert(!b.m_Root->m_Child[4]->m_Child[4]->m_Child[3]);
    assert(!b.m_Root->m_Child[4]->m_Child[4]->m_Child[4]);
    assert(!b.m_Root->m_Child[4]->m_Child[4]->m_Child[5]);
    assert(!b.m_Root->m_Child[4]->m_Child[4]->m_Child[6]);
    assert(!b.m_Root->m_Child[4]->m_Child[4]->m_Child[7]);
    assert(!b.m_Root->m_Child[4]->m_Child[4]->m_Child[8]);
    assert(!b.m_Root->m_Child[4]->m_Child[4]->m_Child[9]);
    assert(!b.m_Root->m_Child[4]->m_Child[5]);
    assert(!b.m_Root->m_Child[4]->m_Child[6]);
    assert(!b.m_Root->m_Child[4]->m_Child[7]);
    assert(!b.m_Root->m_Child[4]->m_Child[8]);
    assert(!b.m_Root->m_Child[4]->m_Child[9]);
    assert(!b.m_Root->m_Child[5]);
    assert(!b.m_Root->m_Child[6]);
    assert(!b.m_Root->m_Child[7]);
    assert(!b.m_Root->m_Child[8]);
    assert(!b.m_Root->m_Child[9]);
    name = findPhone(&b, "420800123456");  assert(!strcmp(name, "Czech Republic"));
    name = findPhone(&b, "420224351111");  assert(!strcmp(name, "Czech Republic CVUT"));
    name = findPhone(&b, "42022435");  assert(!strcmp(name, "Czech Republic CVUT"));
    name = findPhone(&b, "4202243");  assert(!strcmp(name, "Czech Republic"));
    name = findPhone(&b, "420224343258985224");  assert(!strcmp(name, "Czech Republic"));
    name = findPhone(&b, "42");  assert(!name);
    name = findPhone(&b, "422");  assert(!name);
    name = findPhone(&b, "4422");  assert(!strcmp(name, "United Kingdom"));
    name = findPhone(&b, "16713425245763");  assert(!strcmp(name, "Guam"));
    name = findPhone(&b, "123456789123456789");  assert(!strcmp(name, "USA"));
    assert(delPhone(&b, "420"));
    assert(!delPhone(&b, "420"));
    assert(delPhone(&b, "421"));
    assert(delPhone(&b, "44"));
    assert(delPhone(&b, "1671"));
    assert(!delPhone(&b, "1672"));
    assert(!delPhone(&b, "1671"));
    name = findPhone(&b, "16713425245763");  assert(!strcmp(name, "USA"));
    name = findPhone(&b, "4422");  assert(!name);
    name = findPhone(&b, "420800123456");  assert(!name);
    name = findPhone(&b, "420224351111");  assert(!strcmp(name, "Czech Republic CVUT"));
    assert(b.m_Size == 2);
    assert(!b.m_Root->m_Name);
    assert(!b.m_Root->m_Child[0]);
    assert(!strcmp(b.m_Root->m_Child[1]->m_Name, "USA"));
    assert(!b.m_Root->m_Child[1]->m_Child[0]);
    assert(!b.m_Root->m_Child[1]->m_Child[1]);
    assert(!b.m_Root->m_Child[1]->m_Child[2]);
    assert(!b.m_Root->m_Child[1]->m_Child[3]);
    assert(!b.m_Root->m_Child[1]->m_Child[4]);
    assert(!b.m_Root->m_Child[1]->m_Child[5]);
    assert(!b.m_Root->m_Child[1]->m_Child[6]);
    assert(!b.m_Root->m_Child[1]->m_Child[7]);
    assert(!b.m_Root->m_Child[1]->m_Child[8]);
    assert(!b.m_Root->m_Child[1]->m_Child[9]);
    assert(!b.m_Root->m_Child[2]);
    assert(!b.m_Root->m_Child[3]);
    assert(!b.m_Root->m_Child[4]->m_Name);
    assert(!b.m_Root->m_Child[4]->m_Child[0]);
    assert(!b.m_Root->m_Child[4]->m_Child[1]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Name);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Name);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[0]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[1]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Name);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[0]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[1]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Name);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[0]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[1]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[2]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[3]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Name);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[0]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[1]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[2]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Name);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[0]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[1]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[2]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[3]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[4]);
    assert(!strcmp(b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[5]->m_Name, "Czech Republic CVUT"));
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[5]->m_Child[0]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[5]->m_Child[1]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[5]->m_Child[2]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[5]->m_Child[3]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[5]->m_Child[4]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[5]->m_Child[5]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[5]->m_Child[6]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[5]->m_Child[7]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[5]->m_Child[8]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[5]->m_Child[9]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[6]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[7]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[8]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[3]->m_Child[9]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[4]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[5]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[6]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[7]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[8]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[4]->m_Child[9]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[5]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[6]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[7]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[8]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[2]->m_Child[9]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[3]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[4]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[5]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[6]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[7]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[8]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[2]->m_Child[9]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[3]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[4]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[5]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[6]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[7]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[8]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[0]->m_Child[9]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[1]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[2]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[3]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[4]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[5]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[6]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[7]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[8]);
    assert(!b.m_Root->m_Child[4]->m_Child[2]->m_Child[9]);
    assert(!b.m_Root->m_Child[4]->m_Child[3]);
    assert(!b.m_Root->m_Child[4]->m_Child[4]);
    assert(!b.m_Root->m_Child[4]->m_Child[5]);
    assert(!b.m_Root->m_Child[4]->m_Child[6]);
    assert(!b.m_Root->m_Child[4]->m_Child[7]);
    assert(!b.m_Root->m_Child[4]->m_Child[8]);
    assert(!b.m_Root->m_Child[4]->m_Child[9]);
    assert(!b.m_Root->m_Child[5]);
    assert(!b.m_Root->m_Child[6]);
    assert(!b.m_Root->m_Child[7]);
    assert(!b.m_Root->m_Child[8]);
    assert(!b.m_Root->m_Child[9]);
    assert(delPhone(&b, "1"));
    assert(delPhone(&b, "42022435"));
    assert(!addPhone(&b, "12345XYZ", "test"));
    assert(b.m_Size == 0);
    assert(!b.m_Root);

    delBook(&b);
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
