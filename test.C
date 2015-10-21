/* 

TEST DE USO DE DATOS DE BIENES Y COMPAÑIAS

*/

#include <stdio.h>
#include <dbQuery.H>
#include <string>
#include "fetchDB.H"

# include <tpl_dynArray.H>


#include <fstream>


int main(int argc, char *argv[])
{

	if(argc<3)
	{
	std::cout<<"usage: "<<"./test [RIF] [ANHO]"<<endl;
	return 0;
	}

	std::string rif, anho;
	rif=argv[1];
	anho=argv[2];

	Fetch f("file.txt");

	NodoProductivo nodo;

	nodo=f.search(rif,anho);
	std::cout<<std::endl<<std::endl<<"BUSCANDOO UNIDAD ECONOMICA o PRODUCTO: "<<rif<<" "<<"CON DATOS DE ANIO:"<<anho<<std::endl;
	
////////////TEST FROM HERE//////////////


	std::cout<<std::endl<<"----------RESULTS----------"<<std::endl<<std::endl;

		for(DynDlist <Productos>::Iterator it3(nodo.productos);it3.has_curr();it3.next())
		{
		std::cout<<"---PRODUCTOS---"<<std::endl<<std::endl;
		std::cout<<"Codigo:"<<it3.get_curr().cod_aran<<std::endl<<"Nombre:"<<it3.get_curr().nombre<<std::endl<<"Marca:"<<it3.get_curr().marca<<std::endl<<"Cantidad Producida:"<<it3.get_curr().cantidad<<std::endl<<"Anho:"<<it3.get_curr().anho<<std::endl<<std::endl;
		std::cout<<"UNIDAD ECONOMICA:  "<< nodo.unidadEconomica.nombre<<" "<<nodo.unidadEconomica.rif;
		std::cout<<std::endl<<"SUBUNIDAD ECONOMICA:  ";
			for(DynDlist<SubUnidadE>::Iterator it1(nodo.subUnidadesEconomicas);it1.has_curr();it1.next())
				std::cout<<it1.get_curr().nombre<<std::endl<<"Estado:"<<it1.get_curr().estado<<std::endl<<std::endl;
			
		}

	
	
	


return 0;


}
