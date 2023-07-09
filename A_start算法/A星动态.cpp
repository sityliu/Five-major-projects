#include<iostream>
#include<vector>
#include<string.h>
#include <unistd.h>
using namespace std;

#define Y 10 //宏定义 X,Y
#define X 10
#define ZX 10 //定义 G

//每个格子的属性 = 基础属性 + 额外属性

/****设置格子模板，基础属性****/
struct box
{
    int y;	//纵坐标
    int x;	//横坐标
    int g;	//实际代价
    int h;	//预估代价
    int f;	//总代价
    
    void set_f(){f=g+h;};	//通过函数实现计算总代价 f = g + h 的值
};

/****设置格子额外属性****/
struct subjoin_box
{
    struct box basics;						//嵌套格子基础属性
    vector<struct subjoin_box*> son;		//记录儿子的格子地址
    struct subjoin_box* father;				//记录父亲的格子地址
};

/****用数字表示上下左右****/
enum direction
{
    Right=0,	//右	0
    Down,		//下	1
    Left,		//左	2
    Up,			//上	3

};

/****函数作用：用来计算四个方向的格子的g，h和f值****/
/****参数解释：
	now_box：当前格子的结构体地址
	end：终点格子的结构体地址
	direction：代表方向的数字
****/
struct subjoin_box* deploy(struct subjoin_box *now_box, struct subjoin_box *end, int direction)
{
    struct subjoin_box * s1= new struct subjoin_box;	//申请堆空间
    memset(s1,0,sizeof(struct subjoin_box));			//申请后，先清空一下空间
	
    s1->basics.y=now_box->basics.y;					//通过指针访问结构体成员
    s1->basics.x=now_box->basics.x;
	
	//右方格子，Y不变，X+1，实际代价为10
    if(direction==0)
    {
        s1->basics.x++; 
        s1->basics.g=ZX;
    }
	//下方格子，Y+1，X不变，实际代价为10
    else if(direction==1)
    {
        s1->basics.y++;
        s1->basics.g=ZX;
    }
	//左方格子，Y不变，X-1，实际代价为10
    else if(direction==2)
    {
        s1->basics.x--;
        s1->basics.g=ZX;
    }
	//上方格子，Y-1，X不变，实际代价为10
    else if(direction==3)
    {
        s1->basics.y--;
        s1->basics.g=ZX;
    }
	
    //计算终点格子坐标与方向格子坐标的差值
    int y=abs(end->basics.y - s1->basics.y);	//纵坐标差 = 终点y - 当前y
    int x=abs(end->basics.x - s1->basics.x);	//横坐标差 = 终点x - 当前x
	
    s1->basics.h=(y+x)*10;	//计算预估代价h
    s1->basics.set_f();		//调用函数计算f
    return s1;				//把格子的结构体地址返回
}

int main()
{
    int map[Y][X]={        			//建立原始地图,0 代表空，1 代表障碍☆☆☆☆☆☆
	
    {0,0,0,0,0,0,0,0,0,0},
    {0,1,2,1,2,1,2,1,2,1},
    {0,1,1,1,1,1,1,1,1,1},
    {0,0,1,2,1,2,1,2,1,2},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,1,2,1,2,1,2,1,2,1},
    {0,1,1,1,1,1,1,1,1,1},
    {0,1,2,1,2,1,2,1,2,1},
    {0,0,0,0,0,0,0,0,0,0}
	
	};
	
    int track_map[Y][X]={        	//建立路径地图,0 代表没走过，1 代表走过了，2 代表路径
	
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0}
	
	};
	
	system("clear");	//清屏
	
	//显示停车场地图☆☆☆☆☆☆
	for(int i=0;i<=9;i++)
	{
		for(int j=0;j<=9;j++)
		{
			if(map[i][j]==1)
			{
				cout << "🔶";
			}
			else if(map[i][j] == 2)
			{
				cout << "Ｐ";
			}
			else
			{
				cout << "⚪";
				
			}
			
		}
		cout<<endl;
	}
	
	cout << endl << "目前停车位如下：" << endl;
	
	//显示停车位坐标☆☆☆☆☆☆
	for(int i=0;i<=9;i++)
	{
		for(int j=0;j<=9;j++)
		{
			if(map[i][j] == 2)
			{
				cout << "(" << i << "," << j << ")\t";
			}
		}
		cout<<endl;
	}
	
	//停车位选择，横坐标与纵坐标☆☆☆☆☆☆
	int track_space_x;
	int track_space_y;
	
	//输入停车位☆☆☆☆☆☆
	cout << "请选择停车位:" ;
	cin >> track_space_y >> track_space_x;
	
    // track_map[0][0]=true; 			//把起点坐标保存到track_map[][]里

    struct subjoin_box start;		//创建起点格子，并配置格子基础属性
    start.basics.y=0;
    start.basics.x=0;
    start.father=NULL;				//配置父指针指向NULL

    struct subjoin_box end;			//创建终点格子，并配置格子基础属性
    end.basics.y=track_space_y;		//确定终点☆☆☆☆☆☆
    end.basics.x=track_space_x;
    end.father=NULL;

    struct subjoin_box *now_box=&start;			//用来保存父亲格子的地址，初始化的父节点就是起点
    struct subjoin_box *New_box = NULL;				//保存新格子的地址
	
    vector<struct subjoin_box*> goodbox;			//创建一个容器保存符合条件的格子的地址
    vector<struct subjoin_box*>::iterator min_f;	//创建迭代器，min_f保存最小f值
    vector<struct subjoin_box*>::iterator each_f ;	//用来遍历f值
	
    int end_flag =0;	//创建一个变量判断哪总情况跳出循环
	
    //每一个格子都要经过以下步骤
    while(1)
    {
		//以当前所在的点为中心，求出周围四个方向的点(以当前所在点的坐标)
        for(int i=0;i<4;i++)
        {
            //试探每个方向的格子
            switch(i)
            {
                case Right:
                New_box = deploy(now_box,&end,Right); 
                break;

                case Down:
                New_box = deploy(now_box,&end,Down);
                break;


                case Left:
                New_box = deploy(now_box,&end,Left);
                break;


                case Up:
                New_box = deploy(now_box,&end,Up);
                break;

            }
            
			
            if((map[New_box->basics.y][New_box->basics.x]!=1) &&		//判断新格子是否障碍
               (New_box->basics.y<10 && New_box->basics.y>=0) &&		//判断新格子是否超出地图最大边界
               (New_box->basics.x<10 && New_box->basics.x>=0) &&
               (track_map[New_box->basics.y][New_box->basics.x]!=true))	//判断信鸽子是否已经走过
            {
                goodbox.push_back(New_box);			//把符合条件的格子的地址存入容器					符合条件的格子列表
                now_box->son.push_back(New_box);	//把符合条件的格子的地址存入起点父节点的son容器里	记住儿子
                New_box->father=now_box;			//把父亲的格子的地址保存father指针					记住父亲
            }
            else
            {
                delete New_box;						//丢弃不符合要求的格子
            }
        }
		
		//合格的格子列表不为空
        if(goodbox.size()!=0)
        {
            min_f = goodbox.begin();				//假设第一个就是最小的 f 值
			
            //用迭代器 each_f 遍历格子列表，并且找到最小的 f 值的格子
            for(each_f = goodbox.begin(); each_f != goodbox.end(); each_f++)
            {
				//寻找容器里 f 值最小的格子
                min_f = ((*min_f)->basics.f < (*each_f)->basics.f) ? min_f : each_f;
                
            }
        }
		//合格的格子列表为空，则表示已经无路可走
        else if(goodbox.size()==0)
        {
                end_flag = 2;
                break;			//跳出死循环
                
        }
		
		//这个时候 min_f 保存的就是 f 值最小的格子的地址
		now_box = *min_f;	//更新父节点的地址，选取 f 值最小的格子作为新的当前格
		goodbox.erase(min_f);	//已被选中的格子要被删除
		track_map[now_box->basics.y][now_box->basics.x]= 1;					//在路径地图上标记该坐标已走过
		
		if(now_box->basics.y==end.basics.y&&now_box->basics.x==end.basics.x)	//判断 新的当前格 是否为终点
		{
			end_flag=1;
			break;				//跳出死循环
			
		}
    }
	
	int box_count = 0;					//用于计算有多少个坐标值☆☆☆☆☆☆
	struct subjoin_box *temp = now_box;	//利用一个结构体指针，进行回溯，并计算坐标值个数☆☆☆☆☆☆
	while(temp->father != NULL)			//回溯并计算☆☆☆☆☆☆
	{
		box_count++;
		temp = temp->father;
	}
	int goodnum = box_count;			//保存个数，后面有用☆☆☆☆☆☆
	int arr_goodbox[box_count][2] = {};	//定义二维数组，并清空☆☆☆☆☆☆
	box_count--;						//自减1，作为二维数组最后一个下标☆☆☆☆☆☆
	
	//判断是否找到终点，1 代表找到终点，2 代表死路
    if(end_flag==1)
    {
		//清空路径地图
		bzero(track_map, X * Y * 4);	//☆☆☆☆☆☆
		track_map[0][0] = 1;			//☆☆☆☆☆☆
		
        //通过回溯找到路径坐标点，当前 now_box 保存的是终点格子地址
        while(now_box->father!=NULL)
        {
			arr_goodbox[box_count][0] = now_box->basics.y;		//存放y坐标
			arr_goodbox[box_count--][1] = now_box->basics.x;	//存放x坐标
			
            now_box=now_box->father;
        }
    }
    else if(end_flag==2)
    {
        cout<<"您选择的停车位不存在或已被使用！！！"<<endl;
		return -1;
    }
    
	//刷新 goodnum 次☆☆☆☆☆☆
	for(int k = 0; k < goodnum; k++)
	{
		system("clear");
		//把最终的图形效果显示☆☆☆☆☆☆
		for(int i=0;i<=9;i++)
		{
			for(int j=0;j<=9;j++)
			{
				if(map[i][j]==1)
				{
					cout<<"🔶";
				}
				//显示路径
				else if(track_map[i][j] == 1 ||( arr_goodbox[k][0] == i && arr_goodbox[k][1] == j ))
				{
					cout<<"🚖";
					// track_map[i][j] = 1;  // 轨迹显示
				}
				else
				{
					cout<<"⚪";
				}
				
			}
			cout<<endl;
		}
		sleep(1);//用于停顿，模仿动态
	}
	
	cout << endl << "停车完毕~" << endl;
	
	
    return 0;
}
