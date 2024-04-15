#项目名称：数字识别
#项目作者：ZDS
#项目时间：2024年3月


import sensor, image, lcd, time 
import KPU as kpu
import gc, sys
from machine import UART
from fpioa_manager import fm
lcd.init(freq=15000000)
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_windowing((224, 224))
sensor.set_hmirror(False)
sensor.set_vflip(True)
sensor.run(1)
fm.register(25, fm.fpioa.UART1_TX, force=True)
fm.register(24, fm.fpioa.UART1_RX, force=True)
uart1 = UART(UART.UART1,115200,8,1,0,timeout=1000, read_buf_len=4096)
def main(anchors = None,labels = None, model_addr="/sd/demo.kmodel"):
	task = kpu.load(model_addr)
	kpu.init_yolo2(task, 0.5, 0.3, 5, anchors)
	try:
		while True:
			img = sensor.snapshot()
			objects = None
			objects = kpu.run_yolo2(task, img)
			if objects:
				for obj in objects:
					pos = obj.rect()
					img.draw_rectangle(pos)
					img.draw_string(pos[0], pos[1], "%s : %.2f" %(labels[obj.classid()], obj.value()), scale=2, color=(255,255,255))
					buf = bytearray([0X3A,int(labels[obj.classid()]),0X5C])
					uart1.write(buf)
					print(buf)
			lcd.display(img)
	except Exception as e:
		raise e
	finally:
		kpu.deinit(task)
if __name__ == "__main__":
	try:
		labels = ["9","1","4","2","3","8","5","6","7"]
		anchors = [0.4192, 0.3702, 0.5744, 1.6689, 0.6932, 0.6054, 1.0054, 0.9615, 2.1672, 1.6683]
		main(anchors = anchors, labels=labels, model_addr="/sd/demo.kmodel")
	except Exception as e:
		sys.print_exception(e)
		pass
	finally:
		gc.collect()
