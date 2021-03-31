完成时间 2021-03-31
## Assignment5
本次作业的框架代码貌似来自这里：https://www.scratchapixel.com/code.php?id=8&origin=/lessons/3d-basic-rendering/ray-tracing-overview

这次作业实际上完成的代码并不是whitted-style ray tracing的核心代码，核心代码在trace castRay fresnel，作业只是让我们填上了一些细枝末节的部分。

### 结果
![image](https://user-images.githubusercontent.com/17798738/113126220-fecc4580-9249-11eb-9e42-b2ab4a77cdae.png)

### 关键代码分析
#### castRay
递归终止条件：
* 对于每一个碰撞点，如果该点是DIFFUSE_AND_GLOSSY，则返回该点的hit_color
* 对于每一个碰撞点，如果该点是REFLECTION_AND_REFRACTION，则对于入射光线，反射光线分别castRay
##### case REFLECTION_AND_REFRACTION
##### case DIFFUSE_AND_GLOSSY (即default)
##### case REFLECTION
程序并不会到达这个分支。

该怎么使用？

是否有bug
#### trace

#### fresnel
https://zhuanlan.zhihu.com/p/31534769

### maxDepth的影响
将scene.maxDepth设为0-5可以得到不同的光线追踪结果（见Assignment5-Windows/build），从完全没有光线追踪，到效果较好的光线追踪。

对于每一个碰撞点，如果该点存在折射与反射，那么该点的折射光线和反射光线会继续进行castRay操作，直到达到maxDepth，所以时间复杂度大致为：

O(2^(maxDepth+1) - 1) * O(width * height)

### 其他问题

#### x,y 的正负值
图像是从左上角开始，逐行生成的，所以framebuffer[0]指的是(-w/2, h/2)处的像素。
