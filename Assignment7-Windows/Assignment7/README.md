# Assignment 7
完成时间 2021-04-05
## 框架代码示意图
![image](https://user-images.githubusercontent.com/17798738/113550142-27c35080-9625-11eb-9051-fe377f067846.png)


## 框架注意事项
### 判断面光源采样点是否被遮挡
判断面光源采样点是否被遮挡，epsilon要设的比较大，框架中的`EPSILON = 0.00001`;太苛刻了，会有这样的效果：
![image](https://user-images.githubusercontent.com/17798738/113547594-ce592280-9620-11eb-9a31-54386e583a36.png)

### get_random_float始终返回固定的数
这个属于框架的bug了。get_random_float每次调用时，都会初始化随机数引擎然后返回**第一个随机数**，所以说永远都是同一个数。C++ Primer第五版第17.4.1节也指出了这种使用标准库随机数的错误方式。

正确的方式是这样的：（并且也会使得程序提速50%以上）
```cpp
inline float get_random_float()
{
    static std::random_device dev;
    static std::mt19937 rng(dev());
    static std::uniform_real_distribution<float> dist(0.f, 1.f); // distribution in range [1, 6]

    return dist(rng);
}
```
## 疑惑
1. 每个pixel的所有path都是从像素正中心射出的，是否应该从像素不同位置射出？类似MSAA，说不定还有不错的抗锯齿效果
2. SPP达到512为什么还会有噪点？
3. 工业界的PT进行了什么优化，我渲染一张SPP=256的图像都要半小时，如何做到实时渲染？还是GPU处理能比CPU快万倍？（我觉得够呛，因为CNN中GPU只能做到快100倍，还是几万块的显卡）
4. 我们认为光源不再反射别的光了，这种假设是合理的吗？
5. BRDF的具体公式


## 实验结果/对比试验
### 线程个数对于执行时间的影响
![image](https://user-images.githubusercontent.com/17798738/113546781-3e66a900-961f-11eb-84ce-37255cf09e88.png)
 
### P_RR对于图片质量的影响
可以看到，P_RR增大的同时，图片噪点逐渐变少，质量提高。

（图片大小784x784，SPP = 128，P_RR = 0.2, 0.4, 0.6, 0.8，#threads = 8）
![image](https://user-images.githubusercontent.com/17798738/113546862-6b1ac080-961f-11eb-8ac4-671a117b773c.png)

### SPP对于图片质量的影响
可以看到，SPP增大的同时，图片噪点逐渐变少，质量提高。比较出乎我意外的是当SPP=512时，图片上仍然有星星点点的噪点。

我有试过将SPP设为1024，并花了1个小时等待结果，但是仍然是有噪点。这是我目前暂时解释不了的。

这里是截图的结果，原图见[这里](https://github.com/LamForest/GAMES101-Computer-Graphics-Assignment/blob/main/Assignment7-Windows/Assignment7/build/white_light/png/visible_WH_0784_SPP_512_RR_0.80.png)及同级目录。

（图片大小784x784，SPP = 1,2, ..., 512, P_RR = 0.8，#threads = 8）
![image](https://user-images.githubusercontent.com/17798738/113547059-c9e03a00-961f-11eb-9734-9e13d928ee87.png)
![image](https://user-images.githubusercontent.com/17798738/113547117-e0869100-961f-11eb-8ebd-ef2b4d7d8545.png)
