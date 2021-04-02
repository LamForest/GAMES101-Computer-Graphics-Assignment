## Assignment6
### 结果
![image](https://user-images.githubusercontent.com/17798738/113426239-c2454900-9405-11eb-950a-ba699db8a5b5.png)
运行时间：
```
$ ./RayTracing.exe
BVH Generation complete:
Time Taken: 0 hrs, 0 mins, 0 secs

 - Generating BVH...

BVH Generation complete:
Time Taken: 0 hrs, 0 mins, 0 secs

Render complete: ======================================================] 100 %
Time taken: 0 hours
          : 0 minutes
          : 6 seconds
```

### 写代码遇到的bug
BVH本质上是一颗二叉树，老熟人了，所以并没有什么bug。

在`Bounds3::IntersectP`中，一开始没有考虑光线可能反穿，导致t_enter和t_exit反过来了，得到的结果只有背景色。



### 问题
#### 求三角形与ray交点时，det = 0说明什么？光线与三角形所在面平行？
说明该线性方程组无唯一解（有很多解？），比如光线与三角形所在面平行

#### 求光线与面的交点时，不用判断光线与面平行吗？
额，不用貌似暂时也没关系，所以先不管了。

#### 场景中只有1个BVH树？
Ass6中有2个，一个是Scene持有的，还有一个是MeshTriangle（即兔子）持有的，Scene查找自己的BVH树时，发现与MeshTriangle相交，MeshTriangle会查找自己的BVH树，发现具体与哪个三角形相交。

其中，scene的BVH树只有1个结点，持有MeshTriangle。而MeshTriangle则有很多个结点，每个叶子结点只持有1个三角形。

查看代码可以发现，BVHAccel的构造函数被调用了3次，分别在
* `Scene::buildBVH()`此为建立scene的BVH
* `MeshTriangle`的构造函数，此为建立兔子的BVH

推而广之，我们可以推测，对于一个N个模型的场景，会有N+1个BVH，分别属于scene和N个模型。

#### 场景中有2个BVH树，应该从哪个BVH树开始查找呢？
从scene开始查找，当它到达MeshTriangle所在叶子结点是，会调用MeshTriangle::getIntersection，而它会从MeshTriangle自己的BVH树的根结点出发，递归查找，直到到达某一个三角形。
```cpp
    Intersection getIntersection(Ray ray)
    {
        Intersection intersec;
        if (bvh) {
            intersec = bvh->Intersect(ray);
        }
        return intersec;
    }
```
所以说，不仅BVH树是递归结构，BVH树之间的关系也是递归的。

#### 为什么只计算Ray与所有模型的第一个交点？
光线追踪是一个递归的过程，在第一个交点处进行光线的折射与反射后，会进入下一层castRay，又会对BVH树进行一次查询。
#### 复杂度
每次castRay都要查询BVH的所有叶子节点，所以假设一根光线穿过了所有object，castRay的最坏复杂度是

$$O( Nlog_2N t_b + t_0)$$

N = #objects, $t_b$是判断光线与Bounding Volume的复杂度,$t_0$是判断object（比如三角形，球）与光线相交的复杂度。而将光线与每个object进行穷举的复杂度为

$O( N t_0)$

两者相除为 

$$O( \frac{Nlog_2N t_b + t_0}{Nt_0}) \approx  \frac{log_2(N) t_b}{t_0}$$

额，这看上去，除非$t_b$比$t_0$小很多才行。也可能比较平均复杂度更公平，但是不知道怎么算。

#### 哪种优化方式最好？

>虎书
>The most popular structures are binary space partitioning trees (BSP trees), uniform subdivision grids, and bounding volume hierarchies. Most people who use BSP trees make the splitting planes axis-aligned, and such trees are usually called k-d trees. There is no clear-cut answer for which is best, but all are much, much better than brute-force search in practice. If I were to implement only one, it would be the bounding volume hierarchy because of its simplicity and robustness.

Uniform subdivision grids：最朴素的方式，对空间做大小相同的格子般的划分。
Space Partitioning trees: 以某种启发式的方法，对空间做axis-aigned的划分，比如KD tree
Bounding volume hierarchies: 对对象做划分，安排在一颗近似平衡的二叉树中

课上都涵盖了




### 算法 建立BVH树 与查询BVH
伪代码见[算法](https://github.com/LamForest/GAMES101-Computer-Graphics-Assignment/blob/main/Assignment6-Windows/Assignment6/%E7%AE%97%E6%B3%95.cpp)


