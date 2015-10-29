# ifndef FETCHBYRIF_H
# define FETCHBYRIF_H

#include <stdio.h>
#include <iostream>
#include <dbQuery.H>
#include <string>
#include <fstream>
#include <locale>
#include <eunits.H>
#include "bloomFilter.H"


class FetchByRif{

	
	private:

	DBConnection connection;
	DBQuery *q = new DBQuery(connection);
	std::string query;

	bloom_parameters parameters;
	DynArray <SubUnidadE> subUEconomicas;
	DynArray <Productos> produccionProductos;
	bloom_filter *filter;
		

	public:

	FetchByRif();

	~FetchByRif(){}

	FetchByRif(std::string);

	DynArray<Productos> search(std::string,std::string);
	UnidadEconomica unidades_economicas(std::string);
	void subUnidadesEconomicas(std::string);
	void produccion_producto(SubUnidadE, std::string);
	DynArray<Insumos> produccion_insumo(std::string);
	DynArray<Proveedores> proveedor(std::string,std::string);
	SubUnidadE retrieve_planta(std::string);
	
		
	};


FetchByRif::FetchByRif() {

	query=" ";
 	parameters.projected_element_count = 1000;
	parameters.false_positive_probability = 0.0001; // 1 in 10000
	parameters.compute_optimal_parameters(); 
	filter=new bloom_filter(parameters);

	
}

FetchByRif::FetchByRif(const std::string name)
{
	connection.open(name.c_str());

 	parameters.projected_element_count = 1000;
	parameters.false_positive_probability = 0.0001; // 1 in 10000
	parameters.compute_optimal_parameters(); 
	filter=new bloom_filter(parameters);

}

SubUnidadE FetchByRif::retrieve_planta(std::string id)
{
	DBQuery f=*q;

	f.clear();
	query=(std::string)"SELECT unidadecon_subunidad_economica.id, unidadecon_subunidad_economica.nombre, unidadecon_subunidad_economica.capacidad_utilizada, comun_entidad.nombre FROM  unidadecon_subunidad_economica, comun_parroquia,comun_municipio,comun_entidad, produccion_insumo WHERE unidadecon_subunidad_economica.parroquia_codigo = comun_parroquia.codigo AND comun_parroquia.municipio_codigo=comun_municipio.codigo AND comun_municipio.entidad_codigo = comun_entidad.codigo AND produccion_insumo.planta_id_id=unidadecon_subunidad_economica.id AND produccion_insumo.id='"+id+"'";
	f.exec(query);

	f.next();
	SubUnidadE su;
		su.id=f.getValue(0);
		su.nombre=f.getValue(1);
		su.capacidadu=f.getValue(2);
		su.estado=f.getValue(3);

	return su;
	
}

DynArray<Productos> FetchByRif::search(std::string rif, std::string anho)
{
		subUnidadesEconomicas(rif);

		for(DynArray<SubUnidadE>::Iterator it(subUEconomicas);it.has_curr();it.next())
		produccion_producto(it.get_curr(),anho);		

		for(DynArray<Productos>::Iterator it(produccionProductos);it.has_curr();it.next())
		{
		it.get_curr().insumos=produccion_insumo(it.get_curr().id);
       		it.get_curr().subue.proveedores=proveedor(it.get_curr().subue.id,anho);
		}
		

		
return produccionProductos;
}

UnidadEconomica FetchByRif::unidades_economicas(std::string rif)
{
	DBQuery f=*q;

   	std::size_t found;
	std::string nombre, razon,query;
	UnidadEconomica UE;
	
	f.clear();
	query=(std::string)"SELECT rif, nombre,razon_social FROM unidadecon_unidad_economica WHERE rif="+"\'"+rif+"\'";
	
	f.exec(query);
	f.reset();

	while(f.hasResult() && f.next())
	{
	 rif=f.getValue(0);
	 nombre=f.getValue(1);
	 razon=f.getValue(2);
         
	found=nombre.find("\"");

		while(found!=std::string::npos)
		{
		 nombre.erase(found,1);
		 found=nombre.find("\"");
		}

	UE.rif=rif;
	UE.nombre=nombre;	

	}


return UE;

	
}


void FetchByRif::subUnidadesEconomicas(std::string search)
{
	DBQuery f=*q;
	std::size_t found;
	std::string capacidad, nombre, id, entidad;
	SubUnidadE su;
	f.clear();
	query=(std::string)"SELECT unidadecon_subunidad_economica.id, unidadecon_subunidad_economica.nombre, unidadecon_subunidad_economica.capacidad_utilizada, comun_entidad.nombre FROM  unidadecon_subunidad_economica, comun_parroquia,comun_municipio,comun_entidad WHERE unidadecon_subunidad_economica.parroquia_codigo = comun_parroquia.codigo AND comun_parroquia.municipio_codigo=comun_municipio.codigo AND comun_municipio.entidad_codigo = comun_entidad.codigo AND unidadecon_subunidad_economica.tipo_subunidad ='Pl' AND unidadecon_subunidad_economica.ue_rif="+"\'"+search+"\'";
	f.exec(query);

		while(f.hasResult() && f.next())
		{
		 id=f.getValue(0);
          	 nombre=f.getValue(1);
		 capacidad=f.getValue(2);
		 entidad=f.getValue(3);
        
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
		su.ue= unidades_economicas(search);
		
			if(subUEconomicas.is_empty()||!filter->contains(su.nombre))
			{
			subUEconomicas.append(su);
			filter->insert(su.nombre);
			}
			
		}

}



void FetchByRif::produccion_producto(SubUnidadE planta, std::string anho)
{

	DBQuery f=*q;
	std::size_t found;
	std::string cantidad, nombre, id,anio,cantproducido,codigo,marca;
	Productos p;

	f.clear();
	query=(std::string)"SELECT DISTINCT produccion_producto.id, produccion_producto.nombre, produccion_producto.codigo_aran_id, produccion_producto.marca, produccion_produccion_precios_r.cant_producido, produccion_produccion_precios_r.anho, cmproveedores_proveedor.pais_procedencia_id FROM produccion_producto, produccion_produccion_precios_r, cmproveedores_proveedor, unidadecon_subunidad_economica WHERE produccion_producto.id = produccion_produccion_precios_r.producto_id_id AND produccion_producto.planta_id_id  ='"+planta.id+"' AND  produccion_producto.planta_id_id=unidadecon_subunidad_economica.id AND cmproveedores_proveedor.planta_id_id = unidadecon_subunidad_economica.id AND cmproveedores_proveedor.pais_procedencia_id<>23";
	f.exec(query);
	
		while(f.hasResult() && f.next())
		{

		 anio=f.getValue(5);
		 anio=anio.substr(0,4);
		
		 id=f.getValue(0);
          	 nombre=f.getValue(1);
		 codigo=f.getValue(2);
		 marca=f.getValue(3);
		 cantidad=f.getValue(4);
        
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
			p.subue=planta;
		
				if(produccionProductos.is_empty()||!filter->contains(p.nombre))
				{
				produccionProductos.append(p);
				filter->insert(p.nombre);
				}
				
			}
	
		
		}
}

DynArray<Insumos> FetchByRif::produccion_insumo(std::string producto)
{
	DBQuery f=*q;
	std::size_t found;
	std::string cantidad, nombre, id,anio,codigo,marca;
	Insumos p;

	DynArray<Insumos> insumoslist;	

	f.clear();

	query=(std::string)"SELECT DISTINCT produccion_insumo.id, produccion_insumo.nombre, produccion_insumo.codigo_aran_id, produccion_insumo.marca, produccion_consumo_precios_r.cant_comp_imp, produccion_consumo_precios_r.anho FROM produccion_insumo, produccion_consumo_precios_r, produccion_producto_t_insumo, cmproveedores_proveedorinsumo, cmproveedores_proveedor WHERE produccion_insumo.id=produccion_consumo_precios_r.insumo_id_id AND produccion_insumo.id=produccion_producto_t_insumo.insumo_id_id AND produccion_producto_t_insumo.producto_id_id='"+producto+"' AND produccion_insumo.id=cmproveedores_proveedorinsumo.insumo_id_id AND cmproveedores_proveedor.id= cmproveedores_proveedorinsumo.proveedor_id_id AND cmproveedores_proveedor.pais_origen_id<>23";
	f.exec(query);
	
	
		while(f.hasResult() && f.next())
		{

		 anio=f.getValue(5);
                 anio=anio.substr(0,4);
		 id=f.getValue(0);
          	 nombre=f.getValue(1);
		 codigo=f.getValue(2);
		 marca=f.getValue(3);
		 cantidad=f.getValue(4);

	
 		 found=nombre.find("\"");

			while(found!=std::string::npos)
			{
			 nombre.erase(found,1);
			 found=nombre.find("\"");
			}
	
		p.id=id;
		p.nombre=nombre;
		p.cod_aran=codigo;
		p.cantidad=cantidad;
		p.marca=marca;
		p.anho=anio;
		insumoslist.append(p);
	
		}

return insumoslist;
 
}




DynArray<Proveedores> FetchByRif::proveedor(std::string search, std::string anho)
{
	DBQuery f=*q;
	std::size_t found;
	std::string id, nombre, rif, paisProcedencia, paisOrigen,anio;
	Proveedores p;
	DynArray<Proveedores> provlist;

	f.clear();
	query=(std::string)"SELECT produccion_insumo.id, cmproveedores_proveedor.nombre, cmproveedores_proveedor.rif, comun_pais.nombre, comun_pais.nombre, cmproveedores_proveedorinsumo.anho_r FROM cmproveedores_proveedor, comun_pais, cmproveedores_proveedorinsumo, unidadecon_subunidad_economica, produccion_insumo WHERE cmproveedores_proveedor.pais_origen_id=comun_pais.id AND cmproveedores_proveedor.pais_procedencia_id = comun_pais.id AND cmproveedores_proveedor.pais_origen_id <> 23 AND produccion_insumo.id= cmproveedores_proveedorinsumo.insumo_id_id AND cmproveedores_proveedorinsumo.proveedor_id_id=cmproveedores_proveedor.id AND unidadecon_subunidad_economica.id=cmproveedores_proveedor.planta_id_id AND unidadecon_subunidad_economica.id ="+"\'"+search+"\' "; //SOLO INTERNACIONALES
	//query=(std::string)"SELECT produccion_insumo.id, cmproveedores_proveedor.nombre, cmproveedores_proveedor.rif, comun_pais.nombre, comun_pais.nombre, cmproveedores_proveedorinsumo.anho_r FROM cmproveedores_proveedor, comun_pais, cmproveedores_proveedorinsumo, unidadecon_subunidad_economica, produccion_insumo WHERE cmproveedores_proveedor.pais_origen_id=comun_pais.id AND cmproveedores_proveedor.pais_procedencia_id = comun_pais.id AND produccion_insumo.id= cmproveedores_proveedorinsumo.insumo_id_id AND cmproveedores_proveedorinsumo.proveedor_id_id=cmproveedores_proveedor.id AND unidadecon_subunidad_economica.id=cmproveedores_proveedor.planta_id_id AND unidadecon_subunidad_economica.id ="+"\'"+search+"\' "; //CON NACIONALES
	f.exec(query);

	while(f.hasResult() && f.next())
	{
		 anio=f.getValue(5);
		 anio=anio.substr(0,4);
		 id=f.getValue(0);
          	 nombre=f.getValue(1);
		 rif=f.getValue(2);
		 paisOrigen=f.getValue(3);
		 paisProcedencia=f.getValue(4);
        
 		 found=nombre.find("\"");

			while(found!=std::string::npos)
			{
			 nombre.erase(found,1);
			 found=nombre.find("\"");
			}
	
			if(anio==anho)
			{	
			p.insumoID=id;
			p.nombre=nombre;
			p.rif=rif;
			p.paisOrigen=paisOrigen;
			p.paisProcedencia=paisProcedencia;
			provlist.append(p);
			}			
	
	}
	
return provlist;

}

#endif



