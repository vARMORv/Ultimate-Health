#ifndef _SAMP_LIBRARY_HPP_
#define _SAMP_LIBRARY_HPP_

#include <Windows.h>

void* pChat = nullptr;
int baseAddress = reinterpret_cast<unsigned long>(GetModuleHandle("samp.dll"));
namespace SAMP
{
	enum class sampVersion : int
	{
		unknown = 0,
		notLoaded = -1,
		R1 = 1,
		R2 = 2,
		R3 = 3,
		R4 = 4,
		DL = 5
	}; // enum class sampVersion
	inline unsigned long GetSAMPHandle() {
		static unsigned long samp{ 0 };
		if (!samp) {
			samp = reinterpret_cast<unsigned long>(GetModuleHandleA("samp"));
			if (!samp || samp == -1 || reinterpret_cast<HANDLE>(samp) == INVALID_HANDLE_VALUE) {
				samp = 0;
			}
		}
		return samp;
	}

	inline unsigned long Library() {
            static DWORD samp = 0;
            if (!samp) {
                samp = (DWORD) GetModuleHandleA("samp");
                if (samp == 0xFFFFFFFF)
                    samp = 0;
            }
            return samp;
        }

	inline enum sampVersion GetSAMPVersion() {
		static sampVersion sampVersion = sampVersion::unknown;
		if (sampVersion != sampVersion::unknown) {
			return sampVersion;
		}
		unsigned long samp = GetSAMPHandle();
		if (!samp) {
			sampVersion = sampVersion::notLoaded;
		}
		else {
			unsigned long EntryPoint = reinterpret_cast<IMAGE_NT_HEADERS*>(samp + reinterpret_cast<IMAGE_DOS_HEADER*>(samp)->e_lfanew)->OptionalHeader.AddressOfEntryPoint;
			switch (EntryPoint) {
			case (0x31DF13): {
				sampVersion = sampVersion::R1;
				break;
			}
			case (0x3195DD): {
				sampVersion = sampVersion::R2;
				break;
			}
			case (0xCC4D0): {
				sampVersion = sampVersion::R3;
                pChat = reinterpret_cast<void*>(GetModuleHandleA("samp.dll") + 0x26E9F8);
				break;
			}
			case (0xCBCB0): {
				sampVersion = sampVersion::R4;
				break;
			}
			case (0xFDB60): {
				sampVersion = sampVersion::DL;
				break;
			}
			default: {
				sampVersion = sampVersion::unknown;
				break;
			}
			}
		}
		return sampVersion;
	}
	inline bool IsSAMPInitialized() {
		if (GetSAMPVersion() == sampVersion::R1)
			return *reinterpret_cast<void**>(GetSAMPHandle() + 0x21A0F8) != nullptr;
		else if (GetSAMPVersion() == sampVersion::R2)
			return *reinterpret_cast<void**>(GetSAMPHandle() + 0x21A100) != nullptr;
		else if (GetSAMPVersion() == sampVersion::R3)
			return *reinterpret_cast<void**>(GetSAMPHandle() + 0x26E8DC) != nullptr;
		else if (GetSAMPVersion() == sampVersion::R4)
			return *reinterpret_cast<void**>(GetSAMPHandle() + 0x26EA0C) != nullptr;
		else if (GetSAMPVersion() == sampVersion::DL)
			return *reinterpret_cast<void**>(GetSAMPHandle() + 0x2ACA24) != nullptr;
		return false;
	}

	void addChatMessage(unsigned long ulColor, const char* szText) {
        reinterpret_cast<void(__thiscall*)(void*, int, const char*, const char*, unsigned long, unsigned long)>(GetModuleHandleA("samp.dll") + 0x67460)(pChat, 4, szText, "", ulColor, 0);
    }
}; // namespace SAMP


#endif // !_SAMP_LIBRARY_HPP_