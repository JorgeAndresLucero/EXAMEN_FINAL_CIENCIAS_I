/**
	@file candidato.h
	@brief clase que gestiona el archivo candidato
	
	este archivo posee todos los metodos necesarios para el manejo de la informacion de los candidatos  
	
	@author Andres Arias & Isabel Perez
	
	@date 8/05/2018,28/05/2018
*/

#ifndef CANDIDATO
#define CANDIDATO
#include <string.h>
#include "Librerias/arbolAVL.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <time.h>
#include "estructuras.h"
#include "facade.h"
#include "ciudad.h"
#include "partido.h"

//esta clase gestiona la lectura, escritura y la busqueda en el archivo candidatos.txt
class candidato: public facade{
	private:
		//arbol avl
		arbolAVL<candidate> *arbolCandidatos;
		//instancia unica
		static candidato *instance;
		//constructor privado
		
			/** 
			@brief metodo constructor 
			@returns lectura de registros 
		*/
		candidato(){
			this->cantidad = 0;
			this->leido = false;
			arbolCandidatos = new arbolAVL<candidate>();
			leerRegistros();
		}
		/**
		@brief funcion para leer los registros del archivo 
		*/
		void leerRegistros(){
			int clave;
			string nombre;
			string apellido;
			long long cc;
			string sexo;
			string estadoCivil;
			string fechaNacimiento;
			int ciudadNatal;
			int ciudadResidencia;
			int partido;
			int territorio;
			int formulaVi;
			int estado;
			candidate can;
			this->cantidad = 0;
			//archivo de entrada
			ifstream archEntrada("Archivos/candidatos.txt", ios::in);
			if(!archEntrada.good()){
			 	cerr << "No se pudo abrir el archivo candidatos.txt" << endl;
    			exit(1);
			}
			//se leen los registros
			while(!archEntrada.eof()){
				archEntrada >> clave;
				archEntrada >> nombre;
				archEntrada >> apellido;
				archEntrada >> cc;
				archEntrada >> sexo;
				archEntrada >> estadoCivil;
				archEntrada >> fechaNacimiento;
				archEntrada >> ciudadNatal;
				archEntrada >> ciudadResidencia;
				archEntrada >> partido;
				archEntrada >> territorio;
				archEntrada >> formulaVi;
				archEntrada >> estado;
				can.clave = clave;
				can.nombre = nombre;
				can.apellido = apellido;
				can.cc = cc;
				can.sexo = sexo;
				can.estadoCivil = estadoCivil;
				can.fechaNacimiento = fechaNacimiento;
				can.ciudadNatal = ciudadNatal;
				can.ciudadResidencia = ciudadResidencia;
				can.partido = partido;
				can.territorio = territorio;
				can.formulaVi = formulaVi;
				can.estado = estado;
				this->insertarCandidato(can);
			}
			archEntrada.close();
		}
	public:
		/** 
		@brief metodo de instanciacion de la clase (Patron singleton)
		@returns devuelve la instancia de la clase
		
		*/
		static candidato* getInstance(){
			if(instance==0){
				instance = new candidato();
			}
			return instance;
		}
		/**
		@brief funcion para agregar una ciudad al arbol avl
		@param candidato candidate
		*/
		void insertarCandidato(candidate candidato){
			candidato.clave = ++this->cantidad;
			arbolCandidatos->agregar(candidato);
			candidate *can = arbolCandidatos->retornarEstructura(candidato.clave); 
			//se agrega el candidato a la ciudad correspondiente
			ciudad::getInstance()->agregarCandidato(can,candidato.territorio);
			//se agrega el candidato al partido correspondiente
			partido::getInstance()->agregarCandidato(can,candidato.partido);
		}
		/** 
		@brief funcion para obtener la cantidad de registros
		@returns cantidad de registros
		*/ 
		int getCantidad(){
			return this->cantidad;
		}
		/**
		@brief modifica un candidado
		@param clave int
		@param nuevo candidate
		*/
		void modificarCandidato(int clave, candidate nuevo){
			candidate *can = arbolCandidatos->retornarEstructura(clave);
			can->nombre = nuevo.nombre;
			can->apellido = nuevo.apellido;
			can->cc = nuevo.cc;
			can->estadoCivil =  nuevo.estadoCivil;
			can->fechaNacimiento = nuevo.fechaNacimiento;
			can->ciudadNatal = nuevo.ciudadNatal;
			can->estado = nuevo.estado;
		}
		/** 
		@brief fucion para obtener un candidato dada su clave 
		@returns apuntador a un candidato dado su codigo
		@param clave int 
		*/
		candidate *getCandidato(int clave){
			return arbolCandidatos->retornarEstructura(clave);
		}
		
		/** 
		@brief funcion para calcular la edad de un candidato dada su fecha de nacimiento
		@returns entero con edad dada en años
		@param fechanacimiento string
		@param yearactual int
		@param mesactual int
		@param diaactual int
		*/
		//calcula una edad dada una fecha de nacimiento
		int calcularedad(string fecha,int yearactual,int mesactual, int diaactual){
			char *nuevafecha=strdup(fecha.c_str());
			int dia,mes,year;
			char *ptr;
			ptr=strtok(nuevafecha,"/");
			dia=atoi(ptr);
			ptr=strtok(NULL,"/");
			mes=atoi(ptr);
			ptr=strtok(NULL,"/");
			year=atoi(ptr);	
			//calcular edad
			int dias,meses,years;
			years=yearactual-year;
			meses=mesactual-mes;
			dias=diaactual-dia;
	
			if (dias<0){
				meses--;
			}
	
			if(meses<0){
				years--;
			}
			return years;
		}
		/** 
		@brief funcion para eliminar (cambiar de estado) un candidato
		@param clave int
		*/
		void eliminarCandidato(int clave){
			candidate *can = arbolCandidatos->retornarEstructura(clave);
			can->estado = 0;
			//si se borra un candidato presidencial, se borra tambien su vicepresidente
			if(can->territorio==0 && can->formulaVi!=0){
				eliminarCandidato(can->formulaVi);
			} 
		}
		/** 
			@brief Funcion que libera memoria eliminando los atributos de la clase
		*/
		void liberar(){
			escribirRegistros();
			delete arbolCandidatos;
			delete instance;
		}
		/** 
			@brief Funcion que reescribe el archivo candidatos.txt con las modificaciones realizadas
		*/
		void escribirRegistros(){
			ofstream archsalida("Archivos/candidatos.txt",ios::out|ios::trunc);
			if (!archsalida.good()){
				cerr << "No se pudo abrir el archivo candidatos" << endl;
				exit(1);
			}
			Lista<candidate> candidatos = *arbolCandidatos->recorridoInOrden();
			for(int i=0;i<candidatos.getTam();i++){
				candidate can = candidatos.devolverDato(i);
				if(i!=candidatos.getTam()-1){
					archsalida<<can.clave<<" "<<can.nombre<<" "<<can.apellido<<" "<<can.cc<<" "<<can.sexo<<" "<<can.estadoCivil
					<<" "<<can.fechaNacimiento<<" "<<can.ciudadNatal<<" "<<can.ciudadResidencia<<" "<<can.partido<<" "<<can.territorio<<" "
					<<can.formulaVi<<" "<<can.estado<<"\n";
				}
				else{
					archsalida<<can.clave<<" "<<can.nombre<<" "<<can.apellido<<" "<<can.cc<<" "<<can.sexo<<" "<<can.estadoCivil
					<<" "<<can.fechaNacimiento<<" "<<can.ciudadNatal<<" "<<can.ciudadResidencia<<" "<<can.partido<<" "<<can.territorio<<" "
					<<can.formulaVi<<" "<<can.estado;
				}
				
			}
			archsalida.close();
		}
		/** 
			@brief Funcion que indica si un candidato esta eliminado
			@returns retorna un 'bool' dependiendo de si esta eliminado o no
			@param clave, int que corresponde al codigo del candidato
		*/
		bool estaDeshabilitado(int clave){
			candidate can = *arbolCandidatos->retornarEstructura(clave);
			if(can.estado==0){
				return true;
			}
			return false;
		}
};


candidato* candidato::instance = 0;
#endif
