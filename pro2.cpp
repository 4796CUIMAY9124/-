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
	Node<T>* GetRoot() { return root; }//���ظ����
	int CreateTree();
	int level(Node<T>* root, Node<T>* tmep,int dep);//LEVEL����
	int levelget(Node<T>* temp);//����ĳһ���Ĳ�Σ�����LEVEL����
	void DataInput(string, int&, Node<T>*& sub);//������
	void LoadInput(string, string);//����ֵ����,����pΪ�����
	int loadinput(Node<T>* p, char, int);//ʵ�ʸ���ֵ���뺯��
	void PrePrint(Node<T>* p);//����������
	void InPrint(Node<T>* p);//����������
	void PostPrint(Node<T>* p);//����������
	void TreePrint(Node<T>* p);//��ӡ��״�ṹ
	void GetSumLoad(Node<T>*, int&);//ͳ�Ƹ����ߵ��ܹ��縺��
	void LoadLoss(Node<T>* p,char error);//ͳ��ʧ���ؽ���ʧ��������,errorΪ����֧·ĩ�˽��
	int searchpath(Node<T>* p, char search);//��ѯ�������һ���ɽڵ�Ĺ���·��������searchnode
	int searchnode(Node<T>* p, char search);//��ѯ��·���Ƿ�Ŀ����
	double SumDemand(Node<T>* p);//ͳ�����ߵ��ܹ�������
};

//string�÷����ַ�������
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
	cout << "����������,�����ȡ�����ļ���:" << endl;
	string name;
	//getline(cin, name);//�������
	cin >> name;
	//���ݶ�ȡ����
	ifstream file;
	file.open(name.c_str(), ios::in);
	if (!file) {
		cout << "file error" << endl;
		exit(1);
	}
	//������
	string d;
	getline(file, d, '\n');
	int i = 0;
	DataInput(d, i, root);

	//��������,aΪ�����룬bΪ����ֵ
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
		//�����ڵ�,RefValueΪ��#��
		if (d[i] != RefValue) {
			sub = new Node<T>(d[i]);
			i++;//�����ɹ������ݺ���
			DataInput(d, i, sub->lchild);
			DataInput(d, i, sub->rchild);
		}
		//����Ϊ��#��,���������,ָ��ָ���
		else {
			sub = NULL;
			i++;
		}
	}
}

template<typename T>void NetTree<T>::LoadInput(string a, string b) {
	//�Ը����ַ����д���,a0Ϊaת���ַ���dΪת����ֵ��b
	char a0 = a[0];
	int d =0;
	for (int i= 0; i <b.length(); i++) {
		d=d*10+b[i]-48;
	}
	//����ֵ����
	loadinput(root, a0, d);
}

template<typename T>int NetTree<T>::loadinput(Node<T>* p, char a, int d) {
	//�������������a����Ľ�㣻��Ϊ�գ��ղ���
	if (p) {
		if (p->data== a) {
			p->load = d;
			//cout << "�������븺��ֵ" << a<<d << endl;
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
	int level = 1;//��ǰ��ӡ���
	while (!Q.IsEmpty()) {
		Node<T>* temp = Q.DeQueue();
		if (levelget(temp)!=level) {//��ǰ���Ϊ��һ�������
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


//sum��ʼΪ0
template<typename T>void NetTree<T>::GetSumLoad(Node<T>* p,int&  sum) {
	if (p) {
		sum+=p->load;
		GetSumLoad(p->lchild,sum);
		GetSumLoad(p->rchild,sum);
	}
}

//loss��ʼΪ0
template<typename T>void NetTree<T>::LoadLoss(Node<T>* p, char error) {
	//�������������error����Ľ�㣻��Ϊ�գ��ղ���
	if (p) {
		if (p->data == error) {
			cout << "ʧ���ɽ��:" << endl;
			PrePrint(p);//ͳ�ƴ�ӡʧ���ؽ�㣬����������
			cout << endl;
			int sum0 = 0;
			cout << "ʧ��������:" << endl;
			GetSumLoad(p, sum0);
			cout << sum0 << endl;
		}
		else {
			LoadLoss(p->lchild, error);
			LoadLoss(p->rchild, error);
		}
	}
}

//��ѯ�������һ���ɽڵ�Ĺ���·��
template<typename T>int NetTree<T>::searchpath(Node<T>* p,char search) {
	if (p) {
		if (p->data == search) {
			cout << p->data;
			return 1;
		}
		else {
			if (searchnode(p->lchild, search)) {//���ӽڵ�·������Ŀ��
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

//��ѯ��·���Ƿ�Ŀ����
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

//��ÿ��֧·����������Ϊ���²㹩�縺�ɵ� 0.1%��ͳ�����ߵ��ܹ�������
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
	cout << "///////////////////�˵�://////////////////" << endl;
	cout << "///////////////////����������:A//////////////////" << endl;
	cout << "///////////////////�������:B//////////////////" << endl;
	cout << "///////////////////�������:C//////////////////" << endl;
	cout << "///////////////////�������:D//////////////////" << endl;
	cout << "///////////////////ͳ���ܹ��縺��:E//////////////////" << endl;
	cout << "///////////////////��ѯʧ������Ϣ:F//////////////////" << endl;
	cout << "////////////////// ��ӡ������״�ṹͼ:G//////////////////" << endl;
	cout << "////////////////// ��ѯ���ɽڵ�Ĺ���·��:H//////////////////" << endl;	
	cout << "////////////////// ͳ�����ߵ��ܹ�������:I//////////////////" << endl;	
}


int main() {
	NetTree<char> net0;	
	char ch;
	int cre=0;//�ж��������Ƿ�ɹ�
	while (1) {
		PrintMeue();
		cout << "����:" << endl;
		cin >> ch;
		switch (ch)
					{
		case 'A': {
			net0.~NetTree();
			cre=net0.CreateTree();	
			cout << "�����������ɹ������빦��:" << endl;
			break;
				}
		case 'B': {
			if (cre) {
				cout << "PrePrint" << endl;
				net0.PrePrint(net0.GetRoot());
				cout << endl;
				cout << endl << "���빦��:" << endl;				
			}	
			else 			cout << "δ�����������ɹ����������빦��:" << endl;

			break;

				}
		case 'C': {
			if (cre) {
				cout << "InPrint" << endl;
				net0.InPrint(net0.GetRoot());
				cout << endl;
				cout << endl << "���빦��:" << endl;
			}
			else 			cout << "δ�����������ɹ����������빦��:" << endl;

			break;
		}
		case 'D': {
			if (cre) {
				cout << "PostPrint" << endl;
				net0.PostPrint(net0.GetRoot());
				cout << endl;
				cout << endl << "���빦��:" << endl;
			}
			else 			cout << "δ�����������ɹ����������빦��:" << endl;

			break;
		}
		case 'E': {
			if (cre) {
				//ͳ�Ƹ����ߵ��ܹ��縺��
				cout << "ͳ�Ƹ����ߵ��ܹ��縺��:" << endl;
				int sum = 0;
				net0.GetSumLoad(net0.GetRoot(), sum);
				cout << sum << endl;
				cout << endl << "���빦��:" << endl;
			}
			else 			cout << "δ�����������ɹ����������빦��:" << endl;

			break;
		}
		case 'F': {
			if (cre) {
				/*��ѯʧ������Ϣ��ͳ�����ʧ���ɽڵ��ż�ʧ����������*/
				cout << "�������֧·ĩ�˽��:" << endl;
				char error;
				cin >> error;
				net0.LoadLoss(net0.GetRoot(), error);
				cout << endl << "���빦��:" << endl;
			}
			else 			cout << "δ�����������ɹ����������빦��:" << endl;

			break;
		}
		case 'G': {
			if (cre) {
				cout << " ��ӡ������״�ṹͼ:" << endl;
				net0.TreePrint(net0.GetRoot());
				cout << endl << "���빦��:" << endl;
			}
			else 			cout << "δ�����������ɹ����������빦��:" << endl;

			break;
		}
		case 'H': {
			if (cre) {
				cout << " ��ѯ���ɽ��Ĺ���·��:" << endl;
				cout << "�����ѯ���ɽ��:" << endl;
				char search;
				cin >> search;
				net0.searchpath(net0.GetRoot(), search);
				cout << endl << "���빦��:" << endl;
			}
			else 		cout << "δ�����������ɹ����������빦��:" << endl;

			break;
		}
		case 'I': {
			if (cre) {
				cout << " ͳ�����ߵ��ܹ�������:" << endl;
				cout<<net0.SumDemand(net0.GetRoot());
				cout << endl << "���빦��:" << endl;
			}
			else 			cout << "δ�����������ɹ����������빦��:" << endl;

			break;
		}
		default: {
						cout << "������ţ��������빦��:" << endl;
					}
		}
	}
	return 0;
}