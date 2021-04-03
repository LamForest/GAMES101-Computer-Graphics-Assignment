#include <algorithm>
#include <cassert>
#include "BVH.hpp"

BVHAccel::BVHAccel(std::vector<Object*> p, int maxPrimsInNode,
                   SplitMethod splitMethod)
    : maxPrimsInNode(std::min(255, maxPrimsInNode)), splitMethod(splitMethod),
      primitives(std::move(p))
{
    time_t start, stop;
    time(&start);
    if (primitives.empty())
        return;

    root = recursiveBuild(primitives);

    time(&stop);
    double diff = difftime(stop, start);
    int hrs = (int)diff / 3600;
    int mins = ((int)diff / 60) - (hrs * 60);
    int secs = (int)diff - (hrs * 3600) - (mins * 60);

    printf(
        "\rBVH Generation complete: \nTime Taken: %i hrs, %i mins, %i secs\n\n",
        hrs, mins, secs);
}

BVHBuildNode* BVHAccel::recursiveBuild(std::vector<Object*> objects)
{
    BVHBuildNode* node = new BVHBuildNode();

    // Compute bounds of all primitives in BVH node
    Bounds3 bounds;
    for (int i = 0; i < objects.size(); ++i)
        bounds = Union(bounds, objects[i]->getBounds());
    if (objects.size() == 1) {
        // Create leaf _BVHBuildNode_
        node->bounds = objects[0]->getBounds();
        node->object = objects[0];
        node->left = nullptr;
        node->right = nullptr;
        node->area = objects[0]->getArea();
        return node;
    }
    else if (objects.size() == 2) {
        node->left = recursiveBuild(std::vector{objects[0]});
        node->right = recursiveBuild(std::vector{objects[1]});

        node->bounds = Union(node->left->bounds, node->right->bounds);
        node->area = node->left->area + node->right->area;
        return node;
    }
    else {
        Bounds3 centroidBounds;
        for (int i = 0; i < objects.size(); ++i)
            centroidBounds =
                Union(centroidBounds, objects[i]->getBounds().Centroid());
        int dim = centroidBounds.maxExtent();
        switch (dim) {
        case 0:
            std::sort(objects.begin(), objects.end(), [](auto f1, auto f2) {
                return f1->getBounds().Centroid().x <
                       f2->getBounds().Centroid().x;
            });
            break;
        case 1:
            std::sort(objects.begin(), objects.end(), [](auto f1, auto f2) {
                return f1->getBounds().Centroid().y <
                       f2->getBounds().Centroid().y;
            });
            break;
        case 2:
            std::sort(objects.begin(), objects.end(), [](auto f1, auto f2) {
                return f1->getBounds().Centroid().z <
                       f2->getBounds().Centroid().z;
            });
            break;
        }

        auto beginning = objects.begin();
        auto middling = objects.begin() + (objects.size() / 2);
        auto ending = objects.end();

        auto leftshapes = std::vector<Object*>(beginning, middling);
        auto rightshapes = std::vector<Object*>(middling, ending);

        assert(objects.size() == (leftshapes.size() + rightshapes.size()));

        node->left = recursiveBuild(leftshapes);
        node->right = recursiveBuild(rightshapes);

        node->bounds = Union(node->left->bounds, node->right->bounds);
        node->area = node->left->area + node->right->area;
    }

    return node;
}

Intersection BVHAccel::Intersect(const Ray& ray) const
{
    Intersection isect;
    if (!root)
        return isect;
    isect = BVHAccel::getIntersection(root, ray);
    return isect;
}

Intersection BVHAccel::getIntersection(BVHBuildNode* node, const Ray& ray) const
{
    //直接拷贝Ass6，没有修改
    // TODO Traverse the BVH to find intersection
    // this->depth += 1;
    Intersection inter;
    // inter.distance = std::numeric_limits<double>::infinity();
    //应该不可能跳进这个分支，在叶子节点就会结束递归
    if(node == nullptr){
        assert(false);
        return inter;
    }

    std::array<int, 3> dirIsNeg{
        int(ray.direction.x > 0), int(ray.direction.y > 0), int(ray.direction.z > 0)
        }; //这个代表正穿反穿，突然懂了
    // Vector3f invDir(
    //     1.0/ray.direction.x, 1.0/ray.direction.y, 1.0/ray.direction.z
    //     );
    // bool is_enter, t_enter;
    // std::tie(is_enter, t_enter) = node->bounds.IntersectP(ray, invDir, dirIsNeg);
    bool is_enter = node->bounds.IntersectP(ray, ray.direction_inv, dirIsNeg);
    // if(is_enter){

    //     printf("depth [%2d], is enter = %s\n", this->depth, is_enter ? "True" :  "False");
    // }
    if(is_enter){
        //利用node->object是否为空，来判断我们是否已到达叶子节点
        if(node->object != nullptr){
            //leaf node
            inter = node->object->getIntersection(ray); //可能进入另一颗BVH树
        }else{

            Intersection inter_left = getIntersection(node->left, ray); 
            Intersection inter_right = getIntersection(node->right, ray); 
            if(inter_left.happened){
                if(inter_left.distance < inter.distance){
                    inter = inter_left;
                }
            }
            if(inter_right.happened){
                if(inter_right.distance < inter.distance){
                    inter = inter_right;
                }
            }
        }

    }
    //is_enter = false则不用递归
    //父结点与光线不相交，则子结点也一定不相交
    // this->depth -= 1;
    return inter;
}


void BVHAccel::getSample(BVHBuildNode* node, float p, Intersection &pos, float &pdf){
    if(node->left == nullptr || node->right == nullptr){
        node->object->Sample(pos, pdf);
        pdf *= node->area;
        return;
    }
    if(p < node->left->area) getSample(node->left, p, pos, pdf);
    else getSample(node->right, p - node->left->area, pos, pdf);
}

void BVHAccel::Sample(Intersection &pos, float &pdf){
    float p = std::sqrt(get_random_float()) * root->area;
    getSample(root, p, pos, pdf);
    pdf /= root->area;
}