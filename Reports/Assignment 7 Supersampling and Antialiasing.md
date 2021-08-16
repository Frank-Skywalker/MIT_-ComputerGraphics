# Assignment 7: Supersampling and Antialiasing







## 1 代码实现



### 1.1 Sampler类



```c++
class Sampler
{
public:
	Sampler(int n) :n(n) {}
	
	virtual Vec2f getSamplePosition(int i) = 0;


protected:
	int n;
};
```



##### RandomSampler

```c++
class RandomSampler :public Sampler
{
public:
	RandomSampler(int i):Sampler(n)
	{
		srand((unsigned int)(time(NULL)));
	}
	virtual Vec2f getSamplePosition(int i)
	{
		return Vec2f((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX);
	}


};
```



##### UniformSampler

```c++
class UniformSampler : public Sampler
{
public:

	UniformSampler(int n) :Sampler(n) 
	{
		axisN = sqrtf(n);
		step = 1.0 / (float)axisN;
		offset = step / 2.0;
	}
	virtual Vec2f getSamplePosition(int i)
	{
		assert(i < n);
		int x = i % axisN;
		int y = i / axisN;
		return Vec2f(offset + x * step, offset + y * step);
	}


private:
	int axisN;
	float step;
	float offset;
};

```



##### JitteredSampler

```c++
class JitteredSampler: public Sampler
{

public:

	JitteredSampler(int n) :Sampler(n) 
	{
		srand((unsigned int)(time(NULL)));
		axisN = sqrtf(n);
		step = 1.0 / (float)axisN;
		offset = step / 2;
	}
	virtual Vec2f getSamplePosition(int i)
	{
		assert(i < n);
		int x = i % axisN;
		int y = i / axisN;
		Vec2f center(x * step, y * step);
		Vec2f random((float)rand() / (float)RAND_MAX * step, (float)rand() / (float)RAND_MAX * step);
		return Vec2f(center.x()+random.x(),center.y()+random.y());
	}

private:
	int axisN;
	float step;
	float offset;
};
```



### 1.2 Filter类



注意边界时采样不能越界

```c++
class Filter
{

public:
	Filter(){}

	Vec3f getColor(int x, int y, Film* film)
	{
		int supportRadius=getSupportRadius();
		int numSamples = film->getNumSamples();
		int width = film->getWidth();
		int height = film->getHeight();
		float totalWeight = 0;
		Vec3f color;
		for (int i = x - supportRadius; i<= x + supportRadius; i++)
		{
			if (i < 0)
			{
				continue;
			}
			else if (i >= width)
			{
				break;
			}
			for (int j = y - supportRadius; j <= y + supportRadius; j++)
			{
				if (j < 0)
				{
					continue;
				}
				else if(j>=height)
				{
					break;
				}
				for (int k = 0; k < numSamples; k++)
				{
					Sample sample=film->getSample(i, j, k);
					Vec2f samplePosition(i+ sample.getPosition().x(),j+sample.getPosition().y());
					float xoffset = samplePosition.x() - x - 0.5;
					float yoffset = samplePosition.y() - y - 0.5;
					float weight = getWeight(xoffset, yoffset);
					color+=sample.getColor() * weight;
					totalWeight += weight;
				}
			}
		}
		
		return color*(1.0/totalWeight);
	}


	virtual float getWeight(float x, float y) = 0;
	virtual int getSupportRadius() = 0;

private:
};

```



##### BoxFilter

```c++
class BoxFilter : public Filter
{
public:
	BoxFilter(float radius) :radius(radius)
	{

	}

	virtual float getWeight(float x, float y)
	{
		if (fabs(x) > radius || fabs(y) > radius)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	virtual int getSupportRadius()
	{
		return ceil(radius-0.5);
	}

private:
	float radius;
};

class TentFilter : public Filter
{

public:
	TentFilter(float radius) :radius(radius)
	{

	}
	virtual float getWeight(float x, float y)
	{
		Vec2f vec(fabs(x),fabs(y));
		return fmax(0 , 1.0 -vec.Length() / radius);
	}
	virtual int getSupportRadius()
	{
		return ceil(radius - 0.5);
	}


private:
	float radius;

};
```



##### GaussianFilter

```c++
class GaussianFilter :public Filter
{

public:
	GaussianFilter(float sigma) :sigma(sigma)
	{
	}
	virtual float getWeight(float x, float y)
	{
		Vec2f vec(fabs(x), fabs(y));
		float d = vec.Length();

		return exp(-powf(d, 2) / (2 * powf(sigma, 2)));
	}
	virtual int getSupportRadius()
	{
		return ceil(2*sigma - 0.5);
	}

private:
	float sigma;
};
```



### 1.3 修改RayTracer

修改RayCast流程以使用Sampler和Filter

```c++
	//Assignment7 
	void RayCastSample(char* outputFile)
	{
        Image outputImage(width, height);
        int xoffset = 0;
        int yoffset = 0;

        //deal with width!=height
        yoffset =max( 0, (squareLength - height) / 2);
        xoffset =max(0, (squareLength - width) / 2);

        for (int i = 0; i < width * height; i++)
        {
            int x = i % width;
            int y = i / width;
            int xrayIndex = x + xoffset;
            int yrayIndex = y + yoffset;

            //sampling
            for (int s = 0; s < numSamples; s++)
            {
                Vec2f sampleOffset=sampler->getSamplePosition(s);
                
                Hit hit;
                Ray ray = generateRayAtIndexWithOffset(xrayIndex, yrayIndex, sampleOffset);
                
                Vec3f color = (this->*tracerayFunction)(ray, scene->getCamera()->getTMin(), 0, 1, VACUUM_REFRACTION_INDEX, hit);
                //cout << "color: " << color<<endl;
                film->setSample(x, y, s, sampleOffset, color);
            }
        }

        for (int i = 0; i < width * height; i++)
        {
            int x = i % width;
            int y = i / width;
            outputImage.SetPixel(x,y,filter->getColor(x, y, film));
        }
        if(outputFile!=NULL)
            outputImage.SaveTGA(outputFile);
	}
```





## 2 实验结果



```shell
raytracer -input scene7_01_sphere_triangle.txt -size 180 180 -output output7_01.tga 
```

![image-20210816124429512](Assignment 7 Supersampling and Antialiasing.assets/image-20210816124429512.png)



```shell
raytracer -input scene7_01_sphere_triangle.txt -size 9 9 -render_samples samples7_01a.tga 20 -random_samples 4
raytracer -input scene7_01_sphere_triangle.txt -size 9 9 -render_samples samples7_01b.tga 20 -uniform_samples 4
raytracer -input scene7_01_sphere_triangle.txt -size 9 9 -render_samples samples7_01c.tga 20 -jittered_samples 4
raytracer -input scene7_01_sphere_triangle.txt -size 9 9 -render_samples samples7_01d.tga 20 -random_samples 9
raytracer -input scene7_01_sphere_triangle.txt -size 9 9 -render_samples samples7_01e.tga 20 -uniform_samples 9
raytracer -input scene7_01_sphere_triangle.txt -size 9 9 -render_samples samples7_01f.tga 20 -jittered_samples 9
raytracer -input scene7_01_sphere_triangle.txt -size 9 9 -render_samples samples7_01g.tga 20 -random_samples 36
raytracer -input scene7_01_sphere_triangle.txt -size 9 9 -render_samples samples7_01h.tga 20 -uniform_samples 36
raytracer -input scene7_01_sphere_triangle.txt -size 9 9 -render_samples samples7_01i.tga 20 -jittered_samples 36
```

![image-20210816124529085](Assignment 7 Supersampling and Antialiasing.assets/image-20210816124529085.png)

![image-20210816124645356](Assignment 7 Supersampling and Antialiasing.assets/image-20210816124645356.png)

![image-20210816124656748](Assignment 7 Supersampling and Antialiasing.assets/image-20210816124656748.png)

![image-20210816124706423](Assignment 7 Supersampling and Antialiasing.assets/image-20210816124706423.png)

![image-20210816124720647](Assignment 7 Supersampling and Antialiasing.assets/image-20210816124720647.png)

![image-20210816124735720](Assignment 7 Supersampling and Antialiasing.assets/image-20210816124735720.png)

![image-20210816124752286](Assignment 7 Supersampling and Antialiasing.assets/image-20210816124752286.png)

![image-20210816124804223](Assignment 7 Supersampling and Antialiasing.assets/image-20210816124804223.png)

![image-20210816124821982](Assignment 7 Supersampling and Antialiasing.assets/image-20210816124821982.png)



```shell
raytracer -input scene7_01_sphere_triangle.txt -size 9 9 -render_filter filter7_01a.tga 20 -box_filter 0.5
raytracer -input scene7_01_sphere_triangle.txt -size 9 9 -render_filter filter7_01b.tga 20 -tent_filter 0.5
raytracer -input scene7_01_sphere_triangle.txt -size 9 9 -render_filter filter7_01c.tga 20 -gaussian_filter 0.5
raytracer -input scene7_01_sphere_triangle.txt -size 9 9 -render_filter filter7_01d.tga 20 -box_filter 1.7
raytracer -input scene7_01_sphere_triangle.txt -size 9 9 -render_filter filter7_01e.tga 20 -tent_filter 1.7
raytracer -input scene7_01_sphere_triangle.txt -size 9 9 -render_filter filter7_01f.tga 20 -gaussian_filter 1.7
raytracer -input scene7_01_sphere_triangle.txt -size 9 9 -render_filter filter7_01g.tga 20 -box_filter 2.3
raytracer -input scene7_01_sphere_triangle.txt -size 9 9 -render_filter filter7_01h.tga 20 -tent_filter 2.3
raytracer -input scene7_01_sphere_triangle.txt -size 9 9 -render_filter filter7_01i.tga 20 -gaussian_filter 2.3
```

![image-20210816125244130](Assignment 7 Supersampling and Antialiasing.assets/image-20210816125244130.png)

![image-20210816125300393](Assignment 7 Supersampling and Antialiasing.assets/image-20210816125300393.png)

![image-20210816125313368](Assignment 7 Supersampling and Antialiasing.assets/image-20210816125313368.png)

![image-20210816125323481](Assignment 7 Supersampling and Antialiasing.assets/image-20210816125323481.png)

![image-20210816125338112](Assignment 7 Supersampling and Antialiasing.assets/image-20210816125338112.png)

![image-20210816125353132](Assignment 7 Supersampling and Antialiasing.assets/image-20210816125353132.png)

![image-20210816125409715](Assignment 7 Supersampling and Antialiasing.assets/image-20210816125409715.png)

![image-20210816125421348](Assignment 7 Supersampling and Antialiasing.assets/image-20210816125421348.png)

![image-20210816125448539](Assignment 7 Supersampling and Antialiasing.assets/image-20210816125448539.png)



```shell
raytracer -input scene7_01_sphere_triangle.txt -size 12 12 -output output7_01_low_res.tga
raytracer -input scene7_01_sphere_triangle.txt -size 12 12 -render_samples samples7_01a_low_res.tga 15 -random_samples 9 
raytracer -input scene7_01_sphere_triangle.txt -size 12 12 -render_samples samples7_01b_low_res.tga 15 -uniform_samples 9
raytracer -input scene7_01_sphere_triangle.txt -size 12 12 -render_samples samples7_01c_low_res.tga 15 -jittered_samples 9
raytracer -input scene7_01_sphere_triangle.txt -size 12 12 -render_filter filter7_01a_low_res.tga 15 -box_filter 0.5
raytracer -input scene7_01_sphere_triangle.txt -size 12 12 -render_filter filter7_01b_low_res.tga 15 -tent_filter 1.5
raytracer -input scene7_01_sphere_triangle.txt -size 12 12 -render_filter filter7_01c_low_res.tga 15 -gaussian_filter 1.0
raytracer -input scene7_01_sphere_triangle.txt -size 12 12 -output output7_01a_low_res.tga -random_samples 9 -box_filter 0.5
raytracer -input scene7_01_sphere_triangle.txt -size 12 12 -output output7_01b_low_res.tga -random_samples 9 -tent_filter 1.5
raytracer -input scene7_01_sphere_triangle.txt -size 12 12 -output output7_01c_low_res.tga -random_samples 9 -gaussian_filter 1.0
raytracer -input scene7_01_sphere_triangle.txt -size 12 12 -output output7_01d_low_res.tga -uniform_samples 9 -box_filter 0.5
raytracer -input scene7_01_sphere_triangle.txt -size 12 12 -output output7_01e_low_res.tga -uniform_samples 9 -tent_filter 1.5
raytracer -input scene7_01_sphere_triangle.txt -size 12 12 -output output7_01f_low_res.tga -uniform_samples 9 -gaussian_filter 1.0
raytracer -input scene7_01_sphere_triangle.txt -size 12 12 -output output7_01g_low_res.tga -jittered_samples 9 -box_filter 0.5
raytracer -input scene7_01_sphere_triangle.txt -size 12 12 -output output7_01h_low_res.tga -jittered_samples 9 -tent_filter 1.5
raytracer -input scene7_01_sphere_triangle.txt -size 12 12 -output output7_01i_low_res.tga -jittered_samples 9 -gaussian_filter 1.0
```

![image-20210816125526853](Assignment 7 Supersampling and Antialiasing.assets/image-20210816125526853.png)

![image-20210816125540344](Assignment 7 Supersampling and Antialiasing.assets/image-20210816125540344.png)

![image-20210816125742073](Assignment 7 Supersampling and Antialiasing.assets/image-20210816125742073.png)

![image-20210816125753168](Assignment 7 Supersampling and Antialiasing.assets/image-20210816125753168.png)



![image-20210816125908409](Assignment 7 Supersampling and Antialiasing.assets/image-20210816125908409.png)

![image-20210816125936837](Assignment 7 Supersampling and Antialiasing.assets/image-20210816125936837.png)

![image-20210816125950722](Assignment 7 Supersampling and Antialiasing.assets/image-20210816125950722.png)

![image-20210816130003639](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130003639.png)



![image-20210816130035001](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130035001.png)

![image-20210816130118501](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130118501.png)

![image-20210816130128847](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130128847.png)

![image-20210816130140502](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130140502.png)



![image-20210816130205216](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130205216.png)

![image-20210816130219630](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130219630.png)

![image-20210816130230181](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130230181.png)

![image-20210816130240627](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130240627.png)



```shell
raytracer -input scene7_02_checkerboard.txt -size 180 180 -output output7_02.tga 
raytracer -input scene7_02_checkerboard.txt -size 9 9 -render_samples samples7_02a.tga 20 -random_samples 16
raytracer -input scene7_02_checkerboard.txt -size 9 9 -render_samples samples7_02b.tga 20 -uniform_samples 16
raytracer -input scene7_02_checkerboard.txt -size 9 9 -render_samples samples7_02c.tga 20 -jittered_samples 16
raytracer -input scene7_02_checkerboard.txt -size 9 9 -render_filter filter7_02a.tga 20 -box_filter 0.5
raytracer -input scene7_02_checkerboard.txt -size 9 9 -render_filter filter7_02b.tga 20 -tent_filter 1.5
raytracer -input scene7_02_checkerboard.txt -size 9 9 -render_filter filter7_02c.tga 20 -gaussian_filter 0.6
raytracer -input scene7_02_checkerboard.txt -size 180 180 -output output7_02a.tga -random_samples 16 -box_filter 0.5
raytracer -input scene7_02_checkerboard.txt -size 180 180 -output output7_02b.tga -random_samples 16 -tent_filter 1.5
raytracer -input scene7_02_checkerboard.txt -size 180 180 -output output7_02c.tga -random_samples 16 -gaussian_filter 0.6
raytracer -input scene7_02_checkerboard.txt -size 180 180 -output output7_02d.tga -uniform_samples 16 -box_filter 0.5
raytracer -input scene7_02_checkerboard.txt -size 180 180 -output output7_02e.tga -uniform_samples 16 -tent_filter 1.5
raytracer -input scene7_02_checkerboard.txt -size 180 180 -output output7_02f.tga -uniform_samples 16 -gaussian_filter 0.6
raytracer -input scene7_02_checkerboard.txt -size 180 180 -output output7_02g.tga -jittered_samples 16 -box_filter 0.5
raytracer -input scene7_02_checkerboard.txt -size 180 180 -output output7_02h.tga -jittered_samples 16 -tent_filter 1.5
raytracer -input scene7_02_checkerboard.txt -size 180 180 -output output7_02i.tga -jittered_samples 16 -gaussian_filter 0.6
```

![image-20210816130319714](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130319714.png)

![image-20210816130338105](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130338105.png)

![image-20210816130348768](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130348768.png)

![image-20210816130357566](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130357566.png)



![image-20210816130419961](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130419961.png)

![image-20210816130440055](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130440055.png)

![image-20210816130456655](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130456655.png)

![image-20210816130508223](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130508223.png)



![image-20210816130526687](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130526687.png)

![image-20210816130549408](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130549408.png)

![image-20210816130600254](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130600254.png)

![image-20210816130610398](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130610398.png)



![image-20210816130624663](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130624663.png)

![image-20210816130641674](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130641674.png)

![image-20210816130651603](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130651603.png)

![image-20210816130700555](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130700555.png)



```shell
raytracer -input scene7_03_marble_vase.txt  -size 200 200 -output output7_03a.tga -grid 15 30 15 -shadows
raytracer -input scene7_03_marble_vase.txt  -size 200 200 -output output7_03b.tga -grid 15 30 15 -shadows -jittered_samples 4 -gaussian_filter 0.4
raytracer -input scene7_03_marble_vase.txt  -size 200 200 -output output7_03c.tga -grid 15 30 15 -shadows -jittered_samples 9 -gaussian_filter 0.4
raytracer -input scene7_03_marble_vase.txt  -size 200 200 -output output7_03d.tga -grid 15 30 15 -shadows -jittered_samples 36 -gaussian_filter 0.4
```

![image-20210816130745767](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130745767.png)

![image-20210816130756763](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130756763.png)

![image-20210816130806066](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130806066.png)

![image-20210816130818085](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130818085.png)



```shell
raytracer -input scene7_04_6.837_logo.txt   -size 400 200 -output output7_04a.tga -shadows -shade_back -bounces 5 -weight 0.01 -grid 80 30 3
raytracer -input scene7_04_6.837_logo.txt   -size 400 200 -output output7_04b.tga -shadows -shade_back -bounces 5 -weight 0.01 -grid 80 30 3 -jittered_samples 9 -gaussian_filter 0.4
```

![image-20210816130842348](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130842348.png)

![image-20210816130851948](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130851948.png)



```shell
raytracer -input scene7_05_glass_sphere.txt -size 300 300 -output output7_05a.tga -shadows -shade_back -bounces 5 -weight 0.01 -grid 20 20 20
raytracer -input scene7_05_glass_sphere.txt -size 300 300 -output output7_05b.tga -shadows -shade_back -bounces 5 -weight 0.01 -grid 20 20 20 -jittered_samples 4 -gaussian_filter 0.4
raytracer -input scene7_05_glass_sphere.txt -size 300 300 -output output7_05c.tga -shadows -shade_back -bounces 5 -weight 0.01 -grid 20 20 20 -jittered_samples 16 -gaussian_filter 0.4
```

![image-20210816130916144](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130916144.png)

![image-20210816130927711](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130927711.png)

![image-20210816130937413](Assignment 7 Supersampling and Antialiasing.assets/image-20210816130937413.png)



```shell
raytracer -input scene7_06_faceted_gem.txt -size 200 200 -output output7_06a.tga -shadows -shade_back -bounces 5 -weight 0.01 -grid 20 20 20 
raytracer -input scene7_06_faceted_gem.txt -size 200 200 -output output7_06b.tga -shadows -shade_back -bounces 5 -weight 0.01 -grid 20 20 20 -jittered_samples 9 -gaussian_filter 0.4
```

![image-20210816131005427](Assignment 7 Supersampling and Antialiasing.assets/image-20210816131005427.png)

![image-20210816131015654](Assignment 7 Supersampling and Antialiasing.assets/image-20210816131015654.png)

