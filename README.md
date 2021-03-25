开始学习时间：2021-03-21
# 作业结果
### Assignment1
![ass1](https://user-images.githubusercontent.com/17798738/112030975-c9eb2f00-8b75-11eb-9578-832e5037da03.gif)
完成时间：2021-03-23

待完成提高题

### Assignment2
![image](https://user-images.githubusercontent.com/17798738/112307344-ce315c80-8cdb-11eb-84fe-c9bded4d44eb.png)
关于投影矩阵，书中的矩阵在作业中使用会使z值倒过来，具体原因不明。待补充
完成时间2021-03-24

#### MSAA
结果：
![image](https://user-images.githubusercontent.com/17798738/112426330-c1137c80-8d72-11eb-9e72-372e9aec67ef.png)

**大概思路**：根据msaa参数，生成采样坐标，比如4xmsaa，那就把每个像素划分为16宫格，每宫格的正中心为采样点。对于每一个采样点，要记录它的深度信息，并判断是否在三角形中。

**空间代价**：每个像素16个采样点，即frame_buf 变为原来的(16(msaa) + 1(与屏幕像素对应))倍，depth_buf变为原来的16倍.

**时间代价**：每个采样点都要执行inside_triangle判断，每个采样点也要插值得到z，最后还要遍历msaa_frame_buf，求平均得到屏幕上像素的颜色。时间代价大致是原来的16倍。

**对比实验**：

不同msaa 实际花费的实际时间为:
>2x msaa cost 9 seconds
4x msaa cost 39 seconds
6x msaa cost 87 seconds
8x msaa cost 161 seconds
符合上面对于时间代价的估算

 
