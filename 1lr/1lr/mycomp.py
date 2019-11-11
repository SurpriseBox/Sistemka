import os
file = open("mkconfig.txt", 'r')
addres= file.read().split("\n")
file.close()
print("Имя Файла загрузчика")
boot=addres[0]+str(input())
print("Имя Файла кода")
main=addres[0]+str(input())
print("Имя Файла данных")
data=[]
for i in str(input()).split():
	data.append(addres[0]+i+".bin")
print("Имя диска")
disk=str(input())
#file = open("mk.bat", 'w')
os.system("cd "+addres[0])
os.system("nasm -f bin "+boot+".asm -o "+boot+".bin")
os.system("gcc -masm=intel -nostdlib "+main+".c -o "+main+".bin")
os.system("objcopy "+main+".bin -O binary")
#file.close()
#system("mk")
file0=[]
for i in range(len(data)):
	file0.append(open(data[i],'rb'))
file1 = open(boot+".bin", "rb")
file2 = open(main+".bin", "rb")
file3 = open(addres[1]+disk+".img", "wb")
print("Сколько секторов на код?")
sector=int(input())
bb = file1.read()
file1.close()
mm=file2.read()
file2.close()
dd=b''
for i in range(len(file0)):
	dd =dd+ file0[i].read()
	file0[i].close()
b = bytearray(bb)
m = bytearray(mm)
while (len(b)<510):
    b.append(0x0)
b.append(0x55)
b.append(0xaa)
if len(m)>sector*512:
    m = m[:sector*512]
while(len(m)<sector*512):
    m.append(0x0)
bb = bytes(b)
mm = bytes(m)
file3.write(bb)
file3.write(mm)
file3.write(dd)
file3.close()
file = open("qvadro.bat", 'w')
file.write("cd "+addres[1]+"\n")
file.write("qemu1 "+disk+".img")
file.close()
os.startfile("qvadro.bat", "runas")
