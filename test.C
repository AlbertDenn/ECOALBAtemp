/* 

TEST DE USO DE DATOS DE BIENES Y COMPAÑIAS

*/

#include <stdio.h>
#include <dbQuery.H>
#include <string>
#include "fetchDB.H"

# include <tclap/CmdLine.h>

# include <tpl_dynArray.H>

#include <grafo.H>
#include <fstream>


int main(int argc, char *argv[])
{

	if(argc<2)
	{
	std::cout<<"ERROR, DEBE INGRESAR RIF"<<endl;
	return 0;
	}

	std::string rif;
	rif=argv[1];

	Fetch f("file.txt");


////////////TEST FROM HERE//////////////

	f.unidades_economicas(rif);
	ofstream file ("Mapa.txt");
	
	Mapa mapa(2013, "unidad_economica.csv", "unidadecon_socio.csv",
   	    "unidadecon_subunidad_economica.csv",
   	    "produccion_producto.csv", "produccion_insumo.csv", 
   	    "cmproveedores_proveedorinsumo.csv", "cmproveedores_proveedor.csv",
   	    "produccion_producto_t_insumo.csv");

	mapa.save(file);

	
	


return 0;


}
