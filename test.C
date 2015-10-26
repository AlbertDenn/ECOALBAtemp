/* 

TEST DE USO DE DATOS DE BIENES Y COMPAÑIAS

*/

#include <stdio.h>
#include <dbQuery.H>
#include <string>
#include "fetchDB.H"

# include <tpl_dynArray.H>
# include <tpl_agraph.H>


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

	DynArray<Productos> nodo;

	std::cout<<std::endl<<std::endl<<"BUSCANDO UNIDAD ECONOMICA o PRODUCTO: "<<rif<<" "<<"CON DATOS DE ANIO:"<<anho<<std::endl;
	nodo=f.search(rif,anho);

	
////////////TEST FROM HERE//////////////

		
		bool found=0;

	   for(DynArray<Productos>::Iterator it(nodo);it.has_curr();it.next())
	    {

			if(!it.get_curr().insumos.is_empty())
			std::cout<<std::endl<<std::endl<<"----------PRODUCTO:"<<it.get_curr().nombre<<std::endl<<std::endl;

		for(DynArray<Insumos>::Iterator it1(it.get_curr().insumos);it1.has_curr();it1.next())
			{

			std::cout<<std::endl<<"***INSUMO:"<<it1.get_curr().nombre<<std::endl;
			for(DynArray<Proveedores>::Iterator it2(it.get_curr().subue.proveedores);it2.has_curr();it2.next())
				{

					if(it1.get_curr().id==it2.get_curr().insumoID)
					{
					std::cout<<std::endl<<"- ORIGEN: "<<it2.get_curr().nombre<<" -- "<<it2.get_curr().paisOrigen<<std::endl; 
					found=1;
					}
			
				}

			 if(found==0)
			 std::cout<<std::endl<<"- ORIGEN: NO INFO"<<std::endl;
			else
			found=0;
			
 			  
			}

	    }

	if(nodo.is_empty())
	std::cout<<std::endl<<"NO SE ENCONTRARON PRODUCTOS CON IMPORTACIONES ASOCIADOS ESE RIF - PRODUCTO"<<std::endl<<std::endl;

	
	std::cout<<std::endl<<std::endl<<"--------------------------------"<<std::endl;


	


	cout<<endl;


return 0;


}
