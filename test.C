/* 

TEST DE USO DE DATOS DE BIENES Y COMPAÑIAS

*/

#include <stdio.h>
#include <dbQuery.H>
#include <string>
#include "fetchByCode.H"
#include "fetchByRif.H"
#include "DownRiver.H"
#include "bloomFilter.H"

# include <tpl_dynArray.H>
# include <tpl_agraph.H>
#include <tpl_dynSetHash.H>
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

DynArray<Productos> *P;

P=new DynArray<Productos>(root.search(d->rif,d->anho));

pthread_exit((void *)P);


}

void* downriverfind(void *ptr)
{

Data *d= (struct Data*) ptr;

DownRiver node("file.txt");

DynArray<Productos> *P;

P=new DynArray<Productos>(node.search(d->rif,d->anho));

pthread_exit((void *)P);


}

*/

int main(int argc, char *argv[])
{

	if(argc<3)
	{
	std::cout<<"usage: "<<"./test [RIF] [ANHO]"<<endl;
	return 0;
	}

	char* rif,*anho;
	rif=argv[1];
	anho=argv[2];
	DynArray<Productos> raiz,nodoDown;

	bloom_parameters parameters;
	parameters.projected_element_count = 5000;
	parameters.false_positive_probability = 0.0001; // 1 in 10000
	parameters.compute_optimal_parameters(); 
	bloom_filter filtro(parameters);

/*
using Net = Array_Graph<Graph_Anode<DynArray<Productos>>, Graph_Aarc<unsigned int>>;	
DynArray<Net::Node*> nodes;
Net red;
	

//SEPARAR EN HILOS DE EJECUCION DIFERENTES

pthread_t thread;
d.rif=rif;
d.anho=anho;
void *something;
pthread_create(&thread,NULL, rootfind, (void*) &d);
pthread_join(thread,&something);
p1=(DynArray<Productos>*)something;
*/


////////////TEST FROM HERE//////////////



	std::locale loc;
   	rif[0]=std::toupper(rif[0],loc);
	std::string val;

	if(rif[0]=='j'||rif[0]=='J')
	{
	std::cout<<std::endl<<"BUSQUEDA POR RIF"<<std::endl;
	FetchByRif root("file.txt");
	raiz=root.search(rif,anho);
	if(raiz.size()==0)
	std::cout<<"---SIN RESULTADO---"<<std::endl<<std::endl;
	}
	else
	{
	std::cout<<std::endl<<"BUSQUEDA POR PRODUCTO"<<std::endl;
	FetchByCode root("file.txt");
	raiz=root.search(rif,anho);
	if(raiz.size()==0)
	std::cout<<"---SIN RESULTADO---"<<std::endl<<std::endl;
	}
	
	if(raiz.size()!=0)
	std::cout<<std::endl<<std::endl<<"-------------ROOT-------------"<<std::endl;


	for(DynArray<Productos>::Iterator it(raiz);it.has_curr();it.next())
	{
		std::cout<<std::endl<<it.get_curr().nombre<<" -- "<<it.get_curr().id<<" -- "<<it.get_curr().cod_aran<<" -- "<<it.get_curr().subue.ue.nombre<<std::endl;
		filtro.insert(it.get_curr().nombre);
	}


	std::cout<<std::endl<<std::endl<<"----------------DOWN RIVER LEVEL ONE----------------"<<std::endl;
	
	
	DownRiver dr("file.txt");
	nodoDown=dr.search(anho,raiz,filtro);
	
	for(DynArray<Productos>::Iterator it(nodoDown);it.has_curr();it.next())
	{
		std::cout<<std::endl<<it.get_curr().nombre<<" -- "<<it.get_curr().id<<" -- "<<it.get_curr().subue.ue.nombre<<std::endl;
		if(!filtro.contains(it.get_curr().nombre))
		filtro.insert(it.get_curr().id);
	}

	std::cout<<std::endl<<nodoDown.size()<<std::endl;
	
	std::cout<<std::endl<<std::endl<<"----------------DOWN RIVER LEVEL TWO----------------"<<std::endl;
	
	nodoDown=dr.search(anho,nodoDown,filtro);


	for(DynArray<Productos>::Iterator it(nodoDown);it.has_curr();it.next())
	{
		std::cout<<std::endl<<it.get_curr().nombre<<" -- "<<it.get_curr().cod_aran<<" -- "<<it.get_curr().subue.ue.nombre<<std::endl;

	}
	std::cout<<std::endl<<nodoDown.size()<<std::endl;

		std::cout<<std::endl<<"------------------------------------------"<<std::endl<<std::endl;

return 0;


}
