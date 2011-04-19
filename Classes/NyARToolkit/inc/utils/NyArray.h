#pragma once
namespace NyARToolkitCPP
{
	template <class T> class NyArray
	{
	public:
		int length;
		T* item;
	public:
		NyArray()
		{
			this->item=new T[32];
			this->length=32;
			return;
		}
		NyArray(int i_length)
		{
			this->item=new T[i_length];
			this->length=i_length;
			return;
		}
		virtual ~NyArray()
		{
			delete this->item;
			return;
		}
		virtual void renew(int i_new_length)
		{
			T* new_item=new T[i_new_length];
			delete this->item;
			this->item=new_item;
			this->length=i_new_length;
			return;
		}
	};
}
