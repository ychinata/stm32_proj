#项目名称：巡线寻找最大色块
#项目作者：ZDS
#项目时间：2024年3月

import sensor,image,lcd,time
from fpioa_manager import fm


fm.register(9, fm.fpioa.UART1_TX, force=True)
fm.register(10, fm.fpioa.UART1_RX, force=True) #映射串口IO口

from machine import UART
import time
uart_A = UART(UART.UART1, 115200, 8, 1, 0, timeout=1000, read_buf_len=4096)#初始化串口

#常用初始化
lcd.init()
sensor.reset()                      #复位摄像头
sensor.set_pixformat(sensor.RGB565) #设置像素格式 RGB565
sensor.set_framesize(sensor.QVGA)   #设置帧尺寸 QVGA (320x240)
sensor.skip_frames(time = 2000)     #跳过不稳定画面

#红色阈值
red_threshold = (0, 100, 15, 127, 15, 127)


#寻找最大色块函数定义
def find_max(blobs):
    max_size=0
    for blob in blobs:
        if blob[4] > max_size:
            max_blob=blob
            max_size = blob[4]
    return max_blob

while True:
    img=sensor.snapshot()
    blobs = img.find_blobs([red_threshold],merge=True)

    if blobs:
        max_blob = find_max(blobs)#调用函数，返回最大色块

    # 圈出最大色块，标记中心位置，写明中心坐标
    img.draw_rectangle(max_blob[0:4], color=(0,0,0)) #用黑色框出最大色块
    img.draw_cross(max_blob[5], max_blob[6])
    img.draw_string(max_blob.x(),max_blob.y(), (str(max_blob.cx())+','+str(max_blob.cy())), color = (0,0,0))
    print("x = %d y = %d", max_blob[5], max_blob[6])
    array = bytearray([0xFF, max_blob[5], 0xFE])
    uart_A.write(array)
    lcd.display(img)
