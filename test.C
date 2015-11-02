/* 

TEST DE USO DE DATOS DE BIENES Y COMPAÑIAS

*/

#include <stdio.h>
#include <dbQuery.H>
#include <string>
#include "fetchRoot.H"
#include "bloomFilter.H"
#include <tpl_dynDlist.H>
#include <stdlib.h>
#include <pthread.h>
#include <fstream>

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
	
	DynDlist<UnidadEconomica> nodo, down;

	ofstream file;

	Chain cadena;

////////////TEST FROM HERE//////////////

	FetchRoot root("file.txt");

//	DownRiver dr("file.txt");
	std::locale loc;
   	rif[0]=std::toupper(rif[0],loc);
	std::string val;

	file.open("outjson.txt");


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
	std::cout<<std::endl<<"------EMPRESA: "<<it.get_curr().nombre<<std::endl;
	
		for(DynDlist<Productos>::Iterator it1(it.get_curr().productos);it1.has_curr();it1.next())
		std::cout<<std::endl<<"<<PRODUCTO:"<<it1.get_curr().nombre<<" "<<it1.get_curr().cantidad<<">>"<<std::endl;
		

			down=(DynDlist<UnidadEconomica>)it.get_curr();
		
			size_t i=0;
			while(i<lvldown)
			{
			down=root.searchUp(down,anho);

			if(!down.is_empty() && !down[0].productos.is_empty())
			{
			std::cout<<std::endl<<"/ / / / UP RIVER LEVEL "<<i+1<<std::endl;
			for(DynDlist<UnidadEconomica>::Iterator it(down);it.has_curr();it.next())
			if(!it.get_curr().productos.is_empty())
			for(DynDlist<Productos>::Iterator it5(it.get_curr().productos);it5.has_curr();it5.next())
			std::cout<<std::endl<<"<-->EMPRESA: "<<it.get_curr().nombre<<" :: "<<it5.get_curr().nombre<<std::endl;
			std::cout<<"--------------------"<<std::endl;
			}
			i++;
			}

	}

	}

	
	std::cout<<endl<<endl;
	return 0;


}
