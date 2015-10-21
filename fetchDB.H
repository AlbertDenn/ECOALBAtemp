# ifndef FETCHDB_H
# define FETCHDB_H

#include <stdio.h>
#include <iostream>
#include <dbQuery.H>
#include <string>
#include <fstream>
#include <locale>
#include <eunits.H>
#include <tpl_dynDlist.H>

class Fetch{

	
	private:

	DBConnection connection;
	DBQuery *q = new DBQuery(connection);
	std::string query;

	UnidadEconomica uEconomica;
	DynDlist <SubUnidadE> subUEconomicas;
	DynDlist <Proveedores> proveedores;
	DynDlist <Productos> produccionProductos;
	DynDlist <Productos> produccionInsumos;
	NodoProductivo nodo;	



	public:

	Fetch();

	~Fetch(){}

	Fetch(std::string);

	NodoProductivo search(std::string,std::string);
	void unidades_economicas(std::string);
	void subUnidadesEconomicas(std::string);
	void produccion_producto(std::string,std::string);
	void produccion_insumo(std::string,std::string);
	void proveedor(std::string);
	UnidadEconomica get_unidadEconomica();
	DynDlist<SubUnidadE> get_subunidades();
	DynDlist<Productos> get_productos();
	DynDlist<Proveedores> get_proveedores();



	};


/*UnidadEconomica Fetch::get_unidadEconomica() {return uEconomica;}
DynDlist<SubUnidadE> Fetch::get_subunidades() {return subUEconomicas;}
DynDlist<Productos> Fetch::get_productos(){return produccionProductos;}
DynDlist<Proveedores> Fetch::get_proveedores(){return proveedores;}
*/

Fetch::Fetch() {

	query=" ";
	
}

Fetch::Fetch(const std::string name)
{
	connection.open(name.c_str());
}


NodoProductivo Fetch::search(std::string rif, std::string anho)
{
	std::locale loc;
   	rif[0]=std::toupper(rif[0],loc);

	if(rif[0]=='j'||rif[0]=='J')
	{
		unidades_economicas(rif);
		subUnidadesEconomicas(rif);
	}
		
	else
	{
		q->clear();
		query=(std::string)"SELECT unidadecon_subunidad_economica.ue_rif FROM unidadecon_subunidad_economica, produccion_producto WHERE produccion_producto.codigo_aran_id="+"\'"+rif+"\'";
		q->exec(query);
		q->reset();


		if(q->hasResult())
			while(q->hasResult() && q->next())
				{
				unidades_economicas(q->getValue(0));
				subUnidadesEconomicas(q->getValue(0));
				}
	}


	for(DynDlist<SubUnidadE>::Iterator it(subUEconomicas);it.has_curr();it.next())
		{
		produccion_producto(it.get_curr().id,anho);
		produccion_insumo(it.get_curr().id,anho);
		proveedor(it.get_curr().id);
		it.get_curr().proveedores= proveedores;
		proveedores.empty();
		}

	nodo.unidadEconomica=uEconomica;
	nodo.subUnidadesEconomicas=subUEconomicas;
	nodo.productos=produccionProductos;
	nodo.insumos=produccionInsumos;

	return nodo;
	


}

void Fetch::unidades_economicas(std::string rif)
{

   	std::size_t found;
	std::string nombre,query;
	
	q->clear();
	query=(std::string)"SELECT rif, nombre FROM unidadecon_unidad_economica WHERE rif="+"\'"+rif+"\'";
	
	q->exec(query);
	q->reset();

	while(q->hasResult() && q->next())
	{
	 rif=q->getValue(0);
	 nombre=q->getValue(1);

         
	found=nombre.find("\"");

	while(found!=std::string::npos)
	{

	 nombre.erase(found,1);
	 found=nombre.find("\"");
	
	}
		uEconomica.rif=rif;
		uEconomica.nombre=nombre;	

	}
	q->clear();

	
}


void Fetch::subUnidadesEconomicas(std::string search)
{

	std::size_t found;
	std::string capacidad, nombre, id, entidad;
	SubUnidadE su;
	q->clear();
	query=(std::string)"SELECT unidadecon_subunidad_economica.id, unidadecon_subunidad_economica.nombre, unidadecon_subunidad_economica.capacidad_utilizada, comun_entidad.nombre FROM  unidadecon_subunidad_economica, comun_parroquia,comun_municipio,comun_entidad WHERE unidadecon_subunidad_economica.parroquia_codigo = comun_parroquia.codigo AND comun_parroquia.municipio_codigo=comun_municipio.codigo AND comun_municipio.entidad_codigo = comun_entidad.codigo AND unidadecon_subunidad_economica.tipo_subunidad ='Pl' AND unidadecon_subunidad_economica.ue_rif="+"\'"+search+"\'";
	q->exec(query);

	q->reset();

	if(q->hasResult())
	{
	
		while(q->hasResult() && q->next())
		{

		 id=q->getValue(0);
          	 nombre=q->getValue(1);
		 capacidad=q->getValue(2);
		 entidad=q->getValue(3);
        
 		 found=nombre.find("\"");

			while(found!=std::string::npos)
			{
	
			 nombre.erase(found,1);
			 found=nombre.find("\"");
		
			}
	
		
		su.id=id;
		su.nombre=nombre;
		su.capacidadu=capacidad;
		su.estado=entidad;

		subUEconomicas.insert(su);
			
		}

	}
	

	q->clear();

		
}



void Fetch::produccion_producto(std::string search,std::string anho)
{

	std::size_t found;
	std::string cantidad, nombre, id,anio,cantproducido,codigo,marca;
	Productos p;

	q->clear();
	query=(std::string)"SELECT  produccion_producto.id, produccion_producto.nombre, produccion_producto.codigo_aran_id, produccion_producto.marca, produccion_produccion_precios_r.cant_producido, produccion_produccion_precios_r.anho FROM produccion_producto, produccion_produccion_precios_r WHERE produccion_producto.id = produccion_produccion_precios_r.producto_id_id AND produccion_producto.planta_id_id  ="+"\'"+search+"\'";
	q->exec(query);
	q->next();
	
	anio=q->getValue(5);
	anio=anio.substr(0,4);

	q->reset();
	if(q->hasResult())
	{
	
		while(q->hasResult() && q->next())
		{

		 id=q->getValue(0);
          	 nombre=q->getValue(1);
		 codigo=q->getValue(2);
		 marca=q->getValue(3);
		 cantidad=q->getValue(4);
        
 		 found=nombre.find("\"");

			while(found!=std::string::npos)
			{
	
			 nombre.erase(found,1);
			 found=nombre.find("\"");
		
			}
	
		if(anho==anio)
		{		
		p.id=id;
		p.nombre=nombre;
		p.cod_aran=codigo;
		p.cantidad=cantidad;
		p.marca=marca;
		p.anho=anio;
		produccionProductos.insert(p);
		
		}
	
		
		}

	}
	

	q->clear();


}

void Fetch::produccion_insumo(std::string search,std::string anho)
{

	std::size_t found;
	std::string cantidad, nombre, id,anio,codigo,marca;
	Productos p;

	q->clear();
	query=(std::string)"SELECT produccion_insumo.id, produccion_insumo.nombre, produccion_insumo.codigo_aran_id, produccion_insumo.marca, produccion_consumo_precios_r.cant_comp_imp, produccion_consumo_precios_r.anho FROM produccion_insumo, produccion_consumo_precios_r, unidadecon_subunidad_economica WHERE produccion_insumo.id=produccion_consumo_precios_r.insumo_id_id AND produccion_insumo.planta_id_id = unidadecon_subunidad_economica.id AND produccion_insumo.planta_id_id=\'"+search+"\'";
	q->exec(query);

	std::cout<<std::endl<<std::endl<<q->hasResult()<<std::endl<<std::endl;	

	std::cout<<std::endl<<anio<<std::endl;

	q->reset();
	if(q->hasResult())
	{
	
		while(q->hasResult() && q->next())
		{

		 id=q->getValue(0);
          	 nombre=q->getValue(1);
		 codigo=q->getValue(2);
		 marca=q->getValue(3);
		 cantidad=q->getValue(4);
        
 		 found=nombre.find("\"");

			while(found!=std::string::npos)
			{
	
			 nombre.erase(found,1);
			 found=nombre.find("\"");
		
			}
	
		if(anho==anio)
		{		
		p.id=id;
		p.nombre=nombre;
		p.cod_aran=codigo;
		p.cantidad=cantidad;
		p.marca=marca;
		p.anho=anio;
		produccionInsumos.insert(p);
		
		}
	
		
		}

	}
	

	q->clear();


}



void Fetch::proveedor(std::string search)
{

	
	
	std::size_t found;
	std::string id, nombre, rif, paisProcedencia, paisOrigen;
	Proveedores p;

	q->clear();																																																								//AND cmproveedores_proveedor.pais_origen_id <> 23
	query=(std::string)"SELECT cmproveedores_proveedor.id, cmproveedores_proveedor.nombre, cmproveedores_proveedor.rif, comun_pais.nombre, comun_pais.nombre FROM cmproveedores_proveedor, comun_pais, cmproveedores_proveedorinsumo, unidadecon_subunidad_economica, produccion_insumo WHERE cmproveedores_proveedor.pais_origen_id=comun_pais.id AND cmproveedores_proveedor.pais_procedencia_id = comun_pais.id AND cmproveedores_proveedor.pais_origen_id <> 23 AND produccion_insumo.id= cmproveedores_proveedorinsumo.insumo_id_id AND cmproveedores_proveedorinsumo.proveedor_id_id=cmproveedores_proveedor.id AND unidadecon_subunidad_economica.id=cmproveedores_proveedor.planta_id_id AND unidadecon_subunidad_economica.id ="+"\'"+search+"\' ";
	q->exec(query);
	q->next();
	
	if(q->hasResult())
	{
	
		while(q->hasResult() && q->next())
		{

		 id=q->getValue(0);
          	 nombre=q->getValue(1);
		 rif=q->getValue(2);
		 paisOrigen=q->getValue(3);
		 paisProcedencia=q->getValue(4);
        
 		 found=nombre.find("\"");

			while(found!=std::string::npos)
			{
	
			 nombre.erase(found,1);
			 found=nombre.find("\"");
		
			}
	
				
		p.id=id;
		p.nombre=nombre;
		p.rif=rif;
		p.paisOrigen=paisOrigen;
		p.paisProcedencia=paisProcedencia;
		proveedores.insert(p);
	
		}

	}
	

	q->clear();


}

#endif



