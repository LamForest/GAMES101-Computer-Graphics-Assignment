## Ass4 贝塞尔曲线
完成时间 2021-03-29
### 结果
naive_bezier使得RGB中的R = 255，recursize_bezier使得RGB中的B = 255。所以下图显示的都是黄色的贝塞尔曲线。

（opencv中像素排列顺序时BGR不是RGB）

![image](https://user-images.githubusercontent.com/17798738/112831725-f1d31900-90c6-11eb-8d9f-1da7dfbf4f73.png)

### 递归算法思路
1. 若控制点序列长度为2，则直接返回插值结果
2. 若控制点序列长度大于2（设为n）， 则将相邻的控制点进行插值，生成n-1个新的控制点，转1。

以5个控制点为例：
![image](https://user-images.githubusercontent.com/17798738/112832517-02d05a00-90c8-11eb-9e98-ea276c35a1a2.png)

### 提高题 反走样
待完成
