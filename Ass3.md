## Assignment3
### 1. normal shader
其实就是将法向量的x, y, z作为了RGB颜色。所以法向量方向相近，则颜色相近，反之，则颜色相反：
![normal](https://user-images.githubusercontent.com/17798738/112757459-669d4900-901c-11eb-8ede-77cbfe8181ab.png)

### 2. blinn-phong shading
blinn-phong的kd是一个常数值（`newtri.setColor(0, 148,121.0,92.0)`)，所以牛是一个纯咖啡色的牛：

p = 30:

![image](https://user-images.githubusercontent.com/17798738/112757527-a3694000-901c-11eb-8083-e7c7a734d384.png)

p = 150:

![phong_spec_pow150](https://user-images.githubusercontent.com/17798738/112757650-4621be80-901d-11eb-8c09-2f5ee44f155a.png)

blinn-phong实际上是简单的将环境光+漫反射+镜面反射进行相加：
![image](https://user-images.githubusercontent.com/17798738/112757298-c34c3400-901b-11eb-8745-249ebe979d49.png)
 
### 3. texture shading
仍然是blinn-phong shading，与2不同的是，kd是从一张材质图中读取的，并不是常数值：
![image](https://user-images.githubusercontent.com/17798738/112757311-d9f28b00-901b-11eb-9a7b-dca0ad8d5bcf.png)

### 4. bump shading
简单地说，就是对每个点的法向量加噪声，噪声并不是随机的，而是根据一个凹凸图得到的。
![image](https://user-images.githubusercontent.com/17798738/112757318-e24ac600-901b-11eb-8033-64280062ee9f.png)

完成时间 2021-03-27

### 5. displacement shading
有bug，待补充。。。

### 6. 提高题：双线性插值
待完成
