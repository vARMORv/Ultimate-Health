#include "main.h"
#include "Library.hpp"

int healthTopColor = { 0 };
int healthDownColor = { 0 };
int CPlayerTags__DrawHealthBar = { 0 };

template <typename T>
inline void write_memory(std::uintptr_t address, T value, bool protect = true) {
	unsigned long oldProt;
	if (protect) VirtualProtect(reinterpret_cast<void*>(address), sizeof(T), PAGE_READWRITE, &oldProt);
	*reinterpret_cast<T*>(address) = value;
	if (protect) VirtualProtect(reinterpret_cast<void*>(address), sizeof(T), oldProt, &oldProt);
}

void __cdecl CDrawHealthBar(float x, float y, unsigned short width, unsigned char height, float progress, signed char progressAdd, unsigned char drawPercentage, unsigned char drawBlackBorder, unsigned long color, unsigned long addColor) {
	if (progress > 50)
		color = 0xFF4ABF2B;
	else if (progress > 20 && progress < 80)
		color = 0xFF45C9D4;
	else
		color = 0xFF1D19B4;
	DrawBarHook->call(x, y, width, height, progress, progressAdd, drawPercentage, drawBlackBorder, color, addColor);
}

void __fastcall CPlayerTagHook(void* this_, void* EDX, CVector* possition, float health, float armour, float dist) {
	if (health > 80) {
		write_memory(SAMP::GetSAMPHandle() + healthTopColor, 0xFF2BBF4A, true);
		write_memory(SAMP::GetSAMPHandle() + healthDownColor, 0xFF155F25, true);
	}
	else if (health > 20 && health < 80){
		write_memory(SAMP::GetSAMPHandle() + healthTopColor, 0xFFD4C945, true);
		write_memory(SAMP::GetSAMPHandle() + healthDownColor, 0xFF6A6422, true);
	}
	else{
		write_memory(SAMP::GetSAMPHandle() + healthTopColor, 0xFFB4191D, true);
		write_memory(SAMP::GetSAMPHandle() + healthDownColor, 0xFF5A0C0E, true);
	}
	PlayerTags__HealthHook->call<void*, CVector*, float, float, float>(this_, possition, health, armour, dist);
}

void GameloopHooked() {
	static bool IsInitialized = false;
	
	if (!IsInitialized && SAMP::IsSAMPInitialized()) {
		switch (SAMP::GetSAMPVersion()) {
		case (SAMP::sampVersion::R1): {
			CPlayerTags__DrawHealthBar = 0x689C0;
			healthTopColor = 0x68B0C;
			healthDownColor = 0x68B33;
			break;
		}
		case (SAMP::sampVersion::R2): {
			CPlayerTags__DrawHealthBar = 0x68A90;
			healthTopColor = 0x68BDC;
			healthDownColor = 0x68C03;
			break;
		}
		case (SAMP::sampVersion::R3): {
			CPlayerTags__DrawHealthBar = 0x6C930;
			healthTopColor = 0x6CA7C;
			healthDownColor = 0x6CAA3;
			break;
		}
		case (SAMP::sampVersion::R4): {
			CPlayerTags__DrawHealthBar = 0x6D060;
			healthTopColor = 0x6D1AC;
			healthDownColor = 0x6D1D3;
			break;

		}
		case (SAMP::sampVersion::DL): {
			CPlayerTags__DrawHealthBar = 0x6CAE0;
			healthTopColor = 0x6CC2C;
			healthDownColor = 0x6CC53;
			break;
		}
		default: {
			break;
		}
		}

                if (CPlayerTags__DrawHealthBar != 0 ){
                    PlayerTags__HealthHook = std::make_unique<memwrapper::memhook<CPlayerTags__Prototype>>(SAMP::GetSAMPHandle() + CPlayerTags__DrawHealthBar, &CPlayerTagHook);
                    PlayerTags__HealthHook->install();
                }
		

		DrawBarHook = std::make_unique<memwrapper::memhook<CSprite2d__DrawBarChart>>(0x589395, &CDrawHealthBar);
		DrawBarHook->install();
		IsInitialized = true;
	}
	GameloopHook->call();
}

AsiPlugin::AsiPlugin() {
	GameloopHook = std::make_unique<memwrapper::memhook<GameloopPrototype>>(0x748DA3, &GameloopHooked);
	GameloopHook->install();
}

AsiPlugin::~AsiPlugin() {
	DrawBarHook->remove();
	GameloopHook->remove();
	PlayerTags__HealthHook->remove();
}
