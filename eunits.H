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
 
 Definicion de los datos utilizados por las Unidades Economicas
*/


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

