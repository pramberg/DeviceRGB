#pragma once
#include "CoreMinimal.h"
#include <InputCoreTypes.h>

class FLogitechKeyConverter
{
public:

	static FKey ToFKey(int32 InX, int32 InY)
	{
		// This is a direct mapping from page 27-28 in LogitechGamingLEDSDK.pdf
		// Might be incorrect, needs testing and verification
		if (InY == 0)
		{
			switch (InX)
			{
			case 0:
				return EKeys::Escape;
			case 1:
				return EKeys::F1;
			case 2:
				return EKeys::F2;
			case 3:
				return EKeys::F3;
			case 4:
				return EKeys::F4;
			case 5:
				return EKeys::F5;
			case 6:
				return EKeys::F6;
			case 7:
				return EKeys::F7;
			case 8:
				return EKeys::F8;
			case 9:
				return EKeys::F9;
			case 10:
				return EKeys::F10;
			case 11:
				return EKeys::F11;
			case 12:
				return EKeys::F12;
			case 13:
				return EKeys::Invalid; // PrintScreen
			case 14:
				return EKeys::ScrollLock;
			case 15:
				return EKeys::Pause;
			}
		}

		if (InY == 1)
		{
			switch (InX)
			{
			case 0:
				return EKeys::Tilde;
			case 1:
				return EKeys::One;
			case 2:
				return EKeys::Two;
			case 3:
				return EKeys::Three;
			case 4:
				return EKeys::Four;
			case 5:
				return EKeys::Five;
			case 6:
				return EKeys::Six;
			case 7:
				return EKeys::Seven;
			case 8:
				return EKeys::Eight;
			case 9:
				return EKeys::Nine;
			case 10:
				return EKeys::Zero;
			case 11:
				return EKeys::Hyphen;
			case 12:
				return EKeys::Equals;
			case 13:
				return EKeys::BackSpace;
			case 14:
				return EKeys::Insert;
			case 15:
				return EKeys::Home;
			case 16:
				return EKeys::PageUp;
			case 17:
				return EKeys::NumLock;
			case 18:
				return EKeys::Invalid; // Numpad Slash
			case 19:
				return EKeys::Invalid; // Numpad Asterisk
			case 20:
				return EKeys::Invalid; // Numpad Minus
			}
		}

		if (InY == 2)
		{
			switch (InX)
			{
			case 0:
				return EKeys::Tab;
			case 1:
				return EKeys::Q;
			case 2:
				return EKeys::W;
			case 3:
				return EKeys::E;
			case 4:
				return EKeys::R;
			case 5:
				return EKeys::T;
			case 6:
				return EKeys::Y;
			case 7:
				return EKeys::U;
			case 8:
				return EKeys::I;
			case 9:
				return EKeys::O;
			case 10:
				return EKeys::P;
			case 11:
				return EKeys::LeftBracket;
			case 12:
				return EKeys::RightBracket;
			case 13:
				return EKeys::Backslash;
			case 14:
				return EKeys::Platform_Delete;
			case 15:
				return EKeys::End;
			case 16:
				return EKeys::PageDown;
			case 17:
				return EKeys::NumPadSeven;
			case 18:
				return EKeys::NumPadEight;
			case 19:
				return EKeys::NumPadNine;
			case 20:
				return EKeys::Invalid; // Numpad plus
			}
		}
		
		if (InY == 3)
		{
			switch (InX)
			{
			case 0:
				return EKeys::CapsLock;
			case 1:
				return EKeys::A;
			case 2:
				return EKeys::S;
			case 3:
				return EKeys::D;
			case 4:
				return EKeys::F;
			case 5:
				return EKeys::G;
			case 6:
				return EKeys::H;
			case 7:
				return EKeys::J;
			case 8:
				return EKeys::K;
			case 9:
				return EKeys::L;
			case 10:
				return EKeys::Semicolon;
			case 11:
				return EKeys::Apostrophe;
			case 12:
				return EKeys::Invalid;
			case 13:
				return EKeys::Enter;
			case 14:
				return EKeys::Invalid;
			case 15:
				return EKeys::Invalid;
			case 16:
				return EKeys::Invalid;
			case 17:
				return EKeys::NumPadFour;
			case 18:
				return EKeys::NumPadFive;
			case 19:
				return EKeys::NumPadSix;
			case 20:
				return EKeys::Invalid;
			}
		}

		if (InY == 4)
		{
			switch (InX)
			{
			case 0:
				return EKeys::LeftShift;
			case 1:
				return EKeys::Invalid;
			case 2:
				return EKeys::Z;
			case 3:
				return EKeys::X;
			case 4:
				return EKeys::C;
			case 5:
				return EKeys::V;
			case 6:
				return EKeys::B;
			case 7:
				return EKeys::N;
			case 8:
				return EKeys::M;
			case 9:
				return EKeys::Comma;
			case 10:
				return EKeys::Period;
			case 11:
				return EKeys::Slash;
			case 12:
				return EKeys::Invalid;
			case 13:
				return EKeys::RightShift;
			case 14:
				return EKeys::Invalid;
			case 15:
				return EKeys::Up;
			case 16:
				return EKeys::Invalid;
			case 17:
				return EKeys::NumPadOne;
			case 18:
				return EKeys::NumPadTwo;
			case 19:
				return EKeys::NumPadThree;
			case 20:
				return EKeys::Invalid;
			}
		}

		if (InY == 5)
		{
			switch (InX)
			{
			case 0:
				return EKeys::LeftControl;
			case 1:
				return EKeys::Invalid; // Platform key
			case 2:
				return EKeys::LeftAlt;
			case 3:
				return EKeys::Invalid;
			case 4:
				return EKeys::Invalid;
			case 5:
				return EKeys::SpaceBar;
			case 6:
				return EKeys::Invalid;
			case 7:
				return EKeys::Invalid;
			case 8:
				return EKeys::Invalid;
			case 9:
				return EKeys::Invalid;
			case 10:
				return EKeys::Invalid;
			case 11:
				return EKeys::RightAlt;
			case 12:
				return EKeys::Invalid; // Platform key
			case 13:
				return EKeys::Invalid; // Application Select
			case 14:
				return EKeys::RightControl;
			case 15:
				return EKeys::Left;
			case 16:
				return EKeys::Down;
			case 17:
				return EKeys::Right;
			case 18:
				return EKeys::NumPadZero;
			case 19:
				return EKeys::Invalid;
			case 20:
				return EKeys::Invalid;
			}
		}
		return EKeys::Invalid;
	}
};