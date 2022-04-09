#include <stdio.h>
#include "tinyxml.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream> // �ṩgetline()ȫ�ֺ���
using namespace std;

/*
	TiXmlDocument���ĵ��࣬������������xml�ļ�
	TiXmlDeclaration�������࣬����ʾ�ļ�����������
	TiXmlComment��ע���࣬����ʾ�ļ���ע�Ͳ���
	TiXmlElement��Ԫ���࣬�����ļ�����Ҫ���֣�����֧��Ƕ�׽ṹ��һ��ʹ�����ֽṹ������Ĵ洢��Ϣ�������԰�����������ı���
	TiXmlAttribute/TiXmlAttributeSet��Ԫ�����ԣ���һ��Ƕ����Ԫ���У����ڼ�¼��Ԫ�ص�һЩ����
	TiXmlText���ı�������Ƕ����ĳ��Ԫ���ڲ�
*/
//����xml�ļ�
int writeXmlFile()
{
	ifstream ifs("C:\\Users\\lenovo\\Desktop\\xml\\test_1.csv", ios::in);

	if (!ifs)
	{
		cout << "���ļ�ʧ�ܣ�" << endl;
		exit(1);
	}
	TiXmlDocument* writeDoc = new TiXmlDocument; //xml�ĵ�ָ��
	//�ĵ���ʽ����
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "yes");
	writeDoc->LinkEndChild(decl); //д���ĵ�


	TiXmlElement* RootElement = new TiXmlElement("Trajectory");//��Ԫ��
	RootElement->SetAttribute("angleUnit", "degree");//��γ�ȸ�ʽ
	RootElement->SetAttribute("distUnit", "m");//���ȸ�ʽ
	RootElement->SetAttribute("speed", "m/s");//�ٶȸ�ʽ��m/s
	RootElement->SetAttribute("DateTimeUnit", "UTC+8");//ʱ���ʽ UTC+8 ����Э��ʱ ������  2022-02-24 00:04(UTC+8)
	writeDoc->LinkEndChild(RootElement);

	int n = 3;	//���ڵ����

	int i = 0;
	string line;
	string field;
	while (getline(ifs, line))//getline(inFile, line)��ʾ���ж�ȡCSV�ļ��е�����
	{
		if (i>=2) //�ӵڶ��п�ʼ����
		{
			const char* a[4]; //���ڴ�Ŷ�ȡ����������
			string field_line, field_line1, field_line2, field_line3, field_line4;
			istringstream sin(line); //�������ַ���line���뵽�ַ�����sin��

			getline(sin, field_line, ','); //������һ������
			
			getline(sin, field_line1, ','); //���ַ�����sin�е��ַ����뵽field�ַ����У��Զ���Ϊ�ָ��� 
			a[0] = field_line1.c_str();		// ����  ���field_line.c_str()�仯����a[0]Ҳ�ᷢ���ı䣬��Ϊa[0]��ʾָ��

			getline(sin, field_line2, ','); //���ַ�����sin�е��ַ����뵽field�ַ����У��Զ���Ϊ�ָ��� 
			a[1] = field_line2.c_str();	   //γ��

			getline(sin, field_line3, ','); //���ַ�����sin�е��ַ����뵽field�ַ����У��Զ���Ϊ�ָ��� 
			a[2] = field_line3.c_str();	 //ʱ��

			getline(sin, field_line4, ','); //���ַ�����sin�е��ַ����뵽field�ַ����У��Զ���Ϊ�ָ���
			a[3] = field_line4.c_str();	 //�ٶ�

			TiXmlElement* TracElement = new TiXmlElement("TracPoint");//Stu
		     //��������	id
			TracElement->SetAttribute("id", i - 1);
			RootElement->LinkEndChild(TracElement);//���ڵ�д���ĵ�

			//����
			TiXmlElement* lonElement = new TiXmlElement("longitude");
			TracElement->LinkEndChild(lonElement);

			TiXmlText* lonContent = new TiXmlText(a[0]);
			lonElement->LinkEndChild(lonContent);

			//γ��
			TiXmlElement* latElement = new TiXmlElement("latitude");
			TracElement->LinkEndChild(latElement);

			TiXmlText* latContent = new TiXmlText(a[1]);
			latElement->LinkEndChild(latContent);

			//�ٶ�
			TiXmlElement* speedElement = new TiXmlElement("speed");
			TracElement->LinkEndChild(speedElement);

			TiXmlText* speedContent = new TiXmlText(a[2]);
			speedElement->LinkEndChild(speedContent);

			//ʱ��
			TiXmlElement* timeElement = new TiXmlElement("time");
			TracElement->LinkEndChild(timeElement);

			TiXmlText* timeContent = new TiXmlText(a[3]);
			timeElement->LinkEndChild(timeContent);
		}
		
		i++;
	}
	ifs.close();
	cout << "��ȡ�������" << endl;

	writeDoc->SaveFile("C:\\Users\\lenovo\\Desktop\\xml\\trackPoint_info.xml");
	delete writeDoc;

	return 1;
}

//����xml�ļ�
int readXmlFile()
{
	TiXmlDocument mydoc("C:\\Users\\lenovo\\Desktop\\xml\\trackPoint_info.xml");//xml�ĵ�����
	bool loadOk = mydoc.LoadFile();//�����ĵ�
	if (!loadOk)
	{
		cout << "could not load the test file.Error:" << mydoc.ErrorDesc() << endl;
		exit(1);
	}

	TiXmlElement* RootElement = mydoc.RootElement();	//��Ԫ��, Info
	cout << "[root name]" << RootElement->Value() << "\n";

	TiXmlElement* pEle = RootElement;

	//�����ý��
	for (TiXmlElement* TracElement = pEle->FirstChildElement();//��һ����Ԫ��
		TracElement != NULL;
		TracElement = TracElement->NextSiblingElement())//��һ���ֵ�Ԫ��
	{
		// TracElement->Value() �ڵ�����
		cout << TracElement->Value() << " ";
		TiXmlAttribute* pAttr = TracElement->FirstAttribute();//��һ������

		while (NULL != pAttr) //�����������
		{
			cout << pAttr->Name() << ":" << pAttr->Value() << " ";
			pAttr = pAttr->Next();
		}
		cout << endl;

		//�����Ԫ�ص�ֵ
		for (TiXmlElement* sonElement = TracElement->FirstChildElement();
			sonElement;
			sonElement = sonElement->NextSiblingElement())
		{
			cout << sonElement->FirstChild()->Value() << endl;
		}
		cout << endl;
	}

	return 1;
}

int main(int argc, char* argv[])
{

	writeXmlFile();
	printf("after write\n");

	readXmlFile();
	system("pause");

	return 0;
}