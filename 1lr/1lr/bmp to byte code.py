# массив используемых картинок
# !ВАЖНО! ширена картинки должна делиться на 4
input_file = ['0','1','2','3','4','5','6','7','ham1','ham2','ham3','ham4','ham5','hentai','knight','bam','buble','sensRKN','unsensRKN','k1','k2','k3','k4','k5','k6','k7','k8','k9','k10','os']
# название файла итогового преобразования
output_file = 'img'
file = open(output_file+'.bin','wb')
pup=62# смещение секторов где они начианются для удобной установки первого сектора
#картинки, смотреть на на адрес предыдущей картинки  чтобы знать первый сектор следующей
for img in input_file:
        read= open(img+".bmp",'rb')
        read=read.read()
        h=read[23]*256+read[22]#высота
        w=read[19]*256+read[18]
        matrix=[]
        blyat=0
        print(img)
        #print(len(read),h,w)
        for i in range (h):
                matrix.append([])
                for j in range (w):                                
                        matrix[i].append([])
                        for z in range (2,-1,-1):
                                matrix[i][-1].append(read[54+(i*w+j)*3+z])
                
                blyat+=1
        rmatrix=[]
        for i in range(1,h+1):
                rmatrix.append(matrix[-i])
        a,b=(h*w*3+6)%512,(h*w*3+6)//512+1
        c,d=b//256,b%256
        pup=pup+b
        print(pup,b)
        file.write(bytes([d]))
        file.write(bytes([c]))
        file.write(bytes([read[22]]))
        file.write(bytes([read[23]]))
        file.write(bytes([read[18]]))
        file.write(bytes([read[19]]))
        for i in rmatrix:
                for j in i:
                        for k in j:
                                file.write(bytes([k]))
        for i in range(512-a): 
                file.write(chr(0).encode())
file.close()
print("ok")
                        
    
