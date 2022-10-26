#include<iostream>
#include<fstream>
#include<string>
#include<assert.h>

using namespace std;

const char RefValue = '#';

template<typename T>class cirqueue {
	T* data;
	int rear, front;
	int QueueSize;
public:
	cirqueue(int size = 100);
	~cirqueue() { delete[]data; }
	void EnQueue(T x);
	T DeQueue();
	int IsEmpty() { return front == rear; }
	int IsFull() { return (rear + 1) % QueueSize == front; }
};

template<typename T> cirqueue<T>::cirqueue(int size) {
	data = new T[QueueSize=size];
	
}


template<typename T>void cirqueue<T>::EnQueue(T x){
	assert(!IsFull());
	rear = (rear + 1) % QueueSize;
	data[rear] = x;
}


template<typename T>T cirqueue<T>::DeQueue() {
	T temp;
	assert(!IsEmpty());
	front = (front + 1) % QueueSize;
	temp=data[front] ;
	return temp;
}

template<typename T>
class NetTree;
template<typename T>
class Node {
	T data;
	int load;
	Node* lchild, * rchild;
public:
	Node() {};
	Node(T mark);
	~Node() {};
	friend class NetTree<T>;
};

template<typename T>Node<T>::Node(T mark) {
	data = mark;
	lchild = NULL;
	rchild = NULL;
}

template<typename T>
class NetTree {
	Node<T>* root;
public:
	NetTree();
	~NetTree();
	void clearTree(Node<T>* root);
	Node<T>* GetRoot() { return root; }//返回根结点
	int CreateTree();
	int level(Node<T>* root, Node<T>* tmep,int dep);//LEVEL函数
	int levelget(Node<T>* temp);//查找某一结点的层次，利用LEVEL函数
	void DataInput(string, int&, Node<T>*& sub);//建立树
	void LoadInput(string, string);//负载值输入,输入p为根结点
	int loadinput(Node<T>* p, char, int);//实际负载值输入函数
	void PrePrint(Node<T>* p);//先序遍历输出
	void InPrint(Node<T>* p);//中序遍历输出
	void PostPrint(Node<T>* p);//后序遍历输出
	void TreePrint(Node<T>* p);//打印树状结构
	void GetSumLoad(Node<T>*, int&);//统计该馈线的总供电负荷
	void LoadLoss(Node<T>* p,char error);//统计失负载结点和失负载总量,error为故障支路末端结点
	int searchpath(Node<T>* p, char search);//查询配电网中一负荷节点的供电路径，利用searchnode
	int searchnode(Node<T>* p, char search);//查询该路径是否含目标结点
	double SumDemand(Node<T>* p);//统计馈线的总供电需求
};

//string用法：字符串数组
template<typename T>NetTree<T>::NetTree() {
	root = NULL;
}

template<typename T>NetTree<T>::~NetTree() {
	clearTree(root);
	root = NULL;
}

template<typename T>void NetTree<T>::clearTree(Node<T>* p) {
	if (p) {
		if (p->lchild) clearTree(p->lchild);
		if (p->rchild) clearTree(p->rchild);
		delete p;
	}
}

template<typename T>int NetTree<T>::CreateTree() {
	cout << "生成馈线树,输入读取数据文件名:" << endl;
	string name;
	//getline(cin, name);//程序出错
	cin >> name;
	//数据读取输入
	ifstream file;
	file.open(name.c_str(), ios::in);
	if (!file) {
		cout << "file error" << endl;
		exit(1);
	}
	//建立树
	string d;
	getline(file, d, '\n');
	int i = 0;
	DataInput(d, i, root);

	//负载输入,a为结点编码，b为负载值
	string a, b;
	while ((file >> a) && (file >> b)) {
		LoadInput(a, b);
	}

	file.close();

	return 1;
}

template<typename T>int NetTree<T>::levelget(Node<T>* temp) {
	return level(root, temp, 1);
}

template<typename T>int NetTree<T>::level(Node<T>* p, Node<T>* temp, int dep) {
	if (p == temp) return dep;
	if (p) {
		int ldep = level(p->lchild, temp, dep + 1);
		if (!ldep) {
			ldep = level(p->rchild, temp, dep + 1);
		}
		return ldep;
	}
	else
		return  0;
}

//CreateTree(d, i, root);
template<typename T>void NetTree<T>::DataInput(string d, int& i, Node<T>*& sub) {
	{
		//建立节点,RefValue为‘#’
		if (d[i] != RefValue) {
			sub = new Node<T>(d[i]);
			i++;//建立成功后，数据后移
			DataInput(d, i, sub->lchild);
			DataInput(d, i, sub->rchild);
		}
		//输入为‘#’,不建立结点,指针指向空
		else {
			sub = NULL;
			i++;
		}
	}
}

template<typename T>void NetTree<T>::LoadInput(string a, string b) {
	//对负载字符进行处理,a0为a转成字符，d为转成数值的b
	char a0 = a[0];
	int d =0;
	for (int i= 0; i <b.length(); i++) {
		d=d*10+b[i]-48;
	}
	//负载值输入
	loadinput(root, a0, d);
}

template<typename T>int NetTree<T>::loadinput(Node<T>* p, char a, int d) {
	//利用先序遍历找a代表的结点；若为空，空操作
	if (p) {
		if (p->data== a) {
			p->load = d;
			//cout << "测试输入负载值" << a<<d << endl;
			return 1;//success
		}
		else {
			if(loadinput(p->lchild, a, d)) return 1;//success,quit
			if(loadinput(p->rchild, a, d)) return 1;//success,quit
			return 0;//fail,quit
		}
	}
	return 0;
}

template<typename T>void NetTree<T>::PrePrint(Node<T>* p) {
	if (p) {
		cout << p->data;
		PrePrint(p->lchild);
		PrePrint(p->rchild);
	}	
}

template<typename T>void NetTree<T>::InPrint(Node<T>* p) {
	if (p) {
		InPrint(p->lchild);
		cout << p->data;
		InPrint(p->rchild);
	}
}

template<typename T>void NetTree<T>::PostPrint(Node<T>* p) {
	if (p) {
		PostPrint(p->lchild);
		PostPrint(p->rchild);
		cout << p->data;
	}
}

template<typename T>void NetTree<T>::TreePrint(Node<T>* p) {
	cirqueue <Node<T>*> Q;
	if (p == NULL) return;
	Q.EnQueue(p);
	int level = 1;//当前打印层次
	while (!Q.IsEmpty()) {
		Node<T>* temp = Q.DeQueue();
		if (levelget(temp)!=level) {//当前结点为上一结点左孩子
			cout << endl;
			level++;
		}
		cout << temp->data;
		if (temp->lchild) {
			Q.EnQueue(temp->lchild);
		}
		if (temp->rchild) {
			Q.EnQueue(temp->rchild);
		}
	}	
}


//sum初始为0
template<typename T>void NetTree<T>::GetSumLoad(Node<T>* p,int&  sum) {
	if (p) {
		sum+=p->load;
		GetSumLoad(p->lchild,sum);
		GetSumLoad(p->rchild,sum);
	}
}

//loss初始为0
template<typename T>void NetTree<T>::LoadLoss(Node<T>* p, char error) {
	//利用先序遍历找error代表的结点；若为空，空操作
	if (p) {
		if (p->data == error) {
			cout << "失负荷结点:" << endl;
			PrePrint(p);//统计打印失负载结点，先序遍历输出
			cout << endl;
			int sum0 = 0;
			cout << "失负荷总量:" << endl;
			GetSumLoad(p, sum0);
			cout << sum0 << endl;
		}
		else {
			LoadLoss(p->lchild, error);
			LoadLoss(p->rchild, error);
		}
	}
}

//查询配电网中一负荷节点的供电路径
template<typename T>int NetTree<T>::searchpath(Node<T>* p,char search) {
	if (p) {
		if (p->data == search) {
			cout << p->data;
			return 1;
		}
		else {
			if (searchnode(p->lchild, search)) {//左孩子节点路径包含目标
				cout << p->data;
				searchpath(p->lchild, search);
				return 1;
			}
			if (searchnode(p->rchild, search)) {
				cout << p->data;
				searchpath(p->rchild, search);
				return 1;
			}
		}
	}
	return 0;
}

//查询该路径是否含目标结点
template<typename T>int NetTree<T>::searchnode(Node<T>* p, char search) {
	if (p) {
		if (p->data == search) return 1;
		int decider = searchnode(p->lchild, search);
		if (!decider) {
			decider = searchnode(p->rchild, search);
		}
		return decider;
	}
	else
		return  0;
}

//若每条支路产生的网损为其下层供电负荷的 0.1%，统计馈线的总供电需求；
template<typename T>double NetTree<T>::SumDemand(Node<T>* p) {
	if (p) {
		if ((!p->lchild) && (!p->rchild)) return p->load;
		double lsum=SumDemand(p->lchild)*1.1;
		double rsum=SumDemand(p->rchild)*1.1;
		return (lsum + rsum+p->load);
	}
	return 0;
}

void PrintMeue() {
	cout << "///////////////////菜单://////////////////" << endl;
	cout << "///////////////////生成馈线树:A//////////////////" << endl;
	cout << "///////////////////先序遍历:B//////////////////" << endl;
	cout << "///////////////////中序遍历:C//////////////////" << endl;
	cout << "///////////////////后序遍历:D//////////////////" << endl;
	cout << "///////////////////统计总供电负荷:E//////////////////" << endl;
	cout << "///////////////////查询失负荷信息:F//////////////////" << endl;
	cout << "////////////////// 打印馈线树状结构图:G//////////////////" << endl;
	cout << "////////////////// 查询负荷节点的供电路径:H//////////////////" << endl;	
	cout << "////////////////// 统计馈线的总供电需求:I//////////////////" << endl;	
}


int main() {
	NetTree<char> net0;	
	char ch;
	int cre=0;//判断生成树是否成功
	while (1) {
		PrintMeue();
		cout << "功能:" << endl;
		cin >> ch;
		switch (ch)
					{
		case 'A': {
			net0.~NetTree();
			cre=net0.CreateTree();	
			cout << "生成馈线树成功，输入功能:" << endl;
			break;
				}
		case 'B': {
			if (cre) {
				cout << "PrePrint" << endl;
				net0.PrePrint(net0.GetRoot());
				cout << endl;
				cout << endl << "输入功能:" << endl;				
			}	
			else 			cout << "未生成馈线树成功，重新输入功能:" << endl;

			break;

				}
		case 'C': {
			if (cre) {
				cout << "InPrint" << endl;
				net0.InPrint(net0.GetRoot());
				cout << endl;
				cout << endl << "输入功能:" << endl;
			}
			else 			cout << "未生成馈线树成功，重新输入功能:" << endl;

			break;
		}
		case 'D': {
			if (cre) {
				cout << "PostPrint" << endl;
				net0.PostPrint(net0.GetRoot());
				cout << endl;
				cout << endl << "输入功能:" << endl;
			}
			else 			cout << "未生成馈线树成功，重新输入功能:" << endl;

			break;
		}
		case 'E': {
			if (cre) {
				//统计该馈线的总供电负荷
				cout << "统计该馈线的总供电负荷:" << endl;
				int sum = 0;
				net0.GetSumLoad(net0.GetRoot(), sum);
				cout << sum << endl;
				cout << endl << "输入功能:" << endl;
			}
			else 			cout << "未生成馈线树成功，重新输入功能:" << endl;

			break;
		}
		case 'F': {
			if (cre) {
				/*查询失负荷信息：统计输出失负荷节点编号及失负荷总量；*/
				cout << "输入故障支路末端结点:" << endl;
				char error;
				cin >> error;
				net0.LoadLoss(net0.GetRoot(), error);
				cout << endl << "输入功能:" << endl;
			}
			else 			cout << "未生成馈线树成功，重新输入功能:" << endl;

			break;
		}
		case 'G': {
			if (cre) {
				cout << " 打印馈线树状结构图:" << endl;
				net0.TreePrint(net0.GetRoot());
				cout << endl << "输入功能:" << endl;
			}
			else 			cout << "未生成馈线树成功，重新输入功能:" << endl;

			break;
		}
		case 'H': {
			if (cre) {
				cout << " 查询负荷结点的供电路径:" << endl;
				cout << "输入查询负荷结点:" << endl;
				char search;
				cin >> search;
				net0.searchpath(net0.GetRoot(), search);
				cout << endl << "输入功能:" << endl;
			}
			else 		cout << "未生成馈线树成功，重新输入功能:" << endl;

			break;
		}
		case 'I': {
			if (cre) {
				cout << " 统计馈线的总供电需求:" << endl;
				cout<<net0.SumDemand(net0.GetRoot());
				cout << endl << "输入功能:" << endl;
			}
			else 			cout << "未生成馈线树成功，重新输入功能:" << endl;

			break;
		}
		default: {
						cout << "错误符号，重新输入功能:" << endl;
					}
		}
	}
	return 0;
}