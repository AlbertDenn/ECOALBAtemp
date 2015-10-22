# ifndef EUNITS_H
# define EUNITS_H

#include <stdio.h>
#include <string>
#include <tpl_dynArray.H>


struct Proveedores
{
	std::string id, nombre, rif, paisProcedencia, paisOrigen;
	
};


struct UnidadEconomica
{
	std::string rif, nombre;
};

struct SubUnidadE
{
	std::string id, ue_rif, nombre, estado, capacidadu;
	UnidadEconomica ue;
	DynArray<Proveedores> proveedores;

};

struct Medida
{
	std::string nombre, descripcion;
};


struct Arco
{
	std::string productoID, insumoID, cantidad;

};


struct Productos
{

	std::string id, cod_aran, nombre, cantidad,anho,marca;
	DynArray<Arco> arcs;
	SubUnidadE subue;

};

struct Data
{
	DynArray<Productos> productos;
	DynArray<Productos> insumos;


};




#endif

