#ifndef __ATFWD_BT_GBK_UTF8_H__
#define __ATFWD_BT_GBK_UTF8_H__

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include "code_convert_tab.h"

#define LOGI(...) fprintf(stderr, "I:" __VA_ARGS__)

typedef  uint8_t            boolean;     /* Boolean value type. */

typedef  uint32_t           uint32;      /* Unsigned 32 bit value */
typedef  uint16_t           uint16;      /* Unsigned 16 bit value */
typedef  uint8_t            uint8;       /* Unsigned 8  bit value */

typedef  int32_t            int32;       /* Signed 32 bit value */
typedef  int16_t            int16;       /* Signed 16 bit value */
typedef  int8_t             int8;        /* Signed 8  bit value */




//for cwssid 
static uint16 codeconvert_Gb2Unicode(uint16 vp_GB2312)
{
  int  First = 0;
  int  Last = CODE_CONVERT_MAXLENGTH_GBK-1;
  int  Mid;

  while ( Last >= First )
  {
    Mid = (First + Last) >> 1;

    if(vp_GB2312 < a_codeconvert_gb2ucs[Mid].gb)
    	Last = Mid - 1;

    else if(vp_GB2312 > a_codeconvert_gb2ucs[Mid].gb)
    	First = Mid + 1;

    else if(vp_GB2312 == a_codeconvert_gb2ucs[Mid].gb)
    	return a_codeconvert_gb2ucs[Mid].ucs;
  }

  return 0xffff;

}

/*P(*************************************************************************** 
 * Procedure name : codeconvert0_1Gb2UnicodeStr
 * Object :  
 *----------------------------------------------------------------------------* 
 * Input parameters  :
 * -------------------
 *                    uint8 *pp_GB2312
 *			  u8 *pp_Unicode
 * Output parameters :
 * -------------------
 *   			
 *
 * Used variables    :
 * -------------------
 *                    uint16 vp_GB2312
 * Used procedures   :
 * -------------------
 * 
 *----------------------------------------------------------------------------* 
 *----------------------------------------------------------------------------* 
 *                                    DESCRIPTION
 *			transfer gb2 string to unicode string
 *----------------------------------------------------------------------------* 
 ***************************************************************************)P*/

static void codeconvert_Gb2UnicodeStr(uint8 *pp_GB2312,uint8 *pp_Unicode)
{
	unsigned short code;
	unsigned short c0, c1;

	while(*pp_GB2312)
	{
	       if(*pp_GB2312 >= 0x20 && *pp_GB2312 <= 0x7E)
              {
                    *pp_Unicode = *pp_GB2312;    
		       pp_Unicode++;
			*pp_Unicode = 0x00;
                     pp_Unicode++;
		       pp_GB2312++;
              }
		else if((*pp_GB2312 == 0x5c) && (*(pp_GB2312 + 1) == 0x6E))
		{
			*pp_Unicode = 0x0a;//0x00;
                     pp_Unicode++;
                     *pp_Unicode = 0x00;//0x0a;
                     pp_Unicode++;
			pp_GB2312 += 2;
		}
		else if(*pp_GB2312 <= 0x80)
		{
			switch (*pp_GB2312)
			{
				case 0x11:
				{
				*pp_Unicode = 0x5F;//0x00;     
				 pp_Unicode++;
				*pp_Unicode = 0x00;//0x5F;
				break;
				}
		 		case 0x60:
				{
				*pp_Unicode = 0xBF;//0x00;     
				 pp_Unicode++;
				*pp_Unicode = 0x00;//0xBF;
				break;
				}
				/*case 0x40:
				{
				*pUcs2 = 0x00;     
				 pUcs2++;
				*pUcs2 = 0xA1;   
				break;
				}*/
				case 0x26:
				{
				*pp_Unicode = 0x26;//0x00;     
				 pp_Unicode++;
				*pp_Unicode = 0x00;//0x26;   
				break;
				}
				case 0x1D:
				{
				*pp_Unicode = 0xE6;//0x00;     
				 pp_Unicode++;
				*pp_Unicode = 0x00;//0xE6;    
				break;
				}
				case 0x1E:
				{
				*pp_Unicode = 0xDF;//00;     
				 pp_Unicode++;
				*pp_Unicode = 0x00;//DF;    
				break;
				}
				case 0x03:
				{
				*pp_Unicode = 0xA5;//00;     
				 pp_Unicode++;
				*pp_Unicode = 0x00;//A5;    
				break;
				}
				case 0x5F:
				{
				*pp_Unicode = 0xA7;//00;     
				 pp_Unicode++;
				*pp_Unicode = 0x00;//A7;    
				break;
				}
				case 0x01:
				{
				*pp_Unicode = 0xA3;//00;     
				 pp_Unicode++;
				*pp_Unicode = 0x00;//A3;    
				break;
				}
				case 0x24:
				{
				*pp_Unicode = 0xA4;//00;     
				 pp_Unicode++;
				*pp_Unicode = 0x00;//A4;    
				break;
				}
				case 0x02:
				{
				*pp_Unicode = 0x24;//00;     
				 pp_Unicode++;
				*pp_Unicode = 0x00;//24;    
				break;
				}
				case 0x0A:
				{
				*pp_Unicode = 0x20;//00;     
				 pp_Unicode++;
				*pp_Unicode = 0x00;//20;    				
				break;
				}
				case 0x80:
				{
				*pp_Unicode = 0x40;//00;     
				 pp_Unicode++;
				*pp_Unicode = 0x00;//40;    				
				break;
				}
				case 0x18:
				{
				*pp_Unicode = 0xA3;//03;     
				 pp_Unicode++;
				*pp_Unicode = 0x03;//A3;  
				break;
				}
				case 0x12:
				{
				*pp_Unicode = 0xA6;//03;     
				 pp_Unicode++;
				*pp_Unicode = 0x03;//A6;  
				break;
				}
				case 0x15:
				{
				*pp_Unicode = 0xA9;//03;     
				 pp_Unicode++;
				*pp_Unicode = 0x03;//A9;  
				break;
				}
			default:
				{
				*pp_Unicode = *pp_GB2312;//0x00;      /* It is a ASCII character */
				pp_Unicode++;
				*pp_Unicode = 0x00;//*pp_GB2312;     /* It is a ASCII character */
				break;
		 		}
			}
		pp_Unicode++;
		pp_GB2312++;
		}
		else if(((*pp_GB2312 > 0x80) && (*pp_GB2312 < 0xff)) && \
             (((*(pp_GB2312 + 1) >= 0x40) && (*(pp_GB2312 + 1) < 0x7f)) || \
             ((*(pp_GB2312 + 1) > 0x7f) && (*(pp_GB2312 + 1) < 0xff))))
		{
			c0 = *pp_GB2312;
			c1 = *(pp_GB2312 + 1);
			code = (c0 << 8) + c1;
            code = codeconvert_Gb2Unicode(code);
			//*pp_Unicode = code >> 8;
			*pp_Unicode = code & 0xff;
			pp_Unicode++;
			//*pp_Unicode = code & 0xff;
                    *pp_Unicode = code >> 8;
			pp_Unicode++;
			pp_GB2312 += 2;
		}
		else
		{
			pp_GB2312++;
		}
	}
	*pp_Unicode = 0;
	pp_Unicode++;
	*pp_Unicode = 0;
}

static uint16  codeconvert_UnicodeToGbk(uint16 vp_Unicode)
{
    int  First = 0;
    int  Last = CODE_CONVERT_MAXLENGTH_GBK-1;
    int  Mid;
    //Pretend 0x2015 exist
    // LOG_MSG_INFO1("codeconvert_UnicodeToGbk[%0x, %d, %d]", vp_Unicode, First, Last);
  
    if(0x2015==vp_Unicode)
    {
        vp_Unicode = 0x2014;
    }
    while ( Last >= First ) 
    {
        Mid = (First + Last) >> 1;
        //LOG_MSG_INFO1("====[%d, %0x]", Mid, a_CodeConvertTab_ucs2gb[Mid].ucs, 0);
        if(vp_Unicode < a_CodeConvertTab_ucs2gb[Mid].ucs)
        	Last = Mid - 1;

        else if(vp_Unicode > a_CodeConvertTab_ucs2gb[Mid].ucs)
        	First = Mid + 1;

        else if(vp_Unicode == a_CodeConvertTab_ucs2gb[Mid].ucs)
        	return a_CodeConvertTab_ucs2gb[Mid].gb;
    }

    return 0xffff;
}


static void codeconvert_UCS2ToGBKStr(uint16 *pp_Ucs2, uint8 *p_pGbk, uint32 vp_Ucs2Len)
{
    uint16 c16;
    uint8 c0,c1;

    // LOG_MSG_INFO1("codeconvert_UCS2ToGBKStr[%d]", vp_Ucs2Len, 0, 0);
    while((*pp_Ucs2)&&(vp_Ucs2Len-- >0))
    {
        // LOG_MSG_INFO1("=====[%0x, %d]", *pp_Ucs2, vp_Ucs2Len, 0);
        c0 = *pp_Ucs2/0x100;
        c1 = *pp_Ucs2&0xff;
        // LOG_MSG_INFO1("=====[%0x, %0x]", c0, c1, 0);
        if (c0==0)
        {   
            *p_pGbk = c1;
        	p_pGbk++;
        	pp_Ucs2++;
        	continue;
        }
        c16 = codeconvert_UnicodeToGbk(*pp_Ucs2);
            // LOG_MSG_INFO1("=====[%0x, %0x]", *pp_Ucs2, c16, 0);
        if (c16==0xffff)
        {
        	pp_Ucs2++;
        	continue;
        }
        *p_pGbk = c16/0x100;
        //*p_pGbk = c16 & 0xff;
        // LOG_MSG_INFO1("=====p_pGbk[%0x]", *p_pGbk, 0, 0);
        p_pGbk++;
        *p_pGbk = c16 & 0xff;
        //*p_pGbk = c16>>8;
        // LOG_MSG_INFO1("=====p_pGbk[%0x]", *p_pGbk, 0, 0);
        p_pGbk++;
        pp_Ucs2++;
        
        // LOG_MSG_INFO1("=====[%0x, %d]", *pp_Ucs2, vp_Ucs2Len, 0);
    }
    *p_pGbk = 0;
}

static int codeconvert_unicode2utf8Str(uint16 *in, int insize, uint8 *out)  
{  
    int i = 0;  
    int outsize = 0;  
    int charscount = 0;  
    //uint8_t *result = NULL;  
    uint8_t *tmp = NULL;  

    // LOG_MSG_INFO1("codeconvert_unicode2utf8[%0x, %d, %0x]", in, insize, out);
    if(in == NULL || out == NULL)
    {
        return -1;
    }
    
    charscount = insize;  
    //result = (uint8_t *)malloc(charscount * 3 + 1);  
    //memset(result, 0, charscount * 3 + 1);  
    tmp = out;//result;  
    // LOG_MSG_INFO1("====charscount[%d]", charscount, 0, 0);
    for (i = 0; i < charscount; i++)  
    {  
        uint16_t unicode = in[i];  
        // LOG_MSG_INFO1("====[%d, %0x]", i, unicode, 0);
         if ( unicode <= 0x0000007F )  
         {  
             // * U-00000000 - U-0000007F:  0xxxxxxx  
             *tmp     = (unicode & 0x7F);  
             tmp++;  
         }  
         else if ( unicode >= 0x00000080 && unicode <= 0x000007FF )  
         {  
             // * U-00000080 - U-000007FF:  110xxxxx 10xxxxxx  
             *(tmp+1) = (unicode & 0x3F) | 0x80;  
             *tmp     = ((unicode >> 6) & 0x1F) | 0xC0;  
             tmp += 2;
         }  
         else if ( unicode >= 0x00000800 && unicode <= 0x0000FFFF )  
         {  
             // * U-00000800 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx  
             *(tmp+2) = (unicode & 0x3F) | 0x80;  
             *(tmp+1) = ((unicode >>  6) & 0x3F) | 0x80;  
             *tmp     = ((unicode >> 12) & 0x0F) | 0xE0;   
             tmp += 3;
         }  
         else if ( unicode >= 0x00010000 && unicode <= 0x001FFFFF )  
         {  
             // * U-00010000 - U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx  
             *(tmp+3) = (unicode & 0x3F) | 0x80;  
             *(tmp+2) = ((unicode >>  6) & 0x3F) | 0x80;  
             *(tmp+1) = ((unicode >> 12) & 0x3F) | 0x80;  
             *tmp     = ((unicode >> 18) & 0x07) | 0xF0;  
             tmp += 4;
         }  
         else if ( unicode >= 0x00200000 && unicode <= 0x03FFFFFF )  
         {  
             // * U-00200000 - U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx  
             *(tmp+4) = (unicode & 0x3F) | 0x80;  
             *(tmp+3) = ((unicode >>  6) & 0x3F) | 0x80;  
             *(tmp+2) = ((unicode >> 12) & 0x3F) | 0x80;  
             *(tmp+1) = ((unicode >> 18) & 0x3F) | 0x80;  
             *tmp     = ((unicode >> 24) & 0x03) | 0xF8;   
             tmp += 5;
         }  
         else if ( unicode >= 0x04000000 && unicode <= 0x7FFFFFFF )  
         {  
             // * U-04000000 - U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx  
             *(tmp+5) = (unicode & 0x3F) | 0x80;  
             *(tmp+4) = ((unicode >>  6) & 0x3F) | 0x80;  
             *(tmp+3) = ((unicode >> 12) & 0x3F) | 0x80;  
             *(tmp+2) = ((unicode >> 18) & 0x3F) | 0x80;  
             *(tmp+1) = ((unicode >> 24) & 0x3F) | 0x80;  
             *tmp     = ((unicode >> 30) & 0x01) | 0xFC; 
             tmp += 6;
         }  
    }  
  
    //*tmp = '\0';  
    //*out = result;  
    return 0;  
}  

int codeconvert_get_utf8_size(uint8 *in)
{
   unsigned char c = *in;
//    LOG_MSG_INFO1("========codeconvert_get_utf8_size[%0x]", c, 0, 0);
    // 0xxxxxxx return 0
    // 10xxxxxx not exist
    // 110xxxxx return 2
    // 1110xxxx return 3
    // 11110xxx return 4
    // 111110xx return 5
    // 1111110x return 6
    if(c< 0x80) return 0;
    if(c>=0x80 && c<0xC0) return 1;
    if(c>=0xC0 && c<0xE0) return 2;
    if(c>=0xE0 && c<0xF0) return 3;
    if(c>=0xF0 && c<0xF8) return 4;
    if(c>=0xF8 && c<0xFC) return 5;
    if(c>=0xFC) return 6;

}

static int codeconvert_utf82unicodeStr(uint8 *in, uint16 *out, int *outsize)  
{  
    uint8 *pInput = in;  
    unsigned char *pOutput = (unsigned char *)out;
    int utfbytes = 0; 
    char b1, b2, b3, b4, b5, b6;
    
    int resultsize = 0;  
    uint16 *tmp = out;  

    // LOG_MSG_INFO1("codeconvert_utf82unicode[%0x, %0x, %0x]", in, out, outsize);
    if(in == NULL || out == NULL || outsize == NULL)
    {
        return -1;
    }

    // LOG_MSG_INFO1("====*pInput[%0x]", *pInput, 0, 0);
    while(*pInput)  
    {  
        utfbytes = codeconvert_get_utf8_size(pInput);
        // LOG_MSG_INFO1("====utfbytes[%d]", utfbytes, 0, 0);
        pOutput = (unsigned char *)(tmp);
        switch ( utfbytes )  
        {  
            case 0:  
                *pOutput     = *pInput;  
                utfbytes    += 1;  
                break;  
                
            case 2:  
                b1 = *pInput;  
                b2 = *(pInput + 1);  
                if ( (b2 & 0xE0) != 0x80 )  
                    return 0;  
                *pOutput     = (b1 << 6) + (b2 & 0x3F);  
                *(pOutput+1) = (b1 >> 2) & 0x07;  
                break;  
                
            case 3:  
                b1 = *pInput;  
                b2 = *(pInput + 1);  
                b3 = *(pInput + 2);  
                if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80) )  
                    return 0;  
                *pOutput     = (b2 << 6) + (b3 & 0x3F);  
                *(pOutput+1) = (b1 << 4) + ((b2 >> 2) & 0x0F);  
                break;  
                
            case 4:  
                b1 = *pInput;  
                b2 = *(pInput + 1);  
                b3 = *(pInput + 2);  
                b4 = *(pInput + 3);  
                if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)  
                        || ((b4 & 0xC0) != 0x80) )  
                    return 0;  
                *pOutput     = (b3 << 6) + (b4 & 0x3F);  
                *(pOutput+1) = (b2 << 4) + ((b3 >> 2) & 0x0F);  
                *(pOutput+2) = ((b1 << 2) & 0x1C)  + ((b2 >> 4) & 0x03);  
                break;  
                
            case 5:  
                b1 = *pInput;  
                b2 = *(pInput + 1);  
                b3 = *(pInput + 2);  
                b4 = *(pInput + 3);  
                b5 = *(pInput + 4);  
                if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)  
                        || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80) )  
                    return 0;  
                *pOutput     = (b4 << 6) + (b5 & 0x3F);  
                *(pOutput+1) = (b3 << 4) + ((b4 >> 2) & 0x0F);  
                *(pOutput+2) = (b2 << 2) + ((b3 >> 4) & 0x03);  
                *(pOutput+3) = (b1 << 6);  
                break;  
                
            case 6:  
                b1 = *pInput;  
                b2 = *(pInput + 1);  
                b3 = *(pInput + 2);  
                b4 = *(pInput + 3);  
                b5 = *(pInput + 4);  
                b6 = *(pInput + 5);  
                if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)  
                        || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80)  
                        || ((b6 & 0xC0) != 0x80) )  
                    return 0;  
                *pOutput     = (b5 << 6) + (b6 & 0x3F);  
                *(pOutput+1) = (b5 << 4) + ((b6 >> 2) & 0x0F);  
                *(pOutput+2) = (b3 << 2) + ((b4 >> 4) & 0x03);  
                *(pOutput+3) = ((b1 << 6) & 0x40) + (b2 & 0x3F);  
                break;  
                
            default:  
                return 0;  
                break;  
        }   

        tmp++;
        pInput += utfbytes;  
        resultsize++;

        // LOG_MSG_INFO1("====*pInput[%0x], resultsize[%d]", *pInput, resultsize, 0);
    }  
  

    *outsize = resultsize;   
    // LOG_MSG_INFO1("=========resultsize[%d]", resultsize, 0, 0);
    return 0;  
}  


#endif /*__ATFWD_BT_GBK_UTF8_H__*/