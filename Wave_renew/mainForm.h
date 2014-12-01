#pragma once

#include<cmath>
#include<cstdio>
#include<iostream>
#include<Windows.h>
#include<WinUser.h>
#include<winstring.h>

#include"defines.h"
#include"ViewForm.h"
#include <msclr/marshal.h>

using namespace std;
using namespace System::Drawing;
using namespace msclr::interop;

namespace Wave_renew
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	
	public ref class mainForm : public System::Windows::Forms::Form
	{

	private:
		System::Windows::Forms::Label^  label1;
		System::Windows::Forms::Label^  label2;
		System::Windows::Forms::Label^  label3;
		System::Windows::Forms::Label^  label4;
		System::Windows::Forms::Label^  label5;
		System::Windows::Forms::Label^  label6;
		System::Windows::Forms::Label^  label7;
		System::Windows::Forms::Label^  label8;
		System::Windows::Forms::Label^  label9;
		System::Windows::Forms::Label^  label10;
		System::Windows::Forms::Label^  label11;

		System::Windows::Forms::TextBox^  textBox_rangeX_start;
		System::Windows::Forms::TextBox^  textBox_rangeY_start;
		System::Windows::Forms::TextBox^  textBox_rangeX_end;
		System::Windows::Forms::TextBox^  textBox_rangeY_end;
		System::Windows::Forms::TextBox^  textBox_sizeX;
		System::Windows::Forms::TextBox^  textBox_sizeY;
		System::Windows::Forms::TextBox^  textBox_dx;
		System::Windows::Forms::TextBox^  textBox_dy;
		System::Windows::Forms::TextBox^  textBox_outTime;
		System::Windows::Forms::TextBox^  textBox_calcTime;
		System::Windows::Forms::TextBox^  textBox_isobath;

		System::Windows::Forms::Button^  button_applyParameters;
		System::Windows::Forms::Button^  button_startCalc;
		System::Windows::Forms::Button^  button_pauseCalc;
		System::Windows::Forms::Button^  button_stopCalc;

		System::Windows::Forms::ToolStripMenuItem^  ToolStrip_file;
		System::Windows::Forms::ToolStripMenuItem^  ToolStrip_file_openMap;
		System::Windows::Forms::ToolStripMenuItem^  ToolStrip_file_openConfig;

		System::ComponentModel::Container ^components;
		System::Windows::Forms::MenuStrip^  menuStrip_main;
		System::Windows::Forms::CheckBox^  checkBox_autoSaveLayers;

		ViewForm^ vf;
		Graphics^ mainGraphics;
		Bitmap^ mainBitmap;
		String^ mapFileName;

	public:
		mainForm(void)
		{
			InitializeComponent();
			scaling = 2.0;
		}

	protected:
		~mainForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		double** new_d(int _y, int _x)
		{
			double **_m = new double *[_y + 1];
			if (!_m) 
				return 0;
			for (int j = 0; j < _y; j++)
				if (!(_m[j] = new double[_x])) 
				{
					for (; j >= 0; j--) 
						delete _m[j];
					return 0;
				}
			_m[_y] = NULL;
			return _m;
		}

		void delete_d(double **_m)
		{
			for (int j = 0; _m[j] != NULL; j++)
				delete _m[j];
			delete _m;
		}

		int copy_d(double **_dest, double **_src, int _y, int _x)
		{
			for (int j = 0; j < _y; j++)
				if (!memcpy(_dest[j], _src[j], _x*sizeof(double)))
					return 1;
			return 0;
		}

		void swap_d(double ***_m1, double ***_m2)
		{
			double **tmp = *_m1;
			*_m1 = *_m2;
			*_m2 = tmp;
		}

		Color bottom2col(double h)
		{
			const int h_max = 80; // = sqrt(5000)
			const int h_min = -110;// = -sqrt(10000)
			if (h>0) 
				h = sqrt(h); 
			else 
				if (h < 0) 
					h = -sqrt(-h);
			if (h >= 0) 
				return Color::FromArgb(0, 55 + h * 200 / h_max, 0);
			else 
				return Color::FromArgb(0, 0, 255 - h * 255 / h_min);
		}

		Color eta2col(double h)
		{
			const int h_max = 72; // = sqrt(5000)
			const int h_minmax = 10; // = sqrt(25)
			const int h_maxmax = 15; // = sqrt(25)
			if (h >= LAND_UP)
			{
				h = sqrt(h - LAND_UP);
				return Color::FromArgb(0, 55 + h * 200 / h_max, 0);
			}
			else 
				if (h <= h_maxmax)
				{
					if (h > h_minmax + 2)
						return Color::FromArgb(161, 19, 33);
					if (h > h_minmax)
						return Color::FromArgb(200, 0, 200);
					if (h > h_minmax - 2)
						return Color::FromArgb(120, 100, 200);
					if (h > h_minmax - 3)
						return Color::FromArgb(25, 25, 225);
					if (h > h_minmax - 4)
						return Color::FromArgb(150, 150, 250);
					if (h > h_minmax - 5)
						return Color::FromArgb(100, 220, 220);
					if (h > h_minmax - 6)
						return Color::FromArgb(200, 220, 220);
					if (h > h_minmax - 6.5)
						return Color::FromArgb(100, 255, 100);
					if (h > h_minmax - 7)
						return Color::FromArgb(133, 27, 181);
					if (h > h_minmax - 7.5)
						return Color::FromArgb(250, 250, 0);
					if (h > h_minmax - 8)
						return Color::FromArgb(255, 181, 41);
					if (h > h_minmax - 8.5)
						return Color::FromArgb(255, 200, 200);
					if (h > h_minmax - 9)
						return Color::FromArgb(255, 0, 0);
					if (h > h_minmax - 9.5)
						return Color::FromArgb(255, 100, 0);
					if (h > h_minmax - 9.99)
						return Color::FromArgb(166, 249, 45);
					else 
						if (h > -h_maxmax)
							return Color::FromArgb(0, 0, 70 + h * 70 / h_maxmax);
						else
							return Color::FromArgb(125, 25, 225);//RGB(0, 0, 0);
				}
				else
				{
					h = sqrt(h);
					return Color::FromArgb(0, h * 255 / h_max / 2, 255 - h * 255 / h_max / 2);
				}
		}

		Color h2col(double h)
		{
			const int h_max = 71; // = sqrt(5000)
			const int h_minmax = 10;// = sqrt(400)
			if (h > LAND_UP)
			{
				h = sqrt(h - LAND_UP);
				return Color::FromArgb(0, 55 + h * 200 / h_max, 0);
			}
			else
			{
				if (h > 0) h = sqrt(h); else if (h < 0) h = -sqrt(-h);
				return Color::FromArgb(0, 0, 127 + h * 127 / h_minmax);
			}
			//   return (Color) RGB(0, 0, 0);;
		}

		int show_d(double **m, int size_y, int size_x, int scaling)
		{
			if (!m) 
				return 1;

			/*is needed?
			if (running)
				pBitmap->Width = floor(size_x / scaling) + 2 * SCALE_WIDTH;
			else
				pBitmap->Width = floor(size_x / scaling);
			pBitmap->Height = floor(size_y / scaling);
			*/

			//Drawing
			//******************************************************************************
			for (int y = 0; y < floor(size_y / scaling); y++)
				for (int x = 0; x < floor(size_x / scaling); x++)
				{
					double h = 0;
					for (int sy = 0; sy < scaling; sy++)
						for (int sx = 0; sx < scaling; sx++)
							h += m[scaling*y + sy][scaling*x + sx];
					this->mainBitmap->SetPixel(x, floor(size_y / scaling) - 1 - y, bottom2col(h / (scaling*scaling)));

					/* deprecated? is needed?
					Application->ProcessMessages();
					*/
				}
			//******************************************************************************

			for (int longitude = System::Convert::ToInt32(this->textBox_rangeY_end->Text); longitude > System::Convert::ToInt32(this->textBox_rangeY_start->Text); longitude--)
			{
				/* is needed?
				pBitmap->Canvas->Font->Color = Color::Black;
				*/

				/* find similar */
				//pBitmap->Canvas->TextOut( 0, (StrToInt(Form1->EndY->Text) - longitude) * 60 / scaling, \
											Format("%d", OPENARRAY(TVarRec, (longitude))) );
			}
			for (int latitude = System::Convert::ToInt32(this->textBox_rangeX_start->Text); latitude <= System::Convert::ToInt32(this->textBox_rangeX_end->Text); latitude++)
			{
				/* is needed?
				pBitmap->Canvas->Font->Color = Color::Black;
				*/

				/* find similar */
				//pBitmap->Canvas->TextOut((latitude - System::Convert::ToInt32(Form1->StartX->Text)) * 60 / scaling, (size_x / scaling - 14), \
											Format("%d", OPENARRAY(TVarRec, (latitude))) );
			}
			return 0;
		}

		/* is needed?
		void form_prepare()
		{
			scaling = System::Convert::ToInt32(Form1->ScalingX->Text);
			if (scaling < 1 || scaling > 30) return;
			if (running)
				Form2->ClientWidth = floor(size_x / scaling) > MAX_FORM_SIZE ?
			MAX_FORM_SIZE :
						  floor(size_x / scaling) + 2 * SCALE_WIDTH;
			else
				Form2->ClientWidth = floor(size_x / scaling) > MAX_FORM_SIZE ?
			MAX_FORM_SIZE :
						  floor(size_x / scaling);

			Form2->ClientHeight = floor(size_y / scaling) > MAX_FORM_SIZE ?
			MAX_FORM_SIZE :
						  floor(size_y / scaling);
			//   offset_x = 0, offset_y = 0;
			Form2->Invalidate(); // For immediately form repaint after it's shows
		}
		*/

		void showBottom()
		{
			show_d(bottom, size_y, size_x, scaling);
		}

		void showDisturbance()
		{
			if (!eta) return;
			/* is needed?
			form_prepare();
			*/
			double **a = new_d(size_y, size_x);
			for (int j = 0; j < size_y; j++)
				for (int i = 0; i < size_x; i++)
					if (bottom[j][i] < 0)
						a[j][i] = eta[j][i];
					else
						a[j][i] = bottom[j][i] + LAND_UP;

			if (show_d(a, size_y, size_x, scaling))
				vf->Hide();
			else
				vf->Show();

			delete_d(a);
			if (!running)
				vf->pictureBox_main->Update();
		}

		void ShowHeights()
		{
			if (!h) return;
			/* if needed
			form_prepare();
			*/
			double **a = new_d(size_y, size_x);
			for (int j = 0; j < size_y; j++)
				for (int i = 0; i < size_x; i++)
					if (bottom[j][i] < 0)
						if (bottom[j][i] < 0.0)
						{
							a[j][i] = visota[j][i];
						}
						else { ; a[j][i] = 0; }
					else
						a[j][i] = bottom[j][i] + LAND_UP;

			if (show_d(a, size_y, size_x, scaling))
				vf->Hide();
			else
				vf->Show();

			delete_d(a);
			if (!running)
				vf->pictureBox_main->Update();
		}

		bool loadMap()
		{
			FILE* infile;

			/* fopen( , "rt") ??? */
			marshal_context ^ context = gcnew marshal_context();
			const char* tmpFileName = context->marshal_as<const char*>(mapFileName);
					 
			if ((infile = fopen(tmpFileName, "r")) == NULL)
			{
				MessageBox::Show("Input file not found!", "Error!", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return false;
			}
			delete context;

			const int param_cnt = 8;
			int param[param_cnt];
			array<String^>^ sparam = gcnew array<String^>(param_cnt) { "version", "size_x", "size_y", "start_x", "end_x", "start_y", "end_y", "data:"};

			char cs[MAX_STR_LEN];

			for (int p = 0; p < param_cnt; p++)
			{
				std::cout << p << std::endl;
				fgets(cs, MAX_STR_LEN, infile);
				String^ s = gcnew String(cs);
				if (s->ToLower()->IndexOf(sparam[p]) == -1)
				{
					MessageBox::Show("Wrong file format!", "Error!", MessageBoxButtons::OK, MessageBoxIcon::Error);
					return false;
				}
				int pos = s->IndexOf("=");
				//if it is not a "data:"
				if (p != 7)
					param[p] = System::Convert::ToInt32(s->Substring(pos + 1, s->Length - pos - 1)->Trim());
			}

			//version = param[0];
			size_x = param[1];
			size_y = param[2];
			start_x = param[3];
			end_x = param[4];
			start_y = param[5];
			end_y = param[6];

			if (bottom)
				delete_d(bottom);
			bottom = new_d(size_y, size_x);

			int _deep;
			for (int y = 0; y < size_y; y++)
				for (int x = 0; x < size_x; x++)
				{
					if (fscanf(infile, "%i", &_deep))
						bottom[y][x] = (double)_deep; // Terrain H coordinates
					else
					{
						MessageBox::Show("Wrong file format!", "Error!", MessageBoxButtons::OK, MessageBoxIcon::Error);
						return false;
					};
				}

			this->textBox_rangeX_start->Text = System::Convert::ToString(start_x);
			this->textBox_rangeX_end->Text = System::Convert::ToString(end_x);
			this->textBox_rangeY_start->Text = System::Convert::ToString(start_y);
			this->textBox_rangeY_end->Text = System::Convert::ToString(end_y);
			this->textBox_sizeX->Text = System::Convert::ToString(size_x);
			this->textBox_sizeY->Text = System::Convert::ToString(size_y);

			/* it is still needed?
			izobata = System::Convert::ToDouble(this->textBox_isobath->Text);
			calc_time = System::Convert::ToDouble(this->textBox_calcTime->Text);
			*/
			delta_x = (end_x - start_x) / (size_x - 1);
			delta_y = (end_y - start_y) / (size_y - 1);
			this->textBox_dx->Text = System::Convert::ToString(delta_x);
			this->textBox_dy->Text = System::Convert::ToString(delta_y);

			fclose(infile);

			MessageBox::Show("File Loading Completed!", "Information!", MessageBoxButtons::OK, MessageBoxIcon::Information);
			this->button_startCalc->Enabled = true;

			return true;
		}

		void SaveBmpClick(int time)
		{
			mainBitmap->Save("output_t" + System::Convert::ToString(time) + ".bmp");
		}

		void fill_tetragon(int** terr, int v,
			int mini, int minj, int maxi, int maxj,
			int i1, int j1,
			int i2, int j2,
			int i3, int j3,
			int i4, int j4)
		{
			int moving = abs(i1 - i2) + abs(i2 - i3) + abs(i3 - i4) + abs(i4 - i1) +
				abs(j1 - j2) + abs(j2 - j3) + abs(j3 - j4) + abs(j4 - j1);
			double x, y;
			int promi, promj;

			for (x = 0; x <= 1; x = x + 1.0 / (double)(moving))
			{
				for (y = 0; y <= 1; y = y + 1.0 / (double)(moving))
				{
					promi = int(x*(i1*y + i2*(1 - y)) + (1 - x)*(i3*y + i4*(1 - y)) - 0.5);
					promj = int(x*(j1*y + j2*(1 - y)) + (1 - x)*(j3*y + j4*(1 - y)) - 0.5);
					if (promi >= 0 && promj >= 0 && promi <= size_x - 2 && promj <= size_y - 2)terr[promj][promi] = v;
				}

			}
		}

		int processing()
		{
			int old_x = 0;
			int old_y = 0;
			running = true;
			if (h) 
				delete_d(h);
			if (eta) 
				delete_d(eta); // don't forgot remove it, if eta can be loaded independently
			if (eta_old) 
				delete_d(eta_old); // don't forgot remove it, if eta can be loaded independently
			if (eta_new) 
				delete_d(eta_new);
			if (visota) 
				delete_d(visota);
			if (u) 
				delete_d(u);
			if (v) 
				delete_d(v);
			if (u_old) 
				delete_d(u_old);
			if (v_old) 
				delete_d(v_old);

			h = new_d(size_y, size_x);
			eta = new_d(size_y, size_x); // don't forgot remove it, if eta can be loaded independently
			eta_old = new_d(size_y, size_x); // don't forgot remove it, if eta_old can be loaded independently   visota = new_d(size_y, size_x);
			eta_new = new_d(size_y, size_x);
			u = new_d(size_y, size_x);
			v = new_d(size_y, size_x);
			u_old = new_d(size_y, size_x);
			v_old = new_d(size_y, size_x);
			visota = new_d(size_y, size_x);
			if (!(bottom || h || eta || eta_old || visota || u || v || u_old || v_old)) // matrix not initialized
				return 2;

			if (delta_y_m)	
				delete delta_y_m;
			delta_y_m = new double[size_y];
			if (delta_t) 
				delete delta_t;
			delta_t = new double[size_y];

			if (terr_points) 
				delete terr_points;
			terr_points = new_d(8, terr_cnt);

			if (point_points) 
				delete point_points;
			point_points = new_d(2, point_cnt);

			if (t_h_v_up) 
				delete t_h_v_up;
			t_h_v_up = new_d(8, terr_cnt);

			if (terr_up)
			{
				for (int j = 0; terr_up[j] != NULL; j++) 
					delete terr_up[j];
				delete terr_up;
			}
			terr_up = new int *[size_y + 1]; 
			for (int j = 0; j<size_y; j++) 
				terr_up[j] = new int[size_x]; 
			terr_up[size_y] = NULL;

			time_moments = System::Convert::ToInt32(this->textBox_calcTime->Text);
			output_moments = System::Convert::ToInt32(this->textBox_outTime->Text);
			file_count = 1;

			//****************************************************************************
			//first problem
			for (int y = 0; y<size_y; y++) 
			{
				for (int x = 0; x<size_x; x++) 
				{
					if (bottom[y][x] <= 0 && bottom[y][x] >= izobata) h[y][x] = izobata;
					else if (bottom[y][x] >= 0) h[y][x] = -1;   //for correct sqrt(-h[j][i]);
					else h[y][x] = bottom[y][x];
					eta_old[y][x] = 0;
					u_old[y][x] = 0;
					v_old[y][x] = 0;
					terr_up[y][x] = 0;
					if (h_max > bottom[y][x]) 
						h_max = bottom[y][x]; // find deeper place
				}
			}
			//*****************************************************************************

			for (int j = 0; j<terr_cnt; j++)
			{
				t_h_v_up[0][j] = System::Convert::ToDouble(terr_tmp[j + 1][1]);
				t_h_v_up[1][j] = System::Convert::ToDouble(terr_tmp[j + 1][2]);
				t_h_v_up[2][j] = t_h_v_up[1][j] / t_h_v_up[0][j];
				for (int i = 0; i<8; i++)
					terr_points[i][j] = System::Convert::ToDouble(terr_tmp[j + 1][i + 3]);
				fill_tetragon(terr_up, j + 1,
					0, 0, size_x - 1, size_y - 1,
					(terr_points[0][j] - start_x) / delta_x + 0.5, (terr_points[1][j] - start_y) / delta_y + 0.5,
					(terr_points[2][j] - start_x) / delta_x + 0.5, (terr_points[3][j] - start_y) / delta_y + 0.5,
					(terr_points[4][j] - start_x) / delta_x + 0.5, (terr_points[5][j] - start_y) / delta_y + 0.5,
					(terr_points[6][j] - start_x) / delta_x + 0.5, (terr_points[7][j] - start_y) / delta_y + 0.5
					);
			}

			delta_x_m = delta_x*M_PI * 6365500 / 180;

			//***************************************************************************
			//Second problem
			for (int j = 0; j<size_y; j++)
			{
				delta_y_m[j] = delta_x_m*cos((start_y + j*delta_y) / 180.0*M_PI);
				delta_t[j] = 1;//delta_x_m*delta_y_m[j]/sqrt(-M_G*h_max*(delta_x_m*delta_x_m + delta_y_m[j]*delta_y_m[j]));
			}
			//   if (delta_t > delta_y_m/sqrt(2*M_G*9500))
			//      delta_t = delta_y_m/sqrt(2*M_G*9500);

			float dlina_ekvatora = 40076;
			float vremya_oborota_zemli = 365 * 24 * 60 * 60 + 6 * 60 * 60 + 9 * 60 + 9;
			float skor_zemli = dlina_ekvatora / vremya_oborota_zemli;
			//float Koef_Sheroh=0.002;
			//float Koef_Sh=-M_G*Koef_Sheroh*Koef_Sheroh;
			for (t = 0; t <= time_moments; t++)
			{
				for (int j = 1; j<size_y - 1; j++)
				{
					float Koef_Koriolisa = 2 * skor_zemli*cos((start_y + j*delta_y) / 180.0*M_PI);
					for (int i = 1; i<size_x - 1; i++)
					{
						if (i<size_x - 2 && j<size_y - 2)
							eta[j][i] = eta_old[j][i]
							+ delta_t[j] * (0.5 / delta_x_m * (u_old[j + 1][i] * (h[j + 2][i] + h[j + 1][i])
							- u_old[j][i] * (h[j + 1][i] + h[j][i])
							)
							+ 0.5 / delta_y_m[j] * (v_old[j][i + 1] * (h[j][i + 2] + h[j][i + 1])
							- v_old[j][i] * (h[j][i + 1] + h[j][i])
							)
							);

						if (i>0 && j>0)
						{
							u[j][i] = u_old[j][i] - (M_G / (2 * delta_x_m)*(eta[j][i] - eta[j - 1][i])
								- Koef_Koriolisa*v_old[j][i]
								//-Koef_Sh/exp(1.8*log(fabs(h[j][i]+eta[j][i])))
								//*u_old[j][i]*sqrt(u_old[j][i]*u_old[j][i]+v_old[j][i]*v_old[j][i])
								)*delta_t[j];
							v[j][i] = v_old[j][i] - (M_G / (2 * delta_y_m[j])*(eta[j][i] - eta[j][i - 1])
								+ Koef_Koriolisa*u_old[j][i]
								//-Koef_Sh/exp(1.8*log(fabs(h[j][i]+eta[j][i])))
								//*v_old[j][i]*sqrt(u_old[j][i]*u_old[j][i]+v_old[j][i]*v_old[j][i])
								)*delta_t[j];
						}

						for (int b = 0; b<terr_cnt; b++)
							if (terr_up[j][i] == b + 1 && t*delta_t[j]<t_h_v_up[0][b] - delta_t[j])
								eta[j][i] = eta[j][i] + t_h_v_up[2][b] * delta_t[j];

					}
				}

				for (int i = 1; i<size_x; i++)
				{
					int temp = (int)(i*size_y / size_x);
					v[0][i] = sqrt(-M_G*h[0][i])*eta[0][i] / (eta[1][i] - h[0][i]);
					v[size_y - 2][i] = sqrt(-M_G*h[size_y - 2][i])*eta[size_y - 3][i] / (eta[size_y - 3][i] - h[size_y - 2][i]);;//v[size_y-3][i];//
					v[size_y - 1][i] = v[size_y - 2][i];// sqrt(-M_G*h[size_y-1][i])*eta[size_y-1][i]/(eta[size_y-2][i]-h[size_y-1][i]);
					//v[size_y][i] =v[size_y-1][i];
					eta[0][i] = eta_old[0][i] - sqrt(-h[0][i] * M_G)*(delta_t[temp] / delta_y_m[temp])*(eta_old[0][i] - eta_old[1][i]);
					eta[size_y - 2][i] = eta_old[size_y - 2][i] - sqrt(-h[size_y - 2][i] * M_G)*(delta_t[temp] / delta_y_m[temp])*(eta_old[size_y - 2][i] - eta_old[size_y - 3][i]);
					eta[size_y - 1][i] = eta[size_y - 2][i];// - sqrt(-h[size_y-1][i]*M_G)*(delta_t[i]/delta_y_m[i])*(eta_old[size_y-1][i]-eta_old[size_y-2][i]);
				}
				for (int j = 1; j<size_y; j++)
				{
					u[j][0] = sqrt(-M_G*h[j][0])*eta[j][1] / (eta[j][1] - h[j][0]);
					u[j][size_x - 2] = u[j][size_x - 3];//-sqrt(-M_G*h[j][size_x-2])*eta[j][size_x-3]/(eta[j][size_x-3]-h[j][size_x-2])
					u[j][size_x - 1] = u[j][size_x - 2];// = sqrt(-M_G*h[j][size_x-1])*eta[j][size_x-1]/(eta[j][size_x-2]-h[j][size_x-1]);
					//u[j][size_x] = u[j][size_x-1];
					eta[j][0] = eta_old[j][0] - sqrt(-h[j][0] * M_G)*(delta_t[j] / delta_x_m)*(eta_old[j][0] - eta_old[j][1]);
					eta[j][size_x - 2] = eta_old[j][size_x - 2] - sqrt(-h[j][size_x - 2] * M_G)*(delta_t[j] / delta_x_m)*(eta_old[j][size_x - 2] - eta_old[j][size_x - 3]);
					eta[j][size_x - 1] = eta[j][size_x - 2];// - sqrt(-h[j][size_x-1]*M_G)*(delta_t[j]/delta_x_m)*(eta_old[j][size_x-1]-eta_old[j][size_x-2]);
				}

				for (int j = 1; j<size_y - 1; j++)
				{
					for (int i = 1; i<size_x - 1; i++)
					{
						if (eta[j][i]>15.) { eta[j][i] = 15.; }
						if (eta[j][i]<-15.) { eta[j][i] = -15.; }
						// SizeX->Text=FloatToStr(eta[j][i]);
						if (visota[j][i]<eta[j][i]){ visota[j][i] = eta[j][i]; }
					}
				}
				swap_d(&eta_old, &eta);
				swap_d(&v_old, &v);
				swap_d(&u_old, &u);

				/* modify to new style
				Process->Caption = "Time = " + System::Convert::ToString(t);
				DeltaT->Caption = Format("dt = %3.3f", ARRAYOFCONST((delta_t[0])));
				*/

				int numberxx = 0.;
				int numberyy = 0.;

				scaling = 2.0;
				if (t%output_moments == 0)
				{
					for (int jpoint = 0; jpoint<point_cnt; jpoint++)
					{
						for (int ipoint = 0; ipoint<2; ipoint++)
						{
							point_points[ipoint][jpoint] = System::Convert::ToDouble(point_tmp[jpoint + 1][ipoint + 1]);
							numberxx = int(((point_points[0][jpoint] - start_x) / delta_x + 0.5));
							numberyy = int(((point_points[1][jpoint] - start_y) / delta_y + 0.5));
							// int rx=a;
							//int ry=b;
						}
					}

					showDisturbance();

					int h = (int)(delta_t[0] * t) / 3600;
					int m = ((int)(delta_t[0] * t) - h * 3600) / 60;
					int s = (int)(delta_t[0] * t) - h * 3600 - m * 60;

					/* is needed?
					Color c1 = pBitmap->Canvas->Brush->Color, c2 = pBitmap->Canvas->Font->Color;
					*/

					/* is needed?
					pBitmap->Canvas->Brush->Color = Color::Black;
					*/

					//old: pBitmap->Canvas->Font->Color = Color::Red;
					//old: pBitmap->Canvas->Font->Size = 1;
					System::Drawing::Font^ font=gcnew System::Drawing::Font(FontFamily::GenericSansSerif, 1.0F);
					SolidBrush^ fontBrush=gcnew SolidBrush(Color::Red);
					// ---------------------------------------------

					//old: pBitmap->Canvas->TextRect( Rect(50, 50, 50 + pBitmap->Canvas->TextWidth("00'00'00"), 50 + pBitmap->Canvas->TextHeight("00'00'00")), \
						50, 50, Format("%2.2d\'%2.2d\'%2.2d", OPENARRAY(TVarRec, (h, m, s))));
					RectangleF r(50, 50, 0, 0);
					this->mainGraphics->DrawString("", font, fontBrush, r);
					// ----------------------------------------------------------------------

					//old: pBitmap->Canvas->Brush->Color = Color::Silver;
					//pBitmap->Canvas->FillRect(Rect(floor(size_x / scaling), 0, pBitmap->Width, pBitmap->Height));
					SolidBrush^ sb = gcnew SolidBrush(Color::Silver);
					r.X = floor(size_x / scaling);
					r.Y = 0;
					r.Width = this->mainBitmap->Width;
					r.Height = this->mainBitmap->Height;
					this->mainGraphics->FillRectangle(sb, r);
					// -------------------------------------------------

					for (float j = 15; j <= 30; j += 0.5) 
					{
						//old: pBitmap->Canvas->Brush->Color = eta2col(j - 15); //(j-15); //eta2col(j-10);
						//pBitmap->Canvas->FillRect(Rect(SCALE_WIDTH / 4 + floor(size_x / scaling), \
							SCALE_WIDTH / 2 + (30 - j)*(pBitmap->Height*1.5 - SCALE_WIDTH / 2) / 30, \
							SCALE_WIDTH + floor(size_x / scaling), \
							SCALE_WIDTH / 2 + (30 - j + 1)*(pBitmap->Height*1.5 - SCALE_WIDTH / 2) / 30));
						sb = gcnew SolidBrush(eta2col(j - 15));
						r.X = SCALE_WIDTH / 4 + floor(size_x / scaling);
						r.Y = SCALE_WIDTH / 2 + (30 - j)*(this->mainBitmap->Height*1.5 - SCALE_WIDTH / 2) / 30;
						r.Width = SCALE_WIDTH + floor(size_x / scaling);
						r.Height = SCALE_WIDTH / 2 + (30 - j + 1)*(this->mainBitmap->Height*1.5 - SCALE_WIDTH / 2) / 30;
						this->mainGraphics->FillRectangle(sb, r);
						// --------------------------------------------------------------------------------
					}
					for (float j = 15; j <= 30; j += 0.5) 
					{
						// pBitmap->Canvas->Font->Color = clBlack;

						//old: pBitmap->Canvas->Brush->Color = Color::Silver;
						//pBitmap->Canvas->Font->Color = RGB(0, 0, 127 + j);
						//int x1 = SCALE_WIDTH + floor(size_x / scaling);
						//int y1 = -pBitmap->Canvas->TextHeight("0000") + \
							SCALE_WIDTH / 4 + (30 - j + 1)*(pBitmap->Height*1.5 - SCALE_WIDTH / 2) / 30;
						//pBitmap->Canvas->TextOut(SCALE_WIDTH + floor(size_x / scaling), \
							-pBitmap->Canvas->TextHeight("0000") + \
							//pBitmap->Height/2 + \
							SCALE_WIDTH / 4 + (30 - j + 1)*(pBitmap->Height*1.5 - SCALE_WIDTH / 2) / 30, \
							Format("%f", OPENARRAY(TVarRec, (j - 15))));//shkala visota
						sb = gcnew SolidBrush(Color::Silver);
						fontBrush = gcnew SolidBrush(Color::FromArgb(255, 0, 0, 127+j));
						PointF point(SCALE_WIDTH + floor(size_x / scaling), \
									-1.0 + SCALE_WIDTH / 4 + (30 - j + 1)*(this->mainBitmap->Height*1.5 - SCALE_WIDTH / 2) / 30);
						this->mainGraphics->DrawString("", font, fontBrush, point);
						// ---------------------------------------------------
					}

					/* is needed?
					pBitmap->Canvas->Brush->Color = c1; pBitmap->Canvas->Font->Color = c2;
					*/

					if (this->checkBox_autoSaveLayers->Checked)
						SaveBmpClick(t);
					file_count++;

					vf->pictureBox_main->Update();
				}
				if (!running) break;
			}

			MessageBox::Show("Modelling Complete!", "Information!", MessageBoxButtons::OK, MessageBoxIcon::Information);
			return 0;
		}

		void checkReadyForCalculationState()
		{
			if (!running)
			{
				if (this->textBox_calcTime->Text != "" && this->textBox_isobath->Text != "" && this->textBox_outTime->Text != "")
				{
					if (System::Convert::ToDouble(this->textBox_calcTime->Text) > 0 && System::Convert::ToDouble(this->textBox_outTime->Text))
					{
						this->button_applyParameters->Enabled = true;
					}
					else
						this->button_applyParameters->Enabled = false;
				}
				else
				{
					this->button_applyParameters->Enabled = false;
				}
			}
			else
				this->button_applyParameters->Enabled = false;
		}

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->textBox_rangeX_start = (gcnew System::Windows::Forms::TextBox());
			this->textBox_rangeY_start = (gcnew System::Windows::Forms::TextBox());
			this->textBox_rangeX_end = (gcnew System::Windows::Forms::TextBox());
			this->textBox_rangeY_end = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->textBox_sizeX = (gcnew System::Windows::Forms::TextBox());
			this->textBox_sizeY = (gcnew System::Windows::Forms::TextBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->textBox_dx = (gcnew System::Windows::Forms::TextBox());
			this->textBox_dy = (gcnew System::Windows::Forms::TextBox());
			this->textBox_outTime = (gcnew System::Windows::Forms::TextBox());
			this->textBox_calcTime = (gcnew System::Windows::Forms::TextBox());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->button_applyParameters = (gcnew System::Windows::Forms::Button());
			this->button_startCalc = (gcnew System::Windows::Forms::Button());
			this->button_pauseCalc = (gcnew System::Windows::Forms::Button());
			this->button_stopCalc = (gcnew System::Windows::Forms::Button());
			this->menuStrip_main = (gcnew System::Windows::Forms::MenuStrip());
			this->ToolStrip_file = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ToolStrip_file_openMap = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ToolStrip_file_openConfig = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->textBox_isobath = (gcnew System::Windows::Forms::TextBox());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->checkBox_autoSaveLayers = (gcnew System::Windows::Forms::CheckBox());
			this->menuStrip_main->SuspendLayout();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(16, 44);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(63, 17);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Range X";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(130, 44);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(13, 17);
			this->label2->TabIndex = 1;
			this->label2->Text = L"-";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(16, 71);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(63, 17);
			this->label3->TabIndex = 2;
			this->label3->Text = L"Range Y";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(130, 71);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(13, 17);
			this->label4->TabIndex = 3;
			this->label4->Text = L"-";
			// 
			// textBox_rangeX_start
			// 
			this->textBox_rangeX_start->Location = System::Drawing::Point(85, 41);
			this->textBox_rangeX_start->Name = L"textBox_rangeX_start";
			this->textBox_rangeX_start->ReadOnly = true;
			this->textBox_rangeX_start->Size = System::Drawing::Size(39, 22);
			this->textBox_rangeX_start->TabIndex = 4;
			this->textBox_rangeX_start->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// textBox_rangeY_start
			// 
			this->textBox_rangeY_start->Location = System::Drawing::Point(85, 68);
			this->textBox_rangeY_start->Name = L"textBox_rangeY_start";
			this->textBox_rangeY_start->ReadOnly = true;
			this->textBox_rangeY_start->Size = System::Drawing::Size(39, 22);
			this->textBox_rangeY_start->TabIndex = 5;
			this->textBox_rangeY_start->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// textBox_rangeX_end
			// 
			this->textBox_rangeX_end->Location = System::Drawing::Point(149, 41);
			this->textBox_rangeX_end->Name = L"textBox_rangeX_end";
			this->textBox_rangeX_end->ReadOnly = true;
			this->textBox_rangeX_end->Size = System::Drawing::Size(39, 22);
			this->textBox_rangeX_end->TabIndex = 6;
			this->textBox_rangeX_end->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// textBox_rangeY_end
			// 
			this->textBox_rangeY_end->Location = System::Drawing::Point(149, 68);
			this->textBox_rangeY_end->Name = L"textBox_rangeY_end";
			this->textBox_rangeY_end->ReadOnly = true;
			this->textBox_rangeY_end->Size = System::Drawing::Size(39, 22);
			this->textBox_rangeY_end->TabIndex = 7;
			this->textBox_rangeY_end->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(207, 71);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(48, 17);
			this->label5->TabIndex = 8;
			this->label5->Text = L"Size Y";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(207, 44);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(48, 17);
			this->label6->TabIndex = 9;
			this->label6->Text = L"Size X";
			// 
			// textBox_sizeX
			// 
			this->textBox_sizeX->Location = System::Drawing::Point(261, 41);
			this->textBox_sizeX->Name = L"textBox_sizeX";
			this->textBox_sizeX->ReadOnly = true;
			this->textBox_sizeX->Size = System::Drawing::Size(39, 22);
			this->textBox_sizeX->TabIndex = 10;
			this->textBox_sizeX->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// textBox_sizeY
			// 
			this->textBox_sizeY->Location = System::Drawing::Point(261, 68);
			this->textBox_sizeY->Name = L"textBox_sizeY";
			this->textBox_sizeY->ReadOnly = true;
			this->textBox_sizeY->Size = System::Drawing::Size(39, 22);
			this->textBox_sizeY->TabIndex = 11;
			this->textBox_sizeY->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(43, 99);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(22, 17);
			this->label7->TabIndex = 12;
			this->label7->Text = L"dx";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(187, 99);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(23, 17);
			this->label8->TabIndex = 13;
			this->label8->Text = L"dy";
			// 
			// textBox_dx
			// 
			this->textBox_dx->Location = System::Drawing::Point(71, 96);
			this->textBox_dx->Name = L"textBox_dx";
			this->textBox_dx->ReadOnly = true;
			this->textBox_dx->Size = System::Drawing::Size(87, 22);
			this->textBox_dx->TabIndex = 14;
			this->textBox_dx->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// textBox_dy
			// 
			this->textBox_dy->Location = System::Drawing::Point(216, 96);
			this->textBox_dy->Name = L"textBox_dy";
			this->textBox_dy->ReadOnly = true;
			this->textBox_dy->Size = System::Drawing::Size(87, 22);
			this->textBox_dy->TabIndex = 15;
			this->textBox_dy->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// textBox_outTime
			// 
			this->textBox_outTime->Location = System::Drawing::Point(238, 165);
			this->textBox_outTime->Name = L"textBox_outTime";
			this->textBox_outTime->Size = System::Drawing::Size(62, 22);
			this->textBox_outTime->TabIndex = 22;
			this->textBox_outTime->Text = L"0";
			this->textBox_outTime->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->textBox_outTime->TextChanged += gcnew System::EventHandler(this, &mainForm::textBox_outTime_TextChanged);
			// 
			// textBox_calcTime
			// 
			this->textBox_calcTime->Location = System::Drawing::Point(85, 134);
			this->textBox_calcTime->Name = L"textBox_calcTime";
			this->textBox_calcTime->Size = System::Drawing::Size(62, 22);
			this->textBox_calcTime->TabIndex = 21;
			this->textBox_calcTime->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->textBox_calcTime->TextChanged += gcnew System::EventHandler(this, &mainForm::textBox_calcTime_TextChanged);
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(166, 168);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(66, 17);
			this->label9->TabIndex = 20;
			this->label9->Text = L"Out Time";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(15, 137);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(66, 17);
			this->label10->TabIndex = 19;
			this->label10->Text = L"CalcTime";
			// 
			// button_applyParameters
			// 
			this->button_applyParameters->Enabled = false;
			this->button_applyParameters->Location = System::Drawing::Point(19, 194);
			this->button_applyParameters->Name = L"button_applyParameters";
			this->button_applyParameters->Size = System::Drawing::Size(286, 28);
			this->button_applyParameters->TabIndex = 23;
			this->button_applyParameters->Text = L"Apply Parameters";
			this->button_applyParameters->UseVisualStyleBackColor = true;
			this->button_applyParameters->Click += gcnew System::EventHandler(this, &mainForm::button_applyParameters_Click);
			// 
			// button_startCalc
			// 
			this->button_startCalc->Enabled = false;
			this->button_startCalc->Location = System::Drawing::Point(19, 241);
			this->button_startCalc->Name = L"button_startCalc";
			this->button_startCalc->Size = System::Drawing::Size(83, 28);
			this->button_startCalc->TabIndex = 24;
			this->button_startCalc->Text = L"Start";
			this->button_startCalc->UseVisualStyleBackColor = true;
			this->button_startCalc->Click += gcnew System::EventHandler(this, &mainForm::button_startCalc_Click);
			// 
			// button_pauseCalc
			// 
			this->button_pauseCalc->Enabled = false;
			this->button_pauseCalc->Location = System::Drawing::Point(124, 241);
			this->button_pauseCalc->Name = L"button_pauseCalc";
			this->button_pauseCalc->Size = System::Drawing::Size(83, 28);
			this->button_pauseCalc->TabIndex = 25;
			this->button_pauseCalc->Text = L"Pause";
			this->button_pauseCalc->UseVisualStyleBackColor = true;
			this->button_pauseCalc->Click += gcnew System::EventHandler(this, &mainForm::button_pauseCalc_Click);
			// 
			// button_stopCalc
			// 
			this->button_stopCalc->Enabled = false;
			this->button_stopCalc->Location = System::Drawing::Point(222, 241);
			this->button_stopCalc->Name = L"button_stopCalc";
			this->button_stopCalc->Size = System::Drawing::Size(83, 28);
			this->button_stopCalc->TabIndex = 26;
			this->button_stopCalc->Text = L"Stop";
			this->button_stopCalc->UseVisualStyleBackColor = true;
			this->button_stopCalc->Click += gcnew System::EventHandler(this, &mainForm::button_stopCalc_Click);
			// 
			// menuStrip_main
			// 
			this->menuStrip_main->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->ToolStrip_file });
			this->menuStrip_main->Location = System::Drawing::Point(0, 0);
			this->menuStrip_main->Name = L"menuStrip_main";
			this->menuStrip_main->Size = System::Drawing::Size(312, 28);
			this->menuStrip_main->TabIndex = 27;
			this->menuStrip_main->Text = L"menuStrip1";
			// 
			// ToolStrip_file
			// 
			this->ToolStrip_file->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->ToolStrip_file_openMap,
					this->ToolStrip_file_openConfig
			});
			this->ToolStrip_file->Name = L"ToolStrip_file";
			this->ToolStrip_file->Size = System::Drawing::Size(44, 24);
			this->ToolStrip_file->Text = L"FIle";
			// 
			// ToolStrip_file_openMap
			// 
			this->ToolStrip_file_openMap->Name = L"ToolStrip_file_openMap";
			this->ToolStrip_file_openMap->Size = System::Drawing::Size(162, 24);
			this->ToolStrip_file_openMap->Text = L"Open Map";
			this->ToolStrip_file_openMap->Click += gcnew System::EventHandler(this, &mainForm::openToolStripMenuItem_Click);
			// 
			// ToolStrip_file_openConfig
			// 
			this->ToolStrip_file_openConfig->Name = L"ToolStrip_file_openConfig";
			this->ToolStrip_file_openConfig->Size = System::Drawing::Size(162, 24);
			this->ToolStrip_file_openConfig->Text = L"Open Config";
			this->ToolStrip_file_openConfig->Click += gcnew System::EventHandler(this, &mainForm::openConfigToolStripMenuItem_Click);
			// 
			// textBox_isobath
			// 
			this->textBox_isobath->Location = System::Drawing::Point(238, 133);
			this->textBox_isobath->Name = L"textBox_isobath";
			this->textBox_isobath->Size = System::Drawing::Size(62, 22);
			this->textBox_isobath->TabIndex = 28;
			this->textBox_isobath->Text = L"-10";
			this->textBox_isobath->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->textBox_isobath->TextChanged += gcnew System::EventHandler(this, &mainForm::textBox_isobath_TextChanged);
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(178, 136);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(54, 17);
			this->label11->TabIndex = 29;
			this->label11->Text = L"Isobath";
			// 
			// checkBox_autoSaveLayers
			// 
			this->checkBox_autoSaveLayers->AutoSize = true;
			this->checkBox_autoSaveLayers->Location = System::Drawing::Point(16, 167);
			this->checkBox_autoSaveLayers->Name = L"checkBox_autoSaveLayers";
			this->checkBox_autoSaveLayers->RightToLeft = System::Windows::Forms::RightToLeft::Yes;
			this->checkBox_autoSaveLayers->Size = System::Drawing::Size(142, 21);
			this->checkBox_autoSaveLayers->TabIndex = 31;
			this->checkBox_autoSaveLayers->Text = L"Auto Save Layers";
			this->checkBox_autoSaveLayers->UseVisualStyleBackColor = true;
			this->checkBox_autoSaveLayers->CheckedChanged += gcnew System::EventHandler(this, &mainForm::checkBox_autoSaveLayers_CheckedChanged);
			// 
			// mainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(312, 283);
			this->Controls->Add(this->checkBox_autoSaveLayers);
			this->Controls->Add(this->label11);
			this->Controls->Add(this->textBox_isobath);
			this->Controls->Add(this->button_stopCalc);
			this->Controls->Add(this->button_pauseCalc);
			this->Controls->Add(this->button_startCalc);
			this->Controls->Add(this->button_applyParameters);
			this->Controls->Add(this->textBox_outTime);
			this->Controls->Add(this->textBox_calcTime);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->label10);
			this->Controls->Add(this->textBox_dy);
			this->Controls->Add(this->textBox_dx);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->textBox_sizeY);
			this->Controls->Add(this->textBox_sizeX);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->textBox_rangeY_end);
			this->Controls->Add(this->textBox_rangeX_end);
			this->Controls->Add(this->textBox_rangeY_start);
			this->Controls->Add(this->textBox_rangeX_start);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->menuStrip_main);
			this->MainMenuStrip = this->menuStrip_main;
			this->MaximizeBox = false;
			this->MaximumSize = System::Drawing::Size(330, 330);
			this->MinimumSize = System::Drawing::Size(330, 298);
			this->Name = L"mainForm";
			this->Text = L"Tsunami";
			this->menuStrip_main->ResumeLayout(false);
			this->menuStrip_main->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private:
		System::Void button_startCalc_Click(System::Object^  sender, System::EventArgs^  e)
		{
			vf->Show();
			
			running = true;

			this->button_startCalc->Enabled = false;
			this->ToolStrip_file_openMap->Enabled = false;
			//int code = processing();
			showBottom();
			this->button_startCalc->Enabled = true;
			this->ToolStrip_file_openMap->Enabled = true;

			running = false;
		}

		System::Void textBox_calcTime_TextChanged(System::Object^  sender, System::EventArgs^  e)
		{
			this->button_startCalc->Enabled = false; 
			checkReadyForCalculationState();
		}

		System::Void textBox_outTime_TextChanged(System::Object^  sender, System::EventArgs^  e)
		{
			this->button_startCalc->Enabled = false;
			checkReadyForCalculationState();
		}

		System::Void textBox_isobath_TextChanged(System::Object^  sender, System::EventArgs^  e)
		{
			this->button_startCalc->Enabled = false;
			checkReadyForCalculationState();
		}

		System::Void checkBox_autoSaveLayers_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
		{
			this->button_startCalc->Enabled = false;
			checkReadyForCalculationState();
		}

		System::Void button_applyParameters_Click(System::Object^  sender, System::EventArgs^  e)
		{
			/* save parameters */
			time_moments = System::Convert::ToInt32(this->textBox_calcTime->Text);
			output_moments = System::Convert::ToDouble(this->textBox_outTime->Text);
			izobata = System::Convert::ToDouble(this->textBox_isobath->Text);
			this->button_startCalc->Enabled = true;
		}

		System::Void button_pauseCalc_Click(System::Object^  sender, System::EventArgs^  e)
		{

		}

		System::Void button_stopCalc_Click(System::Object^  sender, System::EventArgs^  e)
		{

		}

		System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		{
			OpenFileDialog^ openMap = gcnew OpenFileDialog;
			openMap->InitialDirectory = "c:\\";
			openMap->RestoreDirectory = true;

			if (openMap->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				mapFileName = openMap->FileName;
			}
			vf = gcnew ViewForm();

			if (this->loadMap())
			{
				this->mainBitmap = gcnew Bitmap(size_x, size_y);
				vf->pictureBox_main->Image = this->mainBitmap;
				System::Drawing::Size pbSize = vf->pictureBox_main->Size;
				//vf->pictureBox_main->Scale(pbSize.Width/size_x, pbSize.Height/size_y);
				//vf->pictureBox_main->Scale(0.5, 0.5);
				mainGraphics = Graphics::FromImage(this->mainBitmap);
			}
			else
			{
				MessageBox::Show("this", "Error!", MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
		}

		System::Void openConfigToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
		{
			/* put .xml reading here */
		}
};
}