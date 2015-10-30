/* 

TEST DE USO DE DATOS DE BIENES Y COMPAÑIAS

*/

#include <stdio.h>
#include <dbQuery.H>
#include <string>
#include "fetchRoot.H"
//#include "DownRiver.H"
#include "bloomFilter.H"

#include <tpl_dynArray.H>
#include <tpl_dynDlist.H>

#include <stdlib.h>

#include <pthread.h>

#include <fstream>



/*
struct Data
{

char *rif,*anho;

};

void* rootfind(void *ptr)
{

Data *d= (struct Data*) ptr;

FetchByCode root("file.txt");

DynDlist<Productos> *P;

P=new DynDlist<Productos>(root.search(d->rif,d->anho));

pthread_exit((void *)P);


}

void* downriverfind(void *ptr)
{

Data *d= (struct Data*) ptr;

DownRiver node("file.txt");

DynDlist<Productos> *P;

P=new DynDlist<Productos>(node.search(d->rif,d->anho));

pthread_exit((void *)P);


}

*/

using NodeLevel = tuple<DynDlist<Productos>, size_t>;
using Chain = DynDlist<NodeLevel>;

int main(int argc, char *argv[])
{

	if(argc<4)
	{
	std::cout<<"usage: "<<"./test [RIF] [ANHO] [LEVELSDOWN]"<<endl;
	return 0;
	}

	char* rif,*anho;
	size_t lvldown=(size_t)atoi(argv[3]);
	rif=argv[1];
	anho=argv[2];
	
	DynDlist<UnidadEconomica> nodo;

	Chain cadena;

////////////TEST FROM HERE//////////////

	FetchRoot root("file.txt");

//	DownRiver dr("file.txt");
	std::locale loc;
   	rif[0]=std::toupper(rif[0],loc);
	std::string val;

	if(rif[0]=='j'||rif[0]=='J')
	{
	std::cout<<std::endl<<"BUSQUEDA POR RIF"<<std::endl;
	nodo=root.searchByRif(rif,anho);

	
		if(nodo.size()==0)
		std::cout<<"---SIN RESULTADO---"<<std::endl<<std::endl;

	}
	else
	{

	std::cout<<std::endl<<"BUSQUEDA POR PRODUCTO"<<std::endl;
	nodo=root.searchByCode(rif,anho);

		if(nodo.size()==0)
		std::cout<<"---SIN RESULTADO---"<<std::endl<<std::endl;
	}
	
	
	for(DynDlist<UnidadEconomica>::Iterator it(nodo);it.has_curr();it.next())
	{
	
	if(!it.get_curr().productos.is_empty() && !it.get_curr().productos[0].insumos.is_empty())
	{
	std::cout<<std::endl<<"EMPRESA: "<<it.get_curr().nombre<<std::endl;
	
		for(DynDlist<Productos>::Iterator it1(it.get_curr().productos);it1.has_curr();it1.next())
		{
		  std::cout<<std::endl<<"<<PRODUCTO:"<<it1.get_curr().nombre<<" "<<it1.get_curr().cantidad<<">>"<<std::endl;

			/* std::cout<<std::endl<<"Insumos del Producto:"<<std::endl;


			 for(DynDlist<Insumos>::Iterator it2(it1.get_curr().insumos);it2.has_curr();it2.next())
			{

			  for(DynDlist<Proveedor>::Iterator it3(std::get<1>(it2.get_curr()));it3.has_curr();it3.next())
				std::cout<<endl<<get<0>(it2.get_curr()).nombre<<" -- "<<it3.get_curr().nombre<<" "<<it3.get_curr().paisOrigen<<std::endl;
			}*/

		}

	}

	}


	return 0;


}
