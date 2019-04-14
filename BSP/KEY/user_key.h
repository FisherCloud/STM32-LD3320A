#ifndef _USER_KEY_H_
#define _USER_KEY_H_

#define key_user2_GETVALUE()  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)
void hw_key_init(void);

#endif
