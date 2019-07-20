/*
MIT License

Copyright  (c) 2009-2019 easyice

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/* Generated by Together */

#include "StdAfx.h"
#include "CTabTSDT.h"

tables::CTabTSDT::~CTabTSDT()
{

}

tables::CTabTSDT::CTabTSDT()
{

}

void tables::CTabTSDT::Parse(const SECTION& section, TABLES* tables)
{

}

void tables::CTabTSDT::Parse(const TABLE_SECTIONS& table_sections, TABLES* tables)
{
	STU_SECTION_TSDT tabTSDT;

	TABLE_SECTIONS::const_iterator it;

	for (it = table_sections.begin(); it != table_sections.end(); it++)
	{
		ParseSection(it->vecData,tabTSDT);
	}
	if (tabTSDT.empty())
	{
		return;
	}

	bool bfinded_tableid = false;
	vector<STU_SECTION_TSDT>::iterator it_tab = tables->vecTabTSDT.begin();
	for (; it_tab != tables->vecTabTSDT.end(); ++it_tab)
	{
		if (it_tab->empty())
		{
			continue;
		}
		if (it_tab->begin()->table_id != tabTSDT.begin()->table_id)
		{
			continue;
		}

		bfinded_tableid = true;

		bool b_section_num_exist = false;
		bool b_version_num_exist = false;

		STU_SECTION_TSDT::iterator it_sec = it_tab->begin();

		for (; it_sec != it_tab->end(); ++it_sec)
		{
			if (it_sec->section_number == tabTSDT.begin()->section_number)
			{
				b_section_num_exist = true;
			}
			
			if (it_sec->version_number == tabTSDT.begin()->version_number)
			{
				b_version_num_exist = true;;
			}
		}
		
		//�����Ҫ������ͬ�汾�Ŵ�ע�͡��ݲ������汾��
		if ((it_tab->begin()->table_id == tabTSDT.begin()->table_id )&& ( !b_section_num_exist /*|| !b_version_num_exist*/ ))
		{
			
			it_tab->push_back(tabTSDT.front());
			DecodeDescriptors(it_tab->back());
			return;
		}
	
	}

	if (!bfinded_tableid)
	{
		
		tables->vecTabTSDT.push_back(tabTSDT);
		DecodeDescriptors(tables->vecTabTSDT.back().back());
	}
}

void tables::CTabTSDT::ParseSection(const vector<BYTE>& vecData,STU_SECTION_TSDT& tabTSDT)
{
	TSDT   t;
	int	len1;
	u_char* b = (u_char*)&vecData[0];

	t.table_id 			 = b[0];
	t.section_syntax_indicator	 = CBit::getBits (b, 0, 8, 1);
	t.reserved_1 			 = CBit::getBits (b, 0, 9, 1);
	t.reserved_2 			 = CBit::getBits (b, 0, 10, 2);
	t.section_length		 = CBit::getBits (b, 0, 12, 12);
	t.reserved_3 			 = CBit::getBits (b, 0, 24, 18);
	t.version_number 		 = CBit::getBits (b, 0, 42, 5);
	t.current_next_indicator	 = CBit::getBits (b, 0, 47, 1);
	t.section_number 		 = CBit::getBits (b, 0, 48, 8);
	t.last_section_number 		 = CBit::getBits (b, 0, 56, 8);

	b   += 8;
	len1 = t.section_length - 5;

	if (t.table_id != 0x03)
	{
		return;
	}

	//t.descriptors = CDescriptor::GetInstancePtr()->DecodeDescriptor(b,len1-4);
	if (len1-4 > 0)
	{
		t.vec_descriptor.resize(len1-4);
		memcpy(&t.vec_descriptor[0],b,len1-4);
	}

	while (len1 > 4)
	{
		int x = 1;

		//x = descriptor (b, MPEG);
		//t.vec_descriptor.push_back(*b);
		b   += x;
		len1 -= x;
	}

	t.crc		 = CBit::getBits (b, 0, 0, 32);

	tabTSDT.push_back(t);
}

//void tables::CTabTSDT::DecodeDescriptors(STU_SECTION_TSDT& tabTSDT)
//{
//	if (tabTSDT.empty())
//	{
//		return;
//	}
//
//	if (!tabTSDT.begin()->vec_descriptor.empty())
//	{
//		CDescriptor::GetInstancePtr()->DecodeDescriptor(tabTSDT.begin()->descriptors,&tabTSDT.begin()->vec_descriptor[0],(int)tabTSDT.begin()->vec_descriptor.size());	
//	}
//}
//
//

void tables::CTabTSDT::DecodeDescriptors(TSDT& tsdt)
{
	if (!tsdt.vec_descriptor.empty())
	{
		CDescriptor::GetInstancePtr()->DecodeDescriptor(tsdt.descriptors,&tsdt.vec_descriptor[0],(int)tsdt.vec_descriptor.size());	
	}
}


