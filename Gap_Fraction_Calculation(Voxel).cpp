#include<bits/stdc++.h>

using namespace std; 

double x,y,z;
double X_max = -0xfffffff;
double Y_max = -0xfffffff;
double X_min = 0xfffffff;
double Y_min = 0xfffffff;
int X_cor;
int Y_cor;
//double map = [100000][100000];
double temp;


//每个栅格边长有多大 
double resolution = 1.0;
//存储点云 
double pc[10000000][2];
//主函数
int main()
{
	//输入的原始点云数据（x,y,z） 
	ifstream input("C:\\Users\\PC\\Desktop\\input.txt");

	//输出的栅格属性 
	ofstream output_data_attribute("C:\\Users\\PC\\Desktop\\test\\output_data_attribute.txt"); 
	int num = 0;
	while(input>>x>>y>>z>>temp)
	{
		pc[num][0] = x;
		pc[num][1] = y;
		num++; 
		if(x > X_max)	X_max = x;
		if(y > Y_max)	Y_max = y;
		if(x < X_min)	X_min = x;
		if(y < Y_min)	Y_min = y;
	}
	//关闭文件
	input.close();
	
	int X_length = ceil((X_max - X_min)/resolution);
	int Y_length = ceil((Y_max - Y_min)/resolution);
	
	output_data_attribute<<"Input the point cloud file path as "<<endl<<"C:\\Users\\PC\\Desktop\\input.txt"<<endl<<endl; 
	output_data_attribute<<"Number of points is "<<num<<endl; 
	output_data_attribute<<"The minimum value of the point cloud x-coordinate is "<<X_min<<endl; 
	output_data_attribute<<"The maximum value of the point cloud x-coordinate is "<<X_max<<endl; 
	output_data_attribute<<"The minimum value of the point cloud y-coordinate is "<<Y_min<<endl; 
	output_data_attribute<<"The maximum value of the point cloud y-coordinate is "<<Y_max<<endl; 
	output_data_attribute<<"The raster spatial resolution is "<<resolution<<endl;
	output_data_attribute<<"The raster size is "<<X_length<<"*"<<Y_length<<endl; 
	
	//关闭栅格属性文件
	output_data_attribute.close();
	
	
	
	//输出的栅格 
	ofstream output_data_mapping("C:\\Users\\PC\\Desktop\\test\\output_data_mapping.txt"); 
	
	double **GF_map;
	GF_map = (double**)calloc(X_length,sizeof(double*));

    for(int j = 0; j < X_length; j++)
    {
        GF_map[j] = (double*)calloc(Y_length,sizeof(double));
    }


	
	for(int i = 0; i < X_length; i++)
	{
		for(int j = 0; j < Y_length; j++)
		{
			GF_map[i][j] = 0;
		}
	}
	
	int count = 0;
	for(int i = 0; i < num; i++)
	{
		X_cor = floor((pc[i][0] - X_min)/resolution);
		Y_cor = floor((pc[i][1] - Y_min)/resolution);
		GF_map[X_cor][Y_cor] = 1;
		count++;
		//cout<<X_cor<<" "<<Y_cor<<endl;
	} 
	


 
	for(int i = 0; i < X_length; i++)
	{
		for(int j = 0; j < Y_length; j++)
		{
			output_data_mapping<<GF_map[i][j]<<" ";
		}
		output_data_mapping<<endl;
	}	

	output_data_mapping.close();//关闭输出栅格
	
	double gf = 1.0*count/(Y_length*X_length);
	
	cout<<"finished!"<<endl;
	cout<<"The gap fraction at teh spatial resolution of "<<resolution<<"m is "<<gf<<endl;
	return 0;
}
