# ifndef EUNITS_H
# define EUNITS_H

#include <stdio.h>
#include <string>
#include <tpl_dynDlist.H>


struct Proveedor
{
	std::string insumoID, nombre, rif, paisProcedencia, paisOrigen;
};

struct Imported
{
	std::string nombre, cod_aran, id, cantidad, marca, anho;
};


using Insumos = std::tuple<Imported,DynDlist<Proveedor>>;



struct Productos
{
	std::string id, cod_aran, nombre, cantidad,anho,marca;
	DynDlist<Insumos> insumos;
};

struct SubUnidadE
{
	std::string id, ue_rif, nombre, estado, capacidadu;
};

struct UnidadEconomica
{
	std::string rif, nombre,razon;
	SubUnidadE planta;
	DynDlist<Productos> productos;
};







#endif

