/*
 Copyright (C) 2015
  Albert Cortez (acortez@cenditel.gob.ve)
 
  CENDITEL Fundación Centro Nacional de Desarrollo e Investigación en
  Tecnologías Libres
 
  Este programa es software libre; Usted puede usarlo bajo los términos de la
  licencia de software GPL versión 2.0 de la Free Software Foundation.
 
  Este programa se distribuye con la esperanza de que sea útil, pero SIN
  NINGUNA GARANTÍA; tampoco las implícitas garantías de MERCANTILIDAD o
  ADECUACIÓN A UN PROPÓSITO PARTICULAR.
  Consulte la licencia GPL para más detalles. Usted debe recibir una copia
  de la GPL junto con este programa; si no, escriba a la Free Software
  Foundation Inc. 51 Franklin Street,5 Piso, Boston, MA 02110-1301, USA.
*/

/*
  Autor:             Argenis A. Cortez
  Fecha de creación: 15/10/2015
 
 Programa que ejecuta una busqueda por RIF o por Codigo Arancelario y crea un JSON con la informacion de Importacion
*/

#include <stdio.h>
#include <dbQuery.H>
#include <string>
#include "fetchRoot.H"
#include "bloomFilter.H"
#include <tpl_dynDlist.H>
#include <tpl_dynArray.H>
#include <stdlib.h>
#include <pthread.h>
#include <fstream>

using NodeLevel = std::tuple<DynDlist<Productos>, size_t>;
using Chain = DynDlist<NodeLevel>;

int main(int argc, char *argv[])
{

	if(argc<3)
	{
	std::cout<<"usage: "<<"./test [RIF] [ANHO]"<<std::endl;
	return 0;
	}

	char* rif,*anho;
	rif=argv[1];
	anho=argv[2];

	DynDlist<UnidadEconomica> nodoraiz;

	std::ofstream file;

	Chain cadena;

	FetchRoot root("file.txt");
	std::locale loc;
   	rif[0]=std::toupper(rif[0],loc);
	std::string val;
	file.open("outjson.json");


	if(rif[0]=='J'||rif[0]=='G')
	nodoraiz=root.searchByRif(rif,anho);
	else
	nodoraiz=root.searchByCode(rif,anho);


if(!nodoraiz.is_empty())
{

	file<<"[";

	for(DynDlist<UnidadEconomica>::Iterator it(nodoraiz);it.has_curr();)
	{

		if(!it.get_curr().productos.is_empty() && !it.get_curr().productos[0].insumos.is_empty())
		{
			file<<"{ \"Unidad Economica\":\""<<it.get_curr().nombre<<"\",\"RIF\":\""<<it.get_curr().rif<<"\",\"children\":["<<std::endl;
	
			for (DynDlist<Productos>::Iterator it1(it.get_curr().productos);it1.has_curr();it1.next())
			{
	
				file<<"{\"Producto\":\""<<it1.get_curr().nombre<<"\",\"Cantidad Producida\":\""<<it1.get_curr().cantidad<<"\",\"children\":["<<std::endl;
						
				for(DynDlist<Insumos>::Iterator it0(it1.get_curr().insumos);it0.has_curr();it0.next())
				{
	
				
					file<<"{\"Insumo\":\""<<std::get<0>(it0.get_curr()).nombre<<"\",\"Cantidad Requerida\":\""<<std::get<0>(it0.get_curr()).cantidad<<"\",\"children\":["<<std::endl;
				
					for(DynDlist<Proveedor>::Iterator it2(std::get<1>(it0.get_curr()));it2.has_curr();it2.next())
					{
					file<<"{\"Proveedor\":\""<<it2.get_curr().nombre<<"\",\"Pais\":\""<<it2.get_curr().paisProcedencia<<"\",\"children\":[]}";
	
					if(it2.get_pos()!=std::get<1>(it0.get_curr()).size()-1 && std::get<1>(it0.get_curr()).size()!=1)
					file<<","<<std::endl;
					
					
	
	
					}	
					if(it0.get_pos()!=it1.get_curr().insumos.size()-1)
					file<<"]},"<<std::endl;
					else
					file<<"]}"<<std::endl;
	
				}	
		
				if(it1.get_pos()!=it.get_curr().productos.size()-1)
				file<<"]},"<<std::endl;
				else
				file<<"]}"<<std::endl;

			}
			
		
			if(it.get_pos()!=nodoraiz.size()-1)
			file<<"]},"<<std::endl;
			else
			file<<"]}"<<std::endl;
	
			it.next();

		}
		else
		it.next();
	


	}
	
	file<<"]";
}
else
	file<<"[[\"ERROR\"]}"<<std::endl;
	std::cout<<"NOT FOUND"<<std::endl;
	file.close();
	std::cout<<std::endl<<std::endl;
	return 0;


}
