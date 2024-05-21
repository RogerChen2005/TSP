#include <graph.h>
#include <random>
#include <cmath>
#include <algorithm>
#include <limits>
#include <iostream>
#include <stack>

double distance(const point& p1,const point& p2){
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return std::sqrt(dx*dx+dy*dy);
}

bool operator < (const edge& x,const edge& y){
    return x.w < y.w;
}

graph::graph(int width,int height,int drone_count,double scan_radius){
    this->width = width;
    this->height = height;
    this->drone_count = drone_count;
    this->scan_radius = scan_radius;
    this->flag = 0;
}

void graph::addPoint(const point& p){
    this->points.push_back(p);
}

void graph::addPoint(double x,double y,double w){
    point _point;
    _point.x = x;
    _point.y = y;
    if(x < this->width && y < this->height)
        this->points.push_back(_point);
}

void graph::randomPoint(int count){
    int initial_size =  this->points.size();
    std::random_device rd;
    std::uniform_real_distribution<double> width_random(0,this->width);
    std::uniform_real_distribution<double> height_random(0,this->height);
    std::uniform_real_distribution<double> urd(0,1);
    this->points.resize(initial_size+count);
    for(auto it = this->points.begin()+initial_size;it != this->points.end();it++){
        (*it).x = width_random(rd);
        (*it).y = height_random(rd);
        (*it).w = urd(rd);
    }
    return;
}

void graph::add_edge(int u,int v,double w){
    edge _edge;
    _edge.u = u;
    _edge.v = v;
    _edge.w = w;
    this->egdes.push(_edge);
}

double graph::calculateTotalW(std::vector<int>& _points){
    double w = 0;
    for(const auto it :_points) w += this->points[it].w;
    return w;
}

double graph::calculateEdgeW(point& p1,point& p2){
    double dist = distance(p1,p2);
    return dist - 1.0 * (p1.w + p2.w);
}

void graph::createMap(){
    this->calculateTarget();
    int count = this->targets.size();
    double tw = 0.0;
    for(int i = 0;i < count;i++){
        for(int j = i+1;j < count ;j++){
            if(i!=j){
                double w = this->calculateEdgeW(this->targets[i],this->targets[j]);
                tw+=w;
                this->add_edge(i,j,w);
            }
        }
    }
    this->connect.resize(count);
    this->createForest(tw*this->targets.size()/this->egdes.size()/this->drone_count*1.4);
    this->createBestMatch();
    this->createEularRoute();
    this->createHamiltonian();
}

void graph::calculateTarget(){
    int points_size = this->points.size();
    this->points_to_targets.resize(points_size);
    std::vector<point_relations> map(this->height * this->width);
    for(int i = 0;i < points_size;i++){
        int mx = this->points[i].x,my = this->points[i].y;
        int left = std::floor(mx - this->scan_radius);
        int right = std::ceil(mx + this->scan_radius);
        for (int x = left; x <= right; x++){
            int dx = x - mx;
            double c = std::sqrt(this->scan_radius * this->scan_radius - dx * dx);
            int bottom = std::floor(my - c);
            int top = std::ceil(my + c);
            for (int y = bottom; y <= top; y++)
            {
                if (x > 0 && x < this->width && y > 0 && y < this->height)
                { 
                    map[y*this->width+x].points.push_back(i);
                }   
            }
        }
    }
    for(int i = 0;i < this->height;i++){
        for(int j = 0;j < this->width;j++){
            map[i*this->width+j].x = j;
            map[i*this->width+j].y = i;
        }
    }
    int n = this->height*this->width;
    for(int i = 0;i < n;i++){
        map[n-1].w = this->calculateTotalW(map[n-1].points);
        for(int j = n-1;j >i;j--){
           map[j-1].w = this->calculateTotalW(map[j-1].points);
           if(map[j].w > map[j-1].w){
                std::swap(map[j],map[j-1]);
           }
        }
        auto & obj = map[i];
        int target_length = this->targets.size();
        for(const auto p:obj.points){
            if(!this->points_to_targets[p]){
                this->points_to_targets[p] = target_length;
                this->points[p].w = 0.0;
            }
        }
        if(obj.w == 0.0) 
            break;
        this->targets.push_back({(double)obj.x,(double)obj.y,obj.w});
    }
    for(const auto p:points_to_targets){
        if(!p) this->targets.push_back(this->points[p]);
    }
    return;
}

int graph::findfa(int x){
	if(fa[x] == x){
		return x;
	}
	else return fa[x] = findfa(fa[x]);
}

bool graph::check(int a,int b){
	if(this->findfa(a) == this->findfa(b)){
		return true;
	}
	return false;
}

void graph::createForest(double p){
    int length = this->targets.size();
    std::vector<double> w(length);
    // for(int i = 0;i<length;i++) w[i] = this->targets[i].w;
    int cnt = 0;
    this->fa.resize(length);
	for(int i = 0;i < length;i++){
		fa[i] = i;
	}
    while(!this->egdes.empty()){
        edge _edge = this->egdes.top();
        this->egdes.pop();
        if(cnt == length - this->drone_count){
			break;
		}
		else{
            int y = w[findfa(_edge.v)];
			if(check(_edge.u,_edge.v) == false && w[findfa(_edge.u)] < p && w[findfa(_edge.v)] < p){
				cnt++;
                w[findfa(_edge.u)] += _edge.w + w[findfa(_edge.v)];
				fa[findfa(_edge.v)] = _edge.u;
                this->connect[_edge.u].push_back(_edge.v);
                this->connect[_edge.v].push_back(_edge.u);
			}
		}
    }
    for(int i = 0;i < length;i++){
		if(fa[i] == i) this->header.push_back(i);
	}
    if(this->header.size()!=this->drone_count){
        std::cerr << "Error Calculating header"<<std::endl;
        exit(-1);
    }
}

void graph::createBestMatch(){
    int count = this->targets.size();
    std::vector<int> path;
    std::vector<int> single;
    std::vector<bool> vis;
    for(int i = 0;i < count;i++){
        int length = this->connect[i].size();
        if(length & 1) single.push_back(i);
    }
    int single_size = single.size();
    vis.resize(single_size);
    for(int i = 0;i < single_size;i++){
        double min_w = std::numeric_limits<double>::infinity();
        int min_index = i;
        if(!vis[i]) {
            for(int j = i+1;j < single_size;j++){
                if(!vis[j] && findfa(i) == findfa(j)) {
                    int w = this->calculateEdgeW(this->targets[i],this->targets[j]);
                    if(w < min_w){
                        min_w = w;
                        min_index = j;
                    }
                }
            }
            this->connect[i].push_back(min_index);
        }
    }
    return;
}

void graph::createEularRoute(){
    int n = this->connect.size();
    std::vector<std::vector<int>> tempList(n);
	for(int i = 0; i < n; i++){
		tempList[i].resize(connect[i].size());
		tempList[i] = connect[i];
	}
    this->path.resize(this->drone_count);
	for(int i = 0;i<this->drone_count;i++){
        std::stack<int> stack;
	    int pos = this->header[i];
	    this->path[i].push_back(pos);
	    while(!stack.empty() || tempList[pos].size() > 0){
		    if(tempList[pos].empty()){
			    this->path[i].push_back(pos);
			    pos = stack.top();
			    stack.pop();
		    }
		    else{
			    stack.push(pos);
			    int neighbor = tempList[pos].back();
			    tempList[pos].pop_back();
			    for(int i = 0; i < tempList[neighbor].size(); i++){
			    	if(tempList[neighbor][i] == pos){
					    tempList[neighbor].erase(tempList[neighbor].begin()+i);
				    }
			    }
			    pos = neighbor;
		    }
	    }
	    this->path[i].push_back(pos);
    }
}

void graph::createHamiltonian(){
    int target_size = this->targets.size();
    std::vector<bool> vis(target_size);
    for(int i = 0;i<drone_count;i++){
        std::fill(vis.begin(),vis.end(),false);
        std::vector<int>::iterator it = this->path[i].begin();
        while(it != path[i].end()){
            if(!vis[*it]){
                vis[*it] = true;
                it++;
            }
            else it = this->path[i].erase(it);
        }
    }
}

void graph::printRoute(){
    for(int i = 0;i<drone_count;i++){
        std::cout << "Route " << i+1 << ":\n";
        for(const auto x:this->path[i]){
            std::cout << x << "->";
        }
        std::cout << "End" << std::endl;
    }
}