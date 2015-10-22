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

	Data nodo;

	nodo=f.search(rif,anho);
	std::cout<<std::endl<<std::endl<<"BUSCANDOO UNIDAD ECONOMICA o PRODUCTO: "<<rif<<" "<<"CON DATOS DE ANIO:"<<anho<<std::endl;
	
////////////TEST FROM HERE//////////////


	std::cout<<std::endl<<"----------RESULTS----------"<<std::endl<<std::endl;

	for(DynArray <Productos>::Iterator it(nodo.productos);it.has_curr();it.next())
	{
		std::cout<<std::endl<<"Product: "<<it.get_curr().nombre<<std::endl<<"Plant: "<<it.get_curr().subue.nombre<<std::endl<<"Company: "<<it.get_curr().subue.ue.nombre<<std::endl<<"REQUIRES:";
	   for(DynArray<Arco>::Iterator it0(it.get_curr().arcs);it0.has_curr();it0.next())

		for(DynArray<Productos>::Iterator it1(nodo.insumos);it1.has_curr();it1.next())
		{

			if(it0.get_curr().insumoID==it1.get_curr().id)
			std::cout<<std::endl<<"--"<<it1.get_curr().nombre;
		
		}
		
		std::cout<<std::endl<<std::endl<<"--------------------------------"<<std::endl;


	}


	cout<<endl;


return 0;


}
