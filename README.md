开始学习时间：2021-03-21
[TOC]
# 作业结果
## Assignment1
![ass1](https://user-images.githubusercontent.com/17798738/112030975-c9eb2f00-8b75-11eb-9578-832e5037da03.gif)

完成时间：2021-03-23

待完成提高题

## Assignment2
![image](https://user-images.githubusercontent.com/17798738/112307344-ce315c80-8cdb-11eb-84fe-c9bded4d44eb.png)

关于投影矩阵，书中的矩阵在作业中使用会使z值为负数，但绝对值一样，具体原因不明。待补充

完成时间2021-03-24

### MSAA
结果：
![image](https://user-images.githubusercontent.com/17798738/112426330-c1137c80-8d72-11eb-9e72-372e9aec67ef.png)

**大概思路**：根据msaa参数，生成采样坐标，比如4xmsaa，那就把每个像素划分为16宫格，每宫格的正中心为采样点。对于每一个采样点，要记录它的深度信息，并判断是否在三角形中。

**空间代价**：每个像素16个采样点，即frame_buf 变为原来的(16(msaa) + 1(与屏幕像素对应))倍，depth_buf变为原来的16倍.

**时间代价**：每个采样点都要执行inside_triangle判断，每个采样点也要插值得到z，最后还要遍历msaa_frame_buf，求平均得到屏幕上像素的颜色。时间代价大致是原来的16倍。

**对比实验**：

不同msaa 实际花费的实际时间为:

- 2x msaa cost 9 seconds
- 4x msaa cost 39 seconds
- 6x msaa cost 87 seconds
- 8x msaa cost 161 seconds

符合上面对于时间代价的估算

值得注意的：
8xmsaa和4xmsaa效果基本没差别，msaa是比较受限的，毕竟单个pixel只能显示一种颜色

完成时间2021-03-25

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

### 5. displacement shading
待补充。。。
