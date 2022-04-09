#include <stdio.h>
#include "tinyxml.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream> // 提供getline()全局函数
using namespace std;

/*
	TiXmlDocument：文档类，它代表了整个xml文件
	TiXmlDeclaration：声明类，它表示文件的声明部分
	TiXmlComment：注释类，它表示文件的注释部分
	TiXmlElement：元素类，它是文件的主要部分，并且支持嵌套结构，一般使用这种结构来分类的存储信息，它可以包含属性类和文本类
	TiXmlAttribute/TiXmlAttributeSet：元素属性，它一般嵌套在元素中，用于记录此元素的一些属性
	TiXmlText：文本对象，它嵌套在某个元素内部
*/
//创建xml文件
int writeXmlFile()
{
	ifstream ifs("C:\\Users\\lenovo\\Desktop\\xml\\test_1.csv", ios::in);

	if (!ifs)
	{
		cout << "打开文件失败！" << endl;
		exit(1);
	}
	TiXmlDocument* writeDoc = new TiXmlDocument; //xml文档指针
	//文档格式声明
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "yes");
	writeDoc->LinkEndChild(decl); //写入文档


	TiXmlElement* RootElement = new TiXmlElement("Trajectory");//根元素
	RootElement->SetAttribute("angleUnit", "degree");//经纬度格式
	RootElement->SetAttribute("distUnit", "m");//长度格式
	RootElement->SetAttribute("speed", "m/s");//速度格式：m/s
	RootElement->SetAttribute("DateTimeUnit", "UTC+8");//时间格式 UTC+8 世界协调时 东八区  2022-02-24 00:04(UTC+8)
	writeDoc->LinkEndChild(RootElement);

	int n = 3;	//父节点个数

	int i = 0;
	string line;
	string field;
	while (getline(ifs, line))//getline(inFile, line)表示按行读取CSV文件中的数据
	{
		if (i>=2) //从第二行开始读起
		{
			const char* a[4]; //用于存放读取出来的数据
			string field_line, field_line1, field_line2, field_line3, field_line4;
			istringstream sin(line); //将整行字符串line读入到字符串流sin中

			getline(sin, field_line, ','); //跳过第一、二列
			
			getline(sin, field_line1, ','); //将字符串流sin中的字符读入到field字符串中，以逗号为分隔符 
			a[0] = field_line1.c_str();		// 经度  如果field_line.c_str()变化，则a[0]也会发生改变，因为a[0]表示指针

			getline(sin, field_line2, ','); //将字符串流sin中的字符读入到field字符串中，以逗号为分隔符 
			a[1] = field_line2.c_str();	   //纬度

			getline(sin, field_line3, ','); //将字符串流sin中的字符读入到field字符串中，以逗号为分隔符 
			a[2] = field_line3.c_str();	 //时间

			getline(sin, field_line4, ','); //将字符串流sin中的字符读入到field字符串中，以逗号为分隔符
			a[3] = field_line4.c_str();	 //速度

			TiXmlElement* TracElement = new TiXmlElement("TracPoint");//Stu
		     //设置属性	id
			TracElement->SetAttribute("id", i - 1);
			RootElement->LinkEndChild(TracElement);//父节点写入文档

			//经度
			TiXmlElement* lonElement = new TiXmlElement("longitude");
			TracElement->LinkEndChild(lonElement);

			TiXmlText* lonContent = new TiXmlText(a[0]);
			lonElement->LinkEndChild(lonContent);

			//纬度
			TiXmlElement* latElement = new TiXmlElement("latitude");
			TracElement->LinkEndChild(latElement);

			TiXmlText* latContent = new TiXmlText(a[1]);
			latElement->LinkEndChild(latContent);

			//速度
			TiXmlElement* speedElement = new TiXmlElement("speed");
			TracElement->LinkEndChild(speedElement);

			TiXmlText* speedContent = new TiXmlText(a[2]);
			speedElement->LinkEndChild(speedContent);

			//时间
			TiXmlElement* timeElement = new TiXmlElement("time");
			TracElement->LinkEndChild(timeElement);

			TiXmlText* timeContent = new TiXmlText(a[3]);
			timeElement->LinkEndChild(timeContent);
		}
		
		i++;
	}
	ifs.close();
	cout << "读取数据完成" << endl;

	writeDoc->SaveFile("C:\\Users\\lenovo\\Desktop\\xml\\trackPoint_info.xml");
	delete writeDoc;

	return 1;
}

//解析xml文件
int readXmlFile()
{
	TiXmlDocument mydoc("C:\\Users\\lenovo\\Desktop\\xml\\trackPoint_info.xml");//xml文档对象
	bool loadOk = mydoc.LoadFile();//加载文档
	if (!loadOk)
	{
		cout << "could not load the test file.Error:" << mydoc.ErrorDesc() << endl;
		exit(1);
	}

	TiXmlElement* RootElement = mydoc.RootElement();	//根元素, Info
	cout << "[root name]" << RootElement->Value() << "\n";

	TiXmlElement* pEle = RootElement;

	//遍历该结点
	for (TiXmlElement* TracElement = pEle->FirstChildElement();//第一个子元素
		TracElement != NULL;
		TracElement = TracElement->NextSiblingElement())//下一个兄弟元素
	{
		// TracElement->Value() 节点名称
		cout << TracElement->Value() << " ";
		TiXmlAttribute* pAttr = TracElement->FirstAttribute();//第一个属性

		while (NULL != pAttr) //输出所有属性
		{
			cout << pAttr->Name() << ":" << pAttr->Value() << " ";
			pAttr = pAttr->Next();
		}
		cout << endl;

		//输出子元素的值
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