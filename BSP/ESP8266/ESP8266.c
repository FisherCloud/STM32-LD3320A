/******************** (C) COPYRIGHT  Ô´µØ¹¤×÷ÊÒ ********************************
 * ÎÄ¼þÃû  £ºesp8266.c
 * ÃèÊö    £ºÍê³É¶ÔESP8266µÄ¸÷ÖÖ²Ù×÷
 * ×÷Õß    £ºzhuoyingxingyu
 * ÌÔ±¦    £ºÔ´µØ¹¤×÷ÊÒhttp://vcc-gnd.taobao.com/
 * ÂÛÌ³µØÖ·£º¼«¿ÍÔ°µØ-Ç¶ÈëÊ½¿ª·¢ÂÛÌ³http://vcc-gnd.com/
 * °æ±¾¸üÐÂ: 2016-04-08
 * Ó²¼þÁ¬½Ó: ÎÞ
 * µ÷ÊÔ·½Ê½£ºJ-Link-OB
********************************************************************************/
#include "ESP8266.h"
#include "usart.h"
#include "usart2.h"
#include "delay.h"
#include "string.h"
#include "includes.h"

u8 Esp8266_DdtaBuff[BUFF_LEN];      //Êý¾Ý»º³åÇø
u8 IP_Buff[20];                     //´æ·Å8266µÄIP

/**
 * @file   ESP8266_STA_TCPServer
 * @brief  ESP8266ÉèÖÃSTAÄ£Ê½ÏÂTCP·þÎñÆ÷
 * @param  ÎÞ
 * @retval 0---7
 */
u8 ESP8266_STA_TCPServer(void)
{
    if(ESP8266_SoftReset(50))
    {
        return 1;    //Èí¼þ¸´Î»8266£¬³¬Ê±Ê±¼ä5s£¬Ê§°Ü¾Í·µ»Ø1
    }
    else
    {
        printf("ESP8266Èí¸´Î»³É¹¦\r\n");
    }

    delay_ms(200);

    if(ESP8266_SendCmd_OK("AT+CWMODE=1", 50))
    {
        return 2;    //ÉèÖÃ8266ÎªSTAÄ£Ê½£¬³¬Ê±Ê±¼ä5s£¬Ê§°Ü·µ»Ø2
    }
    else
    {
        printf("ESP8266ÉèÖÃSTAÄ£Ê½³É¹¦\r\n");
    }

    delay_ms(100);
    printf("ESP8266Á¬½ÓAPÖÐ... ...\r\n");

    if(ESP8266_ConnectionAP(AP_SSID, AP_PASS, 200))
    {
        return 3;    //ÉèÖÃ8266Á¬½ÓAP£¬³¬Ê±Ê±¼ä20s£¬Ê§°Ü·µ»Ø3
    }
    else
    {
        printf("ESP8266Á¬½ÓAP³É¹¦\r\n");
    }

    delay_ms(100);

    if(ESP8266_GetIP(IP_Buff, 50))
    {
        return 4;    //²éÑ¯8266µÄIP£¬³¬Ê±Ê±¼ä5s£¬Ê§°Ü·µ»Ø4
    }
    else
    {
        printf("ESP8266²éÑ¯IPµØÖ·³É¹¦\r\n");
    }

    delay_ms(100);

    if(ESP8266_SendCmd_OK("AT+CIPMODE=0", 50))
    {
        return 5;    //¹Ø±ÕÍ¸´«£¬³¬Ê±Ê±¼ä5s£¬Ê§°Ü·µ»Ø5
    }
    else
    {
        printf("ESP8266¹Ø±ÕÍ¸´«³É¹¦\r\n");
    }

    delay_ms(100);

    if(ESP8266_SendCmd_OK("AT+CIPMUX=1", 50))
    {
        return 6;    //¿ªÆô¶àÂ·Á¬½Ó£¬³¬Ê±Ê±¼ä5s£¬Ê§°Ü·µ»Ø6
    }
    else
    {
        printf("ESP8266¿ªÆô¶àÂ·Á¬½Ó³É¹¦\r\n");
    }

    delay_ms(100);

    if(ESP8266_SendCmd_OK("AT+CIPSERVER=1,5050", 50))
    {
        return 7;    //¿ªÆôTCP·þÎñÆ÷£¬¶Ë¿Ú5050£¬³¬Ê±Ê±¼ä5s£¬Ê§°Ü·µ»Ø7
    }

    return 0;                                                  //³É¹¦ÉèÖÃSTA_TCP·þÎñÆ÷·µ»Ø0
}

/**
 * @file   ESP8266_STA_TCPServer
 * @brief  ESP8266ÉèÖÃSTAÄ£Ê½ÏÂTCP¿Í»§¶Ë
 * @param  ÎÞ
 * @retval 1---10
 */
u8 ESP8266_STA_TCPCLient(void)
{
    u8 res = 0;

    if(ESP8266_SoftReset(50))
    {
        return 1;    //Èí¼þ¸´Î»8266£¬³¬Ê±Ê±¼ä5s£¬Ê§°Ü¾Í·µ»Ø1
    }
    else
    {
        printf("ESP8266Èí¸´Î»³É¹¦\r\n");
    }

    delay_ms(200);

    if(ESP8266_SendCmd_OK("AT+CWMODE=1", 50))
    {
        return 2;    //ÉèÖÃ8266ÎªSTAÄ£Ê½£¬³¬Ê±Ê±¼ä5s£¬Ê§°Ü·µ»Ø2
    }
    else
    {
        printf("ESP8266ÉèÖÃSTAÄ£Ê½³É¹¦\r\n");
    }

    delay_ms(100);
    printf("ESP8266Á¬½ÓAPÖÐ... ...\r\n");

    if(ESP8266_ConnectionAP(AP_SSID, AP_PASS, 200))
    {
        return 3;    //ÉèÖÃ8266Á¬½ÓAP£¬³¬Ê±Ê±¼ä20s£¬Ê§°Ü·µ»Ø3
    }
    else
    {
        printf("ESP8266Á¬½ÓAP³É¹¦\r\n");
    }

    delay_ms(100);

    if(ESP8266_GetIP(IP_Buff, 50))
    {
        return 4;    //²éÑ¯8266µÄIP£¬³¬Ê±Ê±¼ä5s£¬Ê§°Ü·µ»Ø4
    }
    else
    {
        printf("ESP8266²éÑ¯IPµØÖ·³É¹¦\r\n");
    }

    delay_ms(100);

    if(ESP8266_SendCmd_OK("AT+CIPMODE=1", 50))
    {
        return 5;    //¿ªÆôÍ¸´«£¬³¬Ê±Ê±¼ä5s£¬Ê§°Ü·µ»Ø5
    }
    else
    {
        printf("ESP8266¿ªÆôÍ¸´«³É¹¦\r\n");
    }

    delay_ms(100);

    if(ESP8266_SendCmd_OK("AT+CIPMUX=0", 50))
    {
        return 6;    //¹Ø±Õ¶àÂ·Á¬½Ó£¬³¬Ê±Ê±¼ä5s£¬Ê§°Ü·µ»Ø6
    }
    else
    {
        printf("ESP8266¹Ø±Õ¶àÂ·Á¬½Ó³É¹¦\r\n");
    }

    delay_ms(100);
    printf("ESP8266Á¬½Ó·þÎñÆ÷ÖÐ... ...\r\n");
    res = ESP8266_Connect_Server(SERVER_IP, SERVER_PORT, 50); //Á¬½ÓTCP·þÎñÆ÷,²¢½øÈëÍ¸´«£¬³¬Ê±Ê±¼ä5s£¬Ê§°Ü·µ»Ø7

    switch(res)
    {
    case 0 :
        break;

    case 1 :
        return 7;

    case 2 :
        return 8;

    case 3 :
        return 9;

    case 4 :
        return 10;
    }

    return res;
}
/**
 * @file   ESP8266_STA_TCPServer
 * @brief  ESP8266ÉèÖÃAPÄ£Ê½ÏÂTCP·þÎñÆ÷
 * @param  ÎÞ
 * @retval 0---7
 */
u8 ESP8266_AP_TCPServer(void)
{
    if(ESP8266_SoftReset(50))
    {
        return 1;    //Èí¼þ¸´Î»8266£¬³¬Ê±Ê±¼ä5s£¬Ê§°Ü¾Í·µ»Ø1
    }
    else
    {
        printf("ESP8266Èí¸´Î»³É¹¦\r\n");
    }

    delay_ms(200);

    if(ESP8266_SendCmd_OK("AT+CWMODE=2", 50))
    {
        return 2;    //ÉèÖÃ8266ÎªAPÄ£Ê½£¬³¬Ê±Ê±¼ä5s£¬Ê§°Ü·µ»Ø2
    }
    else
    {
        printf("ESP8266ÉèÖÃAPÄ£Ê½³É¹¦\r\n");
    }

    delay_ms(100);
    printf("ESP8266×¼±¸ÉèÖÃAP²ÎÊý\r\n");
    printf("SSID£ºESP8266\r\n");
    printf("ÃÜÂë£º012345678\r\n");
    printf("¼ÓÃÜ·½Ê½£ºWPA_WPA2_PSK\r\n");

    if(ESP8266_SET_AP(SSID, PASS, WPA_WPA2_PSK, 50))
    {
        return 3;    //ÉèÖÃ8266µÄAP£¬³¬Ê±Ê±¼ä5s£¬Ê§°Ü·µ»Ø3
    }
    else
    {
        printf("ESP8266ÉèÖÃAP²ÎÊý³É¹¦\r\n");
    }

    delay_ms(100);

    if(ESP8266_SendCmd_OK("AT+CIPMODE=0", 50))
    {
        return 4;    //¹Ø±ÕÍ¸´«£¬³¬Ê±Ê±¼ä5s£¬Ê§°Ü·µ»Ø4
    }
    else
    {
        printf("ESP8266¹Ø±ÕÍ¸´«³É¹¦\r\n");
    }

    delay_ms(100);

    if(ESP8266_SendCmd_OK("AT+CIPMUX=1", 50))
    {
        return 5;    //¿ªÆô¶àÂ·Á¬½Ó£¬³¬Ê±Ê±¼ä5s£¬Ê§°Ü·µ»Ø5
    }
    else
    {
        printf("ESP8266¿ªÆô¶àÂ·Á¬½Ó³É¹¦\r\n");
    }

    delay_ms(100);
    printf("ÉèÖÃAPÄ£Ê½IPµØÖ·£º192.168.10.1\r\n");

    if(ESP8266_SendCmd_OK("AT+CIPAP=\"192.168.10.1\"", 50))
    {
        return 6;    //ÉèÖÃAPµÄIPµØÖ·£¬³¬Ê±Ê±¼ä5s£¬Ê§°Ü·µ»Ø6
    }
    else
    {
        printf("ESP8266ÉèÖÃAPÄ£Ê½IP³É¹¦\r\n");
    }

    delay_ms(100);

    if(ESP8266_SendCmd_OK("AT+CIPSERVER=1,5050", 50))
    {
        return 7;    //¿ªÆôTCP·þÎñÆ÷£¬¶Ë¿Ú5050£¬³¬Ê±Ê±¼ä5s£¬Ê§°Ü·µ»Ø7
    }

    return 0;                                                  //³É¹¦ÉèÖÃSTA_TCP·þÎñÆ÷·µ»Ø0
}
/**
 * @file   ESP8266_SoftReset
 * @brief  ESP8266Èí¼þ¸´Î»
 * @param  timeout£º³¬Ê±Ê±¼ä
 * @retval ³¬Ê±´íÎó£¬·µ»Ø1;ÎÞ´í£¬·µ»Ø0
 */
u8 ESP8266_SoftReset(u16 timeout)
{
    u16  time;

    USART2_RX_STA = 0;
    memset(USART2_RX_BUF, 0, USART2_REC_LEN); //Çå³ý´®¿Ú2Êý¾Ý
    u2_printf("AT+RST\r\n");                //·¢ËÍÖ¸Áî
    time = 0;

    while(time < timeout)
    {
        if(strstr((const char *)USART2_RX_BUF, (const char *)"OK"))  //Èç¹û½ÓÊÜµ½ready±íÊ¾¸´Î»³É¹¦
        {
            break;
        }

        delay_ms(100);//ÑÓÊ±100ms
        time++;
    }

    if(time >= timeout)
    {
        return 1;    //³¬Ê±´íÎó£¬·µ»Ø1
    }
    else
    {
        return 0;    //ÎÞ´í£¬·µ»Ø0
    }
}
/**
 * @file   ESP8266_SendCmd_OK
 * @brief  ESP8266·¢ËÍÖ¸Áî£¬ÆÚ´ýµÄ»Ø´ðÊÇOK
* @param  cmd: Ö¸Áî timeout£º³¬Ê±Ê±¼ä
 * @retval ³¬Ê±´íÎó£¬·µ»Ø1;ÎÞ´í£¬·µ»Ø0
 */
u8 ESP8266_SendCmd_OK(u8 *cmd, u16 timeout)
{
    u16 time = 0;

    USART2_RX_STA = 0;
    memset(USART2_RX_BUF, 0, USART2_REC_LEN);  //Çå³ý´®¿Ú2Êý¾Ý
    u2_printf("%s\r\n", cmd);                  //·¢ËÍÖ¸Áî
    time = 0;

    while(time < timeout)
    {
        if(strstr((const char *)USART2_RX_BUF, (const char *)"\r\nOK\r\n"))  //Èç¹û½ÓÊÜµ½OK±íÊ¾Ö¸Áî³É¹¦
        {
            break;
        }

        delay_ms(100);//ÑÓÊ±100ms
        time++;
    }

    if(time >= timeout)
    {
        return 1;    //³¬Ê±´íÎó£¬·µ»Ø1
    }
    else
    {
        return 0;    //ÎÞ´í£¬·µ»Ø0
    }
}
//ÉèÖÃAP
//ssid£ºssid
//password£ºÃÜÂë
//mode:¼ÓÃÜ·½Ê½
//timeout ³¬Ê±Ê±¼ä
/**
 * @file   ESP8266_SendCmd_OK
 * @brief  ESP8266·¢ËÍÖ¸Áî£¬ÆÚ´ýµÄ»Ø´ðÊÇOK
* @param  cmd: Ö¸Áî timeout£º³¬Ê±Ê±¼ä
 * @retval ³¬Ê±´íÎó£¬·µ»Ø1;ÎÞ´í£¬·µ»Ø0
 */
u8 ESP8266_SET_AP(u8 *ssid, u8 *password, u8 mode, u16 timeout)
{
    u16 time = 0;

    USART2_RX_STA = 0;
    memset(USART2_RX_BUF, 0, USART2_REC_LEN);                         //Çå³ý´®¿Ú2Êý¾Ý
    u2_printf("AT+CWSAP=\"%s\",\"%s\",4,%d\r\n", ssid, password, mode); //·¢ËÍÉèÖÃAPÖ¸Áî
    time = 0;

    while(time < timeout)
    {
        if(strstr((const char *)USART2_RX_BUF, (const char *)"OK"))  //Èç¹û½ÓÊÜµ½OK±íÊ¾Ö¸Áî³É¹¦
        {
            break;
        }

        delay_ms(100);//ÑÓÊ±100ms
        time++;
    }

    if(time >= timeout)
    {
        return 1;    //³¬Ê±´íÎó£¬·µ»Ø1
    }
    else
    {
        return 0;    //ÎÞ´í£¬·µ»Ø0
    }
}
/**
 * @file   ESP8266_ConnectionAP
 * @brief  Á¬½Óµ½AP
* @param  ssid£ºÎÞÏßÂ·ÓÉµÄssid ; password£ºÃÜÂë ;timeout ³¬Ê±Ê±¼ä
 * @retval ³¬Ê±´íÎó£¬·µ»Ø1;ÎÞ´í£¬·µ»Ø0
 */
u8 ESP8266_ConnectionAP(u8 *ssid, u8 *password, u16 timeout)
{
    u16 time = 0;

    USART2_RX_STA = 0;
    memset(USART2_RX_BUF, 0, USART2_REC_LEN);             //Çå³ý´®¿Ú2Êý¾Ý
    u2_printf("AT+CWJAP_DEF=\"%s\",\"%s\"\r\n", ssid, password); //·¢ËÍÁ¬½ÓAPÖ¸Áî
    time = 0;

    while(time < timeout)
    {
        if(strstr((const char *)USART2_RX_BUF, (const char *)"WIFI CONNECTED\r\nWIFI GOT IP\r\n\r\nOK\r\n"))  //Èç¹û½ÓÊÜµ½OK±íÊ¾Ö¸Áî³É¹¦
        {
            break;
        }

        delay_ms(100);//ÑÓÊ±100ms
        time++;
    }

    if(time >= timeout)
    {
        return 1;    //³¬Ê±´íÎó£¬·µ»Ø1
    }
    else
    {
        return 0;    //ÎÞ´í£¬·µ»Ø0
    }
}
/**
 * @file   ESP8266_GetIP
 * @brief  »ñÈ¡µ±Ç°Ä£¿éµÄIPµØÖ·
* @param  ip£º±£´æIPµÄÊý×é;timeout ³¬Ê±Ê±¼ä
 * @retval ³¬Ê±´íÎó£¬·µ»Ø1;ÎÞ´í£¬·µ»Ø0
 */
u8 ESP8266_GetIP(u8 *ip, u16 timeout)
{
    u16  time = 0;
    char *presult1, *presult2;

    USART2_RX_STA = 0;
    memset(USART2_RX_BUF, 0, USART2_REC_LEN);             //Çå³ý´®¿Ú2Êý¾Ý
    u2_printf("AT+CIFSR\r\n");                            //·¢ËÍ²éÑ¯IPÖ¸Áî
    time = 0;

    while(time < timeout)
    {
        if(strstr((const char *)USART2_RX_BUF, (const char *)"\r\nOK\r\n"))  //Èç¹û½ÓÊÜµ½OK±íÊ¾Ö¸Áî³É¹¦
        {
            break;
        }

        delay_ms(100);//ÑÓÊ±100ms
        time++;
    }

    if(time >= timeout)
    {
        return 1;    //³¬Ê±´íÎó£¬·µ»Ø1
    }
    else
    {
        presult1 = strstr((const char *)USART2_RX_BUF, (const char *)"\"");

        if(presult1 != NULL)
        {
            presult2 = strstr((const char *)presult1 + 1, (const char *)"\"");

            if(presult2 != NULL)
            {
                memcpy(ip, presult1 + 1, presult2 - presult1 - 1);
                //printf("IPµØÖ·£º%s\r\n", ip);
                return 0;
            }
            else
            {
                return 3;    //Î´ÊÕµ½Ô¤ÆÚÊý¾Ý
            }
        }
        else
        {
            return 2;    //Î´ÊÕµ½Ô¤ÆÚÊý¾Ý
        }
    }
}
/**
 * @file   ESP8266_Get_LinkSta
 * @brief  ESP8266»ñÈ¡Á¬½Ó×´Ì¬
* @param ÎÞ
* @retval 1£ºÓÐ¿Í»§½ÓÈë;2£ºÓÐ¿Í»§¶Ï¿ª;0:ÎÞ×´Ì¬¸Ä±ä
 */
u8 ESP8266_Get_LinkSta(void)
{
    u8 id_temp[10] = {0}; //»º³åÇø£¬´æ·ÅID
    u8 sta_temp[10] = {0}; //»º³åÇø£¬´æ·Å×´Ì¬

    if(strstr((const char *)USART2_RX_BUF, (const char *)"CONNECT"))  //Èç¹û½ÓÊÜµ½CONNECT±íÊ¾ÓÐÁ´½Ó³É¹¦
    {
        sscanf((const char *)USART2_RX_BUF, "%[^,],%[^,]", id_temp, sta_temp);
        printf("ÓÐ¿Í»§¶Ë½ÓÈë£¬ID=%s\r\n", id_temp);
        USART2_RX_STA = 0;
        memset(USART2_RX_BUF, 0, USART2_REC_LEN);             //Çå³ý´®¿Ú2Êý¾Ý
        return 1;  //ÓÐ¿Í»§¶Ë½ÓÈë
    }

    if(strstr((const char *)USART2_RX_BUF, (const char *)"CLOSED"))  //Èç¹û½ÓÊÜµ½CLOSED±íÊ¾ÓÐÁ´½Ó¶Ï¿ª
    {
        sscanf((const char *)USART2_RX_BUF, "%[^,],%[^,]", id_temp, sta_temp);
        printf("ÓÐ¿Í»§¶Ë¶Ï¿ª£¬ID=%s\r\n", id_temp);
        USART2_RX_STA = 0;
        memset(USART2_RX_BUF, 0, USART2_REC_LEN);             //Çå³ý´®¿Ú2Êý¾Ý
        return 2;  //ÓÐ¿Í»§¶Ë¶Ï¿ª
    }
    else
    {
        return 0;    //ÎÞ×´Ì¬¸Ä±ä
    }
}
/**
 * @file   ESP8266_Get_Data
 * @brief  ESP8266»ñÈ¡¿Í»§¶ËÊý¾Ý
* @param  data£º¿Í»§¶ËÊý¾Ý´æ´¢µØÖ·;len:Êý¾Ý³¤¶È;id:Á¬½ÓµÄIDºÅ
* @retval 1£ºÓÐÊý¾Ýµ½À´;0:ÎÞÊý¾Ýµ½À´
 */
u8 ESP8266_Get_Data(u8 *data, u8 *len, u8 *id)
{
    u8 temp[10] = {0};    //»º³åÇø£¬
    char *presult;

    if(strstr((const char *)USART2_RX_BUF, (const char *)"+IPD"))  //Èç¹û½ÓÊÜµ½+IPD±íÊ¾ÓÐÊý¾Ý
    {
        delay_ms(5);
        sscanf((const char *)USART2_RX_BUF, "%[^,],%[^,],%[^:]", temp, id, len); //½ØÈ¡¸÷¶ÎÊý¾Ý
        presult = strstr((const char *)USART2_RX_BUF, (const char *)":");

        if(presult != NULL)
        {
            sprintf((char *)data, "%s", (presult + 1));
        }

        USART2_RX_STA = 0;
        memset(USART2_RX_BUF, 0, USART2_REC_LEN);             //Çå³ý´®¿Ú2Êý¾Ý
        return 1;  //ÓÐÊý¾Ýµ½À´
    }
    else
    {
        return 0;    //ÎÞÊý¾Ýµ½À´
    }
}

/**
 * @file   ESP8266_Tcp_GetData
 * @brief  ESP8266»ñÈ¡·þÎñ¶ËÊý¾Ý
* @param  data£º¿Í»§¶ËÊý¾Ý´æ´¢µØÖ·;len:Êý¾Ý³¤¶È;
* @retval 1£ºÓÐÊý¾Ýµ½À´;0:ÎÞÊý¾Ýµ½À´
 */
u8 ESP8266_Tcp_GetData(u8 *data, int *len)
{
    u8 temp[10] = {0};    //»º³åÇø£¬
    char *presult;
	
	delay_ms(5);
	sscanf((const char *)USART2_RX_BUF, "%[^,],%d", temp, len); //½ØÈ¡¸÷¶ÎÊý¾Ý
	presult = strstr((const char *)USART2_RX_BUF, (const char *)":");

	if(presult != NULL)
	{
		sprintf((char *)data, "%s", (presult + 1));
	}

	USART2_RX_STA = 0;
	//memset(USART2_RX_BUF, 0, USART2_REC_LEN);             //Çå³ý´®¿Ú2Êý¾Ý
	return 1;  //ÓÐÊý¾Ýµ½À´

//    if(strstr((const char *)USART2_RX_BUF, (const char *)"+IPD"))  //Èç¹û½ÓÊÜµ½+IPD±íÊ¾ÓÐÊý¾Ý
//    {
//        delay_ms(5);
//        sscanf((const char *)USART2_RX_BUF, "%[^,],%d", temp, len); //½ØÈ¡¸÷¶ÎÊý¾Ý
//        presult = strstr((const char *)USART2_RX_BUF, (const char *)":");

//        if(presult != NULL)
//        {
//            sprintf((char *)data, "%s", (presult + 1));
//        }

//        USART2_RX_STA = 0;
//        //memset(USART2_RX_BUF, 0, USART2_REC_LEN);             //Çå³ý´®¿Ú2Êý¾Ý
//        return 1;  //ÓÐÊý¾Ýµ½À´
//    }
//    else
//    {
//        return 0;    //ÎÞÊý¾Ýµ½À´
//    }
}
/**
 * @file   ESP8266_SendData
 * @brief  TCP·þÎñÆ÷·¢ËÍÊý¾Ý
* @param  id£ºÁ¬½ÓµÄIDºÅ£»databuff£ºÊý¾Ý»º³åÇø<2048£data_len£ºÊý¾Ý³¤¶È»
* @retval 1£ºÓÐÊý¾Ýµ½À´;0:ÎÞÊý¾Ýµ½À´
 */
u8 ESP8266_SendData(u8 id, u8 *databuff, u16 data_len, u16 timeout)
{
    u16 time = 0;
    u16 i;

    USART2_RX_STA = 0;
    memset(USART2_RX_BUF, 0, USART2_REC_LEN);             //Çå³ý´®¿Ú2Êý¾Ý
    u2_printf("AT+CIPSEND=%d,%d\r\n", id, data_len);              //·¢ËÍ·¢ËÍÊý¾ÝÖ¸Áî
    time = 0;

    while(time < timeout)
    {
        if(strstr((const char *)USART2_RX_BUF, (const char *)">"))  //Èç¹û½ÓÊÜµ½>±íÊ¾Ö¸Áî³É¹¦,×¼±¸·¢ËÍÊý¾Ý
        {
            break;
        }

        delay_ms(100);//ÑÓÊ±100ms
        time++;
    }

    if(time >= timeout)
    {
        return 1;    //³¬Ê±´íÎó£¬·µ»Ø1
    }
    else
    {
        USART2_RX_STA = 0;
        memset(USART2_RX_BUF, 0, USART2_REC_LEN);                //Çå³ý´®¿Ú2Êý¾Ý

        for(i = 0; i < data_len; i++)
        {
            u2_printf("%c", databuff[i]);    //·¢ËÍÊý¾Ý
        }

        time = 0;

        while(time < timeout)
        {
            if(strstr((const char *)USART2_RX_BUF, (const char *)"SEND OK"))  //Èç¹û½ÓÊÜSEND OK£¬±íÊ¾·¢ËÍ³É¹¦
            {
                USART2_RX_STA = 0;
                memset(USART2_RX_BUF, 0, USART2_REC_LEN);                //Çå³ý´®¿Ú2Êý¾Ý
                break;
            }

            if(strstr((const char *)USART2_RX_BUF, (const char *)"link is not valid"))  //Èç¹û½ÓÊÜlink is not valid£¬±íÊ¾Á¬½Ó¶Ï¿ª
            {
                USART2_RX_STA = 0;
                memset(USART2_RX_BUF, 0, USART2_REC_LEN);                //Çå³ý´®¿Ú2Êý¾Ý
                return 2;
            }

            delay_ms(100);  //ÑÓÊ±100ms
            time++;
        }

        if(time >= timeout)
        {
            return 3;    //³¬Ê±´íÎó£¬·µ»Ø3
        }
        else
        {
            return 0;
        }
    }
}

/**
 * @file   ESP8266_Tcp_SendData
 * @brief  TCP·þÎñÆ÷·¢ËÍÊý¾Ý
 * @param  databuff£ºÊý¾Ý»º³åÇø<2048£data_len£ºÊý¾Ý³¤¶È?
 * @retval 1£ºÓÐÊý¾Ýµ½À´;0:ÎÞÊý¾Ýµ½À´
 */
u8 ESP8266_Tcp_SendData(u8 *databuff, u16 data_len, u16 timeout)
{
    u16 time = 0;
    u16 i;

    USART2_RX_STA = 0;
    memset(USART2_RX_BUF, 0, USART2_REC_LEN);             //Çå³ý´®¿Ú2Êý¾Ý
	if(ESP8266_SendCmd_OK((u8*)"AT+CIPMODE=1\r\n", 50))
    {
        printf("ESP8266ÉèÖÃÍ¸´«Ä£Ê½Ê§°Ü\r\n");
    }
    printf("ESP8266ÉèÖÃÍ¸´«Ä£Ê½³É¹¦\r\n");
	
	if(ESP8266_SendCmd_OK((u8*)"AT+CIPSEND\r\n", 50))
    {
        printf("ESP8266ÉèÖÃÍ¸´«Ö¸ÁîÊ§°Ü\r\n");
    }
    printf("ESP8266ÉèÖÃÍ¸´«Ö¸Áî³É¹¦\r\n");
    time = 0;

    while(time < timeout)
    {
        if(strstr((const char *)USART2_RX_BUF, (const char *)">"))  //Èç¹û½ÓÊÜµ½>±íÊ¾Ö¸Áî³É¹¦,×¼±¸·¢ËÍÊý¾Ý
        {
            break;
        }

        delay_ms(100);//ÑÓÊ±100ms
        time++;
    }

    if(time >= timeout)
    {
        return 1;    //³¬Ê±´íÎó£¬·µ»Ø1
    }
    else
    {
        USART2_RX_STA = 0;
        memset(USART2_RX_BUF, 0, USART2_REC_LEN);                //Çå³ý´®¿Ú2Êý¾Ý

        for(i = 0; i < data_len; i++)
        {
            u2_printf("%c", databuff[i]);    //·¢ËÍÊý¾Ý
        }

        time = 0;

        while(time < timeout)
        {
            if(strstr((const char *)USART2_RX_BUF, (const char *)"SEND OK"))  //Èç¹û½ÓÊÜSEND OK£¬±íÊ¾·¢ËÍ³É¹¦
            {
                USART2_RX_STA = 0;
                memset(USART2_RX_BUF, 0, USART2_REC_LEN);                //Çå³ý´®¿Ú2Êý¾Ý
                break;
            }

            if(strstr((const char *)USART2_RX_BUF, (const char *)"link is not valid"))  //Èç¹û½ÓÊÜlink is not valid£¬±íÊ¾Á¬½Ó¶Ï¿ª
            {
                USART2_RX_STA = 0;
                memset(USART2_RX_BUF, 0, USART2_REC_LEN);                //Çå³ý´®¿Ú2Êý¾Ý
                return 2;
            }

            delay_ms(100);  //ÑÓÊ±100ms
            time++;
        }

        if(time >= timeout)
        {
            return 3;    //³¬Ê±´íÎó£¬·µ»Ø3
        }
        else
        {
            return 0;
        }
    }
}

u8 ESP8266_Ping(char *IP, u8 time)
{
    char cmd[30];
    sprintf(cmd, "AT+PING=\"%s\"\r\n", IP);
    return ESP8266_SendCmd_OK((u8*)cmd, time) ? 0 : 1;
}

/**
 * @file  ESP8266_Connect_Server
 * @brief Á¬½ÓTCP·þÎñÆ÷£¬²¢½øÈëÍ¸´«Ä£Ê½
 * @param
 *					ip£º·þÎñÆ÷ip
 *					port£º·þÎñÆ÷¶Ë¿ÚºÅ
 * @retval 1£º·þÎñÆ÷Î´¿ªÆô
 *					2£ºÒÑÓë·þÎñÆ÷½¨Á¢Á´½Ó
 *					3£º³¬Ê±´íÎó
 *					4£ºÍ¸´«³¬Ê±
 *					0£ºÍ¸´«³É¹¦
 */
u8 ESP8266_Connect_Server(u8 *ip, u16 port, u16 timeout)
{
    u16 time = 0;

    USART2_RX_STA = 0;
    memset(USART2_RX_BUF, 0, USART2_REC_LEN);              //Çå³ý´®¿Ú2Êý¾Ý
    u2_printf("AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", ip, port); //·¢ËÍÁ¬½Ó·þÎñÆ÷Ö¸Áî
    time = 0;

    while(time < timeout)
    {
        if(strstr((const char *)USART2_RX_BUF, (const char *)"CONNECT"))  //Èç¹û½ÓÊÜµ½CONNECT±íÊ¾Á¬½Ó³É¹¦
        {
            break;
        }

        if(strstr((const char *)USART2_RX_BUF, (const char *)"CLOSED"))  //Èç¹û½ÓÊÜµ½CLOSED±íÊ¾·þÎñÆ÷Î´¿ªÆô
        {
            return 1;
        }

        if(strstr((const char *)USART2_RX_BUF, (const char *)"ALREADY CONNECTED"))  //Èç¹û½ÓÊÜµ½ALREADY CONNECTEDÒÑ¾­½¨Á¢Á¬½Ó
        {
            return 2;
        }

        delay_ms(100);//ÑÓÊ±100ms
        time++;
    }

    if(time >= timeout)
    {
        return 3;    //³¬Ê±´íÎó£¬·µ»Ø3
    }
    else  //×¼±¸½øÈëÍ¸´«
    {
        printf("ESP8266Á¬½Ó·þÎñÆ÷³É¹¦£¬×¼±¸½øÈëÍ¸´«\r\n");
        USART2_RX_STA = 0;
        memset(USART2_RX_BUF, 0, USART2_REC_LEN);             //Çå³ý´®¿Ú2Êý¾Ý
		u2_printf("AT+CIPMODE=1\r\n");
		delay_ms(1000);
        u2_printf("AT+CIPSEND\r\n");                          //·¢ËÍ½øÈëÍ¸´«Ö¸Áî
        time = 0;

        while(time < timeout)
        {
            if(strstr((const char *)USART2_RX_BUF, (const char *)"\r\nOK\r\n\r\n>"))  //Èç¹û³ÉÁ¢±íÊ¾½øÈëÍ¸´«³É¹¦
            {
                break;
            }

            delay_ms(100);//ÑÓÊ±100ms
            time++;
        }

        if(time >= timeout)
        {
            return 4;    //Í¸´«³¬Ê±´íÎó£¬·µ»Ø4
        }
        else
        {
            return 0;    //³É¹¦·µ»Ø0
        }
    }
}

/**
 * @file  ESP8266_Disconnect_Server
 * @brief ¶Ï¿ªTCP·þÎñÆ÷
 */
u8 ESP8266_Disconnect_Server(u16 timeout)
{
    //u8 i = 0, tmp = 0;
    u2_printf("AT+CIPCLOSE\r\n");
    delay_ms(timeout * 1000);

//    for(i = 0; i < timeout; i++)
//    {
//        tmp = strstr((const char *)cmpbuf, "OK") ? 1 : 0;
//    }

    return 1;
}
//ÏÈ¼ì²â£¬Èç¹ûÍ¸´«Ä£Ê½ÔòÍË³öÍ¸´«
/**
 * @file  ESP8266_Connect_Server
 * @brief Á¬½ÓTCP·þÎñÆ÷£¬²¢½øÈëÍ¸´«Ä£Ê½
* @param timeout£º³¬Ê±Ê±¼ä
* @retval ÎÞ
 */
void ESP8266_Exit_Transfer(u16 timeout)
{
    u16 time = 0;

    USART2_RX_STA = 0;
    memset(USART2_RX_BUF, 0, USART2_REC_LEN);              //Çå³ý´®¿Ú2Êý¾Ý
    u2_printf("AT\r\n");//·¢ËÍAT²âÊÔÖ¸Áî
    time = 0;

    while(time < timeout)
    {
        if(strstr((const char *)USART2_RX_BUF, (const char *)"\r\nOK\r\n"))  //Èç¹û³ÉÁ¢±íÊ¾½øÈëÍ¸´«³É¹¦
        {
            break;
        }

        delay_ms(100);//ÑÓÊ±100ms
        time++;
    }

    if(time >= timeout)
    {
        printf("ESP8266´¦ÓÚÍ¸´«×´Ì¬£¬×¼±¸ÍË³öÍ¸´«\r\n");
        u2_printf("+++");//·¢ËÍÍË³öÍ¸´«Ö¸Áî°ü
        delay_ms(1000);
    }
}

/*
    DHCPÉèÖÃ
*/
u8 ESP8266_DHCP_Setup(void)
{
    return ESP8266_SendCmd_OK((u8 *)"AT+CWDHCP=1,1", 300);  //ÔÝÊ±Ö»×öSTAÄ£Ê½¿ªÆôDHCP
}

/*
    Á¬½ÓÄ£Ê½
	0 : µ¥Á¬½Ó
	1 : ¶àÁ¬½Ó
*/
u8 ESP8266_Connect_Mode(u8 mode)
{
    if(mode == 0)
    {
        return ESP8266_SendCmd_OK((u8 *)"AT+CIPMUX=0", 300);  //µ¥Á´½ÓÄ£Ê½
    }
    else
    {
        return ESP8266_SendCmd_OK((u8 *)"AT+CIPMUX=1", 300);  //¶àÁ´½ÓÄ£Ê½
    }
}

/*
    ÉèÖÃ´«ÊäÄ£Ê½£ºÍ¸´«ºÍ·ÇÍ¸´«
*/
u8 ESP8266_TransMode(u8 mode)
{
    bool tmp;

    switch(mode)
    {
    case 0://·ÇÍ¸´«Ä£Ê½
        tmp = ESP8266_SendCmd_OK((u8 *)"AT+CIPMODE=0", 300);
        break;

    case 1://Í¸´«Ä£Ê½
        if(ESP8266_SendCmd_OK((u8 *)"AT+CIPMODE=1", 300) == true)
        {
            tmp = ESP8266_SendCmd_OK((u8 *)"AT+CIPSEND", 1000);
        }
        else
        {
            tmp = false;
        }

        break;
    }

    return tmp;
}

/**
 * @file   Char_to_Hex
 * @brief  ½«×Ö·ûÐÍÊý×Ö×ª»»³ÉÊý×Ö£¬´¦ÀíËÄÎ»ÊýÒÔÏÂ0-9999
* @param  char_data£º×Ö·ûÐÍÊý×Ö£»lenÒª×ª»»µÄ³¤¶È
* @retval temp£º·µ»¹µÄÊý×Ö
 */
u16 Char_to_Hex(u8 *char_data, u16 len)
{
    u16 temp;

    switch(len)
    {
    case 1 :
        temp = (char_data[0] - 0x30) * 1;
        break;

    case 2 :
        temp = (char_data[0] - 0x30) * 10;
        temp += (char_data[1] - 0x30) * 1;
        break;

    case 3 :
        temp = (char_data[0] - 0x30) * 100;
        temp += (char_data[2] - 0x30) * 10;
        temp += (char_data[3] - 0x30) * 1;
        break;

    case 4 :
        temp = (char_data[0] - 0x30) * 1000;
        temp += (char_data[1] - 0x30) * 100;
        temp += (char_data[2] - 0x30) * 10;
        temp += (char_data[3] - 0x30) * 1;
        break;
    }

    return temp;
}

