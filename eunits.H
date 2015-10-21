# ifndef EUNITS_H
# define EUNITS_H

#include <stdio.h>
#include <string>
#include <tpl_dynDlist.H>


struct Proveedores
{
	std::string id, nombre, rif, paisProcedencia, paisOrigen;
	
};

struct SubUnidadE
{
	std::string id, ue_rif, nombre, estado, capacidadu;
	DynDlist<Proveedores> proveedores;

};

struct UnidadEconomica
{
	std::string rif, nombre;
};


struct Medida
{
	std::string nombre, descripcion;
};

struct Productos
{

	std::string id, cod_aran, nombre, cantidad,anho,marca;

};

struct NodoProductivo
{
	UnidadEconomica unidadEconomica;
	DynDlist<SubUnidadE> subUnidadesEconomicas;
	DynDlist<Productos> productos;
	DynDlist<Productos> insumos;

};


#endif

