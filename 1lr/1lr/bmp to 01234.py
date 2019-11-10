input_file = 'os'
output_file = 'a'
class picture ():
  def __init__(self,name=''):
    self.name=name
    self.pict=open(self.name,'rb')
    self.read=self.pict.read()
    self.m=self.read[22]
    self.n=self.read[18]
    self.bpp=int(self.read[28]/8)
    self.atr=self.read[:self.read[10]]
    self.colors=self.read[self.read[10]:]
    self.pict.close()
  def optimize(self):
    colors = dict()
    k=0
    matrix = []
    for i in range(self.m):
      matrix.append([])
      for j in range(self.n):
        if str(self.matrix[i][j]) not in colors:
          colors[str(self.matrix[i][j])]=k
          k+=1
        matrix[i].append(colors[str(self.matrix[i][j])])
    print (k)
    self.opt_matrix=matrix
          
  def real_matrix(self):
    matrix=[]
    for i in range(1,self.m+1):
      matrix.append(self.matrix[-i])
    self.matrix = matrix
  def str_in_matrix(self):
    self.matrix=[]
    for i in range (self.m):
      self.matrix.append([])
      for j in range (self.n):
        self.matrix[i].append([])
        for z in range (self.bpp):
          self.matrix[i][-1].append(self.colors[i*(self.n)*self.bpp+j*self.bpp+z])
if __name__ == "__main__":
  pict = picture(input_file+'.bmp')
  pict.str_in_matrix()
  pict.real_matrix()
  pict.optimize()
  matrix = pict.opt_matrix
  file = open(output_file+'.bin','wb')

  a=len(matrix)*len(matrix[0])
  print(len(matrix),len(matrix[0]),a)
  for row in matrix:
    for key in row:
      file.write(chr(key).encode())
  for i in range(512-a%512):
    file.write(chr(0).encode())
  file.close()
