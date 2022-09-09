#include <bits/stdc++.h>
using namespace std;

#define PI 3.1415926

//random函数 
float Random(float L, float R)
{
	return L + (R - L)*rand() / RAND_MAX;
}


////文件路径 
//原始数据路径，为txt文档，且内容格式有要求。 
char raw_data[255] = "D:\\尤航凯\\uestc_lidar\\yulan.txt";
//滤去地面点并且转换了坐标的文件 
char data_ground_filtered[255] = "D:\\尤航凯\\uestc_lidar\\yulan_nonground.txt";
//此处后缀不要加。txt 
char zenith_file[255] = "D:\\尤航凯\\uestc_lidar\\分块测试\\";
//最终gap fraction结果统计输出 
char stats_file[255] = "D:\\尤航凯\\uestc_lidar\\分块测试\\stats\\stats.txt";

//主要参数 
double theta_start = 20.0;
double theta_end = 90.0;
double delta_theta = 5.0;
constexpr auto N = 100000;

//中间变量 
double temp_x,temp_y,temp_z,temp_theta,temp_phai,temp_r,temp_capture_dis; 
double pc[2000000][10] = {0};
double ran[N][3];
int num = 0;
int countnum;
double  gf,c, d, a, b, p, s, beam_zenith, beam_azimuth;

int main()
{
	//转换坐标并删除地面点 
	ifstream raw_data_in(raw_data);//读入原始文件
	ofstream data_ground_filtered_out(data_ground_filtered);//输出数据
	
	if (!raw_data_in.is_open())
	{
		cout << "Error:opening file fail! Raw data reading failed!!!" << endl;
		exit(1);
	}
	
	while (!raw_data_in.eof())
	{
		raw_data_in >> temp_x >> temp_y >> temp_z;
		if (temp_z > 0) //	仪器架设位置为（0，0，0）
		{
			temp_r = sqrt(temp_x*temp_x + temp_y*temp_y + temp_z*temp_z);
			temp_theta = atan(sqrt(temp_x*temp_x + temp_y*temp_y) / temp_z) * 180 / PI;   //计算天顶角，弧度转角度 *180/π

			if (temp_x >= 0 && temp_y >= 0)
			{
				temp_phai = asin(temp_x / sqrt(temp_x*temp_x + temp_y*temp_y)) * 180 / 3.14159;
			}
			else if (temp_x >= 0 && temp_y <= 0)
			{
				temp_phai = asin(fabs(temp_y) / sqrt(temp_x*temp_x + temp_y*temp_y)) * 180 / 3.14159 + 90;
			}
			else if (temp_x <= 0 && temp_y <= 0)
			{
				temp_phai = asin(fabs(temp_x) / sqrt(temp_x*temp_x + temp_y*temp_y)) * 180 / 3.14159 + 180;
			}
			else if (temp_x <= 0 && temp_y >= 0)
			{
				temp_phai = asin(temp_y / sqrt(temp_x*temp_x + temp_y*temp_y)) * 180 / 3.14159 + 270;
			}
			
			temp_capture_dis = 0.05 * temp_r /(2.0 * 100.0);
			data_ground_filtered_out << temp_x << " " << temp_y << " " << temp_z << " " << temp_theta << " " << temp_phai << " " << temp_r <<" "<<temp_capture_dis<<endl;
		}
		
	}
	cout<<"Coordinates changed!"<<endl; 
	raw_data_in.close(); 
	data_ground_filtered_out.close(); 
	
	//说是地面点删除，但是其实是删去扫描仪水平位置以下的所有点。在平地范围内无所谓，但是在坡地或是其他特殊情况，切记要对上述步骤进行修正，不然会出现大问题。 
	//至于坐标转换方面，则是将原有的空间直角坐标系x，y，z转换成了空间直角坐标系+以扫描仪为球心的球坐标系的融合。 
	
	
	//分天顶角
	for(double zenith = theta_start; zenith <= theta_end; zenith = zenith + delta_theta)
	{
		ifstream data_ground_filtered_in(data_ground_filtered);	
		ofstream output_zenith((zenith_file+to_string(zenith)+"to"+to_string(zenith + delta_theta)+".txt").c_str());
		
		if (!data_ground_filtered_in.is_open())
		{
			cout << "Error:opening file fail" << endl;
			exit(1);
		}

		while (!data_ground_filtered_in.eof())
		{
			data_ground_filtered_in >> temp_x >> temp_y >> temp_z >> temp_theta >> temp_phai >> temp_r>>temp_capture_dis;

			if (temp_theta > zenith && temp_theta <= zenith + delta_theta)
			{
				output_zenith << temp_x << " " << temp_y << " " << temp_z << " " << temp_theta << " " << temp_phai << " " << temp_r <<" "<<temp_capture_dis<<endl;
			}
	}
	
	data_ground_filtered_in.close(); 
	output_zenith.close();
	}
	cout<<"Zenith seperation finished!"<<endl;
	
	//分方位角
	for(double zenith = theta_start; zenith <= theta_end; zenith = zenith + delta_theta)
	{
		ifstream input_zenith((zenith_file+to_string(zenith)+"to"+to_string(zenith + delta_theta)+".txt").c_str());
		ofstream  outfile1((zenith_file+to_string(zenith)+"to"+to_string(zenith + delta_theta)+"_a0.txt").c_str());
		ofstream  outfile2((zenith_file+to_string(zenith)+"to"+to_string(zenith + delta_theta)+"_a45.txt").c_str());
		ofstream  outfile3((zenith_file+to_string(zenith)+"to"+to_string(zenith + delta_theta)+"_a90.txt").c_str());
		ofstream  outfile4((zenith_file+to_string(zenith)+"to"+to_string(zenith + delta_theta)+"_a135.txt").c_str());
		ofstream  outfile5((zenith_file+to_string(zenith)+"to"+to_string(zenith + delta_theta)+"_a180.txt").c_str());
		ofstream  outfile6((zenith_file+to_string(zenith)+"to"+to_string(zenith + delta_theta)+"_a225.txt").c_str());
		ofstream  outfile7((zenith_file+to_string(zenith)+"to"+to_string(zenith + delta_theta)+"_a270.txt").c_str());
		ofstream  outfile8((zenith_file+to_string(zenith)+"to"+to_string(zenith + delta_theta)+"_a315.txt").c_str());
		
		
		if (!input_zenith.is_open())
		{
			cout << "Error:opening file fail" << endl;
			exit(1);
		}

		while (!input_zenith.eof())
		{
			input_zenith >> temp_x >> temp_y >> temp_z >> temp_theta >> temp_phai >> temp_r>>temp_capture_dis;

			if ((temp_phai > 337.5 && temp_phai <= 360) || (temp_phai >= 0 && temp_phai <= 22.5))
			{
				outfile1 << temp_x << " " << temp_y << " " << temp_z << " " << temp_theta << " " << temp_phai << " " << temp_r <<" "<<temp_capture_dis<<endl;
			}
			else if (temp_phai > 22.5 && temp_phai <= 67.5)
			{
				outfile2 << temp_x << " " << temp_y << " " << temp_z << " " << temp_theta << " " << temp_phai << " " << temp_r <<" "<<temp_capture_dis<<endl;
			}
			else if (temp_phai > 67.5 && temp_phai <= 112.5)
			{
				outfile3 << temp_x << " " << temp_y << " " << temp_z << " " << temp_theta << " " << temp_phai << " " << temp_r <<" "<<temp_capture_dis<<endl;
			}
			else if (temp_phai > 112.5 && temp_phai <= 157.5)
			{
				outfile4 << temp_x << " " << temp_y << " " << temp_z << " " << temp_theta << " " << temp_phai << " " << temp_r <<" "<<temp_capture_dis<<endl;
			}
			else if (temp_phai > 157.5 && temp_phai <= 202.5)
			{
				outfile5 << temp_x << " " << temp_y << " " << temp_z << " " << temp_theta << " " << temp_phai << " " << temp_r <<" "<<temp_capture_dis<<endl;
			}
			else if (temp_phai > 202.5 && temp_phai <= 247.5)
			{
				outfile6 << temp_x << " " << temp_y << " " << temp_z << " " << temp_theta << " " << temp_phai << " " << temp_r <<" "<<temp_capture_dis<<endl;
			}
			else if (temp_phai > 247.5 && temp_phai <= 292.5)
			{
				outfile7 << temp_x << " " << temp_y << " " << temp_z << " " << temp_theta << " " << temp_phai << " " << temp_r <<" "<<temp_capture_dis<<endl;
			}
			else if (temp_phai > 292.5 && temp_phai <= 337.5)
			{
				outfile8 << temp_x << " " << temp_y << " " << temp_z << " " << temp_theta << " " << temp_phai << " " << temp_r <<" "<<temp_capture_dis<<endl;
			}	 
		}
		//上述过程也可以用循环变成变长度分块去简化，但是我累了，就不优化了。
		
		input_zenith.close();
		outfile1.close();
		outfile2.close();
		outfile3.close();
		outfile4.close();
		outfile5.close();
		outfile6.close();
		outfile7.close();
		outfile8.close();
	} 
	cout<<"Area seperation finished!"<<endl;
	
	//基于蒙特卡洛的间隙率求解 
	//结果输出于stats中 
	ofstream  stats(stats_file);
	
	for(double zenith = theta_start; zenith <= theta_end; zenith = zenith + delta_theta)
	{
		for(int angle = 0; angle <= 315; angle = angle + 45)
		{
			num = 0;
			countnum = 0;
			ifstream  infile((zenith_file+to_string(zenith)+"to"+to_string(zenith + delta_theta)+"_a"+to_string(angle)+".txt").c_str());
			
			if (!infile.is_open())
			{
				cout << "Error:opening file fail" << endl;
				exit(1);
			}
			if (!stats.is_open())
			{
				cout << "Error:opening outfile fail" << endl;
				exit(1);
			}
			while (!infile.eof())
			{
				infile >> temp_x >> temp_y >> temp_z >> temp_theta >> temp_phai >> temp_r >> temp_capture_dis;
				pc[num][0] = temp_x;
				pc[num][1] = temp_y;
				pc[num][2] = temp_z;
				pc[num][4] = temp_phai; 
				pc[num][5] = temp_r;
				pc[num][6] = temp_capture_dis;
				num++;
			}
			
			srand(time(NULL));
			
			stats<<"天顶角在:"<<zenith<<"到"<<zenith+delta_theta<<"之间，正中方位角为"<<":  "<<angle<<"的数据:  "<<endl;
			
			for (int i = 0; i < N; i++)
			{
				beam_zenith = Random(0.0, 5.0)*PI / 180;
				beam_azimuth = Random(angle-22.5, angle+22.5)*PI / 180;
	
				ran[i][0] = sin(beam_zenith)*sin(beam_azimuth);//x
				ran[i][1] = sin(beam_zenith)*cos(beam_azimuth);//y
				ran[i][2] = cos(beam_zenith);//z
	
				for (int j = 0; j < num; j++)
				{
					c = sqrt((pc[j][0] - ran[i][0])*(pc[j][0] - ran[i][0]) + (pc[j][1] - ran[i][1])*(pc[j][1] - ran[i][1]) + (pc[j][2] - ran[i][2])*(pc[j][2] - ran[i][2]));
					d = 0.5*sqrt((1 + pc[j][5] + c)*(1 + pc[j][5] - c)*(1 - pc[j][5] + c)*(-1 + pc[j][5] + c));
					//cout << d << endl;

					if (d <= pc[j][6])
					{
						countnum++;
						break;
					}
				}

				if (i == N / 10 *3)
				cout <<zenith<<":  "<<angle<<":  "<< "进度10%" << endl;
				if (i == N / 10 *5)
				cout <<zenith<<":  "<<angle<<":  "<< "进度50%" << endl;
				if (i == N / 10 *7)
				cout <<zenith<<":  "<<angle<<":  "<< "进度70%" << endl;
				if (i == N)
				cout <<zenith<<":  "<<angle<<":  "<< "进度100%" << endl;
			}
			stats<<"撞击次数："<< countnum << endl;
			infile.close();
			gf = 1 - 1.0 * countnum  / N;
			stats<<"间隙率为： "<< gf << endl;
		}
	}
	
	
	
	stats.close();
	return 0;
} 
