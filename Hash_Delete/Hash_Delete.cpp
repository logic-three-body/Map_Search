#include<iostream>
#include<cstdlib>
#include<cmath>
#define MAXTABLESIZE 100000
typedef int Index;
typedef int ElementType;
typedef struct LNode *PtrToLNode;
struct LNode {   // 单链表 
	ElementType Data;
	PtrToLNode Next;
};
typedef PtrToLNode Position;
typedef PtrToLNode List;

typedef struct TblNode *HashTable;  // 散列表
struct TblNode {
	int TableSize;   // 表的最大长度 
	List Heads;  // 指向链表头结点的数组 
};
using namespace std;

int NextPrime(int N) {
	int p = (N % 2) ? (N + 2) : (N + 1);   // 比 tablesize 大的奇数 
	int i;
	while (p <= MAXTABLESIZE) {
		for (i = (int)sqrt(p); i > 2; i--)
			if (!(p%i))
				break;
		if (i == 2)  // 找到素数了 
			break;
		p += 2; // 下一个奇数
	}
	return p;
}

// 创建哈希表 
HashTable CreateTable(int TableSize) {
	HashTable H;
	H = (HashTable)malloc(sizeof(struct TblNode));
	//H->TableSize = NextPrime(TableSize);
	H->TableSize = TableSize;
	H->Heads = (List)malloc(sizeof(struct TblNode) * H->TableSize);
	for (int i = 0; i < H->TableSize; i++)
		H->Heads[i].Next = NULL;  // 链表头：H->Heads[i] 是不存东西的 
	return H;
}

// 除留余数法哈希函数 
Index Hash(int TableSize, ElementType Key) {
	return  Key % TableSize;
}

// 查找
Position Find(HashTable H, ElementType Key) {
	Position p;
	Index pos;

	pos = Hash(H->TableSize, Key);
	p = H->Heads[pos].Next;  //获得链表头 
	while (p && p->Data != Key)
		p = p->Next;
	return p;
}

// 插入
bool Insert(HashTable H, ElementType Key) {
	Position p, NewCell;
	Index pos;

	p = Find(H, Key);
	if (!p) {  // 关键词未找到，可以插入 
		NewCell = (Position)malloc(sizeof(struct LNode));
		NewCell->Data = Key;
		NewCell->Next = nullptr;//尾插
		pos = Hash(H->TableSize, Key);   // 初始散列表地址
		// 将新增结点插到最前面
		//尾插
		List p = H->Heads[pos].Next;
		if (!p)
		{
			H->Heads[pos].Next = NewCell;
		}
		else
		{
			while (p->Next)
			{
				p = p->Next;
			}
			p->Next = NewCell;
		}
		//头插
		//NewCell->Next = H->Heads[pos].Next;
		//H->Heads[pos].Next = NewCell;
		return true;
	}
	else {
		return false;
	}
}


// 删除
bool Delete(HashTable H, ElementType Key) {
	Position p, NewCell;
	Index pos;

	p = Find(H, Key);
	if (p) {  // 关键词找到，删除 
		pos = Hash(H->TableSize, Key);   // 初始散列表地址
		List pre = &H->Heads[pos];
		List q = pre->Next;
		while (q->Data!=Key&&q->Next)
		{
			q = q->Next;
			pre = pre->Next;
		}
		pre->Next = q->Next;
		delete q;
		return true;
	}
	else {
		return false;
	}
}

void output(HashTable H) {
	for (int i = 0; i < H->TableSize; i++) {
		cout << i;
		List p = H->Heads[i].Next;
		while (p) {
			cout << " " << p->Data;
			p = p->Next;
		}
		cout << endl;
	}
}

void DestroyTable(HashTable H) {
	Position P, tmp;
	for (int i = 0; i < H->TableSize; i++) {
		P = H->Heads[i].Next;
		while (P) {
			tmp = P->Next;
			free(P);
			P = tmp;
		}
	}
	free(H->Heads);
	free(H);
}


int main() {
	while (true)
	{
		HashTable H = CreateTable(13);
		int N;
		cin >> N;
		if (!N)
		{
			break;
		}
		for (int i = 0; i < N; i++) {
			int tmp;
			cin >> tmp;
			Insert(H, tmp);
		}
		ElementType data = 0;
		std::cin >> data;
		Delete(H, data);
		output(H);
		DestroyTable(H);
	}
	return 0;
}