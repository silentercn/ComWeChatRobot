#include "pch.h"
#include <vector>

wstring selfinfo = L"";

struct SelfInfoStruct {
	DWORD message;
	DWORD length;
} ret;

// 这里有待解决的BUG
DWORD GetSelfInfoRemote() {
	DWORD WeChatWinBase = GetWeChatWinBase();
	/*vector<DWORD> SelfInfoAddr = {
		*(DWORD*)(WeChatWinBase + 0x21DC9C4),
		WeChatWinBase + 0x21DCBB8,
		*(DWORD*)(WeChatWinBase + 0x21DCA3C),
		*(DWORD*)(WeChatWinBase + 0x21DCB74),
		*(DWORD*)(WeChatWinBase + 0x21DCD34),
		*(DWORD*)(WeChatWinBase + 0x21DCD1C),
		WeChatWinBase + 0x21DCC30,
		WeChatWinBase + 0x21DCB44,
		WeChatWinBase + 0x21DCB5C,
		WeChatWinBase + 0x21DCA70
	};*/
	vector<DWORD> SelfInfoAddr = {
		*(DWORD*)(WeChatWinBase + 0x21DC9C4),
		WeChatWinBase + 0x21DCBB8,
		WeChatWinBase + 0x21DCA3C,
		*(DWORD*)(WeChatWinBase + 0x21DCB74),
		*(DWORD*)(WeChatWinBase + 0x21DCD34),
		*(DWORD*)(WeChatWinBase + 0x21DCD1C),
		WeChatWinBase + 0x21DCC30,
		WeChatWinBase + 0x21DCB44,
		WeChatWinBase + 0x21DCB5C,
		WeChatWinBase + 0x21DCA70
	};

	vector<wstring> SelfInfoKey = {
		L"\"wxId\"",
		L"\"wxNumber\"",
		L"\"wxNickName\"",
		L"\"wxSignature\"",
		L"\"wxBigAvatar\"",
		L"\"wxSmallAvatar\"",
		L"\"wxNation\"",
		L"\"wxProvince\"",
		L"\"wxCity\"",
		L"\"PhoneNumber\""
	};
#ifdef _DEBUG
	wcout.imbue(locale("chs"));
#endif
	selfinfo = selfinfo + L"{";
	for (unsigned int i = 0; i < SelfInfoAddr.size(); i++) {
		selfinfo = selfinfo + SelfInfoKey[i] + L":";
		selfinfo = selfinfo + L"\"";
		char* temp = NULL;
		if (!SelfInfoKey[i].compare(L"\"wxNickName\"")) {
			if (*(DWORD*)(SelfInfoAddr[i] + 0x14) == 0xF) {
				temp = (*((DWORD*)SelfInfoAddr[i]) != 0) ? (char*)SelfInfoAddr[i] : (char*)"null";
			}
			else {
				temp = (*((DWORD*)SelfInfoAddr[i]) != 0) ? (char*)(*(DWORD*)SelfInfoAddr[i]) : (char*)"null";
			}
		}
		else {
			temp = (char*)SelfInfoAddr[i];
			if (strlen(temp) == 0)
				temp = (char*)"null";
		}
		wchar_t* wtemp = new wchar_t[strlen(temp) + 1];
		ZeroMemory(wtemp, (strlen(temp) + 1) * 2);
		MultiByteToWideChar(CP_UTF8, 0, temp, -1, wtemp, strlen(temp) + 1);
		selfinfo = selfinfo + wtemp;
		selfinfo = selfinfo + L"\"";
		if(i!= SelfInfoAddr.size() - 1)
			selfinfo = selfinfo + L",";
		delete[] wtemp;
		wtemp = NULL;
	}
	selfinfo = selfinfo + L"}";
	ret.message = (DWORD)selfinfo.c_str();
	ret.length = selfinfo.length();
#ifdef _DEBUG
	wcout << selfinfo << endl;
	DeleteSelfInfoCacheRemote();
#endif
	return (DWORD)&ret;
}

VOID DeleteSelfInfoCacheRemote() {
	if (ret.length) {
		ZeroMemory((wchar_t*)ret.message, ret.length*2 + 2);
		ret.length = 0;
		selfinfo = L"";
	}
}