/**
	@file ciudad.h
	@brief clase que gestiona el archivo ciudad
	
	este archivo posee todos los metodos necesarios para el manejo de la informacion de las ciudades  
	
	@author Andres Arias & Isabel Perez
	
	@date 8/05/2018,28/05/2018
*/
#ifndef CIUDAD
#define CIUDAD
#include <string.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <time.h>
#include "Librerias/arbolAVL.h"
#include "facade.h"
#include "estructuras.h"
#include "departamento.h"

//esta clase gestiona la lectura, escritura y la busqueda en el archivo ciudad.txt
class ciudad: public facade{
	private:
		//arbol AVL
		arbolAVL<city> *arbolCiudades;
		//instancia unica
		static ciudad *instance;
		/** 
			@brief metodo constructor 
			@returns lectura de registros 
		*/
		ciudad(){
			arbolCiudades = new arbolAVL<city>();
			this->cantidad = 0;
			this->leido = false;
			leerRegistros();
		}
		/**
		@brief funcion para leer los registros del archivo 
		*/
		void leerRegistros(){
			if(this->leido==false){
				int clave,estado;
				string nombre;
				int departamento;		
				long long censo;
				city ciuda;
				//archivo de entrada
				ifstream archEntrada("Archivos/ciudades.txt", ios::in);
				if (!archEntrada.good()){
					cerr << "No se pudo abrir el archivo ciudades" << endl;
			    	exit(1);
				}
				while(!archEntrada.eof()){
					archEntrada >> clave;
					archEntrada >> nombre;
					archEntrada >> departamento;
					archEntrada >> censo;
					archEntrada >> estado;
					ciuda.clave = clave;
					ciuda.nombre = nombre;
					ciuda.departamento = departamento;
					ciuda.censo = censo;
					ciuda.estado = estado;
					this->agregarCiudad(ciuda);
				}
				archEntrada.close();
				this->leido = true;
			} 
		}
		
	public:
		/** 
		@brief metodo de instanciacion de la clase (Patron singleton)
		@returns devuelve la instancia de la clase
		
		*/
		static ciudad *getInstance(){
			if(instance == 0){
				instance = new ciudad();
			}
			return instance;
		}
		/**
		@brief funcion para agregar una ciudad al arbol avl
		@param ciudad city
		*/
		void agregarCiudad(city ciudad){
			city *ciu;
			ciudad.clave = this->cantidad++;
			arbolCiudades->agregar(ciudad);
			ciu = arbolCiudades->retornarEstructura(ciudad.clave);
			//se agrega la ciudad al departamento
			departamento::getInstance()->agregarCiudad(ciudad.departamento,ciu);
		}
		/**
		@brief se consultan todas las ciudades habilitadas para el censo electoral
		@returns lista del tipo city con todas las ciudades
		*/
		Lista<city> *consultarCiudades(){
			Lista<city> *lista = arbolCiudades->recorridoInOrden();
			return lista;
		}
		/** 
		@brief funcion para obtener los candidatos de una ciudad dada su clave
		@returns lista de apuntadores de candidatos asociados a una ciudad
		@param clave int
		*/
		Lista<candidate*> getCandidatoByCiudad(int clave){
			city *ciudad = arbolCiudades->retornarEstructura(clave);
			Lista<candidate*> lista = ciudad->candidatos;
			return lista;
		}
		/** 
		 @brief agrega un candidato a su ciudad correspondiente
		 @param candidato candidate*
		 @param territorio int
		*/
		void agregarCandidato(candidate *candidato,int territorio){
			city *ciu = arbolCiudades->retornarEstructura(territorio);
			ciu->candidatos.anadir_final(candidato);
		}
		/**
		@brief funcion para obtener el nombre de una ciudad dada la clave
		@returns string con nombre de la ciudad
		@param clave int
		*/
		string getNombreCiudad(int clave){
			city *ciu = arbolCiudades->retornarEstructura(clave);
			return ciu->nombre;
		}
		/**
		@brief funcion para obtener el censo de una ciudad dada la clave
		@returns long long con censo de una ciudad
		@param clave int
		*/
		long long getCenso(int clave){
			city *ciu=arbolCiudades->retornarEstructura(clave);
			return ciu->censo;
		}
		/**
		@brief funcion que suma todos los censos por ciudad para obtener censo nacional
		@returns long long con censo nacional
		*/
		long long getCensoTotal(){
			Lista <city> *cities=arbolCiudades->recorridoInOrden();
			long long censototal=0;
			for(int i=1;i<cities->getTam();i++){
				censototal+=getCenso(i);
			}
			return censototal;
		}
		
		/**
		@brief funcion para obtener todos los candidatos presidenciales
		@returns lista tipo candiate* de los candidatos presidenciales
		*/
		Lista<candidate*>getCandidatoPresidencial(){
			city *ciu = arbolCiudades->retornarEstructura(0);
			Lista <candidate*> candidatoszona=ciu->candidatos;
			Lista <candidate*> candidatosp;
			candidate *can;
			int i;
			for (i=0;i<candidatoszona.getTam();i++){
				can=candidatoszona.devolverDato(i);
				if(can->formulaVi!=0 && can->estado==1){
					candidatosp.anadir_final(can);
				}
			}
			return candidatosp; 
		}
		/**
		@brief funcion para motrar candidato presidencial dado un partido
		@returns candidate* con candidato presidencial segun el partido
		@param partido int
		*/
		candidate *getCandidatoPByPartido(int partido){
			Lista<candidate*> lista = ciudad::getInstance()->getCandidatoByCiudad(0);
			candidate *presi = NULL;
			for(int i=0;i<lista.getTam();i++){
				presi = lista.devolverDato(i);
				if(presi->partido==partido && presi->estado==1){
					i=lista.getTam();
				}
			}
			return presi;
		}
		/**
		
		@brief funcion para obtener la informacion de la ciudad dada su clave 
		@returns apuntador del tipo city que apunta a la ciudad dado su codigo
		@param clave int
		*/
		city *getCiudad(int clave){
			return arbolCiudades->retornarEstructura(clave);
		}
		//retonar el codigo del departamento de una ciudad
		int getDepartamento(int clave){
			return arbolCiudades->retornarEstructura(clave)->departamento;
		}
		/** 
		@brief funcion para obtener los votos presidenciales por ciudad dada su clave
		@returns lista del tipo long long con todos los votos presidenciales de una ciudad
		@param clave int 
		
		*/
		Lista<long long> getVotosPbyCiudad(int clave){
			//valor objetivo->censo de la ciudad
			long long objetivo=getCenso(clave);
			double esteVoto=0;
			double votos=0;
			long long verdaderosVotos=0;
			//semilla de tiempo
			srand(time(NULL));
			//se necesita lista de numeros aleatorios y otra de los votos como tal
			Lista <long long> aleatorios;
			Lista <long long> lista;
			//captura los candidatos
			Lista<candidate*> candidatos=getCandidatoPresidencial();
			int i;
			//tam + 3 ya que son los candidatos mas votos en blanco, nulos y abstinencia
			for (i=0;i<candidatos.getTam()+3;i++){
				esteVoto=rand();
				votos+=esteVoto;
				aleatorios.anadir_final(esteVoto);
			}
			
			//votos de los candidatos 
			//logica tomada de excel XD
			for (i=0;i<aleatorios.getTam()-1;i++){
				esteVoto=(rand()/votos)*objetivo;
				verdaderosVotos+=esteVoto;
				lista.anadir_final(esteVoto);
			}
			//estos serian los votos de abstinencia
			if(objetivo>verdaderosVotos){
				lista.anadir_final((objetivo-verdaderosVotos));
			}else{
				lista.anadir_final((verdaderosVotos-objetivo));
			}
			return lista;
		}
		/** 
		@brief funcion para obtener todos los votos presidenciales de todas las ciudades
		@returns lista de listas long con todos los votos presidenciales de todas las ciudades
		*/
		Lista <Lista <long long> > getVotosCiudades (){
			Lista <city> *ciudades=consultarCiudades();
			Lista <Lista <long long> > lista;
			Lista <long long> auxiliar;
			int i;
			for (i=0;i<ciudades->getTam();i++){
				auxiliar=getVotosPbyCiudad(i);
				lista.anadir_final(auxiliar);
			}
			return lista;
		}
		/** 
		@brief funcion que suma todos los votos presidenciales por ciudad para obtener los votos presidenciales a nivel nacional
		@returns lista tipo long long con todos los votos a nivel nacional 
		@param lista Lista de listas tipo long long
		*/
		Lista <long long> getVotosPTotal(Lista <Lista <long long> > lista){
			Lista <long long> totales;
			int i=0;
			int j=0;
			long long total;
			for (i=0;i<lista.devolverDato(i).getTam();i++){
				total=0;
				for(j=1;j<lista.getTam();j++){
					total+=lista.devolverDato(j).devolverDato(i);
				}
				totales.anadir_final(total);
			}
			return totales;
		}
		/** 
		@brief funcion para calcular porcentajes de votos presidenciales a nivel nacional
		@returns lista del tipo float con porcentajes de notos presidenciales a nivel nacional
		@param enviada Lista tipo long long
		*/
		Lista <float> getPorcentajesPTotal(Lista <long long>enviada){
			Lista <float> porcentajes;
			float porcentaje=0.0;
			int i;
			for(i=0;i<enviada.getTam();i++) {
				porcentaje= ((float)enviada.devolverDato(i)*100.0)/(float)getCensoTotal();
				porcentajes.anadir_final(porcentaje);
			}
			return porcentajes;
		}
		/** 
		@brief funcion para calcular porcentajes de votos presidenciales en todas las ciudades
		@returns lista del tipo float con porcentajes de notos presidenciales en todas las ciudades
		@param simulacion Lista de listas tipo long long
		*/
		Lista < Lista <float> > getPorcentajesCiudades (Lista <Lista <long long> > simulacion ){
			int i;
			int j;
			Lista <Lista <float> > porcentajes;
			float porcentaje=0.0;
			for (i=1;i<simulacion.getTam();i++){
				Lista <float>  auxiliar;
				for(j=0;j<simulacion.devolverDato(i).getTam();j++){
					porcentaje= ((float)simulacion.devolverDato(i).devolverDato(j)/(float)getCenso(i))*100.0;
					auxiliar.anadir_final(porcentaje);
				}
				porcentajes.anadir_final(auxiliar);
			}
			return porcentajes;
		}
		/** 
		@brief funcion para calcular obtener votos a candidatas presidenciales mujer y votos a candidatos presidenciales hombres
		@returns lista del tipo long long con votos presidenciales para mujeres y para hombres
		@param votostotales Lista tipo long long
		*/
		Lista <long long> getVotosPSexo (Lista <long long>votostotales){
			Lista <candidate*>candidatos=getCandidatoPresidencial();
			candidate *can;
			Lista <long long> retorno;
			int i;
			long long hombres=0;
			long long mujeres=0;
			for(i=0;i<candidatos.getTam();i++){
				can=candidatos.devolverDato(i);
				if(can->sexo.compare("Mujer")==0){
					mujeres+=votostotales.devolverDato(i);
				}
				else{
					hombres+=votostotales.devolverDato(i);
				}
			}
			retorno.anadir_final(mujeres);
			retorno.anadir_final(hombres);
			
			return retorno;
		}
			/** 
			@brief Funcion que retona la cantidad de regisitros de ciudades
			@returns retornar un 'int' con la cantidad de regisitros
		*/
		int getCantidad(){
			return this->cantidad;
		}
		/** 
			@brief Funcion que libera memoria eliminando los atributos de la clase
		*/
		void liberar(){
			delete arbolCiudades;
			delete instance;
		}
		/** 
			@brief Funcion que reescribe el archivo ciudades.txt con las modificaciones realizadas
		*/
		void escribirRegistros(){
			ofstream archsalida("Archivos/ciudades.txt",ios::out|ios::trunc);
			if (!archsalida.good()){
				cerr << "No se pudo abrir el archivo ciudades" << endl;
				exit(1);
			}
			Lista<city> ciudades = *arbolCiudades->recorridoInOrden();
			for(int i=0;i<ciudades.getTam();i++){
				city ciu = ciudades.devolverDato(i);
				if(i!=ciudades.getTam()-1){
					archsalida<<ciu.clave<<" "<<ciu.nombre<<" "<<ciu.departamento<<" "<<ciu.censo<<" "<<ciu.estado<<"\n";
				}
				else{
					archsalida<<ciu.clave<<" "<<ciu.nombre<<" "<<ciu.departamento<<" "<<ciu.censo<<" "<<ciu.estado;
				}
			}
			archsalida.close();
		}
		/** 
			@brief Funcion que modifica una ciudad
			@param nueva, city que corresponde a la nueva informacion que tendra la ciudad
		*/
		void modificarCiudad(city nueva){
			city *ciu = arbolCiudades->retornarEstructura(nueva.clave);
			ciu->nombre = nueva.nombre;
			ciu->departamento = nueva.departamento;
			ciu->censo = nueva.censo;
		}
};
ciudad* ciudad::instance = 0;
#endif
