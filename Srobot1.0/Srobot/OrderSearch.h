#pragma once
#ifndef _OrderSearch_H_
#define _OrderSearch_H_
//���ڲ�����������
#include<map>
#include<string>

class OrderSearch
{
public:
	OrderSearch();
	~OrderSearch();
	bool Search(std::string recorder, int &Device, int &order);
private:
	void Init();
	enum DeviceList { PhizModule, MoveModule, ArmModule };//ģ���б�
	enum ArmOrderList { GymSw, PiaffeSw, ActionRes, StopAll, HugArm, InviteArm };//�ֱ۶����б�
	struct device_order
	{
		int device;
		int order;
	};
	std::map<std::string, device_order> order_dic;//����ʣ�ģ�飬��������
};
#endif