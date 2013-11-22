#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>

float *a;

const float pi = 3.141592654;
float alpha;
float q;
float Ti;
float Tf;
float x0;
 
float Erfc(float x){
	//função erro complementar de x
	int i;
	float result;

	result = 0;
	for(i = 0; i <=6; i++)
		result += a[i]*pow(x, i);
	
	result = pow(result, -16);
	return(result);
	}

float T(float x, float t){
	double T_xt;
		T_xt = Ti + q * (2* sqrt(alpha*t/pi) * exp(-1*pow(x, 2)/(4*alpha*t)) - x*Erfc(x/(2*sqrt(alpha*t))));
	return(T_xt);
	}

float f(float t){
	return(T(x0, t) - Tf);
	}

float df(float t){//aproxima a derivada de f por diferenças finitas de 2ª ordem
	float result, dt;

	dt = 0.0005;
	result = (f(t + dt) - f(t - dt))/(2.0*dt);
	return(result);
	}

float g(float t_n){
	float result;

	result = t_n - f(t_n)/df(t_n);
	return(result);
	}

void check_f(){

	FILE *f_out;

	f_out = fopen("./data/7-2.dat", "w");
	float t;

	for(t = 1310; t <= 1330; t+= 1)
		fprintf(f_out, "%.2f \t %.6f\n", t, f(t));
	fclose(f_out);
	
	}


int main(){
	/*------------------------------------------
	 *------------VARIÁVEIS GLOGBAIS------------
	 *------------------------------------------ */
	a =(float *)malloc(7*sizeof(float));
	a[0] = 1.0;
	a[1] = 0.0705230784;
	a[2] = 0.0422820123;
	a[3] = 0.0092705272;
	a[4] = 0.0001520143;
	a[5] = 0.0002765672;
	a[6] = 0.0000430638;

	alpha = 1.0;
	q = 1.0;
	Ti = 10;
	Tf = 50;
	x0 = 1 ;
	/*------------------------------------------
	 *------------VARIÁVEIS LOCAIS------------
	 *------------------------------------------ */


	float u, v, w, t_0;
	float a, b;
	int n, k;
	FILE *f_out;

	f_out = fopen("./data/7-1.dat", "w+");

	n = 1600;
	t_0 = 1320;

	a = t_0 - 1;
	b = t_0 + 1;
	check_f();
	if(f(a)*f(b) >= 0)
		{
		printf("Método da Bisseção não aplicável! \n");
		return(0);
		}

	u = a;
	v = b;
	for(k = 0; k < n; k++)
		{
			w = (u + v)/2.0;
			if(f(w) == 0)	break;
			if(f(u)*f(w) < 0)	v = w;
			else u = w;
			fprintf(f_out, "%f\t %d\n", w, k);
		}

	fclose(f_out);
	return(0);
	}
