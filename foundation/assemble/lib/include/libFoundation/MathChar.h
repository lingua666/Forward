
#ifndef __MATHCHAR_20160418173310_1438860785_H__
#define __MATHCHAR_20160418173310_1438860785_H__

#include <libString_v30/string.hpp>

namespace	_foundation_{
	
	template<typename T1>
	struct MathChar
	{
		static T1 NonBracketsCalculation(const T1& Formula)
		{
			int iMulPos = Formula.find("*");
			int iDivPos = Formula.find("/");

			int iRet = 0.0;

			int iPos = __min(iMulPos, iDivPos) == -1 ? (iMulPos == -1 ? iDivPos : iMulPos) : __min(iMulPos, iDivPos);
			if (iPos == -1)
			{//²»´æÔÚ³Ë³ý·¨
				Int8 Sig = 0;
				T1 Number;
				for (int i = 0; i < Formula.size(); i++)
				{
					char c = Formula[i];
					if (Formula[i] >= '0' && Formula[i] <= '9')
					{//Êý×Ö
						Number.append(&Formula[i], 1);
					}
					else if (Formula[i] == '+')
					{//ºöÂÔ
						if (Sig == 0)
						{
							Sig = 1;
							iRet = T1::StringToInt(Number);
							Number.update_size(0);
						}
						else
						{
							iRet += T1::StringToInt(Number) * Sig;
							Sig = 1;
							Number.update_size(0);
						}
					}
					else if (Formula[i] == '-')
					{
						if (Sig == 0)
						{
							Sig = -1;
							iRet = T1::StringToInt(Number);
							Number.update_size(0);
						}
						else
						{
							iRet += T1::StringToInt(Number) * Sig;
							Sig = -1;
							Number.update_size(0);
						}
					}
					else if (Formula[i] == ' ')
					{//ºöÂÔ

					}
					else
					{
						printf("error NonBracketsCalculation() Unknown symbol;%s\r\n", Formula.c_str());
						return "";
					}
				}

				if (Number.size() > 0)
				{
					iRet += T1::StringToInt(Number) * Sig;
				}

				return T1::NumberToStr(iRet);
			}
			else
			{//¼ÆËã³Ë³ý
				T1 sResult;

				int iOffset = iPos - 1;
				if (iOffset < 0)
				{
					printf("error NonBracketsCalculation() Unknown symbol;%s\r\n", Formula.c_str());
					return "";
				}

				T1 Number;

				if (iOffset > 0)
				{
					do
					{
						if ((Formula[iOffset] < '0' || Formula[iOffset] > '9') && Formula[iOffset] != ' ')
							break;
					} while (--iOffset > 0);
				}
				
				if (iOffset > 0)
				{
					Number.append(&Formula[iOffset + 1], iPos - iOffset - 1);
					sResult.append(&Formula[0], iOffset + 1);
				}
				else
				{
					Number.append(&Formula[iOffset], iPos - iOffset);
					sResult.append(&Formula[0], iOffset);
				}

				iRet = T1::StringToInt(Number);
				Number.update_size(0);

				iOffset = iPos + 1;
				do
				{
					if ((Formula[iOffset] < '0' || Formula[iOffset] > '9') && Formula[iOffset] != ' ')
						break;
				} while (++iOffset < Formula.size());

				Number.append(&Formula[iPos + 1], iOffset - iPos - 1);

				if (Formula[iPos] == '*')
					iRet *= T1::StringToInt(Number);
				else  if (Formula[iPos] == '/')
				{
					if (iRet != 0)
						iRet /= T1::StringToInt(Number);
				}

				sResult += T1::NumberToStr(iRet);
				if (iOffset < Formula.size() - 1)
					sResult.append(&Formula[iOffset], Formula.size() - iOffset);

				if (sResult.find('+') == -1 && sResult.find('-') == -1 &&
					sResult.find('*') == -1 && sResult.find('/') == -1)
					return sResult;

				return NonBracketsCalculation(sResult);
			}
		}

		static T1 BracketsCalculation(const T1& Formula)
		{
			int iPos = 0, iOffset = 0;
			T1 sData = Formula;
			T1 sResult;
			do
			{
				iPos = sData.find(')');
				if (iPos == -1)
				{//Ã»ÓÐÀ¨ºÅ
					break;
				}
				else
				{//ÓÐÀ¨ºÅ
					iOffset = iPos;
					if (iOffset <= 0)
					{
						printf("error BracketsCalculation() Unknown symbol;%s\r\n", Formula.c_str());
						return "";
					}

					do
					{
						if (sData[iOffset] == '(')
							break;
					} while (--iOffset > 0);

					sResult.update_size(0);
					sResult.append(&sData[iOffset + 1], iPos - (iOffset + 1));
					sResult = NonBracketsCalculation(sResult);
					sResult.append(&sData[iPos + 1], sData.size() - (iPos + 1));
					sData.update_size(iOffset);
					sData += sResult;
				}

			} while (true);

			if (sResult.find('+') == -1 && sResult.find('-') == -1 &&
				sResult.find('*') == -1 && sResult.find('/') == -1)
				return sResult;
			return NonBracketsCalculation(sResult);
		}

		static T1 NonBracketsCalculationFloat(const T1& Formula)
		{
			int iMulPos = Formula.find("*");
			int iDivPos = Formula.find("/");

			float fRet = 0.0;

			int iPos = __min(iMulPos, iDivPos) == -1 ? (iMulPos == -1 ? iDivPos : iMulPos) : __min(iMulPos, iDivPos);
			if (iPos == -1)
			{//²»´æÔÚ³Ë³ý·¨
				Int8 Sig = 0;
				T1 Number;
				for (int i = 0; i < Formula.size(); i++)
				{
					char c = Formula[i];
					if (Formula[i] >= '0' && Formula[i] <= '9' || Formula[i] == '.')
					{//Êý×Ö¡¢Ð¡Êýµã
						Number.append(&Formula[i], 1);
					}
					else if (Formula[i] == '+')
					{//ºöÂÔ
						if (Sig == 0)
						{
							Sig = 1;
							fRet = T1::StringToFloat(Number);
							Number.update_size(0);
						}
						else
						{
							fRet += T1::StringToFloat(Number) * Sig;
							Sig = 1;
							Number.update_size(0);
						}
					}
					else if (Formula[i] == '-')
					{
						if (Sig == 0)
						{
							Sig = -1;
							fRet = T1::StringToFloat(Number);
							Number.update_size(0);
						}
						else
						{
							fRet += T1::StringToFloat(Number) * Sig;
							Sig = -1;
							Number.update_size(0);
						}
					}
					else if (Formula[i] == ' ')
					{//ºöÂÔ

					}
					else
					{
						printf("error NonBracketsCalculation() Unknown symbol;%s\r\n", Formula.c_str());
						return "";
					}
				}

				if (Number.size() > 0)
				{
					fRet += T1::StringToFloat(Number) * Sig;
				}

				return T1::FloatToStr(fRet, 2);
			}
			else
			{//¼ÆËã³Ë³ý
				T1 sResult;

				int iOffset = iPos - 1;
				if (iOffset < 0)
				{
					printf("error NonBracketsCalculation() Unknown symbol;%s\r\n", Formula.c_str());
					return "";
				}

				T1 Number;

				if (iOffset > 0)
				{
					do
					{
						if ((Formula[iOffset] < '0' || Formula[iOffset] > '9') && Formula[iOffset] != ' ' && Formula[iOffset] != '.')
							break;
					} while (--iOffset > 0);
				}

				if (iOffset > 0)
				{
					Number.append(&Formula[iOffset + 1], iPos - iOffset - 1);
					sResult.append(&Formula[0], iOffset + 1);
				}
				else
				{
					Number.append(&Formula[iOffset], iPos - iOffset);
					sResult.append(&Formula[0], iOffset);
				}

				fRet = T1::StringToFloat(Number);
				Number.update_size(0);

				iOffset = iPos + 1;
				do
				{
					if ((Formula[iOffset] < '0' || Formula[iOffset] > '9') && Formula[iOffset] != ' ' && Formula[iOffset] != '.')
						break;
				} while (++iOffset < Formula.size());

				Number.append(&Formula[iPos + 1], iOffset - iPos - 1);

				if (Formula[iPos] == '*')
					fRet *= T1::StringToFloat(Number);
				else  if (Formula[iPos] == '/')
				{
					if(fRet != 0)
						fRet /= T1::StringToFloat(Number);
				}

				sResult += T1::FloatToStr(fRet, 2);
				if (iOffset < Formula.size() - 1)
					sResult.append(&Formula[iOffset], Formula.size() - iOffset);

				if (sResult.find('+') == -1 && sResult.find('-') == -1 &&
					sResult.find('*') == -1 && sResult.find('/') == -1)
					return sResult;

				return NonBracketsCalculationFloat(sResult);
			}
		}

		static T1 BracketsCalculationFloat(const T1& Formula)
		{
			int iPos = 0, iOffset = 0;
			T1 sData = Formula;
			T1 sResult;
			do
			{
				iPos = sData.find(')');
				if (iPos == -1)
				{//Ã»ÓÐÀ¨ºÅ
					break;
				}
				else
				{//ÓÐÀ¨ºÅ
					iOffset = iPos;
					if (iOffset <= 0)
					{
						printf("error BracketsCalculation() Unknown symbol;%s\r\n", Formula.c_str());
						return "";
					}

					do
					{
						if (sData[iOffset] == '(')
							break;
					} while (--iOffset > 0);

					sResult.update_size(0);
					sResult.append(&sData[iOffset + 1], iPos - (iOffset + 1));
					sResult = NonBracketsCalculationFloat(sResult);
					sResult.append(&sData[iPos + 1], sData.size() - (iPos + 1));
					sData.update_size(iOffset);
					sData += sResult;
				}

			} while (true);

			if (sResult.find('+') == -1 && sResult.find('-') == -1 &&
				sResult.find('*') == -1 && sResult.find('/') == -1)
				return sResult;
			return NonBracketsCalculationFloat(sResult);
		}

		static T1 Calculation(const T1& Formula)
		{
			if (Formula.find("(") != -1)
			{
				return BracketsCalculation(Formula);
			}

			return	NonBracketsCalculation(Formula);
		}

		static T1 CalculationFloat(const T1& Formula)
		{
			if (Formula.find("(") != -1)
			{
				return BracketsCalculationFloat (Formula);
			}

			return	NonBracketsCalculationFloat(Formula);
		}
	};


} //_foundation


#endif // __MODAFX_20160418173310_1438860785_H__
