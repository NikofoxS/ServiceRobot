#include"stdafx.h"
#include"OrderSearch.h"

OrderSearch::OrderSearch()
{
	Init();
}

OrderSearch::~OrderSearch()
{
	order_dic.clear();
}
void OrderSearch::Init()
{
	order_dic["ɽ���Ƽ���ѧ��"] = { PhizModule,18 };
	order_dic["ǰ����"] = { MoveModule,1 }; order_dic["���ˡ�"] = { MoveModule,2 }; order_dic["��ת��"] = { MoveModule,3 };
	order_dic["��ת��"] = { MoveModule,4 }; order_dic["ֹͣ��"] = { MoveModule,0 };
	order_dic["�ɰ���"] = { PhizModule,1 }; order_dic["������"] = { PhizModule,2 }; order_dic["���ᡣ"] = { PhizModule,3 }; order_dic["͵Ц��"] = { PhizModule,4 };
	order_dic["���ȡ�"] = { PhizModule,5 }; order_dic["Ц����"] = { PhizModule,6 }; order_dic["�塣"] = { PhizModule,7 }; order_dic["��Ƥ��"] = { PhizModule,8 };
	order_dic["���ˡ�"] = { PhizModule,9 }; order_dic["���ǡ�"] = { PhizModule,10 }; order_dic["���ܡ�"] = { PhizModule,12 }; order_dic["��ŭ��"] = { PhizModule,11 };
	order_dic["�㲥��١�"] = { ArmModule,GymSw }; order_dic["ԭ��̤����"] = { ArmModule,PiaffeSw }; order_dic["��λ��"] = { ArmModule,ActionRes };
	order_dic["ֹͣ������"] = { ArmModule,StopAll }; order_dic["ӵ����"] = { ArmModule,HugArm }; order_dic["�������롣"] = { ArmModule,InviteArm };
}
bool OrderSearch::Search(std::string recorder, int &Device, int &order)
{
	std::map<std::string, device_order>::iterator result;
	result = order_dic.find(recorder);
	if (result == order_dic.end())//û�ҵ�
	{
		return false;
	}
	else//�ҵ� 
	{
		Device = result->second.device;
		order = result->second.order;
	}
	return true;
}