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

	DynArray <SubUnidadE> subUEconomicas;
	DynArray <Productos> produccionProductos;
	DynArray <Productos> produccionInsumos;
	

	Arco aux;
	
	public:

	Fetch();

	~Fetch(){}

	Fetch(std::string);

	DynArray<Productos> search(std::string,std::string);
	UnidadEconomica unidades_economicas(std::string);
	void subUnidadesEconomicas(std::string);
	void produccion_producto(SubUnidadE, std::string);
	void produccion_producto(SubUnidadE, std::string,std::string);
	DynArray<Insumos> produccion_insumo(std::string);
	DynArray<Proveedores> proveedor(std::string,std::string);
	DynArray<Proveedores> proveedor(std::string,std::string, std::string);
	SubUnidadE retrieve_planta(std::string);
	
	};


Fetch::Fetch() {

	query=" ";
	
}

Fetch::Fetch(const std::string name)
{
	connection.open(name.c_str());

}


SubUnidadE Fetch::retrieve_planta(std::string id)
{

	q->clear();
	query=(std::string)"SELECT unidadecon_subunidad_economica.id, unidadecon_subunidad_economica.nombre, unidadecon_subunidad_economica.capacidad_utilizada, comun_entidad.nombre FROM  unidadecon_subunidad_economica, comun_parroquia,comun_municipio,comun_entidad, produccion_insumo WHERE unidadecon_subunidad_economica.parroquia_codigo = comun_parroquia.codigo AND comun_parroquia.municipio_codigo=comun_municipio.codigo AND comun_municipio.entidad_codigo = comun_entidad.codigo AND produccion_insumo.planta_id_id=unidadecon_subunidad_economica.id AND produccion_insumo.id="+"\'"+id+"\'";
	q->exec(query);

	q->next();


	SubUnidadE su;

		su.id=q->getValue(0);
		su.nombre=q->getValue(1);
		su.capacidadu=q->getValue(2);
		su.estado=q->getValue(3);

	return su;
	
}

DynArray<Productos> Fetch::search(std::string rif, std::string anho)
{
	std::locale loc;
   	rif[0]=std::toupper(rif[0],loc);
	std::string val;

	if(rif[0]=='j'||rif[0]=='J')
	{
		subUnidadesEconomicas(rif);

		for(DynArray<SubUnidadE>::Iterator it(subUEconomicas);it.has_curr();it.next())
		produccion_producto(it.get_curr(),anho);		
		
		

		for(DynArray<Productos>::Iterator it(produccionProductos);it.has_curr();it.next())
		{
		it.get_curr().insumos=produccion_insumo(it.get_curr().id);
       		it.get_curr().subue.proveedores=proveedor(it.get_curr().subue.id,anho);
		}
	
		
	}
		
	else
	{
	
	q->clear();
	query=(std::string)"SELECT DISTINCT unidadecon_subunidad_economica.ue_rif FROM unidadecon_subunidad_economica, produccion_producto WHERE unidadecon_subunidad_economica.tipo_subunidad='Pl' AND produccion_producto.codigo_aran_id=\'"+rif+"\'AND produccion_producto.planta_id_id=unidadecon_subunidad_economica.id";
	q->exec(query);
	q->reset();
	DynArray<std::string> val;

		if(q->hasResult())
			while(q->hasResult() && q->next())
				val.append(q->getValue(0));
		
		for(DynArray<std::string>::Iterator it(val);it.has_curr();it.next())
		subUnidadesEconomicas(it.get_curr());
			
		for(DynArray<SubUnidadE>::Iterator it(subUEconomicas);it.has_curr();it.next())
		produccion_producto(it.get_curr(),anho,rif);

		for(DynArray<Productos>::Iterator it(produccionProductos);it.has_curr();it.next())
			{
			it.get_curr().insumos=produccion_insumo(it.get_curr().id);
            		it.get_curr().subue.proveedores=proveedor(it.get_curr().subue.id,anho,rif);
			}
		
		
	}

		
		

return produccionProductos;
	


}

UnidadEconomica Fetch::unidades_economicas(std::string rif)
{

   	std::size_t found;
	std::string nombre,query;
	UnidadEconomica UE;
	
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
		UE.rif=rif;
		UE.nombre=nombre;	

	}
	q->clear();

	return UE;

	
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
		su.ue= unidades_economicas(search);
		subUEconomicas.append(su);
			
		}

	}
	

	q->clear();

		
}



void Fetch::produccion_producto(SubUnidadE planta, std::string anho)
{

	std::size_t found;
	std::string cantidad, nombre, id,anio,cantproducido,codigo,marca;
	Productos p;

	q->clear();
	query=(std::string)"SELECT DISTINCT produccion_producto.id, produccion_producto.nombre, produccion_producto.codigo_aran_id, produccion_producto.marca, produccion_produccion_precios_r.cant_producido, produccion_produccion_precios_r.anho, cmproveedores_proveedor.pais_procedencia_id FROM produccion_producto, produccion_produccion_precios_r, cmproveedores_proveedor, unidadecon_subunidad_economica WHERE produccion_producto.id = produccion_produccion_precios_r.producto_id_id AND produccion_producto.planta_id_id  ='"+planta.id+"' AND  produccion_producto.planta_id_id=unidadecon_subunidad_economica.id AND cmproveedores_proveedor.planta_id_id = unidadecon_subunidad_economica.id AND cmproveedores_proveedor.pais_procedencia_id<>23";
	q->exec(query);
	
	if(q->hasResult())
	{
	
		while(q->hasResult() && q->next())
		{

		 anio=q->getValue(5);
		 anio=anio.substr(0,4);
		
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
		p.subue=planta;
		

		if(produccionProductos.is_empty())
			produccionProductos.append(p);

			if(produccionProductos.get_last().nombre != p.nombre)
			{

			produccionProductos.append(p);
			}
		}
	
		
		}

	}
	

	q->clear();


}

DynArray<Insumos> Fetch::produccion_insumo(std::string producto)
{

	std::size_t found;
	std::string cantidad, nombre, id,anio,codigo,marca;
	Insumos p;

	DynArray<Insumos> insumoslist;	

	q->clear();

	query=(std::string)"SELECT DISTINCT produccion_insumo.id, produccion_insumo.nombre, produccion_insumo.codigo_aran_id, produccion_insumo.marca, produccion_consumo_precios_r.cant_comp_imp, produccion_consumo_precios_r.anho FROM produccion_insumo, produccion_consumo_precios_r, produccion_producto_t_insumo, cmproveedores_proveedorinsumo, cmproveedores_proveedor WHERE produccion_insumo.id=produccion_consumo_precios_r.insumo_id_id AND produccion_insumo.id=produccion_producto_t_insumo.insumo_id_id AND produccion_producto_t_insumo.producto_id_id='"+producto+"' AND produccion_insumo.id=cmproveedores_proveedorinsumo.insumo_id_id AND cmproveedores_proveedor.id= cmproveedores_proveedorinsumo.proveedor_id_id AND cmproveedores_proveedor.pais_origen_id<>23";
	q->exec(query);
	
	
		while(q->hasResult() && q->next())
		{

			
		 anio=q->getValue(5);
                 anio=anio.substr(0,4);
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




DynArray<Proveedores> Fetch::proveedor(std::string search, std::string anho)
{

	std::size_t found;
	std::string id, nombre, rif, paisProcedencia, paisOrigen,anio;
	Proveedores p;
	DynArray<Proveedores> provlist;

	q->clear();
	query=(std::string)"SELECT produccion_insumo.id, cmproveedores_proveedor.nombre, cmproveedores_proveedor.rif, comun_pais.nombre, comun_pais.nombre, cmproveedores_proveedorinsumo.anho_r FROM cmproveedores_proveedor, comun_pais, cmproveedores_proveedorinsumo, unidadecon_subunidad_economica, produccion_insumo WHERE cmproveedores_proveedor.pais_origen_id=comun_pais.id AND cmproveedores_proveedor.pais_procedencia_id = comun_pais.id AND cmproveedores_proveedor.pais_origen_id <> 23 AND produccion_insumo.id= cmproveedores_proveedorinsumo.insumo_id_id AND cmproveedores_proveedorinsumo.proveedor_id_id=cmproveedores_proveedor.id AND unidadecon_subunidad_economica.id=cmproveedores_proveedor.planta_id_id AND unidadecon_subunidad_economica.id ="+"\'"+search+"\' "; //SOLO INTERNACIONALES
	//query=(std::string)"SELECT produccion_insumo.id, cmproveedores_proveedor.nombre, cmproveedores_proveedor.rif, comun_pais.nombre, comun_pais.nombre, cmproveedores_proveedorinsumo.anho_r FROM cmproveedores_proveedor, comun_pais, cmproveedores_proveedorinsumo, unidadecon_subunidad_economica, produccion_insumo WHERE cmproveedores_proveedor.pais_origen_id=comun_pais.id AND cmproveedores_proveedor.pais_procedencia_id = comun_pais.id AND produccion_insumo.id= cmproveedores_proveedorinsumo.insumo_id_id AND cmproveedores_proveedorinsumo.proveedor_id_id=cmproveedores_proveedor.id AND unidadecon_subunidad_economica.id=cmproveedores_proveedor.planta_id_id AND unidadecon_subunidad_economica.id ="+"\'"+search+"\' "; //CON NACIONALES
	q->exec(query);

	while(q->hasResult() && q->next())
	{
			
		 anio=q->getValue(5);
		 anio=anio.substr(0,4);
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
	
		if(anio==anho)
		{	
		p.insumoID=id;
		p.nombre=nombre;
		p.rif=rif;
		p.paisOrigen=paisOrigen;
		p.paisProcedencia=paisProcedencia;

		if(provlist.is_empty())
			provlist.append(p);

		if(provlist.get_last().nombre != p.nombre)
			provlist.append(p);

		}
	
	}
	



	return provlist;

	q->clear();


}



void Fetch::produccion_producto(SubUnidadE planta, std::string anho, std::string producto )
{

	std::size_t found;
	std::string cantidad, nombre, id,anio,cantproducido,codigo,marca;
	Productos p;

	q->clear();
	query=(std::string)"SELECT DISTINCT produccion_producto.id, produccion_producto.nombre, produccion_producto.codigo_aran_id, produccion_producto.marca, produccion_produccion_precios_r.cant_producido, produccion_produccion_precios_r.anho, cmproveedores_proveedor.pais_procedencia_id FROM produccion_producto, produccion_produccion_precios_r, cmproveedores_proveedor, unidadecon_subunidad_economica WHERE produccion_producto.id = produccion_produccion_precios_r.producto_id_id AND produccion_producto.planta_id_id  ='"+planta.id+"' AND  produccion_producto.planta_id_id=unidadecon_subunidad_economica.id AND cmproveedores_proveedor.planta_id_id = unidadecon_subunidad_economica.id AND cmproveedores_proveedor.pais_procedencia_id<>23 AND produccion_producto.codigo_aran_id='"+producto+"'";

	q->exec(query);

	if(q->hasResult())
	{
		while(q->hasResult() && q->next())
		{

                 anio=q->getValue(5);
		 anio=anio.substr(0,4);

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
			p.subue=planta;

			if(produccionProductos.is_empty())
			produccionProductos.append(p);

			if(produccionProductos.get_last().nombre != p.nombre)
			{

			produccionProductos.append(p);
			}

			
			}
	
		
		}

	}
	



}

DynArray<Proveedores> Fetch::proveedor(std::string search, std::string anho, std::string producto)
{

	std::size_t found;
	std::string id, nombre, rif, paisProcedencia, paisOrigen,anio;
	Proveedores p;
	DynArray<Proveedores> provlist;

	q->clear();
	query=(std::string)"SELECT produccion_insumo.id, cmproveedores_proveedor.nombre, cmproveedores_proveedor.rif, comun_pais.nombre, comun_pais.nombre, cmproveedores_proveedorinsumo.anho_r FROM cmproveedores_proveedor, comun_pais, cmproveedores_proveedorinsumo, unidadecon_subunidad_economica, produccion_insumo,produccion_producto WHERE cmproveedores_proveedor.pais_origen_id=comun_pais.id AND cmproveedores_proveedor.pais_procedencia_id = comun_pais.id AND cmproveedores_proveedor.pais_origen_id <> 23 AND produccion_insumo.id= cmproveedores_proveedorinsumo.insumo_id_id AND cmproveedores_proveedorinsumo.proveedor_id_id=cmproveedores_proveedor.id AND unidadecon_subunidad_economica.id=cmproveedores_proveedor.planta_id_id AND unidadecon_subunidad_economica.id ="+"\'"+search+"\' AND produccion_producto.planta_id_id=unidadecon_subunidad_economica.id AND produccion_producto.codigo_aran_id='"+producto+"'"; // SOLO INTERNACIONALES
//	query=(std::string)"SELECT DISTINCT produccion_insumo.id, cmproveedores_proveedor.nombre, cmproveedores_proveedor.rif, comun_pais.nombre, comun_pais.nombre, cmproveedores_proveedorinsumo.anho_r FROM cmproveedores_proveedor, comun_pais, cmproveedores_proveedorinsumo, unidadecon_subunidad_economica, produccion_insumo,produccion_producto WHERE cmproveedores_proveedor.pais_origen_id=comun_pais.id AND cmproveedores_proveedor.pais_procedencia_id = comun_pais.id AND produccion_insumo.id= cmproveedores_proveedorinsumo.insumo_id_id AND cmproveedores_proveedorinsumo.proveedor_id_id=cmproveedores_proveedor.id AND unidadecon_subunidad_economica.id=cmproveedores_proveedor.planta_id_id AND unidadecon_subunidad_economica.id ="+"\'"+search+"\' AND produccion_producto.planta_id_id=unidadecon_subunidad_economica.id AND produccion_producto.codigo_aran_id='"+producto+"'"; //CON NACIONALES
	q->exec(query);
	
	while(q->hasResult() && q->next())
	{
			
		 anio=q->getValue(5);
		 anio=anio.substr(0,4);
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
	
		if(anio==anho)
		{	
		p.insumoID=id;
		p.nombre=nombre;
		p.rif=rif;
		p.paisOrigen=paisOrigen;
		p.paisProcedencia=paisProcedencia;

		if(provlist.is_empty())
			provlist.append(p);

		if(provlist.get_last().nombre != p.nombre)
			provlist.append(p);
			
		}
	
	}
	
	return provlist;

	q->clear();


}





#endif



