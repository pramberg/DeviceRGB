#pragma once
#include "CoreMinimal.h"
#include <CorsairLedIdEnum.h>
#include <InputCoreTypes.h>

class FCorsairKeyConverter
{
public:
	static FKey ToFKey(CorsairLedId InId)
	{
		switch (InId)
		{
		case CLI_Invalid:
			return EKeys::Invalid;
		case CLK_Escape:
			return EKeys::Escape;
		case CLK_F1:
			return EKeys::F1;
		case CLK_F2:
			return EKeys::F2;
		case CLK_F3:
			return EKeys::F3;
		case CLK_F4:
			return EKeys::F4;
		case CLK_F5:
			return EKeys::F5;
		case CLK_F6:
			return EKeys::F6;
		case CLK_F7:
			return EKeys::F7;
		case CLK_F8:
			return EKeys::F8;
		case CLK_F9:
			return EKeys::F9;
		case CLK_F10:
			return EKeys::F10;
		case CLK_F11:
			return EKeys::F11;
		case CLK_GraveAccentAndTilde:
			return EKeys::Tilde;
		case CLK_1:
			return EKeys::One;
		case CLK_2:
			return EKeys::Two;
		case CLK_3:
			return EKeys::Three;
		case CLK_4:
			return EKeys::Four;
		case CLK_5:
			return EKeys::Five;
		case CLK_6:
			return EKeys::Six;
		case CLK_7:
			return EKeys::Seven;
		case CLK_8:
			return EKeys::Eight;
		case CLK_9:
			return EKeys::Nine;
		case CLK_0:
			return EKeys::Zero;
		case CLK_MinusAndUnderscore:
			return EKeys::Hyphen;
		case CLK_Tab:
			return EKeys::Tab;
		case CLK_Q:
			return EKeys::Q;
		case CLK_W:
			return EKeys::W;
		case CLK_E:
			return EKeys::E;
		case CLK_R:
			return EKeys::R;
		case CLK_T:
			return EKeys::T;
		case CLK_Y:
			return EKeys::Y;
		case CLK_U:
			return EKeys::U;
		case CLK_I:
			return EKeys::I;
		case CLK_O:
			return EKeys::O;
		case CLK_P:
			return EKeys::P;
		case CLK_BracketLeft:
			return EKeys::LeftBracket;
		case CLK_CapsLock:
			return EKeys::CapsLock;
		case CLK_A:
			return EKeys::A;
		case CLK_S:
			return EKeys::S;
		case CLK_D:
			return EKeys::D;
		case CLK_F:
			return EKeys::F;
		case CLK_G:
			return EKeys::G;
		case CLK_H:
			return EKeys::H;
		case CLK_J:
			return EKeys::J;
		case CLK_K:
			return EKeys::K;
		case CLK_L:
			return EKeys::L;
		case CLK_SemicolonAndColon:
			return EKeys::Colon;
		case CLK_ApostropheAndDoubleQuote:
			return EKeys::Apostrophe;
		case CLK_LeftShift:
			return EKeys::LeftShift;
		case CLK_NonUsBackslash:
			return EKeys::Backslash;
		case CLK_Z:
			return EKeys::Z;
		case CLK_X:
			return EKeys::X;
		case CLK_C:
			return EKeys::C;
		case CLK_V:
			return EKeys::V;
		case CLK_B:
			return EKeys::B;
		case CLK_N:
			return EKeys::N;
		case CLK_M:
			return EKeys::M;
		case CLK_CommaAndLessThan:
			return EKeys::Comma;
		case CLK_PeriodAndBiggerThan:
			return EKeys::Period;
		case CLK_SlashAndQuestionMark:
			return EKeys::Slash;
		case CLK_LeftCtrl:
			return EKeys::LeftControl;
		case CLK_LeftGui:
			break;
		case CLK_LeftAlt:
			return EKeys::LeftAlt;
		case CLK_Lang2:
			break;
		case CLK_Space:
			return EKeys::SpaceBar;
		case CLK_Lang1:
			break;
		case CLK_International2:
			break;
		case CLK_RightAlt:
			return EKeys::RightAlt;
		case CLK_RightGui:
			break;
		case CLK_Application:
			break;
		case CLK_LedProgramming:
			break;
		case CLK_Brightness:
			break;
		case CLK_F12:
			return EKeys::F12;
		case CLK_PrintScreen:
			break;
		case CLK_ScrollLock:
			return EKeys::ScrollLock;
		case CLK_PauseBreak:
			return EKeys::Pause;
		case CLK_Insert:
			return EKeys::Insert;
		case CLK_Home:
			return EKeys::Home;
		case CLK_PageUp:
			return EKeys::PageUp;
		case CLK_BracketRight:
			return EKeys::RightBracket;
		case CLK_Backslash:
			return EKeys::Backslash;
		case CLK_NonUsTilde:
			return EKeys::Tilde;
		case CLK_Enter:
			return EKeys::Enter;
		case CLK_International1:
			break;
		case CLK_EqualsAndPlus:
			return EKeys::Equals;
		case CLK_International3:
			break;
		case CLK_Backspace:
			return EKeys::BackSpace;
		case CLK_Delete:
			return EKeys::Delete;
		case CLK_End:
			return EKeys::End;
		case CLK_PageDown:
			return EKeys::PageDown;
		case CLK_RightShift:
			return EKeys::RightShift;
		case CLK_RightCtrl:
			return EKeys::RightControl;
		case CLK_UpArrow:
			return EKeys::Up;
		case CLK_LeftArrow:
			return EKeys::Left;
		case CLK_DownArrow:
			return EKeys::Down;
		case CLK_RightArrow:
			return EKeys::Right;
		case CLK_WinLock:
			break;
		case CLK_Mute:
			break;
		case CLK_Stop:
			break;
		case CLK_ScanPreviousTrack:
			break;
		case CLK_PlayPause:
			break;
		case CLK_ScanNextTrack:
			break;
		case CLK_NumLock:
			return EKeys::NumLock;
		case CLK_KeypadSlash:
			break;
		case CLK_KeypadAsterisk:
			break;
		case CLK_KeypadMinus:
			break;
		case CLK_KeypadPlus:
			break;
		case CLK_KeypadEnter:
			break;
		case CLK_Keypad7:
			return EKeys::NumPadSeven;
		case CLK_Keypad8:
			return EKeys::NumPadEight;
		case CLK_Keypad9:
			return EKeys::NumPadNine;
		case CLK_KeypadComma:
			break;
		case CLK_Keypad4:
			return EKeys::NumPadFour;
		case CLK_Keypad5:
			return EKeys::NumPadFive;
		case CLK_Keypad6:
			return EKeys::NumPadSix;
		case CLK_Keypad1:
			return EKeys::NumPadOne;
		case CLK_Keypad2:
			return EKeys::NumPadTwo;
		case CLK_Keypad3:
			return EKeys::NumPadThree;
		case CLK_Keypad0:
			return EKeys::NumPadZero;
		case CLK_KeypadPeriodAndDelete:
			break;
		case CLK_G1:
			break;
		case CLK_G2:
			break;
		case CLK_G3:
			break;
		case CLK_G4:
			break;
		case CLK_G5:
			break;
		case CLK_G6:
			break;
		case CLK_G7:
			break;
		case CLK_G8:
			break;
		case CLK_G9:
			break;
		case CLK_G10:
			break;
		case CLK_VolumeUp:
			break;
		case CLK_VolumeDown:
			break;
		case CLK_MR:
			break;
		case CLK_M1:
			break;
		case CLK_M2:
			break;
		case CLK_M3:
			break;
		case CLK_G11:
			break;
		case CLK_G12:
			break;
		case CLK_G13:
			break;
		case CLK_G14:
			break;
		case CLK_G15:
			break;
		case CLK_G16:
			break;
		case CLK_G17:
			break;
		case CLK_G18:
			break;
		case CLK_International5:
			break;
		case CLK_International4:
			break;
		case CLK_Fn:
			break;
		case CLM_1:
			break;
		case CLM_2:
			break;
		case CLM_3:
			break;
		case CLM_4:
			break;
		case CLH_LeftLogo:
			break;
		case CLH_RightLogo:
			break;
		case CLK_Logo:
			break;
		case CLMM_Zone1:
			break;
		case CLMM_Zone2:
			break;
		case CLMM_Zone3:
			break;
		case CLMM_Zone4:
			break;
		case CLMM_Zone5:
			break;
		case CLMM_Zone6:
			break;
		case CLMM_Zone7:
			break;
		case CLMM_Zone8:
			break;
		case CLMM_Zone9:
			break;
		case CLMM_Zone10:
			break;
		case CLMM_Zone11:
			break;
		case CLMM_Zone12:
			break;
		case CLMM_Zone13:
			break;
		case CLMM_Zone14:
			break;
		case CLMM_Zone15:
			break;
		case CLKLP_Zone1:
			break;
		case CLKLP_Zone2:
			break;
		case CLKLP_Zone3:
			break;
		case CLKLP_Zone4:
			break;
		case CLKLP_Zone5:
			break;
		case CLKLP_Zone6:
			break;
		case CLKLP_Zone7:
			break;
		case CLKLP_Zone8:
			break;
		case CLKLP_Zone9:
			break;
		case CLKLP_Zone10:
			break;
		case CLKLP_Zone11:
			break;
		case CLKLP_Zone12:
			break;
		case CLKLP_Zone13:
			break;
		case CLKLP_Zone14:
			break;
		case CLKLP_Zone15:
			break;
		case CLKLP_Zone16:
			break;
		case CLKLP_Zone17:
			break;
		case CLKLP_Zone18:
			break;
		case CLKLP_Zone19:
			break;
		case CLM_5:
			break;
		case CLM_6:
			break;
		case CLHSS_Zone1:
			break;
		case CLHSS_Zone2:
			break;
		case CLHSS_Zone3:
			break;
		case CLHSS_Zone4:
			break;
		case CLHSS_Zone5:
			break;
		case CLHSS_Zone6:
			break;
		case CLHSS_Zone7:
			break;
		case CLHSS_Zone8:
			break;
		case CLHSS_Zone9:
			break;
		case CLD_C1_1:
			break;
		case CLD_C1_2:
			break;
		case CLD_C1_3:
			break;
		case CLD_C1_4:
			break;
		case CLD_C1_5:
			break;
		case CLD_C1_6:
			break;
		case CLD_C1_7:
			break;
		case CLD_C1_8:
			break;
		case CLD_C1_9:
			break;
		case CLD_C1_10:
			break;
		case CLD_C1_11:
			break;
		case CLD_C1_12:
			break;
		case CLD_C1_13:
			break;
		case CLD_C1_14:
			break;
		case CLD_C1_15:
			break;
		case CLD_C1_16:
			break;
		case CLD_C1_17:
			break;
		case CLD_C1_18:
			break;
		case CLD_C1_19:
			break;
		case CLD_C1_20:
			break;
		case CLD_C1_21:
			break;
		case CLD_C1_22:
			break;
		case CLD_C1_23:
			break;
		case CLD_C1_24:
			break;
		case CLD_C1_25:
			break;
		case CLD_C1_26:
			break;
		case CLD_C1_27:
			break;
		case CLD_C1_28:
			break;
		case CLD_C1_29:
			break;
		case CLD_C1_30:
			break;
		case CLD_C1_31:
			break;
		case CLD_C1_32:
			break;
		case CLD_C1_33:
			break;
		case CLD_C1_34:
			break;
		case CLD_C1_35:
			break;
		case CLD_C1_36:
			break;
		case CLD_C1_37:
			break;
		case CLD_C1_38:
			break;
		case CLD_C1_39:
			break;
		case CLD_C1_40:
			break;
		case CLD_C1_41:
			break;
		case CLD_C1_42:
			break;
		case CLD_C1_43:
			break;
		case CLD_C1_44:
			break;
		case CLD_C1_45:
			break;
		case CLD_C1_46:
			break;
		case CLD_C1_47:
			break;
		case CLD_C1_48:
			break;
		case CLD_C1_49:
			break;
		case CLD_C1_50:
			break;
		case CLD_C1_51:
			break;
		case CLD_C1_52:
			break;
		case CLD_C1_53:
			break;
		case CLD_C1_54:
			break;
		case CLD_C1_55:
			break;
		case CLD_C1_56:
			break;
		case CLD_C1_57:
			break;
		case CLD_C1_58:
			break;
		case CLD_C1_59:
			break;
		case CLD_C1_60:
			break;
		case CLD_C1_61:
			break;
		case CLD_C1_62:
			break;
		case CLD_C1_63:
			break;
		case CLD_C1_64:
			break;
		case CLD_C1_65:
			break;
		case CLD_C1_66:
			break;
		case CLD_C1_67:
			break;
		case CLD_C1_68:
			break;
		case CLD_C1_69:
			break;
		case CLD_C1_70:
			break;
		case CLD_C1_71:
			break;
		case CLD_C1_72:
			break;
		case CLD_C1_73:
			break;
		case CLD_C1_74:
			break;
		case CLD_C1_75:
			break;
		case CLD_C1_76:
			break;
		case CLD_C1_77:
			break;
		case CLD_C1_78:
			break;
		case CLD_C1_79:
			break;
		case CLD_C1_80:
			break;
		case CLD_C1_81:
			break;
		case CLD_C1_82:
			break;
		case CLD_C1_83:
			break;
		case CLD_C1_84:
			break;
		case CLD_C1_85:
			break;
		case CLD_C1_86:
			break;
		case CLD_C1_87:
			break;
		case CLD_C1_88:
			break;
		case CLD_C1_89:
			break;
		case CLD_C1_90:
			break;
		case CLD_C1_91:
			break;
		case CLD_C1_92:
			break;
		case CLD_C1_93:
			break;
		case CLD_C1_94:
			break;
		case CLD_C1_95:
			break;
		case CLD_C1_96:
			break;
		case CLD_C1_97:
			break;
		case CLD_C1_98:
			break;
		case CLD_C1_99:
			break;
		case CLD_C1_100:
			break;
		case CLD_C1_101:
			break;
		case CLD_C1_102:
			break;
		case CLD_C1_103:
			break;
		case CLD_C1_104:
			break;
		case CLD_C1_105:
			break;
		case CLD_C1_106:
			break;
		case CLD_C1_107:
			break;
		case CLD_C1_108:
			break;
		case CLD_C1_109:
			break;
		case CLD_C1_110:
			break;
		case CLD_C1_111:
			break;
		case CLD_C1_112:
			break;
		case CLD_C1_113:
			break;
		case CLD_C1_114:
			break;
		case CLD_C1_115:
			break;
		case CLD_C1_116:
			break;
		case CLD_C1_117:
			break;
		case CLD_C1_118:
			break;
		case CLD_C1_119:
			break;
		case CLD_C1_120:
			break;
		case CLD_C1_121:
			break;
		case CLD_C1_122:
			break;
		case CLD_C1_123:
			break;
		case CLD_C1_124:
			break;
		case CLD_C1_125:
			break;
		case CLD_C1_126:
			break;
		case CLD_C1_127:
			break;
		case CLD_C1_128:
			break;
		case CLD_C1_129:
			break;
		case CLD_C1_130:
			break;
		case CLD_C1_131:
			break;
		case CLD_C1_132:
			break;
		case CLD_C1_133:
			break;
		case CLD_C1_134:
			break;
		case CLD_C1_135:
			break;
		case CLD_C1_136:
			break;
		case CLD_C1_137:
			break;
		case CLD_C1_138:
			break;
		case CLD_C1_139:
			break;
		case CLD_C1_140:
			break;
		case CLD_C1_141:
			break;
		case CLD_C1_142:
			break;
		case CLD_C1_143:
			break;
		case CLD_C1_144:
			break;
		case CLD_C1_145:
			break;
		case CLD_C1_146:
			break;
		case CLD_C1_147:
			break;
		case CLD_C1_148:
			break;
		case CLD_C1_149:
			break;
		case CLD_C1_150:
			break;
		case CLD_C2_1:
			break;
		case CLD_C2_2:
			break;
		case CLD_C2_3:
			break;
		case CLD_C2_4:
			break;
		case CLD_C2_5:
			break;
		case CLD_C2_6:
			break;
		case CLD_C2_7:
			break;
		case CLD_C2_8:
			break;
		case CLD_C2_9:
			break;
		case CLD_C2_10:
			break;
		case CLD_C2_11:
			break;
		case CLD_C2_12:
			break;
		case CLD_C2_13:
			break;
		case CLD_C2_14:
			break;
		case CLD_C2_15:
			break;
		case CLD_C2_16:
			break;
		case CLD_C2_17:
			break;
		case CLD_C2_18:
			break;
		case CLD_C2_19:
			break;
		case CLD_C2_20:
			break;
		case CLD_C2_21:
			break;
		case CLD_C2_22:
			break;
		case CLD_C2_23:
			break;
		case CLD_C2_24:
			break;
		case CLD_C2_25:
			break;
		case CLD_C2_26:
			break;
		case CLD_C2_27:
			break;
		case CLD_C2_28:
			break;
		case CLD_C2_29:
			break;
		case CLD_C2_30:
			break;
		case CLD_C2_31:
			break;
		case CLD_C2_32:
			break;
		case CLD_C2_33:
			break;
		case CLD_C2_34:
			break;
		case CLD_C2_35:
			break;
		case CLD_C2_36:
			break;
		case CLD_C2_37:
			break;
		case CLD_C2_38:
			break;
		case CLD_C2_39:
			break;
		case CLD_C2_40:
			break;
		case CLD_C2_41:
			break;
		case CLD_C2_42:
			break;
		case CLD_C2_43:
			break;
		case CLD_C2_44:
			break;
		case CLD_C2_45:
			break;
		case CLD_C2_46:
			break;
		case CLD_C2_47:
			break;
		case CLD_C2_48:
			break;
		case CLD_C2_49:
			break;
		case CLD_C2_50:
			break;
		case CLD_C2_51:
			break;
		case CLD_C2_52:
			break;
		case CLD_C2_53:
			break;
		case CLD_C2_54:
			break;
		case CLD_C2_55:
			break;
		case CLD_C2_56:
			break;
		case CLD_C2_57:
			break;
		case CLD_C2_58:
			break;
		case CLD_C2_59:
			break;
		case CLD_C2_60:
			break;
		case CLD_C2_61:
			break;
		case CLD_C2_62:
			break;
		case CLD_C2_63:
			break;
		case CLD_C2_64:
			break;
		case CLD_C2_65:
			break;
		case CLD_C2_66:
			break;
		case CLD_C2_67:
			break;
		case CLD_C2_68:
			break;
		case CLD_C2_69:
			break;
		case CLD_C2_70:
			break;
		case CLD_C2_71:
			break;
		case CLD_C2_72:
			break;
		case CLD_C2_73:
			break;
		case CLD_C2_74:
			break;
		case CLD_C2_75:
			break;
		case CLD_C2_76:
			break;
		case CLD_C2_77:
			break;
		case CLD_C2_78:
			break;
		case CLD_C2_79:
			break;
		case CLD_C2_80:
			break;
		case CLD_C2_81:
			break;
		case CLD_C2_82:
			break;
		case CLD_C2_83:
			break;
		case CLD_C2_84:
			break;
		case CLD_C2_85:
			break;
		case CLD_C2_86:
			break;
		case CLD_C2_87:
			break;
		case CLD_C2_88:
			break;
		case CLD_C2_89:
			break;
		case CLD_C2_90:
			break;
		case CLD_C2_91:
			break;
		case CLD_C2_92:
			break;
		case CLD_C2_93:
			break;
		case CLD_C2_94:
			break;
		case CLD_C2_95:
			break;
		case CLD_C2_96:
			break;
		case CLD_C2_97:
			break;
		case CLD_C2_98:
			break;
		case CLD_C2_99:
			break;
		case CLD_C2_100:
			break;
		case CLD_C2_101:
			break;
		case CLD_C2_102:
			break;
		case CLD_C2_103:
			break;
		case CLD_C2_104:
			break;
		case CLD_C2_105:
			break;
		case CLD_C2_106:
			break;
		case CLD_C2_107:
			break;
		case CLD_C2_108:
			break;
		case CLD_C2_109:
			break;
		case CLD_C2_110:
			break;
		case CLD_C2_111:
			break;
		case CLD_C2_112:
			break;
		case CLD_C2_113:
			break;
		case CLD_C2_114:
			break;
		case CLD_C2_115:
			break;
		case CLD_C2_116:
			break;
		case CLD_C2_117:
			break;
		case CLD_C2_118:
			break;
		case CLD_C2_119:
			break;
		case CLD_C2_120:
			break;
		case CLD_C2_121:
			break;
		case CLD_C2_122:
			break;
		case CLD_C2_123:
			break;
		case CLD_C2_124:
			break;
		case CLD_C2_125:
			break;
		case CLD_C2_126:
			break;
		case CLD_C2_127:
			break;
		case CLD_C2_128:
			break;
		case CLD_C2_129:
			break;
		case CLD_C2_130:
			break;
		case CLD_C2_131:
			break;
		case CLD_C2_132:
			break;
		case CLD_C2_133:
			break;
		case CLD_C2_134:
			break;
		case CLD_C2_135:
			break;
		case CLD_C2_136:
			break;
		case CLD_C2_137:
			break;
		case CLD_C2_138:
			break;
		case CLD_C2_139:
			break;
		case CLD_C2_140:
			break;
		case CLD_C2_141:
			break;
		case CLD_C2_142:
			break;
		case CLD_C2_143:
			break;
		case CLD_C2_144:
			break;
		case CLD_C2_145:
			break;
		case CLD_C2_146:
			break;
		case CLD_C2_147:
			break;
		case CLD_C2_148:
			break;
		case CLD_C2_149:
			break;
		case CLD_C2_150:
			break;
		case CLI_Oem1:
			break;
		case CLI_Oem2:
			break;
		case CLI_Oem3:
			break;
		case CLI_Oem4:
			break;
		case CLI_Oem5:
			break;
		case CLI_Oem6:
			break;
		case CLI_Oem7:
			break;
		case CLI_Oem8:
			break;
		case CLI_Oem9:
			break;
		case CLI_Oem10:
			break;
		case CLI_Oem11:
			break;
		case CLI_Oem12:
			break;
		case CLI_Oem13:
			break;
		case CLI_Oem14:
			break;
		case CLI_Oem15:
			break;
		case CLI_Oem16:
			break;
		case CLI_Oem17:
			break;
		case CLI_Oem18:
			break;
		case CLI_Oem19:
			break;
		case CLI_Oem20:
			break;
		case CLI_Oem21:
			break;
		case CLI_Oem22:
			break;
		case CLI_Oem23:
			break;
		case CLI_Oem24:
			break;
		case CLI_Oem25:
			break;
		case CLI_Oem26:
			break;
		case CLI_Oem27:
			break;
		case CLI_Oem28:
			break;
		case CLI_Oem29:
			break;
		case CLI_Oem30:
			break;
		case CLI_Oem31:
			break;
		case CLI_Oem32:
			break;
		case CLI_Oem33:
			break;
		case CLI_Oem34:
			break;
		case CLI_Oem35:
			break;
		case CLI_Oem36:
			break;
		case CLI_Oem37:
			break;
		case CLI_Oem38:
			break;
		case CLI_Oem39:
			break;
		case CLI_Oem40:
			break;
		case CLI_Oem41:
			break;
		case CLI_Oem42:
			break;
		case CLI_Oem43:
			break;
		case CLI_Oem44:
			break;
		case CLI_Oem45:
			break;
		case CLI_Oem46:
			break;
		case CLI_Oem47:
			break;
		case CLI_Oem48:
			break;
		case CLI_Oem49:
			break;
		case CLI_Oem50:
			break;
		case CLI_Oem51:
			break;
		case CLI_Oem52:
			break;
		case CLI_Oem53:
			break;
		case CLI_Oem54:
			break;
		case CLI_Oem55:
			break;
		case CLI_Oem56:
			break;
		case CLI_Oem57:
			break;
		case CLI_Oem58:
			break;
		case CLI_Oem59:
			break;
		case CLI_Oem60:
			break;
		case CLI_Oem61:
			break;
		case CLI_Oem62:
			break;
		case CLI_Oem63:
			break;
		case CLI_Oem64:
			break;
		case CLI_Oem65:
			break;
		case CLI_Oem66:
			break;
		case CLI_Oem67:
			break;
		case CLI_Oem68:
			break;
		case CLI_Oem69:
			break;
		case CLI_Oem70:
			break;
		case CLI_Oem71:
			break;
		case CLI_Oem72:
			break;
		case CLI_Oem73:
			break;
		case CLI_Oem74:
			break;
		case CLI_Oem75:
			break;
		case CLI_Oem76:
			break;
		case CLI_Oem77:
			break;
		case CLI_Oem78:
			break;
		case CLI_Oem79:
			break;
		case CLI_Oem80:
			break;
		case CLI_Oem81:
			break;
		case CLI_Oem82:
			break;
		case CLI_Oem83:
			break;
		case CLI_Oem84:
			break;
		case CLI_Oem85:
			break;
		case CLI_Oem86:
			break;
		case CLI_Oem87:
			break;
		case CLI_Oem88:
			break;
		case CLI_Oem89:
			break;
		case CLI_Oem90:
			break;
		case CLI_Oem91:
			break;
		case CLI_Oem92:
			break;
		case CLI_Oem93:
			break;
		case CLI_Oem94:
			break;
		case CLI_Oem95:
			break;
		case CLI_Oem96:
			break;
		case CLI_Oem97:
			break;
		case CLI_Oem98:
			break;
		case CLI_Oem99:
			break;
		case CLI_Oem100:
			break;
		case CLDRAM_1:
			break;
		case CLDRAM_2:
			break;
		case CLDRAM_3:
			break;
		case CLDRAM_4:
			break;
		case CLDRAM_5:
			break;
		case CLDRAM_6:
			break;
		case CLDRAM_7:
			break;
		case CLDRAM_8:
			break;
		case CLDRAM_9:
			break;
		case CLDRAM_10:
			break;
		case CLDRAM_11:
			break;
		case CLDRAM_12:
			break;
		case CLD_C3_1:
			break;
		case CLD_C3_2:
			break;
		case CLD_C3_3:
			break;
		case CLD_C3_4:
			break;
		case CLD_C3_5:
			break;
		case CLD_C3_6:
			break;
		case CLD_C3_7:
			break;
		case CLD_C3_8:
			break;
		case CLD_C3_9:
			break;
		case CLD_C3_10:
			break;
		case CLD_C3_11:
			break;
		case CLD_C3_12:
			break;
		case CLD_C3_13:
			break;
		case CLD_C3_14:
			break;
		case CLD_C3_15:
			break;
		case CLD_C3_16:
			break;
		case CLD_C3_17:
			break;
		case CLD_C3_18:
			break;
		case CLD_C3_19:
			break;
		case CLD_C3_20:
			break;
		case CLD_C3_21:
			break;
		case CLD_C3_22:
			break;
		case CLD_C3_23:
			break;
		case CLD_C3_24:
			break;
		case CLD_C3_25:
			break;
		case CLD_C3_26:
			break;
		case CLD_C3_27:
			break;
		case CLD_C3_28:
			break;
		case CLD_C3_29:
			break;
		case CLD_C3_30:
			break;
		case CLD_C3_31:
			break;
		case CLD_C3_32:
			break;
		case CLD_C3_33:
			break;
		case CLD_C3_34:
			break;
		case CLD_C3_35:
			break;
		case CLD_C3_36:
			break;
		case CLD_C3_37:
			break;
		case CLD_C3_38:
			break;
		case CLD_C3_39:
			break;
		case CLD_C3_40:
			break;
		case CLD_C3_41:
			break;
		case CLD_C3_42:
			break;
		case CLD_C3_43:
			break;
		case CLD_C3_44:
			break;
		case CLD_C3_45:
			break;
		case CLD_C3_46:
			break;
		case CLD_C3_47:
			break;
		case CLD_C3_48:
			break;
		case CLD_C3_49:
			break;
		case CLD_C3_50:
			break;
		case CLD_C3_51:
			break;
		case CLD_C3_52:
			break;
		case CLD_C3_53:
			break;
		case CLD_C3_54:
			break;
		case CLD_C3_55:
			break;
		case CLD_C3_56:
			break;
		case CLD_C3_57:
			break;
		case CLD_C3_58:
			break;
		case CLD_C3_59:
			break;
		case CLD_C3_60:
			break;
		case CLD_C3_61:
			break;
		case CLD_C3_62:
			break;
		case CLD_C3_63:
			break;
		case CLD_C3_64:
			break;
		case CLD_C3_65:
			break;
		case CLD_C3_66:
			break;
		case CLD_C3_67:
			break;
		case CLD_C3_68:
			break;
		case CLD_C3_69:
			break;
		case CLD_C3_70:
			break;
		case CLD_C3_71:
			break;
		case CLD_C3_72:
			break;
		case CLD_C3_73:
			break;
		case CLD_C3_74:
			break;
		case CLD_C3_75:
			break;
		case CLD_C3_76:
			break;
		case CLD_C3_77:
			break;
		case CLD_C3_78:
			break;
		case CLD_C3_79:
			break;
		case CLD_C3_80:
			break;
		case CLD_C3_81:
			break;
		case CLD_C3_82:
			break;
		case CLD_C3_83:
			break;
		case CLD_C3_84:
			break;
		case CLD_C3_85:
			break;
		case CLD_C3_86:
			break;
		case CLD_C3_87:
			break;
		case CLD_C3_88:
			break;
		case CLD_C3_89:
			break;
		case CLD_C3_90:
			break;
		case CLD_C3_91:
			break;
		case CLD_C3_92:
			break;
		case CLD_C3_93:
			break;
		case CLD_C3_94:
			break;
		case CLD_C3_95:
			break;
		case CLD_C3_96:
			break;
		case CLD_C3_97:
			break;
		case CLD_C3_98:
			break;
		case CLD_C3_99:
			break;
		case CLD_C3_100:
			break;
		case CLD_C3_101:
			break;
		case CLD_C3_102:
			break;
		case CLD_C3_103:
			break;
		case CLD_C3_104:
			break;
		case CLD_C3_105:
			break;
		case CLD_C3_106:
			break;
		case CLD_C3_107:
			break;
		case CLD_C3_108:
			break;
		case CLD_C3_109:
			break;
		case CLD_C3_110:
			break;
		case CLD_C3_111:
			break;
		case CLD_C3_112:
			break;
		case CLD_C3_113:
			break;
		case CLD_C3_114:
			break;
		case CLD_C3_115:
			break;
		case CLD_C3_116:
			break;
		case CLD_C3_117:
			break;
		case CLD_C3_118:
			break;
		case CLD_C3_119:
			break;
		case CLD_C3_120:
			break;
		case CLD_C3_121:
			break;
		case CLD_C3_122:
			break;
		case CLD_C3_123:
			break;
		case CLD_C3_124:
			break;
		case CLD_C3_125:
			break;
		case CLD_C3_126:
			break;
		case CLD_C3_127:
			break;
		case CLD_C3_128:
			break;
		case CLD_C3_129:
			break;
		case CLD_C3_130:
			break;
		case CLD_C3_131:
			break;
		case CLD_C3_132:
			break;
		case CLD_C3_133:
			break;
		case CLD_C3_134:
			break;
		case CLD_C3_135:
			break;
		case CLD_C3_136:
			break;
		case CLD_C3_137:
			break;
		case CLD_C3_138:
			break;
		case CLD_C3_139:
			break;
		case CLD_C3_140:
			break;
		case CLD_C3_141:
			break;
		case CLD_C3_142:
			break;
		case CLD_C3_143:
			break;
		case CLD_C3_144:
			break;
		case CLD_C3_145:
			break;
		case CLD_C3_146:
			break;
		case CLD_C3_147:
			break;
		case CLD_C3_148:
			break;
		case CLD_C3_149:
			break;
		case CLD_C3_150:
			break;
		case CLLC_C1_1:
			break;
		case CLLC_C1_2:
			break;
		case CLLC_C1_3:
			break;
		case CLLC_C1_4:
			break;
		case CLLC_C1_5:
			break;
		case CLLC_C1_6:
			break;
		case CLLC_C1_7:
			break;
		case CLLC_C1_8:
			break;
		case CLLC_C1_9:
			break;
		case CLLC_C1_10:
			break;
		case CLLC_C1_11:
			break;
		case CLLC_C1_12:
			break;
		case CLLC_C1_13:
			break;
		case CLLC_C1_14:
			break;
		case CLLC_C1_15:
			break;
		case CLLC_C1_16:
			break;
		case CLLC_C1_17:
			break;
		case CLLC_C1_18:
			break;
		case CLLC_C1_19:
			break;
		case CLLC_C1_20:
			break;
		case CLLC_C1_21:
			break;
		case CLLC_C1_22:
			break;
		case CLLC_C1_23:
			break;
		case CLLC_C1_24:
			break;
		case CLLC_C1_25:
			break;
		case CLLC_C1_26:
			break;
		case CLLC_C1_27:
			break;
		case CLLC_C1_28:
			break;
		case CLLC_C1_29:
			break;
		case CLLC_C1_30:
			break;
		case CLLC_C1_31:
			break;
		case CLLC_C1_32:
			break;
		case CLLC_C1_33:
			break;
		case CLLC_C1_34:
			break;
		case CLLC_C1_35:
			break;
		case CLLC_C1_36:
			break;
		case CLLC_C1_37:
			break;
		case CLLC_C1_38:
			break;
		case CLLC_C1_39:
			break;
		case CLLC_C1_40:
			break;
		case CLLC_C1_41:
			break;
		case CLLC_C1_42:
			break;
		case CLLC_C1_43:
			break;
		case CLLC_C1_44:
			break;
		case CLLC_C1_45:
			break;
		case CLLC_C1_46:
			break;
		case CLLC_C1_47:
			break;
		case CLLC_C1_48:
			break;
		case CLLC_C1_49:
			break;
		case CLLC_C1_50:
			break;
		case CLLC_C1_51:
			break;
		case CLLC_C1_52:
			break;
		case CLLC_C1_53:
			break;
		case CLLC_C1_54:
			break;
		case CLLC_C1_55:
			break;
		case CLLC_C1_56:
			break;
		case CLLC_C1_57:
			break;
		case CLLC_C1_58:
			break;
		case CLLC_C1_59:
			break;
		case CLLC_C1_60:
			break;
		case CLLC_C1_61:
			break;
		case CLLC_C1_62:
			break;
		case CLLC_C1_63:
			break;
		case CLLC_C1_64:
			break;
		case CLLC_C1_65:
			break;
		case CLLC_C1_66:
			break;
		case CLLC_C1_67:
			break;
		case CLLC_C1_68:
			break;
		case CLLC_C1_69:
			break;
		case CLLC_C1_70:
			break;
		case CLLC_C1_71:
			break;
		case CLLC_C1_72:
			break;
		case CLLC_C1_73:
			break;
		case CLLC_C1_74:
			break;
		case CLLC_C1_75:
			break;
		case CLLC_C1_76:
			break;
		case CLLC_C1_77:
			break;
		case CLLC_C1_78:
			break;
		case CLLC_C1_79:
			break;
		case CLLC_C1_80:
			break;
		case CLLC_C1_81:
			break;
		case CLLC_C1_82:
			break;
		case CLLC_C1_83:
			break;
		case CLLC_C1_84:
			break;
		case CLLC_C1_85:
			break;
		case CLLC_C1_86:
			break;
		case CLLC_C1_87:
			break;
		case CLLC_C1_88:
			break;
		case CLLC_C1_89:
			break;
		case CLLC_C1_90:
			break;
		case CLLC_C1_91:
			break;
		case CLLC_C1_92:
			break;
		case CLLC_C1_93:
			break;
		case CLLC_C1_94:
			break;
		case CLLC_C1_95:
			break;
		case CLLC_C1_96:
			break;
		case CLLC_C1_97:
			break;
		case CLLC_C1_98:
			break;
		case CLLC_C1_99:
			break;
		case CLLC_C1_100:
			break;
		case CLLC_C1_101:
			break;
		case CLLC_C1_102:
			break;
		case CLLC_C1_103:
			break;
		case CLLC_C1_104:
			break;
		case CLLC_C1_105:
			break;
		case CLLC_C1_106:
			break;
		case CLLC_C1_107:
			break;
		case CLLC_C1_108:
			break;
		case CLLC_C1_109:
			break;
		case CLLC_C1_110:
			break;
		case CLLC_C1_111:
			break;
		case CLLC_C1_112:
			break;
		case CLLC_C1_113:
			break;
		case CLLC_C1_114:
			break;
		case CLLC_C1_115:
			break;
		case CLLC_C1_116:
			break;
		case CLLC_C1_117:
			break;
		case CLLC_C1_118:
			break;
		case CLLC_C1_119:
			break;
		case CLLC_C1_120:
			break;
		case CLLC_C1_121:
			break;
		case CLLC_C1_122:
			break;
		case CLLC_C1_123:
			break;
		case CLLC_C1_124:
			break;
		case CLLC_C1_125:
			break;
		case CLLC_C1_126:
			break;
		case CLLC_C1_127:
			break;
		case CLLC_C1_128:
			break;
		case CLLC_C1_129:
			break;
		case CLLC_C1_130:
			break;
		case CLLC_C1_131:
			break;
		case CLLC_C1_132:
			break;
		case CLLC_C1_133:
			break;
		case CLLC_C1_134:
			break;
		case CLLC_C1_135:
			break;
		case CLLC_C1_136:
			break;
		case CLLC_C1_137:
			break;
		case CLLC_C1_138:
			break;
		case CLLC_C1_139:
			break;
		case CLLC_C1_140:
			break;
		case CLLC_C1_141:
			break;
		case CLLC_C1_142:
			break;
		case CLLC_C1_143:
			break;
		case CLLC_C1_144:
			break;
		case CLLC_C1_145:
			break;
		case CLLC_C1_146:
			break;
		case CLLC_C1_147:
			break;
		case CLLC_C1_148:
			break;
		case CLLC_C1_149:
			break;
		case CLLC_C1_150:
			break;
		case CLD_C1_151:
			break;
		case CLD_C1_152:
			break;
		case CLD_C1_153:
			break;
		case CLD_C1_154:
			break;
		case CLD_C1_155:
			break;
		case CLD_C1_156:
			break;
		case CLD_C1_157:
			break;
		case CLD_C1_158:
			break;
		case CLD_C1_159:
			break;
		case CLD_C1_160:
			break;
		case CLD_C1_161:
			break;
		case CLD_C1_162:
			break;
		case CLD_C1_163:
			break;
		case CLD_C1_164:
			break;
		case CLD_C1_165:
			break;
		case CLD_C1_166:
			break;
		case CLD_C1_167:
			break;
		case CLD_C1_168:
			break;
		case CLD_C1_169:
			break;
		case CLD_C1_170:
			break;
		case CLD_C1_171:
			break;
		case CLD_C1_172:
			break;
		case CLD_C1_173:
			break;
		case CLD_C1_174:
			break;
		case CLD_C1_175:
			break;
		case CLD_C1_176:
			break;
		case CLD_C1_177:
			break;
		case CLD_C1_178:
			break;
		case CLD_C1_179:
			break;
		case CLD_C1_180:
			break;
		case CLD_C1_181:
			break;
		case CLD_C1_182:
			break;
		case CLD_C1_183:
			break;
		case CLD_C1_184:
			break;
		case CLD_C1_185:
			break;
		case CLD_C1_186:
			break;
		case CLD_C1_187:
			break;
		case CLD_C1_188:
			break;
		case CLD_C1_189:
			break;
		case CLD_C1_190:
			break;
		case CLD_C1_191:
			break;
		case CLD_C1_192:
			break;
		case CLD_C1_193:
			break;
		case CLD_C1_194:
			break;
		case CLD_C1_195:
			break;
		case CLD_C1_196:
			break;
		case CLD_C1_197:
			break;
		case CLD_C1_198:
			break;
		case CLD_C1_199:
			break;
		case CLD_C1_200:
			break;
		case CLD_C1_201:
			break;
		case CLD_C1_202:
			break;
		case CLD_C1_203:
			break;
		case CLD_C1_204:
			break;
		case CLD_C1_205:
			break;
		case CLD_C1_206:
			break;
		case CLD_C1_207:
			break;
		case CLD_C1_208:
			break;
		case CLD_C1_209:
			break;
		case CLD_C1_210:
			break;
		case CLD_C1_211:
			break;
		case CLD_C1_212:
			break;
		case CLD_C1_213:
			break;
		case CLD_C1_214:
			break;
		case CLD_C1_215:
			break;
		case CLD_C1_216:
			break;
		case CLD_C1_217:
			break;
		case CLD_C1_218:
			break;
		case CLD_C1_219:
			break;
		case CLD_C1_220:
			break;
		case CLD_C1_221:
			break;
		case CLD_C1_222:
			break;
		case CLD_C1_223:
			break;
		case CLD_C1_224:
			break;
		case CLD_C1_225:
			break;
		case CLD_C1_226:
			break;
		case CLD_C1_227:
			break;
		case CLD_C1_228:
			break;
		case CLD_C1_229:
			break;
		case CLD_C1_230:
			break;
		case CLD_C1_231:
			break;
		case CLD_C1_232:
			break;
		case CLD_C1_233:
			break;
		case CLD_C1_234:
			break;
		case CLD_C1_235:
			break;
		case CLD_C1_236:
			break;
		case CLD_C1_237:
			break;
		case CLD_C1_238:
			break;
		case CLD_C1_239:
			break;
		case CLD_C1_240:
			break;
		case CLD_C1_241:
			break;
		case CLD_C1_242:
			break;
		case CLD_C1_243:
			break;
		case CLD_C1_244:
			break;
		case CLD_C1_245:
			break;
		case CLD_C1_246:
			break;
		case CLD_C1_247:
			break;
		case CLD_C1_248:
			break;
		case CLD_C1_249:
			break;
		case CLD_C1_250:
			break;
		case CLD_C1_251:
			break;
		case CLD_C1_252:
			break;
		case CLD_C1_253:
			break;
		case CLD_C1_254:
			break;
		case CLD_C1_255:
			break;
		case CLD_C1_256:
			break;
		case CLD_C1_257:
			break;
		case CLD_C1_258:
			break;
		case CLD_C1_259:
			break;
		case CLD_C1_260:
			break;
		case CLD_C1_261:
			break;
		case CLD_C1_262:
			break;
		case CLD_C1_263:
			break;
		case CLD_C1_264:
			break;
		case CLD_C1_265:
			break;
		case CLD_C1_266:
			break;
		case CLD_C1_267:
			break;
		case CLD_C1_268:
			break;
		case CLD_C1_269:
			break;
		case CLD_C1_270:
			break;
		case CLD_C1_271:
			break;
		case CLD_C1_272:
			break;
		case CLD_C1_273:
			break;
		case CLD_C1_274:
			break;
		case CLD_C1_275:
			break;
		case CLD_C1_276:
			break;
		case CLD_C1_277:
			break;
		case CLD_C1_278:
			break;
		case CLD_C1_279:
			break;
		case CLD_C1_280:
			break;
		case CLD_C1_281:
			break;
		case CLD_C1_282:
			break;
		case CLD_C1_283:
			break;
		case CLD_C1_284:
			break;
		case CLD_C1_285:
			break;
		case CLD_C1_286:
			break;
		case CLD_C1_287:
			break;
		case CLD_C1_288:
			break;
		case CLD_C1_289:
			break;
		case CLD_C1_290:
			break;
		case CLD_C1_291:
			break;
		case CLD_C1_292:
			break;
		case CLD_C1_293:
			break;
		case CLD_C1_294:
			break;
		case CLD_C1_295:
			break;
		case CLD_C1_296:
			break;
		case CLD_C1_297:
			break;
		case CLD_C1_298:
			break;
		case CLD_C1_299:
			break;
		case CLD_C1_300:
			break;
		case CLD_C2_151:
			break;
		case CLD_C2_152:
			break;
		case CLD_C2_153:
			break;
		case CLD_C2_154:
			break;
		case CLD_C2_155:
			break;
		case CLD_C2_156:
			break;
		case CLD_C2_157:
			break;
		case CLD_C2_158:
			break;
		case CLD_C2_159:
			break;
		case CLD_C2_160:
			break;
		case CLD_C2_161:
			break;
		case CLD_C2_162:
			break;
		case CLD_C2_163:
			break;
		case CLD_C2_164:
			break;
		case CLD_C2_165:
			break;
		case CLD_C2_166:
			break;
		case CLD_C2_167:
			break;
		case CLD_C2_168:
			break;
		case CLD_C2_169:
			break;
		case CLD_C2_170:
			break;
		case CLD_C2_171:
			break;
		case CLD_C2_172:
			break;
		case CLD_C2_173:
			break;
		case CLD_C2_174:
			break;
		case CLD_C2_175:
			break;
		case CLD_C2_176:
			break;
		case CLD_C2_177:
			break;
		case CLD_C2_178:
			break;
		case CLD_C2_179:
			break;
		case CLD_C2_180:
			break;
		case CLD_C2_181:
			break;
		case CLD_C2_182:
			break;
		case CLD_C2_183:
			break;
		case CLD_C2_184:
			break;
		case CLD_C2_185:
			break;
		case CLD_C2_186:
			break;
		case CLD_C2_187:
			break;
		case CLD_C2_188:
			break;
		case CLD_C2_189:
			break;
		case CLD_C2_190:
			break;
		case CLD_C2_191:
			break;
		case CLD_C2_192:
			break;
		case CLD_C2_193:
			break;
		case CLD_C2_194:
			break;
		case CLD_C2_195:
			break;
		case CLD_C2_196:
			break;
		case CLD_C2_197:
			break;
		case CLD_C2_198:
			break;
		case CLD_C2_199:
			break;
		case CLD_C2_200:
			break;
		case CLD_C2_201:
			break;
		case CLD_C2_202:
			break;
		case CLD_C2_203:
			break;
		case CLD_C2_204:
			break;
		case CLD_C2_205:
			break;
		case CLD_C2_206:
			break;
		case CLD_C2_207:
			break;
		case CLD_C2_208:
			break;
		case CLD_C2_209:
			break;
		case CLD_C2_210:
			break;
		case CLD_C2_211:
			break;
		case CLD_C2_212:
			break;
		case CLD_C2_213:
			break;
		case CLD_C2_214:
			break;
		case CLD_C2_215:
			break;
		case CLD_C2_216:
			break;
		case CLD_C2_217:
			break;
		case CLD_C2_218:
			break;
		case CLD_C2_219:
			break;
		case CLD_C2_220:
			break;
		case CLD_C2_221:
			break;
		case CLD_C2_222:
			break;
		case CLD_C2_223:
			break;
		case CLD_C2_224:
			break;
		case CLD_C2_225:
			break;
		case CLD_C2_226:
			break;
		case CLD_C2_227:
			break;
		case CLD_C2_228:
			break;
		case CLD_C2_229:
			break;
		case CLD_C2_230:
			break;
		case CLD_C2_231:
			break;
		case CLD_C2_232:
			break;
		case CLD_C2_233:
			break;
		case CLD_C2_234:
			break;
		case CLD_C2_235:
			break;
		case CLD_C2_236:
			break;
		case CLD_C2_237:
			break;
		case CLD_C2_238:
			break;
		case CLD_C2_239:
			break;
		case CLD_C2_240:
			break;
		case CLD_C2_241:
			break;
		case CLD_C2_242:
			break;
		case CLD_C2_243:
			break;
		case CLD_C2_244:
			break;
		case CLD_C2_245:
			break;
		case CLD_C2_246:
			break;
		case CLD_C2_247:
			break;
		case CLD_C2_248:
			break;
		case CLD_C2_249:
			break;
		case CLD_C2_250:
			break;
		case CLD_C2_251:
			break;
		case CLD_C2_252:
			break;
		case CLD_C2_253:
			break;
		case CLD_C2_254:
			break;
		case CLD_C2_255:
			break;
		case CLD_C2_256:
			break;
		case CLD_C2_257:
			break;
		case CLD_C2_258:
			break;
		case CLD_C2_259:
			break;
		case CLD_C2_260:
			break;
		case CLD_C2_261:
			break;
		case CLD_C2_262:
			break;
		case CLD_C2_263:
			break;
		case CLD_C2_264:
			break;
		case CLD_C2_265:
			break;
		case CLD_C2_266:
			break;
		case CLD_C2_267:
			break;
		case CLD_C2_268:
			break;
		case CLD_C2_269:
			break;
		case CLD_C2_270:
			break;
		case CLD_C2_271:
			break;
		case CLD_C2_272:
			break;
		case CLD_C2_273:
			break;
		case CLD_C2_274:
			break;
		case CLD_C2_275:
			break;
		case CLD_C2_276:
			break;
		case CLD_C2_277:
			break;
		case CLD_C2_278:
			break;
		case CLD_C2_279:
			break;
		case CLD_C2_280:
			break;
		case CLD_C2_281:
			break;
		case CLD_C2_282:
			break;
		case CLD_C2_283:
			break;
		case CLD_C2_284:
			break;
		case CLD_C2_285:
			break;
		case CLD_C2_286:
			break;
		case CLD_C2_287:
			break;
		case CLD_C2_288:
			break;
		case CLD_C2_289:
			break;
		case CLD_C2_290:
			break;
		case CLD_C2_291:
			break;
		case CLD_C2_292:
			break;
		case CLD_C2_293:
			break;
		case CLD_C2_294:
			break;
		case CLD_C2_295:
			break;
		case CLD_C2_296:
			break;
		case CLD_C2_297:
			break;
		case CLD_C2_298:
			break;
		case CLD_C2_299:
			break;
		case CLD_C2_300:
			break;
		case CLD_C3_151:
			break;
		case CLD_C3_152:
			break;
		case CLD_C3_153:
			break;
		case CLD_C3_154:
			break;
		case CLD_C3_155:
			break;
		case CLD_C3_156:
			break;
		case CLD_C3_157:
			break;
		case CLD_C3_158:
			break;
		case CLD_C3_159:
			break;
		case CLD_C3_160:
			break;
		case CLD_C3_161:
			break;
		case CLD_C3_162:
			break;
		case CLD_C3_163:
			break;
		case CLD_C3_164:
			break;
		case CLD_C3_165:
			break;
		case CLD_C3_166:
			break;
		case CLD_C3_167:
			break;
		case CLD_C3_168:
			break;
		case CLD_C3_169:
			break;
		case CLD_C3_170:
			break;
		case CLD_C3_171:
			break;
		case CLD_C3_172:
			break;
		case CLD_C3_173:
			break;
		case CLD_C3_174:
			break;
		case CLD_C3_175:
			break;
		case CLD_C3_176:
			break;
		case CLD_C3_177:
			break;
		case CLD_C3_178:
			break;
		case CLD_C3_179:
			break;
		case CLD_C3_180:
			break;
		case CLD_C3_181:
			break;
		case CLD_C3_182:
			break;
		case CLD_C3_183:
			break;
		case CLD_C3_184:
			break;
		case CLD_C3_185:
			break;
		case CLD_C3_186:
			break;
		case CLD_C3_187:
			break;
		case CLD_C3_188:
			break;
		case CLD_C3_189:
			break;
		case CLD_C3_190:
			break;
		case CLD_C3_191:
			break;
		case CLD_C3_192:
			break;
		case CLD_C3_193:
			break;
		case CLD_C3_194:
			break;
		case CLD_C3_195:
			break;
		case CLD_C3_196:
			break;
		case CLD_C3_197:
			break;
		case CLD_C3_198:
			break;
		case CLD_C3_199:
			break;
		case CLD_C3_200:
			break;
		case CLD_C3_201:
			break;
		case CLD_C3_202:
			break;
		case CLD_C3_203:
			break;
		case CLD_C3_204:
			break;
		case CLD_C3_205:
			break;
		case CLD_C3_206:
			break;
		case CLD_C3_207:
			break;
		case CLD_C3_208:
			break;
		case CLD_C3_209:
			break;
		case CLD_C3_210:
			break;
		case CLD_C3_211:
			break;
		case CLD_C3_212:
			break;
		case CLD_C3_213:
			break;
		case CLD_C3_214:
			break;
		case CLD_C3_215:
			break;
		case CLD_C3_216:
			break;
		case CLD_C3_217:
			break;
		case CLD_C3_218:
			break;
		case CLD_C3_219:
			break;
		case CLD_C3_220:
			break;
		case CLD_C3_221:
			break;
		case CLD_C3_222:
			break;
		case CLD_C3_223:
			break;
		case CLD_C3_224:
			break;
		case CLD_C3_225:
			break;
		case CLD_C3_226:
			break;
		case CLD_C3_227:
			break;
		case CLD_C3_228:
			break;
		case CLD_C3_229:
			break;
		case CLD_C3_230:
			break;
		case CLD_C3_231:
			break;
		case CLD_C3_232:
			break;
		case CLD_C3_233:
			break;
		case CLD_C3_234:
			break;
		case CLD_C3_235:
			break;
		case CLD_C3_236:
			break;
		case CLD_C3_237:
			break;
		case CLD_C3_238:
			break;
		case CLD_C3_239:
			break;
		case CLD_C3_240:
			break;
		case CLD_C3_241:
			break;
		case CLD_C3_242:
			break;
		case CLD_C3_243:
			break;
		case CLD_C3_244:
			break;
		case CLD_C3_245:
			break;
		case CLD_C3_246:
			break;
		case CLD_C3_247:
			break;
		case CLD_C3_248:
			break;
		case CLD_C3_249:
			break;
		case CLD_C3_250:
			break;
		case CLD_C3_251:
			break;
		case CLD_C3_252:
			break;
		case CLD_C3_253:
			break;
		case CLD_C3_254:
			break;
		case CLD_C3_255:
			break;
		case CLD_C3_256:
			break;
		case CLD_C3_257:
			break;
		case CLD_C3_258:
			break;
		case CLD_C3_259:
			break;
		case CLD_C3_260:
			break;
		case CLD_C3_261:
			break;
		case CLD_C3_262:
			break;
		case CLD_C3_263:
			break;
		case CLD_C3_264:
			break;
		case CLD_C3_265:
			break;
		case CLD_C3_266:
			break;
		case CLD_C3_267:
			break;
		case CLD_C3_268:
			break;
		case CLD_C3_269:
			break;
		case CLD_C3_270:
			break;
		case CLD_C3_271:
			break;
		case CLD_C3_272:
			break;
		case CLD_C3_273:
			break;
		case CLD_C3_274:
			break;
		case CLD_C3_275:
			break;
		case CLD_C3_276:
			break;
		case CLD_C3_277:
			break;
		case CLD_C3_278:
			break;
		case CLD_C3_279:
			break;
		case CLD_C3_280:
			break;
		case CLD_C3_281:
			break;
		case CLD_C3_282:
			break;
		case CLD_C3_283:
			break;
		case CLD_C3_284:
			break;
		case CLD_C3_285:
			break;
		case CLD_C3_286:
			break;
		case CLD_C3_287:
			break;
		case CLD_C3_288:
			break;
		case CLD_C3_289:
			break;
		case CLD_C3_290:
			break;
		case CLD_C3_291:
			break;
		case CLD_C3_292:
			break;
		case CLD_C3_293:
			break;
		case CLD_C3_294:
			break;
		case CLD_C3_295:
			break;
		case CLD_C3_296:
			break;
		case CLD_C3_297:
			break;
		case CLD_C3_298:
			break;
		case CLD_C3_299:
			break;
		case CLD_C3_300:
			break;
		case CLMB_Zone1:
			break;
		case CLMB_Zone2:
			break;
		case CLMB_Zone3:
			break;
		case CLMB_Zone4:
			break;
		case CLMB_Zone5:
			break;
		case CLMB_Zone6:
			break;
		case CLMB_Zone7:
			break;
		case CLMB_Zone8:
			break;
		case CLMB_Zone9:
			break;
		case CLMB_Zone10:
			break;
		case CLMB_Zone11:
			break;
		case CLMB_Zone12:
			break;
		case CLMB_Zone13:
			break;
		case CLMB_Zone14:
			break;
		case CLMB_Zone15:
			break;
		case CLMB_Zone16:
			break;
		case CLMB_Zone17:
			break;
		case CLMB_Zone18:
			break;
		case CLMB_Zone19:
			break;
		case CLMB_Zone20:
			break;
		case CLMB_Zone21:
			break;
		case CLMB_Zone22:
			break;
		case CLMB_Zone23:
			break;
		case CLMB_Zone24:
			break;
		case CLMB_Zone25:
			break;
		case CLMB_Zone26:
			break;
		case CLMB_Zone27:
			break;
		case CLMB_Zone28:
			break;
		case CLMB_Zone29:
			break;
		case CLMB_Zone30:
			break;
		case CLMB_Zone31:
			break;
		case CLMB_Zone32:
			break;
		case CLMB_Zone33:
			break;
		case CLMB_Zone34:
			break;
		case CLMB_Zone35:
			break;
		case CLMB_Zone36:
			break;
		case CLMB_Zone37:
			break;
		case CLMB_Zone38:
			break;
		case CLMB_Zone39:
			break;
		case CLMB_Zone40:
			break;
		case CLMB_Zone41:
			break;
		case CLMB_Zone42:
			break;
		case CLMB_Zone43:
			break;
		case CLMB_Zone44:
			break;
		case CLMB_Zone45:
			break;
		case CLMB_Zone46:
			break;
		case CLMB_Zone47:
			break;
		case CLMB_Zone48:
			break;
		case CLMB_Zone49:
			break;
		case CLMB_Zone50:
			break;
		case CLMB_Zone51:
			break;
		case CLMB_Zone52:
			break;
		case CLMB_Zone53:
			break;
		case CLMB_Zone54:
			break;
		case CLMB_Zone55:
			break;
		case CLMB_Zone56:
			break;
		case CLMB_Zone57:
			break;
		case CLMB_Zone58:
			break;
		case CLMB_Zone59:
			break;
		case CLMB_Zone60:
			break;
		case CLMB_Zone61:
			break;
		case CLMB_Zone62:
			break;
		case CLMB_Zone63:
			break;
		case CLMB_Zone64:
			break;
		case CLMB_Zone65:
			break;
		case CLMB_Zone66:
			break;
		case CLMB_Zone67:
			break;
		case CLMB_Zone68:
			break;
		case CLMB_Zone69:
			break;
		case CLMB_Zone70:
			break;
		case CLMB_Zone71:
			break;
		case CLMB_Zone72:
			break;
		case CLMB_Zone73:
			break;
		case CLMB_Zone74:
			break;
		case CLMB_Zone75:
			break;
		case CLMB_Zone76:
			break;
		case CLMB_Zone77:
			break;
		case CLMB_Zone78:
			break;
		case CLMB_Zone79:
			break;
		case CLMB_Zone80:
			break;
		case CLMB_Zone81:
			break;
		case CLMB_Zone82:
			break;
		case CLMB_Zone83:
			break;
		case CLMB_Zone84:
			break;
		case CLMB_Zone85:
			break;
		case CLMB_Zone86:
			break;
		case CLMB_Zone87:
			break;
		case CLMB_Zone88:
			break;
		case CLMB_Zone89:
			break;
		case CLMB_Zone90:
			break;
		case CLMB_Zone91:
			break;
		case CLMB_Zone92:
			break;
		case CLMB_Zone93:
			break;
		case CLMB_Zone94:
			break;
		case CLMB_Zone95:
			break;
		case CLMB_Zone96:
			break;
		case CLMB_Zone97:
			break;
		case CLMB_Zone98:
			break;
		case CLMB_Zone99:
			break;
		case CLMB_Zone100:
			break;
		case CLGPU_Zone1:
			break;
		case CLGPU_Zone2:
			break;
		case CLGPU_Zone3:
			break;
		case CLGPU_Zone4:
			break;
		case CLGPU_Zone5:
			break;
		case CLGPU_Zone6:
			break;
		case CLGPU_Zone7:
			break;
		case CLGPU_Zone8:
			break;
		case CLGPU_Zone9:
			break;
		case CLGPU_Zone10:
			break;
		case CLGPU_Zone11:
			break;
		case CLGPU_Zone12:
			break;
		case CLGPU_Zone13:
			break;
		case CLGPU_Zone14:
			break;
		case CLGPU_Zone15:
			break;
		case CLGPU_Zone16:
			break;
		case CLGPU_Zone17:
			break;
		case CLGPU_Zone18:
			break;
		case CLGPU_Zone19:
			break;
		case CLGPU_Zone20:
			break;
		case CLGPU_Zone21:
			break;
		case CLGPU_Zone22:
			break;
		case CLGPU_Zone23:
			break;
		case CLGPU_Zone24:
			break;
		case CLGPU_Zone25:
			break;
		case CLGPU_Zone26:
			break;
		case CLGPU_Zone27:
			break;
		case CLGPU_Zone28:
			break;
		case CLGPU_Zone29:
			break;
		case CLGPU_Zone30:
			break;
		case CLGPU_Zone31:
			break;
		case CLGPU_Zone32:
			break;
		case CLGPU_Zone33:
			break;
		case CLGPU_Zone34:
			break;
		case CLGPU_Zone35:
			break;
		case CLGPU_Zone36:
			break;
		case CLGPU_Zone37:
			break;
		case CLGPU_Zone38:
			break;
		case CLGPU_Zone39:
			break;
		case CLGPU_Zone40:
			break;
		case CLGPU_Zone41:
			break;
		case CLGPU_Zone42:
			break;
		case CLGPU_Zone43:
			break;
		case CLGPU_Zone44:
			break;
		case CLGPU_Zone45:
			break;
		case CLGPU_Zone46:
			break;
		case CLGPU_Zone47:
			break;
		case CLGPU_Zone48:
			break;
		case CLGPU_Zone49:
			break;
		case CLGPU_Zone50:
			break;
		case CLKLP_Zone20:
			break;
		case CLKLP_Zone21:
			break;
		case CLKLP_Zone22:
			break;
		case CLKLP_Zone23:
			break;
		case CLKLP_Zone24:
			break;
		case CLKLP_Zone25:
			break;
		case CLKLP_Zone26:
			break;
		case CLKLP_Zone27:
			break;
		case CLKLP_Zone28:
			break;
		case CLKLP_Zone29:
			break;
		case CLKLP_Zone30:
			break;
		case CLKLP_Zone31:
			break;
		case CLKLP_Zone32:
			break;
		case CLKLP_Zone33:
			break;
		case CLKLP_Zone34:
			break;
		case CLKLP_Zone35:
			break;
		case CLKLP_Zone36:
			break;
		case CLKLP_Zone37:
			break;
		case CLKLP_Zone38:
			break;
		case CLKLP_Zone39:
			break;
		case CLKLP_Zone40:
			break;
		case CLKLP_Zone41:
			break;
		case CLKLP_Zone42:
			break;
		case CLKLP_Zone43:
			break;
		case CLKLP_Zone44:
			break;
		case CLKLP_Zone45:
			break;
		case CLKLP_Zone46:
			break;
		case CLKLP_Zone47:
			break;
		case CLKLP_Zone48:
			break;
		case CLKLP_Zone49:
			break;
		case CLKLP_Zone50:
			break;
		case CLK_Profile:
			break;
		case CLI_Oem101:
			break;
		case CLI_Oem102:
			break;
		case CLI_Oem103:
			break;
		case CLI_Oem104:
			break;
		case CLI_Oem105:
			break;
		case CLI_Oem106:
			break;
		case CLI_Oem107:
			break;
		case CLI_Oem108:
			break;
		case CLI_Oem109:
			break;
		case CLI_Oem110:
			break;
		case CLI_Oem111:
			break;
		case CLI_Oem112:
			break;
		case CLI_Oem113:
			break;
		case CLI_Oem114:
			break;
		case CLI_Oem115:
			break;
		case CLI_Oem116:
			break;
		case CLI_Oem117:
			break;
		case CLI_Oem118:
			break;
		case CLI_Oem119:
			break;
		case CLI_Oem120:
			break;
		case CLI_Oem121:
			break;
		case CLI_Oem122:
			break;
		case CLI_Oem123:
			break;
		case CLI_Oem124:
			break;
		case CLI_Oem125:
			break;
		case CLI_Oem126:
			break;
		case CLI_Oem127:
			break;
		case CLI_Oem128:
			break;
		case CLI_Oem129:
			break;
		case CLI_Oem130:
			break;
		case CLI_Oem131:
			break;
		case CLI_Oem132:
			break;
		case CLI_Oem133:
			break;
		case CLI_Oem134:
			break;
		case CLI_Oem135:
			break;
		case CLI_Oem136:
			break;
		case CLI_Oem137:
			break;
		case CLI_Oem138:
			break;
		case CLI_Oem139:
			break;
		case CLI_Oem140:
			break;
		case CLI_Oem141:
			break;
		case CLI_Oem142:
			break;
		case CLI_Oem143:
			break;
		case CLI_Oem144:
			break;
		case CLI_Oem145:
			break;
		case CLI_Oem146:
			break;
		case CLI_Oem147:
			break;
		case CLI_Oem148:
			break;
		case CLI_Oem149:
			break;
		case CLI_Oem150:
			break;
		case CLI_Oem151:
			break;
		case CLI_Oem152:
			break;
		case CLI_Oem153:
			break;
		case CLI_Oem154:
			break;
		case CLI_Oem155:
			break;
		case CLI_Oem156:
			break;
		case CLI_Oem157:
			break;
		case CLI_Oem158:
			break;
		case CLI_Oem159:
			break;
		case CLI_Oem160:
			break;
		case CLI_Oem161:
			break;
		case CLI_Oem162:
			break;
		case CLI_Oem163:
			break;
		case CLI_Oem164:
			break;
		case CLI_Oem165:
			break;
		case CLI_Oem166:
			break;
		case CLI_Oem167:
			break;
		case CLI_Oem168:
			break;
		case CLI_Oem169:
			break;
		case CLI_Oem170:
			break;
		case CLI_Oem171:
			break;
		case CLI_Oem172:
			break;
		case CLI_Oem173:
			break;
		case CLI_Oem174:
			break;
		case CLI_Oem175:
			break;
		case CLI_Oem176:
			break;
		case CLI_Oem177:
			break;
		case CLI_Oem178:
			break;
		case CLI_Oem179:
			break;
		case CLI_Oem180:
			break;
		case CLI_Oem181:
			break;
		case CLI_Oem182:
			break;
		case CLI_Oem183:
			break;
		case CLI_Oem184:
			break;
		case CLI_Oem185:
			break;
		case CLI_Oem186:
			break;
		case CLI_Oem187:
			break;
		case CLI_Oem188:
			break;
		case CLI_Oem189:
			break;
		case CLI_Oem190:
			break;
		case CLI_Oem191:
			break;
		case CLI_Oem192:
			break;
		case CLI_Oem193:
			break;
		case CLI_Oem194:
			break;
		case CLI_Oem195:
			break;
		case CLI_Oem196:
			break;
		case CLI_Oem197:
			break;
		case CLI_Oem198:
			break;
		case CLI_Oem199:
			break;
		case CLI_Oem200:
			break;
		case CLI_Oem201:
			break;
		case CLI_Oem202:
			break;
		case CLI_Oem203:
			break;
		case CLI_Oem204:
			break;
		case CLI_Oem205:
			break;
		case CLI_Oem206:
			break;
		case CLI_Oem207:
			break;
		case CLI_Oem208:
			break;
		case CLI_Oem209:
			break;
		case CLI_Oem210:
			break;
		case CLI_Oem211:
			break;
		case CLI_Oem212:
			break;
		case CLI_Oem213:
			break;
		case CLI_Oem214:
			break;
		case CLI_Oem215:
			break;
		case CLI_Oem216:
			break;
		case CLI_Oem217:
			break;
		case CLI_Oem218:
			break;
		case CLI_Oem219:
			break;
		case CLI_Oem220:
			break;
		case CLI_Oem221:
			break;
		case CLI_Oem222:
			break;
		case CLI_Oem223:
			break;
		case CLI_Oem224:
			break;
		case CLI_Oem225:
			break;
		case CLI_Oem226:
			break;
		case CLI_Oem227:
			break;
		case CLI_Oem228:
			break;
		case CLI_Oem229:
			break;
		case CLI_Oem230:
			break;
		case CLI_Oem231:
			break;
		case CLI_Oem232:
			break;
		case CLI_Oem233:
			break;
		case CLI_Oem234:
			break;
		case CLI_Oem235:
			break;
		case CLI_Oem236:
			break;
		case CLI_Oem237:
			break;
		case CLI_Oem238:
			break;
		case CLI_Oem239:
			break;
		case CLI_Oem240:
			break;
		case CLI_Oem241:
			break;
		case CLI_Oem242:
			break;
		case CLI_Oem243:
			break;
		case CLI_Oem244:
			break;
		case CLI_Oem245:
			break;
		case CLI_Oem246:
			break;
		case CLI_Oem247:
			break;
		case CLI_Oem248:
			break;
		case CLI_Oem249:
			break;
		case CLI_Oem250:
			break;
		case CLM_7:
			break;
		case CLM_8:
			break;
		case CLM_9:
			break;
		case CLM_10:
			break;
		case CLM_11:
			break;
		case CLM_12:
			break;
		case CLM_13:
			break;
		case CLM_14:
			break;
		case CLM_15:
			break;
		case CLM_16:
			break;
		case CLM_17:
			break;
		case CLM_18:
			break;
		case CLM_19:
			break;
		case CLM_20:
			break;
		}

		return EKeys::Invalid;
	}
};