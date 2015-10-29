# ifndef EUNITS_H
# define EUNITS_H

#include <stdio.h>
#include <string>
#include <tpl_dynArray.H>


struct Proveedores
{
	std::string insumoID, nombre, rif, paisProcedencia, paisOrigen;


};


struct UnidadEconomica
{
	std::string rif, nombre,razon;

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

struct Insumos
{
	std::string nombre, cod_aran, id, cantidad, marca, anho;


};

struct Productos
{

	std::string id="empty", cod_aran, nombre, cantidad,anho,marca;
	SubUnidadE subue;
	DynArray<Insumos> insumos;


};



#endif

