#include<iostream>
#include<string>
#include<algorithm>//for sort
using namespace std;

#define X_MAX 512
#define Y_MAX 512


unsigned char** mem_alloc2_d(int n_height, int n_width, unsigned char nlnit_val);

unsigned char** mem_alloc2_d(int n_height, int n_width, unsigned char nlnit_val)
{
	unsigned char** rtn = new unsigned char*[n_height];
	for (int n = 0; n < n_height; n++)
	{
		rtn[n] = new unsigned char[n_width];
		memset(rtn[n], nlnit_val, sizeof(unsigned char)*n_width);
	}
	return rtn;
}

unsigned char** padding(unsigned char** in, int n_height, int n_width, int n_filter_size)
{
	int n_pad_size = (int)(n_filter_size / 2);
	unsigned char** pad = mem_alloc2_d(n_height + 2 * n_pad_size, n_width + 2 * n_pad_size, 0);

	for (int h = 0; h < n_height; h++)
	{
		for (int w = 0; w < n_width; w++)
		{
			pad[h + n_pad_size][w + n_pad_size] = in[h][w];
		}
	}

	for (int h = 0; h < n_pad_size; h++)
	{
		for (int w = 0; w < n_width; w++)
		{
			pad[h][w + n_pad_size] = in[0][w];
			pad[h + (n_height - 1)][w + n_pad_size] = in[n_height - 1][w];
		}
	}

	for (int h = 0; h < n_height; h++)
	{
		for (int w = 0; w < n_pad_size; w++)
		{
			pad[h + n_pad_size][w] = in[h][0];
			pad[h + n_pad_size][w + (n_width - 1)] = in[h][n_width - 1];
		}
	}

	for (int h = 0; h < n_pad_size; h++)
	{
		for (int w = 0; w < n_pad_size; w++)
		{
			pad[h][w] = in[0][0];
			pad[h + (n_height - 1)][w] = in[n_height - 1][0];
			pad[h][w + (n_width - 1)] = in[0][n_width - 1];
			pad[h + (n_height - 1)][w + (n_width - 1)] = in[n_height - 1][n_width - 1];
		}
	}

	return pad;
}

typedef struct _RGB {
	unsigned char r;//받을때 bgr순서라서 이렇게 함
	unsigned char g;
	unsigned char b;

} RGB;
int readBMP(unsigned char** r, unsigned char** g, unsigned char** b, int nHeight_in, int nWidth_in, string file_name)
{

	FILE* in = fopen(file_name.c_str(), "rb");

	unsigned char* header = (unsigned char*)malloc(sizeof(unsigned char) * 54);
	fread(header, 54, sizeof(unsigned char), in);

	RGB** rgb = new RGB*[nHeight_in];

	for (int n = 0; n < nHeight_in; n++)
	{
		rgb[n] = new RGB[nWidth_in];
		memset(rgb[n], 0, sizeof(RGB) * nWidth_in);
	}
	for (int i = nHeight_in - 1; i >= 0; i--)//BMP는 영상도 거꾸로 입력 해주어야 하네
	{
		fread(rgb[i], sizeof(RGB), nWidth_in, in);
	}
	fclose(in);
	for (int h = 0; h < nHeight_in; h++)

	{
		for (int w = 0; w < nWidth_in; w++)

		{
			r[h][w] = rgb[h][w].b;//bit맵은 역순이라서 이런식으로 넣어주어야 한다.
			g[h][w] = rgb[h][w].g;
			b[h][w] = rgb[h][w].r;

		}
	}
	return 0;
}
int writeRGB(unsigned char** r, unsigned char** g, unsigned char** b, int nHeight_in, int nWidth_in, string file_name)
{
	RGB** out = new RGB*[nHeight_in];
	for (int n = 0; n < nHeight_in; n++)
	{
		out[n] = new RGB[nWidth_in];
		memset(out[n], 0, sizeof(RGB) * nWidth_in);
	}
	for (int h = 0; h < nHeight_in; h++)

	{
		for (int w = 0; w < nWidth_in; w++)

		{
			out[h][w].b = b[h][w];
			out[h][w].r = r[h][w];
			out[h][w].g = g[h][w];

		}
	}
	FILE* outfile = fopen(file_name.c_str(), "w+b");
	/*for (int i = 0; i < nHeight_in; i++)
	{
		fwrite(out[i], sizeof(RGB), nWidth_in, outfile);
	}*/
	for (int i = 0; i < nHeight_in; i++)
	{
		fwrite(out[i], sizeof(RGB), nWidth_in, outfile);
	}
	return 0;


}
int readBMPheader(int &nHeight_in, int &nWidth_in, string file_name)
{
	FILE* in = fopen(file_name.c_str(), "rb");
	unsigned char info[54];

	if (in == NULL)
	{
		printf("File not found!!\n");
		exit(1);
		return 1;
	}
	fread(info, sizeof(unsigned char), 54, in); // read the 54-byte header

	nWidth_in = *(int*)&info[18];
	nHeight_in = *(int*)&info[22];
}
int WhiteBalancing(unsigned char** r, unsigned char** g, unsigned char** b, int nHeight_in, int nWidth_in)
{
	int r_value = 190;
	int g_value = 142;
	int b_value = 66;
	int radius = 80;
	for (int h = 0; h < nHeight_in; h++)
	{
		for (int w = 0; w < nWidth_in; w++)
		{
			if (r[h][w] <= 190)
			{
				r[h][w] = (r[h][w])*255.0 / 190.0;
			}
			else {
				r[h][w] = 255;

			}
			if (r[h][w] > 255)
				r[h][w] = 255;
			else if (r[h][w] <= 0)
			{
				r[h][w] = 0;

			}
			if (g[h][w] <= 142)
			{
				g[h][w] = (g[h][w])*255.0 / 142.0;
			}
			else {
				g[h][w] = 255;

			}
			if (g[h][w] > 255)
				g[h][w] = 255;
			else if (g[h][w] <= 0)
			{
				g[h][w] = 0;

			}
			if (b[h][w] <= 66)
			{
				b[h][w] = (b[h][w])*255.0 / 66.0;
			}
			else {
				b[h][w] = 255;

			}
			if (b[h][w] > 255)
				b[h][w] = 255;
			else if (g[h][w] <= 0)
			{
				g[h][w] = 0;

			}
		}
	}
	return 0;
}
int main(void)
{
	int nHeight_in = 0;
	int nWidth_in = 0;
	string filename = "input2.bmp";
	readBMPheader(nWidth_in, nHeight_in, filename);
	unsigned char**ch_in_r = mem_alloc2_d(nHeight_in, nWidth_in, 0);//rgb가 아니라 bgr로 들어오는것 같기도 하다. 한번 알아보고 해야겠다.
	unsigned char**ch_in_g = mem_alloc2_d(nHeight_in, nWidth_in, 0);
	unsigned char**ch_in_b = mem_alloc2_d(nHeight_in, nWidth_in, 0);

	readBMP(ch_in_r, ch_in_g, ch_in_b, nHeight_in, nWidth_in, filename);
	WhiteBalancing(ch_in_r, ch_in_g, ch_in_b, nHeight_in, nWidth_in);

	writeRGB(ch_in_r, ch_in_g, ch_in_b, nHeight_in, nWidth_in, "out1.raw");




	return 0;
}