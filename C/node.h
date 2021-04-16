//
// Created by server1 on 21-4-2.
//

#ifndef STUDYFORC_NODE_H//标准头文件结构　， ifndef XX_H_如果没有定义过这个宏，那么我们久定义这个宏,为了防止重复定义
//                 #define xx_h_  #endif
#define STUDYFORC_NODE_H
extern int gAll; // 外部可访问的全局变量
void node_method();
void printLog(char *a[]);
typedef struct _node{//链表
    int value;
    struct _node *next;//指向下一个的指针,为什么不直接Node *next,因为编译器到这一步并不认识Node.但是认识struct _node
} Node;

/**为什么不写成这样！，因为typedef可以省略到处达struct.
struct _node{
    int value;
    struct _node *next;
};
 */

#endif //STUDYFORC_NODE_H
