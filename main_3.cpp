#include <iostream>
#include <stdlib.h>
#include <ctime>

using namespace std;

double random;
double randomreal;
long double votos_candidato	[10];
 long  double porcentaje;
int k =100;
int sumita= 0;
int dif;
long long censo=0;
long long votantes =0;
long long m = 1000;//minimo
long long  n = 10000;//maximo
 const  int GALGO = 100;
 long double porcentajetotal=0;
 long double porcentajeab;
 double abstencion =0;
 int azar;
 string nombres[7]= {"Gustavo Petro","Ivan Duque","Humberto de la Calle","German Vargas Lleras","Sergio Fajardo","Voto en blanco","Votos Nulos"};
int main (){
	//srand (getpid());
	srand((unsigned)time(0));
	
	censo = rand() %(n - m +1);
	censo = censo*GALGO;
	cout<<"\t EL CENSO ELECTORAL EN LA CIUDAD DE BOGOTA  ES  DE : "<<censo<<endl;
	votantes = (rand() % (censo + 1));
	abstencion = censo-votantes;
	porcentajeab = (abstencion*100)/censo;
	cout<<"la abstencion es de : % "<<porcentajeab<<endl;
	 
	
	dif=votantes;
		for(int i=0 ; i<7; i++){//digamos que son 7
	        random = rand() %(dif +1);
	        dif=dif-random;
	        if(random != votantes){
	        votos_candidato[i]=random;	
			}
			else{
			votos_candidato[i] = votantes-sumita;
			}
			sumita =sumita + votos_candidato[i];
		}
		
		if(sumita != votantes){
		//	cout<<"faltan: "<<votantes-sumita<<endl;
		//	cout<< votos_candidato[9];
			votos_candidato[6] += (votantes-sumita);//aqui eran 9 
		}
		
		cout<<endl;
	//	cout<<"LA CANTIDAD DE VOTOS DE CADA CANDIDATO EN ESTA CIUDAD ES : "<<endl;
		for(int i = 0; i<7 ; i++){
				//cout<<"  "<<votos_candidato[i];
				porcentaje= (votos_candidato[i]/votantes)*100;
				cout<<endl;
				//azar = rand() % (7);//aquiva (limite maximo dese 0 )
				cout<< nombres[i]<<"  obtuvo : "<<votos_candidato[i]<<" votos  con el  "<<porcentaje<<"%"<<endl;
		
				porcentajetotal = porcentajetotal + porcentaje;
		}
		
		cout<<"TOTAL DE LAS MESAS INFORMADAS: "<<porcentajetotal<<"%"<<endl;
		cout<<"VOTOS TOTALES: "<<votantes<<endl;
	
		
		cout<<" LA ABSTENCION DE LA CIUDAD X FUE DE: "<<abstencion<<" PERSONAS PEREZOSAS CON UN "<<porcentajeab<<"%"<<endl;
			
	 //dif=k;
	 
	 
	 
/*	 
	for(int i=0 ; i<10; i++){
				if(i != 10){
				cout<<" dif : "<<dif<<endl;
				random = rand() % (dif+1);
				randomreal = random*0.01;
				cout<<" random  : "<<random<<endl;
				cout<<" randomreal  : "<<randomreal<<endl;
				dif=dif-random;
				if(random == k){
				votos_candidato[i] = random;
				votos_candidato[i+1] = 0;	
				}
				else {
					if(random!= k){
				votos_candidato[i] = randomreal;
					}
				}
			
				}
				else 
				if(i == 10 ){
				votos_candidato[i] = k-sumita;
				}
				sumita =sumita +votos_candidato[i];
			}
			
			for(int i =0; i<10 ; i++){
				cout<<" "<<votos_candidato[i];
			}
			*/
}

