#pragma once
#include "CoreMinimal.h"
#include <InputCoreTypes.h>

class FWootingKeyConverter
{
public:
    static FKey ToFKey(const FIntPoint& InPos)
    {
		// This is based on https://dev.wooting.nl/wp-content/uploads/2018/06/new2-1.png
		// Not tested or verified :D
		if (InPos.Y == 0)
		{
			switch (InPos.X)
			{
			case 0:
				return EKeys::Escape;
			case 1:
				return EKeys::Invalid;
			case 2:
				return EKeys::F1;
			case 3:
				return EKeys::F2;
			case 4:
				return EKeys::F3;
			case 5:
				return EKeys::F4;
			case 6:
				return EKeys::F5;
			case 7:
				return EKeys::F6;
			case 8:
				return EKeys::F7;
			case 9:
				return EKeys::F8;
			case 10:
				return EKeys::F9;
			case 11:
				return EKeys::F10;
			case 12:
				return EKeys::F11;
			case 13:
				return EKeys::F12;
			case 14:
				return EKeys::Invalid; // PrintScreen
			case 15:
				return EKeys::Pause;
			case 16:
				return EKeys::ScrollLock;
			}
		}

		if (InPos.Y == 1)
		{
			switch (InPos.X)
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
				return EKeys::Invalid; // Numpad asterisk
			case 20:
				return EKeys::Invalid; // Numpad hyphen
			}
		}

		if (InPos.Y == 2)
		{
			switch (InPos.X)
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
				return EKeys::Delete;
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

		if (InPos.Y == 3)
		{
			switch (InPos.X)
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
				return EKeys::Invalid; // ISO?
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

		if (InPos.Y == 4)
		{
			switch (InPos.X)
			{
			case 0:
				return EKeys::LeftShift;
			case 1:
				return EKeys::Invalid; // ISO?
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

		if (InPos.Y == 5)
		{
			switch (InPos.X)
			{
			case 0:
				return EKeys::LeftControl;
			case 1:
				return EKeys::Invalid; // Platform
			case 2:
				return EKeys::LeftAlt;
			case 3:
				return EKeys::Invalid;
			case 4:
				return EKeys::Invalid;
			case 5:
				return EKeys::Invalid;
			case 6:
				return EKeys::SpaceBar;
			case 7:
				return EKeys::Invalid;
			case 8:
				return EKeys::Invalid;
			case 9:
				return EKeys::Invalid;
			case 10:
				return EKeys::RightAlt;
			case 11:
				return EKeys::Invalid; // Platform
			case 12:
				return EKeys::Invalid; // Fn
			case 13:
				return EKeys::RightControl;
			case 14:
				return EKeys::Left;
			case 15:
				return EKeys::Down;
			case 16:
				return EKeys::Right;
			case 17:
				return EKeys::Invalid;
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