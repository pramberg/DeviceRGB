#pragma once
#include "CoreMinimal.h"
#include "RazerController.h"
#include <InputCoreTypes.h>

class FRazerKeyConverter
{
public:
	template<ERazerDeviceType DeviceType>
	static FKey ToFKey(const FIntPoint& InPos) { return EKeys::Invalid; }

	template<>
	static FKey ToFKey<ERazerDeviceType::Keyboard>(const FIntPoint& InPos)
	{
		// This obviously doesn't work properly, but it does to a certain extent...
		// This whole thing should be reworked to use additional effects for different layers on Razer devices.
		if (InPos.Y == 1)
		{
			switch (InPos.X)
			{
			case 2:
				return EKeys::Escape;
			case 4:
				return EKeys::F1;
			case 5:
				return EKeys::F2;
			case 6:
				return EKeys::F3;
			case 7:
				return EKeys::F4;
			case 8:
				return EKeys::F5;
			case 9:
				return EKeys::F6;
			case 10:
				return EKeys::F7;
			case 11:
				return EKeys::F8;
			case 12:
				return EKeys::F9;
			case 13:
				return EKeys::F10;
			case 14:
				return EKeys::F11;
			case 15:
				return EKeys::F12;
			}
		}

		if (InPos.Y == 2)
		{
			switch (InPos.X)
			{
			case 2:
				return EKeys::Tilde;
			case 3:
				return EKeys::One;
			case 4:
				return EKeys::Two;
			case 5:
				return EKeys::Three;
			case 6:
				return EKeys::Four;
			case 7:
				return EKeys::Five;
			case 8:
				return EKeys::Six;
			case 9:
				return EKeys::Seven;
			case 10:
				return EKeys::Eight;
			case 11:
				return EKeys::Nine;
			case 12:
				return EKeys::Zero;
			}
		}

		if (InPos.Y == 3)
		{
			switch (InPos.X)
			{
			case 2:
				return EKeys::Tab;
			case 3:
				return EKeys::Q;
			case 4:
				return EKeys::W;
			case 5:
				return EKeys::E;
			case 6:
				return EKeys::R;
			case 7:
				return EKeys::T;
			case 8:
				return EKeys::Y;
			case 9:
				return EKeys::U;
			case 10:
				return EKeys::I;
			case 11:
				return EKeys::O;
			case 12:
				return EKeys::P;
			}
		}

		if (InPos.Y == 4)
		{
			switch (InPos.X)
			{
			case 2:
				return EKeys::CapsLock;
			case 3:
				return EKeys::A;
			case 4:
				return EKeys::S;
			case 5:
				return EKeys::D;
			case 6:
				return EKeys::F;
			case 7:
				return EKeys::G;
			case 8:
				return EKeys::H;
			case 9:
				return EKeys::J;
			case 10:
				return EKeys::K;
			case 11:
				return EKeys::L;
			}
		}

		if (InPos.Y == 5)
		{
			switch (InPos.X)
			{
			case 2:
				return EKeys::LeftShift;
			case 3:
				return EKeys::Z;
			case 4:
				return EKeys::X;
			case 5:
				return EKeys::C;
			case 6:
				return EKeys::V;
			case 7:
				return EKeys::B;
			case 8:
				return EKeys::N;
			case 9:
				return EKeys::M;
			case 10:
				return EKeys::Comma;
			case 11:
				return EKeys::Period;
			}
		}
		return EKeys::Invalid;
	}
};