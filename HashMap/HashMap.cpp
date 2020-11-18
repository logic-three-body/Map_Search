#include<iostream>
#include<string>
#include<stdlib.h>
#include<cmath>
using namespace std;
#define MAXTABLESIZE 100000   // 定义允许开辟的最大散列表长度 
typedef int Index;
typedef string ElementType;
typedef Index Position;
typedef enum {   // 分别对应：有合法元素、空、有已删除元素 
	Legitimate, Empty, Deleted
} EntryType;  // 定义单元状态类型 

typedef struct HashEntry Cell;
struct HashEntry {   //  哈希表存值单元 
	ElementType Data;  // 存放元素
	EntryType Info;  // 单元状态	
};

//以下答案可以写在课设问题里以及解决方案
/*
https://bbs.csdn.net/topics/392156390
嗯，当然不是简单的输出 “hello”了，在Linux下用g++编译后运行试试，会出现“Segmentation fault (core dumped)”，
why？问题就出在给fr指针分配内存的时候，注意这里用的是C中的malloc而不是new，
如果你换成new再运行，就不会报错 了，成功的输出“hello”，那为什么malloc就不行呢？
这就要看malloc()与new()的区别了，关于两者的区别是程序员面试中屡问不爽的经典面试题，
所以相信一般的程序员都知道它们之间有一个非常重要的区别就是：new在分配内存时会调用默认的构造函数，而malloc不会调用。
而STL的string在赋值之前需要调用默认的构造函数以初始化string后才能使用，如赋值、打印等操作，
如果使用malloc分配内存，就不会调 用string默认的构造函数来初始化结构体中的app_name字符串，
因此这里给其直接赋值是错误的，应该使用new操作符。
这也提示我们用C++开发程序时，就尽量使用C++中的函数，不要C++与C混合编程，导致使用混淆，比如有时候new分配的内存却用free释放。


调试方法：若感觉某语句使程序崩溃，则注释此语句直至程序正常运行，之后逐项排查
*/

typedef struct HashTbl *HashTable;
struct HashTbl {  // 哈希表结构体 
	int TableSize;   // 哈希表大小 
	Cell *Cells;   // 哈希表存值单元数组 
};



int NextPrime(int N);  // 查找素数 
HashTable CreateTable(int TableSize); // 创建哈希表 
Index Hash(ElementType Key, int TableSize);   // 哈希函数 

// 查找素数 
int NextPrime(int N) {
	int p = (N % 2) ? N + 2 : N + 1;  // 从大于 N 的下个奇数开始
	int i;

	while (p <= MAXTABLESIZE) {
		for (i = (int)sqrt(p); i > 2; i--)
			if (!(p%i))  // p 不是素数 
				break;
		if (i == 2)
			break;
		p += 2;  // 继续试探下个奇数 
	}
	return p;
}

// 创建哈希表 
HashTable CreateTable(int TableSize) {
	HashTable H;
	int i;
	//H = (HashTable)malloc(sizeof(struct HashTbl)); //ERROR 查看上方注释解说
	H = new HashTbl;
	// 保证哈希表最大长度是素数 
	H->TableSize = NextPrime(TableSize);
	// 初始化单元数组
	//H->Cells = (Cell *)malloc(sizeof(Cell)*H->TableSize); //ERROR 查看上方注释解说
	H->Cells = new Cell[H->TableSize];
	// 初始化单元数组状态 
	for (int i = 0; i < H->TableSize; i++)
	{
		H->Cells[i].Info = Empty;
		//H->Cells[i].Data="";
	}	
	return H;
}

// 平方探测查找 
Position Find(HashTable H, ElementType Key) {
	Position CurrentPos, NewPos;
	int CNum = 0;   // 记录冲突次数
	CurrentPos = NewPos = Hash(Key, H->TableSize);
	// 如果当前单元状态不为空，且数值不等，则一直做 
	while (H->Cells[NewPos].Info != Empty && H->Cells[NewPos].Data != Key) {
		if (++CNum % 2) { // 冲突奇数次发生 
			NewPos = CurrentPos + (CNum + 1) / 2 * (CNum + 1) / 2;
			// 如果越界，一直减直到再次进入边界 
			while (H->TableSize <= NewPos) {
				NewPos -= H->TableSize;
			}
		}
		else {  // 冲突偶数次发生 
			NewPos = CurrentPos - CNum / 2 * CNum / 2;
			// 如果越界，一直加直到再次进入边界 
			while (NewPos < 0) {
				NewPos += H->TableSize;
			}
		}
	}
	return NewPos;
}

// 插入
bool Insert(HashTable H, ElementType Key, int i) {
	Position Pos = i;
	Pos = Find(H, Key);
	// 如果单元格状态不是"存在合法元素" 
	if (H->Cells[Pos].Info != Legitimate) {
		H->Cells[Pos].Info = Legitimate;
		H->Cells[Pos].Data = Key;
	}
	return true;
}

// 除留余数法哈希函数 
Index Hash(ElementType Key, int TableSize) {
	///return Key % TableSize;
	unsigned int h = 0;// 散列值函数，初始化为 0 
	for (int i = 0; i <= Key.length(); i++)//位移映射
	{
		h = ((h << 5) + Key[i])% TableSize;
	}
	return h % TableSize;
}


void output(HashTable H) {
	for (int i = 0; i < H->TableSize; i++)
		cout << i << " " << H->Cells[i].Data << endl;
}

int main() {
	HashTable H = CreateTable(26);
	int N;
	cin >> N;
	for (int i = 0; i < N; i++) {
		ElementType tmp;
		cin >> tmp;
		Insert(H, tmp, i);
	}
	output(H);
	return 0;
}