/**
	@file departamento.h
	@brief clase que gestiona el archivo candidato
	
	este archivo posee todos los metodos necesarios para el manejo de la informacion de los departamentos  
	
	@author Andres Arias & Isabel Perez
	
	@date 8/05/2018,28/05/2018
*/

#ifndef DEPARTAMENTO
#define DEPARTAMENTO
#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "Librerias/arbolAVL.h"
#include "facade.h"
#include <stddef.h>
#include "estructuras.h"


//esta clase gestiona la lectura, escritura y la busqueda en el archivo departamentos.txt
class departamento: public facade{
	private:
		//arbol AVL
		arbolAVL<departament> *arbolDep;
		//instancia unica
		static departamento *instance;
		//constructor privado
		/** 
			@brief metodo constructor 
			@returns lectura de registros 
		*/
		departamento(){
			arbolDep = new arbolAVL<departament>();
			this->cantidad = 0;
			this->leido = false;
			leerRegistros();
		}
		/**
		@brief funcion para leer los registros del archivo 
		*/
		void leerRegistros(){
			if(leido==false){
				int clave;
				string nombre;
				departament dep;
				//archivo de entrada
				ifstream archEntrada("Archivos/departamentos.txt", ios::in);
				if (!archEntrada.good()){
				 	cerr << "No se pudo abrir el archivo departamentos" << endl;
			    	exit(1);
				}
				while(!archEntrada.eof()) {
					archEntrada >> clave;
					archEntrada >> nombre;
					dep.clave = clave;
					dep.nombre = nombre;
					agregarDepartamento(dep);
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
		static departamento *getInstance(){
			if(instance==0){
				instance = new departamento();
			}
			return instance;
		}
		/** 
			@brief Funcion que agregar un departamento al arbol AVL
			@param clave int, corresponde al codigo de la ciudad que se desea consultar
		*/
		void agregarDepartamento(departament dep){
			dep.clave = ++this->cantidad;
			arbolDep->agregar(dep);
		}
		/**
		@brief funcion para obtener el nombre de un departamento dada su clave
		@returns string con nombre del departamento dada su clave
		@param clave int
		*/
		string getNombreDepartamento(int clave){
			return arbolDep->retornarEstructura(clave)->nombre;
		}
		/** 
			@brief Funcion que retona una lista con todos los departamentos
			@returns lista Lista<departament>
		*/
		Lista<departament> *consultarDepartamentos(){
			return arbolDep->recorridoInOrden();
		}
		/** 
			@brief Funcion libera memoria, eliminando los atributos de la clase
		*/
		void liberar(){
			delete arbolDep;
			delete instance;
		}
		/** 
		@brief funcion para agregar una ciudad a su departamento correspondiente
		@param clave int 
		@param ciudad city*
		
		*/
		void agregarCiudad(int clave,city *ciudad){
			departament *dep = arbolDep->retornarEstructura(clave);
			dep->cities.anadir_final(ciudad); 
		}
		/** 
		@brief funcion para consultar obtener todos los departamentos
		@returns apuntador a una lista del tipo departamento
		*/
		Lista<departament> *consultarDepartamento(){
			Lista<departament> *lista = arbolDep->recorridoInOrden();
			return lista;
		}
		/** 
		@brief funcion para obtener los votos presidenciales de un departamento dada su clave
		@returns lista del tipo long long con los votos presidenciales del departamento
		@param clave int
		@param votosciudades lista de listas del tipo long long
		*/
		Lista <long long> getVotosPbyDepartamento(int clave, Lista <Lista <long long> > votosciudades){
			departament *dep=arbolDep->retornarEstructura(clave);
			Lista <city*> ciudades = dep->cities;
			Lista <Lista <long long> > votosciudep;
			Lista <long long> totales;
			long long total;
			int i;
			int j;
			for (i=0;i<ciudades.getTam();i++){	
				votosciudep.anadir_final(votosciudades.devolverDato(ciudades.devolverDato(i)->clave));	
			}
			for (i=0;i<votosciudep.devolverDato(i).getTam();i++){
				total=0;
				for(j=0;j<votosciudep.getTam();j++){
					total+=votosciudep.devolverDato(j).devolverDato(i);
				}
				totales.anadir_final(total);
			}
			return totales;
		}
		/** 
		@brief fucion para obtener los votos presidenciales de todos los departamentos
		@returns lista de listas del tipo long long con todos los votos presidenciales por departamento
		@param votosciudades lista de listas del tipo long long
		*/
		Lista <Lista<long long> > getVotosDepartamentos (Lista <Lista<long long> > votosciudades){
			Lista <departament> *departamentos=consultarDepartamento();
			Lista <Lista <long long> > lista;
			Lista <long long> auxiliar;
			int i;
			for (i=1;i<=departamentos->getTam();i++){
				auxiliar=getVotosPbyDepartamento(i,votosciudades);
				lista.anadir_final(auxiliar);
			}
			return lista;
		}
		/** 
		@brief funcion para obtener los porcentajes de los votos presidenciales de todos los departamentos
		@returns lista de listas del tipo float con los porcentajes de los votos presidenciales de todos los departamentos
		@param votosdepartamento lista de listas del tipo long long
		*/
		Lista <Lista<float> > getPorcentajesDepartamentos (Lista <Lista <long long> > votosdepartamento ){
			int i;
			int j;
			Lista <Lista <float> > porcentajes;
			double porcentaje=0.0;
			for (i=0;i<votosdepartamento.getTam();i++){
				Lista <float>  auxiliar;
				for(j=0;j<votosdepartamento.devolverDato(i).getTam();j++){
					porcentaje= ((float)votosdepartamento.devolverDato(i).devolverDato(j)/(float)getCensobyDepartamento(i+1))*100.0;
					auxiliar.anadir_final(porcentaje);
				}
				porcentajes.anadir_final(auxiliar);
			}
			return porcentajes;
		}
		/** 
		@brief funcion para obtenr censo de un departamento dada su clave
		@returns long long con censo por departamento dada su clave
		@param op int
		*/
		long long getCensobyDepartamento(int op){
			int i;
			long long censo=0;
			departament *dep=arbolDep->retornarEstructura(op);
			Lista <city*> ciudades = dep->cities;
			for (i=0;i<ciudades.getTam();i++){
				censo+=ciudades.devolverDato(i)->censo;	
			}
			return censo;
		}
		/** 
		@brief funcion para obtener votos presidenciales para hombre y para mujer por departamento
		@returns lista de listas del tipo long long con los votos presidenciales para hombre y mujer por departamento
		@param votos lista de listas del tipo long long
		@param candidatos lista del tipo canddidate*
		*/
		Lista <Lista <long long> > votosDepartamentoSexo (Lista <Lista <long long> >votos, Lista <candidate*>candidatos){
			Lista<Lista <long long> > votosSexo;
			candidate *can;
			int i;
			int j;
			long long mujeres;
			long long hombres;
			for (i=0;i<votos.getTam();i++){
				Lista<long long> auxiliar;
				hombres=0;
				mujeres=0;
				for(j=0;j<candidatos.getTam();j++){
					can=candidatos.devolverDato(j);
					if(can->sexo.compare("Mujer")==0){
						mujeres+=votos.devolverDato(i).devolverDato(j);
					}
					else{
						hombres+=votos.devolverDato(i).devolverDato(j);
					}
				}
				
				auxiliar.anadir_final(mujeres);
				auxiliar.anadir_final(hombres);
				votosSexo.anadir_final(auxiliar);
			}
			return votosSexo;
		}
		/** 
			@brief Funcion que retona la cantidad de regisitros de departamentos
			@returns retornar un 'int' con la cantidad de regisitros
		*/
		int getCantidad(){
			return this->cantidad;
		}
		/** 
			@brief Funcion que reescribe el archivo departamentos.txt con las modificaciones realizadas
		*/
		void escribirRegistros(){
			ofstream archsalida("Archivos/departamentos.txt",ios::out|ios::trunc);
			if (!archsalida.good()){
				cerr << "No se pudo abrir el archivo departamentos" << endl;
				exit(1);
			}
			Lista<departament> departamentos = *arbolDep->recorridoInOrden();
			for(int i=0;i<departamentos.getTam();i++){
				departament dep = departamentos.devolverDato(i);
				if(i!=departamentos.getTam()-1){
					archsalida<<dep.clave<<" "<<dep.nombre<<"\n";
				}
				else{
					archsalida<<dep.clave<<" "<<dep.nombre;
				}
			}
			archsalida.close();
		}
		departament getDepartamento(int clave){
			return *arbolDep->retornarEstructura(clave);
		}
};
departamento* departamento::instance = 0;
#endif
