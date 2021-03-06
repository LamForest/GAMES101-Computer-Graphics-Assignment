//By GeT Left
//关于BVH建立和查询的伪代码
//包围盒与光线相交的算法就不写了，很简单

Node:
	object //所包含的物体，数量为1。对于内点，此项应为空，只有叶子结点可以拥有object
	boundingVolume //包围盒
	left 
	right

Node BVHBuild(objects):
	node = new Node();
	//recursively split until one object left
	//leaf node
	if objects.size == 1:
		node.boundingVolume = objects[0].getBoundingVolume();
		node.object = object[0]; //each leaf node will contain only one object
	//internal node
	else:
		// Union all objects into one boundingVolume, 
		// then find its longest axis
		axis = LongestAxis(Union(objects)) 
		objects = sortByAxis(objects, axis)
		
		//midean object
		mid = objects.size / 2
		
		node.left, node.right  = BVHBuild(objects[0:mid]), BVHBuild(objects[mid+1:])
		
		//For internal nodes, BoundingVolume is the Union of child nodes
		node.boundingVolume = Union(
			node.left.boundingVolume, node.right.boundingVolume
		)
		
		//Note that internal nodes dont hold any objects, they only hold boundingVolume
	
	return node

Intersection
	t //光线相交时的t值
	object //光线相交的第一个物体
	is_intersect //光线是否与任何物体相交了

Intersection BVHIntersect(Ray, Node root):
	Intersection inter
	inter.t = FLOAT_MAX, inter.object = null, inter.is_intersect = false
	if root == null:
		return inter
	else:
		//课上所讲的3对平面分别求交点
		if intersectVolume(Ray, root):
		
			if isInternalNode():
				inter_left, inter_right = BVHIntersect(Ray, root.left), BVHIntersect(Ray, root.right)
				//如果左子树确实与某个object相交了，且比之前的交点要近（即t更小）
				if inter_left.is_intersect and inter_left.t < inter.t:
					inter.t = inter_left.t
					inter.object = inter_left.object
					inter.is_intersect = true
				//do same thing to inter_right
				if inter_right.is_intersect and inter_right.t < inter.t:
					inter.t = inter_right.t
					inter.object = inter_right.object
					inter.is_intersect = true
			else: //leaf node
				//BVH的工作已经结束
				//现在让object的intersect函数判断光线是否与object相交
				//不同object的判断光线相交的写法都是不一样的，所以是object的一个方法
				inter = root.object.intersect(Ray)
					
	return inter	
		
