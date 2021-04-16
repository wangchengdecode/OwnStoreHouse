//
// Created by server1 on 21-4-2.
//
#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>
#define TAG "node"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)

/**
 * 链表的结构，　value(HEAD),p (p->next结构体)　value,p (p->next结构体) value,p (p->next结构体) ... value p(p->NULL)
 * @param argc
 * @param argv
 * @return
 */
 typedef struct _list{
     Node* head;
//     Node* tail;//指向末尾，解决add()中的从head　while循环效率问题
 } List;//创建一个数据结构解决指向指针的指针，这就是链表

void add(List *pList,int number);
void print(List* pList);
int gAll = 12;

void node_method(){
//    gAll = 12;
//    Node * head = NULL;
    List list;
    list.head /*=list.tail*/= NULL;
    int number;
    do{
        scanf("%d",&number);
        if(number != -1){
           add(&list,number);//无法修改head add(head,number)，需要指向指针的一个指针，add(&head,number)，void add(Node** phead,int number)
        }
    }while(number != -1);

    //遍历搜索list
//    Node *p;
//    for(p = list.head; p ;p=p->next){
//        printf("%d\t",p->value);
//    }
//    printf("\n");
    print(&list);
    printf("请输入一个数字（若存在），则删除：");
    scanf("%d",&number);
    Node* p;
    int isFound = 0;
    for(p = list.head; p ; p = p->next){
        if( p->value == number){
            printf("找到了！\n");//delete 需要让前一个node 指向它后一个node ,并free本身
            isFound = 1;
            break;
        }
    }
    if(!isFound){
        printf("没找到!\n");
    }
    //delete
    Node *q;
    /**
     * 一个技巧，只要一个指针出现在->的左边，就不能是ＮＵＬＬ，就需要进行边界判断/非空判断时一定要检查！
     * 下循环已经在第二个条件判断了p的非空，但是q的非空没有判断，如果Ｌist的第一个元素即要找的数字，那么q会成为NULL，会出错。
     */
    for(q = NULL,p = list.head; p; q = p,p = p->next){//指针q用来指向p的上一个位置，如果p指针存在，那么让q指向p，并让p指向下一个位置。
        //如果存在了，需要让q指向p的下一个，并释放掉p。
            if(p->value == number){
                if(q) {
                    q->next = p->next;//q需要边界条件限制
                }else{
                    list.head = p->next;
                }
                free(p);
                break;
            }
    }

//    清除整个链表
//    for(p = head;p;p=q){
//        q=p->next;
//        free(p);
//    }
//    return 0;
}

void add(List* pList,int number){//(Node** head,int number)
    //add-to linked list
    Node *p = (Node*)malloc(sizeof(Node));
    p->value = number;
    p->next = NULL;//新的结构体所指向的因该是NULL,接着让上一个末尾的last 指向新的Node
    //find the last
    Node *last = pList->head;// = head
    if(last) {
        while (last->next) {//如果last还有next的话
            last = last->next;
        }
        last->next = p;
    }else{//这个情况last = NULL
        pList->head = p;//head = p
    }
    //attach
}

void print(List* pList){
    Node* p;
    for(p = pList->head; p ;p=p->next){
        printf("%d\t",p->value);
    }
    printf("\n");
}

void printLog(char* a[]){
    LOGD("%s",*a);
}