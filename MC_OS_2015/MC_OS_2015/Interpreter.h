#include <string.h>
#include <string>
#include <iostream>
using namespace std;
namespace AssemblerInterpreter
{
	/// <summary>
	/// Wartoœci zakodowanego rozkazu.
	/// </summary>
	enum AssemblerCode : short
	{
		ADD_A_value = 1,
		ADD_A_B = 2,
		ADD_A_adress = 3,
		SUB_A_value = 4,
		SUB_A_B = 5,
		SUB_A_adress = 6,
		MOV_A_value = 7,
		MOV_A_B = 8,
		MOV_A_adress = 9,
		MOV_B_value = 10,
		MOV_B_A = 11,
		MOV_B_adress = 12,
		MOV_adress_A = 13,
		MOV_adress_B = 14,
		HALT = 20,
		XC = 30,
		XD = 31,
		XH = 32,
		XR = 33,
		XS = 34,
		XY = 35,
		XY2 = 36,
		XZ = 37,
		XA = 38,
		XA2 = 39,
		XB = 40
	};
}
static class AssemblerInterpreter
{
	static string command;

	/// <summary>
	/// Koduje rozkaz i zwraca jego wartoœæ.
	/// </summary>
	/// <param name="command">Linia z rozkazem.</param>
	/// <returns>Wartosc rozkazu.</returns>
	static short EncodeID(string newCommand)
	{
		if (newCommand != "HALT")
		{

			string split = newCommand.Split(new char[] { ' ', ',', '[', ']' }, StringSplitOptions.RemoveEmptyEntries);

			if (split.length() > 1)
			{
				command = split[0] + split[1];
			}
			else
			{
				command = split[0];
			}

			if (command[0] != 'X')
			{
				if (newCommand[6] == '[')
				{
					command = split[0] + split[1];
					command += '[';
				}
				else
				{
					if (newCommand[4] == '[')
					{
						command = split[0] + "d" + split[2];
					}
					else
					{
						command = split[0] + split[1];
						if (newCommand[6] == 'B' || newCommand[6] == 'A')
						{
							command += split[2];
						}
						else
						{
							command += 'd';
						}
					}
				}
			}
			else
			{
				command = "X";
			}

			//Console.WriteLine(command);

			switch (command)
			{
			case "ADDAd":
			{
				
				return Convert.ToInt16(AssemblerCode.ADD_A_value);
			}
			case "ADDAB":
			{
				return Convert.ToInt16(AssemblerCode.ADD_A_B);
			}
			case "ADDA[":
			{
				return Convert.ToInt16(AssemblerCode.ADD_A_adress);
			}
			case "SUBAd":
			{
				return Convert.ToInt16(AssemblerCode.SUB_A_value);
			}
			case "SUBAB":
			{
				return Convert.ToInt16(AssemblerCode.SUB_A_B);
			}
			case "SUBA[":
			{
				return Convert.ToInt16(AssemblerCode.SUB_A_adress);
			}
			case "MOVAd":
			{
				return Convert.ToInt16(AssemblerCode.MOV_A_value);
			}
			case "MOVAB":
			{
				return Convert.ToInt16(AssemblerCode.MOV_A_B);
			}
			case "MOVA[":
			{
				return Convert.ToInt16(AssemblerCode.MOV_A_adress);
			}
			case "MOVBd":
			{
				return Convert.ToInt16(AssemblerCode.MOV_B_value);
			}
			case "MOVBA":
			{
				return Convert.ToInt16(AssemblerCode.MOV_B_A);
			}
			case "MOVB[":
			{
				return Convert.ToInt16(AssemblerCode.MOV_B_adress);
			}
			case "MOVdA":
			{
				return Convert.ToInt16(AssemblerCode.MOV_adress_A);
			}
			case "MOVdB":
			{
				return Convert.ToInt16(AssemblerCode.MOV_adress_B);
			}
			case "X":
			{
				switch (command[1])
				{
				case 'C':
				{
					return Convert.ToInt16(AssemblerCode.XC);
				}
				case 'D':
				{
					return Convert.ToInt16(AssemblerCode.XD);
				}
				case 'H':
				{
					return Convert.ToInt16(AssemblerCode.XH);
				}
				case 'R':
				{
					return Convert.ToInt16(AssemblerCode.XR);
				}
				case 'S':
				{
					return Convert.ToInt16(AssemblerCode.XS);
				}
				case 'Y':
				{
					split = newCommand.Split(new char[] { ' ', ',', '[', ']' }, StringSplitOptions.RemoveEmptyEntries);
					if (split.Length > 1)
					{
						return Convert.ToInt16(AssemblerCode.XY2);
					}
					else
					{
						return Convert.ToInt16(AssemblerCode.XY);
					}
				}
				case 'Z':
				{
					return Convert.ToInt16(AssemblerCode.XZ);
				}
				case 'A':
				{
					split = newCommand.Split(new char[] { ' ', ',', '[', ']' }, StringSplitOptions.RemoveEmptyEntries);
					if (split.Length > 1)
					{
						return Convert.ToInt16(AssemblerCode.XA2);
					}
					else
					{
						return Convert.ToInt16(AssemblerCode.XA);
					}
				}
				case 'B':
				{
					return Convert.ToInt16(AssemblerCode.XB);
				}
				default:
					Console.WriteLine("nieprawidlowy kod rozkazu");
					return 0;
				}
				break;
			}
			default:
				Console.WriteLine("nieprawidlowy kod rozkazu");
				return 0;
			}
		}
		else
		{
			return Convert.ToInt16(AssemblerCode.HALT);
		}
	}