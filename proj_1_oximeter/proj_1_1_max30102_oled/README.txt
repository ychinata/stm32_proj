
USART:usart.h
使用USART1
波特率115200
TX GPIOA_PIN9(A9)
RX GPIOA_PIN10(A10)

IIC管脚配置:iic.h
(OLED/MAX3010X血氧传感器共用)
IIC_SCL:B6
ICC_SDA:B7

DS18B20：
B12

MAX3010X:max30102.h
INT:B9

TFT屏幕显示？用HAL库？

TFT-LCD模块关联文件：
lcd.c, spi.c, gui.c


//2022.9.30
为何TFT屏与血氧模块功能会冲突？
走读作者tft屏项目代码
